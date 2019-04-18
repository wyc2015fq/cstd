# Servlet实现文件上传，可多文件上传 - z69183787的专栏 - CSDN博客
2016年04月26日 16:11:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1115
一、Servlet实现文件上传，需要添加第三方提供的jar包
下载地址：
1) [commons-fileupload-1.2.2-bin.zip](http://labs.mop.com/apache-mirror//commons/fileupload/binaries/commons-fileupload-1.2.2-bin.zip)     
 :   [点击打开链接](http://commons.apache.org/fileupload/download_fileupload.cgi)
2) [commons-io-2.3-bin.zip](http://labs.renren.com/apache-mirror//commons/io/binaries/commons-io-2.3-bin.zip)                      
 :    [点击打开链接](http://commons.apache.org/io/download_io.cgi)
接着把这两个jar包放到 lib文件夹下：
![](https://img-my.csdn.net/uploads/201204/30/1335785379_5732.jpg)
二：
文件上传的表单提交方式必须是POST方式，
编码类型：enctype="multipart/form-data"，默认是 application/x-www-form-urlencoded
比如：<form action="FileUpLoad"enctype="multipart/form-data"method="post">
三、举例：
1.fileupload.jsp
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'fileupload.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
  </head>
  
  <body>
     <!-- enctype 默认是 application/x-www-form-urlencoded -->
     <form action="FileUpLoad" enctype="multipart/form-data" method="post" >
        
               用户名：<input type="text" name="usename"> <br/>
               上传文件：<input type="file" name="file1"><br/>
              上传文件： <input type="file" name="file2"><br/>
              <input type="submit" value="提交"/>
     
     </form>
     
     
     
  </body>
</html>
```
2.实际处理文件上传的 FileUpLoad.java
```java
package com.servlet.fileupload;
import java.io.File;
import java.io.*;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
/**
 * 
 * @author Administrator
 * 文件上传
 * 具体步骤：
 * 1）获得磁盘文件条目工厂 DiskFileItemFactory 要导包
 * 2） 利用 request 获取 真实路径 ，供临时文件存储，和 最终文件存储 ，这两个存储位置可不同，也可相同
 * 3）对 DiskFileItemFactory 对象设置一些 属性
 * 4）高水平的API文件上传处理  ServletFileUpload upload = new ServletFileUpload(factory);
 * 目的是调用 parseRequest（request）方法  获得 FileItem 集合list ，
 *     
 * 5）在 FileItem 对象中 获取信息，   遍历， 判断 表单提交过来的信息 是否是 普通文本信息  另做处理
 * 6）
 *    第一种. 用第三方 提供的  item.write( new File(path,filename) );  直接写到磁盘上
 *    第二种. 手动处理  
 *
 */
public class FileUpLoad extends HttpServlet {
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		request.setCharacterEncoding("utf-8");	//设置编码
		
		//获得磁盘文件条目工厂
		DiskFileItemFactory factory = new DiskFileItemFactory();
		//获取文件需要上传到的路径
		String path = request.getRealPath("/upload");
		
		//如果没以下两行设置的话，上传大的 文件 会占用 很多内存，
		//设置暂时存放的 存储室 , 这个存储室，可以和 最终存储文件 的目录不同
		/**
		 * 原理 它是先存到 暂时存储室，然后在真正写到 对应目录的硬盘上， 
		 * 按理来说 当上传一个文件时，其实是上传了两份，第一个是以 .tem 格式的 
		 * 然后再将其真正写到 对应目录的硬盘上
		 */
		factory.setRepository(new File(path));
		//设置 缓存的大小，当上传文件的容量超过该缓存时，直接放到 暂时存储室
		factory.setSizeThreshold(1024*1024) ;
		
		//高水平的API文件上传处理
		ServletFileUpload upload = new ServletFileUpload(factory);
		
		
		try {
			//可以上传多个文件
			List<FileItem> list = (List<FileItem>)upload.parseRequest(request);
			
			for(FileItem item : list)
			{
				//获取表单的属性名字
				String name = item.getFieldName();
				
				//如果获取的 表单信息是普通的 文本 信息
				if(item.isFormField())
				{					
					//获取用户具体输入的字符串 ，名字起得挺好，因为表单提交过来的是 字符串类型的
					String value = item.getString() ;
					
					request.setAttribute(name, value);
				}
				//对传入的非 简单的字符串进行处理 ，比如说二进制的 图片，电影这些
				else
				{
					/**
					 * 以下三步，主要获取 上传文件的名字
					 */
					//获取路径名
					String value = item.getName() ;
					//索引到最后一个反斜杠
					int start = value.lastIndexOf("\\");
					//截取 上传文件的 字符串名字，加1是 去掉反斜杠，
					String filename = value.substring(start+1);
					
					request.setAttribute(name, filename);
					
					//真正写到磁盘上
					//它抛出的异常 用exception 捕捉
					
					//item.write( new File(path,filename) );//第三方提供的
					
					//手动写的
					OutputStream out = new FileOutputStream(new File(path,filename));
					
					InputStream in = item.getInputStream() ;
					
					int length = 0 ;
					byte [] buf = new byte[1024] ;
					
					System.out.println("获取上传文件的总共的容量："+item.getSize());
					// in.read(buf) 每次读到的数据存放在   buf 数组中
					while( (length = in.read(buf) ) != -1)
					{
						//在   buf 数组中 取出数据 写到 （输出流）磁盘上
						out.write(buf, 0, length);
					    
					}
					
					in.close();
					out.close();
				}
			}
			
			
			
		} catch (FileUploadException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			
			//e.printStackTrace();
		}
		
		
		request.getRequestDispatcher("filedemo.jsp").forward(request, response);
		
	}
}
```
System.out.println("获取上传文件的总共的容量："+item.getSize()); 
3.filedemo.jsp
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'filedemo.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
  </head>
  
  <body>
    
    用户名：${requestScope.usename } <br/>
    文件：${requestScope.file1 }<br/>
    ${requestScope.file2 }<br/>
    <!-- 把上传的图片显示出来 -->
    <img alt="go" src="upload/<%=(String)request.getAttribute("file1")%> " />
    
    
    
  </body>
</html>
```
4结果页面：
![](https://img-my.csdn.net/uploads/201204/30/1335786549_1230.jpg)
下载链接：
1）Servlet 文件上传 ———— [点击打开链接](http://blog.csdn.net/hzc543806053/article/details/7524491)
2）Struts2之下载  [点击打开链接](http://blog.csdn.net/hzc543806053/article/details/7538723)
3）Struts2之上传  [点击打开链接](http://blog.csdn.net/hzc543806053/article/details/7526306)
