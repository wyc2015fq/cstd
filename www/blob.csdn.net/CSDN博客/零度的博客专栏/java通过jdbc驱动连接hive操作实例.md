# java通过jdbc驱动连接hive操作实例 - 零度的博客专栏 - CSDN博客
2016年10月09日 15:24:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：3510
基于hadoop的Hive数据仓库JavaAPI简单调用的实例，关于Hive的简介在此不赘述。hive提供了三种用户接口：CLI，JDBC/ODBC和 WebUI- CLI，即Shell命令行
- JDBC/ODBC 是 Hive 的Java，与使用传统数据库JDBC的方式类似
- WebGUI是通过浏览器访问 Hive
本文主要介绍的就是第二种用户接口，直接进入正题。
 1、Hive 安装：
        1）hive的安装请参考网上的相关文章，测试时只在hadoop一个节点上安装hive即可。
        2）测试数据data文件'\t'分隔：
              1   zhangsan
              2    lisi
              3   wangwu
        3）将测试数据data上传到linux目录下，我放置在：/home/hadoop01/data 
 2、在使用 JDBC 开发 Hive 程序时,  必须首先开启 Hive 的远程服务接口。使用下面命令进行开启:
Java代码  ![收藏代码](http://787141854-qq-com.iteye.com/images/icon_star.png)
- hive --service hiveserver >/dev/null2>/dev/null &  
![](http://static.blog.csdn.net/images/save_snippets.png)
 3、测试代码：
Java代码  ![收藏代码](http://787141854-qq-com.iteye.com/images/icon_star.png)
- import java.sql.Connection;  
- import java.sql.DriverManager;  
- import java.sql.ResultSet;  
- import java.sql.SQLException;  
- import java.sql.Statement;  
- 
- import org.apache.log4j.Logger;  
- 
- /**
-  * Hive的JavaApi
-  * 
-  * 启动hive的远程服务接口命令行执行：hive --service hiveserver >/dev/null 2>/dev/null &
-  * 
-  * @author 吖大哥
-  * 
-  */
- publicclass HiveJdbcCli {  
- 
- privatestatic String driverName = "org.apache.hadoop.hive.jdbc.HiveDriver";  
- privatestatic String url = "jdbc:hive://hadoop3:10000/default";  
- privatestatic String user = "hive";  
- privatestatic String password = "mysql";  
- privatestatic String sql = "";  
- privatestatic ResultSet res;  
- privatestaticfinal Logger log = Logger.getLogger(HiveJdbcCli.class);  
- 
- publicstaticvoid main(String[] args) {  
-         Connection conn = null;  
-         Statement stmt = null;  
- try {  
-             conn = getConn();  
-             stmt = conn.createStatement();  
- 
- // 第一步:存在就先删除
-             String tableName = dropTable(stmt);  
- 
- // 第二步:不存在就创建
-             createTable(stmt, tableName);  
- 
- // 第三步:查看创建的表
-             showTables(stmt, tableName);  
- 
- // 执行describe table操作
-             describeTables(stmt, tableName);  
- 
- // 执行load data into table操作
-             loadData(stmt, tableName);  
- 
- // 执行 select * query 操作
-             selectData(stmt, tableName);  
- 
- // 执行 regular hive query 统计操作
-             countData(stmt, tableName);  
- 
-         } catch (ClassNotFoundException e) {  
-             e.printStackTrace();  
-             log.error(driverName + " not found!", e);  
-             System.exit(1);  
-         } catch (SQLException e) {  
-             e.printStackTrace();  
-             log.error("Connection error!", e);  
-             System.exit(1);  
-         } finally {  
- try {  
- if (conn != null) {  
-                     conn.close();  
-                     conn = null;  
-                 }  
- if (stmt != null) {  
-                     stmt.close();  
-                     stmt = null;  
-                 }  
-             } catch (SQLException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- privatestaticvoid countData(Statement stmt, String tableName)  
- throws SQLException {  
-         sql = "select count(1) from " + tableName;  
-         System.out.println("Running:" + sql);  
-         res = stmt.executeQuery(sql);  
-         System.out.println("执行“regular hive query”运行结果:");  
- while (res.next()) {  
-             System.out.println("count ------>" + res.getString(1));  
-         }  
-     }  
- 
- privatestaticvoid selectData(Statement stmt, String tableName)  
- throws SQLException {  
-         sql = "select * from " + tableName;  
-         System.out.println("Running:" + sql);  
-         res = stmt.executeQuery(sql);  
-         System.out.println("执行 select * query 运行结果:");  
- while (res.next()) {  
-             System.out.println(res.getInt(1) + "\t" + res.getString(2));  
-         }  
-     }  
- 
- privatestaticvoid loadData(Statement stmt, String tableName)  
- throws SQLException {  
-         String filepath = "/home/hadoop01/data";  
-         sql = "load data local inpath '" + filepath + "' into table "
-                 + tableName;  
-         System.out.println("Running:" + sql);  
-         res = stmt.executeQuery(sql);  
-     }  
- 
- privatestaticvoid describeTables(Statement stmt, String tableName)  
- throws SQLException {  
-         sql = "describe " + tableName;  
-         System.out.println("Running:" + sql);  
-         res = stmt.executeQuery(sql);  
-         System.out.println("执行 describe table 运行结果:");  
- while (res.next()) {  
-             System.out.println(res.getString(1) + "\t" + res.getString(2));  
-         }  
-     }  
- 
- privatestaticvoid showTables(Statement stmt, String tableName)  
- throws SQLException {  
-         sql = "show tables '" + tableName + "'";  
-         System.out.println("Running:" + sql);  
-         res = stmt.executeQuery(sql);  
-         System.out.println("执行 show tables 运行结果:");  
- if (res.next()) {  
-             System.out.println(res.getString(1));  
-         }  
-     }  
- 
- privatestaticvoid createTable(Statement stmt, String tableName)  
- throws SQLException {  
-         sql = "create table "
-                 + tableName  
-                 + " (key int, value string)  row format delimited fields terminated by '\t'";  
-         stmt.executeQuery(sql);  
-     }  
- 
- privatestatic String dropTable(Statement stmt) throws SQLException {  
- // 创建的表名
-         String tableName = "testHive";  
-         sql = "drop table " + tableName;  
-         stmt.executeQuery(sql);  
- return tableName;  
-     }  
- 
- privatestatic Connection getConn() throws ClassNotFoundException,  
-             SQLException {  
-         Class.forName(driverName);  
-         Connection conn = DriverManager.getConnection(url, user, password);  
- return conn;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
4、测试结果
![](http://dl2.iteye.com/upload/attachment/0097/2825/3c8d8054-968e-336a-b64d-c4f04e6b42d9.png)
5、终端查询结果：
hive> select * from testHive;
OK
1       zhangsan
2       lisi
3       wangwu
Time taken: 11.232 seconds
hive> 
