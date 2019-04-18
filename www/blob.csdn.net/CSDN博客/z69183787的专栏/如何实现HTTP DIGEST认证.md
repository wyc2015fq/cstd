# 如何实现HTTP DIGEST认证 - z69183787的专栏 - CSDN博客
2015年07月31日 15:37:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2348
[http://robblog.javaeye.com/blog/556436](http://robblog.javaeye.com/blog/556436)
[http://advosys.ca/papers/web/63-http-digest-authentication.html](http://advosys.ca/papers/web/63-http-digest-authentication.html)
[http://httpd.apache.org/](http://httpd.apache.org/)
[http://articles.techrepublic.com.com/5100-10878_11-5860901.html](http://articles.techrepublic.com.com/5100-10878_11-5860901.html)
[http://www.yolinux.com/TUTORIALS/LinuxTutorialApacheAddingLoginSiteProtection.html](http://www.yolinux.com/TUTORIALS/LinuxTutorialApacheAddingLoginSiteProtection.html)
[http://download.oracle.com/javaee/1.4/tutorial/doc/Security5.html](http://download.oracle.com/javaee/1.4/tutorial/doc/Security5.html)
[http://httpd.apache.org/docs/2.2/howto/auth.html](http://httpd.apache.org/docs/2.2/howto/auth.html)
[http://www.faqs.org/rfcs/rfc2617.html](http://www.faqs.org/rfcs/rfc2617.html)
[http://www.webdavsystem.com/javaserver/doc/authentication](http://www.webdavsystem.com/javaserver/doc/authentication)
[http://www.cafesoft.com/products/cams/tomcat-security.html](http://www.cafesoft.com/products/cams/tomcat-security.html)
上面是一些参考的网络资料：
具体步骤如下：
1、在tomcat服务器下的webapps下新建一个digesttest的文件夹，在该文件夹下新建一个test的文件夹，进入test文件夹，在该文件夹下新建一个index.jsp。（jsp的内容可以随便选择，比如说是一个随意的网页的代码）
2、在digesttest新建一个WEB-INF的文件夹，在该文件夹下新建一个web.xml文件。内容如下所示：
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5"
 xmlns="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee)"
 xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"
 xsi:schemaLocation="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee)
[http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd](http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd)">
   <security-constraint> 
    <web-resource-collection> 
        <web-resource-name> 
            My App  
        </web-resource-name> 
        <url-pattern>/test/*</url-pattern> 
    </web-resource-collection> 
    <auth-constraint> 
        <role-name>tomcat</role-name> 
    </auth-constraint> 
   </security-constraint> 
   <login-config> 
    <auth-method>DIGEST</auth-method>  <!-- DIGEST here --> 
    <realm-name>My Realm</realm-name> 
   </login-config>
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app>
3、在tomcat\conf下搜索tomcat-users.xml文件，打开编辑，添加一个用户，密码和角色。用户名为:test；密码：test；角色可以是默认的tomcat或者自己定义的。
4、重启tomcat服务器，在地址栏如入[http://localhost:8080/digesttest/test/index.jsp](http://localhost:8080/digesttest/test/index.jsp)。就会弹出来一个对话框，在其中输入你在tomcat-users.xml下新设置的用户名和密码（test,test）。就会跳转到该页面。
![如何实现HTTP <wbr>DIGEST认证](http://s16.sinaimg.cn/middle/518e9d61g96df2f9dac3f&690)
5、如果不跳转，可能有以下原因，1）重启tomcat服务器2）新打开一个ie页面，在其下输入上述地址重试。
