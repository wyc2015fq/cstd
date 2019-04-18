# SSM配置、整合、注意事项 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月22日 20:02:25[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：426


首先说说SSM整合给我们带来的便利性，mybatis通过动态代理以及工厂模式实现自动生成dao层实现类，然后通过spring-mybatis 实现将创建出来的dao层实现类放入IOC容器。spring 就是将 要用到的实现类 都加上注解，然后放入IOC容器，这样我们就不需要 去new 实现类的对象了。 Spring-MVC 可以解析url 地址栏，然后去根据解析的url去找 controllor 中对应的方法，从而给客户端不同的回应，达到所有的处理方法全都写在一个.java文件中的效果。并且可以实现 从客户端表单接收到的数据 自动放入pojo对应的变量中，这一点是通过getParamMapping获取表单参数，然后找input 标签中的name 的值 和 pojo中变量名 一样的，实现自动赋值，所有input标签中的name的值 一定要和 pojo 中变量名 一样。

配置：SpringMVC

1、导jar包，推荐用maven。 

<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-context</artifactId>
    <version>4.1.6.RELEASE</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-webmvc</artifactId>
    <version>4.1.6.RELEASE</version>
</dependency>

<dependency>

2、配置servlet

spring-MVC 默认的配置文件位置是WEB-INF下的。还有个默认的名字（忘了是什么了）

但习惯性的我们把所有的配置文件放在src/main/resources/   下

所以 需要配置xml 文件 路径，同时配置xml 文件名

由于 默认情况是 第一次访问对应地址的时候 才会 调用servlet中的init的方法，这就会导致 第一个访问的用户加载的时间长，为了更好地体验，我们设置 服务器一启动 就 创建 对象，即调用init方法。

classpath 代表资源文件夹根目录、只有在spring 框架中有效

<servlet>
<servlet-name>springmvc</servlet-name>
<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
<init-param>
<param-name>contextConfigLocation</param-name>
<param-value>classpath:mvc/springmvc.xml</param-value>
</init-param>
<load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
<servlet-name>springmvc</servlet-name>
<url-pattern>/</url-pattern>
</servlet-mapping>

3、创建springmvc.xml 文件

xml文件需要模板，可以自行上网搜，下面列出主要内容。

      //配置 注解驱动

    <mvc:annotation-driven></mvc:annotation-driven>

//配置需要扫描的包，即需要放入IOC容器的 .java文件 所在的文件夹

       <context:component-scan base-package="com.公司名.web"></context:component-scan>

     //由于servlet会拦截 所有的get请求，也会把css、jquery 等拦截，所以配置下面这条语句

             //告诉servlet 不要拦截 我WEB-INF/resources/ 这个文件夹下的所有文件。 **代表所有文件

             <mvc:resources mapping="/resources/**" location="/resources/" />

//配置视图解析器

        <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">

//需要用到jstl 所以 配置一下

                <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"></property>

//controllor 中控制页面 重定向、转发 的url地址栏的 前缀和后缀

                <property name="prefix" value="/WEB-INF/pages/student/"></property>
<property name="suffix" value=".jsp"></property>
</bean>

4、把需要放入IOC 容器中的类 加上 注解。 

为了让别人 看的清楚， 通常 dao层 加@Repository 。  service层加@service   web层加@Controller   pojo加@Component

然后 把 需要自动赋值的属性 加上 @Autowired 或@resource（name="  "） 

5、给controllor这个 .java文件 加上 @RequestMapping("/A")

     对应的 增删改查 方法也 加上@RequestMapping("/B")

     这样 就可以通过    http：//ip地址：端口号/项目名/A/B访问到 对应的方法

配置Spring和mybatis

1、导包   -- lombok 这个包 可以实现给pojo 类自动加 get、set 方法，还有toString 方法（需给该pojo加上@Data注解）

common 类似于 日志， 不需要配置

<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.0.29</version>
</dependency>
<dependency>
    <groupId>org.duracloud</groupId>
    <artifactId>common</artifactId>
    <version>4.3.6</version>
</dependency>
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>5.1.38</version>

</dependency>

<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-orm</artifactId>
    <version>4.1.6.RELEASE</version>
</dependency>
<dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
    <version>1.3.0</version>
</dependency>
<dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis</artifactId>
    <version>3.4.1</version>

</dependency>

<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <version>1.16.6</version>
  <scope>provided</scope>
</dependency>


2、配置文件， 同样也是在src/main/resources/   下

<mvc:annotation-driven></mvc:annotation-driven>
<context:component-scan base-package="com.公司名.service"></context:component-scan>
<!-- 配置数据库连接池 maxActive最大连接数，minIdle最小连接数 initial一次增加的连接数目 
 maxWait 最大等待时间-->
