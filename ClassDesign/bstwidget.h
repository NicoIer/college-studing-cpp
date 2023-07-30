#ifndef BSTWIDGET_H
#define BSTWIDGET_H

#include <QWidget>
#include "CBst.h"
namespace Ui {
class BstWidget;
}

class BstWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BstWidget(QWidget *parent = nullptr);
    ~BstWidget();
signals:
    void back();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::BstWidget *ui;
    shared_ptr<Node> root;
};

#endif // BSTWIDGET_H
