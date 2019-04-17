# mybatis-spring 学习笔记之简介与入门 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月17日 23:47:18[boonya](https://me.csdn.net/boonya)阅读数：1269








 前段时间自己配置mybatis与spring整合时遇到了问题，所以再回过头来看看mybatis-spring的配置，希望可以进一步了解其原理，找到问题关键点之所在。

    原文地址：[http://mybatis.github.io/spring/getting-started.html](http://mybatis.github.io/spring/getting-started.html)

## **1、 什么是MyBatis-Spring **

       MyBatis-Spring 会帮助你将 MyBatis 代码无缝地整合到 Spring 中。 使用这个类库中的类, Spring 将会加载必要的 MyBatis 工厂类和 session 类。 这个类库也提供一个简单的方式来注入
 MyBatis 数据映射器和 SqlSession 到业务层的 bean 中。 而且它也会处理事务, 翻译 MyBatis 的异常到 Spring 的DataAccessException 异常(数据访问异常,译者注)中。最终,它并 不会依赖于 MyBatis,Spring 或 MyBatis-Spring 来构建应用程序代码。

## **2、添加Maven依赖（Installation）**

       要使用 MyBatis-Spring 模块,你只需要包含 mybatis-spring-x.x.x.jar 文
 件就可以了,并在类路径中加入相关的依赖。如果你使用 Maven,那么在 pom.xml 中加入下面的代码即可:




```
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis-spring</artifactId>
  <version>x.x.x</version>
</dependency>
```





## **3、快速建立项目（Quick setup）**

       要和 Spring 一起使用 MyBatis,你需要在 Spring 应用上下文中定义至少两样东西:一个 SqlSessionFactory 和至少一个数据映射器类。在 MyBatis-Spring 中,SqlSessionFactoryBean
 是用于创建 SqlSessionFactory 的。要 配置这个工厂 bean,放置下面的代码在 Spring 的 XML 配置文件中:



```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
  <property name="dataSource" ref="dataSource" />
</bean>
```




要注意 SqlSessionFactory 需要一个 DataSource(数据源,译者注) 。这可以是任意 的 DataSource,配置它就和配置其它 Spring 数据库连接一样。


数据*Mpper类的配置如下所示:



```java
public interface UserMapper {
  @Select("SELECT * FROM users WHERE id = #{userId}")
  User getUser(@Param("userId") String userId);
}
```




那么可以使用 MapperFactoryBean,像下面这样来把接口加入到 Spring 中:



```
<bean id="userMapper" class="org.mybatis.spring.mapper.MapperFactoryBean">
  <property name="mapperInterface" value="org.mybatis.spring.sample.mapper.UserMapper" />
  <property name="sqlSessionFactory" ref="sqlSessionFactory" />
</bean>
```



要注意,所指定的映射器类必须是一个接口,而不是具体的实现类。在这个示例中,注 解被用来指定 SQL 语句,但是 MyBatis 的映射器 XML 文件也可以用。一旦配置好,你可以用注入其它任意 Spring 的 bean 相同的方式直接注入映射器到你的 business/service
 对象中。MapperFactoryBean 处理 SqlSession 的创建和关闭它。如果使用 了 Spring 的事务,那么当事务完成时,session 将会提交或回滚。最终,任何异常都会被翻 译成 Spring 的 DataAccessException 异常。调用 MyBatis 数据方法现在只需一行代码:



```java
public class FooServiceImpl implements FooService {

private UserMapper userMapper;

public void setUserMapper(UserMapper userMapper) {
  this.userMapper = userMapper;
}

public User doSomeBusinessStuff(String userId) {
  return this.userMapper.getUser(userId);
}
```













