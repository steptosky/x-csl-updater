// SPDX-License-Identifier: MPL-2.0

#include "AltitudeDefs.h"
#include <qsettings.h>

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

AltitudeDefs::AltitudeDefs(QObject * parent)
    : QObject(parent) {
    //-------------------------------------------------------------------------

}

AltitudeDefs::~AltitudeDefs() {}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void AltitudeDefs::setSimDir(const QString & simDir, bool isSimDirCustom) {
    mSimDir = simDir;
    mIsSimDurCustom = isSimDirCustom;
}

bool AltitudeDefs::getAllPathsAndUrlsReady() {
    const QString fileName = configFileLocalPath();
    if (!QFile::exists(fileName)) {
        qCritical() << QString("Config file for ALTITUDE client is not found! <%1>").arg(QFileInfo(fileName).absoluteFilePath());
        return false;
    }
    QSettings config(fileName, QSettings::IniFormat);
    config.beginGroup("x-csl-package");
    mCslRemoteDir = config.value("remoteDir").toString();
    mCslLocalDir = config.value("localDir").toString();
    config.endGroup();
    if (mCslRemoteDir.isEmpty() || mCslLocalDir.isEmpty()) {
        qCritical() << QString("Config file for ALTITUDE client is broken or has wrong format! <%1>").arg(QFileInfo(fileName).absoluteFilePath());
        return false;
    }
    //
    config.beginGroup("folders");
    QStringList keys = config.allKeys();
    mFoldersMap.clear();
    for (const auto & key : keys) {
        mFoldersMap[key] = config.value(key).toString();
    }
    config.endGroup();
    //
    return true;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
