# 解决http请求跨域问题

2018年08月03日 11:26:47 [准时长大](https://me.csdn.net/T_james) 阅读数：2403



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/T_james/article/details/81382679

​      一个众所周知的问题，Ajax直接请求普通文件存在跨域无权限访问的问题，甭管你是静态页面、动态网页、web服务、WCF，只要是跨域请求，一律不准。在接口方面涉及到浏览器访问其他项目时也会碰到跨域问题，这个时候我们就要去解决它。

​       了解下什么是跨域？

​       概念：只要协议、域名、端口有任何一个不同，都被当作是不同的域。



|URL|    URL|                  说明|       是否允许通信|
|:----|:----|:----|:----|
|http://www.a.com/a.js|http://www.a.com/b.js  |   同一域名下  | 允许|
|http://www.a.com/lab/a.js|http://www.a.com/script/b.js |同一域名下不同文件夹 |允许|
|http://www.a.com:8000/a.js|http://www.a.com/b.js   |  同一域名，不同端口  |不允许|
|http://www.a.com/a.js|https://www.a.com/b.js |同一域名，不同协议 |不允许|
|http://www.a.com/a.js|http://70.32.92.74/b.js |域名和域名对应ip |不允许|
|http://www.a.com/a.js|http://script.a.com/b.js |主域相同，子域不同 |不允许|
|http://www.a.com/a.js|http://a.com/b.js |同一域名，不同二级域名（同上）| 不允许（cookie这种情况下也不允许访问）|
|http://www.cnblogs.com/a.js|http://www.a.com/b.js |不同域名 |不允许|



对于端口和协议的不同，只能通过后台来解决。

![img](https://img-blog.csdn.net/20180803105708663?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RfamFtZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![img](https://img-blog.csdn.net/201808031056521?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RfamFtZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

像这种属于后端没有设置相应的跨域请求头的问题，所以我们需要去解决。

今天我们就来通过后台解决一下跨域的问题，一个是普通类型的ssm类型项目，一个是微服务级别的springboot类型的项目。

​    1.首先我们看一下ssm类型的项目怎么解决跨域问题，说实话，用惯了springboot再去用这个感觉配置文件在哪里配都不知道，不过好歹我最后还是想起来了。

​      针对网上说的下jar包的问题，是没有的，我们只需要首先一个过滤器filter,进行相关的过滤配置后在web.xml中关联一下链接即可。

```java
public class CrossFilter implements Filter {
	@Override
	public void destroy() {
		// TODO Auto-generated method stub
	}
 
	public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {
		HttpServletRequest request = (HttpServletRequest)req;
		HttpServletResponse response = (HttpServletResponse) res;
		String originHeader = request.getHeader("Origin");
		response.setHeader("Access-Control-Allow-Origin", originHeader);
		response.setHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE"); 
		response.setHeader("Access-Control-Max-Age", "0"); 
		response.setHeader("Access-Control-Allow-Headers", "Authorization,Origin, No-Cache, X-Requested-With, If-Modified-Since, Pragma, Last-Modified, Cache-Control, Expires, Content-Type, X-E4M-With,userId,token"); 
		response.setHeader("Access-Control-Allow-Credentials", "true"); 
		response.setHeader("XDomainRequestAllowed","1"); 
		response.setHeader("XDomainRequestAllowed","1"); 
		chain.doFilter(request, response); 
    }
 
	@Override
	public void init(FilterConfig arg0) throws ServletException {
		// TODO Auto-generated method stub
	}
}
```

这一段代码则是设置相关的请求，包含请求格式，请求头，最大请求等等的设置，这里特别需要注意一个地方

```java
response.setHeader("Access-Control-Allow-Headers", "Authorization,Origin, No-Cache, X-Requested-With, If-Modified-Since, Pragma, Last-Modified, Cache-Control, Expires, Content-Type, X-E4M-With,userId,token"); 
```

有很多时候你看别人的博客然后去设置相应的过滤器，但是还是会出现错误，提示你像这样的错误。

![img](https://img-blog.csdn.net/2018080311062559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RfamFtZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其实你已经配置成功了，只不过少配置了一个请求头Authorization的设置，所以有的时候，你离成功只差那么一点点。大家根据我这个去配置应该是可以满足需要的，如果有特殊需求的话，可以根据错误进行相应的补充和调整。

```html
<filter>     
    <filter-name>crossFilter</filter-name>  
   <filter-class>com.youfuli.filters.CrossFilter</filter-class>  
 </filter>  
<filter-mapping>  
   <filter-name>crossFilter</filter-name>  
   <url-pattern>/*</url-pattern>  
</filter-mapping>
```

下一步则是去web.xml中去配置下你这个过滤器的路径，配置成功后可以按住ctrl键鼠标跳转即可。这个时候我们就可以正常的去执行操作了。

2.第二种，在springboot项目中配置跨域请求。关于这一种就简单了，新建一个config，实现WebMvcConfigurerAdapter接口即可

```java
public class WebMvcConfig extends WebMvcConfigurerAdapter {
 /**
     * <跨域访问>
     */
    @Bean
    public CorsFilter corsFilter() {
        UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
        source.registerCorsConfiguration("/**", buildConfig());
        return new CorsFilter(source);
    }
 
    private CorsConfiguration buildConfig() {
        CorsConfiguration corsConfiguration = new CorsConfiguration();
        corsConfiguration.addAllowedOrigin("*");
        corsConfiguration.addAllowedHeader("*");
        corsConfiguration.addAllowedMethod("*");
        return corsConfiguration;
    }
}
```

这样即可，当然，你也可以去编写filter进行过滤，但是这样是不是就有点落伍了呢，所以多学点新技术还是好的。

跨域不可怕，可怕的是你碰到跨域这样的新事物就害怕那就真的可怕了。