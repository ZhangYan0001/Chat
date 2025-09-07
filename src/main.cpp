#include <QApplication>
#include <QProcess>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QProcess *process= new QProcess(&a);
  process->start("D:\\3project\\qt\\Chat\\env\\Scripts\\python.exe",
                 QStringList()
                     << "D:\\3project\\qt\\Chat\\py_script\\verify_code.py");
  MainWindow w;
  w.show();
  QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
    if (process->state() == QProcess::Running) {
      process->terminate();
      if (!process->waitForFinished(3000)) {  // 等待 3 秒
        process->kill();                      // 如果没关掉，就强制杀死
      }
    }
  });
  return a.exec();
}
