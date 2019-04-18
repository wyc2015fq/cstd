# 项目从tomcat迁移到weblogic环境下问题汇总----不断补充中ing... - z69183787的专栏 - CSDN博客
2013年02月20日 09:30:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11332
1、 JDK和Servlet版本问题 
WebLogic 8.1 sp4以前（包括sp4）只支持JDK1.4，建议使用JDK1.4进行编译代码，有时JDK1.5编译的程序无法运行，由于WebLogic 8.1不支持J2EE1.4，不要使用Servlet2.4和JSP2.0进行编码。 
比如：在tomcat下代码里中可以有response.setCharacterEncoding("UTF-8")方法，这是servlet2.4的方法，所以要发布到weblogic上需要把此方法注释掉。 
另外关于查看weblogic支持的servlet版本方法如下： 
Java代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- javax.servlet.ServletContext   
- 
- getMajorVersion()  //主版本号（2）
- getMinorVersion()  //次版本号 （3）
结果分别为2、3的话就说明，版本号为 servlet2.3 
2、 web.xml 中的启动加载问题 
在TOMCAT中，加载Struts的顺序是通过servlet加载，排在Listener加载之后。如果在Struts中使用Plugin，会在TOMCAT启动的最后加载，所以在Plugin中可以使用Spring中的Bean。 
移植到WebLogic后，Struts会在容器启动的时候全部加载，包括Plugin。这样就出现了在Plugin加载的时候，不能得到 Spring管理的Bean，也就是说Struts Plugin在WebLogic里不能使用Spring管理的Bean。所以如果需要启动时加载部分代码，建议使用Servlet
 init()方法。 
Spring为通过Web启动的程序提供了一个工具，该工具可以从Context中直接得到WebApplicationContext，其工具的方法签名如下： 
Java代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- org.springframework.web.context.support.WebApplicationContextUtils. getWebApplicationContext(ServletContext);     
- 
- org.springframework.web.context.support.WebApplicationContextUtils. getWebApplicationContext(ServletContext);  
3、 JSP 的 Include问题  （建议使用动态引入）
在BEA WebLogic中不允许在一个文件中出现一次以上类似<%@ page contentType="text/html; charset=GBK"%>的代码，所以使用include file时，请将被include的文件中类似代码删除。 
在TOMCAT时允许上述代码出现多回，并且使用include file时，被include的文件中，不包含上述代码，编译后客户端显示为乱码。BEA为此解释为TOMCAT不符合J2EE规范。 
为了增加代码的通用性和可移植性，建议使用<jsp:include>方式。 
<jsp:include>将被include的jsp代码视为独立存在的文件，所以可以在不同文件内使用多个<%@ page contentType="text/html; charset=GBK"%>。<jsp:include>直接传参由<jsp:param>标签完成，在被 include页面可以通过request得到传入的值，也可以通过request.setAttribute()、
 request.getAttribute()进行内外文件参数传递。
4、 对Log4j支持问题 
打包成.war部署到WebLogic后，出现如下问题： 
Java代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- Error: weblogic.management.DeploymentException: Cannot set web app root system property when WAR file is not expanded - with nested exception:      
- 
- [java.lang.IllegalStateException: Cannot set web app root system property when WAR file is not expanded]     
- 
- Error: weblogic.management.DeploymentException: Cannot set web app root system property when WAR file is not expanded - with nested exception:   
- 
- [java.lang.IllegalStateException: Cannot set web app root system property when WAR file is not expanded]   
问题解决：通常您不需要亲自编写servlet或者listener，比如直接利用log4j的 com.apache.jakarta.log4j.Log4jInit类,Spring的 org.springframework.web.util.Log4jConfigServlet和 org.springframework.web.util.ServletContextListener方式配置，找到.Log4jConfigServlet和ServletContextListener的源码，他们都在适当的地方(callback
 method)调用了Log4jWebConfigurer.initLogging(getServletContext());定位到这个方法，第一句就是:WebUtils.setWebAppRootSystemProperty(servletContext);再定位到该方法，方法很短： 
