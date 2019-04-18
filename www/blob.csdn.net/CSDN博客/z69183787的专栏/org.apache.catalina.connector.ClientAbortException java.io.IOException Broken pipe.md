# org.apache.catalina.connector.ClientAbortException: java.io.IOException: Broken pipe - z69183787的专栏 - CSDN博客
2018年12月12日 10:19:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：160
[https://blog.csdn.net/u013059432/article/details/80415533](https://blog.csdn.net/u013059432/article/details/80415533)
完整错误信息：
org.apache.catalina.connector.ClientAbortException: java.io.IOException: Broken pipe
    at org.apache.catalina.connector.OutputBuffer.realWriteBytes(OutputBuffer.java:396)
    at org.apache.tomcat.util.buf.ByteChunk.flushBuffer(ByteChunk.java:426)
    at org.apache.catalina.connector.OutputBuffer.doFlush(OutputBuffer.java:345)
    at org.apache.catalina.connector.OutputBuffer.close(OutputBuffer.java:298)
    at org.apache.catalina.connector.CoyoteOutputStream.close(CoyoteOutputStream.java:151)
    at com.wanyu.frame.base.BaseAdminController.outJson(BaseAdminController.java:204)
    at com.wanyu.frame.base.BaseAdminController.outResultJson(BaseAdminController.java:305)
    at com.wanyu.yuyi.controller.HomeuserController.findList(HomeuserController.java:97)
    at sun.reflect.GeneratedMethodAccessor252.invoke(Unknown Source)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:498)
    at org.springframework.web.method.support.InvocableHandlerMethod.invoke(InvocableHandlerMethod.java:215)
    at org.springframework.web.method.support.InvocableHandlerMethod.invokeForRequest(InvocableHandlerMethod.java:132)
    at org.springframework.web.servlet.mvc.method.annotation.ServletInvocableHandlerMethod.invokeAndHandle(ServletInvocableHandlerMethod.java:104)
    at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.invokeHandleMethod(RequestMappingHandlerAdapter.java:749)
    at org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter.handleInternal(RequestMappingHandlerAdapter.java:690)
    at org.springframework.web.servlet.mvc.method.AbstractHandlerMethodAdapter.handle(AbstractHandlerMethodAdapter.java:83)
    at org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:945)
    at org.springframework.web.servlet.DispatcherServlet.doService(DispatcherServlet.java:876)
    at org.springframework.web.servlet.FrameworkServlet.processRequest(FrameworkServlet.java:961)
    at org.springframework.web.servlet.FrameworkServlet.doGet(FrameworkServlet.java:852)
    at javax.servlet.http.HttpServlet.service(HttpServlet.java:622)
    at org.springframework.web.servlet.FrameworkServlet.service(FrameworkServlet.java:837)
    at javax.servlet.http.HttpServlet.service(HttpServlet.java:729)
    at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:292)
    at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:207)
    at org.apache.tomcat.websocket.server.WsFilter.doFilter(WsFilter.java:52)
    at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:240)
    at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:207)
    at org.springframework.web.filter.CharacterEncodingFilter.doFilterInternal(CharacterEncodingFilter.java:88)
    at org.springframework.web.filter.OncePerRequestFilter.doFilter(OncePerRequestFilter.java:108)
    at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:240)
    at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:207)
    at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:212)
    at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:94)
    at org.apache.catalina.authenticator.AuthenticatorBase.invoke(AuthenticatorBase.java:504)
    at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:141)
    at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:79)
    at org.apache.catalina.valves.AbstractAccessLogValve.invoke(AbstractAccessLogValve.java:620)
    at org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:88)
    at org.apache.catalina.connector.CoyoteAdapter.service(CoyoteAdapter.java:509)
    at org.apache.coyote.http11.AbstractHttp11Processor.process(AbstractHttp11Processor.java:1104)
    at org.apache.coyote.AbstractProtocol$AbstractConnectionHandler.process(AbstractProtocol.java:684)
    at org.apache.tomcat.util.net.NioEndpoint$SocketProcessor.doRun(NioEndpoint.java:1524)
    at org.apache.tomcat.util.net.NioEndpoint$SocketProcessor.run(NioEndpoint.java:1480)
    at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1142)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:617)
    at org.apache.tomcat.util.threads.TaskThread$WrappingRunnable.run(TaskThread.java:61)
    at java.lang.Thread.run(Thread.java:745)
Caused by: java.io.IOException: Broken pipe
    at sun.nio.ch.FileDispatcherImpl.write0(Native Method)
    at sun.nio.ch.SocketDispatcher.write(SocketDispatcher.java:47)
    at sun.nio.ch.IOUtil.writeFromNativeBuffer(IOUtil.java:93)
    at sun.nio.ch.IOUtil.write(IOUtil.java:65)
    at sun.nio.ch.SocketChannelImpl.write(SocketChannelImpl.java:471)
    at org.apache.tomcat.util.net.NioChannel.write(NioChannel.java:124)
    at org.apache.tomcat.util.net.NioBlockingSelector.write(NioBlockingSelector.java:101)
    at org.apache.tomcat.util.net.NioSelectorPool.write(NioSelectorPool.java:172)
    at org.apache.coyote.http11.InternalNioOutputBuffer.writeToSocket(InternalNioOutputBuffer.java:140)
    at org.apache.coyote.http11.InternalNioOutputBuffer.addToBB(InternalNioOutputBuffer.java:198)
    at org.apache.coyote.http11.InternalNioOutputBuffer.access$000(InternalNioOutputBuffer.java:42)
    at org.apache.coyote.http11.InternalNioOutputBuffer$SocketOutputBuffer.doWrite(InternalNioOutputBuffer.java:321)
    at org.apache.coyote.http11.filters.IdentityOutputFilter.doWrite(IdentityOutputFilter.java:84)
    at org.apache.coyote.http11.AbstractOutputBuffer.doWrite(AbstractOutputBuffer.java:256)
    at org.apache.coyote.Response.doWrite(Response.java:491)
    at org.apache.catalina.connector.OutputBuffer.realWriteBytes(OutputBuffer.java:391)
    ... 48 more
环境：Eclipse + Tomcat + Spring + Spring MVC + MyBatis
在Eclipse中以Debug模式启动调试
错误原因：
这个异常是由于以下几个原因造成。
1、客户端再发起请求后没有等服务器端相应完，点击了stop按钮，导致服务器端接收到取消请求（频繁的刷新就会产生此问题）。
   通常情况下是不会有这么无聊的用户，出现这种情况可能是由于用户提交了请求，服务器端相应缓慢，比如业务逻辑有问题等原因，导致页面过了很久也没有刷新出来，用户就有可能取消或重新发起请求。
这种错误是合理范围内的，无法避免的，不必关心它。
2、Tomcat服务器在接受用户请求的时候，有其自身的处理能力，线程、服务器等各个资源限制，超出Tomcat承载范围的请求，就会被tomcat停掉，也可能产生该错误。
3、Linux的线程机制会产生JVM出错的问题，特别是在连接高峰期间经常出现这样的问题，tomcat在linux下也出现类似情况。
提示：
也有可能产生的错误是： 
java.io.IOException: Connection reset by peer
