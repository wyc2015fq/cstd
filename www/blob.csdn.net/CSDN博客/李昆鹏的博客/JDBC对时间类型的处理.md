
# JDBC对时间类型的处理 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:29:51[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：667


-----------------------------------------------JDBC对时间类型的处理----------------------------------------------
# 时间类型

## 1　Java中的时间类型
java.sql包下给出三个与数据库相关的日期时间类型，分别是：
l  Date：表示日期，只有年月日，没有时分秒。会丢失时间；
l  Time：表示时间，有年月日时分秒；
l  Timestamp：表示时间戳，有年月日时分秒，以及毫秒。
这三个类都是java.util.Date的子类。
java.util.Date -- 年月日时分秒
java.util.Calendar -- DategetTime()

## 2　时间类型相互转换
把数据库的三种时间类型赋给java.util.Date，基本不用转换，因为这是把子类对象给父类的引用，不需要转换。
java.sql.Datedate = …
java.util.Date d= date;
java.sql.Timetime = …
java.util.Date d= time;
java.sql.Timestamptimestamp = …
java.util.Date d= timestamp;
当需要把java.util.Date转换成数据库的三种时间类型时，这就不能直接赋值了，这需要使用数据库三种时间类型的构造器。java.sql包下的Date、Time、TimeStamp三个类的构造器都需要一个long类型的参数，表示毫秒值。创建这三个类型的对象，只需要有毫秒值即可。我们知道java.util.Date有getTime()方法可以获取毫秒值，那么这个转换也就不是什么问题了。
java.utl.Date d= new java.util.Date();
java.sql.Datedate = new java.sql.Date(d.getTime());//会丢失时分秒
Time time = newTime(d.getTime());
Timestamptimestamp = new Timestamp(d.getTime());
Ø**代码**
我们来创建一个dt表：
|CREATE TABLE dt(
|d  DATE,
|t  TIME,
|ts TIMESTAMP
|)
|
下面是向dt表中插入数据的代码：
|@Test
|public|void|fun1()|throws|SQLException {
|Connection con = JdbcUtils.|getConnection|();
|String sql =|"insert into dt value(?,?,?)"|;
|PreparedStatement pstmt =  con.prepareStatement(sql);
|java.util.Date d =|new|java.util.Date();
|pstmt.setDate(1,|new|java.sql.Date(d.getTime()));
|pstmt.setTime(2,|new|Time(d.getTime()));
|pstmt.setTimestamp(3,|new|Timestamp(d.getTime()));
|pstmt.executeUpdate();
|}
|
下面是从dt表中查询数据的代码：
|@Test
|public|void|fun2()|throws|SQLException {
|Connection con = JdbcUtils.|getConnection|();
|String sql =|"select * from dt"|;
|PreparedStatement pstmt =  con.prepareStatement(sql);
|ResultSet rs = pstmt.executeQuery();
|rs.next();
|java.util.Date d1 = rs.getDate(1);
|java.util.Date d2 = rs.getTime(2);
|java.util.Date d3 = rs.getTimestamp(3);
|System.|out|.println(d1);
|System.|out|.println(d2);
|System.|out|.println(d3);
|}
|
代码示例：
|package|com.rl.jdbc;
|import|java.sql.Connection;
|import|java.sql.Date;
|import|java.sql.DriverManager;
|import|java.sql.PreparedStatement;
|import|java.sql.ResultSet;
|import|java.sql.SQLException;
|import|java.sql.Time;
|import|java.sql.Timestamp;
|public|class|JDBCAdd2 {
|/**
|*|@param|args
|*/
|public|static|void|main(String[]|args|) {
|getDate|();
|}
|/**
|*|写入日期数据到数据库中
|*/
|public|static|void|saveDate(){
|//|设置|oracle|数据库驱动的类
|String|driverClass|=|"oracle.jdbc.OracleDriver"|;
|//|定义连接|oracle|的|url
|String|url|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|//|用户名
|String|username|=|"scott"|;
|//|密码
|String|password|=|"tiger"|;
|//|定义|connection|连接
|Connection|conn|=|null|;
|//|定义数据库预编译的|sql|执行对象
|PreparedStatement|ps|=|null|;
|//|定义要执行的|sql|增加语句
|String|addsql|=|"insert into time_test values(?,?,?)  "|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建预编译|sql|对象
|ps|=|conn|.prepareStatement(|addsql|);
|//|创建时间对象
|java.util.Date|date|=|new|java.util.Date();
|//|给预编译|sql|语句赋值
|ps|.setDate(1,|new|Date(System.|currentTimeMillis|()));
|ps|.setTime(2,|new|Time(System.|currentTimeMillis|()));
|ps|.setTimestamp(3,|new|Timestamp(System.|currentTimeMillis|()));
|int|count|=|ps|.executeUpdate();
|System.|out|.println(|"|当前|sql|的更新条数：|"|+|count|);
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|ps|!=|null|)
|ps|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|/**
|*|从数据库中取出日期数据
|*/
|public|static|void|getDate(){
|//|设置|oracle|数据库驱动的类
|String|driverClass|=|"oracle.jdbc.OracleDriver"|;
|//|定义连接|oracle|的|url
|String|url|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|//|用户名
|String|username|=|"scott"|;
|//|密码
|String|password|=|"tiger"|;
|//|定义|connection|连接
|Connection|conn|=|null|;
|//|定义数据库预编译的|sql|执行对象
|PreparedStatement|ps|=|null|;
|//|定义查询数据库结果集对象
|ResultSet|rs|=|null|;
|//|定义要执行的|sql|查询语句
|String|querysql|=|"select * from time_test "|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建预编译|sql|对象
|ps|=|conn|.prepareStatement(|querysql|);
|//|获得结果集对象
|rs|=|ps|.executeQuery();
|//|第一种方法使用对应的|SQL|时间类型来接收结果，循环遍历结果
|/*while(rs.next()){
|Date date =  rs.getDate(1);
|Time time =  rs.getTime(2);
|Timestamp timestamp|= rs.getTimestamp(3);
|System.out.println("|日期：|"+date);
|System.out.println("|时间：|"+time);
|System.out.println("|时间戳：|"+|timestamp|);
|}*/
|//|第二种方法使用全部用|java.util.Date|类型来接收结果，循环遍历结果
|/*while(rs.next()){
|java.util.Date date  = rs.getDate(1);
|java.util.Date time  = rs.getTime(2);
|java.util.Date|timestamp|= rs.getTimestamp(3);
|System.out.println("|日期：|"+date);
|System.out.println("|时间：|"+time);
|System.out.println("|时间戳：|"+|timestamp|);
|}*/
|//|第三种方法我们可以全部用|Date|类型来获取日期数据，再使用|java.util.date|类型来接收
|while|(|rs|.next()){
|java.util.Date|date|=|rs|.getDate(1);
|java.util.Date|time|=|rs|.getDate(2);
|java.util.Date|timestamp|=|rs|.getDate(3);
|System.|out|.println(|"|日期：|"|+|date|);
|System.|out|.println(|"|时间：|"|+|time|);
|System.|out|.println(|"|时间戳：|"|+|timestamp|);
|}
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|ps|!=|null|)
|ps|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|}
|


