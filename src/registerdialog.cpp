#include "registerdialog.h"

#include "global.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
  ui->setupUi(this);
  ui->pwd_edid->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->back_btn->setIcon(QIcon(":/images/previous.png"));
  ui->login_btn->setDisabled(true);
  ui->reg_btn->setDisabled(true);

  tipTimer = new QTimer(this);
  connect(tipTimer, &QTimer::timeout, this, [this]() {
    ui->err_label->clear();
    repolish(ui->err_label);
  });

}

RegisterDialog::~RegisterDialog() { delete ui; }
void RegisterDialog::showTip(QString str) {
  ui->err_label->setText(str);
  repolish(ui->err_label);

  tipTimer->stop();
  tipTimer->start(4000);
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
