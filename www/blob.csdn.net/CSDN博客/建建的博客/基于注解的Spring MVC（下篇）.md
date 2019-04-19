# 基于注解的Spring MVC（下篇） - 建建的博客 - CSDN博客
2017年12月21日 09:44:00[纪建](https://me.csdn.net/u013898698)阅读数：69
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
Model
Spring MVC环境搭建、@RequestMapping以及参数绑定，这是Spring MVC中最基础也是最重要的内容，本篇文章继续讲讲Spring MVC中其余的知识点，先从Model开始。
前一篇文章比较详细地解读了数据从页面请求到服务器后台的一些细节，那么下一个要解决的问题就是数据如何从后台再次传回前台，答案就是这里要说的Model，关于Model在写例子之前我特别先说明三点：
1、Model本身是一个接口，其实现类为ExtendedModelMap，除了使用Model之外还可以使用ModelAndView、ModelMap这些，不过要是没有特殊需求，使用Model比较简单，我个人也比较喜欢使用Model
2、Model的生命周期是Request，也就是说要通过Model传值只能使用转发而不能使用重定向
3、为什么要使用Model而不是用Request，最主要的原因就是减少代码的侵入性或者说代码的耦合度也行。因为Model是Spring的组件，Request是J2EE的组件，使用Model而不去使用Request可以减少对J2EE的依赖，也便于调试
OK，接下来看例子，总体的代码还是按照上一篇文章的来，先看后台的代码：
```
@Controller
@RequestMapping(value = "/test")
public class TestController
{
    @RequestMapping
    public String dispatchTest(Test test, Model model)
    {
        model.addAttribute("modelKey", "modelValue");
        return "test";
    }
}
```
就往Model里面塞一个Key-Value，然后转发到test.jsp下，test.jsp页面要取Model的值，可以通过JSTL（EL表达式也可以）获取，反正直接在jsp页面上通过"<% ... %>"写Java脚本是行不通的。test.jsp页面这么写：
```
<%@ page language="java" import="java.util.*" pageEncoding="ISO-8859-1"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
      <head>
        <base href="<%=basePath%>">
        
        <title>test页面</title>
        
        <meta http-equiv="pragma" content="no-cache">
        <meta http-equiv="cache-control" content="no-cache">
        <meta http-equiv="expires" content="0">    
        <meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
        <meta http-equiv="description" content="This is my page">
        <!--
        <link rel="stylesheet" type="text/css" href="styles.css">
        -->
      </head>
  
      <body>
          <c:out value="${modelKey}" />
      </body>
</html>
```
OK，然后访问一下"http://localhost:8080/SpringMVC/test"这个地址，页面上"modelValue"这几个字符就出来了。
之前说过了，Model的生命周期是Request，那么如果页面是重定向到test.jsp上面去，肯定是取不到"modelValue"的，可以自己试一下，因此重定向过去的话，要在后台把数据设置到session中。
test.jsp页面不变，Controller可以这么改：
```
@Controller
@RequestMapping(value = "/test")
public class TestController
{
    @RequestMapping
    public String dispatchTest(Test test, HttpSession session)
    {
        session.setAttribute("modelKey", "modelValue");
        return "redirect:/test.jsp";
        //return "test";
    }
}
```
可以试一下，再访问一下"http://localhost:8080/SpringMVC/test"这个地址，"modelValue"这几个字符在页面上就出来了。
在Spring MVC中，Request、Response、Session、InputStream、OutputStream这些对象是自动注入的，但是就像之前说的，为了减少代码的侵入性与耦合度，能不使用尽量还是不使用这些J2EE的对象的好。
拦截器（Interceptor）
SpringMVC中的拦截器相当于J2EE中的过滤器，是非常重要和相当有用的，它的主要作用就是拦截用户的请求并进行相应的处理的，比如通过它来进行权限验证，或者是来判断用户是否登陆。
在SpringMVC中使用拦截器的方法比较简单，首先实现HandlerInterceptor接口，实现afterCompletion、postHandle、preHandle三个抽象方法，这里定义两个Interceptor：
```
public class TestInterceptor1 implements HandlerInterceptor
{
    public void afterCompletion(HttpServletRequest arg0,
            HttpServletResponse arg1, Object arg2, Exception arg3)
            throws Exception
    {
        System.out.println("TestInterceptor1.afterCompletion()");
    }
    public void postHandle(HttpServletRequest arg0, HttpServletResponse arg1,
            Object arg2, ModelAndView arg3) throws Exception
    {
        System.out.println("TestInterceptor1.postHandle()");
    }
    public boolean preHandle(HttpServletRequest arg0, HttpServletResponse arg1,
            Object arg2) throws Exception
    {
        System.out.println("TestInterceptor1.preHandle()");
        return true;
    }
}
```
```
public class TestInterceptor2 implements HandlerInterceptor
{
    public void afterCompletion(HttpServletRequest arg0,
            HttpServletResponse arg1, Object arg2, Exception arg3)
            throws Exception
    {
        System.out.println("TestInterceptor2.afterCompletion()");
    }
    public void postHandle(HttpServletRequest arg0, HttpServletResponse arg1,
            Object arg2, ModelAndView arg3) throws Exception
    {
        System.out.println("TestInterceptor2.postHandle()");
    }
    public boolean preHandle(HttpServletRequest arg0, HttpServletResponse arg1,
            Object arg2) throws Exception
    {
        System.out.println("TestInterceptor2.preHandle()");
        return true;
    }
}
```
说明一下三个方法的作用：
1、afterCompletion：在整个视图渲染完毕之后执行方法里面的内容，主要用于释放一些资源
2、postHandle：在Controller执行之后，视图渲染之前执行方法里面的内容，也就是说postHandle方法可以对Model进行操作
3、preHandle：在Controller执行之前，执行方法里面的内容，注意该方法是有返回值的，当方法返回false时整个请求就结束了
然后在springmvc-servlet.xml里面增加拦截器的配置：
```
<!-- 配置拦截器 -->
<mvc:interceptors>
   <mvc:interceptor>
       <mvc:mapping path="/test" />
        <bean class="com.xrq.interceptor.TestInterceptor2" />
    </mvc:interceptor>
    <mvc:interceptor>
        <mvc:mapping path="/test" />
        <bean class="com.xrq.interceptor.TestInterceptor1" />
    </mvc:interceptor>
</mvc:interceptors>
```
假如有多个拦截器的话，"<mvc:interceptor>...</mvc:interceptor>"定义的顺序就是拦截器执行的顺序。
下面继续访问"http://localhost:8080/SpringMVC/test"，代码执行的结果是：
```
TestInterceptor2.preHandle()
TestInterceptor1.preHandle()
TestInterceptor1.postHandle()
TestInterceptor2.postHandle()
TestInterceptor1.afterCompletion()
TestInterceptor2.afterCompletion()
```
也许有些朋友对这个执行结果不是很理解，我其实是懂的，但确实一下子也说不清楚。
如果不是很理解的朋友，可以去看一下Java设计模式里面的责任链模式，拦截器的这种调用方法实际上是一种链式的调用法，TestInterceptor2调用TestInterceptor1，TestInterceptor1方法走了才会回到TestInterceptor2的方法里面。
