# Spring Security3源码分析-FilterChainProxy初始化 - z69183787的专栏 - CSDN博客
2014年04月08日 11:59:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1471
个人分类：[安全框架-SpringSecurity](https://blog.csdn.net/z69183787/article/category/2175245)
很久没有更新博客了，最近对Spring Security做了比较深入的研究。 
spring security的教程网上很多： 
[http://lengyun3566.iteye.com/category/153689](http://lengyun3566.iteye.com/category/153689)
[http://wenku.baidu.com/view/b0c0dc0b79563c1ec5da7179.html](http://wenku.baidu.com/view/b0c0dc0b79563c1ec5da7179.html)
以上教程足够应付在实际项目中使用spring security这一安全框架了。如果想深入研究下，网上的资料就很少了，比如： 
[http://www.blogjava.net/SpartaYew/archive/2011/05/19/spingsecurity3.html](http://www.blogjava.net/SpartaYew/archive/2011/05/19/spingsecurity3.html)
[http://www.blogjava.net/youxia/archive/2008/12/07/244883.html](http://www.blogjava.net/youxia/archive/2008/12/07/244883.html)
[http://www.cnblogs.com/hzhuxin/archive/2011/12/19/2293730.html](http://www.cnblogs.com/hzhuxin/archive/2011/12/19/2293730.html)
但还是没有从filter配置开始进行一步一步分析。 
带着很多疑问，逐步拨开spring security3的面纱 
spring security在web.xml中的配置为 
Xml代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- <filter>
- <filter-name>springSecurityFilterChain</filter-name>
- <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>springSecurityFilterChain</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
一看就知道，这是spring的类，这个类位于org.springframework.web-3.0.1.RELEASE.jar这个jar下面，说明这个类本身是和springSecurity无关。DelegatingFilterProxy类继承于抽象类GenericFilterBean,间接地实现了javax.servlet.Filter接口。细节方面就不一一讲述了。看doFilter方法 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- publicvoid doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain)  
- throws ServletException, IOException {  
- 
- // Lazily initialize the delegate if necessary.
-     Filter delegateToUse = null;  
- synchronized (this.delegateMonitor) {  
- if (this.delegate == null) {  
-             WebApplicationContext wac = findWebApplicationContext();  
- if (wac == null) {  
- thrownew IllegalStateException("No WebApplicationContext found: no ContextLoaderListener registered?");  
-             }  
- this.delegate = initDelegate(wac);  
-         }  
-         delegateToUse = this.delegate;  
-     }  
- 
- // Let the delegate perform the actual doFilter operation.
-     invokeDelegate(delegateToUse, request, response, filterChain);  
- }  
这里做了两件事： 
一、initDelegate(wac);//初始化FilterChainProxy 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- protected Filter initDelegate(WebApplicationContext wac) throws ServletException {  
-     Filter delegate = wac.getBean(getTargetBeanName(), Filter.class);  
- if (isTargetFilterLifecycle()) {  
-         delegate.init(getFilterConfig());  
-     }  
- return delegate;  
- }  
getTargetBeanName()返回的是Filter的name:springSecurityFilterChain 
Filter delegate = wac.getBean(getTargetBeanName(), Filter.class); 
这里根据springSecurityFilterChain的bean name直接获取FilterChainProxy的实例。 
这里大家会产生疑问，springSecurityFilterChain这个bean在哪里定义的呢？ 
此时似乎忽略了spring security的bean配置文件了 
Xml代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xsi:schemaLocation="  
-        http://www.springframework.org/schema/beans  
-        http://www.springframework.org/schema/beans/spring-beans.xsd  
-        http://www.springframework.org/schema/security  
-        http://www.springframework.org/schema/security/   
-               spring-security-3.0.xsd">
- <httpauto-config="true">
- <intercept-urlpattern="/*"access="ROLE_USER"/>
- </http>
- <authentication-manageralias="authenticationManager">
- <authentication-provider>
- <user-service>
- <userauthorities="ROLE_USER"name="guest"password="guest"/>
- </user-service>
- </authentication-provider>
- </authentication-manager>
- </beans:beans>
这是最简单的配置了，同时也是解开springSecurityFilterChain这个bean没有定义的疑问了。 
这里主要利用了spring的自定义标签。具体参见： 
[url] http://www.w3school.com.cn/schema/schema_example.asp[/url] 
首先spring security的标签解析部分的源码包为： 
spring-security-config-3.0.2.RELEASE.jar 
这个jar包的META-INF目录下面有spring.handlers,spring.schemas两个文件，其中spring.schemas文件主要是标签的规范、约束；而spring.handlers这个文件时真正解析自定义标签的类，这个文件的内容为： 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- http\://www.springframework.org/schema/security=org.springframework.security.config.SecurityNamespaceHandler
从这里可以看出来spring security的标签解析由org.springframework.security.config.SecurityNamespaceHandler 
来处理。该类实现接口：NamespaceHandler，spring中自定义标签都要实现该接口，这个接口有三个方法init、parse、decorate，其中init用于自定义标签的初始化，parse用于解析标签，decorate用于装饰。 
SecurityNamespaceHandler类的init方法完成了标签解析类的注册工作 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- publicvoid init() {  
- // Parsers
-        parsers.put(Elements.LDAP_PROVIDER, new LdapProviderBeanDefinitionParser());  
-        parsers.put(Elements.LDAP_SERVER, new LdapServerBeanDefinitionParser());  
-        parsers.put(Elements.LDAP_USER_SERVICE, new LdapUserServiceBeanDefinitionParser());  
-        parsers.put(Elements.USER_SERVICE, new UserServiceBeanDefinitionParser());  
-        parsers.put(Elements.JDBC_USER_SERVICE, new JdbcUserServiceBeanDefinitionParser());  
-        parsers.put(Elements.AUTHENTICATION_PROVIDER, new AuthenticationProviderBeanDefinitionParser());  
-        parsers.put(Elements.GLOBAL_METHOD_SECURITY, new GlobalMethodSecurityBeanDefinitionParser());  
-        parsers.put(Elements.AUTHENTICATION_MANAGER, new AuthenticationManagerBeanDefinitionParser());  
- //       registerBeanDefinitionDecorator(Elements.INTERCEPT_METHODS, new InterceptMethodsBeanDefinitionDecorator());
- 
- // Only load the web-namespace parsers if the web classes are available
- if (ClassUtils.isPresent("org.springframework.security.web.FilterChainProxy", getClass().getClassLoader())) {  
-            parsers.put(Elements.HTTP, new HttpSecurityBeanDefinitionParser());  
-            parsers.put(Elements.FILTER_INVOCATION_DEFINITION_SOURCE, new FilterInvocationSecurityMetadataSourceParser());  
-            parsers.put(Elements.FILTER_SECURITY_METADATA_SOURCE, new FilterInvocationSecurityMetadataSourceParser());  
-            filterChainMapBDD = new FilterChainMapBeanDefinitionDecorator();  
- //registerBeanDefinitionDecorator(Elements.FILTER_CHAIN_MAP, new FilterChainMapBeanDefinitionDecorator());
-        }  
-    }  
可以看出，http的标签解析类注册代码为： 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- parsers.put(Elements.HTTP, new HttpSecurityBeanDefinitionParser());  
authentication-manager的标签解析类注册代码为： 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- parsers.put(Elements.AUTHENTICATION_MANAGER, new AuthenticationManagerBeanDefinitionParser());  
HttpSecurityBeanDefinitionParser的parse方法源码为： 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- public BeanDefinition parse(Element element, ParserContext pc) {  
-         CompositeComponentDefinition compositeDef =  
- new CompositeComponentDefinition(element.getTagName(), pc.extractSource(element));  
-         pc.pushContainingComponent(compositeDef);  
- final Object source = pc.extractSource(element);  
- 
- final String portMapperName = createPortMapper(element, pc);  
- final UrlMatcher matcher = createUrlMatcher(element);  
- 
-         HttpConfigurationBuilder httpBldr = new HttpConfigurationBuilder(element, pc, matcher, portMapperName);  
- 
-         httpBldr.parseInterceptUrlsForEmptyFilterChains();  
-         httpBldr.createSecurityContextPersistenceFilter();  
-         httpBldr.createSessionManagementFilters();  
- 
-         ManagedList<BeanReference> authenticationProviders = new ManagedList<BeanReference>();  
-         BeanReference authenticationManager = createAuthenticationManager(element, pc, authenticationProviders, null);  
- 
-         httpBldr.createServletApiFilter();  
-         httpBldr.createChannelProcessingFilter();  
-         httpBldr.createFilterSecurityInterceptor(authenticationManager);  
- 
-         AuthenticationConfigBuilder authBldr = new AuthenticationConfigBuilder(element, pc,  
-                 httpBldr.isAllowSessionCreation(), portMapperName);  
- 
-         authBldr.createAnonymousFilter();  
-         authBldr.createRememberMeFilter(authenticationManager);  
-         authBldr.createRequestCache();  
-         authBldr.createBasicFilter(authenticationManager);  
-         authBldr.createFormLoginFilter(httpBldr.getSessionStrategy(), authenticationManager);  
-         authBldr.createOpenIDLoginFilter(httpBldr.getSessionStrategy(), authenticationManager);  
-         authBldr.createX509Filter(authenticationManager);  
-         authBldr.createLogoutFilter();  
-         authBldr.createLoginPageFilterIfNeeded();  
-         authBldr.createUserServiceInjector();  
-         authBldr.createExceptionTranslationFilter();  
- 
-         List<OrderDecorator> unorderedFilterChain = new ArrayList<OrderDecorator>();  
- 
-         unorderedFilterChain.addAll(httpBldr.getFilters());  
-         unorderedFilterChain.addAll(authBldr.getFilters());  
- 
-         authenticationProviders.addAll(authBldr.getProviders());  
- 
-         BeanDefinition requestCacheAwareFilter = new RootBeanDefinition(RequestCacheAwareFilter.class);  
-         requestCacheAwareFilter.getPropertyValues().addPropertyValue("requestCache", authBldr.getRequestCache());  
-         unorderedFilterChain.add(new OrderDecorator(requestCacheAwareFilter, REQUEST_CACHE_FILTER));  
- 
-         unorderedFilterChain.addAll(buildCustomFilterList(element, pc));  
- 
-         Collections.sort(unorderedFilterChain, new OrderComparator());  
-         checkFilterChainOrder(unorderedFilterChain, pc, source);  
- 
-         List<BeanMetadataElement> filterChain = new ManagedList<BeanMetadataElement>();  
- 
- for (OrderDecorator od : unorderedFilterChain) {  
-             filterChain.add(od.bean);  
-         }  
- 
-         ManagedMap<BeanDefinition, List<BeanMetadataElement>> filterChainMap = httpBldr.getFilterChainMap();  
-         BeanDefinition universalMatch = new RootBeanDefinition(String.class);  
-         universalMatch.getConstructorArgumentValues().addGenericArgumentValue(matcher.getUniversalMatchPattern());  
-         filterChainMap.put(universalMatch, filterChain);  
- 
-         registerFilterChainProxy(pc, filterChainMap, matcher, source);  
- 
-         pc.popAndRegisterContainingComponent();  
- returnnull;  
-     }  
很多spring security的教程都会说http标签配置了auto-config="true"属性，spring security就会自动配置好了过滤器链。但是这些过滤器是如何添加到链中的呢，教程没说。 
但是上面的代码已经告诉我们，就在这里设置的 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- httpBldr.createSecurityContextPersistenceFilter();  
- httpBldr.createSessionManagementFilters();  
- httpBldr.createServletApiFilter();  
- httpBldr.createChannelProcessingFilter();  
- httpBldr.createFilterSecurityInterceptor(authenticationManager);  
- …………  
- authBldr.createAnonymousFilter();  
- authBldr.createRememberMeFilter(authenticationManager);  
- authBldr.createRequestCache();  
- authBldr.createBasicFilter(authenticationManager);  
- authBldr.createFormLoginFilter(httpBldr.getSessionStrategy(), authenticationManager);  
- authBldr.createOpenIDLoginFilter(httpBldr.getSessionStrategy(), authenticationManager);  
- authBldr.createX509Filter(authenticationManager);  
- authBldr.createLogoutFilter();  
- authBldr.createLoginPageFilterIfNeeded();  
- authBldr.createUserServiceInjector();  
- authBldr.createExceptionTranslationFilter();  
具体create分析下一篇再细说。接下来完成Filter的排序、并添加到filterChainMap集合中 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- List<OrderDecorator> unorderedFilterChain = new ArrayList<OrderDecorator>();  
- 
-         unorderedFilterChain.addAll(httpBldr.getFilters());  
-         unorderedFilterChain.addAll(authBldr.getFilters());  
- 
-         authenticationProviders.addAll(authBldr.getProviders());  
- 
-         BeanDefinition requestCacheAwareFilter = new RootBeanDefinition(RequestCacheAwareFilter.class);  
-         requestCacheAwareFilter.getPropertyValues().addPropertyValue("requestCache", authBldr.getRequestCache());  
-         unorderedFilterChain.add(new OrderDecorator(requestCacheAwareFilter, REQUEST_CACHE_FILTER));  
- 
-         unorderedFilterChain.addAll(buildCustomFilterList(element, pc));  
- 
-         Collections.sort(unorderedFilterChain, new OrderComparator());  
-         checkFilterChainOrder(unorderedFilterChain, pc, source);  
- 
-         List<BeanMetadataElement> filterChain = new ManagedList<BeanMetadataElement>();  
- 
- for (OrderDecorator od : unorderedFilterChain) {  
-             filterChain.add(od.bean);  
-         }  
- 
-         ManagedMap<BeanDefinition, List<BeanMetadataElement>> filterChainMap = httpBldr.getFilterChainMap();  
-         BeanDefinition universalMatch = new RootBeanDefinition(String.class);  
-         universalMatch.getConstructorArgumentValues().addGenericArgumentValue(matcher.getUniversalMatchPattern());  
-         filterChainMap.put(universalMatch, filterChain);  
此时，已经为FilterChainProxy提供了必须的参数，接下来就是该完成FilterChainProxy的bean定义过程了 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- registerFilterChainProxy(pc, filterChainMap, matcher, source);  
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- privatevoid registerFilterChainProxy(ParserContext pc, Map<BeanDefinition, List<BeanMetadataElement>> filterChainMap, UrlMatcher matcher, Object source) {  
- if (pc.getRegistry().containsBeanDefinition(BeanIds.FILTER_CHAIN_PROXY)) {  
-         pc.getReaderContext().error("Duplicate <http> element detected", source);  
-     }  
- //定义FilterChainProxy的BeanDefinition构造对象
-      BeanDefinitionBuilder fcpBldr = BeanDefinitionBuilder.rootBeanDefinition(FilterChainProxy.class);  
-     fcpBldr.getRawBeanDefinition().setSource(source);  
-     fcpBldr.addPropertyValue("matcher", matcher);  
-     fcpBldr.addPropertyValue("stripQueryStringFromUrls", Boolean.valueOf(matcher instanceof AntUrlPathMatcher));  
- //注入过滤器链
-     fcpBldr.addPropertyValue("filterChainMap", filterChainMap);  
-     BeanDefinition fcpBean = fcpBldr.getBeanDefinition();  
- //注册bean
-     pc.registerBeanComponent(new BeanComponentDefinition(fcpBean, BeanIds.FILTER_CHAIN_PROXY));  
- //注册bean的alias，其中别名为springSecurityFilterChain      
-     pc.getRegistry().registerAlias(BeanIds.FILTER_CHAIN_PROXY, BeanIds.SPRING_SECURITY_FILTER_CHAIN);  
- }  
这里需要说明的是BeanDefinitionBuilder类，该类能够动态创建spring的bean，并通过ParserContext完成bean的注册，而不需要在xml中进行配置。 
此时FilterChainProxy实例化过程已经完成。 
二、invokeDelegate(delegateToUse, request, response, filterChain); 
//调用代理类的doFilter方法 
Java代码  ![收藏代码](http://dead-knight.iteye.com/images/icon_star.png)
- protectedvoid invokeDelegate(  
-         Filter delegate, ServletRequest request, ServletResponse response, FilterChain filterChain)  
- throws ServletException, IOException {  
-     delegate.doFilter(request, response, filterChain);  
- }  
执行第一步获取的FilterChainProxy实例的doFilter方法。 
通过以上分析，对FilterChainProxy如何产生的，以及Spring Security的标签是如何解析有了大体的认识。 
具体标签的解析、Filter链的执行，下次再更新……
