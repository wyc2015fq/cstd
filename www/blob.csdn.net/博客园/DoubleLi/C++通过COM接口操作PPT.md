# C++通过COM接口操作PPT - DoubleLi - 博客园






## 一、 背景说明

在VS环境下，开发C++代码操作PPT，支持对PPT模板的修改。包括修改文本标签、图表、表格。满足大多数软件生成PPT报告的要求，先手工创建好PPT模板，在程序中修改模板数据。

## 二、 开发环境构建

通过VS2012的Class Wizard创建PowerPoint和Excel的COM接口；由于需要操作PPT中的图表，而图表的数据使用Excel存储的，需要修改图表的数据就得生成Excel的COM接口。

# 1.1 进入类向导

![这里写图片描述](http://img.blog.csdn.net/20150503164252372)

# 1.2 添加PowerPoint COM接口

![这里写图片描述](http://img.blog.csdn.net/20150503164613737)

![这里写图片描述](http://img.blog.csdn.net/20150503164646107)

# 1.3 添加Excel COM接口

![这里写图片描述](http://img.blog.csdn.net/20150503164755278)
选中所有的COM接口，生成接口文件。

## 三、 定义PPT文件基础操作函数(头文件略)

# 3.1 定义PPT应用基础对象

```php
class CPPTObject
{
public:
    CApplication m_PPTApp;
    CSlides m_Slides;
    CSlide m_curSlide;
    CPresentation m_Presentaion;
};
```
- 

# 3.2 启动PowerPoint软件，调用COM接口需要安装Office

```
// 创建PPT应用，启动powerpoint程序。
bool CPPTUtil::CreatePPTApplication()
{
    COleException exception;

    LPCSTR str = "Powerpoint.Application";
    if(!m_pPPTObject->m_PPTApp.CreateDispatch(str, &exception))
    {
        AfxMessageBox(exception.m_sc, MB_SETFOREGROUND);
        return false;
    }

    m_pPPTObject->m_PPTApp.put_Visible(true);
    return true;
}
```

# 3.3 打开PPT模板文件。修改PPT内容前，先打开PPT。

```
// 打开模板ppt。
bool CPPTUtil::OpenPPT(const std::string& pptPath)
{
    CPresentations presentations = m_pPPTObject->m_PPTApp.get_Presentations();
    m_pPPTObject->m_Presentaion = presentations.Open(CString(pptPath.c_str()), 0, 0, 1);

    m_pPPTObject->m_Slides = m_pPPTObject->m_Presentaion.get_Slides();

    return true;
}
```
- 

# 3.4 保存PPT文件内容，关闭文件，退出PowerPoint程序。

```
// 关闭PPT，保存数据关闭。
bool CPPTUtil::ClosePPT()
{
    m_pPPTObject->m_Presentaion.Save();
    m_pPPTObject->m_Presentaion.Close();
    m_pPPTObject->m_PPTApp.Quit();
    return true;
}
```

# 3.5 选中具体的PPT幻灯片。

```
// 选中PPT指定索引的幻灯片。
bool CPPTUtil::SelectSlide(long slideIndex)
{
    if (slideIndex > m_pPPTObject->m_Slides.get_Count())
    {
        return false;
    }

    m_pPPTObject->m_curSlide = m_pPPTObject->m_Slides.Range(COleVariant(slideIndex));

    return true;
}
```

## 四、 修改文本编辑框函数

```cpp
// 修改文本框
bool CPPTUtil::ModifyTextBox(const std::string& boxName, const std::string& strValue)
{
    CShapes shapes = m_pPPTObject->m_curSlide.get_Shapes();
    for(long i = 1; i <= shapes.get_Count(); ++i)
    {
       CShape shape(shapes.Item(COleVariant(i)));
       CString name = shape.get_Name();

       if(shape.get_Type() == (long)Office::msoTextBox
           && name.Compare(CString(boxName.c_str())) == 0) 
       {
            CTextFrame textFrame = shape.get_TextFrame();
            CTextRange textRange = textFrame.get_TextRange();
            CString txt = textRange.get_Text();

            textRange.put_Text(strValue.c_str());
       }
    }

    return true;
}
```

boxName对应于PPT中的Shape Name。这个Shape Name貌似在PowerPoint中没有地方能看到，也没有办法修改。只能在调试时记录下来。

## 五、 修改PPT中的图表函数。先在PPT中定义图表模板，通过COM接口修改图表数据。

# 5.1 定义图表数据结构。图表的数据都是用Excel存储的。

5.1.1 定义单元格[数据结构](http://lib.csdn.net/base/datastructure)

```cpp
CCellDataCom::CCellDataCom(const CellValueType valueType, const std::string& strValue, 
            const int iRow, const int iCol)
{
    m_ValueType = valueType;
    m_strValue = strValue;

    m_strPos = indexToString(iRow, iCol);
}

// 获取单元格值类型
CellValueType CCellDataCom::getValueType()
{
    return m_ValueType;
}

// 获取字符串类型值
const std::string& CCellDataCom::getStringValue()
{
    return m_strValue;
}

// 获取整型值
long CCellDataCom::getLongValue()
{
    return atol(m_strValue.c_str());
}

// 获取浮点类型值
double CCellDataCom::getDoubleValue()
{
    return atof(m_strValue.c_str());
}

// 获取单元格位置名称
const std::string& CCellDataCom::getPos()
{
    return m_strPos;
}

// 将单元格坐标转换名称字符串
CString CCellDataCom::indexToString( int row, int col )   
{   
    CString strResult;  
    if( col > 26 )   
    {   
        strResult.Format(_T("%c%c%d"),'A' + (col-1)/26-1,'A' + (col-1)%26,row);  
    }   
    else   
    {   
    strResult.Format(_T("%c%d"), 'A' + (col-1)%26,row);  
    }   

    return strResult;  
}   
5.1.2   定义图表数据结构
// 插入一行记录
void CChartDataCom::insertRowData(const std::list<CCellDataCom>& lstRowData)
{
    m_lstValue.push_back(lstRowData);
}

// 获取图表数据
const std::list<std::list<CCellDataCom> >& CChartDataCom::getValue()const
{
    return m_lstValue;
}
```

# 5.2 修改图表数据函数

// 修改图表

```cpp
bool CPPTUtil::ModifyChart(const std::string& chartName, const CChartDataCom& chartData)
{
    CShapes shapes = m_pPPTObject->m_curSlide.get_Shapes();
    for(long i = 1; i <= shapes.get_Count(); ++i)
    {
        CShape shape(shapes.Item(COleVariant(i)));
        if(shape.get_Type() != (long)Office::msoChart
            || chartName != std::string(shape.get_Name().GetBuffer()))
        {
            continue;
        }

        // 修改图表数据
        return ModifyChartData(shape.get_Chart(), chartData);
    }

    return false;
}

// 修改图表数据
bool CPPTUtil::ModifyChartData(CChart chart, const CChartDataCom& chartData)
{
    // 激活图表组件的excel数据表格，打开内嵌的excel.
    CChartData chartDataModel = chart.get_ChartData();
    chartDataModel.Activate();

    CWorkbook workBook = chartDataModel.get_Workbook();
    CWorksheets sheets = workBook.get_Worksheets();
    if(sheets.get_Count() == 0)
    {
        return false;
    }

    // 获取第一个sheet， 图表组件的数据都在内嵌excel的第一个sheet页。
    VARIANT vaSheetIndex;
    vaSheetIndex.vt = VT_I4;
    vaSheetIndex.lVal = 1;
    CWorksheet sheet = sheets.get_Item(vaSheetIndex);

    bool bRet = true;

    // 循环修改单元格的数据
    const std::list<std::list<CCellDataCom> >& lstValue = chartData.getValue();
    std::list<std::list<CCellDataCom> >::const_iterator iterAllData = lstValue.begin();
    for(; iterAllData != lstValue.end(); ++iterAllData)
    {
        std::list<CCellDataCom>::const_iterator iterRowData = iterAllData->begin();
        for(; iterRowData != iterAllData->end(); ++iterRowData)
        {
            bRet = ModifyCellData(sheet, *iterRowData);
            if(bRet == false)
            {
                break;
            }
        }

        if(bRet == false)
        {
            break;
        }
    }

    // 关闭Excel
    CApplication0 app0 = workBook.get_Application();
    app0.Quit();
    Sleep(2000);

    return bRet;
}

// 修改单元格数据
bool CPPTUtil::ModifyCellData(CWorksheet sheet, CCellDataCom cellData)
{
    const std::string& cellPos = cellData.getPos();
    CRange range = sheet.get_Range(COleVariant(cellPos.c_str()), COleVariant(cellPos.c_str()));

    COleVariant* pOleVar = NULL;
    if(cellData.getValueType() == CELL_STRING_TYPE)
    {
        pOleVar = new COleVariant(CString(cellData.getStringValue().c_str()));
    }
    else if(cellData.getValueType() == CELL_LONG_TYPE)
    {
        pOleVar = new COleVariant(cellData.getLongValue());
    }
    else if(cellData.getValueType() == CELL_DOUBLE_TYPE)
    {
        pOleVar = new COleVariant(cellData.getDoubleValue());
    }
    else
    {
        return false;
    }

    range.put_Value2(*pOleVar);
    delete pOleVar;

    return true;
}
```

## 六、 合并多个PPT文件函数

```cpp
// 合并PPT
bool CPPTUtil::MergePPT(const std::string& outputPPTPath, const std::list<std::string>& lstMergePPTPath)
{
    CApplication pptApp;
    COleException exception;

    // 打开PowerPoint程序
    LPCSTR str = "Powerpoint.Application";
    if(!pptApp.CreateDispatch(str, &exception))
    {
        AfxMessageBox(exception.m_sc, MB_SETFOREGROUND);
        return false;
    }

    pptApp.put_Visible(true);

    // 打开输出文件
    CPresentations presentations = pptApp.get_Presentations();
    CPresentation outPresention = presentations.Open(CString(outputPPTPath.c_str()), 0, 0, 1);

    // 循环打开合并文件插入PPT页面
    std::list<std::string>::const_iterator iterMergeFile = lstMergePPTPath.begin();
    for(; iterMergeFile != lstMergePPTPath.end(); ++iterMergeFile)
    {
        CPresentation mergePresention = presentations.Open(CString(iterMergeFile->c_str()), 0, 0, 1);
        CSlides mergeSlides = mergePresention.get_Slides();
        int pageNum = mergeSlides.get_Count();
        mergePresention.Close();

        // 合并PPT页签
        CSlides outSlides = outPresention.get_Slides();
        outSlides.InsertFromFile(CString(iterMergeFile->c_str()), outSlides.get_Count(), 1, pageNum);
    }

    outPresention.Save();
    outPresention.Close();
    pptApp.Quit();

    return true;
}
```









