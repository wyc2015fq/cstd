# Spring MVC 4.2 增加 CORS 支持 - z69183787的专栏 - CSDN博客
2016年11月09日 17:08:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3731
# Spring MVC 4.2 增加 CORS 支持
跨站 HTTP 请求(Cross-site HTTP request)是指发起请求的资源所在域不同于该请求所指向资源所在的域的 HTTP 请求。比如说，域名A([http://domaina.example](http://domaina.example/))的某 Web 应用程序中通过标签引入了域名B([http://domainb.foo](http://domainb.foo/))站点的某图片资源([http://domainb.foo/image.jpg](http://domainb.foo/image.jpg))，域名A的那
 Web 应用就会导致浏览器发起一个跨站 HTTP 请求。在当今的 Web 开发中，使用跨站 HTTP 请求加载各类资源（包括CSS、图片、[JavaScript](http://lib.csdn.net/base/javascript) 脚本以及其它类资源），已经成为了一种普遍且流行的方式。
正如大家所知，出于安全考虑，浏览器会限制脚本中发起的跨站请求。比如，使用 XMLHttpRequest 对象发起 HTTP 请求就必须遵守同源策略（same-origin policy）。 具体而言，Web 应用程序能且只能使用 XMLHttpRequest 对象向其加载的源域名发起 HTTP 请求，而不能向任何其它域名发起请求。为了能开发出更强大、更丰富、更安全的Web应用程序，开发人员渴望着在不丢失安全的前提下，Web 应用技术能越来越强大、越来越丰富。比如，可以使用 XMLHttpRequest 发起跨站
 HTTP 请求。（这段描述跨域不准确，跨域并非浏览器限制了发起跨站请求，而是跨站请求可以正常发起，但是返回结果被浏览器拦截了。最好的例子是crsf跨站攻击原理，请求是发送到了后端服务器无论是否跨域！注意：有些浏览器不允许从HTTPS的域跨域访问HTTP，比如Chrome和Firefox，这些浏览器在请求还未发出的时候就会拦截请求，这是一个特例。）
更多CORS介绍请看这里：
> 
[https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Access_control_CORS](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Access_control_CORS)
在WEB项目中，如果我们想支持CORS，一般都要通过过滤器进行实现，可以定义一些基本的规则，但是不方便提供更细粒度的配置，如果你想参考过滤器实现，你可以阅读下面这篇文章：
> 
[http://my.oschina.net/huangyong/blog/521891](http://my.oschina.net/huangyong/blog/521891)
## Spring MVC 从4.2版本开始增加了对CORS的支持
在[spring](http://lib.csdn.net/base/javaee) MVC 中增加CORS支持非常简单，可以配置全局的规则，也可以使用`@CrossOrigin`注解进行细粒度的配置。
### 使用`@CrossOrigin`注解
先通过源码看看该注解支持的属性：
```java
@Target({ ElementType.METHOD, ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface CrossOrigin {
    String[] DEFAULT_ORIGINS = { "*" };
    String[] DEFAULT_ALLOWED_HEADERS = { "*" };
    boolean DEFAULT_ALLOW_CREDENTIALS = true;
    long DEFAULT_MAX_AGE = 1800;
    /**
     * 同origins属性一样
     */
    @AliasFor("origins")
    String[] value() default {};
    /**
     * 所有支持域的集合，例如"http://domain1.com"。
     * <p>这些值都显示在请求头中的Access-Control-Allow-Origin
     * "*"代表所有域的请求都支持
     * <p>如果没有定义，所有请求的域都支持
     * @see #value
     */
    @AliasFor("value")
    String[] origins() default {};
    /**
     * 允许请求头重的header，默认都支持
     */
    String[] allowedHeaders() default {};
    /**
     * 响应头中允许访问的header，默认为空
     */
    String[] exposedHeaders() default {};
    /**
     * 请求支持的方法，例如"{RequestMethod.GET, RequestMethod.POST}"}。
     * 默认支持RequestMapping中设置的方法
     */
    RequestMethod[] methods() default {};
    /**
     * 是否允许cookie随请求发送，使用时必须指定具体的域
     */
    String allowCredentials() default "";
    /**
     * 预请求的结果的有效期，默认30分钟
     */
    long maxAge() default -1;
}
```
如果你对这些属性的含义不是很明白，建议阅读下面的文章了解更多:
> 
[http://fengchj.com/?p=1888](http://fengchj.com/?p=1888)
下面举例在方法和`Controller`上使用该注解。
#### 在Controller上使用`@CrossOrigin`注解
```java
@CrossOrigin(origins = "http://domain2.com", maxAge = 3600)
@RestController
@RequestMapping("/account")
public class AccountController {
    @RequestMapping("/{id}")
    public Account retrieve(@PathVariable Long id) {
        // ...
    }
    @RequestMapping(method = RequestMethod.DELETE, path = "/{id}")
    public void remove(@PathVariable Long id) {
        // ...
    }
}
```
#### 这里指定当前的`AccountController`中所有的方法可以处理`http://domain2.com`域上的请求，
#### 在方法上使用`@CrossOrigin`注解
```java
@CrossOrigin(maxAge = 3600)
@RestController
@RequestMapping("/account")
public class AccountController {
    @CrossOrigin("http://domain2.com")
    @RequestMapping("/{id}")
    public Account retrieve(@PathVariable Long id) {
        // ...
    }
    @RequestMapping(method = RequestMethod.DELETE, path = "/{id}")
    public void remove(@PathVariable Long id) {
        // ...
    }
}
```
#### 在这个例子中，`AccountController`类上也有`@CrossOrigin`注解，`retrieve`方法上也有注解，Spring会合并两个注解的属性一起使用。
### CORS全局配置
除了细粒度基于注解的配置，你可能会想定义一些全局CORS的配置。这类似于使用过滤器，但可以在Spring MVC中声明，并结合细粒度@CrossOrigin配置。默认情况下所有的域名和GET、HEAD和POST方法都是允许的。
#### 基于JAVA的配置
看下面例子：
```java
@Configuration
@EnableWebMvc
public class WebConfig extends WebMvcConfigurerAdapter {
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**");
    }
}
```
您可以轻松地更改任何属性，以及配置适用于特定的路径模式的CORS：
```java
@Configuration
@EnableWebMvc
public class WebConfig extends WebMvcConfigurerAdapter {
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/api/**")
            .allowedOrigins("http://domain2.com")
            .allowedMethods("PUT", "DELETE")
            .allowedHeaders("header1", "header2", "header3")
            .exposedHeaders("header1", "header2")
            .allowCredentials(false).maxAge(3600);
    }
}
```
如果你使用Spring Boot，你可以通过这种方式方便的进行配置。
#### 基于XML的配置
```java
<mvc:cors>
    <mvc:mapping path="/**" />
</mvc:cors>
```
#### 这个配置和上面[Java](http://lib.csdn.net/base/javaee)方式的第一种作用一样。
同样，你可以做更复杂的配置：
```java
<mvc:cors>
    <mvc:mapping path="/api/**"
        allowed-origins="http://domain1.com, http://domain2.com"
        allowed-methods="GET, PUT"
        allowed-headers="header1, header2, header3"
        exposed-headers="header1, header2" allow-credentials="false"
        max-age="123" />
    <mvc:mapping path="/resources/**"
        allowed-origins="http://domain1.com" />
</mvc:cors>
```
Access-Control-Allow-Origin: *
来允许任何站点对该资源进行跨域请求
#### 在SpringMVC下的解决方案：
定义CORSFilter
```java
import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Component;
@Component
public class CORSFilter implements Filter {
  public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {
    HttpServletResponse response = (HttpServletResponse) res;
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE");
    response.setHeader("Access-Control-Max-Age", "3600");
    response.setHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    chain.doFilter(req, res);
  }
  public void init(FilterConfig filterConfig) {}
  public void destroy() {}
}
```
web.xml：
```
<filter>
  <filter-name>cors</filter-name>
  <filter-class>com.web.filter.CORSFilter</filter-class>
</filter>
<filter-mapping>
  <filter-name>cors</filter-name>
  <url-pattern>/*</url-pattern>
</filter-mapping>
```
