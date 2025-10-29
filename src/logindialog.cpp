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
  connect(HttpMgr::GetInstance().get(), &HttpMgr::login_finish_signal, this,
          &LoginDialog::login_mod_finish_slot);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::on_login_btn_clicked() {
  auto email = loginInfo->_email;
  auto pwd = loginInfo->_pwd;
  if (!email.isEmpty() && !pwd.isEmpty()) {
    // 设置登陆按钮loading 动画
    ui->login_btn->setEnabled(false);
    ui->login_btn->setText("正在登陆中...");
    loadingMovie->start();
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["pwd"] = pwd;
    // TODO: 登陆请求
    HttpMgr::GetInstance()->PostHttpReq(
        QUrl("http://localhost:8080/api/login/login"), json_obj,
        ReqId::ID_LOGIN_USER, Modules::LOGIN);
    //
    QTimer::singleShot(3000, this, [=]() {
      loadingMovie->stop();
      ui->login_btn->setIcon(QIcon());
      ui->login_btn->setEnabled(true);
      ui->login_btn->setText("登陆");
    });
  } else {
    QMessageBox::warning(this, "提示", "请正确输入邮箱或密码");
  }
}

void LoginDialog::on_forgetpwd_label_linkActivated(const QString& link) {
  // 2. 检查传递过来的 link 是否是我们期望的，确保点击的是正确的链接
  if (link == "forget_password") {
    qDebug() << "Forget password link clicked!";  // 打印调试信息
  }
}

void LoginDialog::login_mod_finish_slot(ReqId id, QString res, ErrorCodes err) {
  qDebug() << "the req id >>--" << id << "---<<"
           << "finish slot err >>---" << err << "---<<";
  if (id != ReqId::ID_LOGIN_USER) return;  // 只处理登录请求

  if (err == ErrorCodes::ERROR_NETWORK) {
    QMessageBox::warning(this, "错误", "网络连接失败，请检查网络。");
    restoreLoginBtn();
    return;
  }

  QJsonParseError jsonError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8(), &jsonError);

  if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
    QMessageBox::warning(this, "错误", "服务器返回数据格式错误。");
    restoreLoginBtn();
    return;
  }

  QJsonObject obj = jsonDoc.object();
  int code = obj.value("code").toInt(-1);
  QString msg = obj.value("msg").toString();

  if (code == 200) {
    QString username = obj.value("username").toString();
    QMessageBox::information(this, "登录成功",
                             QString("欢迎回来，%1！").arg(username));
    emit login_success_signal(loginInfo->_email,
    obj.value("token").toString());
    // this->accept();
  } else {
    QMessageBox::warning(this, "登录失败",
                         msg.isEmpty() ? "账号或密码错误" : msg);
    restoreLoginBtn();
  }
}
void LoginDialog::restoreLoginBtn() {
  loadingMovie->stop();
  ui->login_btn->setIcon(QIcon());
  ui->login_btn->setEnabled(true);
  ui->login_btn->setText("登录");
}