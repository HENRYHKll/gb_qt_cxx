#ifndef TEXTEDITER_H
#define TEXTEDITER_H

#include <QDialog>

namespace Ui {
class TextEditer;
}

class TextEditer : public QDialog {
  Q_OBJECT

public:
  explicit TextEditer(QWidget *parent = nullptr);
  ~TextEditer();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

private:
  Ui::TextEditer *ui;
  QString buf;
};

#endif // TEXTEDITER_H
