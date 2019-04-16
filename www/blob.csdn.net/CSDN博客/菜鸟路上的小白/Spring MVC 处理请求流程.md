# Spring MVC 处理请求流程 - 菜鸟路上的小白 - CSDN博客





2017年08月15日 09:06:17[somnus_小凯](https://me.csdn.net/u012486840)阅读数：442








![](https://img-my.csdn.net/uploads/201304/13/1365825529_4693.png)


一、客户端发出一个HTTP请求，WEB应用服务器接收到这个请求，如果匹配DispatcherServlet的请求映射路径（在web.xml中指定），web容器将该请求转交给DispatcherServlet处理。


二、DispatcherServlet接收到这个请求后，根据请求信息及HandlerMapping的配置找到处理请求的处理器（Handler）。


三、DispatcherServlet 根据获得的Handler，选择一个合适的HandlerAdapter，它用统一的适配器接口调用Handler。


四、处理器完成业务逻辑的处理后将返回一个ModelAndView给DispatcherServlet，ModelAndView包含了视图逻辑名和模型数据信息。


五、ModelAndView中包含的是“逻辑视图名”而非真正的视图对象，DispatcherServlet借由ViewResolver完成逻辑视图名到真实视图对象的解析工作。


六、得到真实的视图对象View后，DispatcherServlet就使用这个View对象对ModelAndView中的模型数据进行视图渲染并返回给客户端。



