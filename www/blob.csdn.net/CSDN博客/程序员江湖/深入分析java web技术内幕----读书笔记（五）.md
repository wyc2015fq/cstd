# 深入分析java web技术内幕----读书笔记（五） - 程序员江湖 - CSDN博客





2017年03月18日 13:09:58[黄小斜](https://me.csdn.net/a724888)阅读数：888








# **这位大侠，这是我的公众号：程序员江湖。****分享程序员面试与技术的那些事。 干货满满，关注就送。**![这里写图片描述](https://img-blog.csdn.net/20180827211353568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# **servlet+tomcat原理剖析**



servlet工作原理分析



servlet是运行在服务器上的java程序。

1ContextConfig负责web应用的配置文件的解析

2servlet被包装为wrapper对象，被添加到context中作为子容器，而web.xml被解析到context中，所以context才是真正的servlet容器，一个web应用对应一个context容器。



3

![](https://img-blog.csdn.net/20170320185431964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170320185441917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

到达wrapper后还可能经过listener和filter。

listener涵盖servlet生命周期的每个事件。

filter用来转发请求和处理请求，传递filterchain对象，使用责任链模式，有init，dofilter，destroy这样的生命周期。

url-pattren的解析由mapper类来定义和完成。会分配到指定的servlet或者filter。





第十一章：

tomcat结构



核心是connector和container



![](https://img-blog.csdn.net/20170320185445963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1container只有一个，connector有多个，多个connector和一个container形成一个service，service由server管理，tomcat的生命周期由server控制。



2service可以为外部提供服务。其中connector负责创建 request和response用来接受请求和交换数据。



3container通过多线程处理多个connector的请求，是设计的核心，connector可分为connetor、processer、protocal。



其中connector封装socket成为request和response，并交给container进行处理。



4container包含engine host context wrapper，四个组件是父子关系。



5、

![](https://img-blog.csdn.net/20170320185449135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170320185452307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

engine没有父容器



![](https://img-blog.csdn.net/20170320185455495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

一个context对应一个应用，但需要war时就要有host容器，因为要靠host解析web.xml



![](https://img-blog.csdn.net/20170320185458198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170320185501323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170320185504245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用wrapper容器可以实现servlet和tomcat容器的脱耦，在此容器中会调用servlet的init方法、然后调用filter方法，接着执行service。





设计模式：门面设计模式：

![](https://img-blog.csdn.net/20170320185507167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要相互通信，但又不想暴露其他接口和数据时使用。







观察者模式：lifecycle和listener监听着servlet。







命令模式：connector封装请求作为命令传给container



责任链模式：从engine到host到context再到wrapper，体现责任链。



