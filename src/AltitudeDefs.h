#pragma once

#include <QString>
#include <QNetworkAccessManager>
#include "Single.h"
#include "Definitions.h"

//-------------------------------------------------------------------------

class AltitudeDefs final : public QObject, public sts::Single<AltitudeDefs> {
Q_OBJECT
    //-------------------------------------------------------------------------
    friend sts::Single<AltitudeDefs>;
    explicit AltitudeDefs(QObject * parent);
    ~AltitudeDefs();

public:
    AltitudeDefs(const AltitudeDefs &) = delete;
    AltitudeDefs(AltitudeDefs &&) = delete;
    AltitudeDefs & operator=(const AltitudeDefs &) = delete;
    AltitudeDefs & operator=(AltitudeDefs &&) = delete;

    //-------------------------------------------------------------------------
signals:
    void ready();

public:

    //-------------------------------------------------------------------------
    bool isReady() const {
        return mIsReady;
    }

    //-------------------------------------------------------------------------
    static QString baseUrl() {
        const QSettings settings(gSettingsFileName, QSettings::IniFormat);
        return settings.value("curServer").toString() + "ALTITUDE";
    }

    static QString baseLocalDir() {
        return gTempDir + "/ALTITUDE";
    }

    //
    static QString indexFileName() {
        return "files.idx";
    }

    static QString configFileName() {
        return "client-config.ini";
    }

    //
    static QString indexFileUrl() {
        return baseUrl() + "/" + indexFileName();
    }

    static QString configFileUrl() {
        return baseUrl() + "/" + configFileName();
    }

    //
    static QString indexFileLocalPath() {
        return baseLocalDir() + "/" + indexFileName();
    }

    static QString configFileLocalPath() {
        return baseLocalDir() + "/" + configFileName();
    }
    //
    bool parseConfigFile();

    //-------------------------------------------------------------------------
private:
    bool mIsReady = false;
    //-------------------------------------------------------------------------
    QNetworkAccessManager & mNetMng;
    //-------------------------------------------------------------------------
    QString mCslRemoteDir;
    QString mCslLocalDir;
    //-------------------------------------------------------------------------
    
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
