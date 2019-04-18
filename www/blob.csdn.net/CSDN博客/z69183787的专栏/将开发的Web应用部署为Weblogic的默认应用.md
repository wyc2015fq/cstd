# 将开发的Web应用部署为Weblogic的默认应用 - z69183787的专栏 - CSDN博客
2013年02月20日 06:38:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5105
环境：Weblogic 10.3, JDeveloper 11.1.1.3.0
用户在请求Weblogic的默认应用时，可以不指定context root。具体配置方法，请参考下文。
1）在ViewController层创建weblogic.xml文件，将Context Root指定为‘/’；
**[html]**[view
 plain](http://blog.csdn.net/luyushuang/article/details/6914911#)[copy](http://blog.csdn.net/luyushuang/article/details/6914911#)[print](http://blog.csdn.net/luyushuang/article/details/6914911#)[?](http://blog.csdn.net/luyushuang/article/details/6914911#)
- <?xmlversion = '1.0'encoding = 'UTF-8'?>
- <weblogic-web-appxmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.bea.com/ns/weblogic/weblogic-web-app http://www.bea.com/ns/weblogic/weblogic-web-app/1.0/weblogic-web-app.xsd"
- xmlns="http://www.bea.com/ns/weblogic/weblogic-web-app">
- <context-root>/</context-root>
- </weblogic-web-app>
2）设置Welcome page
在web.xml中添加如下代码：
```java
<welcome-file-list>
<welcome-file>/untitled1.jsp</welcome-file>
</welcome-file-list>
```
3）**将应用部署到Weblogic中**；在Weblogic控制台中，Admin Console -> Servers -> DefaultServer(admin) -> Protocols -> HTTP -> Default WebApp Context Root:
修改为Web应用的 Java EE Web Context Root，**保存**，不需要重启Weblogic。
4）对不使用JSF Faces的jsp页面，可以直接访问：http://127.0.0.1:7001
![](http://hi.csdn.net/attachment/201110/28/0_1319790922Pqg9.gif)
reference: http://middlewaremagic.com/weblogic/?p=886
