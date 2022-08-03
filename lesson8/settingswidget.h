#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QComboBox;
QT_END_NAMESPACE

class GeneralTab : public QWidget {
  Q_OBJECT

public:
  explicit GeneralTab(QWidget *parent = nullptr);

private slots:
  //  void changeStyle(const QString &styleName);

public:
  QComboBox *styleComboBox;
};
class FontTab : public QWidget {
  Q_OBJECT

public:
  explicit FontTab(QWidget *parent = nullptr);
};
class KeyBoardTab : public QWidget {
  Q_OBJECT

public:
  explicit KeyBoardTab(QWidget *parent = nullptr);
};

class SettingsWidget : public QDialog {
  Q_OBJECT

public:
  explicit SettingsWidget(QWidget *parent = nullptr);

  // signals:
  //  void signalEditStyle(QString);

public:
  QTabWidget *tabWidget;
  //  QDialogButtonBox *buttonBox;
};

#endif // SETTINGSWIDGET_H
