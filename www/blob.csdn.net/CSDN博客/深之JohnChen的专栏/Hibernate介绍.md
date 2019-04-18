# Hibernate介绍 - 深之JohnChen的专栏 - CSDN博客

2017年08月12日 11:23:58[byxdaz](https://me.csdn.net/byxdaz)阅读数：338
个人分类：[hibernate](https://blog.csdn.net/byxdaz/article/category/6892926)



# 一Hibernate介绍

## 什么是Hibernate？

持久化（Persistence），即把数据（如内存中的对象）保存到可永久保存的存储设备中（如磁盘）。持久化的主要应用是将内存中的对象存储在关系型的数据库中，当然也可以存储在磁盘文件中、XML数据文件中等等。

1、Hibernate是对JDBC进一步封装

       原来没有使用Hiberante做持久层开发时，存在很多冗余，如：各种JDBC语句，connection的管理，所以出现了Hibernate把JDBC封装了一下，我们不用操作数据，直接操作它就行了。

2、我们再从分层的角度来看

       我们知道非常典型的三层架构：表示层，业务层，还有持久层。Hiberante也是持久层的框架，而且持久层的框架还有很多，比如：IBatis，Nhibernate，JDO，OJB，EJB等等。

3、Hibernate是开源的一个ORM（对象关系映射）框架。

      ORM，即Object-Relational Mapping，它的作用就是在关系型数据库和对象之间做了一个映射。从对象（Object）映射到关系（Relation），再从关系映射到对象。这样，我们在操作数据库的时候，不需要再去和复杂SQL打交道，只要像操作对象一样操作它就可以了（把关系数据库的字段在内存中映射成对象的属性）。

Hibernate的核心：

     Hibernate六大核心接口，两个主要配置文件，以及他们直接的关系。

1、Configuration接口:负责配置并启动Hibernate，加载配置文件(hibernate.cfg.xml)，配置文件中包含了多个映射文件(xxx.hbm.xml)。

2、SessionFactory接口:负责初始化Hibernate。

3、Session接口:负责持久化对象的CRUD操作。

4、Transaction接口:负责事务。

5、Query接口和Criteria接口:负责执行各种数据库查询。Criteria查询（Criteria Query），这种查询方式把查询条件封装为一个Criteria对象。在实际应用中，使用Session的createCriteria()方法构建一个org.hibernate.Criteria实例，然后把具体的查询条件通过Criteria的add()方法加入到Criteria实例中。这样，程序员可以不使用SQL甚至HQL的情况下进行数据查询。

注意：Configuration实例是一个启动期间的对象，一旦SessionFactory创建完成它就被丢弃了。

Hibernate的优/缺点：

优点：

1、更加对象化

      以对象化的思维操作数据库，我们只需要操作对象就可以了，开发更加对象化。

2、移植性
      因为Hibernate做了持久层的封装，你就不知道数据库，你写的所有的代码都具有可复用性。

3、Hibernate是一个没有侵入性的框架，没有侵入性的框架我们称为轻量级框架。

      对比Struts的Action和ActionForm，都需要继承，离不开Struts。Hibernate不需要继承任何类，不需要实现任何接口。这样的对象叫POJO对象。

4、Hibernate代码测试方便。

5、提高效率，提高生产力。

缺点：

1、使用数据库特性的语句，将很难调优

2、对大批量数据更新存在问题

3、系统中存在大量的攻击查询功能

# 二Hibernate开发环境搭建

## 1、开发环境

 jdk1.7 + MyEclipse/Eclipse + MySQL

说明：其实Hibernate是非常独立的框架，根本不需要MyEclipse，Eclipse，Tomcat，Log4J等，他们只不过是能满足我们其他的需求，才把他们引进来的。

## 2、下载文件

你需要[Java](http://lib.csdn.net/base/java)SDK、 Hibernate包、和JDBC Driver。

1、Hibernate包下载地址:
[https://sourceforge.net/projects/hibernate/files/](https://sourceforge.net/projects/hibernate/files/)
2、JDBC Driver根据你的数据库来定，一般database官网都有。Hibernate支持常用的数据库，比如 MySQL， Oracle等等。这两个数据库是现在比较常用的，都有JDBC Driver：
Oracle JDBC Driver下载地址(下载前必须同意Oracle协议书)
[http://otn.oracle.com/software/htdocs/distlic.html?/software/tech/java/sqlj_jdbc/htdocs/jdbc9201.html](http://otn.oracle.com/software/htdocs/distlic.html?/software/tech/java/sqlj_jdbc/htdocs/jdbc9201.html)
MySQL JDBC Driver下载地址
[http://dev.mysql.com/downloads/connector/j/3.0.html](http://dev.mysql.com/downloads/connector/j/3.0.html)

## 三、所需jar包

**hibernate核心包：hibernate-distribution-3.5.1-Final（8个）**

cglib-2.2.jar

hibernate\lib\bytecode\cglib\cglib-2.2.jar

cglib库，hibernate用它来实现PO字节码的动态生成，非常核心的库，必须使用的jar包

antlr-2.7.6.jar

Antlr（ANother Tool for Language Recognition）是一个工具，它为我们构造自己的识别器（recognizers）、编译器（compiler）和转换器（translators）提供了一个基础。通过定义自己的语言规则，Antlr可以为我们生成相应的语言解析器，这样便可以省却了自己全手工打造的劳苦。

commons-collections-3.1.jar

dom4j-1.6.1.jar

javassist-3.9.0.GA.jar

jta-1.1.jar

slf4j-api-1.5.8.jar

hibernate\lib\required\*

**hibernate注解包：hibernate-annotations-3.4.0.GA（3个）**

hibernate-annotations.jar

ejb3-persistence.jar

hibernate-commons-annotations.jar

**hibernate针对JPA的实现包：hibernate-entitymanager-3.4.0.GA（3个）**

hibernate-entitymanager.jar

log4j.jar

slf4j-log4j12.jar

hibernate 所需jar包及简介
|Hibernate3.jar|这个是hibernate最主要的jar包|
|----|----|
|ant-1.63.jar|Ant 的核心包,在构建Hibernate  时会用到|
|antlr-2.7.5H3.jar|语言转换工,Hibernate利用它实现 HQL 到 SQL的转换|
|asm.jar/asm-attrs.jar|ASM 字节转换库|
|c3p0-0.8.5.2.jar|C3PO JDBC 连接池工具|
|cglib-2.1.jar|接口高效的代码生成工具,  Hibernate用它在运行时扩展  Java类和实现 Java|
|commons-collections-2.1.1.jar|Apache 的工具集,用来增强Java对集合的处理能力|
|commons-logging-1.0.4.jar|Apache 软件基我组所提供的日志工具|
|concurrent-1.3.2.jar|线程同步工具,在使用JBoss 的树状缓存时需要用到|
|connector.jar|用连接多个应用服务器的标准连接器|
|dom4j-1.6.jar|dom4j XML 解析器|
|ehcache-1.1.jar|缓存工具.在没有提供其他缓存工具时,这个缓存工具是必不可少的|
|jaas.jar|标准的 Java 权限和认证服务包|
|jaxen-1.1-beta-4.jar|通用的 XPath 处理引擎|
|jboss-cache.jar|JBoss 的一种树状缓存实现工具|
|jboss-common.jar|Jboss 的基础包,在使用 JBoss 的树状缓存时必须有此包|
|jboss-jmx.jar|JBoss 的 JMX 实现包|
|jboss-system.jar|JBoss 的核心,包括服务器和部署引擎|
|jdbc2_0-stdext.jar|标准的 JDBC 2.0  扩展API|
|jgroups2.2.7.jar|网络通信工具包|
|jta.jar|标准的 JAVA 事务处理接口|
|junit-3.8.1.jar|JUnit 测试框架|
|log4.j-1.2.9.jar|log4j 库,Apache 的日志工具|
|oscache-2.1.jar|Java 对象的缓存工具|
|proxool-0.8.3.jar|Proxool JDBC 连接池 工具|
|swarmcache-1.0rc2.jar|Hibernate 可以使用的一种缓存工具|
|versioncheck.jar|版本检查工具包,用于在构建 Hibernate 的时候检查辅助 jar 包文件的版本|
|xerces-2.6.2.jar|SAX XML 解析器|
|xml-apis.jar|Java语言中标准的XML 解析器|

Hibernate jar下载地址：

http://hibernate.org/orm/downloads/

https://sourceforge.net/projects/hibernate/files/hibernate-orm/

