/* vol_read: read every dataset of HDF5 files, whole and as a middle hyperslab.
 *
 *   vol_read <file.h5>...
 *
 * Opens each file read-only with the default VOL, so HDF5_VOL_CONNECTOR=clio
 * routes it through the CLIO VOL connector and an unset one reads natively.
 * Per dataset prints one line:
 *   DS <file> <dataset> bytes=<n> whole_us=<t> slab_us=<t> hash=<fnv1a-64 of whole>
 * and per file a TOTAL line. The hashes are for comparing a native run with a VOL
 * run of the same files; they stay in the private run directory.
 *
 * Datasets are read in their native memory type. The hyperslab is the middle
 * 10 % along the last dimension (all of the others), which a VOL serves from the
 * tier only when it holds the dataset.
 */
#include <hdf5.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_us(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

static uint64_t fnv1a(const unsigned char *p, size_t n) {
  uint64_t h = 1469598103934665603ULL;
  for (size_t i = 0; i < n; i++) { h ^= p[i]; h *= 1099511628211ULL; }
  return h;
}

typedef struct { const char *file; size_t nds, bytes; double whole_us, slab_us; int errors; } ctx_t;

static void read_one(hid_t fid, const char *name, ctx_t *c) {
  hid_t d = H5Dopen2(fid, name, H5P_DEFAULT);
  if (d < 0) { c->errors++; return; }
  hid_t ft = H5Dget_type(d), mt = H5Tget_native_type(ft, H5T_DIR_DEFAULT);
  hid_t fs = H5Dget_space(d);
  int nd = H5Sget_simple_extent_ndims(fs);
  hsize_t dims[H5S_MAX_RANK];
  H5Sget_simple_extent_dims(fs, dims, NULL);
  size_t esz = H5Tget_size(mt), nelem = (size_t)H5Sget_simple_extent_npoints(fs);
  size_t bytes = nelem * esz;
  unsigned char *buf = malloc(bytes ? bytes : 1);
  double t0 = now_us();
  herr_t rc = H5Dread(d, mt, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);
  double whole = now_us() - t0;
  uint64_t h = rc >= 0 ? fnv1a(buf, bytes) : 0;
  if (rc < 0) c->errors++;

  double slab = 0;
  if (nd >= 1 && dims[nd - 1] >= 10) {
    hsize_t start[H5S_MAX_RANK] = {0}, count[H5S_MAX_RANK];
    size_t n = 1;
    for (int i = 0; i < nd; i++) { count[i] = dims[i]; }
    count[nd - 1] = dims[nd - 1] / 10;
    start[nd - 1] = dims[nd - 1] / 2 - count[nd - 1] / 2;
    for (int i = 0; i < nd; i++) n *= count[i];
    hid_t ss = H5Scopy(fs);
    H5Sselect_hyperslab(ss, H5S_SELECT_SET, start, NULL, count, NULL);
    hid_t ms = H5Screate_simple(nd, count, NULL);
    t0 = now_us();
    if (H5Dread(d, mt, ms, ss, H5P_DEFAULT, buf) < 0) c->errors++;
    slab = now_us() - t0;
    (void)n;
    H5Sclose(ms); H5Sclose(ss);
  }
  printf("DS %s %s bytes=%zu whole_us=%.0f slab_us=%.0f hash=%016" PRIx64 "\n",
         c->file, name, bytes, whole, slab, h);
  c->nds++; c->bytes += bytes; c->whole_us += whole; c->slab_us += slab;
  free(buf);
  H5Sclose(fs); H5Tclose(mt); H5Tclose(ft); H5Dclose(d);
}

static herr_t visit(hid_t obj, const char *name, const H5O_info2_t *info, void *op) {
  if (info->type != H5O_TYPE_DATASET) return 0;
  char path[4096];
  snprintf(path, sizeof path, "/%s", name);
  read_one(obj, path, (ctx_t *)op);
  return 0;
}

int main(int argc, char **argv) {
  int bad = 0;
  for (int i = 1; i < argc; i++) {
    ctx_t c = {argv[i], 0, 0, 0, 0, 0};
    const char *slash = strrchr(argv[i], '/');
    c.file = slash ? slash + 1 : argv[i];
    double t0 = now_us();
    hid_t fid = H5Fopen(argv[i], H5F_ACC_RDONLY, H5P_DEFAULT);
    if (fid < 0) { printf("TOTAL %s open-failed\n", c.file); bad = 1; continue; }
    H5Ovisit3(fid, H5_INDEX_NAME, H5_ITER_NATIVE, visit, &c, H5O_INFO_BASIC);
    H5Fclose(fid);
    double all = now_us() - t0;
    printf("TOTAL %s datasets=%zu bytes=%zu whole_us=%.0f slab_us=%.0f open_to_close_us=%.0f errors=%d\n",
           c.file, c.nds, c.bytes, c.whole_us, c.slab_us, all, c.errors);
    bad |= c.errors != 0;
  }
  return bad;
}
