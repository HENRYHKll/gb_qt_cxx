#include "mainwindow.h"
#include "midchildtxt.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include <QPrinter>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : mdiArea(new QMdiArea)
//    , QMainWindow(parent)
//    , ui(new Ui::MainWindow)
{
  mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setCentralWidget(mdiArea);
  connect(mdiArea, &QMdiArea::subWindowActivated, this,
          &MainWindow::updateMenus);
  setWindowIcon(QIcon(":/file/icon/icon-programm.svg"));
  createActions();
  createStatusBar();
  updateMenus();
  setupTextActions();
  readSettings();

  setWindowTitle(nameProgramm);
  setUnifiedTitleAndToolBarOnMac(true);

  //  connect(settingsWindows, &QComboBox::currentTextChanged, this,
  //          &MainWindow::setThemeColor);

  //  ui->setupUi(this);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  mdiArea->closeAllSubWindows();
  if (mdiArea->currentSubWindow()) {
    event->ignore();
  } else {
    writeSettings();
    event->accept();
  }
}
void MainWindow::newFile() {
  MidChildTxt *child = createMdiChild();
  child->newFile();
  child->show();
}

void MainWindow::open() {
  const QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty())
    openFile(fileName);
}

bool MainWindow::openFile(const QString &fileName) {
  if (QMdiSubWindow *existing = findMdiChild(fileName)) {
    mdiArea->setActiveSubWindow(existing);
    return true;
  }
  const bool succeeded = loadFile(fileName);
  if (succeeded)
    statusBar()->showMessage(tr("File loaded"), 2000);
  return succeeded;
}

bool MainWindow::loadFile(const QString &fileName) {
  MidChildTxt *child = createMdiChild();
  const bool succeeded = child->loadFile(fileName);
  if (succeeded)
    child->show();
  else
    child->close();
  MainWindow::prependToRecentFiles(fileName);
  return succeeded;
}

static inline QString recentFilesKey() {
  return QStringLiteral("recentFileList");
}
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings) {
  QStringList result;
  const int count = settings.beginReadArray(recentFilesKey());
  for (int i = 0; i < count; ++i) {
    settings.setArrayIndex(i);
    result.append(settings.value(fileKey()).toString());
  }
  settings.endArray();
  return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings) {
  const int count = files.size();
  settings.beginWriteArray(recentFilesKey());
  for (int i = 0; i < count; ++i) {
    settings.setArrayIndex(i);
    settings.setValue(fileKey(), files.at(i));
  }
  settings.endArray();
}

bool MainWindow::hasRecentFiles() {
  QSettings settings(QCoreApplication::organizationName(),
                     QCoreApplication::applicationName());
  const int count = settings.beginReadArray(recentFilesKey());
  settings.endArray();
  return count > 0;
}

void MainWindow::prependToRecentFiles(const QString &fileName) {
  QSettings settings(QCoreApplication::organizationName(),
                     QCoreApplication::applicationName());

  const QStringList oldRecentFiles = readRecentFiles(settings);
  QStringList recentFiles = oldRecentFiles;
  recentFiles.removeAll(fileName);
  recentFiles.prepend(fileName);
  if (oldRecentFiles != recentFiles)
    writeRecentFiles(recentFiles, settings);

  setRecentFilesVisible(!recentFiles.isEmpty());
}

void MainWindow::setRecentFilesVisible(bool visible) {
  recentFileSubMenuAct->setVisible(visible);
  recentFileSeparator->setVisible(visible);
}

void MainWindow::updateRecentFileActions() {
  QSettings settings(QCoreApplication::organizationName(),
                     QCoreApplication::applicationName());

  const QStringList recentFiles = readRecentFiles(settings);
  const int count = qMin(int(MaxRecentFiles), recentFiles.size());
  int i = 0;
  for (; i < count; ++i) {
    const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
    recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
    recentFileActs[i]->setData(recentFiles.at(i));
    recentFileActs[i]->setVisible(true);
  }
  for (; i < MaxRecentFiles; ++i)
    recentFileActs[i]->setVisible(false);
}

void MainWindow::openRecentFile() {
  if (const QAction *action = qobject_cast<const QAction *>(sender()))
    openFile(action->data().toString());
}

