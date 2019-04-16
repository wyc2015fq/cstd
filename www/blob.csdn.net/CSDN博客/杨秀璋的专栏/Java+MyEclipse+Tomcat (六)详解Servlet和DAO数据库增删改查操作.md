# Java+MyEclipse+Tomcat (六)详解Servlet和DAO数据库增删改查操作 - 杨秀璋的专栏 - CSDN博客





置顶2015年05月24日 05:16:55[Eastmount](https://me.csdn.net/Eastmount)阅读数：27201







        此篇文章主要讲述DAO、Java Bean和Servlet实现操作数据库，把链接数据库、数据库操作、前端界面显示分模块化实现。其中包括数据的CRUD增删改查操作，并通过一个常用的JSP网站前端模板界面进行描述。参考前文：
[Java+MyEclipse+Tomcat
 (一)配置过程及jsp网站开发入门](http://blog.csdn.net/eastmount/article/details/45492065)
[Java+MyEclipse+Tomcat
 (二)配置Servlet及简单实现表单提交](http://blog.csdn.net/eastmount/article/details/45536369)
[Java+MyEclipse+Tomcat
 (三)配置MySQL及查询数据显示在JSP网页中](http://blog.csdn.net/eastmount/article/details/45653615)
[Java+MyEclipse+Tomcat
 (四)Servlet提交表单和数据库操作](http://blog.csdn.net/eastmount/article/details/45725077)
[Java+MyEclipse+Tomcat (五)DAO和Java Bean实现数据库和界面分开操作](http://blog.csdn.net/eastmount/article/details/45833663)
        免费资源下载地址：
[http://download.csdn.net/detail/eastmount/8733385](http://download.csdn.net/detail/eastmount/8733385)

PS:这篇文章可以认为是对前面五篇文章的一系列总结和应用，同时我认为理解该篇文章基本就能简单实现一个基于数据库操作的JSP网站，对你的课程项目或毕设有所帮助！但同时没有涉及事务、触发器、存储过程、并发处理等数据库知识，也没有Struts、Hibernate、Spring框架知识，它还是属于基础性文章吧！希望对你有所帮助~


## 一. 项目结构

        该项目的结构如下图所示：


![](https://img-blog.csdn.net/20150524050926439)
        这是典型的DAO模式，其中bean文件夹中TrainManage.java类封装了数据库表TrainManage中的属性和get/set操作；DAO文件夹中TrainManageDAO.java是对类TrainManage（或火车表）的数据库增删改查操作；util中JDBCConnect.java主要是连接数据库MySQL的操作；servlet主要是POST方法请求表单。




## 二. 数据库初始化操作

        打开MySQL，输入默认超级root用户的密码，然后数据库的操作如下代码：


```
--创建数据库
create database ManageTrain;
--使用数据库
use ManageTrain;
--创建表 车次信息管理表 主键:车次
--属性：车次 出发地 目的地 行车时间 硬座票价 软座票价 硬卧票价 软卧票价 车辆路线 
create table TrainManage(
    trainid varchar(20) primary key,
    start varchar(20),
    end varchar(20),
    time varchar(20),
    yzprice decimal(10,1),
    rzprice decimal(10,1),
    ywprice decimal(10,1),
    rwprice decimal(10,1),
    root varchar(200)
);
--插入数据
insert TrainManage (trainid,start,end,time,yzprice,rzprice,ywprice,rwprice,root)
    values ("T87","Beijing","Guiyang","28小时","278","320","464.5","550",
            "Beijing Shijiazhuang Guiyang");
insert TrainManage (trainid,start,end,time,yzprice,rzprice,ywprice,rwprice,root)
    values ("T87","Guiyang","Beijing","28小时","278","320","464.5","550",
            "Guiyang Shijiazhuang Beijing");
--查询数据
select * from TrainManage;
```
        注意：上面操作在MySQL黑框中输出增删改查的SQL语言就可以，不要把中文注释也执行。同时设置所有编码方式都统一为utf-8防止乱码，数据库表结构如下图所示：

![](https://img-blog.csdn.net/20150523190516352)




## 三. 简单查询操作DAO方法

        新建Web Project，项目名为“TrainDatabase”,对火车车次数据库的增删改查。
        运行效果如下图所示：


![](https://img-blog.csdn.net/20150523223016746)

**        1.****在src下新建文件夹util，然后添加类JDBCConnect.java。代码如下：**        主要是调用getConnection(url, userName, password)方法进行连接数据库操作，数据库的名称为TrainManage，默认的连接对象为root，密码为123456。同时定义两个函数executeUpdate()执行无参数的SQL语句操作和有参数的SQL语句操作。

```java
package util;

import java.sql.*;
import com.mysql.jdbc.Driver;

public class JDBCConnect {
	
	//获取默认数据库连接
	public static Connection getConnection() throws SQLException {
		return getConnection("ManageTrain", "root", "123456"); //数据库名 默认用户 密码
	}
	
	//连接数据库   参数:数据库名 root登录名 密码
	public static Connection getConnection(String dbName, String userName,
			String password) throws SQLException {
		String url = "jdbc:mysql://localhost:3306/" + dbName 
				+ "?characterEncoding=utf-8";
		//连接MySQL"com.mysql.jdbc.Driver"
		DriverManager.registerDriver(new Driver());
		return DriverManager.getConnection(url, userName, password);
	}

	//设置 PreparedStatement 参数 
	public static void setParams(PreparedStatement preStmt, Object... params)
			throws SQLException {
		if (params == null || params.length == 0)
			return;
		for (int i = 1; i <= params.length; i++) {
			Object param = params[i - 1];
			if (param == null) {
				preStmt.setNull(i, Types.NULL);
			} else if (param instanceof Integer) {
				preStmt.setInt(i, (Integer) param);
			} else if (param instanceof String) {
				preStmt.setString(i, (String) param);
			} else if (param instanceof Double) {
				preStmt.setDouble(i, (Double) param);
			} else if (param instanceof Long) {
				preStmt.setDouble(i, (Long) param);
			} else if (param instanceof Timestamp) {
				preStmt.setTimestamp(i, (Timestamp) param);
			} else if (param instanceof Boolean) {
				preStmt.setBoolean(i, (Boolean) param);
			} else if (param instanceof Date) {
				preStmt.setDate(i, (Date) param);
			}
		}
	}

	//执行 SQL，返回影响的行数 异常处理
	public static int executeUpdate(String sql) throws SQLException {
		return executeUpdate(sql, new Object[] {});
	}

	//带参数执行SQL，返回影响的行数 异常处理
	public static int executeUpdate(String sql, Object... params)
			throws SQLException {
		Connection conn = null;
		PreparedStatement preStmt = null;
		try {
			conn = getConnection();
			preStmt = conn.prepareStatement(sql);
			setParams(preStmt, params);
			return preStmt.executeUpdate(); //执行SQL操作
		} finally {
			if (preStmt != null)
				preStmt.close();
			if (conn != null)
				conn.close();
		}
	}
}
```

**2.****在src下新建文件夹bean，然后添加类TrainManage.java。代码如下：**

```java
package bean;

public class TrainManage {
	
	private String trainid;       //车次
	private String start;         //出发地
	private String end;           //目的地
	private String time;          //行车时间
	private float yzprice;        //硬座票价
	private float rzprice;        //软座票价
	private float ywprice;        //硬卧票价
	private float rwprice;        //软卧票价
	private String root;          //车辆路线 
	
	public String getTrainid() { return trainid; }
	public String getStart() { return start; }
	public String getEnd() { return end; }
	public String getTime() { return time; }
	public Float getYzprice() { return yzprice; }
	public Float getRzprice() { return rzprice; }
	public Float getYwprice() { return ywprice; }
	public Float getRwprice() { return rwprice; }
	public String getRoot() { return root; }
	
	public void setTrainid(String str) { this.trainid =  str; }
	public void setStart(String str) { this.start =  str; }
	public void setEnd(String str) { this.end = str; }
	public void setTime(String str) { this.time = str; }
	public void setYzprice(Float price) { this.yzprice = price; }
	public void setRzprice(Float price) { this.rzprice = price; }
	public void setYwprice(Float price) { this.ywprice = price; }
	public void setRwprice(Float price) { this.rwprice = price; }
	public void setRoot(String str) { this.root = str; }
}
```
**3.在src下新建文件夹DAO，然后添加类StudentDAO.java。代码如下：**
        通常DAO（Data Access Object）数据访问对象是负责与数据库连接，主要功能执行对数据表的CUDR操作（创建、更新、删除、查询）。每个数据表都定义一个DAO接口或类实现，实现对此表的读写操作。换句话说，就是在域名.项目.模块.dao文件夹下创建个DAO类即可。

```java
package DAO;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import bean.TrainManage;
import util.JDBCConnect;

public class TrainManageDAO {
	
	//插入车次
	public static int insert(TrainManage train) throws Exception {
		String sql = "INSERT INTO TrainManage (trainid,start,end,time,yzprice,rzprice,ywprice,rwprice,root) " +
				"VALUES ('" + train.getTrainid() +"','"+ train.getStart() +"','"+ train.getEnd() +
				"','"+ train.getTime() +"','"+ train.getYzprice() +"','"+ train.getRzprice() +
				"','"+ train.getYwprice() +"','"+ train.getRwprice() +"','"+ train.getRoot()  +"');";
		System.out.println(sql);
		return JDBCConnect.executeUpdate(sql);
		
		/**
		 * 总是报错  No value specified for parameter 5
		 * 很多原因是 insert into train (?,?,?) values (?,?,?) 前面不应该是问号 
		 * 但我的怀疑是参数过多使用executeUpdate(sql,?,?,?...)方法是错误
		 * 
		 * String sql = "INSERT INTO TrainManage (trainid,start,end,time,yzprice,rzprice,ywprice,rwprice,root) VALUES (?,?,?,?,?,?,?,?,?);";
		 * System.out.println(sql);
		 * return JDBCConnect.executeUpdate(sql, train.getTrainid(), train.getStart(), train.getEnd(),
		 *		train.getTime(), train.getYzprice(), train.getRzprice(), train.getYwprice(), 
		 *		train.getRwprice(), train.getRoot());
		 */
	}
	
	//更新车次
	public static int update(TrainManage train) throws Exception {
		String sql = "UPDATE TrainManage SET start = ?, end = ? WHERE trainid = ? ";
		return JDBCConnect.executeUpdate(sql, train.getStart(), train.getEnd(), train.getTrainid());
	}

	//删除操作
	public static int delete(String id) throws Exception {
		String sql = "DELETE FROM TrainManage WHERE trainid = ? ";
		return JDBCConnect.executeUpdate(sql, id);
	}
	
	//查找记录 某车次
	public static TrainManage find(String id) throws Exception {
		String sql = "SELECT * FROM TrainManage WHERE trainid = ? ";
		Connection conn = null;
		PreparedStatement preStmt = null;
		ResultSet rs = null;
		try {
			//链接数据库执行SQL语句
			conn = JDBCConnect.getConnection(); //连接默认数据库
			preStmt = conn.prepareStatement(sql);
			preStmt.setString(1, id);
			rs = preStmt.executeQuery();
			//获取查询结果
			if (rs.next()) {
				TrainManage train = new TrainManage();
				train.setTrainid(rs.getString("trainid"));
				train.setStart(rs.getString("start"));
				train.setEnd(rs.getString("end"));
				train.setTime(rs.getString("time"));
				train.setYzprice(rs.getFloat("yzprice"));
				train.setYwprice(rs.getFloat("ywprice"));
				train.setRzprice(rs.getFloat("rzprice"));
				train.setRwprice(rs.getFloat("rwprice"));
				train.setRoot(rs.getString("root"));
				return train;
			} else {
				return null;
			}
		} finally { //依次关闭 记录集 声明 连接对象
			if (rs != null)
				rs.close();
			if (preStmt != null)
				preStmt.close();
			if (conn != null)
				conn.close();
		}
	}
	
	//查找记录 起始站 达到站
	public static List<TrainManage> findStartEnd(String start,String end) throws Exception {
		List<TrainManage> list = new ArrayList<TrainManage>();
		String sql = null; 
		Connection conn = null;
		Statement statement = null;
		ResultSet rs = null;
		//判断SQL语句
		if(start==""&&end=="") {
			sql = "SELECT * FROM TrainManage;";
		} else if(end=="") {
			sql = "SELECT * FROM TrainManage WHERE start = '"+ start + "';";
		} else if(start=="") {
			sql = "SELECT * FROM TrainManage WHERE end = '"+ end + "';";
		} else {
			sql = "SELECT * FROM TrainManage WHERE start = '" 
					+ start + "' and end = '"+ end +"';"; 
		}
		//执行
		try {
			//链接数据库执行SQL语句
			conn = JDBCConnect.getConnection(); //连接默认数据库
			statement = conn.createStatement();
			System.out.println(start+" "+end);
			System.out.println(sql);
			rs = statement.executeQuery(sql);
			//获取查询结果
			while(rs.next()) {
				TrainManage train = new TrainManage();
				train.setTrainid(rs.getString("trainid"));
				train.setStart(rs.getString("start"));
				train.setEnd(rs.getString("end"));
				train.setTime(rs.getString("time"));
				train.setYzprice(rs.getFloat("yzprice"));
				train.setYwprice(rs.getFloat("ywprice"));
				train.setRzprice(rs.getFloat("rzprice"));
				train.setRwprice(rs.getFloat("rwprice"));
				train.setRoot(rs.getString("root"));
				list.add(train);
			}
		} catch (Exception e) {
			System.out.println("错误："+e.getMessage());  
		}
		finally { //依次关闭 记录集 声明 连接对象
			if (rs != null)
				rs.close();
			if (statement != null)
				statement.close();
			if (conn != null)
				conn.close();
		}
		return list;
	}
	
	//查询所有车次信息
	public static List<TrainManage> listStudents() throws Exception {
		List<TrainManage> list = new ArrayList<TrainManage>();
		String sql = "SELECT * FROM TrainManage";
		Connection conn = null;
		PreparedStatement preStmt = null;
		ResultSet rs = null;
		try {
			conn = JDBCConnect.getConnection();
			preStmt = conn.prepareStatement(sql);
			rs = preStmt.executeQuery();
			while (rs.next()) {
				//设置数据库中表参数 否则报错java.sql.SQLException: Column 'id' not found.
				TrainManage train = new TrainManage();
				train.setTrainid(rs.getString("trainid"));      
				train.setStart(rs.getString("start"));
				train.setEnd(rs.getString("end"));
				train.setTime(rs.getString("time"));
				train.setYzprice(rs.getFloat("yzprice"));
				train.setYwprice(rs.getFloat("ywprice"));
				train.setRzprice(rs.getFloat("rzprice"));
				train.setRwprice(rs.getFloat("rwprice"));
				train.setRoot(rs.getString("root"));
				list.add(train);
			}	
		} finally {
			if (rs != null)
				rs.close();
			if (preStmt != null)
				preStmt.close();
			if (conn != null)
				conn.close();
		}
		return list;
	}
}
```

**        4.在WebRoot文件夹下创建trainManage.jsp文件，页面布局代码如下：**

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>  
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="java.util.List"/> 
<%  
    List trainList = TrainManageDAO.listStudents();  
    request.setAttribute("trainList", trainList);  
%>   

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>火车卡订票系统管理系统</title>
  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	  
	  <div class="trainCSS">
		<table border="0" width="1024" cellpadding="0" cellspacing="0">
		<tr><td width="130" bgcolor="#dfeaf1" valign="top">
		  <table width="100%" cellpadding="0" cellspacing="0"  border="0">
		  	<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.jsp">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="buyManage.jsp">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="peopleManage.jsp">
				<img src="image/hygl.jpg" width="194" height="37" border="0"></a>
			</td></tr>					
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="serverManage.jsp">									  
				<img src="image/spqktj.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="infoManage.jsp">
				<img src="image/lygl.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="exitManage.jsp">
				<img src="image/exit.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
		   </table>
		   </td>
		   <td>
			<table width="100%" height="350" border="1"  cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><TD align="center" valign="top" style="padding-left:20px;padding-top:10px;">
					<div align="left">当前位置: <a href="homepage.jsp">首页</a> > 车次信息管理</div>
					<br /><br />
				<form action="trainManageSelect.jsp" method="post">
				<!-- html:form为struts -->
					出发地: <input type="text" id="start" name="start" style='font-size:18px' width=200/>
					到达地: <input type="text" id="end" name="end" style='font-size:18px'/><br /><br />
					<input type="submit" name="Select" value="查询车次" style='font-size:18px'/>
					  <a href="trainManageInsert.jsp" style='font-size:20px'>插入车次</a>
				</form>
				<table border="2" bordercolor="#12A0F5"> 
		        <tr align=center>
		        	<td>火车列次</td><td>出发地</td><td>目的地</td><td>行车时间</td>
		        	<td>硬座票价</td><td>软座票价</td><td>硬卧票价</td><td>软卧票价</td>
		        	<td>车辆路线</td><td>信息操作</td>
		        </tr>
		        <c:forEach items="${trainList}" var="train">  
                    <tr align=center>  
                        <td>${train.trainid}</td>  
                        <td>${train.start}</td>  
                        <td>${train.end}</td>  
                        <td>${train.time}</td>  
                        <td>${train.yzprice}</td>  
                        <td>${train.rzprice}</td>  
                        <td>${train.ywprice}</td>  
                        <td>${train.rwprice}</td> 
                        <td>
                  <A href="trainManageDetail.jsp?action=see&id=${train.trainid}">查看</A></td> 
                        <td>  
                  <a href="trainManageUpdate.jsp?action=edit&id=${train.trainid}">修改</a>  
                  <a href="trainManageDelete.jsp?action=del&id=${train.trainid}"   
                                onclick="return confirm('确定删除?')">删除</a>  
                        </td>  
                    </tr>  
                </c:forEach>
		        </table>
		        <br /><br />												
				</TD></tr>
			 </table>
			</td>
		</tr>
		<tr><td height="20"></td></tr>
		</table>
	  </div> 	  
	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 北京邮电大学 Eastmount   <BR>
		      	京ICP备10009636号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
        5在WebRoot路径下创建image文件夹，并添加jsp中需要使用的图片资源。

        6.在WebRoot/WEB-INF/lib文件夹中添加mysql-connector-java-5.1.15-bin.jar文件，访问MySQL数据库需要用到。

        7.右键项目，Run As在Tomcat 7.x下即可运行，效果如前图所示。

        其中JSP文件与DAO的交互核心代码如下：

```
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="java.util.List"/> 
<%  
    List trainList = TrainManageDAO.listStudents();  
    request.setAttribute("trainList", trainList);  
%> 
...
<table border="2" bordercolor="#12A0F5"> 
<tr align=center>
    <td>火车列次</td><td>出发地</td><td>目的地</td><td>行车时间</td>
    <td>硬座票价</td><td>软座票价</td><td>硬卧票价</td><td>软卧票价</td>
    <td>车辆路线</td><td>信息操作</td>
</tr>
<c:forEach items="${trainList}" var="train">  
<tr align=center>  
    <td>${train.trainid}</td>  
    <td>${train.start}</td>  
    <td>${train.end}</td>  
    <td>${train.time}</td>  
    <td>${train.yzprice}</td>  
    <td>${train.rzprice}</td>  
    <td>${train.ywprice}</td>  
    <td>${train.rwprice}</td> 
    <td><A href="trainManageDetail.jsp?action=see&id=${train.trainid}">查看</A></td> 
    <td>  
        <a href="trainManageUpdate.jsp?action=edit&id=${train.trainid}">修改</a>  
        <a href="trainManageDelete.jsp?action=del&id=${train.trainid}"   
            onclick="return confirm('确定删除?')">删除</a>  
    </td>  
</tr>  
</c:forEach>
</table>
```





## 四. 关键字查询操作

        关键字查询运行截图如下图所示：


![](https://img-blog.csdn.net/20150524024315036)

![](https://img-blog.csdn.net/20150524024217525)

        该步骤只需要在WebRoot文件夹下创建trainManageSelect.jsp即可，代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>  
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="bean.TrainManage"/>
<jsp:directive.page import="java.util.List"/> 
<%  
	request.setCharacterEncoding("UTF-8");
	response.setCharacterEncoding("UTF-8");
	String mstart = request.getParameter("start");
	String mend = request.getParameter("end");
    List<TrainManage> trainList = TrainManageDAO.findStartEnd(mstart,mend); 
    //List trainList = TrainManageDAO.listStudents();  
    request.setAttribute("trainList", trainList);  
%>   

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>火车卡订票系统管理系统</title>
  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	  
	  <div class="trainCSS">
		<table border="0" width="1024" cellpadding="0" cellspacing="0">
		<tr><td width="130" bgcolor="#dfeaf1" valign="top">
		  <table width="100%" cellpadding="0" cellspacing="0"  border="0">
		  	<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.jsp">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="buyManage.jsp">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="peopleManage.jsp">
				<img src="image/hygl.jpg" width="194" height="37" border="0"></a>
			</td></tr>					
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="serverManage.jsp">									  
				<img src="image/spqktj.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="infoManage.jsp">
				<img src="image/lygl.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="exitManage.jsp">
				<img src="image/exit.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
		   </table>
		   </td>
		   <td>
			<table width="100%" height="350" border="1"  cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><TD align="center" valign="top" style="padding-left:20px;padding-top:10px;">
					<div align="left">当前位置: <a href="homepage.jsp">首页</a> > 车次信息管理</div>
					<br /><br />
				<form action="trainManageSelect.jsp" method="post">
				<!-- html:form为struts -->
					出发地: <input type="text" id="start" name="start" style='font-size:18px' width=200/>
					到达地: <input type="text" id="end" name="end" style='font-size:18px'/><br /><br />
					<input type="submit" name="Select" value="查询车次" style='font-size:18px'/>
					  <a href="trainManageInsert.jsp" style='font-size:20px'>插入车次</a>
				</form>
				<table border="2" bordercolor="#12A0F5"> 
		        <tr align=center>
		        	<td>火车列次</td><td>出发地</td><td>目的地</td><td>行车时间</td>
		        	<td>硬座票价</td><td>软座票价</td><td>硬卧票价</td><td>软卧票价</td>
		        	<td>车辆路线</td><td>信息操作</td>
		        </tr>
		        <c:forEach items="${trainList}" var="train">  
                    <tr align=center>  
                        <td>${train.trainid}</td>  
                        <td>${train.start}</td>  
                        <td>${train.end}</td>  
                        <td>${train.time}</td>  
                        <td>${train.yzprice}</td>  
                        <td>${train.rzprice}</td>  
                        <td>${train.ywprice}</td>  
                        <td>${train.rwprice}</td> 
                        <td>
                        <A href="trainManageDetail.jsp?action=see&id=${train.trainid}">查看</A></td> 
                        <td>  
                        <a href="trainManageUpdate.jsp?action=edit&id=${train.trainid}">修改</a>  
                        <a href="trainManageDelete.jsp?action=del&id=${train.trainid}"   
                            onclick="return confirm('确定删除?')">删除</a>  
                        </td>  
                    </tr>  
                </c:forEach>
		        </table>
		        <br /><br />												
				</TD></tr>
			 </table>
			</td>
		</tr>
		<tr><td height="20"></td></tr>
		</table>
	  </div> 	  
	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 北京邮电大学 Eastmount   <BR>
		      	京ICP备10009636号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
        调用的trainManageDAO.java中的函数findStartEnd(String start,String end)如下，它如果不输入则进行所有查找，也支持单方面出发或到达查找：

```java
//查找记录 某车次
	public static List<TrainManage> findStartEnd(String start,String end) throws Exception {
		List<TrainManage> list = new ArrayList<TrainManage>();
		String sql = null; 
		Connection conn = null;
		Statement statement = null;
		ResultSet rs = null;
		//判断SQL语句
		if(start==""&&end=="") {
			sql = "SELECT * FROM TrainManage;";
		} else if(end=="") {
			sql = "SELECT * FROM TrainManage WHERE start = '"+ start + "';";
		} else if(start=="") {
			sql = "SELECT * FROM TrainManage WHERE end = '"+ end + "';";
		} else {
			sql = "SELECT * FROM TrainManage WHERE start = '" 
					+ start + "' and end = '"+ end +"';"; 
		}
		//执行
		try {
			//链接数据库执行SQL语句
			conn = JDBCConnect.getConnection(); //连接默认数据库
			statement = conn.createStatement();
			System.out.println(start+" "+end);
			System.out.println(sql);
			rs = statement.executeQuery(sql);
			
			//获取查询结果
			while(rs.next()) {
				TrainManage train = new TrainManage();
				train.setTrainid(rs.getString("trainid"));
				train.setStart(rs.getString("start"));
				train.setEnd(rs.getString("end"));
				train.setTime(rs.getString("time"));
				train.setYzprice(rs.getFloat("yzprice"));
				train.setYwprice(rs.getFloat("ywprice"));
				train.setRzprice(rs.getFloat("rzprice"));
				train.setRwprice(rs.getFloat("rwprice"));
				train.setRoot(rs.getString("root"));
				list.add(train);
			}

		} catch (Exception e) {
			System.out.println("错误："+e.getMessage());  
		}
		finally { //依次关闭 记录集 声明 连接对象
			if (rs != null)
				rs.close();
			if (statement != null)
				statement.close();
			if (conn != null)
				conn.close();
		}
		return list;
	}
```
        其中核心步骤如下：

        1.在trainManage.jsp提交POST表单出发地(<input id="start")和到达地(<input id="end")，提交给trainManageSelect.jsp；

        2.在trainManageSelect.jsp中通过获取出发地和到达地，并执行DAO中的TrainManageDAO.java中关键字查找函数:
String mstart = request.getParameter("start");

        String mend = request.getParameter("end");

        List<TrainManage> trainList = TrainManageDAO.findStartEnd(mstart,mend); 

        request.setAttribute("trainList", trainList); 

        3.在JSP中通过EL循环显示结果如下：
<c:forEach items="${trainList}" var="train">  

            <tr align=center>  

                <td>${train.trainid}</td> 

                <td>${train.start}</td>

                 ...

            </tr>

        </c:forEach>
        PS：同时在这过程中你会遇到执行关键字中文查询时显示乱码。

        在trainManageSelect.java文件中通过函数findStartEnd(start,end)打桩输出提交表单"北京 长沙"如下所示：

![](https://img-blog.csdn.net/20150524025427856)

       而且我的服务器、MySQL、JSP、URL3306地址中都设置了UTF-8编码方式，最终解决有两个方面：

       第一个方面是在进行SQL查找时，我最初使用的第一种方法核心代码：

```java
String sql = "SELECT * FROM TrainManage WHERE start = ? and end = ? ;";
Connection conn = JDBCConnect.getConnection(); //连接默认数据库
PreparedStatement preStmt = conn.prepareStatement(sql);
preStmt.setString(1, start);
preStmt.setString(2, end);
ResultSet rs = preStmt.executeQuery();
```
        但是在查询英文Beijing => Guizhou 时能正确显示，而中文就没有结果，后改为带引号（"'"+string+"'"）的参数方式，核心代码如下：

```java
String sql = "SELECT * FROM TrainManage WHERE start = '" 
		+ start + "' and end = '"+ end +"';"; 
Connection conn = JDBCConnect.getConnection(); //连接默认数据库
Statement statement = conn.createStatement();
ResultSet rs = statement.executeQuery(sql);
while(rs.next()){...}
```
        后来使用该方法仍然存在乱码，但是这次我发现了原因，在form提交表单时采用Get方法就会出现中文乱码，虽然URL中显示的是：

        http://localhost:8080/TrainDatabase/trainManageSelect.jsp?start=北京&end=长沙&sumbit=提交

        但是可能URL转String出现乱码的，改为POST即可实现正常查询，显示正常中文。

        注意：前提是我的所有编码字符集都统一为UTF-8。








## 五. 插入操作调用Servlet

        该部分运行截图如下图所示：


![](https://img-blog.csdn.net/20150524040457220)
        在src中创建文件夹servlet，同时创建**Servlet**文件InsertTrainAction.java。
```java
package servlet;

import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import bean.TrainManage;
import util.JDBCConnect;
import DAO.TrainManageDAO;

public class InsertTrainAction extends HttpServlet {

	public InsertTrainAction() {
		super();
	}

	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		// Put your code here
	}

	//The doGet method of the servlet	 
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		out.println("<HTML>");
		out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
		out.println("  <BODY>");
		out.print("    This is ");
		out.print(this.getClass());
		out.println(", using the GET method");
		out.println("  </BODY>");
		out.println("</HTML>");
		out.flush();
		out.close();
	}

	//The doPost method of the servlet
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		response.setCharacterEncoding("UTF-8"); //设置输出编码  
	    request.setCharacterEncoding("UTF-8");  
		
		try {
			//插入数据库
			String m_trainid = request.getParameter("trainid");
			String m_start = request.getParameter("start");
			String m_end = request.getParameter("end");
			String m_time = request.getParameter("time");
			String m_yz = request.getParameter("yzprice");
			Float m_yzprice = Float.parseFloat(m_yz);
			String m_rz = request.getParameter("rzprice");
			Float m_rzprice = Float.parseFloat(m_rz);
			String m_yw = request.getParameter("ywprice");
			Float m_ywprice = Float.parseFloat(m_yw);
			String m_rw = request.getParameter("rwprice");
			Float m_rwprice = Float.parseFloat(m_rw);
			String m_root = request.getParameter("root");
				
			TrainManage train = new TrainManage();
			train.setTrainid(m_trainid);
			train.setStart(m_start);
			train.setEnd(m_end);
			train.setTime(m_time);
			train.setYzprice(m_yzprice);
			train.setRzprice(m_rzprice);
			train.setYwprice(m_ywprice);
			train.setRwprice(m_rwprice);
			train.setRoot(m_root);
			  
		    int success  = TrainManageDAO.insert(train);  
			request.setAttribute("success", success);  
			//设置全路径 否则trainManage.jsp跳转到servlet/trainManage.jsp路径下不存在
		    response.sendRedirect("http://localhost:8080/TrainDatabase/trainManage.jsp");  
		    
		} catch(Exception e) {
			System.out.println("错误："+e.getMessage());  
            response.sendRedirect("http://localhost:8080/TrainDatabase/trainManage.jsp");  
		}
		
	}

	public void init() throws ServletException {
		// Put your code here
	}

}
```
 
       它会自动配置WebRoot/WEB-INF/web.xml文件中servlet和其映射。同时在WebRoot中添加trainManageInsert.jsp。

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>  
<%@ page session = "true" %>
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="bean.TrainManage"/>
<jsp:directive.page import="java.util.List"/> 

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>火车卡订票系统管理系统</title>
  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	  
	  <div class="trainCSS">
		<table border="0" width="1024" cellpadding="0" cellspacing="0">
		<tr><td width="130" bgcolor="#dfeaf1" valign="top">
		  <table width="100%" cellpadding="0" cellspacing="0"  border="0">
		  	<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.jsp">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="buyManage.jsp">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="peopleManage.jsp">
				<img src="image/hygl.jpg" width="194" height="37" border="0"></a>
			</td></tr>					
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="serverManage.jsp">									  
				<img src="image/spqktj.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="infoManage.jsp">
				<img src="image/lygl.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="exitManage.jsp">
				<img src="image/exit.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
		   </table>
		   </td>
		   <td>
			<table width="100%" height="350" border="1"  cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><TD align="center" valign="top" style="padding-left:20px;padding-top:10px;">
					<div align="left">当前位置: <a href="homepage.jsp">首页</a> > 
						<a href="trainManage.jsp">车次信息管理</a> > 插入车次</div>
					<br /><br />
				<form action="/TrainDatabase/servlet/InsertTrainAction" method="post">
				<table border="1" bordercolor="#12A0F5" align=center> 
					<tr>
						<td>火车车次:</td><td><input type="text" id="trainid" 
							name="trainid" style='font-size:18px' width=200/></td>
					</tr>
					<tr>
						<td>出发地:</td><td><input type="text" id="start" 
							name="start" style='font-size:18px' width=200/></td>
					</tr>
					<tr>
						<td>到达地:</td><td><input type="text" id="end" 
							name="end" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>行车时间:</td><td><input type="text" id="time" 
							name="time" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>硬座票价:</td><td><input type="text" id="yzprice" 
							name="yzprice" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>软座票价:</td><td><input type="text" id="rzprice" 
							name="rzprice" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>硬卧票价:</td><td><input type="text" id="ywprice" 
							name="ywprice" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>软卧票价:</td><td><input type="text" id="rwprice" 
							name="rwprice" style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>车辆路线:</td><td>
							<textarea id="root" name="root" rows="5" 
								style="width:300px;font-size:18px"></textarea> 
						</td>
					</tr>
					<tr>
						<td colspan=2 align=center><input type="submit" 
							name="Submit" value="提交" style='font-size:18px' width=200/></td>
					</tr>
				</table>
				</form>
		        <br /><br />												
				</TD></tr>
			 </table>
			</td>
		</tr>
		<tr><td height="20"></td></tr>
		</table>
	  </div>
	  
	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 北京邮电大学 Eastmount<BR>
		      	京ICP备10009636号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
该方法不同于前面的，它是通过Servlet实现的。主要是在采用JSP调用DAO中插入操作总是失败，其流程如下：
        1.点击"插入车次"通过超链接到trainManageInsert.jsp，然后通过表单POST提交到Java文件servlet；

        <form action="/TrainDatabase/servlet/InsertTrainAction" method="post">
2.在InsertTrainAction.java中通过doPost方法获取提交的表单参数，再调用TrainManageDAO.insert(train)插入数据。最后通过response.sendRedirect重定向到管理页面。
 注意：在插入过程中我也是采用"'"+ name +"'"的方式，而不是?来添加参数，否则总报错，详见代码trainManageDAO.java函数insert(TrainManage train)。





## 六. 删除操作

        运行效果如下图所示，点击删除后弹出提示框“确定”后删除：


![](https://img-blog.csdn.net/20150524042106752)

        同时在删除后，通过JavaScript实现5秒后返回管理界面的效果。(因为在PHP网站中我常用这个功能，觉得有意思在JSP中就加入了该功能)


![](https://img-blog.csdn.net/20150524042116721)
        在WebRoot下创建trainManageDelete.jsp文件实现删除操作，代码如下：
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>  
<%@ page session = "true" %>
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="bean.TrainManage"/>
<jsp:directive.page import="java.util.List"/> 
<%
	String action = request.getParameter("action");
	String id = request.getParameter("id");
	if(id == null || id==""){	out.println("没有选中删除的车次");	 return;	}
	if("del".equals(action)){
		int isDelete = TrainManageDAO.delete(id);  
    	request.setAttribute("isDelete", isDelete);  
	}
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
	<title>火车卡订票系统管理系统</title>
  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	  
	  <div class="trainCSS">
		<table border="0" width="1024" cellpadding="0" cellspacing="0">
		<tr><td width="130" bgcolor="#dfeaf1" valign="top">
		  <table width="100%" cellpadding="0" cellspacing="0"  border="0">
		  	<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.jsp">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="buyManage.jsp">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="peopleManage.jsp">
				<img src="image/hygl.jpg" width="194" height="37" border="0"></a>
			</td></tr>					
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="serverManage.jsp">									  
				<img src="image/spqktj.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="infoManage.jsp">
				<img src="image/lygl.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="exitManage.jsp">
				<img src="image/exit.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
		   </table>
		   </td>
		   <td>
			<table width="100%" height="350" border="1"  cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><TD align="center" valign="top" style="padding-left:20px;padding-top:10px;">
					<div align="left">当前位置: <a href="homepage.jsp">首页</a> > 
						<a href="trainManage.jsp">车次信息管理</a> > 删除车次</div>
					<br /><br />
					<table border="2" bordercolor="#12A0F5" width="50%" height="80%">
			        	<tr align=center>
			            	<td>
			            		<H2 style="COLOR: #880000">删除成功</H2>  
			            		<H4>管理员  xxx 成功删除该车次信息<br />系统将在  
								    <input type="text" style='font-size:18px; border:0px; width:20px;'   
								        readonly="true" value="5" id="time">秒后返回火车票系统管理员车次管理界面 
							    </H4>  
			            	</td>
			        	</tr>
			        </table>
			        <!-- 时间函数 -->  
					<script language="javascript">  
					    var t = 5;  
					    var time = document.getElementById("time");  
					    function fun()  
					    {  
					        t--;  
					        time.value = t;  
					        if(t<=0)  
					        {  
					            location.href="trainManage.jsp";   
					        }  
					    }  
					    var inter = setInterval("fun()",1000);  
					</script>  
		        <br /><br />												
				</TD></tr>
			 </table>
			</td>
		</tr>
		<tr><td height="20"></td></tr>
		</table>
	  </div>
	  
	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 北京邮电大学 Eastmount   <BR>
		      	京ICP备10009636号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
         删除的核心代码如下：

        1.在trainManage.jsp中定义删除超链接及提示框：
<a href="trainManageDelete.jsp?action=del&id=${train.trainid}"   

                onclick="return confirm('确定删除?')">删除</a>  

        2.在DAO中trainManageDAO.java文件中定义SQL删除操作：
        public static int delete(String id) throws Exception {
                String sql = "DELETE FROM TrainManage WHERE trainid = ? ";
                return JDBCConnect.executeUpdate(sql, id);
        }
        3.在trainManageDelete.jsp中获取传递的trainid(火车车次)值并调用该函数实现删除操作：
<%
        String action = request.getParameter("action");
        String id = request.getParameter("id");
        if(id == null || id==""){out.println("没有选中删除的车次");return;}
        if("del".equals(action)){
                int isDelete = TrainManageDAO.delete(id);  

                request.setAttribute("isDelete", isDelete);  
        }

        %>




## 七. 更新操作及Servlet

        具体流程是：首先点击修改，通过超链传递trainid（火车车次）到修改界面，然后通过调用trainManageDAO.java中find查找该车次的所有信息并显示。再通过提交POST及Servlet到UpdateTrainAction.java中，在doPost方法中调用DAO中update(TrainMManage)实现更新。其中运行结果如下图所示：


![](https://img-blog.csdn.net/20150524043807805)
        该方法与插入数据的思路类似，都是通过Servlet实现。其中WebRoot文件夹下创建trainManageUpdate.jsp。其中车次是只读的。

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>  
<%@ page session = "true" %>
<jsp:directive.page import="DAO.TrainManageDAO"/>  
<jsp:directive.page import="bean.TrainManage"/>
<jsp:directive.page import="java.util.List"/> 
<%
	String action = request.getParameter("action");
	String id = request.getParameter("id");
	TrainManage train = TrainManageDAO.find(id);  
    request.setAttribute("train", train);   
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>火车卡订票系统管理系统</title>
  </head>
  
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	  
	  <div class="trainCSS">
		<table border="0" width="1024" cellpadding="0" cellspacing="0">
		<tr><td width="130" bgcolor="#dfeaf1" valign="top">
		  <table width="100%" cellpadding="0" cellspacing="0"  border="0">
		  	<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.jsp">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="buyManage.jsp">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="peopleManage.jsp">
				<img src="image/hygl.jpg" width="194" height="37" border="0"></a>
			</td></tr>					
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="serverManage.jsp">									  
				<img src="image/spqktj.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="infoManage.jsp">
				<img src="image/lygl.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="exitManage.jsp">
				<img src="image/exit.jpg" width="194" height="37" border="0"></a>								
			</td></tr>
		   </table>
		   </td>
		   <td>
			<table width="100%" height="350" border="1"  cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><TD align="center" valign="top" style="padding-left:20px;padding-top:10px;">
					<div align="left">当前位置: <a href="homepage.jsp">首页</a> > <a href="trainManage.jsp">车次信息管理</a> > 插入车次</div>
					<br /><br />
					<H2 style="COLOR: #880000">修改车次信息</H2> 
				<form action="/TrainDatabase/servlet/UpdateTrainAction" method="post">	
				<table border="1" bordercolor="#12A0F5" align=center> 
					<tr>
						<td>火车车次:</td><td><input type="text" id="trainid" name="trainid" 
							value=${train.trainid} style='font-size:18px' width=200 readonly="true""/></td>
					</tr>
					<tr>
						<td>出发地:</td><td><input type="text" id="start" name="start" 
							value=${train.start} style='font-size:18px' width=200/></td>
					</tr>
					<tr>
						<td>到达地:</td><td><input type="text" id="end" name="end" 
							value=${train.end} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>行车时间:</td><td><input type="text" id="time" name="time" 
							value=${train.time} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>硬座票价:</td><td><input type="text" id="yzprice" name="yzprice" 
							value=${train.yzprice} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>软座票价:</td><td><input type="text" id="rzprice" name="rzprice" 
							value=${train.rzprice} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>硬卧票价:</td><td><input type="text" id="ywprice" name="ywprice" 
							value=${train.ywprice} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>软卧票价:</td><td><input type="text" id="rwprice" name="rwprice" 
							value=${train.rwprice} style='font-size:18px'/></td>
					</tr>
					<tr>
						<td>车辆路线:</td><td>
							<textarea id="root" name="root" rows="5" 
								style="width:300px;font-size:18px">${train.root}</textarea>  
						</td>
					</tr>
					<tr>
						<td colspan=2 align=center><input type="submit" name="Submit" 
							value="提交" style='font-size:18px' width=200/></td>
					</tr>
				</table>
				</form>
		        <br /><br />												
				</TD></tr>
			 </table>
			</td>
		</tr>
		<tr><td height="20"></td></tr>
		</table>
	  </div>
	  
	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 北京邮电大学 Eastmount   <BR>
		      	京ICP备10009636号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
        然后再src/servlet中创建Servlet类，即UpdateTrainAction.java文件。代码如下：

```java
package servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import DAO.TrainManageDAO;
import bean.TrainManage;

public class UpdateTrainAction extends HttpServlet {

	public UpdateTrainAction() {
		super();
	}

	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		// Put your code here
	}

	//The doGet method of the servlet.
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		out.println("<HTML>");
		out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
		out.println("  <BODY>");
		out.print("    This is ");
		out.print(this.getClass());
		out.println(", using the GET method");
		out.println("  </BODY>");
		out.println("</HTML>");
		out.flush();
		out.close();
	}

	//The doPost method of the servlet
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		response.setCharacterEncoding("UTF-8"); //设置输出编码  
	    request.setCharacterEncoding("UTF-8");  
		
		try {
			//更新数据库
			String m_trainid = request.getParameter("trainid");
			String m_start = request.getParameter("start");
			String m_end = request.getParameter("end");
			String m_time = request.getParameter("time");
			String m_yz = request.getParameter("yzprice");
			Float m_yzprice = Float.parseFloat(m_yz);
			String m_rz = request.getParameter("rzprice");
			Float m_rzprice = Float.parseFloat(m_rz);
			String m_yw = request.getParameter("ywprice");
			Float m_ywprice = Float.parseFloat(m_yw);
			String m_rw = request.getParameter("rwprice");
			Float m_rwprice = Float.parseFloat(m_rw);
			String m_root = request.getParameter("root");
				
			TrainManage train = new TrainManage();
			train.setTrainid(m_trainid);
			train.setStart(m_start);
			train.setEnd(m_end);
			train.setTime(m_time);
			train.setYzprice(m_yzprice);
			train.setRzprice(m_rzprice);
			train.setYwprice(m_ywprice);
			train.setRwprice(m_rwprice);
			train.setRoot(m_root);
			  
		    int success  = TrainManageDAO.update(train);  
			request.setAttribute("success", success);  
			//设置全路径 否则trainManage.jsp跳转到servlet/trainManage.jsp路径下不存在
		    response.sendRedirect("http://localhost:8080/TrainDatabase/trainManage.jsp");  
		    
		} catch(Exception e) {
			System.out.println("错误："+e.getMessage());  
            response.sendRedirect("http://localhost:8080/TrainDatabase/trainManage.jsp");  
		}
	}

	public void init() throws ServletException {
		// Put your code here
	}

}
```
        你可能发现，update函数只修改了start和end，其它你可以自己添加，方法类似。同时如果参数过多建议使用下面的方法替代：

```java
public static int update(TrainManage train) throws Exception {
     /**
       * String sql = "UPDATE TrainManage SET start = ?, end = ? WHERE trainid = ? ";
       * return JDBCConnect.executeUpdate(sql, train.getStart(), train.getEnd(), train.getTrainid());
       */
       String sql = "UPDATE TrainManage SET start = '"+ train.getStart() +
          "', end = '" + train.getEnd() + "' WHERE trainid = '" + train.getTrainid() +"';";
       System.out.println(sql);
       return JDBCConnect.executeUpdate(sql);
}
```
还有一个查看详情就不介绍了，参看源代码吧~

![](https://img-blog.csdn.net/20150524051034885)





## 八. 总结

        最后文章就不总结了，因为每步都讲得非常清楚，无论是步骤、思想，还是源代码截图。作者真的很用心的写这篇文章，从晚上8点写到了凌晨5点，中间打了两把dota2。一方面由于我也认为这些都非常重要又基础，另一方面为哪些初学者做Java网站的。

        最后希望文章对你有所帮助！如果文章有错误或不足之处，还请海涵~真的有点累了睡了。有时候想想值不值得这样写文章，心安虽好但对身体不好。



        （By:Eastmount 2015-5-24 凌晨5点   [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）







