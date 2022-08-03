#include "figure.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

constexpr int kSceneWidth = 800;
constexpr int kSceneHeight = 600;
constexpr int kRectWidth = 100;
constexpr int kRectHeight = 40;
constexpr int kStarSize = 80;
constexpr double kScaleStep = 0.1;

class ItemEventFilter : public QGraphicsItem {
public:
    QRectF boundingRect() const override { return {}; }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* = 0) override {}

protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override {
        if (move(watched, event)) {
            return true;
        }
        return rotate(watched, event);
    }

private:
    bool move(QGraphicsItem *watched, QEvent *event) {
        if (event->type() == QEvent::GraphicsSceneMouseMove) {
            auto move_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (move_) {
                auto delta_point = move_event->scenePos() - move_event->lastScenePos();
                watched->moveBy(delta_point.x(), delta_point.y());
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            auto press_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (press_event->button() == Qt::LeftButton) {
                move_ = true;
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            auto press_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (press_event->button() == Qt::LeftButton) {
                move_ = false;
                return true;
            }
        }

        return false;
    }

    bool rotate(QGraphicsItem *watched, QEvent *event) {
        if (event->type() == QEvent::GraphicsSceneMouseMove) {
            auto move_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (rotate_) {
                QTransform transform;
                auto mouse_pos = move_event->pos().toPoint();
                auto center = watched->boundingRect().center();
                double angle = mouse_pos.x() - center.x();
                transform.translate(center.x(), center.y()).rotate(angle).translate(-center.x(), -center.y());
                watched->setTransform(transform);
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            auto press_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (press_event->button() == Qt::MidButton) {
                rotate_ = true;
                return true;
            }
        }
        if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            auto press_event = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (press_event->button() == Qt::MidButton) {
                rotate_ = false;
                return true;
            }
        }

        return false;
    }

    bool move_{};
    bool rotate_{};
};

FigureWindow::FigureWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , scene_{kSceneWidth / 2 - kSceneWidth, kSceneHeight / 2 - kSceneHeight, kSceneWidth, kSceneHeight}
{
    ui_->setupUi(this);
    ui_->graphicsView->setScene(&scene_);
    ui_->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui_->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui_->graphicsView->installEventFilter(this);

    auto filter = new ItemEventFilter{};
    scene_.addItem(filter);

    makers_map_.insert({kRect, [this, filter] (int x, int y) {
                            auto item = scene_.addRect(makeRect(x, y), QPen{Qt::NoPen}, getRandomBrush());
                            item->installSceneEventFilter(filter);
                        }});
    makers_map_.insert({kEllipse, [this, filter] (int x, int y) {
                            auto item = scene_.addEllipse(makeRect(x, y), QPen{Qt::NoPen}, getRandomBrush());
                            item->installSceneEventFilter(filter);
                        }});
    makers_map_.insert({kStar, [this, filter] (int x, int y) {
                            auto item = scene_.addPolygon(makeStar(x, y), QPen{Qt::NoPen}, getRandomBrush());
                            item->installSceneEventFilter(filter);
                        }});

    setWindowTitle("Crazy figures!");
}

FigureWindow::~FigureWindow()
{
    delete ui_;
}

bool FigureWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui_->graphicsView) {
        return false;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        auto mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton) {
            makeObject(mouse_event->x(), mouse_event->y());
        } else {
            auto figure = scene_.itemAt(ui_->graphicsView->mapToScene(mouse_event->x(), mouse_event->y()), QTransform{});
            if (figure) {
                scene_.removeItem(figure);
            }
        }
    } else if (event->type() == QEvent::Wheel) {
        auto weel_event = static_cast<QWheelEvent*>(event);
        double sx_sy = 1. + (weel_event->angleDelta().y() > 0 ? kScaleStep : -kScaleStep);
        ui_->graphicsView->scale(sx_sy, sx_sy);
    } else if (event->type() == QEvent::KeyPress) {
        auto key = static_cast<QKeyEvent*>(event)->key();
        if (key == Qt::Key_Plus || key == Qt::Key_Minus) {
            double sx_sy = 1. + (key == Qt::Key_Plus ? kScaleStep : -kScaleStep);
            ui_->graphicsView->scale(sx_sy, sx_sy);
        }
    }

    return false;
}

QBrush FigureWindow::getRandomBrush()
{
    constexpr uint8_t kMaxColorValue = std::numeric_limits<uint8_t>::max();
    return QBrush{QColor{static_cast<int>(QRandomGenerator::global()->bounded(kMaxColorValue)),
                    static_cast<int>(QRandomGenerator::global()->bounded(kMaxColorValue)),
                    static_cast<int>(QRandomGenerator::global()->bounded(kMaxColorValue))}};
}

int FigureWindow::getNextFigureIndex()
{
    return (figure_index_++) % kEnd;
}

void FigureWindow::makeObject(int x, int y)
{
    auto it = makers_map_.find(getNextFigureIndex());
    if (it == makers_map_.end()) {
        qDebug() << "Maker function doesn't exist";
        return;
    }
    it->second(x, y);
}

QRectF FigureWindow::makeRect(int x, int y)
{
    auto top_left_point = ui_->graphicsView->mapToScene(x - kRectWidth / 2, y - kRectHeight / 2);
    auto bottom_right_point = ui_->graphicsView->mapToScene(x + kRectWidth / 2, y + kRectHeight / 2);
    return QRectF{top_left_point, bottom_right_point};
}

QPolygonF FigureWindow::makeStar(int x, int y)
{
    auto point = ui_->graphicsView->mapToScene(x, y);
    QPolygonF polygon{};
    polygon << QPointF{point.x(), point.y() - kStarSize / 2}
            << QPointF{point.x() - kStarSize / 6, point.y() - kStarSize / 7}
            << QPointF{point.x() - kStarSize / 2, point.y() - kStarSize / 7}
            << QPointF{point.x() - kStarSize / 5, point.y() + kStarSize / 8}
            << QPointF{point.x() - kStarSize / 3, point.y() + kStarSize / 2}
            << QPointF{point.x(), point.y() + kStarSize / 4}
            << QPointF{point.x() + kStarSize / 3, point.y() + kStarSize / 2}
            << QPointF{point.x() + kStarSize / 5, point.y() + kStarSize / 8}
            << QPointF{point.x() + kStarSize / 2, point.y() - kStarSize / 7}
            << QPointF{point.x() + kStarSize / 6, point.y() - kStarSize / 7};
    return polygon;
}

