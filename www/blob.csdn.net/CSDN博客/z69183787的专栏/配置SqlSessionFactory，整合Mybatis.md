# 配置SqlSessionFactory，整合Mybatis - z69183787的专栏 - CSDN博客
2017年10月31日 14:17:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：835
  要利用Mybatis首先是需要导入mybatis-3.3.0.jar，其次，要整合Spring和Mybatis需要导入mybatis-spring-1.2.3.jar。
### 1、Spring整合Mybatis的xml配置
常用配置：
（如果在mybatis-config.xml利用<mappers>进行xml映射文件的配置，就可以不用配置下面的mapperLocation属性了）
```xml
<!-- mybatis文件配置，扫描所有mapper文件 -->
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean"
      p:dataSource-ref="dataSource"
      p:configLocation="classpath:mybatis-config.xml"
      p:mapperLocations="classpath:com/eliteams/quick4j/web/dao/*.xml"/>
<!-- spring与mybatis整合配置，扫描所有dao，在单数据源的情况下可以不写sqlSessionFactoryBeanName -->
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer"
      p:basePackage="com.eliteams.quick4j.web.dao"
      p:sqlSessionFactoryBeanName="sqlSessionFactory"/>
```
### 2、Spring和Mybatis整合的三种方式
#### 一
- 
SqlSessionFactoryBean来替代SqlSessionFactoryBuilder来创建SqlSession
- 
利用mybatis映射文件**.xml来配置
    SqlSessionFactoryBean有一个必须属性dataSource，另外其还有一个通用属性configLocation（用来指定mybatis的xml配置文件路径）。
Spring的xml配置：
```xml
<!-- 创建SqlSessionFactory，同时指定数据源-->
      <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
          <property name="dataSource" ref="dataSource" /> 
          <!-- 指定sqlMapConfig总配置文件，订制的environment在spring容器中不在生效-->
          <property  name="configLocation"  value="classpath:sqlMapConfig.xml"/>
      </bean>
```
mybatis总配置文件sqlMapConfig.xml：
```xml
<configuration>
  <typeAliases>
     <typeAlias type="com.xxt.ibatis.dbcp.domain.User" alias="User" />
  </typeAliases>
  <mappers>
     <mapper resource="com/xxt/ibatis/dbcp/domain/userMapper.xml" />
  </mappers>
</configuration>
```
userMapper.xml:
```
<mapper namespace="com.xxt.ibatis.dbcp.dao.UserDao">
     <resultMap type="User" id="userMap">
        <id property="id" column="id" />
        <result property="name" column="name" />
        <result property="password" column="password" />
        <result property="createTime" column="createtime" />
     </resultMap>
     <select id="getUserById" parameterType="int" resultMap="userMap">
       select * from user where id = #{id}
     </select>
<mapper/>
```
DAO层接口类UserDao.java：注意此处定义的接口方法需要和UserMapper.xml映射的<select>标签的id对应
```java
```java
public interface UserDao {
    public User getUserById(int id);
}
```
```
需要操作数据时调用的类UserService.java：
```java
```java
public class UserService {
     //此处省略sqlSession的获取方法
     private SqlSession sqlSession;
     private UserDao userDao;
     public User getUserById(int id) {
         return userDao.getUserById(id);
     }
}
```
```
#### 二
- 
SqlSessionFactoryBean来替代SqlSessionFactoryBuilder来创建SqlSession
- 
采用数据映射器（MapperFactoryBean）的方式
- 
不用写mybatis映射文件
- 
采用注解方式提供相应的sql语句和输入参数。
Spring的xml配置：
```
<!-- 引入jdbc配置文件 -->
     <context:property-placeholder location="jdbc.properties"/>
      <!--创建jdbc数据源 -->
      <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
          <property name="driverClassName" value="${driver}"/>
          <property name="url" value="${url}"/>
          <property name="username" value="${username}"/>
          <property name="password" value="${password}"/>
          <property name="initialSize" value="${initialSize}"/>
          <property name="maxActive" value="${maxActive}"/>
          <property name="maxIdle" value="${maxIdle}"/>
          <property name="minIdle" value="${minIdle}"/>
      </bean>
      <!-- 创建SqlSessionFactory，同时指定数据源-->
      <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
          <property name="dataSource" ref="dataSource" /> 
      </bean>
      <!--创建数据映射器，数据映射器必须为接口-->
      <bean id="userMapper" class="org.mybatis.spring.mapper.MapperFactoryBean"> 
          <property name="mapperInterface" value="com.xxt.ibatis.dbcp.dao.UserMapper" />
          <property name="sqlSessionFactory" ref="sqlSessionFactory" /> 
      </bean>
      <bean id="userDaoImpl" class="com.xxt.ibatis.dbcp.dao.impl.UserDaoImpl">
          <property name="userMapper" ref="userMapper"/>
      </bean>
```
数据映射器UserMapper.java：
```java
```
public interface UserMapper {
        @Select("SELECT * FROM user WHERE id = #{userId}") 
        User getUser(@Param("userId") long id); 
}
```
```
DAO接口类UserDao.java：
```java
```java
public interface UserDao {
       public User getUserById(User user);
}
```
```
DAO接口实现类UserDaoImpl.java：
```java
```java
public class UserDaoImpl implements UserDao {
       private UserMapper userMapper;
       public void setUserMapper(UserMapper userMapper) { 
           this.userMapper = userMapper; 
       } 
       public User getUserById(User user) {
          return userMapper.getUser(user.getId()); 
       }
}
```
```
#### 三
- 
SqlSessionFactoryBean来替代SqlSessionFactoryBuilder创建SqlSession
- 
不采用采用数据映射器（MapperFactoryBean）的方式，改为MapperScannerConfigurer 进行扫描
- 
不用写mybatis映射文件
- 
采用注解方式提供相应的sql语句和输入参数。
- 
采用注解方式省去定义mapper的Bean
    MapperFactoryBean 创建的代理类实现了 UserMapper 接口,并且注入到应用程序中。 因为代理创建在运行时环境中(Runtime,译者注) ,那么指定的映射器必须是一个接口,而 不是一个具体的实现类。
    上面的MapperFactoryBean配置有一个很大的缺点，就是系统有很多的配置文件时 全部需要手动编写，所以上述的方式已经不用了。
    没有必要在 Spring 的 XML 配置文件中注册所有的映射器。相反,你可以使用一个 MapperScannerConfigurer , 它将会查找类路径下的映射器并自动将它们创建成MapperFactoryBean。
Spring的xml配置：
```java
```java
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
  <property name="basePackage" value="org.mybatis.spring.sample.mapper" />
</bean>
```
```
basePackage 属性是让你为映射器接口文件设置基本的包路径。 你可以使用分号或逗号 作为分隔符设置多于一个的包路径。每个映射器将会在指定的包路径中递归地被搜索到。
    注 意 , 没有必要去指定SqlSessionFactory 或 SqlSessionTemplate , 因为 MapperScannerConfigurer 将会创建MapperFactoryBean，之后自动装配。但是,如果你使 用了一个 以上的 DataSource，那 么自动装配可能会失效 。这种情况下 ，你可以使用 sqlSessionFactoryBeanName 或 sqlSessionTemplateBeanName
 属性来设置正确的 bean 名称来使用。
