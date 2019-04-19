# Http认证之Basic认证 - 零度的博客专栏 - CSDN博客
2016年05月11日 10:04:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：3987
                
文章主要讲如何在tomcat中配置Basic认证以及工作流程： 
**Tomcat配置:**
1 在tomcat的webapps下新建一个目录authen，再建立子目录subdir,下面放一个index.jsp 
2 在authen目录下建立WEB-INF目录，下放web.xml文件，内容如下 
Xml代码 ![复制代码](http://robblog.iteye.com/images/icon_copy.gif)![收藏代码](http://robblog.iteye.com/images/icon_star.png)![](http://robblog.iteye.com/images/spinner.gif)
- <security-constraint>
- <web-resource-collection>
- <web-resource-name>
-             My App  
- </web-resource-name>
- <url-pattern>/subdir/*</url-pattern>
- </web-resource-collection>
- <auth-constraint>
- <role-name>test</role-name>
- </auth-constraint>
- </security-constraint>
- 
- <login-config>
- <auth-method>BASIC</auth-method>
- <realm-name>My Realm</realm-name>
- </login-config>
<security-constraint>
	<web-resource-collection>
		<web-resource-name>
			My App
		</web-resource-name>
		<url-pattern>/subdir/*</url-pattern>
	</web-resource-collection>
	<auth-constraint>
		<role-name>test</role-name>
	</auth-constraint>
</security-constraint>
<login-config>
	<auth-method>BASIC</auth-method>
	<realm-name>My Realm</realm-name>
</login-config>
3 在tomcat的tomcat-users.xml文件中添加一个用户名密码为test，test的用户，角色test。 
**客户端访问：**
访问http://localhost:port/authen/subdir/index.jsp 
会弹出对话框提示认证，输入test test可以登录。 
![](http://dl.iteye.com/upload/attachment/184068/d9bb95a9-5e35-304b-a8c1-b910a8b33a48.jpg)
**工作流程（通过firebug可以查看请求头）**
1 客户端先发请求（不知道要认证，头里不包含任何特殊信息） 
2 服务器发一个401返回，并含有下面的头 
WWW-Authenticate Basic realm="My Realm" 
![](http://dl.iteye.com/upload/attachment/184072/aee4ad21-1db5-3723-8350-d8315c407ad9.jpg)
3 客户端认证，含有下面的头 
Authorization Basic dGVzdDp0ZXN0 
“dGVzdDp0ZXN0”是"test:test"的Base64编码。 (可以通过php函数base64_encode()验证) 
![](http://dl.iteye.com/upload/attachment/184074/7396cfeb-826f-3d0d-88c9-b7bfe4fd8c4f.jpg)
**缺点:**
密码明文传输，非常不安全。 
**httpclient中的实现**
Java代码 ![复制代码](http://robblog.iteye.com/images/icon_copy.gif)![收藏代码](http://robblog.iteye.com/images/icon_star.png)![](http://robblog.iteye.com/images/spinner.gif)
- 查看org.apache.commons.httpclient.auth包的BasicScheme类  
- 
- // Copy from the httpclient source code
- // Omit some codes
- publicstatic String authenticate(UsernamePasswordCredentials credentials, String charset) {  
- 
-         ...  
- 
-         StringBuffer buffer = new StringBuffer();  
-         buffer.append(credentials.getUserName());  
-         buffer.append(":");  
-         buffer.append(credentials.getPassword());  
- 
- return"Basic " + EncodingUtil.getAsciiString(Base64.encodeBase64(EncodingUtil.getBytes(buffer.toString(), charset)));  
-     }  
