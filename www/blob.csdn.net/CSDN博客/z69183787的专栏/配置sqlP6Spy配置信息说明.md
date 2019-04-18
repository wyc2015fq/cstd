# 配置sqlP6Spy配置信息说明 - z69183787的专栏 - CSDN博客
2015年02月03日 19:18:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7914
时间紧张，先记一笔，后续优化与完善。
P6SPY 是一个监控JDBC执行语句的开源产品，利用P6SPY很容易的就可以监控到JDBC中执行的SQL语句，便于系统调试和性能调优。
P6SPY 实现道理是对JDBC的症结类停止了一次包装，让应用系统调用自己的类；自己的类截获到SQL语句后再调用实在的JDBC驱动停止执行SQL，这样，在自己的类里面就可以够监控到所有的SQL语句。
    P6SPY 应用非常简略，只须要简略的几步即实现配置：
一．   将p6spy.jar包放到应用的classpath地点的路径中；
二．   修改连接池或者连接配置的jdbc的驱动为p6spy所提供的保证后的驱动，com.p6spy.engine.spy.P6SpyDriver
三．   修改spy.properties并将其放到类搜索目录.
下面,我们以Hibernate 的配置文件为例停止配置文件的说明。
第一步：将p6spy.jar包放到应用的classpath地点的路径中
第二步：修改连接池或者连接配置的jdbc的驱动为p6spy所提供的保证后的驱动，com.p6spy.engine.spy.P6SpyDriver
    说明：
    在单独的Hibernate的应用中，数据库驱动配置在hibernate.cfg.xml里面,所以我须要将配置文件中的connection.driver_class属性从
oracle.jdbc.driver.OracleDriver改为com.p6spy.engine.spy.P6SpyDriver
其他的用户名密码等等配置信息全部不用修改.在web程序中，配置的连接池部份，也只须要修改jdbc-driver的配置便可。
Hibernate.cfg.xml典型配置如下(注意其中的红色粗体部份的配置):
<?xml version='1.0' encoding='utf-8'?>
<!DOCTYPE hibernate-configuration PUBLIC
        "-//Hibernate/Hibernate Configuration DTD 3.0//EN"
        "http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">
<hibernate-configuration>
    <session-factory>
 <property name="connection.driver_class">com.p6spy.engine.spy.P6SpyDriver</property>       
        <property name="connection.url">jdbc:oracle:thin:@localhost:1521:hero</property>
        <property name="connection.username">hr</property>
        <property name="connection.password">hr</property>
        <!-- JDBC connection pool (use the built-in) -->
        <property name="connection.pool_size">1</property>
        <!-- SQL dialect -->
        <property name="dialect">org.hibernate.dialect.Oracle9Dialect</property>
        <!-- Enable Hibernate's automatic session context management -->
        <property name="current_session_context_class">thread</property>
        <!-- Disable the second-level cache  -->
        <property name="cache.provider_class">org.hibernate.cache.NoCacheProvider</property>
        <!-- Echo all executed SQL to stdout -->
        <property name="show_sql">true</property>
        <!-- Drop and re-create the database schema on startup -->
        <property name="hbm2ddl.auto">false</property>
       <property name="hibernate.jdbc.batch_size">0</property>
    </session-factory>
</hibernate-configuration>
第三步.修改spy.properties并将其放到类搜索目录.
    说明:
       spy.properties中须要注意的地方较多.
