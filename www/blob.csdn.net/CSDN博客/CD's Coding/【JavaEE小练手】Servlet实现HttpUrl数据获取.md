# 【JavaEE小练手】Servlet实现HttpUrl数据获取 - CD's Coding - CSDN博客





2015年07月07日 23:14:12[糖果天王](https://me.csdn.net/okcd00)阅读数：544标签：[javaee																[servlet																[服务器																[jsp																[同步](https://so.csdn.net/so/search/s.do?q=同步&t=blog)
个人分类：[DIY](https://blog.csdn.net/okcd00/article/category/1775037)





## 前言

嘛……对于深恶痛绝的JAVA，不得已还是得稍微做些东西，再怎么恨得牙痒痒烦的头冒烟也得写啊……

JAVA来说最烦的就是分支太多，选择太多，坑太多，环境太难配……

呐，这不是，失败的想法——HttpUrl为手机动态同步服务器数据，的半成品拿来作以后写项目的备忘了么~

咳咳，这个大概是一个什么呢……是一个**使用JAVAEE搭建一个使用JSP页面获取所需值**的项目




## 环境说明

Win8.1 Professional

Java 1.8.0_25

JavaEE Eclipse Java EE IDE for Web Developers. Version: Luna Service Release 1 (4.4.1)




## 过程记录

![](https://img-blog.csdn.net/20150707231851081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


首先， New 一个 Project 的时候要注意是 Dynamic Web Project， 即动态网页项目




![](https://img-blog.csdn.net/20150707231935502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


项目名字，然后Finish




![](https://img-blog.csdn.net/20150707231955782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对于这个jsp的实现自然需要在Java Resources里添加代码，需要的是Method和Servlet两个package，本来想像上图这种方式添加包，后来发现——




![](https://img-blog.csdn.net/20150707232140673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


新建class的时候就可以加package啊！




【示例】

我这里的Method里写的是 ReturnValue.java



```java
package Method;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ReturnValue 
{
	private String info = null;
	
	public void setString(String t)
	{
		this.info=t;
	}
	public String getValue()
	{
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		//this.info = sdf.format(new Date());
		return info;
	}
}
```


然而servlet里写的是 COMServlet.java



```java
package Servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import Method.ReturnValue;

//import com.sun.org.apache.xml.internal.serialize.Printer;

/**
 * Servlet implementation class COMServlet
 */
@WebServlet("/COMServlet")
public class COMServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public COMServlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//url形式：http://xxx.xxx.xxx.xxx:8080/项目名称/COMServlet?op=getValue
		//8080端口是不一定的 得看看你电脑是不是用这个作为http协议的端口 一般是这个啦
		String op = request.getParameter("op");
		if(op!=null){
			switch(op){
			case "getValue":
				sendValue(request,response);
			}
		}
	}

	
	private void sendValue(HttpServletRequest request,
			HttpServletResponse response) {
		response.setContentType("text/html;charset=utf-8");
		response.setCharacterEncoding("UTF-8");
		PrintWriter writer;
		
		//获得系统中的值
		ReturnValue r = new ReturnValue();
		
		try {
			writer = response.getWriter();
			writer.println(r.getValue());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
```






![](https://img-blog.csdn.net/20150707232203066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后在WebContent里添加文件 xxx.jsp

body部分写一个超链接，超链接到 servlet，其中?后面是传参，详见上文中代码，即传入参数op为getValue



```java
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<body>
	<a href="COMServlet?op=getValue"> Just_A_Test </a>
</body>
</html>
```






![](https://img-blog.csdn.net/20150707232301219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


写完了之后对jsp右键 run on server




![](https://img-blog.csdn.net/20150707232313577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


就会有显示了，这个截图中由于info没有被更新，所以返回是null

返回服务器实时更新的值，示例如下，为返回服务器时间：



```java
public String getValue()
	{
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		this.info = sdf.format(new Date());
		return info;
	}
```](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)](https://so.csdn.net/so/search/s.do?q=javaee&t=blog)




