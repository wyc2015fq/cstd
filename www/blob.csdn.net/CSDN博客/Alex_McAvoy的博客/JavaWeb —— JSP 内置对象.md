# JavaWeb —— JSP 内置对象 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
所属专栏：[JavaWeb 学习笔记](https://blog.csdn.net/column/details/34361.html)









# 【概述】

在 JSP 页面中，可以在 <% %> 中书写 Java 代码，而 JSP 的内置对象，就是在其中书写 Java 代码时，无需声明可以直接使用的对象。

经常使用的，有以下 9 个对象：
- request：实质上是 HttpServletRequest 对象，通过 request 对象提供的相应方法可以处理客户端浏览器提交的 HTTP 请求中的各项参数。
- response：实质上是 HttpServletResponse 对象，在 JSP 页面中，几乎不会调用 Response 对象的方法。
- pageContext：是 PageContext 的一个对象，可以从其中获取其他 8 个内置对象（getXXX() 方法），也可以从中获取当前页面的信息，常用于自定义标签。
- session：实质上是 HttpSession 对象，代表浏览器与服务器的一次会话
- application：应用对象，代表当前 WEB 应用，是 ServletContext 对象，常用于获取当前 Web 应用的初始化参数
- config：当前 JSP 对应的 Servlet 的ServletConfig 对象，但在开发时基本不使用
- out：JspWriter 对象，常用于向浏览器输出信息
- page：指向当前 JSP 对应的 Servlet 对象的引用，但为 Object 类型，只能调用 Object 类的方法，在开发时基本不使用
- exception：异常对象，只能在声明了错误界面时使用（<%@ page isErrorPage="true" %>）

# 【域对象】

域对象是服务器在内存上创建的存储空间，用于在不同的动态资源（Servlet）之间传递、共享数据。

其有 4 个：request、session、application、pageContext

## 1.作用域

域对象对属性的作用域的范围大小：pageContext < request < session < application
- pageContext：属性的作用范围仅限于当前 JSP 页面
- request：属性的作用范围仅限于同一个请求
- session：属性的作用范围仅限于一次会话（浏览器打开到关闭）
- application：属性的作用范围仅限于当前 Web 应用，在整个应用中，所有的 Servlet、JSP 都可以获取到

在一个 Servlet 中，除无法获取 pageContext 对象外，其他对象的属性均可获得

## 2.常用方法

域对象均有如下常用方法：
- Object getAttribute(String name)：从域对象中获取指定属性
- Enumeration getAttributeNames()：从域对象中获取所有属性名组成的 Enumeration 对象
- void setAttribute(String name,Object value)：向域对象中添加 key-value 形式的数据
- void removeAttribute(String name)：从域对象中移除指定属性

```java
public void init(ServletConfig config) throws ServletException{
    ServletContext context=config.getServletContext();
    context.setAttribute("username", "admin");  //向serlvetContext中存入数据    
    Object valueObject=context.getAttribute("username");  //获取servletContext中的数据
    context.removeAttribute("username");  //从servletContext中删除数据
}
```





