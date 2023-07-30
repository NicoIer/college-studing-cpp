#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <fstream>
#include <QMap>
#include <QDir>
#include <QStringList>
#include <io.h>
#include <vector>
#include <string>
#include "qdebug.h"

//8888888888888888888888888888888888888888888888888
#define KEYPATH "./data/keys.txt"
#define CONTENTPATH "./data/contents/"
//8888888888888888888888888888888888888888888888888



//8888888888888888888888888888888888888888888888888
using std::string;
using std::vector;
//8888888888888888888888888888888888888888888888888



//8888888888888888888888888888888888888888888888888
void getFiles(string path, vector<string>& files);
//8888888888888888888888888888888888888888888888888

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);
    bool createIndex(const QString&path = CONTENTPATH);
    bool readKeys(const QString& path = KEYPATH);
    bool saveKeys(const QString&path =KEYPATH);
    QVector<QPair<int,QString>> find(const QString&key);
signals:

public:
    QMap<QString,QMap<QString,int>> keys;//关键字 - {文件路径/频率集合}

};

#endif // DATA_H
