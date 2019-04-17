# JavaWeb —— Servlet 3.0 异步处理 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40








# 【概述】

在异步处理之前，一个 Servlet 的工作流程是：Servlet 接到请求后，对请求携带的数据进行预处理，接着调用业务接口的方法，以完成业务处理，最后根据处理结果提交响应，至此 Servlet 线程结束。

但在上述过程中，若任何一个任务没有结束，Servlet 的线程就处于堵塞状态，直到业务方法执行完毕，因而对于较大的应用很容易造成程序性能的降低。

Servlet 3.0 根据上述问题提供了解决方案，即异步处理。

# 【原理】

异步处理机制可以将 Servlet 流程调整为以下过程：Servlet 接到请求后，对请求携带的数据进行预处理，接着 Servlet 线程将请求转交给一个异步线程来执行业务处理，线程本身返回至 Web 容器，此时 Servlet 还未生成响应数据，待异步线程处理完业务后，可直接生成响应数据，或将请求继续转发给其他 Servlet。

这样，Servlet 线程就不再是一直处于堵塞状态以等待业务处理，而是启动异步后可以立即返回。

# 【实现】

异步处理机制可应用于 Servlet、Filter 两种组件，默认情况下没有开启，如果想使用，需要将 @WebServlet 或 @WebFilter 注解中提供的 asyncSupported 属性设为 true

```java
@WebServlet(name="text",urlPatterns="/*",asyncSupported=true)
public class Text implements HttpServlet{
   ...
}
```

若选择在 web.xml 文件中对 Servlet 或 Filter 进行异步配置，选用 <servlet>、<filter> 标签下的 <async-supported> 子标签即可

```
<servlet>
  <servlet-name>test</servlet-name>
  <servlet-class>servlet.Test</servlet-class>
  <async-supported>true</async-supported>
</servlet>
```





