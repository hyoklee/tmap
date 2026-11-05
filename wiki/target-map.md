# TARGET-MAP dataset

Source: `raw/2026-11-05-physionet-target-map-page.md`.

TARGET-MAP (v1.0.0, PhysioNet, published 2026-10-22) holds transcranial Doppler (TCD)
and bedside monitor recordings from 24 neurocritical patients at Zuckerberg San Francisco
General Hospital. It was collected to study cerebral autoregulation for mean arterial
pressure (MAP) optimization. Sessions last about 30–60 minutes and were recorded for up
to five days after ICU admission. Outcome is the Cerebral Performance Category (CPC) at
discharge.

## Signals

| Signal | Rate |
|--------|------|
| TCD mean velocity (left/right MCA; PCA in some files) | 125 Hz |
| Arterial blood pressure (ABP) | 125 Hz |
| SpO2, end-tidal CO2 | 125 Hz |
| ECG | 500 Hz |

Some recordings also include EEG.

## Files

| Format | Contents |
|--------|----------|
| `.h5` | Raw, unsynchronized, de-identified recordings; timestamps in microseconds |
| `.csv` | Synchronized ABP and TCD; time starts at 0 per recording |
| clinical `.csv` | De-identified baseline clinical data (~5 KB) |

Files range from about 2.5 MB to about 198 MB. The HDF5 files are the natural input for
CLIO I/O tests.

## Access

Restricted: credentialed PhysioNet account plus the signed Restricted Health Data Use
Agreement 1.5.0. Keep the data in the git-ignored `data/` directory; see the restricted-data
rules in `CLAUDE.md`. Download:

```bash
wget -r -N -c -np --user <physionet-username> --ask-password \
  https://physionet.org/files/target-map/1.0.0/ -P data/
```

## Citation

Trombin Marques, M., Bao, K., Hayman, A., Peterson, K., Lee, E., Vitt, J., Hu, X., Smith, W.,
Hemphill, C., Sampaio Silva, G., & Amorim, E. (2026). TARGET-MAP: Transcranial doppler
AutoReGulation assessment of brain perfusion in nEurocritical patients for MAP optimization
(version 1.0.0). PhysioNet. https://doi.org/10.13026/b53c-b825

Also cite Pollard, T., et al. (2026). PhysioNet as a global platform for biomedical research.
*Nature Health*. https://doi.org/10.1038/s44360-026-00096-z

## Open

- Download the data and record the actual file inventory (count, sizes, HDF5 layout).
- Decide which CLIO component and workloads to test.
