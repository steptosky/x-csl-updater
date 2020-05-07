This repo contains the X-CSL-Updater, which is a part of the X-CSL-Package project  
The official project site is http://csl.x-air.ru  
Developer site is http://stepstosky.com  

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