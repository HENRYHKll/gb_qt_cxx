#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("Lesson2");

  sel_model.setModel(&model);

  ui->listView->setModel(&model);
  ui->listView->setSelectionModel(&sel_model);
  ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

  model.appendRow(new QStandardItem(QIcon{":/_/icon/cpp.png"}, "c++"));
  model.appendRow(new QStandardItem(QIcon{":/_/icon/c.png"}, "c"));
  model.appendRow(new QStandardItem(QIcon{":/_/icon/python.png"}, "python"));

  connect(ui->CheckBoxListAndIcon, &QCheckBox::clicked, this,
          &MainWindow::listAndIcon);
  connect(ui->pushButtonUp, &QCheckBox::clicked, this, &MainWindow::buttonUp);
  connect(ui->pushButtonDel, &QCheckBox::clicked, this, &MainWindow::buttonDel);
  connect(ui->pushButtonDown, &QCheckBox::clicked, this,
          &MainWindow::buttonDown);
  connect(ui->pushButtonAdd, &QCheckBox::clicked, this, &MainWindow::buttonAdd);
}
QString MainWindow::icon_detec(QString &lang_name) {

  QMap<QString, QString>::const_iterator it;
  for (it = icon_map.begin(); it != icon_map.end(); it++) {
    if (it.key() == lang_name)
      return it.value();
  }
  return ":/_/icon/information.png";
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::listAndIcon(bool checked) {
  if (checked) {
    ui->listView->setViewMode(QListView::IconMode);
  } else {
    ui->listView->setViewMode(QListView::ListMode);
  }
}

void MainWindow::buttonUp() {
  if (sel_model.hasSelection()) {
    auto curr_index = sel_model.currentIndex();
    if (curr_index.isValid()) {
      auto row = curr_index.row();
      if (row > 0) {
        auto item = model.takeRow(curr_index.row());
        model.insertRow(--row, item);
      }
      ui->listView->setCurrentIndex(model.index(row, 0));
    }
  }
}
void MainWindow::buttonDown() {
  if (sel_model.hasSelection()) {
    auto curr_index = sel_model.currentIndex();
    if (curr_index.isValid()) {
      auto row = curr_index.row();
      if (row < model.rowCount() - 1) {
        auto item = model.takeRow(curr_index.row());
        model.insertRow(++row, item);
      }
      ui->listView->setCurrentIndex(model.index(row, 0));
    }
  }
}
void MainWindow::buttonDel() {
  if (sel_model.hasSelection()) {
    auto curr_index = sel_model.currentIndex();
    if (curr_index.isValid()) {
      model.removeRow(curr_index.row());
    }
  }
}
void MainWindow::buttonAdd() {
  if (!ui->lineEdit->text().isEmpty()) {
    QString lang_name = ui->lineEdit->text(), src_icon = icon_detec(lang_name);
    model.appendRow(new QStandardItem(QIcon{src_icon}, lang_name));
  }
}

void MainWindow::on_pushButton_clicked() {
  close();
  task3 = new CheckerLocalPC(this);
  task3->show();
}
