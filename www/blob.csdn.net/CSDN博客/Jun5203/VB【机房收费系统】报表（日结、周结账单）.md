# VB【机房收费系统】报表（日结、周结账单） - Jun5203 - CSDN博客
2018年09月02日 12:20:53[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：352
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
####前言
 做机房的三大难点之一——“报表”即将“问世”，不知你准备好了吗？说实话，一开始我刚看到原系统日结、周结账单的时候真不知道它是个啥东西，自己天真的以为Text文本框里边加的内容，经站在巨人的肩膀上之后我终于懂得了其中的道理，原来它是一个软件和VB进行了交互。虽说网上步骤千万遍，但是自己总结一下也是很不错的呢！
####步骤
1、安装Grid++Report报表设计器；
2、插入页眉、页脚、报表头、报表尾、明细网络、静态框、综合文字框；
3、明细网络——数据库连接
![这里写图片描述](https://img-blog.csdn.net/20180902080208760?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080259518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080313397?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080325287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080803473?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018090208081938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080839646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180902080859508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、综合文字框——VB连接
![这里写图片描述](https://img-blog.csdn.net/20180902080908752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
综合文本框文本内“插入域”——选择数据域——参数，选择自己定义的参数，确定即可。
5、在报表上添加控件，设置参数的为综合文本框，系统变量的为系统变量框，其余固定显示的为静态框；
6、VB中插入部件Grid++Report Designer5.0 Type Library和Grid++Report Engine5.0 Type Library
7、选择GRDiplayViewer，在窗体中画出来；
8、代码
```
日结
Option Explicit
Dim WithEvents Report As grproLibCtl.GridppReport  '实例化报表
Dim txtSQL As String
Private Sub Command1_Click()
    GRDisplayViewer1.Refresh                                       '刷新
    
End Sub
Private Sub Command2_Click()
    Report.[Print] (True)   '打印，因为报表对象的print方法名与vb的内部定义有冲突所以要用中括号
End Sub
Private Sub Command3_Click()
    Report.PrintPreview (True)      '打印预览
End Sub
Private Sub Form_Load()     
    txtSQL = "select * from CheckDay_Info where date='" & Format(Date, "yyyy-mm-dd") & "'"
    Set Report = New grproLibCtl.GridppReport  '实例化模板
    
    Report.LoadFromFile (App.Path & "\日结报表.grf")               '加载模版
    Report.DetailGrid.Recordset.ConnectionString = ConnectionString() '连接数据源
    Report.DetailGrid.Recordset.QuerySQL = txtSQL                  '通过SELECT查询创建记录集
    
    Report.ParameterByName("管理员").AsString = UserName
    Report.ParameterByName("管理员").Value = UserName '显示管理员编号
    GRDisplayViewer1.Report = Report     '将数据赋给GRD的report
    GRDisplayViewer1.Start   '在报表中显示
End Sub
```
补充：若要在表中加入日期，比如说“制表时间”，套路其实一样，只不过在“选择数据域”中，引用类型换成“系统变量”，系统变量选择“CurrentDataTime（当前日期）”，如果是周结账单的一个区域的时间，记得设置参数哦~
```
周结
Option Explicit
Dim WithEvents Report As grproLibCtl.GridppReport  '实例化报表
Dim mrc As ADODB.Recordset
Dim mrc1 As ADODB.Recordset
Dim txtSQL As String
Dim MsgText As String
Private Sub Command1_Click()
    Report.[Print] (True)
End Sub
Private Sub Command2_Click()
    Report.PrintPreview (True)
End Sub
Private Sub Command3_Click()
  
    If DTPicker1.Value > DTPicker2.Value Then
        MsgBox "终止时间不能小于起始时间！", vbOKOnly + vbExclamation, "警告"
    Exit Sub
    End If
       
    txtSQL = "select * from CheckDay_Info where date between'" & Format$(DTPicker1.Value, "yyyy-mm-dd") & "'" & "and'" & Format$(DTPicker2.Value, "yyyy-mm-dd") & "'"
    Set mrc = ExecuteSQL(txtSQL, MsgText)
        
    txtSQL = "select * from CheckWeek_Info"
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
    
    Do While Not mrc1.EOF
        mrc1.Delete
        mrc1.MoveNext
    Loop
    
    Do While Not mrc.EOF
    With mrc1
        .AddNew
        .Fields(0) = mrc.Fields(0)
        .Fields(1) = mrc.Fields(1)
        .Fields(2) = mrc.Fields(2)
        .Fields(3) = mrc.Fields(3)
        .Fields(4) = mrc.Fields(4)
        .Fields(5) = mrc.Fields(5)
        .Update
        mrc.MoveNext
    End With
    Loop
        
    GRDisplayViewer1.Refresh
    
    Report.DetailGrid.Recordset.QuerySQL = "select * from checkWeek_Info where date between '" & DTPicker1.Value & "' and '" & DTPicker2.Value & "'"
    '获取始终时间
    Report.ParameterByName("StartDate").Value = Format$(DTPicker1.Value, "yyyy-mm-dd")
    Report.ParameterByName("EndDate").Value = Format$(DTPicker2.Value, "yyyy-mm-dd")
     '显示管理员编号
    Report.ParameterByName("管理员").Value = UserName
      
    
End Sub
Private Sub Form_Load()
    txtSQL = "select * from CheckDay_Info where date='" & Format(Date, "yyyy-mm-dd") & "'"
    Set Report = New grproLibCtl.GridppReport  '实例化模板
    
    Report.LoadFromFile (App.Path & "\周结报表.grf")               '加载模版
    Report.DetailGrid.Recordset.ConnectionString = ConnectionString() '连接数据源
    Report.DetailGrid.Recordset.QuerySQL = txtSQL                  '通过SELECT查询创建记录集
    
    Report.ParameterByName("管理员").AsString = UserName
    Report.ParameterByName("管理员").Value = UserName '显示管理员编号
    GRDisplayViewer1.Report = Report     '将数据赋给GRD的report
    GRDisplayViewer1.Start   '在报表中显示
    
End Sub
```
####小结
 分享的很多，希望可以帮到你们，越努力越幸运！
