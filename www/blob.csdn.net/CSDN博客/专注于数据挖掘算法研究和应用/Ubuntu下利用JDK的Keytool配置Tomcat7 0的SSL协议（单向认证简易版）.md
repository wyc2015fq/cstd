# Ubuntu下利用JDK的Keytool配置Tomcat7.0的SSL协议（单向认证简易版） - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年12月09日 15:15:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2271标签：[tomcat																[jdk																[ubuntu																[ssl																[scheme																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)





 1.用JDK自带的Keytool生成服务器证书：

  打开终端控制台，转向tomcat主目录，执行生成keystore文件命令：

    keytool -genkey -alias tomcat -keyalg RSA -keypass tomcat -storepass tomcat -keystore server.keystore -validity 3600

    ——在tomcat主目录下生成server.keystore文件；


2.修改tomcat配置：conf/server.xml

  <Connector port="8443" protocol="HTTP/1.1" SSLEnabled="true"

               maxThreads="150" scheme="https" secure="true"

               clientAuth="false" sslProtocol="TLS" 

               keystoreFile="server.keystore" keystorePass="tomcat"

    />

3.https://127.0.0.1:8443/ 打开tomcat首页。](https://so.csdn.net/so/search/s.do?q=scheme&t=blog)](https://so.csdn.net/so/search/s.do?q=ssl&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)




