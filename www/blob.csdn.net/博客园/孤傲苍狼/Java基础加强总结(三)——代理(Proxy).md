# Java基础加强总结(三)——代理(Proxy) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Java基础加强总结(三)——代理(Proxy)](https://www.cnblogs.com/xdp-gacl/p/3971367.html)



## 一、代理的概念

　　动态代理技术是整个java技术中最重要的一个技术，它是学习java框架的基础，不会动态代理技术，那么在学习Spring这些框架时是学不明白的。

**动态代理技术就是用来产生一个对象的代理对象的**。在开发中为什么需要为一个对象产生代理对象呢？
　　举一个现实生活中的例子：歌星或者明星都有一个自己的经纪人，这个经纪人就是他们的代理人，当我们需要找明星表演时，不能直接找到该明星，只能是找明星的代理人。比如刘德华在现实生活中非常有名，会唱歌，会跳舞，会拍戏，刘德华在没有出名之前，我们可以直接找他唱歌，跳舞，拍戏，刘德华出名之后，他干的第一件事就是找一个经纪人，这个经纪人就是刘德华的代理人(代理)，当我们需要找刘德华表演时，不能直接找到刘德华了(刘德华说，你找我代理人商谈具体事宜吧!)，只能是找刘德华的代理人，因此刘德华这个代理人存在的价值就是拦截我们对刘德华的直接访问！
　　这个现实中的例子和我们在开发中是一样的，我们在开发中之所以要产生一个对象的代理对象，主要用于拦截对真实业务对象的访问。那么代理对象应该具有什么方法呢？代理对象应该具有和目标对象相同的方法

　　所以在这里明确代理对象的两个概念：
　　　　1、**代理对象存在的价值主要用于拦截对真实业务对象的访问**。
　　　　2、**代理对象应该具有和目标对象(真实业务对象)相同的方法**。刘德华**(真实业务对象)**会唱歌，会跳舞，会拍戏，我们现在不能直接找他唱歌，跳舞，拍戏了，只能找他的代理人(**代理对象**)唱歌，跳舞，拍戏，一个人要想成为刘德华的代理人，那么他必须具有和刘德华一样的行为(会唱歌，会跳舞，会拍戏)，刘德华有什么方法，他(代理人)就要有什么方法，我们找刘德华的代理人唱歌，跳舞，拍戏，但是代理人不是真的懂得唱歌，跳舞，拍戏的，真正懂得唱歌，跳舞，拍戏的是刘德华，在现实中的例子就是我们要找刘德华唱歌，跳舞，拍戏，那么只能先找他的经纪人，交钱给他的经纪人，然后经纪人再让刘德华去唱歌，跳舞，拍戏。

## 二、java中的代理

### 2.1、"**java.lang.reflect.Proxy**"类介绍

**现在要生成某一个对象的代理对象，这个代理对象通常也要编写一个类来生成**，所以首先要编写用于生成代理对象的类。在java中如何用程序去生成一个对象的代理对象呢，java在JDK1.5之后提供了一个"**java.lang.reflect.Proxy**"类，通过"**Proxy**"类提供的一个**newProxyInstance**方法用来创建一个对象的代理对象，如下所示：

```
1 static Object **newProxyInstance**(ClassLoader loader, Class<?>[] interfaces, InvocationHandler h)
```

**　　newProxyInstance**方法用来返回一个代理对象，这个方法总共有3个参数，ClassLoader loader用来指明生成代理对象使用哪个类装载器，Class<?>[] interfaces用来指明生成哪个对象的代理对象，通过接口指定，InvocationHandler h用来指明产生的这个代理对象要做什么事情。所以我们只需要调用**newProxyInstance**方法就可以得到某一个对象的代理对象了。

### 2.2、编写生成代理对象的类

**在java中规定，要想产生一个对象的代理对象，那么这个对象必须要有一个接口**，所以我们第一步就是设计这个对象的接口，在接口中定义这个对象所具有的行为(方法)