（1）       module.log的属性必须配置，如果不配置，P6SPY将不起任何作用，典型配置为：module.log=com.p6spy.engine.logging.P6LogFactory
（2）       realdriver表示实在的驱动，上面配置的com.p6spy.engine.spy.P6SpyDriver会终究调用真是的驱动程序执行SQL
Oracle数据库的配置为：
realdriver=oracle.jdbc.driver.OracleDriver
（3）       appender配置，一般分为三种
#appender=com.p6spy.engine.logging.appender.Log4jLogger
# appender=com.p6spy.engine.logging.appender.StdoutLogger
#appender=com.p6spy.engine.logging.appender.FileLogger
其中，appender=com.p6spy.engine.logging.appender.StdoutLogger表示将输出的SQL或者日志信息输出到Console窗口。
（4）       log4j.logger.p6spy一般须要将log4j的相关参数从默认的
log4j.logger.p6spy=info,STDOUT
修改为
log4j.logger.p6spy=warn,STDOUT
以减少p6spy信息的输出
（5）       可以将dateformat=修改为dateformat=hh:mm:ss,SSS有利用时间信息的查看。
（6）       尽量根据P6Spy的标准的示例的配置文件停止修改，不用随便删除配置信息。
典型spy. spy.properties的配置文件如下,（注意下面的红色粗体部份）：
#################################################################
# P6Spy Options File                                            #
# See documentation for detailed instructions                   #
#################################################################
#################################################################
# MODULES                                                       #
#                                                               #
# Modules provide the P6Spy functionality.  If a module, such   #
# as module_log is commented out, that functionality will not   #
# be available.  If it is not commented out (if it is active),  #
# the functionality will be active.                             #
#                                                               #
# Values set in Modules cannot be reloaded using the            #
# reloadproperties variable.  Once they are loaded, they remain #
# in memory until the application is restarted.                 #
#                                                               #
#################################################################
module.log=com.p6spy.engine.logging.P6LogFactory
#module.outage=com.p6spy.engine.outage.P6OutageFactory
#################################################################
# REALDRIVER(s)                                                 #
#                                                               #
# In your application server configuration file you replace the #
# "real driver" name with com.p6spy.engine.P6SpyDriver. This is #
# where you put the name of your real driver P6Spy can find and #
# register your real driver to do the database work.            #
#                                                               #
# If your application uses several drivers specify them in      #
# realdriver2, realdriver3.  See the documentation for more     #
# details.                                                      #
#                                                               #
# Values set in REALDRIVER(s) cannot be reloaded using the      #
# reloadproperties variable.  Once they are loaded, they remain #
# in memory until the application is restarted.                 #
#                                                               #
#################################################################
# oracle driver
 realdriver=oracle.jdbc.driver.OracleDriver
# mysql Connector/J driver
# realdriver=com.mysql.jdbc.Driver
# informix driver
# realdriver=com.informix.jdbc.IfxDriver
# ibm db2 driver
# realdriver=COM.ibm.db2.jdbc.net.DB2Driver
# the mysql open source driver
# realdriver=org.gjt.mm.mysql.Driver
#specifies another driver to use
realdriver2=
#specifies a third driver to use
realdriver3=
#the DriverManager class sequentially tries every driver that is
#registered to find the right driver.  In some instances, it's possible to
#load up the realdriver before the p6spy driver, in which case your connections
#will not get wrapped as the realdriver will "steal" the connection before
#p6spy sees it.  Set the following property to "true" to cause p6spy to
#explicitily deregister the realdrivers
deregisterdrivers=false
################################################################
# P6LOG SPECIFIC PROPERTIES                                    #
################################################################
# no properties currently available
################################################################
# EXECUTION THRESHOLD PROPERTIES                               #
################################################################
# This feature applies to the standard logging of P6Spy.       #
# While the standard logging logs out every statement          #
# regardless of its execution time, this feature puts a time   #
# condition on that logging.  Only statements that have taken  #
# longer than the time specified (in milliseconds) will be     #
# logged.  This way it is possible to see only statements that #
# have exceeded some high water mark.                          #
# This time is reloadable.                                     #
#
# executionthreshold=integer time (milliseconds)
#
executionthreshold=
################################################################
# P6OUTAGE SPECIFIC PROPERTIES                                 #
################################################################
# Outage Detection
#
# This feature detects long-running statements that may be indicative of
# a database outage problem. If this feature is turned on, it will log any
# statement that surpasses the configurable time boundary during its execution.
# When this feature is enabled, no other statements are logged except the long
# running statements. The interval property is the boundary time set in seconds.
# For example, if this is set to 2, then any statement requiring at least 2
# seconds will be logged. Note that the same statement will continue to be logged
# for as long as it executes. So if the interval is set to 2, and the query takes
# 11 seconds, it will be logged 5 times (at the 2, 4, 6, 8, 10 second intervals).
#
# outagedetection=true|false
# outagedetectioninterval=integer time (seconds)
#
outagedetection=false
outagedetectioninterval=
################################################################
# COMMON PROPERTIES                                            #
################################################################
# filter what is logged
filter=false
# comma separated list of tables to include when filtering
include     =
# comma separated list of tables to exclude when filtering
exclude     =
# sql expression to evaluate if using regex filtering
sqlexpression =
# turn on tracing
autoflush   = true
# sets the date format using Java's SimpleDateFormat routine
dateformat=hh:mm:ss,SSS
#list of categories to explicitly include
includecategories=
#list of categories to exclude: error, info, batch, debug, statement,
#commit, rollback and result are valid values
excludecategories=info,debug,result,batch
#allows you to use a regex engine or your own matching engine to determine
#which statements to log
#
#stringmatcher=com.p6spy.engine.common.GnuRegexMatcher
#stringmatcher=com.p6spy.engine.common.JakartaRegexMatcher
stringmatcher=
# prints a stack trace for every statement logged
stacktrace=false
# if stacktrace=true, specifies the stack trace to print
stacktraceclass=
# determines if property file should be reloaded
reloadproperties=false
# determines how often should be reloaded in seconds
reloadpropertiesinterval=60
#if=true then url must be prefixed with p6spy:
useprefix=false
#specifies the appender to use for logging
#appender=com.p6spy.engine.logging.appender.Log4jLogger
appender=com.p6spy.engine.logging.appender.StdoutLogger
#appender=com.p6spy.engine.logging.appender.FileLogger
# name of logfile to use, note Windows users should make sure to use forward slashes in their pathname (e:/test/spy.log) (used for file logger only)
logfile     = spy.log
# append to  the p6spy log file.  if this is set to false the
# log file is truncated every time.  (file logger only)
append=true
#The following are for log4j logging only
log4j.appender.STDOUT=org.apache.log4j.ConsoleAppender
log4j.appender.STDOUT.layout=org.apache.log4j.PatternLayout
log4j.appender.STDOUT.layout.ConversionPattern=p6spy-%m%n
#log4j.appender.CHAINSAW_CLIENT=org.apache.log4j.net.SocketAppender
#log4j.appender.CHAINSAW_CLIENT.RemoteHost=localhost
#log4j.appender.CHAINSAW_CLIENT.Port=4445
#log4j.appender.CHAINSAW_CLIENT.LocationInfo=true
log4j.logger.p6spy=warn,STDOUT
#################################################################
# DataSource replacement                                        #
#                                                               #
# Replace the real DataSource class in your application server  #
# configuration with the name com.p6spy.engine.spy.P6DataSource,#
# then add the JNDI name and class name of the real     #
# DataSource here                         #
#                                                               #
# Values set in this item cannot be reloaded using the          #
# reloadproperties variable.  Once it is loaded, it remains     #
# in memory until the application is restarted.                 #
#                                                               #
#################################################################
#realdatasource=/RealMySqlDS
#realdatasourceclass=com.mysql.jdbc.jdbc2.optional.MysqlDataSource
    每日一道理 
