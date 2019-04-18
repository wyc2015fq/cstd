# 在weblogic 部署出现的 错误 - z69183787的专栏 - CSDN博客
2013年02月21日 14:48:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：33144
- The validator **class**: "org.apache.taglibs.standard.tlv.JstlCoreTLV" has failed with the following exception:"java.lang.ClassCastException:
 weblogic.xml.jaxp.RegistrySAXParserFactory cannot be cast to javax.xml.parsers.SAXParserFactory".   
- <%@page contentType="text/html;charset=utf-8"%>   
- ^---------------------------------------------^  
If your webapplication is using the following JAR file : xml-apis-1.x.x then please remove it from your WEB-INF/lib directory.
附录： 在weblogic下部署出现问题汇总
1、 JDK和Servlet版本问题
WebLogic 8.1 sp4以前（包括sp4）只支持JDK1.4，建议使用JDK1.4进行编译代码，有时JDK1.5编译的程序无法运行。
由于WebLogic 8.1不支持J2EE1.4，不要使用Servlet2.4和JSP2.0进行编码。
2、 Struts 加载问题
在TOMCAT中，加载Struts的顺序是通过servlet加载，排在Listener加载之后。如果在Struts中使用Plugin，会在TOMCAT启动的最后加载，所以在Plugin中可以使用Spring中的Bean。
移植到WebLogic后，Struts会在容器启动的时候全部加载，包括Plugin。这样就出现了在Plugin加载的时候，不能得到Spring管理的Bean，也就是说Struts Plugin在WebLogic里不能使用Spring管理的Bean。所以如果需要启动时加载部分代码，建议使用Servlet init()方法。
Spring为通过Web启动的程序提供了一个工具，该工具可以从Context中直接得到WebApplicationContext，其工具的方法签名如下：
org.springframework.web.context.support.WebApplicationContextUtils. getWebApplicationContext(ServletContext);
3、 Include问题
在BEA WebLogic中不允许在一个文件中出现一次以上类似<%@ page contentType="text/html; charset=GBK"%>的代码，所以使用include file时，请将被include的文件中类似代码删除。
在TOMCAT时允许上述代码出现多回，并且使用include file时，被include的文件中，不包含上述代码，编译后客户端显示为乱码。BEA为此解释为TOMCAT不符合J2EE规范。
为了增加代码的通用性和可移植性，建议使用<jsp:include>方式。
<jsp:include>将被include的jsp代码视为独立存在的文件，所以可以在不同文件内使用多个<%@ page contentType="text/html; charset=GBK"%>。<jsp:include>直接传参由<jsp:param>标签完成，在被include页面可以通过request得到传入的值，也可以通过request.setAttribute()、request.getAttribute()进行内外文件参数传递。
4、 打包后Log4j支持问题
打包成.war部署到WebLogic后，出现如下问题：
Error: weblogic.management.DeploymentException: Cannot set web app root system property when WAR file is not expanded - with nested exception:
[java.lang.IllegalStateException: Cannot set web app root system property when WAR file is not expanded]
问题解决：通常您不需要亲自编写servlet或者listener，比如直接利用log4j的com.apache.jakarta.log4j.Log4jInit类,Spring的org.springframework.web.util.Log4jConfigServlet和org.springframework.web.util.ServletContextListener方式配置，找到.Log4jConfigServlet和ServletContextListener的源码，他们都在适当的地方(callback
 method)调用了Log4jWebConfigurer.initLogging(getServletContext());定位到这个方法，第一句就是:WebUtils.setWebAppRootSystemProperty(servletContext);再定位到该方法，方法很短：
