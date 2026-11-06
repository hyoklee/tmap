# Log

- 2026-11-05 — repo created.
- 2026-11-05 — ingest: `raw/2026-11-05-physionet-target-map-page.md` (PhysioNet landing page for TARGET-MAP v1.0.0). New page: `target-map`. Open: download data and inventory the HDF5 files; choose CLIO workloads.
- 2026-11-05 — ingest: `raw/2026-11-05-physionet-target-map-files.tsv` (Table 1 of the landing page: 124 file names and sizes). New page: `clio-cae-ingest`; new `omni/` (generator, 3 OMNI files, credentials template); updated `target-map`. Open: confirm server folder names; run the smoke OMNI file.
- 2026-11-05 — ingest: `raw/2026-11-05-cae-ingest-job-7817854.md` (first CAE run on Polaris). Updated `clio-cae-ingest`: Run (job script `polaris/cae_ingest.pbs`, credential handling) and Results (build and OMNI load OK; PhysioNet answered 403 to every request, so nothing was downloaded). Open: run the wget test to tell an account-access problem from a login-flow problem.
- 2026-11-05 — query: how tmap connects to ~/NeuroFAIR (at its commit `31d1c89`). New page: `neurofair`; linked from `clio-cae-ingest`. Open: can the VOL read CAE-ingested tags; pick an I/O-bound TARGET-MAP read workload.
- 2026-11-06 — query: can MiV-Simulator or neuroh5 be used for TARGET-MAP research? No: neuroh5 is a network-model format, and MiV models spiking neurons with no blood-flow or pressure physiology. Updated `neurofair` with a section on what fits instead (Mx, transfer-function analysis, lumped circulation models) and what still carries over (I/O tooling, benchmark contrast, parallel read patterns).
