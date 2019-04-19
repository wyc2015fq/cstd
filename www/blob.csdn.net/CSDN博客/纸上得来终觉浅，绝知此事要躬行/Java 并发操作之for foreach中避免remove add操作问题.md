# Java 并发操作之for foreach中避免remove/add操作问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月16日 16:28:06[boonya](https://me.csdn.net/boonya)阅读数：25
此问题在阿里编程规范中也有提及，为什么不建议foreach或者for里面执行add/remove原因是遍历的对象长度改变之后引起并发异常，轻则抛异常重则tomcat服务挂掉。这里以一个老项目中定期移除Token为例。
**目录**
[Java并发remove代码](#Java%E5%B9%B6%E5%8F%91remove%E4%BB%A3%E7%A0%81)
[问题代码](#%E9%97%AE%E9%A2%98%E4%BB%A3%E7%A0%81)
[解决方法](#%E8%A7%A3%E5%86%B3%E6%96%B9%E6%B3%95)
[Tomcat异常输出日志](#%E5%BC%82%E5%B8%B8%E8%BE%93%E5%87%BA%E6%97%A5%E5%BF%97)
[Timer的调用](#Timer%E7%9A%84%E8%B0%83%E7%94%A8)
[Tomcat服务重载问题](#Tomcat%E6%9C%8D%E5%8A%A1%E9%87%8D%E8%BD%BD%E9%97%AE%E9%A2%98)
## Java并发remove代码
### 问题代码
```java
/**
	 * 移除失效token
	 */
	@SuppressWarnings("unchecked")
	public void removeInvalidToken(){
		int i = 0;
		Long invalidTime = new Date().getTime() - invalidatInterval;
		HashMap<String, Token> tokenMap = (HashMap<String, Token>) XHTCacheManager.getModeByCache("moblieTokenList");
		if(null != tokenMap){//tokenMap不为空
			for (Entry<String, Token> entry : tokenMap.entrySet()) {
				Token token = entry.getValue();
				Long time = token.getCreateTime().getTime();
				if(time < invalidTime){
					String tokenUUID = entry.getKey();
					tokenMap.remove(tokenUUID);
					i ++;
				}
			}
		}
		System.out.println("removeInvalidToken = "+i);
	}
```
### 解决方法
使用Iterator遍历修改，这个操作跟长度无关了。
```java
public static Object obj=new Object();
/**
	 * 移除失效token
	 */
	@SuppressWarnings("unchecked")
	public void removeInvalidToken(){
		int i = 0;
		Long invalidTime = new Date().getTime() - invalidatInterval;
		HashMap<String, Token> tokenMap = (HashMap<String, Token>) XHTCacheManager.getModeByCache("moblieTokenList");
		if(null != tokenMap){//tokenMap不为空
			/*for (Entry<String, Token> entry : tokenMap.entrySet()) {
				Token token = entry.getValue();
				Long time = token.getCreateTime().getTime();
				if(time < invalidTime){
					String tokenUUID = entry.getKey();
					tokenMap.remove(tokenUUID);
					i ++;
				}
			}*/
			Iterator<Entry<String, Token>> iter=tokenMap.entrySet().iterator();
			while (iter.hasNext()) {
				Entry<String, Token> entry=iter.next();
				Token token = entry.getValue();
				Long time = token.getCreateTime().getTime();
				if(time < invalidTime){
					String tokenUUID = entry.getKey();
					// 防止并发移除问题
					synchronized (obj) {
						tokenMap.remove(tokenUUID);
					}
					i ++;
				}
			}
		}
		System.out.println("removeInvalidToken = "+i);
	}
public void createToken(Token token){
     synchronized(obj){
              tokenMap = (HashMap<String, Token>) XHTCacheManager.getModeByCache("moblieTokenList");
			userMap =  (HashMap<String, String>) XHTCacheManager.getModeByCache("moblieUserList");
			if (null == tokenMap){
				tokenMap = new HashMap<String, Token>();
				XHTCacheManager.addMode("moblieTokenList", tokenMap);
			}
			if (null == userMap){
				userMap = new HashMap<String, String>();
				XHTCacheManager.addMode("moblieUserList", userMap);
			}
     }
    .....
}
```
注：并发场景下remove操作加锁，而不是Iterator加锁，这个是很多网文的谬误，HashMap是非线程安全的它的put和remove都应该加锁处理，可以使用HashTable建议使用ConcurrentHashMap。
## Tomcat异常输出日志
### Timer的调用
Timer整体来说没有Quartz好用，因为是很早的项目代码，最好改为springframework.scheduling.quartz的方式
```
Exception in thread "Timer-3" java.util.ConcurrentModificationException
	at 
java.util.HashMap$HashIterator.nextNode(HashMap.java:1442)
	at java.util.HashMap
$EntryIterator.next(HashMap.java:1476)
	at java.util.HashMap$EntryIterator.next
(HashMap.java:1474)
	at 
com.forestar.webservice.manager.MobileTokenManager.removeInvalidToken
(MobileTokenManager.java:61)
	at 
com.forestar.webservice.manager.MobileTokenTimerManager$1.run
(MobileTokenTimerManager.java:32)
	at java.util.TimerThread.mainLoop(Timer.java:555)
	
at java.util.TimerThread.run(Timer.java:505)
```
### Tomcat服务重载问题
tomcat重载导致第三方库dll或者so二次加载失败
```
四月 16, 2019 11:47:29 上午 net.sf.ehcache.constructs.web.filter.Filter logThrowable
警告: 
Throwable thrown during doFilter on request with URI: /webService/uploadAttach.do and 
Query: token=4d5065ad-6667-4641-88c6-20eed14b4c1c&attachType=1&eventCode=6f95800d-7a2a-
41ad-a29a-bbb1d6fa003dHandler processing failed; nested exception is 
java.lang.UnsatisfiedLinkError: Native Library /usr/lib64/libwebp-imageio.so already 
loaded in another classloader
org.springframework.web.util.NestedServletException: Handler 
processing failed; nested exception is java.lang.UnsatisfiedLinkError: Native Library 
/usr/lib64/libwebp-imageio.so already loaded in another classloader
	at 
org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:839)
	
at org.springframework.web.servlet.DispatcherServlet.doService
(DispatcherServlet.java:719)
	at 
org.springframework.web.servlet.FrameworkServlet.processRequest
(FrameworkServlet.java:644)
	at 
org.springframework.web.servlet.FrameworkServlet.doPost(FrameworkServlet.java:560)
	
at javax.servlet.http.HttpServlet.service(HttpServlet.java:650)
	at 
javax.servlet.http.HttpServlet.service(HttpServlet.java:731)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:303)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.apache.tomcat.websocket.server.WsFilter.doFilter(WsFilter.java:52)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.listener.UserFilter.doFilter(UserFilter.java:42)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.session.RequestFilter.doFilter(RequestFilter.java:44)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.filter.GZIPFilter.doFilter(GZIPFilter.java:22)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.springframework.web.filter.CharacterEncodingFilter.doFilterInternal
(CharacterEncodingFilter.java:88)
	at 
org.springframework.web.filter.OncePerRequestFilter.doFilter
(OncePerRequestFilter.java:76)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.cache.ehcache.PageCacheFilter.doFilter(PageCacheFilter.java:51)
	
at net.sf.ehcache.constructs.web.filter.Filter.doFilter(Filter.java:86)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:220)
	
at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:122)
	
at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:505)
	
at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:170)
	
at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:103)
	
at org.apache.catalina.valves.AccessLogValve.invoke(AccessLogValve.java:957)
	at 
org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:116)
	
at org.apache.catalina.ha.session.JvmRouteBinderValve.invoke
(JvmRouteBinderValve.java:218)
	at org.apache.catalina.ha.tcp.ReplicationValve.invoke
(ReplicationValve.java:333)
	at org.apache.catalina.connector.CoyoteAdapter.service
(CoyoteAdapter.java:423)
	at 
org.apache.coyote.http11.AbstractHttp11Processor.process
(AbstractHttp11Processor.java:1079)
	at org.apache.coyote.AbstractProtocol
$AbstractConnectionHandler.process(AbstractProtocol.java:620)
	at 
org.apache.tomcat.util.net.JIoEndpoint$SocketProcessor.run(JIoEndpoint.java:316)
	
at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149)
	
at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624)
	
at org.apache.tomcat.util.threads.TaskThread$WrappingRunnable.run(TaskThread.java:61)
	
at java.lang.Thread.run(Thread.java:748)
Caused by: java.lang.UnsatisfiedLinkError: Native 
Library /usr/lib64/libwebp-imageio.so already loaded in another classloader
	at 
java.lang.ClassLoader.loadLibrary0(ClassLoader.java:1907)
	at 
java.lang.ClassLoader.loadLibrary(ClassLoader.java:1857)
	at 
java.lang.Runtime.loadLibrary0(Runtime.java:870)
	at java.lang.System.loadLibrary
(System.java:1122)
	at com.luciad.imageio.webp.WebP.loadNativeLibrary(WebP.java:29)
	
at com.luciad.imageio.webp.WebP.<clinit>(WebP.java:34)
	at 
com.luciad.imageio.webp.WebPReadParam.<clinit>(WebPReadParam.java:22)
	at 
com.forestar.webservice.MobileController.uploadAttach(MobileController.java:1106)
	
at sun.reflect.GeneratedMethodAccessor84.invoke(Unknown Source)
	at 
sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	
at java.lang.reflect.Method.invoke(Method.java:498)
	at 
org.springframework.web.bind.annotation.support.HandlerMethodInvoker.invokeHandlerMethod
(HandlerMethodInvoker.java:176)
	at 
org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter.invokeHandl
erMethod(AnnotationMethodHandlerAdapter.java:426)
	at 
org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter.handle
(AnnotationMethodHandlerAdapter.java:414)
	at 
org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:790)
	
... 44 more
四月 16, 2019 11:47:29 上午 net.sf.ehcache.constructs.web.filter.Filter logThrowable
警告: 
Throwable thrown during doFilter on request with URI: /webService/uploadAttach.do and 
Query: token=4d5065ad-6667-4641-88c6-20eed14b4c1c&attachType=1&eventCode=6f95800d-7a2a-
41ad-a29a-bbb1d6fa003dHandler processing failed; nested exception is 
java.lang.NoClassDefFoundError: Could not initialize class 
com.luciad.imageio.webp.WebPReadParam
org.springframework.web.util.NestedServletException: 
Handler processing failed; nested exception is java.lang.NoClassDefFoundError: Could not 
initialize class com.luciad.imageio.webp.WebPReadParam
	at 
org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:839)
	
at org.springframework.web.servlet.DispatcherServlet.doService
(DispatcherServlet.java:719)
	at 
org.springframework.web.servlet.FrameworkServlet.processRequest
(FrameworkServlet.java:644)
	at 
org.springframework.web.servlet.FrameworkServlet.doPost(FrameworkServlet.java:560)
	
at javax.servlet.http.HttpServlet.service(HttpServlet.java:650)
	at 
javax.servlet.http.HttpServlet.service(HttpServlet.java:731)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:303)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.apache.tomcat.websocket.server.WsFilter.doFilter(WsFilter.java:52)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.listener.UserFilter.doFilter(UserFilter.java:42)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.session.RequestFilter.doFilter(RequestFilter.java:44)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.filter.GZIPFilter.doFilter(GZIPFilter.java:22)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.springframework.web.filter.CharacterEncodingFilter.doFilterInternal
(CharacterEncodingFilter.java:88)
	at 
org.springframework.web.filter.OncePerRequestFilter.doFilter
(OncePerRequestFilter.java:76)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at com.forestar.core.cache.ehcache.PageCacheFilter.doFilter(PageCacheFilter.java:51)
	
at net.sf.ehcache.constructs.web.filter.Filter.doFilter(Filter.java:86)
	at 
org.apache.catalina.core.ApplicationFilterChain.internalDoFilter
(ApplicationFilterChain.java:241)
	at 
org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:208)
	
at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:220)
	
at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:122)
	
at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:505)
	
at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:170)
	
at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:103)
	
at org.apache.catalina.valves.AccessLogValve.invoke(AccessLogValve.java:957)
	at 
org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:116)
	
at org.apache.catalina.ha.session.JvmRouteBinderValve.invoke
(JvmRouteBinderValve.java:218)
	at org.apache.catalina.ha.tcp.ReplicationValve.invoke
(ReplicationValve.java:333)
	at org.apache.catalina.connector.CoyoteAdapter.service
(CoyoteAdapter.java:423)
	at 
org.apache.coyote.http11.AbstractHttp11Processor.process
(AbstractHttp11Processor.java:1079)
	at org.apache.coyote.AbstractProtocol
$AbstractConnectionHandler.process(AbstractProtocol.java:620)
	at 
org.apache.tomcat.util.net.JIoEndpoint$SocketProcessor.run(JIoEndpoint.java:316)
	
at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149)
	
at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624)
	
at org.apache.tomcat.util.threads.TaskThread$WrappingRunnable.run(TaskThread.java:61)
	
at java.lang.Thread.run(Thread.java:748)
Caused by: java.lang.NoClassDefFoundError: Could 
not initialize class com.luciad.imageio.webp.WebPReadParam
	at 
com.forestar.webservice.MobileController.uploadAttach(MobileController.java:1106)
	
at sun.reflect.GeneratedMethodAccessor84.invoke(Unknown Source)
	at 
sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	
at java.lang.reflect.Method.invoke(Method.java:498)
	at 
org.springframework.web.bind.annotation.support.HandlerMethodInvoker.invokeHandlerMethod
(HandlerMethodInvoker.java:176)
	at 
org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter.invokeHandl
erMethod(AnnotationMethodHandlerAdapter.java:426)
	at 
org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter.handle
(AnnotationMethodHandlerAdapter.java:414)
	at 
org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:790)
	
... 44 more
```
