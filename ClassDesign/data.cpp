#include "data.h"

Data::Data(QObject *parent)
    : QObject{parent}
{

}

bool Data::createIndex(const QString &path)
{//读取一个目录下的所有文件
    //目录
    QDir dir(path);
    qDebug()<<dir.dirName();
    dir.setFilter(QDir::Files);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.txt";
    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    qDebug()<<files;

    //得到目录下的所有文件 提取文件关键词 到keys 同时保存出现次数
    for(auto &i:files)
    {
        QString road = CONTENTPATH+i;//即为这个文件路径
        //读取文件中的关键字 存到keys
        QFile file(road);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream in(&file);
        qDebug()<<"当前读取文件:"<<road;
        while(!in.atEnd())
        {//一行一度去
            QString key;
            in>>key;
            qDebug()<<"当前读取key"<<key;
            ++keys[key][road];//找到关键字对应map 找到map中对应road对应的times
        }
        file.close();
    }
    return true;
}

bool Data::readKeys(const QString &path)
{//读取关键字
    QFile file;
    QFileInfo fileInfo;
    file.setFileName(path);
    fileInfo.setFile(file);
    qDebug()<<path;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"未打开";
        return false;
    }
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QString fileLine;

    while(!in.atEnd())
    {//每一行的格式  key:#road,times#road,times#
        fileLine = in.readLine();
        auto line = fileLine.split(":");
        if(line.size()>=2)
        {
            QString key = line[0];
            QString roads = line[1];
            QMap<QString,int>sets;
            auto items = roads.split("#");
            for(auto&item:items)
            {//road,times
                auto ps = item.split(",");
                QString road = ps[0];
                int times = ps[1].toInt();
                sets.insert(road,times);
            }
            keys.insert(key,sets);
        }
        else if(line.size()==1)
        {
            QString key = line[0];
            keys.insert(key,QMap<QString,int>());//空
        }
        else
        {
            qDebug()<<"未知错误";
            return false;
        }
    }
    //测试映射
    for(auto key = keys.begin();key!=keys.end();++key)
    {//key是迭代器
        qDebug()<<key.key()<<"对应的road 和 times";
        for(auto i=keys[key.key()].begin();i!=keys[key.key()].end();++i)
        {//关键字对应集合映射
            qDebug()<<"road:"<<i.key()<<"times:"<<i.value();
        }
        qDebug()<<"--------------------------------";
    }
    file.close();
    return true;
}

bool Data::saveKeys(const QString &path)
{//保存key到文件
    QFile file;
    QFileInfo fileInfo;
    file.setFileName(path);
    qDebug()<<path;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"保存文件时路径错误 未打开";
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    for(auto beg = keys.begin();beg!=keys.end();++beg)
    {// QString   -   QMap<QString  int >
        QString line;
        line+=beg.key();
        line+=":";
        for(auto i = keys[beg.key()].begin();i!=keys[beg.key()].end();++i)
        {
            line+="#";
            //road
            line+=i.key();
            line+=",";
            //times
            line+=QString::number(i.value());
            line+="#";
        }
        in<<line;
    }
    file.close();
    return true;
}

QVector<QPair<int,QString>> Data::find(const QString &key)
{//通过key值查找出现文件
    QVector<QPair<int,QString>>ans;
    for(auto i = keys[key].begin();i!=keys[key].end();++i)
        ans.push_back({i.value(),i.key()});
    std::sort(ans.begin(),ans.end());
    qDebug()<<ans;
    return ans;

}

void getFiles(string path, vector<string>& files)
{
    intptr_t   hFile = 0;//文件句柄，过会儿用来查找
    struct _finddata_t fileinfo;//文件信息
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    //如果查找到第一个文件
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))//如果是文件夹
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("/").append(fileinfo.name), files);
            }
            else//如果是文件
            {
                files.push_back(p.assign(path).append("/").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);	//能寻找到其他文件

        _findclose(hFile);	//结束查找，关闭句柄
    }
}
