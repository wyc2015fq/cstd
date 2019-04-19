# c++操作EXCEL - u013366022的专栏 - CSDN博客
2015年10月13日 14:18:27[slitaz](https://me.csdn.net/u013366022)阅读数：466
**向Excel表中导出&导入数据**
**一、            向工程中添加Excel操作接口类**
1、  向项目工程中添加基本的7个类，分别是：CApplication，CMyFont，CRange，CWorkBook，CWorkBooks，CWorkSheet，CWorkSheets
1.1、打开Class Wizard，如下图
1.2、选择MFC ClassFrom TypeLib…，如下图
1.3、在Add Class From中选中File，在Location中输入或者选择EXCEL.EXE（officer2007为EXCEL.EXE，officer2003为EXCEL9.OLB）的路径。然后在Interface框（即1号框）中选择操作Excel的基本接口，分别是：_Application，_WorkBook，_WorkSheet， Range，Font，WorkBooks，WorkSheets。点击Generated classes（即2号框）中CFont0类，在classes（即3号框）和File（即4号框）中修改其类名为CMyFont，头文件名为CMyFont.h（类名和头文件名，也可以是其他名字，由自己确定）。点击Finish完成添加。，如下图
1.4、在Class View中可以看见添加进去的7个类，如下图
2、  修改这6个类
2.1、把CApplication.h，CMyFont.h，CRange.h，CWorkBook.h，CWorkBooks.h，CWorkSheet.h，CWorkSheets.h中的“#import "C:\\Program Files\\MicrosoftOffice\\Office12\\EXCEL.EXE" no_namespace”注释掉，然后添加头文件：#include <afxdisp.h>到上面7个文件中去。
3、  添加头文件到要对Excel进行操作的文件中去
4、双击error C2059，将VARIANT DialogBox()改成VARIANT _DialogBox()
再次编译，通过！！
**二、            数据导出到EXCEL**
1、  程序代码段 
//定义接口类变量
CApplication app;
    CWorkbookbook;
    CWorkbooksbooks;
    CWorksheetsheet;
    CWorksheetssheets;
    CRange range;
    CMyFont font;
    CRange cols;
COleVariant covOptional((long)
DISP_E_PARAMNOTFOUND,VT_ERROR);
if(!app.CreateDispatch(_T("Excel.Application")))
    {
    this->MessageBox(_T("无法创建Excel应用！"));
    return;
    }
    books =app.get_Workbooks();
    book = books.Add(covOptional);
    sheets = book.get_Worksheets();
    sheet = sheets.get_Item(COleVariant((short)1));
//获得坐标为（A，1）和（B，1）的两个单元格
    range = sheet.get_Range(COleVariant(_T("A1")),
COleVariant(_T("B1")));
//设置单元格类容为HelloExcel
range.put_Value2(COleVariant(_T("HelloExcel")));
//选择整列，并设置宽度为自适应
cols = range.get_EntireColumn();
    cols.AutoFit();
//设置字体为粗体
font = range.get_Font();
    font.put_Bold(COleVariant((short)TRUE));
//获得坐标为（C，2）单元格
    range = sheet.get_Range(COleVariant(_T("C2")),
COleVariant(_T("C2")));
//设置公式“=RAND()*100000”
    range.put_Formula(COleVariant(_T("=RAND()*100000")));
//设置数字格式为货币型
    range.put_NumberFormat(COleVariant(_T("$0.00")));
//选择整列，并设置宽度为自适应
    cols = range.get_EntireColumn();
    cols.AutoFit();
//显示Excel表
    app.put_Visible(TRUE);
    app.put_UserControl(TRUE);
**三、            数据从Excel导入到程序中**
1、  程序代码段
CApplication app;
CWorkbook book;
CWorkbooks books;
CWorksheet sheet;
CWorksheets sheets;
CRange range;
LPDISPATCH lpDisp;
//定义变量
COleVariant covOptional((long)
DISP_E_PARAMNOTFOUND,VT_ERROR);
if(!app.CreateDispatch(_T("Excel.Application")))
{
this->MessageBox(_T("无法创建Excel应用"));
return;
}
books =app.get_Workbooks();
//打开Excel，其中pathname为Excel表的路径名
lpDisp =books.Open(pathname,covOptional
,covOptional,covOptional,covOptional
,covOptional,covOptional,covOptional
,covOptional,covOptional,covOptional
,covOptional,covOptional,covOptional
,covOptional);
book.AttachDispatch(lpDisp);
sheets = book.get_Worksheets();
sheet = sheets.get_Item(COleVariant((short)1));
//获得坐标为（A，1）的单元格
range =sheet.get_Range(COleVariant(_T("A1"))
,COleVariant(_T("A1")));
//获得单元格的内容
COleVariant rValue;rValue = 
COleVariant(range.get_Value2());
//转换成宽字符
rValue.ChangeType(VT_BSTR);
//转换格式，并输出
this->MessageBox(CString(rValue.bstrVal));
book.put_Saved(TRUE);
app.Quit();