Java代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- publicstaticvoid setWebAppRootSystemProperty(ServletContext servletContext) throws IllegalStateException {      
- 
- String param = servletContext.getInitParameter(WEB_APP_ROOT_KEY_PARAM);      
- 
- String key = (param != null ? param : DEFAULT_WEB_APP_ROOT_KEY);      
- 
- String oldValue = System.getProperty(key);      
- 
- if (oldValue != null) {      
- 
- thrownew IllegalStateException("WARNING: Web app root system property already set: " + key + " = " + oldValue + " - Choose unique webAppRootKey values in your web.xml files!");      
- 
- }      
- 
- String root = servletContext.getRealPath("/");      
- 
- if (root == null) {      
- 
- thrownew IllegalStateException("Cannot set web app root system property when WAR file is not expanded");      
- 
- }      
- 
- System.setProperty(key, root);      
- 
- servletContext.log("Set web app root system property: " + key + " = " + root);      
- 
- }     
- 
- publicstaticvoid setWebAppRootSystemProperty(ServletContext servletContext) throws IllegalStateException {   
- 
- String param = servletContext.getInitParameter(WEB_APP_ROOT_KEY_PARAM);   
- 
- String key = (param != null ? param : DEFAULT_WEB_APP_ROOT_KEY);   
- 
- String oldValue = System.getProperty(key);   
- 
- if (oldValue != null) {   
- 
- thrownew IllegalStateException("WARNING: Web app root system property already set: " + key + " = " + oldValue + " - Choose unique webAppRootKey values in your web.xml files!");   
- 
- }   
- 
- String root = servletContext.getRealPath("/");   
- 
- if (root == null) {   
- 
- thrownew IllegalStateException("Cannot set web app root system property when WAR file is not expanded");   
- 
- }   
- 
- System.setProperty(key, root);   
- 
- servletContext.log("Set web app root system property: " + key + " = " + root);   
- 
- }   
系统需要读取webAppRootKey这个参数，所以在部署到WebLogic里的时候，在web.xml中手动添加如下代码： 
Xml代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- <context-param>
- 
- <param-name>webAppRootKey</param-name>
- 
- <param-value>webapp.root</param-value>
- 
- </context-param>
- 
- <context-param>
- 
- <param-name>webAppRootKey</param-name>
- 
- <param-value>webapp.root</param-value>
- 
- </context-param>
WebLogic自身也包含对Log4j的支持，在打包部署（.war）的时候，会和Spring的 org.springframework.web.util.Log4jConfigListener有冲突（拷贝到WebLogic散放部署不会出错）。所以改用Servlet加载。（不通过应用加载Log4j好像也可以使用，但未进行完整测试，下面代码修改后，系统会报Log4j加载重复错误，不影响应用启动。） 
web.xml中删除下面代码： 
Xml代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- <listenerid="log4jConfigListener">
- 
- <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
- 
- </listener>
- 
- <listenerid="log4jConfigListener">
- 
- <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
- 
- </listener>
将Listener加载改为通过Servlet加载，再在web.xml增加： 
Xml代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- <servlet>
- 
- <servlet-name>log4jConfigListener</servlet-name>
- 
- <servlet-class>org.springframework.web.util.Log4jConfigServlet</servlet-class>
- 
- <load-on-startup>0</load-on-startup>
- 
- </servlet>
- 
- <servlet>
- 
- <servlet-name>log4jConfigListener</servlet-name>
- 
- <servlet-class>org.springframework.web.util.Log4jConfigServlet</servlet-class>
- 
- <load-on-startup>0</load-on-startup>
- 
- </servlet>
5、 Hibernate3、Axis部署问题 
Hibernate3中hibernate.query.factory_class的默认值为 org.hibernate.hql.ast.ASTQueryTranslatorFactory，在WebLogic下系统运行时会抛出 org.hibernate.QueryException: ClassNotFoundException:
 org.hibernate.hql.ast.HqlToken异常。 
