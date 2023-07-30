#ifndef QWORDENGINE_H
#define QWORDENGINE_H

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QAxBase>
#include <QFile>
#include <QTextStream>
#include <QDir>


/*
* 微软的word的查看函数网站
* https://docs.microsoft.com/zh-cn/dotnet/api/microsoft.office.interop.word?view=word-pia
*
* 参考https://blog.csdn.net/u010304326/article/details/82292195#comments
* 参考https://blog.csdn.net/qq_35192280/article/details/83021975
* https://blog.csdn.net/zy47675676/article/details/86251991 表格垂直居中、水平居中
*/

enum TITLE_NUMBER
{
    TITLE_ONE = 0,
    TITLE_TWO,
    TITLE_THREE,
    NORMAL
};

//MOVEEND_INDEX来自于https://docs.microsoft.com/zh-cn/dotnet/api/microsoft.office.interop.word.wdunits?view=word-pia
enum MOVEEND_INDEX
{
    wdParagraph = 4, //段落。
    wdStory = 6 , 	//部分。
    wdRow = 10, //行
    wdParagraphFormatting = 14, //段落格式。
    wdTable = 15 //表格

};

//文本对齐方式
enum WdParagraphAlignment
{
    AlignParLeft = 0, //左对齐
    AlignParCenter = 1, //居中对齐。
    AlignParRight = 2, //右对齐。
    AlignParJustify = 3, //完全两端对齐。
};

class QWordEngine : public QObject
{
    Q_OBJECT

public:
    QWordEngine(QObject *parent = NULL);
    QWordEngine(QString filename,QObject *parent= NULL);
    ~QWordEngine();


    //打开文件 bVisable 是否显示弹窗
    bool open(bool bvisable = false);
    bool open(const QString& strFile, bool bVisable = false);
    ///

    //关闭文件
    bool close();
    bool isOpen();

    //保存
    void save();
    bool saveAs(const QString& strSaveFile);
    //


    //添加文本 titlestr 添加的文本 number 标题还是正文，默认是正文
    bool addText(QString titlestr,TITLE_NUMBER number = NORMAL ,WdParagraphAlignment alignment = AlignParLeft);
    //默认是黑色,问题:没有实现在一行实现不同颜色
    bool addText(QString titlestr,QFont font,QColor fontcolor = Qt::black);
    //返回QAxObject方便设置颜色以及其他样式
    QAxObject* addText2(QString titlestr);
    //

    //

    //功能：插入回车
    bool insertEnter();
    //

    //光标移动到最后
    bool moveToEnd();
    bool moveToEnd(MOVEEND_INDEX wd);
    //移动到表格的最下面一行
    bool moveToEnd(QAxObject *table);
    //


    //表格操作====================================================================
    /******************************************************************************
    * 函数：insertTable
    * 功能：创建表格
    * 参数：nStart 开始位置; nEnd 结束位置; row 行; column 列
    * 返回值： QAxObject*
    *****************************************************************************/
    QAxObject* insertTable(int nStart, int nEnd, int row, int column);

    /******************************************************************************
    *创建表格
    *QStringList headList 添加表头
    ******************************************************************************/
    QAxObject* createTable(int row, int column,QStringList headList = QStringList());

    //设置列宽
    void setColumnWidth(QAxObject *table, int column, int width);
    // 为表格添加行
    void addTableRow(QAxObject *table, int nRow, int rowCount);
    void appendTableRow(QAxObject *table,int rowCount);


    /******************************************************************************
    * 函数：setCellString
    * 功能：设置表格内容
    * 参数：table 表格; row 行数; column 列数; text 插入文本   row 和 column从0开始
    *****************************************************************************/
    void setCellString(QAxObject *table, int row, int column, const QString& text);

    // 设置内容粗体  isBold控制是否粗体
    void setCellFontBold(QAxObject *table, int row, int column, bool isBold);
    // 设置文字大小
    void setCellFontSize(QAxObject *table, int row, int column, int size);
    // 在表格中插入图片
    void insertCellPic(QAxObject *table, int row,int column,const QString& picPath);


    /******************************************************************************
    * 函数：MergeCells
    * 功能：合并单元格
    * 参数：table 表格; nStartRow 起始单元格行数; nStartCol ; nEndRow ; nEndCol
    *****************************************************************************/
    void MergeCells(QAxObject *table, int nStartRow,int nStartCol,int nEndRow,int nEndCol);

    //===============================================================================




    //插入图片 picPath 图片路径
    void insertPic(QString picPath);

    //文字对齐方式
    void setAlignment(int index);

    //设置颜色 不能直接利用QColor需要将颜色转成int值
    void setColor(QColor color);
    void setColor(QAxObject *obj,QColor color);
    void setBgColor(QAxObject *obj,QColor color);

    //设置字号
    void setFontSize(int size);

private:
    void writeFile( QString savestr,QString filename );
    QString getTitleStr(TITLE_NUMBER number); //返回标题字符串
    void setPropraty(QAxObject *axobj,QString proname,QVariant provalue); //设置某个对象的某个属性值
    int colorToInt(QColor color); //将颜色转化成整数，因为QColor("blue").value()是255，不是想要的结果


private:
    QString m_filename;
    bool m_bOpened;
    QAxObject *m_wordDocuments;
public:
    QAxWidget *m_wordWidget;

    WdParagraphAlignment m_paralignment; //文本对齐方式

};

#endif // QWORDENGINE_H
