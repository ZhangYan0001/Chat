#include "registerdialog.h"

#include "global.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
  loadStyleSheet(":/style/reg_dlg_style.qss");
  ui->setupUi(this);
  ui->pwd_edid->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->back_btn->setIcon(QIcon(":/images/previous.png"));
  ui->login_btn->setDisabled(true);
  ui->reg_btn->setDisabled(true);

  
}

RegisterDialog::~RegisterDialog() { delete ui; }
void RegisterDialog::showTip(const QString &text, const QString &type) {
  ui->err_label->setText(text);
  ui->err_label->setProperty("tipType", type);
  repolish(ui->err_label);

  QTimer::singleShot(4000, this, [this] {
    ui->err_label->clear();
    ui->err_label->setProperty("tipType", "");
    repolish(ui->err_label);
  });
}

void RegisterDialog::on_reg_btn_clicked() {}
void RegisterDialog::on_back_btn_clicked() { emit back_signal(); }

void RegisterDialog::on_getcode_btn_clicked() {
  auto email = ui->email_edit->text();
  if (email.isEmpty()) {
    showTip(QString("email is empty, please input an email"), "error");
    return;
  }
  QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
  bool match = regex.match(email).hasMatch();
  if (match) {
  } else {
    showTip(QString("email format error, please reinput"), "error");
  }
}
