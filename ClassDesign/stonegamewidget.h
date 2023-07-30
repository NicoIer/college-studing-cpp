#ifndef STONEGAMEWIDGET_H
#define STONEGAMEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QInputDialog>
#include <QPixmap>
#include <QPainter>
#include <QMap>
#include <QSet>
#include <QPoint>
#include <QMessageBox>
#include "StoneGame.h"
#include "mypushbutton.h"
#include "qdebug.h"
namespace Ui {
class StoneGameWidget;
}

class StoneGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StoneGameWidget(QWidget *parent = nullptr);
    ~StoneGameWidget();
    void paintEvent(QPaintEvent*ev)override;
    void iniPix();
signals:
    void back();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    //节点 - 坐标映射
    QMap<QString,QPoint>points;
    QPixmap *pix;
    ChenJun Nico;
    GamePlay game;
    Ui::StoneGameWidget *ui;
    QVector<QPushButton>p1;
    QVector<QPushButton>p2;
};

#endif // STONEGAMEWIDGET_H
