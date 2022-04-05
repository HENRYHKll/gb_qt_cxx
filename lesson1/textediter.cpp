#include "textediter.h"
#include "ui_textediter.h"

TextEditer::TextEditer(QWidget *parent)
    : QDialog(parent), ui(new Ui::TextEditer) {
  ui->setupUi(this);
}

TextEditer::~TextEditer() { delete ui; }

void TextEditer::on_pushButton_clicked() {
  QString strEdit = ui->plainTextEdit->toPlainText();
  ui->plainTextRewiu->setPlainText(strEdit);
}

void TextEditer::on_pushButton_2_clicked() {
  QString strEdit = ui->plainTextEdit->toPlainText();
  buf.append(strEdit);
  ui->plainTextRewiu->setPlainText(buf);
}

void TextEditer::on_pushButton_3_clicked() {
  QString strEdit = ui->plainTextEdit->toPlainText(),
          html_str = "<font color='red'>";
  html_str += strEdit + "</font>";

  buf.append(html_str);
  ui->plainTextRewiu->document()->setHtml(buf);
  ui->textBrowser->setHtml(buf);
}
