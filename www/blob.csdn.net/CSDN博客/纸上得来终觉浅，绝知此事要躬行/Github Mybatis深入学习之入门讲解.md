# Github Mybatis深入学习之入门讲解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月08日 01:10:31[boonya](https://me.csdn.net/boonya)阅读数：4783








  MyBatis的是一个一流的持久层框架，支持自定义SQL、存储过程和高级映射。MyBatis消除了几乎所有的JDBC代码和手动设定参数和检索结果。MyBatis可以使用简单的XML或注解配置去原始映射接口和Java的POJO（简单Java对象）与数据库中的记录对应。

下面是Mybatis入门基础知识，原文地址：[http://mybatis.github.io/mybatis-3/getting-started.html](http://mybatis.github.io/mybatis-3/getting-started.html)

**1、添加MyBatis的Jar包**

     要使用MyBatis的，你只是需要 在classpath，包括 的MyBatis-x.x.x.jar的 文件。如果您正在使用Maven的依赖你的pom.xml添加以下：



```
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis</artifactId>
  <version>x.x.x</version>
</dependency>
```



**2、从XML构建SqlSessionFactory**

      每个MyBatis的应用程序围绕SqlSessionFactory的一个实例。SqlSessionFactory的实例可以通过使用SqlSessionFactoryBuilder进行回收。SqlSessionFactoryBuilder可以从一个XML配置文件建立一个SqlSessionFactory的实例，创建一个自定义配置类的实例。
      通过XML文件创建SqlSessionFactory实例非常简单，你可以使用任意的InputStream实例读取资源路径下的MyBatis的XML配置文件，然后通过此InputStream对象构建SqlSessionFactory对象即可，下面是Java示例代码：



```java
String resource = "org/mybatis/example/mybatis-config.xml";
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
```

      该XML的核心配置包含于MyBatis系统，包括了数据源和数据库连接，并且有事务和作用域的控制。该XML的详细配置信息可以在下面找到，但这是一个简单的例子，示例配置如下：





```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
  PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
  "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
  <environments default="development">
    <environment id="development">
      <transactionManager type="JDBC"/>
      <dataSource type="POOLED">
        <property name="driver" value="${driver}"/>
        <property name="url" value="${url}"/>
        <property name="username" value="${username}"/>
        <property name="password" value="${password}"/>
      </dataSource>
    </environment>
  </environments>
  <mappers>
    <mapper resource="org/mybatis/example/BlogMapper.xml"/>
  </mappers>
</configuration>
```

        上面就是包含许多典型的XML配置文件，注意XML文件的头是请求验证文件的。environment元素是配置事务管理和连接池的，mappers包含了一个映射的列表，而mapper定义的是SQL的代码及映射定义。



**3、不使用XML构建SqlSessionFactory**

如果你愿意直接创建Java配置而不是Xml配置，那么你需要创建自己的构建者，而MyBatis为我们提供了与XML配置相同的配置对象。



```java
DataSource dataSource = BlogDataSourceFactory.getBlogDataSource();
TransactionFactory transactionFactory = new JdbcTransactionFactory();
Environment environment = new Environment("development", transactionFactory, dataSource);
Configuration configuration = new Configuration(environment);
configuration.addMapper(BlogMapper.class);
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(configuration);
```

       注意，这种情况下的配置是添加mapper类。mapper类是那种包含了SQL映射的注解，避免需要XML。然而，由于一些Java注解的限制和MyBatis的一些复杂映射，XML仍然适用于高级的映射。从这个原因，MyBatis会动态地加载和查找对应存在的XML文件。这就好比是BlogMapper.xml从文件路径下被加载。



**4、从SqlSessionFactory获得SqlSession**

      如果你有一个SqlSessionFactory，那么你可以获得名称探索建议那样的一个SqlSession实例。SqlSession包含每个方法对应到数据库执行的SQL的命令。你可以执行SQL对应的声明直接对应到SqlSession的实例。例如：



```java
SqlSession session = sqlSessionFactory.openSession();
try {
  Blog blog = session.selectOne("org.mybatis.example.BlogMapper.selectBlog", 101);
} finally {
  session.close();
}
```

      虽然这种方法是MyBatis的以前版本的用户所熟悉的，现在有一个更清洁的方法。使用接口（如BlogMapper.class）正确地描述了给定语句的参数和返回值，你现在可以执行更简单，更安全的代码，不容易出错的字符串文字和转换。例如：





```java
SqlSession session = sqlSessionFactory.openSession();
try {
  BlogMapper mapper = session.getMapper(BlogMapper.class);
  Blog blog = mapper.selectBlog(101);
} finally {
  session.close();
}
```
**5、探索映射声明**


        在这一点上，你可能想知道到底什么正在执行的SqlSession和Mapper类。映射的SQL语句是一个大的主题，这个主题会主宰本文档中的大部分。但给你一个想法，究竟是正在运行，这里有几个例子。

       在上面的例子中，语句可以被定义XML或注解。让我们来看看XML。 MyBatis的全套功能，可以通过使用基于XML的映射语言实现使MyBatis流行了多年。如果你之前已经使用的MyBatis，这一概念将是你熟悉的，但已经有许多改进XML映射文件，将变得清晰。下面是一个例子，一个基于XML的映射语句将满足上述SqlSession的通话。



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

尽管这个简单的例子看起来像一个很大的开销，但它实际上是很轻的。只要你喜欢您可以定义很多的映射语句在一个单独的XML映射文件，所以你得到了很多里程XML标头和doctype声明。其余的文件相当于自我解释，它定义映射的语句的“selectBlog”的名称，命名空间中的的“org.mybatis.example.BlogMapper”，这将允许你打电话给它指定完全合格的名称“org.mybatis.example.BlogMapper.selectBlog”像上面我们在下面的例子：

```java
Blog blog = session.selectOne("org.mybatis.example.BlogMapper.selectBlog", 101);
```

注意多么的相似，这是一个完全合格的Java类上调用一个方法，并且有一个原因支撑它。这个名字可以直接映射到一个映射器类相同的名称命名空间，映射的select语句的名称，参数和返回类型相匹配的方法。这可以让你非常容易地调用映射器接口的方法对你上面看到的，例子：

```java
BlogMapper mapper = session.getMapper(BlogMapper.class);
Blog blog = mapper.selectBlog(101);
```

第二种方式有很多优点。首先，它不依赖于一个字符串常量，所以它是更安全的。其次，如果你的IDE有代码完成，您可以利用导航映射SQL语句。
注：命名空间笔记{


以前版本MyBatis的命名空间是可选的，这是混乱和无助。命名空间现在需要更长的时间，完全合格的名称超越了简单的隔离报表的目的。命名空间使你在这里看到接口绑定，即使你不认为你会使用他们今天奠定了这里的情况下，你改变你的想法应该遵循这些做法。一次使用的命名空间，并把它在一个适当的Java包命名空间将清理你的代码，并在长期内提高MyBatis的可用性。

}

名称解析：{为了减少打字量，MyBatis的使用下列名称解析规则的所有命名的配置元素，包括语句，结果映射，缓存等。


完全限定名（如“com.mypackage.MyMapper.selectAllThings”）会直查找，如果发现使用。

短的名称（如“selectAllThings”）可以被用来引用任何明确的条目。但是，如果有两个或两个以上（例如的“com.foo.selectAllThings com.bar.selectAllThings”），然后你会收到一条错误报告认为，短期的名字是不明确的，因此必须完全合格。


有一个良好的映射器类，如BlogMapper。其对应的报表并不需要在所有的XML映射。相反，他们可以使用Java注解。例如，上面的XML可以被消除，替换为：




```java
package org.mybatis.example;
public interface BlogMapper {
  @Select("SELECT * FROM blog WHERE id = #{id}")
  Blog selectBlog(int id);
}
```

      注解很多清除器简单的声明，但是，Java注解都是有限的混乱，更复杂的声明。因此，如果你必须做什么复杂的，你最好用XML映射语句。

这将是你和你的项目小组，以确定哪些是适合你的，是多么的重要，以一致的方式被定义映射语句。这就是说，你永远不会被锁定到一个单一的方法。你可以很容易地迁移​​注释到XML映射语句，反之亦然。



}

**6、范围和生命周期**

这是非常重要的是要了解不同的范围和生命周期类，到目前为止我们已经讨论了。使用不当可能会导致严重的并发问题。

**7、SqlSessionFactoryBuilder**

这个类可以被实例化，使用和扔掉。有没有必要保持它的遍布，一旦你已经创建了SqlSessionFactory。因此，最好的范围是SqlSessionFactoryBuilder实例方法范围内（即本地方法变量）。您可以重复使用SqlSessionFactoryBuilder建立多个SqlSessionFactory的实例，但它仍然是最好不要保持它的遍布，以确保所有的XML解析资源得以释放，更重要的事情。
**8、SqlSessionFactory**

      一旦创建了SqlSessionFactory应该让他存在于你的应用程序执行的期间。它应该有很少或根本没有理由永远处置或重新创建它。这是一个最好的做法是不能重建了SqlSessionFactory多次在应用程序运行。这样应该算是一个“坏味道”。因此SqlSessionFactory的最佳扩散是在应用范围。这可以通过多种方式实现。最简单的就是使用Singleton模式或静态Singleton模式（单例工厂）。


**9、SqlSession**

      每个线程都应该有自己的SqlSession实例。 SqlSession的实例不能共享，不是线程安全的。因此，最好的范围是请求或方法范围。不要让一个SqlSession实例的引用静态字段或一个类的实例字段。不要让引用了一个SqlSession任何形式的管理范围，如HttpSession中的Servlet框架。如果您使用的是任何形式的web框架，要考虑SqlSession放在一个HTTP请求遵循一个相似的范围内。换句话说，接收HTTP请求时，你可以打开了一个SqlSession，然后返回响应后，你可以关闭它。它的会话结束是非常重要的。您应该始终确保它在finally块中关闭。以下为标准模式确保SqlSessions关闭




```java
SqlSession session = sqlSessionFactory.openSession();
try {
  // do work
} finally {
  session.close();
}
```

使用这种模式始终贯穿你的代码，将确保所有的数据库资源都正确地关闭（这就是良好习惯的养成之道）。



**10、mapper 实例**

       映射器创建绑定映射语句的接口，而映射器接口的实例从SqlSession中获得。因此，在技术上他们要求SqlSession的范围最广的任何映射实例是相同的。然而，映射器实例的最佳范围是方法范围。也就是说，它们应请求被用来在该方法内，然后被丢弃。他们并不需要明确地关闭。
 SqlSession的，类似请求，让他们周围的整个虽然它不是一个问题，你可能会发现，在这个层面上，管理太多的资源很快就会一发不可收拾。保持简单，将映射器放在方法范围。下面的例子演示了这种做法。



```java
SqlSession session = sqlSessionFactory.openSession();
try {
  BlogMapper mapper = session.getMapper(BlogMapper.class);
  // do work
} finally {
  session.close();
}
```
注：对象的生命周期和依赖注入框架。依赖注入框架可以创建线程安全的，事务性SqlSessions和映射器，并注入他们直接进入你的bean，这样你就可以忘掉自己的生命周期。你可能想看看的MyBatis-Spring或MyBatis-Guice子项目更多地了解使用MyBatis的DI框架。




