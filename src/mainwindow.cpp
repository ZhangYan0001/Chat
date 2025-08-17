#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  _login_dlg = new LoginDialog(this);
  _register_dlg = new RegisterDialog(this);

  _stacked_widget = new QStackedWidget(this);
  _stacked_widget->addWidget(_login_dlg);
  _stacked_widget->addWidget(_register_dlg);
  setCentralWidget(_stacked_widget);
  _stacked_widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

  connect(_login_dlg, &LoginDialog::login_reg_signal, this, [=]() {
    _stacked_widget->setCurrentWidget(_register_dlg);
  });

  connect(_register_dlg, &RegisterDialog::back_signal, this, [=]() {
    _stacked_widget->setCurrentWidget(_login_dlg);
  });
}

MainWindow::~MainWindow() {
  delete ui;
}
