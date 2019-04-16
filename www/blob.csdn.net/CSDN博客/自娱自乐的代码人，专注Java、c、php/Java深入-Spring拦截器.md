# Java深入 - Spring拦截器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月19日 14:18:34[initphp](https://me.csdn.net/initphp)阅读数：14257
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Spring的拦截器具备在web的前置和后置来处理各种请求。




### 拦截器接口-HandlerInterceptor

自定义的拦截器，需要继承HandlerInterceptor接口，并且实现HandlerInterceptor中提供的三个方法：

1. preHandle 方法会在请求处理前被调用。这个方法返回boolean值，如果返回true则继续往下执行，如果返回false则中断。

2. postHandle 方法会在请求处理后，继续调用。

3. afterCompletion 方法会在视图渲染之后调用。



```java
/*
 * Copyright 2002-2006 the original author or authors.
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

package org.springframework.web.servlet;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Workflow interface that allows for customized handler execution chains.
 * Applications can register any number of existing or custom interceptors
 * for certain groups of handlers, to add common preprocessing behavior
 * without needing to modify each handler implementation.
 *
 * <p>A HandlerInterceptor gets called before the appropriate HandlerAdapter
 * triggers the execution of the handler itself. This mechanism can be used
 * for a large field of preprocessing aspects, e.g. for authorization checks,
 * or common handler behavior like locale or theme changes. Its main purpose
 * is to allow for factoring out repetitive handler code.
 *
 * <p>Typically an interceptor chain is defined per HandlerMapping bean,
 * sharing its granularity. To be able to apply a certain interceptor chain
 * to a group of handlers, one needs to map the desired handlers via one
 * HandlerMapping bean. The interceptors themselves are defined as beans
 * in the application context, referenced by the mapping bean definition
 * via its "interceptors" property (in XML: a <list> of <ref>).
 *
 * <p>HandlerInterceptor is basically similar to a Servlet 2.3 Filter, but in
 * contrast to the latter it just allows custom pre-processing with the option
 * of prohibiting the execution of the handler itself, and custom post-processing.
 * Filters are more powerful, for example they allow for exchanging the request
 * and response objects that are handed down the chain. Note that a filter
 * gets configured in web.xml, a HandlerInterceptor in the application context.
 *
 * <p>As a basic guideline, fine-grained handler-related preprocessing tasks are
 * candidates for HandlerInterceptor implementations, especially factored-out
 * common handler code and authorization checks. On the other hand, a Filter
 * is well-suited for request content and view content handling, like multipart
 * forms and GZIP compression. This typically shows when one needs to map the
 * filter to certain content types (e.g. images), or to all requests.
 *
 * @author Juergen Hoeller
 * @since 20.06.2003
 * @see HandlerExecutionChain#getInterceptors
 * @see org.springframework.web.servlet.handler.HandlerInterceptorAdapter
 * @see org.springframework.web.servlet.handler.AbstractHandlerMapping#setInterceptors
 * @see org.springframework.web.servlet.handler.UserRoleAuthorizationInterceptor
 * @see org.springframework.web.servlet.i18n.LocaleChangeInterceptor
 * @see org.springframework.web.servlet.theme.ThemeChangeInterceptor
 * @see javax.servlet.Filter
 */
public interface HandlerInterceptor {

	/**
	 * Intercept the execution of a handler. Called after HandlerMapping determined
	 * an appropriate handler object, but before HandlerAdapter invokes the handler.
	 * <p>DispatcherServlet processes a handler in an execution chain, consisting
	 * of any number of interceptors, with the handler itself at the end.
	 * With this method, each interceptor can decide to abort the execution chain,
	 * typically sending a HTTP error or writing a custom response.
	 * @param request current HTTP request
	 * @param response current HTTP response
	 * @param handler chosen handler to execute, for type and/or instance evaluation
	 * @return <code>true</code> if the execution chain should proceed with the
	 * next interceptor or the handler itself. Else, DispatcherServlet assumes
	 * that this interceptor has already dealt with the response itself.
	 * @throws Exception in case of errors
	 */
	boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
	    throws Exception;

	/**
	 * Intercept the execution of a handler. Called after HandlerAdapter actually
	 * invoked the handler, but before the DispatcherServlet renders the view.
	 * Can expose additional model objects to the view via the given ModelAndView.
	 * <p>DispatcherServlet processes a handler in an execution chain, consisting
	 * of any number of interceptors, with the handler itself at the end.
	 * With this method, each interceptor can post-process an execution,
	 * getting applied in inverse order of the execution chain.
	 * @param request current HTTP request
	 * @param response current HTTP response
	 * @param handler chosen handler to execute, for type and/or instance examination
	 * @param modelAndView the <code>ModelAndView</code> that the handler returned
	 * (can also be <code>null</code>)
	 * @throws Exception in case of errors
	 */
	void postHandle(
			HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView)
			throws Exception;

	/**
	 * Callback after completion of request processing, that is, after rendering
	 * the view. Will be called on any outcome of handler execution, thus allows
	 * for proper resource cleanup.
	 * <p>Note: Will only be called if this interceptor's <code>preHandle</code>
	 * method has successfully completed and returned <code>true</code>!
	 * <p>As with the {@code postHandle} method, the method will be invoked on each
	 * interceptor in the chain in reverse order, so the first interceptor will be
	 * the last to be invoked.
	 * @param request current HTTP request
	 * @param response current HTTP response
	 * @param handler chosen handler to execute, for type and/or instance examination
	 * @param ex exception thrown on handler execution, if any
	 * @throws Exception in case of errors
	 */
	void afterCompletion(
			HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)
			throws Exception;

}
```



