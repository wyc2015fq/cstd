# spring之DelegatingFilterProxy - z69183787的专栏 - CSDN博客
2017年12月19日 18:33:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：296
个人分类：[Spring-Filter&Listener&Event																[过滤器 & 监听器](https://blog.csdn.net/z69183787/article/category/2275811)](https://blog.csdn.net/z69183787/article/category/7068775)
DelegatingFilterProxy是一个标准servlet Filter的代理，代理实现了Filter接口的spring管理的Bean。支持一个在web.xml的init-param定义的"targetBeanName" filter，在spring applicationContext中指定了target bean的名称。
web.xml通常包含一个DelegatingFilterProxy的定义，它在root springContext中指定了filter-name及对应的bean。在springcontext中实现了标准servlet filter接口的bean在调用时都将被代理。
   注意：Servlet Filter接口默认定义的lifecycle方法不会被代理到target bean，它依赖spring applicationContext管理target bean的lifecycle。在filter的属性targetFilterLifecycle的init-param中指定为true将强制执行Filter.init()和Filter.destory()方法，让Filter来管理filter的lifecycle。
　从spring 3.1开始，DelegatingFilterProxy更新到可以支持servlet 3.0基于实例的filter注册方法来构建，通常和spring 3.1的 org.springframework.web.WebApplicationInitializer spi一起使用。这个类由spring security中Ben Alex 写的FilterToBeanProxy灵感触发而写的。
具体实现如下：
1.初始化方法
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    @Override
    protected void initFilterBean() throws ServletException {
        synchronized (this.delegateMonitor) {
            if (this.delegate == null) {
                // If no target bean name specified, use filter name.
                if (this.targetBeanName == null) {
                    this.targetBeanName = getFilterName();
                }
                // Fetch Spring root application context and initialize the delegate early,
                // if possible. If the root application context will be started after this
                // filter proxy, we'll have to resort to lazy initialization.
                WebApplicationContext wac = findWebApplicationContext();
                if (wac != null) {
                    this.delegate = initDelegate(wac);
                }
            }
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
初始化代理
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    /**
     * Initialize the Filter delegate, defined as bean the given Spring
     * application context.
     * <p>The default implementation fetches the bean from the application context
     * and calls the standard {@code Filter.init} method on it, passing
     * in the FilterConfig of this Filter proxy.
     * @param wac the root application context
     * @return the initialized delegate Filter
     * @throws ServletException if thrown by the Filter
     * @see #getTargetBeanName()
     * @see #isTargetFilterLifecycle()
     * @see #getFilterConfig()
     * @see javax.servlet.Filter#init(javax.servlet.FilterConfig)
     */
    protected Filter initDelegate(WebApplicationContext wac) throws ServletException {
        Filter delegate = wac.getBean(getTargetBeanName(), Filter.class);
        if (isTargetFilterLifecycle()) {
            delegate.init(getFilterConfig());
        }
        return delegate;
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
2. 触发代理方法
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        // Lazily initialize the delegate if necessary.
        Filter delegateToUse = this.delegate;
        if (delegateToUse == null) {
            synchronized (this.delegateMonitor) {
                if (this.delegate == null) {
                    WebApplicationContext wac = findWebApplicationContext();
                    if (wac == null) {
                        throw new IllegalStateException("No WebApplicationContext found: " +
                                "no ContextLoaderListener or DispatcherServlet registered?");
                    }
                    this.delegate = initDelegate(wac);
                }
                delegateToUse = this.delegate;
            }
        }
        // Let the delegate perform the actual doFilter operation.
        invokeDelegate(delegateToUse, request, response, filterChain);
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
触发代理
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    /**
     * Actually invoke the delegate Filter with the given request and response.
     * @param delegate the delegate Filter
     * @param request the current HTTP request
     * @param response the current HTTP response
     * @param filterChain the current FilterChain
     * @throws ServletException if thrown by the Filter
     * @throws IOException if thrown by the Filter
     */
    protected void invokeDelegate(
            Filter delegate, ServletRequest request, ServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {
        delegate.doFilter(request, response, filterChain);
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
3.销毁方法
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    /**
     * Destroy the Filter delegate.
     * Default implementation simply calls {@code Filter.destroy} on it.
     * @param delegate the Filter delegate (never {@code null})
     * @see #isTargetFilterLifecycle()
     * @see javax.servlet.Filter#destroy()
     */
    protected void destroyDelegate(Filter delegate) {
        if (isTargetFilterLifecycle()) {
            delegate.destroy();
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
