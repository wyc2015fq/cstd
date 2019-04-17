# Spring容器托管的HttpServletRequest对象被获取之后再次获取 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月14日 14:23:42[boonya](https://me.csdn.net/boonya)阅读数：81








**目录**

[通过上下文过滤器拿走HttpServletRequest请求](#%E9%80%9A%E8%BF%87%E4%B8%8A%E4%B8%8B%E6%96%87%E8%BF%87%E6%BB%A4%E5%99%A8%E6%8B%BF%E8%B5%B0%E8%AF%B7%E6%B1%82)

[RequestContextHolder获取HttpServletRequest](#RequestContextHolder%E8%8E%B7%E5%8F%96HttpServletRequest)

HttpServletRequest对象被获取一次之后再次去操作与流相关的操作会抛异常，原因是HttpServletRequest不允许直接对其流的读取进行二次访问，如果要访问我们必须获取到原始的HttpServletRequest或者是由Spring托管的本地化线程的HttpServletRequest对象。RequestContextHolder就可以帮我们办到。

### 通过上下文过滤器拿走HttpServletRequest请求

以下是过滤器反编译代码：

```java
/*     */ package com.xxx.core.filter;
/*     */ 
/*     */ import java.io.IOException;
/*     */ import javax.servlet.Filter;
/*     */ import javax.servlet.FilterChain;
/*     */ import javax.servlet.FilterConfig;
/*     */ import javax.servlet.ServletContext;
/*     */ import javax.servlet.ServletException;
/*     */ import javax.servlet.ServletRequest;
/*     */ import javax.servlet.ServletResponse;
/*     */ import javax.servlet.annotation.WebFilter;
/*     */ import javax.servlet.http.HttpServletRequest;
/*     */ import javax.servlet.http.HttpServletResponse;
/*     */ import javax.servlet.http.HttpSession;
/*     */ import org.apache.log4j.Logger;

/*     */ @WebFilter(filterName="context_Filter", urlPatterns={"/*"})
/*     */ public class ContextFilter
/*     */   implements Filter
/*     */ {
/*  34 */   static Logger logger = Logger.getLogger(ContextFilter.class);
/*     */   
/*     */ 
/*  37 */   private static ThreadLocal<HttpServletRequest> threadLocalRequest = new ThreadLocal();
/*  38 */   private static ThreadLocal<HttpServletResponse> threadLocalResponse = new ThreadLocal();
/*     */   
/*  40 */   private static ThreadLocal<String> threadLocalObject = new ThreadLocal();
/*     */   
/*     */ 
/*     */ 
/*     */   public void destroy() {}
/*     */   
/*     */ 
/*     */   public void doFilter(ServletRequest arg0, ServletResponse arg1, FilterChain arg2)
/*     */     throws IOException, ServletException
/*     */   {
/*  50 */     threadLocalRequest.set((HttpServletRequest)arg0);
/*  51 */     threadLocalResponse.set((HttpServletResponse)arg1);
/*  52 */     arg2.doFilter(arg0, arg1);
/*     */   }
/*     */   
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */   public void init(FilterConfig arg0)
/*     */     throws ServletException
/*     */   {}
/*     */   
/*     */ 
/*     */ 
/*     */ 
/*     */   public static HttpServletRequest getRequest()
/*     */   {
/*  68 */     HttpServletRequest req = (HttpServletRequest)threadLocalRequest.get();
/*  69 */     return req;
/*     */   }
/*     */   
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */   public static HttpSession getSession()
/*     */   {
/*  81 */     HttpServletRequest req = (HttpServletRequest)threadLocalRequest.get();
/*  82 */     if (req == null) {
/*  83 */       return null;
/*     */     }
/*  85 */     return req.getSession();
/*     */   }
/*     */   
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */ 
/*     */   public ServletContext getServletContext()
/*     */   {
/*  97 */     if (null == getSession()) {
/*  98 */       return null;
/*     */     }
/* 100 */     return getSession().getServletContext();
/*     */   }
/*     */   
/*     */   public static void setSchema(String value)
/*     */   {
/* 105 */     if (null == getRequest()) {
/* 106 */       threadLocalObject.set(value);
/*     */     } else {
/* 108 */       getRequest().setAttribute("_schema_", value);
/*     */     }
/* 110 */     logger.debug(" setSchema >> id=" + Thread.currentThread().getId() + ",name " + Thread.currentThread().getName() + "," + value);
/*     */   }
/*     */   
/*     */   public static String getSchema() {
/* 114 */     if (null == getRequest()) {
/* 115 */       return (String)threadLocalObject.get();
/*     */     }
/* 117 */     return (String)getRequest().getAttribute("_schema_");
/*     */   }
/*     */ }

/* Location:           D:\DEVELOPEWORKS\dataissue\WebRoot\WEB-INF\lib\forestar-core-3.6.4-SNAPSHOT.jar
 * Qualified Name:     com.forestar.core.filter.ContextFilter
 * Java Class Version: 7 (51.0)
 * JD-Core Version:    0.7.1
 */
```

上面表现的处理中我们已经取走了HttpServletRequest对象作为本地化线程的请求保存起来了。我们在其他地方可以这样使用而不用将HttpServletRequest作为参数传递：

```java
HttpServletRequest  request = ContextFilter.getRequest();
```

但是当需要读取流的时候，我们就会遇到问题。此时我们需要获得持有被Spring管理的HttpServletRequest。

### RequestContextHolder获取HttpServletRequest

获取HttpServletRequest：

```java
HttpServletRequest request = ((ServletRequestAttributes) RequestContextHolder
        .getRequestAttributes()).getRequest();
```

RequestContextHolder源码：

```java
public abstract class RequestContextHolder  {

    private static final ThreadLocal<RequestAttributes> requestAttributesHolder =
            new NamedThreadLocal<RequestAttributes>("Request attributes");

    private static final ThreadLocal<RequestAttributes> inheritableRequestAttributesHolder =
            new NamedInheritableThreadLocal<RequestAttributes>("Request context");


    public static void resetRequestAttributes() {
        requestAttributesHolder.remove();
        inheritableRequestAttributesHolder.remove();
    }


    public static void setRequestAttributes(RequestAttributes attributes) {
        setRequestAttributes(attributes, false);
    }

    //将RequestAttributes对象放入到ThreadLocal中，而HttpServletRequest和HttpServletResponse等则封装在RequestAttributes对象中，在此处就不对RequestAttributes这个类展开。反正我们需要知道的就是要获取RequestAttributes对象，然后再从RequestAttributes对象中获取到我们所需要的HttpServletRequest即可
    public static void setRequestAttributes(RequestAttributes attributes, boolean inheritable) {
        if (attributes == null) {
            resetRequestAttributes();
        }
        else {
            if (inheritable) {
                inheritableRequestAttributesHolder.set(attributes);
                requestAttributesHolder.remove();
            }
            else {
                requestAttributesHolder.set(attributes);
                inheritableRequestAttributesHolder.remove();
            }
        }
    }

    public static RequestAttributes getRequestAttributes() {
        RequestAttributes attributes = requestAttributesHolder.get();
        if (attributes == null) {
            attributes = inheritableRequestAttributesHolder.get();
        }
        return attributes;
    }

}
```

那么在spring-mvc中是怎么实现的呢，我们来简单分析的，想了解具体机制的可以去看看spring-mvc的源码。

我们看下FrameworkServlet这个类，也就是DispatcherServlet的父类，里面有个processRequest方法，根据方法名称我们也可以大概了解到这个是方法用于处理请求的。

```java
protected final void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        long startTime = System.currentTimeMillis();
        Throwable failureCause = null;

        LocaleContext previousLocaleContext = LocaleContextHolder.getLocaleContext();
        LocaleContext localeContext = buildLocaleContext(request);

        RequestAttributes previousAttributes = RequestContextHolder.getRequestAttributes();
        ServletRequestAttributes requestAttributes = buildRequestAttributes(request, response, previousAttributes);

        WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);
        asyncManager.registerCallableInterceptor(FrameworkServlet.class.getName(), new RequestBindingInterceptor());

        //将RequestAttributes设置到RequestContextHolder
        initContextHolders(request, localeContext, requestAttributes);

        try {
            //具体的业务逻辑
            doService(request, response);
        }
        catch (ServletException ex) {
            failureCause = ex;
            throw ex;
        }
        catch (IOException ex) {
            failureCause = ex;
            throw ex;
        }
        catch (Throwable ex) {
            failureCause = ex;
            throw new NestedServletException("Request processing failed", ex);
        }

        finally {
            //重置RequestContextHolder之前设置RequestAttributes
            resetContextHolders(request, previousLocaleContext, previousAttributes);
            if (requestAttributes != null) {
                requestAttributes.requestCompleted();
            }

            if (logger.isDebugEnabled()) {
                if (failureCause != null) {
                    this.logger.debug("Could not complete request", failureCause);
                }
                else {
                    if (asyncManager.isConcurrentHandlingStarted()) {
                        logger.debug("Leaving response open for concurrent processing");
                    }
                    else {
                        this.logger.debug("Successfully completed request");
                    }
                }
            }

            publishRequestHandledEvent(request, response, startTime, failureCause);
        }
    }
    
    
    private void initContextHolders(
            HttpServletRequest request, LocaleContext localeContext, RequestAttributes requestAttributes) {

        if (localeContext != null) {
            LocaleContextHolder.setLocaleContext(localeContext, this.threadContextInheritable);
        }
        if (requestAttributes != null) {
            RequestContextHolder.setRequestAttributes(requestAttributes, this.threadContextInheritable);
        }
        if (logger.isTraceEnabled()) {
            logger.trace("Bound request context to thread: " + request);
        }
    }
```

我们可以知道HttpServletRequest是在执行doService方法之前，也就是具体的业务逻辑前进行设置的，然后在执行完业务逻辑或者抛出异常时重置RequestContextHolder移除当前的HttpServletRequest。

更多阅读：

[【Spring】RequestContextHolder简析](https://www.jianshu.com/p/80165b7743cf)

[SpringMVC学习记录(九)--RequestContextHolder分析](https://blog.csdn.net/u012706811/article/details/53432032)



