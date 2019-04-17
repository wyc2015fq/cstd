# Jorm简介及其使用配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月01日 21:55:25[boonya](https://me.csdn.net/boonya)阅读数：837标签：[Jorm																[hibernate																[framework																[数据库连接池																[proxy](https://so.csdn.net/so/search/s.do?q=proxy&t=blog)
个人分类：[JORM](https://blog.csdn.net/boonya/article/category/1720951)





   说到Jorm，大家第一反应想到就是Hibernate的ORM,Jorm 的确与Hibernate有许多的相似之处，但是Jorm比它更加轻量。

**1、什么是JORM**

JORM（Java对象存储映射)Java开源的持久性框架。它为JOnAS J2EE应用服务器提供EJB 2.0 CMP。JORM还与[Speedo](http://speedo.objectweb.org/) JDO实现结合。

它也是JDBC ORM Framework ，与Hibernate非常相似，但是Hibernate内置太过复杂，相比而言Jorm更加轻量。

该项目主页:[http://jorm.objectweb.org/](http://jorm.objectweb.org/)

**所需开发环境：**

 1.JDK 1.5 及其以上版本

 2.如需要lazy加载，需要引入cglib或javaassit（这是两种不同的代理模式jar包），需要时需要做相应的配置。


**2、英文资源网站介绍**

from:[http://grepcode.com/snapshot/repo1.maven.org/maven2/net.sf.javaclub/gerald-jorm/1.0.8.RC1/](http://grepcode.com/snapshot/repo1.maven.org/maven2/net.sf.javaclub/gerald-jorm/1.0.8.RC1/)




This is a java orm framework [http://javaclub.sourceforge.net/jorm.html](http://javaclub.sourceforge.net/jorm.html)


Project metadata download: [gerald-jorm-1.0.8.RC1.pom](http://repo1.maven.org/maven2/net/sf/javaclub/gerald-jorm/1.0.8.RC1/gerald-jorm-1.0.8.RC1.pom)

Binary  download: [gerald-jorm-1.0.8.RC1.jar](http://repo1.maven.org/maven2/net/sf/javaclub/gerald-jorm/1.0.8.RC1/gerald-jorm-1.0.8.RC1.jar)

Source download: [gerald-jorm-1.0.8.RC1-sources.jar](http://repo1.maven.org/maven2/net/sf/javaclub/gerald-jorm/1.0.8.RC1/gerald-jorm-1.0.8.RC1-sources.jar)





Release date: 11 October 2011

License: [The Apache
 Software License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.txt)





Files: 153, 

Classes: 183, 

Lines of code: 21120


![](http://grepcode.com/static/app/images/1x1.gif)Google
 AppEngine: Partial support

Depends on:[(View
 as diagram)](http://grepcode.com/static/data/html/repo1.maven.org$maven2/net.sf.javaclub/gerald-jorm/1.0.8.RC1/visualization.svg)- 

Maven-Central / c3p0 / c3p0



[0.9.1.2](http://grepcode.com/snapshot/repo1.maven.org/maven2/c3p0/c3p0/0.9.1.2)


- 

Maven-Central / cglib / cglib-nodep



[2.2.2](http://grepcode.com/snapshot/repo1.maven.org/maven2/cglib/cglib-nodep/2.2.2)


- 

Maven-Central / com.h2database / h2



[1.3.158](http://grepcode.com/snapshot/repo1.maven.org/maven2/com.h2database/h2/1.3.158)


- 

Maven-Central / commons-dbcp / commons-dbcp



[1.3](http://grepcode.com/snapshot/repo1.maven.org/maven2/commons-dbcp/commons-dbcp/1.3)


- 

Maven-Central / commons-logging / commons-logging



[1.1.1](http://grepcode.com/snapshot/repo1.maven.org/maven2/commons-logging/commons-logging/1.1.1)


- 

Maven-Central / commons-pool / commons-pool



[1.5.4](http://grepcode.com/snapshot/repo1.maven.org/maven2/commons-pool/commons-pool/1.5.4)


- 

Maven-Central / javassist / javassist



[3.11.0.GA](http://grepcode.com/snapshot/repo1.maven.org/maven2/javassist/javassist/3.11.0.GA)


- 

Maven-Central / junit / junit



[4.8.2](http://grepcode.com/snapshot/repo1.maven.org/maven2/junit/junit/4.8.2)


- 

Maven-Central / log4j / log4j



[1.2.14](http://grepcode.com/snapshot/repo1.maven.org/maven2/log4j/log4j/1.2.14)


- 

Maven-Central / net.sourceforge.jtds / jtds



[1.2.4](http://grepcode.com/snapshot/repo1.maven.org/maven2/net.sourceforge.jtds/jtds/1.2.4)


- 

Maven-Central / org.slf4j / slf4j-api



[1.5.10](http://grepcode.com/snapshot/repo1.maven.org/maven2/org.slf4j/slf4j-api/1.5.10)


- 

Maven-Central / org.slf4j / slf4j-log4j12



[1.5.10](http://grepcode.com/snapshot/repo1.maven.org/maven2/org.slf4j/slf4j-log4j12/1.5.10)


- 

Maven-Central / org.springframework / spring



[2.5.6](http://grepcode.com/snapshot/repo1.maven.org/maven2/org.springframework/spring/2.5.6)


- 

Maven-Central / org.springframework / spring-test



[2.5.6](http://grepcode.com/snapshot/repo1.maven.org/maven2/org.springframework/spring-test/2.5.6)


- 

Maven-Central / postgresql / postgresql



[9.0-801.jdbc3](http://grepcode.com/snapshot/repo1.maven.org/maven2/postgresql/postgresql/9.0-801.jdbc3)


- 

JDK / jdk / openjdk



[6-b14](http://grepcode.com/snapshot/repository.grepcode.com/java/root/jdk/openjdk/6-b14)






MD5 Signatures:- 934c9e230078751e93f167b02beabbee
- b67ac7e34293de2d1e64e7068a53db6e




**3、Jorm的特性**

  1.支持多数据源管理和配置

  2.自动封装Entity

  3.支持事务

  4.支持存储过程的方便调用

  5.支持lazy加载

  6.支持分页查询

  7.支持多种数据库H2,MySQL,Oracle,PostgrSQL,SQLServer 等

  8.支持多种主流数据库连接池，如c3p0,proxool,dbcp,bonecp等

  9.自动管理数据库连接的生命周期


（在接下来几篇的博文中将一一介绍这些特性）

**4、在src下创建jorm.cfg.xml文件并做如下配置**



```
<?xml version='1.0' encoding="UTF-8"?>
<jdbc-configuration>

  <constant name="show_sql" value="true" />
  <constant name="jdbc.batch_size" value="600" />
  <constant name="bytecode.provider" value="cglib" />
 
  <connections default="simple">
 
   <!-- MYSQL数据源配置 -->
    <connection name="simple">
      <property name="connection.implementation">org.javaclub.jorm.jdbc.connection.impl.SimpleConnection</property>
      <property name="connection.dialect">MySQLDialect</property>
      <property name="connection.driver">com.mysql.jdbc.Driver</property>
      <property name="connection.jdbcurl">jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=UTF-8</property>
      <property name="connection.database">test</property>
      <property name="connection.username">root</property>
      <property name="connection.password">root</property>
    </connection>

    <!-- C3P0连接池配置 -->
    <connection name="c3p0">
      <property name="connection.implementation">org.javaclub.jorm.jdbc.connection.impl.PooledConnection</property>
      <property name="connection.dialect">MySQLDialect</property>
      <property name="connection.driver">com.mysql.jdbc.Driver</property>
      <property name="connection.jdbcurl">jdbc:mysql://127.0.0.1:3306/test?useUnicode=true&characterEncoding=UTF-8</property>
      <property name="connection.database">test</property>
      <property name="connection.username">root</property>
      <property name="connection.password">root</property>
      <property name="connection.pool.min">1</property>
      <property name="connection.pool.max">8</property>
      <property name="connection.test.sql">select 1</property>
    </connection>
    
  </connections>

</jdbc-configuration>
```
注：更多配置以后再深入研究。
**5、下载相应的jar包**


下载地址：[http://sourceforge.net/projects/javaclub/files/jorm/](http://sourceforge.net/projects/javaclub/files/jorm/)


将下载的jorm jar包放至项目的WEB-INF下的lib目录下，并将此jar包导入构建路径下。

注：早在2011年的时候JORM就已经停止更新了。](https://so.csdn.net/so/search/s.do?q=数据库连接池&t=blog)](https://so.csdn.net/so/search/s.do?q=framework&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=Jorm&t=blog)




