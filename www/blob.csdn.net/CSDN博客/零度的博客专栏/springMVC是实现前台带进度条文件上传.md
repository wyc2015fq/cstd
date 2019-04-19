# springMVC是实现前台带进度条文件上传 - 零度的博客专栏 - CSDN博客
2016年07月11日 16:23:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：13029
        项目框架采用spring+hibernate+springMVC如果上传文件不想使用flash那么你可以采用html5;截图前段模块是bootstarp框架;不废话直接来代码;spring-mvc配置文件;效果截图如下:
![](https://img-blog.csdn.net/20150312231551369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2RzX19tb29u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        详细实现如下:
1、mvc-config.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"         
        xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc"
	xsi:schemaLocation="http://www.springframework.org/schema/beans 
	http://www.springframework.org/schema/beans/spring-beans.xsd
	http://www.springframework.org/schema/context
	http://www.springframework.org/schema/context/spring-context-3.2.xsd
	http://www.springframework.org/schema/mvc
	http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd">
	<!-- 注解扫描包 -->
	<context:component-scan base-package="com.yunfang.banks.controllers" />
	<!-- 开启注解 -->
	<mvc:annotation-driven />
	<!-- 不拦截静态资源 -->
	<mvc:default-servlet-handler />
	<mvc:interceptors>
		<mvc:interceptor>
			<mvc:mapping path="/js/" />
			<mvc:mapping path="/js/**" />
			<bean class="com.yunfang.banks.intreceptor.MyInterceptor"></bean>
		</mvc:interceptor>
	</mvc:interceptors>
	<mvc:interceptors>
		<bean class="com.yunfang.banks.intreceptor.MyInterceptor"></bean>
	</mvc:interceptors>
	<!-- 静态资源(js/image)的访问 -->
	<mvc:resources location="/js/" mapping="/js/**" />
	<!-- 定义视图解析器 -->
	<bean id="viewResolver"
		class="org.springframework.web.servlet.view.InternalResourceViewResolver">
		<property name="prefix" value="/"></property>
		<property name="suffix" value=""></property>
	</bean>
	<bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">
		<property name="messageConverters">
			<list>
				<!--下载的时候专用 -->
				<bean
					class="org.springframework.http.converter.ByteArrayHttpMessageConverter" />
				<bean id="jsonHttpMessageConverter"
					class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">
					<property name="supportedMediaTypes">
						<list>
							<value>text/plain;charset=UTF-8</value>
						</list>
					</property>
				</bean>
			</list>
		</property>
	</bean>
	<!-- 配置springMVC处理上传文件的信息,自定义上传,带进度条 -->
	<bean id="multipartResolver" class="com.yunfang.banks.listener.CustomMultipartResolver">
	        <property name="defaultEncoding" value="utf-8" />
		<property name="maxUploadSize" value="10000000000" />
	</bean>
        <!-- 	
        <bean id="multipartResolver"
		class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
		<property name="defaultEncoding" value="utf-8" />
		<property name="maxUploadSize" value="10485760000" />
		<property name="maxInMemorySize" value="40960" />
	</bean> 
        -->
	
</beans>
```
2、实体工具类：Progress.java
```
package com.yunfang.banks.listener;
public class Progress {
	
	private long pBytesRead;
	private long pContentLength;
	private long pItems;
	public long getpBytesRead() {
		return pBytesRead;
	}
	public void setpBytesRead(long pBytesRead) {
		this.pBytesRead = pBytesRead;
	}
	public long getpContentLength() {
		return pContentLength;
	}
	public void setpContentLength(long pContentLength) {
		this.pContentLength = pContentLength;
	}
	public long getpItems() {
		return pItems;
	}
	public void setpItems(long pItems) {
		this.pItems = pItems;
	}
	@Override
	public String toString() {
		return "Progress [pBytesRead=" + pBytesRead + ", pContentLength="
				+ pContentLength + ", pItems=" + pItems + "]";
	}
	
	
	
}
```
3、文件上传进度监听类：FileUploadProgressListener.java
```
package com.yunfang.banks.listener;
import javax.servlet.http.HttpSession;
import org.apache.commons.fileupload.ProgressListener;
import org.springframework.stereotype.Component;
@Component
public class FileUploadProgressListener implements ProgressListener {
	private HttpSession session;
	public void setSession(HttpSession session){
		this.session=session;
		Progress status = new Progress();//保存上传状态
		session.setAttribute("status", status);
	}
	public void update(long pBytesRead, long pContentLength, int pItems) {
		Progress status = (Progress) session.getAttribute("status");
		try {
			Thread.sleep(5);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		status.setpBytesRead(pBytesRead);
		status.setpContentLength(pContentLength);
		status.setpItems(pItems);
		//System.out.println(">>>>>>>>>>>>>>>>>>>>"+status);
		
	}
}
```
4、自定义扩展org.springframework.web.multipart.commons.CommonsMultipartResolver类，重写public MultipartParsingResult parseRequest(HttpServletRequest request) throws MultipartException方法：CustomMultipartResolver.java
```
package com.yunfang.banks.listener;
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUpload;
import org.apache.commons.fileupload.FileUploadBase;
import org.apache.commons.fileupload.FileUploadException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.multipart.MaxUploadSizeExceededException;
import org.springframework.web.multipart.MultipartException;
import org.springframework.web.multipart.commons.CommonsMultipartResolver;
public class CustomMultipartResolver extends CommonsMultipartResolver {
	@Autowired
	private FileUploadProgressListener progressListener;
	public void setFileUploadProgressListener(
			FileUploadProgressListener progressListener) {
		this.progressListener = progressListener;
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public MultipartParsingResult parseRequest(HttpServletRequest request)
			throws MultipartException {
		String encoding = determineEncoding(request);
		FileUpload fileUpload = prepareFileUpload(encoding);
		progressListener.setSession(request.getSession());
		fileUpload.setProgressListener(progressListener);
		try {
			List<FileItem> fileItems = ((ServletFileUpload) fileUpload).parseRequest(request);
			return parseFileItems(fileItems, encoding);
		}
		catch (FileUploadBase.SizeLimitExceededException ex) {
			throw new MaxUploadSizeExceededException(fileUpload.getSizeMax(), ex);
		}
		catch (FileUploadException ex) {
			throw new MultipartException("Could not parse multipart servlet request", ex);
		}
	}
	
}
```
5、uploadController.java
```
@RequestMapping("UserControllers/progress")
public void uploadFile(HttpServletRequest request,HttpServletResponse response,
                       @RequestParam("file") CommonsMultipartFile file) throws IOException {
     response.setContentType("text/html");
     response.setCharacterEncoding("GBK");
     PrintWriter out;
     boolean flag = false;
     if (file.getSize() > 0) {
          //文件上传的位置可以自定义
         flag = FileUploadUtil.upLoadFile(file, request);
     }
     out = response.getWriter();
     if (flag == true) {
        out.print("1");
     } else {
        out.print("2");
     }
}
```
6、FileUploadUtil.java
```java
import java.io.File;
import javax.servlet.http.HttpServletRequest;
import org.springframework.web.multipart.MultipartFile;
public class FileUploadUtil {
	
	public static Boolean uploadFile(HttpServletRequest request, MultipartFile file) {
		 System.out.println("开始");
		 String path = request.getSession().getServletContext().getRealPath("upload");
		 String fileName = file.getOriginalFilename();
		 System.out.println(path);
		 File targetFile = new File(path, fileName);
		 if (!targetFile.exists()) {
		     targetFile.mkdirs();
		 }
		 // 保存
		 try {
		     file.transferTo(targetFile);
		     return true;
		 } catch (Exception e) {
			 e.printStackTrace();
			 return false;
		 }
	}
}
```
7、前台页面
```
<%@ page language="java" import="java.util.*" pageEncoding="Utf-8"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
%>
<!DOCTYPE html>
<html>
<head>
<base href="<%=basePath%>">
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
<meta http-equiv="description" content="This is my page">
<!--
 <link rel="stylesheet" type="text/css" href="styles.css">
-->
<link href="css/bootstrap.min.css" rel="stylesheet">
<style type="text/css">
.file-box {
	position: relative;
	width: 340px
}
.txt {
	height: 22px;
	border: 1px solid #cdcdcd;
	width: 180px;
	vertical-align: middle;
	margin: 0;
	padding: 0
}
.btn {
	border: 1px solid #CDCDCD;
	height: 24px;
	width: 70px;
	vertical-align: middle;
	margin: 0;
	padding: 0
}
.file {
	position: absolute;
	top: 0;
	right: 80px;
	height: 24px;
	filter: alpha(opacity :   0);
	opacity: 0;
	width: 260px;
	vertical-align: middle;
	margin: 0;
	padding: 0
}
</style>
<%--<script type="text/javascript">
       function myInterval()
       {
    	   $("#progress").html("");
    	   $.ajax({
    		   type: "POST",
    		   url: "<%=basePath%>UserControllers/getSessions",
			data : "1=1",
			dataType : "text",
			success : function(msg) {
				var data = msg;
				console.log(data);
				$("#pdiv").css("width", data + "%");
				$("#progress").html(data + "%");
			}
		});
	}
	function autTime() {
		setInterval("myInterval()", 200);//1000为1秒钟
	}
</script>
--%>
<script type="text/javascript">
	function UpladFile() {
		var fileObj = document.getElementById("file").files[0]; // js 获取文件对象
		var FileController = "UserControllers/progress"; // 接收上传文件的后台地址 
		// FormData 对象---进行无刷新上传
		var form = new FormData();
		form.append("author", "hooyes"); // 可以增加表单数据
		form.append("file", fileObj); // 文件对象
		// XMLHttpRequest 对象
		var xhr = new XMLHttpRequest();
		xhr.open("post", FileController, true);
		xhr.onload = function() {
			alert("上传完成!");
			//$('#myModal').modal('hide');
		};
                //监听progress事件
		xhr.upload.addEventListener("progress", progressFunction, false);
 		xhr.send(form);
	}
	function progressFunction(evt) {
		var progressBar = document.getElementById("progressBar");
		var percentageDiv = document.getElementById("percentage");
		if (evt.lengthComputable) {
			progressBar.max = evt.total;
			progressBar.value = evt.loaded;
			percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100)
					+ "%";
		}
	}
</script>
</head>
<body style="width: 80%;height: 80%;margin: 0px auto;">
	<div class="row bootstrap-admin-no-edges-padding">
		<div class="col-md-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					<div class="text-muted bootstrap-admin-box-title">文件管理</div>
				</div>
				<div class="bootstrap-admin-panel-content">
					<button class="btn btn-primary btn-lg" data-toggle="modal"
						data-target="#myModal">上传</button>
					<input type="text" id="test">
				</div>
			</div>
		</div>
	</div>
	<!-- 模态框（Modal） -->
	<div class="modal fade" id="myModal" tabindex="-1" role="dialog"
		aria-labelledby="myModalLabel" aria-hidden="true">
		<div class="modal-dialog">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"
						aria-hidden="true">×</button>
					<h4 class="modal-title" id="myModalLabel">文件上传进度</h4>
				</div>
				<div class="modal-body">
					<progress id="progressBar" value="0" max="100"
						style="width: 100%;height: 20px; "> </progress>
					<span id="percentage" style="color:blue;"></span> <br>
					<br>
					<div class="file-box">
						<input type='text' name='textfield' id='textfield' class='txt' />
						<input type='button' class='btn' value='浏览...' /> <input
							type="file" name="file" class="file" id="file" size="28"
							onchange="document.getElementById('textfield').value=this.value" />
						<input type="submit" name="submit" class="btn" value="上传"
							onclick="UpladFile()" />
						
					</div>
				</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-default" data-dismiss="modal">关闭
					</button>
				</div>
			</div>
			<!-- /.modal-content -->
		</div>
		<!-- /.modal -->
	</div>
	<script type="text/javascript"
		src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
</body>
</html>
```

