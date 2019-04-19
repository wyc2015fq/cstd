# 三层VB.NET版的实现 - Big Smile - CSDN博客
2015年12月27日 21:22:05[王啸tr1912](https://me.csdn.net/tr1912)阅读数：992
        其实呢，在敲三层额时候，C#版的很快就敲完了，但是VB.NET版的却一直没有动，主要原因是因为自己感觉用VB.NET写有关数据库的操作比较难，另外VB.NET的视频没有好好的看，所以就没有很积极的想要去完成VB.NET版的三层，而是在中间借了好几本书来看，到底VB.NET是个什么东西，看完基础之后发现和VB简直就是一个样子，没有什么值得看的，再看和ADO.NET的数据连接就发现，真的是好难啊，没有什么具体的思路。所以就在网上搜索了一个完成的比较不错的VB.NET三层的例子来看的，发现和C#版的例子又有很多相似的地方，所以写此博客来比较一下。
首先来看声明和实例化对象：
|VB|C#|
|----|----|
|```Dim userName as stringuserName=txtUsername.Text.Trim()```|`string userName = txtUsername .Text .Trim ();`|
|`Dim Deal1 As New Login.BLL.LoginService`|`Login.BLL.LoginService svr = new Login.BLL.LoginService();`|
|`MsgBox("登录用户：" + user1.UserName, , "恭喜")`|`MessageBox.Show("登录用户：" + user.UserName );`|
其次是对方法或者类的声明：
|VB|C#|
|----|----|
|`Public Function UserLogin(User1 As Login.Model.UserInfo) As Login.Model.UserInfo`|`public Login.Model.UserInfo  UserLogin(string userName, string password)`|
|`user.UserName <> ""`|`user != null`|
|```Public Property UserName As String        Get            Return _username        End Get        Set(value As String)            _username = value        End SetEnd Property```|`public string UserName { get; set; }`|
具体到了数据库连接的地方就比较相似了，只有写方法和函数的地方是不一样的。下面是我的VB.NET版的代码：
U层：
```
Private Sub btnLogin_Click(sender As Object, e As EventArgs) Handles btnLogin.Click
        Dim user1 As New Login.Model.UserInfo
        Try
            '传值
            user1.UserName = txtUsername.Text.Trim()
            user1.Password = txtPassword.Text.Trim()
            '调用BLL层
            Dim Deal1 As New Login.BLL.LoginService
            Deal1.UserLogin(user1)
        Catch ex As Exception
            MessageBox.Show(ex.Message.ToString())
        End Try
        'MsgBox("登录用户：" + user1.UserName, , "恭喜")
    End Sub
```
B层：
```
Public Class LoginService
    Public Function UserLogin(User1 As Login.Model.UserInfo) As Login.Model.UserInfo
        Dim uDao As New Login.DAL.UserDAO
        Dim user As New Login.Model.UserInfo
        Dim sDao As New Login.DAL.ScoreDAO
        user = uDao.SelectUser(User1)
        If (user.UserName <> "") Then
            sDao.UpdateScore(user.UserName, 10)
            MsgBox("登录成功，马上进入系统......", , "恭喜")
            Return user
        Else
            Throw New Exception("登录失败。")
            Return user
        End If
    End Function
End Class
```
D层（UserDAO）：
```
Imports System.Data
Imports System.Data.SqlClient
Public Class UserDAO
       Public Function SelectUser(User As Login.Model.UserInfo) As Login.Model.UserInfo
        Dim conn As New SqlConnection(DbUtil.ConnString)
        Dim reader As SqlDataReader
        Dim eUser As New Login.Model.UserInfo                                                              '实例化新的UserInfo
        Dim sql As String = "Select  UserName,Password From Users Where UserName=@UserName And Password=@Password"
        Dim cmd As New SqlCommand(sql, conn)
        cmd.CommandText = sql                                                                           '获取SQL语句的具体内容
        cmd.CommandType = CommandType.Text                                                '获取上述SQL语句的具体类型，在此为Select
        cmd.Parameters.Add(New SqlParameter("@UserName", User.UserName))
        cmd.Parameters.Add(New SqlParameter("@Password", User.Password))
        conn.Open()                                                                                               '打开数据连接
        reader = cmd.ExecuteReader()                                                                    '执行查询语句，并生成一个DataReader
        '读取查询到的数据，并返回给相应的属性
        While reader.Read()
            '获取数据库中相应字段的数据
            eUser.UserName = reader.GetString(0)
            eUser.Password = reader.GetString(1)
        End While
        Return eUser                                                                                               '返回查询到的实体
        conn.Close()                                                                                                '关闭连接
    End Function
End Class
```
其中的DbUtil.ConnString是来自一个模块DbUtill的具体代码为：
```
Module DbUtil
    Public ConnString As String = "Server=sampc;Database=Login;User ID=sa;Password=1"
End Module
```
具体为什么这里用模块，而在C#里面用的是类，详情见我的博客：[http://blog.csdn.net/tr1912/article/details/50363253](http://blog.csdn.net/tr1912/article/details/50363253)
        以上就是VB.NET版的三层的具体实现方法，数据库的连接方式都是一样的，因为使用了ADO.NET的访问方式，所以代码的方式是公共的。
