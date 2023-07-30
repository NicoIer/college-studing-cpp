
#include "qwordengine.h"

QWordEngine::QWordEngine(QObject *parent)
    : QObject(parent),m_bOpened(false)
{

}

QWordEngine::QWordEngine( QString filename,QObject *parent/*= NULL*/ )
    :QObject(parent), m_filename(filename) ,m_bOpened(false)
{

}

QWordEngine::~QWordEngine()
{
    close();
}

bool QWordEngine::open( bool bvisable )
{
    m_bOpened = false;
    m_wordWidget = new QAxWidget;
    bool bFlag = m_wordWidget->setControl("kwps.Application" );
    if(!bFlag)
    {
        // 用wps打开
        bFlag = m_wordWidget->setControl( "Word.Application" );
        if(!bFlag)
        {
            return m_bOpened;

        }
    }
    m_wordWidget->setProperty("Visible", false);
    m_wordWidget->setProperty("DisplayAlerts",false);
    //获取所有的工作文档
    QAxObject *document = m_wordWidget->querySubObject("Documents");
    if(!document)
    {
        return m_bOpened;
    }
    //新建一个文档页
    document->dynamicCall("Open(const QString&)",m_filename);

    //获取当前激活的文档
    m_wordDocuments = m_wordWidget->querySubObject("ActiveDocument");

    if (m_wordDocuments)
        m_bOpened = true;
    else
        m_bOpened = false;

    return m_bOpened;
}

bool QWordEngine::open( const QString& strFile, bool bVisable /*= false*/ )
{
    m_filename = strFile;
    close();
    return open(bVisable);
}

bool QWordEngine::close()
{
    if (m_bOpened)
    {
        if (m_wordDocuments)
        {
            m_wordDocuments->dynamicCall("Close (boolean)", true);
        }
        if(m_wordWidget)
        {
            m_wordWidget->dynamicCall("Quit()");//退出word
            m_wordWidget->close();
        }

        if(m_wordDocuments)
            delete m_wordDocuments;
        if(m_wordWidget)
            delete m_wordWidget;
        m_bOpened = false;
    }

    return m_bOpened;
}

bool QWordEngine::isOpen()
{
    return m_bOpened;
}

void QWordEngine::save()
{
    m_wordDocuments->dynamicCall("SaveAs(const QString&)",m_filename);
    m_wordDocuments->dynamicCall("Close (boolean)", true);
}

bool QWordEngine::saveAs(const QString& strSaveFile)
{
    return m_wordDocuments->dynamicCall("SaveAs (const QString&)",
                                        strSaveFile).toBool();
}




bool QWordEngine::addText( QString titlestr,TITLE_NUMBER number /*= NORMAL*/,WdParagraphAlignment alignment /*= AlignParLeft*/ )
{
    if (!m_bOpened) return false;

    QAxObject *selection = NULL;
    selection = m_wordWidget->querySubObject("Selection");
    if (selection)
    {
        selection->querySubObject("Range")->setProperty("Text", titlestr);
        selection->querySubObject("Range")->dynamicCall("SetStyle(QVariant)", getTitleStr(number));

        //selection->querySubObject("ParagraphFormat")->setProperty("Alignment",alignment); //文本位置设置
        //背景色
        //selection->querySubObject("Range")
        //	->querySubObject("ParagraphFormat")
        //	->querySubObject("Shading")
        //	->setProperty("BackgroundPatternColor",QColor("blue").value());
        moveToEnd();
        return true;
    }
    return false;

}

bool QWordEngine::addText( QString titlestr,QFont font,QColor fontcolor )
{
    if (!m_bOpened) return false;

    QAxObject *selection = NULL;
    selection = m_wordWidget->querySubObject("Selection");
    if (selection)
    {
        //selection->querySubObject("Range")->querySubObject("Font")->setProperty("Size", fo); //不好用
        //selection->querySubObject("Range")->querySubObject("Font")->dynamicCall("Size", 20);
        selection->querySubObject("Range")->querySubObject("Font")->setProperty("Size", QVariant(font.pointSize()));
        selection->querySubObject("Range")->querySubObject("Font")->setProperty("Color", colorToInt(fontcolor));

        if (font.weight() >= QFont::Bold)
        {
            selection->querySubObject("Range")->querySubObject("Font")->setProperty("Bold", true);
        }

        selection->querySubObject("Range")->setProperty("Text", titlestr); //方式1
        //selection->dynamicCall("TypeText(const QString&)",titlestr);         //方式2 使用方法2不能设置字体


        moveToEnd();
        return true;
    }
    return false;
}

