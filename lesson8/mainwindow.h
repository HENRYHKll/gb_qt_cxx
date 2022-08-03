#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "figure.h"
#include "settingswidget.h"
#include <QMainWindow>
#include <QPlainTextEdit>

class MidChildTxt;
QT_BEGIN_NAMESPACE
class QFontComboBox;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool openFile(const QString &fileName);

protected:
  void closeEvent(QCloseEvent *event) override;
public slots:
  void setThemeColor(QString themeColor);
private slots:
  void newFile();
  void open();
  void save();
  void saveAs();
  void updateRecentFileActions();
  void openRecentFile();
#ifndef QT_NO_CLIPBOARD
  void cut();
  void copy();
  void paste();
#endif
  void about();
  void updateMenus();
  void updateWindowMenu();
  MidChildTxt *createMdiChild();
  void switchLayoutDirection();
  void openSettings();
  void openFigureWindow();
  void toPrintDoc();

private:
  Ui::MainWindow *ui;

  enum { MaxRecentFiles = 5 };

  void createActions();
  void createStatusBar();
  void readSettings();
  void writeSettings();
  bool loadFile(const QString &fileName);
  static bool hasRecentFiles();
  void prependToRecentFiles(const QString &fileName);
  void setRecentFilesVisible(bool visible);
  MidChildTxt *activeMdiChild() const;
  QMdiSubWindow *findMdiChild(const QString &fileName) const;

  QMdiArea *mdiArea;

  QMenu *windowMenu;
  QMenu *toolsMenu;
  QAction *settingsAct;
  QAction *newAct;
  QAction *saveAct;
  QAction *printAct;
  QAction *saveAsAct;
  QAction *recentFileActs[MaxRecentFiles];
  QAction *recentFileSeparator;
  QAction *recentFileSubMenuAct;
#ifndef QT_NO_CLIPBOARD
  QAction *cutAct;
  QAction *copyAct;
  QAction *pasteAct;
#endif
//----------------------
  QAction *actionTextBold;
  QAction *actionTextItalic;
  QAction *actionTextUnderline;
  QAction *actionAlignLeft;
  QAction *actionAlignCenter;
  QAction *actionAlignRight;
  QAction *actionAlignJustify;
  QAction *actionDateAndTime;
  void textBold();
  void textUnderline();
  void textItalic();
  void textDateAndTime();
  void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
  void textAlign(QAction *a);
  QComboBox *comboStyle;
  void textStyle(int styleIndex);
  QFontComboBox *comboFont;
  void textFamily(const QString &f);
  QComboBox *comboSize;
  void textSize(const QString &p);
  QAction *actionFigureWindow;
//----------------------
  QAction *closeAct;
  QAction *closeAllAct;
  QAction *tileAct;
  QAction *cascadeAct;
  QAction *nextAct;
  QAction *previousAct;
  QAction *windowMenuSeparatorAct;
  SettingsWidget *settingsWindows = nullptr;
  FigureWindow * figureWindow =nullptr;
  const QString nameProgramm = "Blade Runner";
  QPlainTextEdit *printDoc;
  //  QString themeColor = "default";
  QPalette *darkPalette = nullptr;
  QPalette *defaultPalette = nullptr;
  void setDefaultTheme();
  void setDarkTheme();
  void setupTextActions();
};
#endif // MAINWINDOW_H
