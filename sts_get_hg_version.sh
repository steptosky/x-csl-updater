#!/bin/bash
hg parent --template "#ifndef STS_VERSION_FILE_H\n#define STS_VERSION_FILE_H\n\n#define STS_REVISION_LOCAL {rev}\n#define STS_REVISION_GLOBAL \"{node|short}\"\n#define STS_REVISION_BRANCH \"{branch}\"\n#define STS_REVISION_AUTHOR \"{author}\"\n\n#endif /*STS_VERSION_FILE_H*/" > version.h
