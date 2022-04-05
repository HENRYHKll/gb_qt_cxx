#include "squareequationwindow.h"
#include "ui_squareequationwindow.h"
#include <QRegExpValidator>
#include <QtCore/qmath.h>

SquareEquationWindow::SquareEquationWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SquareEquationWindow) {
  ui->setupUi(this);
  QRegExp exp("[-]{0,1}[0-9]{1,10}[.]{0,1}[0-9]{0,10}");
  ui->aQe->setValidator(new QRegExpValidator(exp, this));
  ui->bQe->setValidator(new QRegExpValidator(exp, this));
  ui->cQe->setValidator(new QRegExpValidator(exp, this));
}

SquareEquationWindow::~SquareEquationWindow() { delete ui; }

void SquareEquationWindow::on_pushButton_clicked() {

  QString a_string = ui->aQe->text();
  QString b_string = ui->bQe->text();
  QString c_string = ui->cQe->text();
  QString res_string;
  if (a_string.toDouble() == 0) {
    if (b_string.toDouble() != 0) {
      res_string.append("X=");
      double res_double = -c_string.toDouble() / b_string.toDouble();
      QString res_temp;
      res_temp.setNum(res_double);
      ui->result->setText(res_string + res_temp);
    } else {
      res_string.append("No roots");
      ui->result->setText(res_string);
    }
  } else {
    double dis(qPow(b_string.toDouble(), 2) -
               4 * a_string.toDouble() * c_string.toDouble());
    if (dis >= 0) {
      QString res_temp;
      double root1 =
          (-b_string.toDouble() + qSqrt(dis)) / (2 * a_string.toDouble());
      double root2 =
          (-b_string.toDouble() - qSqrt(dis)) / (2 * a_string.toDouble());
      if (dis == 0) {
        res_string.append("X=");
        res_temp.setNum(root1);
        res_string += res_temp;
        ui->result->setText(res_string);
      } else if (dis > 0) {
        res_string.append("X1=");
        res_temp.setNum(root1);
        res_string += res_temp;
        res_string.append(" X2=");
        res_temp.setNum(root2);
        res_string += res_temp;
        ui->result->setText(res_string);
      } else {
        res_string.append("No roots");
        ui->result->setText(res_string);
      }

    } else {
      res_string.append("No roots");
      ui->result->setText(res_string);
    }
  }
}
