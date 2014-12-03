@echo off

set TARGET_PATH=%1

if "%TARGET_PATH%"=="" (
	set TARGET_PATH="version.h"
)

hg parent --template "#ifndef STS_VERSION_FILE_H\n#define STS_VERSION_FILE_H\n\n#define STS_REVISION_LOCAL {rev}\n#define STS_REVISION_GLOBAL \"{node^|short}\"\n#define STS_REVISION_BRANCH \"{branch}\"\n#define STS_REVISION_AUTHOR \"{author}\"\n\n#endif /*STS_VERSION_FILE_H*/" > %TARGET_PATH%

:pause;