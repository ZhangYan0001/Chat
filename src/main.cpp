#include <QApplication>
#include <QProcess>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QProcess *process = new QProcess;
  process->start("D:\\3project\\qt\\Chat\\env\\Scripts\\python.exe",
                 QStringList()
                     << "D:\\3project\\qt\\Chat\\py_script\\verify_code.py");
  MainWindow w;
  w.show();
  return a.exec();
}
