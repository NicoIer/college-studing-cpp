#ifndef QDATAFRAME_H
#define QDATAFRAME_H

#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <qdebug.h>
#include <QRegularExpression>
#include <QList>
#include <QPair>
#include <QMap>
#include <QHash>
//为了完成作业 写一个简单的 类似Python中的Pandas库 中的DataFrame用于读取表格数据

//存储结构1 - QList存Pair
//存储结构2 - QMap存Pair
class QDataFrame
{
public:
    QDataFrame();
    void read_csv(const QString&filePath,bool readTitle = false);//读取csv
private:
    QFile file;
    QFileInfo fileinfo;
public:
    QList<QPair<QString,QString>> list;//最后的键值对信息储存在这里
    QStringList englishList;
    QMap<QString,QString> map;
    QHash<QString,QString>  hash;
};

#endif // QDATAFRAME_H
