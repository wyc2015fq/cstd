# Http认证之Digest认证 - 零度的博客专栏 - CSDN博客
2016年05月11日 10:09:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：2224
                
和讲Basic篇的内容差不多，不同的是过程采用的是DIGEST认证: 
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
- <auth-method>DIGEST</auth-method><!-- DIGEST here -->
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
	<auth-method>DIGEST</auth-method>  <!-- DIGEST here -->
	<realm-name>My Realm</realm-name>
</login-config>
3 在tomcat的tomcat-users.xml文件中添加一个用户名密码为test，test的用户，角色test。 
**客户端访问：**
访问http://localhost:port/authen/subdir/index.jsp 
会弹出对话框提示认证，输入test test可以登录。 
![](http://dl.iteye.com/upload/attachment/185241/62c76571-fdd2-3d32-a24d-014ab98ae923.jpg)
**工作流程（通过firebug可以查看请求头）**
1 客户端先发请求（不知道要认证，头里不包含任何特殊信息） 
2 服务器发一个401返回，并含有下面的头 
![](http://dl.iteye.com/upload/attachment/185243/1299fe63-d366-3052-9b0f-d1392e6e9f2b.jpg)
3 客户端认证，含有下面的头 
![](http://dl.iteye.com/upload/attachment/185245/cbe8b59c-997c-3f76-9fcd-3c0cdb88a628.jpg)
response="..."就是客户端用来签名的部分。 
**缺点:**
监听到消息的攻击者可以使用这个消息提交请求。 
**httpclient中的实现**
查看org.apache.commons.httpclient.auth包的DigestScheme类的authenticate方法。 
RFC2617描述了计算方法： 
A valid response contains a checksum (by default, the MD5 checksum) of the username, the password, the given nonce value, the HTTP method, and the requested URI.
