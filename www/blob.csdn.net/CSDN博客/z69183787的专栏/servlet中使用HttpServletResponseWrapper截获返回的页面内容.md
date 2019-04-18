# servlet中使用HttpServletResponseWrapper截获返回的页面内容 - z69183787的专栏 - CSDN博客
2014年07月07日 13:36:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2921
  像标题所说的，虽然在在JEE开发中并不常见，但也不是没有需要。比如知名的页面装饰框架sitemesh，就是利用filter过滤器先截获返回给客户端的页面，然后分析html代码并最终装饰页面效果后返回给客户端。我这里也先简单的说下如何来自己编程实现，然后说一个我遇到的问题，应该也是大家会遇到但很难找到原因的问题。
      要截获页面返回的内容，整体的思路是先把原始返回的页面内容写入到一个字符Writer，然后再组装成字符串并进行分析，最后再返回给客户端。代码如下：
```java
package cc.mzone.test;
 
import java.io.CharArrayWriter;
import java.io.PrintWriter;
 
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;
 
/**
 * 自定义一个响应结果包装器，将在这里提供一个基于内存的输出器来存储所有
 * 返回给客户端的原始HTML代码。
 * 
 * @author 铁木箱子
 *
 */
public class ResponseWrapper extends HttpServletResponseWrapper {
	private PrintWriter cachedWriter;
	private CharArrayWriter bufferedWriter;
 
	public ResponseWrapper(HttpServletResponse response) {
		super(response);
		// 这个是我们保存返回结果的地方
		bufferedWriter = new CharArrayWriter();
		// 这个是包装PrintWriter的，让所有结果通过这个PrintWriter写入到bufferedWriter中
		cachedWriter = new PrintWriter(bufferedWriter);
	}
 
	@Override
	public PrintWriter getWriter() {
		return cachedWriter;
	}
 
	/**
	 * 获取原始的HTML页面内容。
	 * @return
	 */
	public String getResult() {
		return bufferedWriter.toString();
	}
}
```
然后再写一个过滤器来截获内容并处理：
```java
package cc.mzone.test;
 
import java.io.IOException;
import java.io.PrintWriter;
 
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;
 
public class MyServletFilter implements Filter {
 
	@Override
	public void destroy() {
		// TODO Auto-generated method stub
 
	}
 
	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		// 使用我们自定义的响应包装器来包装原始的ServletResponse
		ResponseWrapper wrapper = new ResponseWrapper((HttpServletResponse)response);
		// 这句话非常重要，注意看到第二个参数是我们的包装器而不是response
		chain.doFilter(request, wrapper);
		// 处理截获的结果并进行处理，比如替换所有的“名称”为“铁木箱子”
		String result = wrapper.getResult();
		result = result.replace("名称", "铁木箱子");
		// 输出最终的结果
		PrintWriter out = response.getWriter();
		out.write(result);
		out.flush();
		out.close();
	}
 
	@Override
	public void init(FilterConfig filterConfig) throws ServletException {
		// TODO Auto-generated method stub
 
	}
 
}
```
然后将该servlet配置在web.xml文件中，如下：
```
<filter>
    <filter-name>myFilter</filter-name>
    <filter-class>cc.mzone.test.MyServletFilter</filter-class>
</filter>
<filter-mapping>
    <filter-name>myFilter</filter-name>
    <url-pattern>*.jsp</url-pattern>
</filter-mapping>
```
我们上面配置的是只过滤访问后缀为.jsp的url地址，当然你可以根据自己的需要修改成其他的过滤模式。然后我们在web应用根目录下建立一个jsp文件test.jsp，内容如下：
```
<%@ page language="java" pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<html>
<head>
    <title>页面返回结果过滤测试</title></head>
</head>
<body>
你好，我叫“名称”。
</body>
</html>
```
   配置完后，部署到tomcat，然后访问应用下的test.jsp文件，就可以发现返回的内容变成了：
- 
你好，我叫“铁木箱子”
      而不是页面中原始的内容：
- 
你好，我叫“名称”
      从而也就达到了我们想要的效果了。在文章开头我也提到了说有一个问题，那就是有可能在运行的过程中页面只输出一部分，尤其是在使用多个框架后(比如sitemesh)出现的可能性非常大，在探究了好久之后终于发现原来是响应的ContentLength惹的祸。因为在经过多个过滤器或是框架处理后，很有可能在其他框架中设置了响应的输出内容的长度，导致浏览器只根据得到的长度头来显示部分内容。知道了原因，处理起来就比较方便了，我们在处理结果输出前重置一下ContentLength即可，如下：
```java
// 重置响应输出的内容长度
response.setContentLength(-1);
// 输出最终的结果
PrintWriter out = response.getWriter();
out.write(result);
out.flush();
out.close();
```
这样处理后就不会再出现只出现部分页面的问题了！
