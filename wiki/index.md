# Index

- [TARGET-MAP dataset](target-map.md) — 24 neurocritical patients, TCD + ABP/ECG/SpO2/ETCO2 recordings in HDF5 and CSV; restricted access, file formats, download and citation.
- [Ingesting TARGET-MAP with CLIO CAE](clio-cae-ingest.md) — the real 155-file inventory (10.5 GB) and HDF5 layout, PhysioNet's wget-only basic auth (`client = wget`), the download-then-ingest recipe (all 155 files into the CTE in 69 s, 1,174 tags), job results, the long-run runtime hang, and the HDF5 VOL read test (VOL reads only `format: hdf5_vol` staging).
- [NeuroFAIR: related CLIO work](neurofair.md) — the sibling MiV/neuroh5 wiki: the shared CAE/OMNI method, and which of its CTE findings (VOL is the only tier-read path, now tested on TARGET-MAP; #470 fixed; page cache vs RAM tier) shape tmap's next steps, and why MiV/neuroh5 cannot model TARGET-MAP physiology.
