# HTTPS - 左直拳的马桶_日用桶 - CSDN博客
2017年03月28日 07:53:27[左直拳](https://me.csdn.net/leftfist)阅读数：668标签：[信息安全																[https																[加密](https://so.csdn.net/so/search/s.do?q=加密&t=blog)](https://so.csdn.net/so/search/s.do?q=https&t=blog)](https://so.csdn.net/so/search/s.do?q=信息安全&t=blog)
个人分类：[计算机基础																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2622721)
https（Hyper Text Transfer Protocol over Secure Socket Layer），超文本传输安全协议。我的理解是安全的http。因为http是明文传输，在需要安全的场合下不够安全。HTTPS基于http进行通信，但利用SSL/TLS来加密数据包。
其作用有二：
> 
1、建立一个信息安全通道，来保证数据传输的安全； 
  2、确认网站的真实性
第一点通过加密实现，SSL/TLS协议；第二点通过证书。
利用SSL/TLS协议进行加密传输之前，客户端与服务器之间有一个握手过程。握手的目的是为了协商出一个双方共同使用的秘钥，好将传输内容进行对称加密。而为了得到这个用于对称加密的秘钥，双方协商过程又使用了非对称加密，你来我往，煞费苦心。
所谓对称加密，加密和解密时使用的是同一个秘钥；而非对称加密有公钥和私钥两个秘钥，用公钥加密，则私钥才能解密，反之亦然。非对称加密安全性比对称加密更好，但花费时间长、速度慢，只适合对少量数据进行加密。
因为有公钥、私钥，非对称加密用于握手过程中是最适合不过了。握手过程如下： 
1、客户端向服务器传送客户端SSL 协议的版本号，加密算法的种类，产生的随机数，以及其他各种信息。
2、服务器向客户端传送SSL 协议的版本号，加密算法的种类，随机数以及其他相关信息，同时服务器还将向客户端传送自己的证书。
3、客户端利用服务器传过来的信息验证服务器的合法性，包括：证书是否过期，发行证书的CA 是否可靠，证书是否有效，证书上的域名是否和实际域名相匹配。
4、客户端随机产生一个新的随机数，然后用服务器的公钥（从服务器的证书中获得）对其加密，传给服务器。同时利用此随机数，经过运算，得到一把秘钥。
5、服务器用自己的私钥解密，得到此随机数，经过运算，得到一把与客户端相同的秘钥。
6、握手过程结束，服务器与客户端用这把秘钥进行愉快的加密传输。
![这里写图片描述](https://img-blog.csdn.net/20170328073230883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在有些安全要求更严格的场合，客户端也需要提供证书，比如网银。这时客户端的证书载体可能就是U盾之类。
客户端，也就是浏览器如何校验服务器的证书呢？浏览器有一份权威CA的名单，凡大网站，用的理应是这些权威CA颁发的证书。其次，认证中心所发放的证书均遵循X.509 V3 标准，里面除了服务器的公钥，还有域名之类的东东。假如浏览器觉得证书有问题，反应如下： 
![这里写图片描述](https://img-blog.csdn.net/20170328075242056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170328075259022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
http使用的端口为80，而HTTPS是443。
相关文章： 
[https的证书](http://write.blog.csdn.net/mdeditor#!postId=67613837)
参考资料：
[百度百科](http://baike.baidu.com/item/https)
[也许，这样理解HTTPS更容易](http://showme.codes/2017-02-20/understand-https/)
