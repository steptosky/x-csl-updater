// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <QString>
#include "Single.h"
#include "Definitions.h"

//-------------------------------------------------------------------------

class AltitudeDefs final : public QObject, public sts::Single<AltitudeDefs> {
Q_OBJECT
    //-------------------------------------------------------------------------
    friend sts::Single<AltitudeDefs>;
    explicit AltitudeDefs(QObject * parent);
    virtual ~AltitudeDefs();

public:
    AltitudeDefs(const AltitudeDefs &) = delete;
    AltitudeDefs(AltitudeDefs &&) = delete;
    AltitudeDefs & operator=(const AltitudeDefs &) = delete;
    AltitudeDefs & operator=(AltitudeDefs &&) = delete;

    //-------------------------------------------------------------------------
    void setServerUrl(const QString & serverUrl) {
        mServerUrl = serverUrl;
        if (mServerUrl.endsWith('/')) {
            mServerUrl = mServerUrl.remove(mServerUrl.size() - 1, 1);
        }
    }

    //-------------------------------------------------------------------------
    QString serverUrl() {
        if (mServerUrl.isEmpty()) {
            const QSettings settings(settingsFileName(), QSettings::IniFormat);
            setServerUrl(settings.value("curServer").toString());
        }
        return mServerUrl;
    }

    QString baseUrL() {
        return serverUrl() + "/ALTITUDE";
    }

    QString baseLocalDir() const {
        if (isCustomSimDirSelected()) {
            return tempDir();
        }
        else{
            return simDir() + "/Output/preferences/X-CSL-Package";
        }
    }

    QString altitudeBaseLocalDir() const {
        return baseLocalDir() + "/ALTITUDE";
    }

    //-------------------------------------------------------------------------
    static QString infoFileName() {
        return "x-csl-info.info";
    }

    static QString configFileName() {
        return "client-config.ini";
    }

    static QString indexFileName() {
        return "files.idx";
    }

    static QString indexForDelFileName() {
        return "files-for-delete.idx";
    }

    static QString cslIndexFileName() {
        return "x-csl-indexes.idx";
    }

    static QString cslIndexForDelFileName() {
        return "x-csl-indexes-for-delete.idx";
    }

    //-------------------------------------------------------------------------
    QString configFileUrl() {
        return baseUrL() + "/" + configFileName();
    }

    QString configFileLocalPath() const {
        return altitudeBaseLocalDir() + "/" + configFileName();
    }

    //-------------------------------------------------------------------------
    QString indexFileUrl() {
        return baseUrL() + "/" + indexFileName();
    }

    QString indexFileLocalPath() const {
        return altitudeBaseLocalDir() + "/" + indexFileName();
    }

    QString indexForDelFileUrl() {
        return baseUrL() + "/" + indexForDelFileName();
    }

    QString indexForDelFileLocalPath() const {
        return altitudeBaseLocalDir() + "/" + indexForDelFileName();
    }

    //-------------------------------------------------------------------------
    QString cslBaseUrl() {
        return serverUrl() + "/" + mCslRemoteDir;
    }

    QString cslBaseLocalDir() const {
        if (isCustomSimDirSelected()){
            return simDir();
        }
        else{
            return safeLocalPath(simDir(), mCslLocalDir, QString("localDir=%1").arg(mCslLocalDir));
        }
    }

    QString cslFileUrl(const QString & fileUri) {
        return cslBaseUrl() + "/" + fileUri;
    }

    QString cslFileLocalPath(const QString & fileUri) const {
        const QString base = cslBaseLocalDir();
        if (base.isEmpty()) {
            return QString();
        }
        return safeLocalPath(base, fileUri, fileUri);
    }

    //-------------------------------------------------------------------------
    QString fileUrl(const QString & fileUri) {
        return baseUrL() + "/" + fileUri;
    }