QAxObject* QWordEngine::addText2( QString titlestr )
{
    QAxObject *selection = NULL;
    if (!m_bOpened) return selection;

    selection = m_wordWidget->querySubObject("Selection");
    if (selection)
    {
        selection->querySubObject("Range")->setProperty("Text", titlestr); //方式1
    }
    return selection;
}


bool QWordEngine::insertEnter()
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    if (selection)
    {
        selection->dynamicCall("TypeParagraph(void)");
        return true;
    }
    return false;
}

bool QWordEngine::moveToEnd()
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    QVariantList params;
    params << wdStory << 0;
    if (selection)
    {
        selection->dynamicCall("EndOf(QVariant&, QVariant&)", params);
        return true;
    }

    return false;
}

bool QWordEngine::moveToEnd( MOVEEND_INDEX wd )
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    QVariantList params;
    params << wd << 0;
    selection->dynamicCall("EndOf(QVariant&, QVariant&)", params);
    return true;
}

bool QWordEngine::moveToEnd( QAxObject *table )
{
    if (!table) return false;
    moveToEnd(wdTable);
    moveToEnd();
    return true;
}



QAxObject* QWordEngine::createTable( int row, int column,QStringList headList )
{
    QAxObject* selection = m_wordWidget->querySubObject("Selection");
    if (!selection) return NULL;
    selection->dynamicCall("InsertAfter(QString&)", "\r\n");
    QAxObject *range = selection->querySubObject("Range");
    QAxObject *tables = m_wordDocuments->querySubObject("Tables");
    QAxObject *table = tables->querySubObject("Add(QVariant,int,int)",range->asVariant(),row,column);
    table->setProperty("Style","网格型");
    //表格自动拉伸列 0固定  1根据内容调整  2 根据窗口调整
    table->dynamicCall("AutoFitBehavior(WdAutoFitBehavior)", 2);
    for(int i=0;i<headList.size();i++)
    {
        table->querySubObject("Cell(int,int)",1,i+1)->querySubObject("Range")->dynamicCall("SetText(QString)", headList.at(i));
        table->querySubObject("Cell(int,int)",1,i+1)->querySubObject("Range")->dynamicCall("SetBold(int)", true);
    }



    return table;

}


QAxObject* QWordEngine::insertTable(int nStart, int nEnd, int row, int column)
{

    QAxObject* ptst = m_wordDocuments->querySubObject( "Range( Long, Long )",
                                                      nStart, nEnd );
    QAxObject* pTables = m_wordDocuments->querySubObject( "Tables" );
    QVariantList params;
    params.append(ptst->asVariant());
    params.append(row);
    params.append(column);
    if( pTables )
    {
        QAxObject *table = pTables->querySubObject( "Add(QAxObject*, Long ,Long )",params);
        table->dynamicCall("AutoFitBehavior(WdAutoFitBehavior)", 2);
        return table;
    }

    return NULL;
}

void QWordEngine::setCellString(QAxObject *table, int row, int column, const QString& text)
{
    if(table)
    {
        QAxObject *cell = table->querySubObject("Cell(int, int)",row+1,column+1);
        QAxObject *range = table->querySubObject("Range");
        range->dynamicCall("SetText(QString)", text);
        range->dynamicCall("SetBold(int)", false);
    }
}


void QWordEngine::MergeCells(QAxObject *table, int nStartRow,int nStartCol,int nEndRow,int nEndCol)
{
    QAxObject* StartCell =table->querySubObject("Cell(int, int)",nStartRow+1,nStartCol+1);
    QAxObject* EndCell = table->querySubObject("Cell(int, int)",nEndRow+1,nEndCol+1);
    StartCell->dynamicCall("Merge(LPDISPATCH)",EndCell->asVariant());
}

/******************************************************************************
 * 函数：setColumnWidth
 * 功能：设置表格列宽
 * 参数：table 表格; column 列数; width 宽度
 *****************************************************************************/
void QWordEngine::setColumnWidth(QAxObject *table, int column, int width)
{
    table->querySubObject("Columns(int)", column+1)->setProperty("Width", width);
}



/******************************************************************************
 * 函数：addTableRow
 * 功能：为表格添加行
 * 参数：table 表格; nRow 插入行; rowCount 插入的行数
 *****************************************************************************/
void QWordEngine::addTableRow(QAxObject *table, int nRow, int rowCount)
{
    QAxObject* rows = table->querySubObject("Rows");
    int Count = rows->dynamicCall("Count").toInt();

    if (nRow > Count)
    {
        nRow = Count;
    }
    QAxObject* row = table->querySubObject("Rows(int)",nRow);
    if (row == NULL)
    {
        row = rows->querySubObject("Last");
    }
    if(0 <= nRow && nRow <=Count)
    {
        for(int i = 0; i < rowCount; ++i)
        {
            rows->dynamicCall("Add(QVariant)", row->asVariant());
        }
    }
}


