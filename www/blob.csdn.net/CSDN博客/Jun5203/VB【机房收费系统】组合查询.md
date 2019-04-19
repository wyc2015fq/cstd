# VB【机房收费系统】组合查询 - Jun5203 - CSDN博客
2018年09月02日 15:47:34[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：88
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
 机房收费系统中共有三个窗体用到了组合查询，其也算是三大难点之一，我做的流程就是先根据源系统备好框架，然后再去网上看相关的博客，这么一来二往，多看几遍也渐渐懂得了其中的一些“套路”。组合查询，顾名思义大于等于一的行数据进行查询，下面我以学生基本信息维护的窗体为例来详细讲述。
![这里写图片描述](https://img-blog.csdn.net/20180902153236898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 看到这个流程图感觉很简单有没有，是呀，确实看着挺简单，但是代码就不是太简单呦，这也得看你怎么去思考这个过程，毕竟每个人的大脑发育情况都不太同。
![这里写图片描述](https://img-blog.csdn.net/20180902154132661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 我的想法就是第一行填写完整即可查询相应的记录，若选择第一个组合关系后，必须把第二行填写完整才可进行查询，否则弹出“提示”，同理第三行和第二个组合关系。
定义函数让数据库中的字段表示成文字
```
Public Function FiledName(StrFiledName As String) As String
    Select Case StrFiledName
    Case "卡号"
        FiledName = "cardno"
    Case "姓名"
        FiledName = "studentName"
    Case "上机日期"
        FiledName = "ondate"
    Case "上机时间"
        FiledName = "ontime"
    Case "下机日期"
        FiledName = "offdate"
    Case "下机时间"
        FiledName = "offtime"
    Case "消费金额"
        FiledName = "consume"
    Case "余额"
        FiledName = "cash"
    Case "备注"
        FiledName = "status"
    End Select
End Function
Public Function Relationname(strRelationname As String) As String
    Select Case strRelationname
    Case "与"
        Relationname = "and"
    Case "或"
        Relationname = "or"
    End Select
End Function
```
查询
```
Private Sub cmdOK_Click()
    txtSQL = "select * from Line_Info where "
   
    If Trim(cbo1.Text) = "" Or Trim(cbo4.Text) = "" Or Trim(txt1.Text) = "" Then
        MsgBox "请将第一行选项内容填写完整！", vbOKOnly + vbExclamation, "警告"
    Exit Sub
    End If
     
    txtSQL = txtSQL & " " & FiledName(cbo1.Text) & " " & Trim(cbo4.Text) & "'" & Trim(txt1.Text) & "'"
    
    If Trim(cbo7.Text) <> "" Then
    
    If Trim(cbo2.Text) = "" Or Trim(cbo5.Text) = "" Or Trim(txt2.Text) = "" Then
        MsgBox "请将第二行选项内容填写完整！", vbOKOnly + vbExclamation, "警告"
    Exit Sub
    Else
    
    txtSQL = txtSQL & " " & Relationname(cbo7.Text) & " " & FiledName(cbo2.Text) & " " & Trim(cbo5.Text) & "'" & Trim(txt2.Text) & "'"
    End If
  End If
    
    If Trim(cbo8.Text) <> "" Then
   
    If Trim(cbo3.Text) = "" Or Trim(cbo6.Text) = "" Or Trim(txt3.Text) = "" Then
        MsgBox "请将第三行选项内容填写完整！", vbOKOnly + vbExclamation, "警告"
    Exit Sub
    Else
    
    txtSQL = txtSQL & " " & Relationname(cbo8.Text) & " " & FiledName(cbo3.Text) & " " & Trim(cbo6.Text) & "'" & Trim(txt3.Text) & "'"
    End If
  End If
  
  Set mrc = ExecuteSQL(txtSQL, MsgText)
  
    If mrc.EOF = True Then
        MsgBox "没有您要查找的学生，请重新填写！", vbOKOnly + vbExclamation, "警告"
    Else
    With MSHFlexGrid1
        .CellAlignment = 4
        .Rows = 1
        .ColWidth(2) = 1900
        .ColWidth(4) = 1900
        .TextMatrix(0, 0) = "卡号"
        .TextMatrix(0, 1) = "姓名"
        .TextMatrix(0, 2) = "上机日期"
        .TextMatrix(0, 3) = "上机时间"
        .TextMatrix(0, 4) = "下机日期"
        .TextMatrix(0, 5) = "下机时间"
        .TextMatrix(0, 6) = "消费金额"
        .TextMatrix(0, 7) = "余额"
        .TextMatrix(0, 8) = "备注"
          
    Do While Not mrc.EOF
        .Rows = .Rows + 1
        .CellAlignment = 4
        .TextMatrix(.Rows - 1, 0) = mrc.Fields(1)
        .TextMatrix(.Rows - 1, 1) = mrc.Fields(3)
        .TextMatrix(.Rows - 1, 2) = mrc.Fields(6)
        .TextMatrix(.Rows - 1, 3) = mrc.Fields(7)
        .TextMatrix(.Rows - 1, 4) = mrc.Fields(8) & ""
        .TextMatrix(.Rows - 1, 5) = mrc.Fields(9) & ""
        .TextMatrix(.Rows - 1, 6) = mrc.Fields(11) & ""
        .TextMatrix(.Rows - 1, 7) = mrc.Fields(12) & ""
        .TextMatrix(.Rows - 1, 8) = mrc.Fields(13)
        mrc.MoveNext
        Loop
    End With
    End If
    mrc.Close
End Sub
```
 机房真的和学生差不多，翻来覆去就那么几套窗体，只要把每套的其中一个搞明白，其余的也都能顺利解决，但是要注意一定要细心、细心再细心~
