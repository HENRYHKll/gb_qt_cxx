#ifndef CHECKERLOCALPC_H
#define CHECKERLOCALPC_H

#include <QDialog>
#include <QItemSelectionModel>
#include <QStandardItemModel>

namespace Ui {
class CheckerLocalPC;
}

class CheckerLocalPC : public QDialog {
  Q_OBJECT

public:
  explicit CheckerLocalPC(QWidget *parent = nullptr);
  ~CheckerLocalPC();

private slots:
  void onApplyColorButton();
  void onCheckAdressButton();

private:
  Ui::CheckerLocalPC *ui;
  QStandardItemModel model;
  QItemSelectionModel sel_model;
};

#endif // CHECKERLOCALPC_H
