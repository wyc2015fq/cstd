# EJB客户端运行错误：端口冲突和缺少jar包 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月24日 16:43:15[boonya](https://me.csdn.net/boonya)阅读数：1481








**1、启动Jboss报错**

错误描述：



```java
java.lang.Exception: Socket bind failed: [730013] ÒÔһÖַÃÎÊȨÏ޲»ÔÊÐíµķ½ʽ×öÁËһ¸ö·ÃÎÊÌ׽Ó×ֵ
.....................
LifecycleException:  service.getName(): "jboss.web";  Protocol handler start failed: java.lang.Exception: Socket bind failed: [730013] ÒÔһÖַÃÎÊȨÏ޲»ÔÊÐíµķ½ʽ×öÁËһ¸ö·ÃÎÊÌ׽Ó×ֵ
```
详细信息如图所示：

![](https://img-blog.csdn.net/20140624163957078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20140624164028984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


原因是Jboss默认的端口是8080，所以此端口可能被其它进程所占用，需要修改端口。

解决方法：需要修改{JBOSS_HOME}/server/default/depoly/jbossweb.sar/server.xml文件的端口。

![](https://img-blog.csdn.net/20140624163716328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**2、缺少jar包允许报错**

错误提示代码：



```java
javax.naming.NoInitialContextException: Cannot instantiate class: org.jnp.interfaces.NamingContextFactory
```


解决方法：需要加入{JBOSS_HOME}/client/jbossall-client.jar到Eclipse的编译路径下，如图：

![](https://img-blog.csdn.net/20140624163604515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