**　　1、定义对象的行为接口**

```
1 package cn.gacl.proxy;
 2 
 3 /**
 4 * @ClassName: Person
 5 * @Description: 定义对象的行为
 6 * @author: 孤傲苍狼
 7 * @date: 2014-9-14 下午9:44:22
 8 *
 9 */ 
10 public interface Person {
11 
12     /**
13     * @Method: sing
14     * @Description: 唱歌
15     * @Anthor:孤傲苍狼
16     *
17     * @param name
18     * @return
19     */ 
20     String sing(String name);
21     /**
22     * @Method: sing
23     * @Description: 跳舞
24     * @Anthor:孤傲苍狼
25     *
26     * @param name
27     * @return
28     */ 
29     String dance(String name);
30 }
```

**2、定义目标业务对象类**

```
1 package cn.gacl.proxy;
 2 
 3 /**
 4 * @ClassName: LiuDeHua
 5 * @Description: 刘德华实现Person接口，那么刘德华会唱歌和跳舞了
 6 * @author: 孤傲苍狼
 7 * @date: 2014-9-14 下午9:22:24
 8 *
 9 */ 
10 public class LiuDeHua implements Person {
11 
12     public String sing(String name){
13         System.out.println("刘德华唱"+name+"歌！！");
14         return "歌唱完了，谢谢大家！";
15     }
16     
17     public String dance(String name){
18         System.out.println("刘德华跳"+name+"舞！！");
19         return "舞跳完了，多谢各位观众！";
20     }
21 }
```

**　　3、创建生成代理对象的代理类**

```
1 package cn.gacl.proxy;
 2 
 3 import java.lang.reflect.InvocationHandler;
 4 import java.lang.reflect.Method;
 5 import java.lang.reflect.Proxy;
 6 
 7 /**
 8 * @ClassName: LiuDeHuaProxy
 9 * @Description: 这个代理类负责生成刘德华的代理人
10 * @author: 孤傲苍狼
11 * @date: 2014-9-14 下午9:50:02
12 *
13 */ 
14 public class LiuDeHuaProxy {
15 
16     //设计一个类变量记住代理类要代理的目标对象
17     private Person ldh = new LiuDeHua();
18     
19     /**
20     * 设计一个方法生成代理对象
21     * @Method: getProxy
22     * @Description: 这个方法返回刘德华的代理对象：Person person = LiuDeHuaProxy.getProxy();//得到一个代理对象
23     * @Anthor:孤傲苍狼
24     *
25     * @return 某个对象的代理对象
26     */ 
27     public Person getProxy() {
28         //使用Proxy.newProxyInstance(ClassLoader loader, Class<?>[] interfaces, InvocationHandler h)返回某个对象的代理对象
29         return (Person) Proxy.newProxyInstance(LiuDeHuaProxy.class
30                 .getClassLoader(), ldh.getClass().getInterfaces(),
31                 new InvocationHandler() {
32                     /**
33                      * InvocationHandler接口只定义了一个invoke方法，因此对于这样的接口，我们不用单独去定义一个类来实现该接口，
34                      * 而是直接使用一个匿名内部类来实现该接口，new InvocationHandler() {}就是针对InvocationHandler接口的匿名实现类
35                      */
36                     /**
37                      * 在invoke方法编码指定返回的代理对象干的工作
38                      * proxy : 把代理对象自己传递进来 
39                      * method：把代理对象当前调用的方法传递进来 
40                      * args:把方法参数传递进来
41                      * 
42                      * 当调用代理对象的person.sing("冰雨");或者 person.dance("江南style");方法时，
43                      * 实际上执行的都是invoke方法里面的代码，
44                      * 因此我们可以在invoke方法中使用method.getName()就可以知道当前调用的是代理对象的哪个方法
45                      */
46                     @Override
47                     public Object invoke(Object proxy, Method method,
48                             Object[] args) throws Throwable {
49                         //如果调用的是代理对象的sing方法
50                         if (method.getName().equals("sing")) {
51                             System.out.println("我是他的经纪人，要找他唱歌得先给十万块钱！！");
52                             //已经给钱了，经纪人自己不会唱歌，就只能找刘德华去唱歌！
53                             return method.invoke(ldh, args); //代理对象调用真实目标对象的sing方法去处理用户请求
54                         }
55                         //如果调用的是代理对象的dance方法
56                         if (method.getName().equals("dance")) {
57                             System.out.println("我是他的经纪人，要找他跳舞得先给二十万块钱！！");
58                             //已经给钱了，经纪人自己不会唱歌，就只能找刘德华去跳舞！
59                             return method.invoke(ldh, args);//代理对象调用真实目标对象的dance方法去处理用户请求
60                         }
61 
62                         return null;
63                     }
64                 });
65     }
66 }
```

