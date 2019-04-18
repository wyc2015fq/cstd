# Ubuntu下利用JDK的Keytool配置Tomcat7.0的SSL协议 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月08日 11:38:05[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4962








Ubuntu下利用JDK的Keytool配置Tomcat7.0的SSL协议：


1.用JDK自带的Keytool生成服务器证书：

  1)打开终端控制台，转向tomcat主目录，执行生成keystore文件命令：

    keytool -genkey -alias tomcat -keyalg RSA -keypass tomcat -storepass tomcat -keystore server.keystore -validity 3600

    ——在tomcat主目录下生成server.keystore文件；

  2)根据keystore文件产生的证书请求，向CA申请服务器数字证书：

    keytool -export -trustcacerts -alias tomcat -file server.cer -keystore server.keystore -storepass tomcat

    ——在tomcat主目录下生成server.cer文件；

  3)将信息中心签发的服务器证书server.cer导入到server.keystore文件：

   keytool -import -trustcacerts -alias tomcat -file server.cer -keystore  server.keystore -storepass tomcat


2.用JDK自带的Keytool生成客户端证书：

  1)为支持证书顺利导入到IE和Firefor，证书格式为PKCS12，命令如下：

   keytool -genkey -v -alias client -keyalg RSA -storetype PKCS12 -validity 3600 -keystore client.p12 -storepass client -keypass client

   ——在tomcat主目录下生成client.p12文件；

  2)让服务器信任客户端证书：

   双向SSL认证，服务器要信任客户端证书，因此要把客户端证书添加为服务器的信任认证，由于不能直接将PKCS12格式的证书导入，要先把客户端证书导出为一个单独的CER文件，命令：

   keytool -export -alias client -keystore client.p12 -storetype PKCS12 -storepass client -rfc -file client.cer

   ——在tomcat主目录下生成client.cer文件；

   将client.cer导入到服务器的证书库server.keystore，添加为一个信任证书：

   keytool -import -v -file client.cer -keystore server.keystore -storepass tomcat

   ——认证已添加至keystore中

  3)通过list命令查看服务器的证书库，可以看到两个输入，一个是服务器证书，一个是受信任的客户端证书：keytool -list -keystore server.keystore -storepass tomcat


  4)删除命令：keytool -delete -alias myKey -keystore server.keystore -storepass tomcat


3.修改tomcat配置：conf/server.xml

  <!-- Define a SSL HTTP/1.1 Connector on port 8443

         This connector uses the JSSE configuration, when using APR, the 

         connector should be using the OpenSSL style configuration

         described in the APR documentation -->

  <Connector port="8443" protocol="HTTP/1.1" SSLEnabled="true"

               maxThreads="150" scheme="https" secure="true"

               clientAuth="false" sslProtocol="TLS" 

               keystoreFile="server.keystore" keystorePass="tomcat"

               truststoreFile="server.keystore" truststorePass="client"

    />

  1)clientAuth="true"，双向认证；

  2)clientAuth="false"，单向认证；

4.重新启动tomcat，执行./bin/startup.sh命令，访问https://127.0.0.1:8443

  1)clientAuth="false"，单向认证，成功打开tomcat首页；

  2)clientAuth="true"，双向认证，打开Firefox菜单：编辑->首选项->高级->加密->查看证书->你的证书，将client.p12导入到IE中，按照Firefox提示完成登录tomcat首页；


5.服务器SSl证书获取代码

  if(request.isSecure())//如果是SSL通信

  {

    java.security.cert.X509Certificate[] certs=(java.security.cert.X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");

    if(certs!=null && certs.lengtt>0)

    {

        subjectDN="Certificates found";

    }

  }

  使用java解析证书可获取证书中用户信息

  issue=certs.getIssuerDN().toString();//证书签发者

  subject=certs.getSubjectDN().getName();//证书所有者

  after=certs.getNotAfter().toString();//证书起效时间

  before=certs.getNotBefore().toString();//证书到期时间

  version=Integer.toString(certs.getVersion());//证书版本

  serialno=certs.getSerialNumber().toString();//证书序列号



5.参考地址：

[http://tomcat.apache.org/tomcat-6.0-doc/ssl-howto.html](http://tomcat.apache.org/tomcat-6.0-doc/ssl-howto.html)  //tomcat6.0 ssl配置

[http://wenku.baidu.com/view/e7b22df0f90f76c661371a6f.html](http://wenku.baidu.com/view/e7b22df0f90f76c661371a6f.html)    //Tomcat SSL配置







