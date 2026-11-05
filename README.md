# tmap

This repository tests the [TARGET-MAP](https://physionet.org/content/target-map/1.0.0/) dataset on PhysioNet with [CLIO](https://github.com/iowarp/clio), IOWarp's agentic data-management ecosystem.

## Dataset

**TARGET-MAP: Transcranial doppler AutoReGulation assessment of brain perfusion in nEurocritical patients for MAP optimization** (v1.0.0)

TARGET-MAP covers 24 neurocritical patients at Zuckerberg San Francisco General Hospital. For each patient it includes:

- baseline clinical data,
- continuous transcranial Doppler (TCD) recordings,
- multiparameter monitor data: ABP, ECG, ETCO2 and SpO2.

| Format | Contents |
|--------|----------|
| `.h5` (HDF5) | Raw, unsynchronized, de-identified recordings. Timestamps are in microseconds. |
| `.csv` | Synchronized ABP and TCD signals. Time starts at 0 in each recording. |
| `.csv` (clinical) | De-identified baseline clinical data and CPC outcome. |

The 125 Hz channels are ABP, TCD, SpO2 and ETCO2. ECG is recorded at 500 Hz. Files range from about 2.5 MB to about 198 MB.

## Data access

TARGET-MAP is **restricted-access** data. It is released under the PhysioNet Restricted Health Data License 1.5.0. To get it, you need a credentialed PhysioNet account and a signed Data Use Agreement.

**Do not commit any dataset files to this repository.** Download them locally into `data/`, which is git-ignored:

```bash
wget -r -N -c -np --user <physionet-username> --ask-password \
  https://physionet.org/files/target-map/1.0.0/ -P data/
```

## Citation

> Trombin Marques, M., Bao, K., Hayman, A., Peterson, K., Lee, E., Vitt, J., Hu, X., Smith, W., Hemphill, C., Sampaio Silva, G., & Amorim, E. (2026). TARGET-MAP: Transcranial doppler AutoReGulation assessment of brain perfusion in nEurocritical patients for MAP optimization (version 1.0.0). PhysioNet. https://doi.org/10.13026/b53c-b825

Please also cite the PhysioNet platform:

> Pollard, T., et al. (2026). PhysioNet as a global platform for biomedical research. *Nature Health*. https://doi.org/10.1038/s44360-026-00096-z
