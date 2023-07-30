#include "drawer.h"

Drawer::Drawer(QWidget *parent) : QToolBox(parent)
{
    //配置窗口
    this->setWindowTitle("Self QQ");
    this->setWindowIcon(QPixmap(":/image/1.png"));
    //初始化成员
    QString str;
    for(int i =0;i!=9;++i)
    {
        str = QString(":/image/%1.png").arg(QString::number(i+1));

        toolBtn.push_back(new QToolButton(this));//添加按钮
        toolBtn[i]->setText(QString("用户%1").arg(QString::number(i+1)));
        toolBtn[i]->setIcon(QPixmap(str));
        toolBtn[i]->setIconSize(/*QPixmap(str).size()*/QSize(124,124));
        toolBtn[i]->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
        toolBtn[i]->setAutoRaise(true);

        chatWidget.push_back(new Widget(nullptr,toolBtn[i]->text()));//这里不指定父亲么
        chatWidget[i]->setWindowTitle(toolBtn[i]->text());
        chatWidget[i]->setWindowIcon(toolBtn[i]->icon());
    }
    QGroupBox *groupBox = new QGroupBox(this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    layout->setAlignment(Qt::AlignLeft);
    for(int i =0;i!=9;++i)
    {
        layout->addWidget(toolBtn[i]);
        layout->addStretch();
        connect(toolBtn[i],&QToolButton::clicked,this,[=]{showChatWidget(i);});//利用lambda传递参数 绑定槽函数
    }
    this->addItem((QWidget*)groupBox,"群成员");

}

void Drawer::showChatWidget(size_t userIndex)
{
    chatWidget[userIndex]->show();
}











































