#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>
#include <QEvent>
#include <qdebug.h>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <graph.h>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QThread>
//功能函数 再graph.h中已经声明
//QPoint transToPoint(QString vertex);
//set<Edge<QString>> transToEdges(QString vertex);
//QString getName(QString vertex);

class PrintWidget : public QWidget
{
    Q_OBJECT
    friend class DfsWork;
public:
    explicit PrintWidget(QWidget *parent = nullptr);
public://overload

    void leaveEvent(QEvent*ev)override;
    void enterEvent(QEnterEvent*ev)override;

    void paintEvent(QPaintEvent*ev)override;
    void resizeEvent(QResizeEvent*)override;
signals:

public slots:

    void iniDevice();

public:
    void dfsPaint(const QString&beg = "A");
    void dfsHelper(const QString& u, set<QString>& visited);
    void primPaint(const QString&beg = "A");
    void primPaintHelper(const QString&beg = "A");
    void dijPaint(const QString&beg = "A");
private:
    void iniMember();
    void iniPen();
    void iniFont();
    void iniColors();
    void iniGraph();
    void iniPainter();
    void drawBeginGraph();
private:
    QPainter* pixmapPainter;
    QPen pointPen;//用于画点的笔
    QPen linePen;//用于画线的笔
    QPen textPen;
    QPen backPen;//用于画背景的笔
    QFont font;//字体
    QHash<QString,QColor> colors;//存储颜色
    QPixmap* pixmap;//绘图区域
    QFile file;
    QHash<QString,QPoint> points;
    bool flag = false;
private:
    void delay(unsigned int);
public:
    Graph<QString>graph;
};

#endif // PRINTWIDGET_H
