# https 双向认证开发实践 - z69183787的专栏 - CSDN博客
2019年02月14日 12:20:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：103
个人分类：[Http协议-HTTPS/SSL/TLS](https://blog.csdn.net/z69183787/article/category/7168332)
[https://blog.csdn.net/dtlscsl/article/details/50118225](https://blog.csdn.net/dtlscsl/article/details/50118225)
https双向认证 证书如何使用
一.概念介绍
1.https协议介绍 与http协议的区别
https协议简单来说就是http协议的基础上增加了SSL协议 ，从而来保证数据传输的安全性。
SSL协议：
SSL协议位于TCP/IP协议与各种应用层协议之间，为数据通讯提供安全支持。SSL协议可分为两层：SSL记录协议（SSL Record Protocol）：它建立在可靠的传输协议（如TCP）之上，为高层协议提供数据封装、压缩、加密等基本功能的支持。SSL握手协议（SSL Handshake Protocol）：它建立在SSL记录协议之上，用于在实际的数据传输开始前，通讯双方进行身份认证、协商加密算法、交换加密密钥等。SSL依靠证书来验证服务器的身份，并为浏览器和服务器之间的通信加密。
区别：
一、https协议需要到ca申请证书。
二、http是超文本传输协议，信息是明文传输，https 则是具有安全性的ssl加密传输协议。
三、http和https使用的是完全不同的连接方式，用的端口也不一样，前者是80，后者是443。
四、http的连接很简单，是无状态的；HTTPS协议是由SSL+HTTP协议构建的可进行加密传输、身份认证的网络协议，比http协议安全。
2.SSL证书
SSL 证书就是遵守 SSL协议，由受信任的数字证书颁发机构CA，在验证服务器身份后颁发，具有服务器身份验证和数据传输加密功能。
证书中一般包含公钥拥有者的信息，公钥，颁发机构的信息等。
使用keytool生成证书的过程，看到比较多的文件类型。cer crt key jks keystore,如果之前没有接触过，真是一头雾水，下面简单介绍一下这几个文件的意义
keystore是Java保存密钥对的秘钥仓库，文件后缀为keystore，后缀类似.jks .truststore都是保存秘钥和证书的仓库
.cer:存储公钥证书的文件格式。
.crt:客户端认证的证书、私钥。
双向认证需要服务端和客户端都提供cer格式的证书，然后导入到彼此的keystore中
3.双向认证的过程
1-客户端请求服务端，
2-服务器向客户端传送SSL协议的版本号，加密算法的种类等信息，将证书发给客户端
3-客户端根据服务器传来的信息验证服务器的合法性
4-客户端生成一个随机秘钥，用服务器传来的证书中的公钥进行加密，将加密后的结果传给服务器
5-双向认证要求客户端将自己的证书传给服务器进行验证
6-服务器验证客户端的信息
简单来说就是客户端先验证服务器的证书 ，然后服务器验证客户端的证书，验证通过，就可以开始传输数据
4.相关的API接口介绍
TrustManager Interface
The primary responsibility of the TrustManager is to determine whether the presented authentication credentials should be trusted. If the credentials are not trusted, the connection will be terminated. To authenticate the remote identity of a secure socket peer, you need to initialize an SSLContext object with one or more TrustManagers. You need to pass one TrustManager for each authentication that is supported. If null is passed into the SSLContext initialization, a trust manager will be created for you. Typically, there is a single trust manager that supports authentication based on X.509 public key certificates (e.g. X509TrustManager). Some secure socket implementations may also support authentication based on shared secret keys, Kerberos, or other mechanisms.
TrustManager接口
TrustManager接口的主要职责是判定提供给它的认证证书是否应该被信任。如果证书不被信任，SSL安全连接将会中断。为了认证一个安全套接字对端的身份，需要初始化一个包含一个到多个TrustManager的SSLContext对象。需要为每一种支持的认证机制提供一个TrustManager。
一般的，仅会有一个支持基于X.509（数字证书标准）公钥认证的TrustManager。
KeyManager Interface
The primary responsibility of the KeyManager is to select the authentication credentials that will eventually be sent to the remote host. To authenticate yourself (a local secure socket peer) to a remote peer, you need to initialize an SSLContext object with one or moreKeyManagers. You need to pass one KeyManager for each different authentication mechanism that will be supported. If null is passed into theSSLContext initialization, an empty KeyManager will be created. If the internal default context is used (e.g. a SSLContext created bySSLSocketFactory.getDefault() or SSLServerSocketFactory.getDefault()), a default KeyManager is created. Typically, there is a single key manager that supports authentication based on X.509 public key certificates. Some secure socket implementations may also support authentication based on shared secret keys, Kerberos, or other mechanisms.
KeyManager接口
KeyManager接口的主要职责是选择一个最终会发送到远端服务器的认证证书。为了向一个远端认证自身（安全套接字本地端），需要初始化一个包含一个到多个KeyManager的SSLContext对象；需要为每一种支持的认证机制提供一个KeyManager。
一般的，仅会有一个支持基于X.509（数字证书标准）公钥认证的KeyManager。
比较：
TrustManager：决定对端的认证证书是否被信任
KeyManager：决定将哪一个认证证书发送给对端服务器
二.结合实际应用：
1.实现功能描述
这是我在企业中经历一个开发工作：对接第三方平台，要求https双向认证通信，服务器会提供自己的证书，同时也会给客户端颁发证书，下面简单介绍如何开发https双向认证客户端 然后成功调用第三方平台的接口。
2.如何实现
第三方平台提供如下格式的证书文件：
每个文件的意义是：
客户端私钥：client.key.pem
客户端证书：client.pem 
客户端证书（p12,无加密）：client.p12
CM-OSP平台服务端证书CA证书：Server_CA.cer
client.pem 和client.p12 都是客户端证书 只是文件的格式不同 可以用openssl进行格式转换 里面存放了证书信息以及客户端秘钥
https双向认证中通常用java.security.KeyStore来加载证书文件，一般分为两种类型 JKS对应加载.jks或者.keyStore格式的文件，PKS12加载p12类型的文件。
为了实现双向认证 同时从服务器安全上面考虑 决定采用自定义TrustManager和KeyManager的方式 来实现
### 一.初始化TrustManager
有以下两种方法：
**方法1.不对服务器的证书做检验，信任所有证书**
```java
SSLContext ctx = SSLContext.getInstance("SSL");
            //Implementation of a trust manager for X509 certificates  
            X509TrustManager tm = new X509TrustManager() {
 
                @Override
                public void checkClientTrusted(java.security.cert.X509Certificate[] chain,
                                               String authType)
                                                               throws java.security.cert.CertificateException {
                }
 
                @Override
                public void checkServerTrusted(java.security.cert.X509Certificate[] chain,
                                               String authType)
                                                               throws java.security.cert.CertificateException {
                }
 
                @Override
                public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                    return null;
                }
            };
            ctx.init(null, new TrustManager[] { tm }, null);
            SSLConnectionSocketFactory ssf = new SSLConnectionSocketFactory(ctx);
            httpclient = HttpClients.custom()
                .setHostnameVerifier(SSLConnectionSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER)
                .setSSLSocketFactory(ssf).build();
```
**方法2.添加服务器的证书信任**
首先将服务器的证书导入到jks秘钥仓库中 在cmd中使用keytool的命令
keytool -import -alias newroot -file Server_CA.cer -keystore server.jks 
若server.jks不存在 则会新建一个server.jks文件 要求你输入秘钥库的密码password，这个密码在你查看秘钥库和代码中加载jks文件的时候都需要用到 这里设置的密码是123456 
```
// 自定义获取TrustManager
String password = "123456";
TrustManagerFactory trustFactory = TrustManagerFactory.getInstance("SunX509", "SunJSSE");
KeyStore tsstore = KeyStore.getInstance("JKS");
tsstore.load(new FileInputStream(new File("server.jks")), password.toCharArray()); 
trustFactory.init(tsstore);
TrustManager[] trustManagers = trustFactory.getTrustManagers();
```
### 二.初始化keyManager
服务器提供了p12类型的客户端证书，所以我先尝试用PKS12类型的keyStore来加载client.p12证书文件 初始化keyManager 
P12文件无加密 所以代码如下：
```java
KeyManagerFactory keyFactory = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
KeyStore keystore = KeyStore.getInstance("PKCS12");
keystore.load(new FileInputStream(new File("client.p12")), null);
keyFactory.init(keystore, null);
```
因为client.p12是无加密的，所以load和init设置密码为null，程序运行到init的时候就报错了，错误如下
java.security.UnrecoverableKeyException: Get Key failed: / by zero
也试过设置密码为空字符串 但是在load这一步就报异常了。
所以使用openssl命令重新生成了一份.p12文件，然后加载成功了。
 openssl pkcs12 -export -out client.p12 -in clent.pem -inkey client.key.pem
还有另外一种方式 
用keyStore加载.jks类型的文件 但是平台没有直接提供这种类型的文件
所以我们要使用自己生成jks文件，jks文件中需要包含客户端的秘钥对和信息
首先按照下面的命令将客户端私钥client.key.pem转换成der格式
openssl pkcs8 -topk8 -nocrypt -in client.key.pem -inform PEM -out key.der -outform DER
然后将客户端证书Alibaba_communication_qa.pem转换成der格式
openssl x509 -in client.pem -inform PEM -out cert.der -outform DER
最后将获得的key.der和cert.der在ImportKey程序中跑一下 导出一个keystore文件，程序和网址参考如下：
http://www.blogjava.net/javabloger/archive/2008/04/17/193800.html
jks文件也可以参考：[https://blog.csdn.net/z69183787/article/details/87254283](https://blog.csdn.net/z69183787/article/details/87254283)
```java
KeyManagerFactory keyFactory = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
KeyStore keystore = KeyStore.getInstance("JKS");
keystore.load(new FileInputStream(new File("client.jks")), null);
keyFactory.init(keystore, null);
```
两者比较 更改了秘钥库的类型 最终代码如下
```java
//自定义获取KeyMananger
String password = "123456";
KeyManagerFactory keyFactory = KeyManagerFactory.getInstance("SunX509", "SunJSSE");
KeyStore keystore = KeyStore.getInstance("JKS");
keystore.load(new FileInputStream(new File("client.jks"))), password.toCharArray());
keyFactory.init(keystore, password.toCharArray());
KeyManager[] keyManagers = keyFactory.getKeyManagers();
```
三.初始化ssl，注册httpClient
```java
ctx.init(keyManagers, new TrustManager[]{tm}, null);
SSLConnectionSocketFactory ssf = new SSLConnectionSocketFactory(ctx);
httpclient = new DefaultHttpClient();
SSLSocketFactory socketFactory = new SSLSocketFactory(ctx);
socketFactory.setHostnameVerifier(SSLConnectionSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);
//通过SchemeRegistry将SSLSocketFactory注册到我们的HttpClient上
httpclient.getConnectionManager().getSchemeRegistry().register(new Scheme("https", socketFactory, Integer.parseInt(DiamondUtil.getOperatorConfigData(sellerCode, "PORT"))));
```
到此为止 核心的部分就已经完成。
--------------------- 
