# PhysioNet page: TARGET-MAP v1.0.0

Clipped 2026-11-05 from https://physionet.org/content/target-map/1.0.0/ (public landing
page only; no data files downloaded).

- Title: TARGET-MAP: Transcranial doppler AutoReGulation assessment of brain perfusion in
  nEurocritical patients for MAP optimization. Version 1.0.0, published 2026-10-22.
- Authors: Marina Trombin Marques, Kevin Bao, Adeline Hayman, Katherine Peterson, Eliot Lee,
  Jeffrey Vitt, Xiao Hu, Wade Smith, Claude Hemphill, Gisele Sampaio Silva, Edilberto Amorim.
- Cohort: 24 neurocritical patients, Zuckerberg San Francisco General Hospital (USA).
- Contents: baseline clinical information, continuous transcranial Doppler (TCD) and
  multiparameter monitor data (blood pressure, ECG, end-tidal CO2, SpO2). Sessions of about
  30–60 minutes, recorded up to five days after ICU admission. Neurological outcome by
  Cerebral Performance Category (CPC) at discharge. Some recordings include EEG or ECG.
- Signals: TCD mean velocities (left and right MCA; PCA in some files), arterial blood
  pressure, ECG at 500 Hz; ABP, TCD, SpO2 and ETCO2 at 125 Hz.
- Files:
  - `.csv`: synchronized ABP and TCD data; time starts at 0 in each recording.
  - `.h5` (HDF5): raw, unsynchronized, de-identified recordings; timestamps in microseconds.
  - One de-identified clinical CSV (about 5 KB).
  - Individual files range from about 2.5 MB to about 198 MB.
- Access: restricted. Requires a registered PhysioNet account and the signed PhysioNet
  Restricted Health Data Use Agreement 1.5.0. License: PhysioNet Restricted Health Data
  License 1.5.0.
- DOI: 10.13026/b53c-b825 (v1.0.0); latest version: 10.13026/50ht-2g86.
- Requested citations: the dataset (Trombin Marques et al., 2026, PhysioNet) and the
  platform paper, Pollard, T., et al. (2026), "PhysioNet as a global platform for
  biomedical research", *Nature Health*, https://doi.org/10.1038/s44360-026-00096-z.