void QWordEngine::appendTableRow( QAxObject *table,int rowCount )
{
    QAxObject* rows = table->querySubObject("Rows");

    int Count = rows->dynamicCall("Count").toInt();
    QAxObject* row = rows->querySubObject("Last");

    for(int i = 0; i < rowCount; ++i)
    {
        QVariant param = row->asVariant();
        rows->dynamicCall("Add(Variant)", param);
    }
}


/******************************************************************************
 * 函数：setCellFontBold
 * 功能：设置内容粗体  isBold控制是否粗体
 * 参数：table 表格; row 插入行; column 列数; isBold 是否加粗
 *****************************************************************************/
void QWordEngine::setCellFontBold(QAxObject *table, int row, int column, bool isBold)
{
    table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")
        ->dynamicCall("SetBold(int)", isBold);
}

/******************************************************************************
 * 函数：setCellFontSize
 * 功能：设置文字大小
 * 参数：table 表格; row 插入行; column 列数; size 字体大小
 *****************************************************************************/
void QWordEngine::setCellFontSize(QAxObject *table, int row, int column, int size)
{
    table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")
        ->querySubObject("Font")->setProperty("Size", size);
}

/******************************************************************************
 * 函数：insertCellPic
 * 功能：在表格中插入图片
 * 参数：table 表格; row 插入行; column 列数; picPath 图片路径
 *****************************************************************************/
void QWordEngine::insertCellPic(QAxObject *table, int row, int column,
                                const QString& picPath)
{
    QAxObject* range = table->querySubObject("Cell(int, int)", row, column)
                           ->querySubObject("Range");
    range->querySubObject("InlineShapes")
        ->dynamicCall("AddPicture(const QString&)", picPath);
}




void QWordEngine::insertPic( QString picPath )
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment", "wdAlignParagraphCenter");
    QVariant tmp = selection->asVariant();
    QList<QVariant>qList;
    qList<<QVariant(picPath);
    qList<<QVariant(false);
    qList<<QVariant(true);
    qList<<tmp;
    QAxObject *Inlineshapes = m_wordDocuments->querySubObject("InlineShapes");
    Inlineshapes->dynamicCall("AddPicture(const QString&, QVariant, QVariant ,QVariant)",qList);
}



void QWordEngine::setColor( QColor color )
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    setColor(selection,color);
    //if (selection)
    //{
    //	selection->querySubObject("Range")->querySubObject("Font")->setProperty("Color", QVariant(color.value()));
    //	selection->querySubObject("Range")->setProperty("Text", "");
    //}
}

void QWordEngine::setColor( QAxObject *obj,QColor color )
{
    if (!obj) return;
    obj->querySubObject("Range")
        ->querySubObject("ParagraphFormat")
        ->querySubObject("Shading")
        ->setProperty("ForegroundPatternColor",colorToInt(color));
}


void QWordEngine::setBgColor( QAxObject *obj,QColor color )
{
    if (!obj) return;
    obj->querySubObject("Range")
        ->querySubObject("ParagraphFormat")
        ->querySubObject("Shading")
        ->setProperty("BackgroundPatternColor",colorToInt(color));
}



//设置对齐方式
void QWordEngine::setAlignment(int index)
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    if (!selection) return;
    selection->querySubObject("ParagraphFormat")->setProperty("Alignment",index);
}

void QWordEngine::setFontSize( int size )
{
    QAxObject *selection = m_wordWidget->querySubObject("Selection");
    if (!selection) return;
    selection->querySubObject("Font")->setProperty("Size",size);
}

QString QWordEngine::getTitleStr( TITLE_NUMBER number )
{
    QString str;
    switch (number)
    {
    case TITLE_ONE : str = "标题 1"; break;
            case TITLE_TWO : str = "标题 2"; break;
            case TITLE_THREE : str = "标题 3"; break;
            default: str = "正文";break;
    }
    return str;
}


void QWordEngine::setPropraty( QAxObject *axobj,QString proname,QVariant provalue )
{
    if (!axobj) return;

    axobj->setProperty(proname.toStdString().c_str(),proname);

}

int QWordEngine::colorToInt( QColor color )
{
    int sum = 0;
    int r = color.red() << 16;
    int g = color.green() << 8;
    int b = color.blue() ;
    int al = color.alpha() <<24;

    sum = al + r + g + b;
    return sum;
}

void QWordEngine::writeFile( QString savestr,QString filename )
{
    QFile savefile(filename);
    savefile.open(QFile::WriteOnly);
    QTextStream saveteam(&savefile);
    saveteam.setEncoding(QStringConverter::Encoding::Utf8);
    saveteam << savestr;
    savefile.close();
}

