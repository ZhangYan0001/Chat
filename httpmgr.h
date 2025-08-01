#ifndef HTTPMGR_H
#define HTTPMGR_H
#include <QJsonObject>
#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include "global.h"
#include "singleton.h"

class HttpMgr : public QObject, public Singleton<HttpMgr>,
{
    Q_OBJECT
public:
    HttpMgr();
};

#endif // HTTPMGR_H
