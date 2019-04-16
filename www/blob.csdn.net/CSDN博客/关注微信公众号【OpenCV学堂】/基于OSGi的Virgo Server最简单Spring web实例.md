# 基于OSGi的Virgo Server最简单Spring web实例 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年06月15日 16:11:36[gloomyfish](https://me.csdn.net/jia20003)阅读数：11439











基于OSGi的Virgo Server最简单Spring web实例




一：开发工具下载与环境搭建


1.       下载并安装JDK6u30版本，下载地址如下：

[http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downlo](http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html#jdk-6u30-oth-JPR)

[ads-javase6-419409.html#jdk-6u30-oth-JPR](http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html#jdk-6u30-oth-JPR)


2.       下载并安装SpringsourceTools Suite开发IDE工具版本为**2.7.2.RELEASE**，下载地址：

[http://download.springsource.com/release/STS/2.7.2/dist/e3.7/springsource-tool-suite-](http://download.springsource.com/release/STS/2.7.2/dist/e3.7/springsource-tool-suite-2.7.2.RELEASE-e3.7-win32-installer.exe)

[2.7.2.RELEASE-e3.7-win32-installer.exe](http://download.springsource.com/release/STS/2.7.2/dist/e3.7/springsource-tool-suite-2.7.2.RELEASE-e3.7-win32-installer.exe)


3.       下载并安装VirgoServer版本为3.0.2，下载地址为：

[http://www.eclipse.org/downloads/download.php?file=/virgo/release/VTS/3.0.2.RE](http://www.eclipse.org/downloads/download.php?file=/virgo/release/VTS/3.0.2.RELEASE/virgo-tomcat-server-3.0.2.RELEASE.zip)

[LEASE/virgo-tomcat-server-3.0.2.RELEASE.zip](http://www.eclipse.org/downloads/download.php?file=/virgo/release/VTS/3.0.2.RELEASE/virgo-tomcat-server-3.0.2.RELEASE.zip)


4.      下载并安装Maven工具版本为3.03， 下载地址：

[http://www.apache.org/dyn/closer.cgi/maven/binaries/apache-maven-3.0.3-bin.zip](http://www.apache.org/dyn/closer.cgi/maven/binaries/apache-maven-3.0.3-bin.zip).


5.       打开安装好的STS(SpringSource Tools Suite)工具，Menu > Help > Install New 

Software >[http://subclipse.tigris.org/update_1.6.x](http://subclipse.tigris.org/update_1.6.x)更新安装Virgo
 Server插件




二：创建WebBundle项目


将VirgoServer与maven分别解压缩到D:\osgi\project\virgo_tomcat与D:\osgi\project\maven


1.       打开STS(SpringSourceTools Suite)， 选择Menu->Window->Preferences->Maven->


          Installations

![](https://img-my.csdn.net/uploads/201206/15/1339747404_7221.png)

2. 根据Wizard创建一个Mavenbundle project，在选择时候选择webapp就可以成为一个web bundle项目：

![](https://img-my.csdn.net/uploads/201206/15/1339747497_4705.png)






右键选择创建好的项目，在弹出菜单中选择SpringTools -> enable OSGi


然后再次选择SpringTools -> enable incremental generation of MANIFEST.MF file


3. 启动与访问virgoserver的splash页面


在serverview中右键选择New->server，弹出如下画面：
![](https://img-my.csdn.net/uploads/201206/15/1339747592_5916.png)






选择VirgoWeb Server点击【add】按钮以后
![](https://img-my.csdn.net/uploads/201206/15/1339747625_7254.png)





点击【finish】结束配置，点击ServerView中的启动按钮，启动VirgoServer以后


在浏览器输入[http://localhost:8080](http://localhost:8080/)即可看到如下画面
![](https://img-my.csdn.net/uploads/201206/15/1339747668_5431.png)






4.  项目布局如下：

![](https://img-my.csdn.net/uploads/201206/15/1339747755_5442.png)


**5.  Web.xml内容**



```
<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >

<web-app>
	<display-name>Archetype Created Web Application</display-name>
	<context-param>
		<param-name>contextClass</param-name>
		<param-value>
			org.eclipse.virgo.web.dm.ServerOsgiBundleXmlWebApplicationContext
		</param-value>
	</context-param>
	<listener>
		<listener-class>
			org.springframework.web.context.ContextLoaderListener
		</listener-class>
	</listener>
	<servlet>
		<servlet-name>gloomyfish</servlet-name>
		<servlet-class>
			org.springframework.web.servlet.DispatcherServlet
		</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>gloomyfish</servlet-name>
		<url-pattern>/app/*</url-pattern>
	</servlet-mapping>
	<welcome-file-list>
        <welcome-file>Hello.jsp</welcome-file>
    </welcome-file-list>
</web-app>
```
**6. gloomyfish-servlet.xml内容**





```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:context="http://www.springframework.org/schema/context"
xmlns:mvc="http://www.springframework.org/schema/mvc"
xsi:schemaLocation="http://www.springframework.org/schema/mvc
http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context.xsd">

<context:component-scan base-package="com.gloomyfish.web" />
<!-- enable anntotation-driven controllers -->
<bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter" />

<!-- configure FreeMarker support -->
<bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
	<property name="templateLoaderPath" value="/WEB-INF/ftl/" />
</bean>

<bean id="viewResolver"	class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
	<property name="cache" value="true" />
	<property name="suffix" value=".ftl" />
</bean>
</beans>
```

打包部署项目，使用Maven编译打包以后，选择配置好的Virgo Server图标，右键[Add and Remove]

在弹出的对话框中选则项目，点击add增加到Virgo Server点击finish

![](https://img-my.csdn.net/uploads/201206/15/1339747972_3685.png)


启动运行，选择绿色运行图标，启动Virgo Sever访问如下URL：

[http://localhost:8080/gloomyfish](http://localhost:8080/gloomyfish), 看到效果是一个基于HTML5的太极图案，要求在支持HTML5的

浏览器上才可以看到效果如下：
![](https://img-my.csdn.net/uploads/201206/15/1339748002_2126.png)
**项目打包源代码下载地址为：**
[**http://download.csdn.net/detail/jia20003/4375418**](http://download.csdn.net/detail/jia20003/4375418)






