# JavaWeb —— HttpServletRequest 接口与 HttpServletResponse 接口 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36








# 【概述】

HttpServletRequest 继承自 ServletRequest 接口，HttpServletResponse 接口继承自 ServletResponse 接口，均位于 javax.servlet.http 包中，是 Servlet 的重要对象。

HttpServletRequest 接口与 HttpServletResponse 接口是根据 Http 协议定义的，除了 ServletRequest 接口与 ServletResponse 接口的方法外，其中还包含了大量获取 Http 请求相关的方法，如：getHeader(String name)，getMethod()，getSession() 等。

# 【HttpServletRequest 接口】

HttpServletRequest 接口是 ServletRequest 的子接口，其与 ServletRequest 接口一样封装了请求信息，可以从中获取到任何与请求相关的信息，其实现类是服务器给予实现的，且在服务器调用 service() 方法时传入。

除继承自 ServletRequest 接口的方法外，其还有大量与 Http 协议相关的方法：

## 1.获取请求的 URL
- String getRequestURI()：返回请求的 URI（包含请求的参数信息）
- StringBuffer getRequestURL()：返回请求的 URL（不包含请求的参数信息）
- String getServletPath()：返回请求 URI 中的 Servlet 路径的字符串，不包含请求的参数信息

## 2.获取请求方式与请求参数
- String getMethod()：返回请求所使用的 HTTP 类型
- String getQueryString()：返回请求中的参数（GET 请求中 ？之后的）

## 3.获取请求对象
- String getContextPath()：返回请求的上下文路径，以 " / " 开头
- Cookie[] getCookies()：返回请求中发送的所有 cookie 对象
- HttpSession getSession()：返回与请求关联的 HttpSession 对象

# 【HttpServletResponse 接口】

HttpServletResponse 接口是 ServletResponse 的子接口，其与 ServletResponse 接口一样封装了响应信息，用户想给的响应，具体可通过该接口的方法实现，其实现类是服务器给予实现的，且在服务器调用 service() 方法时传入。

除继承自 ServletResponse 接口的方法外，其还有大量与 Http 协议相关的方法：

## 1.设置请求重定向
- void sendRedirect(String locatoin)：使用客户端重定向到新的 URL，参数 location 为新的地址

## 2.写入信息
- void addCookie(Cookie cookie)：向客户端写入 Cookie 信息
- void sendError(int sc)：发送一个错误状态码为 sc 的错误响应到客户端
- void sendError(int sc,String msg)：发送一个包含错误状态码为 sc 错误信息为 msg 的错误信息的响应到客户端

## 3.URL 重写
- encodeURL()：对给定 URL，通过加上 sessionID 的方式进行编码，若不需编码，则直接返回未经修改的 URL
- encodeRedirectURL()：对给定 URL 进行编码，以用于 sendRedirect() 方法，若不需编码，则直接返回未经修改的 URL



