# 基于Spring3 MVC实现基于HTML form表单文件上传 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年01月06日 19:17:57[gloomyfish](https://me.csdn.net/jia20003)阅读数：14969标签：[file																[File																[html																[Html																[HTML																[mvc																[MVC																[spring																[Spring																[upload																[Upload](https://so.csdn.net/so/search/s.do?q=Upload&t=blog)
个人分类：[J2EE](https://blog.csdn.net/jia20003/article/category/871633)





基于Spring3 MVC实现基于form表单文件上传

一：杂项准备

环境搭建参考这里-http://blog.csdn.net/jia20003/article/details/8471169

二：前台页面

根据RFC1867，只要在提交form表单中声明提交方法为POST，enctype属

性声明为multipart/form-data, action声明到要提交的url即可。具体如下：

![](https://img-my.csdn.net/uploads/201301/06/1357470742_8718.png)


三：spring配置

使用spring3的MultipartHttpReqest来接受来自浏览器的发送的文件内容。

需要配Multipart解析器在express-servlet.xml中。内容如下：

![](https://img-my.csdn.net/uploads/201301/06/1357470770_2552.png)


同时还需要在maven的pom.xml文件添加apachefileupload与common-io两个包。

![](https://img-my.csdn.net/uploads/201301/06/1357470796_6012.png)


四：Controller中方法实现



```java
@RequestMapping(value = "/uploadFile", method = RequestMethod.POST)
public ModelAndView getUploadFile(HttpServletRequest request, HttpServletResponse response) {
	System.out.println("fucking spring3 MVC upload file with Multipart form");
	String myappPath = request.getSession().getServletContext().getRealPath("/");
	try {
		if (request instanceof MultipartHttpServletRequest) {
			MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest) request;
			System.out.println("fucking spring3 MVC upload file with Multipart form");
			// String myappPath = multipartRequest.getServletContext().getRealPath("/"); 
			// does not work, oh my god!!
			MultipartFile file = multipartRequest.getFiles("userfile1").get(0);
			long size = file.getSize();
			byte[] data = new byte[(int) size];
			InputStream input = file.getInputStream();
			input.read(data);
			
			// create file, if no app context path, will throws access denied.
			// seems like you could not create any file at tomcat/bin directory!!!
			File outFile = new File(myappPath + File.separator + file.getOriginalFilename());
			if(!outFile.exists()) {
				outFile.createNewFile();
				System.out.println("full path = " + outFile.getAbsolutePath());
			} else {
				System.out.println("full path = " + outFile.getAbsolutePath());
			}
			FileOutputStream outStream = new FileOutputStream(outFile);
			
			outStream.write(data);
			outStream.close();
			input.close();
		}
	} catch (Exception e) {
		e.printStackTrace();
	}

	return new ModelAndView("welcome");
}
```

常见问题：

1. java.io.IOException: Access is denied避免这个错误是把文件创建在app 

context path的下面所以要获取servlet context的本地路径。

2. Request类型不是MultipartHttpReqest类型，原因是没有配置spring的Multipart解析器

Chrome中运行截屏：

![](https://img-my.csdn.net/uploads/201301/06/1357471347_1534.png)


**转载请注明**](https://so.csdn.net/so/search/s.do?q=upload&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)](https://so.csdn.net/so/search/s.do?q=MVC&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=HTML&t=blog)](https://so.csdn.net/so/search/s.do?q=Html&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=File&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)




