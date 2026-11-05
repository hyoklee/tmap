# tmap — LLM wiki

This repo is a Markdown knowledge base that an LLM writes and keeps up to date.
People add sources; the LLM reads them and maintains the wiki. The subject is testing
the PhysioNet TARGET-MAP dataset (transcranial Doppler + bedside monitor recordings
from neurocritical patients) with IOWarp's CLIO.

## Layout

- `raw/` — source material (notes, web clippings, scripts, job outputs). Never edit these files once added.
  Name new files `YYYY-MM-DD-<topic>` (a file or a directory).
- `wiki/` — pages the LLM writes. One topic per page, file names in kebab-case (`target-map.md`).
- `wiki/index.md` — a list of every page with a one-line summary. Keep it current.
- `wiki/log.md` — a dated record of every ingest, query and cleanup pass, newest at the bottom.
- `data/` — local copy of the dataset. Git-ignored; never committed.

## Workflows

**Ingest** (a new file in `raw/`): read it, write a summary page for it, then update every
existing page it affects, add `[[links]]` between related pages, add the new pages to
`index.md`, and add a line to `log.md`.

**Query**: answer from the wiki, citing pages. If the answer is worth keeping, save it as a page.

**Cleanup**: look for contradictions, stale claims, orphan pages, missing links and
topics that are mentioned but have no page. Fix what you can and log the rest.

## Rules

- Every factual claim cites a source in `raw/` or another wiki page.
- Prefer editing an existing page over creating a near-duplicate.
- When sources disagree, record both views and say which source holds each.

## Restricted data

TARGET-MAP is released under the PhysioNet Restricted Health Data License 1.5.0 and its
Data Use Agreement, and this repo is public.

- Never put dataset files (`.h5`, signal or clinical `.csv`) or excerpts of them in `raw/` or `wiki/`.
- Record only aggregate, non-identifying results: timings, file sizes, counts, schemas,
  benchmark numbers. No per-patient values.
- `.gitignore` blocks `*.h5`, `*.hdf5` and `*.csv`. To commit a derived CSV (for example
  benchmark timings), first check that it has no patient data, then use `git add -f`.
