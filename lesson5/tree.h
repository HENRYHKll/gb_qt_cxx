#ifndef TREE_H
#define TREE_H

#include <QApplication>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>
Q_PROPERTY(QStandardItemModel *model READ getCurrentModel WRITE setNewModel)
class Tree : public QWidget {
  Q_OBJECT
public:
  explicit Tree(QWidget *parent = nullptr);
  void clearTree();
  QStandardItemModel *getCurrentModel() const { return model; }
  void setNewModel(QStandardItemModel *newmodel);
  void rebuildModel(QString str);

private:
  QGridLayout *gridLay;
  QTreeView *tree;
  QPushButton *mainPath;
  QComboBox *disckSelBox;
private slots:
  void chgDisk(int index); // получаем индекс выбранного диска
  void goMainPath(); // Для UNIX-подобных ОС верхним уровнем является
  // путь /
private:
  QStandardItemModel *model;
  QString curretnPath;

protected:
};
#endif // TREE_H
