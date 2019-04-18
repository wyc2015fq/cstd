# comet4j java服务端推送消息到web页面实例 - z69183787的专栏 - CSDN博客
2014年03月17日 13:01:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6572
    对于页面一直监控,以前都是使用ajax请求即可,但是小并发这做法没多大问题,但是到了大并发就不太合适,如果不想自己写线程来操控就可以偷懒找一些插件,例如comet4j
下面我来演示下如何使用这个插件
      先准备需要的工具:
comet4j-tomcat6.jar(tomcat6的就导入这个)
comet4j-tomcat7.jar(tomcat7的就导入这个)
comet4j.js(页面引入这个js)
具体操作看下面
然后就写个class
**[java]**[view
 plain](http://blog.csdn.net/shadowsick/article/details/9014139#)[copy](http://blog.csdn.net/shadowsick/article/details/9014139#)
- package com.shadow.extras.comet4j;  
- 
- import javax.servlet.ServletContextEvent;  
- import javax.servlet.ServletContextListener;  
- 
- import org.comet4j.core.CometContext;  
- import org.comet4j.core.CometEngine;  
- 
- publicclass TestComet implements ServletContextListener {  
- privatestaticfinal String CHANNEL = "test";  
- privatestaticfinal String CHANNEL2 = "test2";  
- 
- publicvoid contextInitialized(ServletContextEvent arg0) {  
-         CometContext cc = CometContext.getInstance();  
-         cc.registChannel(CHANNEL);// 注册应用的channel
-         cc.registChannel(CHANNEL2);  
- 
-         Thread helloAppModule = new Thread(new HelloAppModule(),  
- "Sender App Module");  
- // 是否启动
-         helloAppModule.setDaemon(true);  
- // 启动线程
-         helloAppModule.start();  
- 
-         Thread helloAppModule2 = new Thread(new HelloAppModule2(),  
- "Sender App Module");  
- // 是否启动
-         helloAppModule2.setDaemon(true);  
- // 启动线程
-         helloAppModule2.start();  
-     }  
- 
- class HelloAppModule2 implements Runnable {  
- publicvoid run() {  
- while (true) {  
- try {  
- // 睡眠时间
-                     Thread.sleep(5000);  
-                 } catch (Exception ex) {  
-                     ex.printStackTrace();  
-                 }  
-                 CometEngine engine = CometContext.getInstance().getEngine();  
- // 获取消息内容
- long l = getFreeMemory();  
- // 开始发送
-                 engine.sendToAll(CHANNEL2, l);  
-             }  
-         }  
-     }  
- 
- class HelloAppModule implements Runnable {  
- publicvoid run() {  
- while (true) {  
- try {  
- // 睡眠时间
-                     Thread.sleep(2000);  
-                 } catch (Exception ex) {  
-                     ex.printStackTrace();  
-                 }  
-                 CometEngine engine = CometContext.getInstance().getEngine();  
- // 获取消息内容
- long l = getFreeMemory();  
- // 开始发送
-                 engine.sendToAll(CHANNEL, l);  
-             }  
-         }  
-     }  
- 
- publicvoid contextDestroyed(ServletContextEvent arg0) {  
- 
-     }  
- 
- publiclong getFreeMemory() {  
- return Runtime.getRuntime().freeMemory() / 1024;  
-     }  
- }  
然后再写个页面
**[html]**[view
 plain](http://blog.csdn.net/shadowsick/article/details/9014139#)[copy](http://blog.csdn.net/shadowsick/article/details/9014139#)
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=utf-8"/>
- <title>Comet4J Hello World</title>
- <scripttype="text/javascript"src="plugin/comet4j/comet4j.js"></script>
- <scripttype="text/javascript">
- function init(){  
-         var kbDom = document.getElementById('kb');  
-         var kbDom2 = document.getElementById('kb2');  
-         JS.Engine.on({  
-                 test : function(aa){//侦听一个channel  
- kbDom.innerHTML = aa;  
-                 },  
-                 test2 : function(bb){  
- kbDom2.innerHTML = bb;  
-                 }  
-         });  
-         JS.Engine.start('comet');  
- }  
- </script>
- </head>
- <bodyonload="init()">
-         剩余内存：<spanid="kb">...</span>KB <br/>
-         剩余内存：<spanid="kb2">...</span>KB  
- </body>
- </html>
接着配置下web.xml就ok了
**[html]**[view
 plain](http://blog.csdn.net/shadowsick/article/details/9014139#)[copy](http://blog.csdn.net/shadowsick/article/details/9014139#)
- <!-- comet4j -->
- <listener>
- <description>Comet4J容器侦听</description>
- <listener-class>org.comet4j.core.CometAppListener</listener-class>
- </listener>
- <servlet>
- <description>Comet连接[默认:org.comet4j.core.CometServlet]</description>
- <display-name>CometServlet</display-name>
- <servlet-name>CometServlet</servlet-name>
- <servlet-class>org.comet4j.core.CometServlet</servlet-class>
- </servlet>
- <servlet-mapping>
- <servlet-name>CometServlet</servlet-name>
- <url-pattern>/comet</url-pattern>
- </servlet-mapping>
- 
- <listener>
- <description>TestComet</description>
- <listener-class>com.shadow.extras.comet4j.TestComet</listener-class>
- </listener>
最后修改下tomcat的server.xml文件
把protocol参数值改成下面的,因为这是基于nio开发的插件
**[html]**[view
 plain](http://blog.csdn.net/shadowsick/article/details/9014139#)[copy](http://blog.csdn.net/shadowsick/article/details/9014139#)
- <ConnectorURIEncoding="UTF-8"connectionTimeout="20000"port="8080"protocol="org.apache.coyote.http11.Http11NioProtocol"redirectPort="8443"/>
测试,很简单就是访问我们刚刚创建的test.html,然后就可以看到内存数值一直自动刷新波动
