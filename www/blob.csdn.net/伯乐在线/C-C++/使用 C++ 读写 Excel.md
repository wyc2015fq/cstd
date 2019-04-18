# 使用 C++ 读写 Excel - 文章 - 伯乐在线
原文出处： [wudong，2011-04-20](http://www.cnblogs.com/wdhust/archive/2011/04/20/2022963.html)
**1.导入Excel类型库**
使用Visual C++的扩展指令#import导入Excel类型库：


```
#import "C:\\Program Files\\Common Files\\microsoft shared\\OFFICE14\\MSO.DLL" 
    rename("RGB","MsoRGB") 
    rename("SearchPath","MsoSearchPath")
 
#import "C:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB"
 
#import "C:\\Program Files\\Microsoft Office\\Office14\\EXCEL.EXE" 
    rename( "DialogBox", "ExcelDialogBox" )
    rename( "RGB", "ExcelRGB" )
    rename( "CopyFile", "ExcelCopyFile" ) 
    rename( "ReplaceText", "ExcelReplaceText" ) 
    exclude( "IFont", "IPicture" ) no_dual_interfaces
```
#import指令会从指定的可执行文件，动态链接库等COM组件中导出类型库(type lib)，在Debug和Release临时目录中生成对应的类型库头文件(type lib header file)，以供C++程序使用。如以上三条指令在编译后会生成excel.tlh, mso.lh和vbetext.olb三个头文件，可以在Debug和Release目录中找到。
**2.访问Excel暴露的COM对象**
下面是一段比较完整的访问Excel的实例代码。首先用生成的数据填充单元格，然后用这些单元格的数据生成了一个图表(Chart):


```
try
{
    Excel::_ApplicationPtr pExcelApp;
    HRESULT hr = pExcelApp.CreateInstance(L"Excel.Application");
    ATLASSERT(SUCCEEDED(hr));
    pExcelApp->Visible = true;   // make Excel’s main window visible
 
    Excel::_WorkbookPtr pWorkbook = pExcelApp->Workbooks->Open(lpszPathName);  // open excel file
    Excel::_WorksheetPtr pWorksheet = pWorkbook->ActiveSheet;
    pWorksheet->Name = L"Chart Data";
    Excel::RangePtr pRange = pWorksheet->Cells;
 
    const int nplot = 100;
    const double xlow = 0.0, xhigh = 20.0;
    double h = (xhigh-xlow)/(double)nplot;
    pRange->Item[1][1] = L"x";  // read/write cell’s data
    pRange->Item[1][2] = L"f(x)";
    for (int i=0;i<nplot;++i)
    {
        double x = xlow+i*h;
        pRange->Item[i+2][1] = x;
        pRange->Item[i+2][2] = sin(x)*exp(-x);
    }
 
    Excel::RangePtr pBeginRange = pRange->Item[1][1];
    Excel::RangePtr pEndRange = pRange->Item[nplot+1][2];
    Excel::RangePtr pTotalRange = 
        pWorksheet->Range[(Excel::Range*)pBeginRange][(Excel::Range*)pEndRange];
    Excel::_ChartPtr pChart = pExcelApp->ActiveWorkbook->Charts->Add();
    // refer to :
    // http://msdn.microsoft.com/en-us/library/microsoft.office.tools.excel.chart.chartwizard(v=vs.80).aspx
    pChart->ChartWizard(
        (Excel::Range*)pTotalRange,
        (long)Excel::xlXYScatter,
        6L,
        (long)Excel::xlColumns,
        1L,1L,
        true,
        L"My Graph",
        L"x",L"f(x)");
    pChart->Name = L"My Data Plot";
 
    pWorkbook->Close(VARIANT_TRUE);  // save changes
    pExcelApp->Quit();
}
catch (_com_error& error)
{
    ATLASSERT(FALSE);
    ATLTRACE2(error.ErrorMessage());
}
```
在这段代码中，Excel::_ApplicationPtr , Excel::_WorkbookPtr 和 Excel::_WorksheetPtr 等均是Visual C++ 编译器根据#import指令自动生成的智能指针，实际上就是C++模板类_com_ptr_t的typedef，其定义可在excel.tlh等类型库头文件中找到。
另外，由于#import指令中没有指定raw_interface_only修饰符，Visual C++对Excel的COM接口进行了适当的封装，以简化COM接口属性和方法的调用，并且将HRESULT返回值都转换成了C++异常，因此，上面的这段代码不需要每一步都坚持HRESULT，而是改为捕获C++异常。
