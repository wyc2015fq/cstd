# mybatis-spring学习笔记之SqlSessionFactoryBean - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月18日 00:07:35[boonya](https://me.csdn.net/boonya)阅读数：5542








 在基本的 MyBatis 中,session 工厂可以使用 SqlSessionFactoryBuilder 来创建。而在 MyBatis-Spring 中,则使用 SqlSessionFactoryBean 来替代。

    原文地址：[http://mybatis.github.io/spring/factorybean.html](http://mybatis.github.io/spring/factorybean.html)


## 1、创建SqlSessionFactory

创建工厂 bean,放置下面的代码在 Spring 的 XML 配置文件中:



```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
  <property name="dataSource" ref="dataSource" />
</bean>
```



要注意 SqlSessionFactoryBean 实现了 Spring 的 FactoryBean 接口(请参考 Spring 文 档的 3.8 章节)这就说明了由 Spring 最终创建的 bean 不是 SqlSessionFactoryBean 本身, 。 而是工厂类的 getObject()返回的方法的结果。这种情况下,Spring 将会在应用启动时为你 创建 SqlSessionFactory
 对象,然后将它以 SqlSessionFactory 为名来存储。在 Java 中, 相同的代码是:

```java
SqlSessionFactoryBean factoryBean = new SqlSessionFactoryBean();
SqlSessionFactory sessionFactory = factoryBean.getObject();
```



在一般的 MyBatis-Spring 用法中, 你不需要直接使用 SqlSessionFactoryBean 或和其对 应的 SqlSessionFactory。相反,session 工厂将会被注入到 MapperFactoryBean 或其它扩 展了 SqlSessionDaoSupport 的 DAO(Data Access Object,数据访问对象,译者注)中。



## 2、SqlSessionFactory 属性

          SqlSessionFactory 有一个单独的必须属性,就是 JDBC 的 DataSource。这可以是任意 的 DataSource,其配置应该和其它 Spring 数据库连接是一样的。一个通用的属性是 configLocation,它是用来指定 MyBatis 的 XML
 配置文件路径的(如：mybatis-config.xml)。 如果基本的 MyBatis 配置需要改变, 那么这就是一个需要它的地方。 通常这会是<settings> 或<typeAliases>的部分。

       要注意这个配置文件不需要是一个完整的 MyBatis 配置。确切地说,任意环境,数据源 和 MyBatis 的事务管理器都会被忽略。SqlSessionFactoryBean 会创建它自己的,使用这些 值定制 MyBatis 的 Environment 时是需要的。

       如果 MyBatis 映射器 XML 文件在和映射器类相同的路径下不存在,那么另外一个需要 配置文件的原因就是它了。使用这个配置,有两种选择。第一是手动在 MyBatis 的 XML 配 置文件中使用<mappers>部分来指定类路径。第二是使用工厂 bean 的 mapperLocations 属 性。mapperLocations
 属性使用一个资源位置的 list。 这个属性可以用来指定 MyBatis 的 XML 映射器文件的位置。 它的值可以包含 Ant 样式来加载一个目录中所有文件, 或者从基路径下 递归搜索所有路径。比如:

```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
  <property name="dataSource" ref="dataSource" />
  <property name="mapperLocations" value="classpath*:sample/config/mappers/**/*.xml" />
</bean>
```
这会从类路径下加载在 sample.config.mappers 包和它的子包中所有的 MyBatis 映射器 XML 文件。在容器环境管理事务中,一个可能需要的属性是 transactionFactoryClass。




注：再看此文章之前我自己配置的mybatis-spring中无论如何都注入不进去mappers，这里（` <property name="mapperLocations" value="classpath*:sample/config/mappers/**/*.xml" />`）给了我启示。