void MainWindow::save() {
  if (activeMdiChild() && activeMdiChild()->save())
    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs() {
  MidChildTxt *child = activeMdiChild();
  if (child && child->saveAs()) {
    statusBar()->showMessage(tr("File saved"), 2000);
    MainWindow::prependToRecentFiles(child->currentFile());
  }
}

#ifndef QT_NO_CLIPBOARD
void MainWindow::cut() {
  if (activeMdiChild())
    activeMdiChild()->cut();
}

void MainWindow::copy() {
  if (activeMdiChild())
    activeMdiChild()->copy();
}

void MainWindow::paste() {
  if (activeMdiChild())
    activeMdiChild()->paste();
}
#endif

void MainWindow::about() {
  QMessageBox::about(
      this, tr("About ") + nameProgramm,
      tr("The <b>Blade Runner</b> this is a program on qt "
         "project of online courses geekbrains.  <h3>version: 1.0 alfa</h3>"));
}

void MainWindow::updateMenus() {
  bool hasMdiChild = (activeMdiChild() != nullptr);
  saveAct->setEnabled(hasMdiChild);
  saveAsAct->setEnabled(hasMdiChild);
#ifndef QT_NO_CLIPBOARD
  pasteAct->setEnabled(hasMdiChild);
#endif
  closeAct->setEnabled(hasMdiChild);
  closeAllAct->setEnabled(hasMdiChild);
  tileAct->setEnabled(hasMdiChild);
  cascadeAct->setEnabled(hasMdiChild);
  nextAct->setEnabled(hasMdiChild);
  previousAct->setEnabled(hasMdiChild);
  windowMenuSeparatorAct->setVisible(hasMdiChild);

#ifndef QT_NO_CLIPBOARD
  bool hasSelection =
      (activeMdiChild() && activeMdiChild()->textCursor().hasSelection());
  cutAct->setEnabled(hasSelection);
  copyAct->setEnabled(hasSelection);
#endif
}

void MainWindow::updateWindowMenu() {
  windowMenu->clear();
  windowMenu->addAction(closeAct);
  windowMenu->addAction(closeAllAct);
  windowMenu->addSeparator();
  windowMenu->addAction(tileAct);
  windowMenu->addAction(cascadeAct);
  windowMenu->addSeparator();
  windowMenu->addAction(nextAct);
  windowMenu->addAction(previousAct);
  windowMenu->addAction(windowMenuSeparatorAct);

  QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
  windowMenuSeparatorAct->setVisible(!windows.isEmpty());

  for (int i = 0; i < windows.size(); ++i) {
    QMdiSubWindow *mdiSubWindow = windows.at(i);
    MidChildTxt *child = qobject_cast<MidChildTxt *>(mdiSubWindow->widget());

    QString text;
    if (i < 9) {
      text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
    } else {
      text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
    }
    QAction *action =
        windowMenu->addAction(text, mdiSubWindow, [this, mdiSubWindow]() {
          mdiArea->setActiveSubWindow(mdiSubWindow);
        });
    action->setCheckable(true);
    action->setChecked(child == activeMdiChild());
  }
}

MidChildTxt *MainWindow::createMdiChild() {
  MidChildTxt *child = new MidChildTxt;
  mdiArea->addSubWindow(child);

#ifndef QT_NO_CLIPBOARD
  connect(child, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
  connect(child, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif

  return child;
}

void MainWindow::createActions() {
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  QToolBar *fileToolBar = addToolBar(tr("File"));

  const QIcon newIcon =
      QIcon::fromTheme("document-new", QIcon(":/file/icon/file.svg"));
  newAct = new QAction(newIcon, tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
  fileMenu->addAction(newAct);
  fileToolBar->addAction(newAct);

  const QIcon openIcon =
      QIcon::fromTheme("document-open", QIcon(":/file/icon/open-file.svg"));
  QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, &QAction::triggered, this, &MainWindow::open);
  fileMenu->addAction(openAct);
  fileToolBar->addAction(openAct);

  const QIcon saveIcon =
      QIcon::fromTheme("document-save", QIcon(":/file/icon/save.svg"));
  saveAct = new QAction(saveIcon, tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);
  fileToolBar->addAction(saveAct);

  const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
  saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
  fileMenu->addAction(saveAsAct);

  const QIcon printIcon =
      QIcon::fromTheme("document-print", QIcon(":/file/icon/print.svg"));
  printAct = new QAction(printIcon, tr("Print"), this);
  printAct->setShortcuts(QKeySequence::Print);
  printAct->setStatusTip(tr("Print the document"));
  connect(printAct, &QAction::triggered, this, &MainWindow::toPrintDoc);
  fileToolBar->addAction(printAct);
  fileMenu->addAction(printAct);
  fileMenu->addSeparator();

  QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
  connect(recentMenu, &QMenu::aboutToShow, this,
          &MainWindow::updateRecentFileActions);
  recentFileSubMenuAct = recentMenu->menuAction();

  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileActs[i] =
        recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
    recentFileActs[i]->setVisible(false);
  }

  recentFileSeparator = fileMenu->addSeparator();

  setRecentFilesVisible(MainWindow::hasRecentFiles());

  fileMenu->addAction(tr("Switch layout direction"), this,
                      &MainWindow::switchLayoutDirection);

  fileMenu->addSeparator();
  const QIcon exitIcon = QIcon::fromTheme("application-exit");
  QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp,
                                         &QApplication::closeAllWindows);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  fileMenu->addAction(exitAct);

#ifndef QT_NO_CLIPBOARD
  QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
  QToolBar *editToolBar = addToolBar(tr("Edit"));

  const QIcon cutIcon =
      QIcon::fromTheme("edit-cut", QIcon(":/file/icon/cut.svg"));
  cutAct = new QAction(cutIcon, tr("Cu&t"), this);
  cutAct->setShortcuts(QKeySequence::Cut);
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                          "clipboard"));
  connect(cutAct, &QAction::triggered, this, &MainWindow::cut);
  editMenu->addAction(cutAct);
  editToolBar->addAction(cutAct);

  const QIcon copyIcon =
      QIcon::fromTheme("edit-copy", QIcon(":/file/icon/copy.svg"));
  copyAct = new QAction(copyIcon, tr("&Copy"), this);
  copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(copyAct, &QAction::triggered, this, &MainWindow::copy);
  editMenu->addAction(copyAct);
  editToolBar->addAction(copyAct);

  const QIcon pasteIcon =
      QIcon::fromTheme("edit-paste", QIcon(":/file/icon/paste.svg"));
  pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
  pasteAct->setShortcuts(QKeySequence::Paste);
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                            "selection"));
  connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);
  editMenu->addAction(pasteAct);
  editToolBar->addAction(pasteAct);
