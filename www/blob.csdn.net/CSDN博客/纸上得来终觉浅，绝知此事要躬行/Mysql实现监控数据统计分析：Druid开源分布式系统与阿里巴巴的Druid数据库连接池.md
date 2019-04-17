# Mysql实现监控数据统计分析：Druid开源分布式系统与阿里巴巴的Druid数据库连接池 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月24日 16:17:30[boonya](https://me.csdn.net/boonya)阅读数：3870








看到Druid很多国内的开发者所熟知的就是阿里巴巴提供的开源数据库连接池工具。如果看了这篇文章还认为Druid仅仅是一个阿里巴巴提供的开源数据库连接池工具那么就错了，它或许指的是用于大数据实时处理的开源分布式系统Druid。Durid有个统一的本质都是分析监控数据，这是本篇文章的关注点。

# Druid一个用于大数据实时处理的开源分布式系统

官方网址：[http://druid.io/](http://druid.io/)




[Druid](http://druid.io/)是一个用于大数据实时查询和分析的高容错、高性能开源分布式系统，旨在快速处理大规模的数据，并能够实现快速查询和分析。尤其是当发生代码部署、机器故障以及其他产品系统遇到宕机等情况时，Druid仍能够保持100%正常运行。创建Druid的最初意图主要是为了解决查询延迟问题，当时试图使用Hadoop来实现交互式查询分析，但是很难满足实时分析的需要。而Druid提供了以交互方式访问数据的能力，并权衡了查询的灵活性和性能而采取了特殊的存储格式。

Druid功能介于[PowerDrill](http://www.vldb.org/pvldb/vol5/p1436_alexanderhall_vldb2012.pdf)和[Dremel](http://research.google.com/pubs/pub36632.html)之间，它几乎实现了Dremel的所有功能，并且从PowerDrill吸收一些有趣的数据格式。Druid允许以类似Dremel和PowerDrill的方式进行单表查询，同时还增加了一些新特性，如为局部嵌套数据结构提供列式存储格式、为快速过滤做索引、实时摄取和查询、高容错的分布式体系架构等。

## Druid的主要特征

从官方得知，Druid的具有以下主要特征：
- **为分析而设计**——Druid是为[OLAP](http://en.wikipedia.org/wiki/Online_analytical_processing)工作流的探索性分析而构建，它支持各种过滤、聚合和查询等类；
- **快速的交互式查询**——Druid的低延迟数据摄取架构允许事件在它们创建后毫秒内可被查询到；
- **高可用性**——Druid的数据在系统更新时依然可用，规模的扩大和缩小都不会造成数据丢失；
- **可扩展**——Druid已实现每天能够处理数十亿事件和TB级数据。

## 监控分析解决方案

Druid应用最多的是类似于广告分析创业公司[Metamarkets](http://metamarkets.com/)中的应用场景，如广告分析、互联网广告系统监控以及网络监控等。当业务中出现以下情况时，Druid是一个很好的技术方案选择：
- 需要交互式聚合和快速探究大量数据时；
- 需要实时查询分析时；
- 具有大量数据时，如每天数亿事件的新增、每天数10T数据的增加；
- 对数据尤其是大数据进行实时分析时；
- 需要一个高可用、高容错、高性能数据库时。

一个Druid集群有各种类型的节点（Node）组成，每个节点都可以很好的处理一些的事情，这些节点包括对非实时数据进行处理存储和查询的[Historical节点](http://druid.io/docs/latest/Historical.html)、实时摄取数据、监听输入数据流的[Realtime节](http://druid.io/docs/latest/Realtime.html)、监控Historical节点的[Coordinator节点](http://druid.io/docs/latest/Coordinator.html)、接收来自外部客户端的查询和将查询转发到Realtime和Historical节点的[Broker节点](http://druid.io/docs/latest/Broker.html)、负责索引服务的[Indexer节点](http://druid.io/docs/latest/Indexing-Service.html)。

查询操作中数据流和各个节点的关系如下图所示：

![](http://cdn4.infoqstatic.com/statics_s2_20170323-0336/resource/news/2015/04/druid-data/zh/resources/11.png)

如下图是Druid集群的管理层架构，该图展示了相关节点和集群管理所依赖的其他组件（如负责服务发现的[ZooKeeper集群](http://druid.io/docs/latest/ZooKeeper.html)）的关系：

![](http://cdn4.infoqstatic.com/statics_s2_20170323-0336/resource/news/2015/04/druid-data/zh/resources/22.png)

Druid已基于[Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)协议开源，代码托管在[GitHub](https://github.com/druid-io/druid)，其当前最新稳定版本是[0.7.1.1](https://github.com/druid-io/druid/releases)。当前，Druid已有63个代码贡献者和将近2000个关注。Druid的主要贡献者包括广告分析创业公司Metamarkets、电影流媒体网站[Netflix](https://www.netflix.com/global)、Yahoo等公司。Druid官方还对Druid同[Shark](http://druid.io/docs/latest/Druid-vs-Impala-or-Shark.html)、[Vertica](http://druid.io/docs/latest/Druid-vs-Vertica.html)、[Cassandra](http://druid.io/docs/latest/Druid-vs-Cassandra.html)、[Hadoop](http://druid.io/docs/latest/Druid-vs-Hadoop.html)、[Spark](http://druid.io/docs/latest/Druid-vs-Spark.html)、[Elasticsearch](http://druid.io/docs/latest/Druid-vs-Elasticsearch.html)等在容错能力、灵活性、查询性能等方便进行了对比说明。更多关于Druid的信息，大家还可以参考官方提供的[入门教程](http://druid.io/docs/latest/Tutorial%3A-A-First-Look-at-Druid.html)、[白皮书](http://static.druid.io/docs/druid.pdf)
 、[设计文档](http://druid.io/docs/latest/Design.html)等。

以上文字来自infoQ：[http://www.infoq.com/cn/news/2015/04/druid-data/](http://www.infoq.com/cn/news/2015/04/druid-data/)

## 13个数字化分析

![](https://img-blog.csdn.net/20170324160331626)

![](https://img-blog.csdn.net/20170324160404939)

![](https://img-blog.csdn.net/20170324160414004)

![](https://img-blog.csdn.net/20170324160424410)

![](https://img-blog.csdn.net/20170324160434285)

![](https://img-blog.csdn.net/20170324160446659)

![](https://img-blog.csdn.net/20170324160503755)

![](https://img-blog.csdn.net/20170324160516472)

![](https://img-blog.csdn.net/20170324160529894)

![](https://img-blog.csdn.net/20170324160540645)

![](https://img-blog.csdn.net/20170324160552240)


# 阿里巴巴的Druid数据库连接池

项目Github地址：[https://github.com/alibaba/druid](https://github.com/alibaba/druid)


实践参考地址：[https://github.com/alibaba/druid/wiki/%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98](https://github.com/alibaba/druid/wiki/%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98)



## 1. Druid是什么？



Druid是Java语言中最好的数据库连接池。Druid能够提供强大的监控和扩展功能。

## 2. 在哪里下载druid
- 正式版本下载：

maven中央仓库: [http://central.maven.org/maven2/com/alibaba/druid/](http://central.maven.org/maven2/com/alibaba/druid/)


## 3. 怎么获取Druid的源码

Druid是一个开源项目，源码托管在github上，源代码仓库地址是 [https://github.com/alibaba/druid](https://github.com/alibaba/druid) 。同时每次Druid发布正式版本和快照的时候，都会把源码打包，你可以从上面的下载地址中找到相关版本的源码

## 4. 怎么配置maven

Druid 0.1.18 之后版本都发布到maven中央仓库中，所以你只需要在项目的pom.xml中加上dependency就可以了。例如：

```
<dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>druid</artifactId>
        <version>${druid-version}</version>
    </dependency>
```

也可以选择 Maven仓库查找公共的仓库地址: [http://www.mvnrepository.com/artifact/com.alibaba/druid](http://www.mvnrepository.com/artifact/com.alibaba/druid)

## 5. 怎么打开Druid的监控统计功能

Druid的监控统计功能是通过filter-chain扩展实现，如果你要打开监控统计功能，配置StatFilter，具体看这里：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatFilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatFilter)

## 6. 怎样使用Druid的内置监控页面

内置监控页面是一个Servlet，具体配置看这里：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatViewServlet%E9%85%8D%E7%BD%AE](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatViewServlet%E9%85%8D%E7%BD%AE)

## 7. 内置监控中的Web和Spring关联监控怎么配置？
- Web关联监控配置 
[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_%E9%85%8D%E7%BD%AEWebStatFilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_%E9%85%8D%E7%BD%AEWebStatFilter)
- Spring关联监控配置 
[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_Druid%E5%92%8CSpring%E5%85%B3%E8%81%94%E7%9B%91%E6%8E%A7%E9%85%8D%E7%BD%AE](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_Druid%E5%92%8CSpring%E5%85%B3%E8%81%94%E7%9B%91%E6%8E%A7%E9%85%8D%E7%BD%AE)

## 8. 怎么配置防御SQL注入攻击

Druid提供了WallFilter，它是基于SQL语义分析来实现防御SQL注入攻击的。具体配置看这里：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter)

## 9. Druid有没有参考配置

不同的业务场景需求不同，你可以使用我们的参考配置，但建议你仔细阅读相关文档，了解清楚之后做定制配置。[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_DruidDataSource%E5%8F%82%E8%80%83%E9%85%8D%E7%BD%AE](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_DruidDataSource%E5%8F%82%E8%80%83%E9%85%8D%E7%BD%AE)

## 10. 我想日志记录JDBC执行的SQL，如何配置

Druid提供了Log4jFilter、CommonsLogFilter和Slf4jFilter，具体配置看这里 [https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_LogFilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_LogFilter)

## 11. 我的程序可能产生连接泄漏了，有什么办法？

Druid提供了多种监测连接泄漏的手段，具体看这里：[https://github.com/alibaba/druid/wiki/%E8%BF%9E%E6%8E%A5%E6%B3%84%E6%BC%8F%E7%9B%91%E6%B5%8B](https://github.com/alibaba/druid/wiki/%E8%BF%9E%E6%8E%A5%E6%B3%84%E6%BC%8F%E7%9B%91%E6%B5%8B)

## 12. 在Druid中使用PSCache会有内存占用过大问题么？

连接Oracle数据库，打开PSCache，在其他的数据库连接池都会存在内存占用过多的问题，Druid是唯一解决这个问题的连接池。具体看这里：[https://github.com/alibaba/druid/wiki/Oracle%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%8BPreparedStatementCache%E5%86%85%E5%AD%98%E9%97%AE%E9%A2%98%E8%A7%A3%E5%86%B3%E6%96%B9%E6%A1%88](https://github.com/alibaba/druid/wiki/Oracle%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%8BPreparedStatementCache%E5%86%85%E5%AD%98%E9%97%AE%E9%A2%98%E8%A7%A3%E5%86%B3%E6%96%B9%E6%A1%88)

## 13. 有没有和其他数据库连接池的对比？
- 各种数据库连接池对比 [https://github.com/alibaba/druid/wiki/%E5%90%84%E7%A7%8D%E6%95%B0%E6%8D%AE%E5%BA%93%E8%BF%9E%E6%8E%A5%E6%B1%A0%E5%AF%B9%E6%AF%94](https://github.com/alibaba/druid/wiki/%E5%90%84%E7%A7%8D%E6%95%B0%E6%8D%AE%E5%BA%93%E8%BF%9E%E6%8E%A5%E6%B1%A0%E5%AF%B9%E6%AF%94)

## 14. 从其他连接池迁移要注意什么？
- 不同连接池的参数参照对比：[http://code.alibabatech.com/wiki/pages/viewpage.action?pageId=6947005](http://code.alibabatech.com/wiki/pages/viewpage.action?pageId=6947005)
- DBCP迁移 [https://github.com/alibaba/druid/wiki/DBCP%E8%BF%81%E7%A7%BB](https://github.com/alibaba/druid/wiki/DBCP%E8%BF%81%E7%A7%BB)

## 15. Druid中有没有类似Jboss DataSource中的ExceptionSorter

ExceptionSorter是JBoss DataSource中的优秀特性，Druid也有一样功能的ExceptionSorter，但不用手动配置，自动识别生效的。具体看这里：[https://github.com/alibaba/druid/wiki/ExceptionSorter_cn](https://github.com/alibaba/druid/wiki/ExceptionSorter_cn)

## 16. Druid中的maxIdle为什么是没用的？

maxIdle是Druid为了方便DBCP用户迁移而增加的，maxIdle是一个混乱的概念。连接池只应该有maxPoolSize和minPoolSize，druid只保留了maxActive和minIdle，分别相当于maxPoolSize和minPoolSize。

## 17. 我的应用配置的是JNDI数据源，可以用DruidDataSource么？

DruidDataSource支持JNDI配置，具体看这里：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_JNDI_Tomcat](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_JNDI_Tomcat)

具体实现的类是这个：com.alibaba.druid.pool.DruidDataSourceFactory，你可以阅读代码加深理解。

## 18. 我的应用已使用DBCP，是代码中写死的，怎样更换为Druid？

可以的，Druid提供了一个中完全平滑迁移DBCP的办法。
- 1) 从[http://repo1.maven.org/maven2/com/alibaba/druid/druid-wrapper/](http://repo1.maven.org/maven2/com/alibaba/druid/druid-wrapper/) 下载druid-wrapper-xxx.jar

- 2) 加入druid-xxx.jar 

- 3) 从你的WEB-INF/lib/中删除dbcp-xxx.jar 

- 4) 按需要加上配置，比如JVM启动参数加上-Ddruid.filters=stat，动态配置druid的filters 


这种用法，使得可以在一些非自己开发的应用中使用Druid，例如在sonar中部署druid，sonar是一个使用jruby开发的web应用，写死了DBCP，只能够通过这种方法来更换。

## 19. 我想试用快照版本，怎么获取？

直接获取快照版本的地址是：[http://code.alibabatech.com/mvn/snapshots/com/alibaba/druid/](http://code.alibabatech.com/mvn/snapshots/com/alibaba/druid/) ，使用快照版本建议加入我们QQ群 92748305，遇到问题直接反馈给我们。

## 20. 有一些SQL执行很慢，我希望日志记录下来，怎么设置？

在StatFilter配置中有慢SQL执行日志记录，看这里 [https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatFilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_StatFilter)

## 21. 我希望加密我的数据库密码怎么办？

运维和DBA都不希望把密码明文直接写在配置文件中，Druid提供了数据库秘密加密的功能。具体看这里：[https://github.com/alibaba/druid/wiki/%E4%BD%BF%E7%94%A8ConfigFilter](https://github.com/alibaba/druid/wiki/%E4%BD%BF%E7%94%A8ConfigFilter)

## 22. 如何参与Druid的开发

Druid是一个通过github开源的项目，github的特性，使得你很容易参与其中。这里有详细说明 [https://github.com/alibaba/druid/wiki/%E5%A6%82%E4%BD%95%E5%8F%82%E4%B8%8E](https://github.com/alibaba/druid/wiki/%E5%A6%82%E4%BD%95%E5%8F%82%E4%B8%8E)

## 23. Druid的发布周期是怎样？

Druid是一个活跃的项目，长期维护。每个月有一个发布窗口，除非遇到重大bug和非常紧急的需求，否则都是每个月最多发布一次。如果没有足够多的需求，发布窗口就不会被使用。

## 24. 如果DruidDataSource在init的时候失败了，不再使用，是否需要close

是的，如果DruidDataSource不再使用，必须调用close来释放资源，释放的资源包括关闭Create和Destory线程。

## 25. DruidDataSource支持哪些数据库？

理论上说，支持所有有jdbc驱动的数据库。实际测试过的有
|数据库|支持状态|
|----|----|
|mysql|支持，大规模使用|
|oracle|支持，大规模使用|
|sqlserver|支持|
|postgres|支持|
|db2|支持|
|h2|支持|
|derby|支持|
|sqlite|支持|
|sybase|支持|

## 26. Oracle下jdbc executeBatch时，更新行数计算不正确

使用jdbc的executeBatch 方法，如果数据库为oracle，则无论是否成功更新到数据，返回值都是-2，而不是真正被sql更新到的记录数，这是Oracle JDBC Driver的问题，Druid不作特殊处理。

## 27. Druid如何自动根据URL自动识别DriverClass的

Druid是根据url前缀来识别DriverClass的，这样使得配置更方便简洁。
|前缀|DriverCLass|描述信息|
|----|----|----|
|jdbc:odps|com.aliyun.odps.jdbc.OdpsDriver||
|jdbc:derby|org.apache.derby.jdbc.EmbeddedDriver||
|jdbc:mysql|com.mysql.jdbc.Driver||
|jdbc:oracle|oracle.jdbc.driver.OracleDriver||
|jdbc:microsoft|com.microsoft.jdbc.sqlserver.SQLServerDriver||
|jdbc:sybase:Tds|com.sybase.jdbc2.jdbc.SybDriver||
|jdbc:jtds|net.sourceforge.jtds.jdbc.Driver||
|jdbc:postgresql|org.postgresql.Driver||
|jdbc:fake|com.alibaba.druid.mock.MockDriver||
|jdbc:mock|com.alibaba.druid.mock.MockDriver||
|jdbc:hsqldb|org.hsqldb.jdbcDriver||
|jdbc:db2|COM.ibm.db2.jdbc.app.DB2Driver|DB2的JDBC Driver十分混乱，这个匹配不一定对|
|jdbc:sqlite|org.sqlite.JDBC||
|jdbc:ingres|com.ingres.jdbc.IngresDriver||
|jdbc:h2|org.h2.Driver||
|jdbc:mckoi|com.mckoi.JDBCDriver||
|jdbc:cloudscape|COM.cloudscape.core.JDBCDriver||
|jdbc:informix-sqli|com.informix.jdbc.IfxDriver||
|jdbc:timesten|com.timesten.jdbc.TimesTenDriver||
|jdbc:as400|com.ibm.as400.access.AS400JDBCDriver||
|jdbc:sapdb|com.sap.dbtech.jdbc.DriverSapDB||
|jdbc:JSQLConnect|com.jnetdirect.jsql.JSQLDriver||
|jdbc:JTurbo|com.newatlanta.jturbo.driver.Driver||
|jdbc:firebirdsql|org.firebirdsql.jdbc.FBDriver||
|jdbc:interbase|interbase.interclient.Driver||
|jdbc:pointbase|com.pointbase.jdbc.jdbcUniversalDriver||
|jdbc:edbc|ca.edbc.jdbc.EdbcDriver||
|jdbc:mimer:multi1|com.mimer.jdbc.Driver||

## 28. 如何保存监控记录

[https://github.com/alibaba/druid/wiki/%E6%80%8E%E4%B9%88%E4%BF%9D%E5%AD%98Druid%E7%9A%84%E7%9B%91%E6%8E%A7%E8%AE%B0%E5%BD%95](https://github.com/alibaba/druid/wiki/%E6%80%8E%E4%B9%88%E4%BF%9D%E5%AD%98Druid%E7%9A%84%E7%9B%91%E6%8E%A7%E8%AE%B0%E5%BD%95)

## 29. 我想Log输出SQL执行的信息怎么办？

[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_LogFilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE_LogFilter)

## 30. 如何配置Druid内置的log实现

[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AEdruid%E5%86%85%E7%BD%AE%E7%9A%84log%E5%AE%9E%E7%8E%B0](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AEdruid%E5%86%85%E7%BD%AE%E7%9A%84log%E5%AE%9E%E7%8E%B0)

# 阿里巴巴Druid数据库连接池实践

## SpringApplicationContext.xml配置

SpringApplicationContext.xml




```
<!-- Druid 配置 start -->
	<bean id="druid-stat-interceptor"
		class="com.alibaba.druid.support.spring.stat.DruidStatInterceptor">
	</bean>

	<bean id="druid-stat-pointcut" class="org.springframework.aop.support.JdkRegexpMethodPointcut"
		scope="prototype">
		<property name="patterns">
			<list>
				<value>com.wlyd.fmcgwms.service.*</value>
			</list>
		</property>
	</bean>

	<aop:config>
		<aop:advisor advice-ref="druid-stat-interceptor"
			pointcut-ref="druid-stat-pointcut" />
	</aop:config>

	<bean
		class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
		<property name="locations" value="classpath*:jdbc.properties" />
	</bean>
	<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"
		init-method="init" destroy-method="close">
		<property name="driverClassName">
			<value>${jdbc.driverClass}</value>
		</property>
		<property name="url">
			<value>${jdbc.jdbcUrl}</value>
		</property>
		<property name="username">
			<value>${jdbc.user}</value>
		</property>
		<property name="password">
			<value>${jdbc.password}</value>
		</property>
		<!-- 连接池最大使用连接数 -->
		<property name="maxActive">
			<value>100</value>
		</property>
		<!-- 初始化连接大小 -->
		<property name="initialSize">
			<value>10</value>
		</property>
		<!-- 获取连接最大等待时间 -->
		<property name="maxWait">
			<value>60000</value>
		</property>
		<!-- 连接池最大空闲 -->
		<property name="maxIdle">
			<value>50</value>
		</property>
		<!-- 连接池最小空闲 -->
		<property name="minIdle">
			<value>10</value>
		</property>
		<!-- 自动清除无用连接 -->
		<property name="removeAbandoned">
			<value>true</value>
		</property>
		<!-- 清除无用连接的等待时间 -->
		<property name="removeAbandonedTimeout">
			<value>1800</value>
		</property>
		<!-- 连接属性 -->
		<property name="connectionProperties">
			<value>clientEncoding=UTF-8</value>
		</property>
		<!-- 配置监控统计拦截的filters -->
		<property name="filters" value="wall,stat" />
	</bean>
	<!-- Druid 配置 end -->
```


## 项目web.xml配置

web.xml



```
<!-- DruidStatView start-->
	<servlet>
		<servlet-name>DruidStatView</servlet-name>
		<servlet-class>com.alibaba.druid.support.http.StatViewServlet</servlet-class>
		<!-- 允许访问的IP地址 -->
		<!-- <init-param>
			<param-name>allow</param-name>
			<param-value>192.168.28.94,192.168.28.100</param-value>
		</init-param> -->
		<!-- 拒绝访问的IP地址 -->
		<!-- <init-param>
			<param-name>deny</param-name>
			<param-value>192.168.28.110</param-value>
		</init-param> -->
		<init-param>  
		    <!-- 允许清空统计数据 -->  
		    <param-name>resetEnable</param-name>  
		    <param-value>true</param-value>  
		    </init-param>  
		    <init-param>  
		    <!-- 用户名 -->  
		    <param-name>loginUsername</param-name>  
		    <param-value>druid</param-value>  
		    </init-param>  
		    <init-param>  
		    <!-- 密码 -->  
		    <param-name>loginPassword</param-name>  
		    <param-value>wlyd_druid</param-value>  
	    </init-param>  
	</servlet>
	<servlet-mapping>
		<servlet-name>DruidStatView</servlet-name>
		<url-pattern>/druid/*</url-pattern>
	</servlet-mapping>

	<filter>
		<filter-name>DruidWebStatFilter</filter-name>
		<filter-class>com.alibaba.druid.support.http.WebStatFilter</filter-class>
		<!-- 经常需要排除一些不必要的url，比如*.js,/jslib/*等等 -->
		<init-param>
			<param-name>exclusions</param-name>
			<param-value>*.js,*.gif,*.jpg,*.png,*.css,*.ico,/druid/*</param-value>
		</init-param>
		<!-- 缺省sessionStatMaxCount是1000个。 -->
		<init-param>
			<param-name>sessionStatMaxCount</param-name>
			<param-value>1000</param-value>
		</init-param>
		<!-- 你可以关闭session统计功能 -->
		<init-param>
			<param-name>sessionStatEnable</param-name>
			<param-value>true</param-value>
		</init-param>
		<!-- 配置principalSessionName，使得druid能够知道当前的session的用户是谁 -->
		<init-param>
			<param-name>principalSessionName</param-name>
			<param-value>sessionBackUser</param-value>
		</init-param>
		<!-- 如果你的user信息保存在cookie中，你可以配置principalCookieName，使得druid知道当前的user是谁 -->
		<init-param>
			<param-name>principalCookieName</param-name>
			<param-value>username</param-value>
		</init-param>
		<!-- druid 0.2.7版本开始支持profile，配置profileEnable能够监控单个url调用的sql列表 -->
		<init-param>
			<param-name>profileEnable</param-name>
			<param-value>true</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>DruidWebStatFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<!-- DruidStatView end-->
```


## 部分操作界面展示



![](https://img-blog.csdn.net/20170324161243242)

![](https://img-blog.csdn.net/20170324161254279)

![](https://img-blog.csdn.net/20170324161306789)

![](https://img-blog.csdn.net/20170324161325680)

注：对Druid线程池的配置需要慢慢研究之，配置适合自身的连接参数。










