#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QVector>
#include <exception>
enum Direction{N,S,E,W,NE,NW,SE,SW};//定义方位
namespace Ui {
class MazeWidget;
}

class MazeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MazeWidget(QWidget *parent = nullptr);
    ~MazeWidget();

    void paintEvent(QPaintEvent*pe)override;

    bool seekPath(int xBegin,int yBegin);//寻路函数

    void iniPainter();
private:
    bool iniMaze(QVector<QVector<bool>>Maze={});
    QPair<int,int> Move(Direction);
private:
    Ui::MazeWidget *ui;
    QVector<QVector<bool>> maze;//迷宫数组
    QVector<QVector<bool>> mark;//标签数组
    QPair<int,int>outer;
    QPair<int,int>enter;
    QPixmap mazePix;
    QPainter painter;
};

#endif // MAZEWIDGET_H
