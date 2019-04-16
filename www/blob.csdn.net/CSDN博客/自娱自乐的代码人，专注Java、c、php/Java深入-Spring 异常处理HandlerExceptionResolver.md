# Java深入 - Spring 异常处理HandlerExceptionResolver - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年09月03日 16:28:01[initphp](https://me.csdn.net/initphp)阅读数：41596
个人分类：[Java深入系列](https://blog.csdn.net/initphp/article/category/2332145)

所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Spring的异常统一处理非常简单，首先我们需要看一下Spring中定义的HandlerExceptionResolver接口：



```java
/**
 * Interface to be implemented by objects than can resolve exceptions thrown
 * during handler mapping or execution, in the typical case to error views.
 * Implementors are typically registered as beans in the application context.
 *
 * <p>Error views are analogous to the error page JSPs, but can be used with
 * any kind of exception including any checked exception, with potentially
 * fine-granular mappings for specific handlers.
 *
 * @author Juergen Hoeller
 * @since 22.11.2003
 */
public interface HandlerExceptionResolver {

	/**
	 * Try to resolve the given exception that got thrown during on handler execution,
	 * returning a ModelAndView that represents a specific error page if appropriate.
	 * <p>The returned ModelAndView may be {@linkplain ModelAndView#isEmpty() empty}
	 * to indicate that the exception has been resolved successfully but that no view
	 * should be rendered, for instance by setting a status code.
	 * @param request current HTTP request
	 * @param response current HTTP response
	 * @param handler the executed handler, or <code>null</code> if none chosen at the
	 * time of the exception (for example, if multipart resolution failed)
	 * @param ex the exception that got thrown during handler execution
	 * @return a corresponding ModelAndView to forward to,
	 * or <code>null</code> for default processing
	 */
	ModelAndView resolveException(
			HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex);

}
```


他定义了一个resolveException方法，我们如果要处理异常的话，需要实现这个接口类，并且实现resolveException方法，在resolveException方法中处理自己的异常逻辑。



例如我设计一个自定义的异常处理类：



```java
/**
 * 自定义一个异常捕获处理类
 * @author zhuli
 * @date 2014-9-3
 */
public class MyExceptionResolver implements HandlerExceptionResolver {

    private static final Logger logger = LoggerFactory.getLogger(MyExceptionResolver.class);

    @Override
    public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        
        //可以自由处理各种异常逻辑
        if (ex instanceof org.springframework.web.HttpRequestMethodNotSupportedException) {
            logger.warn(Logger.ACTION.DEFAULT, "Http Method Error");
        }
        return null;
    }

}
```


然后需要将我们的我们自定义的MyExceptionResolver类注入到bean中




`<bean id="MyExceptionResolver" class="com.tiantian.xxx.web.handler.MyExceptionResolver"/>`

具体Spring是怎么调用的？我们可以先看下Spring中的doDispatch方法中，有这么一段代码：





```java
catch (ModelAndViewDefiningException ex) {
				logger.debug("ModelAndViewDefiningException encountered", ex);
				mv = ex.getModelAndView();
			}
			catch (Exception ex) {
				Object handler = (mappedHandler != null ? mappedHandler.getHandler() : null);
				mv = processHandlerException(processedRequest, response, handler, ex);
				errorView = (mv != null);
			}
```


其中processHandlerException方法就是来捕获异常处理的，那么继续看processHandlerException这个方法：





```java
protected ModelAndView processHandlerException(HttpServletRequest request, HttpServletResponse response,
			Object handler, Exception ex) throws Exception {

		// Check registered HandlerExceptionResolvers...
		ModelAndView exMv = null;
		for (HandlerExceptionResolver handlerExceptionResolver : this.handlerExceptionResolvers) {
			exMv = handlerExceptionResolver.resolveException(request, response, handler, ex);
			if (exMv != null) {
				break;
			}
		}
		if (exMv != null) {
			if (exMv.isEmpty()) {
				return null;
			}
			// We might still need view name translation for a plain error model...
			if (!exMv.hasView()) {
				exMv.setViewName(getDefaultViewName(request));
			}
			if (logger.isDebugEnabled()) {
				logger.debug("Handler execution resulted in exception - forwarding to resolved error view: " + exMv, ex);
			}
			WebUtils.exposeErrorRequestAttributes(request, ex, getServletName());
			return exMv;
		}

		throw ex;
	}
```


这个方法中的handlerExceptionResolver.resolveException就是用来捕获异常的，并且Spring允许多个自定义的异常类实现。



可以看this.handlerExceptionResolvers方法，跟踪进去



```java
private void initHandlerExceptionResolvers(ApplicationContext context) {
		this.handlerExceptionResolvers = null;

		if (this.detectAllHandlerExceptionResolvers) {
			// Find all HandlerExceptionResolvers in the ApplicationContext, including ancestor contexts.
			Map<String, HandlerExceptionResolver> matchingBeans = BeanFactoryUtils
					.beansOfTypeIncludingAncestors(context, HandlerExceptionResolver.class, true, false);
			if (!matchingBeans.isEmpty()) {
				this.handlerExceptionResolvers = new ArrayList<HandlerExceptionResolver>(matchingBeans.values());
				// We keep HandlerExceptionResolvers in sorted order.
				OrderComparator.sort(this.handlerExceptionResolvers);
			}
		}
		else {
			try {
				HandlerExceptionResolver her =
						context.getBean(HANDLER_EXCEPTION_RESOLVER_BEAN_NAME, HandlerExceptionResolver.class);
				this.handlerExceptionResolvers = Collections.singletonList(her);
			}
			catch (NoSuchBeanDefinitionException ex) {
				// Ignore, no HandlerExceptionResolver is fine too.
			}
		}

		// Ensure we have at least some HandlerExceptionResolvers, by registering
		// default HandlerExceptionResolvers if no other resolvers are found.
		if (this.handlerExceptionResolvers == null) {
			this.handlerExceptionResolvers = getDefaultStrategies(context, HandlerExceptionResolver.class);
			if (logger.isDebugEnabled()) {
				logger.debug("No HandlerExceptionResolvers found in servlet '" + getServletName() + "': using default");
			}
		}
	}
```


可以清洗看到这个方法是将handlerExceptionResolvers进行了初始化，并将自定义的异常处理类（可以多个）写入this.handlerExceptionResolvers








