# Java深入 - WEB容器监听器详解 ServletContextListener - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月26日 08:59:09[initphp](https://me.csdn.net/initphp)阅读数：11478
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









WEB容器监听器ServletContextListener主要用来监听容器启动和 销毁的时候需要做一些操作，就可以使用这个监听器来做。

ServletContextListener在Spring启动前启动。





我们实现一个简单的监听器，需要继承接口ServletContextListener：



```java
* 一个测试的监听器例子
 * @author zhuli
 * @date 2014-7-26
 */
public class TestContextLister implements ServletContextListener {

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        System.out.println("==============================容器装载");

    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        System.out.println("==============================容器销毁");
    }

}
```



ServletContextListener 实现两个接口，一个是容器启动的时候，一个是容器销毁的时候：



```java
public interface ServletContextListener extends EventListener {
	/**
	 ** Notification that the web application initialization
	 ** process is starting.
	 ** All ServletContextListeners are notified of context
	 ** initialization before any filter or servlet in the web
	 ** application is initialized.
	 */

    public void contextInitialized ( ServletContextEvent sce );

	/**
	 ** Notification that the servlet context is about to be shut down.
	 ** All servlets and filters have been destroy()ed before any
	 ** ServletContextListeners are notified of context
	 ** destruction.
	 */
    public void contextDestroyed ( ServletContextEvent sce );
}
```


在web.xml中的配置：





```
<listener>
		<listener-class>com.xxx.controller.web.TestContextLister</listener-class>
	</listener>
```


容器启动后，会在容器启动的日志中看到：



```java
==============================容器装载
2014-07-26 08:54:01.302:INFO:/:Initializing Spring FrameworkServlet 'apiServlet'
```












