## [javaweb学习总结(四十六)——Filter(过滤器)常见应用](https://www.cnblogs.com/xdp-gacl/p/3948422.html)

## 一、统一全站字符编码

　　通过配置参数charset指明使用何种字符编码,以处理Html Form请求参数的中文问题

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.filter;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.Filter;
 5 import javax.servlet.FilterChain;
 6 import javax.servlet.FilterConfig;
 7 import javax.servlet.ServletException;
 8 import javax.servlet.ServletRequest;
 9 import javax.servlet.ServletResponse;
10 import javax.servlet.http.HttpServletRequest;
11 import javax.servlet.http.HttpServletRequestWrapper;
12 import javax.servlet.http.HttpServletResponse;
13 
14 /**
15 * @ClassName: CharacterEncodingFilter
16 * @Description: 此过滤器用来解决全站中文乱码问题
17 * @author: 孤傲苍狼
18 * @date: 2014-8-31 下午11:09:37
19 *
20 */ 
21 public class CharacterEncodingFilter implements Filter {
22 
23     private FilterConfig filterConfig = null;
24     //设置默认的字符编码
25     private String defaultCharset = "UTF-8";
26 
27     public void doFilter(ServletRequest req, ServletResponse resp,
28             FilterChain chain) throws IOException, ServletException {
29         
30         HttpServletRequest request = (HttpServletRequest) req;
31         HttpServletResponse response = (HttpServletResponse) resp;
32         String charset = filterConfig.getInitParameter("charset");
33         if(charset==null){
34             charset = defaultCharset;
35         }
36         request.setCharacterEncoding(charset);
37         response.setCharacterEncoding(charset);
38         response.setContentType("text/html;charset="+charset);
39         
40         MyCharacterEncodingRequest requestWrapper = new MyCharacterEncodingRequest(request);
41         chain.doFilter(requestWrapper, response);
42     }
43 
44     public void init(FilterConfig filterConfig) throws ServletException {
45         //得到过滤器的初始化配置信息
46         this.filterConfig = filterConfig;
47     }
48     
49     public void destroy() {
50 
51     }
52 }
53 
54 /*
55 1.实现与被增强对象相同的接口 
56 2、定义一个变量记住被增强对象
57 3、定义一个构造器，接收被增强对象
58 4、覆盖需要增强的方法
59 5、对于不想增强的方法，直接调用被增强对象（目标对象）的方法
60  */
61  
62 class MyCharacterEncodingRequest extends HttpServletRequestWrapper{
63     
64     private HttpServletRequest request;
65     public MyCharacterEncodingRequest(HttpServletRequest request) {
66         super(request);
67         this.request = request;
68     }
69     /* 重写getParameter方法
70      * @see javax.servlet.ServletRequestWrapper#getParameter(java.lang.String)
71      */
72     @Override
73     public String getParameter(String name) {
74         
75         try{
76             //获取参数的值
77             String value= this.request.getParameter(name);
78             if(value==null){
79                 return null;
80             }
81             //如果不是以get方式提交数据的，就直接返回获取到的值
82             if(!this.request.getMethod().equalsIgnoreCase("get")) {
83                 return value;
84             }else{
85                 //如果是以get方式提交数据的，就对获取到的值进行转码处理
86                 value = new String(value.getBytes("ISO8859-1"),this.request.getCharacterEncoding());
87                 return value;
88             }
89         }catch (Exception e) {
90             throw new RuntimeException(e);
91         }
92     }
93 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　web.xml文件中的配置如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  <filter>
 2       <filter-name>CharacterEncodingFilter</filter-name>
 3       <filter-class>me.gacl.web.filter.CharacterEncodingFilter</filter-class>
 4       <init-param>
 5           <param-name>charset</param-name>
 6           <param-value>UTF-8</param-value>
 7       </init-param>
 8   </filter>
 9   
10   <filter-mapping>
11       <filter-name>CharacterEncodingFilter</filter-name>
12       <url-pattern>/*</url-pattern>
13   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 二、禁止浏览器缓存所有动态页面

　　有3 个HTTP 响应头字段都可以禁止浏览器缓存当前页面，它们在 Servlet 中的示例代码如下：

```
1 response.setDateHeader("Expires",-1);
2 response.setHeader("Cache-Control","no-cache");
3 response.setHeader("Pragma","no-cache"); 
```

　　并不是所有的浏览器都能完全支持上面的三个响应头，因此最好是同时使用上面的三个响应头。

- Expires数据头：值为GMT时间值，为-1指浏览器不要缓存页面
- Cache-Control响应头有两个常用值：
- no-cache指浏览器不要缓存当前页面。
- max-age:xxx指浏览器缓存页面xxx秒。

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
12 import javax.servlet.http.HttpServletResponse;
13 
14 /**
15 * @ClassName: NoCacheFilter
16 * @Description: 禁止浏览器缓存所有动态页面
17 * @author: 孤傲苍狼
18 * @date: 2014-8-31 下午11:25:40
19 *
20 */ 
21 public class NoCacheFilter implements Filter {
22 
23 
24     public void doFilter(ServletRequest req, ServletResponse resp,
25             FilterChain chain) throws IOException, ServletException {
26         //把ServletRequest强转成HttpServletRequest
27         HttpServletRequest request = (HttpServletRequest) req;
28         //把ServletResponse强转成HttpServletResponse
29         HttpServletResponse response = (HttpServletResponse) resp;
30         //禁止浏览器缓存所有动态页面
31         response.setDateHeader("Expires", -1);
32         response.setHeader("Cache-Control", "no-cache");
33         response.setHeader("Pragma", "no-cache");
34         
35         chain.doFilter(request, response);
36     }
37 
38     public void init(FilterConfig filterConfig) throws ServletException {
39 
40     }
41     
42     public void destroy() {
43         
44     }
45 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　web.xml文件中的配置如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1   <filter>
 2       <filter-name>NoCacheFilter</filter-name>
 3       <filter-class>me.gacl.web.filter.NoCacheFilter</filter-class>
 4   </filter>
 5   
 6   <filter-mapping>
 7       <filter-name>NoCacheFilter</filter-name>
 8         <!--只拦截Jsp请求-->
 9       <servlet-name>*.jsp</servlet-name>
10   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 三、控制浏览器缓存页面中的静态资源

　　有些动态页面中引用了一些图片或css文件以修饰页面效果，这些图片和css文件经常是不变化的，所以为减轻服务器的压力，可以使用filter控制浏览器缓存这些文件，以提升服务器的性能。

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
12 import javax.servlet.http.HttpServletResponse;
13 
14 /**
15 * @ClassName: CacheFilter
16 * @Description: 控制缓存的filter
17 * @author: 孤傲苍狼
18 * @date: 2014-9-1 下午9:39:38
19 *
20 */ 
21 public class CacheFilter implements Filter {
22 
23     private FilterConfig filterConfig;
24 
25     public void doFilter(ServletRequest req, ServletResponse resp,
26             FilterChain chain) throws IOException, ServletException {
27     
28         HttpServletRequest request = (HttpServletRequest) req;
29         HttpServletResponse response = (HttpServletResponse) resp;
30         
31         //1.获取用户想访问的资源
32         String uri = request.getRequestURI(); 
33         
34         //2.得到用户想访问的资源的后缀名
35         String ext = uri.substring(uri.lastIndexOf(".")+1);
36         
37         //得到资源需要缓存的时间
38         String time = filterConfig.getInitParameter(ext);
39         if(time!=null){
40             long t = Long.parseLong(time)*3600*1000;
41             //设置缓存
42             response.setDateHeader("expires", System.currentTimeMillis() + t);
43         }
44         
45         chain.doFilter(request, response);
46 
47     }
48 
49     public void init(FilterConfig filterConfig) throws ServletException {
50         this.filterConfig = filterConfig;
51     }
52     
53     public void destroy() {
54         
55     }
56 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　web.xml文件中的配置如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  <!-- 配置缓存过滤器 -->
 2    <filter>
 3       <filter-name>CacheFilter</filter-name>
 4       <filter-class>me.gacl.web.filter.CacheFilter</filter-class>
 5        <!-- 配置要缓存的web资源以及缓存时间，以小时为单位 -->
 6       <init-param>
 7           <param-name>css</param-name>
 8           <param-value>4</param-value>
 9       </init-param>
10       <init-param>
11           <param-name>jpg</param-name>
12           <param-value>1</param-value>
13       </init-param>
14       <init-param>
15           <param-name>js</param-name>
16           <param-value>4</param-value>
17       </init-param>
18       <init-param>
19           <param-name>png</param-name>
20           <param-value>4</param-value>
21       </init-param>
22   </filter>
23   <!-- 配置要缓存的web资源的后缀-->
24   <filter-mapping>
25       <filter-name>CacheFilter</filter-name>
26       <url-pattern>*.jpg</url-pattern>
27   </filter-mapping>
28   
29   <filter-mapping>
30       <filter-name>CacheFilter</filter-name>
31       <url-pattern>*.css</url-pattern>
32   </filter-mapping>
33   
34   <filter-mapping>
35       <filter-name>CacheFilter</filter-name>
36       <url-pattern>*.js</url-pattern>
37   </filter-mapping>
38    <filter-mapping>
39       <filter-name>CacheFilter</filter-name>
40       <url-pattern>*.png</url-pattern>
41   </filter-mapping>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 四、实现用户自动登陆

　　思路是这样的：

　　1、在用户登陆成功后，发送一个名称为user的cookie给客户端，cookie的值为用户名和md5加密后的密码。
　　2、编写一个AutoLoginFilter，这个filter检查用户是否带有名称为user的cookie来，如果有，则调用dao查询cookie的用户名和密码是否和数据库匹配，匹配则向session中存入user对象（即用户登陆标记），以实现程序完成自动登陆。

　　核心代码如下：

　　处理用户登录的控制器：LoginServlet

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.Cookie;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 import me.gacl.dao.UserDao;
12 import me.gacl.domain.User;
13 import me.gacl.util.WebUtils;
14 
15 public class LoginServlet extends HttpServlet {
16 
17     public void doGet(HttpServletRequest request, HttpServletResponse response)
18             throws ServletException, IOException {
19 
20         String username = request.getParameter("username");
21         String password = request.getParameter("password");
22         
23         UserDao dao = new UserDao();
24         User user = dao.find(username, password);
25         if(user==null){
26             request.setAttribute("message", "用户名或密码不对！！");
27             request.getRequestDispatcher("/message.jsp").forward(request, response);
28             return;
29         }
30         request.getSession().setAttribute("user", user);
31         //发送自动登陆cookie给客户端浏览器进行存储
32         sendAutoLoginCookie(request,response,user);
33         request.getRequestDispatcher("/index.jsp").forward(request, response);
34     }
35 
36     /**
37     * @Method: sendAutoLoginCookie
38     * @Description: 发送自动登录cookie给客户端浏览器
39     * @Anthor:孤傲苍狼
40     *
41     * @param request
42     * @param response
43     * @param user
44     */ 
45     private void sendAutoLoginCookie(HttpServletRequest request, HttpServletResponse response, User user) {
46         if (request.getParameter("logintime")!=null) {
47             int logintime = Integer.parseInt(request.getParameter("logintime"));
48             //创建cookie,cookie的名字是autologin，值是用户登录的用户名和密码，用户名和密码之间使用.进行分割，密码经过md5加密处理
49             Cookie cookie = new Cookie("autologin",user.getUsername() + "." + WebUtils.md5(user.getPassword()));
50             //设置cookie的有效期
51             cookie.setMaxAge(logintime);
52             //设置cookie的有效路径
53             cookie.setPath(request.getContextPath());
54             //将cookie写入到客户端浏览器
55             response.addCookie(cookie);
56         }
57     }
58     
59     public void doPost(HttpServletRequest request, HttpServletResponse response)
60             throws ServletException, IOException {
61 
62         doGet(request, response);
63     }
64 
65 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　处理用户自动登录的过滤器：AutoLoginFilter

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
11 import javax.servlet.http.Cookie;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 
15 import me.gacl.dao.UserDao;
16 import me.gacl.domain.User;
17 import me.gacl.util.WebUtils;
18 
19 public class AutoLoginFilter implements Filter {
20 
21     public void doFilter(ServletRequest req, ServletResponse resp,
22             FilterChain chain) throws IOException, ServletException {
23         
24         HttpServletRequest request = (HttpServletRequest) req;
25         HttpServletResponse response = (HttpServletResponse) resp;
26         //如果已经登录了，就直接chain.doFilter(request, response)放行
27         if(request.getSession().getAttribute("user")!=null){
28             chain.doFilter(request, response);
29             return;
30         }
31         
32         //1.得到用户带过来的authlogin的cookie
33         String value = null;
34         Cookie cookies[] = request.getCookies();
35         for(int i=0;cookies!=null && i<cookies.length;i++){
36             if(cookies[i].getName().equals("autologin")){
37                 value = cookies[i].getValue();
38             }
39         }
40         
41         //2.得到 cookie中的用户名和密码 
42         if(value!=null){
43             String username = value.split("\\.")[0];
44             String password = value.split("\\.")[1];
45             
46             //3.调用dao获取用户对应的密码
47             UserDao dao = new UserDao();
48             User user = dao.find(username);
49             String dbpassword = user.getPassword();
50             
51             //4.检查用户带过来的md5的密码和数据库中的密码是否匹配,如匹配则自动登陆
52             if(password.equals(WebUtils.md5(dbpassword))){
53                 request.getSession().setAttribute("user", user);
54             }
55         }
56         
57         chain.doFilter(request, response);
58     }
59     
60     public void destroy() {
61         
62     }
63 
64     public void init(FilterConfig filterConfig) throws ServletException {
65 
66     }
67 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　如果想取消自动登录，那么可以在用户注销时删除自动登录cookie，核心代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.Cookie;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 public class CancelAutoLoginServlet extends HttpServlet {
12 
13     public void doGet(HttpServletRequest request, HttpServletResponse response)
14             throws ServletException, IOException {
15         //移除存储在session中的user
16         request.getSession().removeAttribute("user");
17         //移除自动登录的cookie
18         removeAutoLoginCookie(request,response);
19         //注销用户后跳转到登录页面
20         request.getRequestDispatcher("/login.jsp").forward(request, response);
21     }
22 
23     /**
24     * @Method: removeAutoLoginCookie
25     * @Description: 删除自动登录cookie，
26     *     JavaWeb中删除cookie的方式就是新创建一个cookie，新创建的cookie与要删除的cookie同名，
27     *     设置新创建的cookie的cookie的有效期设置为0，有效路径与要删除的cookie的有效路径相同
28     * @Anthor:孤傲苍狼
29     *
30     * @param request
31     * @param response
32     */ 
33     private void removeAutoLoginCookie(HttpServletRequest request, HttpServletResponse response) {
34         //创建一个名字为autologin的cookie
35         Cookie cookie = new Cookie("autologin","");
36          //将cookie的有效期设置为0，命令浏览器删除该cookie
37         cookie.setMaxAge(0);
38         //设置要删除的cookie的path
39         cookie.setPath(request.getContextPath());
40         response.addCookie(cookie);
41     }
42     
43     public void doPost(HttpServletRequest request, HttpServletResponse response)
44             throws ServletException, IOException {
45         doGet(request, response);
46     }
47 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　以上就是过滤器的几个常见应用场景。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)