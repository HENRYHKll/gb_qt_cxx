#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QTextCodec>
#include <highlighter.h>

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

private slots:
  void on_actionOpen_clicked();
  void on_actionSave_clicked();
  void on_actionHelp_clicked();
  void on_actionSettings_clicked();
  void on_actionNew_triggered();

private:
  Ui::MainWindow *ui;
  Highlighter *m_highlighter;
  QString filter;
  QString hedersFile;
  const QString Name_Programm = "Blade Runner";
  void setTitleName(QString s);
  QFont font;
  // bool style_wdget = true;
};
#endif // MAINWINDOW_H
