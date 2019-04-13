
# java入门教程-12.5几个重要的java数据库访问类和接口 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:49:28[seven-soft](https://me.csdn.net/softn)阅读数：366


编写访问数据库的Java程序还需要几个重要的类和接口。
## DriverManager类
DriverManager类处理驱动程序的加载和建立新数据库连接。DriverManager是java.sql包中用于管理数据库驱动程序的类。通常，应用程序只使用类DriverManager的getConnection()静态方法，用来建立与数据库的连接，返回Connection对象：
static Connection getConnection(String url,String username,String password)
指定数据的URL用户名和密码创建数据库连接对象。url的语法格式是：
jdbc:<数据库的连接机制>:<ODBC数据库名>。
## Connection类
Connection类是java.sql包中用于处理与特定数据库连接的类。Connection对象是用来表示数据库连接的对象，Java程序对数据库的操作都在这种对象上进行。Connection类的主要方法有：Statement createStatement()：创建一个Statement对象。
Statement createStatement(int resultSetType,int resultSetConcurrency)：创建一个Statement对象，生成具有特定类型的结果集。
void commit()：提交对数据库的改动并释放当前持有的数据库的锁。
void rollback()：回滚当前事务中的所有改动并释放当前连接持有的数据库的锁。
String getCatalog()：获得连接对象的当前目录。
boolean isClose()：判断连接是否已关闭。
boolean isReadOnly()：判断连接是否为只读模式。
void setReadOnly()：设置连接为只读模式。
void close()：释放连接对象的数据库和JDBC资源。
## Statement类
Statement类是java.sql包中用于在指定的连接中处理SQL语句的类。数据库编程的要点是在程序中嵌入SQL命令。程序需要声明和创建连接数据库的Connection对象，并让该对象连接数据库。调用类DriverManager的静态方法getConnection()获得Connection对象，实现程序与数据库的连。然后，用Statement类声明SQL语句对象，并调用Connection对象的createStatement()方法，创建SQL语句对象。例如，以下代码创建语句对象sql：
Statement sql = null;
try{
sql = con.createStatement();
}catch(SQLException e){}
## ResultSet类
有了SQL语句对象后，调用语句对象的方法executeQuery()执行SQL查询，并将查询结果存放在一个用ResultSet类声明的对象中，例如，以下代码读取学生成绩表存于rs 对象中：
ResultSet rs = sql.executeQuery(“SELECT * FROM ksInfo”);
ResultSet对象实际上是一个由查询结果数据的表，是一个管式数据集，由统一形式的数据行组成，一行对应一条查询记录。在ResultSet对象中隐含着一个游标，一次只能获得游标当前所指的数据行，用next方法可取下一个数据行。用数据行的字段(列)名称或位置索引(自1开始)调用形如getXXX()方法获得记录的字段植
 。以下是ResultSet对象的部分方法：byte getByte(int columnIndex)：返回指定字段的字节值。
Date getDate(int columnIndex)：返回指定字段的日期值。
float getFloat(int columnIndex)：返回指定字段的浮点值。
int getInt(int columnIndex)：返回指定字段的整数值。
String getString(int columnIndex)：返回指定字段的字符串值。
double getDouble(String columnName)：返回指定字段的双精度值。
long getLong(String columnName)：返回指定字段的long型整值。
boolean next()：返回是否还有下一字段。
以上方法中的columnIndex是位置索引，用于指定字段，columnName是字段名。
用户需要在查询结果集上浏览，或前后移动、或显示结果集的指定记录，这称为可滚动结果集。程序要获得一个可滚动结果集，只要在获得SQL的语句对象时，增加指定结果集的两个参数即可。例如，以下代码：
Statement stmt = con.createStatement(type,concurrency);
ResultSet rs = stmt.executeQuery(SQL语句)
语句对象stmt的SQL查询就能得到相应类型的结果集。int 型参数type决定可滚动集的滚动方式：ResultSet.TYPE_FORWORD_ONLY,结果集的游标只能向下滚动。
ResultSet.TYPE_SCROLL_INSENSITIVE,游标可上下移动，当数据库变化时，当前结果集不变。
ResultSet. TYPE_SCROLL_SENSITIVE，游标可上下移动，当数据库变化时，当前结果集同步改变。
int 型参数concurrency决定数据库是否与可滚动集同步更新：ResultSet.CONCUR_READ_ONLY,不能用结果集更新数据库中的表。
ResultSet.CONCUR_UPDATETABLE,能用结果集更新数据库中的表。

例如，以下代码利用连接对象connect，创建Statement对象stmt，指定结果集可滚动，并以只读方式读数据库：
stmt = connect.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,
ResultSet.CONCUR_READ_ONLY);
可滚动集上另外一些常用的方法如下：boolean previous()：将游标向上移动，当移到结果集的第一行时，返回false。
void beforeFirst()：将游标移结果集的第一行之前。
void afterLast()：将游标移到结果集的最后一行之后。
void first()：将游标移到第一行。
void last()：将游标移到最后一行。
boolean isAfterLast()：判游标是否在最后一行之后。
boolean isBeforeFirst()：判游标是否在第一行之前。
boolean isLast()：判游标是否在最后一行。
boolean isFirst()：判游标是否在第一行。
int getRow()：获取当前所指的行(行号自1开始编号，结果集空，返回0)。
boolean absolute(int row)：将游标移到row行。

