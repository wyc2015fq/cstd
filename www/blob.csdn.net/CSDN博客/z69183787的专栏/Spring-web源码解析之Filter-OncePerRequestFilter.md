# Spring-web源码解析之Filter-OncePerRequestFilter: - z69183787的专栏 - CSDN博客
2017年08月15日 12:32:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：917
我们先看一个filter-mapping的配置 
**[html]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- <filter-mapping>
- <filter-name>encodingFilter</filter-name>
- <url-pattern>/*</url-pattern>
- <dispatcher>REQUEST</dispatcher>
- <dispatcher>ASYNC</dispatcher>
- </filter-mapping>
这里指定了一个ASYNC的配置，表明过滤异步请求，这个ASYNC即是枚举类DispatcherType中的一个元素，在Servlet3.0中，如果一个请求是DispatcherType.ASYNC类型的，那么在一个单一请求的过程中，filter能够被多个线程调用，也就是意味着一个filter可能在一次请求中被多次执行，这显然是会有问题的，那么[spring](http://lib.csdn.net/base/javaee)是怎么避免这个问题的呢？这就是今天要说的OncePerRequestFilter。
直接看doFilter方法
**[java]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- @Override
- publicfinalvoid doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain)  
- throws ServletException, IOException {  
- 
- if (!(request instanceof HttpServletRequest) || !(response instanceof HttpServletResponse)) {  
- thrownew ServletException("OncePerRequestFilter just supports HTTP requests");  
-    }  
-    HttpServletRequest httpRequest = (HttpServletRequest) request;  
-    HttpServletResponse httpResponse = (HttpServletResponse) response;  
- 
-    String alreadyFilteredAttributeName = getAlreadyFilteredAttributeName();  
- boolean hasAlreadyFilteredAttribute = request.getAttribute(alreadyFilteredAttributeName) != null;  
- 
- if (hasAlreadyFilteredAttribute || skipDispatch(httpRequest) || shouldNotFilter(httpRequest)) {  
- 
- // Proceed without invoking this filter...
-       filterChain.doFilter(request, response);  
-    }  
- else {  
- // Do invoke this filter...
-       request.setAttribute(alreadyFilteredAttributeName, Boolean.TRUE);  
- try {  
-          doFilterInternal(httpRequest, httpResponse, filterChain);  
-       }  
- finally {  
- // Remove the "already filtered" request attribute for this request.
-          request.removeAttribute(alreadyFilteredAttributeName);  
-       }  
-    }  
- }  
从源码可以看出，spring会给已经过滤过的request设置一个attribute，在filter链和目标方法执行完毕之后才会释放这个attribute，attribute的名字是从  getAlreadyFilteredAttributeName() 方法得来，默认为filter的名字加后缀，如果filter没有完全初始化，则改为类名加后缀，后缀为“.FILTERED”
**[java]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- protected String getAlreadyFilteredAttributeName() {  
-    String name = getFilterName();  
- if (name == null) {  
-       name = getClass().getName();  
-    }  
- return name + ALREADY_FILTERED_SUFFIX;  
- }  
获取完名字之后，需要进行判断是否已经执行过这个filter了，判断条件有3个
1 是否有hasAlreadyFilteredAttribute 
2 是否skipDispatch
3 是否不进行过滤
我们直接看2和3，步骤3里，根据shouldNotFilter(httpRequest)来判断是否进行过滤，其实现依赖于子类。
在2里面判断条件有两种
**[java]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- privateboolean skipDispatch(HttpServletRequest request) {  
- if (isAsyncDispatch(request) && shouldNotFilterAsyncDispatch()) {  
- returntrue;  
-    }  
- if (request.getAttribute(WebUtils.ERROR_REQUEST_URI_ATTRIBUTE) != null && shouldNotFilterErrorDispatch()) {  
- returntrue;  
-    }  
- returnfalse;  
- }  
1 是异步并且不应该过滤异步，则skipDispatch为true，即不进行过滤
2 是ERROR请求并且不应该过滤ERROR，同样返回true
在上述所有判断条件完成之后，就可以决定是否执行
**[java]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- doFilterInternal(httpRequest, httpResponse, filterChain);  
方法了，这个方法就是子类具体实现的方法，其中之一便是前一篇文章中讲的CharacterEncodingFilter。
还需要注意的一个方法是 
**[java]**[view
 plain](http://blog.csdn.net/ktlifeng/article/details/50630934#)[copy](http://blog.csdn.net/ktlifeng/article/details/50630934#)
- protectedboolean isAsyncStarted(HttpServletRequest request) {  
- return WebAsyncUtils.getAsyncManager(request).isConcurrentHandlingStarted();  
- }  
如果这个返回true，那么当前线程结束时不会将response提交回去。
在Spring中，filter默认继承OncePerRequestFilter，
OncePerRequestFilter源代码如下：
```
![复制代码](http://img.voidcn.com/vcimg/000/000/005/338_48f_eef.gif)
```java
/*
 * Copyright 2002-2008 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.springframework.web.filter;
import java.io.IOException;
import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
/**
 * Filter base class that guarantees to be just executed once per request,
 * on any servlet container. It provides a {@link #doFilterInternal}
 * method with HttpServletRequest and HttpServletResponse arguments.
 *
 * <p>The {@link #getAlreadyFilteredAttributeName} method determines how
 * to identify that a request is already filtered. The default implementation
 * is based on the configured name of the concrete filter instance.
 *
 * @author Juergen Hoeller
 * @since 06.12.2003
 */
