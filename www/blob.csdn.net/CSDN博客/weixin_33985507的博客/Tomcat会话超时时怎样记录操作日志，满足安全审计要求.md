# Tomcat会话超时时怎样记录操作日志，满足安全审计要求 - weixin_33985507的博客 - CSDN博客
2016年01月25日 21:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6

众所周知。在实际的Web应用程序中，会话管理一般都採用Web容器会话管理功能。
使用Tomcat做Webserver也是如此，并且从安全的角度考虑，尽量避免去更改和干预Web容器的会话管理功能。
Tomcat会话管理功能肯定比我们自己做出来要全面和可靠，况且Tomcat是主流开源社区维护的。有专门的团队来开发和维护。一旦爆出安全漏洞，也能非常快被修复。
在实际开发中，为了满足安全审计的要求。Web应用程序一旦有会话注销。就应该记录操作日志。注销一般分为操作者主动注销、应用程序检測到异常攻击主动注销会话、超时注销会话。
对于主动注销和检測到攻击注销会话，可以非常easy的记录操作日志。达到要求。
可是对于超时会话注销。非常多人都感觉没办法实现，一种比較常见的方法就是自己开发一个心跳程序，通过client页面不断向服务端发送心跳，服务端通过线程来接收心跳和轮训来去超时注销，从而记录操作日志。
这样的尽管可以达到记录操作日志的效果。可是client向服务端多发了非常多请求，一旦系统并发量非常高的时候，服务端业务压力会添加。不是一个好的处理方式。
Webeasy可以配置会话超时时间。就应该可以监听会话的创建和注销吧？答案是肯定的，Tomcat确实可以通过HttpSessionListener来实现。Session创建事件发生在每次一个新的session创建的时候。类似地Session失效事件发生在每次一个Session失效的时候。
这个接口也仅仅包括两个方法。分别相应于Session的创建和失效：
public void sessionCreated(HttpSessionEvent se); 
public void sessionDestroyed(HttpSessionEvent se);
所以我们仅仅要实现HttpSessionListener。在sessionDestroyed方法中能够实现记录操作日志的功能。
详细须要在我们Web应用程序的web.xml文件里来配置下我们实现的监听类，代码例子和web.xml配置例如以下：
1）代码例子
```java
public class SessionManager implements HttpSessionListener
{
    public void sessionCreated(HttpSessionEvent se)
    {
        // ....
    }
    public void sessionDestroyed(HttpSessionEvent se)
    {
        // ......
    }
}
```
2）web.xml配置
```
<session-config>  
<session-timeout>30</session-timeout>  
</session-config> 
<listener>  
<listener-class>com.test.SessionManager</listener-class>  
</listener>
```
