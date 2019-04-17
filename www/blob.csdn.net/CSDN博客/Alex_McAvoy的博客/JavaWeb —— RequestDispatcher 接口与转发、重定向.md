# JavaWeb —— RequestDispatcher 接口与转发、重定向 - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:42:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59








# 【概述】

RequestDispatcher 对象是由 Servlet 引擎创建的，用于封装一个要被其他资源调用的资源，如：Servlet、HTML、JSP 等，并可通过其中的方法将客户端的请求转发给所包装的资源。

该接口的方法常用于请求的转发，而 ServletResponse 接口中的 sendReadirect() 方法常用于请求的重定向。

# 【获取接口对象】

通过 ServletContext 对象：
- RequestDispatcher getRequestDispatcher(String path)：根据以 " / " 开头的路径获取 RequestDispatcher 对象
- String getRequestDispatcher(String path)：根据以 " / " 开头的路径获取 RequestDispatcher 对象名

通过 ServletRequest 对象：
- RequestDispatcher getRequestDispatcher(String path)：根据路径来获取 RequestDispatcher 对象（可不需要 " / "）

# 【接口方法】

RequestDispatcher 接口中定义了两个方法，用于请求转发：
- **void forward(ServletRequest req,ServletResponse res)：**将请求转发给服务器上另外一个 Servlet、HTML、JSP
- **void include(ServletRequest req,ServletResponse res)：**用于在响应中包含其他资源的内容，即请求转发后，原来的 Servlet 还可继续输出响应信息，转发到的 Servlet 对请求做出的响应将并入原先 Servlet 的响应对象中

上述两个方法接收的两个参数一般是传递给当前 Servlet 的 service() 方法的 ServletRequest、ServletResponse 对象，但也可为对它们进行了包装的 ServletRequestWrapper 或 ServletResponseWrapper 对象

两方法的区别：forward() 方法调用后在响应中的没有提交的内容被自动消除，include() 方法使原先的 Servlet 和转发到的 Servlet 都可以输出响应信息。   

# 【请求转发与重定向】

RequestDispatcher 接口中的 forward() 方法常用于请求的转发，而 ServletResponse 接口中的 sendReadirect() 方法常用于请求的重定向。

在进行开发时，当需要用到转发、重定向时，建议使用绝对路径而不要使用相对路径，即当 " /" 代表站点的根目录时， 在其前面加上 contextPath 即可，而 contextPath 可有 request 或 application 的 getContextPath() 方法获取。

二个方法都是用于转发给另外的资源为客户端服务，但二者有着本质的区别。

## 1.不同

1）forward() 转发：
- 转发的地址栏是初次发出请求的地址
- 转发只能在当前 WEB 应用内部转发
- 在最终的 Servlet 中，request 对象和中转的 request 是同一对象，原有 request 对象属性不变
- 转发过程中，在方法中使用的 " / " 代表当前 WEB 应用的根目录 

2）sendReadirect() 重定向：
- 重定向的地址栏不是初次发出的请求地址，而是最后响应的地址
- 重定向可以跨 WEB 应用、服务器重新定位资源
- 在最终的 Servlet 中，request 对象和中转的 request 不是同一对象，原有 request 对象属性全部失效，开始新的 request 对象
- 重定向过程中，在方法中使用的 " / " 代表当前 WEB 站点的根目录 

注：关于 WEB 应用与 WEB 站点的根目录

以 http://localhost:8080/test/ 为例，test/ 为应用的根目录，http://localhost:8080/ 为站点的根目录

## 2.原理

### 1）forward() 方法
- 客户端发送请求，Servlet1 做出处理
- Servlet1 调用 forward() 方法，将请求**转发**给 Servlet2 来处理请求，为客户端服务
- Servlet2 对客户端浏览器做出响应

### 2）sendReadirect() 方法   
- 客户端发送请求，Servlet1 做出处理
- Servlet1 调用 sendReadirect() 方法，将客户端的请求**重新定位**到 Servlet2
- 客户端浏览器访问 Servlet2
- Servlet2 对客户端浏览器做出响应

## 3.实例

```java
//请求转发
protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {		
    String path="testServlet";//要转发的地址
    RequestDispatcher requestDispatcher=request.getRequestDispatcher("/"+path);//获取requestDispatcher对象并填入地址
    requestDispatcher.forward(request, response);//请求转发
}

//请求重定向
protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {		
    String path="testServlet";
    response.sendRedirect(path);
}
```





