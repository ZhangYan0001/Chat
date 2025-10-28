#include "httpmgr.h"

HttpMgr::HttpMgr() {
  connect(this, &HttpMgr::http_finish_signal, this, &HttpMgr::http_finish_slot);
}

HttpMgr::~HttpMgr() {}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id,
                          Modules mod) {
  QByteArray data = QJsonDocument(json).toJson();

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,
                    QByteArray::number(data.length()));

  auto self = shared_from_this();
  QNetworkReply *reply = _manager.post(request, data);

  QObject::connect(
      reply, &QNetworkReply::finished, [reply, self, req_id, mod]() {
        if (reply->error() != QNetworkReply::NoError) {
          qDebug() << "reply error>>---"<< reply->errorString()<<"---<<";
          emit self->http_finish_signal(req_id, "", ErrorCodes::ERROR_NETWORK,
                                        mod);
          reply->deleteLater();
          return;
        }

        QString res = reply->readAll();
        qDebug() << "reply is>>---"<<res.toStdString() << "---<<";
        emit self->http_finish_signal(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
      });
}

void HttpMgr::http_finish_slot(ReqId id, QString res, ErrorCodes err,
                               Modules mod) {
  if (mod == Modules::REGISTERMOD) {
    emit register_finish_signal(id, res, err);
  }
  if (mod == Modules::LOGIN){
    emit login_finish_signal(id, res, err);
  }
}
