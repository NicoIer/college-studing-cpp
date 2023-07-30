#include "qdataframe.h"
//不太对 应该可以得到对应的  行表头  列表头
//存储方式 是否可以使用huffman树呢 ?
QDataFrame::QDataFrame()
{

}

void QDataFrame::read_csv(const QString &filePath,bool readTitle)
{
    file.setFileName(filePath);//配置文件路径
    fileinfo.setFile(file);//获取文件信息

    if(fileinfo.suffix()=="csv")//读取的文件确实是.csv文件
    {
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))//打开文件
        {
            qDebug()<<"a error happens when open the csv file";
            return;
        }


        QTextStream in(&file);//绑定读取的文件 设置编码格式
        in.setEncoding(QStringConverter::Utf8);

        QString fileLine;
        if(!readTitle)//如果不读title
            fileLine = in.readLine();//把第一行读了去! 覆盖就行

        QPair<QString,QString>pair;
        size_t fristIndex = 0;
        while(!in.atEnd())
        {//csv 文件 每个元素用" "括起来  以第一个逗号分割
            fileLine = in.readLine();//按行读取
            fristIndex = fileLine.indexOf(',');//找第一个逗号的位置
            pair = {fileLine.mid(0,fristIndex),fileLine.mid(fristIndex+1)};//按第一个逗号的位置分割字符串
            //qDebug()<<pair.first<<"     "<<pair.second;

            list<<pair;//把所有pair存到QList里面
            englishList<<fileLine[0];//English表项存
            map.insert(fileLine.mid(0,fristIndex),fileLine.mid(fristIndex+1));//Pair存到map;
            hash.insert(fileLine.mid(0,fristIndex),fileLine.mid(fristIndex+1));//Pair存到Hash
        }


    }
    else//非csv格式文件
    {
        qDebug()<<"非csv文件 请采用其他格式读取";
        return;
    }

    file.close();
}
