# CAE download and ingest runs, 2026-11-05 to 2026-11-06

Polaris jobs that ran `polaris/cae_ingest.pbs`, and a download run on a login node. Full logs
are in the private run directories `/eagle/radix-io/hyoklee/tmap_data/runs/run_<jobid>/`
(not committed). Only counts, sizes, timings and log messages are recorded here; no data.

## PhysioNet basic auth is wget-only (2026-11-05)

Job 7817854 got HTTP 403 for every file (`2026-11-05-cae-ingest-job-7817854.md`). After the
account was approved for TARGET-MAP, a plain `wget --user ... --ask-password` download
worked from the login node, while curl with the same credentials still got 403.

PhysioNet's server code (MIT-LCP/physionet-build, `project/utility.py`) answers basic auth
only when the User-Agent starts with `Wget/`, `libwfdb/` or `PhysioNet-Python-Client/`.
Other clients get 403 with no 401 challenge. OAuth bearer tokens (scope `data:download`)
are accepted from any client, but need a registered OAuth app.

Fix: iowarp/clio-core `d2f4edf42` (PR #1331) adds `client = wget` to
`~/.clio/cae_http_auth.ini`; CAE then runs wget with credentials in a private config
file. After it, `cae_http_tool probe` returned 200 for the clinical CSV (3,375 bytes),
`HDF5_Trimmed/TAR004_D4_ALL_TRIMMED.h5` (50,866,112) and `CSV_files/TAR001_D2_LMCA_RMCA.csv`
(52,797,651).

## Server inventory (SHA256SUMS.txt)

156 entries; paths in `2026-11-05-physionet-target-map-paths.txt`.

| Folder | Files | Bytes | Smallest | Largest |
|--------|-------|-------|----------|---------|
| `HDF5_Trimmed/*.h5` | 69 | 8,487,899,236 | 50,866,112 | 243,623,928 |
| `CSV_files/*.csv` | 84 | 2,022,403,791 | 2,473,284 | 56,568,665 |
| top-level `*.csv` (clinical data, data dictionary) | 2 | 6,179 | 2,804 | 3,375 |
| `LICENSE.txt` | 1 | | | |

Total data: 155 files, 10,510,309,206 bytes. HDF5 files come from 21 patients, CSV files
from 20. The HDF5 files hold 1,088 datasets, all under `/Waveforms`: 544 signals (TCD 342,
ABP 69, ECG 68, RESP 65), each with a `,timestamps` twin.

## Job 7818647 (debug queue, 1 h): download + ingest OMNI files

- clio-core `d2f4edf42`. Probes: 200 for all three files.
- Smoke (`target_map_smoke.omni.yaml`): rc 0 in 109.5 s, 50,869,487 bytes. Both files
  pass SHA-256. The clinical CSV became 1 tag (2 blobs); TAR004_D4 has 6 datasets and
  became 6 tags.
- HDF5 OMNI file: 13 files downloaded (one at a time, 0.3–0.5 MB/s each; for example
  183,094,488 bytes in 426 s) before PBS killed the job at 3,616 s
  (`walltime 3616 exceeded limit 3600`). The only download error, `wget exit 143`, is
  the SIGTERM from that kill.
- Login-node test the same day: one wget stream to /dev/null gave about 0.34 MB/s; four
  parallel streams about 0.65 MB/s in total.

## Job 7819031 (capacity queue, 6 h): runtime hang

- Queued 12 h 17 min (`Insufficient amount of resource: queue_tags`), ran 10:37–16:39 UTC.
- Smoke: rc 0 in 2.0 s (files already present; CAE skipped the downloads).
- HDF5 OMNI file: 18 more downloads (0.4–0.5 MB/s each), then the runtime hung at
  transfer 62 of 138, the ingest of the 31st HDF5 file (`TAR017_D5_ALL_TRIMMED.h5`).
  Killed at walltime (6:01:31, exit -29).
- From the first real download on, the scheduler logged
  `[#781] worker N STALLED on one task (pool 400 method 10 ...)` followed by
  `[#785] RESCUE` and lane adoption: 38,213 times (worker 6: 2,674; worker 19: 17,678;
  worker 20: 17,861). 5 elastic workers were spawned (16 → 21 workers).
- After transfer 62 started, the runtime logged
  `GlobalDeserialize::read_binary: Attempted to read beyond end of data` 19,112,452
  times (runtime log: 19.2 M lines, 4.5 GB; deleted 2026-11-07, excerpt kept privately).
  The client logged `[HANGWATCH-CLIENT] waited ... ms` up to 1.536e+07 ms.

## Jobs 7823444 and 7823501 (debug queue): isolation, ingest only

| Job | OMNI file | clio_cae | CTE tags | Runtime errors |
|-----|-----------|----------|----------|----------------|
| 7823444 | `TAR017_D5_ALL_TRIMMED.h5` alone | rc 0, 1.04 s | 14 (14 datasets) | 0 |
| 7823501 | the 31 HDF5 files on disk, same order as 7819031 | rc 0, 22.4 s | 408 (408 datasets) | 0 |

Neither logged a STALLED warning.

## Login-node download (2026-11-06)

`cae_http_tool get` (wget client), four files at a time, for the 123 files not yet on disk:
121 OK in 10,703 s (7,110,517,905 bytes, about 0.66 MB/s in total). Two CSVs failed and
downloaded on a manual retry (0.5 MB/s each). Afterwards all 155 data files matched
SHA256SUMS.txt.

## Job 7824018 (debug queue): full ingest, ingest-only OMNI files

| OMNI file | Files | Bytes | clio_cae | Rate | CTE result |
|-----------|-------|-------|----------|------|------------|
| `target_map_hdf5_ingest.omni.yaml` | 69 | 8,487,899,236 | rc 0, 54.8 s | 154.7 MB/s | 1,088 tags = 1,088 datasets |
| `target_map_csv_ingest.omni.yaml` | 86 | 2,022,409,970 | rc 0, 14.0 s | 144.2 MB/s | 86 tags, 2,057 blobs |

All 155 SHA-256 checks pass; 1,174 `target_map` tags in all; 0 runtime errors; job
walltime 5 min 6 s. Tag lookups with `cte_search` matched every HDF5 dataset name, all of
which contain commas.