    QString fileLocalPath(const QString & fileUri) const {
        const QString nativeFileUri = QDir::fromNativeSeparators(fileUri);
        const QString normalizedFileUri = QDir::cleanPath(nativeFileUri);
        if (normalizedFileUri.startsWith("/") || QFileInfo(normalizedFileUri).isAbsolute()) {
            qWarning() << "Rejected unsafe file uri: " << fileUri;
            return QString();
        }
        QStringList list = nativeFileUri.split('/');
        if (list.size() < 2 || list[0].isEmpty() || list[1].isEmpty() || list[0] == "..") {
            qWarning() << "Cannot map malformed file uri: " << fileUri;
            return QString();
        }
        // mapping
        if (mFoldersMap.contains(list[1])) {
            QString relativePath = mFoldersMap[list[1]];
            for (int i = 2; i < list.size(); ++i) {
                relativePath += "/" + list.at(i);
            }
            return safeLocalPath(simDir(), relativePath, fileUri);
        }
        // we have no mapping for the specified file
        if (fileUri.contains(infoFileName())){
            // for info file
            QString relativePath;
            for (int i = 1; i < list.size(); ++i) {
                if (!relativePath.isEmpty()) {
                    relativePath += "/";
                }
                relativePath += list.at(i);
            }
            return safeLocalPath(altitudeBaseLocalDir(), relativePath, fileUri);
        }
        // for other unmapped files
        QString relativePath;
        for (int i = 1; i < list.size(); ++i) {
            if (!relativePath.isEmpty()) {
                relativePath += "/";
            }
            relativePath += list.at(i);
        }
        const QString res = safeLocalPath(altitudeBaseLocalDir(), "unmapped/" + relativePath, fileUri);
        if (!res.isEmpty()) {
            qDebug() << "Cannot map the file uri: " << fileUri;
            qDebug() << "Will map it to unmapped directory: " << res;
        }
        return res;
    }

    //-------------------------------------------------------------------------
    QString fullLocalPath(eFileType fileType, const QString & fileUri) const {
        switch (fileType) {
            case CSL_PACK_FILE:
                return cslFileLocalPath(fileUri);
                break;
            case ADDITIONAL_FILE: 
            default: 
                return fileLocalPath(fileUri);
                break;
        }
    }

    QString fullUrl(eFileType fileType, const QString & fileUri) {
        switch (fileType) {
            case CSL_PACK_FILE: 
                return cslFileUrl(fileUri);
                break;
            case ADDITIONAL_FILE: 
            default: ;
                return fileUrl(fileUri);
                break;
        }
    }

    //-------------------------------------------------------------------------
    QString cslIndexFileUrl() {
        return serverUrl() + QDir::cleanPath("/" + mCslRemoteDir + "/" + cslIndexFileName());
    }

    QString cslIndexFileLocalPath() const {
        return baseLocalDir() + "/" + cslIndexFileName();
    }

    QString cslIndexForDelFileUrl() {
        return serverUrl() + QDir::cleanPath("/" + mCslRemoteDir + "/" + cslIndexForDelFileName());
    }

    QString cslIndexForDelFileLocalPath() const {
        return baseLocalDir() + "/" + cslIndexForDelFileName();
    }

    //-------------------------------------------------------------------------
    void setSimDir(const QString & simDir, bool isSimDirCustom);

    const QString & simDir() const {
        return mSimDir;
    }

    bool isCustomSimDirSelected() const {
        return mIsSimDurCustom;
    }

    bool getAllPathsAndUrlsReady();

    //-------------------------------------------------------------------------
private:
    QString safeLocalPath(const QString & baseDir, const QString & relativePath, const QString & sourceUri) const {
        const QString normalizedRelative = QDir::cleanPath(QDir::fromNativeSeparators(relativePath));
        if (baseDir.isEmpty() || normalizedRelative.isEmpty() || normalizedRelative == "."
            || normalizedRelative.startsWith("/") || QFileInfo(normalizedRelative).isAbsolute()) {
            qWarning() << "Rejected unsafe file uri: " << sourceUri
                       << "; relative path: " << relativePath
                       << "; base dir: " << baseDir;
            return QString();
        }

        const QString base = QDir::cleanPath(QFileInfo(baseDir).absoluteFilePath());
        const QString target = QDir::cleanPath(QDir(base).absoluteFilePath(normalizedRelative));
#ifdef Q_OS_WIN
        const Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
#else
        const Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive;
#endif
        const QString basePrefix = base.endsWith("/") ? base : base + "/";
        if (target != base && !target.startsWith(basePrefix, caseSensitivity)) {
            qWarning() << "Rejected file uri outside base dir: " << sourceUri
                       << "; target: " << target
                       << "; base dir: " << base;
            return QString();
        }

        return target;
    }

    bool mIsReady = false;
    //-------------------------------------------------------------------------
    QString mServerUrl;
    QString mSimDir;
    bool mIsSimDurCustom = false;
    //-------------------------------------------------------------------------
    QString mCslRemoteDir;
    QString mCslLocalDir;
    //-------------------------------------------------------------------------
    QMap<QString, QString> mFoldersMap;
    //-------------------------------------------------------------------------
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
