# Struts2注解错误之There is no Action mapped for namespace [/]…… - z69183787的专栏 - CSDN博客
2012年11月01日 11:00:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：22772
我用最新版本的Struts2.3.4做注解操作的时候，一直出现这个问题。花了近两个小时才解决，错误原因让人啼笑皆非……
**[plain]**[view
 plain](http://blog.csdn.net/baiyanglu/article/details/7649341#)[copy](http://blog.csdn.net/baiyanglu/article/details/7649341#)
- There is no Action mapped for namespace [/] and action name [test] associated with context path [/test]. - [unknown location]  
-     at com.opensymphony.xwork2.DefaultActionProxy.prepare(DefaultActionProxy.java:185)  
-     at org.apache.struts2.impl.StrutsActionProxy.prepare(StrutsActionProxy.java:63)  
-     at org.apache.struts2.impl.StrutsActionProxyFactory.createActionProxy(StrutsActionProxyFactory.java:39)  
-     at com.opensymphony.xwork2.DefaultActionProxyFactory.createActionProxy(DefaultActionProxyFactory.java:58)  
-     at org.apache.struts2.dispatcher.Dispatcher.serviceAction(Dispatcher.java:501)  
-     at org.apache.struts2.dispatcher.ng.ExecuteOperations.executeAction(ExecuteOperations.java:77)  
-     at org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter.doFilter(StrutsPrepareAndExecuteFilter.java:91)  
-     at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:243)  
-     at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:210)  
-     at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:224)  
-     at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:169)  
-     at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:472)  
-     at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:168)  
-     at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:98)  
-     at org.apache.catalina.valves.AccessLogValve.invoke(AccessLogValve.java:927)  
-     at org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:118)  
-     at org.apache.catalina.connector.CoyoteAdapter.service(CoyoteAdapter.java:407)  
-     at org.apache.coyote.http11.AbstractHttp11Processor.process(AbstractHttp11Processor.java:987)  
-     at org.apache.coyote.AbstractProtocol$AbstractConnectionHandler.process(AbstractProtocol.java:579)  
-     at org.apache.tomcat.util.net.AprEndpoint$SocketProcessor.run(AprEndpoint.java:1805)  
-     at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1110)  
-     at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:603)  
-     at java.lang.Thread.run(Thread.java:722)  
web.xml
**[html]**[view
 plain](http://blog.csdn.net/baiyanglu/article/details/7649341#)[copy](http://blog.csdn.net/baiyanglu/article/details/7649341#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="3.0"xmlns="http://java.sun.com/xml/ns/javaee"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   
-     http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
- <display-name>test-struts</display-name>
- <filter>
- <filter-name>struts2</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts2</filter-name>
- <url-pattern>*.action</url-pattern>
- </filter-mapping>
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
TestAction.java
**[java]**[view
 plain](http://blog.csdn.net/baiyanglu/article/details/7649341#)[copy](http://blog.csdn.net/baiyanglu/article/details/7649341#)
- package com.test.Action;  
- 
- import org.apache.struts2.convention.annotation.Action;  
- import org.apache.struts2.convention.annotation.Result;  
- import org.apache.struts2.convention.annotation.Results;  
- 
- import com.opensymphony.xwork2.ActionSupport;  
- 
- @Results(value = { @Result(name = "success", location = "/result.jsp") })  
- publicclass TestAction extends ActionSupport {  
- 
- private String name;  
- /**
-      * 
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- @Action("test")  
- public String test() {  
-         name = "qqqqqqq";  
- return SUCCESS;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- }  
目录结构：
![](https://img-my.csdn.net/uploads/201206/10/1339268302_7705.png)
乍一看，是没有任何问题的！可是只要一运行就报错！在百思不得其解后，突然发现我这个包名建的有点特别，问题就出在这里！将Action换成action就可以了，以下是Struts2的原文文档：
 First the Convention plugin finds packages named **struts, struts2, action or actions**.
 Any packages that match those names are considered the root packages for the Convention plugin. Next, the plugin looks at all of the classes in those packages as well as sub-packages and determines if the classes implementcom.opensymphony.xwork2.Action or
 if their name ends with Action (i.e. FooAction).
注意我标红的内容，也就是说，如果要用Struts2的注解，你还非得将action处理类放在struts, struts2, action,actions包或者其子包中。以前总是习惯性的将所有的action处理类放在com.xxx.action下，以为仅仅是习惯而已，没曾想Struts2还就是这么规定的。
**碰到了同样的问题，以前确实不知道基于注解的 struts 配置 需要将action类 放在 名字为 action 的package下。**
**所用JAR包：**
**commons-beanutils-1.7.0.jarcommons-codec-1.3.jarcommons-collections-3.2.jarcommons-digester-1.5.jarcommons-fileupload-1.2.1.jarcommons-io-1.4.jarcommons-lang-2.3.jarcommons-logging-1.1.1.jarcommons-validator-1.3.1.jarezmorph-1.0.4.jarfreemarker-2.3.8.jarjsf-api.jarjsf-impl.jarjstl-1.2.jarognl-2.6.11.jarstruts2-convention-plugin-2.2.1.1.jarstruts2-core-2.2.1.1.jarxml-apis-1.0.b2.jarxwork-core-2.2.1.1.jar**
