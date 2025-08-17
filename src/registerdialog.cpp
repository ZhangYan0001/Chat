#include "registerdialog.h"

#include "global.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
  ui->setupUi(this);
  ui->pwd_edid->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->back_btn->setIcon(QIcon(":/images/previous.png"));
}

RegisterDialog::~RegisterDialog() { delete ui; }
void RegisterDialog::showTip(QString str) {
  ui->err_label->setText(str);
  repolish(ui->err_label);
}

void RegisterDialog::on_reg_btn_clicked() {}
void RegisterDialog::on_back_btn_clicked() {
  emit back_signal();
}

void RegisterDialog::on_getcode_btn_clicked() {
  auto email = ui->email_edit->text();
  QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
  bool match = regex.match(email).hasMatch();
  if (match) {
  } else {
    showTip(QString("email format error, please reinput"));
  }
}
