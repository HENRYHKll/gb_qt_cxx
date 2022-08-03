#ifndef MIDCHILDTXT_H
#define MIDCHILDTXT_H

#include <QTextEdit>

class MidChildTxt : public QTextEdit {
  Q_OBJECT
public:
  MidChildTxt();
  void newFile();
  bool loadFile(const QString &fileName);
  bool save();
  bool saveAs();
  bool saveFile(const QString &fileName);
  QString userFriendlyCurrentFile();
  QString currentFile() { return curFile; }

protected:
  void closeEvent(QCloseEvent *event) override;
private slots:
  void documentWasModified();

private:
  bool maybeSave();
  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullFileName);

  QString curFile;
  bool isUntitled;
};

#endif // MIDCHILDTXT_H
