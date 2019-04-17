# Github Mybatis深入学习之日志 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月11日 12:33:42[boonya](https://me.csdn.net/boonya)阅读数：2745








       日志（Logging）它可以记录跟踪系统错误，是系统维护不可缺少的部分。原文地址：[http://mybatis.github.io/mybatis-3/logging.html](http://mybatis.github.io/mybatis-3/logging.html)

 MyBatis提供的日志信息通过使用内部记录工厂，内部的日志工厂将日志信息委托给以下日志实现：


SLF4JApache Commons LoggingLog4j 2Log4jJDK logging


       选择的日志解决方案，是基于运行时内省MyBatis的日志取决于内部工厂。 MyBatis的日志工厂将使用第一个发现日志实现（实现上述顺序搜索） 。如果MyBatis的发现没有上述的实现，然后记录将被禁用。 许多环境运输下议院记录作为应用服务器类路径（很好的例子包括Tomcat和WebSphere的）的一部分。重要的是要知道，在这样的环境中， MyBatis将会使用共享日志记录，日志记录实现。如WebSphere的环境中，这将意味着你的Log4J配置将被忽略，因为WebSphere提供自己的专有实施下议院记录。这可以是非常令人沮丧的，因为它会出现，MyBatis会忽略您的Log4J配置（其实，MyBatis会忽视您的Log4J配置，因为MyBatis将会在这样的环境中使用共享日志记录）
 。如果您的应用程序运行在下议院日志包含在类路径的环境下，但你宁愿使用的其他日志记录实现，你可以选择一个不同的日志记录实现的MyBatis -config.xml文件中添加一个设置如下：




```
<configuration>
  <settings>
    ...
    <setting name="logImpl" value="LOG4J"/>
    ...
  </settings>
</configuration>
```
有效值是SLF4J，log4j，LOG4J2，JDK_LOGGING，COMMONS_LOGGING，STDOUT_LOGGING，NO_LOGGING或完全限定类名实现org.apache.ibatis.logging.Log的，并得到一个字符串作为构造函数参数。您也可以选择执行，通过调用下面的方法之一：

```java
org.apache.ibatis.logging.LogFactory.useSlf4jLogging();
org.apache.ibatis.logging.LogFactory.useLog4JLogging();
org.apache.ibatis.logging.LogFactory.useLog4J2Logging();
org.apache.ibatis.logging.LogFactory.useJdkLogging();
org.apache.ibatis.logging.LogFactory.useCommonsLogging();
org.apache.ibatis.logging.LogFactory.useStdOutLogging();
```
        如果您选择调用这些方法之一，你应该这样做之前调用任何其他MyBatis的方法。此外，这些方法只会切换到请求的日志实现，如果实现是运行时类路径。例如，如果您尝试选择Log4J日志和Log4J是不是在运行时可用，那么MyBatis将忽略该请求使用Log4J，发现日志实现将使用它的正常算法。

      SLF4J，Apache common logging,Apache Log4J，和JDK日志记录API的细节超出了本文的讨论范围。然而，下面的示例配置应该让你开始。如果您想了解更多有关这些框架，你可以得到更多的信息，从下列位置：[SLF4J](http://www.slf4j.org/)[Apache Commons Logging](http://commons.apache.org/logging)[Apache Log4j 1.x and 2.x](http://logging.apache.org/log4j/)[JDK Logging API](http://java.sun.com/j2se/1.4.1/docs/guide/util/logging/)


**日志记录配置（Logging Configuration）**


     要看到MyBatis的日志报表，您可以启用一个包，一个映射器完全合格的类名，命名空间OA完全合格声明名称的记录。同样，你是怎么做到这一点的是依赖于使用的日志实现。我们将展示如何使用log4j。配置日志记录服务是根本的问题，包括一个或多个额外的配置文件（例如log4j.properties文件），有时一个新的JAR文件（如log4j.jar的）。以下的配置例子使用Log4J作为一个供应商的完整的日志记录服务。有两个步骤。



- **步骤1：添加Log4J的JAR文件**



          因为我们使用Log4J，我们将需要确保其JAR文件是提供给我们的应用程序。使用Log4J，你需要将JAR文件添加到您的应用程序的classpath。您可以从上面的URL下载Log4J。对于Web或企业应用程序，您可以添加，将log4j.jar WEB-INF/lib目录中，或为一个独立的应用程序，你可以简单地把它添加到类路径中的JVM启动参数。



- **步骤2：配置log4j**



         log4j的配置很简单，假设你要启用此映射日志：




```java
package org.mybatis.example;
public interface BlogMapper {
  @Select("SELECT * FROM blog WHERE id = #{id}")
  Blog selectBlog(int id);
}
```
创建一个名为log4j.properties文件内容如下所示，将其放置在你的classpath中：




```
# Global logging configuration
log4j.rootLogger=ERROR, stdout
# MyBatis logging configuration...
log4j.logger.org.mybatis.example.BlogMapper=TRACE
# Console output...
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%5p [%t] - %m%n
```
上述文件将导致log4j的报告详细的日志记录为org.mybatis.example.BlogMapper只是你的应用程序其余类的错误。

如果你想更精细的水平调整日志记录，可以为特定的语句，而不是整个映射文件打开记录。以下的生产线将使只是记录的selectBlog声明：
`log4j.logger.org.mybatis.example.BlogMapper.selectBlog=TRACE`此外，你可能希望要启用日志记录的一组映射器。在这种情况下，你应该添加记录器根包映射器所在：



`log4j.logger.org.mybatis.example=TRACE`有些查询可以返回巨大的结果集。在这种情况下，你可能希望看到的SQL语句，但不是结果集。为了这个目的，SQL语句都被记录在DEBUG级别的（罚款JDK记录）和结果的跟踪级别（细JDK日志），所以如果你想看到的语句，但没有结果，设置级别设置为DEBUG。
`log4j.logger.org.mybatis.example=DEBUG`但是，如果你不使用映射接口，但像这样的一个XML映射文件呢？

```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
  PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
  "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="org.mybatis.example.BlogMapper">
  <select id="selectBlog" parameterType="int" resultType="Blog">
    select * from Blog where id = #{id}
  </select>
</mapper>
```
在这种情况下，你可以启用日志记录整个XML文件添加一个logger的命名空间，如下所示：



`log4j.logger.org.mybatis.example.BlogMapper=TRACE`或一个特定的语句：



`log4j.logger.org.mybatis.example.BlogMapper.selectBlog=TRACE`是的，正如你可能已经注意到了，这里没有什么区别，在配置映射接口或XML映射文件记录。
注：如果您正在使用SLF4J或Log4j2 MyBatis会调用它使用标记的MyBatis。

其余的配置log4j.properties文件是用来配置追加，这是超出本文件的范围。但是，你可以找到更多的信息的Log4J的网站（上述网址）。或者，你可以简单地尝试它，看看有什么效果有不同配置选项。


