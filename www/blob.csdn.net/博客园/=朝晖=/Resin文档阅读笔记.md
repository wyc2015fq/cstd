# Resin文档阅读笔记 - =朝晖= - 博客园
# [Resin文档阅读笔记](https://www.cnblogs.com/dhcn/p/7100204.html)
阅读文档对应的版本为Resin4.0，且基本只关注Standard版本的功能。
1、Resin可以注册为服务:
> 
To install the service, use
C:/> resin-3.2.x/resin.exe -install -conf conf/myconf.xml /
                              -user MyResinUser -password mypassword
To remove the service, use
C:/> resin-3.2.x/resin.exe -remove
2、Resin Logs目录里面有访问服务器访问日志在./logs子目录内。
3、Resin服务器本身提供一套类似[spring](http://lib.csdn.net/base/javaee) Security的可配置权限控制解决方案.详见[http://www.caucho.com/resin/admin/security.xtp](http://www.caucho.com/resin/admin/security.xtp)
4、显式定义Web应用目录的方法：
```
<host id="">  
     <web-app id='/' document-directory="/usr/local/apache/htdocs"/>  
   </host>
```
5、Resin本身提供依赖注入：[Java](http://lib.csdn.net/base/java)Injection (CanDI, JSR-299) Injection :[http://www.caucho.com/resin/admin/candi.xtp](http://www.caucho.com/resin/admin/candi.xtp) ,注意这句：CanDI provides a straightforward driver for integrating Resin capabilities with popular frameworks. [Struts2](http://wiki.caucho.com/Struts2), [Spring](http://wiki.caucho.com/Spring), [Mule](http://wiki.caucho.com/Mule), and [Wicket](http://wiki.caucho.com/Wicket) have already been integrate。
6、在database configuration部分，注意现在的配置有Protecting the database password的方法。
7、关于Server Push (Comet):Resin's Comet support focuses on solving the concurrency issues around server push.
8、Resin内建Rewrite and Dispatch机制
9、Resin's remoting lets applications write services as plain [java](http://lib.csdn.net/base/java)objects and export them with a choice of protocols, including Hessian, Burlap, CXF (SOAP), XFire. Because Resin activates the service as an IoC singleton, the service can use any of [Resin's IoC capabilities](http://www.caucho.com/resin/doc/resin-ioc.xtp), including dependency injection, AOP interception, EJB transactions, and event handling.
10、virtual host的设置样例：
```
<resin xmlns="http://caucho.com/ns/resin">  
<cluster id="">  
<host host-name="www.foo.com">  
  <host-alias>foo.com</host-alias>  
  <host-alias>web.foo.com</host-alias>  
  <root-directory>/opt/www/www.foo.com</root-directory>  
  <web-app id="/" document-directory="webapps/ROOT">  
      
  </web-app>  
  ...  
</host>  
</cluster>  
</resin>
```

