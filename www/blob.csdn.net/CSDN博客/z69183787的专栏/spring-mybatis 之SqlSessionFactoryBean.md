# spring-mybatis 之SqlSessionFactoryBean - z69183787的专栏 - CSDN博客
2017年08月04日 15:49:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2155
[http://blog.csdn.net/liuxiao723846/article/details/52424802](http://blog.csdn.net/liuxiao723846/article/details/52424802)
在MyBatis中，使用SqlSessionFactoryBuilder创建SqlSessionFactory，进而来创建 SqlSession。一旦你获得一个 session 之后,你可以使用它来执行映射语句,提交或回滚连接,最后,当不再需要它的时候, 你可以关闭 session。
Mybatis社区自己开发了一个Mybatis-[spring](http://lib.csdn.net/base/javaee)用来满足Mybatis用户整合Spring的需求。
使用 MyBatis-Spring 之后, 会使用SqlSessionFactoryBean来代替SqlSessionFactoryBuilder创建SqlSessionFactory，然后可以使用下面两种方式使用sqlSession。
1、SqlSessionTemplate：
Mybatis-Spring提供了一种直接使用SqlSession的方式（一个实现了SqlSession接口的SqlSessionTemplate实现类）
- 它是线程安全的，可以被多个Dao同时使用；
- 它跟Spring的事务进行了关联，确保当前被使用的SqlSession是一个已经和Spring的事务进行绑定了的，而且它还可以自己管理Session的提交和关闭。
1）spring.xml
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <context:annotation-config/>
- <!-- 扫描service、dao组件 -->
- <context:component-scanbase-packagecontext:component-scanbase-package="cn.edu.nuc"/>
- <!-- 分解配置 jdbc.properites -->
- <context:property-placeholderlocationcontext:property-placeholderlocation="classpath:jdbc.properties"/>
- 
- <!-- 数据源c3p0 -->
- <beanid="dataSource"class="com.mchange.v2.c3p0.ComboPooledDataSource">
- <propertynamepropertyname="driverClass"value="${jdbc.driverClassName}"/>
- <propertyname="jdbcUrl"value="${jdbc.url}" />
- <propertyname="user"value="${jdbc.username}" />
- <propertyname="password"value="${jdbc.password}" />
- </bean>
- 
- <!--配置sqlSessionFactory-->
- <beanid="sqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertynamepropertyname="dataSource"ref="dataSource"/>
- <propertynamepropertyname="configLocation"value="classpath:mybatis/configuration.xml"/>
- <propertynamepropertyname="mapperLocations"value="classpath*:cn/edu/nuc/map/*.xml"/>
- </bean>
- 
- <beanid="sqlSession"class="org.mybatis.spring.SqlSessionTemplate">
- <constructor-argindex="0"ref="sqlSessionFactory" />
- </bean>
mybatis配置文件：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <?xmlversionxmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPEconfiguration      
-     PUBLIC "-//mybatis.org//DTD Config3.0//EN"      
-     "http://mybatis.org/dtd/mybatis-3-config.dtd">
- <configuration>
- <settings>
- <!--changes from the defaults for testing -->
- <settingnamesettingname="cacheEnabled"value="false"/>
- <settingnamesettingname="useGeneratedKeys"value="true"/>
- <settingnamesettingname="defaultExecutorType"value="REUSE"/>
- <settingnamesettingname="lazyLoadingEnabled"value="false"/>
- <settingnamesettingname="logImpl"value="LOG4J"/>
- </settings>
- <mappers>
- 
- </mappers>
- </configuration>
2）dao层实现：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- packagecn.edu.nuc.dao;  
- importorg.apache.ibatis.session.SqlSession;  
- importorg.springframework.beans.factory.annotation.Autowired;  
- importorg.springframework.stereotype.Component;  
- importcn.edu.nuc.bean.User;  
- 
- @Component  
- public classUserDaoImpl implements UserDao {  
- @Autowired  
- private SqlSession sqlSession;  
- 
- publicvoid insertUsers(User user) {  
- sqlSession.insert("cn.edu.nuc.map.UserMapper1.insertUser",user);  
- }  
- }  
3）mapper文件：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <?xmlversionxmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapperPUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- <mappernamespacemappernamespace="cn.edu.nuc.map.UserMapper1">
- 
- <insertid="insertUser"parameterType="cn.edu.nuc.bean.User">
-             insert into user (name)values (#{name});  
- </insert>
- </mapper>
4）[测试](http://lib.csdn.net/base/softwaretest)：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration({"classpath:spring-sqlSessionTemplate.xml"})  
- public classUserServiceSqlSesionTemplateTest {  
-            @Autowired  
-       private UserDao userDao;   
- 
- @Test  
- publicvoid testFindUserById() throws Exception {  
- Useruser = new User();  
- user.setName("lanxiaoli");  
- userDao.insertUsers(user);  
- }  
- }  
补充：在dao层sqlSession.insert("cn.edu.nuc.map.UserMapper1.insertUser",user);第一个参数前半部分是mapper文件的namespace，最后一个是sql的id；这两部分都是随意写的。
2、SqlSessionDaoSupport：
SqlSessionDaoSupport 是一个抽象的支持类, 用来为你提供 SqlSession。调用 getSqlSession()方法你会得到一个SqlSessionTemplate,之后可以用于执行 SQL 方法。
SqlSessionDaoSupport 需要一个 sqlSessionFactory 或sqlSessionTemplate 属性来设置 。 这 些要被 被 明 确地 设 置 或 者由 Spring 来 自 动 装 配 。 如 果 两 者 都 被 设 置 了 , 那 么 SqlSessionFactory 是被忽略的。（其实可以使用下面的MapperScannerConfigurer来自动装配）
1）spring.xml
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <context:annotation-config/>
- 
- <!-- 扫描service、dao组件 -->
- <context:component-scanbase-packagecontext:component-scanbase-package="cn.edu.nuc"/>
- <!-- 分解配置 jdbc.properites -->
- <context:property-placeholderlocationcontext:property-placeholderlocation="classpath:jdbc.properties"/>
- 
- <!-- 数据源c3p0 -->
- <beanid="dataSource"class="com.mchange.v2.c3p0.ComboPooledDataSource">
- <propertynamepropertyname="driverClass"value="${jdbc.driverClassName}"/>
- <propertyname="jdbcUrl"value="${jdbc.url}" />
- <propertyname="user"value="${jdbc.username}" />
- <propertyname="password"value="${jdbc.password}" />
- </bean>
- 
- <!--sqlSessionFactory-->
- <beanid="sqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertynamepropertyname="dataSource"ref="dataSource"/>
- <propertynamepropertyname="configLocation"value="classpath:mybatis/configuration.xml"/>
- <propertynamepropertyname="mapperLocations"value="classpath*:cn/edu/nuc/map/*.xml"/>
- </bean>
- <!--SqlSessionTemplate-->
- <beanid="sqlSession"class="org.mybatis.spring.SqlSessionTemplate">
- <constructor-argindex="0"ref="sqlSessionFactory" />
- </bean>
- 
- <beanid="userDao"class="cn.edu.nuc.dao.UserDaoImpl1">
- <!--注入SqlSessionTemplate实例 -->
- <propertynamepropertyname="sqlSessionTemplate"ref="sqlSession"/>
- <!--也可直接注入SqlSessionFactory实例，二者都指定时，SqlSessionFactory失效 -->
- <!-- <propertyname="sqlSessionFactory" ref="sqlSessionFactory" />-->
- </bean>
2）dao层实现类：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- packagecn.edu.nuc.dao;  
- importorg.mybatis.spring.support.SqlSessionDaoSupport;  
- importcn.edu.nuc.bean.User;  
- 
- public classUserDaoImpl1 extends SqlSessionDaoSupport implements UserDao {  
- publicvoid insertUsers(User user) {  
- getSqlSession().update("cn.edu.nuc.map.UserMapper1.updateUser",user);  
- }  
- }  
3）mapper文件：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <?xmlversionxmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapperPUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- <mappernamespacemappernamespace="cn.edu.nuc.map.UserMapper1">
- 
- <updateidupdateid="updateUser"parameterType="cn.edu.nuc.bean.User">
-             update user setname=#{name} where id=#{id}  
- </update>
- </mapper>
4）测试：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration({"classpath:spring-sqlSessiondaosupport.xml"})  
- public classUserServiceSqlSessionDaoSupportTest {  
- @Autowired  
-       private UserDaouserDao;  
- 
- @Test  
- publicvoid testFindUserById() throws Exception {  
- Useruser = new User();  
- user.setId(2L);  
- user.setName("12323");  
- userDao.insertUsers(user);  
- }  
- }  
分析：
上面做法没问题，但就是在spring.xml中需要配置所有的dao层实现，把sqlSession或者sqlSessionFactory注入进去。下面，可以使用MapperScannerConfigurer来解决这个问题。（通常使用这种方法作为dao层基础[架构](http://lib.csdn.net/base/architecture)）
1）spring.xml配置文件：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- <context:annotation-config/>
- 
- <!-- 扫描service、dao组件 -->
- <context:component-scanbase-packagecontext:component-scanbase-package="cn.edu.nuc"/>
- <!-- 分解配置 jdbc.properites -->
- <context:property-placeholderlocationcontext:property-placeholderlocation="classpath:jdbc.properties"/>
- 
- <!-- 数据源c3p0 -->
- <beanid="dataSource"class="com.mchange.v2.c3p0.ComboPooledDataSource">
- <propertynamepropertyname="driverClass"value="${jdbc.driverClassName}"/>
- <propertyname="jdbcUrl"value="${jdbc.url}" />
- <propertyname="user"value="${jdbc.username}" />
- <propertyname="password"value="${jdbc.password}" />
- <propertynamepropertyname="maxPoolSize"value="${c3p0.pool.size.max}"/>
- <propertynamepropertyname="minPoolSize"value="${c3p0.pool.size.min}"/>
- <propertynamepropertyname="initialPoolSize"value="${c3p0.pool.size.ini}"/>
- <propertynamepropertyname="acquireIncrement"value="${c3p0.pool.size.increment}"/>
- </bean>
- 
- <!--sqlSessionFactory-->
- <beanid="sqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertynamepropertyname="dataSource"ref="dataSource"/>
- <propertynamepropertyname="configLocation"value="classpath:mybatis/configuration.xml"/>
- <propertynamepropertyname="mapperLocations"value="classpath*:cn/edu/nuc/map/*.xml"/>
- </bean>
- <beanclassbeanclass="org.mybatis.spring.mapper.MapperScannerConfigurer">
-         <!-- 可以不用指定，因为MapperScannerConfigurer会自动装配  
- <propertynamepropertyname="sqlSessionFactoryBeanName"value="sqlSessionFactory"/>-->
- <propertynamepropertyname="basePackage"value="cn.edu.nuc.dao"/>
- </bean>
2）dao层实现类：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- packagecn.edu.nuc.dao;  
- importjavax.annotation.Resource;  
- importorg.apache.ibatis.session.SqlSessionFactory;  
- importorg.mybatis.spring.support.SqlSessionDaoSupport;  
- importorg.springframework.stereotype.Component;  
- importcn.edu.nuc.bean.User;  
- 
- @Component  
- public classUserDaoImpl1 extends SqlSessionDaoSupport implements UserDao {  
- @Resource(name="sqlSessionFactory")  
- public void setSqlSessionFactory(SqlSessionFactorysqlSessionFactory) {  
- super.setSqlSessionFactory(sqlSessionFactory);  
- }  
- publicvoid insertUsers(User user) {  
- getSqlSession().update("cn.edu.nuc.map.UserMapper1.updateUser",user);  
- }  
- }  
3）测试类：
**[html]**[view
 plain](http://blog.csdn.net/liuxiao723846/article/details/52424802#)[copy](http://blog.csdn.net/liuxiao723846/article/details/52424802#)
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration({"classpath:spring-sqlSessiondaosupport.xml"})  
- public classUserServiceSqlSessionDaoSupportTest {  
- @Autowired  
-     private UserDao userDaoImpl1;   
- 
- @Test  
- publicvoid testFindUserById() throws Exception {  
- Useruser = new User();  
- user.setId(2L);  
- user.setName("4321");  
- userDaoImpl1.insertUsers(user);  
- }  
- }  
注：在dao层的实现里，必须指定public void setSqlSessionFactory(SqlSessionFactorysqlSessionFactory)；一般的做法是写一个baseDao，然后把所有常见的CRUD方法实现，每个业务模块的dao去继承该baseDao
MapperScannerConfigurer具有下面两个重要特性：
1）不需要指定SqlSessionFactory 或SqlSessionTemplate,MapperScannerConfigurer 将会创建 MapperFactoryBean,之后自动装配；
2）MapperScannerConfigurer 其实和MapperFactoryBean类似，都是创建代理然后注入到spring中；区别是前者是自动扫描数据映射器接口，生成代理类、并注入到Spring的容器中；
我们这里不需要用MapperScannerConfigurer来创建MapperFactoryBean，以及生成代理类注入到spring中（后面的使用中会用到），我们需要：
1）需要MapperScannerConfigurer帮我们自动装配SqlSessionFactory 或 SqlSessionTemplate（因为SqlSessionDaoSupport 需要一个 sqlSessionFactory 或 sqlSessionTemplate属性来设置）；
2）我们需要自己显示的在dao层加上@Component注解（否则依赖的时候会拿到注入到spring中的MapperScannerConfigurer生成的代理类）
3）dao层我们继承了SqlSessionDaoSupport，所以可以直接使用getSqlSession() 方法获取sqlSessionTemplate，然后操作数据映射文件（mapper文件）里的sql了；
