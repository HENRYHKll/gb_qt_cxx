#ifndef SQUAREEQUATIONWINDOW_H
#define SQUAREEQUATIONWINDOW_H

#include <QDialog>

namespace Ui {
class SquareEquationWindow;
}

class SquareEquationWindow : public QDialog {
  Q_OBJECT

public:
  explicit SquareEquationWindow(QWidget *parent = nullptr);
  ~SquareEquationWindow();

private slots:
  void on_pushButton_clicked();

private:
  Ui::SquareEquationWindow *ui;
};

#endif // SQUAREEQUATIONWINDOW_H
