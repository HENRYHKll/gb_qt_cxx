#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include "settingswidget.h"
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QTextCodec>

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

  void on_actionView_triggered();

private:
  Ui::MainWindow *ui;
  Highlighter *m_highlighter;
  settingsWidget *settingsWindow;
  QString filter;
  QString hedersFile;
  const QString Name_Programm = "Blade Runner";
  void setTitleName(QString s);
  QFont font;
};
#endif // MAINWINDOW_H
