# 机房收费——Excel打开调整与注册控件的实现 - Big Smile - CSDN博客
2015年08月11日 11:23:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：740标签：[vb编程																[excel																[创新](https://so.csdn.net/so/search/s.do?q=创新&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=vb编程&t=blog)
个人分类：[机房收费](https://blog.csdn.net/tr1912/article/category/5715155)
** 一、Excel的调用     **
        在vb中调用excel其实也不是难事，但是不知道怎么调用还真的是不会写程序，所以上网上搜索了一下就有了如下的答案：
首先需要引用excel的库文件：
![](https://img-blog.csdn.net/20150811112737225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
不同的office版本后面的数字是不一样的
然后在按钮的click事件下写下如下命令：
```
Dim i As Integer
    Dim j As Integer
    Dim Excelapp As Excel.Application
    Dim Excelbook As Excel.Workbook
    Dim Excelsheet As Excel.Worksheet
    
    Set Excelapp = New Excel.Application
    Set Excelbook = Excelapp.Workbooks.Add
    Set Excelsheet = Excelbook.Worksheets(1)
    Excelsheet.Activate
    DoEvents
    With MSFresult
        For i = 0 To .Rows - 1
            For j = 0 To .Cols - 1
                DoEvents
                Excelapp.ActiveSheet.Cells(i + 1, j + 1) = .TextMatrix(i, j)
            Next j
        Next i
    End With
    Excelapp.Workbooks.Open
    Excelapp.ActiveWorkbook.SaveAs App.Path & "\表格\上机记录.xlsx"
    Excelapp.ActiveWorkbook.Save
    Excelapp.Quit
    MsgBox "导出完成！", vbInformation, "提示"
```
需要注意的是“表格”这个文件夹是必须得建立的
然后注意到了其实他是自动保存文件，并没有让用户选择，所以经过一段时间的查阅就得到了如下代码：
```
Dim i As Integer
    Dim j As Integer
    Dim Excelapp As Excel.Application
    Dim Excelbook As Excel.Workbook
    Dim Excelsheet As Excel.Worksheet
    
    Set Excelapp = New Excel.Application
    Set Excelbook = Excelapp.Workbooks.Add
    Set Excelsheet = Excelbook.Worksheets(1)
    Excelapp.Visible = True
    Excelsheet.Activate
    DoEvents
    With MSFInfo
        For i = 0 To .Rows - 1
            For j = 0 To .Cols - 1
                DoEvents
                Excelapp.ActiveSheet.Cells(i + 1, j + 1) = .TextMatrix(i, j)
            Next j
        Next i
    End With
    Excelapp.Cells.EntireColumn.AutoFit
```
这样就实现了打开Excel文档，让用户自己选择保存的位置，其中`Excelapp.Cells.EntireColumn.AutoFit`为自动调整单元格的大小的作用的
**二、控件自动注册的实现**
        由于程序的发布需要在任何一台机子上都可以直接运行，所以就应该把控件注册的步骤省略到程序中，于是有了以下的过程
**下面为控件注册的实现**
首先把要注册的文件放到程序根目录下
![](https://img-blog.csdn.net/20150811114020188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在登陆窗体的加载事件中写入
```
Dim a As Integer
    a = GetSetting("JFMsi", "dllRegister", "Value", 0)
    If a = 0 Then
        Shell "cmd.exe /c eacho off"
        Shell "cmd.exe /c regsvr32 grdes50.dll /s"
        Shell "cmd.exe /c regsvr32 gregn50.dll /s"
        Shell "cmd.exe /c regsvr32 MSCOMCTL.OCX /s"
        SaveSetting "JFMsi", "dllRegister", "Value", 1
    End If
```
其中GetSetting 和 SaveSetting 为注册表的操作函数，详情请见：[http://blog.sina.com.cn/s/blog_5479e1ae0100eh9h.html](http://blog.sina.com.cn/s/blog_5479e1ae0100eh9h.html)
基本的思路就是利用注册表来记忆是否运行过此程序，以判断是否注册过控件，如果没有运行过则运行CMD的命令进行注册，需要注意的是在UAC设置等级较高的系统里必须用管理员权限运行才可以。
[](http://blog.sina.com.cn/s/blog_5479e1ae0100eh9h.html)
