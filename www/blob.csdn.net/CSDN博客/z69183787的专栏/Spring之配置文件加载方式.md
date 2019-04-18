# Spring之配置文件加载方式 - z69183787的专栏 - CSDN博客
2012年11月24日 20:02:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：809
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)

Spring在org.springframework.core.io包中提供了多种配置文件加载方式。无论是XML、URL还是文件，都有很好的支持。比如基于URL的UrlResource、基于输入流的InputStreamResource、基于文件系统的FileSystemResource、基于应用classpath的ClassPathResource。开发者可以从不同位置、以不同方式加载Spring配置文件。
常用的集中配置文件加载方式：
一:XmlBeanFactory 加载方式。此方式常用于应用程序，如swt、applet等。
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136599#)[copy](http://blog.csdn.net/weoln/article/details/4136599#)
- //基于资源文件加载：
- Resource resource = new ClassPathResource("appcontext.xml");  
- BeanFactory factory = new XmlBeanFactory(resource);  
- 
- //基于流文件加载：
-     InputStream in = new FileInputStream("appcontext.xml");  
- BeanFactory factory = new XmlBeanFactory(in);  
 二:ApplicationContext加载方式。此方式常用于J2EE程序。
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136599#)[copy](http://blog.csdn.net/weoln/article/details/4136599#)
- ApplicationContext context=new classPathXmlApplicationContext("classpath:appcontext.xml");  
-  BeanFactory factory = (BeanFactory)context;  
- 
- // src目录下的
-  ApplicationContext context=new ClassPathXmlApplicationContext("appcontext.xml");      
-  BeanFactory factory = (BeanFactory)context;  
- 
- // src/conf 目录下的
-  ApplicationContext context=new ClassPathXmlApplicationContext("conf/appcontext.xml");     
-  BeanFactory factory = (BeanFactory)context;  
- //绝对路径
-  ApplicationContext context=new ClassPathXmlApplicationContext("file:G:/Test/src/appcontext.xml");  
-  BeanFactory factory = (BeanFactory)context;  
    三 : 文件系统的加载方式
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136599#)[copy](http://blog.csdn.net/weoln/article/details/4136599#)
- ApplicationContext factory=new FileSystemXmlApplicationContext("src/appcontext.xml");  
- //使用了  classpath:  前缀,作为标志,  这样,FileSystemXmlApplicationContext 也能够读入classpath下的相对路径
- ApplicationContext factory=new FileSystemXmlApplicationContext("classpath:appcontext.xml");  
- ApplicationContext factory=new FileSystemXmlApplicationContext("file:G:/Test/src/appcontext.xml");  
- ApplicationContext factory=new FileSystemXmlApplicationContext("G:/Test/src/appcontext.xml");  
    四: XmlWebApplicationContext   是专为Web工程定制的。
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136599#)[copy](http://blog.csdn.net/weoln/article/details/4136599#)
- ServletContext servletContext = request.getSession().getServletContext();  
-  ApplicationContext ctx = WebApplicationContextUtils.getWebApplicationContext(servletContext );  
