# VB.NET调用SQL Sever存储过程 - weixin_33985507的博客 - CSDN博客
2013年06月29日 21:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
概要：
本文介绍VB.NET使用创建并调用带有输入参数和输出参数的SQL Sever存储过程
本文代码首先检查要创建的存储过程再数据库中是否存在在。如果不存在，则代码创建一个存储过程，该过程一个参数来搜索用户的密码，并在输出参数中返回匹配的行及行数。
步骤：
1.      创建基本的用户界面
![](https://img-blog.csdn.net/20130628203116984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHlwZjE5OTAwOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.      创建GetUserID存储过程
```
create Procedure GetUserID (@username varchar(40), @RowCount int output)  
as 
select * from userlogin where adminUser = @username; 
/* @@ROWCOUNT 返回最后一个受影响的行数 */ 
select @RowCount=@@ROWCOUNT
```
此代码包括两个参数：@username和@RowCount。@username是输入参数，用于获取搜索字符串比便在userlogin中执行搜索功能。@RowCount是输出参数，用于使用@@ROWCOUNT变量获取受影响的行。
3.      编写窗体代码
3.1添加“声明”部分
```
Imports System.Data
Imports System.Data.SqlClient
```
3.2若要确保存储过程并创建存储过程，结合使用**SqlCommand**对象和**SqlDataReader**对象。可以使用**SqlCommand**针对数据库运行任何 SQL命令。然后调用 **SqlCommand**的**ExecuteReader**方法以返回**SqlDataReader**，它包含查询的匹配行。
```
Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim MyConnection As SqlConnection
        Dim MyCommand As SqlCommand
        Dim MyDataReader As SqlDataReader
        '设置连接语句
        MyConnection = New SqlConnection("Data Source=192.168.24.75;Initial Catalog=P_charge;User ID=sa;Pwd=123456")
        MyCommand = New SqlCommand
        '检查存储过程是否存在
        MyCommand = New SqlCommand("if object_id('pubs..GetUserID') is not null " + "begin" + " if objectproperty(object_id('pubs..GetUserID'), 'IsProcedure')= 1" + " select object_id('pubs..GetUserID')" + " else" + " return " + "end" + " else" + " return", MyConnection)
        '设置command命令按钮的属性
        With MyCommand
            .Connection = MyConnection
            .CommandType = CommandType.Text
            .Connection.Open()
            '执行sql语句
            MyDataReader = .ExecuteReader()
            '如果不存在那么就创建一个新的存储过程，如果存在就关闭数据库
            If MyDataReader.Read() Then
                .CommandText = "create procedure GetUserID (@username varchar(40), " & "@RowCount int output) " & " as select * from userlogin where adminUser = @username; select @RowCount=@@ROWCOUNT"
                MyDataReader.Close()
                .ExecuteNonQuery()
            Else
                MyDataReader.Close()
            End If
            '关闭数据库
            .Dispose()  'Dispose of the Command object.
            MyConnection.Close() 'Close the connection.
        End With
    End Sub
```
3.3在按钮的**Click**事件中调用存储过程，然后使用**SqlDataAdapter**对象运行存储过程。必须为该存储过程创建参数，并将它附加到**SqlDataAdapter**对象的**Parameters**集合中。
```
Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim DS As DataSet
        Dim MyConnection As SqlConnection
        Dim MyDataAdapter As SqlDataAdapter
        '连接数据库语句
        MyConnection = New SqlConnection("Data Source=192.168.24.75;Initial Catalog=userlogin;User ID=sa;Pwd=123456")
        '创建一个dataapater，然后给出存储过程的名字
        MyDataAdapter = New SqlDataAdapter("GetUserID", MyConnection)
        '设置命令类型是存储过程
        MyDataAdapter.SelectCommand.CommandType = CommandType.StoredProcedure
        '设置存储过程参数
        MyDataAdapter.SelectCommand.Parameters.Add(New SqlParameter("@username", SqlDbType.VarChar, 40))
        '指定存储过程过程参数
        MyDataAdapter.SelectCommand.Parameters("@username").Value = Trim(TxtLastName.Text)
        '设置存储过程参数 
        MyDataAdapter.SelectCommand.Parameters.Add(New SqlParameter("@RowCount", SqlDbType.Int, 4))
        '设置参数是输出参数
        MyDataAdapter.SelectCommand.Parameters("@RowCount").Direction = ParameterDirection.Output
        '创建一个新的记录集来保存数据
        DS = New DataSet()
        MyDataAdapter.Fill(DS, "userid") '返回的行填充到数据集
        '得到返回行数，赋给lblRowCount
        'lblRowCount.Text = DS.Tables(0).Rows.Count().ToString() & " Rows Found!"
        lblRowCount.Text = MyDataAdapter.SelectCommand.Parameters(1).Value & " Rows Found!"
        '设定DataGrid控件的数据源
        Grdauthors.DataSource = DS.Tables("userid").DefaultView
        '关闭dataapater和数据库
        MyDataAdapter.Dispose() 'Dispose of the DataAdapter.
        MyConnection.Close() 'Close the connection.
    End Sub
```
4． 运行结果
![](https://img-blog.csdn.net/20130628203455843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHlwZjE5OTAwOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