public static void setWebAppRootSystemProperty(ServletContext servletContext) throws IllegalStateException {
String param = servletContext.getInitParameter(WEB_APP_ROOT_KEY_PARAM);
String key = (param != null ? param : DEFAULT_WEB_APP_ROOT_KEY);
String oldValue = System.getProperty(key);
if (oldValue != null) {
throw new IllegalStateException("WARNING: Web app root system property already set: " + key + " = " + oldValue + " - Choose unique webAppRootKey values in your web.xml files!");
}
String root = servletContext.getRealPath("/");
if (root == null) {
throw new IllegalStateException("Cannot set web app root system property when WAR file is not expanded");
}
System.setProperty(key, root);
servletContext.log("Set web app root system property: " + key + " = " + root);
}
系统需要读取webAppRootKey这个参数，所以在部署到WebLogic里的时候，在web.xml中手动添加如下代码：
<context-param>
<param-name>webAppRootKey</param-name>
<param-value>webapp.root</param-value>
</context-param>
WebLogic自身也包含对Log4j的支持，在打包部署（.war）的时候，会和Spring的org.springframework.web.util.Log4jConfigListener有冲突（拷贝到WebLogic散放部署不会出错）。所以改用Servlet加载。（不通过应用加载Log4j好像也可以使用，但未进行完整测试，下面代码修改后，系统会报Log4j加载重复错误，不影响应用启动。）
web.xml中删除下面代码：
<listener id="log4jConfigListener">
<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
</listener>
将Listener加载改为通过Servlet加载，再在web.xml增加：
<servlet>
<servlet-name>log4jConfigListener</servlet-name>
<servlet-class>org.springframework.web.util.Log4jConfigServlet</servlet-class>
<load-on-startup>0</load-on-startup>
</servlet>
5、 Hibernate3、Axis部署问题
Hibernate3中hibernate.query.factory_class的默认值为org.hibernate.hql.ast.ASTQueryTranslatorFactory，在WebLogic下系统运行时会抛出org.hibernate.QueryException: ClassNotFoundException: org.hibernate.hql.ast.HqlToken异常。
这个问题网上说的很多，解决方法也各式各样，其实很简单，Weblogic系统默认加载EJB-QL parser，存在重名类，所以使用时会出现ClassNotFoundException。一般网上的修改方式都是修改startWebLogic运行的脚本，将antlr-2.7.5H3.jar文件优先加载。但这样的方法会带来一些其他问题，所以不推荐使用。最好的方法是，在WEB-INF目录下建一个weblogic.xml文件，文件中写入如下代码：
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "[http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd](http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd)">
<weblogic-web-app>
<container-descriptor>
    <prefer-web-inf-classes>true</prefer-web-inf-classes>
</container-descriptor>
</weblogic-web-app>
说明：prefer-web-inf-classes=true是WebLogic's classloader在有重名类时，优先加载Web应用中WEB-INF内的类。
Axis部署同理。
注意：
1、如果有包在通过修改startWebLogic启动脚本优先加载后，web应用中有重复的包，并且将prefer-web-inf-classes=true，BEA WebLogic编译JSP时会报错。（直接设置true就可以，无需再修改脚本；如果已经修改过脚本，需要还原。）
2、在apache中提供两种方式部署Axis包，一种为prefer-web-inf-classes=true，另外一种将saaj.jar一个包在webservices.jar之前优先加载。经实际验证，只将saaj.jar一个包优先加载并不能解决全部问题，如果不使用前一种方法，请将axis全部的包加载在webservices.jar之前。
6、 BEA WebLogic Crashes问题
在实际部署到Unix下的BEA时，我遇到过两回WebLogic Crash问题，都是通过BEA Support解决的，下面分享一下两回Crash的过程、解决方法及经验。
服务器环境：HP Unix 11.23；HP JDK1.4.2_10；WebLogic8.1 SP5
7.       发布Web Service接口，远程程序调用两回后BEA WebLogic Crash
在Tomcat上测试正常的Web Service接口，移植到BEA 后，第一回调用该接口一切正常，第二次调用时，系统直接报错，WebLogic进程从系统中消失。
BEA Support给出的解释是内存分配不足，增加BEA WebLogic应用内存最大值后，系统正常。
8.     Axis远程调用.net Web Service接口，出现java.lang.NoSuchMethodError: javax.xml.namespace.QName.getPrefix()Ljava/lang/String;错误，将包含该类的包优先加载，调用.net Web Service接口时系统Crash
我所用的应用系统需要调用远程.net平台的Web Service接口，该程序在Tomcat和Windows 下BEA WebLogic 8.1 SP5下进行测试，全部正常使用，但移植到HPUX上，每次调用接口时都会找不到javax.xml.namespace.QName.getPrefix()方法。查明该方法存在于jaxrpc.jar文件中，而webservices.jar存在名为javax.xml.namespace.QName的重名类。在startWebLogic.sh文件中修改，手动将jaxpc.jar排在webservices.jar之前加载，重启系统后，调用.net
 Web Service时出现Crash。
