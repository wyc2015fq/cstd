
# JDBC的概述 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:24:06[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：107


--------------------------------------------------JDBC的概述------------------------------------------------------------
# 1    JDBC概述
### 1　什么是JDBC
JDBC（JavaDataBase Connectivity）就是Java数据库连接，说白了就是用Java语言来操作数据库。原来我们操作数据库是在控制台使用SQL语句来操作数据库，JDBC是用Java语言向数据库发送SQL语句。
### 2　JDBC原理
早期SUN公司的天才们想编写一套可以连接天下所有数据库的API，但是当他们刚刚开始时就发现这是不可完成的任务，因为各个厂商的数据库服务器差异太大了。后来SUN开始与数据库厂商们讨论，最终得出的结论是，由SUN提供一套访问数据库的规范（就是一组接口），并提供连接数据库的协议标准，然后各个数据库厂商会遵循SUN的规范提供一套访问自己公司的数据库服务器的API出现。SUN提供的规范命名为JDBC，而各个厂商提供的，遵循了JDBC规范的，可以访问自己数据库的API被称之为驱动！
![](https://img-blog.csdn.net/20180615232318272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
JDBC是接口，而JDBC驱动才是接口的实现，没有驱动无法完成数据库连接！每个数据库厂商都有自己的驱动，用来连接自己公司的数据库。
当然还有第三方公司专门为某一数据库提供驱动，这样的驱动往往不是开源免费的！

# 2 JDBC接口、类介绍

## 1　JDBC中的主要类（接口）
在JDBC中常用的类有：
l  DriverManager – 类，用来获取Connection；
l  Connection – 接口；
l  Statement – 接口；
l  ResultSet – 接口。
![](https://img-blog.csdn.net/20180615232333186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2　DriverManager
其实我们今后只需要会用DriverManager的getConnection()方法即可：
1.    Class.forName(“oracle.jdbc.OracleDriver”);//注册驱动
2.    String url = “jdbc:oracle:thin:@127.0.0.1:1521:orcl”;
3.    String username = “scott”;
4.    String password = “tiger”;
5.    Connection con =DriverManager.getConnection(url, username, password);
注意，上面代码可能出现的两种异常：
1.    ClassNotFoundException：这个异常是在第1句上出现的，出现这个异常有两个可能：
l  你没有给出oracle的jar包；
l  你把类名称打错了，查看类名是不是oracle.jdbc.OracleDriver。
2.    SQLException：这个异常出现在第5句，出现这个异常就是三个参数的问题，往往username和password一般不是出错，所以需要认真查看url是否打错。
对于DriverManager.registerDriver()方法了解即可，因为我们今后注册驱动只会Class.forName()，而不会使用这个方法。

## 3　Connection
Connection最为重要的方法就是获取Statement：
l  Statement stmt = con.createStatement();
后面在学习ResultSet方法时，还要学习一下下面的方法：
l  Statement stmt = con.createStatement(int,int);

## 4　Statement
Statement最为重要的方法是：
l  int executeUpdate(String sql)：执行更新操作，即执行insert、update、delete语句，其实这个方法也可以执行create table、alter table，以及drop table等语句，但我们很少会使用JDBC来执行这些语句；
l  ResultSet executeQuery(String sql)：执行查询操作，执行查询操作会返回ResultSet，即结果集。
boolean execute()
Statement还有一个boolean execute()方法，这个方法可以用来执行增、删、改、查所有SQL语句。该方法返回的是boolean类型，表示SQL语句是否执行成功。
如果使用execute()方法执行的是更新语句，那么还要调用int getUpdateCount()来获取insert、update、delete语句所影响的行数。
如果使用execute()方法执行的是查询语句，那么还要调用ResultSet getResultSet()来获取select语句的查询结果。

## 5　ResultSet之获取列数据
可以通过next()方法使ResultSet的游标向下移动，当游标移动到你需要的行时，就需要来获取该行的数据了，ResultSet提供了一系列的获取列数据的方法：
l  String getString(int columnIndex)：获取指定列的String类型数据；
l  int getInt(int columnIndex)：获取指定列的int类型数据；
l  double getDouble(int columnIndex)：获取指定列的double类型数据；
l  Object getObject(int columnIndex)：获取指定列的Object类型的数据。
上面方法中，参数columnIndex表示列的索引，列索引从1开始，而不是0，这第一点与数组不同。如果你清楚当前列的数据类型，那么可以使用getInt()之类的方法来获取，如果你不清楚列的类型，那么你应该使用getObject()方法来获取。
ResultSet还提供了一套通过列名称来获取列数据的方法：
l  String getString(String columnName)：获取名称为columnName的列的String数据；
l  int getInt(String columnName)：获取名称为columnName的列的int数据；
l  double getDouble(String columnName)：获取名称为columnName的列的double数据；
l  Object getObject(String columnName)：获取名称为columnName的列的Object数据；
------------------------------------------------------------------------------------------------------------------
结果集的列数
ResultSet rs =stm.executeQuery(sql);
ResultSetMetaData rsmd =rs.getMetaData();
**int**count = rsmd.getColumnCount();
获取行数 只能通过滚动 resultset
ResultSet rs = stm.executeQuery(sql);
rs.last();
**int**row= rs.getRow();

