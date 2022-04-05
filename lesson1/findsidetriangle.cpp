#include "findsidetriangle.h"
#include "ui_findsidetriangle.h"
#include <QRegExpValidator>
#include <QtCore/qmath.h>

FindSideTriangle::FindSideTriangle(QWidget *parent)
    : QDialog(parent), ui(new Ui::FindSideTriangle) {
  ui->setupUi(this);
  QRegExp exp("[-]{0,1}[0-9]{1,10}[.]{0,1}[0-9]{0,10}");
  ui->b_side->setValidator(new QRegExpValidator(exp, this));
  ui->c_side->setValidator(new QRegExpValidator(exp, this));
  ui->al_side->setValidator(new QRegExpValidator(exp, this));
}

FindSideTriangle::~FindSideTriangle() { delete ui; }

void FindSideTriangle::on_pushButton_clicked() {
  QString b_string = ui->b_side->text();
  QString c_string = ui->c_side->text();
  QString al_string = ui->al_side->text();
  double al_double = al_string.toDouble(), result_double,
         b(b_string.toDouble()), c(c_string.toDouble());

  if (ui->radioRadians->isChecked()) {
    al_double = (al_double * 180) / 3.14;
  }
  result_double = qSqrt(qPow(b, 2) + qPow(c, 2) - 2 * b * c * qCos(al_double));
  QString result_str;
  ui->ladel_result->setText(result_str.setNum(result_double));
}
