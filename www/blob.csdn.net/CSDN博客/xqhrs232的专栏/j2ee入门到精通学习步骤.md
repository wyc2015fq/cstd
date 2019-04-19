# j2ee入门到精通学习步骤 - xqhrs232的专栏 - CSDN博客
2018年10月18日 15:33:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：42
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/miderph84/article/details/4084778](https://blog.csdn.net/miderph84/article/details/4084778)
相关文章
1、J2EE基础总结（1）——J2EE入门----[https://blog.csdn.net/zhangzijiejiayou/article/details/44775541](https://blog.csdn.net/zhangzijiejiayou/article/details/44775541)
一、准备篇 
1 什么是J2EE？它和普通的Java有什么不同？ 
答：J2EE全称为Java2 Platform, Enterprise Edition。 
“J2EE平台本质上是一个分布式的服务器应用程序设计环境——一个Java环境，它提供了： 
·宿主应用的一个运行基础框架环境。 
·一套用来创建应用的Java扩展API。”（引自《J2EE服务器端高级编程》） 
2 J2EE好学吗？ 
答：J2EE是很多技术的集合体，并且还在成长中。 
你会遇到很多专有名词：比如(X)HTML，Servlet/JSP，JDBC，JMS，JNDI，EJB，XML，Web Service……。 
尤其是XML和Web Service正在快速成长。幸运的是，你不需要等到学会所有技术后再开始编程。 
大体上J2EE可以分成3个主要应用：Servlet/JSP，EJB，XML/Web Service 和一些支撑技术例如JDBC和JNDI。 
你可以一个一个的学。 
3 J2EE有什么用？ 
答：用来建设大型的分布式企业级应用程序。或者用更时髦的名词说就是“电子商务”应用程序。 
这些企业可能大到拥有中心数据库服务器，Web服务器集群和遍布全国的办公终端，也可能小到只不过想做一个网站。 
4 学J2EE有前途吗？ 
答：在这一市场目前只有一种技术可以和J2EE竞争，那就是Microsoft的.NET。 
相对来说.NET要“新”一些而J2EE要“老”一些。 
但是.NET只能用于Windows平台（Microsoft声称要开发C#在Linux上的虚拟机但是尚未兑现该诺言）， 
考虑到Linux的发展势头，你可以相信.NET不会一统天下。 
5 据说J2EE的性能不如.NET好，是真的吗？ 
答：在Sun公司提供的样例程序Pet Store上，Microsoft声称不如相同的.NET程序好。 
而Sun公司反驳说这一程序不能真正体现J2EE的性能，并且指责Microsoft在数据库上做了优化。 
作者没有学习过.NET因而不能妄下断言。 
无论如何，大型分布式程序中的性能瓶颈通常首先来自于错误的设计。 
6 听你说了这么多，我想学着玩玩。 
答：除非你想靠它当饭吃或者作为技术储备，否则请不要浪费你的时间。 
Flash要好玩得多。计算机游戏就更加好玩了。 
7 学习J2EE该怎么开始？ 
答：首先，下载一个J2EE服务器。其次，去java.sun.com下载J2EE的API。第三，找一本好的参考书。最后，找一个顺手的IDE。 
J2EE服务器。你可以用Sun的J2EE SDK（免费），或者Weblogic（性能最好，但是太大，而且作者不推荐盗版行为），或者JBoss（免费，就是文档太少），或者JRun（开发版免费，作者用这个）。参考书作者感觉Wrox的《J2EE服务器端高级编程》不错，但是太老（作者手头的是2001年中文版）。你还需要去下载一些最新的技术资料（当然肯定是英文的）。 
IDE如果你的机器配置够好（内存至少512M以上，256M或以下请勿考虑），可以用IBM的WSAD，不然就继续用Eclipse或者其他。 
你也可以经常去水木清华的Java版逛逛，但是在发贴前先看看精华区里有没有你要的答案。 
8 我下了一个J2EE服务器但是不会配置。 
答：请认真阅读随机指导文档，不同的服务器的配置都不一样，作者爱莫能助。 
9 我发现你没有提到Tomcat。 
答：Tomcat只是一个Web服务器，更准确地说主要只是一个Web Container。 
如果你想要学习EJB的话，Tomcat无法满足你的需要。 
二、 Servlet/JSP篇 
10 什么是Servlet？ 
答：一个Servlet是一个Java类。它处理Http(s)请求并作出响应，包括返回一个HTML页面或转交给其他URL处理。 
Servlet必须运行在一个Web Container例如Tomcat中。 
Servlet必须是javax.servlet.http.Http Servlet的子类， 
你可以继承doGet()或者doPost()方法，两者分别对应于Http(s)中的Get请求和Post请求。 
11 我怎么获得Http请求里的参数？ 
答：HttpR
--------------------- 
作者：miderph 
来源：CSDN 
原文：https://blog.csdn.net/miderph84/article/details/4084778 
版权声明：本文为博主原创文章，转载请附上博文链接！
