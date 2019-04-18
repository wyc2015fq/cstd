# Struts图片上传下载 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月07日 23:01:48[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：69


```java
struts.xml中核心配置代码

		<action name="uploadAction" class="com.zzh.web.UploadAction">
	
			<interceptor-ref name="fileUpload">
				<param name="maximumSize">1000000000000000</param>
				<param name="allowedExtensions">.jpg,.jpeg,.png,.gif</param> 
				<!-- <param name="allowedTypesSet">image/jpg,image/jpeg,image/png,image/gif</param> -->
			</interceptor-ref>		
			<!-- 配置了action内部的拦截器，全局的就无效了，所以要把默认的copy过来 -->
			<interceptor-ref name="defaultStack" />		
			<!-- 上传页面 -->
			<result name="input">/upload.jsp</result>
			<!-- 上传成功页面 -->
			<result name="success">/uploadSucc.jsp</result>
		</action>
```

```java
action中

public class UploadAction extends ActionSupport{

	//文件上传要对应3个变量
	private File upload;
	//文件名称
	private String uploadFileName;
	//文件类型
	private String uploadContentType;
	//页面传回来的文件名
	private String filename;
	
	public String getFilename() {
		return filename;
	}
	public void setFilename(String filename) {
		this.filename = filename;
	}
	public File getUpload() {
		return upload;
	}
	public void setUpload(File upload) {
		this.upload = upload;
	}
	public String getUploadFileName() {
		return uploadFileName;
	}
	public void setUploadFileName(String uploadFileName) {
		this.uploadFileName = uploadFileName;
	}
	public String getUploadContentType() {
		return uploadContentType;
	}
	public void setUploadContentType(String uploadContentType) {
		this.uploadContentType = uploadContentType;
	}
	//下载图片
	public String download() throws Exception{
		
		HttpServletResponse response = ServletActionContext.getResponse();
		
		//乱码处理,文件名称包含中文时必须进行乱码处理
		filename = new String(filename.getBytes("iso-8859-1"),"utf-8");
		HttpServletRequest request = ServletActionContext.getRequest();
		//获取项目物理路径且在后面加上/upload 等价于 request.getContentPath().getRealPath();
		String path = request.getRealPath("");
		path += "/" + filename;
		File file = new File(path);

		/*
		 * 设置response的Header
		 */
		String fs = filename.substring(filename.lastIndexOf("/")+1);
		response.addHeader("Content-Disposition", "attachment;filename=" + fs);//文件名称
		response.addHeader("Content-Length", "" + file.length()); //文件大小
		response.setContentType("application/octet-stream"); //下载的是二进制流文件	
		
		//定义输出到浏览器的输出流
		OutputStream toClient = new BufferedOutputStream(response.getOutputStream());
		
		//将本地文件读取到输入流中
		InputStream fis = new BufferedInputStream(new FileInputStream(file));
		byte[] buffer = new byte[1024];
		int i =-1;
		while((i = fis.read(buffer))!=-1) {
			toClient.write(buffer, 0, i);
		}
		toClient.flush();
		toClient.close();
		fis.close();
		
		return null;		
	}
	
	
	
	//保存图片
	public String save() throws Exception{
		
		System.out.println("uploadFileName :"+uploadFileName);
		System.out.println("uploadContentType :"+uploadContentType);
		String extname = uploadFileName.substring(uploadFileName.lastIndexOf("."));
		String sfilename = "upload/"+fileName()+extname;
		System.out.println("filename="+sfilename);
		
//		HttpServletResponse response = ServletActionContext.getResponse();
		HttpServletRequest request = ServletActionContext.getRequest();
/*		HttpSession session = request.getSession();
		ServletContext application = ServletActionContext.getServletContext();
		*/
		String path = request.getRealPath("/");
		System.out.println(path+sfilename);
		//保存文件
		FileOutputStream fos = new FileOutputStream(new File(path+sfilename));
		FileInputStream fis = new FileInputStream(upload);
		byte b[] = new byte[4096]; //4k
		int len = 0;
		while((len = fis.read(b))!=-1) {
			fos.write(b, 0, len);
		}
		fos.flush();
		fos.close();
		uploadFileName = sfilename;
		return SUCCESS;
	}
	//起文件名
	public String fileName() {
		 String sfile =new Date().toLocaleString();
		 sfile = sfile.replaceAll("-", "");
		 sfile = sfile.replaceAll(" ", "");
		 sfile = sfile.replaceAll(":", "");
		 
		 return sfile;
	}
}
```

```
上传页面

<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/struts-tags" prefix="s"%>    
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1 align="center">文件上传</h1>
	<hr>
	
	<form  name="form1" action="uploadAction!save.action" method="post" enctype="multipart/form-data">
	请选择要上传的文件：<s:file name="upload"></s:file><br>
	<input type="submit" value="上 传">
	
	</form>
</body>
</html>
```

```
上传成功页面

<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/struts-tags" prefix="s" %>
<%@ include file="head.jsp"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>恭喜您，文件上传成功！！！</h1>
	<hr>
	<a href="${aBasePath }/uploadAction!download.action?filename=${uploadFileName }">下载</a><br>
	<img src="${aBasePath }/${uploadFileName }">
</body>
</html>
```

```
封装的项目路径

<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<%
	String url = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextPath();
	if(request.getServerPort()==80){
		url=request.getScheme()+"://"+request.getServerName()+request.getContextPath();
	}
	
	pageContext.setAttribute("BasePath", "http://192.168.46.225/static/resource");
	pageContext.setAttribute("aBasePath", url);
%>
</body>
</html>
```

注意 ： 如果图片上传到windows下的tomcat上面， 每一次重启服务器 图片都会消失，所以一般情况我们会将静态资源放在linux下的tomcat服务器上。

之前写过一篇关于 SSM实现图片上传的博客，有兴趣的可以看看，涉及了一些工具包，比如获取图片后缀名，以及将图片放入linux服务器下的工具包。

[SSM实现图片上传](https://blog.csdn.net/weixin_40247263/article/details/79718177)

[SSM实现图片上传(优化)，放入linux中的tomcat的服务器](https://blog.csdn.net/weixin_40247263/article/details/79750772)


