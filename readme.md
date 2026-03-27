[![Build only](https://github.com/steptosky/x-csl-updater/actions/workflows/build_only.yaml/badge.svg)](https://github.com/steptosky/x-csl-updater/actions/workflows/build_only.yaml)
## X-CSL-Package

This repo contains the X-CSL-Updater application, which is a part of the X-CSL-Package project.

The project web site is https://x-csl.ru

The application developer web site is https://steptosky.com

The X-CSL-Package project is a centralized CSL model library for displaying IVAO traffic in X-Plane.
The project is developed, maintained and served by the X-AiR and StepToSky Teams.
It can be installed and continuously updated using this X-CSL-Updater software.

### A bit technical details as a reminder:

    index files format:
    <id>%<file or dir path>%<size>%<not used for dirs or md5 hash for files>%<date>%<time>
    
    x-csl-indexes.idx
    11 - folder (a csl package)
    10 - file
    15 - subfolder, used for deleting logic (to be able to delete subfolders), not used for updating
    
    x-csl-indexes-for-delete.idx
    id - not used
    file or dir name - used to deletion
    the rest - not used



