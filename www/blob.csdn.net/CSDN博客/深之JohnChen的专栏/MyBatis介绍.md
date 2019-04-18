# MyBatis介绍 - 深之JohnChen的专栏 - CSDN博客

2017年11月08日 22:47:52[byxdaz](https://me.csdn.net/byxdaz)阅读数：797


一、概念

MyBatis的前身叫iBatis，本是apache的一个开源项目, 2010年这个项目由apache software foundation 迁移到了google code，并且改名为MyBatis。MyBatis是支持普通SQL查询，存储过程和高级映射的优秀持久层框架。MyBatis消除了几乎所有的JDBC代码和参数的手工设置以及结果集的检索。MyBatis使用简单的XML或注解用于配置和原始映射，将接口和Java的POJOs（Plan Old Java Objects，普通的Java对象）映射成数据库中的记录。

Mybatis的功能架构分为三层：

1)       API接口层：提供给外部使用的接口API，开发人员通过这些本地API来操纵数据库。接口层一接收到调用请求就会调用数据处理层来完成具体的数据处理。

2)       数据处理层：负责具体的SQL查找、SQL解析、SQL执行和执行结果映射处理等。它主要的目的是根据调用的请求完成一次数据库操作。

3)      基础支撑层：负责最基础的功能支撑，包括连接管理、事务管理、配置加载和缓存处理，这些都是共用的东西，将他们抽取出来作为最基础的组件。为上层的数据处理层提供最基础的支撑。

