# SpringMvc Interceptor拦截器的配置与使用 - z69183787的专栏 - CSDN博客
2018年01月31日 11:13:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：327
[http://blog.csdn.net/huangjp_hz/article/details/73614314](http://blog.csdn.net/huangjp_hz/article/details/73614314)
## DispatcherServlet
SpringMVC具有统一的入口DispatcherServlet，所有的请求都通过DispatcherServlet。DispatcherServlet是前置控制器，配置在web.xml文件中的。拦截匹配的请求，Servlet拦截匹配规则要自已定义，把拦截下来的请求，依据某某规则分发到目标Controller来处理。 所以我们现在web.xml中加入以下配置：
```xml
<servlet>
    <servlet-name>demo</servlet-name>
    <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <init-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath:spring-mvc.xml</param-value>
    </init-param>
    <load-on-startup>2</load-on-startup>
    <async-supported>true</async-supported>
</servlet>
<servlet-mapping>
    <servlet-name>demo</servlet-name>
    <!-- *.do的请求，会被DispatcherServlet处理 -->
    <url-pattern>*.do</url-pattern>
</servlet-mapping>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
## 静态资源不拦截
一般实现拦截器主要是为了权限管理，主要是拦截一些url请求，所以不对静态资源进行拦截。要过滤掉静态资源可以在spring-mvc.xml配置文件中这样配置:
```xml
<!-- cache-period:配置缓存时间 -->
<mvc:resources mapping="/resources/**" location="/resources/" cache-period="300" />
```
- 1
- 2
## 自定义拦截器
SpringMVC的拦截器HandlerInterceptorAdapter对应提供了三个preHandle，postHandle，afterCompletion方法。
- preHandle在业务处理器处理请求之前被调用;
- postHandle在业务处理器处理请求执行完成后,生成视图之前执行;
- afterCompletion在DispatcherServlet完全处理完请求后被调用,可用于清理资源等 。
所以要想实现自己的权限管理逻辑，需要继承HandlerInterceptorAdapter并重写其三个方法。
- springmvc.xml中加入自己定义的拦截器:
```xml
<!--配置拦截器, 多个拦截器,顺序执行 -->  
<mvc:interceptors>
    <bean class="org.springframework.web.servlet.i18n.LocaleChangeInterceptor"/>
    <mvc:interceptor>
        <!-- 匹配的是url路径， 如果不配置或/**,将拦截所有的Controller -->    
        <mvc:mapping path="/**"/>
        <mvc:exclude-mapping path="/resources/**"/>
        <bean class="com.wx.app.ygp.action.interceptor.LoginInterceptor"></bean>
    </mvc:interceptor>
    <!-- 当设置多个拦截器时，先按顺序调用preHandle方法，然后逆序调用每个拦截器的postHandle和afterCompletion方法 -->  
    <mvc:interceptor>
        <mvc:mapping path="/**"/>
        <mvc:exclude-mapping path="/resources/**"/>
        <!-- 自定义拦截器文件的路径 -->
        <bean class="**com.wx.app.ygp.action.interceptor.MessageInterceptor**"></bean> 
    </mvc:interceptor>
</mvc:interceptors>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 自定义拦截器的具体代码：
```java
package com.wx.app.ygp.action.interceptor;
import java.io.IOException;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.log4j.Logger;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;
/**
 * 
 * @author huangjp
 * @Descripition : 短信验证拦截器
 * 2017年6月22日 下午1:11:11
 */
public class MessageInterceptor extends HandlerInterceptorAdapter{
    /**
     * 需要拦截的URL
     */
    private static final String[] INTERCEPTOR_URL = {
            "/icCard/workCard.do?menuId=97",
            "/billManagement/counterPayment/show.do?menuId=2003",
            "/icCard/dailyBusiness.do?menuId=95",
            "/icCard/exceptionBusiness.do?menuId=96"
    };
    private static final String MESSAGE_URL = "/message/show.do";
    private final Logger logger = Logger.getLogger(MessageInterceptor.class);
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws Exception {
        //获取当前请求url:http://localhost:8080/ygp/icCard/workCard.do
        String url = request.getRequestURL().toString(); 
        String message = request.getParameter("message");
        //如果是需要短信验证的页面，则先跳转到短信验证页面
        for(String oneUrl : INTERCEPTOR_URL){
            if(url.contains(oneUrl.split("[?]")[0]) && message == null){
                logger.info("该页面需要短信验证");
                fowardMessage(request, response, oneUrl);
                return false;
            }
        }
        logger.info("该页面不需要短信验证");
        return true;
    }
    /**
     * 跳转到短信验证页面
     * @param : menuUrl
     * @throws IOException 
     * @throws ServletException 
     */
    private void fowardMessage(HttpServletRequest request,HttpServletResponse response, String menuUrl) throws ServletException, IOException{
        RequestDispatcher rd = request.getRequestDispatcher(MESSAGE_URL+"?menuUrl="+menuUrl);
        rd.forward(request, response);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
这样，就完成了一个简单的自定义拦截器。
## 参考资料
[http://blog.csdn.net/tonytfjing/article/details/39207551](http://blog.csdn.net/tonytfjing/article/details/39207551)
[http://blog.csdn.net/liumm0000/article/details/7565480](http://blog.csdn.net/liumm0000/article/details/7565480)
[http://hc24.iteye.com/blog/2063660](http://hc24.iteye.com/blog/2063660)
