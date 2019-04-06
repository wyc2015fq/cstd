## [javaweb学习总结(八)——HttpServletResponse对象(二)](https://www.cnblogs.com/xdp-gacl/p/3791993.html)

## 一、HttpServletResponse常见应用——生成验证码

### 1.1、生成随机图片用作验证码

　　生成图片主要用到了一个**BufferedImage**类，

　　![img](data:image/png;base64,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)

**生成随机图片范例：**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package gacl.response.study;
 2 
 3 import java.awt.Color;
 4 import java.awt.Font;
 5 import java.awt.Graphics2D;
 6 import java.awt.image.BufferedImage;
 7 import java.io.IOException;
 8 import java.util.Random;
 9 import javax.imageio.ImageIO;
10 import javax.servlet.ServletException;
11 import javax.servlet.http.HttpServlet;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 
15 public class ResponseDemo03 extends HttpServlet {
16 
17     public void doGet(HttpServletRequest request, HttpServletResponse response)
18             throws ServletException, IOException {
19         
20         response.setHeader("refresh", "5");//设置refresh响应头控制浏览器每隔5秒钟刷新一次
21         //1.在内存中创建一张图片
22         BufferedImage image = new BufferedImage(80, 20, BufferedImage.TYPE_INT_RGB);
23         //2.得到图片
24         //Graphics g = image.getGraphics();
25         Graphics2D g = (Graphics2D)image.getGraphics();
26         g.setColor(Color.WHITE);//设置图片的背景色
27         g.fillRect(0, 0, 80, 20);//填充背景色
28         //3.向图片上写数据
29         g.setColor(Color.BLUE);//设置图片上字体的颜色
30         g.setFont(new Font(null, Font.BOLD, 20));
31         g.drawString(makeNum(), 0, 20);
32         //4.设置响应头控制浏览器浏览器以图片的方式打开
33         response.setContentType("image/jpeg");//等同于response.setHeader("Content-Type", "image/jpeg");
34         //5.设置响应头控制浏览器不缓存图片数据
35         response.setDateHeader("expries", -1);
36         response.setHeader("Cache-Control", "no-cache");
37         response.setHeader("Pragma", "no-cache");
38         //6.将图片写给浏览器
39         ImageIO.write(image, "jpg", response.getOutputStream());
40     }
41 
42     /**
43      * 生成随机数字
44      * @return
45      */
46     private String makeNum(){
47         Random random = new Random();
48         String num = random.nextInt(9999999)+"";
49         StringBuffer sb = new StringBuffer();
50         for (int i = 0; i < 7-num.length(); i++) {
51             sb.append("0");
52         }
53         num = sb.toString()+num;
54         return num;
55     }
56     
57     public void doPost(HttpServletRequest request, HttpServletResponse response)
58             throws ServletException, IOException {
59         doGet(request, response);
60     }
61 
62 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行结果如下：

　　![img](https://images0.cnblogs.com/i/289233/201406/192218479895841.gif)

## 二、HttpServletResponse常见应用——设置响应头控制浏览器的行为

### 2.1、设置http响应头控制浏览器禁止缓存当前文档内容     

```
1 response.setDateHeader("expries", -1);
2 response.setHeader("Cache-Control", "no-cache");
3 response.setHeader("Pragma", "no-cache");
```

### 2.2、设置http响应头控制浏览器定时刷新网页(refresh)

```
1 response.setHeader("refresh", "5");//设置refresh响应头控制浏览器每隔5秒钟刷新一次
```

###  2.3、通过response实现请求重定向

　　请求重定向指：**一个web资源收到客户端请求后，通知客户端去访问另外一个web资源，这称之为请求重定向。**

　　应用场景：用户登陆，用户首先访问登录页面，登录成功后，就会跳转到某个页面，这个过程就是一个请求重定向的过程

　　实现方式：response.sendRedirect(String location)，即调用response对象的sendRedirect方法实现请求重定向
　　sendRedirect内部的实现原理：**使用response设置****302状态码和设置location响应头实现重定向**

例如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package gacl.response.study;
 2 import java.io.IOException;
 3 import javax.servlet.ServletException;
 4 import javax.servlet.http.HttpServlet;
 5 import javax.servlet.http.HttpServletRequest;
 6 import javax.servlet.http.HttpServletResponse;
 7 
 8 public class ResponseDemo04 extends HttpServlet {
 9 
10     public void doGet(HttpServletRequest request, HttpServletResponse response)
11             throws ServletException, IOException {
12         /**
13          * 1.调用sendRedirect方法实现请求重定向,
14          * sendRedirect方法内部调用了
15          * response.setHeader("Location", "/JavaWeb_HttpServletResponse_Study_20140615/index.jsp");
16          * response.setStatus(HttpServletResponse.SC_FOUND);//设置302状态码，等同于response.setStatus(302);
17          */
18         response.sendRedirect("/JavaWeb_HttpServletResponse_Study_20140615/index.jsp");
19         
20         //2.使用response设置302状态码和设置location响应头实现重定向实现请求重定向
21         //response.setHeader("Location", "/JavaWeb_HttpServletResponse_Study_20140615/index.jsp");
22         //response.setStatus(HttpServletResponse.SC_FOUND);//设置302状态码，等同于response.setStatus(302);
23     }
24 
25     public void doPost(HttpServletRequest request, HttpServletResponse response)
26             throws ServletException, IOException {
27         doGet(request, response);
28     }
29 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 三、web工程中URL地址的推荐写法

　　在JavaWeb开发中，只要是写URL地址，那么建议最好以"/"开头，也就是使用绝对路径的方式，那么这个"/"到底代表什么呢？可以用如下的方式来记忆"/"：**如果"/"是给服务器用的，则代表当前的web工程，如果"/"是给浏览器用的，则代表webapps目录。**

### 3.1、**"/"**代表**当前****web工程**的常见应用场景

**①.ServletContext.getRealPath(String path)获取资源的绝对路径**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 /**
2 * 1.ServletContext.getRealPath("/download/1.JPG")是用来获取服务器上的某个资源，
3 * 那么这个"/"就是给服务器用的，"/"此时代表的就是web工程
4  * ServletContext.getRealPath("/download/1.JPG")表示的就是读取web工程下的download文件夹中的1.JPG这个资源
5 * 只要明白了"/"代表的具体含义，就可以很快写出要访问的web资源的绝对路径
6 */
7 this.getServletContext().getRealPath("/download/1.JPG");
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**②.在服务器端forward到其他页面**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 /**
2 * 2.forward
3  * 客户端请求某个web资源，服务器跳转到另外一个web资源，这个forward也是给服务器用的，
4 * 那么这个"/"就是给服务器用的，所以此时"/"代表的就是web工程
5 */
6 this.getServletContext().getRequestDispatcher("/index.jsp").forward(request, response);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**③.使用include指令或者<jsp:include>标签引入页面**

```
1 <%@include file="/jspfragments/head.jspf" %>
1 <jsp:include page="/jspfragments/demo.jsp" />
```

　　此时"/"代表的都是web工程。

### 3.2、**"/"****代表webapps目录**的常见应用场景

**①.使用sendRedirect实现请求重定向**

```
1 response.sendRedirect("/JavaWeb_HttpServletResponse_Study_20140615/index.jsp");
```

　　服务器发送一个URL地址给浏览器，浏览器拿到URL地址之后，再去请求服务器，所以这个"/"是给浏览器使用的，此时"/"代表的就是webapps目录，"/JavaWeb_HttpServletResponse_Study_20140615/index.jsp"这个地址指的就是"webapps\JavaWeb_HttpServletResponse_Study_20140615\index.jsp"

　　**response.sendRedirect("/项目名称/文件夹目录/页面");**这种写法是将项目名称写死在程序中的做法，不灵活，万一哪天项目名称变了，此时就得改程序，所以推荐使用下面的灵活写法：

将

```
1 response.sendRedirect("/JavaWeb_HttpServletResponse_Study_20140615/index.jsp");
```

　　这种写法改成

```
1 response.sendRedirect(request.getContextPath()+"/index.jsp");
```

　　request.getContextPath()获取到的内容就是"/JavaWeb_HttpServletResponse_Study_20140615"，这样就比较灵活了，使用request.getContextPath()代替"/项目名称"，推荐使用这种方式，灵活方便！

②.**使用超链接跳转**

```
1 <a href="/JavaWeb_HttpServletResponse_Study_20140615/index.jsp">跳转到首页</a>
```

　　这是客户端浏览器使用的超链接跳转，这个"/"是给浏览器使用的，此时"/"代表的就是webapps目录。

　　使用超链接访问web资源，绝对路径的写法推荐使用下面的写法改进：

```
1 <a href="${pageContext.request.contextPath}/index.jsp">跳转到首页</a>
```

　　这样就可以避免在路径中出现项目的名称，使用**${pageContext.request.contextPath}**取代"/JavaWeb_HttpServletResponse_Study_20140615"

**③.Form表单提交**

```
1 <form action="/JavaWeb_HttpServletResponse_Study_20140615/servlet/CheckServlet" method="post">    
2         <input type="submit" value="提交">
3 </form>
```

　　这是客户端浏览器将form表单提交到服务器，所以这个"/"是给浏览器使用的，此时"/"代表的就是webapps目录。

 对于form表单提交中action属性绝对路径的写法，也推荐使用如下的方式改进：

```
1 <form action="${pageContext.request.contextPath}/servlet/CheckServlet" method="post">
2          <input type="submit" value="提交">
3 </form>
```

　　**${pageContext.request.contextPath}得到的就是"/JavaWeb_HttpServletResponse_Study_20140615"**

　　**${pageContext.request.contextPath}的效果等同于request.getContextPath()，两者获取到的都是"/项目名称"**

**④.js脚本和css样式文件的引用**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1  <%--使用绝对路径的方式引用js脚本--%>
2  <script type="text/javascript" src="${pageContext.request.contextPath}/js/index.js"></script>
3  <%--${pageContext.request.contextPath}与request.getContextPath()写法是得到的效果是一样的--%>
4  <script type="text/javascript" src="<%=request.getContextPath()%>/js/login.js"></script>
5  <%--使用绝对路径的方式引用css样式--%>
6  <link rel="stylesheet" href="${pageContext.request.contextPath}/css/index.css" type="text/css"/>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**综合范例：**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 
 3 <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
 4 <html>
 5   <head>
 6     <title>"/"代表webapps目录的常见应用场景</title>
 7     <%--使用绝对路径的方式引用js脚本--%>
 8     <script type="text/javascript" src="${pageContext.request.contextPath}/js/index.js"></script>
 9     <%--${pageContext.request.contextPath}与request.getContextPath()写法是得到的效果是一样的--%>
10     <script type="text/javascript" src="<%=request.getContextPath()%>/js/login.js"></script>
11     <%--使用绝对路径的方式引用css样式--%>
12       <link rel="stylesheet" href="${pageContext.request.contextPath}/css/index.css" type="text/css"/>
13   </head>
14   
15   <body>
16       <%--form表单提交--%>
17        <form action="${pageContext.request.contextPath}/servlet/CheckServlet" method="post">
18            <input type="submit" value="提交">
19        </form>
20        <%--超链接跳转页面--%>
21        <a href="${pageContext.request.contextPath}/index.jsp">跳转到首页</a>
22   </body>
23 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

##  四、response细节问题

　　getOutputStream和getWriter方法分别用于得到输出**二进制数据**、输出**文本数据**的ServletOuputStream、Printwriter对象。
　　getOutputStream和getWriter这两个方法**互相排斥**，调用了其中的任何一个方法后，就不能再调用另一方法。  
　　Servlet程序向ServletOutputStream或PrintWriter对象中写入的数据将被Servlet引擎从response里面获取，Servlet引擎将这些数据当作响应消息的正文，然后再与响应状态行和各响应头组合后输出到客户端。 
　　Serlvet的service方法结束后，Servlet引擎将检查getWriter或getOutputStream方法返回的输出流对象是否已经调用过close方法，如果没有，Servlet引擎将调用close方法关闭该输出流对象。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)