#endif

  windowMenu = menuBar()->addMenu(tr("&Window"));
  connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

  closeAct = new QAction(tr("Cl&ose"), this);
  closeAct->setStatusTip(tr("Close the active window"));
  connect(closeAct, &QAction::triggered, mdiArea,
          &QMdiArea::closeActiveSubWindow);

  closeAllAct = new QAction(tr("Close &All"), this);
  closeAllAct->setStatusTip(tr("Close all the windows"));
  connect(closeAllAct, &QAction::triggered, mdiArea,
          &QMdiArea::closeAllSubWindows);

  tileAct = new QAction(tr("Tile"), this);
  tileAct->setStatusTip(tr("Tile the windows"));
  connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

  cascadeAct = new QAction(tr("Cascade"), this);
  cascadeAct->setStatusTip(tr("Cascade the windows"));
  connect(cascadeAct, &QAction::triggered, mdiArea,
          &QMdiArea::cascadeSubWindows);

  nextAct = new QAction(tr("Next"), this);
  nextAct->setShortcuts(QKeySequence::NextChild);
  nextAct->setStatusTip(tr("Move the focus to the next window"));
  connect(nextAct, &QAction::triggered, mdiArea,
          &QMdiArea::activateNextSubWindow);

  previousAct = new QAction(tr("Pre&vious"), this);
  previousAct->setShortcuts(QKeySequence::PreviousChild);
  previousAct->setStatusTip(tr("Move the focus to the previous "
                               "window"));
  connect(previousAct, &QAction::triggered, mdiArea,
          &QMdiArea::activatePreviousSubWindow);

  windowMenuSeparatorAct = new QAction(this);
  windowMenuSeparatorAct->setSeparator(true);

  updateWindowMenu();

  menuBar()->addSeparator();
  // settings
  toolsMenu = menuBar()->addMenu(tr("Tools"));
  settingsAct = new QAction(tr("Settins"), this);
  settingsAct->setStatusTip(tr("Open settings programm"));
  connect(settingsAct, &QAction::triggered, this, &MainWindow::openSettings);
  toolsMenu->addAction(settingsAct);
  //-------------------------------------------------------
  QMenu *helpMenu = menuBar()->addMenu(tr("Help"));

  QAction *aboutAct =
      helpMenu->addAction(tr("About"), this, &MainWindow::about);
  aboutAct->setStatusTip(tr("Show the application's About box "));

  //  QAction *aboutQtAct =
  //      helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
  //  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar() { statusBar()->showMessage(tr("Ready")); }

