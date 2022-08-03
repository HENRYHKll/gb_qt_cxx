#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setApplicationName("Blade Runner");
  QCoreApplication::setOrganizationName("IP IVANOV :)");
  qApp->setStyle(QStyleFactory::create("Fusion"));
  //  qDebug() << QStyleFactory::keys();
  MainWindow w;
  w.show();
  return a.exec();
}
