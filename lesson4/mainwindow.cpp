#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QHash>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle(Name_Programm);
  hedersFile.clear();
  filter = trUtf8("all(*.*);;bin file(*.bin)");
  connect(ui->actionOpen, &QAction::triggered, this,
          &MainWindow::on_actionOpen_clicked);
  connect(ui->actionSave, &QAction::triggered, this,
          &MainWindow::on_actionSave_clicked);
  connect(ui->actionHelp, &QAction::triggered, this,
          &MainWindow::on_actionHelp_clicked);
  connect(ui->actionSettings, &QAction::triggered, this,
          &MainWindow::on_actionSettings_clicked);

  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(13);
  ui->plainTextEdit->setFont(font);

  m_highlighter = new Highlighter(ui->plainTextEdit->document());
}

void MainWindow::setTitleName(QString s = "") {

  setWindowTitle(s.mid(s.lastIndexOf('/') + 1, s.length()) + " - " +
                 Name_Programm);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionHelp_clicked() {
  QMessageBox::warning(this, "Help",
                       "this is help information for the sake of help "
                       "information help information");
}

void MainWindow::on_actionSave_clicked() {
  QString s = QFileDialog::getSaveFileName(this, "Save file",
                                           QDir::current().path(), filter);
  if (s.length() > 0) {
    int index = s.indexOf(".txt"); // определяем, есть ли в названии
    // строка ".txt"
    QFile file(s);
    if (file.open(QFile::WriteOnly)) {
      if (index != -1) // если текстовый файл (выполняются
      // 2 условия)
      {
        QTextStream stream(&file);
        stream << ui->plainTextEdit->toPlainText();
      } else {
        QDataStream stream(&file);
        QByteArray b = QString("GB_Qt").toUtf8();
        stream.writeRawData(b.data(), b.length());
        QHash<QChar, int> symbamount;
        symbamount.clear();
        QString str = ui->plainTextEdit->toPlainText();
        int amount = str.length();
        QString usedS = "";
        for (int i = 0; i < amount; ++i) {
          QChar ch = str.at(i);
          int index = usedS.indexOf(ch);
          if (index == -1) {
            symbamount[ch] = 0;
            usedS += ch;
          } else {
            symbamount[ch]++;
          }
        }
        for (int i = 0; i < amount; ++i) {
          QChar chi = usedS[i];
          for (int j = i + 1; j < amount; ++j) {
            QChar chj = usedS[j];
            if (symbamount[chi] < symbamount[chj]) {
              usedS[i] = chj;
              usedS[j] = chi;
              chi = chj;
            }
          }
        }
        symbamount.clear(); // больше не понадобится
        //**********************************************
        b = usedS.toUtf8();
        amount = b.length();
        stream.writeRawData(reinterpret_cast<char *>(&amount), sizeof amount);
        stream.writeRawData(b.data(), amount);
        //***********************************************
        amount = str.length();
        for (int i = 0; i < amount; i++) {
          int index = usedS.indexOf(str.at(i));
          for (bool w = true; w;) {
            char wr = index % 128;
            index /= 128;
            if (index) {
              wr |= 0x80;
              stream.writeRawData(&wr, 1); // старший бит
              // сообщает, что
              // значение состоит
              // еще из 1 байта
            } else {
              stream.writeRawData(&wr, 1);
              w = false;
            }
          }
        }
      }
      file.close();
    }
  }
}

void MainWindow::on_actionOpen_clicked() {
  ui->plainTextEdit->setReadOnly(false);
  QString s = QFileDialog::getOpenFileName(this, "Open file",
                                           QDir::current().path(), filter);
  setTitleName(s);
  if (s.length() > 0) {
    int index = s.indexOf(".txt");
    QFile file(s);
    if (file.open(QFile::ReadOnly | QFile::ExistingOnly)) {

      if (index != -1 && s.length() - 4 == index) // если текстовый файл
      // (выполняются 2
      // условия)
      {
        QTextStream stream(&file);
        ui->plainTextEdit->setPlainText(stream.readAll());
      } else {
        QDataStream stream(&file);
        QByteArray b = QString("GB_Qt").toUtf8();
        char mH[15];
        stream.readRawData(mH, b.length());
        if (memcmp(mH, b.data(), b.length())) // если не совпадает
        // заголовок, то открытие
        // недопустимо, так как
        // формат у файла другой
        {
          file.close();
          return;
        }
        uint amount = 0;
        stream.readRawData(reinterpret_cast<char *>(&amount), sizeof amount);
        b.resize(amount);
        stream.readRawData(b.data(), amount);
        QString usedS = trUtf8(b.data());
        QString str = "";
        uint pos = 0;
        uint mm = 1;
        for (; !stream.atEnd();) { // atEnd() true конец файла
          char ch;
          stream.readRawData(&ch, 1);
          bool bl = (ch & 0x80) ? false : true;
          int rValue = (ch & 0x7F);
          pos += mm * rValue;
          mm *= 128;
          if (bl) {
            mm = 1;
            str += usedS.at(pos);
            pos = 0;
          }
        }
        ui->plainTextEdit->setPlainText(str);
      }
    }
    file.seek(0);
    auto pp = file.readAll();
    file.close();
  }
}

void MainWindow::on_actionNew_triggered() {
  ui->plainTextEdit->clear();
  ui->plainTextEdit->setReadOnly(false);
  setWindowTitle(Name_Programm);
}

void MainWindow::on_actionSettings_clicked() {

  settingsWidget settingsWidget(this);
  settingsWindow->show();
  //  QPalette darkPalette;
  //  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  //  darkPalette.setColor(QPalette::WindowText, Qt::white);
  //  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
  //  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  //  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  //  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  //  darkPalette.setColor(QPalette::Text, Qt::white);
  //  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
  //  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  //  darkPalette.setColor(QPalette::BrightText, Qt::red);
  //  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  //  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  //  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
  //  qApp->setPalette(darkPalette);

  //  // qApp->setPalette(style()->standardPalette()); -> не работает
}

void MainWindow::on_actionView_triggered() {
  on_actionOpen_clicked();
  ui->plainTextEdit->setReadOnly(true);
}
