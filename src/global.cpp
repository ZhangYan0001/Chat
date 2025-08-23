#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w) {
  w->style()->unpolish(w);
  w->style()->polish(w);
};


void loadStyleSheet(const QString& qssFile) {
  QFile file(qssFile);
  if (file.open(QFile::ReadOnly)) {
    QString style = QString::fromUtf8(file.readAll());
    qApp->setStyleSheet(style);
    file.close();
  }
}

bool validatePassword(const QString& pwd, QString& errMsg) {
  // 1. 长度校验
  if (pwd.length() < 8) {
    errMsg = "密码至少需要 8 位";
    return false;
  }

  // 2. 正则校验：至少包含字母和数字，可以包含符号
  static const QRegularExpression re(R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?`~]{8,}$)");

  if (!re.match(pwd).hasMatch()) {
    errMsg = "密码必须包含字母和数字，可以包含符号";
    return false;
  }

  return true;  // 校验通过
}
