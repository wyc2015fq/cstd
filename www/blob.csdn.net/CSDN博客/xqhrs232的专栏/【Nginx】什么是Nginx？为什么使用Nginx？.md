# 【Nginx】什么是Nginx？为什么使用Nginx？ - xqhrs232的专栏 - CSDN博客
2018年07月27日 10:45:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：82
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[https://blog.csdn.net/kisscatforever/article/details/73129270](https://blog.csdn.net/kisscatforever/article/details/73129270)
# **一、前言**
      为毛要用nginx服务器代理，不直接用tomcat 7.0，还做多了一次接请求？
      这个是我想问的，公司的新项目是要用Nginx+tomcat7+jdk开发的，用户命名可以直接访问tomcat，为啥还要用Nginx？这货是个啥玩意？
# **二、为什么使用Nginx？**
      在传统的Web项目中，并发量小，用户使用的少。所以在低并发的情况下，用户可以直接访问tomcat服务器，然后tomcat服务器返回消息给用户。比如，我们上传图片：
![这里写图片描述](https://img-blog.csdn.net/20170612174630569)
      当然我们知道，为了解决并发，可以使用负载均衡：也就是我们多增加几个tomcat服务器。当用户访问的时候，请求可以提交到空闲的tomcat服务器上。
![这里写图片描述](https://img-blog.csdn.net/20170612175104670)
      但是这种情况下可能会有一种这样的问题：上传图片操作。我们把图片上传到了tomcat1上了，当我们要访问这个图片的时候，tomcat1正好在工作，所以访问的请求就交给其他的tomcat操作，而tomcat之间的数据没有进行同步，所以就发生了我们要请求的图片找不到。
      为了解决这种情况，我们就想出了分布式。我们专门建立一个图片服务器，用来存储图片。这样当我们都把图片上传的时候，不管是哪个服务器接收到图片，都把图片上传到图片服务器。
      图片服务器上需要安装一个http服务器，可以使用tomcat、apache、nginx。
![这里写图片描述](https://img-blog.csdn.net/20170612175553583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2lzc2NhdGZvcmV2ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      看到这里大家可能会问，既然我们要选择的是http服务器，为什么不继续使用tomcat，而要使用Nginx？
      原因如下：nginx常用做静态内容服务和代理服务器（不是你FQ那个代理），直面外来请求转发给后面的应用服务（tomcat，django什么的），tomcat更多用来做做一个应用容器，让java web app跑在里面的东西，对应同级别的有jboss,jetty等东西。
      借鉴博客：
[nginx和tomcat的区别](http://www.cnblogs.com/flypie/p/5153702.html)
# **三、什么是Nginx？**
      根据前面的对比，我们可以了解到Nginx是一个[http服务器](http://baike.baidu.com/link?url=a2dLY11NbWgcCzbX1s7JDyWLOh_QFjVlC2wys--TLKbZybTCA8oEP7c-5gEDCK35jFmQHG0YFRoAVEI8M7cbFcp74nDVgz1ckZiWAuntvCF_oxMSMDlDIWEGGN-63mTb)。是一个使用c语言开发的高性能的http服务器及反向代理服务器。Nginx是一款高性能的http 服务器/反向代理服务器及电子邮件（IMAP/POP3）代理服务器。由俄罗斯的程序设计师Igor Sysoev所开发，官方测试nginx能够支支撑5万并发链接，并且cpu、内存等资源消耗却非常低，运行非常稳定。
## **3.1 Nginx的应用场景**
      1、 http服务器。Nginx是一个http服务可以独立提供http服务。可以做网页静态服务器。
      2、 虚拟主机。可以实现在一台服务器虚拟出多个网站。例如个人网站使用的虚拟主机。
- 基于端口的，不同的端口
- 基于域名的，不同域名
      3、 反向代理，负载均衡。当网站的访问量达到一定程度后，单台服务器不能满足用户的请求时，需要用多台服务器集群可以使用nginx做反向代理。并且多台服务器可以平均分担负载，不会因为某台服务器负载高宕机而某台服务器闲置的情况。
# **四、小结**
      通过了解Nginx也算是多了一门技术，而且还是挺简单的，使用也比较方便。重点是可以给系统带来很大的性能提升。
      下一篇将讲解如何安装Nginx。