　　测试代码：

```
1 package cn.gacl.proxy;
 2 
 3 public class ProxyTest {
 4     
 5     public static void main(String[] args) {
 6         
 7         LiuDeHuaProxy proxy = new LiuDeHuaProxy();
 8         //获得代理对象
 9         Person p = proxy.getProxy();
10         //调用代理对象的sing方法
11         String retValue = p.sing("冰雨");
12         System.out.println(retValue);
13         //调用代理对象的dance方法
14         String value = p.dance("江南style");
15         System.out.println(value);
16     }
17 }
```

　　运行结果如下：

![](data:image/png;base64,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)
　　Proxy类负责创建代理对象时，如果指定了handler（处理器），那么不管用户调用代理对象的什么方法，该方法都是调用处理器的invoke方法。
　　由于invoke方法被调用需要三个参数：代理对象、方法、方法的参数，因此不管代理对象哪个方法调用处理器的invoke方法，都必须把自己所在的对象、自己（调用invoke方法的方法）、方法的参数传递进来。

## 三、动态代理应用

**在动态代理技术里，由于不管用户调用代理对象的什么方法，都是调用开发人员编写的处理器的invoke方法（这相当于invoke方法拦截到了代理对象的方法调用）**。并且，开发人员通过invoke方法的参数，还可以在拦截的同时，知道用户调用的是什么方法，因此利用这两个特性，就可以实现一些特殊需求，例如：拦截用户的访问请求，以检查用户是否有访问权限、动态为某个对象添加额外的功能。

### 3.1、在字符过滤器中使用动态代理解决中文乱码

　　在平时的JavaWeb项目开发中，我们一般会写一个CharacterEncodingFilter(字符过滤器)来解决整个JavaWeb应用的中文乱码问题，如下所示：

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
11 
12 /**
13 * @ClassName: CharacterEncodingFilter
14 * @Description: 解决中文乱码的字符过滤器
15 * @author: 孤傲苍狼
16 * @date: 2014-9-14 下午10:38:12
17 *
18 */ 
19 public class CharacterEncodingFilter implements Filter {
20 
21     @Override
22     public void init(FilterConfig filterConfig) throws ServletException {
23 
24     }
25 
26     @Override
27     public void doFilter(ServletRequest request, ServletResponse response,
28             FilterChain chain) throws IOException, ServletException {
29         //解决以Post方式提交的中文乱码问题
30         request.setCharacterEncoding("UTF-8");
31         response.setCharacterEncoding("UTF-8");
32         response.setContentType("text/html;charset=UTF-8");
33         chain.doFilter(request, response);
34     }
35 
36     @Override
37     public void destroy() {
38 
39     }
40 }
```

　　但是这种写法是没有办法解决以get方式提交中文参数时的乱码问题的，我们可以用如下的代码来证明上述的解决中文乱码过滤器只对以post方式提交中文参数时有效，而对于以get方式提交中文参数时无效

　　jsp测试页面如下：

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <%--引入jstl标签库 --%>
 3 <%@taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
 4 <!DOCTYPE HTML>
 5 <html>
 6   <head>
 7     <title>使用字符过滤器解决解决get、post请求方式下的中文乱码问题</title>
 8   </head>
 9   <body>
10        <%--使用c:url标签构建url，构建好的url存储在servletDemo1变量中--%>
11        <c:url value="/servlet/ServletDemo1" scope="page" var="servletDemo1">
12            <%--构建的url的附带的中文参数 ，参数名是：username，值是：孤傲苍狼--%>
13            <c:param name="username" value="孤傲苍狼"></c:param>
14        </c:url>
15       <%--使用get的方式访问 --%>
16        <a href="${servletDemo1}">超链接(get方式请求)</a>
17        <hr/>
18        <%--使用post方式提交表单 --%>
19        <form action="${pageContext.request.contextPath}/servlet/ServletDemo1" method="post">
20            用户名：<input type="text" name="username" value="孤傲苍狼" />
21            <input type="submit" value="post方式提交">
22        </form>
23        
24   </body>
25 </html>
```

