# Struts2源码分析 - z69183787的专栏 - CSDN博客
2015年07月18日 21:02:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：842
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
**Struts2架构流程图**
**![](http://www.blogjava.net/images/blogjava_net/myyate/15.PNG)**
**Struts2部分类介绍**
这部分从Struts2参考文档中翻译就可以了。
ActionMapper
        ActionMapper其实是HttpServletRequest和Action调用请求的一个映射，它屏蔽了Action对于Request等java Servlet类的依赖。Struts2中它的默认实现类是DefaultActionMapper，ActionMapper很大的用处可以根据自己的需要来设计url格式，它自己也有Restful的实现，具体可以参考文档的docs\actionmapper.html。
ActionProxy&ActionInvocation
        Action的一个代理，由ActionProxyFactory创建，它本身不包括Action实例，默认实现DefaultActionProxy是由ActionInvocation持有Action实例。ActionProxy作用是如何取得Action，无论是本地还是远程。而ActionInvocation的作用是如何执行Action，拦截器的功能就是在ActionInvocation中实现的。
ConfigurationProvider&Configuration
        ConfigurationProvider就是Struts2中配置文件的解析器，Struts2中的配置文件主要是尤其实现类XmlConfigurationProvider及其子类StrutsXmlConfigurationProvider来解析
**Struts2请求流程**
1、客户端发送请求
2、请求先通过ActionContextCleanUp-->FilterDispatcher
3、FilterDispatcher通过ActionMapper来决定这个Request需要调用哪个Action
4、如果ActionMapper决定调用某个Action，FilterDispatcher把请求的处理交给ActionProxy，这儿已经转到它的Delegate--Dispatcher来执行
5、ActionProxy根据ActionMapping和ConfigurationManager找到需要调用的Action类
6、ActionProxy创建一个ActionInvocation的实例
7、ActionInvocation调用真正的Action，当然这涉及到相关拦截器的调用
8、Action执行完毕，ActionInvocation创建Result并返回，当然，如果要在返回之前做些什么，可以实现PreResultListener。添加PreResultListener可以在Interceptor中实现，不知道其它还有什么方式？
**Struts2(2.3.4)部分代码阅读**
web.xml配置：
自从struts 2.1.3以后，FilterDispatcher已标注为过时改用StrutsPrepareAndExecuteFilter。我们此文将剖析StrutsPrepareAndExecuteFilter，其在工程中作为一个Filter配置在web.xml中，配置如下：
**[html]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- <filter>
- <filter-name>struts2</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
- </filter>
- 
- <filter-mapping>
- <filter-name>struts2</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
从org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter开始
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid init(FilterConfig filterConfig) throws ServletException {  
- //创建一个InitOperations初始化操作的对象
-      InitOperations init = new InitOperations();  
- try {  
- //封装filterConfig，其中有个主要方法getInitParameterNames将参数名字以String格式存储在List中
-          FilterHostConfig config = new FilterHostConfig(filterConfig);  
- // 初始化struts内部日志  
-          init.initLogging(config);  
- //创建dispatcher对象，并读取配置文件   
-          Dispatcher dispatcher = init.initDispatcher(config);  
-          init.initStaticContentLoader(config, dispatcher);  
- 
- //初始化类属性：prepare 、execute  
-          prepare = new PrepareOperations(filterConfig.getServletContext(), dispatcher);  
-          execute = new ExecuteOperations(filterConfig.getServletContext(), dispatcher);  
- this.excludedPatterns = init.buildExcludedPatternsList(dispatcher);  
- 
- //回调空的postInit方法
-          postInit(dispatcher, filterConfig);  
-      } finally {  
-          init.cleanup();  
-      }  
-  }  
首先看下FilterHostConfig ，只有短短的几行代码，getInitParameterNames是这个类的核心，将Filter初始化参数名称有枚举类型转为Iterator。此类的主要作为是对filterConfig 封装，源码如下：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicclass FilterHostConfig implements HostConfig {  
- 
- private FilterConfig config;  
- /*
-      * 构造函数
-      */
- public FilterHostConfig(FilterConfig config) {  
- this.config = config;  
-     }  
- 
- //根据init-param配置的param-name获取param-value的值
- public String getInitParameter(String key) {  
- return config.getInitParameter(key);  
-     }  
- 
- //返回初始化参数名的List
- public Iterator<String> getInitParameterNames() {  
- return MakeIterator.convert(config.getInitParameterNames());  
-     }  
- 
- public ServletContext getServletContext() {  
- return config.getServletContext();  
-     }  
- }  
InitOperations的initDispatcher方法。initDispatcher方法，创建dispatcher对象，并读取配置文件 。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- public Dispatcher initDispatcher(HostConfig filterConfig) {  
- // 创建dispatcher对象，将参数传递dispatcher全局变量
-     Dispatcher dispatcher = createDispatcher(filterConfig);  
- // 初始化配置文件/读取配置文件
-     dispatcher.init();  
- return dispatcher;  
- }  
InitOperations的createDispatcher方法。创建Dispatcher，会读取 filterConfig中的配置信息。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- /*
-      * 创建Dispatcher，会读取 filterConfig
-      * 中的配置信息，将配置信息解析出来，封装成为一个Map，然后根绝servlet上下文和参数Map构造Dispatcher
-      */
- private Dispatcher createDispatcher(HostConfig filterConfig) {  
-         Map<String, String> params = new HashMap<String, String>();  
- // 获得在web.xml中所有的配置文件，将参数放入params Map集合中
- for (Iterator e = filterConfig.getInitParameterNames(); e.hasNext();) {  
-             String name = (String) e.next();  
-             String value = filterConfig.getInitParameter(name);  
-             params.put(name, value);  
-         }  
- // 创建Dispatcher 对象,将ServletContext()，将参数赋给Dispatcher的全局私有变量中
- returnnew Dispatcher(filterConfig.getServletContext(), params);  
-     }  
顺着流程我们看Disptcher的init方法。init方法里就是初始读取一些配置文件等，先看init_DefaultProperties，主要是读取properties配置文件。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid init() {  
- /*
-          * 如果 configurationManager为空，则创建configurationManager对象,
-          * 在configurationManager构函数 将 BeanSelectionProvider.DEFAULT_BEAN_NAME(常量值struts)
-          * 赋给全局变量protected String defaultFrameworkBeanName;
-          */
- if (configurationManager == null) {  
-             configurationManager = createConfigurationManager(BeanSelectionProvider.DEFAULT_BEAN_NAME);  
-         }  
- 
- try {  
- //主要是读取properties配置文件
-             init_DefaultProperties(); // [1]
- //读取struts-default.xml和Struts.xml的方法
-             init_TraditionalXmlConfigurations(); // [2]
-             init_LegacyStrutsProperties(); // [3]
- /* 
-              * init_CustomConfigurationProviders方式初始自定义的Provider，
-              * 配置类全名和实现ConfigurationProvider接口，用逗号隔开即可。
-              */
-             init_CustomConfigurationProviders(); // [5]
- 
- //Filter的初始化参数
-             init_FilterInitParameters() ; // [6]
-             init_AliasStandardObjects() ; // [7]
- 
-             Container container = init_PreloadConfiguration();  
-             container.inject(this);  
-             init_CheckConfigurationReloading(container);  
-             init_CheckWebLogicWorkaround(container);  
- 
- if (!dispatcherListeners.isEmpty()) {  
- for (DispatcherListener l : dispatcherListeners) {  
-                     l.dispatcherInitialized(this);  
-                 }  
-             }  
-         } catch (Exception ex) {  
- if (LOG.isErrorEnabled())  
-                 LOG.error("Dispatcher initialization failed", ex);  
- thrownew StrutsException(ex);  
-         }  
-     }  
init_DefaultProperties方法，初始化default.properties，具体的初始化操作在DefaultPropertiesProvider类中。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- privatevoid init_DefaultProperties() {  
-         configurationManager.addContainerProvider(new DefaultPropertiesProvider());  
-     }  
打开DefaultPropertiesProvider类源码：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid register(ContainerBuilder builder, LocatableProperties props)  
- throws ConfigurationException {  
- 
-         Settings defaultSettings = null;  
- try {  
- // 读取properties属性文件方法
-             defaultSettings = new PropertiesSettings(  
- "org/apache/struts2/default");  
-         } catch (Exception e) {  
- thrownew ConfigurationException(  
- "Could not find or error in org/apache/struts2/default.properties",  
-                     e);  
-         }  
- 
-         loadSettings(props, defaultSettings);  
-     }  
再来看init_TraditionalXmlConfigurations方法，这个是读取struts-default.xml和Struts.xml的方法。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //init_TraditionalXmlConfigurations方法，这个是读取struts-default.xml和Struts.xml的方法
- privatevoid init_TraditionalXmlConfigurations() {  
- /*
-          * 首先读取web.xml中的config初始参数值
-          * 如果没有配置就使用默认的"struts-default.xml,struts-plugin.xml,struts.xml"，
-          * 这儿就可以看出为什么默认的配置文件必须取名为这三个名称了
-          * 如果不想使用默认的名称，直接在web.xml中配置config初始参数即可
-          */
-         String configPaths = initParams.get("config");  
- if (configPaths == null) {  
-             configPaths = DEFAULT_CONFIGURATION_PATHS;  
-         }  
-         String[] files = configPaths.split("\\s*[,]\\s*");  
- for (String file : files) {  
- if (file.endsWith(".xml")) {  
- //依次解析配置文件，xwork.xml单独解析,除xwork.xml外，
- //全都调用createStrutsXmlConfigurationProvider()方法，
- //StrutsXmlConfigurationProvider进行解析
- if ("xwork.xml".equals(file)) {  
-                     configurationManager.addContainerProvider(createXmlConfigurationProvider(file, false));  
-                 } else {  
-                     configurationManager.addContainerProvider(createStrutsXmlConfigurationProvider(file, false, servletContext));  
-                 }  
-             } else {  
- thrownew IllegalArgumentException("Invalid configuration file name");  
-             }  
-         }  
-     }   
       StrutsXmlConfigurationProvider类继承XmlConfigurationProvider，而XmlConfigurationProvider又实现 ConfigurationProvider接口。类XmlConfigurationProvider负责配置文件的读取和解析，addAction()方法负责读取<action>标签，并将数据保存在ActionConfig
 中；addResultTypes()方法负责将<result-type>标签转化为ResultTypeConfig对象；loadInterceptors()方法负责将<interceptor>标签转化为InterceptorConfi对象；loadInterceptorStack()方法负责将<interceptor-ref>标签转化为 InterceptorStackConfig对象；loadInterceptorStacks()方法负责将<interceptor- stack>标签转化成InterceptorStackConfig对象。而上面的方法最终会被addPackage()方法调用，将所读取到的数据汇集到PackageConfig对象中。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- protected PackageConfig addPackage(Element packageElement) throws ConfigurationException {  
-         PackageConfig.Builder newPackage = buildPackageContext(packageElement);  
- 
- if (newPackage.isNeedsRefresh()) {  
- return newPackage.build();  
-         }  
- 
- if (LOG.isDebugEnabled()) {  
-             LOG.debug("Loaded " + newPackage);  
-         }  
- 
- // add result types (and default result) to this package
-         addResultTypes(newPackage, packageElement);  
- 
- // load the interceptors and interceptor stacks for this package
-         loadInterceptors(newPackage, packageElement);  
- 
- // load the default interceptor reference for this package
-         loadDefaultInterceptorRef(newPackage, packageElement);  
- 
- // load the default class ref for this package
-         loadDefaultClassRef(newPackage, packageElement);  
- 
- // load the global result list for this package
-         loadGlobalResults(newPackage, packageElement);  
- 
- // load the global exception handler list for this package
-         loadGobalExceptionMappings(newPackage, packageElement);  
- 
- // get actions
-         NodeList actionList = packageElement.getElementsByTagName("action");  
- 
- for (int i = 0; i < actionList.getLength(); i++) {  
-             Element actionElement = (Element) actionList.item(i);  
-             addAction(actionElement, newPackage);  
-         }  
- 
- // load the default action reference for this package
-         loadDefaultActionRef(newPackage, packageElement);  
- 
-         PackageConfig cfg = newPackage.build();  
-         configuration.addPackageConfig(cfg.getName(), cfg);  
- return cfg;  
-     }  
- 
- private List<Document> loadConfigurationFiles(String fileName, Element includeElement) {  
-         List<Document> docs = new ArrayList<Document>();  
-         List<Document> finalDocs = new ArrayList<Document>();  
- if (!includedFileNames.contains(fileName)) {  
- if (LOG.isDebugEnabled()) {  
-                 LOG.debug("Loading action configurations from: " + fileName);  
-             }  
- 
-             includedFileNames.add(fileName);  
- 
-             Iterator<URL> urls = null;  
-             InputStream is = null;  
- 
-             IOException ioException = null;  
- try {  
-                 urls = getConfigurationUrls(fileName);  
-             } catch (IOException ex) {  
-                 ioException = ex;  
-             }  
- 
- if (urls == null || !urls.hasNext()) {  
- if (errorIfMissing) {  
- thrownew ConfigurationException("Could not open files of the name " + fileName, ioException);  
-                 } else {  
- if (LOG.isInfoEnabled()) {  
-                     LOG.info("Unable to locate configuration files of the name "
-                             + fileName + ", skipping");  
-                     }  
- return docs;  
-                 }  
-             }  
- 
-             URL url = null;  
- while (urls.hasNext()) {  
- try {  
-                     url = urls.next();  
-                     is = fileManager.loadFile(url);  
- 
-                     InputSource in = new InputSource(is);  
- 
-                     in.setSystemId(url.toString());  
- 
-                     docs.add(DomHelper.parse(in, dtdMappings));  
-                 } catch (XWorkException e) {  
- if (includeElement != null) {  
- thrownew ConfigurationException("Unable to load " + url, e, includeElement);  
-                     } else {  
- thrownew ConfigurationException("Unable to load " + url, e);  
-                     }  
-                 } catch (Exception e) {  
- final String s = "Caught exception while loading file " + fileName;  
- thrownew ConfigurationException(s, e, includeElement);  
-                 } finally {  
- if (is != null) {  
- try {  
-                             is.close();  
-                         } catch (IOException e) {  
-                             LOG.error("Unable to close input stream", e);  
-                         }  
-                     }  
-                 }  
-             }  
- 
- //sort the documents, according to the "order" attribute
-             Collections.sort(docs, new Comparator<Document>() {  
- publicint compare(Document doc1, Document doc2) {  
- return XmlHelper.getLoadOrder(doc1).compareTo(XmlHelper.getLoadOrder(doc2));  
-                 }  
-             });  
- 
- for (Document doc : docs) {  
-                 Element rootElement = doc.getDocumentElement();  
-                 NodeList children = rootElement.getChildNodes();  
- int childSize = children.getLength();  
- 
- for (int i = 0; i < childSize; i++) {  
-                     Node childNode = children.item(i);  
- 
- if (childNode instanceof Element) {  
-                         Element child = (Element) childNode;  
- 
- final String nodeName = child.getNodeName();  
- //解析每个action配置是，对于include文件可以使用通配符*来进行配置
- //如Struts.xml中可配置成<include file="actions_*.xml"/>
- if ("include".equals(nodeName)) {  
- //获得file属性 例如： <include file="example.xml"/>
-                             String includeFileName = child.getAttribute("file");  
- if (includeFileName.indexOf('*') != -1) {  
- // handleWildCardIncludes(includeFileName, docs, child);
-                                 ClassPathFinder wildcardFinder = new ClassPathFinder();  
-                                 wildcardFinder.setPattern(includeFileName);  
- 
-                                 Vector<String> wildcardMatches = wildcardFinder.findMatches();  
- for (String match : wildcardMatches) {  
-                                     finalDocs.addAll(loadConfigurationFiles(match, child));  
-                                 }   
-                             } else {  
-                                 finalDocs.addAll(loadConfigurationFiles(includeFileName, child));  
-                             }  
-                         }  
-                     }  
-                 }  
-                 finalDocs.add(doc);  
-                 loadedFileUrls.add(url.toString());  
-             }  
- 
- if (LOG.isDebugEnabled()) {  
-                 LOG.debug("Loaded action configuration from: " + fileName);  
-             }  
-         }  
- return finalDocs;  
-     }  
init_CustomConfigurationProviders方式初始自定义的Provider，配置类全名和实现ConfigurationProvider接口，用逗号隔开即可。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //init_CustomConfigurationProviders方式初始自定义的Provider，配置类全名和实现ConfigurationProvider接口，用逗号隔开即可。
- privatevoid init_CustomConfigurationProviders() {  
- /*
-          * 首先读取web.xml中的configProviders初始参数值
-          * 如果有配置则去加载。
-          */
-         String configProvs = initParams.get("configProviders");  
- if (configProvs != null) {  
-             String[] classes = configProvs.split("\\s*[,]\\s*");  
- for (String cname : classes) {  
- try {  
-                     Class cls = ClassLoaderUtil.loadClass(cname, this.getClass());  
-                     ConfigurationProvider prov = (ConfigurationProvider)cls.newInstance();  
-                     configurationManager.addContainerProvider(prov);  
-                 } catch (InstantiationException e) {  
- thrownew ConfigurationException("Unable to instantiate provider: "+cname, e);  
-                 } catch (IllegalAccessException e) {  
- thrownew ConfigurationException("Unable to access provider: "+cname, e);  
-                 } catch (ClassNotFoundException e) {  
- thrownew ConfigurationException("Unable to locate provider class: "+cname, e);  
-                 }  
-             }  
-         }  
-     }  
现在再回到FilterDispatcher，每次发送一个Request，FilterDispatcher都会调用doFilter方法。doFilter是过滤器的执行方法，它拦截提交的HttpServletRequest请求，HttpServletResponse响应，是strtus2的核心拦截器。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //每次发送一个Request，StrutsPrepareAndExecuteFilter都会调用doFilter方法
- publicvoid doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {  
- 
-         HttpServletRequest request = (HttpServletRequest) req;  
-         HttpServletResponse response = (HttpServletResponse) res;  
- 
- try {  
- //设置编码和国际化  
-             prepare.setEncodingAndLocale(request, response);  
- //ActionContext创建
-             prepare.createActionContext(request, response);  
-             prepare.assignDispatcherToThread();  
- if ( excludedPatterns != null && prepare.isUrlExcluded(request, excludedPatterns)) {  
-                 chain.doFilter(request, response);  
-             } else {  
-                 request = prepare.wrapRequest(request);  
-                 ActionMapping mapping = prepare.findActionMapping(request, response, true);  
- //如果找不到对应的action配置
- if (mapping == null) {  
- /*
-                      * 就是如果path是以“/struts”开头，则到初始参数packages配置的包路径去查找对应的静态资源并输出到页面流中，
-                      * 当然.class文件除外。如果再没有则跳转到404
-                      */
- boolean handled = execute.executeStaticResourceRequest(request, response);  
- if (!handled) {  
-                         chain.doFilter(request, response);  
-                     }  
-                 } else {  
- /*
-                      * 找到对应action配置文件后，调用ExecuteOperations类中executeAction，
-                      * 开始谳用Action的方法。
-                      */
-                     execute.executeAction(request, response, mapping);  
-                 }  
-             }  
-         } finally {  
-             prepare.cleanupRequest(request);  
-         }  
-     }  
setEncodingAndLocale调用了dispatcher方法的prepare方法。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //setEncodingAndLocale调用了dispatcher方法的prepare方法
- publicvoid setEncodingAndLocale(HttpServletRequest request, HttpServletResponse response) {  
-         dispatcher.prepare(request, response);  
-     }  
prepare方法，这个方法很简单只是设置了encoding 、locale ，做的只是一些辅助的工作。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid prepare(HttpServletRequest request, HttpServletResponse response) {  
-         String encoding = null;  
- if (defaultEncoding != null) {  
-             encoding = defaultEncoding;  
-         }  
- // check for Ajax request to use UTF-8 encoding strictly http://www.w3.org/TR/XMLHttpRequest/#the-send-method
- if ("XMLHttpRequest".equals(request.getHeader("X-Requested-With"))) {  
-             encoding = "utf-8";  
-         }  
- 
-         Locale locale = null;  
- if (defaultLocale != null) {  
-             locale = LocalizedTextUtil.localeFromString(defaultLocale, request.getLocale());  
-         }  
- //设置encoding编号为UTF-8
- if (encoding != null) {  
-             applyEncoding(request, encoding);  
-         }  
- 
- if (locale != null) {  
-             response.setLocale(locale);  
-         }  
- 
- if (paramsWorkaroundEnabled) {  
-             request.getParameter("foo"); // simply read any parameter (existing or not) to "prime" the request
-         }  
-     }  
ActionContext是一个容器，这个容易主要存储request、session、application、parameters等相关信息.ActionContext是一个线程的本地变量，这意味着不同的action之间不会共享ActionContext，所以也不用考虑线程安全问题。其实质是一个Map，key是标示request、session、……的字符串，值是其对应的对象
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicclass ActionContext implements Serializable {  
- static ThreadLocal actionContext = new ThreadLocal();  
-         Map<String, Object> context;  
- //省略其它的代码……
-     }  
ActionContext上下文的创建
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //创建ActionContext，初始化thread local 
- public ActionContext createActionContext(HttpServletRequest request, HttpServletResponse response) {  
-         ActionContext ctx;  
-         Integer counter = 1;  
-         Integer oldCounter = (Integer) request.getAttribute(CLEANUP_RECURSION_COUNTER);  
- if (oldCounter != null) {  
-             counter = oldCounter + 1;  
-         }  
- 
- //从ThreadLocal中获取此ActionContext变量
-         ActionContext oldContext = ActionContext.getContext();  
- if (oldContext != null) {  
- // detected existing context, so we are probably in a forward
-             ctx = new ActionContext(new HashMap<String, Object>(oldContext.getContextMap()));  
-         } else {  
-             ValueStack stack = dispatcher.getContainer().getInstance(ValueStackFactory.class).createValueStack();  
-             stack.getContext().putAll(dispatcher.createContextMap(request, response, null, servletContext));  
- //stack.getContext()返回的是一个Map<String，Object>，根据此Map构造一个ActionContext  
-             ctx = new ActionContext(stack.getContext());  
-         }  
-         request.setAttribute(CLEANUP_RECURSION_COUNTER, counter);  
- 
- //将ActionContext保存ThreadLocal  
-         ActionContext.setContext(ctx);  
- return ctx;  
-     }  
上面代码中dispatcher.createContextMap，如何封装相关参数：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- public Map<String,Object> createContextMap(HttpServletRequest request, HttpServletResponse response,  
-             ActionMapping mapping, ServletContext context) {  
- 
- /*
-          * 对request包装requestMap
-          * 对params包装 params
-          * 对session包装 session
-          * 对context包装application
-          * 实际都是Map
-          */
- // request map wrapping the http request objects
-         Map requestMap = new RequestMap(request);  
- 
- // parameters map wrapping the http parameters.  ActionMapping parameters are now handled and applied separately
-         Map params = new HashMap(request.getParameterMap());  
- 
- // session map wrapping the http session
-         Map session = new SessionMap(request);  
- 
- // application map wrapping the ServletContext
-         Map application = new ApplicationMap(context);  
- 
- //requestMap、params、session等Map封装成为一个上下文Map，逐个调用了map.put(Map p). 
-         Map<String,Object> extraContext = createContextMap(requestMap, params, session, application, request, response, context);  
- 
- if (mapping != null) {  
-             extraContext.put(ServletActionContext.ACTION_MAPPING, mapping);  
-         }  
- //返回一个封装对象的Map——extraContext
- return extraContext;  
-     }  
简单看下RequestMap，其他的省略。RequestMap类实现了抽象Map，故其本身是一个Map，主要方法实现：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //map的get实现  
- public Object get(Object key) {  
- return request.getAttribute(key.toString());  
-     }  
- 
- //map的put实现  
- public Object put(Object key, Object value) {  
-         Object oldValue = get(key);  
-         entries = null;  
-         request.setAttribute(key.toString(), value);  
- return oldValue;  
-     }  
Dispatcher类的serviceAction方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- //map的get实现  
- public Object get(Object key) {  
- return request.getAttribute(key.toString());  
-     }  
- 
- //map的put实现  
- public Object put(Object key, Object value) {  
-         Object oldValue = get(key);  
-         entries = null;  
-         request.setAttribute(key.toString(), value);  
- return oldValue;  
-     }  
执行Action，抛出ServletException异常：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid executeAction(HttpServletRequest request, HttpServletResponse response, ActionMapping mapping) throws ServletException {  
-         dispatcher.serviceAction(request, response, servletContext, mapping);  
-     }  
继续查看，dispatcher.serviceAction方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- publicvoid serviceAction(HttpServletRequest request, HttpServletResponse response, ServletContext context,  
-                               ActionMapping mapping) throws ServletException {  
- /*
-          * createContextMap()方法，该方法主要把Application、Session、Request的key value值拷贝到Map中，
-          * 并放在HashMap<String,Object>中，可以参见createContextMap方法：
-          */
-         Map<String, Object> extraContext = createContextMap(request, response, mapping, context);  
- 
- // If there was a previous value stack, then create a new copy and pass it in to be used by the new Action
- //从request范围中通过struts.valueStack获得 stack对象
-         ValueStack stack = (ValueStack) request.getAttribute(ServletActionContext.STRUTS_VALUESTACK_KEY);  
- boolean nullStack = stack == null;  
- if (nullStack) {  
-             ActionContext ctx = ActionContext.getContext();  
- if (ctx != null) {  
-                 stack = ctx.getValueStack();  
-             }  
-         }  
- if (stack != null) {  
-             extraContext.put(ActionContext.VALUE_STACK, valueStackFactory.createValueStack(stack));  
-         }  
- 
-         String timerKey = "Handling request from Dispatcher";  
- try {  
-             UtilTimerStack.push(timerKey);  
- //获得命名空间
-             String namespace = mapping.getNamespace();  
- //获得action配置的名称
-             String name = mapping.getName();  
- //获得action配置的方法，即method属性
-             String method = mapping.getMethod();  
- 
-             Configuration config = configurationManager.getConfiguration();  
- /*
-              * 从容器中获得ActionProxyFactory代理工厂
-              * ActionProxyFactory，它是创建ActionProxy来执行一个特定的命名空间和动作的名称是由调度使用XWork的切入点。
-              * 由ActionProxyFactory创建ActionProxy
-              */
-             ActionProxy proxy = config.getContainer().getInstance(ActionProxyFactory.class).createActionProxy(  
-                     namespace, name, method, extraContext, true, false);  
- 
-             request.setAttribute(ServletActionContext.STRUTS_VALUESTACK_KEY, proxy.getInvocation().getStack());  
- 
- // if the ActionMapping says to go straight to a result, do it!
- //执行execute方法，并转向结果 
- if (mapping.getResult() != null) {  
-                 Result result = mapping.getResult();  
-                 result.execute(proxy.getInvocation());  
-             } else {  
-                 proxy.execute();  
-             }  
- 
- // If there was a previous value stack then set it back onto the request
- if (!nullStack) {  
-                 request.setAttribute(ServletActionContext.STRUTS_VALUESTACK_KEY, stack);  
-             }  
-         } catch (ConfigurationException e) {  
- // WW-2874 Only log error if in devMode
- if(devMode) {  
-                 String reqStr = request.getRequestURI();  
- if (request.getQueryString() != null) {  
-                     reqStr = reqStr + "?" + request.getQueryString();  
-                 }  
-                 LOG.error("Could not find action or result\n" + reqStr, e);  
-             }  
- else {  
- if (LOG.isWarnEnabled()) {  
-                 LOG.warn("Could not find action or result", e);  
-                     }  
-             }  
-             sendError(request, response, context, HttpServletResponse.SC_NOT_FOUND, e);  
-         } catch (Exception e) {  
-             sendError(request, response, context, HttpServletResponse.SC_INTERNAL_SERVER_ERROR, e);  
-         } finally {  
-             UtilTimerStack.pop(timerKey);  
-         }  
-     }  
在上面的源代码中，dispatcher.serviceAction方法里面，调用了createActionProxy方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
-             ActionProxy proxy = config.getContainer().getInstance(ActionProxyFactory.class).createActionProxy(  
-                     namespace, name, method, extraContext, true, false);  
创建ActionPorxy对象，并调用调用proxy的prepare方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- public ActionProxy createActionProxy(String namespace, String actionName, String methodName, Map<String, Object> extraContext, boolean executeResult, boolean cleanupContext) {  
-         ActionInvocation inv = new DefaultActionInvocation(extraContext, true);  
-         container.inject(inv);  
- return createActionProxy(inv, namespace, actionName, methodName, executeResult, cleanupContext);  
-     }  
- 
- public ActionProxy createActionProxy(ActionInvocation inv, String namespace, String actionName, String methodName, boolean executeResult, boolean cleanupContext) {  
-         DefaultActionProxy proxy = new DefaultActionProxy(inv, namespace, actionName, methodName, executeResult, cleanupContext);  
-         container.inject(proxy);  
- //调用proxy的prepare()方法
-         proxy.prepare();  
- return proxy;  
-     }  
- 
- protectedvoid prepare() {  
-         String profileKey = "create DefaultActionProxy: ";  
- try {  
-             UtilTimerStack.push(profileKey);  
-             config = configuration.getRuntimeConfiguration().getActionConfig(namespace, actionName);  
- 
- if (config == null && unknownHandlerManager.hasUnknownHandlers()) {  
-                 config = unknownHandlerManager.handleUnknownAction(namespace, actionName);  
-             }  
- if (config == null) {  
- thrownew ConfigurationException(getErrorMessage());  
-             }  
- 
-             resolveMethod();  
- 
- if (!config.isAllowedMethod(method)) {  
- thrownew ConfigurationException("Invalid method: " + method + " for action " + actionName);  
-             }  
- //invocation调用初始化的方法
-             invocation.init(this);  
- 
-         } finally {  
-             UtilTimerStack.pop(profileKey);  
-         }  
-     }  
       后面才是最主要的--ActionProxy，ActionInvocation。ActionProxy是Action的一个代理类，也就是说Action的调用是通过ActionProxy实现的，其实就是调用了ActionProxy.execute()方法，而该方法又调用了ActionInvocation.invoke()方法。归根到底，最后调用的是DefaultActionInvocation.invokeAction()方法。先看DefaultActionInvocation的init方法。
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- /*
-      * (non-Javadoc)
-      * @see com.opensymphony.xwork2.ActionInvocation#init(com.opensymphony.xwork2.ActionProxy)
-      */
- publicvoid init(ActionProxy proxy) {  
- this.proxy = proxy;  
-         Map<String, Object> contextMap = createContextMap();  
- 
- // Setting this so that other classes, like object factories, can use the ActionProxy and other
- // contextual information to operate
-         ActionContext actionContext = ActionContext.getContext();  
- 
- if (actionContext != null) {  
-             actionContext.setActionInvocation(this);  
-         }  
- //创建Action，可Struts2里是每次请求都新建一个Action
-         createAction(contextMap);  
- 
- if (pushAction) {  
-             stack.push(action);  
- //将创建的action放置到的
-             contextMap.put("action", action);  
-         }  
- //将contextMap进行封装
-         invocationContext = new ActionContext(contextMap);  
-         invocationContext.setName(proxy.getActionName());  
- 
- // get a new List so we don't get problems with the iterator if someone changes the list
-         List<InterceptorMapping> interceptorList = new ArrayList<InterceptorMapping>(proxy.getConfig().getInterceptors());  
-         interceptors = interceptorList.iterator();  
-     }  
- 
- protectedvoid createAction(Map<String, Object> contextMap) {  
- // load action
-         String timerKey = "actionCreate: " + proxy.getActionName();  
- try {  
-             UtilTimerStack.push(timerKey);   
- /*
-              * 默认建立Action是StrutsObjectFactory，
-              * 实际中可以是使用Spring创建的Action，这个时候使用的是SpringObjectFactory
-              */
-             action = objectFactory.buildAction(proxy.getActionName(), proxy.getNamespace(), proxy.getConfig(), contextMap);  
-         } catch (InstantiationException e) {  
- thrownew XWorkException("Unable to intantiate Action!", e, proxy.getConfig());  
-         } catch (IllegalAccessException e) {  
- thrownew XWorkException("Illegal access to constructor, is it public?", e, proxy.getConfig());  
-         } catch (Exception e) {  
-             String gripe = "";  
- 
- if (proxy == null) {  
-                 gripe = "Whoa!  No ActionProxy instance found in current ActionInvocation.  This is bad ... very bad";  
-             } elseif (proxy.getConfig() == null) {  
-                 gripe = "Sheesh.  Where'd that ActionProxy get to?  I can't find it in the current ActionInvocation!?";  
-             } elseif (proxy.getConfig().getClassName() == null) {  
-                 gripe = "No Action defined for '" + proxy.getActionName() + "' in namespace '" + proxy.getNamespace() + "'";  
-             } else {  
-                 gripe = "Unable to instantiate Action, " + proxy.getConfig().getClassName() + ",  defined for '" + proxy.getActionName() + "' in namespace '" + proxy.getNamespace() + "'";  
-             }  
- 
-             gripe += (((" -- " + e.getMessage()) != null) ? e.getMessage() : " [no message in exception]");  
- thrownew XWorkException(gripe, e, proxy.getConfig());  
-         } finally {  
-             UtilTimerStack.pop(timerKey);  
-         }  
- 
- if (actionEventListener != null) {  
-             action = actionEventListener.prepare(action, stack);  
-         }  
-     }  
接下来看看DefaultActionInvocation 的invoke方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- /*
-      * invoke()方法中的if(interceptors.hasNext())语句，
-      * 当然，interceptors里存储的是interceptorMapping列表（它包括一个Interceptor和一个name），
-      * 所有的截拦器必须实现Interceptor的intercept()方法，
-      * 而该方法的参数恰恰又是ActionInvocation，在intercept方法中还是调用invocation.invoke()，
-      * 从而实现了一个Interceptor链的调用。当所有的Interceptor执行完，
-      * 最后调用invokeActionOnly方法来执行Action相应的方法。
-      */
- public String invoke() throws Exception {  
-         String profileKey = "invoke: ";  
- try {  
-             UtilTimerStack.push(profileKey);  
- 
- if (executed) {  
- thrownew IllegalStateException("Action has already executed");  
-             }  
- // 判断interceptors是否有拦截器
- if (interceptors.hasNext()) {  
- final InterceptorMapping interceptor = (InterceptorMapping) interceptors.next();  
-                 String interceptorMsg = "interceptor: " + interceptor.getName();  
-                 UtilTimerStack.push(interceptorMsg);  
- try {  
- //执行拦截器，返回一个字符串返回代码
-                     resultCode = interceptor.getInterceptor().intercept(DefaultActionInvocation.this);  
-                 }finally {  
-                     UtilTimerStack.pop(interceptorMsg);  
-                 }  
-             } else {  
- //interceptor执行完了之后执行action
-                 resultCode = invokeActionOnly();  
-             }  
- 
- // this is needed because the result will be executed, then control will return to the Interceptor, which will
- // return above and flow through again
- if (!executed) {  
- //在Result返回之前调用preResultListeners
- if (preResultListeners != null) {  
- for (Object preResultListener : preResultListeners) {  
-                         PreResultListener listener = (PreResultListener) preResultListener;  
- 
-                         String _profileKey = "preResultListener: ";  
- try {  
-                             UtilTimerStack.push(_profileKey);  
-                             listener.beforeResult(this, resultCode);  
-                         }  
- finally {  
-                             UtilTimerStack.pop(_profileKey);  
-                         }  
-                     }  
-                 }  
- 
- // now execute the result, if we're supposed to
- if (proxy.getExecuteResult()) {  
- /*
-                      * action执行完了，还要根据ResultConfig返回到view，
-                      * 也就是在invoke方法中调用executeResult方法
-                      */
-                     executeResult();  
-                 }  
- 
-                 executed = true;  
-             }  
- //返回代理执行Action后的回的字符串
- return resultCode;  
-         }  
- finally {  
-             UtilTimerStack.pop(profileKey);  
-         }  
-     }  
调用invokeActionOnly方法来执行Action相应的方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- public String invokeActionOnly() throws Exception {  
- return invokeAction(getAction(), proxy.getConfig());  
-     }  
- 
- protected String invokeAction(Object action, ActionConfig actionConfig) throws Exception {  
- //通过代理proxy.获得方法名称
-         String methodName = proxy.getMethod();  
- 
- if (LOG.isDebugEnabled()) {  
-             LOG.debug("Executing action method = " + actionConfig.getMethodName());  
-         }  
- 
-         String timerKey = "invokeAction: " + proxy.getActionName();  
- try {  
-             UtilTimerStack.push(timerKey);  
- 
- boolean methodCalled = false;  
-             Object methodResult = null;  
-             Method method = null;  
- try {  
- //java反射机制得到要执行的方法
-                 method = getAction().getClass().getMethod(methodName, EMPTY_CLASS_ARRAY);  
-             } catch (NoSuchMethodException e) {  
- // hmm -- OK, try doXxx instead
- try {  
- //如果没有对应的方法，则使用do+Xxxx来再次获得方法  
-                     String altMethodName = "do" + methodName.substring(0, 1).toUpperCase() + methodName.substring(1);  
-                     method = getAction().getClass().getMethod(altMethodName, EMPTY_CLASS_ARRAY);  
-                 } catch (NoSuchMethodException e1) {  
- // well, give the unknown handler a shot
- //当未知的action、result或者方法被执行的时候，通过框架被调用。
- if (unknownHandlerManager.hasUnknownHandlers()) {  
- try {  
-                             methodResult = unknownHandlerManager.handleUnknownMethod(action, methodName);  
-                             methodCalled = true;  
-                         } catch (NoSuchMethodException e2) {  
- // throw the original one
- throw e;  
-                         }  
-                     } else {  
- throw e;  
-                     }  
-                 }  
-             }  
- //执行Method
- if (!methodCalled) {  
-                 methodResult = method.invoke(action, EMPTY_OBJECT_ARRAY);  
-             }  
- 
- return saveResult(actionConfig, methodResult);  
-         } catch (NoSuchMethodException e) {  
- //无法找到某一特定方法时，抛出该(NoSuchMethodException)异常
- thrownew IllegalArgumentException("The " + methodName + "() is not defined in action " + getAction().getClass() + "");  
-         } catch (InvocationTargetException e) {  
- // We try to return the source exception.
-             Throwable t = e.getTargetException();  
- 
- if (actionEventListener != null) {  
-                 String result = actionEventListener.handleException(t, getStack());  
- if (result != null) {  
- return result;  
-                 }  
-             }  
- if (t instanceof Exception) {  
- throw (Exception) t;  
-             } else {  
- throw e;  
-             }  
-         } finally {  
-             UtilTimerStack.pop(timerKey);  
-         }  
-     }  
action执行完了，还要根据ResultConfig返回到view，也就是在invoke方法中调用executeResult方法：
**[java]**[view
 plain](http://blog.csdn.net/yuan_xw/article/details/7838123#)[copy](http://blog.csdn.net/yuan_xw/article/details/7838123#)
- privatevoid executeResult() throws Exception {  
- //根据ResultConfig创建Result   
-         result = createResult();  
- 
-         String timerKey = "executeResult: " + getResultCode();  
- try {  
-             UtilTimerStack.push(timerKey);  
- if (result != null) {  
- /*
-                  * 开始执行Result,
-                  * 可以参考Result的实现，如用了比较多的ServletDispatcherResult,
-                  * ServletActionRedirectResult,ServletRedirectResult
-                  */
-                 result.execute(this);  
-             } elseif (resultCode != null && !Action.NONE.equals(resultCode)) {  
- thrownew ConfigurationException("No result defined for action " + getAction().getClass().getName()  
-                         + " and result " + getResultCode(), proxy.getConfig());  
-             } else {  
- if (LOG.isDebugEnabled()) {  
-                     LOG.debug("No result returned for action " + getAction().getClass().getName() + " at " + proxy.getConfig()  
- 　　　　　　　　　　    .getLocation());  
-                 }  
-             }  
-         } finally {  
-             UtilTimerStack.pop(timerKey);  
-         }  
-     }  
- 
- public Result createResult() throws Exception {  
- //如果Action中直接返回的Result类型,在invokeAction()保存在explicitResult   
- if (explicitResult != null) {  
-             Result ret = explicitResult;  
-             explicitResult = null;  
- 
- return ret;  
-         }  
- //根据result名称获得ResultConfig，resultCode就是result的name
-         ActionConfig config = proxy.getConfig();  
-         Map<String, ResultConfig> results = config.getResults();  
- 
-         ResultConfig resultConfig = null;  
- 
- try {  
- //通过返回的String来匹配resultConfig 
-             resultConfig = results.get(resultCode);  
-         } catch (NullPointerException e) {  
- // swallow
-         }  
- 
- if (resultConfig == null) {  
- // If no result is found for the given resultCode, try to get a wildcard '*' match.
- //如果找不到对应name的ResultConfig，则使用name为通配符*的Result     
- //说明可以用*通配所有的Result
-             resultConfig = results.get("*");  
-         }  
- 
- if (resultConfig != null) {  
- try {  
- //构造result 
- return objectFactory.buildResult(resultConfig, invocationContext.getContextMap());  
-             } catch (Exception e) {  
-                 LOG.error("There was an exception while instantiating the result of type " + resultConfig.getClassName(), e);  
- thrownew XWorkException(e, resultConfig);  
-             }  
-         } elseif (resultCode != null && !Action.NONE.equals(resultCode) && unknownHandlerManager.hasUnknownHandlers()) {  
- return unknownHandlerManager.handleUnknownResult(invocationContext, proxy.getActionName(), proxy.getConfig(), resultCode);  
-         }  
- returnnull;  
-     }  
- 
- public Result buildResult(ResultConfig resultConfig, Map<String, Object> extraContext) throws Exception {  
- 
-         String resultClassName = resultConfig.getClassName();  
-         Result result = null;  
- if (resultClassName != null) {  
- /*
-              * buildBean中会用反射机制Class.newInstance来创建bean,
-              * 因为Result是有状态的，所以每次请求都新建一个
-              */
-             result = (Result) buildBean(resultClassName, extraContext);  
-             Map<String, String> params = resultConfig.getParams();  
- if (params != null) {  
- for (Map.Entry<String, String> paramEntry : params.entrySet()) {  
- try {  
- /*
-                          * reflectionProvider参见OgnlReflectionProvider
-                          * resultConfig.getParams()就是result配置文件里所配置的参数<param></param>
-                          * setProperties方法最终调用的是Ognl类的setValue方法
-                          * 这句其实就是把param名值设置到根对象result上
-                          */
-                         reflectionProvider.setProperty(paramEntry.getKey(), paramEntry.getValue(), result, extraContext, true);  
-                     } catch (ReflectionException ex) {  
- if (result instanceof ReflectionExceptionHandler) {  
-                             ((ReflectionExceptionHandler) result).handle(ex);  
-                         }  
-                     }  
-                 }  
-             }  
-         }  
- 
- return result;  
-     }  
--以上就是struts2一个的请求流程基本上就结束了。工作中不涉及struts2，最近不很忙，稍微看了下struts2的文档，写了个demo，从源码的角度研究了下运行原理，如有分析不当请指出，我后续逐步完善更正，大家共同提高。
