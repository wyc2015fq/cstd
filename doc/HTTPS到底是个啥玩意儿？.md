# HTTPS到底是个啥玩意儿？

2016年03月14日 21:09:48 [简单的老王](https://me.csdn.net/zgwangbo) 阅读数：10911 标签： [安全](http://so.csdn.net/so/search/s.do?q=%E5%AE%89%E5%85%A8&t=blog)[网络](http://so.csdn.net/so/search/s.do?q=%E7%BD%91%E7%BB%9C&t=blog)[移动](http://so.csdn.net/so/search/s.do?q=%E7%A7%BB%E5%8A%A8&t=blog)[java](http://so.csdn.net/so/search/s.do?q=java&t=blog)[密码](http://so.csdn.net/so/search/s.do?q=%E5%AF%86%E7%A0%81&t=blog)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/zgwangbo/article/details/50889623

[ 注：转载请注明来源：http://blog.csdn.net/zgwangbo , 也可关注微信： simplemain]

![img](https://img-blog.csdn.net/20160314211045391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们现在经常听说谁谁谁密码被盗了，谁谁谁信息又被劫持了。其中有一个原因：绝大部分网站用的是http这个明文协议。你以为很安全的在password框里填了隐藏的密码，他却一字一句[明明白白](https://www.baidu.com/s?wd=%E6%98%8E%E6%98%8E%E7%99%BD%E7%99%BD&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的写到了网络上。于是乎好多网站开始从http迁移到https（至少登录部分）。我也准备做同样的事情，因此抽时间和小伙伴tt一起研究了一下https。

刚开始看https的时候，各种头大。国内网上讲相关的资料虽然一大堆，但是大部分是相互的抄，内容多而乱，且没有把事情讲清楚。后来查阅了一些外文资料（包括rfc、wikipedia等），读了JSSE的源代码以后，基本把这个事情的[来龙去脉](https://www.baidu.com/s?wd=%E6%9D%A5%E9%BE%99%E5%8E%BB%E8%84%89&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)看懂了大部分，但是涉及到很多很细节的东西还是觉得不是完全懂，如有疏漏和错误，敬请大家指正和原谅 :-)

这篇文章的目标：用尽量简单和有趣的语言，把这个复杂的东东讲述清楚。所以，接下来我打算分成三部分来聊聊我理解的Https：

1、入门篇：主要用通俗的语言讲讲Https是什么东东，以及他大体的工作方式；

2、技术篇：结合抓包工具和源代码，分析Https的通讯流程和细节；

3、理论篇：不是特别深入的聊聊一些跟Https相关的算法。

  

**==== 入门篇的分割线 ====**

 

**What’s HTTPS？**

 

简单的说，https就是给http带了一个安全套，即使别人拿到了信息，也不知道这个里面装的啥。客户端（包括browser、手机app等）和服务器每次发http包的时候，都对这个包加个密，让第三者看到的只是加密后的乱码（我只想对你说：你猜你猜你猜猜猜），到对端以后再解密。

这个安全套，原来是叫SSL（Secure Sockets Layer），最先是Netscape弄出来的，后来哥们儿完蛋了，就慢慢变了名字，叫TLS（Transport Layer Security Protocol）。具体的区别可以去wikipedia搜索TLS，他们之间的升级细节讲述的非常详细（这一点百度百科真的差的有点远~）。

这个安全套跑在TCP的上层，在TCP连接完成后且HTTP启动前，协商一些跟加密相关的工作，完成协商之后，就可以对要发送的http包加密/解密了。

那他到底协商了些啥呢？其实就是保证安全的几个问题：

1、服务器要证明自己是靠谱的、安全的，不然给一个假网站发加密的密文就跟裸奔没啥区别

2、服务器和客户端通讯需要的加密算法和加密密钥

就跟当年天地会和韦小宝通信一样，先要亮出身份，证明自己，然后再拿出暗语的书信。



**Come On! How TLS works?**

 

第一步，服务器证明自己是靠谱的。

一个哥们儿XX说他是天地会的。如果你是韦小宝，你会怎么确认他的身份呢？

其中有一种方案可能是这样的：他会说S1是他师傅，如果你知道S1并和他确认了，就ok了。如果不认识，就继续问S1的师傅S2……一直问道陈近南，只要陈近南确认了，那就可以证明他了。看起来好像设计模式里面的责任链 XX -> S1 ->S2 -> … ->ROOT

服务器证明自己也是同样的逻辑，服务器S0有一个证书，说我是谁谁谁，这个证书由上级签发机构S1核准，如果你本地有这个S1的证书，那验证一下就可以了。如果没有，就问S1的签发机构S2。直到根的签发机构。如果本地认证找到了其中任何一级的证书，就认为S0是靠谱的。否则就是不靠谱。S0 -> S1 -> S2 -> … -> Root CA

 

实际上非常像工商局发的营业执照，你上面有我盖的红坨坨才是靠谱的。

![img](https://img-blog.csdn.net/20160314204241624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





上图就是淘宝的认证级联关系。

这些靠谱的证书内置在操作系统、jdk等地方（百度或者谷歌上搜索“https数字证书设置”相关内容就可以看到）。

![img](https://img-blog.csdn.net/20160314204626098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

此图就是我本机证书列表的一部分。

这个就是基本逻辑，说白了，就是找一个我们都公认靠谱的人来证实你的靠谱。

 

第二步，协商加密算法+密钥。

加密和摘要算法有很多，常见的比如RSA、AES、DES、MD5、SHA等等。

大家把他们这样来分：

1、加密/解密算法：能加密同时能反解的，就是加解密算法。按照加解密的密钥是否一样，又分为对称和非对称算法。比如对称加密算法：AES、DES；非对称加密算法：RSA。

2、摘要算法：就是只用来做摘要、签名、验证防止被别人篡改，基本不能反解（有可能可以通过碰撞暴力破解）。比如：MD5、SHA。

那服务器和客户端接下来就协商一下，我们要用什么加密解密算法和密钥防止别人看见，用什么摘要算法，防止别人篡改。

一般来讲，对称加密算法效率会比非对称高，所以通常选择对称加密的AES较多。双方通过某种方式协商出一个密钥，后面就通过这个密钥和加密算法进行加解密。

 

客户端发送一个：“地振高冈，一派溪山千古秀”

服务端回复一个：“门朝大海，三河合水万年流”

整个过程大体就是这样，后面双方就开始发HTTP的加密包，对方解包得到对应的HTTP数据。

世界一下就清晰了，对吗？

No No No 其实还是很复杂滴……如果要想了解详细的技术内容，就让我带着你继续往下看（你敢不敢跟我来）

 

**=== 技术篇的分割线 ===**

 

**工欲善其事，必先利其器**

 

为了做详细的分析，我做了几个准备工作：

1、装了一个wireshark，用来抓取网络包

2、写了一个java程序，打开debug运行（java -Djavax.net.debug=all TestHttps），用来看交互细节

import java.net.URL;

import java.net.URLConnection;

 

public class TestHttps

{

​    public staticvoidmain(String[] args)throws Exception

​    {

​       final URL url = new URL("https://www.taobao.com");

​       final URLConnectionconn = url.openConnection();

​       conn.connect();

​    }

}

 

3、找到openjdk源代码：http://grepcode.com/

通过前两个工作可以看到网络交互的过程和详细的数据包，第三个可以用来分析整个流程的代码。

（注：以下涉及到代码的分析，都是基于JDK8进行的，如果因为版本原因，相关函数和代码行数对接不上，请大家查找对应版本的代码）

 

好了，准备工作做好了，我们开始吧！



**抓个包，先看看门道**

先给taobao同学发个请求吧：curl https://www.taobao.com，看到整个交互过程大体是这样的（我把tcp三次握手，ACK包等无关的数据包都过滤掉了，只剩TLS相关的数据包）：





上图有几个交互数据都合并到一个TCP包进行发送了，比如漂蓝的那一行（No = 49）的TCP包实际上包含了三个TLS包（Certificate、Server Key Exchange、Server Hello Done），下面分析的时候，我就把这个包展开。

 

| **Client**                     | **Server**                     |
| ------------------------------ | ------------------------------ |
| Client Hello ->                |                                |
|                                | <- Server Hello                |
|                                | <- Certificate                 |
|                                | <- Server Key Exchange         |
|                                | <- Server Hello Done           |
| Client Key Exchange ->         |                                |
| Change Cipher Spec ->          |                                |
| Encrypted Handshake Message -> |                                |
|                                | <- Change Cipher Spec          |
|                                | <- Encrypted Handshake Message |
| Application Data ->            |                                |
|                                | <- Application Data            |
| Encrypted Alert ->             |                                |

 

上面抓的包全部展开就是这样的效果。怎么样，是不是差不多也看了个大概？我来翻译翻译吧。

 

| **Client**                                        | **Server**                                        |
| ------------------------------------------------- | ------------------------------------------------- |
| Client Hello你好！                                |                                                   |
|                                                   | Server Hello嗯，你好！                            |
|                                                   | Certificate我的证书给你，验证我吧                 |
|                                                   | Server Key Exchange这是我给你的加密密钥相关的东东 |
|                                                   | Server Hello Done好，我说完了                     |
| Client Key Exchange这是我给你的加密密钥相关的东东 |                                                   |
| Change Cipher Spec准备转换成密文了哦              |                                                   |
| Encrypted Handshake Message%……&*4 （密文思密达）  |                                                   |
|                                                   | Change Cipher Spec我也转换密文了                  |
|                                                   | Encrypted Handshake Message#%&……* （密文思密达）  |
| Application Data%&￥&%*……（HTTP密文数据）         |                                                   |
|                                                   | Application Data**……&%（HTTP密文数据）            |
| Encrypted Alert警告（实际就是说完了，拜拜~）      |                                                   |

 

 

看起来是不是很简单呢？

这实际上就是文章一开始，我说的要解决的两个大问题：

1、认证server端的靠谱性

2、交换加密算法和密钥

具体每个包里面都发了哪些数据？server端靠谱性是如何来证明的？加密算法和密钥是怎么交换的？接下来让我一一给你道来。

  

**具体的交互流程和代码的实现**

 

 ![img](https://img-blog.csdn.net/20160314204846367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 

我们就按命令逐个来分析一下。

 

C：ClientHello





可以看到发送了很多数据，但是最关键的几个数据：

1、TLS的版本

2、随机数：这个是用来生成最后加密密钥的影响因子之一，包含两部分：时间戳（4-Bytes）和随机数（28-Bytes）

3、session-id：用来表明一次会话，第一次建立没有。如果以前建立过，可以直接带过去。

4、加密算法套装列表：客户端支持的加密-签名算法的列表，让服务器去选择。

5、压缩算法：似乎一般都不用

6、扩展字段：比如密码交换算法的参数、请求主机的名字等等

 

这一段的java实现，是在sun.security.ssl.HandshakeMessage.ClientHello里面：

 





 

S：ServerHello

 

当服务器收到客户端的问候以后，立即做出了响应： 





 

大体内容和客户端差不多，只是把加密算法的套装列表换成了服务器选择支持的具体算法。

通过这一步，客户端和服务器就**完成了加密和签名算法的交换**。这里的TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256拆分开看就是：TLS协议，用ECDH密钥交换算法交换对称加密密钥相关的参数，用RSA算法做签名，最后使用AES_128_CBC做内容的对称加密，SHA256做摘要。

 

具体实现在：sun.security.ssl.HandshakeMessage.ServerHello 







S：Certificate

 

这一步很关键，是

服务器给客户端展示证书

的时候。

![img](https://img-blog.csdn.net/20160314205234384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

证书是一个链，从最底层一直到最顶层，表示谁谁谁给我认证的。翻译过来就是文章一开始给大家看到的那个东东：

![img](https://img-blog.csdn.net/20160314205306228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

证书一般采用X.509标准，后面我会详细来讲述证书格式和如何级联认证。X509证书具体实现在：sun.security.x509.X509CertImpl

 

Certificate消息的实现代码在：sun.security.ssl.HandshakeMessage.CertificateMsg 里面





 

S：ServerKey Exchange

 

这个消息是用来发送密钥交换算法相关参数和数据的。这里要提前提一下，就是根据密钥交换算法的不同，传递的参数也是不同的。

常用的密钥交换算法：RSA、DH（Diffie-Hellman）、ECDH（Ellipticcurve Diffie–Hellman）

后面会详细来讲这几个算法的某几个，现在就不详细走这个分支，只是知道他们可以交换密钥，有参数要传递即可。

（这里不得不感叹一句，老外对基础科学的研究真的是太深入了，这些算法十分的巧妙。希望有一天中国人也能对基础科学做出更多的贡献~）

![img](https://img-blog.csdn.net/20160314205420947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

可以看到这里用到的是ECDH算法，交换了一些参数，对数据做了签名，防止劫持者篡改。

 

在Java里，这个消息有多个实现，分别代表RSA、DH、ECDH算法，对应的类分别是：

sun.security.ssl.HandshakeMessage.ServerKeyExchange

sun.security.ssl.HandshakeMessage.RSA_ServerKeyExchange

sun.security.ssl.HandshakeMessage.DH_ServerKeyExchange

sun.security.ssl.HandshakeMessage.ECDH_ServerKeyExchange





以下是ECDH的实现：

![img](https://img-blog.csdn.net/20160314205508641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

S：ServerHello Done

 

Server要表达的信息基本表达完了，把主持人话筒交给客户端：





对应的实现：sun.security.ssl.HandshakeMessage.ServerHelloDone

 





 

C：ClientKey Exchange

 

这是客户端对Server Key Exchange的回应，用于交换密钥需要的参数。和服务器一样，不同的密钥交换算法实现是不一样的，因此需要的参数也是有差异的。





这里用的是ECDH交换算法。

 

Java的实现也对应的多个，分别是：

sun.security.ssl.RSAClientKeyExchange

sun.security.ssl.DHClientKeyExchange

sun.security.ssl.ECDHClientKeyExchange

 

以下是ECDH的具体实现：





好了，经过以上的步骤，Server-Client已经将服务器认证的相关工作做完了，密文函数&密钥交换需要的参数也都相互传递了。剩下的，就是各自用一个叫做PRF（Pseudo-RandomFunction）的算法去生成加密密钥，具体的这个函数是一个对多因子多次迭代摘要运算等的实现，这里姑且就当做是一个很简单的随机运算函数吧，比如：key = rand_c +rand_s + C。

到这一步，**客户端和服务器就完成了密钥相关的交换**。

 

有了这个密钥，接下来，客户端和服务器就开始切换交流语言了（用密文开始说悄悄话），他们会各自发一个命令，说明自己已经准备好，开始切换语言了。

 

C：ChangeCipher Spec

 

客户端切换成密文模式

 





 

这个在Java里的实现在：sun.security.ssl.Handshaker

 





 

C：Finished（Encrypted Handshake Message）

 

这个包表明握手已经完成，并且对之前发过的数据进行加密发送给对方做校验，防止被篡改。同时也验证一下，加密算法、密钥工作是否正常。

![img](https://img-blog.csdn.net/20160314205849489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



具体代码在：sun.security.ssl.HandshakeMessage.Finished

![img](https://img-blog.csdn.net/20160314205922302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

在收到这两个消息以后，服务器也发出同样的消息，即：

 

S：Change Cipher Spec

S：Finished（EncryptedHandshake Message）

 

嘘~~~（此地长出一口气） 至此，整个身份验证、加密/解密算法&密钥的交换都已经结束，剩余的，就是进行正常的HTTP请求以及对请求数据的加密/解密。

以上这些步骤，都是对于使用ECDH密钥交换算法、没有session、不要求验证客户端有效性的情况。对于有session的、或者要求验证客户端有效性、或者使用其他密钥交换算法的，请求会有不一样，具体的可以看看实现代码，里面都非常详细也容易阅读，具体代码在：

sun.security.ssl.ClientHandshaker

sun.security.ssl.ServerHandshaker

里面有一个processMessage函数，是用switch...case写的相关的状态机。

看看，为了做安全的HTTP请求，需要额外付出多少的代价，来来回回需要多出多少次数据交换。

好了，如果还想了解其他更多更详细的东东，就继续跟我往下走，否则，在这里就可以return了 ^o^

 

 

 

**=== 有点难度的理论分割线 ===**

 

接下来准备聊聊关于x509证书&证书验证、加解密、签名、密钥交换、随机的一些算法。由于我自己对这部分没有专门的研究，只是借这次机会看了一些资料，所以了解的不是非常深入，可能只能涉及一些皮毛，大家多多谅解和指正。

 

 

**1、关于X.509证书&证书验证**

 

X.509就是一个数字证书的标准，就像工商营业执照一样，证明你这个网站是合法的。详细的可以参见wikipedia和RFC：

<https://en.wikipedia.org/wiki/X.509>

<http://www.ietf.org/rfc/rfc2459.txt>

 

在TLS中使用到了这样一个证书来进行有效性的认证。因为不是专业研究这个的机构，我们就不深入去研究这个标准，而是看看他的数据格式和如何进行验证。

 

**证书的模样**

我们先看看wireshark抓到的包长什么样：





 

再看看Java的API文档给出的比较详细的定义：

<http://docs.oracle.com/javase/8/docs/api/java/security/cert/X509Certificate.html>







 

可以看出来，整体分为证书和对证书的签名两大部分。

证书包含：版本、序号、证书的签名算法、签发者、主题（被签发者）、有效期等的信息。

 

为了方便阅读，我们直接用chrome来查看证书：

![img](https://img-blog.csdn.net/20160314210116919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

可以看到非常详细的信息，包括：被签发者的基本信息、签发者的基本信息，加密信息和签名。我们如果自己用openssl做证书的话，都会要求相关项的填写。有兴趣的同学可以自己做一个证书试试手 ^_^

 

 

**证书的验证**

 

当TLS协议验证一个网站是否有效的时候，Server会给出一个X509的证书链。客户端收到这个证书链以后，对证书链进行验证，所做的工作如下：

1、用最底端（证书链第一个）的证书，去验证请求的主机和证书里的是否是一致

2、逐次验证证书链里每张证书的合法性，直到找到一张证书在系统中存在：这一步又包含每张证书是否在不信任名单里、检查签名算法、检查时间是否过期、检查证书的发布者和证书链的上一级是否匹配、证书链的签名检查

 

以上检查中，大多是按字节对比，相对比较简单，相关代码参见以下几个函数的实现：

sun.security.ssl.X509TrustManagerImpl.checkTrusted

sun.security.validator.SimpleValidator.engineValidate

sun.security.x509.X509CertImpl.verify

sun.security.x509.X509CertImpl.checkValidity

 

不过，其中证书链的签名检查是一个非常有意思的算法，这个算法我着实研究了一阵儿，还写了一个简单的程序去实验，在这里稍微详细讲述一下。

我们得到了一个证书链，将他扩展开成为一个层级关系：

![img](https://img-blog.csdn.net/20160314210152403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

对于每一级的证书，都是由上一级用私钥对证书的sha摘要值进行签名（Root一般由自己签发），签名一般使用RSA算法。验证的时候，用上一级的公钥对签名进行解密，还原对应的摘要值。如下图：

 





 

这里先提前最最最简单的插入一下RSA算法。RSA是非对称加密，有一个公钥（模数和指数）和一个私钥。M代表明文消息，C代表密文，n代表公钥模数，e代表公钥指数，d代表私钥。

如果我们用公钥对明文M加密，私钥解密，则是为了传递信息，对消息进行加密；

如果我们用私钥对明文M加密，公钥解密，则是为了保证消息是我签发的，没有伪造和篡改。

这里，我们介绍后一种（即防篡改和伪造）。

签名加密：C = (M ^ d) mod n

签名解密：M = (C ^ e) mod n

先请不要问我为什么，后面会单独讲，哈哈哈（是不是很贱）~

好，回归正题，上一级认证机构用私钥(d)对下一级的证书的SHA数字签名(M) 进行RSA加密得到密文(C)。而第三方只要用上一级的公钥(e, n)对这个加密进行还原，并能得到相关的SHA数字签名(M)，就可以认为是经过上一级认证过的（因为只有他才能签的出来）。



Come on，我们来实践一下吧：

下图淘宝上一级证书的公钥（这些机构就卖证书就可以赚翻了）：n（图中的256字节公共密钥）和 e（图中的指数） 





下图是淘宝证书的数字签名：C





我们写一个程序来验证一下：





 

因为要用大数计算，所以用到了Java的大数类（这是用的最直接暴力的算法，其实还可以有很多优化，比如O(n) -> O(lgn)，先取模再乘等等）。

最后输出的结果如下：

1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff003031300d060960864801650304020105000420bedfc063c41b62e0438bc8c0fff669de1926b5accfb487bf3fa98b8ff216d650

其中1fff...ff00是PKCS #1v1.5 标准的前导补位，详见：<http://tools.ietf.org/html/rfc2313#page-9>

接下来 303130…0420 是SHA-256摘要算法的标识，详见：<http://tools.ietf.org/html/rfc3447#page-43>

剩余部分正好32Bytes，是整个证书的SHA-256摘要。

 

是真的吗？确认那个32Bytes就是SHA-256的摘要么？来吧，我们写一个程序验证一下：





 

这个程序大致的意思，就是截获X509证书的验证，输出对应的证书信息。并用SHA-256对整个证书做摘要。得到的信息输出如下：

=========================================================================

cert-count:

3

 

subject:

CN=*.tmall.com, O="Alibaba (China) Technology Co., Ltd.",L=HangZhou, ST=ZheJiang, C=CN

 

cert-sign:

3ec0c71903a19be74dca101a01347ac1464c97e6e5be6d3c6677d599938a13b0db7faf2603660d4aec7056c53381b5d24c2bc0217eb78fb734874714025a0f99259c5b765c26cacff0b3c20adc9b57ea7ca63ae6a2c990837473f72c19b1d29ec575f7d7f34041a2eb744ded2dff4a2e2181979dc12f1e7511464d23d1a40b82a683df4a64d84599df0ac5999abb8946d36481cf3159b6f1e07155cf0e8125b17aba962f642e0817a896fd6c83e9e7a9aeaebfcc4adaae4df834cfeebbc95342a731f7252caa2a97796b148fd35a336476b6c23feef94d012dbfe310da3ea372043d1a396580efa7201f0f405401dff00ecd86e0dcd2f0d824b596175cb07b3d

 

sha-256:

bedfc063c41b62e0438bc8c0fff669de1926b5accfb487bf3fa98b8ff216d650

=========================================================================

 

这个证书签名以及对证书做的SHA-256摘要，和我们之前所得到的结果一样一样的~~

 

好了，到此为止，X509证书的格式和验证基本上也讲了一个大概了，真是非常不容易啊（为了写文章，肚子已经饿的咕咕叫了）~

 

 

**2. 关于RSA算法**

 

仔细想想，RSA算法在哪些地方被用到了呢？

1、证书签名的时候（上面刚刚做了验证，对吧）

2、密钥交换







 

为了防止信息被截获篡改，需要对密钥交换的参数做签名。

 

所以，RSA算法是一个非常关键的加解密算法。那我们就来简单聊聊吧~

（最近看RSA，越看越觉得这个算法很有意思~）

注：为了在文本上打印方便，以下采用 ^ 这个符号作为乘方的运算符，即2^4代表2的4次方。

 

来看看他的定义吧，老复杂了！

1、选两个超级大的素数：p 和 q

2、把他们乘起来：n = p * q

3、然后把p-1和q-1也乘起来：m = (p - 1) * (q - 1)

4、再找一个和m互质的数：e -> gcd(e, m) = 1

5、最后，找一个d，满足：(e * d) mod m = 1

6、然后公钥就是(n, e)的组合，私钥就是(n, d)的组合

……

我的妈，这么复杂，人都要疯了，是不是？

为什么要搞这么复杂呢？

 

其实用到了几个原理或者定理或者…… 他们分别是：

1、大数分解难题

2、费马小定理

3、中国剩余定理

4、扩展欧几里德算法

是不是被吓蒙了呢？哈哈哈，由于这一篇是讲Https的，所以就不详细讲这几个原理，简单表述一下（就算是简单，也要说很多，也要码很多的字……）。

 

首先，RSA建立的一个基本原则就是大数分解，如果没有这个原则，就扯淡了。

我们给两个素数，比如 5 和 11。我们能很容易求出他们的乘积：5 * 11 = 55

当然，在这个规模下，我们也很容易将 55 分解成 5 * 11。

但是，如果这两个素数很大呢，比如10的几百次方。我们还是很容易求出他们的乘积。不过，你再想分解他，就不容易咯~

 

其次，著名的费马同学，发明了很多很多定理，其中比较著名的就是费马大小定理，小定理是这么说的：对于一个素数n和任意的正整数a，( a ^ ( n - 1 ) ) modn = 1

我们来试试，比如 n = 5，a= 4， 那么 a ^ ( n - 1 ) = 4^ ( 5 - 1 ) = 256，256 mod 5 = 1

很神奇吧！网上可以搜一搜详细的证明。

这个公式演化一下，就可以得到a的n次方和a分别对n取模，结果是一样的：( a ^ n ) ≡ a (mod n)

比如上面的那个例子：( 4 ^ 5 ) mod 5 = 4 ; 4 mod 5 = 4

那就是说如果n是一个素数，任意一个数的i次方模n，都会呈现n-1个数的循环周期（不一定是最短的周期），比如：

4的1，2，3，4，5，6……次方模5的余数：4 1 4 1 4 1……

如果n是两个素数p和q的乘积，那么这个周期会呈现 (p -1) * (q - 1)这么长。也就是说，a和 a的(p - 1)* (q - 1) + 1次方模n，得到的结果是一样的。特别的，如果a < n，那么这个余数是不是就是a了呢？！

哈哈哈，绕了那么大一个弯子，最后的意思就是说，我只要知道 ( a ^ ( (p - 1)* (q - 1) + 1) ) mod n的余数，实际上就是知道了a，对吧！

那我只要变个花样儿就可以。 我先让m = (p- 1) * (q - 1) ，然后找一个和m互质的数e，再求出一个d，让 e * d = k * m + 1。这样，我给一个数字 T (其中T < n)，计算出 C=(T ^ e) mod n， 我拿着C做一个(C ^ d ) mod n，于是乎，神奇的一幕发生了：(C ^ d) mod n = ( (T ^ e) ^ d ) mod n = ( T ^ (e * d) )mod n = ( T ^ (k * m + 1) ) mod n = T mod n，因为 T < n ，所以结果就为T！！ 

举个例子，我们让p = 5, q = 7，推算出来 n = 5 * 7 = 35, m = (5 - 1) * (7 - 1) = 24

我们找一个e，比方是11，那么一定可以找到一个d = 35，e * d = 11 * 35 =385,

(e * d ) mod m = 385 mod 24 = 1

我们选一个数字T = 18， 计算 C = (T ^ e) mod n = 2

然后 计算 (C ^ d) mod n =18 = T

以上就是RSA的原理，是不是要清楚一点了呢？

现在还有一个问题没有解答，就是我们是怎么找出d来的，实际上就是要解一个方程：

(e * x ) mod m = 1 等价于 e * x + m * y = 1，其中x就是d。

这个就是中国剩余定理里面讲到的东东，具体实现的时候，用扩展辗转相除法（这就是扩展欧拉算法）做个迭代就出来了。

补充一点，RSA涉及到指数运算，效率会比较低。实际上有很多优化的方法，比如O(n) -> O(lgn)，先取模再乘，降维到p和q等等，就不在这里细展开了。

 

 

 

**3. 最后简单聊一个密钥交换的DH算法**

 

我们在之前提到，TLS要做的事情就是两个：身份校验 & 加密算法和密钥协商。

身份校验我们前面已经比较详细的讲述过了。加密算法的协商我们之前也在流程中讲述过。剩下关于密钥交换和协商，我们之前轻描淡写的聊了下。下面稍微详细的讲述一下。

在密钥交换的过程中，会用到一个PRF的函数，是”Pseudo-Random Function”这个的简称，中间计算过程比较复杂，有兴趣的同学可以在网上搜索查阅（由于偷懒，我没有详细深入这个函数 ^_^）。

整个密钥的生成大体如下：

 

master_secret = PRF(pre_master_secret, "master secret",ClientHello.random + ServerHello.random)

 

具体代码可以参见：





 

抽象上来讲，就是 server和client根据Hello时的两个随机数 加上 客户端产生的pre_master_secret来产生一个master_secret，最后由这个东东生成需要的MAC（Message Authentication Code）、key等等加密需要东东。

 

那其中就有一个关键问题，客户端的pre_master_secret怎么样告诉服务器的？

 

我们可以用之前讲过的RSA算法，客户端通过服务器公钥将这个值加密后传递给服务器，服务器再去解密。也可以通过一个叫做DH（Diffie-Hellman）的算法。维基百科对这个算法讲的十分详细。

 

我就简单翻译一下：

 

有两个哥们儿，Alice和Bob，他们想交换数据，于是乎也不知道怎么就想出了一个牛逼的算法：

1、取模数 p = 23，底数 g = 5

2、然后Alice想了一个整数a = 6，发送给Bob一个数：A = (g ^ a) mod p = (5 ^ 6) % 23 = 8

3、同理，Bob想了一个整数b = 15，发送给Alice一个数：B = (g ^ b) mod p = (5 ^ 15) % 23 = 19

4、Alice拿着Bob给的B = 19，计算了一个数：s = (B ^ a) mod p = (19 ^ 6) % 23 = 2

5、Bob也用同样的方法，算了一下 : s = (A ^ b) mod p = (8 ^ 15) % 23 = 2

 

就这样，在不泄露a、b的情况下，他们两都得到了一个一样的数。就这样，数据交换了。。。

其实理论基础就是： A ^ b ≡ (g ^ a) ^  b ≡ g ^ (a * b) ≡ (g ^ b) ^ a ≡ B ^ a (modp)

更详细的说明，可以看维基百科的解释。

后来又有一个改进的算法ECDH，这里就不详细讲述了（偷懒了，以后有机会再补~）

 

 

**==== 总结的分割线 ====**

 

好了，断断续续的抽大家睡觉的时间，把这篇文章写完了（总算没有当太监）。个人觉得把HTTPS整个的流程、交互的过程做了一个大体的了解。有一部分做的深入些，还有很多算法看了一个一知半解，越是深入却发现不懂或者不了解的越多。剩余还有几个点由于篇幅和时间的原因没有讲到，比如：如何访篡改、如何防回放攻击、如何做PRF、交互里面有些扩展参数等，后面准备再抽时间把这些补全。

 

由于之前主要是聚焦在常规算法和互联网技术架构方面，对信息相关的科学了解不是那么深入。后面准备把信息论、加解密算法的相关东西系统性的看下，完了之后分享出来。有兴趣的同学可以关注我的微信

![img](https://img-blog.csdn.net/20160314210550795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

作为一个码工，就想安安静静做做技术。春天来了，可以有更多的时间听着优美的轻音乐写更多的东西了。下面这张照片是我2012年4月在山东济南拍的春天，4年时间过去了，又把他翻出来，感受一下当年的春意盎然。Hello, World！

![img](https://img-blog.csdn.net/20160314210621421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



 

**附：部分参考资料**

 

https://en.wikipedia.org/wiki/Transport_Layer_Security

http://www.moserware.com/2009/06/first-few-milliseconds-of-https.html

http://www-brs.ub.ruhr-uni-bochum.de/netahtml/HSS/Diss/MeyerChristopher/diss.pdf

http://grepcode.com/snapshot/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/

http://drops.wooyun.org/tips/11232

http://tools.ietf.org/html/rfc3447#page-43

http://netsecurity.51cto.com/art/201505/476337_all.htm

 

x509:

http://download.oracle.com/technetwork/java/javase/6/docs/zh/api/java/security/cert/X509Certificate.html

 

RSA:

http://blog.csdn.net/starryheavens/article/details/8536238

https://en.wikipedia.org/wiki/RSA_%28cryptosystem%29

 

Diffie-Hellman:

https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange

http://my.oschina.net/u/138297