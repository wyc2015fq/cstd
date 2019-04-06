## [javaweb学习总结(四十三)——Filter高级开发](https://www.cnblogs.com/xdp-gacl/p/3952405.html)

　　在filter中可以得到代表用户请求和响应的request、response对象，因此在编程中可以使用Decorator(装饰器)模式对request、response对象进行包装，再把包装对象传给目标资源，从而实现一些特殊需求。

## 一、Decorator设计模式

### 1.1、Decorator设计模式介绍

　　当某个对象的方法不适应业务需求时，通常有2种方式可以对方法进行增强：

1. 编写子类，覆盖需增强的方法。
2. 使用Decorator设计模式对方法进行增强。

　　在阎宏博士的《JAVA与模式》一书中开头是这样描述装饰（Decorator）模式的：**装饰模式又名包装(Wrapper)模式。装饰模式以对客户端透明的方式扩展对象的功能，是继承关系的一个替代方案。****装饰模式是在不必改变原类文件和使用继承的情况下，动态的扩展一个对象的功能。它是通过创建一个包装对象，也就是装饰来包裹真实的对象。**

　　那么在实际应用中遇到需增强对象的方法时，到底选用哪种方式比较好呢？这个没有具体的定式，只能是根据具体的需求来采用具体的方式，不过**有一种情况下，必须使用Decorator设计模式：即被增强的对象，开发人员只能得到它的对象，无法得到它的class文件**。比如request、response对象，开发人员之所以在servlet中能通过sun公司定义的HttpServletRequest\response接口去操作这些对象，是因为Tomcat服务器厂商编写了request、response接口的实现类。web服务器在调用servlet时，会用这些接口的实现类创建出对象，然后传递给servlet程序。此种情况下，由于开发人员根本不知道服务器厂商编写的request、response接口的实现类是哪个？在程序中只能拿到服务器厂商提供的对象，因此就只能采用Decorator设计模式对这些对象进行增强。

### 1.2、Decorator设计模式的实现

　　1.首先看需要被增强对象继承了什么接口或父类，编写一个类也去继承这些接口或父类。
　　2.在类中定义一个变量，变量类型即需增强对象的类型。
　　3.在类中定义一个构造函数，接收需增强的对象。
　　4.覆盖需增强的方法，编写增强的代码。

##  二、使用Decorator设计模式增强request对象

　　Servlet API  中提供了一个request对象的Decorator设计模式的默认实现类HttpServletRequestWrapper，HttpServletRequestWrapper  类实现了request 接口中的所有方法，但这些方法的内部实现都是仅仅调用了一下所包装的的 request  对象的对应方法，以避免用户在对request对象进行增强时需要实现request接口中的所有方法。

### 2.1、使用Decorator模式包装request对象解决get和post请求方式下的中文乱码问题

