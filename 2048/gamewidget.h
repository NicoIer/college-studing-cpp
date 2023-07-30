#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QKeyEvent>
enum GestureDirect{left=0,right=1,up=2,down=3};
enum AnimationType{move,appear};

class Animation
{
public:
    AnimationType type;
    GestureDirect direct;

    QPointF beginPos;
    QPointF endPos;

    unsigned int digit;
    unsigned int digit2;
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent*ev)override;

    void enterEvent(QEnterEvent*ev)override;
    void leaveEvent(QEvent*ev)override;

    void keyPressEvent(QKeyEvent*ev)override;
    void keyReleaseEvent(QKeyEvent*ev)override;
private://动画相关
    bool drawAnimation(QPainter&painter);
    bool playAnimation(Animation&animation,QPainter&painter);
private://游戏相关
    bool check();
    bool win();
public slots:
    void onGestureMove(GestureDirect);
    void resart();
signals:
    void GestureMove(GestureDirect);
    void ScoreInc(unsigned int);
    void GameOver();
    void Win();
private://数据成员
    //QPainter painter;
    QVector<QVector<unsigned int>>board;
    QVector<QColor> backColors;
    QVector<QColor> numColors;
    QVector<QPointF> dPos;//方向增量
    unsigned int w = 98;
    unsigned int h = 98;

    unsigned int score = 0;
    unsigned int digitCount =2;
    bool isAnimating = false;

    QList<Animation> animationList;//动画列表
    QTimer* timer;


private:
    void iniPainter();
    void iniColors();
    void iniVar();
    void iniBoard();
    unsigned int getBitCount(unsigned int n);
};

inline void GameWidget::iniColors()
{
    //background colors
    backColors ={QColor(240,230,220),
                 QColor(236,224,200),
                 QColor(242,176,121),
                 QColor(245,150,98),
                 QColor(245,124,96),
                 QColor(245,93,60),
                 QColor(236,207,114),
                 QColor(237,204,98),
                 QColor(237,201,80),
                 QColor(239,197,63),
                 QColor(255,128,0)};//2 4 8 16 32 64 128 256 512 1024 2048
    //number colors
    numColors = {QColor(119,119,101),
                 Qt::white};
}

#endif // GAMEWIDGET_H
