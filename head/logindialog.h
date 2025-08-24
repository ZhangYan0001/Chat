#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog();
 signals:
  void login_reg_signal();
 private slots:
  void on_login_btn_clicked();

  void on_forgetpwd_label_linkActivated(const QString &link);

 private:
  Ui::LoginDialog *ui;
};

#endif  // LOGINDIALOG_H