　　编写一个用于处理中文乱码的过滤器CharacterEncodingFilter，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.IOException;
  4 
  5 import javax.servlet.Filter;
  6 import javax.servlet.FilterChain;
  7 import javax.servlet.FilterConfig;
  8 import javax.servlet.ServletException;
  9 import javax.servlet.ServletRequest;
 10 import javax.servlet.ServletResponse;
 11 import javax.servlet.http.HttpServletRequest;
 12 import javax.servlet.http.HttpServletRequestWrapper;
 13 import javax.servlet.http.HttpServletResponse;
 14 
 15 /**
 16 * @ClassName: CharacterEncodingFilter
 17 * @Description: 此过滤器用来解决解决get、post请求方式下的中文乱码问题
 18 * @author: 孤傲苍狼
 19 * @date: 2014-8-31 下午11:09:37
 20 *
 21 */ 
 22 public class CharacterEncodingFilter implements Filter {
 23 
 24     private FilterConfig filterConfig = null;
 25     //设置默认的字符编码
 26     private String defaultCharset = "UTF-8";
 27 
 28     public void doFilter(ServletRequest req, ServletResponse resp,
 29             FilterChain chain) throws IOException, ServletException {
 30         
 31         HttpServletRequest request = (HttpServletRequest) req;
 32         HttpServletResponse response = (HttpServletResponse) resp;
 33         //得到在web.xml中配置的字符编码
 34         String charset = filterConfig.getInitParameter("charset");
 35         if(charset==null){
 36             charset = defaultCharset;
 37         }
 38         request.setCharacterEncoding(charset);
 39         response.setCharacterEncoding(charset);
 40         response.setContentType("text/html;charset="+charset);
 41         
 42         MyCharacterEncodingRequest requestWrapper = new MyCharacterEncodingRequest(request);
 43         chain.doFilter(requestWrapper, response);
 44     }
 45 
 46     public void init(FilterConfig filterConfig) throws ServletException {
 47         //得到过滤器的初始化配置信息
 48         this.filterConfig = filterConfig;
 49     }
 50     
 51     public void destroy() {
 52 
 53     }
 54 }
 55 
 56 /**
 57 * @ClassName: MyCharacterEncodingRequest
 58 * @Description: Servlet API中提供了一个request对象的Decorator设计模式的默认实现类HttpServletRequestWrapper,
 59 * (HttpServletRequestWrapper类实现了request接口中的所有方法，但这些方法的内部实现都是仅仅调用了一下所包装的的 request对象的对应方法)
 60 * 以避免用户在对request对象进行增强时需要实现request接口中的所有方法。
 61 * 所以当需要增强request对象时，只需要写一个类继承HttpServletRequestWrapper类，然后在重写需要增强的方法即可
 62 * @author: 孤傲苍狼
 63 * @date: 2014-9-2 下午10:42:57
 64 *     1.实现与被增强对象相同的接口 
 65     2、定义一个变量记住被增强对象
 66     3、定义一个构造函数，接收被增强对象
 67     4、覆盖需要增强的方法
 68     5、对于不想增强的方法，直接调用被增强对象（目标对象）的方法
 69 */ 
 70 class MyCharacterEncodingRequest extends HttpServletRequestWrapper{
 71     //定义一个变量记住被增强对象(request对象是需要被增强的对象)
 72     private HttpServletRequest request;
 73     //定义一个构造函数，接收被增强对象
 74     public MyCharacterEncodingRequest(HttpServletRequest request) {
 75         super(request);
 76         this.request = request;
 77     }
 78     /* 覆盖需要增强的getParameter方法
 79      * @see javax.servlet.ServletRequestWrapper#getParameter(java.lang.String)
 80      */
 81     @Override
 82     public String getParameter(String name) {
 83         try{
 84             //获取参数的值
 85             String value= this.request.getParameter(name);
 86             if(value==null){
 87                 return null;
 88             }
 89             //如果不是以get方式提交数据的，就直接返回获取到的值
 90             if(!this.request.getMethod().equalsIgnoreCase("get")) {
 91                 return value;
 92             }else{
 93                 //如果是以get方式提交数据的，就对获取到的值进行转码处理
 94                 value = new String(value.getBytes("ISO8859-1"),this.request.getCharacterEncoding());
 95                 return value;
 96             }
 97         }catch (Exception e) {
 98             throw new RuntimeException(e);
 99         }
100     }
101 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml文件中配置CharacterEncodingFilter

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <!--配置字符过滤器，解决get、post请求方式下的中文乱码问题-->
 2   <filter>
 3       <filter-name>CharacterEncodingFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.CharacterEncodingFilter</filter-class>
 5   </filter>
 6   
 7   <filter-mapping>
 8       <filter-name>CharacterEncodingFilter</filter-name>
 9       <url-pattern>/*</url-pattern>
10   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　编写jsp测试页面，如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <%--引入jstl标签库 --%>
 3 <%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
 4 <!DOCTYPE HTML>
 5 <html>
 6   <head>
 7     <title>使用字符过滤器解决解决get、post请求方式下的中文乱码问题</title>
 8   </head>
 9   
10   <body>
11        <%--使用c:url标签构建url，构建好的url存储在servletDemo1变量中--%>
12        <c:url value="/servlet/ServletDemo1" scope="page" var="servletDemo1">
13            <%--构建的url的附带的中文参数 ，参数名是：username，值是：孤傲苍狼--%>
14            <c:param name="username" value="孤傲苍狼"></c:param>
15        </c:url>
16       <%--使用get的方式访问 --%>
17        <a href="${servletDemo1}">超链接(get方式请求)</a>
18        <hr/>
19        <%--使用post方式提交表单 --%>
20        <form action="${pageContext.request.contextPath}/servlet/ServletDemo1" method="post">
21            用户名：<input type="text" name="username" value="孤傲苍狼" />
22            <input type="submit" value="post方式提交">
23        </form>
24        
25   </body>
26 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　编写处理用户请求的ServletDemo1

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import java.io.PrintWriter;
 5 
 6 import javax.servlet.ServletException;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 public class ServletDemo1 extends HttpServlet {
12 
13     public void doGet(HttpServletRequest request, HttpServletResponse response)
14             throws ServletException, IOException {
15         //接收参数
16         String username = request.getParameter("username");
17         //获取请求方式
18         String method = request.getMethod();
19         //获取输出流
20         PrintWriter out = response.getWriter();
21         out.write("请求的方式："+method);
22         out.write("<br/>");
23         out.write("接收到的参数："+username);
24     }
25 
26     public void doPost(HttpServletRequest request, HttpServletResponse response)
27             throws ServletException, IOException {
28         doGet(request, response);
29     }
30 
31 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　测试结果如下：

　　![img](https://images0.cnblogs.com/blog/289233/201409/022317178911173.gif)

　　从运行结果中可以看到，无论是get请求方式还是post请求方式，中文乱码问题都可以完美解决了。

### 2.2、使用Decorator模式包装request对象实现html标签转义功能

　　编写一个html转义过滤器，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.IOException;
  4 
  5 import javax.servlet.Filter;
  6 import javax.servlet.FilterChain;
  7 import javax.servlet.FilterConfig;
  8 import javax.servlet.ServletException;
  9 import javax.servlet.ServletRequest;
 10 import javax.servlet.ServletResponse;
 11 import javax.servlet.http.HttpServletRequest;
 12 import javax.servlet.http.HttpServletRequestWrapper;
 13 import javax.servlet.http.HttpServletResponse;
 14 
 15 /**
 16 * @ClassName: HtmlFilter
 17 * @Description: html转义过滤器
 18 * @author: 孤傲苍狼
 19 * @date: 2014-9-2 下午11:28:41
 20 *
 21 */ 
 22 public class HtmlFilter implements Filter {
 23 
 24     public void doFilter(ServletRequest req, ServletResponse resp,
 25             FilterChain chain) throws IOException, ServletException {
 26         
 27         HttpServletRequest request = (HttpServletRequest) req;
 28         HttpServletResponse response = (HttpServletResponse) resp;
 29 
 30         MyHtmlRequest myrequest = new MyHtmlRequest(request);
 31         chain.doFilter(myrequest, response);
 32         
 33     }
 34 
 35     
 36     public void destroy() {
 37         
 38     }
 39 
 40     
 41     public void init(FilterConfig filterConfig) throws ServletException {
 42         
 43     }
 44 }
 45 
 46 /**
 47 * @ClassName: MyHtmlRequest
 48 * @Description: 使用Decorator模式包装request对象，实现html标签转义功能
 49 * @author: 孤傲苍狼
 50 * @date: 2014-9-2 下午11:29:09
 51 *
 52 */ 
 53 class MyHtmlRequest extends HttpServletRequestWrapper {
 54 
 55     private HttpServletRequest request;
 56 
 57     public MyHtmlRequest(HttpServletRequest request) {
 58         super(request);
 59         this.request = request;
 60     }
 61 
 62     /* 覆盖需要增强的getParameter方法
 63      * @see javax.servlet.ServletRequestWrapper#getParameter(java.lang.String)
 64      */
 65     @Override
 66     public String getParameter(String name) {
 67         String value = this.request.getParameter(name);
 68         if (value == null) {
 69             return null;
 70         }
 71         //调用filter转义value中的html标签
 72         return filter(value);
 73     }
 74 
 75     /**
 76     * @Method: filter
 77     * @Description: 过滤内容中的html标签
 78     * @Anthor:孤傲苍狼
 79     * @param message
 80     * @return
 81     */ 
 82     public String filter(String message) {
 83         if (message == null){
 84             return null;
 85         }
 86         char content[] = new char[message.length()];
 87         message.getChars(0, message.length(), content, 0);
 88         StringBuffer result = new StringBuffer(content.length + 50);
 89         for (int i = 0; i < content.length; i++) {
 90             switch (content[i]) {
 91             case '<':
 92                 result.append("&lt;");
 93                 break;
 94             case '>':
 95                 result.append("&gt;");
 96                 break;
 97             case '&':
 98                 result.append("&amp;");
 99                 break;
100             case '"':
101                 result.append("&quot;");
102                 break;
103             default:
104                 result.append(content[i]);
105             }
106         }
107         return result.toString();
108     }
109 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml文件中配置HtmlFilter

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <!--配置Html过滤器，转义内容中的html标签-->
 2   <filter>
 3       <filter-name>HtmlFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.HtmlFilter</filter-class>
 5   </filter>
 6   
 7   <filter-mapping>
 8       <filter-name>HtmlFilter</filter-name>
 9       <url-pattern>/*</url-pattern>
10   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　编写jsp测试页面，如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>html过滤器测试</title>
 6   </head>
 7   
 8   <body>
 9        <form action="${pageContext.request.contextPath}/servlet/ServletDemo2" method="post">
10            留言:
11            <textarea rows="8" cols="70" name="message">
12            <script type="text/javascript">
13            while(true){
14                alert("死循环了，我会不停地弹出了");
15            }
16            </script>
17         <a href="http://www.cnblogs.com">访问博客园</a>
18            </textarea>
19            <input type="submit" value="发表">
20        </form>
21   </body>
22 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　编写处理用户请求的ServletDemo2

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 
10 public class ServletDemo2 extends HttpServlet {
11 
12     public void doGet(HttpServletRequest request, HttpServletResponse response)
13             throws ServletException, IOException {
14         //获取用户输入的内容
15         String message = request.getParameter("message");
16         response.getWriter().write("您上次的留言是：<br/>" + message);
17     }
18 
19     public void doPost(HttpServletRequest request, HttpServletResponse response)
20             throws ServletException, IOException {
21         doGet(request, response);
22     }
23 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

测试结果如下：

　　![img](https://images0.cnblogs.com/blog/289233/201409/022357043282069.gif)

　　从运行结果中可以看到，所有的html标签都被转义输出了。

### 2.3、使用Decorator模式包装request对象实现敏感字符过滤功能

　　编写一个敏感字符过滤器，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.BufferedReader;
  4 import java.io.IOException;
  5 import java.io.InputStream;
  6 import java.io.InputStreamReader;
  7 import java.io.UnsupportedEncodingException;
  8 import java.util.ArrayList;
  9 import java.util.List;
 10 import javax.servlet.Filter;
 11 import javax.servlet.FilterChain;
 12 import javax.servlet.FilterConfig;
 13 import javax.servlet.ServletException;
 14 import javax.servlet.ServletRequest;
 15 import javax.servlet.ServletResponse;
 16 import javax.servlet.http.HttpServletRequest;
 17 import javax.servlet.http.HttpServletRequestWrapper;
 18 import javax.servlet.http.HttpServletResponse;
 19 
 20 /**
 21 * @ClassName: DirtyFilter
 22 * @Description: 敏感词过滤器
 23 * @author: 孤傲苍狼
 24 * @date: 2014-9-6 上午10:43:11
 25 *
 26 */ 
 27 public class DirtyFilter implements Filter {
 28 
 29     private FilterConfig config = null;
 30     
 31     @Override
 32     public void init(FilterConfig filterConfig) throws ServletException {
 33         this.config = filterConfig;
 34     }
 35 
 36     @Override
 37     public void doFilter(ServletRequest req, ServletResponse resp,
 38             FilterChain chain) throws IOException, ServletException {
 39         
 40         HttpServletRequest request = (HttpServletRequest) req;
 41         HttpServletResponse response = (HttpServletResponse) resp;
 42         DirtyRequest dirtyrequest = new DirtyRequest(request);
 43         
 44         chain.doFilter(dirtyrequest, response);
 45     }
 46 
 47     @Override
 48     public void destroy() {
 49 
 50     }
 51     
 52     /**
 53     * @Method: getDirtyWords
 54     * @Description: 获取敏感字符
 55     * @Anthor:孤傲苍狼
 56     *
 57     * @return
 58     */ 
 59     private List<String> getDirtyWords(){
 60         List<String> dirtyWords = new ArrayList<String>();
 61         String dirtyWordPath = config.getInitParameter("dirtyWord");
 62         InputStream inputStream = config.getServletContext().getResourceAsStream(dirtyWordPath);
 63         InputStreamReader is = null;
 64         try {
 65             is = new InputStreamReader(inputStream,"UTF-8");
 66         } catch (UnsupportedEncodingException e2) {
 67             e2.printStackTrace();
 68         }
 69         BufferedReader reader = new BufferedReader(is);
 70         String line;
 71         try {
 72             while ((line = reader.readLine())!= null) {//如果 line为空说明读完了
 73                 dirtyWords.add(line);
 74             }
 75         } catch (IOException e) {
 76             e.printStackTrace();
 77         } 
 78         return dirtyWords;
 79     }
 80     
 81     /**
 82     * @ClassName: DirtyRequest
 83     * @Description: 使用Decorator模式包装request对象，实现敏感字符过滤功能
 84     * @author: 孤傲苍狼
 85     * @date: 2014-9-6 上午11:56:35
 86     *
 87     */ 
 88     class DirtyRequest extends HttpServletRequestWrapper{
 89 
 90         private List<String> dirtyWords = getDirtyWords();
 91         private HttpServletRequest request;
 92         public DirtyRequest(HttpServletRequest request) {
 93             super(request);
 94             this.request = request;
 95         }
 96         /* 重写getParameter方法，实现对敏感字符的过滤
 97          * @see javax.servlet.ServletRequestWrapper#getParameter(java.lang.String)
 98          */
 99         @Override
100         public String getParameter(String name) {
101             
102             String value = this.request.getParameter(name);
103             if(value==null){
104                 return null;
105             }
106             
107             for(String dirtyWord : dirtyWords){
108                 if(value.contains(dirtyWord)){
109                     System.out.println("内容中包含敏感词："+dirtyWord+"，将会被替换成****");
110                     //替换敏感字符
111                     value = value.replace(dirtyWord, "****");
112                 }
113             }
114             return value;
115         }
116     }
117 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml文件中配置DirtyFilter

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  <!--配置敏感字符过滤器-->
 2   <filter>
 3       <filter-name>DirtyFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.DirtyFilter</filter-class>
 5       <!-- 配置要过滤的敏感字符文件 -->
 6       <init-param>
 7          <param-name>dirtyWord</param-name>    
 8          <param-value>/WEB-INF/DirtyWord.txt</param-value>
 9     </init-param>
10   </filter>
11   
12   <filter-mapping>
13       <filter-name>DirtyFilter</filter-name>
14       <url-pattern>/*</url-pattern>
15   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　当用户填写的内容包含一些敏感字符时，在DirtyFilter过滤器中就会将这些敏感字符替换掉。

　　我们如果将上述的CharacterEncodingFilter、HtmlFilter、DirtyFilter这三个过滤器联合起来使用，那么就相当于是把request对象包装了3次，request对象的getParameter方法经过3次重写，使得getParameter方法的功能大大增强，可以同时解决中文乱码，html标签转义，敏感字符过滤这些需求。

　　在实际开发中完全可以将上述的三个过滤器合并成一个，让合并后的过滤器具有解决中文乱码，html标签转义，敏感字符过滤这些功能，例如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.BufferedReader;
  4 import java.io.IOException;
  5 import java.io.InputStream;
  6 import java.io.InputStreamReader;
  7 import java.io.UnsupportedEncodingException;
  8 import java.util.ArrayList;
  9 import java.util.List;
 10 
 11 import javax.servlet.Filter;
 12 import javax.servlet.FilterChain;
 13 import javax.servlet.FilterConfig;
 14 import javax.servlet.ServletException;
 15 import javax.servlet.ServletRequest;
 16 import javax.servlet.ServletResponse;
 17 import javax.servlet.http.HttpServletRequest;
 18 import javax.servlet.http.HttpServletRequestWrapper;
 19 import javax.servlet.http.HttpServletResponse;
 20 
 21 /**
 22 * @ClassName: AdvancedFilter
 23 * @Description: 这个过滤器是用来解决中文乱码，转义内容中的html标签，过滤内容中的敏感字符的
 24 * @author: 孤傲苍狼
 25 * @date: 2014-9-6 下午6:17:37
 26 *
 27 */ 
 28 public class AdvancedFilter implements Filter {
 29 
 30     private FilterConfig filterConfig = null;
 31     //设置默认的字符编码
 32     private String defaultCharset = "UTF-8";
 33     
 34     @Override
 35     public void init(FilterConfig filterConfig) throws ServletException {
 36         //得到过滤器的初始化配置信息
 37         this.filterConfig = filterConfig;
 38     }
 39 
 40     @Override
 41     public void doFilter(ServletRequest req, ServletResponse resp,
 42             FilterChain chain) throws IOException, ServletException {
 43         HttpServletRequest request = (HttpServletRequest) req;
 44         HttpServletResponse response = (HttpServletResponse) resp;
 45         //得到在web.xml中配置的字符编码
 46         String charset = filterConfig.getInitParameter("charset");
 47         if(charset==null){
 48             charset = defaultCharset;
 49         }
 50         request.setCharacterEncoding(charset);
 51         response.setCharacterEncoding(charset);
 52         response.setContentType("text/html;charset="+charset);
 53         
 54         AdvancedRequest requestWrapper = new AdvancedRequest(request);
 55         chain.doFilter(requestWrapper, response);
 56     }
 57 
 58     @Override
 59     public void destroy() {
 60 
 61     }
 62 
 63     class AdvancedRequest extends HttpServletRequestWrapper{
 64         
 65         private List<String> dirtyWords = getDirtyWords();
 66         
 67         //定义一个变量记住被增强对象(request对象是需要被增强的对象)
 68         private HttpServletRequest request;
 69         //定义一个构造函数，接收被增强对象
 70         public AdvancedRequest(HttpServletRequest request) {
 71             super(request);
 72             this.request = request;
 73         }
 74         /* 覆盖需要增强的getParameter方法
 75          * @see javax.servlet.ServletRequestWrapper#getParameter(java.lang.String)
 76          */
 77         @Override
 78         public String getParameter(String name) {
 79             try{
 80                 //获取参数的值
 81                 String value= this.request.getParameter(name);
 82                 if(value==null){
 83                     return null;
 84                 }
 85                 //如果不是以get方式提交数据的，就直接返回获取到的值
 86                 if(!this.request.getMethod().equalsIgnoreCase("get")) {
 87                     //调用filter转义value中的html标签
 88                     value= filter(value);
 89                 }else{
 90                     //如果是以get方式提交数据的，就对获取到的值进行转码处理
 91                     value = new String(value.getBytes("ISO8859-1"),this.request.getCharacterEncoding());
 92                     //调用filter转义value中的html标签
 93                     value= filter(value);
 94                 }
 95                 
 96                 for(String dirtyWord : dirtyWords){
 97                     if(value.contains(dirtyWord)){
 98                         System.out.println("内容中包含敏感词："+dirtyWord+"，将会被替换成****");
 99                         //替换敏感字符
100                         value = value.replace(dirtyWord, "****");
101                     }
102                 }
103                 return value;
104             }catch (Exception e) {
105                 throw new RuntimeException(e);
106             }
107         }
108     }
109 
110     /**
111     * @Method: filter
112     * @Description: 过滤内容中的html标签
113     * @Anthor:孤傲苍狼
114     * @param value
115     * @return
116     */ 
117     public String filter(String value) {
118         if (value == null){
119             return null;
120         }
121         char content[] = new char[value.length()];
122         value.getChars(0, value.length(), content, 0);
123         StringBuffer result = new StringBuffer(content.length + 50);
124         for (int i = 0; i < content.length; i++) {
125             switch (content[i]) {
126             case '<':
127                 result.append("&lt;");
128                 break;
129             case '>':
130                 result.append("&gt;");
131                 break;
132             case '&':
133                 result.append("&amp;");
134                 break;
135             case '"':
136                 result.append("&quot;");
137                 break;
138             default:
139                 result.append(content[i]);
140             }
141         }
142         return (result.toString());
143     }
144     
145     /**
146     * @Method: getDirtyWords
147     * @Description: 获取敏感字符
148     * @Anthor:孤傲苍狼
149     *
150     * @return
151     */ 
152     private List<String> getDirtyWords(){
153         List<String> dirtyWords = new ArrayList<String>();
154         String dirtyWordPath = filterConfig.getInitParameter("dirtyWord");
155         InputStream inputStream = filterConfig.getServletContext().getResourceAsStream(dirtyWordPath);
156         InputStreamReader is = null;
157         try {
158             is = new InputStreamReader(inputStream,defaultCharset);
159         } catch (UnsupportedEncodingException e2) {
160             e2.printStackTrace();
161         }
162         BufferedReader reader = new BufferedReader(is);
163         String line;
164         try {
165             while ((line = reader.readLine())!= null) {//如果 line为空说明读完了
166                 dirtyWords.add(line);
167             }
168         } catch (IOException e) {
169             e.printStackTrace();
170         } 
171         return dirtyWords;
172     }
173 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml文件中配置AdvancedFilter

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1   <filter>
 2       <filter-name>AdvancedFilter</filter-name>
 3       <filter-class>me.gacl.web.filter.AdvancedFilter</filter-class>
 4       <init-param>
 5          <param-name>charset</param-name>    
 6          <param-value>UTF-8</param-value>
 7     </init-param>
 8       <init-param>
 9          <param-name>dirtyWord</param-name>    
10          <param-value>/WEB-INF/DirtyWord.txt</param-value>
11     </init-param>
12   </filter>
13   
14   <filter-mapping>
15       <filter-name>AdvancedFilter</filter-name>
16       <url-pattern>/*</url-pattern>
17   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　AdvancedFilter过滤器同时具有解决中文乱码，转义内容中的html标签，过滤内容中的敏感字符这些功能。

## 三、使用Decorator设计模式增强response对象

　　Servlet  API  中提供了response对象的Decorator设计模式的默认实现类HttpServletResponseWrapper  ，HttpServletResponseWrapper类实现了response接口中的所有方法，但这些方法的内部实现都是仅仅调用了一下所包装的的  response对象的对应方法，以避免用户在对response对象进行增强时需要实现response接口中的所有方法。

### 3.1、response增强案例——压缩响应正文内容

　　应用HttpServletResponseWrapper对象，压缩响应正文内容。

　　具体思路:通过filter向目标页面传递一个自定义的response对象。在自定义的response对象中，重写getOutputStream方法和getWriter方法，使目标资源调用此方法输出页面内容时，获得的是我们自定义的ServletOutputStream对象。在我们自定义的ServletOuputStream对象中，重写write方法，使写出的数据写出到一个buffer中。当页面完成输出后，在filter中就可得到页面写出的数据，从而我们可以调用GzipOuputStream对数据进行压缩后再写出给浏览器，以此完成响应正文件压缩功能。

　　编写压缩过滤器，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.ByteArrayOutputStream;
  4 import java.io.IOException;
  5 import java.io.OutputStreamWriter;
  6 import java.io.PrintWriter;
  7 import java.util.zip.GZIPOutputStream;
  8 
  9 import javax.servlet.Filter;
 10 import javax.servlet.FilterChain;
 11 import javax.servlet.FilterConfig;
 12 import javax.servlet.ServletException;
 13 import javax.servlet.ServletOutputStream;
 14 import javax.servlet.ServletRequest;
 15 import javax.servlet.ServletResponse;
 16 import javax.servlet.http.HttpServletRequest;
 17 import javax.servlet.http.HttpServletResponse;
 18 import javax.servlet.http.HttpServletResponseWrapper;
 19 
 20 /**
 21 * @ClassName: GzipFilter
 22 * @Description: 压缩过滤器，将web应用中的文本都经过压缩后再输出到浏览器
 23 * @author: 孤傲苍狼
 24 * @date: 2014-9-7 上午10:52:42
 25 *
 26 */ 
 27 public class GzipFilter implements Filter {
 28 
 29     public void doFilter(ServletRequest req, ServletResponse resp,
 30             FilterChain chain) throws IOException, ServletException {
 31         
 32         HttpServletRequest request = (HttpServletRequest) req;
 33         HttpServletResponse response = (HttpServletResponse) resp;
 34         
 35         BufferResponse myresponse = new BufferResponse(response);
 36         chain.doFilter(request, myresponse);
 37         //拿出缓存中的数据，压缩后再打给浏览器
 38         byte out[] = myresponse.getBuffer();
 39         System.out.println("原始大小:" + out.length);
 40         
 41         ByteArrayOutputStream bout = new ByteArrayOutputStream();
 42         //压缩输出流中的数据
 43         GZIPOutputStream gout = new GZIPOutputStream(bout);
 44         gout.write(out);
 45         gout.close();
 46         
 47         byte gzip[] = bout.toByteArray();
 48         System.out.println("压缩后的大小:" + gzip.length);
 49         
 50         response.setHeader("content-encoding", "gzip");
 51         response.setContentLength(gzip.length);
 52         response.getOutputStream().write(gzip);
 53     }
 54     
 55     public void destroy() {
 56         
 57     }
 58 
 59     public void init(FilterConfig filterConfig) throws ServletException {
 60         
 61     }
 62 }
 63 
 64 class BufferResponse extends HttpServletResponseWrapper{
 65 
 66     private ByteArrayOutputStream bout = new ByteArrayOutputStream();
 67     private PrintWriter pw;
 68     private HttpServletResponse response;
 69     public BufferResponse(HttpServletResponse response) {
 70         super(response);
 71         this.response = response;
 72     }
 73     @Override
 74     public ServletOutputStream getOutputStream() throws IOException {
 75         return new MyServletOutputStream(bout);
 76     }
 77     @Override
 78     public PrintWriter getWriter() throws IOException {
 79         pw = new PrintWriter(new OutputStreamWriter(bout,this.response.getCharacterEncoding()));
 80         return pw;
 81     }
 82     
 83     public byte[] getBuffer(){
 84         try{
 85             if(pw!=null){
 86                 pw.close();
 87             }
 88             if(bout!=null){
 89                 bout.flush();
 90                 return bout.toByteArray();
 91             }
 92             
 93             
 94             return null;
 95         }catch (Exception e) {
 96             throw new RuntimeException(e);
 97         }
 98     }
 99 }
100 
101 class MyServletOutputStream extends ServletOutputStream{
102 
103     private ByteArrayOutputStream bout;
104     public MyServletOutputStream(ByteArrayOutputStream bout){
105         this.bout = bout;
106     }
107     
108     @Override
109     public void write(int b) throws IOException {
110         this.bout.write(b);
111     }
112 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml中配置压缩过滤器

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  <filter>
 2       <description>配置压缩过滤器</description>
 3       <filter-name>GzipFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.GzipFilter</filter-class>
 5   </filter>
 6   
 7   <!--jsp文件的输出的内容都经过压缩过滤器压缩后才输出 -->
 8   <filter-mapping>
 9       <filter-name>GzipFilter</filter-name>
10       <url-pattern>*.jsp</url-pattern>
11       <!-- 配置过滤器的拦截方式-->
12       <!-- 对于在Servlet中通过
13           request.getRequestDispatcher("jsp页面路径").forward(request, response) 
14       方式访问的Jsp页面的要进行拦截 -->
15       <dispatcher>FORWARD</dispatcher>
16       <!--对于直接以URL方式访问的jsp页面进行拦截，过滤器的拦截方式默认就是 REQUEST-->
17       <dispatcher>REQUEST</dispatcher>
18   </filter-mapping>
19   <!--js文件的输出的内容都经过压缩过滤器压缩后才输出 -->
20   <filter-mapping>
21       <filter-name>GzipFilter</filter-name>
22       <url-pattern>*.js</url-pattern>
23   </filter-mapping>
24   <!--css文件的输出的内容都经过压缩过滤器压缩后才输出 -->
25   <filter-mapping>
26       <filter-name>GzipFilter</filter-name>
27       <url-pattern>*.css</url-pattern>
28   </filter-mapping>
29   <!--html文件的输出的内容都经过压缩过滤器压缩后才输出 -->
30   <filter-mapping>
31       <filter-name>GzipFilter</filter-name>
32       <url-pattern>*.html</url-pattern>
33   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

###  3.2、response增强案例——缓存数据到内存

　　对于页面中很少更新的数据，例如商品分类，为避免每次都要从数据库查询分类数据，因此可把分类数据缓存在内存或文件中，以此来减轻数据库压力，提高系统响应速度。

　　编写缓存数据的过滤器，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.filter;
  2 
  3 import java.io.ByteArrayOutputStream;
  4 import java.io.IOException;
  5 import java.io.OutputStreamWriter;
  6 import java.io.PrintWriter;
  7 import java.util.HashMap;
  8 import java.util.Map;
  9 
 10 import javax.servlet.Filter;
 11 import javax.servlet.FilterChain;
 12 import javax.servlet.FilterConfig;
 13 import javax.servlet.ServletException;
 14 import javax.servlet.ServletOutputStream;
 15 import javax.servlet.ServletRequest;
 16 import javax.servlet.ServletResponse;
 17 import javax.servlet.http.HttpServletRequest;
 18 import javax.servlet.http.HttpServletResponse;
 19 import javax.servlet.http.HttpServletResponseWrapper;
 20 
 21 /**
 22 * @ClassName: WebResourceCachedFilter
 23 * @Description: Web资源缓存过滤器
 24 * @author: 孤傲苍狼
 25 * @date: 2014-9-8 上午12:20:16
 26 *
 27 */ 
 28 public class WebResourceCachedFilter implements Filter {
 29     /**
 30     * @Field: map
 31     *          缓存Web资源的Map容器
 32     */ 
 33     private Map<String,byte[]> map = new HashMap<String,byte[]>();
 34     
 35     @Override
 36     public void init(FilterConfig filterConfig) throws ServletException {
 37 
 38     }
 39 
 40     @Override
 41     public void doFilter(ServletRequest req, ServletResponse resp,
 42             FilterChain chain) throws IOException, ServletException {
 43         HttpServletRequest request = (HttpServletRequest) req;
 44         HttpServletResponse response = (HttpServletResponse) resp;
 45         //1.得到用户请求的uri
 46         String uri = request.getRequestURI();
 47         //2.看缓存中有没有uri对应的数据
 48         byte b[] = map.get(uri);
 49         //3.如果缓存中有，直接拿缓存的数据打给浏览器，程序返回
 50         if(b!=null){
 51             //根据字节数组和指定的字符编码构建字符串
 52             String webResourceHtmlStr = new String(b,response.getCharacterEncoding());
 53             System.out.println(webResourceHtmlStr);
 54             response.getOutputStream().write(b);
 55             return;
 56         }
 57         //4.如果缓存没有，让目标资源执行，并捕获目标资源的输出
 58         BufferResponse myresponse = new BufferResponse(response);
 59         chain.doFilter(request, myresponse);
 60         //获取缓冲流中的内容的字节数组
 61         byte out[] = myresponse.getBuffer();
 62         //5.把资源的数据以用户请求的uri为关键字保存到缓存中
 63         map.put(uri, out);
 64         //6.把数据打给浏览器
 65         response.getOutputStream().write(out);
 66     }
 67 
 68     @Override
 69     public void destroy() {
 70 
 71     }
 72 
 73     class BufferResponse extends HttpServletResponseWrapper{
 74         private ByteArrayOutputStream bout = new ByteArrayOutputStream();  //捕获输出的缓存
 75         private PrintWriter pw;
 76         private HttpServletResponse response;
 77         public BufferResponse(HttpServletResponse response) {
 78             super(response);
 79             this.response = response;
 80         }
 81         @Override
 82         public ServletOutputStream getOutputStream() throws IOException {
 83             return new MyServletOutputStream(bout);
 84         }
 85         @Override
 86         public PrintWriter getWriter() throws IOException {
 87             pw = new PrintWriter(new OutputStreamWriter(bout,this.response.getCharacterEncoding()));
 88             return pw;
 89         }
 90         
 91         public byte[] getBuffer(){
 92             try{
 93                 if(pw!=null){
 94                     pw.close();
 95                 }
 96                 return bout.toByteArray();
 97             }catch (Exception e) {
 98                 throw new RuntimeException(e);
 99             }
100         }
101     }
102 
103     class MyServletOutputStream extends ServletOutputStream{
104         private ByteArrayOutputStream bout;
105         public MyServletOutputStream(ByteArrayOutputStream bout){  //接收数据写到哪里
106             this.bout = bout;
107         }
108         @Override
109         public void write(int b) throws IOException {
110             bout.write(b);
111         }
112     }
113 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在web.xml中配置Web资源缓存过滤器

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1   <filter>
 2        <description>Web资源缓存过滤器</description>
 3       <filter-name>WebResourceCachedFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.WebResourceCachedFilter</filter-class>
 5   </filter>
 6   
 7   <filter-mapping>
 8       <filter-name>WebResourceCachedFilter</filter-name>
 9       <!-- 映射需要缓存输出的JSP页面，这几个页面都只是单纯作为输入UI，不会有太多的变化，因此可以缓存输出 -->
10       <url-pattern>/login.jsp</url-pattern>
11       <url-pattern>/test.jsp</url-pattern>
12       <url-pattern>/test2.jsp</url-pattern>
13   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)