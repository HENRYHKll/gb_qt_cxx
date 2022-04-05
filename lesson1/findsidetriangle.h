#ifndef FINDSIDETRIANGLE_H
#define FINDSIDETRIANGLE_H

#include <QDialog>

namespace Ui {
class FindSideTriangle;
}

class FindSideTriangle : public QDialog {
  Q_OBJECT

public:
  explicit FindSideTriangle(QWidget *parent = nullptr);
  ~FindSideTriangle();

private slots:
  void on_pushButton_clicked();

private:
  Ui::FindSideTriangle *ui;
};

#endif // FINDSIDETRIANGLE_H
