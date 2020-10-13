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
            return simDir() + "/" + mCslLocalDir;
        }
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
        if (mFoldersMap.contains(list[1])) {
            QString res = simDir() + "/" + mFoldersMap[list[1]];
            for (int i = 2; i < list.size(); ++i) {
                res += "/" + list.at(i);
            }
            return res;
        }
        // we have no mapping for the specified file
        if (fileUri.contains(infoFileName())){
            // for info file
            QString res = altitudeBaseLocalDir();
            for (int i = 1; i < list.size(); ++i) {
                res += "/" + list.at(i);
            }
            return res;
        }
        // for other unmapped files
        QString res = altitudeBaseLocalDir() + "/unmapped";
        for (int i = 1; i < list.size(); ++i) {
            res += "/" + list.at(i);
        }
        qDebug() << "Cannot map the file uri: " << fileUri;
        qDebug() << "Will map it to unmapped directory: " << res;
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