![](https://gss0.bdstatic.com/-4o3dSag_xI4khGkpoWK1HF6hhy/baike/c0%3Dbaike80%2C5%2C5%2C80%2C26/sign=4df3ab1439dbb6fd3156ed74684dc07d/0b46f21fbe096b63ea0d41bf0c338744eaf8accc.jpg)

二、Mybatis执行流程图

![](https://img-blog.csdn.net/20171108225141740)

详细说明

1、SessionFactory会话工厂创建

SqlSessionFactoryBuilder

每一个MyBatis的应用程序的入口是SqlSessionFactoryBuilder，它的作用是通过XML配置文件创建Configuration对象（当然也可以在程序中自行创建），然后通过build方法创建SqlSessionFactory对象。没有必要每次访问Mybatis就创建一次SqlSessionFactoryBuilder，通常的做法是创建一个全局的对象就可以了。示例程序如下：

private static SqlSessionFactoryBuilder sqlSessionFactoryBuilder;  

private static SqlSessionFactory sqlSessionFactory;  

private static void init() throws IOException {  

    String resource = "mybatis-config.xml";  

    Reader reader = Resources.getResourceAsReader(resource);  

    sqlSessionFactoryBuilder = new SqlSessionFactoryBuilder();  

    sqlSessionFactory = sqlSessionFactoryBuilder.build(reader);  

}  

SqlSessionFactory

SqlSessionFactory对象由SqlSessionFactoryBuilder创建。它的主要功能是创建SqlSession对象，和SqlSessionFactoryBuilder对象一样，没有必要每次访问Mybatis就创建一次SqlSessionFactory，通常的做法是创建一个全局的对象就可以了。SqlSessionFactory对象一个必要的属性是Configuration对象,它是保存Mybatis全局配置的一个配置对象，通常由SqlSessionFactoryBuilder从XML配置文件创建。这里给出一个简单的示例：

<?xml version="1.0" encoding="UTF-8" ?>  

<!DOCTYPE configuration PUBLIC   

    "-//mybatis.org//DTD Config 3.0//EN"  

    "http://mybatis.org/dtd/mybatis-3-config.dtd">  

<configuration>  

    <!-- 配置别名 -->  

    <typeAliases>  

        <typeAlias type="org.iMybatis.abc.dao.UserDao" alias="UserDao" />  

        <typeAlias type="org.iMybatis.abc.dto.UserDto" alias="UserDto" />  

    </typeAliases>  

    <!-- 配置环境变量 -->  

    <environments default="development">  

        <environment id="development">  

            <transactionManager type="JDBC" />  

            <dataSource type="POOLED">  

                <property name="driver" value="com.mysql.jdbc.Driver" />  

                <property name="url" value="jdbc:mysql://127.0.0.1:3306/iMybatis?characterEncoding=GBK" />  

                <property name="username" value="iMybatis" />  

                <property name="password" value="iMybatis" />  

            </dataSource>  

        </environment>  

    </environments>  

    <!-- 配置mappers -->  

    <mappers>  

        <mapper resource="org/iMybatis/abc/dao/UserDao.xml" />  

    </mappers>  

</configuration>  

要注意XML头部的声明，需要用来验证XML文档正确性。typeAliases 元素是包含所有typeAlias（别名）的列表，别名用来替换完整类名，这样在需要完整类名的地方就可以用别名来代替。environment元素体中包含对事务管理和连接池的环境配置。mappers元素是包含所有mapper（映射器）的列表，这些mapper的XML文件包含SQL代码和映射定义信息。当然，在XML配置文件中还有很多可以配置的，上面的示例指出的则是最关键的部分，其他配置请参考Mybatis的官方文档。

2、SqlSession

SqlSession对象的主要功能是完成一次数据库的访问和结果的映射，它类似于数据库的session概念，由于不是线程安全的，所以SqlSession对象的作用域需限制方法内。SqlSession的默认实现类是DefaultSqlSession，它有两个必须配置的属性：Configuration和Executor。Configuration前文已经描述这里不再多说。SqlSession对数据库的操作都是通过Executor来完成的，Executor的具体功能在下一小节在描述。

到目前为止，我们看到的都是mybatis的流程，我们的应用程序在什么地方插入到这个流程中并获得我们想要的结果呢？就是SqlSession这里。SqlSession有一个重要的方法getMapper，顾名思义，这个方式是用来获取Mapper对象的。什么是Mapper对象？根据Mybatis的官方手册，应用程序除了要初始并启动Mybatis之外，还需要定义一些接口，接口里定义访问数据库的方法，存放接口的包路径下需要放置同名的XML配置文件。SqlSession的getMapper方法是联系应用程序和Mybatis纽带，应用程序访问getMapper时，Mybatis会根据传入的接口类型和对应的XML配置文件生成一个代理对象，这个代理对象就叫Mapper对象。应用程序获得Mapper对象后，就应该通过这个Mapper对象来访问Mybatis的SqlSession对象，这样就达到里插入到Mybatis流程的目的。示例代码如下：

SqlSession session= sqlSessionFactory.openSession();  

UserDao userDao = session.getMapper(UserDao.class);  

UserDto user = new UserDto();  

user.setUsername("iMybatis");  

List<UserDto> users = userDao.queryUsers(user);  

对应的接口：

public interface UserDao {  

    public List<UserDto> queryUsers(UserDto user) throws Exception;  

}  

对应的配置文件：

<?xml version="1.0" encoding="UTF-8" ?>  

<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">  

<mapper namespace="org.iMybatis.abc.dao.UserDao">  

    <select id="queryUsers" parameterType="UserDto" resultType="UserDto"  

        useCache="false">  

        <![CDATA[ 

        select * from t_user t where t.username = #{username} 

        ]]>  

    </select>  

</mapper>  

3、Executor

Executor对象在创建Configuration对象的时候创建，并且缓存在Configuration对象里。Executor对象的主要功能是调用StatementHandler访问数据库，并将查询结果存入缓存中（如果配置了缓存的话）。

4、StatementHandler

StatementHandler是真正访问数据库的地方，并调用ResultSetHandler处理查询结果。

5、ResultSetHandler

处理查询结果。

三、MyBatis官网

MyBatis官网:
[http://www.mybatis.org/mybatis-3/](http://www.mybatis.org/mybatis-3/)

[https://github.com/mybatis/mybatis-3/releases](https://github.com/mybatis/mybatis-3/releases)

所需jar包

mybatis-x.x.x.jar 

mybatis的核心包只有一个mybatis-3.x.0.jar,另外还有一些【可选】的依赖包(日志、代理等所需要的)，在下载的压缩包中可以找到。

[http://www.mybatis.org/mybatis-3/zh/dependencies.html](http://www.mybatis.org/mybatis-3/zh/dependencies.html)

四、MyBatis-config xml配置文件详解

<?xml version="1.0" encoding="UTF-8" ?>  

<!DOCTYPE configuration  

PUBLIC "-//mybatis.org//DTD Config 3.0//EN"  

"http://mybatis.org/dtd/mybatis-3-config.dtd">  

<!-- 配置文件的根元素 -->  

<configuration>  

    <!-- 属性：定义配置外在化 -->  

    <properties></properties>  

    <!-- 设置：定义mybatis的一些全局性设置 -->  

    <settings>  

       <!-- 具体的参数名和参数值 -->  

       <setting name="" value=""/>   

    </settings>  

    <!-- 类型名称：为一些类定义别名 -->  

    <typeAliases></typeAliases>  

    <!-- 类型处理器：定义Java类型与数据库中的数据类型之间的转换关系 -->  

    <typeHandlers></typeHandlers>  

    <!-- 对象工厂 -->  

    <objectFactory type=""></objectFactory>  

    <!-- 插件：mybatis的插件,插件可以修改mybatis的内部运行规则 -->  

    <plugins>  

       <plugin interceptor=""></plugin>  

    </plugins>  

    <!-- 环境：配置mybatis的环境 -->  

    <environments default="">  

       <!-- 环境变量：可以配置多个环境变量，比如使用多数据源时，就需要配置多个环境变量 -->  

       <environment id="">  

          <!-- 事务管理器 -->  

          <transactionManager type=""></transactionManager>  

          <!-- 数据源 -->  

          <dataSource type=""></dataSource>  

       </environment>   

    </environments>  

    <!-- 数据库厂商标识 -->  

    <databaseIdProvider type=""></databaseIdProvider>  

    <!-- 映射器：指定映射文件或者映射类 -->  

    <mappers></mappers>  

</configuration>  

五、Mapper.xml配置文件详解

<?xml version="1.0"encoding="UTF-8" ?>    

<!DOCTYPE mapper    

    PUBLIC "-//mybatis.org//DTD Mapper3.0//EN"    

    "[http://mybatis.org/dtd/mybatis-3-mapper.dtd](http://mybatis.org/dtd/mybatis-3-mapper.dtd)">

<mapper namespace="com...persistence.usermanager.UserMapper">

 <!-- 要对应到所有属性 才能使用-->

 <!--<resultMap id="users" type="User">

 <result property="userName"column="user_name"/>

 </resultMap>-->

 <!-- 这里namespace必须是UserMapper接口的路径，不然要运行的时候要报错 "is not knownto the MapperRegistry" -->

 <insert id="createUser" parameterType="User">

  <![CDATA[ insert into

  user_info (user_id, user_name, user_password,

  user_email,user_role) values (#{userId},#{userName}

  ,#{userPassword},#{userEmail},#{userRole})]]>

  <!-- 这里sql结尾不能加分号，否则报"ORA-00911"的错误 -->

 </insert>

 <!-- 这里的id必须和UserMapper接口中的接口方法名相同，不然运行的时候也要报错 -->

 <delete id="deleteUser" parameterType="[Java](http://lib.csdn.net/base/java).lang.String">

  <![CDATA[ delete from user_info where id = #{id} ]]>

 </delete>

 <update id="updateUsers"parameterType="User">

  <![CDATA[update user_info set

  user_name = #{userName},

  user_password = #{userPassword},

  user_email = #{userEmail},

  user_role = #{userRole}

  where user_id = #{userId} ]]>

 </update>

 <selectid="selectAllUsers" resultType="User">

  <![CDATA[select * from user_info ]]>

 </select>

 <selectid="selectUserById" resultType="User"parameterType="java.lang.String">

  <![CDATA[select * from user_info where user_id = #{userId}]]>

 </select>

 <select id="selectUsers"resultType="User" parameterType="User">

  <![CDATA[select * from user_info ]]>

  <where>

   <if test="userName!=null">

    <![CDATA[And user_name like'%'||#{userName}||'%']]>

   </if>

   <if test="userRole!=null">

    <![CDATA[And user_role like'%'||#{userRole}||'%']]>

   </if>

  </where>

 </select>

 <selectid="selectUsersCount" resultType="int">

  <![CDATA[select count(*) from user_info ]]>

 </select>

 <selectid="selectUserByName" resultType="User"parameterType="java.lang.String">

  <![CDATA[select * from user_info where user_name =#{userName}]]>

 </select>

</mapper>  

六、Hibernate与Mybatis的本质区别和应用场景

Hibernate：标准的ORM框架，不需要写SQL语句，但是优化和修改SQL语句比较难。

应用于需求变化固定的中小型的项目，例如后台管理系统、ERP、ORM、OA。

Mybatis：专注SQL本身，SQL的优化比较方便，是不完全的ORM。

主要适用于需求变化较多的项目，例如互联网项目。

参考资料：

MyBatis-config xml配置：[http://blog.csdn.net/summer_yuxia/article/details/53169227](http://blog.csdn.net/summer_yuxia/article/details/53169227)

Mapper.xml配置：[http://www.cnblogs.com/lcngu/p/5470695.html](http://www.cnblogs.com/lcngu/p/5470695.html)

