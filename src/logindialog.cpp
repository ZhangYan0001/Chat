#include "logindialog.h"

#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);
  ui->pwd_edit->setEchoMode(QLineEdit::Password);
  loadingMovie = new QMovie(":/images/loading.gif");
  loadingMovie->setScaledSize(QSize(16, 16));
  connect(ui->reg_btn, &QPushButton::clicked, this,
          [=]() { emit login_reg_signal(); });

  loginInfo = std::make_unique<UserLoginInfo>();

  connect(ui->email_edit, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            if (emailRegex.match(text).hasMatch()) {
              loginInfo->_email = text;
            } else {
              loginInfo->_email = "";
            }
          });
  connect(ui->pwd_edit, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            if (pwdRegex.match(text).hasMatch()) {
              loginInfo->_pwd = text;
            } else {
              loginInfo->_pwd = "";
            }
          });

  connect(loadingMovie, &QMovie::frameChanged, this, [=]() {
    ui->login_btn->setIcon(QIcon(loadingMovie->currentPixmap()));
  });
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

    // TODO: 登陆请求

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

void LoginDialog::on_forgetpwd_label_linkActivated(const QString &link) {
  // 2. 检查传递过来的 link 是否是我们期望的，确保点击的是正确的链接
  if (link == "forget_password") {
    qDebug() << "Forget password link clicked!";  // 打印调试信息
  }
}
