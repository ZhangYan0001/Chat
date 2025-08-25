#ifndef HTTPMGR_H
#define HTTPMGR_H
#include <QJsonObject>
#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <memory>

#include "global.h"
#include "singleton.h"

class HttpMgr : public QObject,
                public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr> {
  Q_OBJECT

 public:
  ~HttpMgr();
  void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

  public slots:
  void http_finish_slot(ReqId id, QString res, ErrorCodes err, Modules mod);
 private:
  friend class Singleton<HttpMgr>;
  HttpMgr();
  QNetworkAccessManager _manager;
 signals:
  void http_finish_signal(ReqId id, QString res, ErrorCodes err, Modules mod);
  void register_finish_signal(ReqId id, QString res, ErrorCodes err);
};

#endif  // HTTPMGR_H
