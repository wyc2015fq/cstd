# HTTPS安全通信：HTTPS与SSL - 零度的博客专栏 - CSDN博客
2016年05月11日 10:59:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：5706
1. HTTPS概念
        1）简介   
        HTTPS（全称：Hypertext Transfer Protocol over Secure Socket Layer），是以安全为目标的[HTTP](http://baike.baidu.com/view/9472.htm)通道，简单讲是HTTP的安全版。即HTTP下加入SSL层，HTTPS的安全基础是SSL，因此加密的详细内容就需要SSL。这个系统的最初研发由网景公司进行，提供了身份验证与加密[通讯](http://baike.baidu.com/view/20429.htm)方法，现在它被广泛用于[万维网](http://baike.baidu.com/view/7833.htm)上安全敏感的通讯，例如交易支付方面。
      2）HTTPS和HTTP的区别
　　a. https协议需要到ca申请证书，一般免费证书很少，需要交费。
　　b. http是[超文本传输协议](http://baike.baidu.com/view/468465.htm)，信息是明文传输；https
 则是具有[安全性](http://baike.baidu.com/view/421194.htm)的ssl加密传输协议。
　　c. http和https使用的是完全不同的连接方式，用的端口也不一样，前者是80，后者是443。
d. http的连接很简单，是无状态的；HTTPS协议是由SSL+HTTP协议构建的可进行加密传输、身份认证的[网络协议](http://baike.baidu.com/view/16603.htm)，比http协议安全。
      3）HTTPS的作用
      它的主要作用可以分为两种：一种是建立一个[信息安全](http://baike.baidu.com/view/17249.htm)通道，来保证数据传输的安全；另一种就是确认网站的真实性。
　　a． 一般意义上的https，就是服务器有一个证书。主要目的是保证服务器就是他声称的服务器，这个跟第一点一样；[服务端](http://baike.baidu.com/view/1087294.htm)和客户端之间的所有[通讯](http://baike.baidu.com/view/20429.htm)，都是加密的。
　　b. 具体讲，是客户端产生一个对称的[密钥](http://baike.baidu.com/view/934.htm)，通过服务器的证书来交换密钥，即一般意义上的握手过程。
　　c. 接下来所有的信息往来就都是加密的。第三方即使截获，也没有任何意义，因为他没有密钥，当然篡改也就没有什么意义了。
　　d．少许对客户端有要求的情况下，会要求客户端也必须有一个证书。
这里客户端证书，其实就类似表示个人信息的时候，除了用户名/密码，还有一个CA 认证过的身份。因为个人证书一般来说是别人无法模拟的，所有这样能够更深的确认自己的身份。目前少数个人银行的专业版是这种做法，具体证书可能是拿U盘（即U盾）作为一个备份的载体。
2.SSL简介
　　1）简介
　　SSL (Secure Socket Layer)为Netscape所研发，用以保障在Internet上数据传输之安全，利用数据加密(Encryption)技术，可确保数据在网络上之传输过程中不会被截取及窃听。它已被广泛地用于Web浏览器与服务器之间的身份认证和加密数据传输。SSL协议位于TCP/IP协议与各种应用层协议之间，为数据通讯提供安全支持。
　　2）SSL提供的服务
　　a.认证用户和服务器，确保数据发送到正确的[客户机](http://baike.baidu.com/view/285153.htm)和服务器
　　b.加密数据以防止数据中途被窃取
　　c.维护数据的完整性，确保数据在传输过程中不被改变。
　　3) SSL协议的握手过程
　　SSL 协议既用到了公钥加密技术又用到了对称加密技术，对称加密技术虽然比公钥加密技术的速度快，可是公钥加密技术提供了更好的身份认证技术。SSL 的握手协议非常有效的让客户和服务器之间完成相互之间的身份认证，其主要过程如下：
　　①客户端的浏览器向服务器传送客户端SSL 协议的版本号，加密算法的种类，产生的随机数，以及其他服务器和客户端之间通讯所需要的各种信息。
　　②服务器向客户端传送SSL 协议的版本号，加密算法的种类，随机数以及其他相关信息，同时服务器还将向客户端传送自己的证书。
　　③客户利用服务器传过来的信息验证服务器的合法性，服务器的合法性包括：证书是否过期，发行服务器证书的CA 是否可靠，发行者证书的公钥能否正确解开服务器证书的“发行者的数字签名”，服务器证书上的域名是否和服务器的实际域名相匹配。如果合法性验证没有通过，通讯将断开；如果合法性验证通过，将继续进行第四步。
　　④用户端随机产生一个用于后面通讯的“对称密码”，然后用服务器的公钥（服务器的公钥从步骤②中的服务器的证书中获得）对其加密，然后传给服务器。
　　⑤服务器用私钥解密“对称密码”(此处的公钥和私钥是相互关联的，公钥加密的数据只能用私钥解密，私钥只在服务器端保留。详细请参看： [http://zh.wikipedia.org/wiki/RSA%E7%AE%97%E6%B3%95](http://zh.wikipedia.org/wiki/RSA%E7%AE%97%E6%B3%95))，然后用其作为服务器和客户端的“通话密码”加解密通讯。同时在SSL 
 通讯过程中还要完成数据通讯的完整性，防止数据通讯中的任何变化。
　　⑥客户端向服务器端发出信息，指明后面的数据通讯将使用的步骤⑤中的主密码为对称密钥，同时通知服务器客户端的握手过程结束。
　　⑦服务器向客户端发出信息，指明后面的数据通讯将使用的步骤⑤中的主密码为对称密钥，同时通知客户端服务器端的握手过程结束。
　　⑧SSL 的握手部分结束，SSL 安全通道的数据通讯开始，客户和服务器开始使用相同的对称密钥进行数据通讯，同时进行通讯完整性的检验。
![](http://pic002.cnblogs.com/images/2012/267603/2012022815142423.gif)
3.配置服务器端证书
　　为了能实施SSL，一个web服务器对每个接受安全连接的外部接口(IP 地址)必须要有相应的证书(Certificate)。关于这个设计的理论是一个服务器必须提供某种合理的保证以证明这个服务器的主人就是你所认为的那个人。这个证书要陈述与这个网站相关联的公司，以及这个网站的所有者或系统管理员的一些基本联系信息。
　　这个证书由所有人以密码方式签字，其他人非常难伪造。对于进行电子商务(e-commerce)的网站，或其他身份认证至关重要的任何商业交易，认证书要向大家所熟知的认证权威(Certificate Authority (CA))如VeriSign或Thawte来购买。这样的证书可用电子技术证明属实。实际上，认证权威单位会担保它发出的认证书的真实性，如果你信任发出认证书的认证权威单位的话，你就可以相信这个认证书是有效的。
　　关于权威证书的申请，请参考：[http://www.cnblogs.com/mikespook/archive/2004/12/22/80591.aspx](http://www.cnblogs.com/mikespook/archive/2004/12/22/80591.aspx)
　　在许多情况下，认证并不是真正使人担忧的事。系统管理员或许只想要保证被服务器传送和接收的数据是秘密的，不会被连接线上的偷窃者盗窃到。庆幸的是，Java提供相对简单的被称为keytool的命令行工具，可以简单地产生“自己签名”的证书。自己签名的证书只是用户产生的证书，没有正式在大家所熟知的认证权威那里注册过，因此不能确保它的真实性。但却能保证数据传输的安全性。
　　用Tomcat来配置SSL主要有下面这么两大步骤：
　　1)生成证书
　　a. 在命令行下执行：
　　%Java_home%\bin\keytool -genkey -alias tomcat -keyalg RSA
　　在此命令中，keytool是JDK自带的产生证书的工具。把RSA运算法则作为主要安全运算法则，这保证了与其它服务器和组件的兼容性。
这个命令会在用户的home directory产生一个叫做" .keystore " 的新文件。在执行后，你首先被要求出示keystore密码。Tomcat使用的默认密码是" changeit "(全都是小写字母)，如果你愿意，你可以指定你自己的密码。你还需要在server.xml配置文件里指定自己的密码，这在以后会有描述。
　　b .你会被要求出示关于这个认证书的一般性信息，如公司，联系人名称，等等。这些信息会显示给那些试图访问你程序里安全网页的用户，以确保这里提供的信息与他们期望的相对应。
　　c.你会被要求出示密钥(key)密码，也就是这个认证书所特有的密码(与其它的储存在同一个keystore文件里的认证书不同)。你必须在这里使用与keystore密码相同的密码。(目前，keytool会提示你按ENTER键会自动帮你做这些)。
　　如果一切顺利，你现在就拥有了一个可以被你的服务器使用的有认证书的keystore文件。
　　2) 配置tomcat
　　第二个大步骤是把secure socket配置在$CATALINA_HOME/conf/server.xml文件里。$CATALINA_HOME代表安装Tomcat的目录。一个例子是SSL连接器的元素被包括在和Tomcat一起安装的缺省server.xml文件里。它看起来象是这样：
$CATALINA_HOME/conf/server.xml
```
< -- Define a SSL Coyote HTTP/1.1 Connector on port 8443 -->
< !--
< Connector
port="8443" minProcessors="5" maxProcessors="75"
enableLookups="true" disableUploadTimeout="true"
acceptCount="100" debug="0" scheme="https" secure="true";
clientAuth="false" sslProtocol="TLS"/>
-->
```
　　Connector元素本身，其默认形式是被注释掉的(commented out)，所以需要把它周围的注释标志删除掉。然后，可以根据需要客户化(自己设置)特定的属性。一般需要增加一下keystoreFile和keystorePass两个属性，指定你存放证书的路径（如：keystoreFile="C:/.keystore"）和刚才设置的密码（如：keystorePass="123456"）。关于其它各种选项的详细信息，可查阅Server
 Configuration Reference。
　　在完成这些配置更改后，必须象重新启动Tomcat，然后你就可以通过SSL访问Tomcat支持的任何web应用程序。只不过指令需要像下面这样：https://localhost:8443
4.客户端代码实现
　　在Java中要访问Https链接时，会用到一个关键类**HttpsURLConnection**；参见如下实现代码：
```
// 创建URL对象
        URL myURL = new URL("https://www.sun.com");
 
        // 创建HttpsURLConnection对象，并设置其SSLSocketFactory对象
        HttpsURLConnection httpsConn = (HttpsURLConnection) myURL.openConnection();
 
        // 取得该连接的输入流，以读取响应内容
        InputStreamReader insr = new InputStreamReader(httpsConn.getInputStream());
 
        // 读取服务器的响应内容并显示
        int respInt = insr.read();
        while (respInt != -1) {
            System.out.print((char) respInt);
            respInt = insr.read();
        }
```
　　在取得connection的时候和正常浏览器访问一样，仍然会**验证服务端的证书是否被信任**（权威机构发行或者被权威机构签名）；如果服务端证书不被信任，则默认的实现就会有问题，一般来说，用**SunJSSE**会抛如下异常信息：
```
javax.net.ssl.SSLHandshakeException:
 sun.security.validator.ValidatorException: PKIX path building
```
```
failed:
 sun.security.provider.certpath.SunCertPathBuilderException: unable to find valid certification path to requested target
```
　　上面提到SunJSSE，JSSE（Java Secure Socket Extension）是实现Internet安全通信的一系列包的集合。它是一个SSL和TLS的纯Java实现，可以透明地提供数据加密、服务器认证、信息完整性等功能，可以使我们像使用普通的套接字一样使用JSSE建立的安全套接字。JSSE是一个开放的标准，不只是Sun公司才能实现一个SunJSSE，事实上其他公司有自己实现的JSSE，然后通过JCA就可以在JVM中使用。
　　关于JSSE的详细信息参考官网Reference：[http://java.sun.com/j2se/1.5.0/docs/guide/security/jsse/JSSERefGuide.html](http://java.sun.com/j2se/1.5.0/docs/guide/security/jsse/JSSERefGuide.html)；
　　以及Java Security Guide：[http://java.sun.com/j2se/1.5.0/docs/guide/security/](http://java.sun.com/j2se/1.5.0/docs/guide/security/)；
　　在深入了解JSSE之前，需要了解一个有关Java安全的概念：客户端的TrustStore文件。客户端的TrustStore文件中保存着被客户端所信任的服务器的证书信息。客户端在进行SSL连接时，JSSE将根据这个文件中的证书决定是否信任服务器端的证书。在SunJSSE中，有一个信任管理器类负责决定是否信任远端的证书，这个类有如下的处理规则：
1)若系统属性**javax.net.sll.trustStore**指定了TrustStore文件，那么信任管理器就去jre安装路径下的lib/security/目录中寻找并使用这个文件来检查证书。
2)若该系统属性没有指定TrustStore文件，它就会去jre安装路径下寻找默认的TrustStore文件，这个文件的相对路径为：lib/security/**jssecacerts**。
3)若jssecacerts不存在，但是cacerts存在（它随J2SDK一起发行，含有数量有限的可信任的基本证书），那么这个默认的TrustStore文件就是lib/security/**cacerts**。
　　那遇到这种情况，怎么处理呢？有以下两种方案：
　　1)按照以上信任管理器的规则，**将服务端的公钥导入到jssecacerts**，或者是在系统属性中设置要加载的trustStore文件的路径；证书导入可以用如下命令：keytool -import -file src_cer_file –keystore dest_cer_store；至于证书可以通过浏览器导出获得；
　　2)、实现自己的证书信任管理器类，比如**MyX509TrustManager**，该类必须实现X509TrustManager接口中的三个method；然后在HttpsURLConnection中加载自定义的类，可以参见如下两个代码片段，其一为自定义证书信任管理器，其二为connect时的代码：
```
package test;
import java.io.FileInputStream;
import java.security.KeyStore;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509TrustManager;
public class MyX509TrustManager implements X509TrustManager {
    /*
     * The default X509TrustManager returned by SunX509.  We'll delegate
     * decisions to it, and fall back to the logic in this class if the
     * default X509TrustManager doesn't trust it.
     */
    X509TrustManager sunJSSEX509TrustManager;
    MyX509TrustManager() throws Exception {
        // create a "default" JSSE X509TrustManager.
        KeyStore ks = KeyStore.getInstance("JKS");
        ks.load(new FileInputStream("trustedCerts"),
            "passphrase".toCharArray());
        TrustManagerFactory tmf =
        TrustManagerFactory.getInstance("SunX509", "SunJSSE");
        tmf.init(ks);
        TrustManager tms [] = tmf.getTrustManagers();
        /*
         * Iterate over the returned trustmanagers, look
         * for an instance of X509TrustManager.  If found,
         * use that as our "default" trust manager.
         */
        for (int i = 0; i < tms.length; i++) {
            if (tms[i] instanceof X509TrustManager) {
                sunJSSEX509TrustManager = (X509TrustManager) tms[i];
                return;
            }
        }
        /*
         * Find some other way to initialize, or else we have to fail the
         * constructor.
         */
        throw new Exception("Couldn't initialize");
    }
    /*
     * Delegate to the default trust manager.
     */
    public void checkClientTrusted(X509Certificate[] chain, String authType)
                throws CertificateException {
        try {
            sunJSSEX509TrustManager.checkClientTrusted(chain, authType);
        } catch (CertificateException excep) {
            // do any special handling here, or rethrow exception.
        }
    }
    /*
     * Delegate to the default trust manager.
     */
    public void checkServerTrusted(X509Certificate[] chain, String authType)
                throws CertificateException {
        try {
            sunJSSEX509TrustManager.checkServerTrusted(chain, authType);
        } catch (CertificateException excep) {
            /*
             * Possibly pop up a dialog box asking whether to trust the
             * cert chain.
             */
        }
    }
    /*
     * Merely pass this through.
     */
    public X509Certificate[] getAcceptedIssuers() {
        return sunJSSEX509TrustManager.getAcceptedIssuers();
    }
}
        // 创建SSLContext对象，并使用我们指定的信任管理器初始化
        TrustManager[] tm = { new MyX509TrustManager() };
        SSLContext sslContext = SSLContext.getInstance("SSL", "SunJSSE");
        sslContext.init(null, tm, new java.security.SecureRandom());
        // 从上述SSLContext对象中得到SSLSocketFactory对象
        SSLSocketFactory ssf = sslContext.getSocketFactory();
        // 创建URL对象
        URL myURL = new URL("https://ebanks.gdb.com.cn/sperbank/perbankLogin.jsp");
        // 创建HttpsURLConnection对象，并设置其SSLSocketFactory对象
        HttpsURLConnection httpsConn = (HttpsURLConnection) myURL.openConnection();
        httpsConn.setSSLSocketFactory(ssf);
        // 取得该连接的输入流，以读取响应内容
        InputStreamReader insr = new InputStreamReader(httpsConn.getInputStream());
        // 读取服务器的响应内容并显示
        int respInt = insr.read();
        while (respInt != -1) {
            System.out.print((char) respInt);
            respInt = insr.read();
        }
```
　　对于以上两种实现方式，各有各的优点，第一种方式**不会破坏JSSE的安全性**，但是要手工导入证书，如果服务器很多，那每台服务器的JRE都必须做相同的操作；第二种方式**灵活性更高**，但是要小心实现，否则可能会留下安全隐患；
