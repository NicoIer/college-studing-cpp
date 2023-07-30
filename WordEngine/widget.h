#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QAxWidget>
#include <QDir>
#include <QAxObject>
#include <QTextStream>
#include <QFileDialog>
#include "qwordengine.h"
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void OpenWord(QString&filename);
    void CloseOffice();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QWordEngine*word;
    QString path;
};

#endif // WIDGET_H