void MainWindow::readSettings() {
  QSettings settings(QCoreApplication::organizationName(),
                     QCoreApplication::applicationName());
  const QByteArray geometry =
      settings.value("geometry", QByteArray()).toByteArray();
  if (geometry.isEmpty()) {
    const QRect availableGeometry = screen()->availableGeometry();
    resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    move((availableGeometry.width() - width()) / 2,
         (availableGeometry.height() - height()) / 2);
  } else {
    restoreGeometry(geometry);
  }
}
void MainWindow::writeSettings() {
  QSettings settings(QCoreApplication::organizationName(),
                     QCoreApplication::applicationName());
  settings.setValue("geometry", saveGeometry());
}

MidChildTxt *MainWindow::activeMdiChild() const {
  if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
    return qobject_cast<MidChildTxt *>(activeSubWindow->widget());
  return nullptr;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const {
  QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

  const QList<QMdiSubWindow *> subWindows = mdiArea->subWindowList();
  for (QMdiSubWindow *window : subWindows) {
    MidChildTxt *mdiChild = qobject_cast<MidChildTxt *>(window->widget());
    if (mdiChild->currentFile() == canonicalFilePath)
      return window;
  }
  return nullptr;
}

void MainWindow::switchLayoutDirection() {
  if (layoutDirection() == Qt::LeftToRight)
    QGuiApplication::setLayoutDirection(Qt::RightToLeft);
  else
    QGuiApplication::setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::toPrintDoc() {
  if (mdiArea->subWindowList().empty())
    return;

  printDoc =
      qobject_cast<QPlainTextEdit *>(mdiArea->activeSubWindow()->widget());
  QPrinter printer;
  QPrintDialog diologPrinter(&printer, this);
  if (diologPrinter.exec() == QDialog::Accepted)
    printDoc->print(&printer);
}

void MainWindow::openSettings() {
  if (!settingsWindows) {
    settingsWindows = new SettingsWidget(this);
  }
  settingsWindows->show();
}
void MainWindow::setDefaultTheme() { defaultPalette = new QPalette; }
void MainWindow::setDarkTheme() {
  darkPalette = new QPalette;
  darkPalette->setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette->setColor(QPalette::WindowText, Qt::white);
  darkPalette->setColor(QPalette::Base, QColor(25, 25, 25));
  darkPalette->setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette->setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette->setColor(QPalette::ToolTipText, Qt::white);
  darkPalette->setColor(QPalette::Text, Qt::white);
  darkPalette->setColor(QPalette::Button, QColor(53, 53, 53));
  darkPalette->setColor(QPalette::ButtonText, Qt::white);
  darkPalette->setColor(QPalette::BrightText, Qt::red);
  darkPalette->setColor(QPalette::Link, QColor(42, 130, 218));
  darkPalette->setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette->setColor(QPalette::HighlightedText, Qt::black);
}

void MainWindow::setThemeColor(QString themeColor) {
  if (themeColor == "default") {
    if (!defaultPalette)
      setDefaultTheme();
    qApp->setPalette(*defaultPalette);
  } else if (themeColor == "dar") {
    if (!darkPalette)
      setDarkTheme();
    qApp->setPalette(*darkPalette);
  }
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = activeMdiChild()->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    activeMdiChild()->mergeCurrentCharFormat(format);
}

void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}
void MainWindow::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        activeMdiChild()->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        activeMdiChild()->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        activeMdiChild()->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        activeMdiChild()->setAlignment(Qt::AlignJustify);
}

