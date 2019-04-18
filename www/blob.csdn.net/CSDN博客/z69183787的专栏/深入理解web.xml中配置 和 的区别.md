# 深入理解web.xml中配置/和/*的区别 - z69183787的专栏 - CSDN博客
2018年10月31日 12:50:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：271
[https://blog.csdn.net/jinghuashuiyue2/article/details/78589655](https://blog.csdn.net/jinghuashuiyue2/article/details/78589655)
 在用SpringMVC进行web开发的时候，如果将DispathcerServlet对外访问的虚拟路径配置成/时，需要在Spring的配置文件中配置<mvc:default-servlet-handler/>这一项，那么为什么需要配置这一项呢？如果对外访问虚拟路径配置成/*可以吗？接下来本文将带着这样的两个疑问，从Tomcat中的conf/web.xml文件着手，深入浅出的分析这两种配置的真正区别。
**1. 一个注意点：why?拦截why?匹配**
       在描述一个Servlet对应的url-patten标签时，很多人会用“拦截”一词来描述这种关系，其实“拦截”一词用在这是很不贴切的。在日常生活中，“拦截”一词顾名思义就是拦下来，不给它通行权。那么用在Servlet中应该就是把请求给拦下来，不给浏览器响应。实际上，这显然不是我们要的结果。那么该用什么词来更为妥当的描述这种关系呢？
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122125559043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图是截自Tomact中conf/web.xml中的org.apache.jasper.servlet.JspServlet（第113行）说明，从中可以看到是用map一词来描述这种关系的。map译为“映射”，但”映射”过于专业化，对于初学者来说不易理解，因此使用意思贴近的”匹配”可以更好的描述这种关系，更为平易近人。因此只有请求匹配到了相应的Servelt，服务器才会给出响应，否则对不起，给出404.
**2. 一点说明：所有的请求资源都需要经过Servlet处理**
       所有的请求资源都需要经过Servlet处理，包括但不限于静态资源（.html、.css、jpg等文件）、.jsp请求和Servlet请求。初学web时，在地址栏直接访问静态资源（localhost:8080/index.html）的时候，给我们的感觉是不需要经过Servlet处理的，服务器会直接给我们做出响应。其实不然，在Tomcat中的conf/web.xml有如下说明（第38行），这段说明是关于org.apache.catalina.servlets.DefaultServlet的。
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122171931268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  从中可以看到DefaultServlet主要处理的是静态资源，这个Servlet处理规则是其它Servlet不能匹配的请求将由它进行处理。 而在Tomcat中的DefalutServlet(默认Servlet、缺省Servlet)的路径配置恰巧是/。
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122182506876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     如果在自己写的web应用中，同样配置一个路径/，则会覆盖Tomcat中的conf/web.xml中的缺省Servlet,为此在自己写的web.xml中配置如下：
```
<servlet>
    <servlet-name>DefaultServlet</servlet-name>
    <servlet-class>cn.tedu.test.DefaultServlet</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>DefaultServlet</servlet-name>
    <url-pattern>/</url-pattern>
  </servlet-mapping>
  <welcome-file-list>
    <welcome-file>index.html</welcome-file>
  </welcome-file-list>
```
 在自己写的DefaultServlet中有如下代码：
```
//告诉浏览器响应的是文本信息
        response.setContentType("text/html;charset=utf-8");
        //获得index.html的真实路径
        String path = this.getServletContext().getRealPath("index.html");
        //获得输入输出流,并将文件内容响应给浏览器
        FileInputStream fis = new FileInputStream(path);
        OutputStream out = response.getOutputStream();
        int i = -1;
        while((i = fis.read()) != -1){
            out.write(i);
        }
```
准备一个简单的html静态资源放在webRoot目录下：
```
<!DOCTYPE html>
    <html>
      <head>
        <title>index.html</title>
        <meta charset="UTF-8">
      </head>
      <body>
        This is my HTML page. <br>
      </body>
    </html>
```
这个时候在浏览器输入相应的地址，浏览器会有”This is my HTML page.”显示，表明此时可以正确访问静态资源index.html。当将自己的DefalutServlet中的java代码给注释掉后，再从浏览器访问index.html静态资源，会发现浏览器不会做出任何响应。从这个结果中可以看到静态资源是由Servlet以流的形式响应给页面的，在这个Servlet中有一个重要的设置response.setContentType(“text/html;charset=utf-8”)。这个设置是告诉浏览器服务器将发送的是text/html信息，浏览器将会按照这种格式去解析浏览器所发送的内容（ig.发送的是png格式的图片则为image/png）。 
       从上我们可以看到当在自己的Servlet中配置/的时候会覆盖Tomcat中config/web.xml中的/配置，这时如果我们不对静态资源进行相应的处理的时候，将导致静态资源无法访问。可见，Tomcat中的config/web.xml中的缺省Servlet主要处理的就是静态资源的访问。 
       这也是为什么平常我们在自己的web应用中没有处理配置处理静态资源ervlet的时候，静态资源仍然可以正常访问和显示的原因。因此当我们在springMVC中将DispatcherServlet配置成/时，我们一定要在spring的配置文件中加上如下配置的原因。
```
<!-- 处理静态资源被"/"所拦截的问题 -->
    <mvc:default-servlet-handler/>
```
**3. /和/*区别**
       其实/和/*都可以匹配所有的请求资源，但其匹配的优先顺序是不同的。/在所有的匹配路径中，优先级最低，即当别的路径都无法匹配时，/所匹配的缺省Servlet才会进行相应的请求资源处理。而 /星号 匹配的优先级是高于/路径和星号.后缀的路径的（如星号.action,星号.jsp等路径）。 
       为了更好的说明这个问题我们以一个例子来说明。相关文件如下（注意导入springMVC的jar包）：
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122203525888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
web.xml配置如下：
```
<servlet>
      <servlet-name>springmvc</servlet-name>
      <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <init-param>
      <param-name>contextConfigLocation</param-name>
      <param-value>classpath:springmvc-servlet.xml</param-value>
    </init-param>
  </servlet>
  <servlet-mapping>
    <servlet-name>springmvc</servlet-name>
    <url-pattern>/</url-pattern>
  </servlet-mapping>
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>！
```
springmvc-servlet.xml配置如下：
```
<!-- 处理静态资源被"/"所拦截的问题 -->
    <mvc:default-servlet-handler/>
```
 index.jsp文件如下：
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE >
<html>
  <head>
    <title>My JSP 'index.jsp' starting page</title>
  </head>
  <body>
    This is my JSP page. <br>
    <img alt="图片" src="1.jpg" border="1px" width="400px" height="200px">
  </body>
</html>
```
在浏览器地址栏输入localhost:8080/hello，我们发现页面文字和图片内容都可以正常显示。
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171123084848489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 当将springmvc-servlet.xml中关于静态资源访问的配置给注释掉后，再次访问结果如下：
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122204530004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
   这时静态的图片资源无法访问，这正如一开始论述的结论一样。这时我们再次打开springmvc-servlet.xml中关于静态资源的配置，并将web.xml中对外访问的虚拟路径配置成/*这样的形式。再次访问会抛出一个404的错误。而将地址栏改为localhost:8080/hello/index.jsp强行访问该资源的时候，会出现以下结果：
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122205740391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
   即将jsp内容以文本的形式显示出来了。接着我们继续将springmvc-servlet.xml中关于静态资源的访问配置给注释掉，继续访问localhost:8080/hello/index.jsp资源，这时会抛出一个404错误，也就是说在上一次访问的时候，服务器把index.jsp当成了静态资源响应给了浏览器，所有我们才会看到这样的文本页面。 
       那么为什么在配置成/*后index.jsp就不能正常访问了？原来在Tomcat中conf/web.xml中配置了org.apache.jasper.servlet.JspServlet用来处理.jsp这样的资源了（第113行）。
![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171122210539217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluZ2h1YXNodWl5dWUy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这段话的意思是说JspServlet是jsp页面的编译和执行的Servlet.也就是说JspServlet可以将.jsp页面转换成.java文件，最终编译成.class文件并执行。 
       至此，对/和/*的配置有一个这样的总结：两者都可以匹配任何资源，只不过两者的匹配的优先级是不一样的;当请求资源与其它Servlet都无法匹配的时候，/所配置的Servlet才会匹配。 
       在Tomcat中conf/web.xml中有关于/和.jsp页面处理的Servlet,当自己所配的web.xml文件中配置/时，会使Tomcat中的DefaultServlet无法访问，导致静态资源无法访问，因此在SpingMVC配置文件中要开启处理静态资源的开关。实际上，当将对外访问的虚拟路径配置成*.action等类似路径的时候，就可以很好的避免上述问题了。
**4. 引申一：Servlet对外访问虚拟路径匹配顺序简介**
       Servlet对外访问的虚拟路径的匹配顺序大致如下：
       1.含有全部或部分对外访问的具体路径配置，如/LoginServlet，/servlet/*(优先匹配更为具体的路径); 
       2./*的配置; 
       3.形如*.action这样后缀形式的配置，如index.jsp,login.action（即使是具体的路径，优先级也会低于1和2的配置）; 
       4.其它配置无法匹配的时候，匹配/所配置的Servlet。
       注意：/servlet/*.action这样的配置是错误的，即不应该存在这样的配置。
**5. 引申二：过滤器对外访问虚拟路径可以配置成/吗?**
       答案是否定的。 
       在Servlet中，请求资源必须要有相应的Servlet进行处理，否则会抛出404错误。但在过滤器中则不一样，请求资源有时候并不需要经过过滤器的处理，因此当配置成/时，过滤器将不会生效。这与/配置的结论是相符的，即/只会匹配其它请求资源无法处理的请求，而在过滤器中对请求资源并不要求一定要处理。
