# log4j2日志配置实战全解 - 零度的博客专栏 - CSDN博客
2017年10月16日 10:13:37[零度anngle](https://me.csdn.net/zmx729618)阅读数：2922
1.目的
替换log4j,log4j2是log4j的2.x版本但是在log4j上做了比较大的改变，log4j2的性能比log4j好。该文不会具体讲解常用的基础配置，主要介绍日志发邮件、日志写mysql数据库、日志写Mongo功能。
2.实战
预设环境
- 
默认项目为maven管理
- 
使用jdk1.8及以上
- 
idea开发环境
- 
mongo
2.1 配置
添加依赖
![log4j2 日志配置实战](http://p1.pstatp.com/large/1db9000599304dde4d78)
![log4j2 日志配置实战](http://p9.pstatp.com/large/1dba0002b3a6db23ee68)
对以上依赖包的说明：
- 
slf4j和log4j*包是日志门面接口和实际日志发送者
- 
log4j-slf4j-impl是用于log4j2桥接slf4j，便于使用slf4j声明，log4j2输出日志。
- 
log4j-nosql是为了使得log4j2支持非关系型数据库（如Mongo，CouchDb）。
- 
log4j-web是为了在代码中使用Spring管理log4j2,通过Spring进行初始化，并应用Druid的数据源。
- 
mysql-connector-java：java操作mysql数据库的驱动。
- 
mail：发送邮件时需要。
- 
spring-data-mongodb： 是Spring对Mongo的集成，方便以Spring的方式操作Mongo。
- 
mongo-java-driver：是mongo的java驱动，便于java对mongo的调用
- 
fastjson：是将java类格式化为json格式，用于发送日志时将日志转为json。
这些只是当前讲解日志相关功能使用的包，而Spring的核心包以及其他如Mybatis等不在此文关心范围内。
2.2 log4j2.xml配置
log4j2.xml是作为log4j2使用时指定的文件名称之一，其他的还有(log4j.configurationFile,log4j2-test.properties,log4j2-test.yaml,log4j2-test.yml,log4j2-test.json,log4j2-test.jsn,log4j2-test.xml,log4j2.properties,log4j2.yaml,log4j2.yml,log4j2.json,log4j2.jsn,log4j2.xml,默认配置)，log42会以名称出现的顺序查找上面的文件依次是否存在，而log4j2.xml是优先级最低的，log4j.configurationFile是指用户自定义指定的文件名，优先级最高。
贴出所有的log4j2.xml代码，但不会全部介绍
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db900059ab20d6d4fab)
![log4j2 日志配置实战](http://p1.pstatp.com/large/1db7000295e3bc2eafce)
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db70002961fd8f5a911)
具体配置的各个属性名称不做具体解释了，自解释了。
2.3 log4j2配置写mysql数据库功能
节选部分配置
![log4j2 日志配置实战](http://p3.pstatp.com/large/1dba0002b5931c2f4b7d)
- 
tableName是数据库中的表名;
- 
ConnectionFactory需要自定义数据库连接工厂类，并提供获取DataSource或者Connection的静态方法。
- 
Column的name属性是指定数据库表中的字段名，pattern是匹配日志的内容格式，会将对应的内容写入到指定的字段下。
2.4 编写LoggerConnectionFactory类实现逻辑
LoggerConnectionFactory
类的代码如下：
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db7000296cddca7d9dd)
这样的方式是可以获取到Mysql数据库的Connection并且也可以将日志写入到数据库中，但是很明显这样有两个很大的缺点：
- 
自己控制对数据库的链接，没有应用业界比较好的数据源库也没有使用线程池。
- 
没有应用到Spring的IOC和DI。
2.5 Log4j2设置解惑
有人会问：
对于连接属性的配置直接调用Spring管理的数据源不行吗？答案是现在这样的配置不行，因为log4j2等日志是在Spring初始化前加载并初始化的，因此log4j2在当前配置下是拿不到Spring的bean信息的。
那这个问题就无解了吗？当然不是，要不然干嘛要引入log4j-web包呢。引入该包就是为了在web应用中使用Log4j2的，具体可以参考【logging.apache.org/log4j/2.x/manual/webapp.html】
要说明的是：Log4j2仅支持Servlet3.0及以上版本，Tomcat7.0及以上
Log4j2会在WEB容器启动和销毁时自动启动和关闭。是通过Log4jServletContainerInitializer(继承自ServletContainerInitializer)达到自动启动的目的。在Tomcat7.0.43之前，基于性能原因，web容器会忽略名为log4j*.jar包，阻止其获取随web容器启动的特性。但在之后的版本中已经修复。
如果禁止Log4j2的自动初始化，那么需要在web.xml中加入配置:
![log4j2 日志配置实战](http://p3.pstatp.com/large/1dba0002b6ba3efa5cfe)
但是在Servlet2.5需要在其他应用代码执行前初始化Log4j。
如果使用的是Servlet2.5，那么需要在web.xml文件中加入配置：
![log4j2 日志配置实战](http://p1.pstatp.com/large/1dbd00028ff200ac35e9)
详情请看【logging.apache.org/log4j/2.x/manual/webapp.html】,接下来就说明下怎样通过在Log4j2中使用Spring管理的bean。
2.6 重新配置Log4j2支持JDBC
这次配置是基于Spring的，使用了Spring容器的功能，对配置文件中Log4j2的配置进行增加。
代码如下：
![log4j2 日志配置实战](http://p1.pstatp.com/large/1dba0002b74c304b8322)
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db700029863ab3247b4)
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db70002989d59875bcd)
以上代码中的注释已经比较详细了，概括来说就是在当前LoggerBean被IOC实例化之后获取到已经从配置文件中读取的Log4j2的配置，并在init()方法中用代码将JDBC部分的逻辑补充上，更新下Log4j2的上下文即可。一个类内就把Log4j2的JDBC功能实现了，不需要在任何地方做log4j2对JBDC支持的配置。
2.7 Log4j2配置发邮件功能
log4j2.xml中SMTP内的配置就是控制日志发送邮件的功能了，默认使用的Lauout是HtmlLayout,对应类org.apache.logging.log4j.core.layout.HtmlLayout,比较蛋疼的是该类是final型的，不能被继承也就是说不能通过继承该类实现自定义Html布局。唉没办法，使用自定义的Layout吧！自定义的layout需要继承org.apache.logging.log4j.core.layout.AbstractStringLayout，并且要在类上加上注解:
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db7000298d1074f3d79)
其中name处value是配置文件log4j2.xml中使用的标签，如HtmlLayout ,写者没有对其原理进行分析，直接强行复制其代码新建了个类，并修改了关键的位置@Plugin的name作为自己的布局模板。
自定义的布局模板关键代码如下：
![log4j2 日志配置实战](http://p3.pstatp.com/large/1db800040dff14e5eaaf)
通过将上面解释的逻辑调整为自己想要的就实现了自定义布局。
2.8 Log4j2支持Mongo写日志
此处不涉及Mongo的安装和使用
对于NoSql的配置就比较简单了，以Mongo为例：
![log4j2 日志配置实战](http://p3.pstatp.com/large/1dbc0002b7d4edad5936)
相信Mongo配置的属性不用解释也能看明白了，就这点配置，只要能连上Mongo就可以将日志写入到Mongo中，当然前提是要在Logger或者Root标签中引用noSqlDbAppender。
Mongo存储结构如下：
![log4j2 日志配置实战](http://p3.pstatp.com/large/1dbc0002b808fd6735c2)
3 注意事项
log4j2 中配置Async标签异步存储日志会使得写Mysql库和Mongo库时，调用Logger的ClassName和MethodName都为空，写者当前在项目中暂时没有使用异步写日志操作。
