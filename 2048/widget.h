#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <qdebug.h>
#include <QFile>
#include <QFont>
#include <QTextStream>
#include <mypushbutton.h>
#include <gamewidget.h>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots://槽函数
    void onScoreInc(unsigned int score);
    void onGameOver();
    void onWin();
    void restart();
protected:
    void resizeEvent(QResizeEvent*ev)override;
private://初始化
    void ini2048();
private://控件成员
    MyPushButton * restartBtn;
    QLabel *ScoreLabel;
    QLabel *bestScorelabel;
    GameWidget* gameWidget;
/* 不允许布局 一步到位
private://布局成员
    qreal rationW;
    qreal rationH;
*/
private://图像成员
    QFont font;
private:
    Ui::Widget *ui;
private://数据成员
    QFile file;
    unsigned int highSocre;
};
#endif // WIDGET_H
