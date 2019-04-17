# JavaWeb —— Filter - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31








# 【概述】

Filter（过滤器）与 Servlet 十分相似，但其具有拦截客户端请求的功能，可以改变请求的内容来满足实际开发中的需要。其实质就是一个在 Web 应用服务器上的 Web 应用组件，用于拦截客户端与目标资源的请求，并对请求进行一定过滤处理后再发送给目标资源。

![](https://img-blog.csdnimg.cn/20181223220355219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

若一个 Web 应用中使用一个过滤器不能解决实际中的业务需要，那么可以部署多个过滤器对业务请求进行多次处理，从而组成一个过滤器链。

![](https://img-blog.csdnimg.cn/20181223220720449.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【结构体系】

过滤器对象是位于 javax.Servlet.Filter 的接口，除这个接口外，还有过滤器的配置对象 javax.Servlet.FilterConfig 接口以及过滤器的传递工具 javax.Servlet.FilterChain 接口，在实际开发中，定义过滤器对象只需实现 Filter 接口即可。

## 1.Filter 接口

每一个过滤器对象都需直接或间接的实现 Filter 接口。
- void init(FilterConfig filterConfig)：过滤器初始化方法，在过滤器初始化时调用
- void doFilter(ServletRequest request,ServletResponse response,FilterChain chain)：对请求进行过滤处理
- void destroy()：销毁方法，用于释放资源

## 2.FilterConfig 接口

FilterConfig 接口由 Servlet 容器实现，主要用于获取过滤器中的配置信息。
- String getFilterName()：获取过滤器名字
- ServletContext getServletContext()：获取 Servlet 上下文
- String getInitParameter(String name)：获取过滤器名为 name 的参数初始化参数值
- Enumeration getInitParameterNames()：获取过滤器的所有初始化参数值

## 3.FilterChain  接口

FilterChain 接口由 Servlet 容器实现，在接口中只有一个方法：
- void doFilter(ServletRequest request,ServletResponse response)：用于将过滤后的请求传递给下一个过滤器，若此过滤器已是过滤器链的最后一个，那么将传送给目标资源。

# 【代码结构】

创建一个过滤器对象需要实现 javax.servlet.Filter 接口，同时实现 Filter 接口的三个方法：init() 方法用于初始化；destroy() 方法用于释放资源；doFilter() 方法用于编写对过滤处理的业务逻辑，在请求过滤处理后，需调用 chain 参数的 doFilter() 方法将请求传递给下一级过滤器或目标资源。

要注意的是，不一定要将请求向下传递给下一级过滤器或目标资源，如果业务逻辑需要，也可以在过滤处理后，直接回应于客户端。

```java
public class Test implements Filter {
    //初始化方法
    public void init(FilterConfig fConfig) throws ServletException {
        //初始化处理
    }
    //过滤处理方法
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        //过滤处理
        chain.doFilter(request, response);//发送给下一过滤器
	}
    //销毁方法
    public void destroy() {
        //释放资源
    }
}
```

# 【创建与配置】

## 1.Filter 创建

Filter 创建方式有两种，一种是创建一个普通的 Java 类，将这个类继承 Filter，再通过手动配置 web.xml 文件注册过滤器对象，但该方法操作繁琐，并不常用，经常使用的是第二种方法——通过 IDE 集成开发工具进行创建。

以 Eclipse 为例创建过滤器：在 Eclipse 包资源管理器中右击，在弹出的快捷菜单中选择 " 新建/Filter "，在弹出的对话框中输入所在包及包名，然后单击完成即可。
![](https://img-blog.csdnimg.cn/20181225200320413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181225200343612.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 2.Filter 配置

要使 Filter 对象正常运行，需要进行适当的配置，以告知 Web 容器哪一个请求调用哪一个 Filter 对象处理，对 Filter 起到一个注册的作用。

Filter 与 Servlet 十分相似，在创建之后同样需要配置，主要分为两个步骤：声明过滤器对象、创建过滤器映射。

### 1）声明 Filter 对象

在 web.xml 文件中，通过 <filter> 标签声明一个 Filter 对象，在此标签下包含两个主要子元素：<filter-name> 用于指定 Filter 的名称，可以自定义；<filter-class> 用于指定 Servlet 的完整位置

例如：

```
<filter>
  <filter-name>test</filter-name>
  <filter-class>filter.Test</filter-class>
</filter>
```

### 2）创建 Filter 映射

在 web.xml 中声明了 Servlet 对象后，需要映射访问 Servlet 的 URL，该操作使用 <filter-mapping> 标签进行配置。

<filter-mapping> 标签包含两个子元素：<filter-name> 与 <servlet> 标签中的 <filter-name> 元素相对应，不可随意命名；<url-pattern> 用于映射访问 URL

例如：

```
<filter-mapping>
  <filter-name>test</filter-name>
  <url-pattern>/filter/test</url-pattern>
</filter-mapping>
```

# 【实例】

由于 Web 容器内部所使用的编码格式不支持中文字符集，因此在处理浏览器请求中的中文数据就会出现乱码现象。解决此问题的方法十分简单，在业务处理中重新指定编码格式即可，但实际开发中，若通过每一个业务都指定中文字符集，则过于繁琐而且容易遗漏某一业务中的字符编码设置，因此可通过字符编码过滤器来处理字符编码，浏览器的每一次请求都会经过过滤器进行转码，从而避免中文乱码现象产生。

1.创建字符编码过滤器

```java
package filter;

import java.io.*;
import javax.servlet.*;
import javax.servlet.annotation.*;

public class CharactorFilter implements Filter {
    String encoding = null;
	
    public CharactorFilter() {
    }

    public void destroy() {
        encoding = null;
    }

    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        if(encoding!=null) {
            request.setCharacterEncoding(encoding);//设置Request编码格式
            response.setContentType("text/html;charset="+encoding);//设置Response字符编码
        }
        chain.doFilter(request, response);
    }

    public void init(FilterConfig fConfig) throws ServletException {//获取初始化参数
        encoding = fConfig.getInitParameter("encoding");
    }

}
```

2.在 web.xml 文件对过滤器进行配置

```
<filter>
  <filter-name>charactorFilter</filter-name>
  <filter-class>filter.CharactorFilter</filter-class>
  <init-param>
    <param-name>encoding</param-name>
    <param-value>UTF-8</param-value>
  </init-param>
</filter>
<filter-mapping>
  <filter-name>charactorFilter</filter-name>
  <url-pattern>/*</url-pattern>    <!-- 使用正则表达式来匹配所有请求 -->
</filter-mapping>
```





