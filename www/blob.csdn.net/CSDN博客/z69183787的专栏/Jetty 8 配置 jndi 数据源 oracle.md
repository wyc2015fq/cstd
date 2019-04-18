# Jetty 8 配置 jndi 数据源 oracle - z69183787的专栏 - CSDN博客
2014年03月17日 16:52:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3631
- 
Classes from `org.mortbay.jetty.webapp` are
 now in `org.eclipse.jetty.webapp`
- 
Classes from `org.mortbay.jetty.plus.naming` are
 now in `org.eclipse.jetty.plus.jndi`
1、contexts 目录下 新建 与项目名一样的 xml 文件，具体类好像 7、8与6 完全不一样，查看了很多文件和资料才配置成功
jetty\lib\ext 下 增加 c3p0 jar  包
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE Configure PUBLIC "-//Mort Bay Consulting//DTD Configure//EN" "http://www.eclipse.org/jetty/configure.dtd">
<Configure class="org.eclipse.jetty.webapp.WebAppContext">
<Set name="contextPath">/portal</Set>
<Set name="resourceBase">E:/Users/zhoushun/Desktop/jetty/webapps/portal</Set>
      <New id="ds1" class="com.mchange.v2.c3p0.ComboPooledDataSource">
         <Set name="driverClass">oracle.jdbc.driver.OracleDriver</Set>
<Set name="jdbcUrl">jdbc:oracle:thin:@10.1.43.11:1521:stnic1</Set>
<Set name="user">stpt</Set>
<Set name="password">ST/G02000100049</Set>
      </New>
    
      <New id="ds2" class="com.mchange.v2.c3p0.ComboPooledDataSource">
         <Set name="driverClass">oracle.jdbc.driver.OracleDriver</Set>
<Set name="jdbcUrl">jdbc:oracle:thin:@10.1.13.10:1521:stptdemo</Set>
<Set name="user">stpt</Set>
<Set name="password">ST/G02000100049</Set>
      </New>
    
      <New id="ds3" class="com.mchange.v2.c3p0.ComboPooledDataSource">
         <Set name="driverClass">oracle.jdbc.driver.OracleDriver</Set>
<Set name="jdbcUrl">jdbc:oracle:thin:@10.1.43.11:1521:stnic1</Set>
<Set name="user">stfb</Set>
<Set name="password">ST/G02000100049</Set>
      </New>
    
    
    <New id="mydatasource1" class="org.eclipse.jetty.plus.jndi.Resource">
<Arg>jdbc/stptinc</Arg>
<Arg><Ref id="ds1"/></Arg>
</New>
    
    <New id="mydatasource2" class="org.eclipse.jetty.plus.jndi.Resource">
<Arg>jdbc/stptdemo</Arg>
<Arg><Ref id="ds2"/></Arg>
</New>
    
    <New id="mydatasource3" class="org.eclipse.jetty.plus.jndi.Resource">
<Arg>jdbc/stfb</Arg>
<Arg><Ref id="ds3"/></Arg>
</New>
</Configure>
```
2、更改根目录的 start.ini文件，增加 引入，etc/jetty-plus.xml 
```
#===========================================================
# Jetty start.jar arguments
# Each line of this file is prepended to the command line 
# arguments # of a call to:
#    java -jar start.jar [arg...]
#===========================================================
#===========================================================
# If the arguements in this file include JVM arguments 
# (eg -Xmx512m) or JVM System properties (eg com.sun.???),
# then these will not take affect unless the --exec 
# parameter is included or if the output from --dry-run
# is executed like:
#   eval $(java -jar start.jar --dry-run)
#
# Below are some recommended options for Sun's JRE
#-----------------------------------------------------------
 --exec
 -Dorg.apache.jasper.compiler.disablejsr199=true
# -Dcom.sun.management.jmxremote
# -Dorg.eclipse.jetty.util.log.IGNORED=true
# -Dorg.eclipse.jetty.LEVEL=DEBUG
# -Dorg.eclipse.jetty.util.log.stderr.SOURCE=true
# -Xmx2000m
# -Xmn512m
# -verbose:gc
# -XX:+PrintGCDateStamps
# -XX:+PrintGCTimeStamps
# -XX:+PrintGCDetails
# -XX:+PrintTenuringDistribution
# -XX:+PrintCommandLineFlags
# -XX:+DisableExplicitGC
# -XX:+UseConcMarkSweepGC
# -XX:ParallelCMSThreads=2
# -XX:+CMSClassUnloadingEnabled  
# -XX:+UseCMSCompactAtFullCollection
# -XX:CMSInitiatingOccupancyFraction=80
#-----------------------------------------------------------
#===========================================================
# Start classpath OPTIONS.
# These control what classes are on the classpath
# for a full listing do
#   java -jar start.jar --list-options
#-----------------------------------------------------------
OPTIONS=Server,jsp,jmx,resources,websocket,ext,plus,annotations
#-----------------------------------------------------------
#===========================================================
# Configuration files.
# For a full list of available configuration files do
#   java -jar start.jar --help
#-----------------------------------------------------------
#etc/jetty-jmx.xml
etc/jetty.xml
etc/jetty-annotations.xml
# etc/jetty-ssl.xml
# etc/jetty-requestlog.xml
etc/jetty-deploy.xml
etc/jetty-plus.xml
#etc/jetty-overlay.xml
etc/jetty-webapps.xml
etc/jetty-contexts.xml
etc/jetty-testrealm.xml
#===========================================================
```
3、web.xml 中增加 对 数据源的引用（不知道是否必要？ 删除运行后，报错。。。一定要加。）
```
<resource-ref>
        <res-ref-name>jdbc/stptinc</res-ref-name>
        <res-type>javax.sql.DataSource</res-type>
        <res-auth>Container</res-auth>
      </resource-ref>
      
       <resource-ref>
        <res-ref-name>jdbc/stptdemo</res-ref-name>
        <res-type>javax.sql.DataSource</res-type>
        <res-auth>Container</res-auth>
      </resource-ref>
      
       <resource-ref>
        <res-ref-name>jdbc/stfb</res-ref-name>
        <res-type>javax.sql.DataSource</res-type>
        <res-auth>Container</res-auth>
      </resource-ref>
```
4、spring 配置文件中：
```
<bean id="dataSource2" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="java:comp/env/jdbc/stptinc"/>
		</bean>
		
		<bean id="dataSource" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="java:comp/env/jdbc/stptdemo"/>
		</bean>
	
		<bean id="dataSource3" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="java:comp/env/jdbc/stfb"/>
		</bean>
```

