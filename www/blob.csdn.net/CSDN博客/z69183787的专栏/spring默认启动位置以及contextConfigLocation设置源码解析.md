# spring默认启动位置以及contextConfigLocation设置源码解析 - z69183787的专栏 - CSDN博客
2012年11月25日 13:17:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5764
这几天在看spring的源码，涉及到spring启动位置的部分，下面就看看spring到底是从哪儿开始加载的。本文使用的是spring3.0M3 
首先spring的加载会借助一个监听器ContextLoaderListener，直接上web.xml文件 
Xml代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
我们通常会对加载位置统一管理   
Xml代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-         /WEB-INF/conf/spring/**/*.xml  
- </param-value>
- </context-param>
这个org.springframework.web.context.ContextLoaderListener类型是springframework中的原始加载上下文的监听器， 
通常我们会自定义一个Listener去继承ContextLoaderListener并另外实现我们需要初始化的接口(通常我们会选择实现一些接口来对session的管理) 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- publicclass FrameServletContextListener extends ContextLoaderListener implements ServletContextListener,HttpSessionAttributeListener,HttpSessionListener {  
- //
- private ServletContext initPath(ServletContextEvent event) {  
- 
-     }  
- 
- publicsynchronizedvoid contextDestroyed(ServletContextEvent event) {  
- //
-     }  
- 
-     ...  
- }  
当监听器设置好了之后 ，启动web容器 监听器开始启动ContextLoaderListenerl 
类中的方法contextInitialized() 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- /**
-  * Initialize the root web application context.
-  */
- publicvoid contextInitialized(ServletContextEvent event) {  
- this.contextLoader = createContextLoader();  
- if (this.contextLoader == null) {  
- this.contextLoader = this;  
-     }  
- this.contextLoader.initWebApplicationContext(event.getServletContext());  
- }  
这样this.contextLoader.initWebApplicationContext(event.getServletContext());ContextLoaderListener 
就会借助容器的上下文去初始一个spring的应用上下文，使用到了ContextLoader这个类 
在ContextLoader初始化时我们看到这样一块static代码 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- static {  
- // Load default strategy implementations from properties file.
- // This is currently strictly internal and not meant to be customized
- // by application developers.
- try {  
- //这一句会去加载同在此包下的一个properties文件的值（ContextLoader.properties）
-         ClassPathResource resource = new ClassPathResource(DEFAULT_STRATEGIES_PATH, ContextLoader.class);  
-         defaultStrategies = PropertiesLoaderUtils.loadProperties(resource);  
-     }  
- catch (IOException ex) {  
- thrownew IllegalStateException("Could not load 'ContextLoader.properties': " + ex.getMessage());  
-     }  
- }  
属性文件中这样定义 
引用
org.springframework.web.context.WebApplicationContext=org.springframework.web.context.support.XmlWebApplicationContext
这样我们就能根据属性文件中的定义反射出一个XmlWebApplicationContext上下文了 
然而我们在XmlWebApplicationContext中看到如下变量 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- /** Default config location for the root context */
- publicstaticfinal String DEFAULT_CONFIG_LOCATION = "/WEB-INF/applicationContext.xml";  
至此我们已经知道默认加载spring文件的启动位置了 
当我们再看ContextLoader类，我们就会看到传说中的参数contextConfigLocation 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- publicstaticfinal String CONFIG_LOCATION_PARAM = "contextConfigLocation";  
而XmlWebApplicationContext对象正是调用了这个参数去设置启动位置 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- wac.setConfigLocation(servletContext.getInitParameter(CONFIG_LOCATION_PARAM));  
再往上看XmlWebApplicationContext继承的AbstractRefreshableConfigApplicationContext类中的setConfigLocation方法将此抽象类中的String[] configLocations值填充 
并在AbstractRefreshableConfigApplicationContext类中我们看到spring对默认启动文件位置和配置启动文件位置的支持 
Java代码  ![收藏代码](http://yuchengyang.iteye.com/images/icon_star.png)
- protected String[] getConfigLocations() {  
- return (this.configLocations != null ? this.configLocations : getDefaultConfigLocations());  
} 
至此我们已经清楚spring将从哪儿加载并知道加载哪些文件了。
