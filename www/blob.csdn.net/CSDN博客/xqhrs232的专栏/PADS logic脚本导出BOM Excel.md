# PADS logic脚本导出BOM Excel - xqhrs232的专栏 - CSDN博客
2016年12月30日 09:35:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4391
原文地址::[http://blog.csdn.net/lyp19910608/article/details/50898448](http://blog.csdn.net/lyp19910608/article/details/50898448)
相关文章
1、pads 9.3导出BOM清单方法----[http://wenku.baidu.com/link?url=lo9EgNDKy7QR6qfHX5q0ukW-IVINjnafMX2ogxEp8enz1wWCrJSHezQZ5YpB4mlzmz7FBWFc4EOaGQzkhxDR-XhHPSsgUs7d_KvY5TCEiQO](http://wenku.baidu.com/link?url=lo9EgNDKy7QR6qfHX5q0ukW-IVINjnafMX2ogxEp8enz1wWCrJSHezQZ5YpB4mlzmz7FBWFc4EOaGQzkhxDR-XhHPSsgUs7d_KvY5TCEiQO)
2、excel表格如何按字母排序----[https://zhidao.baidu.com/question/104498885.html](https://zhidao.baidu.com/question/104498885.html)
3、[如何在Excel中按字母顺序排列](http://zh.wikihow.com/%E5%9C%A8Excel%E4%B8%AD%E6%8C%89%E5%AD%97%E6%AF%8D%E9%A1%BA%E5%BA%8F%E6%8E%92%E5%88%97)----[http://zh.wikihow.com/%E5%9C%A8Excel%E4%B8%AD%E6%8C%89%E5%AD%97%E6%AF%8D%E9%A1%BA%E5%BA%8F%E6%8E%92%E5%88%97](http://zh.wikihow.com/%E5%9C%A8Excel%E4%B8%AD%E6%8C%89%E5%AD%97%E6%AF%8D%E9%A1%BA%E5%BA%8F%E6%8E%92%E5%88%97)
版权声明：本文为博主原创文章，未经博主允许不得转载。
```
' PADS Logic BOM Output
' By lyp (337252561@qq.com)
Dim fn As String
Sub Main
    fn = ActiveDocument
    If fn = "" Then
        fn = "Untitled"
    End If
    tempFile = DefaultFilePath & "\temp.txt"
    Open tempFile For Output As #1
    item = 0
    StatusBarText = "Generating report..."
    Print #1, "ITEM";vbTab;"Part Type"; vbTab;"P/N_1"; vbTab;"Manufacturer_1_P/N"; vbTab;"Description"; vbTab;"Manufacturer_1"; vbTab; "Value"; vbTab; "QTY"; vbTab; "REF-DES"
    For Each pkg in ActiveDocument.PartTypes
        'Print #1, pkg.Name; vbTab; note
        qty = 0
        value = ""
        description = ""
        manufacturer = ""
        pn = ""
        manufacturerpn = ""
        symbol = ""
        item = item + 1
        'Print #1, item; vbTab;
        For Each part In pkg.Components
            value = AttrValue(part, "Value")
            description = AttrValue(part, "Description")
            manufacturer = AttrValue(part, "Manufacturer_1")
            pn = AttrValue(part, "P/N_1")
            value = AttrValue(part, "Value")    
            manufacturerpn = AttrValue(part, "Manufacturer_1_P/N")
            sysid = AttrValue(part, "SYSID")
            qty = qty+1
            symbol = symbol + part.Name + ", "
        Next        
        symbol_len = Len(symbol)
        symbol = Mid(symbol,1, symbol_len - 2)
        Print #1, item; vbTab; part.PartType ; vbTab; pn ; vbTab;manufacturerpn; vbTab;description; vbTab;manufacturer; vbTab;value; vbTab; qty; vbTab; symbol;
        Print #1
    Next pkg
    StatusBarText = ""
    Close #1
    ExportToExcel
End Sub
Sub ExportToExcel
    FillClipboard
    Dim xl As Object
    On Error Resume Next
    Set xl =  GetObject(,"Excel.Application")
    On Error GoTo ExcelError    ' Enable error trapping.
    If xl Is Nothing Then
        Set xl =  CreateObject("Excel.Application")
    End If
    xl.Visible = True
    xl.Workbooks.Add
    xl.ActiveSheet.Paste
    xl.Range("A1:I1").Font.Bold = True
    xl.Range("A1:I1").NumberFormat = "@"
    xl.Range("A1:I1").AutoFilter
    xl.ActiveSheet.UsedRange.Columns.AutoFit
    'Output Report Header
    xl.Rows(1).Insert
    xl.Rows(1).Cells(1) = Space(1) & "Part Report "& " WWZL-BOM " & " on " & Now
    xl.Rows(2).Insert
    xl.Rows(1).Font.bold = True
    'Output Design Totals
    lastRow = xl.ActiveSheet.UsedRange.Rows.Count + 1
    xl.Rows(lastRow + 1).Font.bold = True
    xl.Rows(lastRow + 1).Cells(1) = Space(1) & "Design Part count: " & ActiveDocument.Components.Count
    xl.Range("A1").Select
    On Error GoTo 0 ' Disable error trapping. 
    Exit Sub    
ExcelError:
    MsgBox Err.Description, vbExclamation, "Error Running Excel"
    On Error GoTo 0 ' Disable error trapping.    
    Exit Sub
End Sub
Sub FillClipboard
    StatusBarText = "Export Data To Clipboard..."
    ' Load whole file to string variable    
    tempFile = DefaultFilePath & "\temp.txt"
    Open tempFile  For Input As #1
    L = LOF(1)
    AllData$ = Input$(L,1)
    Close #1
    'Copy whole data to clipboard
    Clipboard AllData$ 
    Kill tempFile
    StatusBarText = ""
End Sub
Function AttrValue (comp As Object, atrName As String) As String
    If comp.Attributes(atrName) Is Nothing Then
        AttrValue = ""
    Else
        AttrValue = comp.Attributes(atrName).Value
    End If
End Function
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
脚本保存为：PADS_Logic_BOM_to_Excel.bas 
加载脚本步骤： 
![这里写图片描述](https://img-blog.csdn.net/20160315175645473)
Bom输出结果，有乱码是ASCII不包含特殊符号，安装open office解决： 
![这里写图片描述](https://img-blog.csdn.net/20160315174011951)
PS: 
1、文件->报告->材料清单->设置，可以导出BOM 
新建excel，pads全选复制到excel 
![这里写图片描述](https://img-blog.csdn.net/20160315180135676)
导出后用Beyond Compare 比较内容
[](http://blog.csdn.net/lyp19910608/article/details/50898448#)[](http://blog.csdn.net/lyp19910608/article/details/50898448#)[](http://blog.csdn.net/lyp19910608/article/details/50898448#)[](http://blog.csdn.net/lyp19910608/article/details/50898448#)[](http://blog.csdn.net/lyp19910608/article/details/50898448#)[](http://blog.csdn.net/lyp19910608/article/details/50898448#)

//=================================================================================================
备注::
1>原来pads logic里面的BOM表也可以直接copy出来粘贴到excel形成表格啊！这个以前都不知道啊！还一直去找脚本工具来弄！！！
不过直接copy出来的还是乱，没有按字母顺序排序，也许写脚本才是最好的解决方式！！！先这样凑合着用吧！还好板不复杂！！！
2>
 excel也可以调整为字母排序啊!!!原来是自己不会用excel!

