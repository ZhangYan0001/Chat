#include "logindialog.h"

#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);
  ui->pwd_edit->setEchoMode(QLineEdit::Password);
  loadingMovie = new QMovie(":/images/loading.gif");
  loadingMovie->setScaledSize(QSize(16, 16));
  connect(ui->reg_btn, &QPushButton::clicked, this,
          [=]() { emit login_reg_signal(); });

  loginInfo = std::make_unique<UserLoginInfo>();

  connect(ui->email_edit, &QLineEdit::textChanged, this,
          [this](const QString& text) {
            if (emailRegex.match(text).hasMatch()) {
              loginInfo->_email = text;
            } else {
              loginInfo->_email = "";
            }
          });
  connect(ui->pwd_edit, &QLineEdit::textChanged, this,
          [this](const QString& text) {
            if (pwdRegex.match(text).hasMatch()) {
              loginInfo->_pwd = text;
            } else {
              loginInfo->_pwd = "";
            }
          });

  connect(loadingMovie, &QMovie::frameChanged, this, [=]() {
    ui->login_btn->setIcon(QIcon(loadingMovie->currentPixmap()));
  });
  connect(ResponseHandler::GetInstance(),
          &ResponseHandler::login_response_signal, this,
          &LoginDialog::login_response_slots);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::on_login_btn_clicked() {
  emit login_success_signal();

  // auto email = loginInfo->_email;
  // auto pwd = loginInfo->_pwd;

  // if (email.isEmpty() || pwd.isEmpty()) {
  //   QMessageBox::warning(this, "提示", "请正确输入邮箱或密码");
  //   return;
  // }

  // // 禁用按钮 + 加载动画
  // ui->login_btn->setEnabled(false);
  // ui->login_btn->setText("正在登录中...");
  // ui->login_btn->setIcon(
  //     QIcon(QPixmap::fromImage(loadingMovie->currentImage())));
  // loadingMovie->start();

  // QJsonObject json_obj;
  // json_obj["email"] = email;
  // json_obj["pwd"] = pwd;

  // HttpMgr::GetInstance()->PostHttpReq(
  //     QUrl("http://localhost:8080/api/login/login"), json_obj,
  //     ReqId::ID_LOGIN_USER, Modules::LOGIN);
}

void LoginDialog::on_forgetpwd_label_linkActivated(const QString& link) {
  // 2. 检查传递过来的 link 是否是我们期望的，确保点击的是正确的链接
  if (link == "forget_password") {
    qDebug() << "Forget password link clicked!";  // 打印调试信息
  }
}

void LoginDialog::login_response_slots(const HttpResponse& rep) {
  // 停止加载动画
  loadingMovie->stop();
  ui->login_btn->setIcon(QIcon());
  ui->login_btn->setEnabled(true);
  ui->login_btn->setText("登录");

  // 1️⃣ 检查网络层错误
  if (rep.errorCode != ErrorCodes::SUCCESS) {
    QString errMsg;
    switch (rep.errorCode) {
      case ErrorCodes::ERROR_NETWORK:
        errMsg = "网络连接失败，请检查网络。";
        break;
      case ErrorCodes::ERROR_HTTP:
        errMsg = QString("HTTP错误：%1").arg(rep.httpStatus);
        break;
      case ErrorCodes::ERROR_JSON_PARSE:
        errMsg = "响应数据解析失败。";
        break;
      default:
        errMsg = "未知错误，请稍后重试。";
        break;
    }
    QMessageBox::warning(this, "登录失败", errMsg);
    return;
  }

  if (rep.code != 0) {
    QMessageBox::warning(this, "登陆失败",
                         rep.msg.isEmpty() ? "服务器返回错误" : rep.msg);
    return;
  }

  QJsonObject data = rep.data;
  QString username = data.value("username").toString();
  QString email = data.value("email").toString();

  if (username.isEmpty()) {
    QMessageBox::warning(this, "登陆失败", "返回数据不完整");
    return;
  }
  qDebug() << "登陆成功 用户" << username << "邮箱 ：" << email;
  QMessageBox::information(this, "登陆成功",
                           QString("欢迎回来,%1!").arg(username));
  this->close();
  emit login_success_signal();
}