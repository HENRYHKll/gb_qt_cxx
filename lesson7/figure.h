#ifndef FIGURE_H
#define FIGURE_H


#include <unordered_map>
#include <functional>

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FigureWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FigureWindow(QWidget *parent = nullptr);
    ~FigureWindow() override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    enum Figure {
        kRect,
        kEllipse,
        kStar,
        kEnd
    };

    static QBrush getRandomBrush();

    int getNextFigureIndex();
    void makeObject(int x, int y);
    QRectF makeRect(int x, int y);
    QPolygonF makeStar(int x, int y);

    Ui::MainWindow *ui_;
    QGraphicsScene scene_;
    std::unordered_map<int, std::function<void(int x, int y)>> makers_map_;
    int figure_index_{};

};
#endif // FIGURE_H
