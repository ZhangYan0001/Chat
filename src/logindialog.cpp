#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    connect(ui->reg_btn, &QPushButton::clicked, this, [=](){
        emit login_reg_signal();
    });

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_login_btn_clicked()
{

}


void LoginDialog::on_forgetpwd_label_linkActivated(const QString &link)
{
    // 2. 检查传递过来的 link 是否是我们期望的，确保点击的是正确的链接
    if (link == "forget_password") {

        qDebug() << "Forget password link clicked!"; // 打印调试信息
    }
}

