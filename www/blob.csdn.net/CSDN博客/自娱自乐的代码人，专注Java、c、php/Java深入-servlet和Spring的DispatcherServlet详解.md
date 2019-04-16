# Java深入 - servlet和Spring的DispatcherServlet详解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月27日 14:42:27[initphp](https://me.csdn.net/initphp)阅读数：36967
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









### Servlet是什么

1. Servlet是服务器端运行的一个程序，是一个被编译好的Java类。它不是框架等。

2. Web容器的启动，需要依赖Servlet。当web服务器开始执行时，servlet类就被初始化

3. 当用户通过浏览器访问的时候，会输入URI，这个时候，web服务器就通过Servlet来分发请求执行不同的内容。




一般我们会使用Tomcat来运行Java的web项目。通常我们可以在web项目的目录中可以看到**WEB-INF**这样的文件夹。这个文件夹是受保护的，外部无法通过url来访问的。文件夹里面包含了**web.xml**以及**class**和**libs**目录。我们要将web项目运行起来，就得在web.xml中定义一个Servlet，因为定义了Servlet，web容器才能知道需要如何去分发请求进行业务处理的。

Servlet是有一个Jar包，里面包含了一些Servlet的相关设计和细节。

![](https://img-blog.csdn.net/20140727143312308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




一个使用Spring的Servlet的web.xml配置例子：



```
<servlet>
		<servlet-name>apiServlet</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:spring-common.xml</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>apiServlet</servlet-name>
		<url-pattern>/api/*</url-pattern>
	</servlet-mapping>
```








### **HttpServlet**

HttpServlet是一个抽象类，具体的实现自己需要的Servlet需要通过继承该类来实现 。

HttpServlet中主要的几个方法：


|方法|用途|
|----|----|
|doGet|处理HTTP GET请求|
|doPost|处理HTTP POST请求|
|doPut|处理HTTP PUT请求|
|doDelete|处理HTTP DELETE请求|

Servlet最终调用的是service方法，这个方法中会根据request的Method来判断具体是执行doGet还是doPost


可以看下HttpServlet类：



```java
package javax.servlet.http;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.text.MessageFormat;
import java.util.Enumeration;
import java.util.Locale;
import java.util.ResourceBundle;

import javax.servlet.GenericServlet;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;


public abstract class HttpServlet extends GenericServlet
    implements java.io.Serializable
{
    private static final String METHOD_DELETE = "DELETE";
    private static final String METHOD_HEAD = "HEAD";
    private static final String METHOD_GET = "GET";
    private static final String METHOD_OPTIONS = "OPTIONS";
    private static final String METHOD_POST = "POST";
    private static final String METHOD_PUT = "PUT";
    private static final String METHOD_TRACE = "TRACE";

    private static final String HEADER_IFMODSINCE = "If-Modified-Since";
    private static final String HEADER_LASTMOD = "Last-Modified";
    
    private static final String LSTRING_FILE =
	"javax.servlet.http.LocalStrings";
    private static ResourceBundle lStrings =
	ResourceBundle.getBundle(LSTRING_FILE);
   

    public HttpServlet() { }
    
    //处理HTTP的GET请求，你需要实现一个Servlet，然后实现该方法
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	String protocol = req.getProtocol();
	String msg = lStrings.getString("http.method_get_not_supported");
	if (protocol.endsWith("1.1")) {
	    resp.sendError(HttpServletResponse.SC_METHOD_NOT_ALLOWED, msg);
	} else {
	    resp.sendError(HttpServletResponse.SC_BAD_REQUEST, msg);
	}
    }

    protected long getLastModified(HttpServletRequest req) {
	return -1;
    }


    protected void doHead(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	NoBodyResponse response = new NoBodyResponse(resp);
	
	doGet(req, response);
	response.setContentLength();
    }
    
    //实现HTTP POST请求
    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	String protocol = req.getProtocol();
	String msg = lStrings.getString("http.method_post_not_supported");
	if (protocol.endsWith("1.1")) {
	    resp.sendError(HttpServletResponse.SC_METHOD_NOT_ALLOWED, msg);
	} else {
	    resp.sendError(HttpServletResponse.SC_BAD_REQUEST, msg);
	}
    }


    protected void doPut(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	String protocol = req.getProtocol();
	String msg = lStrings.getString("http.method_put_not_supported");
	if (protocol.endsWith("1.1")) {
	    resp.sendError(HttpServletResponse.SC_METHOD_NOT_ALLOWED, msg);
	} else {
	    resp.sendError(HttpServletResponse.SC_BAD_REQUEST, msg);
	}
    }


    protected void doDelete(HttpServletRequest req,
			    HttpServletResponse resp)
	throws ServletException, IOException
    {
	String protocol = req.getProtocol();
	String msg = lStrings.getString("http.method_delete_not_supported");
	if (protocol.endsWith("1.1")) {
	    resp.sendError(HttpServletResponse.SC_METHOD_NOT_ALLOWED, msg);
	} else {
	    resp.sendError(HttpServletResponse.SC_BAD_REQUEST, msg);
	}
    }
    

    private Method[] getAllDeclaredMethods(Class c) {

        if (c.equals(javax.servlet.http.HttpServlet.class)) {
            return null;
        }

        Method[] parentMethods = getAllDeclaredMethods(c.getSuperclass());
        Method[] thisMethods = c.getDeclaredMethods();
	
        if ((parentMethods != null) && (parentMethods.length > 0)) {
            Method[] allMethods =
                new Method[parentMethods.length + thisMethods.length];
	    System.arraycopy(parentMethods, 0, allMethods, 0,
                             parentMethods.length);
	    System.arraycopy(thisMethods, 0, allMethods, parentMethods.length,
                             thisMethods.length);

	    thisMethods = allMethods;
	}

	return thisMethods;
    }
         
    protected void doOptions(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	Method[] methods = getAllDeclaredMethods(this.getClass());
	
	boolean ALLOW_GET = false;
	boolean ALLOW_HEAD = false;
	boolean ALLOW_POST = false;
	boolean ALLOW_PUT = false;
	boolean ALLOW_DELETE = false;
	boolean ALLOW_TRACE = true;
	boolean ALLOW_OPTIONS = true;
	
	for (int i=0; i<methods.length; i++) {
	    Method m = methods[i];
	    
	    if (m.getName().equals("doGet")) {
		ALLOW_GET = true;
		ALLOW_HEAD = true;
	    }
	    if (m.getName().equals("doPost")) 
		ALLOW_POST = true;
	    if (m.getName().equals("doPut"))
		ALLOW_PUT = true;
	    if (m.getName().equals("doDelete"))
		ALLOW_DELETE = true;
	    
	}
	
	String allow = null;
	if (ALLOW_GET)
	    if (allow==null) allow=METHOD_GET;
	if (ALLOW_HEAD)
	    if (allow==null) allow=METHOD_HEAD;
	    else allow += ", " + METHOD_HEAD;
	if (ALLOW_POST)
	    if (allow==null) allow=METHOD_POST;
	    else allow += ", " + METHOD_POST;
	if (ALLOW_PUT)
	    if (allow==null) allow=METHOD_PUT;
	    else allow += ", " + METHOD_PUT;
	if (ALLOW_DELETE)
	    if (allow==null) allow=METHOD_DELETE;
	    else allow += ", " + METHOD_DELETE;
	if (ALLOW_TRACE)
	    if (allow==null) allow=METHOD_TRACE;
	    else allow += ", " + METHOD_TRACE;
	if (ALLOW_OPTIONS)
	    if (allow==null) allow=METHOD_OPTIONS;
	    else allow += ", " + METHOD_OPTIONS;
	
	resp.setHeader("Allow", allow);
    }
    
   
    protected void doTrace(HttpServletRequest req, HttpServletResponse resp) 
	throws ServletException, IOException
    {
	
	int responseLength;
	
	String CRLF = "\r\n";
	String responseString = "TRACE "+ req.getRequestURI()+
	    " " + req.getProtocol();
	
	Enumeration reqHeaderEnum = req.getHeaderNames();
	
	while( reqHeaderEnum.hasMoreElements() ) {
	    String headerName = (String)reqHeaderEnum.nextElement();
	    responseString += CRLF + headerName + ": " +
		req.getHeader(headerName); 
	}
	
	responseString += CRLF;
	
	responseLength = responseString.length();
	
	resp.setContentType("message/http");
	resp.setContentLength(responseLength);
	ServletOutputStream out = resp.getOutputStream();
	out.print(responseString);	
	out.close();
	return;
    }		

    //最终所有的具体实现都是需要实现Service方法.容器最终调用的就是该方法
    protected void service(HttpServletRequest req, HttpServletResponse resp)
	throws ServletException, IOException
    {
	String method = req.getMethod();
        //这边通过HTTP的方法来判断，具体的HTTP METHOD，然后决定需要执行的方法名称。   
	if (method.equals(METHOD_GET)) {
	    long lastModified = getLastModified(req);
	    if (lastModified == -1) {
		// servlet doesn't support if-modified-since, no reason
		// to go through further expensive logic
		doGet(req, resp);
	    } else {
		long ifModifiedSince = req.getDateHeader(HEADER_IFMODSINCE);
		if (ifModifiedSince < (lastModified / 1000 * 1000)) {
		    // If the servlet mod time is later, call doGet()
                    // Round down to the nearest second for a proper compare
                    // A ifModifiedSince of -1 will always be less
		    maybeSetLastModified(resp, lastModified);
		    doGet(req, resp);
		} else {
		    resp.setStatus(HttpServletResponse.SC_NOT_MODIFIED);
		}
	    }

	} else if (method.equals(METHOD_HEAD)) {
	    long lastModified = getLastModified(req);
	    maybeSetLastModified(resp, lastModified);
	    doHead(req, resp);
        
	} else if (method.equals(METHOD_POST)) {
	    doPost(req, resp);
	    
	} else if (method.equals(METHOD_PUT)) {
	    doPut(req, resp);	
	    
	} else if (method.equals(METHOD_DELETE)) {
	    doDelete(req, resp);
	    
	} else if (method.equals(METHOD_OPTIONS)) {
	    doOptions(req,resp);
	    
	} else if (method.equals(METHOD_TRACE)) {
	    doTrace(req,resp);
	    
	} else {
	    //
	    // Note that this means NO servlet supports whatever
	    // method was requested, anywhere on this server.
	    //

	    String errMsg = lStrings.getString("http.method_not_implemented");
	    Object[] errArgs = new Object[1];
	    errArgs[0] = method;
	    errMsg = MessageFormat.format(errMsg, errArgs);
	    
	    resp.sendError(HttpServletResponse.SC_NOT_IMPLEMENTED, errMsg);
	}
    }

    private void maybeSetLastModified(HttpServletResponse resp,
				      long lastModified) {
	if (resp.containsHeader(HEADER_LASTMOD))
	    return;
	if (lastModified >= 0)
	    resp.setDateHeader(HEADER_LASTMOD, lastModified);
    }
   
    public void service(ServletRequest req, ServletResponse res)
	throws ServletException, IOException
    {
	HttpServletRequest	request;
	HttpServletResponse	response;
	
	try {
	    request = (HttpServletRequest) req;
	    response = (HttpServletResponse) res;
	} catch (ClassCastException e) {
	    throw new ServletException("non-HTTP request or response");
	}
	service(request, response);
    }
}
```


HttpServlet最上层是继承Servlet这个接口类。Servlet这个接口类非常简单，只定义了5个方法。

可以看下下面的Servlet接口：



```java
package javax.servlet;

import java.io.IOException;




public interface Servlet {

    //容器初始化
    public void init(ServletConfig config) throws ServletException;
    
    
    //获取配置信息
    public ServletConfig getServletConfig();
    
    //具体的service 请求分发
    public void service(ServletRequest req, ServletResponse res)
	throws ServletException, IOException;
	
	
   //获取servlet信息
    public String getServletInfo();
    
    //容器销毁的时候
    public void destroy();
}
```



### Spring的DispatcherServlet

Spring主要通过DispatcherServlet实现了Servlet。DispatcherServlet最上层也是继承的是HttpServlet这个类。

我们主要看下DispatcherServlet两个比较重要的方法：



```java
@Override
	protected void doService(HttpServletRequest request, HttpServletResponse response) throws Exception {
		if (logger.isDebugEnabled()) {
			String requestUri = urlPathHelper.getRequestUri(request);
			logger.debug("DispatcherServlet with name '" + getServletName() + "' processing " + request.getMethod() +
					" request for [" + requestUri + "]");
		}

		// Keep a snapshot of the request attributes in case of an include,
		// to be able to restore the original attributes after the include.
		Map<String, Object> attributesSnapshot = null;
		if (WebUtils.isIncludeRequest(request)) {
			logger.debug("Taking snapshot of request attributes before include");
			attributesSnapshot = new HashMap<String, Object>();
			Enumeration<?> attrNames = request.getAttributeNames();
			while (attrNames.hasMoreElements()) {
				String attrName = (String) attrNames.nextElement();
				if (this.cleanupAfterInclude || attrName.startsWith("org.springframework.web.servlet")) {
					attributesSnapshot.put(attrName, request.getAttribute(attrName));
				}
			}
		}

		// Make framework objects available to handlers and view objects.
		request.setAttribute(WEB_APPLICATION_CONTEXT_ATTRIBUTE, getWebApplicationContext());
		request.setAttribute(LOCALE_RESOLVER_ATTRIBUTE, this.localeResolver);
		request.setAttribute(THEME_RESOLVER_ATTRIBUTE, this.themeResolver);
		request.setAttribute(THEME_SOURCE_ATTRIBUTE, getThemeSource());

		FlashMap inputFlashMap = this.flashMapManager.retrieveAndUpdate(request, response);
		if (inputFlashMap != null) {
			request.setAttribute(INPUT_FLASH_MAP_ATTRIBUTE, Collections.unmodifiableMap(inputFlashMap));
		}
		request.setAttribute(OUTPUT_FLASH_MAP_ATTRIBUTE, new FlashMap());
		request.setAttribute(FLASH_MAP_MANAGER_ATTRIBUTE, this.flashMapManager);

		try {
			doDispatch(request, response); //这边最终也是调用了doDispatch方法，该方法主要用来处理SPring框架的具体业务分发逻辑。
		}
		finally {
			// Restore the original attribute snapshot, in case of an include.
			if (attributesSnapshot != null) {
				restoreAttributesAfterInclude(request, attributesSnapshot);
			}
		}
	}
        
        //Spring框架最终的分发都是通过该方法的
	protected void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {
		HttpServletRequest processedRequest = request;
		HandlerExecutionChain mappedHandler = null;
		int interceptorIndex = -1;

		try {
			ModelAndView mv;
			boolean errorView = false;

			try {
				processedRequest = checkMultipart(request);

				// Determine handler for the current request.
				mappedHandler = getHandler(processedRequest, false);
				if (mappedHandler == null || mappedHandler.getHandler() == null) {
					noHandlerFound(processedRequest, response);
					return;
				}

				// Determine handler adapter for the current request.
				HandlerAdapter ha = getHandlerAdapter(mappedHandler.getHandler());

                // Process last-modified header, if supported by the handler.
				String method = request.getMethod();
				boolean isGet = "GET".equals(method);
				if (isGet || "HEAD".equals(method)) {
					long lastModified = ha.getLastModified(request, mappedHandler.getHandler());
					if (logger.isDebugEnabled()) {
						String requestUri = urlPathHelper.getRequestUri(request);
						logger.debug("Last-Modified value for [" + requestUri + "] is: " + lastModified);
					}
					if (new ServletWebRequest(request, response).checkNotModified(lastModified) && isGet) {
						return;
					}
				}

				// 这里是处理前置拦截器
				HandlerInterceptor[] interceptors = mappedHandler.getInterceptors();
				if (interceptors != null) {
					for (int i = 0; i < interceptors.length; i++) {
						HandlerInterceptor interceptor = interceptors[i];
						if (!interceptor.preHandle(processedRequest, response, mappedHandler.getHandler())) {
							triggerAfterCompletion(mappedHandler, interceptorIndex, processedRequest, response, null);
							return;
						}
						interceptorIndex = i;
					}
				}

				//处理最终的Action逻辑
				mv = ha.handle(processedRequest, response, mappedHandler.getHandler());

				// Do we need view name translation?
				if (mv != null && !mv.hasView()) {
					mv.setViewName(getDefaultViewName(request));
				}

                                //处理后置拦截器
				if (interceptors != null) {
					for (int i = interceptors.length - 1; i >= 0; i--) {
						HandlerInterceptor interceptor = interceptors[i];
						interceptor.postHandle(processedRequest, response, mappedHandler.getHandler(), mv);
					}
				}
			}
			catch (ModelAndViewDefiningException ex) {
				logger.debug("ModelAndViewDefiningException encountered", ex);
				mv = ex.getModelAndView();
			}
			catch (Exception ex) {
				Object handler = (mappedHandler != null ? mappedHandler.getHandler() : null);
				mv = processHandlerException(processedRequest, response, handler, ex);
				errorView = (mv != null);
			}

			// Did the handler return a view to render?
			if (mv != null && !mv.wasCleared()) {
				render(mv, processedRequest, response);
				if (errorView) {
					WebUtils.clearErrorRequestAttributes(request);
				}
			}
			else {
				if (logger.isDebugEnabled()) {
					logger.debug("Null ModelAndView returned to DispatcherServlet with name '" + getServletName() +
							"': assuming HandlerAdapter completed request handling");
				}
			}

			// Trigger after-completion for successful outcome.
			triggerAfterCompletion(mappedHandler, interceptorIndex, processedRequest, response, null);
		}

		catch (Exception ex) {
			// Trigger after-completion for thrown exception.
			triggerAfterCompletion(mappedHandler, interceptorIndex, processedRequest, response, ex);
			throw ex;
		}
		catch (Error err) {
			ServletException ex = new NestedServletException("Handler processing failed", err);
			// Trigger after-completion for thrown exception.
			triggerAfterCompletion(mappedHandler, interceptorIndex, processedRequest, response, ex);
			throw ex;
		}

		finally {
			// Clean up any resources used by a multipart request.
			if (processedRequest != request) {
				cleanupMultipart(processedRequest);
			}
		}
	}
```














