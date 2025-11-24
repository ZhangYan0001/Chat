#include "httpmgr.h"

HttpMgr::HttpMgr() {
  // connect(this, &HttpMgr::http_finish_signal, this, &HttpMgr::http_finish_slot);
  connect(&_manager, &QNetworkAccessManager::finished, this, &HttpMgr::on_http_reply_finished);
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

}

void HttpMgr::on_http_reply_finished(QNetworkReply* reply) {
  ReqId req_id = static_cast<ReqId>(reply->property("req_id").toInt());
  Modules mod = static_cast<Modules>(reply->property("module").toInt());

  HttpResponse reps;
  if (reply->error() != QNetworkReply::NoError) {
    HandleNetworkError(reply, reps);
    emit http_finish_signal(req_id, reps, mod);
    reply->deleteLater();
    return;
  }
  reps.httpStatus =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  reps = ParseResponse(reply);
  emit http_finish_signal(req_id, reps, mod);
  reply->deleteLater();
}

void HttpMgr::HandleNetworkError(QNetworkReply* reply, HttpResponse& reps) {
  reps.errorCode = ErrorCodes::ERROR_NETWORK;
  reps.httpStatus =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  reps.msg = reply->errorString();
  qWarning() << "[HttpMgr] 网络错误:" << reps.msg;
}

HttpResponse HttpMgr::ParseResponse(QNetworkReply* reply) {
  HttpResponse reps;
  reps.httpStatus =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  reps.errorCode = ErrorCodes::SUCCESS;
  QByteArray raw = reply->readAll();
  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);

  if (parseError.error != QJsonParseError::NoError) {
    reps.errorCode = ErrorCodes::ERROR_JSON_PARSE;
    reps.msg = "响应解析失败";
    qWarning() << "[HttpMgr] JSON解析失败:" << parseError.errorString();
    return reps;
  }

  if (!doc.isObject()) {
    reps.errorCode = ErrorCodes::ERROR_JSON_PARSE;
    reps.msg = "响应不是对象";
    return reps;
  }
  QJsonObject obj = doc.object();
  reps.code = obj.value("code").toInt();
  reps.msg = obj.value("msg").toString();
  reps.data = obj.value("data").toObject();

  return reps;
}

ResponseHandler::ResponseHandler() { InitConnections(); }

ResponseHandler::~ResponseHandler() {}

void ResponseHandler::InitConnections() {
  connect(HttpMgr::GetInstance(), &HttpMgr::http_finish_signal, this,
          &ResponseHandler::on_http_finished);
}

void ResponseHandler::on_http_finished(ReqId id, HttpResponse rep, Modules mod) {
  Q_UNUSED(mod);

  switch (id) {
    case ReqId::ID_LOGIN_USER:
      login_response_handler(rep);
      break;
    case ReqId::ID_REG_USER:
      register_response_handler(rep);
      break;
    case ReqId::ID_GET_VARIFY_CODE:
      verify_code_response_handler(rep);
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

void ResponseHandler::verify_code_response_handler(const HttpResponse &rep){
  emit verify_code_response_signal(rep);
}