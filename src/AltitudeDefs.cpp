#include "AltitudeDefs.h"
#include <qsettings.h>

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

AltitudeDefs::AltitudeDefs(QObject * parent)
    : QObject(parent),
      mNetMng(*new QNetworkAccessManager(this)) {
    //-------------------------------------------------------------------------

}

AltitudeDefs::~AltitudeDefs() {
    delete &mNetMng;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void AltitudeDefs::getFullyReady() {
    const QSettings settings(gSettingsFileName, QSettings::IniFormat);
    QString serverUrl = settings.value("curServer").toString();

    // QNetworkRequest request;
    // request.setUrl(serverUrl + configFileUrl());
    // request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(mIndexFile));
    // QNetworkReply * reply = mNetMng->get(request);
    // connect(this, &IndexStep::abortAllReplaysSig, reply, &QNetworkReply::abort);
    // connect(reply, &QNetworkReply::downloadProgress, this, &IndexStep::indexDownloadProgress);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
