#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QIntValidator>
#include <QMessageBox>
#include "Clist.h"
#include "qdebug.h"
namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);
    ~ListWidget();
signals:
    void back();
private slots:
    void on_pushButton_clicked();

    void on_createBtn_clicked();

    void on_pushButton_5_clicked();

    void on_finishBtn_clicked();

    void on_remakeBtn_clicked();

    void on_judgeBtn_clicked();

private:
    Ui::ListWidget *ui;
    bool isTwo =false;
    ListNode*node;
    ListNode*head;
    ListNode*head2;
    ListNode*tail;
    ListNode*beg;
    ListNode*end;
};

#endif // LISTWIDGET_H
