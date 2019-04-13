
# JDBC预编译PreparedStatement - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:28:22[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：38标签：[JDBC																](https://so.csdn.net/so/search/s.do?q=JDBC&t=blog)个人分类：[JDBC																](https://blog.csdn.net/weixin_41547486/article/category/7739465)


-----------------------------------------------------------JDBC预编译PreparedStatement----------------------------------------
# JDBC预编译

## 1　什么是SQL注入
在需要用户输入的地方，用户输入的是SQL语句的片段，最终用户输入的SQL片段与我们DAO中写的SQL语句合成一个完整的SQL语句！例如用户在登录时输入的用户名和密码都是为SQL语句的片段！

## 2　演示SQL注入
首先我们需要创建一张用户表，用来存储用户的信息。
下面我们写一个login()方法！
|public|void|login(String username,  String password) {
|Connection con =|null|;
|Statement stmt =|null|;
|ResultSet rs =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|stmt = con.createStatement();
|String sql =|"SELECT * FROM user WHERE "|+
|"username='"|+ username +
|"' and password='"|+  password +|"'"|;
|rs = stmt.executeQuery(sql);
|if|(rs.next()) {
|System.|out|.println(|"|欢迎|"|+ rs.getString(|"username"|));
|}|else|{
|System.|out|.println(|"|用户名或密码错误！|"|);
|}
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|JdbcUtils.|close|(con, stmt,  rs);
|}
|}
|
下面是调用这个方法的代码：
|login(|"a' or 'a'='a"|,|"a' or 'a'='a"|);
|
这行当前会使我们登录成功！因为是输入的用户名和密码是SQL语句片段，最终与我们的login()方法中的SQL语句组合在一起！我们来看看组合在一起的SQL语句：
|SELECT * FROM tab_user WHERE username='|a' or 'a'='a|' and password='|a' or 'a'='a|'
|
代码示例：
|package com.rl.jdbc;
|import java.sql.Connection;
|import java.sql.DriverManager;
|import java.sql.ResultSet;
|import java.sql.ResultSetMetaData;
|import java.sql.SQLException;
|import java.sql.Statement;
|import java.util.ArrayList;
|import java.util.List;
|import oracle.net.aso.s;
|public class JDBCQuery {
|/**
|* @param args
|*/
|public  static void main(String[] args) {
|//用户登录,即使密码错误，我们可以利用数据库的判断条件进行修改达到登录的效果
|//说明通过不合法的sql片段可以破坏原有的sql，带来极大的风险
|boolean  isLogin = login("likunpeng","***' or 'a' = 'a");
|if(isLogin){
|System.out.println("登录成功");
|}else{
|System.out.println("用户或者密码错误！");
|}
|}
|/**
|* 创建用户登录方法
|* @param uname
|* @param pword
|* @return
|*/
|public  static boolean login(String uname,String pword){
|//定义返回值变量，默认是登录失败
|boolean  isExist = false;
|//设置oracle数据库驱动的类
|String  driverClass = "oracle.jdbc.OracleDriver";
|//定义连接oracle的url
|String  url = "jdbc:oracle:thin:@127.0.0.1:1521:orcl";
|//用户名
|String  username = "scott";
|//密码
|String  password = "tiger";
|//定义connection连接
|Connection  conn = null;
|//定义数据库的sql执行对象
|Statement  stmt = null;
|//定义查询结果的ResultSet对象
|ResultSet  rs = null;
|//定义sql查询语句
|String  querySql = "select * from users   where username = '" +uname+"' and password =  '"+pword+"'";
|System.out.println(querySql);
|try  {
|//注册数据库的驱动程序
|Class.forName(driverClass);
|//获得数据库的连接
|conn  = DriverManager.getConnection(url, username,password);
|//创建sql对象
|stmt  = conn.createStatement();
|//执行查询sql
|rs  = stmt.executeQuery(querySql);
|//如果有数据登录成功
|isExist  = rs.next();
|} catch (ClassNotFoundException e) {
|e.printStackTrace();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|try {
|if(stmt != null)
|stmt.close();
|if(conn != null)
|conn.close();
|if(rs != null)
|rs.close();
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|//返回登录结果
|return  isExist;
|}
|}
|

## 3　防止SQL注入
l  过滤用户输入的数据中是否包含非法字符；
l  使用PreparedStatement。

## 4　PreparedStatement是什么？
PreparedStatement叫预编译声明！
PreparedStatement是Statement的子接口，你可以使用PreparedStatement来替换Statement。
PreparedStatement的好处：
l  防止SQL攻击；
l  提高代码的可读性，以可维护性；
l  提高效率。

## 5　PreparedStatement的使用
|String sql = “select * from tab_student  where s_number=?”;
|PreparedStatement pstmt =  con.prepareStatement(sql);
|pstmt.setString(1, “S_1001”);
|ResultSet rs = pstmt.executeQuery();
|rs.close();
|pstmt.clearParameters();
|pstmt.setString(1, “S_1002”);
|rs = pstmt.executeQuery();
|
在使用Connection创建PreparedStatement对象时需要给出一个SQL模板，所谓SQL模板就是有“?”的SQL语句，其中“?”就是参数。
在得到PreparedStatement对象后，调用它的setXXX()方法为“?”赋值，这样就可以得到把模板变成一条完整的SQL语句，然后再调用PreparedStatement对象的executeQuery()方法获取ResultSet对象。
注意PreparedStatement对象独有的executeQuery()方法是没有参数的，而Statement的executeQuery()是需要参数（SQL语句）的。因为在创建PreparedStatement对象时已经让它与一条SQL模板绑定在一起了，所以在调用它的executeQuery()和executeUpdate()方法时就不再需要参数了。
PreparedStatement最大的好处就是在于重复使用同一模板，给予其不同的参数来重复的使用它。这才是真正提高效率的原因。
**所以，建议大家在今后的开发中，无论什么情况，都去需要PreparedStatement****，而不是使用Statement。**
预编译代码示例：
|package com.rl.jdbc;
|import java.sql.Connection;
|import java.sql.DriverManager;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|public class JDBCQuery2 {
|/**
|* @param args
|*/
|public  static void main(String[] args) {
|//用户登录,即使密码错误，我们可以利用数据库的判断条件进行修改达到登录的效果
|//说明通过不合法的sql片段可以破坏原有的sql，带来极大的风险
|//我们可以通过PreparedStatement预编译来解决
|boolean  isLogin = login("likunpeng","123 or 'a' = 'a'");
|if(isLogin){
|System.out.println("登录成功");
|}else{
|System.out.println("用户或者密码错误！");
|}
|}
|/**
|* 创建用户登录方法
|* @param uname
|* @param pword
|* @return
|*/
|public  static boolean login(String uname,String pword){
|//定义返回值变量，默认是登录失败
|boolean  isExist = false;
|//设置oracle数据库驱动的类
|String  driverClass = "oracle.jdbc.OracleDriver";
|//定义连接oracle的url
|String  url = "jdbc:oracle:thin:@127.0.0.1:1521:orcl";
|//用户名
|String  username = "scott";
|//密码
|String  password = "tiger";
|//定义connection连接
|Connection  conn = null;
|//定义预编译数据库的sql执行对象
|PreparedStatement  ps = null;
|//定义查询结果的ResultSet对象
|ResultSet  rs = null;
|//定义sql查询语句
|String  querySql = "select * from users   where username = ? and password = ?";
|System.out.println(querySql);
|try  {
|//注册数据库的驱动程序
|Class.forName(driverClass);
|//获得数据库的连接
|conn  = DriverManager.getConnection(url, username,password);
|//创建预编译sql对象
|ps  = conn.prepareStatement(querySql);
|//给编译好的sql来设置参数值，参数值从左到右，从1开始依次递增索引
|ps.setString(1,  uname);
|ps.setString(2,  pword);
|//执行查询sql
|rs  = ps.executeQuery();
|//如果有数据登录成功
|isExist  = rs.next();
|}  catch (ClassNotFoundException e) {
|e.printStackTrace();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|try {
|if(ps != null)
|ps.close();
|if(conn != null)
|conn.close();
|if(rs != null)
|rs.close();
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|//返回登录结果
|return  isExist;
|}
|}
|


