#ifndef LOGOWIDGET_H
#define LOGOWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QScreen>
#include <QLabel>
#include <qdebug.h>
#include <QPoint>
class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoWidget(QWidget *parent = nullptr);
public://override
    void paintEvent(QPaintEvent*ev)override;
    void mouseMoveEvent(QMouseEvent*ev)override;
    void mousePressEvent(QMouseEvent*ev)override;
    void mouseReleaseEvent(QMouseEvent*ev)override;
private://ini
    void iniWidget();
private://控件
    QPushButton* beginButton;
    QLabel* logo;
private://数据
    QFont font;
    QPointF z;
signals:

    void start();

};

#endif // LOGOWIDGET_H