<!-- 这个id 必须是dataSource -->
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" destroy-method="close">
<property name="driverClassName" value=""></property>
<property name="url" value=""></property>
<property name="username" value=""></property>
<property name="password" value=""></property>
</bean>
<!-- 配置mybatis中的SQLsessionFactory -->
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
<!--配置所使用的连接池  -->
<property name="dataSource" ref="dataSource"></property>
<!-- 配置维护sql语句的xml文件中 pojo地址的前缀 -->
<property name="typeAliasesPackage" value="com.公司名.pojo"></property>
</bean>
<!-- 扫包 把sqlSession创建出的代理对象扫描进IOC容器中 -->
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
<property name="basePackage" value="com.公司名.dao"></property>

</bean>

3、配置维护sql语句的配置文件。这个文件的位置最终在硬盘里必须和dao层接口 在一个文件夹里

所有的资源文件夹在硬盘里 都会 合并一起。

所以 比如 dao层接口在 src/main/java/com/公司名/dao 这个目录下 

那么  该配置文件必须在  src/main/resources/com/公司名/dao 这个目录下 

<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
  PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
  "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<!-- 要实现的接口 -->
<mapper namespace="com.公司名.dao.StudentDao">

<!-- 如果数据库字段和pojo属性 名 不一样  需要配置这个  type是要操作的pojo

                这里的resultMap由于我在xml中配置typeAliasesPackage，所以只需要写类名即可

                 否则需要写完整的类地址 如 com.公司名.pojo.Student-->
<resultMap type="Student" id="studentMap">
<result property="bId" column="b_id"/>
</resultMap>
<!-- 配置要实现dao接口的方法 ，以及返回的结果要存入的pojo（这里可直接用上面的id） -->
<select id="getStudents" resultMap="studentMap">
select id,name,b_id,gerden from student
</select>
        <select id="getStudent" resultMap="studentMap" parameterType="java.lang.Integer">
select id,name,b_id,gerden from student where id=#{1}
</select>
<insert id="add" parameterType="Student">
insert into student(name,b_id,gerden) values(#{name},#{bId},#{gerden})
</insert>
<update id="update" parameterType="Student">
update student set name=#{name},b_id=#{bId},gerden=#{gerden} where id=#{id}
</update>
<delete id="del" parameterType="java.lang.Integer">
delete from student where id=#{id}
</delete>
</mapper>


4、也是为了服务器一启动 就开始读取各种配置文件 做好准备工作。配置监听器、过滤器(解决中文乱码问题)

       context-param 标签中 配置的是 xml文件的位置和名字

注意： 这里 跟spring-MVC有个区别， Spring-MVC 是通过 配置servlet的 load-in-start 来控制的。

而 spring 是通过 监听器 来控制的

<context-param>
<param-name>contextConfigLocation</param-name>
<param-value>classpath:spring/applicationContext.xml</param-value>
</context-param>

<filter>
<filter-name>encoding</filter-name>
<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
<init-param>
<param-name>encoding</param-name>
<param-value>UTF-8</param-value>
</init-param>
    <init-param>
<param-name>forceEncoding</param-name>
<param-value>true</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>encoding</filter-name>
<url-pattern>*</url-pattern>
</filter-mapping>

<listener>
<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>

总结一下 需要注意的点： 

1、配置数据库连接池的bean 标签的id 必须为dataSource。

2、维护sql语句的 xml配置文件中 带参数的 sql 语句  最好不要用${ } 而用#{ }

     原因（1） 防止sql语句注入

            （2） ${ } 如果传的是字符串 参数 必须用带引号 括住 如  '${ }' 而#{} 不需要

3、#{ } 括号中 写的 pojo的属性名 ，前端页面 el表达式 ${} 括号中写的是 该pojo对象的get方法，去掉get这三个字母，然后剩下的字母第一个字母变小写。如果用了lombok包的 @Data注释， 那个自动生成的 get方法一般都为 get+成员变量名（变量名的第一个字母变大写）， 例如，Student 类 其中的一个 变量 为 String name；那么自动生成的 get方法名 就是 getName();这时我们的el表达式就得写成${Student.name}

这里 需要注意 一点 ， 如果 其中一个 变量名 为 bId (第二个字母是大写，看不清楚，说明一下)。那么自动生成的get方法名就是getBId() ，像这种第二字母是 大写 的情况， el 表达式 中 就不需要变了 即 $(Studnet.BId) 。

还有 就是 sql语句 中涉及的 字段 是 跟数据库 表单中的 字段 一致的， 别写着写着 晕乎了，跟pojo 一致 去了。

4、表单中 input 标签的 name 的值 必须和pojo 对应的变量名 相同， 否则无法自动赋值

5、property 中 是 get方法名字 去掉get以后 ，第一个字母变小写 ， 跟上面说的el 表达式 写法 一样。

6、维护sql语句的xml文件 必须 和 dao层 接口 最终 在 系统硬盘里 在 一个 文件夹。 


