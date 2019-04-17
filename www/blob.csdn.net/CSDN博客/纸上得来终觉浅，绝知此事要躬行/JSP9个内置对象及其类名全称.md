# JSP9个内置对象及其类名全称 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月28日 11:04:55[boonya](https://me.csdn.net/boonya)阅读数：2634








在很多面试中都会被问及JSP是什么，以及它的内置对象包括哪些。JSP 页面中包含9 个内置对象，这9 个内置对象都是Servlet API 的类或者接口的实例，只是JSP 规范将它们完成了默认初始化，即它们已经是对象，可以直接使用。

这9个内置对象依次如下：

        （1）**application**：**javax.servlet.ServletContext**的实例，该实例代表JSP所属的WEB应用本身，可用于JSP页面，或者Servlet之间交换信息。常用的方法有*getAttribute(String attName), setAttribute(String attName,String attValue)*和*getInitParameter(String Param)*等。

        （2）**config**：**javax.servlet.ServletConfig**的实例，该实例代表JSP的配置信息，常用的方法有*getInitparameter(String paramName)*及*getInitParametername()*等方法。事实上，JSP页面通常无需配置，也就不存在配置信息。因此该对象更多地在Servlet中有效。

        （3）**exception**：**java.lang.Throwable**的实例，该实例代表其他页面中的错误和异常。只有当页面时错误处理页面，即编译指令的isErrorPage属性是true，该对象才可以使用。常用的方法有*getMessage()*和*printStackTrace()*等。

        （4）**out**：**javax.servlet.jsp.JspWriter**的实例，该实例代表Jsp页面输出流，用于输出内容，形成HTML页面。

        （5）**page**：代表页面本身，通常没有太大的用处，也就有是Servlet中的this，其类型就是生成Servlet。

        （6）**pageContext**：**javax.servlet.jsp.PageContext**的实例，该实例代表JSP页面的上下文，使用该实例可以访问页面中的共享数据。常用的方法*getServletContext()*和*getSevletConfig()*等。

        （7）**request**：**javax.servlet.http.HttpServletRequest**的实例，该对象封装了一次请求，客户端的请求都被封装在该对象里。这是一个常用的对象，获取客户端请求参数必须使用该对象。常用的方法有：*getParameter(String paramName),getParamValues(String paramName),setAttribute(String attributeName,String attributeValue),getAttribute(String attributeName)*和*setCharacterEncoding(String env)*等。

        （8）**response**：**javax.servlet.http.HttpServletResponse**的实例，代表服务器对于客户端的响应。通常，很少使用该对象直接响应，输出响应对象使用out对象，而response对象常用于重定向。常用的对象有*sendRedirect(java.lang.String location)*等。

        （9）**session**：**javax.servlet.http.HttpSession**的实例，该对象代表一次会话。在servlet中,session是封装在javax.servlet.http.HttpSession这个接口中的，这个接口是构建在cookie或者URL重写的基础上,要得到一个HttpSession的实例，就可以通过HttpServletRequest的getSession()方法来获得。从客户端浏览器和站点建立连接起，开始会话，直到关闭浏览器结束会话。常用的方法有*getAttribute(String attName),setAttribute(String attName,String attValue)*等



