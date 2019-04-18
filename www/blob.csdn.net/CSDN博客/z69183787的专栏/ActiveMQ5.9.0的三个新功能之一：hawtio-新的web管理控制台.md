# ActiveMQ5.9.0的三个新功能之一：hawtio-新的web管理控制台 - z69183787的专栏 - CSDN博客
2014年03月06日 13:30:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3281
                
分类： [JMS/MQ](http://blog.csdn.net/KimmKing/article/category/1293901)2013-10-30 18:31
1478人阅读[评论](http://blog.csdn.net/kimmking/article/details/13631853#comments)(1)[收藏]()[举报](http://blog.csdn.net/kimmking/article/details/13631853#report)
hawtio-web management console
简单的说，就是终于有个像样的管理控制台了。
页面基于jquery、angularJS、bootstrap等主流js框架，界面简洁、操作流畅。
集成了JMX、LOG、Health、connect remote server等功能。
主要以JMX为主，原来的create destinations、send、delete等功能都集成到JMX中对应的节点操作中了。
华丽丽的新控制台登陆界面：
![](https://img-blog.csdn.net/20131030181941218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
默认的用户名密码是 admin/admin ; 保存在conf文件夹下的jetty-realm.properties中;
可以替换成自己的用户名密码，也可以使用自己的验证机制替换掉相关的Realm.
![](https://img-blog.csdn.net/20131030182006578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
集成了很多东西，有些是不需要的。
![](https://img-blog.csdn.net/20131030182011656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里看到的是同一个JVM内的ActiveMQ实例的JMX信息。
点击左侧具体的项可以查看相应的属性信息，以及执行相应的JMX操作。
当然，如果是Host、Queue、Topic之类的节点，可以直接看到Create、Send、Remove等功能性操作。
![](https://img-blog.csdn.net/20131030182016562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
JMX功能页面可以看到所有的JMX信息。
![](https://img-blog.csdn.net/20131030183050046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Connect页面可以连接到其他ActiveMQ实例。
![](https://img-blog.csdn.net/20131030183053593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2ltbUtpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Logs页面可以看到所有的日志信息。
﻿﻿
