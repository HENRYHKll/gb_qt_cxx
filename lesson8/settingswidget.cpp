#include "settingswidget.h"
#include <QtWidgets>

SettingsWidget::SettingsWidget(QWidget *parent) : QDialog(parent) {
  QFileInfo fileInfo("Settings");

  tabWidget = new QTabWidget;
  tabWidget->addTab(new GeneralTab(), tr("General"));
  tabWidget->addTab(new FontTab(), tr("Font"));
  tabWidget->addTab(new KeyBoardTab(), tr("Keyboard"));

  //  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok

  //                                   | QDialogButtonBox::Cancel);

  //  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  //  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tabWidget);
  //  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(tr("Settings"));
}

GeneralTab::GeneralTab(QWidget *parent) : QWidget(parent) {

  QLabel *languageLabel = new QLabel(tr("language:"));
  QComboBox *setlanguage = new QComboBox();
  setlanguage->addItem("English");
  setlanguage->addItem("Belarus");

  QLabel *styleLabel = new QLabel(tr("Style:"));
  styleComboBox = new QComboBox;
  //  const QString defaultStyleName = QApplication::style()->objectName();
  QStringList styleNames /*= QStyleFactory::keys()*/;
  styleNames.append("default");
  styleNames.append("dark");
  styleComboBox->addItems(styleNames);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(languageLabel);
  mainLayout->addWidget(setlanguage);
  mainLayout->addWidget(styleLabel);
  mainLayout->addWidget(styleComboBox);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}

FontTab::FontTab(QWidget *parent) : QWidget(parent) {}
KeyBoardTab::KeyBoardTab(QWidget *parent) : QWidget(parent) {

  QGroupBox *setShortcutUserGroup = new QGroupBox(tr("Shortcut Users"));

  QLabel *KeySave = new QLabel(tr("Save:"));
  QLineEdit *KeySaveEdit = new QLineEdit();

  QLabel *KeyOpen = new QLabel(tr("Open:"));
  QLineEdit *KeyOpenEdit = new QLineEdit();

  QLabel *KeyNew = new QLabel(tr("New:"));
  QLineEdit *KeyNewEdit = new QLineEdit();

  QVBoxLayout *setShortcutUserLayout = new QVBoxLayout;
  setShortcutUserLayout->addWidget(KeySave);
  setShortcutUserLayout->addWidget(KeySaveEdit);
  setShortcutUserLayout->addWidget(KeyOpen);
  setShortcutUserLayout->addWidget(KeyOpenEdit);
  setShortcutUserLayout->addWidget(KeyNew);
  setShortcutUserLayout->addWidget(KeyNewEdit);
  setShortcutUserGroup->setLayout(setShortcutUserLayout);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(setShortcutUserGroup);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}
