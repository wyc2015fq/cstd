# spring源码分析之——spring 事务管理实现方式 - 零度的博客专栏 - CSDN博客
2016年05月04日 13:43:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：3471
# 注意：这里只是分析spring事务的实现方式。也就是spring的事务管理是怎么发生作用的，而不分析具体的实现细节(细节将在下一篇里面分析).
 转载:http://michael-softtech.iteye.com/blog/813835
紧接着上一篇提到的，Spring是通过NameSpaceHandler来解析配置文件中的标签的。下面就已事务的配置为例，讲述一下
事务配置的标签的解析过程，从来理解事物是如何通过aop产生作用的。
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- <!-- 以AspectJ方式 定义 AOP -->  
- <aop:config proxy-target-class="true">  
-     <aop:advisor pointcut="execution(* commo.base.BaseManager.*(..))" advice-ref="txAdvice"/>  
-     <aop:advisor pointcut="execution(* com.*..*.service.*Manager.*(..))" advice-ref="txAdvice"/>  
- </aop:config>  
- 
- <!-- 基本事务定义,使用transactionManager作事务管理,默认get* find*方法的事务为readonly,其余方法按默认设置.  
-          默认的设置请参考Spring文档事务一章. -->  
- <tx:advice id="txAdvice" transaction-manager="transactionManager">  
-     <tx:attributes>  
-         <tx:method name="find*" read-only="true"/>  
-         <tx:method name="get*" read-only="true"/>  
-         <tx:method name="query*" read-only="true"/>  
-         <tx:method name="*" read-only="false"/>  
-     </tx:attributes>  
- </tx:advice>  
 以上的配置相信很多人已经很熟悉了，在此不赘述。而是具体分析一下原理。
 先来分析<tx:advice>...</tx:advice>。
tx是TransactionNameSpace。对应的是handler是TxNamespaceHandler.
这个类一个init方法：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- publicvoid init() {  
-     registerBeanDefinitionParser("advice", new TxAdviceBeanDefinitionParser());  
-     registerBeanDefinitionParser("annotation-driven", new AnnotationDrivenBeanDefinitionParser());  
-     registerBeanDefinitionParser("jta-transaction-manager", new JtaTransactionManagerBeanDefinitionParser());  
- }  
这个方法是在DefaultNamespaceHandlerResolver的resolve中调用的。在为对应的标签寻找namespacehandler的时候，调用这个resolve方法。resolve方法先寻找namespaceUri对应的namespacehandler,如果找到了就先调用Init方法。
    OK.我们的<tx:advice>对应的解析器也注册了，那就是上面代码里面的
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- new TxAdviceBeanDefinitionParser()  
    那么，这个解析器是什么时候调用的哪？
    上一篇提到了，对应标签解析时会先选择namespacehandler,然后调用其parse方法。
    TxNamespaceHandler的parse方法在其父类NamespaceHandlerSupport中，代码如下:
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- public BeanDefinition parse(Element element, ParserContext parserContext) {  
- return findParserForElement(element, parserContext).parse(element, parserContext);  
-     }  
   这下明白了吧？<tx:advice>在解析出来的Document里面是一个Element,而这个Element的parse就是上面注册了的
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- TxAdviceBeanDefinitionParser  
  现在这个parser的parse方法在NamespaceHandlerSupport的parse方法中被调用了，下面我们来看看这个
  TxAdviceBeanDefinitionParser的parse方法吧，这个方法在TxAdviceBeanDefinitionParser的祖父类AbstractBeanDefinitionParser中：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- publicfinal BeanDefinition parse(Element element, ParserContext parserContext) {  
-         AbstractBeanDefinition definition = parseInternal(element, parserContext);  
- if (definition != null && !parserContext.isNested()) {  
- try {  
-                 String id = resolveId(element, definition, parserContext);  
- if (!StringUtils.hasText(id)) {  
-                     parserContext.getReaderContext().error(  
- "Id is required for element '" + parserContext.getDelegate().getLocalName(element)  
-                                     + "' when used as a top-level tag", element);  
-                 }  
-                 String[] aliases = new String[0];  
-                 String name = element.getAttribute(NAME_ATTRIBUTE);  
- if (StringUtils.hasLength(name)) {  
-                     aliases = StringUtils.trimArrayElements(StringUtils.commaDelimitedListToStringArray(name));  
-                 }  
-                 BeanDefinitionHolder holder = new BeanDefinitionHolder(definition, id, aliases);  
-                 registerBeanDefinition(holder, parserContext.getRegistry());  
- if (shouldFireEvents()) {  
-                     BeanComponentDefinition componentDefinition = new BeanComponentDefinition(holder);  
-                     postProcessComponentDefinition(componentDefinition);  
-                     parserContext.registerComponent(componentDefinition);  
-                 }  
-             }  
- catch (BeanDefinitionStoreException ex) {  
-                 parserContext.getReaderContext().error(ex.getMessage(), element);  
- returnnull;  
-             }  
-         }  
- return definition;  
-     }  
   注意其中这样一行：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- AbstractBeanDefinition definition = parseInternal(element, parserContext);  
    这个parseInternal是在TxAdviceBeanDefinitionParser的父类AbstractSingleBeanDefinitionParser中实现的，代码如下：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- @Override
