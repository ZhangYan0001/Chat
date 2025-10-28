#ifndef GLOBAL_H
#define GLOBAL_H

#include <QApplication>
#include <QFile>
#include <QRegularExpression>
#include <QWidget>
#include <functional>

#include "QStyle"

enum ReqId {
  ID_GET_VARIFY_CODE = 1001,
  ID_REG_USER = 1002,
  ID_LOGIN_USER = 2001,
};

enum ErrorCodes {
  SUCCESS = 0,
  ERROR_JSON = 1,
  ERROR_NETWORK = 2,
  ERROR_VERIFY_CODE = 3,
  ERROR_NULL = 4,
};

enum Modules {
  REGISTERMOD = 0,
  LOGIN = 1,
};

struct UserLoginInfo {
  QString _email;
  QString _pwd;
};

struct UserRegisterInfo {
  QString _user;
  QString _getcode;
  QString _email;
  QString _pwd;
};


// 邮箱正则
static const QRegularExpression emailRegex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");

// 密码正则：必须包含字母和数字，不少于 8 位，可以包含符号
static const QRegularExpression pwdRegex(
        R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?`~]{8,}$)");

extern std::function<void(QWidget*)> repolish;
void loadStyleSheet(const QString& qssFile);
bool validatePassword(const QString& pwd, QString& errMsg);
#endif  // GLOBAL_H
