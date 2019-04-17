# JavaWeb —— ServletRequest 接口与 ServletResponse 接口 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:11[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30








# 【概述】

ServletRequest 接口与 ServletResponse 接口，均位于 javax.servlet.http 包中，是 Servlet 的重要对象，他们与提交表单时的 GET 方法、POST 方法息息相关。

使用 GET 方法传递参数：
- 在浏览器地址栏中输入某个 URL 或单击网页某一超链接时，浏览器发出的 HTTP 请求消息的请求方式是 GET
- 网页中 <form> 表单的 method 属性被设置为 GET，浏览器提交这个 FORM 表单时生成的 HTTP 请求消息的请求方式是 GET
- 使用 GET 方法传递参数时，参数将被嵌入到 URL 的 ？之后（这样十分不安全，因此常使用 POST 请求来传递参数）
- 传递的数据量一般在 1KB 以下

使用 POST 方法传递参数：
- POST 请求方式主要用于向 WEB 服务器提交 FORM 表单中的数据，即将 form 表单中的 method 属性置为 POST
- POST 方式将各个表单字段元素及其数据作为 HTTP 消息的实体内容发送给 Web 服务器（即在请求体中传递参数）
- 传递的数据量比 GET 请求方式大的多

关于 HTTP 协议：[点击这里](https://blog.csdn.net/u011815404/article/details/81748842)

# 【ServletRequest 接口】

ServletRequest 封装了请求信息，可以从中获取到任何与请求相关的信息，其实现类是服务器给予实现的，且在服务器调用 service() 方法时传入。

该接口的方法大量是读取客户端请求数据的方法，最常用的方法是获取请求参数的方法：
- String getParameter(String name)：根据请求参数名返回参数值（一般用于表单提交的用户名密码验证）
- String[] getParameterValues(String name)：根据请求参数名返回请求参数对应的字符串数组（一般用于表单提交的复选框）
- Enumeration getParameterNames()：返回参数名对应的 Enumeration 对象
- Map getParameterMap()：返回请求参数的键值对，key 是参数名，value 是参数值，参数值是 String 数组类型

此外，还有用于获取与转发请求相关的 RequestDispatcher 对象的方法：
- RequestDispatcher getRequestDispatcher(String path)：根据路径来获取 RequestDispatcher 对象

# 【ServletResponse 接口】

ServletResponse 封装了响应信息，用户想给的响应，具体可通过该接口的方法实现，其实现类是服务器给予实现的，且在服务器调用 service() 方法时传入。

该接口的方法大量是与生成响应相关的方法：
- void setCharacterEncoding(String charset)：设置响应正文的字符编码，默认字符编码为 ISO-8859-1
- String getCharacterEncoding（）：返回响应正文的字符编码
- void setContentType(String type)：设置响应正文的 MIME 类型
- String getContentType()：返回响应正文的 MIME类型。
- PrintWriter getWriter()：返回 PrintWriter 对象，可以将 print() 中的参数值直接打印到浏览器上

要注意的是 ServletResponse 中的响应正文的默认 MIME类 型为 text/plain（纯文本类型），而 HttpServletResponse 中的响应正文的默认 MIME 类型为 text/html（HTML 文档类型）

此外，还有用于请求重定向的方法：
- void sendReadirect(String path)：该方法会重新定位到 URL 到另外一个资源来处理请求，让客户端重新访问另一资源



