#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QCompleter>
#include <QLineEdit>
#include <QStringListModel>
#include <QTimer>
#include <QMessageBox>
#include <QAction>
#include "data.h"
namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();
signals:
    void back();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::SearchWidget *ui;
    QCompleter*competer;
    QStringListModel*stringListModel;
    QStringList indicator;
    QTimer*timer;
    Data data;//存放文件索引啥的;
};

#endif // SEARCHWIDGET_H
