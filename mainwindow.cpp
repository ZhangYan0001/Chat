#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this);
    _register_dlg = new RegisterDialog(this);
    setCentralWidget(_login_dlg);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_login_dlg, &LoginDialog::login_reg_signal, this, [=](){
        _login_dlg->hide();
        setCentralWidget(_register_dlg);
    });
    _register_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _register_dlg->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
