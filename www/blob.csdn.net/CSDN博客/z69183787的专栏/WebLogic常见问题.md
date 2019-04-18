# WebLogic常见问题 - z69183787的专栏 - CSDN博客
2014年07月30日 12:29:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18503
**1、ClassNotFoundException：org.hibernate.hql.ast.HqlToken异常**
使用Hibernate的工程部署到WebLogic下时，经常会出现ClassNotFoundException：org.hibernate.hql.ast.HqlToken的问题，出现该问题的主要原因在于WebLogic已经集成了antlr.jar包，在使用Hibernate时，会先加载其自身所带的antlr.jar包，会和Hibernate捆绑的antlr-2.7.6.jar冲突，导致上述错误。
解决方法：
(1)在myeclipse7.0中：
依次点击windows→preferences→myeclipse workbench→servers→weblogic→weblogic10.x→path，在prepend to classpath选项中，把hibernate要用到的包antlr-2.7.6.jar添加到weblogic的classpath中。
这样在weblogic便可以通过myeclipse正常启动，以上提到的问题解决。
(2)如果想要通过weblogic自带的脚本来运行：
将工程里的antlr-2.7.6.jar拷贝到weblogic的wlserver_10.3\server\
lib文件夹下面，在user_projects\domains\base_domain\bin里找到startWebLogic.cmd文件，找到：
set CLASSPATH=%SAVE_CLASSPATH%，在下面增加以下3行代码：
@REM hibernate3.0
set PRE_CLASSPATH=%WL_HOME%\server\lib\antlr-2.7.6.jar 
set CLASSPATH=%PRE_CLASSPATH%;%CLASSPATH%
保存后双击该脚本启动weblogic，即可由非myeclipse的方式启动工程了。
**2、工程打包成.war部署到WebLogic后，出现如下问题：**
Error: weblogic.management.DeploymentException: Cannot set web app root system property when WAR file is not expanded - with nested exception: [java.lang.IllegalStateException: Cannot set web app root system property when WAR
 file is not expanded]
原因为org.springframework.web.util.Log4jConfigListener 调用了getRealPath()方法，返回null，
String root = servletContext.getRealPath("/");
if (root == null) {
throw new IllegalStateException("Cannot set web app root system property when WAR file is not expanded");
}
**3、关于request.getRealPath()的出错**
而在servlet中使用getServletContext.getRealPath()这个方法收到war 和non-war的影响，以及不同app server实现的影响，运气好的话，你常常会得到null，嘿嘿，比如你在weblogic上部署war文件，又调用这个方法..
推荐ServletContext.getResourceAsStream
参考资料：[http://bbs.chinaunix.net/viewthread.php?tid=383861](http://bbs.chinaunix.net/viewthread.php?tid=383861)
关于serveletContext.getRealPath返回NULL和不同的app server返回不同的结果
resoure url：
http://dev2dev.bea.com.cn/bbs/thread.jspa?forumID=121&threadID=12968&start=0&tstart=0
问题：
我的应用有几个配置文本配置文件（是一些报表的模板)，我放在WEB-INF下面的config目录下，我有程序中是这样得到这个config的实际路径的，先用serveletContext.getRealPath得到根路径，tomcat中比如是c:\tomcat\webapp\test，然后我加上 "/WEB-INF/config/aa.config"，这样得到文件的path然后进行读入，　　应用在tomcat上跑是ok的，后来将war放到weblogic上，出错，原因是：在weblogic上用getRealPath得到的是像myserver\stage\_appsdir_test_war\test.war!\WEB-INF\config....这样的路径，于是一直报FileNotFoundException。
解决：
serveletContext.getRealPath
这个方法在不同的服务器上所获得的实现是不一样的，这种情况我也同样遇到过，建议是通过classloader来获得你配置的资源文件，context.getRealPath("/")可能返回了null，你可以输入来看看，对一个打包的应用来说，是没有RealPath的概念的，调用getRealPath只会简单地返回null。其实，也很好理解，一个文件被打包入了.war文件，就不存在目录结构了（虽然包中仍然存在目录结构，但这不等同于文件系统中的目录结构）。所以，对war包中的资源是无法得到RealPath的。这样也就无从通过文件IO进行读取了。那么，如何读取war包中的资源呢？答案是使用ServletContext.getResourceAsStream
("/WEB-INF/config/aa.config")方法。
原则：基本上就是尽量使用j2ee规范中的各层次classloader来获取资源，而不是试图去找文件的绝对路径方法：调用this.getClass().getClassLoader().getResource("/").getPath(); 获取到classes目录的全路径
使用：在得到classes目录的全路径后再根据字符串的截取与拼装达到你的要求即可。
**4、getServletContext()方法报错**
在java文件中执行getServletContext()方法时，报weblogic.servlet.jsp.getServletContext()不存在的错误，解决办法：把this.getServletContext()改为this.getServletConfig().getServletContext()。
**5、java.lang.NoClassDefFoundError: weblogic/Server**
配置出了问题，应该WebLogic installation directory，将C:\Oracle\Middleware\wlserver_10.3\server改为C:\Oracle\Middleware\wlserver_10.3
如下图:
![2011年11月25日 - JUN - Blog Of Jun](http://img9.ph.126.net/I2hdwHbkytPwD9Cp81dPZA==/62768919823579249.jpg)
6、**Could not find the main class**
因为，高版本的JDK生成的class文件使用的格式，可能与低版本的JDK的.class文件格式不同。这样，低版本的JVM无法解释执行这个.class文件，会抛出Could not find the main class.program will
 exit，java.lang.UnsupportedClassVersionError不支持的Class版本错误。
确定Weblogic配置的JDK与程序使用的一致
![2011年11月25日 - JUN - Blog Of Jun](http://img3.ph.126.net/mzAe0Yia6BwchdG3gxCGHw==/2548755914132093823.jpg)
7、**乱码问题**
乱码问题见我的日志:JavaScript-url传递中文乱码
[http://daizhenghua.good.blog.163.com/blog/static/10528772620111018193357/](http://daizhenghua.good.blog.163.com/blog/static/10528772620111018193357/)
