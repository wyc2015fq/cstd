# CAS(2)-部署Cas Server - 零度的博客专栏 - CSDN博客
2016年05月25日 11:30:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：1053
       Cas应用都需要有一个Cas Server。Cas Server是基于[Java](http://lib.csdn.net/base/17)
 Servlet实现的，其要求部署在Servlet2.4以上版本的Web容器中。在此笔者将其部署到tomcat7中。Cas Server是要求使用https协议进行访问的，所以如果你的Web容器没有开启https通道，则需先开启。
## 1.1     开启tomcat7的https通道 
       官方文档在这里[http://tomcat.apache.org/tomcat-7.0-doc/ssl-howto.html](http://tomcat.apache.org/tomcat-7.0-doc/ssl-howto.html)。简单来说具体步骤如下：
       1、利用Java的keytool工具创建一个keystore文件，该文件保存了服务端的证书和私钥。打开命令窗口敲入如下命令：
       %JAVA_HOME%\bin\keytool -genkey -alias tomcat -keyalg RSA
       2、根据提示输入密码等信息，笔者选择的是“password”，该密码稍后将用于配置tomcat。
![](http://dl2.iteye.com/upload/attachment/0101/8888/a15987df-ab00-3866-8d98-8978d8008402.png)
       3、打开tomcat安装目录conf/server.xml文件，将如下语句的注释打开，并加上属性keystorePass，密码为我们在建立keystore时设立的密码。这样我们就可以使用https来访问tomcat了。默认其会到${user.home}/.keystore寻找keystore文件，我们之前建立的keystore文件默认就在这个位置。
`<Connector port="8443" protocol="org.apache.coyote.http11.Http11Protocol" SSLEnabled="true"               maxThreads="150" scheme="https" secure="true"               clientAuth="false" sslProtocol="TLS"                        keystorePass="password"/></`
## 1.2     部署Cas Server 
       从官网下载的cas server中会含有一个cas server对应的war包，对应路径为cas-server-3.5.2\modules\cas-server-webapp-3.5.2.war，我们可以把该war包重命名为cas.war，然后丢到tomcat的webapps目录下。之后我们就可以启动tomcat，然后访问https://localhost:8443/cas/login就可以看到Cas的登录页面了。
![](http://dl2.iteye.com/upload/attachment/0101/8890/00d3452c-6e4c-3e5f-b292-6c80838e572e.png)
       这些UI如果不想用，都是可以进行修改，自定义的。敲入用户名和密码就可以进行登录了。默认使用的AuthenticationHandler的处理逻辑是，只要用户名和密码一致就可以登录成功。登录后的效果如下所示：
![](http://dl2.iteye.com/upload/attachment/0101/8892/d57b392b-ccc1-3ce7-94dc-b9403d00c021.png)
       下一节将介绍如何更改认证方式。
原文地址：[http://haohaoxuexi.iteye.com/blog/2128732](http://haohaoxuexi.iteye.com/blog/2128732)
