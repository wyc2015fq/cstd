# SOSOAPI前后端分离开发模式下的接口测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月27日 10:19:59[boonya](https://me.csdn.net/boonya)阅读数：3093








第一次体验前后端分离开发,感觉不错，效率大大提高。前端负责数据展示，后端负责组装数据，各司其职比我以往接触的开发方式要快许多，使得各自专注自己的领域。SOSOAPI既可以完成接口测试，还可以生成规范的接口文档。有了前后端分离，再也不用关注前端了。





## SOSOAPI

基于有权限的访问方式需要提供token访问。


![](https://img-blog.csdn.net/20170627095648761)


### 操作步骤

1、创建项目

2、创建分组

3、设置登录接口获取令牌

4、编写对应的业务接口

5、预览发布接口测试

6、分享给前端使用接口（需设置访问密码）

7、导出文档

### 操作实例

#### 定义接口

![](https://img-blog.csdn.net/20170627101355761)


#### 预览接口

![](https://img-blog.csdn.net/20170627101424085)

#### 测试接口

![](https://img-blog.csdn.net/20170627101738937)





## 后端接口跨域问题解决

在web项目的web.xml添加过滤器处理跨域请求。

### web.xml配置



```
<!--ajax跨越请求  -->
	<filter>
		<filter-name>FilterAJAX</filter-name>
		<filter-class>com.cvnavi.common.filter.AjaxURLFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>FilterAJAX</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
```





### 过滤器AjaxURLFilter



```java
package com.cvnavi.common.filter;

import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;

public class AjaxURLFilter implements Filter{
	
	@Override
	public void init(FilterConfig filterConfig) throws ServletException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
			throws IOException, ServletException {
		HttpServletResponse resp = (HttpServletResponse)response;
		//"*"存在风险，建议指定可信任的域名来接收响应信息，如"http://www.sosoapi.com"
		resp.addHeader("Access-Control-Allow-Origin", "http://www.sosoapi.com");
		//如果存在自定义的header参数，需要在此处添加，逗号分隔
		resp.addHeader("Access-Control-Allow-Headers", "Origin, No-Cache, X-Requested-With, "
		    + "If-Modified-Since, Pragma, Last-Modified, Cache-Control, Expires, "
			+ "Content-Type, X-E4M-With");
		resp.addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
		
		chain.doFilter(request, response);
	
	}

	@Override
	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	
	
}
```










