#include "checkerlocalpc.h"
#include "ui_checkerlocalpc.h"

CheckerLocalPC::CheckerLocalPC(QWidget *parent)
    : QDialog(parent), ui(new Ui::CheckerLocalPC), model(0, 3) {
  ui->setupUi(this);

  setWindowTitle("PC IP AND MAC");
  sel_model.setModel(&model);

  ui->tableView->setModel(&model);
  ui->tableView->setSelectionModel(&sel_model);
  ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  model.setHeaderData(0, Qt::Horizontal, "Name");
  model.setHeaderData(1, Qt::Horizontal, "IP address");
  model.setHeaderData(2, Qt::Horizontal, "MAC address");

  model.appendRow({new QStandardItem("My PC"),
                   new QStandardItem("192.168.0.23"),
                   new QStandardItem("01-00-5e-00-00-fc")});
  model.appendRow({new QStandardItem("Router"),
                   new QStandardItem("192.168.0.1"),
                   new QStandardItem("01-21-5e-20-00-fc")});
  model.appendRow({new QStandardItem("IPhone"),
                   new QStandardItem("192.168.0.100"),
                   new QStandardItem("01-55-53-ff-ff-fc")});
  connect(ui->pushButton, &QPushButton::clicked, this,
          &CheckerLocalPC::onApplyColorButton);
  connect(ui->pushButtonCheckAdr, &QPushButton::clicked, this,
          &CheckerLocalPC::onCheckAdressButton);
}

CheckerLocalPC::~CheckerLocalPC() { delete ui; }

void CheckerLocalPC::onApplyColorButton() {
  if (sel_model.hasSelection()) {
    auto sel_rows = sel_model.selectedRows();
    for (const auto &index : sel_rows) {
      for (int i = 0; i < 3; ++i) {
        auto item = model.item(index.row(), i);
        item->setData(QColor(Qt::gray), Qt::BackgroundColorRole);
      }
    }
  }
}
void CheckerLocalPC::onCheckAdressButton() {}
