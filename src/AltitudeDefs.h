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
            const QSettings settings(gSettingsFileName, QSettings::IniFormat);
            setServerUrl(settings.value("curServer").toString());
        }
        return mServerUrl;
    }

    QString baseUrL() {
        return serverUrl() + "/ALTITUDE";
    }

    static QString baseLocalDir() {
        return gTempDir + "/ALTITUDE";
    }

    //-------------------------------------------------------------------------
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

    static QString configFileLocalPath() {
        return baseLocalDir() + "/" + configFileName();
    }

    //-------------------------------------------------------------------------
    QString indexFileUrl() {
        return baseUrL() + "/" + indexFileName();
    }

    static QString indexFileLocalPath() {
        return baseLocalDir() + "/" + indexFileName();
    }

    QString indexForDelFileUrl() {
        return baseUrL() + "/" + indexForDelFileName();
    }

    static QString indexForDelFileLocalPath() {
        return baseLocalDir() + "/" + indexForDelFileName();
    }

    //-------------------------------------------------------------------------
    QString cslBaseUrl() {
        return serverUrl() + "/" + mCslRemoteDir;
    }

    QString cslBaseLocalDir() const {
        return mSimDir + "/" + mCslLocalDir;
    }

    QString cslFileUrl(const QString & fileUri) {
        return cslBaseUrl() + "/" + fileUri;
    }

    QString cslFileLocalPath(const QString & fileUri) const {
        return cslBaseLocalDir() + "/" + fileUri;
    }

    //-------------------------------------------------------------------------
    QString fileUrl(const QString & fileUri) {
        return baseUrL() + "/" + fileUri;
    }

    QString fileLocalPath(const QString & fileUri) const {
        QStringList list = fileUri.split('/');
        // mapping
        if (mFoldersMap.contains(list[0])) {
            return simDir() + "/" + QString(fileUri).replace(list[0], mFoldersMap[list[0]]);
        }
        // prefix
        // if (mFoldersMap.contains(list[0])) {
        //     return simDir() + "/" + QString(fileUri).prepend(mFoldersMap[list[0] + "/"]);
        // }
        return QString();
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

    static QString cslIndexFileLocalPath() {
        return baseLocalDir() + "/" + cslIndexFileName();
    }

    QString cslIndexForDelFileUrl() {
        return serverUrl() + QDir::cleanPath("/" + mCslRemoteDir + "/" + cslIndexForDelFileName());
    }

    static QString cslIndexForDelFileLocalPath() {
        return baseLocalDir() + "/" + cslIndexForDelFileName();
    }

    //-------------------------------------------------------------------------
    void setSimDir(const QString & simDir, bool isSimDirCustom);

    const QString & simDir() const {
        return mSimDir;
    }

    bool getAllPathsAndUrlsReady();

    //-------------------------------------------------------------------------
private:
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
