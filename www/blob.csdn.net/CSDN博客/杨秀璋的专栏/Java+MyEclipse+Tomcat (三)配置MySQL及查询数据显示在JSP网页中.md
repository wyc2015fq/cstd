
# Java+MyEclipse+Tomcat (三)配置MySQL及查询数据显示在JSP网页中 - 杨秀璋的专栏 - CSDN博客

2015年05月12日 02:01:08[Eastmount](https://me.csdn.net/Eastmount)阅读数：11046


前面两篇文章讲述了如何配置MyEclipse和Tomcat开发JSP网站、如何配置Servlet简单实现表单提交，这篇文章主要讲述配置MySQL实现数据库连接MyEclipse，最后将查询表中的数据显示在JSP网页中。 文章主要以图片描述为主，请海涵~
[Java+MyEclipse+Tomcat (一)配置过程及jsp网站开发入门](http://blog.csdn.net/eastmount/article/details/45492065)
[Java+MyEclipse+Tomcat (二)配置Servlet及简单实现表单提交](http://blog.csdn.net/eastmount/article/details/45536369)
代码及MySQL下载地址：
[http://download.csdn.net/detail/eastmount/8701657](http://download.csdn.net/detail/eastmount/8701657)

## 一. 配置MySQL
首先下载mysql-5.0.96-winx64，安装过程如下图所示。
1.安装MySQL 5.0
![](https://img-blog.csdn.net/20150512004058210)![](https://img-blog.csdn.net/20150512004130127)
![](https://img-blog.csdn.net/20150512004348502)![](https://img-blog.csdn.net/20150512004219533)
2.选择手动配置、服务类型、通用多功能型和安装路径
![](https://img-blog.csdn.net/20150512004628075)![](https://img-blog.csdn.net/20150512004740178)
![](https://img-blog.csdn.net/20150512004807151)![](https://img-blog.csdn.net/20150512004643641)
3.设置数据库访问量连接数为15、端口为3306（代码中设置URL用到）、编码方式为utf-8
![](https://img-blog.csdn.net/20150512004903745)![](https://img-blog.csdn.net/20150512005117097)
![](https://img-blog.csdn.net/20150512005034568)![](https://img-blog.csdn.net/20150512005102601)
4.设置默认超级root用户的用户名和密码，最后安装成功
![](https://img-blog.csdn.net/20150512005402410)![](https://img-blog.csdn.net/20150512005426918)


## 二. 查询MySQL
安装MySQL 5.0成功后，进行数据库的简单操作。
1.运行MySQL输入默认用户密码123456
![](https://img-blog.csdn.net/20150512005659517)
2.创建数据库test01和使用数据库(第二次调用直接use database)
![](https://img-blog.csdn.net/20150512005748717)
3.创建表student，其中学号为主键
![](https://img-blog.csdn.net/20150512005909322)
4.显示表结构，使用语句desc student
![](https://img-blog.csdn.net/20150512005955171)
5.向学生表中插入数据并显示查询的数据
![](https://img-blog.csdn.net/20150512010232001)
此时MySQL操作数据库基本讲解结束，你同样可以实现数据库的增删改查、事务、存储过程等操作，建议安装可视化的软件来替代黑框。



## 三. MyEclipse查询数据库
为统一并简化Java语言操作各种数据库，Sun公司提供了JDBC框架，用于所有Java应用以统一的方式连接数据库。从适用于企业级Oracle、DB2、SQL Server，到中型应用MySQL、Oracle XE，最后适用于小型个人应用的Access、FoxPro等。JDBC(Java DataBase Connectivity，Java数据库连接)通过使用数据库厂家提供的数据库JDBC驱动器类，可以连接到任何流程的数据库上。
使用前一篇文章Servlet中的例子，在JSP中使用JDBC查询数据，其核心操作如下。参考hongten博客，地址如下：
[http://www.cnblogs.com/hongten/archive/2011/03/29/1998311.html](http://www.cnblogs.com/hongten/archive/2011/03/29/1998311.html)
1.加载JDBC驱动程序(MySQL驱动)

```python
Class.forName("com.mysql.jdbc.Driver") ;
```
2.提供JDBC连接的URL
```python
//驱动程序名   
String driverName = "com.mysql.jdbc.Driver";  
//数据库用户名   
String userName = "root";  
//密码   
String userPasswd = "123456";  
//数据库名   
String dbName = "test01";  
//表名   
String tableName = "student";  
//联结字符串   
String url = "jdbc:mysql://localhost:3306/" + dbName + "?user="  
        + userName + "&password=" + userPasswd;
```
3.创建数据库的连接
```python
Connection connection = DriverManager.getConnection(url);
```
4.创建一个Statement
要执行SQL语句，必须获得java.sql.Statement实例，Statement实例分为以下3种类型：
1).执行静态SQL语句。通常通过Statement实例实现。
2).执行动态SQL语句。通常通过PreparedStatement实例实现。
3).执行数据库存储过程。通常通过CallableStatement实例实现。
5.执行SQL语句
Statement接口提供了三种执行SQL语句的方法：executeQuery 、executeUpdate和execute
1).ResultSet executeQuery(String sqlString)：执行查询数据库的SQL语句，返回一个结果集（ResultSet）对象。
2).int executeUpdate(String sqlString)：用于执行INSERT、UPDATE或DELETE语句以及SQL DDL语句，如：CREATE TABLE和DROP TABLE等
3).execute(sqlString):用于执行返回多个结果集、多个更新计数或二者组合的语句。
6.处理结果
两种情况：执行更新返回的是本次操作影响到的记录数、执行查询返回的结果是一个ResultSet对象。
• ResultSet包含符合SQL语句中条件的所有行，并且它通过一套get方法提供了对这些行中数据的访问。
• 使用结果集（ResultSet）对象的访问方法获取数据：
```python
// 此方法比较高效  列是从左到右编号的，并且从列1开始 
while(rs.next()){   
         String name = rs.getString("name") ;   
    	 String pass = rs.getString(1) ; 
}
```
7.关闭JDBC对象
```python
//释放连接方法 con ps rs
public static void release(Connection con,Statement ps,ResultSet rs){
	try{
	if(rs!=null){ // 关闭记录集
		rs.close();
	}
	if(ps!=null){ // 关闭声明 
		ps.close();
	}
	if(con!=null){ // 关闭连接对象 
		con.close();
	}
	}catch (Exception e) {
		e.printStackTrace();
	}
}
```
需要在项目TestServlet文件夹TestServlet\WebRoot\WEB-INF\lib复制mysql-connector-java-5.1.15-bin.jar包文件。然后修改success.jsp代码。具体代码如下：
```python
<%@ page language="java" import="java.sql.*,java.io.*,java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!-- 参考博文 http://blog.csdn.net/believejava/article/details/39111823 -->
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>验证成功界面</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<style type="text/css">  
	table {  
	    border: 2px #CCCCCC solid;  
	    width: 360px;  
	}  
	  
	td,th {  
	    height: 30px;  
	    border: #CCCCCC 1px solid;  
	}  
	</style>  
  </head>
  
  <body>
    	界面表单提交跳转成功 <br>
    	<a href="index.jsp">返回</a>
    	
    <%  
        //驱动程序名   
        String driverName = "com.mysql.jdbc.Driver";  
        //数据库用户名   
        String userName = "root";  
        //密码   
        String userPasswd = "123456";  
        //数据库名   
        String dbName = "test01";  
        //表名   
        String tableName = "student";  
        //联结字符串   
        String url = "jdbc:mysql://localhost:3306/" + dbName + "?user="  
                + userName + "&password=" + userPasswd;  
        Class.forName("com.mysql.jdbc.Driver").newInstance();  
        Connection connection = DriverManager.getConnection(url);  
        Statement statement = connection.createStatement();  
        String sql = "SELECT * FROM " + tableName;  
        ResultSet rs = statement.executeQuery(sql);  
    %>  
    <br>  
    <br>  
    <table align="center">  
        <tr>  
            <th>  
                <%  
                    out.print("学号");  
                %>  
            </th>  
            <th>  
                <%  
                    out.print("姓名");  
                %>  
            </th>  
            <th>  
                <%  
                    out.print("专业");  
                %>  
            </th>  
        </tr>  
  
        <%  
            while (rs.next()) {  
        %>  
        <tr>  
            <td>  
                <%  
                    out.print(rs.getString(1));  
                %>  
            </td>  
            <td>  
                <%  
                    out.print(rs.getString(2));  
                %>  
            </td>  
            <td>  
                <%  
                    out.print(rs.getString(3));  
                %>  
            </td>  
        </tr>  
        <%  
            }  
        %>  
    </table>  
    <div align="center">  
        <br> <br> <br>  
        <%  
            out.print("数据查询成功，恭喜你");  
        %>  
    </div>  
    <%  
        rs.close();  
        statement.close();  
        connection.close();  
    %>  
  </body>
</html>
```
运行效果如下图所示：（可参考第二篇文章[(二)配置Servlet及简单实现表单提交](http://blog.csdn.net/eastmount/article/details/45536369)）
![](https://img-blog.csdn.net/20150512015725476)
![](https://img-blog.csdn.net/20150512015952399)
最后希望文章对你有所帮助，这篇文章是讲述JSP连接MySQL数据库，下一篇文章准备讲述Java文件和JSP文件之间相互操作数据库。如果文章有不足或错误的地方，还请海涵！这四篇文章基本就涵盖了Java网址的基础知识，你也可以实现简单的JSP网站了。
（By:Eastmount 2015-5-12 半夜2点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）

