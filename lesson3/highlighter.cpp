#include "weatherdiolog.h"
#include "ui_weatherdiolog.h"

weatherDiolog::weatherDiolog(QWidget *parent)
    : QDialog(parent), ui(new Ui::weatherDiolog) {
  ui->setupUi(this);
}

weatherDiolog::~weatherDiolog() { delete ui; }
