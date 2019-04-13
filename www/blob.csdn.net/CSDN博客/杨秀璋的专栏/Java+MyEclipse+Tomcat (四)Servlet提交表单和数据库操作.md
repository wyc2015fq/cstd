
# Java+MyEclipse+Tomcat (四)Servlet提交表单和数据库操作 - 杨秀璋的专栏 - CSDN博客

2015年05月15日 01:29:03[Eastmount](https://me.csdn.net/Eastmount)阅读数：10567


前面三篇文章讲述了如何配置MyEclipse和Tomcat开发JSP网站、如何配置Servlet简单实现表单提交、如何配置MySQL实现JSP数据库查询。
这篇文章主要讲述Servlet表单的提交、Java中实现数据库的查询操作和自己遇到的瓶颈及理解。Java Web基础性文章，希望对大家有所帮助~
[Java+MyEclipse+Tomcat
 (一)配置过程及jsp网站开发入门](http://blog.csdn.net/eastmount/article/details/45492065)
[Java+MyEclipse+Tomcat
 (二)配置Servlet及简单实现表单提交](http://blog.csdn.net/eastmount/article/details/45536369)
[Java+MyEclipse+Tomcat (三)配置MySQL及查询数据显示在JSP网页中](http://blog.csdn.net/eastmount/article/details/45653615)
两个项目的免费下载地址(希望对你有所帮助)：
[http://download.csdn.net/detail/eastmount/8701707](http://download.csdn.net/detail/eastmount/8701707)

## 一. Servlet表单提交
新建Web Project，项目名称为TestServlet01。项目结构如下图所示：
![](https://img-blog.csdn.net/20150514200913627)
然后修改index.jsp代码如下：
```python
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'index.jsp' starting page</title>
	<style>
		body, div, td, input {font-size:18px; margin:0px; }
		.line {margin:2px; }
	</style>
  </head>
  
  <body>
  <form action="/TestServlet01/servlet/PostServlet" method="POST">
  <div align="center">
	<br/>
	<fieldset style='width:60%'>
		<legend>填写用户信息</legend>
		<br/>
		<div class='line'>
			<div align="left">出发地:
				<input type="text" id="start" name="start" style='font-size:18px' width=200/>
			</div>
		</div>
		<div class='line'>
			<div align="left">到达地:
				<input type="text" id="end" name="end" style='font-size:18px'/>
			</div>
		</div>
		<div class='line'>
			<br />
			<div align="left">请选择性别：
				<input type="radio" name="sex" value="男" id="sexMale">
				<label for="sexMale">男</label>
				<input type="radio" name="sex" value="女" id="sexFemale">
				<label for="sexFemale">女</label>
		    </div>
		</div>
		<div class='line'>
			<div align="left">请选择您的爱好：
				<input type="checkbox" name="interesting" value="音乐" id="i1">
				<label for="i1">音乐</label> 
				<input type="checkbox" name="interesting" value="旅游" id="i2">
				<label for="i2">旅游</label> 
				<input type="checkbox" name="interesting" value="运动" id="i3">
				<label for="i3">运动</label> 
			</div>
		</div>
		<div class='line'>
			<div align="left">请选择车票类型：
				<select name="seat">
					<option>---请选择乘坐类型---</option>
					<optgroup label="卧铺">
						<option value="上铺">上铺</option>
						<option value="中铺">中铺</option>
						<option value="下铺">下铺</option>
					</optgroup>
					<optgroup label="其他">
						<option value="硬座">硬座</option>
						<option value="软座">软座</option>
						<option value="站票">站票</option>
					</optgroup>
				</select>
			</div>
		</div>
		<div class='line'>
			<br />
			<div align="left" class='leftDiv'>备注信息：</div>
			<div align="left" class='rightDiv'>
				<textarea name="description" rows="8" style="width:300px; ">请填写备注信息... </textarea>
			</div>
		</div>
		<div class='line'>
			<div align="left">
				<br/><input type="submit" name="Select" value="提交信息" style='font-size:18px'/><br/>
			</div>
		</div>
	</fieldset>
</div>
</form>
</body>
</html>
```
运行效果如下图所示：
![](https://img-blog.csdn.net/20150514200715379)
核心代码：
<form action="/TestServlet01/servlet/PostServlet" method="POST">
出发地:<input type="text" id="start" name="start" style='font-size:18px'/>
<input type="submit" name="Select" value="提交信息"/>
</form>
然后再src中右键添加Package，包名为servlet；再添加Servlet文件，文件名PostServlet.java。选择![](https://img-blog.csdn.net/20150514201531682)图标。前面文章讲述过Servlet的手动配置过程，包括servlet类、映射等，现在它自动生成的WebRoot/WEB-INF/web.xml文件如下：
```python
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0" 
	xmlns="http://java.sun.com/xml/ns/javaee" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
	http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
  <display-name></display-name>
  
  
  <servlet>
    <description>This is the description of my J2EE component</description>
    <display-name>This is the display name of my J2EE component</display-name>
    <servlet-name>PostServlet</servlet-name>
    <servlet-class>servlet.PostServlet</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>PostServlet</servlet-name>
    <url-pattern>/servlet/PostServlet</url-pattern>
  </servlet-mapping>	
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app>
```
同时修改src/servlet/PostServlet.java文件，采用POST方法显示表单数据：
```python
package servlet;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class PostServlet extends HttpServlet {
	public PostServlet() {
		super();
	}
	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		// Put your code here
	}
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
	/**
	 * The doPost method of the servlet. <br>
	 *
	 * This method is called when a form has its tag value method equals to post.
	 * 
	 * @param request the request send by the client to the server
	 * @param response the response send by the server to the client
	 * @throws ServletException if an error occurred
	 * @throws IOException if an error occurred
	 */
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		response.setCharacterEncoding("UTF-8"); //设置输出编码
		request.setCharacterEncoding("UTF-8");
		
		String startName = request.getParameter("start"); //获取出发地
		String endName = request.getParameter("end");     //获取到达地
		String sex = request.getParameter("sex");         //获取性别
		String [] interest = request.getParameterValues("interesting"); //获取兴趣
		String seat = request.getParameter("seat");        //获取座位
		String info = request.getParameter("description"); //获取备注信息
		
		response.setContentType("text/html");  //设置输出类型
		PrintWriter out = response.getWriter(); //获取out对象
		
		out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		out.println("<HTML>");
		out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
		out.println("  <BODY>");
		out.println("<H2>出发地："+ startName +"</H2>");
		out.println("<H2>到达地："+ endName +"</H2>");
		out.println("<H2>性别："+ sex +"</H2>");
		out.println("<H2>兴趣");
		for(String str:interest) {
			out.println(str+" ");
		}
		out.println("</H2><H2>座位类型："+ seat +"</H2>");
		out.println("<H2>备注信息："+ info +"</H2>");
		out.println("  </BODY>");
		out.println("</HTML>");
		out.flush();
		out.close();
	}
	/**
	 * Initialization of the servlet. <br>
	 *
	 * @throws ServletException if an error occurs
	 */
	public void init() throws ServletException {
		// Put your code here
	}
}
```
运行效果如下图所示：
![](https://img-blog.csdn.net/20150514204653618)


## 二. Servlet数据库查询
还是使用上面的项目进行修改，实现Servlet数据库查询操作。数据库配置可以参照上一篇博客配置MySQL的过程，我新建数据库test01，插入表Train，表中数据如下图：
![](https://img-blog.csdn.net/20150515005722186)
然后修改index.jsp，代码如下：
```python
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'index.jsp' starting page</title>
	<style>
		body, div, td, input {font-size:18px; margin:0px; }
		.line {margin:2px; }
	</style>
  </head>
  
  <body>
  <form action="/TestServlet01/servlet/PostServlet" method="POST">
  <div align="center">
	<br/>
	<fieldset style='width:60%'>
		<legend>填写用户信息</legend>
		<br/>
		<div class='line'>
			<div align="left">出发地:
				<input type="text" id="start" name="start" style='font-size:18px' width=200/>
			</div>
		</div>
		<div class='line'>
			<div align="left">
				<br/><input type="submit" name="Select" value="提交信息" style='font-size:18px'/><br/>
			</div>
		</div>
	</fieldset>
</div>
</form>
</body>
</html>
```
修改的PostServlet.java代码如下：
```python
package servlet;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class PostServlet extends HttpServlet {
	//自定义变量
	private Connection connection = null; //定义数据库连接对象
	private String driverName = "com.mysql.jdbc.Driver"; //数据库驱动器
	private String userName = "root"; //数据库用户名
	private String userPasswd = "123456"; //密码
	private String dbName = "test01"; //数据库名称
	private String tableName = "Train"; //表明
	//连接字符串 数据库地址URL MySQL数据库端口3306
	private String url = "jdbc:mysql://localhost:3306/" + dbName + "?user="    
            + userName + "&password=" + userPasswd;
		
	//初始化方法
	public void init(ServletConfig config) throws ServletException
	{
		super.init(config);
	}
	
	public PostServlet() {
		super();
	}
	//处理GET请求方法
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException
	{
		response.setCharacterEncoding("UTF-8"); //设置输出编码
		request.setCharacterEncoding("UTF-8");
		response.setContentType("text/html");  //设置输出类型
		PrintWriter out = response.getWriter(); //获取out对象
		
		try {
			//数据库操作
			Class.forName(driverName).newInstance();
			connection = DriverManager.getConnection(url);
			Statement statement = connection.createStatement();
			String startName = request.getParameter("start"); //获取出发地
			//注意:startName需要加单引号 否则报错 ——错误：Unknown column 'BeiJing' in 'where clause'
			String sql = "SELECT * FROM " + tableName +" WHERE startname='" + startName+"';";
			if(startName=="") {
				sql = "SELECT * FROM " + tableName;
			}
			ResultSet rs = statement.executeQuery(sql); 
			
			out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
			out.println("<HTML>");
			out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
			out.println("  <BODY>");
			out.println("  <fieldset style='width:60%' ><legend>搜索结果</legend><br />");
			out.println("	<TABLE align='center'border='1' cellspacing='1' cellpadding='1'>");
			out.println("		<TR><TH>车号</TH><TH>出发地</TH><TH>到达地</TH></TR>");
			//循环输出查询结果
			while(rs.next()) {
				out.println("		<TR><TD>" + rs.getString(1) + "</TD>");
				out.println("		<TD>" + rs.getString(2) + "</TD>");
				out.println("		<TD>" + rs.getString(3) + "</TD></TR>");
			}
			out.println("  </TABLE>");
			out.println("  </fieldset>");
			out.println("  </BODY>");
			out.println("</HTML>");
			out.flush();
			out.close();
			rs.close(); // 关闭记录集
			statement.close(); // 关闭声明
		
		} catch(Exception e) {
			System.out.println("错误："+e.getMessage());
			response.sendRedirect("index.jsp");
		}
	}
	//处理POST请求方法
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException 
	{	
		doGet(request,response);
	}
	//销毁方法
	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		try {
			connection.close(); // 关闭连接对象
		}catch(Exception e) {
			System.out.println("关闭数据库错误："+e.getMessage());
		}
	}
}
```
同时WebRoot/WEB-INF/web.xml文件Servlet映射都没有变化，需要在WebRoot/WEB-INF/lib中添加mysql-connector-java-5.1.15-bin.jar，否则会报错“com.mysql.jdbc.Driver错误”。
运行效果如下图所示：
![](https://img-blog.csdn.net/20150515011050910)
![](https://img-blog.csdn.net/20150515011120332)
写到此处我产生了一个疑问，当表单提交信息时，获取数据库的结果有两种方法：
1.第一种是上一篇博客中写到的，在JSP中通过<% ....%>调用Java代码实现连接数据库，获取MySQL表中数据并显示；
2.第二种就是这篇博客中写到的，在JSP中通过Post方法提交表单Form，在Java中通过Servlet获取请求/响应，再通过Java中out.println("<HTML>...")输出数据库中值。
就这两种方法而言，我想实现的功能是：JSP就赋值布局，显示界面；Java就负责连接数据库、数据库增删改查，处理结果再返回给JSP中显示，而不是相互嵌套的。换句话说：JSP中点击“提交”按钮，TextBox中传递出发地，Java中介绍请求，数据库查询，得到的结果再返回给JSP中显示。
那怎么实现呢？后面的文章可能会讲到。
DAO和Java Bean是对JDBC进行分层、模块化的最有效两个方法。DAO(数据库操作对象，Database Access Object)是JDBC下常用模式，DAO出现之前，操作数据库的代码与业务代码都出现在Servlet或者JSP中，不利用业务代码的分离。DAO出现后，所有与数据库相关的操作全被拿到了DAO层实现，Servlet或JSP只操作Java Bean或者DAP层，而DAO层值操作数据库。
PS：非常高兴我自己通过实际项目找到了这个难点，然后又找到了解决方法。虽然才学习Java Web一周时间，还是学到很多东西的。个人感觉DAO类似于中间件的东西吧！最后希望文章对你有所帮助，这篇文章是讲述Servlet连接MySQL数据库及表单交互之间的知识。如果文章有不足或错误的地方，还请海涵！下一篇文章讲讲Session和一个典型简单的界面布局等相关知识吧！
（By:Eastmount 2015-5-15 半夜1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）





