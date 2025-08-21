#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include <QTimer>

namespace Ui {
  class RegisterDialog;
}

class RegisterDialog : public QDialog {
  Q_OBJECT

public:
  explicit RegisterDialog(QWidget *parent = nullptr);

  ~RegisterDialog();

  void showTip(QString str);

private slots:
  void on_reg_btn_clicked();

  void on_back_btn_clicked();

  void on_getcode_btn_clicked();

private:
  Ui::RegisterDialog *ui;
  QTimer *tipTimer;
signals:
  void back_signal();
};

#endif  // REGISTERDIALOG_H
