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