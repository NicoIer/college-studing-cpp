#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include "stonegamewidget.h"
#include "bstwidget.h"
#include "listwidget.h"
#include "searchwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void StoneGame();
    void List();
    void Bst();
    void Search();

    void back();
private:
    void iniWidget();
private:
    Ui::Widget *ui;
    StoneGameWidget* stoneGameWidget;
    BstWidget* bstWidget;
    ListWidget* listWidget;
    SearchWidget* searchWidget;
};
#endif // WIDGET_H
