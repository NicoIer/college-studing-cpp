#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qdebug.h>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QFile>
#include <QTimer>
#include <logowidget.h>
//无向有权图
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public://重载函数
    void paintEvent(QPaintEvent*ev)override;
    bool eventFilter(QObject *watched, QEvent *event)override;
private slots:
    void on_iniBtn_clicked();

    void on_startBtn_clicked();

private://控件成员

private://窗口成员
    LogoWidget *logo;
private://函数成员
    void iniMember();
private://ini

private://功能成员
    QTimer *timer;
private://数据成员
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