public abstract class OncePerRequestFilter extends GenericFilterBean {
    /**
     * Suffix that gets appended to the filter name for the
     * "already filtered" request attribute.
     * @see #getAlreadyFilteredAttributeName
     */
    public static final String ALREADY_FILTERED_SUFFIX = ".FILTERED";
    /**
     * This <code>doFilter</code> implementation stores a request attribute for
     * "already filtered", proceeding without filtering again if the
     * attribute is already there.
     * @see #getAlreadyFilteredAttributeName
     * @see #shouldNotFilter
     * @see #doFilterInternal
     */
    public final void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        if (!(request instanceof HttpServletRequest) || !(response instanceof HttpServletResponse)) {
            throw new ServletException("OncePerRequestFilter just supports HTTP requests");
        }
        HttpServletRequest httpRequest = (HttpServletRequest) request;
        HttpServletResponse httpResponse = (HttpServletResponse) response;
        String alreadyFilteredAttributeName = getAlreadyFilteredAttributeName();
        if (request.getAttribute(alreadyFilteredAttributeName) != null || shouldNotFilter(httpRequest)) {
            // Proceed without invoking this filter...
            filterChain.doFilter(request, response);
        }
        else {
            // Do invoke this filter...
            request.setAttribute(alreadyFilteredAttributeName, Boolean.TRUE);
            try {
                doFilterInternal(httpRequest, httpResponse, filterChain);
            }
            finally {
                // Remove the "already filtered" request attribute for this request.
                request.removeAttribute(alreadyFilteredAttributeName);
            }
        }
    }
    /**
     * Return the name of the request attribute that identifies that a request
     * is already filtered.
     * <p>Default implementation takes the configured name of the concrete filter
     * instance and appends ".FILTERED". If the filter is not fully initialized,
     * it falls back to its class name.
     * @see #getFilterName
     * @see #ALREADY_FILTERED_SUFFIX
     */
    protected String getAlreadyFilteredAttributeName() {
        String name = getFilterName();
        if (name == null) {
            name = getClass().getName();
        }
        return name + ALREADY_FILTERED_SUFFIX;
    }
    /**
     * Can be overridden in subclasses for custom filtering control,
     * returning <code>true</code> to avoid filtering of the given request.
     * <p>The default implementation always returns <code>false</code>.
     * @param request current HTTP request
     * @return whether the given request should <i>not</i> be filtered
     * @throws ServletException in case of errors
     */
    protected boolean shouldNotFilter(HttpServletRequest request) throws ServletException {
        return false;
    }
    /**
     * Same contract as for <code>doFilter</code>, but guaranteed to be
     * just invoked once per request. Provides HttpServletRequest and
     * HttpServletResponse arguments instead of the default ServletRequest
     * and ServletResponse ones.
     */
    protected abstract void doFilterInternal(
            HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException;
}
```
![复制代码](http://img.voidcn.com/vcimg/000/000/005/338_48f_eef.gif)
```
OncePerRequestFilter，顾名思义，它能够确保在一次请求中只通过一次filter，而需要重复的执行。大家常识上都认为，一次请求本来就只filter一次，为什么还要由此特别限定呢，往往我们的常识和实际的实现并不真的一样，经过一番资料的查阅，此方法是为了兼容不同的web container，也就是说并不是所有的container都入我们期望的只过滤一次，servlet版本不同，执行过程也不同，
请看官方对OncePerRequestFilter的注释：
```
![复制代码](http://img.voidcn.com/vcimg/000/000/005/338_48f_eef.gif)
```
/**
 * Filter base class that guarantees to be just executed once per request,
 * on any servlet container. It provides a {@link #doFilterInternal}
 * method with HttpServletRequest and HttpServletResponse arguments.
 *
 * <p>The {@link #getAlreadyFilteredAttributeName} method determines how
 * to identify that a request is already filtered. The default implementation
 * is based on the configured name of the concrete filter instance.
 *
 * @author Juergen Hoeller
 * @since 06.12.2003
 */
```
![复制代码](http://img.voidcn.com/vcimg/000/000/005/338_48f_eef.gif)
```
如：servlet2.3与servlet2.4也有一定差异：
```
```
在servlet2.3中，Filter会经过一切请求，包括服务器内部使用的forward转发请求和<%@ include file=”/login.jsp”%>的情况
servlet2.4中的Filter默认情况下只过滤外部提交的请求，forward和include这些内部转发都不会被过滤，
```
```
因此，为了兼容各种不同运行环境和版本，默认filter继承OncePerRequestFilter是一个比较稳妥的选择
