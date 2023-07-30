#ifndef DRAWER_H
#define DRAWER_H

#include <QToolBox>
#include <QToolButton>
#include <QVector>
#include <QGroupBox>
#include <QVBoxLayout>
#include "widget.h"
class Drawer : public QToolBox
{
    Q_OBJECT
public:
    explicit Drawer(QWidget *parent = nullptr);

signals:
private slots:
    void showChatWidget(size_t userIndex);
private:
    QVector<Widget*> chatWidget;
    QVector<QToolButton*> toolBtn;
};

#endif // DRAWER_H
