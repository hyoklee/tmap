# NeuroFAIR: related CLIO work

[NeuroFAIR](https://github.com/hyoklee/NeuroFAIR) is the sibling LLM wiki about running
IOWarp/CLIO on neuroscience HDF5 data: the MiV-Simulator CA1 microcircuit and its
neuroh5 files. This page records what tmap shares with it and what tmap can reuse.
NeuroFAIR claims below cite its wiki pages (`NF:<page>`, under
`https://github.com/hyoklee/NeuroFAIR/blob/main/wiki/`) at commit `31d1c89`
(2026-10-21).

The science differs. NeuroFAIR studies simulated neural circuits; tmap studies
clinical TCD and bedside-monitor recordings from 24 ICU patients ([[target-map]]).
The two share no data.

## Shared method

- **Wiki pattern:** both follow the LLM-wiki pattern (NeuroFAIR's `llm-wiki.md`). tmap
  keeps sources in `raw/`; NeuroFAIR keeps them outside the repo (`../neuroh5/data/`) and
  also writes semantic metadata into the HDF5 files through MCP tools (`NF:CLAUDE.md`).
  tmap cannot do that: the TARGET-MAP DUA and this public repo allow schemas and
  aggregates only (`CLAUDE.md`).
- **Ingest through CAE OMNI:** NeuroFAIR staged MiV files with CAE OMNI over Globus
  (`~/aurora/omni/miv_*.omni.yaml`; `NF:log.md`, 2026-04-17). It planned
  `Hdf5FileAssimilator` pre-staging into the CTE RAM tier (`NF:perf-clio-core.md`). tmap
  uses the same OMNI format and HDF5 assimilator, with an https source instead
  (PR #1331; [[clio-cae-ingest]]).
- **Platforms:** NeuroFAIR has run on Aurora, Polaris, Ares, Vista, Frontera and jelly;
  tmap runs on Polaris. Both use PBS job scripts and g++-14 clio-core builds.

## NeuroFAIR findings that shape tmap's next steps

| NeuroFAIR finding | What it means for tmap |
|---|---|
| Only the HDF5 **VOL** connector serves reads from the CTE. It reads CTE-core blobs and bypasses clio-fs. The **VFD** only writes to the tier (no `PreadFd`), so its reads track native. The POSIX adapter needs `clio::` paths; the VFD refuses them (`NF:concept-clio-fs-vs-hdf5-vfd.md`). | Tested ([[clio-cae-ingest]], job 7826129): the VOL does not read CAE's `format: hdf5` tags (one per dataset), but it serves 100 % of reads from the CTE, byte-identical, when CAE stages with `format: hdf5_vol`. |
| The 2026-05 result that tier reads were ~4× slower than NFS with `GetBlob` errors (`NF:miv_iowarp_ares_case7_feasibility.md`) came before clio-core #470 was fixed (closed 2026-07-15). | Don't plan around it. Current-dev overheads on MiV case 7 are VFD +0.5 %, VOL +4.3 %, VFD read tier +4.2 % and clio-fs via the MPI-IO adapter +4.5 % (`NF:miv_iowarp_ares_case7_1020.md`). |
| On jelly's slow disk the CTE RAM tier is ~7–10× faster for writes and uncached reads (1.9–2.4 GB/s). A warm page cache (2.9 GB/s) wins for small data read once (`NF:miv_jelly_iowarp.md`). | TARGET-MAP is 10.5 GB with files ≤244 MB ([[clio-cae-ingest]]), so it fits in a Polaris node's page cache. A CTE gain needs cold reads or repeated reads across jobs or nodes, not one warm pass. Confirmed on Polaris: the VOL read 395 MB from the DRAM tier at 0.99 GB/s, against 0.25 GB/s for a first Eagle read and 3.0 GB/s warm ([[clio-cae-ingest]]). |
| MiV is compute-bound, so CTE never sped it up (`NF:miv_iowarp_ares_case7_feasibility.md`). | An autoregulation analysis over the TCD/ABP series ([[target-map]]) is closer to I/O-bound: many recordings, light arithmetic. It may be the regime NeuroFAIR could not reach. |
| Running clio-fs under parallel HDF5 needed OMPIO and a raised `fbtl_posix_max_gap_size`. ROMIO rejects the `clio::` prefix (`NF:miv_iowarp_ares_case7_clio_fs.md`). | Only relevant if tmap reads through MPI-IO. Serial h5py reads avoid it. |

## HDF5 layout differences

- MiV/neuroh5 files: deep groups (`Projections/<SRC>/<DST>/Edges`) and committed
  datatypes (`NF:overview.md`).
- TARGET-MAP files: flat root datasets whose names contain commas
  (`/TCD,LMCA-L1,Mean,Float,Multigon`, each with a `,timestamps` twin), plus `/Trends`
  and `/Waveforms` ([[clio-cae-ingest]]).
- NeuroFAIR has not met commas in dataset names, so tmap's open question about how they
  map to CTE tag names is new.

## Can MiV-Simulator or neuroh5 do TARGET-MAP science?

No. Query answered 2026-11-06 from general domain knowledge, not from a source in `raw/`.

- **neuroh5 is not a simulator.** It is an HDF5 format and library for network models:
  cell attributes and connections (`Projections/<SRC>/<DST>/Edges`). The simulator is
  MiV-Simulator, which runs a spiking CA1 microcircuit and uses neuroh5 for its input
  and output (`NF:overview.md`).
- **The variables differ.** TARGET-MAP measures cerebral blood-flow velocity (TCD) and
  arterial blood pressure to study cerebral autoregulation ([[target-map]]). MiV models
  no vessels, blood flow, blood pressure or neurovascular coupling, so it cannot produce
  or explain these signals.
- **The scales differ.** MiV simulates milliseconds of activity in a few mm² of
  hippocampus. TARGET-MAP covers hours of whole-brain blood flow. Linking them would need
  a neurovascular-coupling model, which MiV does not have.

What fits TARGET-MAP instead:

- **Autoregulation measures** computed from the recordings: the moving-window
  correlation between blood pressure and flow velocity (Mx), and transfer-function
  analysis following the CARNet white paper.
- **Small circulation models** for simulation: lumped-parameter models (the Ursino
  family) or Windkessel models fitted to each patient's pressure and flow traces.

What tmap can still take from NeuroFAIR:

- **I/O infrastructure:** CAE ingest, the VOL read path from the CTE, and the Polaris and
  Aurora run lessons (sections above).
- **Benchmark contrast:** MiV is compute-bound, while an autoregulation analysis over
  many recordings is closer to I/O-bound (table above).
- **Parallel read patterns:** neuroh5's way of splitting reads across MPI ranks could be
  borrowed for a parallel analysis. Its cell and connection data model does not suit
  time series, so tmap should not store data in neuroh5.

## Open

- ~~Test whether the CLIO VOL connector reads the tags CAE ingested.~~ Done: only with
  `format: hdf5_vol` ([[clio-cae-ingest]]).
- Pick an I/O-bound TARGET-MAP read workload (cold or repeated reads) to compare native,
  VOL and clio-fs.
