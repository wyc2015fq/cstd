# struts2文件上传与错误信息国际化 - z69183787的专栏 - CSDN博客
2016年09月14日 11:23:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1667
struts.xml文件
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC
    "-//Apache Software Foundation//DTD Struts Configuration 2.1.7//EN"
    "http://struts.apache.org/dtds/struts-2.1.7.dtd">
<struts>
	<!-- struts2文件上传时，第一个关卡，默认为2M，当上传的文件大于2M时，
	文件不上传并且后台会报错，将错误定义为actionerror级别，内容如下：
	the request was rejected because its size (2722341) exceeds 
	the configured maximum (2097152)
	这个错误不能被国家化，但是可以通过相应的action类继承ActionSupport类，
	并且重写addActionError()方法修改错误信息。 -->
	<constant name="struts.multipart.maxSize" value="9000000" />
	<constant name="struts.custom.i18n.resources"
		value="com.ysu.web.action.messages_zh_CN" />
	<package name="struts2_file_upload" extends="struts-default">
		<default-action-ref name="upload"></default-action-ref>
		<action name="upload" class="com.ysu.web.action.MainAction">
			<result name="success">
				/WEB-INF/jsp/upload.jsp
			</result>
		</action>
		<action name="fileupload"
			class="com.ysu.web.action.FileUploadAction">
			<interceptor-ref name="defaultStack">
				<!--struts2上传文件的第二道关卡，当上传的文件小于第一道关卡中设置的大小，
				这时文件已经上传，拦截器才起作用，在分析文件的各种属性，例如文件大小、类别、扩展名。
				如果不满足拦截器的要求，会将错误定义为fielderror级别,并将上传的文件删除。
				通过<s:fielderror></s:fielderror>在页面显示错误信息，这个错误信息可以被国际化。
				注意：1M = 1*1024*1024 -->
				<param name="fileUpload.maximumSize">1048576</param>
				
				<!-- 允许类型为excel的文件上传 
				注意：限制上传文件为.xls文件,当本地电脑装有WPS时,需要添加application/kset.
				如果没有装WPS只需要application/vnd.ms-excel-->
				<param name="fileUpload.allowedTypes">
					application/kset,application/vnd.ms-excel
				</param>
				
				<!-- 允许后缀名为xls的文件上传 
				注意：allowedTypes的优先级别高于allowedExtensions,
				如果配置了allowedTypes,则allowedExtensions将不起作用
				-->
				<param name="allowedExtensions">
					xls
				</param>
			</interceptor-ref>
			<result name="success">
				/WEB-INF/jsp/show.jsp
			</result>
			<result name="input">
				/WEB-INF/jsp/upload.jsp
			</result>
		</action>
	</package>
</struts>
```
FileUploadAction.java
```
<span style="box-sizing: border-box; margin: 0px; padding: 0px; font-size: 18px;">package com.ysu.web.action;
import java.io.File;
import java.io.IOException;
import javax.servlet.ServletContext;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;
import com.opensymphony.xwork2.ActionSupport;
public class FileUploadAction extends ActionSupport{
	private static final long serialVersionUID = 1L;
	private File upload;//文件名，对应表单中的name属性值。
	
	private String uploadContentType;//文件名+ContentType
	
	private String uploadFileName;//文件名+FileName
	
	public File getUpload() {
		return upload;
	}
	public void setUpload(File upload) {
		this.upload = upload;
	}
	public String getUploadContentType() {
		return uploadContentType;
	}
	public void setUploadContentType(String uploadContentType) {
		this.uploadContentType = uploadContentType;
	}
	public String getUploadFileName() {
		return uploadFileName;
	}
	public void setUploadFileName(String uploadFileName) {
		this.uploadFileName = uploadFileName;
	}
	public String uploadFile(){
		try {
			ServletContext context = ServletActionContext.getServletContext();
			String path = context.getRealPath("/file");
			System.out.println("文件路径"+path);
			File destFile = new File(path,uploadFileName);
			if(!destFile.exists()){
				destFile.createNewFile();
			}
			FileUtils.copyFile(upload, destFile);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return "success";
	}
	
	@Override
	public void addActionError(String anErrorMessage) {
		// 这里要先判断一下，是我们要替换的错误，才处理
		if (anErrorMessage.startsWith("the request was rejected because its size")) {
			//getText()方法是ActionSupport类中的方法，可以得到国际化文件中的信息
			super.addActionError(getText("struts.messages.error.file.too.large"));
			/*或者将该错误定义为fielderror级别：
			 * super.addFieldError("fileTooLargeInfo", "文件的大小超出系统处理的范围");*/
		} else {// 否则按原来的方法处理
			super.addActionError(anErrorMessage);
		}
	}
	
}
</span>
```
messages_zh_CN.properties
```
struts.messages.error.file.too.large=\u60A8\u4E0A\u4F20\u7684\u6587\u4EF6\u592A\u5927
struts.messages.error.uploading=\u4E0A\u4F20\u9519\u8BEF\uFF0C\u8BF7\u91CD\u65B0\u4E0A\u4F20
struts.messages.error.content.type.not.allowed=\u8BF7\u4E0A\u4F20.xls\u6587\u4EF6
```
upload.jsp
```
<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@ taglib uri="/struts-tags" prefix="s" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<title>Insert title here</title>
	</head>
	<body>
		<!--enctype="multipart/form-data"必不可少 -->
		<s:form action="fileupload!uploadFile" namespace="/"
			enctype="multipart/form-data" method="post" theme="simple">
			<s:file name="upload"></s:file>
			<s:submit value="上传"></s:submit>
			
