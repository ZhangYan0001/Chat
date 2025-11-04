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

struct HttpResponse;

class HttpMgr : public QObject,
                public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr> {
  Q_OBJECT

 public:
  ~HttpMgr();
  void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

 public slots:

  void http_finish_slot(ReqId id, HttpResponse rep, Modules mod);
  void on_http_reply_finished(QNetworkReply* reply);

 private:
  friend class Singleton<HttpMgr>;
  HttpMgr();
  QNetworkAccessManager _manager;
  void HandleNetworkError(QNetworkReply* reply, HttpResponse& reps);
  HttpResponse ParseResponse(QNetworkReply* reply);
 signals:

  void http_finish_signal(ReqId id, HttpResponse rep, Modules mod);
  void register_finish_signal(ReqId id, HttpResponse rep, Modules mod);  //
  void login_finish_signal(ReqId id, HttpResponse rep, Modules mod);     //
};

struct HttpResponse {
  int httpStatus;
  int code;
  QString msg;
  QJsonObject data;
  ErrorCodes errorCode;
};

class ResponseHandler : public QObject, public Singleton<ResponseHandler> {
  Q_OBJECT
  friend class Singleton<ResponseHandler>;

 public:
  void InitConnections();

 signals:
  void login_response_signal(const HttpResponse& rep);
  void register_response_signal(const HttpResponse& rep);

 private slots:
  void on_http_finish(ReqId id, HttpResponse rep, Modules mod);

 private:
  ResponseHandler();
  ~ResponseHandler();
  void login_response_handler(const HttpResponse& rep);
  void register_response_handler(const HttpResponse& rep);
};
#endif  // HTTPMGR_H
