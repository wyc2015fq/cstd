# Spring乱码问题解决方案 - ggabcda的博客 - CSDN博客





2017年08月16日 21:33:28[ggabcda](https://me.csdn.net/ggabcda)阅读数：335








### 请求乱码

#### GET请求乱码：

原因：请求参数带在url地址上、url地址什么时候解析？

tomcat收到请求对url进行编解码（ISO8859-1）

解决方案：在tomcat的8080端口配置出加上 URIEncoding="utf-8"；

![](http://images2017.cnblogs.com/blog/554581/201708/554581-20170815232205271-1692074272.png)


```
<Connector URIEncoding="utf-8" connectionTimeout="20000" port="8080" protocol="HTTP/1.1" redirectPort="8443"/>
```

#### POST请求乱码：

 原因：请求参数是带在请求体

 请求体的解码是在第一次获取参数的时候

 filter可以用来解决乱码

 request.setCharacterEncoding("utf-8")



###  响应乱码

 response.setContentType("text/html;charset=utf-8")



 1、tomcat一安装；上手就给server.xml中的8080处加上URIEncoding="utf-8"

 2、给项目里面再搞一个filter让他来解决所有的post乱码；

 3、SpringMVC自带的一个乱码解决的filter

项目的web.xml中配置：


```
<!--字符编码的filter一定要放在最前面  -->
    <filter>
        <filter-name>CharacterEncodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <!-- 配置encoding，告诉我们指定的编码格式 -->
        <init-param>
            <param-name>encoding</param-name>
            <param-value>utf-8</param-value>
        </init-param>
        <!-- 解决响应乱码 -->
        <init-param>
            <param-name>forceEncoding</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>CharacterEncodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    
    <!-- 支持rest的filter -->
    <filter>
        <filter-name>HiddenHttpMethodFilter</filter-name>
        <filter-class>org.springframework.web.filter.HiddenHttpMethodFilter</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>HiddenHttpMethodFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
```

在配置里面配置了encoding和forceEncoding

来看看CharacterEncodingFilter的源码,通过request.setCharacterEncoding(this.encoding)和response.setCharacterEncoding(this.encoding)来解决post请求和响应乱码的问题：


```
public class CharacterEncodingFilter extends OncePerRequestFilter {

    private String encoding;

    private boolean forceEncoding = false;

    public void setEncoding(String encoding) {
        this.encoding = encoding;
    }

    public void setForceEncoding(boolean forceEncoding) {
        this.forceEncoding = forceEncoding;
    }


    @Override
    protected void doFilterInternal(
            HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        //encoding是UTF-8
        //forceEncoding位true
        if (this.encoding != null && (this.forceEncoding || request.getCharacterEncoding() == null)) {
            request.setCharacterEncoding(this.encoding);
            if (this.forceEncoding) {
                response.setCharacterEncoding(this.encoding);
            }
        }
        filterChain.doFilter(request, response);
    }

}
```








