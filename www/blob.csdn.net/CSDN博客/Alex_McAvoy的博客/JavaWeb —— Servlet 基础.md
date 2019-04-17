# JavaWeb —— Servlet 基础 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52








# 【概述】

Servlet 实质上就是按 Servlet 规范写的 Java 类，是使用 Java Servlet 接口运行在 Web 应用服务器上的 Java 程序，它是作为来自 Web 浏览器或其他 HTTP 客户端的请求和 HTTP 服务器上的数据库或应用程序之间的中间层。

通过使用 Servlet，不但可以处理 HTTP 请求中的业务逻辑，还可以输出 HTML 代码来显示指定页面。

# 【Servlet 容器】

Servlet 的运行需要 Servlet 容器支持，Servlet 容器通过调用 Servlet 对象提供标准 API 接口对请求进行处理。

Servlet 容器是 Web 服务器或应用程序服务器的一部分，用于在发送的请求和响应之上提供网络服务。Servlet 没有 main 方法，不能独立运行，它必须被部署到 Servlet 容器中，由容器来实例化和调用 Servlet 的方法，Servlet 容器在 Servlet 的生命周期内包容和管理 Servlet 。在 JSP 技术推出后，管理和运行 Servlet/JSP 的容器也称为 Web 容器。

![](https://img-blog.csdnimg.cn/20190112172000599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【结构体系】

Servlet 可以使用 **javax.servlet** 和 **javax.servlet.http **包创建，在 Servlet 开发中，任何一个 Servlet 对象都要直接或间接地实现 javax.servlet.Servlet 接口。

![](https://img-blog.csdnimg.cn/20181218210733313.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

Servlet、ServletConfig 是两个主要接口对象，Servlet 接口定义了 Servlet 基本方法，ServletConfig 封装了 Servlet 相关配置信息，GenericServlet 对象是一个抽象类，实现了上述两个接口，但其并没有对 HTTP 请求进行处理，而是由其子类 HttpServlet 进行实现，其为 HTTP 请求中的 POST、GET 等类型提供了具体的操作方法，通常情况下，所编写的 Servlet 对象都继承于 HttpServlet 类。