- protectedfinal AbstractBeanDefinition parseInternal(Element element, ParserContext parserContext) {  
-         BeanDefinitionBuilder builder = BeanDefinitionBuilder.genericBeanDefinition();  
-         String parentName = getParentName(element);  
- if (parentName != null) {  
-             builder.getRawBeanDefinition().setParentName(parentName);  
-         }  
-         Class beanClass = getBeanClass(element);  
- if (beanClass != null) {  
-             builder.getRawBeanDefinition().setBeanClass(beanClass);  
-         }  
- else {  
-             String beanClassName = getBeanClassName(element);  
- if (beanClassName != null) {  
-                 builder.getRawBeanDefinition().setBeanClassName(beanClassName);  
-             }  
-         }  
-         builder.getRawBeanDefinition().setSource(parserContext.extractSource(element));  
- if (parserContext.isNested()) {  
- // Inner bean definition must receive same scope as containing bean.
-             builder.setScope(parserContext.getContainingBeanDefinition().getScope());  
-         }  
- if (parserContext.isDefaultLazyInit()) {  
- // Default-lazy-init applies to custom bean definitions as well.
-             builder.setLazyInit(true);  
-         }  
-         doParse(element, parserContext, builder);  
- return builder.getBeanDefinition();  
-     }  
    其中有一行：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- Class beanClass = getBeanClass(element);  
   getBeanClass是在TxAdviceBeanDefinitionParser中实现的，很简单:
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- @Override
- protected Class getBeanClass(Element element) {  
- return TransactionInterceptor.class;  
-     }  
  至此，这个标签解析的流程已经基本清晰了。那就是：解析除了一个以TransactionInerceptor为classname的beandefinition并且注册这个bean。剩下来要看的，就是这个TranscationInterceptor到底是什么？
 看看这个类的接口定义，就明白了：
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- publicclass TransactionInterceptor extends TransactionAspectSupport implements MethodInterceptor, Serializable  
  这根本就是一个spring AOP的advice嘛！现在明白为什么事务的配置能通过aop产生作用了吧？
  下面具体看看这个advice的advice:
Java代码  ![收藏代码](http://image76.360doc.com/DownloadImg/2014/07/3014/43858002_1.png)
- public Object invoke(final MethodInvocation invocation) throws Throwable {  
- // Work out the target class: may be <code>null</code>.
- // The TransactionAttributeSource should be passed the target class
- // as well as the method, which may be from an interface.
-         Class targetClass = (invocation.getThis() != null ? invocation.getThis().getClass() : null);  
- 
- // If the transaction attribute is null, the method is non-transactional.
- final TransactionAttribute txAttr =  
-                 getTransactionAttributeSource().getTransactionAttribute(invocation.getMethod(), targetClass);  
- final PlatformTransactionManager tm = determineTransactionManager(txAttr);  
- final String joinpointIdentification = methodIdentification(invocation.getMethod());  
- 
- if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {  
- // Standard transaction demarcation with getTransaction and commit/rollback calls.
-             TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);  
-             Object retVal = null;  
- try {  
- // This is an around advice: Invoke the next interceptor in the chain.
- // This will normally result in a target object being invoked.
-                 retVal = invocation.proceed();  
-             }  
- catch (Throwable ex) {  
- // target invocation exception
-                 completeTransactionAfterThrowing(txInfo, ex);  
- throw ex;  
-             }  
- finally {  
-                 cleanupTransactionInfo(txInfo);  
-             }  
-             commitTransactionAfterReturning(txInfo);  
- return retVal;  
-         }  
- 
- else {  
- // It's a CallbackPreferringPlatformTransactionManager: pass a TransactionCallback in.
- try {  
-                 Object result = ((CallbackPreferringPlatformTransactionManager) tm).execute(txAttr,  
- new TransactionCallback<Object>() {  
- public Object doInTransaction(TransactionStatus status) {  
-                                 TransactionInfo txInfo = prepareTransactionInfo(tm, txAttr, joinpointIdentification, status);  
- try {  
- return invocation.proceed();  
-                                 }  
- catch (Throwable ex) {  
- if (txAttr.rollbackOn(ex)) {  
- // A RuntimeException: will lead to a rollback.
- if (ex instanceof RuntimeException) {  
- throw (RuntimeException) ex;  
-                                         }  
- else {  
- thrownew ThrowableHolderException(ex);  
-                                         }  
-                                     }  
- else {  
- // A normal return value: will lead to a commit.
- returnnew ThrowableHolder(ex);  
-                                     }  
-                                 }  
- finally {  
-                                     cleanupTransactionInfo(txInfo);  
-                                 }  
-                             }  
-                         });  
- 
- // Check result: It might indicate a Throwable to rethrow.
- if (result instanceof ThrowableHolder) {  
- throw ((ThrowableHolder) result).getThrowable();  
-                 }  
- else {  
- return result;  
-                 }  
-             }  
- catch (ThrowableHolderException ex) {  
- throw ex.getCause();  
-             }  
-         }  
-     }  
   哦，原来就是在这里控制了method invocation(spring aop是基于method的)!根据我们的配置，来决定
  是不是对method使用事务。
   至此，spring的事务管理已经基本清晰了。就是解析出一个advice bean(of class : TransactionInterceptor)来，
   然后在aop中配置pointcut和这个advice,就能产生作用了！
