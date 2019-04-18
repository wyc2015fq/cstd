# servlet 2.5的web.xml - z69183787的专栏 - CSDN博客
2014年04月17日 12:53:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6378
17 March 2007
sun从来不把servlet各个版本的web.xml的规格公布一下，即使上网查找，也很难发现。倒是跟随支援的servlet容器会发布一些范例程序，好运的话，里面将会有最新版本的web.xml写法，比如说，tomcat6正式版里面有了servlet 2.5的写法，如下：
```xml
<web-app xmlns="http://java.sun.com/xml/ns/j2ee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee
    http://java.sun.com/xml/ns/j2ee/web-app_2_5.xsd"
    version="2.5">
```
别以为看到这里就结束了，很可惜地告诉你，这段代码是错误的。不信你尝试打开一下这个链接http://java.sun.com/xml/ns/j2ee/web-app_2_5.xsd，你会发现是no page found。
那为什么tomcat6的范例程序能够工作呢，那是因为在tomcat6的lib里面，已经存在这个文件，所以也不需要从网络上面抓取。其实你按照这个web.xml写了servlet 2.5的程序，在tomcat6里面也是可以运行的。
可是当我使用eclipse+xmlbuddy的时候，问题就出来了，因为web-app_2_5.xsd一直不能下载，xmlbuddy一直报错，并且没有语法提示功能。通过搜索，我发现了web-app_2_5.xsd的真实地址其实是http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd，而它的namespace是http://java.sun.com/xml/ns/javaee，于是代码应该改成：
```xml
<web-app xmlns="http://java.sun.com/xml/ns/javaee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee
    http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
    version="2.5">
```
留意一下不同的部分，其实是因为sun把j2ee改名为javaee。
ps，另外附上servlet 2.4的写法
```xml
<web-app xmlns="http://java.sun.com/xml/ns/j2ee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee
    http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd"
    version="2.4">
```
servlet 2.3 新增功能:
2000年10月份出来
Servlet API 2.3中最重大的改变是增加了filters
servlet 2.4 新增功能:
2003年11月份出来
1、web.xml DTD改用了XML Schema;
Servlet 2.3之前的版本使用DTD作为部署描述文件的定义，其web.xml的格式为如下所示：
<?xml version="1.0" encoding="IS0-8859-1"?>
<!DOCTYPE web-app
PUBLIC "-//sunMicrosystems,Inc.//DTD WebApplication 2.3f//EN"
"http://java.sun.com/j2ee/dtds/web-app_2.3.dtd">
<web-app>
.......
</web-app>
Servlet 2.4版首次使用XML Schema定义作为部署描述文件，这样Web容器更容易校验web.xml语法。同时XML Schema提供了更好的扩充性，其web.xml中的格式如下所示：
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.4" xmlns="http://java.sun.com/xml/ns/j2ee"
xmlns:workflow="http://www.workflow.com"
xmins:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee
http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
.........
</web-app>
注意: 改为Schema后主要加强了两项功能:
(1) 元素不依照顺序设定 
(2) 更强大的验证机制
主要体现在: 
a.检查元素的值是否为合法的值
b.检查元素的值是否为合法的文字字符或者数字字符
c.检查Servlet,Filter,EJB-ref等等元素的名称是否唯一
2.新增Filter四种设定：REQUEST、FORWARD、INCLUDE和ERROR。
3.新增Request Listener、Event和Request Attribute Listener、Enent。
4.取消SingleThreadModel接口。当Servlet实现SingleThreadModel接口时，它能确保同时间内，只能有一个thread执行此Servlet。
5.<welcome-file-list>可以为Servlet。
6.ServletRequest接口新增一些方法。
public String getLocalName()
public String getLocalAddr()
public int getLocalPort()
public int getRemotePort()
Servlet 2.5的新特征
2005年9月发布Servlet 2.5
Servlet2.5一些变化的介绍：
1） 基于最新的J2SE 5.0开发的。
2） 支持annotations 。
3） web.xml中的几处配置更加方便。
4） 去除了少数的限制。
5） 优化了一些实例
servlet的各个版本对监听器的变化有：
(1)servlet2.2和jsp1.1
新增Listener:HttpSessionBindingListener
新增Event: HttpSessionBindingEvent
(2)servlet2.3和jsp1.2
新增Listener:ServletContextListener,ServletContextAttributeListener
,HttpSessionListener,HttpSessionActivationListener,HttpSessionAttributeListener
新增Event: ServletContextEvent,ServletContextAttributeEvent,HttpSessionEvent
(3)servlet2.4和jsp2.0
新增Listener:ServletRequestListener,ServletRequestAttribureListener
新增Event: ServletRequestEvent,ServletRequestAttributeEvent