这个问题网上说的很多，解决方法也各式各样，其实很简单，Weblogic系统默认加载EJB-QL parser，存在重名类，所以使用时会出现ClassNotFoundException。一般网上的修改方式都是修改startWebLogic运行的脚本，将antlr-2.7.5H3.jar文件优先加载。但这样的方法会带来一些其他问题，所以不推荐使用。最好的方法是，在WEB-INF目录下建一个weblogic.xml文件，文件中写入如下代码： 
Xml代码  ![收藏代码](http://flg126-126-com.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd">
- 
- <weblogic-web-app>
- 
- <container-descriptor>
- 
- <prefer-web-inf-classes>true</prefer-web-inf-classes>
- 
- </container-descriptor>
- 
- </weblogic-web-app>
- 
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd">
- 
- <weblogic-web-app>
- 
- <container-descriptor>
- 
- <prefer-web-inf-classes>true</prefer-web-inf-classes>
- 
- </container-descriptor>
- 
- </weblogic-web-app>
说明：prefer-web-inf-classes=true是WebLogic's classloader在有重名类时，优先加载Web应用中WEB-INF内的类。 
Axis部署同理。 
注意： 
1、如果有包在通过修改startWebLogic启动脚本优先加载后，web应用中有重复的包，并且将prefer-web-inf- classes=true，BEA WebLogic编译JSP时会报错。（直接设置true就可以，无需再修改脚本；如果已经修改过脚本，需要还原。） 
2、在apache中提供两种方式部署Axis包，一种为prefer-web-inf-classes=true，另外一种将saaj.jar一个包在webservices.jar之前优先加载。经实际验证，只将saaj.jar一个包优先加载并不能解决全部问题，如果不使用前一种方法，请将 axis全部的包加载在webservices.jar之前。 
6、 BEA WebLogic Crashes问题 
在实际部署到Unix下的BEA时，我遇到过两回WebLogic Crash问题，都是通过BEA Support解决的，下面分享一下两回Crash的过程、解决方法及经验。 
服务器环境：HP Unix 11.23；HP JDK1.4.2_10；WebLogic8.1 SP5 
7.       发布Web Service接口，远程程序调用两回后BEA WebLogic Crash 
在Tomcat上测试正常的Web Service接口，移植到BEA 后，第一回调用该接口一切正常，第二次调用时，系统直接报错，WebLogic进程从系统中消失。 
BEA Support给出的解释是内存分配不足，增加BEA WebLogic应用内存最大值后，系统正常。 
8.     Axis远程调用.net Web Service接口，出现java.lang.NoSuchMethodError: javax.xml.namespace.QName.getPrefix()Ljava/lang/String;错误，将包含该类的包优先加载，调用.net Web Service接口时系统Crash 
我所用的应用系统需要调用远程.net平台的Web Service接口，该程序在Tomcat和Windows 下BEA WebLogic 8.1 SP5下进行测试，全部正常使用，但移植到HPUX上，每次调用接口时都会找不到 javax.xml.namespace.QName.getPrefix()方法。查明该方法存在于jaxrpc.jar文件中，而
 webservices.jar存在名为javax.xml.namespace.QName的重名类。在startWebLogic.sh文件中修改，手动将jaxpc.jar排在webservices.jar之前加载，重启系统后，调用.net Web Service时出现Crash。 
BEA Support给的解释是classloader冲突，确切的冲突原因不明，Windows下没有问题的话考虑升级JVM，或者使用Application内的Class优先加载。 
最后通过问题4的解决方式解决。（详细原理请参见http://e-docs.bea.com/wls/docs81/programming/classloading.html） 
个人感觉在UNIX下BEA不是很稳定，如果类似出Crash问题，最好请BEA Support解决。BEA Support定位问题需要出现问题的原因、出错生成的日志文件，并且配合他修改一些参数获得更多的日志文件，这些日志文件都在启动的Domain目录下。当购买BEA服务后，不建议过多花费时间在解决这类问题上（网上基本没有太多资料）。 
9. 还有this.getServletContext()要改为this.getServletConfig().getServletContext() 
网络采集表明出处是种美德，原文参见：[http://feicer.iteye.com/blog/561933](http://feicer.iteye.com/blog/561933)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
1、异常：在weblogic报java.lang.StringIndexOutOfBoundsException: String index out of range: -1
原因：工程中使用的spring包的MANIFEST.MF文件太大，导致weblogic加载时出现字符串越界。 
解决：解压包，修改文件的内容，即去除掉无用的说明信息，保证在1k大小左右，再打包放入工程,发布工程就没问题了. 
           这个也主要是对weblogic8.1而言,更高版本的可能已经解决了这个bug 
2、异常：工程中使用到的导出报表模块,当运行到报表填充数据JasperFillManager.fillReport方法时，出现异常。
原因：在fillReport的方法中使用到了java.awt包中的        GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames()的方法,这个是用来 
取字体信息的,取的过程出现异常。 
解决：在weblogic启动参数中加入-Djava.awt.headless=true，使其使用到字体信息时能够得到字体名字列表信息。 
    归结起来这个参数其实是告诉jvm,目前系统中需要用到字体信息. 
3、异常：导出报表文件，出现异常的数据。
原因：response的OutputStream输出流没有清空缓存。 
解决：导出报表文件时，使用的是OutputStream输出流，tomacat默认会清空缓存，weblogic则不会 
所以在调用完报表的exportReport()函数后，还必须把OutputStream输出流flush一下。这里需要注意的是我使用的报表 
组件是JasperReport报表组件.看过原代码后,其实在传入输出流,调用exportReport()生成excel时,并没有在内部flush数据.所以我们必须自己在外面flush缓冲数据. 
4、异常：输出页面时出现Error 500--Internal Server Error：weblogic.utils.ParsingException: nested        TokenStreamException:antlr.TokenStreamIOException异常。原因：页面和weblogic设置的编码格式不一样。 
解决：保证weblogic启动参数的编码和所返回的页面编码一致。比如页面中是<%@ page contentType="text/html; charset=UTF-8" %> 
     则启动参数则加入 -Dibm.system.encoding=UTF-8 -Dfile.encoding=UTF-8。 
     总的来说在，所有涉及的编码格式时，项目中要统一使用编码格式，包括系统配置，应用服务器配置，页面配置等,可以有效避免编码转换出现的乱码等问题. 
网络采集表明出处是种美德，原文参见：[http://hi.baidu.com/ngdh83hd7nv/blog/item/6b7e2c559f944aceb745ae83.html](http://hi.baidu.com/ngdh83hd7nv/blog/item/6b7e2c559f944aceb745ae83.html)
