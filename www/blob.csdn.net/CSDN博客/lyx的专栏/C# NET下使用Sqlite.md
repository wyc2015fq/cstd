# C#.NET下使用Sqlite - lyx的专栏 - CSDN博客





2015年11月08日 15:09:22[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：364
个人分类：[软件教程](https://blog.csdn.net/qtlyx/article/category/5936699)










			今天做这个，发现网上好多说法，而且还不一致。亲测一个可用。

1下载SQLite-1.0.60.0-binaries

2.下载sqlite3


3.打开中工程，分别点击“项目”，“添加引用”，”浏览“，然后选择1中下载文件中的”System.Data.SQLite.DLL“就可以添加了。注意的是，要选着x86还是x64.这里的x86和x64的意思不是开发者电脑对应的系统，而是开发对象将运行的系统位数。所以，我们选择文件外面的x86即可。

4.可以在适当的地方加入下面这一段代码作为测试，（代码源自网络）：


     
     
SQLiteConnection conn = null;

     
      string
dbPath = "Data Source =" + Environment.CurrentDirectory +
"/test.db";

     
      conn = new
SQLiteConnection(dbPath);//创建数据库实例，指定文件位置  

     
     
conn.Open();//打开数据库，若文件不存在会自动创建  




     
      string sql
= "CREATE TABLE IF NOT EXISTS student(id integer, name varchar(20),
sex varchar(2));";//建表语句  

     
     
SQLiteCommand cmdCreateTable = new SQLiteCommand(sql, conn);

     
     
cmdCreateTable.ExecuteNonQuery();//如果表不存在，创建数据表
 




     
     
SQLiteCommand cmdInsert = new SQLiteCommand(conn);

     
     
cmdInsert.CommandText = "INSERT INTO student VALUES(1, '小红',
'男')";//插入几条数据  

     
     
cmdInsert.ExecuteNonQuery();

     
     
cmdInsert.CommandText = "INSERT INTO student VALUES(2, '小李',
'女')";

     
     
cmdInsert.ExecuteNonQuery();

     
     
cmdInsert.CommandText = "INSERT INTO student VALUES(3, '小明',
'男')";

     
     
cmdInsert.ExecuteNonQuery();


5.找到建立的数据库文件（.db），存储位置可以修改，默认在工程文件夹下的bin，Debug里面。

6.可用SQLite Database Browser 查看数据库存储是否符合要求，是否正确。

7.上述文件均可以从本人百度网盘下载：http://pan.baidu.com/s/1qW7RlpE