		</s:form>
		
		<s:fielderror></s:fielderror>
	</body>
</html>
```
可能出现的疑惑：
1、struts.xml文件
```
<interceptor-ref name="fileUpload">
			<param name="allowedTypes">
				application/kset,application/vnd.ms-excel
			</param>
			<param name="maximumSize">1048576</param>
			</interceptor-ref>
<interceptor-ref name="defaultStack"></interceptor-ref>
```
如果采用以上方式配置，当上传的文件大小大于1M，且小于struts.multipart.maxSize时，由于该文件已经上传了，系统删除文件的时候删除了两次。为什么要删除两次要等以后研究源码后讨论。
删除两次的信息如下：
2014-04-23 04:43:54,026 [org.apache.struts2.interceptor.FileUploadInterceptor]-[WARN] 您上传的文件太大
2014-04-23 04:43:54,161 [org.apache.struts2.interceptor.FileUploadInterceptor]-[INFO] Removing file upload \tmp\upload__295faecc_1458b2e3a63__8000_00000000.tmp
2014-04-23 04:43:54,163 [org.apache.struts2.interceptor.FileUploadInterceptor]-[INFO] Removing file upload \tmp\upload__295faecc_1458b2e3a63__8000_00000000.tmp
而改回原来的方式，则只会出现一次：
2014-04-23 04:47:42,183 [org.apache.struts2.interceptor.FileUploadInterceptor]-[WARN] 您上传的文件太大
2014-04-23 04:47:42,301 [org.apache.struts2.interceptor.FileUploadInterceptor]-[INFO] Removing file upload \tmp\upload_30586163_1458b31b5ea__8000_00000000.tmp
2、如果项目采用的是SSH框架，第一次上传的错误会一直存在，导致页面输出错误会有以前的错误。结局方法是在spring的配置文件添加scope="prototype“。
```
<bean id="studentAction" class="com.ysu.web.action.StudentAction" scope="prototype">
  <property name="iuserServ" ref="userServImpl" />
</bean>
```
3、当项目只用了struts2框架时，输出错误信息时会一直有两个一模一样的信息。其实一个是struts2标签自带的输出错误信息，一个是我们自己写的<s:fielderror></s:fielderror>显示的信息。
![](https://img-blog.csdn.net/20140423153906593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlydWkwODIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方法：给表单添加theme="simple"
4、默认的错误信息为英文, 在struts2-core-2.x.x.x.jar\org\apache\struts2\struts-messages.properties文件中定义:
struts.messages.error.uploading=Error uploading: {0}
struts.messages.error.file.too.large=File too large: {0} "{1}" "{2}" {3}
struts.messages.error.content.type.not.allowed=Content-Type not allowed: {0} "{1}" "{2}" {3}
struts.messages.error.file.extension.not.allowed=File extension not allowed: {0} "{1}" "{2}" {3}
{0}:<input type=“file” name=“uploadImage”>中name属性的值
{1}:上传文件的名称
{2}:上传文件保存到临时目录的名称
{3}:上传文件的类型(对struts.messages.error.file.too.large是上传文件的大小)
我们可以在Action的统计目录下创建一个fileuploadmessage.properties文件, 文件名没有要求, 但必须是properties文件, 在其中输入:
struts.messages.error.uploading=上传错误: {0}
struts.messages.error.file.too.large=文件太大: {0} "{1}" "{2}" {3}
struts.messages.error.content.type.not.allowed=不支持的文件类型: {0} "{1}" "{2}" {3}
struts.messages.error.file.extension.not.allowed=不支持的文件扩展名: {0} "{1}" "{2}" {3}
