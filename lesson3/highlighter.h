#ifndef WEATHERDIOLOG_H
#define WEATHERDIOLOG_H

#include <QDialog>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include <QtPositioning/QGeoPositionInfoSource>
#include <QtQml/QQmlListProperty>

namespace Ui {
class weatherDiolog;
}

class weatherDiolog : public QDialog {
  Q_OBJECT

public:
  explicit weatherDiolog(QWidget *parent = nullptr);
  ~weatherDiolog();

private:
  Ui::weatherDiolog *ui;
};

#endif // WEATHERDIOLOG_H
