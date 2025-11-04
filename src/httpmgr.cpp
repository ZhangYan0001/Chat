#include "httpmgr.h"

HttpMgr::HttpMgr() {
  connect(this, &HttpMgr::http_finish_signal, this, &HttpMgr::http_finish_slot);
}

HttpMgr::~HttpMgr() {}


void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id,
                          Modules mod) {
  QByteArray data = QJsonDocument(json).toJson();

  QNetworkRequest request(url);
  QByteArray body = QJsonDocument(json).toJson(QJsonDocument::Compact);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,
                    QByteArray::number(data.length()));

  // auto self = shared_from_this();
  QNetworkReply* reply = _manager.post(request, data);
  reply->setProperty("req_id", static_cast<int>(req_id));
  reply->setProperty("module", static_cast<int>(mod));
  qDebug() << "[HttpMgr] POST:" << url.toString();
  qDebug() << "[HttpMgr] Body:" << QString::fromUtf8(body);

  // QObject::connect(
  //     reply, &QNetworkReply::finished, [reply, self, req_id, mod]() {
  //       HttpResponse response;
  //       response.httpStatus =
  //           reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  //       QByteArray rawData = reply->readAll();
  //       response.errorCode = ErrorCodes::SUCCESS;

  //       // 1️⃣ 网络层错误
  //       if (reply->error() != QNetworkReply::NoError) {
  //         qWarning() << "Network error:" << reply->errorString();
  //         response.errorCode = ErrorCodes::ERROR_NETWORK;
  //         emit self->http_finish_signal(req_id, response, mod);
  //         reply->deleteLater();
  //         return;
  //       }

  //       // 2️⃣ HTTP 状态码错误
  //       if (response.httpStatus != 200) {
  //         qWarning() << "HTTP error:" << response.httpStatus;
  //         response.errorCode = ErrorCodes::ERROR_HTTP;
  //         emit self->http_finish_signal(req_id, response, mod);
  //         reply->deleteLater();
  //         return;
  //       }

  //       // 3️⃣ 解析 JSON
  //       QJsonParseError parseError;
  //       QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);
  //       if (parseError.error != QJsonParseError::NoError) {
  //         qWarning() << "JSON parse error:" << parseError.errorString();
  //         response.errorCode = ErrorCodes::ERROR_JSON_PARSE;
  //         emit self->http_finish_signal(req_id, response, mod);
  //         reply->deleteLater();
  //         return;
  //       }

  //       if (doc.isObject()) {
  //         QJsonObject obj = doc.object();
  //         response.code = obj.value("code").toInt(-1);
  //         response.msg = obj.value("message").toString();
  //         response.data = obj.value("data").toObject();

  //         // 4️⃣ 后端业务码检查
  //         if (response.code != 0) {
  //           response.errorCode = ErrorCodes::ERROR_BACKEND;
  //         }
  //       } else {
  //         response.errorCode = ErrorCodes::ERROR_JSON_FORMAT;
  //       }

  //       qDebug() << "HTTP Response:" << rawData;

  //       emit self->http_finish_signal(req_id, response, mod);
  //       reply->deleteLater();
  //     });
}

void HttpMgr::http_finish_slot(ReqId id, HttpResponse rep, Modules mod) {
  if (mod == Modules::REGISTERMOD) {
    emit register_finish_signal(id, rep, mod);
  }
  if (mod == Modules::LOGIN) {
    emit login_finish_signal(id, rep, mod);
  }
}

void HttpMgr::on_http_reply_finished(QNetworkReply *reply) {
  ReqId req_id = static_cast<ReqId>(reply->property("req_id").toInt());
  Modules mod = static_cast<Modules>(reply->property("module").toInt());

  HttpResponse reps;
  if (reply->error() != QNetworkReply::NoError) {
    HandleNetworkError(reply, reps);
    emit http_finish_signal(req_id, reps, mod);
  }
}

void HttpMgr::HandleNetworkError(QNetworkReply *reply, HttpResponse &reps){

}

ResponseHandler::ResponseHandler() {
  InitConnections();
}

ResponseHandler::~ResponseHandler() {}

void ResponseHandler::InitConnections() {
  connect(HttpMgr::GetInstance().get(), &HttpMgr::http_finish_signal, this,
          &ResponseHandler::on_http_finish);
}

void ResponseHandler::on_http_finish(ReqId id, HttpResponse rep, Modules mod) {
  Q_UNUSED(mod);

  switch (id) {
    case ReqId::ID_LOGIN_USER:
      login_response_handler(rep);
      break;
    case ReqId::ID_REG_USER:
      register_response_handler(rep);
      break;
    default:
      qDebug() << "[ResponseHandler] 未处理的 ReqId: " << static_cast<int>(id);
      break;
  }
}

void ResponseHandler::register_response_handler(const HttpResponse& rep) {
  emit register_response_signal(rep);
}

void ResponseHandler::login_response_handler(const HttpResponse& rep) {
  emit login_response_signal(rep);
}