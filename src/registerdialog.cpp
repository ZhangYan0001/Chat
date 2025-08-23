#include "registerdialog.h"

#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RegisterDialog) {
  loadStyleSheet(":/style/reg_dlg_style.qss");
  ui->setupUi(this);
  ui->pwd_edid->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->back_btn->setIcon(QIcon(":/images/previous.png"));
  ui->user_edit->setPlaceholderText("请起一个昵称");
  ui->pwd_edid->setPlaceholderText("设置密码至少8位，字母+数字可包含符号");
  ui->getcode_btn->setEnabled(false);
  regInfo = new UserRegisterInfo();
  initValidators();
}

RegisterDialog::~RegisterDialog() {
  delete ui;
  if (regInfo != nullptr) {
    regInfo = nullptr;
    delete regInfo;
  }
}

void RegisterDialog::showTip(const QString &text, const QString &type) {
  ui->err_label->setText(text);
  ui->err_label->setProperty("tipType", type);
  repolish(ui->err_label);

  QTimer::singleShot(2000, this, [this] {
    ui->err_label->clear();
    ui->err_label->setProperty("tipType", "");
    repolish(ui->err_label);
  });
}

void RegisterDialog::reset() {
  ui->user_edit->clear();
  ui->pwd_edid->clear();
  ui->email_edit->clear();
  ui->confirm_edit->clear();
  ui->code_edit->clear();
  ui->err_label->clear();
}

void RegisterDialog::initValidators() {
  // bool email_valid = false;
  // bool pwd_valid = false;
  // bool confimr_valid = false;
  // bool code_valid = false;
  connect(ui->email_edit, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text.isEmpty()) {
              showTip("邮箱不能为空", "warning");
            } else if (!emailRegex.match(text).hasMatch()) {
              showTip("邮箱格式错误", "warning");
            } else {
              email_valid = true;
              showTip("邮箱格式正确", "success");
            }
            regInfo->_email = text;
            regValid = email_valid && pwd_valid && confimr_valid;
            if (regValid) {
              emit reg_valid_signal();
            }
          });

  connect(ui->pwd_edid, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text.length() < 8) {
              showTip("密码至少需要8位", "warning");
            } else if (!pwdRegex.match(text).hasMatch()) {
              showTip("密码必须包含字母和数字，可以包含符号", "warning");
            } else {
              pwd_valid = true;
              showTip("密码可用", "success");
            }
            regInfo->_pwd = text;
            regValid = email_valid && pwd_valid && confimr_valid;
            if (regValid) {
              emit reg_valid_signal();
            }
          });

  connect(ui->confirm_edit, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text != ui->pwd_edid->text()) {
              showTip("两次密码不一致", "error");
            } else {
              confimr_valid = true;
              regValid = email_valid && pwd_valid && confimr_valid;
              if (regValid) {
                emit reg_valid_signal();
              }
              showTip("密码一致", "success");
            }
          });

  connect(ui->user_edit, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            if (!text.isEmpty()) {
              regInfo->_user = text;
            }
          });
  connect(ui->code_edit, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (!text.isEmpty()) {
              regInfo->_getcode = text;
              code_valid = true;
            }
          });

  connect(this, &RegisterDialog::reg_valid_signal, this,
          [this]() { ui->getcode_btn->setEnabled(true); });
}

void RegisterDialog::on_reg_btn_clicked() {
  if (regValid && code_valid) {
    QString user = regInfo->_user;
    QString email = regInfo->_email;
    QString pwd = regInfo->_pwd;
    QString code = regInfo->_getcode;
    QString errMsg;
    // user name
    if (email.isEmpty() || pwd.isEmpty()) {
      showTip("相关信息缺失", "error");
    }
    qDebug() << "the print the debug regInfo\n"
             << "user: " << user << "\n "
             << "email: " << email << "\n"
             << "pwd: " << pwd << "\n"
             << "code: " << code << "\n";
  }
  //   QString user = regInfo->_user;
  //   QString email = regInfo->_email;
  //   QString pwd = regInfo->_pwd;
  //   QString code = regInfo->_getcode;
  //   QString errMsg;
  //   // user name
  //   if (email.isEmpty() || pwd.isEmpty() || code.isEmpty()) {
  //     showTip("相关信息缺失", "error");
  //   }
  //   qDebug() << "the print the debug regInfo\n"
  //            << "user: " << user << "\n "
  //            << "email: " << email << "\n"
  //            << "pwd: " << pwd << "\n"
  //            << "code: " << code << "\n";

  //   if (!valid) return;  // 阻止提交
}

void RegisterDialog::on_back_btn_clicked() {
  reset();
  emit back_signal();
}

void RegisterDialog::on_getcode_btn_clicked() {
  auto email = regInfo->_email;
  qDebug() << "the get code btn emali is :" << email << '\n';
  if (email.isEmpty()) {
    showTip(QString("email is empty, please input an email"), "error");
    return;
  }
  // QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
  // bool match = regex.match(email).hasMatch();
  // if (match) {
  // } else {
  //   showTip(QString("email format error, please reinput"), "error");
  // }

  // 发送请求 get code
  // QTimer::singleShot(60000, [this]() {});
  // ui->getcode_btn->setEnabled(true);

  //
}
