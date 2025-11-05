#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include <QTimer>
#include <memory>
#include <QMap>

#include "global.h"
#include "httpmgr.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RegisterDialog(QWidget *parent = nullptr);

  ~RegisterDialog();

  void showTip(const QString &text, const QString &type);
  void reset();
  void initValidators();
  void isRegValidSignal();

 private slots:
  void on_reg_btn_clicked();

  void on_back_btn_clicked();

  void on_getcode_btn_clicked();

  void register_finish_slot(const HttpResponse &rep);
  void verify_finish_slot(const HttpResponse &rep);

 private:
  Ui::RegisterDialog *ui;
  QTimer *tipTimer;
  std::unique_ptr<UserRegisterInfo> regInfo;
  bool regValid = false; 
  bool email_valid = false;
  bool pwd_valid = false;
  bool confimr_valid = false;
  bool code_valid = false;
  bool isCountingDown = false;
 signals:
  void back_signal();
  void get_code_signal();
};

#endif  // REGISTERDIALOG_H
