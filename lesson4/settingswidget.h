#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

//! [0]
class GeneralTab : public QWidget {
  Q_OBJECT

public:
  explicit GeneralTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [0]

//! [1]
class PermissionsTab : public QWidget {
  Q_OBJECT

public:
  explicit PermissionsTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [1]

//! [2]
class ApplicationsTab : public QWidget {
  Q_OBJECT

public:
  explicit ApplicationsTab(const QFileInfo &fileInfo,
                           QWidget *parent = nullptr);
};
//! [2]

//! [3]
class settingsWidget : public QDialog {
  Q_OBJECT

public:
  explicit settingsWidget(QWidget *parent = nullptr);

private:
  QTabWidget *tabWidget;
  QDialogButtonBox *buttonBox;
};
//! [3]

#endif
