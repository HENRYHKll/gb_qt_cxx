#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelectionModel>
#include <QListView>
#include <QMainWindow>
#include <QMap>
#include <QStandardItemModel>
#include <checkerlocalpc.h>
#include <functional>
#include <tuple>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  QString icon_detec(QString &lang_name);

private slots:
  void listAndIcon(bool checked);
  void buttonUp();
  void buttonDown();
  void buttonDel();
  void buttonAdd();
  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
  QStandardItemModel model;
  QItemSelectionModel sel_model;
  CheckerLocalPC *task3;
  QMap<QString, QString> icon_map{
      {"c++", ":/_/icon/cpp.png"},       {"c", ":/_/icon/c.png"},
      {"python", ":/_/icon/python.png"}, {"js", ":/_/icon/js.png"},
      {"php", ":/_/icon/php.png"},
  };
};
#endif // MAINWINDOW_H
