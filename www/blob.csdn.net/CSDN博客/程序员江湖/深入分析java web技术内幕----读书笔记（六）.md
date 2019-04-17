# 深入分析java web技术内幕----读书笔记（六） - 程序员江湖 - CSDN博客





2017年03月19日 11:16:32[黄小斜](https://me.csdn.net/a724888)阅读数：2473








﻿﻿**这位大侠，这是我的公众号：程序员江湖。**
**分享程序员面试与技术的那些事。 干货满满，关注就送。**
![这里写图片描述](https://img-blog.csdn.net/20180827211353568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

session和cookie都是用来保持用户与后端服务器的交互状态。

cookie大小大小受限，并且占带宽。

session不能在多个服务器间共享。



cookie是保存在客户端的一个数据结构

session是保存在服务端的一个数据结构。

![](https://img-blog.csdn.net/20170320185930033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



服务端：在tomcat中使用addcookie方法添加cookie，真正则是在response中完成创建。

接受客户端cookie：通过request.getcookies()来获得。



request和response

Request 和 Response 对象起到了服务器与客户机之间的信息传递作用。Request 对象用于接收客户端浏览器提交的数据，而 Response 对象的功能则是将服务器端的数据发送到客户端浏览器。



session对cookie的改进：

cookie在每次访问时需要传回，增加传输量，可以设置id作为标志进行传递，即session id是通过cookie传递的，属性值为jsessionid。



有了seesionid，服务端可以创建httpsession对象。第一次触发通过request.getsession（）。以后都通过session id来找到session。



session没过期时，即使容器关闭或重启也会被持久化。

同个浏览器打开同一个应用多次会使用同一个session，不同浏览器打开一个应用会用不同的session。

![](https://img-blog.csdn.net/20170320185933794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170320185938017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





cookie问题：客户端存储限制、cookie管理混乱，cookie不能跨域名，安全问题。

可以用分布式session框架解决。

实现方式：



![](https://img-blog.csdn.net/20170320185942122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1统一使用订阅服务器管理cookie，应用服务器需要哪个cookie由订阅服务器来推送。一般用zookeeper实现

集群的配置管理。





2session将保存到分布式缓存中，让服务器可以共享访问，因为session不可能同步到每个服务器上。



分布式缓存可以用memcache实现。



3cookie太大的办法是压缩cookie，将keyvalue转化为文本。



4多终端session统一：手机和电脑的cookie和session需要统一，即包括数据结构，存储。防止二次登陆等问题。所以需要在服务端统一session，所有终端拿到的session id都一样。



5：热门。电脑打开网页扫码登录

![](https://img-blog.csdn.net/20170320185946877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

服务端不停检测标识，手机登录后产生标识使pc端登录成功。