记不清有多少个夜晚，在我翻阅纸张的指间滑落；记不清有多少支蜡烛，在我的凝视中化为灰烬。逝者如斯，我时时刻刻会听见自己对生命承诺的余音，感到岁月的流转在渐渐稀释我的年少无知，我愿自己是一只上足了发条的时钟，在昼夜不停的流转中留下自己充实的每一刻。
#################################################################
# DataSource properties                                         #
#                                                               #
# If you are using the DataSource support to intercept calls    #
# to a DataSource that requires properties for proper setup,    #
# define those properties here. Use name value pairs, separate  #
# the name and value with a semicolon, and separate the         #
# pairs with commas.                                            #
#                                         #
# The example shown here is for mysql                         #
#                                                               #
#################################################################
#realdatasourceproperties=port;3306,serverName;ibmhost,databaseName;mydb
#################################################################
# JNDI DataSource lookup                                        #
#                                                               #
# If you are using the DataSource support outside of an app     #
# server, you will probably need to define the JNDI Context     #
# environment.                                                  #
#                                                               #
# If the P6Spy code will be executing inside an app server then #
# do not use these properties, and the DataSource lookup will   #
# use the naming context defined by the app server.             #
#                                                               #
# The two standard elements of the naming environment are   #
# jndicontextfactory and jndicontextproviderurl. If you need    #
# additional elements, use the jndicontextcustom property.      #
# You can define multiple properties in jndicontextcustom,      #
# in name value pairs. Separate the name and value with a       #
# semicolon, and separate the pairs with commas.                #
#                                                               #
# The example shown here is for a standalone program running on #
# a machine that is also running JBoss, so the JDNI context     #
# is configured for JBoss (3.0.4).                              #
#                                                               #
#################################################################
#jndicontextfactory=org.jnp.interfaces.NamingContextFactory
#jndicontextproviderurl=localhost:1099
#jndicontextcustom=java.naming.factory.url.pkgs;org.jboss.nameing:org.jnp.interfaces
#jndicontextfactory=com.ibm.websphere.naming.WsnInitialContextFactory
#jndicontextproviderurl=iiop://localhost:900
|配置项名称|默认值|配置项意义及相关注意事项|
|----|----|----|
|module.log|com.p6spy.engine.logging. P6LogFactory|用来拦截和记录任务应用程序的 JDBC 语句。若无配置或注释掉则无此功能。|
|module.outage|com.p6spy.engine.outage. P6OutageFactory|检测和记录执行时间比较长的 SQL 语句。若无配置或注释掉则无此功能。|
|realdriver||真正的应用系统应用的数据库驱动程序名称。|
|realdriver2||真正的应用系统应用的第二种备用数据库驱动程序名称。|
|realdriver3||真正的应用系统应用的第三种备用数据库驱动程序名称。|
|deregisterdrivers|false|显示地把真正的数据库的驱动程序停止反注册掉。取值 true| false|
|executionthreshold||P6Log 模块执行时间设置，整数值 (以毫秒为单位)，只有当超过这个时间才停止记录 Log。|
|outagedetection|false|P6Outage 模块是否记录较长时间运行的语句。取值 true| false|
|outagedetectioninterval||P6Outage 模块执行时间设置，整数值 （以秒为单位)），只有当超过这个时间才停止记录 Log。|
|filter|false|是否过滤 Log，取值 true| false|
|include||过滤 Log 时所包含的表名列表，以逗号分隔。|
|exclude||过滤 Log 时所排除的表名列表，以逗号分隔。|
|sqlexpression||过滤 Log 时的 SQL 表达式名称|
|autoflush|true|是否自动刷新。取值 true| false|
|dateformat||设置时间的格式，也就是用 Java 的 SimpleDateFormat 程序。|
|includecategories||显示指定过滤 Log 时包含的分类列表，取值为 error，info，batch，debug，statement，commit，rollback，result 的各种组合。|
|excludecategories||显示指定过滤 Log 时排队的分类列表，取值同上。|
|stringmatcher||应用正则表达式来过滤 Log，取值为 com.p6spy.engine.common.GnuRegexMatcher 和 com.p6spy.engine.common.JakartaRegexMatcher|
|stacktrace|false|打印堆栈跟踪信息。取值 true| false|
|stacktraceclass||如果 stacktrace=true，则可以指定具体的类名来停止过滤。|
|reloadproperties|false|监测属性配置文件是否停止重新加载。取值 true| false|
|reloadpropertiesinterval|60|属性配置文件重新加载的时间间隔，以秒为单位。|
|useprefix|false|是否加上前缀，设置为 true，会加上 p6spy: 作为前缀。取值 true| false|
|appender|com.p6spy.engine.logging. appender.FileLogger|指定 Log 的 appender，与 Log4J 有点同义，取值：com.p6spy.engine.logging.appender.Log4jLogger、com.p6spy.engine.logging.appender.StdoutLogger 和 com.p6spy.engine.logging.appender.FileLogger|
|logfile|spy.log|指定 Log 的文件名，任何适于操作系统的文件。|
|append|true|指定是否每次是增加 Log，设置为 false 则每次都会先停止清空。取值 true| false|
|log4j.appender.STDOUT|org.apache.log4j.ConsoleAppender|当 appender 为 log4j 时采用的配置，配置如同 Log4J 的相关配置。|
|log4j.appender.STDOUT.layout|org.apache.log4j.PatternLayout|同上|
|log4j.appender.STDOUT. layout.ConversionPattern|p6spy - %m%n|同上|
|log4j.logger.p6spy|INFO,STDOUT|Log 级别的设置，取值同 Log4J 的配置|
|realdatasource||设置数据源 DataSource 的配置名称。|
|realdatasourceclass||设置数据源 DataSource 的类的全称。|
|realdatasourceproperties||设置数据源 DataSource 的属性，以分号分隔。|
|jndicontextfactory||设置 JNDI 数据源的 NamingContextFactory。|
|jndicontextproviderurl||设置 JNDI 数据源的提供者的 URL。|
|jndicontextcustom||设置 JNDI 数据源的一些定制信息，以分号分隔。|
文章结束给大家分享下程序员的一些笑话语录： 据说有一位软件工程师，一位硬件工程师和一位项目经理同坐车参加研讨会。不幸在从盘山公路下山时坏在半路上了。于是两位工程师和一位经理就如何修车的问题展开了讨论。
硬件工程师说：“我可以用随身携带的瑞士军刀把车坏的部分拆下来，找出原因，排除故障。”
项目经理说：“根据经营管理学，应该召开会议，根据问题现状写出需求报告，制订计划，编写日程安排，逐步逼近，alpha测试，beta1测试和beta2测试解决问题。”
软件工程说：“咱们还是应该把车推回山顶再开下来，看看问题是否重复发生。”
--------------------------------- 原创文章 By 
配置和sql
---------------------------------
