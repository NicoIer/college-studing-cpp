#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCompleter>
#include <QPushButton>
#include "qdataframe.h"
#include <QLineEdit>
#include <QAction>
#include <QStringListModel>//视图模型类
#include <QTimer>
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

private:
    Ui::Widget *ui;
    QCompleter* competer;//补全框
    QStringListModel* stringListModel;//显示检索结果的视图
    QStringList indicator;//检索结果的存储List
    QPushButton* searchBtn;//搜索ACtion
    QDataFrame data;
    QTimer* timer;//定时检测以更新补全框
};
#endif // WIDGET_H
