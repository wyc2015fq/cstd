# HTTPS 原理及安全加密方案 - z69183787的专栏 - CSDN博客
2017年09月23日 21:16:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：829
[http://www.jianshu.com/p/0556a610e68e](http://www.jianshu.com/p/0556a610e68e)
## HTTPS 原理及安全加密方案
### 一、什么是HTTPS
在说HTTPS之前先说说什么是HTTP，HTTP就是我们平时浏览网页时候使用的一种协议。HTTP协议传输的数据都是未加密的，也就是明文的，因此使用HTTP协议传输隐私信息非常不安全。为了保证这些隐私数据能加密传输，于是网景公司设计了SSL（Secure Sockets Layer）协议用于对HTTP协议传输的数据进行加密，从而就诞生了HTTPS。SSL目前的版本是3.0，被IETF（Internet Engineering Task Force）定义在RFC 6101中，之后IETF对SSL 3.0进行了升级，于是出现了TLS（Transport
 Layer Security） 1.0，定义在RFC 2246。实际上我们现在的HTTPS都是用的TLS协议，但是由于SSL出现的时间比较早，并且依旧被现在浏览器所支持，因此SSL依然是HTTPS的代名词，但无论是TLS还是SSL都是上个世纪的事情，SSL最后一个版本是3.0，今后TLS将会继承SSL优良血统继续为我们进行加密服务。目前TLS的版本是1.2，定义在RFC 5246中，暂时还没有被广泛的使用。
<!--more-->
对历史感兴趣的朋友可以参考[http://en.wikipedia.org/wiki/Transport_Layer_Security](http://en.wikipedia.org/wiki/Transport_Layer_Security)，这里有对TLS/SSL详尽的叙述。
### 二、HTTPS到底安全吗？
这个答案是肯定的，很安全。谷歌公司已经行动起来要大力推广HTTPS的使用，在未来几周，谷歌将对全球所有本地域名都启用HTTPS，用户只要在搜索前用Google帐号登录，之后所有的搜索操作都将使用TLS协议加密，见：[http://thenextweb.com/google/2012/03/05/google-calls-for-a-more-secure-web-expands-ssl-encryption-to-local-domains/](http://thenextweb.com/google/2012/03/05/google-calls-for-a-more-secure-web-expands-ssl-encryption-to-local-domains/)。
### 三、HTTPS的工作原理
HTTPS在传输数据之前需要客户端（浏览器）与服务端（网站）之间进行一次握手，在握手过程中将确立双方加密传输数据的密码信息。TLS/SSL协议不仅仅是一套加密传输的协议，更是一件经过艺术家精心设计的艺术品，TLS/SSL中使用了非对称加密，对称加密以及HASH算法。握手过程的简单描述如下：
1.浏览器将自己支持的一套加密规则发送给网站。
2.网站从中选出一组加密算法与HASH算法，并将自己的身份信息以证书的形式发回给浏览器。证书里面包含了网站地址，加密公钥，以及证书的颁发机构等信息。
3.获得网站证书之后浏览器要做以下工作：
a) 验证证书的合法性（颁发证书的机构是否合法，证书中包含的网站地址是否与正在访问的地址一致等），如果证书受信任，则浏览器栏里面会显示一个小锁头，否则会给出证书不受信的提示。
b) 如果证书受信任，或者是用户接受了不受信的证书，浏览器会生成一串随机数的密码，并用证书中提供的公钥加密。
c) 使用约定好的HASH计算握手消息，并使用生成的随机数对消息进行加密，最后将之前生成的所有信息发送给网站。
4.网站接收浏览器发来的数据之后要做以下的操作：
a) 使用自己的私钥将信息解密取出密码，使用密码解密浏览器发来的握手消息，并验证HASH是否与浏览器发来的一致。
b) 使用密码加密一段握手消息，发送给浏览器。
5.浏览器解密并计算握手消息的HASH，如果与服务端发来的HASH一致，此时握手过程结束，之后所有的通信数据将由之前浏览器生成的随机密码并利用对称加密算法进行加密。
这里浏览器与网站互相发送加密的握手消息并验证，目的是为了保证双方都获得了一致的密码，并且可以正常的加密解密数据，为后续真正数据的传输做一次测试。另外，HTTPS一般使用的加密与HASH算法如下：
非对称加密算法：RSA，DSA/DSS
对称加密算法：AES，RC4，3DES
HASH算法：MD5，SHA1，SHA256
其中非对称加密算法用于在握手过程中加密生成的密码，对称加密算法用于对真正传输的数据进行加密，而HASH算法用于验证数据的完整性。由于浏览器生成的密码是整个数据加密的关键，因此在传输的时候使用了非对称加密算法对其加密。非对称加密算法会生成公钥和私钥，公钥只能用于加密数据，因此可以随意传输，而网站的私钥用于对数据进行解密，所以网站都会非常小心的保管自己的私钥，防止泄漏。
TLS握手过程中如果有任何错误，都会使加密连接断开，从而阻止了隐私信息的传输。正是由于HTTPS非常的安全，攻击者无法从中找到下手的地方，于是更多的是采用了假证书的手法来欺骗客户端，从而获取明文的信息，但是这些手段都可以被识别出来，我将在后续的文章进行讲述。
不过2010年还是有安全专家发现了TLS 1.0协议处理的一个漏洞：
[http://www.theregister.co.uk/2011/09/19/beast_exploits_paypal_ssl/](http://www.theregister.co.uk/2011/09/19/beast_exploits_paypal_ssl/)，实际上这种称为BEAST的攻击方式早在2002年就已经被安全专家发现，只是没有公开而已。目前微软和Google已经对此漏洞进行了修复。见：[http://support.microsoft.com/kb/2643584/en-us
 https://src.chromium.org/viewvc/chrome?view=rev&revision=90643](http://support.microsoft.com/kb/2643584/en-us%20https://src.chromium.org/viewvc/chrome?view=rev&revision=90643)
### HTTPS建立连接时证书的验证流程
![](http://upload-images.jianshu.io/upload_images/670028-d6bbf887a484d6dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
证书验证流程1
![](http://upload-images.jianshu.io/upload_images/670028-cfd007c755f85314.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
证书验证流程2
### 2.什么是SSL Pinning？
可以理解为证书绑定，是指客户端直接保存服务端的证书，建立https连接时直接对比服务端返回的和客户端保存的两个证书是否一样，一样就表明证书是真的，不再去系统的信任证书机构里寻找验证。这适用于非浏览器应用，因为浏览器跟很多未知服务端打交道，无法把每个服务端的证书都保存到本地，但CS架构的像手机APP事先已经知道要进行通信的服务端，可以直接在客户端保存这个服务端的证书用于校验。
为什么直接对比就能保证证书没问题？如果中间人从客户端取出证书，再伪装成服务端跟其他客户端通信，它发送给客户端的这个证书不就能通过验证吗？确实可以通过验证，但后续的流程走不下去，因为下一步客户端会用证书里的公钥加密，中间人没有这个证书的私钥就解不出内容，也就截获不到数据，这个证书的私钥只有真正的服务端有，中间人伪造证书主要伪造的是公钥。
为什么要用SSL Pinning？正常的验证方式不够吗？如果服务端的证书是从受信任的的CA机构颁发的，验证是没问题的，但CA机构颁发证书比较昂贵，小企业或个人用户可能会选择自己颁发证书，这样就无法通过系统受信任的CA机构列表验证这个证书的真伪了，所以需要SSL Pinning这样的方式去验证。
作者：RunningYoung丶
链接：http://www.jianshu.com/p/0556a610e68e
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
