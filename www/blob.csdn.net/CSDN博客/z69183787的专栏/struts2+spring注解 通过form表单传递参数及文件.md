# struts2+spring注解 通过form表单传递参数及文件 - z69183787的专栏 - CSDN博客
2013年05月10日 13:25:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4223
表单页面：
```
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<html>
<head>
<script src="js/jquery-1.7.1.js"></script>
<script type="text/javascript">   
$(function(){
	
	$("#add").click(function(){
		$("#p").append("file:<input type='file' name='fileupload'><br>	");
	})
	
})
</script>
</head>
<body>
<form action="test/test.action" method="post" enctype="multipart/form-data">
    username: <input type="text" name="username"> <br>
    age: <input type="age" name="age"> <br>
    <div id="p">
	   	file:<input type="file" name="fileupload"><br>	
	   	
	   	
   	</div>
   	<input type="button" name="add" id="add"/>
    <input type="button" name="btn" value="submit" onclick="javascript:document.forms[0].submit();">
    </form>
</body>
</html>
```
后台action：
```java
/**
 * 
 */
package com.wonders.zs.action;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import org.apache.struts2.ServletActionContext;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;
/** 
 * @ClassName: AbstractParamAction 
 * @Description: TODO() 
 * @author zhoushun 
 * @date 2013-5-10 12:12:32 
 *  
 */
public class AbstractParamAction extends ActionSupport{
	private static final long serialVersionUID = 8570298190546820694L;
	
	public ActionContext actionContext = ActionContext.getContext();
	public HttpServletRequest request = (HttpServletRequest) actionContext.get(ServletActionContext.HTTP_REQUEST);
	public HttpServletResponse response = (HttpServletResponse) actionContext.get(ServletActionContext.HTTP_RESPONSE);
	
	//public ServletContext  application = (ServletContext)actionContext.getApplication();
	public HttpSession session = request.getSession();
}
```
```java
/**
 * 
 */
package com.wonders.zs.action;
import java.io.File;
import java.io.IOException;
import java.util.List;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.Namespace;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.springframework.stereotype.Controller;
import org.springframework.context.annotation.Scope;
/** 
 * @ClassName: FormSubmitAction 
 * @Description: TODO() 
 * @author zhoushun 
 * @date 2013-5-10 11:43:51 
 *  
 */
@ParentPackage("struts-default")
@Namespace(value="/test")
@Controller("testAction")
@Scope("prototype")
public class TestAction extends AbstractParamAction{
	private static final long serialVersionUID = 6165059252734258592L;
	
	private List<File> fileupload;//这里的"fileName"一定要与表单中的文件域名相同  
    private List<String> fileuploadContentType;//格式同上"fileName"+ContentType  
    private List<String> fileuploadFileName;//格式同上"fileName"+FileName  
	private String username;
	private String age;
	
	@SuppressWarnings("deprecation")
	@Action(value="test")
	public String test(){
		System.out.println("username= "+username + "age= "+ age);
		String path = request.getSession().getServletContext().getRealPath("attach");
		System.out.println("path= "+path);
		for(int i=0;i<fileupload.size();i++){
			System.out.println("fileName= "+fileuploadFileName.get(i));
			System.out.println("fileExt= "+fileuploadContentType.get(i));
			System.out.println("fileLen=" + fileupload.get(i).length());
		
			try {
				FileUtils.copyFile(fileupload.get(i), new File(path+"/"+fileuploadFileName.get(i)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		try {
			this.response.getWriter().write("aaa");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	/**
	 * @return the fileupload
	 */
	public List<File> getFileupload() {
		return fileupload;
	}
	/**
	 * @param fileupload the fileupload to set
	 */
	public void setFileupload(List<File> fileupload) {
		this.fileupload = fileupload;
	}
	/**
	 * @return the fileuploadContentType
	 */
	public List<String> getFileuploadContentType() {
		return fileuploadContentType;
	}
	/**
	 * @param fileuploadContentType the fileuploadContentType to set
	 */
	public void setFileuploadContentType(List<String> fileuploadContentType) {
		this.fileuploadContentType = fileuploadContentType;
	}
	/**
	 * @return the fileuploadFileName
	 */
	public List<String> getFileuploadFileName() {
		return fileuploadFileName;
	}
	/**
	 * @param fileuploadFileName the fileuploadFileName to set
	 */
	public void setFileuploadFileName(List<String> fileuploadFileName) {
		this.fileuploadFileName = fileuploadFileName;
	}
	/**
	 * @return the username
	 */
	public String getUsername() {
		return username;
	}
	/**
	 * @param username the username to set
	 */
	public void setUsername(String username) {
		this.username = username;
	}
	/**
	 * @return the age
	 */
	public String getAge() {
		return age;
	}
	/**
	 * @param age the age to set
	 */
	public void setAge(String age) {
		this.age = age;
	}
    
	
}
```
可以实现 参数 及 多个文件同时上传。
