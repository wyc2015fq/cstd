# SQLServer中ADO,OLEDB,ODBC的区别 - xqhrs232的专栏 - CSDN博客
2017年10月19日 17:44:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：554
原文地址::[http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065)
相关文章
1、[ADO,OLEDB,ODBC,DAO的区别](http://blog.csdn.net/ithomer/article/details/6624684)----[http://blog.csdn.net/ithomer/article/details/6624684](http://blog.csdn.net/ithomer/article/details/6624684)
2、sql server 数据库连接方式分析、详解----[http://blog.csdn.net/wang379275614/article/details/7859398](http://blog.csdn.net/wang379275614/article/details/7859398)
3、**[OleDb连接SQLServer](http://blog.csdn.net/lovegonghui/article/details/53508221)----**[http://blog.csdn.net/lovegonghui/article/details/53508221](http://blog.csdn.net/lovegonghui/article/details/53508221)
 之前看过王志鹏一片博客《[sql server 数据库连接方式分析、详解](http://blog.csdn.net/wang379275614/article/details/7859398)》一直以为连接数据库只有OLEDB和ODBC两种方式，因为自己可以肯定这次连接数据库使用的不是OLEDB方式。所以自己就断定使用的是 ODBC方式，因为自己想把自己的数据库连接改变为OLEDB方式尝试一下 ，经过各种尝试发现自己的想法是错的，其实还有ADO和DAO这两种方式， SQLServer连接VS是通过ADO这种方式连接数据库的。
**三种方式的差别：**
**如果是ADO方式，则**
      命名空间中引入的是：Imports System.Data.SqlClient
      连接数据库使用的相应对象为：SqlCommand,SqlConnection,SqlDataAdapter,SqlDataReader       
      举例：
**[html]**[view
 plain](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)[copy](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)
- <spanstyle="font-size:18px;"> Dim strconn As String = "server=localhost;uid=sa;pwd=123456654321;database=charge_sys"
-         Dim strsql As String = "select * from User_Info"
-         Dim conn As SqlConnection = New SqlConnection(strconn)  
-         Dim cmd As SqlCommand = New SqlCommand(strsql, conn)  
- cmd.CommandType = CommandType.Text  
- cmd.CommandTimeout = 0.1  
-         conn.Open()  
-         Dim rdr As SqlDataReader = cmd.ExecuteReader  
-         Dim dt As New DataTable  
-         dt.Load(rdr)  
-         rdr.Close()  
-         conn.Close()  
-         Return dt</span>
**[html]**[view
 plain](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)[copy](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)
- <spanstyle="font-size:18px;"></span>
**如果是ODBC方式，则**
     命名空间中引入的是：Imports System.Data.Odbc
     连接数据库使用的相应对象为：OdbcCommand,OdbcConnection,OdbcAdapter,OdbcDataReader       
      举例：
**[html]**[view
 plain](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)[copy](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)
- <spanstyle="font-size:18px;">Dim strconn As String = "Driver={sql server};server=localhost;uid=sa;pwd=123456654321;database=charge_sys"
-         Dim strsql As String = "select * from User_Info"
-         Dim conn As OdbcConnection = New OdbcConnection(strconn)  
-         Dim cmd As OdbcCommand = New OdbcCommand(strsql, conn)  
- cmd.CommandType = CommandType.Text  
- cmd.CommandTimeout = 0.1  
-         conn.Open()  
-         Dim rdr As OdbcDataReader = cmd.ExecuteReader  
-         Dim dt As New DataTable  
-         dt.Load(rdr)  
-         rdr.Close()  
-         conn.Close()  
-         Return dt</span>
**如果是OLEDB方式，则**
      命名空间中引入的是：Imports System.Data.Oledb
      连接数据库使用的相应对象为：OledbCommand,OledbConnection,OledbAdapter,OledbDataReader       
       举例：
**[vb]**[view
 plain](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)[copy](http://blog.csdn.net/zhaohuanhuanjiuqi/article/details/17270065#)
- <span style="font-size:18px;"> Dim strconn AsString = "Provider=SQLOLEDB;server=localhost;uid=sa;pwd=123456654321;database=charge_sys"
- Dim strsql AsString = "select * from User_Info"
- Dim conn As OleDbConnection = New OleDbConnection(strconn)  
- Dim cmd As OleDbCommand = New OleDbCommand(strsql, conn)  
-         cmd.CommandType = CommandType.Text  
-         cmd.CommandTimeout = 0.1  
-         conn.Open()  
- Dim rdr As OleDbDataReader = cmd.ExecuteReader  
- Dim dt AsNew DataTable  
-         dt.Load(rdr)  
-         rdr.Close()  
-         conn.Close()  
- Return dt</span>  
      大家可以发现，其实数据库连接字符串是不同的，不同的连接机制，对应的数据库连接字符串是不同的，这点大家都知道，推荐大家看一下王志鹏一片博客《[sql
 server 数据库连接方式分析、详解](http://blog.csdn.net/wang379275614/article/details/7859398)》还有我在csdn上看到的另外一片博客《[ADO,OLEDB,ODBC,DAO的区别](http://blog.csdn.net/ithomer/article/details/6624684)》
