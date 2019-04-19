# EJB和JBOSS的宏观理解 - Big Smile - CSDN博客
2017年03月18日 12:56:19[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1119
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
         小编最近在学习jboss的相关内容，所以在此给大家分享一下JBOSS的宏观和EJB与JBOSS的关系。 在学习这两个东西之前，我们要了解一下什么是中间件。
# 一、中间件(Middleware)
        先来看一张图：
![](https://img-blog.csdn.net/20170318004959045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        中间件，也就是图中的Middleware，他的作用是什么呢？
        简单来说，中间件就是操作系统和应用程序之间的桥梁。没有中间件，你写出来的应用程序(主要是web application 和 J2EE )就无法运行 。
![](https://img-blog.csdn.net/20170318004834727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么中间件的作用我们知道了，什么是中间件的具体应用呢？
EJB就是一种基于J2EE的中间件，JBOSS也是一种J2EE的中间件。
# 二、EJB(Enterprise Java Bean)
        说道EJB，他的全称就是企业级的J2EE组件模型，说白了是什么呢？他就是JAVA的企业级的组件，为了服务集群和企业级开发而生的。
         J2EE 对于这个问题的处理方法是将业务逻辑从客户端软件中抽取出来，封装在一个组 件中。这个组件运行在一个独立的服务器上，客户端软件通过网络调用组件提供的服务以实 现业务逻辑，而客户端软件的功能单纯到只负责发送调用请求和显示处理结果。在J2EE 中， 这个运行在一个独立的服务器上，并封装了业务逻辑的组件就是EJB（Enterprise Java Bean）组件。
![](https://img-blog.csdn.net/20170318005503704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**简单讲：**把你编写的软件中那些需要执行制定的任务的类，不放到客户端软 件上了，而是给他打成包放到一个服务器上了，这就是EJB。
还有一个核心就是分布式的集群管理，如图：
![](https://img-blog.csdn.net/20170318005649892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**EJB实现原理：**就是把原来放到客户端实现的代码放到服务器端，并依靠RMI进行通信。
# 三、JBOSS
        Jboss是什么？他就是一个J2EE的开放源代码的应用服务器，其实整体上说，jboss不只是有应用服务器这一个作用这么简单，他还兼具管理应用的作用。
        他的特点也很显著，他是面向SOA的应用，并且是开源免费的，其运行对于服务器的资源消耗也是很小的，并且是支持热部署和卸载的，兼有集群功能。
**JBOSS  AS(Application Server)**
这个是什么呢？他就是整个JBOSS的核心，应用服务器，也是EAP上游基础，最新版本的这个叫做WildFly，并且和JBOSS一样他具有很多值得期待的功能。
# 四、EJB和JBOSS的关系
        JBoss是一个开源的javaEE服务器，提供JEE的运行环境。EJB是企业级javabean，用来处理复杂的后台逻辑和实现企业级需求，和spring类似。最后来说一下JBOSS和EJB的关系吧，JBOSS 其实就是一个EJB的容器，因为他可以装载EJB的应用，并且可以进行高级管理和高级配置。
![](https://img-blog.csdn.net/20170318010705208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
