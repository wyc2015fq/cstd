# springMVC两种方式实现多文件上传及效率比较 - z69183787的专栏 - CSDN博客
2016年08月15日 12:54:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2379
springMVC实现多文件上传的方式有两种，一种是我们经常使用的以字节流的方式进行文件上传，另外一种是使用springMVC包装好的解析器进行上传。这两种方式对于实现多文件上传效率上却有着很大的差距，下面我们通过实例来看一下这两种方式的实现方式，同时比较一下在效率上到底存在着多大的差距。
1.下载相关jar包。需要引入的jar出了springMVC的jar包外，还需要引入com.springsource.org.apache.commons.fileupload-1.2.0.jar和com.springsource.org.apache.commons.io-1.4.0.jar。所有的jar包可以通过“[点击这里](http://download.csdn.net/detail/a1314517love/7222291)”进行下载。
2.配置springAnnotation-servlet.xml文件（文件名称可以自定义，只要和web.xml中引入的名称一样即可）：
```
<?xml version="1.0" encoding="UTF-8"?>
<!-- Bean头部 -->
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:mvc="http://www.springframework.org/schema/mvc"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:util="http://www.springframework.org/schema/util"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
            http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd
            http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
            http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-3.0.xsd">
   
   <!-- 注解扫描包 -->
   <context:component-scan base-package="com.tgb.web.controller.annotation"></context:component-scan>
   <!-- 代替下面的两行代码 -->
   <mvc:annotation-driven/>
  
   <!-- 静态资源访问 -->
   <mvc:resources location="/img/" mapping="/img/**"/>
    <mvc:resources location="/js/" mapping="/js/**"/>
   
   
   <bean id="viewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
   		<property name="prefix" value="/"></property>
   		<property name="suffix" value=".jsp"></property>
   	
   </bean>
   
   <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
   		<property name="defaultEncoding" value="utf-8"></property> 
   		<property name="maxUploadSize" value="10485760000"></property>
   		<property name="maxInMemorySize" value="40960"></property>
   </bean>
</beans>
```
3. 配置web.xml文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" id="WebApp_ID" version="2.5">
  <display-name>springMVC1</display-name>
  <welcome-file-list>
    <welcome-file>index.html</welcome-file>
  </welcome-file-list>
  
  <servlet>
  	 <servlet-name>springMVC</servlet-name>
  	 <!-- springMVC的分发器 -->
  	 <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
  	 <init-param>
  	 	<param-name>contextConfigLocation</param-name>
  	 	<param-value>classpath*:config/springAnnotation-servlet.xml</param-value>
  	 </init-param>
  	 <!-- 表示当Tomcat已启动的时候初始化这个Servlet -->
  	 <load-on-startup>1</load-on-startup>
  </servlet>
  
  <filter>  
      <filter-name>encodingFilter</filter-name>  
      <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>  
      <init-param>  
          <param-name>encoding</param-name>  
          <param-value>UTF-8</param-value> <!--设置你想用的字符集，我这里用的是GB18030-->  
      </init-param>  
      <init-param>
      	<param-name>forceEncoding</param-name>
      	<param-value>true</param-value>
      </init-param>
  </filter>  
    
  <filter-mapping>  
      <filter-name>encodingFilter</filter-name>  
      <url-pattern>/*</url-pattern> <!--设置你想过滤的页面或者是Servlet，根据自己的需要配置-->  
 </filter-mapping>  
 
  <servlet-mapping>
  	<servlet-name>springMVC</servlet-name>
  	<url-pattern>/</url-pattern>
  </servlet-mapping>
</web-app>
```
4. jsp页面代码：
```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<script type="text/javascript" src="../js/jquery-1.7.2.js"></script>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<script type="text/javascript">
	i = 1;
	j = 1;
	$(document).ready(function(){
		
		$("#btn_add1").click(function(){
			document.getElementById("newUpload1").innerHTML+='<div id="div_'+i+'"><input  name="file" type="file"  /><input type="button" value="删除"  onclick="del_1('+i+')"/></div>';
			  i = i + 1;
		});
		
		$("#btn_add2").click(function(){
			document.getElementById("newUpload2").innerHTML+='<div id="div_'+j+'"><input  name="file_'+j+'" type="file"  /><input type="button" value="删除"  onclick="del_2('+j+')"/></div>';
			  j = j + 1;
		});
	});
	function del_1(o){
	 document.getElementById("newUpload1").removeChild(document.getElementById("div_"+o));
	}
	
	function del_2(o){
		 document.getElementById("newUpload2").removeChild(document.getElementById("div_"+o));
	}
</script>
</head>
<body>
	 <h1>springMVC字节流输入上传文件</h1> 
    <form name="userForm1" action="/springMVC7/file/upload" enctype="multipart/form-data" method="post">
		<div id="newUpload1">
			<input type="file" name="file">
		</div>
		
		<input type="button" id="btn_add1" value="增加一行" >
		<input type="submit" value="上传" >
 	</form> 
 	<br>
 	<br>
 	<hr align="left" width="60%" color="#FF0000" size="3">
 	<br>
 	<br>
 	 <h1>springMVC包装类上传文件</h1> 
 	<form name="userForm2" action="/springMVC7/file/upload2" enctype="multipart/form-data" method="post"">
 		<div id="newUpload2">
			<input type="file" name="file">
		</div>
		<input type="button" id="btn_add2" value="增加一行" >
		<input type="submit" value="上传" >
		
		
 	</form> 
</body>
</html>
```
5.实现上传功能的java
 bean：
```java
package com.tgb.web.controller.annotation.upload;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.Date;
import java.util.Iterator;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.swing.filechooser.FileNameExtensionFilter;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;
import org.springframework.web.multipart.commons.CommonsMultipartFile;
import org.springframework.web.multipart.commons.CommonsMultipartResolver;
import org.springframework.web.servlet.ModelAndView;
import com.tgb.web.controller.entity.User;
@Controller
@RequestMapping("/file")
public class UploadController { 
	
	
	@RequestMapping("/upload"	)
	public String addUser(@RequestParam("file") CommonsMultipartFile[] files,HttpServletRequest request){
		
		for(int i = 0;i<files.length;i++){
			System.out.println("fileName---------->" + files[i].getOriginalFilename());
		
			if(!files[i].isEmpty()){
				int pre = (int) System.currentTimeMillis();
				try {
					//拿到输出流，同时重命名上传的文件
					FileOutputStream os = new FileOutputStream("H:/" + new Date().getTime() + files[i].getOriginalFilename());
					//拿到上传文件的输入流
					FileInputStream in = (FileInputStream) files[i].getInputStream();
					
					//以写字节的方式写文件
					int b = 0;
					while((b=in.read()) != -1){
						os.write(b);
					}
					os.flush();
					os.close();
					in.close();
					int finaltime = (int) System.currentTimeMillis();
					System.out.println(finaltime - pre);
					
				} catch (Exception e) {
					e.printStackTrace();
					System.out.println("上传出错");
				}
		}
		}
		return "/success";
	}
	
	
	@RequestMapping("/upload2"	)
	public String upload2(HttpServletRequest request,HttpServletResponse response) throws IllegalStateException, IOException {
		//创建一个通用的多部分解析器
		CommonsMultipartResolver multipartResolver = new CommonsMultipartResolver(request.getSession().getServletContext());
		//判断 request 是否有文件上传,即多部分请求
		if(multipartResolver.isMultipart(request)){
			//转换成多部分request  
			MultipartHttpServletRequest multiRequest = (MultipartHttpServletRequest)request;
			//取得request中的所有文件名
			Iterator<String> iter = multiRequest.getFileNames();
			while(iter.hasNext()){
				//记录上传过程起始时的时间，用来计算上传时间
				int pre = (int) System.currentTimeMillis();
				//取得上传文件
				MultipartFile file = multiRequest.getFile(iter.next());
				if(file != null){
					//取得当前上传文件的文件名称
					String myFileName = file.getOriginalFilename();
					//如果名称不为“”,说明该文件存在，否则说明该文件不存在
					if(myFileName.trim() !=""){
						System.out.println(myFileName);
						//重命名上传后的文件名
						String fileName = "demoUpload" + file.getOriginalFilename();
						//定义上传路径
						String path = "H:/" + fileName;
						File localFile = new File(path);
						file.transferTo(localFile);
					}
				}
				//记录上传该文件后的时间
				int finaltime = (int) System.currentTimeMillis();
				System.out.println(finaltime - pre);
			}
			
		}
		return "/success";
	}
	
	@RequestMapping("/toUpload"	) 
	public String toUpload() {
		
		return "/upload";
	}
	
}
```
6.最后看后台打印数据，数据来源于后台打印的上传文件所用的时间，第一幅图片是使用字节流写入方式完成三个文件上传中每个文件用时，第二幅图片是使用springMVC包装好的解析器进行的三个相同的文件上传中每个文件的用时：
字节流实现文件上传的传递效率，结果显示传递三个文件用时分别为534ms，453ms和387ms。
![](https://img-blog.csdn.net/20140420152356468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTEzMTQ1MTdsb3Zl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用springMVC解析器进行文件上传用时分别为2ms，1ms和2ms。
![](https://img-blog.csdn.net/20140420152657265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTEzMTQ1MTdsb3Zl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过对比这两种方式我们可以发现使用springMVC进行多文件的效率显然要比字符流写入方式效率上要高得多。