　　处理请求的ServletDemo1代码如下：

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
15         // 接收参数
16         String username = request.getParameter("username");
17         // 获取请求方式
18         String method = request.getMethod();
19         // 获取输出流
20         PrintWriter out = response.getWriter();
21         out.write("请求的方式：" + method);
22         out.write("<br/>");
23         out.write("接收到的参数：" + username);
24     }
25 
26     public void doPost(HttpServletRequest request, HttpServletResponse response)
27             throws ServletException, IOException {
28         doGet(request, response);
29     }
30 }
```

　　在web.xml中注册上述的CharacterEncodingFilter和ServletDemo1

```
1  <filter>
 2       <filter-name>CharacterEncodingFilter</filter-name>
 3       <filter-class>me.gacl.web.filter.CharacterEncodingFilter</filter-class>
 4   </filter>
 5   
 6   <filter-mapping>
 7       <filter-name>CharacterEncodingFilter</filter-name>
 8       <url-pattern>/*</url-pattern>
 9   </filter-mapping>
10   
11   <servlet>
12     <servlet-name>ServletDemo1</servlet-name>
13     <servlet-class>me.gacl.web.controller.ServletDemo1</servlet-class>
14   </servlet>
15 
16   <servlet-mapping>
17     <servlet-name>ServletDemo1</servlet-name>
18     <url-pattern>/servlet/ServletDemo1</url-pattern>
19   </servlet-mapping>
```

　　测试结果如下所示：

![](https://images0.cnblogs.com/blog/289233/201409/142254370599503.gif)

　　从运行结果可以看出，上述的过滤器的确是不能解决以get方式提交中文参数的乱码问题，下面使用动态代理技术改造上述的过滤器，使之能够解决以get方式提交中文参数的乱码问题，改造后的过滤器代码如下：

```
1 package me.gacl.web.filter;
 2 
 3 import java.io.IOException;
 4 import java.lang.reflect.InvocationHandler;
 5 import java.lang.reflect.Method;
 6 import java.lang.reflect.Proxy;
 7 
 8 import javax.servlet.Filter;
 9 import javax.servlet.FilterChain;
10 import javax.servlet.FilterConfig;
11 import javax.servlet.ServletException;
12 import javax.servlet.ServletRequest;
13 import javax.servlet.ServletResponse;
14 import javax.servlet.http.HttpServletRequest;
15 import javax.servlet.http.HttpServletResponse;
16 
17 /**
18 * @ClassName: CharacterEncodingFilter
19 * @Description: 解决中文乱码的字符过滤器
20 * @author: 孤傲苍狼
21 * @date: 2014-9-14 下午10:38:12
22 *
23 */ 
24 public class CharacterEncodingFilter implements Filter {
25 
26     @Override
27     public void init(FilterConfig filterConfig) throws ServletException {
28 
29     }
30 
31     @Override
32     public void doFilter(ServletRequest req, ServletResponse resp,
33             FilterChain chain) throws IOException, ServletException {
34         
35         final HttpServletRequest request = (HttpServletRequest) req;
36         HttpServletResponse response = (HttpServletResponse) resp;
37         //解决以Post方式提交的中文乱码问题
38         request.setCharacterEncoding("UTF-8");
39         response.setCharacterEncoding("UTF-8");
40         response.setContentType("text/html;charset=UTF-8");
41         //获取获取HttpServletRequest对象的代理对象
42         ServletRequest requestProxy = getHttpServletRequestProxy(request);
43         /**
44          * 传入代理对象requestProxy给doFilter方法，
45          * 这样用户在使用request对象时实际上使用的是HttpServletRequest对象的代理对象requestProxy
46          */
47         chain.doFilter(requestProxy, response);
48     }
49 
50     
51     /**
52     * @Method: getHttpServletRequestProxy
53     * @Description: 获取HttpServletRequest对象的代理对象
54     * @Anthor:孤傲苍狼
55     *
56     * @param request
57     * @return HttpServletRequest对象的代理对象
58     */ 
59     private ServletRequest getHttpServletRequestProxy(final HttpServletRequest request){
60         ServletRequest proxy  = (ServletRequest) Proxy.newProxyInstance(
61                 CharacterEncodingFilter.class.getClassLoader(),
62                 request.getClass().getInterfaces(),
63                 new InvocationHandler(){
64                     @Override
65                     public Object invoke(Object proxy, Method method, Object[] args)
66                             throws Throwable {
67                         //如果请求方式是get并且调用的是getParameter方法
68                         if (request.getMethod().equalsIgnoreCase("get") && method.getName().equals("getParameter")) {
69                             //调用getParameter方法获取参数的值
70                             String value = (String) method.invoke(request, args);
71                             if(value==null){
72                                 return null;
73                             }
74                             //解决以get方式提交的中文乱码问题
75                             return new String(value.getBytes("iso8859-1"),"UTF-8");
76                         }else {
77                             //直接调用相应的方法进行处理
78                             return method.invoke(request, args);
79                         }
80                     }
81                 });
82         //返回HttpServletRequest对象的代理对象
83         return proxy;
84     }
85     
86     @Override
87     public void destroy() {
88 
89     }
90 }
```

　　我们在过滤器中使用动态代理技术生成一个HttpServletRequest对象的代理对象requestProxy，然后把代理对象requestProxy进行chain.doFilter(requestProxy, response)传递给用户使用，这样用户实际上使用的就是HttpServletRequest对象的代理对象requestProxy。然而这一过程对于用户来说是透明的，用户是不知道自己使用的HttpServletRequest对象是一个代理对象requestProxy，由于代理对象requestProxy和目标对象HttpServletRequest具有相同的方法，当用户调用getParameter方法接收中文参数时，实际上调用的就是代理对象requestProxy的invoke方法，因此我们就可以在invoke方法中就判断当前的请求方式以及用户正在调用的方法，如果判断当前的请求方式是get方式并且用户正在调用的是getParameter方法，那么我们就可以手动处理get方式提交中文参数的中文乱码问题了。

　　测试结果如下所示：

![](https://images0.cnblogs.com/blog/289233/201409/142325431681431.gif)

###  3.2、在字符过滤器中使用动态代理压缩服务器响应的内容后再输出到客户端

　　压缩过滤器的代码如下：

```
1 package me.gacl.web.filter;
  2 
  3 import java.io.ByteArrayOutputStream;
  4 import java.io.IOException;
  5 import java.io.OutputStreamWriter;
  6 import java.io.PrintWriter;
  7 import java.lang.reflect.InvocationHandler;
  8 import java.lang.reflect.Method;
  9 import java.lang.reflect.Proxy;
 10 import java.util.zip.GZIPOutputStream;
 11 
 12 import javax.servlet.Filter;
 13 import javax.servlet.FilterChain;
 14 import javax.servlet.FilterConfig;
 15 import javax.servlet.ServletException;
 16 import javax.servlet.ServletOutputStream;
 17 import javax.servlet.ServletRequest;
 18 import javax.servlet.ServletResponse;
 19 import javax.servlet.http.HttpServletRequest;
 20 import javax.servlet.http.HttpServletResponse;
 21 
 22 /**
 23 * @ClassName: GzipFilter
 24 * @Description: 压缩过滤器，将web应用中的文本都经过压缩后再输出到浏览器
 25 * @author: 孤傲苍狼
 26 * @date: 2014-9-15 下午9:35:36
 27 *
 28 */ 
 29 public class GzipFilter implements Filter {
 30 
 31     @Override
 32     public void init(FilterConfig filterConfig) throws ServletException {
 33 
 34     }
 35 
 36     @Override
 37     public void doFilter(ServletRequest req, ServletResponse resp,
 38             FilterChain chain) throws IOException, ServletException {
 39         
 40         final HttpServletRequest request = (HttpServletRequest) req;
 41         final HttpServletResponse response = (HttpServletResponse) resp;
 42         final ByteArrayOutputStream bout = new ByteArrayOutputStream();
 43         final PrintWriter pw = new PrintWriter(new OutputStreamWriter(bout,"UTF-8"));
 44         
 45         chain.doFilter(request, getHttpServletResponseProxy(response, bout, pw));
 46         pw.close();
 47         //拿到目标资源的输出
 48         byte result[] = bout.toByteArray();
 49         System.out.println("原始大小：" + result.length);
 50         
 51         ByteArrayOutputStream bout2 = new ByteArrayOutputStream();
 52         GZIPOutputStream gout = new GZIPOutputStream(bout2);
 53         gout.write(result);
 54         gout.close();
 55         
 56         //拿到目标资源输出的压缩数据
 57         byte gzip[] = bout2.toByteArray();
 58         System.out.println("压缩大小：" + gzip.length);
 59         
 60         response.setHeader("content-encoding", "gzip");
 61         response.setContentLength(gzip.length);
 62         response.getOutputStream().write(gzip);
 63     }
 64 
 65     /**
 66     * @Method: getHttpServletResponseProxy
 67     * @Description: 获取HttpServletResponse对象的代理对象
 68     * @Anthor:孤傲苍狼
 69     *
 70     * @param response
 71     * @param bout
 72     * @param pw
 73     * @return HttpServletResponse对象的代理对象
 74     */ 
 75     private ServletResponse getHttpServletResponseProxy(
 76             final HttpServletResponse response,
 77             final ByteArrayOutputStream bout, 
 78             final PrintWriter pw) {
 79         
 80         return (ServletResponse) Proxy.newProxyInstance(GzipFilter.class.getClassLoader(), 
 81                 response.getClass().getInterfaces(), 
 82                 new InvocationHandler(){
 83                     @Override
 84                     public Object invoke(Object proxy, Method method, Object[] args)
 85                             throws Throwable {
 86                         if(method.getName().equals("getWriter")){
 87                             return pw; 
 88                         }else if(method.getName().equals("getOutputStream")){
 89                             return new MyServletOutputStream(bout);
 90                         }else{
 91                             return method.invoke(response, args);
 92                         }
 93                     }
 94                 });
 95     }
 96 
 97     @Override
 98     public void destroy() {
 99 
100     }
101 
102     class MyServletOutputStream extends ServletOutputStream{
103         
104         private ByteArrayOutputStream  bout = null;
105         public MyServletOutputStream(ByteArrayOutputStream  bout){
106             this.bout = bout;
107         }
108         @Override
109         public void write(int b) throws IOException {
110             bout.write(b);
111         }
112         
113     }
114 }
```

　　在web.xml中注册上述的GzipFilter

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
16       <!--对于直接以URL方式访问的jsp页面进行拦截，过滤器的拦截方式默认就是REQUEST-->
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

　　GzipFilter过滤器会将*.jsp，*.js，*.css，*.html这些文件里面的文本内容都经过压缩后再输出到客户端显示。









