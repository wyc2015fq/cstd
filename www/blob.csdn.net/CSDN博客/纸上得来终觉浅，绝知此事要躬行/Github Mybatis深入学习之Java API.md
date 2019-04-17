# Github Mybatis深入学习之Java API - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月10日 01:37:35[boonya](https://me.csdn.net/boonya)阅读数：4077








       了解Mybatis的Java API对编写将Mybatis为持久化框架的Java程序很有帮助，尤其是对于数据库操作方面以及SqlSession的管理方面很重要。

       原文地址：[http://mybatis.github.io/mybatis-3/java-api.html](http://mybatis.github.io/mybatis-3/java-api.html)

        现在，你知道如何配置MyBatis的创建映射，准备好（需要的）东西。 MyBatis的Java API是你在哪里得到你的努力获得回报。正如你所看到的，JDBC相比，MyBatis很大程度简化了你的代码，并保持干净，易于理解和维护。 MyBatis的3已推出了多项重大改进，使工作更好的SQL映射。

**1、目录结构（Dictionary structure）**

     在我们深入到Java API的本身之前，重要的是要了解周围的目录结构的最佳实践。 MyBatis的是非常灵活的，并与你的文件，你几乎可以做任何事情。但是，任何框架，是最佳的方式。让我们来看看一个典型的应用程序目录结构：




```
/my_application
  /bin
  /devlib
  /lib                <-- MyBatis *.jar files go here.
  /src
    /org/myapp/
      /action
      /data           <-- MyBatis artifacts go here, including, Mapper Classes, XML Configuration, XML Mapping Files.
        /mybatis-config.xml
        /BlogMapper.java
        /BlogMapper.xml
      /model
      /service
      /view
    /properties       <-- Properties included in your XML Configuration go here.
  /test
    /org/myapp/
      /action
      /data
      /model
      /service
      /view
    /properties
  /web
    /WEB-INF
      /web.xml
```
请记住，这些喜好，而不是要求，但别人会感谢你使用一个共同的目录结构。本节的其余部分中的例子，假设你按照这个目录结构。


**2、SqlSessions**

        初级Java接口与MyBatis的SqlSession的。通过这个界面，你可以执行命令，获取映射器和管理事务。我们将讨论更多关于SqlSession本身，但首先我们要学习如何获取SqlSession实例。 SqlSessions SqlSessionFactory的实例创建的。 SqlSessionFactory的包含SqlSessions所有不同的方式创建实例的方法。创建SqlSessionFactory的本身由SqlSessionFactoryBuilder可以创建SqlSessonFactory的XML，注解或手工编码的Java配置。
   注意：当使用MyBatis的依赖注入框架如Spring或Guice，SqlSessions创建和注入DI框架，所以你并不需要使用SqlSessionFactoryBuilder或SqlSessionFactory的，可以直接去SqlSession的部分。请参阅MyBatis的Spring或MyBatis的吉斯手册进一步信息。


- **SqlSessionFactoryBuilder**



SqlSessionFactoryBuilder有五个build（）方法，它允许你建立一个SqlSession的从不同的来源。



```java
SqlSessionFactory build(InputStream inputStream)
SqlSessionFactory build(InputStream inputStream, String environment)
SqlSessionFactory build(InputStream inputStream, Properties properties)
SqlSessionFactory build(InputStream inputStream, String env, Properties props)
SqlSessionFactory build(Configuration config)
```
前四个方法是最常见的，因为他们需要的InputStream的实例，该实例是指一个XML文档，或者更具体地说，上面所讨论的MyBatis-config.xml文件。可选参数为环境和物业。环境决定加载哪种环境，包括数据源和事务管理。例如：

```
<environments default="development">
  <environment id="development">
    <transactionManager type="JDBC">
        ...
    <dataSource type="POOLED">
        ...
  </environment>
  <environment id="production">
    <transactionManager type="MANAGED">
        ...
    <dataSource type="JNDI">
        ...
  </environment>
</environments>
```
如果你调用一个构建方法，该方法的环境参数，那么MyBatis会使用配置，环境。当然，如果你指定了无效的环境，您将收到一条错误。如果你打电话的构建方法，不采取环境参数之一，那么默认的环境使用（默认情况下，被指定为“development”在上面的例子）。如果你调用一个方法，它有一个属性实例，那么MyBatis就会加载这些属性，并让他们提供给你的配置。那些属性可以用在这里的大多数值在配置中使用以下语法：$ {propName}。

       回想一下，属性可以被引用的MyBatis-config.xml文件，或直接指定它。因此，重要的是要了解的优先。我们前面已经提及它在这个文件中，但这里要再次重申，以供参考：


**如果一个属性存在于一个以上的这些地方的MyBatis的加载按以下顺序。**
（1）、指定的属性的属性元素在体内首先被读取。

（2）、加载classpath中的资源或URL属性的属性的属性元素第二个被读取，并覆盖已经指定的任何重复的属性，

（3）、传递的属性作为方法参数最后被读取，并覆盖任何重复的特性，可能已加载从属性的身体和资源/ url属性。

因此，最高优先级的属性是那些作为方法参数，其次是资源/ url属性，最后指定的属性的属性元素在体内传递。

所以总结，前四种方法都大致相同，但覆盖就允许您选择指定的环境和/或属性。下面是一个例子SqlSessionFactory的建设的MyBatis-config.xml文件。



```java
String resource = "org/mybatis/builder/mybatis-config.xml";
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactoryBuilder builder = new SqlSessionFactoryBuilder();
SqlSessionFactory factory = builder.build(inputStream);
```
 请注意，我们使用了Resources工具类，住在的org.apache.ibatis.io包。资源类，正如它的名字所暗示的，可以帮助你从资源加载的类路径，文件系统或一个web URL。通过你的IDE就让我们来看看这个类的源代码或检查将揭示其相当明显的一整套有用的方法。下面是一个简单的列表：

```java
URL getResourceURL(String resource)
URL getResourceURL(ClassLoader loader, String resource)
InputStream getResourceAsStream(String resource)
InputStream getResourceAsStream(ClassLoader loader, String resource)
Properties getResourceAsProperties(String resource)
Properties getResourceAsProperties(ClassLoader loader, String resource)
Reader getResourceAsReader(String resource)
Reader getResourceAsReader(ClassLoader loader, String resource)
File getResourceAsFile(String resource)
File getResourceAsFile(ClassLoader loader, String resource)
InputStream getUrlAsStream(String urlString)
Reader getUrlAsReader(String urlString)
Properties getUrlAsProperties(String urlString)
Class classForName(String className)
```
最后的构建方法需要配置的一个实例。 Configuration类包含了一切你可能需要了解SqlSessionFactory的实例。 Configuration类是有用的反思的配置，包括查找和操作SQL映射（不推荐，一旦申请被接受请求）。配置类有所有配置的开关，你已经了解了，只露出了一个Java API。这里有一个简单的例子，如何手动配置实例，并把它传递给build（）方法创建了SqlSessionFactory。

```java
DataSource dataSource = BaseDataTest.createBlogDataSource();
TransactionFactory transactionFactory = new JdbcTransactionFactory();

Environment environment = new Environment("development", transactionFactory, dataSource);

Configuration configuration = new Configuration(environment);
configuration.setLazyLoadingEnabled(true);
configuration.setEnhancementEnabled(true);
configuration.getTypeAliasRegistry().registerAlias(Blog.class);
configuration.getTypeAliasRegistry().registerAlias(Post.class);
configuration.getTypeAliasRegistry().registerAlias(Author.class);
configuration.addMapper(BoundBlogMapper.class);
configuration.addMapper(BoundAuthorMapper.class);

SqlSessionFactoryBuilder builder = new SqlSessionFactoryBuilder();
SqlSessionFactory factory = builder.build(configuration);
```
现在你有一个SqlSessionFactory的，可以用来创建SqlSession的实例。



- **SqlsessionFactory**



SqlSessionFactory有六个被用来创建SqlSessionInstances的方法。在一般情况下，你会选择下列方法之一时决定：

（1）Transaction：你要使用的交易范围为会话，或使用自动提交（通常指没有交易与大多数数据库和/或JDBC驱动程序）？

（2）Connection：你想MyBatis的收购为您配置的数据源的连接，或者你要提供你自己的吗？

（3）Execution：你想MyBatis复用预处理和/或批量更新（包括插入和删除）？

重载的openSession（）的方法签名的一组允许您选择这些选项的任意组合，这是有道理的。


```java
SqlSession openSession()
SqlSession openSession(boolean autoCommit)
SqlSession openSession(Connection connection)
SqlSession openSession(TransactionIsolationLevel level)
SqlSession openSession(ExecutorType execType,TransactionIsolationLevel level)
SqlSession openSession(ExecutorType execType)
SqlSession openSession(ExecutorType execType, boolean autoCommit)
SqlSession openSession(ExecutorType execType, Connection connection)
Configuration getConfiguration();
```
默认的openSession（）方法不带任何参数，将创建一个SqlSession的，具有以下特点：


（1）、启动一个事务范围（即不自动提交）。

（2）、Connection对象将获得由活动环境配置的DataSource实例。

（3）、事务隔离级别将使用默认的驱动程序或数据源。

（4）、预处理语句不会得到重用，并没有分批更新。

大部分的方法都是相当自我解释的。要启用自动提交，通过可选的autoCommit参数的值为true。要提供自己的连接，通过连接的连接参数的实例。请注意，没有覆盖同时设置连接和autoCommit，因为MyBatis会使用任何设置connection对象提供的，目前正在使用。 MyBatis的事务隔离级别，使用Java枚举包装称为TransactionIsolationLevel，但在其他方面如预期般运作，并有5级支持JDBC（NONE，READ_UNCOMMITTED，READ_COMMITTED，REPEATABLE_READ，SERIALIZABLE）的。
 一个参数，这可能是新的给你是ExecutorType的。此枚举定义了3个值：

（1）、ExecutorType.SIMPLE：这种类型的执行人确实没什么特别的。它创建了一个新的预处理每个执行语句。

（2）、ExecutorType.REUSE：这个执行器类型会复用预处理。

（3）、ExecutorType.BATCH：此遗嘱执行人会批量更新语句标定它们是必要的，如果SELECT在它们之间执行，以确保一个易于理解的行为。

注意：还有一SqlSessionFactory的方法，我们没有提到，那就是getConfiguration（）。此方法将返回配置MyBatis的配置在运行时，您可以使用内省的一个实例。

如果你已经使用之前版本MyBatis的，你会记得，会话，事务和批次都是分离的。这已不再是如此。所有这些都包含在一个session范围内。你不必单独处理交易或批次他们得到充分的好处。


- **SqlSession**



        正如上面所提到的，SqlSession实例类MyBatis中是最强大的。正是在这里，你会发现所有的方法执行语句，提交或回滚事务，还有获取映射器实例。

SqlSession的类有二十多个方法，让我们将它们分解为易于理解的组合。
**语句执行方法（Statement Execution Methods）**

         这些方法用于执行SELECT，INSERT，UPDATE和DELETE语句中定义的SQL映射的XML文件。他们是会自我解释，每个语句和参数对象的ID属性，它可以是原生类型（自动装箱或包装），一个JavaBean，POJO或Map。




```java
<T> T selectOne(String statement, Object parameter)
<E> List<E> selectList(String statement, Object parameter)
<K,V> Map<K,V> selectMap(String statement, Object parameter, String mapKey)
int insert(String statement, Object parameter)
int update(String statement, Object parameter)
int delete(String statement, Object parameter)
```
     在selectOne和选择列表之间的区别仅在于selectOne必须返回一个对象或空（无）。如果不止一个，将引发异常。如果不知道需要多少对象，使用选择列表。如果你想检查一个对象是否存在，你最好还是返回一个数（0或1）。 selectMap是一个特殊情况，因为它被设计成一个结果列表转换成地图基于结果对象中的属性之一。因为不是所有的报表都需要一个参数，这些方法被重载的版本，不需要参数对象。返回的值插入，更新和删除方法表示的语句影响的行数。

```java
<T> T selectOne(String statement)
<E> List<E> selectList(String statement)
<K,V> Map<K,V> selectMap(String statement, String mapKey)
int insert(String statement)
int update(String statement)
int delete(String statement)
```
最后，还有三个高级版本的查询方法，它们允许你限制返回的行，或者提供定制结果处理逻辑，通常是非常大的数据集的范围。

```java
<E> List<E> selectList (String statement, Object parameter, RowBounds rowBounds)
<K,V> Map<K,V> selectMap(String statement, Object parameter, String mapKey, RowBounds rowbounds)
void select (String statement, Object parameter, ResultHandler handler)
void select (String statement, Object parameter, RowBounds rowBounds, ResultHandler handler)
```
参数RowBounds导致MyBatis的跳过指定数量的记录，以及返回一定数量的结果的数量限制。 RowBounds类有一个构造采取的偏移和限制，否则是不可改变的。

```java
int offset = 100;
int limit = 25;
RowBounds rowBounds = new RowBounds(offset, limit);
```
不同的驱动能够实现不同程度的在这方面的效率。为了获得最佳性能，使用结果集类型的SCROLL_SENSITIVE或SCROLL_INSENSITIVE的（换句话说：不FORWARD_ONLY）。

使用ResultHandler参数可以让你处理每一行，除非你喜欢。你可以把它添加到一个列表中，创建一个地图，设置，或抛出每个结果，而不是只保留卷起计算的总和。你可以做几乎任何使用ResultHandler，它是什么MyBatis的使用本身内部创建结果集列表。该接口是非常简单的。

```java
package org.apache.ibatis.session;
public interface ResultHandler {
  void handleResult(ResultContext context);
}
```
ResultContext参数给你访问结果对象本身的计数结果对象被创建的数量，和一个布尔stop（）方法，你可以用它来停止的MyBatis加载更多的结果。使用ResultHandler的有两个限制，你应该知道：

（1）、从一个方法调用与ResultHandler得到的数据将不被缓存。

（2）、当使用先进的resultmaps MyBatis的可能会需要几行建立一个对象。如果使用一个ResultHandler是你可能会给出一个对象，其协会或尚未填充的集合。



**事务控制方法（Transaction Control Methods）**


        这里有四种方法控制的交易的范围。当然，这些都没有效果，如果你已经选择了使用自动提交，或者如果你正在使用外部事务管理。但是，如果您使用的是JDBC事务管理，管理连接实例，然后会派上用场的四个方法：




```java
void commit()
void commit(boolean force)
void rollback()
void rollback(boolean force)
```
          默认情况下，MyBatis的实际上并没有承诺，除非它检测到数据库已更改通过调用插入，更新或删除。如果你已经做出了一些更改，恕不调用这些方法，那么你可以通过真正的commit和rollback方法来保证它会被提交（请注意，您仍然无法在自动提交模式​​下强制session，或者是使用外部事务管理器）。大多数时候，你会不会有调用rollback（），因为MyBatis会为你做的，如果你不调用commit。但是，如果您需要更细粒度的控制多个会话提交和回滚都可能，你必须回滚选项有可能。

注意：MyBatis-Spring和MyBatis-Guice提供声明式事务处理。所以，如果你正在使用的MyBatis与Spring或Guice，请参考手册。


**本地缓存(Local cache)**

        MyBatis的使用两个缓存：本地缓存和二级缓存。

        创建一个新会话的MyBatis每次创建一个本地缓存和重视它的会话。在会话中执行的任何查询将被存储在本地缓存中，以便进一步执行相同的查询相同的输入参数不会碰到数据库。清除本地缓存更新时，提交，回滚和关闭。

        默认情况下，本地缓存数据用于整个会话持续时间。此缓存需要解决循环引用，并加快反复嵌套查询，所以它永远不能被完全禁用，但可以配置语句执行的持续时间设置localCacheScope =语句一样使用本地缓存。

       请注意，当localCacheScope设置节的MyBatis的返回引用相同的对象被存储在本地缓存中。返回的对象（名单等）影响的任何修改本地缓存的内容，随后从缓存的生命周期中的会议返回值。因此，最好的做法，不修改的MyBatis的返回的对象。你可以随时调用清除本地缓存：


```java
void clearCache()
```
**确保SqlSession被关闭(Ensuring that SqlSession is Closed)**

```java
void close()
```
最重要的事情就是你必须确保关闭你打开的任何会话。最好的办法，以确保这是使用以下单位工作模式：

```java
SqlSession session = sqlSessionFactory.openSession();
try {
    // following 3 lines pseudocode for "doing some work"
    session.insert(...);
    session.update(...);
    session.delete(...);
    session.commit();
} finally {
    session.close();
}
```
或者，如果你使用的是JDK1.7+和MyBatis的3.2+，你可以使用的尝试与资源语句：




```java
try (SqlSession session = sqlSessionFactory.openSession()) {
    // following 3 lines pseudocode for "doing some work"
    session.insert(...);
    session.update(...);
    session.delete(...);
    session.commit();
}
```
注意：如同SqlSessionFactory一样，你可以得到配置SqlSession的使用通过调用getConfiguration（）方法的配置实例。

```java
Configuration getConfiguration()
```
**使用映射器（Using Mappers）**
```java
<T> T getMapper(Class<T> type)
```
       虽然各种插入，更新，删除和选择上述方法是强大的，他们也很详细，但不是类型安全的，也不是你的IDE或单元测试，因为他们可以帮助。我们已经看到了一个例子，使用映射器在上面的入门章节。

      因此，一种较为普遍的方式来执行映射语句是使用映射器类​​。一个映射类是一个简单的接口的方法定义匹配于SqlSession方法。下面的例子展示了一些方法签名和它们是如何映射到SqlSession。

```java
public interface AuthorMapper {
  // (Author) selectOne("selectAuthor",5);
  Author selectAuthor(int id); 
  // (List<Author>) selectList(“selectAuthors”)
  List<Author> selectAuthors();
  // (Map<Integer,Author>) selectMap("selectAuthors", "id")
  @MapKey("id")
  Map<Integer, Author> selectAuthors();
  // insert("insertAuthor", author)
  int insertAuthor(Author author);
  // updateAuthor("updateAuthor", author)
  int updateAuthor(Author author);
  // delete("deleteAuthor",5)
  int deleteAuthor(int id);
}
```
简而言之，每个映射方法签名应该匹配SqlSession方法，它的相关的，但没有字符串参数ID 。相反，方法名必须匹配映射语句的ID 。

此外，返回类型必须符合，类型单一结果或多个结果的数组或集合预期的结果。支持所有常用的类型，包括：原生，地图， POJO和JavaBean的。
注意：映射器接口不需要实现任何接口或扩展任何类。只要方法签名可以用来唯一标识对应语句。映射器接口可以扩展其他接口。要确保你有适当的命名空间的报表中使用XML映射器接口时。此外，唯一的限制是你不能有相同的方法签名在层次结构中的两个接口（一个坏主意） 。

你可以传递多个参数映射方法。如果你这样做的话，他们将被命名为文字“参数”其次是自己的位置参数列表中的默认情况下，例如：＃ {param1 } ， param2} ＃{等，如果你想改变的参数名称（多只），那么你可以使用@参数（ “ PARAMNAME ” ）标注的参数。您也可以通过一个RowBounds实例的方法来限制查询结果。
**映射注解（Mapper Annotations）**


        从一开始， MyBatis是一个XML驱动的框架。配置是基于XML的，而且映射语句中定义XML 。而到了MyBatis 3 ，有新的可供选择。 MyBatis的基础上全面而强大的基于Java的配置API之上。这个配置API是基于XML的MyBatis配置，以及新的基于注解配置的基础。注解提供了一种简单的方式来实现简单映射语句没有引入大量的开销。

       注意不幸的是，他们的表现和灵活的限制Java注解。尽管大量的时间花费在勘测，设计和试验，最强大的MyBatis映射根本无法建立与注解 - 那并不可笑。 C＃属性（例如）没有受到这些限制，因此MyBatis.NET将享受更丰富的替代XML 。这就是说，基于Java注解的配置离不开它的好处。注解如下：


|注解|目标|等价XML|描述|
|----|----|----|----|
|@CacheNamespace|Class|<cache>|配置给定命名空间的缓存（即类）。属性：执行，驱逐，flushInterval（刷新间隔），大小，READWRITE。|
|@CacheNamespaceRef|Class|<cacheRef>|参考另外一个命名空间的缓存来使用。属性：value，这应该是一个命名空间的字符串值（即一个完全合格的类名）。|
|@ConstructorArgs|Method|<constructor>|收集一组结果被传递到一个结果对象的构造函数。属性：value，这是一个参数的数组。|
|@Arg|Method|· <arg><idArg>|一个单一的构造函数的参数的一部分ConstructorArgs集合。属性：id，列java类型，而且jdbcType类型处理器，选择，resultMap的。 id属性是一个布尔值，标识属性被用于比较类似于<idArg> XML元素。|
|@TypeDiscriminator|Method|<discriminator>|一组值的情况下，可以用来确定的结果映射到执行。属性：列，java类型，而且jdbcType，的上面的TypeHandler，案件。案件的情况下，属性是一个数组。|
|@Case|Method|<case>|一个单一的情况下的价值和它对应的映射。属性：value，类型，结果。结果属性的结果是一个数组，因此这个注解是类似的actualResultMap，由下面的Results注解指定。|
|@Results|Method|<resultMap>|结果映射的列表，包含一个特定的结果列映射到一个属性或字段的详细信息。属性：value，这是一个阵列的结果注解。|
|@Result|Method|· <result><id>|一个单一的结果一列，属性或字段之间的映射。属性：ID，柱，物业，java类型，而且jdbcType类型处理器之一，很多。 id属性是一个布尔值，表示此属性应该被用于比较（类似的<id>在XML映射）。一个属性是单一协会，类似于<association>的，和许多属性是集合，<collection>相似。它们被命名，因为它们是为了避免名称冲突。|
|@One|Method|<association>|映射到一个单一的复杂类型的属性值。属性：选择，这是完全合格的名称映射语句（即映射法），可以加载适当类型的一个实例。注意你会发现，加入映射不支持通过注解API。这是由于在Java注解的限制，不允许循环引用。|
|@Many|Method|<collection>|一个复杂类型的集合属性的映射。属性：选择，这是完全合格的名称映射语句（即映射法），可以加载适当类型的实例的集合。注意你会发现，加入映射不支持通过注解API。这是由于在Java注解的限制，不允许循环引用。|
|@MapKey|Method||这是用在方法返回类型是一个Map。作为地图根据这些对象的属性，它是用来转换一个结果对象列表。属性：value，这是一个物业作为地图的关键。|
|@Options|Method|Attributes of mapped statements.|这个注解提供广泛的交换和配置选项属性映射语句通常存在。而不是复杂的每条语句注解，Options注解是一贯和明确的方式来访问它们。属性：useCache的真实，FLUSHCACHE=假的，resultSetType= FORWARD_ONLY statementType=有准备的范围fetchSize=-1，超时=-1，useGeneratedKeys=假，keyProperty=“身份证”，keyColumn=“”。重要的是要了解Java注解，也没有办法指定null值。因此，一旦你从事Optionsannotation的，你的说法是所有的默认值。注意默认值是什么，以避免意外的行为。需要注意的是，keyColumn只需要在某些数据库（如Oracle和PostgreSQL）。见讨论keyColumn和keyProperty以上讨论这些属性中的允许值的更多信息，在插入语句。|
|· @Insert· @Update· @Delete@Select|Method|· <insert>· <update>· <delete><select>|这些注解中的每一个都代表要被执行的实际SQL。他们每个人都需要一个字符串数组（或单独的字符串做）。如果一个字符串数组传递，他们是连接彼此之间用一个空格将它们分开。这有助于避免“丢失空间”的问题时，在Java代码构建SQL。不过，也欢迎你，单个字符串连接在一起，如果你喜欢。属性：value，这是形成单个SQL语句的字符串数组。|
|· @InsertProvider· @UpdateProvider· @DeleteProvider@SelectProvider|Method|· <insert>· <update>· <delete><select>|允许动态SQL创建。这些替代SQL注解允许你指定一个类名和方法，将返回在执行时运行的SQL。执行映射语句后，MyBatis会实例化的类，执行的方法，由提供商指定。这个方法可以选择性接受参数对象作为其唯一的参数，但必须指定该参数或者没有参数。属性：类型，方法。 type属性是一个类的完全限定名。该方法是该类别的方法的名称。注：本节讨论对SelectBuilder类，它可以帮助建立一个更清洁，更容易阅读的方式动态SQL。|
|@Param|Parameter|N/A|如果你映射的方法需要多个参数，这个注解可以应用到一个映射方法的参数，给他们每个人一个名字。否则，多个参数，将其命名为其前缀“param”（不包括任何RowBounds参数）的位置。例如＃{param1}，{param2}等是默认设置。 @Param（“person”），参数将被命名为＃{person}。|
|@SelectKey|Method|<selectKey>|这个注解的复制<selectKey>的的功能注释的方法，插入器@ InsertProvider。它忽略其他方法。如果你指定一个@ selectKey元素注解，那么MyBatis将忽略任何通过@选项注释，或配置属性生成的密钥属性设置。属性：声明一个字符串数组，这是执行的SQL语句，这是参数对象将更新为新的值，在此之前表示必须是真的还是假的，如果SQL语句应执行财产keyProperty插入之前或之后，与resultType是Java类型的keyProperty，statementType是一种类型的声明是任何一个ofSTATEMENT的准备或可调用映射到声明，PreparedStatement andCallableStatement的。的默认编制。|
|@ResultMap|Method|N/A|到一个选择或@ SelectProvider的注释，注解是用来提供一个XML映射在一个<resultMap>元素的id。这使得注明选用以重用resultmaps的XML中定义的。这个注解，如果两者都指定一个注解的选择上，将覆盖任何@结果或_AT_ ConstructorArgsannotation的。|
|@ResultType|Method|N/A|这个注解用来当使用结果处理。在这种情况下，返回类型为void MyBatis的，所以必须有一种方法来确定对象的类型构造的每一行。如果有一个XML结果地图，使用@结果映射批注。如果结果类型中指定XML<select>元素，再没有其他的注解是必要的。在其他情况下，使用此批注。例如，如果一个@选择注解的方法将使用结果处理，返回类型必须是无效的，这个注解（或@的ResultMap）是必需的。这个注解被忽略，除非该方法的返回类型为void。|

**映射注解示例（Mapper Annotation Examples）**

     这个例子显示使用@ selectKey元素注解检索值从一个序列插入之前：




```java
@Insert("insert into table3 (id, name) values(#{nameId}, #{name})")
@SelectKey(statement="call next value for TestSequence", keyProperty="nameId", before=true, resultType=int.class)
int insertTable3(Name name);
```
   这个例子显示检索标识值插入后使用@ selectKey元素注解：

```java
@Insert("insert into table2 (name) values(#{name})")
@SelectKey(statement="call identity()", keyProperty="nameId", before=false, resultType=int.class)
int insertTable2(Name name);
```





