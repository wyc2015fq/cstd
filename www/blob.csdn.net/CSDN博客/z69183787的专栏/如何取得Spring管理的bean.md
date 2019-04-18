# 如何取得Spring管理的bean - z69183787的专栏 - CSDN博客
2016年05月04日 10:09:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：987
如何取得[Spring](http://www.121ask.com/list-1168.html)管理的bean  （请用第3种方法）：
1、servlet方式加载时，
【web.xml】
Xml代码
- <servlet>
- <servlet-name>springMVC</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>cont[Ext](http://www.121ask.com/list-1174.html)ConfigLocation</param-name>
- <param-value>classpath*:/springMVC.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
 spring容器放在ServletContext中的key是org.springframework.web.servlet.FrameworkServlet.CONTEXT.springMVC
注意后面的springMVC，是你的servlet-name配置的值，注意适时修改。
Java代码
- ServletContext sc=略  
- WebApplicationContext attr = (WebApplicationContext)sc.getAttribute("org.springframework.web.servlet.FrameworkServlet.CONTEXT.springMVC");  
2、listener方式加载时：
【web.xml】
Xml代码
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/applicationContext</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
 【[jsp](http://www.121ask.com/list-1166.html)/servlet】可以这样取得
Java代码
- ServletContext context = getServletContext();  
- WebApplicationContext applicationContext  = WebApplicationContextUtils .getWebApplicationContext(context);   
3、通用的方法来了，神器啊，前的  1、2两种方法并不通用，可以抛弃了。
在配置文件中加入：
Xml代码
- <!-- 用于持有ApplicationContext,可以使用SpringContextHolder.getBean('xxxx')的静态方法得到spring bean对象 -->
- <beanclass="com.xxxxx.SpringContextHolder"lazy-init="false"/>
Java代码
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.ApplicationContextAware;  
- /**
-  * 以静态变量保存Spring ApplicationContext, 可在任何代码任何地方任何时候中取出ApplicaitonContext.
-  * 
-  */
- publicclass SpringContextHolder implements ApplicationContextAware {  
- privatestatic ApplicationContext applicationContext;  
- 
- /**
- * 实现ApplicationContextAware接口的context注入函数, 将其存入静态变量.
- */
- publicvoid setApplicationContext(ApplicationContext applicationContext) {  
- SpringContextHolder.applicationContext = applicationContext; // NOSONAR
- }  
- 
- /**
- * 取得存储在静态变量中的ApplicationContext.
- */
- publicstatic ApplicationContext getApplicationContext() {  
- checkApplicationContext();  
- return applicationContext;  
- }  
- 
- /**
- * 从静态变量ApplicationContext中取得Bean, 自动转型为所赋值对象的类型.
- */
- @SuppressWarnings("unchecked")  
- publicstatic <T> T getBean(String name) {  
- checkApplicationContext();  
- return (T) applicationContext.getBean(name);  
- }  
- 
- /**
- * 从静态变量ApplicationContext中取得Bean, 自动转型为所赋值对象的类型.
- */
- @SuppressWarnings("unchecked")  
- publicstatic <T> T getBean(Class<T> clazz) {  
- checkApplicationContext();  
-  Map<String, T> beanMap = applicationContext.getBeansOfType(clazz);  
- Collection<T> valueSet = beanMap.values();  
List<T> valueList = new ArrayList<T>(valueSet);  
return valueList.get(0) ; 
- }  
- 
- /**
- * 清除applicationContext静态变量.
- */
- publicstaticvoid cleanApplicationContext() {  
- applicationContext = null;  
- }  
- 
- privatestaticvoid checkApplicationContext() {  
- if (applicationContext == null) {  
- thrownew IllegalStateException("applicaitonContext未注入,请在applicationContext.xml中定义SpringContextHolder");  
- }  
- }  
- }  
- 
