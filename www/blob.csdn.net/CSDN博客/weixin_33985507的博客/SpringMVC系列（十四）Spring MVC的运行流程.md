# SpringMVC系列（十四）Spring MVC的运行流程 - weixin_33985507的博客 - CSDN博客
2017年11月19日 17:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
Spring MVC的运行流程图：
1.首先看能不能发送请求到Spring MVC的DispatcherServlet的url-pattern
2.如果能发送请求，就看在Spring MVC中是否存在对应的映射
不存在映射：就看是否配置了<mvc:default-servlet-handler/>，如果没有配置<mvc:default-servlet-handler/>就在控制台打印错误日志：No mapping found for HTTP request with URI [/xx/xx] in DispatcherServlet，同时转向404页面；如果配置了<mvc:default-servlet-handler/>就转向目标资源
存在映射：由HandlerMapping获取HandlerExectionChain对象，获取HandlerAdaper对象，调用拦截器的Prehandle方法，调用目标handler的目标方法得到ModelAndView对象
3.调用拦截器的postHandle方法
存在异常：由异常解析器HandleExceptionResolver组件处理异常，得到新的ModelAndView对象，由视图解析器ViewReslover组件根据ModelAndView对象得到实际的view渲染视图
不存在异常：由视图解析器ViewReslover组件根据ModelAndView对象得到实际的view渲染视图
4. 调用拦截器的afterCompletion方法
![](https://images2017.cnblogs.com/blog/1227483/201711/1227483-20171119172813968-1329683908.png)
 说明：
- 用户请求首先发送到前端控制器DispatcherServlet，DispatcherServlet根据请求的信息来决定使用哪个页面控制器Controller（也就是我们通常编写的Controller）来处理该请求。找到控制器之后，DispatcherServlet将请求委托给控制器去处理。
- 接下来页面控制器开始处理用户请求，页面控制器会根据请求信息进行处理，调用业务层等等，处理完成之后，会把结果封装成一个ModelAndView返回给DispatcherServlet。
- 前端控制器DispatcherServlet接到页面控制器的返回结果后，根据返回的视图名选择相应的视图模板（ViewReslover），并根据返回的数据进行渲染。
- 最后前端控制器DispatcherServlet将结果返回给用户。
