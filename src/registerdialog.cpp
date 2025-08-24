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
  regInfo = std::make_unique<UserRegisterInfo>();
  initValidators();
}

RegisterDialog::~RegisterDialog() { delete ui; }

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
  connect(ui->email_edit, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text.isEmpty()) {
              email_valid = false;
              showTip("邮箱不能为空", "warning");
            } else if (!emailRegex.match(text).hasMatch()) {
              email_valid = false;
              showTip("邮箱格式错误", "warning");
            } else {
              email_valid = true;
              showTip("邮箱格式正确", "success");
            }
            regInfo->_email = text;
            isRegValidSignal();
          });

  connect(ui->pwd_edid, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text.length() < 8) {
              pwd_valid = false;
              showTip("密码至少需要8位", "warning");
            } else if (!pwdRegex.match(text).hasMatch()) {
              pwd_valid = false;
              showTip("密码必须包含字母和数字，可以包含符号", "warning");
            } else {
              pwd_valid = true;
              showTip("密码可用", "success");
            }
            regInfo->_pwd = text;

            isRegValidSignal();
          });

  connect(ui->confirm_edit, &QLineEdit::textChanged, this,
          [&, this](const QString &text) {
            if (text != ui->pwd_edid->text()) {
              confimr_valid = false;
              showTip("两次密码不一致", "error");
            } else {
              confimr_valid = true;
              showTip("密码一致", "success");
            }
            isRegValidSignal();
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
}

void RegisterDialog::isRegValidSignal() {
  regValid = email_valid && pwd_valid && confimr_valid;
  // 如果处于倒计时阶段，按钮仍然不可用
  if (regValid && !isCountingDown) {
    ui->getcode_btn->setEnabled(true);
  } else {
    ui->getcode_btn->setEnabled(false);
  }
}

void RegisterDialog::on_reg_btn_clicked() {
  if (regValid && code_valid) {
    QString user = regInfo->_user;
    QString email = regInfo->_email;
    QString pwd = regInfo->_pwd;
    QString code = regInfo->_getcode;
    QString errMsg;

    qDebug() << "the print the debug regInfo\n"
             << "user: " << user << "\n "
             << "email: " << email << "\n"
             << "pwd: " << pwd << "\n"
             << "code: " << code << "\n";
  } else {
    showTip("请补充相关信息", "error");
    return;
  }
}

void RegisterDialog::on_back_btn_clicked() {
  reset();
  emit back_signal();
}

void RegisterDialog::on_getcode_btn_clicked() {
  if (regValid) {
    auto email = regInfo->_email;
    qDebug() << "the get code btn emali is :" << email << '\n';
    // TODO: 发起http请求，发送验证码

    // 禁用按钮并开始计时
    ui->getcode_btn->setEnabled(false);
    int countdown = 10;
    isCountingDown = true;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
      countdown--;
      if (countdown > 0) {
        ui->getcode_btn->setText(QString("重新获取(%1s)").arg(countdown));
      } else {
        timer->stop();
        timer->deleteLater();
        isCountingDown = false;
        ui->getcode_btn->setText("获取");
        if (regValid) {
          ui->getcode_btn->setEnabled(true);
        }
      }
    });
    timer->start(1000);
  }
}
