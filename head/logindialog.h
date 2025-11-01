#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTimer>
#include <memory>
#include <QMovie>

#include "global.h"
#include "httpmgr.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LoginDialog(QWidget *parent = nullptr);
  void restoreLoginBtn();
  ~LoginDialog();
 signals:
  void login_reg_signal();
  void login_success_signal(QString email, QString token);
 private slots:
  void on_login_btn_clicked();

  void on_forgetpwd_label_linkActivated(const QString &link);

  // void login_mod_finish_slot(ReqId id, QString res, ErrorCodes err);
  void login_mod_finish_slot(ReqId id, HttpResponse rep, Modules mod);

 private:
  Ui::LoginDialog *ui;
  std::unique_ptr<UserLoginInfo> loginInfo;
  QMovie *loadingMovie = nullptr;
};

#endif  // LOGINDIALOG_H
