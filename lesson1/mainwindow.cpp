#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/qmath.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  hide();
  sewindow = new SquareEquationWindow(this);
  sewindow->show();
}

void MainWindow::on_pushButton_triangle_clicked() {
  hide();
  fstwindow = new FindSideTriangle(this);
  fstwindow->show();
}

void MainWindow::on_pushButton_texteditor_clicked() {
  hide();
  tewindow = new TextEditer(this);
  tewindow->show();
}
