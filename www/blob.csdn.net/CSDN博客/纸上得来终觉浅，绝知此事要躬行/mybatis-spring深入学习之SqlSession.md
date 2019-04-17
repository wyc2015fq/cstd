# mybatis-spring深入学习之SqlSession - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月22日 21:13:32[boonya](https://me.csdn.net/boonya)阅读数：2956








  在 MyBatis 中,你可以使用 SqlSessionFactory 来创建 SqlSession。一旦你获得一个 session 之后,你可以使用它来执行映射语句,提交或回滚连接,最后,当不再需要它的时 候, 你可以关闭 session。 使用 MyBatis-Spring 之后, 你不再需要直接使用 SqlSessionFactory 了,因为你的 bean 可以通过一个线程安全的
 SqlSession 来注入,基于 Spring 的事务配置 来自动提交,回滚,关闭 session。

  原文地址：[http://mybatis.github.io/spring/sqlsession.html](http://mybatis.github.io/spring/sqlsession.html)

注意：通常不必直接使用 SqlSession。 在大多数情况下 MapperFactoryBean, 将会在 bean 中注入所需要的映射器。


## 1、SqlSessionTemplate

  SqlSessionTemplate 是 MyBatis-Spring 的核心。 这个类负责管理 MyBatis 的 SqlSession, 调用 MyBatis 的 SQL 方法, 翻译异常。 SqlSessionTemplate
 是线程安全的, 可以被多个 DAO 所共享使用。

      当调用 SQL 方法时, 包含从映射器 getMapper()方法返回的方法, SqlSessionTemplate 将会保证使用的 SqlSession 是和当前 Spring 的事务相关的。此外,它管理 session 的生命 周期,包含必要的关闭,提交或回滚操作。

      SqlSessionTemplate 实现了 SqlSession 接口,这就是说,在代码中无需对 MyBatis 的 SqlSession 进行替换。 SqlSessionTemplate 通常是被用来替代默认的 MyBatis 实现的 DefaultSqlSession , 因为模板可以参与到 Spring 的事务中并且被多个注入的映射器类所使 用时也是线程安全的。相同应用程序中两个类之间的转换可能会引起数据一致性的问题。

SqlSessionTemplate 对象可以使用 SqlSessionFactory 作为构造方法的参数来创建。

```
<bean id="sqlSession" class="org.mybatis.spring.SqlSessionTemplate">
  <constructor-arg index="0" ref="sqlSessionFactory" />
</bean>
```



这个 bean 现在可以直接注入到 DAO bean 中。你需要在 bean 中添加一个 SqlSession 属性,就像下面的代码:



```java
public class UserDaoImpl implements UserDao {

  private SqlSession sqlSession;

  public void setSqlSession(SqlSession sqlSession) {
    this.sqlSession = sqlSession;
  }

  public User getUser(String userId) {
    return (User) sqlSession.selectOne("org.mybatis.spring.sample.mapper.UserMapper.getUser", userId);
  }
}
```



如下注入 SqlSessionTemplate:

```
<bean id="userDao" class="org.mybatis.spring.sample.dao.UserDaoImpl">
  <property name="sqlSession" ref="sqlSession" />
</bean>
```



SqlSessionTemplate 有一个使用 ExecutorType 作为参数的构造方法。这允许你用来 创建对象,比如,一个批量 SqlSession,但是使用了下列 Spring 配置的 XML 文件:

```
<bean id="sqlSession" class="org.mybatis.spring.SqlSessionTemplate">
  <constructor-arg index="0" ref="sqlSessionFactory" />
  <constructor-arg index="1" value="BATCH" />
</bean>
```



现在你所有的语句可以批量操作了,下面的语句就可以在 DAO 中使用了。

```java
public void insertUsers(User[] users) {
   for (User user : users) {
     sqlSession.insert("org.mybatis.spring.sample.mapper.UserMapper.insertUser", user);
   }
 }
```



注意,如果所需的执行方法和默认的 SqlSessionFactory 设置不同,这种配置风格才 能使用。

对这种形式需要说明的是当这个方法被调用时,不能有一个存在使用不同 ExecutorType 运行的事务。也要保证在不同的事务中,使用不同执行器来调用 SqlSessionTemplate 时, (比如 PROPAGATION_REQUIRES_NEW)或完全在一个事务外面。



## 2、SqlSessionDaoSupport

  SqlSessionDaoSupport 是 一 个 抽象 的支 持 类, 用来 为你 提供 SqlSession 。 调 用 getSqlSession()方法你会得到一个 SqlSessionTemplate,之后可以用于执行 SQL 方法, 就像下面这样:

```java
public class UserDaoImpl extends SqlSessionDaoSupport implements UserDao {
  public User getUser(String userId) {
    return (User) getSqlSession().selectOne("org.mybatis.spring.sample.mapper.UserMapper.getUser", userId);
  }
}
```



    通常 MapperFactoryBean 是这个类的首选,因为它不需要额外的代码。但是,如果你 需要在 DAO 中做其它非 MyBatis 的工作或需要具体的类,那么这个类就很有用了。

       SqlSessionDaoSupport 需要一个 sqlSessionFactory 或 sqlSessionTemplate 属性来 设 置 。 这 些 被 明 确 地 设 置 或 由 Spring 来 自 动 装 配 。 如 果 两 者 都 被 设 置 了 , 那 么 SqlSessionFactory 是被忽略的。假设类
 UserMapperImpl 是 SqlSessionDaoSupport 的子类,它可以在 Spring 中进行如 下的配置:



```
<bean id="userMapper" class="org.mybatis.spring.sample.mapper.UserDaoImpl">
  <property name="sqlSessionFactory" ref="sqlSessionFactory" />
</bean>
```