自定义一个拦截器：



```java
/**
 * 一个自定义的拦截器，实现：preHandle postHandle afterCompletion 三个接口
 * @author zhuli
 * @date 2014-7-19
 */
public class DefaultInterceptor implements HandlerInterceptor {

    @Autowired
    private Gson gson;

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response,
 Object handler) throws Exception {
        System.out.print("preHandler");
        return true; //如果返回false则会中断
    }

    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, 
Object handler, ModelAndView modelAndView) throws Exception {
        System.out.print("postHandle");
    }

    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response,
 Object handler, Exception ex) throws Exception {
        System.out.print("afterCompletion");
    }

}
```


### 拦截器配置-DefaultAnnotationHandlerMapping

上面自定义的拦截器通过DefaultAnnotationHandlerMapping注入到spring框架中。

可以打开org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping，可以看到具体的注入细节。



```
<!-- 通过DefaultAnnotationHandlerMapping将拦截器注入 -->
	<bean
		class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping">
		<property name="interceptors">
			<list>
				<!-- 这边可以实现多个拦截器 -->
				<ref bean="DefaultInterceptor"  />
			</list>
		</property>
	</bean>
	<!-- 自定义的default的拦截器 -->
	<bean id="DefaultInterceptor" class="com.xxx.test.DefaultInterceptor">
	</bean>
```


还可以通过 <mvc:interceptors>  的方式配置：





```
<mvc:interceptors>
		<mvc:interceptor>
			<mvc:mapping path="/api/time/get_time/" />
			<bean class="com.xxx.test.DefaultInterceptor"></bean>  
		</mvc:interceptor>
	</mvc:interceptors>
```


### 拦截器在哪里运行

我们可以找到

org.springframework.web.servlet.DispatcherServlet

里面的代码中找到doDispatch方法，我们可以看到相应的拦截器的方法执行

**preHandle**：



```java
// Apply preHandle methods of registered interceptors.
				HandlerInterceptor[] interceptors = mappedHandler.getInterceptors();
				if (interceptors != null) {
					for (int i = 0; i < interceptors.length; i++) {
                                                //这里获取拦截器
                                                HandlerInterceptor interceptor = interceptors[i];
                                                //执行拦截器的preHandle方法，如果为false，则中断
                                                if (!interceptor.preHandle(processedRequest, response, 
mappedHandler.getHandler())) {
							triggerAfterCompletion(mappedHandler, interceptorIndex, 
processedRequest, response, null);
							return;
						}
						interceptorIndex = i;
					}
				}
```



**postHandle：**



```java
// Actually invoke the handler.
				mv = ha.handle(processedRequest, response, mappedHandler.getHandler()); //这里执行具体业务

				// Do we need view name translation?
				if (mv != null && !mv.hasView()) {
					mv.setViewName(getDefaultViewName(request));
				}

				// Apply postHandle methods of registered interceptors.
				if (interceptors != null) {
					for (int i = interceptors.length - 1; i >= 0; i--) {
						HandlerInterceptor interceptor = interceptors[i]; //拦截器
						interceptor.postHandle(processedRequest, response, 
mappedHandler.getHandler(), mv); //执行post拦截器
					}
				}
```



**afterCompletion**：


```java
private void triggerAfterCompletion(HandlerExecutionChain mappedHandler,
			int interceptorIndex,
			HttpServletRequest request,
			HttpServletResponse response,
			Exception ex) throws Exception {

		// Apply afterCompletion methods of registered interceptors.
		if (mappedHandler != null) {
			HandlerInterceptor[] interceptors = mappedHandler.getInterceptors();
			if (interceptors != null) {
				for (int i = interceptorIndex; i >= 0; i--) {
					HandlerInterceptor interceptor = interceptors[i];
					try {
						interceptor.afterCompletion(request, response, mappedHandler.getHandler(), 
ex); //这里执行afterCompletion
					}
					catch (Throwable ex2) {
						logger.error("HandlerInterceptor.afterCompletion threw exception", ex2);
					}
				}
			}
		}
	}
```



### 拦截器和过滤器filter的区别：

1. Filter是Servlet规范规定的，只能用于web程序中。而拦截器既可以用于web程序，也可以用于Application、Swing程序中。

2. Filter是在Servlet规范中定义的，是Servlet容器支持的。而拦截器是在Spring容器中的，是Spring框架支持的。

3. 同其他代码块一样，拦截器也是一个Spring的组件，归Spring管理，配置在Spring文件中，因此能使用Spring里的任何资源、对象，例如Service对象、数据源、事务管理等，通过IoC注入到拦截器即可；而Filter则不能

4. Filter只在Servlet前后起作用。而拦截器能够深入得到方法前后、异常抛出前后等，因为拦截器的使用具有更大的弹性。所以在Spring构架的程序中，要优先使用拦截器。
















