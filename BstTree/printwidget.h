#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>
#include "BstTree.h"
#include <QPainter>
#include <qdebug.h>
#include <QTimer>
#include <QTime>
#include <QPixmap>
namespace Ui {
class PrintWidget;
}

class PrintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent = nullptr);
    ~PrintWidget();
    void SetRoot(BstTree<QChar>*r);
protected:
    void paintEvent(QPaintEvent *pe);//重写绘图事件
    void DrawTree(QPainter*painter ,BstNode<QChar> *rootr,QPoint begin,QPoint end);
    void iniVar();//初始化参数
    void iniPen(QColor color = QColor(0,255,0),int width = 5,Qt::PenStyle style = Qt::PenStyle::SolidLine);//设置笔
private slots:
    void on_btnStep_clicked();

    void on_btnAll_clicked();

private:
    Ui::PrintWidget *ui;
    BstTree<QChar> *tree;
    QPixmap* pixmap;//作为画布
    QPainter* painterOnPixmap;//画家
    QPen linePen;//画线的笔
    QPen eilPen;//画圈的笔
    QPen textPen;//写字的笔
    bool flag = false;//是否分部打印
    unsigned int treeHeight;//层高 不是树的深度
};

#endif // PRINTWIDGET_H