BEA Support给的解释是classloader冲突，确切的冲突原因不明，Windows下没有问题的话考虑升级JVM，或者使用Application内的Class优先加载。
最后通过问题4的解决方式解决。（详细原理请参见[http://e-docs.bea.com/wls/docs81/programming/classloading.html](http://e-docs.bea.com/wls/docs81/programming/classloading.html)）
个人感觉在UNIX下BEA不是很稳定，如果类似出Crash问题，最好请BEA Support解决。BEA Support定位问题需要出现问题的原因、出错生成的日志文件，并且配合他修改一些参数获得更多的日志文件，这些日志文件都在启动的Domain目录下。当购买BEA服务后，不建议过多花费时间在解决这类问题上（网上基本没有太多资料）。
9. 还有this.getServletContext()要改为this.getServletConfig().getServletContext()
10.weblogic.xml.jaxp.RegistrySAXParserFactory cannot be cast to javax.xml.parsers.SAXParserFactory
解决办法：
删除xml-apis-1.3.03 从WEB-INF/lib下 。
配置weblogic.xml ，强制weblogic 首先加载应用WEB-INF 中的class。内容如下：
<?xml version="1.0" encoding="UTF-8"?>
<weblogic-web-app>
<container-descriptor>
    <prefer-web-inf-classes>true</prefer-web-inf-classes>
</container-descriptor>
</weblogic-web-app>
直接将 weblogic.xml放到和web.xml同目录下就可以了。这样可以优先执行web-inf下的包或者类。可以解决包冲突的解决办法。
11.
如果出现java.lang.ClassCastException: weblogic.xml.jaxp.RegistryDocumentBuilder 
这样的错误,则肯定是weblogic解析xml时跟别的包冲突了.我把xercesImpl-2.6.2.jar包去掉后就ok.
12..web.xml将spring加载换成listener
<listener>
   <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>
<!--
<servlet>
      <servlet-name>context</servlet-name>
    <servlet-class>org.springframework.web.context.ContextLoaderServlet</servlet-class>
    <load-on-startup>1</load-on-startup>
    </servlet>
13 hiber3
   <?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "weblogic810-web-jar.dtd">
<weblogic-web-app>
 <jsp-descriptor>
  <jsp-param>
   <param-name>keepgenerated</param-name>
   <param-value>true</param-value>
  </jsp-param>
 </jsp-descriptor>
 <container-descriptor>
  <prefer-web-inf-classes>true</prefer-web-inf-classes>
 </container-descriptor>
</weblogic-web-app>
14 weblogic 发布时候报session 被锁定，解决方式
**The domain edit lock is owned by another session - this deployment operation requires exclusive access to the edit lock and hence cannot proceed.You can release the lock in Administration Console by first disabling "Automatically Acquire Lock and Activate Changes" in Preference,then clicking the Release Configuration button.**
[http://coderdream.iteye.com/blog/346592](http://coderdream.iteye.com/blog/346592)
15 weblogic 10 项目包
weblogic.application.ModuleException: 
 at weblogic.servlet.internal.WebAppModule.prepare(WebAppModule.java:387)
 at weblogic.application.internal.flow.ScopedModuleDriver.prepare(ScopedModuleDriver.java:176)
 at weblogic.application.internal.flow.ModuleListenerInvoker.prepare(ModuleListenerInvoker.java:93)
 at weblogic.application.internal.flow.DeploymentCallbackFlow$1.next(DeploymentCallbackFlow.java:387)
 at weblogic.application.utils.StateMachineDriver.nextState(StateMachineDriver.java:37)
 at weblogic.application.internal.flow.DeploymentCallbackFlow.prepare(DeploymentCallbackFlow.java:58)
 at weblogic.application.internal.flow.DeploymentCallbackFlow.prepare(DeploymentCallbackFlow.java:42)
 at weblogic.application.internal.BaseDeployment$1.next(BaseDeployment.java:615)
 at weblogic.application.utils.StateMachineDriver.nextState(StateMachineDriver.java:37)
 at weblogic.application.internal.BaseDeployment.prepare(BaseDeployment.java:191)
 at weblogic.application.internal.SingleModuleDeployment.prepare(SingleModuleDeployment.java:16)
 at weblogic.application.internal.DeploymentStateChecker.prepare(DeploymentStateChecker.java:155)
 at weblogic.deploy.internal.targetserver.AppContainerInvoker.prepare(AppContainerInvoker.java:60)
 at weblogic.deploy.internal.targetserver.AppDeployment.prepare(AppDeployment.java:141)
 at weblogic.management.deploy.internal.DeploymentAdapter$1.doPrepare(DeploymentAdapter.java:39)
 at weblogic.management.deploy.internal.DeploymentAdapter.prepare(DeploymentAdapter.java:187)
 at weblogic.management.deploy.internal.AppTransition$1.transitionApp(AppTransition.java:21)
 at weblogic.management.deploy.internal.ConfiguredDeployments.transitionApps(ConfiguredDeployments.java:233)
 at weblogic.management.deploy.internal.ConfiguredDeployments.prepare(ConfiguredDeployments.java:165)
 at weblogic.management.deploy.internal.ConfiguredDeployments.deploy(ConfiguredDeployments.java:122)
 at weblogic.management.deploy.internal.DeploymentServerService.resume(DeploymentServerService.java:173)
 at weblogic.management.deploy.internal.DeploymentServerService.start(DeploymentServerService.java:89)
 at weblogic.t3.srvr.SubsystemRequest.run(SubsystemRequest.java:64)
 at weblogic.work.ExecuteThread.execute(ExecuteThread.java:201)
 at weblogic.work.ExecuteThread.run(ExecuteThread.java:173)
java.lang.ClassCastException: com.ctc.wstx.stax.WstxInputFactory
 at javax.xml.stream.XMLInputFactory.newInstance(XMLInputFactory.java:137)
 at weblogic.servlet.internal.TldCacheHelper$TldIOHelper.parseXML(TldCacheHelper.java:124)
 at weblogic.descriptor.DescriptorCache.parseXML(DescriptorCache.java:380)
 at weblogic.servlet.internal.TldCacheHelper.parseTagLibraries(TldCacheHelper.java:65)
 at weblogic.servlet.internal.War.getTagInfo(War.java:891)
 at weblogic.servlet.internal.WebAppServletContext$ServletContextWebAppHelper.getTldInfo(WebAppServletContext.java:3618)
 at weblogic.servlet.internal.WebAppServletContext$ServletContextWebAppHelper.getTagLibListeners(WebAppServletContext.java:3591)
 at weblogic.servlet.internal.EventsManager.registerEventListeners(EventsManager.java:98)
 at weblogic.servlet.internal.EventsManager.registerPreparePhaseListeners(EventsManager.java:65)
 at weblogic.servlet.internal.WebAppServletContext.initContextListeners(WebAppServletContext.java:1765)
 at weblogic.servlet.internal.WebAppServletContext.prepare(WebAppServletContext.java:1126)
 at weblogic.servlet.internal.HttpServer.doPostContextInit(HttpServer.java:449)
 at weblogic.servlet.internal.HttpServer.loadWebApp(HttpServer.java:424)
 at weblogic.servlet.internal.WebAppModule.registerWebApp(WebAppModule.java:910)
 at weblogic.servlet.internal.WebAppModule.prepare(WebAppModule.java:364)
 at weblogic.application.internal.flow.ScopedModuleDriver.prepare(ScopedModuleDriver.java:176)
 at weblogic.application.internal.flow.ModuleListenerInvoker.prepare(ModuleListenerInvoker.java:93)
 at weblogic.application.internal.flow.DeploymentCallbackFlow$1.next(DeploymentCallbackFlow.java:387)
 at weblogic.application.utils.StateMachineDriver.nextState(StateMachineDriver.java:37)
 at weblogic.application.internal.flow.DeploymentCallbackFlow.prepare(DeploymentCallbackFlow.java:58)
 at weblogic.application.internal.flow.DeploymentCallbackFlow.prepare(DeploymentCallbackFlow.java:42)
 at weblogic.application.internal.BaseDeployment$1.next(BaseDeployment.java:615)
 at weblogic.application.utils.StateMachineDriver.nextState(StateMachineDriver.java:37)
 at weblogic.application.internal.BaseDeployment.prepare(BaseDeployment.java:191)
 at weblogic.application.internal.SingleModuleDeployment.prepare(SingleModuleDeployment.java:16)
 at weblogic.application.internal.DeploymentStateChecker.prepare(DeploymentStateChecker.java:155)
 at weblogic.deploy.internal.targetserver.AppContainerInvoker.prepare(AppContainerInvoker.java:60)
 at weblogic.deploy.internal.targetserver.AppDeployment.prepare(AppDeployment.java:141)
 at weblogic.management.deploy.internal.DeploymentAdapter$1.doPrepare(DeploymentAdapter.java:39)
 at weblogic.management.deploy.internal.DeploymentAdapter.prepare(DeploymentAdapter.java:187)
 at weblogic.management.deploy.internal.AppTransition$1.transitionApp(AppTransition.java:21)
 at weblogic.management.deploy.internal.ConfiguredDeployments.transitionApps(ConfiguredDeployments.java:233)
 at weblogic.management.deploy.internal.ConfiguredDeployments.prepare(ConfiguredDeployments.java:165)
 at weblogic.management.deploy.internal.ConfiguredDeployments.deploy(ConfiguredDeployments.java:122)
 at weblogic.management.deploy.internal.DeploymentServerService.resume(DeploymentServerService.java:173)
 at weblogic.management.deploy.internal.DeploymentServerService.start(DeploymentServerService.java:89)
 at weblogic.t3.srvr.SubsystemRequest.run(SubsystemRequest.java:64)
 at weblogic.work.ExecuteThread.execute(ExecuteThread.java:201)
 at weblogic.work.ExecuteThread.run(ExecuteThread.java:173)
原因是wstl。jar 有冲突。 先吧这个jar删除发布，然后呢，添加上发布就没有事情了，
 在weblogic 10.3.0 以后的版本没有这个错误
