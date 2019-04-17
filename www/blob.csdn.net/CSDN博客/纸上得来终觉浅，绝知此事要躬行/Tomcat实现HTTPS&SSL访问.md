# Tomcat实现HTTPS&SSL访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月19日 16:09:51[boonya](https://me.csdn.net/boonya)阅读数：1570









### 1.0 HTTPS和HTTP的区别

　一、https协议需要到ca申请证书，一般免费证书很少，需要交费。

　二、http是超文本传输协议，信息是明文传输，https 则是具有安全性的ssl加密传输协议。

　三、http和https使用的是完全不同的连接方式，用的端口也不一样，前者是80，后者是443。

    四、http的连接很简单，是无状态的；HTTPS协议是由SSL+HTTP协议构建的可进行加密传输、身份认证的网络协议，比http协议安全。

### 2.0 HTTPS



### 2.1 https的实现原理



有两种基本的加解密算法类型：

1）对称加密：密钥只有一个，加密解密为同一个密码，且加解密速度快，典型的对称加密算法有DES、AES等；

2）非对称加密：密钥成对出现（且根据公钥无法推知私钥，根据私钥也无法推知公钥），加密解密使用不同密钥（公钥加密需要私钥解密，私钥加密需要公钥解密），相对对称加密速度较慢，典型的非对称加密算法有RSA、DSA等。
![](https://img-blog.csdn.net/20160630145505726)




#### 2.2 https通信的优点

1）客户端产生的密钥只有客户端和服务器端能得到；

2）加密的数据只有客户端和服务器端才能得到明文；

3）客户端到服务端的通信是安全的。

加密过程：

在https的通信过程中，组合使用了公用密钥方式加密和共享密钥方式加密这两种技术。公用密钥方式要比共享密钥方式慢得多，因此使用公用密钥来完成共享密钥的交换。公用密钥是从证书中获得的。拿到共享密钥之后，所有的内容之间的通信都会使用共享密钥来进行加密和解密。公用密钥的作用就是为了安全的交换共享密钥，共享密钥是用来通信内容的加密和解密。这样既确保了通信之间的安全，也确保了加密和解密的效率。





### 3.0 SSL




### 3.1 SSL介绍

        为Netscape所研发，用以保障在Internet上数据传输之安全，利用数据加密(Encryption)技术，可确保数据在网络上之传输过程中不会被截取及窃听。目前一般通用之规格为40 bit之安全标准，美国则已推出128 bit之更高安全标准，但限制出境。只要3.0版本以上之I.E.或Netscape浏览器即可支持SSL。

　　当前版本为3.0。它已被广泛地用于Web浏览器与服务器之间的身份认证和加密数据传输。

SSL协议位于TCP/IP协议与各种应用层协议之间，为数据通讯提供安全支持。SSL协议可分为两层：SSL记录协议（SSL Record Protocol）：它建立在可靠的传输协议（如TCP）之上，为高层协议提供数据封装、压缩、加密等基本功能的支持。SSL握手协议（SSL Handshake Protocol）：它建立在SSL记录协议之上，用于在实际的数据传输开始前，通讯双方进行身份认证、协商加密算法、交换加密密钥等。

连接分为两个阶段，即握手和数据传输阶段。

握手阶段对服务器进行认证并确立用于保护数据传输的加密密钥。必须在传输任何应用数据之前完成握手。一旦握手完成，数据就被分成一系列经过保护的记录进行传输。



#### 3.2 SSL协议主要提供的服务

　　1）认证用户和服务器，确保数据发送到正确的客户机和服务器

　　2）加密数据以防止数据中途被窃取

　　3）维护数据的完整性，确保数据在传输过程中不被改变。

[](http://jingyan.baidu.com/album/0f5fb099f1f9566d8334ea38.html?picindex=2)





### 4.0 配置Tomcat的HTTPS和SSL

#### 4.1 利用JDK生成创建用户安全证书
![](https://img-blog.csdn.net/20150619160049990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

创建成功后的文件
![](https://img-blog.csdn.net/20150619160137298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 4.2 修改Tomcat的server.xml文件
![](https://img-blog.csdn.net/20150619160225132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 4.3启动Tomcat并测试HTTPS配置是否成功
![](https://img-blog.csdn.net/20150619160314330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如上图，可以看到https可以打开，以下分别是在IE、Firefox打开效果：

![](https://img-blog.csdn.net/20150619160427697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

IE说：证书还有问题，地址栏右上角有提示。
![](https://img-blog.csdn.net/20150619160526749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Firefox也可以打开开到效果，不过如果是真实项目的访问，必须要安全证书通过验证才行，下面是我允许不用安全模式进入系统的效果：

![](https://img-blog.csdn.net/20150619160709081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









