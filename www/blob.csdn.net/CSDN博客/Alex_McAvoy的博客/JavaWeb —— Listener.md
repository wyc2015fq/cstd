# JavaWeb —— Listener - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19








# 【概述】

在 Servlet 中定义了一些事件，且可以针对这些事件编写相关的事件监听器，从而做出相应处理。

监听器的作用是监听 Web 容器的有效期事件，因此它是由容器管理的，利用 Listener 接口监听在容器中的某个执行程序，并且根据其应用程序的需求做出适当的响应。
|Listener 接口|Event 类（JS 中）|
|----|----|
|ServletContextListener|ServletContextEvent|
|ServletContextAttributeListener|ServletContextAttributeEvent|
|HttpSessionListener			HttpSessionActivationListener|HttpSessionEvent|
|HttpSessionAttributeListener			HttpSessionBindingListener|HttpSessionBindingEvent|
|ServletRequestListener|ServletRequestEvent|
|ServletRequestAttributeListener|ServletRequestAttributeEvent|

# 【ServletContext 监听】

ServletContext 监听可以监听 ServletContext 对象的创建、删除以及属性添加、删除和修改操作，该监听器需要用到以下 2 个接口：

## 1.ServletContextListener 接口

该接口存放在 javax.servlet 包内，它主要监听 ServletContext 的创建和删除。
- contextInitialized(ServletContextEvent event)：通知正在收听的对象，应用程序已被加载且初始化
- contextDestroyed(ServletContextEvent event)：通过正在收听的对象，应用程序已被载出

## 2.ServletAttributeListener 接口

该接口存放在 javax.servlet 包内，它主要实现监听 ServletContext 属性的增加、删除和修改。
- attributeAdded(ServletContextAttributeEvent event)：当有对象加入 Application 的范围时，通知正在收听的对象
- attributeReplace(ServletContextAttributeEvent event)：当在 Application 的范围内有对象取代另一个对象时，通知正在收听的对象
- attributeRemoved(ServletContextAttributeEvent event)：当有对象从 Application 的范围移除时，通知正在收听的对象

# 【HttpSession 监听】

HttpSession 监听信息，有 4 个接口可以进行监听。

## 1.HttpSessionListener 接口

该接口存放在 javax.servlet.http 包内，它主要实现监听 HttpSession 会话的创建和销毁。
- sessionCreated(HttpSessionEvent event)：通知正在收听的对象，session 已被加载且初始化
- sessionDestroyed(HttpSessionEvent event)：通知正在收听的对象，session 已被销毁

**注：HttpSessionEvent 类的主要方法是 getSession()，该方法会回传一个 session 对象。**

## 2.HttpSessionActivationListener 接口

该接口存放在 javax.servlet.http 包内，它主要监听 HttpSession 的钝化和活化。
- attributeAdded(HttpSessionBindingEvent event)：当有对象加入 Application 的范围时，通知正在收听的对象
- attributeReplace(HttpSessionBindingEvent event)：当在 Application 的范围内有对象取代另一个对象时，通知正在收听的对象
- attributeRemoved(HttpSessionBindingEvent event)：当有对象从 Application 的范围移除时，通知正在收听的对象

**注：**HttpSessionBindingEvent **类有三个常用方法：getValues()、getName()、getSession()，分别会回传被变动的属性值、被修改的属性名、被修改的 session 对象。**

## 3.HttpBindingListener 接口

该接口存放在 javax.servlet.http 包内，它主要监听 HttpSession 的绑定信息，其是唯一不需要在 web.xml 中设定 Listener 的。
- valueBound(HttpSessionBindingEvent event)：当有对象加入 session 范围时，会自动调用
- valueUNBound(HttpSessionEvent event)：当有对象从 session 范围内移除时，会自动调用

## 4.HttpSessionAttributeListener 接口

该接口存放在 javax.servlet.http 包内，它主要监听 HttpSession 中属性的设置请求。
- sessionDidActivate(HttpSessionEvent event)：通知正在收听的对象，它的 session 已经变为有效状态
- sessionWillPassivate(HttpSessionEvent event)：通知正在收听的对象，它的 session 已经变为无效状态

# 【ServletRequest 监听】

ServletRequest 监听可以监听客户端的请求，一旦能够在监听程序中获取客户端的请求，就可以对请求进行统一处理，该监听器需要用到以下 2 个接口：

## 1.ServletRequestListener 接口

该接口位于 javax.servlet 包中，用于监听 Request 对象的创建和销毁。
- requestInitalized(ServletRequestEvent event)：通知正在收听的对象，ServletRequest 已被加载且初始化
- requestDestroyed(ServletRequestEvent event)：通知正在收听的对象，ServletRequest 已被载出

## 2.ServletRequestAttributeListener 接口

该接口位于 javax.servlet 包中，用于监听 Request 对象的增加、删除和修改。
- attributeAdded(ServletRequesAttributeEvent event)：当有对象加入 request 的范围时，通知正在收听的对象
- attributeReplace(ServletRequesAttributeEvent event)：当在 request 的范围内有对象取代另一个对象时，通知正在收听的对象
- attributeRemoved(ServletRequesAttributeEvent event)：当有对象从 request 的范围移除时，通知正在收听的对象

# 【创建与配置】

## 1.创建

Listener 创建方式有两种，一种是创建一个普通的 Java 类，将这个类继承相应的监听接口，再通过手动配置 web.xml 文件注册过滤器对象，但该方法操作繁琐，并不常用，经常使用的是第二种方法——通过 IDE 集成开发工具进行创建。

以 Eclipse 为例创建监听器：

1）在 Eclipse 包资源管理器中右击，在弹出的快捷菜单中选择 " 新建/Listener "。

![](https://img-blog.csdnimg.cn/20181227213208327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

2）在弹出的对话框中输入所在包及包名，然后单击下一步。

![](https://img-blog.csdnimg.cn/20181227213314539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

3）在配置页面进行勾选，选择相应的监听接口，然后单击完成即可。

![](https://img-blog.csdnimg.cn/20181227213400660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 2.配置

要使 Listener 对象正常运行，需要进行适当的配置，以告知 Web 容器哪一个请求调用哪一个 Listener 对象处理，对 Listener 起到一个注册的作用。

在 web.xml 文件中，通过 <listener> 标签声明一个 Listener 对象，在此标签下包含两个主要子元素：<description> 用于指定 Listener 的名称；<listener-class> 用于指定 Listener 的完整位置。

例如：

```
<listener>
  <description>Test</description>
  <listener-class>listener.Test</listener-class>
</listener>
```





