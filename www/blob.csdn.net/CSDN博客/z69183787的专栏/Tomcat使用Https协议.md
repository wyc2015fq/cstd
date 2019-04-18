# Tomcat使用Https协议 - z69183787的专栏 - CSDN博客
2017年09月13日 16:20:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：518
[http://blog.csdn.net/a35038438/article/details/50598429](http://blog.csdn.net/a35038438/article/details/50598429)
在配置Https协议之前你需要知道：
1.ssl
2.keystore
3.<security-constraint>认证类型
目录：
1.生成安全证书
2.配置Tomcat
3.测试页面
4.https与http协议转换
5.页面强制使用https协议
1.生成安全证书(cmd)：
**[plain]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 创建证书命令：keytool -genkeypair -alias "tomcat" -keyalg "RSA" -validity 90 -keystore "D:\tomcat.keystore"  
"D:\tomcat.keystore"生成的keystore位置和名称
-keyalg "RSA"加密算法
-validity 90有效天数
![](https://img-blog.csdn.net/20160128105837482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
PS:值得注意的是"输入keystore密码"在你输入时，并不会显示你的密码
2.配置Tomcat(conf文件夹中的server.xml)：
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 添加此段代码：<Connectorport="8443"protocol="org.apache.coyote.http11.Http11Protocol"
- maxThreads="150"SSLEnabled="true"scheme="https"secure="true"
- clientAuth="false"sslProtocol="TLS"
- keystoreFile="D:/tomcat.keystore"
- keystorePass="tomcat"/>
3.此时在浏览器地址栏输入https://localhost:8443进行访问进入到如下页面则配置成功：
![](https://img-blog.csdn.net/20160128111214784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.https与http协议转换(有些页面需要使用https访问，有些页面使用http访问)
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 找到此段代码：<Connectorport="8080"protocol="HTTP/1.1"
- connectionTimeout="20000"
- redirectPort="8009"/>
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 修改为：      <Connectorport="80"protocol="HTTP/1.1"
- connectionTimeout="20000"
- redirectPort="443"/>
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 找到此段代码：<Connectorport="8443"protocol="org.apache.coyote.http11.Http11Protocol"
- maxThreads="150"SSLEnabled="true"scheme="https"secure="true"
- clientAuth="false"sslProtocol="TLS"
- keystoreFile="D:/tomcat.keystore"
- keystorePass="tomcat"/>
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 修改为：     <Connectorport="443"protocol="org.apache.coyote.http11.Http11Protocol"
- maxThreads="150"SSLEnabled="true"scheme="https"secure="true"
- clientAuth="false"sslProtocol="TLS"
- keystoreFile="D:/tomcat.keystore"
- keystorePass="tomcat"/>
PS:80和443是http和https协议的默认端口号，这样设置，是为了在浏览器地址栏不需要输入端口号就能访问页面，需要访问https协议页面，就直接在地址栏输入https://localhost。
5.页面强制使用https协议(例如：登录页面必须使用https协议)
**[html]**[view
 plain](http://blog.csdn.net/a35038438/article/details/50598429#)[copy](http://blog.csdn.net/a35038438/article/details/50598429#)
- 在项目资源中的web.xml或tomcat/conf/web.xml文件中的</welcome-file-list>后加上：  
- <web-resource-collection>
- <web-resource-name>SSL</web-resource-name>
- <url-pattern>/*</url-pattern><!-- 这里表示所有页面都必须使用https协议 -->
- </web-resource-collection>
- <user-data-constraint>
- <transport-guarantee>CONFIDENTIAL</transport-guarantee>
- </user-data-constraint>
- </security-constraint>
配置完成后，使用http(不输入端口号)协议方式进入页面也会自动转换为https协议进行访问。
