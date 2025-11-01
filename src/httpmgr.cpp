#include "httpmgr.h"

HttpMgr::HttpMgr() {
  connect(this, &HttpMgr::http_finish_signal, this, &HttpMgr::http_finish_slot);
}

HttpMgr::~HttpMgr() {}

// void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id,
//                           Modules mod) {
//   QByteArray data = QJsonDocument(json).toJson();

//   QNetworkRequest request(url);
//   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//   request.setHeader(QNetworkRequest::ContentLengthHeader,
//                     QByteArray::number(data.length()));

//   auto self = shared_from_this();
//   QNetworkReply *reply = _manager.post(request, data);

//   QObject::connect(
//       reply, &QNetworkReply::finished, [reply, self, req_id, mod]() {
//         int statusCode =
//         reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//         qDebug() << "the status code is >> ---" << statusCode << "---<<";
//         QString res = reply->readAll();
//         if (reply->error() != QNetworkReply::NoError) {
//           qDebug() << "reply error>>---"<< reply->errorString()<<"---<<";
//           emit self->http_finish_signal(req_id, "",
//           ErrorCodes::ERROR_NETWORK,
//                                         mod);
//           reply->deleteLater();
//           return;
//         }

//         if (statusCode !=200) {
//           qDebug() << "HTTP error code:" << statusCode;
//           emit self->http_finish_signal(req_id, res, ErrorCodes::ERROR_HTTP,
//           mod); reply->deleteLater(); return;
//         }
//         qDebug() << "reply is>>---"<<res.toStdString() << "---<<";
//         emit self->http_finish_signal(req_id, res, ErrorCodes::SUCCESS, mod);
//         reply->deleteLater();
//         return;
//       });
// }
void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id,
                          Modules mod) {
  QByteArray data = QJsonDocument(json).toJson();

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,
                    QByteArray::number(data.length()));

  auto self = shared_from_this();
  QNetworkReply* reply = _manager.post(request, data);

  QObject::connect(
      reply, &QNetworkReply::finished, [reply, self, req_id, mod]() {
        HttpResponse response;
        response.httpStatus =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        QByteArray rawData = reply->readAll();
        response.errorCode = ErrorCodes::SUCCESS;

        // 1️⃣ 网络层错误
        if (reply->error() != QNetworkReply::NoError) {
          qWarning() << "Network error:" << reply->errorString();
          response.errorCode = ErrorCodes::ERROR_NETWORK;
          emit self->http_finish_signal(req_id, response, mod);
          reply->deleteLater();
          return;
        }

        // 2️⃣ HTTP 状态码错误
        if (response.httpStatus != 200) {
          qWarning() << "HTTP error:" << response.httpStatus;
          response.errorCode = ErrorCodes::ERROR_HTTP;
          emit self->http_finish_signal(req_id, response, mod);
          reply->deleteLater();
          return;
        }

        // 3️⃣ 解析 JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
          qWarning() << "JSON parse error:" << parseError.errorString();
          response.errorCode = ErrorCodes::ERROR_JSON_PARSE;
          emit self->http_finish_signal(req_id, response, mod);
          reply->deleteLater();
          return;
        }

        if (doc.isObject()) {
          QJsonObject obj = doc.object();
          response.code = obj.value("code").toInt(-1);
          response.msg = obj.value("message").toString();
          response.data = obj.value("data").toObject();

          // 4️⃣ 后端业务码检查
          if (response.code != 0) {
            response.errorCode = ErrorCodes::ERROR_BACKEND;
          }
        } else {
          response.errorCode = ErrorCodes::ERROR_JSON_FORMAT;
        }

        qDebug() << "HTTP Response:" << rawData;

        emit self->http_finish_signal(req_id, response, mod);
        reply->deleteLater();
      });
}

void HttpMgr::http_finish_slot(ReqId id, HttpResponse rep, Modules mod) {
  if (mod == Modules::REGISTERMOD) {
    emit register_finish_signal(id, rep, mod);
  }
  if (mod == Modules::LOGIN) {
    emit login_finish_signal(id, rep, mod);
  }
}
