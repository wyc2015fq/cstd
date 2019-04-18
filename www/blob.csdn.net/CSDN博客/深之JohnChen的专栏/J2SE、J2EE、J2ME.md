# J2SE、J2EE、J2ME - 深之JohnChen的专栏 - CSDN博客

2009年04月09日 12:42:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1623


**J2SE**

Java2平台包括：标准版（J2SE）、企业版（J2EE）和微缩版（J2ME）三个版本。J2SE,J2ME和J2EE,这也就是SunONE(Open NetEnvironment)体系。J2SE就是Java2的标准版，主要用于桌面应用软件的编程；J2ME主要应用于嵌入是系统开发，如手机和PDA的编程；J2EE是Java2的企业版，主要用于分布式的网络程序的开发，如电子商务网站和ERP系统。

　　Standard Edition(标准版) J2SE 包含那些构成Java语言核心的类。比如：数据库连接、接口定义、输入/输出、网络编程

　　Enterprise Edition(企业版) J2EE 包含J2SE 中的类，并且还包含用于开发企业级应用的类。比如：EJB、servlet、JSP、XML、事务控制

　　Micro Edition(微缩版) J2ME 包含J2SE中一部分类，用于消费类电子产品的软件开发。比如：呼机、智能卡、手机、PDA、机顶盒

　　简单讲就是：

　　j2se java 2 simple edition 小型程序用

　　j2ee java 2 enterprise edition 大型程序用

　　j2me java 2 micro edition 手机上用

　　他们的范围是：J2SE包含于J2EE中，J2ME包含了J2SE的核心类，但新添加了一些专有类

　　应用场合，API的覆盖范围各不相同。

　　笼统的讲，可以这样理解：J2SE是基础；压缩一点，再增加一些CLDC等方面的特性就是J2ME；扩充一点，再增加一些EJB等企业应用方面的特性就是J2EE。

　　补充一点J2EE更恰当的说，应该是JAVA2企业开发的技术规范，不仅仅是比标准版多了一些类。J2EE又包括许多组件，如Jsp, Servlet, JavaBean, EJB, JDBC, JavaMail等。

　　J2SE商业版本，标准版本 (Java2 Standard Edition) 定位在客户端，主要用于桌面应用软件的编程。

　　J2SE 包含那些构成Java语言核心的类。

　　比如：数据库连接、接口定义、输入/输出、网络编程

　　Java 代码签名证书主要用于证明软件开发者开发的软件代码符合 Sun Java 规范。您可以使用Java 代码签名证书来为您的 Java 代码进行数字签名，以便通过互联网安全分发，使得最终用户能确信您的代码已经通过权威的第三方认证，而且没有在传输过程中被非法篡改和被破坏。

　　Java 代码签名证书可以用于数字签名运行J2ME MIDlet(MIDP 2.0)代码，支持诺基亚和索爱等手机的MIDlet应用软件，运行签名后的MIDlet Suite就会显示软件发行商名称，否则会显示让用户不敢下载的警告“应用软件来源未知，继续？”。同时，签名后访问网络等都不会弹出烦人的警告框了，让手机应用更加安全，更有利于软件开发商推广其应用软件。如果需要签名J2SE的.Jar代码，则推荐您购买Thawte Java代码签名证书，因为其价格比VeriSign的便宜。 如果您希望有比Thawte Java代码签名证书更便宜的Java代码签名证书，则您可以选购 WoSign Java代码签名证书，但请注意，WoSign Java代码签名证书只支持Java J2SE 1.5.008版本或以上版本。

　　J2SE 是J2EE的基础，他大量的JDK代码库是每个要学习J2EE的编程人员必须掌握的。

　J2SE就是Java2的标准版，主要用于桌面应用软件的编程；J2ME主要应用于嵌入是系统开发，如手机和PDA的编程；J2EE是Java2的企业版，主要用于分布式的网络程序的开发，如电子商务网站和ERP系统。

　　Standard Edition(标准版) J2SE 包含那些构成Java语言核心的类。比如：数据库连接、接口定义、输入/输出、网络编程

　　Enterprise Edition(企业版) J2EE 包含J2SE 中的类，并且还包含用于开发企业级应用的类。比如：EJB、servlet、JSP、XML、事务控制

　　Micro Edition(微缩版) J2ME 包含J2SE中一部分类，用于消费类电子产品的软件开发。比如：呼机、智能卡、手机、PDA、机顶盒
　从JDK 5.0开始，不再叫J2SE,改名为 Java SE了，因为那个2已经失去了其应该有的意义。
- 参考资料： 
1.[http://www.j2sechina.cn](http://www.j2sechina.cn/) j2se中国 2.[http://www.jsjjiaoxue.cn](http://www.jsjjiaoxue.cn/) JAVA视频资料下载 3.[http://www.java.com/zh_CN/](http://www.java.com/zh_CN/) Java中国官方网站 4.[http://www.sun.com/java/](http://www.sun.com/java/) Sun Java官方网站 5.[http://www.java2000.net/f47](http://www.java2000.net/f47)6.[http://www.java2000.net/f22](http://www.java2000.net/f22)