void MainWindow::textStyle(int styleIndex)
{
    QTextCursor cursor = activeMdiChild()->textCursor();
    QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;
    QTextBlockFormat::MarkerType marker = QTextBlockFormat::MarkerType::NoMarker;

    switch (styleIndex) {
    case 1:
        style = QTextListFormat::ListDisc;
        break;
    case 2:
        style = QTextListFormat::ListCircle;
        break;
    case 3:
        style = QTextListFormat::ListSquare;
        break;
    case 4:
        if (cursor.currentList())
            style = cursor.currentList()->format().style();
        else
            style = QTextListFormat::ListDisc;
        marker = QTextBlockFormat::MarkerType::Unchecked;
        break;
    case 5:
        if (cursor.currentList())
            style = cursor.currentList()->format().style();
        else
            style = QTextListFormat::ListDisc;
        marker = QTextBlockFormat::MarkerType::Checked;
        break;
    case 6:
        style = QTextListFormat::ListDecimal;
        break;
    case 7:
        style = QTextListFormat::ListLowerAlpha;
        break;
    case 8:
        style = QTextListFormat::ListUpperAlpha;
        break;
    case 9:
        style = QTextListFormat::ListLowerRoman;
        break;
    case 10:
        style = QTextListFormat::ListUpperRoman;
        break;
    default:
        break;
    }

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    if (style == QTextListFormat::ListStyleUndefined) {
        blockFmt.setObjectIndex(-1);
        int headingLevel = styleIndex >= 11 ? styleIndex - 11 + 1 : 0; // H1 to H6, or Standard
        blockFmt.setHeadingLevel(headingLevel);
        cursor.setBlockFormat(blockFmt);

        int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2
        QTextCharFormat fmt;
        fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
        fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.mergeCharFormat(fmt);
        activeMdiChild()->mergeCurrentCharFormat(fmt);
    } else {
        blockFmt.setMarker(marker);
        cursor.setBlockFormat(blockFmt);
        QTextListFormat listFmt;
        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    }

    cursor.endEditBlock();
}

void MainWindow::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}
void MainWindow::setupTextActions()
{
    QToolBar *tb = addToolBar(tr("Format Actions"));
    QMenu *menu = menuBar()->addMenu(tr("F&ormat"));

    const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(":/file/icon/textbold.png"));
    actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &MainWindow::textBold);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    tb->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(":/file/icon/textitalic.png"));
    actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &MainWindow::textItalic);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    tb->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(":/file/icon/textunder.png"));
    actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &MainWindow::textUnderline);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    tb->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(":/file/icon/textleft.png"));
    actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(":/file/icon/textcenter.png"));
    actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(":/file/icon/textright.png"));
    actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(":/file/icon/textjustify.png"));
    actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    // Make sure the alignLeft  is always left of the alignRight
    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, &QActionGroup::triggered, this, &MainWindow::textAlign);

    if (QApplication::isLeftToRight()) {
        alignGroup->addAction(actionAlignLeft);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignRight);
    } else {
        alignGroup->addAction(actionAlignRight);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignLeft);
    }
    alignGroup->addAction(actionAlignJustify);

    tb->addActions(alignGroup->actions());
    menu->addActions(alignGroup->actions());

    menu->addSeparator();

    tb = addToolBar(tr("Format Actions"));
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Task List (Unchecked)");
    comboStyle->addItem("Task List (Checked)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");
    comboStyle->addItem("Heading 1");
    comboStyle->addItem("Heading 2");
    comboStyle->addItem("Heading 3");
    comboStyle->addItem("Heading 4");
    comboStyle->addItem("Heading 5");
    comboStyle->addItem("Heading 6");

    connect(comboStyle, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::textStyle);

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, &QComboBox::textActivated, this, &MainWindow::textFamily);

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

    connect(comboSize, &QComboBox::textActivated, this, &MainWindow::textSize);
}
MainWindow::~MainWindow() { /*delete ui;*/
}
