#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "BstTree.h"
#include <QString>
#include <QChar>
#include <QMessageBox>
#include <qdebug.h>
#include "printwidget.h"
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
    void CreatTree();
private:
    bool unique(QString T);
private:
    Ui::Widget *ui;
    BstTree<QChar>*tree;
    PrintWidget *p;
};
#endif // WIDGET_H
