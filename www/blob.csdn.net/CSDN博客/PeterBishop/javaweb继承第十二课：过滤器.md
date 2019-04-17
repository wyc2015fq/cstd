# javaweb继承第十二课：过滤器 - PeterBishop - CSDN博客





2018年12月26日 14:33:36[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)











这节课我们来讲讲过滤器Filter



Servlet 过滤器可以动态地拦截请求和响应，以变换或使用包含在请求或响应中的信息。

可以将一个或多个 Servlet 过滤器附加到一个 Servlet 或一组 Servlet。Servlet 过滤器也可以附加到 Java Server Pages (JSP) 文件和 HTML 页面。调用 Servlet 前调用所有附加的 Servlet 过滤器。

Servlet 过滤器是可用于 Servlet 编程的 Java 类，可以实现以下目的：

- 在客户端的请求访问后端资源之前，拦截这些请求。

- 在服务器的响应发送回客户端之前，处理这些响应。



我们来学习怎么定义或者说创建过滤器


package com.filter;



import javax.servlet.*;

import javax.servlet.http.HttpServletRequest;

import java.io.IOException;



*/** * **创建Filter步骤: *  1. 定义一个类实现javax.servlet.Filter接口 *  2. 重写抽象方法 *  3. 在web.xml里配置这个filter */*public class TeachFilter implements Filter {

    @Override

    public void init(FilterConfig filterConfig) throws ServletException {



    }



    */**     *     * **@param ****servletRequest **request**对象     * **@param ****servletResponse **response**对象     * **@param ****filterChain **过滤器链，用于决定是否放行     * **@throws **IOException     * **@throws **ServletException     */    *@Override

    */**     * **说明一下，ServletRequest是HttpServletRequest的父接口     * HttpServletRequest可以通过getRequestURL()方法来获取请求路径,返回对象是StringBuffer类型，可通过toString()方法转成字符串     * ServletRequest并没有可以用来获取请求路径的方法     */    *public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {

        */**         * instanceof**关键字: 作用是判断左边的对象是否是右边的类型         */        *if(servletRequest instanceof HttpServletRequest)

        {

            String url = ((HttpServletRequest) servletRequest).getRequestURL().toString();

            System.*out*.println("目前过滤的是 '" + url + "' 路径");

        }

        else

        {

            System.*out*.println("这不是一个Http请求");

        }

        //可以发现它只是执行了这个方法的代码，但好像没有 放行，即servlet或者jsp的代码没执行

        //所以，filter是在servlet之前执行的

        //放行代码:

        filterChain.doFilter(servletRequest,servletResponse);

        //filterChain是过滤器链，在参数里

        //可以看到添加了这行代码后，页面就显示出来了

    }



    @Override

    public void destroy() {



    }

}


下面写个具体应用:字符编码过滤器
package com.filter;



import javax.servlet.*;

import java.io.IOException;



//写完了别忘了在web.xml里配置一下

//这个字符编码过滤器等下注册功能要用

public class CharacterEncodingFilter implements Filter {

    @Override

    public void init(FilterConfig filterConfig) throws ServletException {



    }



    @Override

    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException

    {

        System.*out*.println("这是字符编码过滤器");

        servletRequest.setCharacterEncoding("UTF-8");

        servletResponse.setCharacterEncoding("UTF-8");

        filterChain.doFilter(servletRequest,servletResponse);

    }



    @Override

    public void destroy() {



    }

}


web.xml配置filter
<!--配置Filter-->

<filter>

    <filter-name>TeachFilter</filter-name>

    <filter-class>com.filter.TeachFilter</filter-class>

</filter>



<filter-mapping>

    <filter-name>TeachFilter</filter-name>

    <!-- /* 表示所有请求， java中级会讲正则表达式，到时候就懂了-->

    <url-pattern>/*</url-pattern>

</filter-mapping>



<filter>

    <filter-name>CharacterEncodingFilter</filter-name>

    <filter-class>com.filter.CharacterEncodingFilter</filter-class>

</filter>



<filter-mapping>

    <filter-name>CharacterEncodingFilter</filter-name>

    <url-pattern>/*</url-pattern>

</filter-mapping>


注意:

对于同一请求所有过滤器都会起作用(当然要加filterChain.doFilter())，执行顺序是配置filter-mapping的顺序

如果过滤器的doFilter方法里没有写filterChain.doFilter(req,resp)则不会继续往后执行，也就是起到了过滤的功能



