
# JDBC中DBCP数据库连接池详解 - 李昆鹏的博客 - CSDN博客


2018年06月24日 23:51:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：51标签：[JDBC																](https://so.csdn.net/so/search/s.do?q=JDBC&t=blog)个人分类：[JDBC																](https://blog.csdn.net/weixin_41547486/article/category/7739465)


------------------------------------------------JDBC中DBCP数据库连接池详解----------------------------------------------
**JDBC数据库连接池接口（DataSource）**
Java为数据库连接池提供了公共的接口：javax.sql.DataSource，各个厂商可以让自己的连接池实现这个接口。这样应用程序可以方便的切换不同厂商的连接池！

## 1 DBCP
### 什么是DBCP？
DBCP是Apache提供的一款开源免费的数据库连接池！
Hibernate3.0之后不再对DBCP提供支持！因为Hibernate声明DBCP有致命的缺欠！DBCP因为Hibernate的这一毁谤很是生气，并且说自己没有缺欠。

### 2　DBCP的使用
|public|static|void|main(String[] args) {
|Properties prop =|new|Properties();
|InputStream in = JDBCPool.|class|.getClassLoader().getResourceAsStream(|"dbcp.properties"|);
|try|{
|prop.load(in);
|DataSource|ds = BasicDataSourceFactory.|createDataSource|(prop);
|Connection conn = ds.getConnection();
|System.|out|.println(conn);
|}|catch|(Exception e) {
|e.printStackTrace();
|}
|}
|

### 3　DBCP的配置信息
下面是对DBCP的配置介绍：
|\#|基本配置
|driverClassName=|oraclel.jdbc.OracleDriver
|url=|jdbc:oracle:thin:@127.0.0.1:1521:orcl
|username=|scott
|password=|tiger
|initialSize ：连接池启动时创建的初始化连接数量（默认值为0）
|maxActive ：连接池中可同时连接的最大的连接数（默认值为8）
|maxIdle：连接池中最大的空闲的连接数，超过的空闲连接将被释放，如果设置为负数表示不限制（默认为8个)
|minIdle：连接池中最小的空闲的连接数
|maxWait  ：最大等待时间，当没有可用连接时，连接池等待连接释放的最大时间，超过该时间限制会抛出异常，如果设置-1表示无限等待（默认为无限，调整为60000ms，避免因线程池不够用，而导致请求被无限制挂起）
|
代码示例：
**首先要导入驱动文件**
![](https://img-blog.csdn.net/20180624235109224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
创建配置文件dbcp.preporties
|driverClassName=oracle.jdbc.OracleDriver
|url=jdbc:oracle:thin:@127.0.0.1:1521:orcl
|username=scott
|password=tiger
|initialSize=10
|maxActive=5
|maxIdle=5
|minIdle=5
|maxWait=-1
|
最后创建连接池的类
|package com.rl.dbcp;
|import java.io.IOException;
|import java.io.InputStream;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import java.util.Properties;
|import javax.sql.DataSource;
|import  org.apache.commons.dbcp.BasicDataSourceFactory;
|public class JDBCPool {
|public  static void main(String[] args) {
|//获得字节输入流对象
|InputStream  in =  JDBCPool.class.getClassLoader().getResourceAsStream("dbcp.properties");
|Properties  prop = new Properties();
|PreparedStatement  ps = null;
|ResultSet  rs = null;
|try  {
|prop.load(in);
|//创建数据库连接池
|DataSource  ds = BasicDataSourceFactory.createDataSource(prop);
|//获得数据库连接
|Connection  conn = ds.getConnection();
|String  sql = "select * from person";
|ps = conn.prepareStatement(sql);
|rs = ps.executeQuery();
|while(rs.next()){
|System.out.println("ID"+rs.getInt(1)+"  name" +rs.getString(2)+"  address" +rs.getString(3)+"  birthday"+rs.getDate(4));
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally{
|try  {
|if(rs  != null){
|rs.close();
|}
|if(ps  != null){
|ps.close();
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|}
|}
|


