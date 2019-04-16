# Spring+SpringMVC+MyBatis框架配置与踩坑笔记 - 彩虹糖的博客 - CSDN博客





2019年03月29日 21:53:07[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：47








# 前言

最近在使用Spring+SpringMVC+MyBatis做软件工程的课程项目，虽然之前有一次使用SSM框架的经历，但是还是要说，自己配置这个框架简直是太难了！在博客上不断查阅资料和搜索报错后，我终于把我项目的整体框架运行了起来，在这里我把在配置过程中的重要信息和遇到的错误记录下来，希望下一次的配置过程不要这么艰辛。



# 一. Spring+SpringMVC+MyBatis 项目整体框架

项目的整体框架如下图所示：

![](https://img-blog.csdnimg.cn/20190329205700355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

框架配置的过程主要参考的是这两篇博文：[手把手搭建ssm框架]()，[ssm框架整合](https://www.cnblogs.com/honey01/p/7680693.html)

关于每个文件夹，具体的作用，很多博文都有详尽的描述，在此不在赘述。

需要注意的是，最好在把所有结构配好之后，再在IDEA上右键文件夹，选择文件夹类型。如果提前把Java文件夹Make Directory as sources root（也就是让它的颜色变为蓝色），会出现无法在其目录下新建多个包的情况。

在此附上spring-mvc.xml的配置信息：

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/mvc
       http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">

    <!-- 扫描web相关的bean -->
    <context:component-scan base-package="ssm.controller"/>

    <!-- 开启SpringMVC注解模式 -->
    <mvc:annotation-driven/>

    <!-- 静态资源默认servlet配置 -->
    <mvc:default-servlet-handler/>

    <!-- 配置jsp 显示ViewResolver -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
        <property name="prefix" value="/WEB-INF/views/"/>
        <property name="suffix" value=".jsp"/>
    </bean>

</beans>
```

# 二. 数据库连接配置与SQL语句

## 1. 基本配置

上述框架中，jdbc.properties的配置如下（IP和密码被隐藏）

```
jdbc.driver=com.mysql.jdbc.Driver

jdbc.url=jdbc:mysql://0.0.0.0:3306/codejudger?useUnicode=true&characterEncoding=utf-8&serverTimezone=UTC

jdbc.username=root

jdbc.password=000000

c3p0.maxPoolSize=30

c3p0.minPoolSize=10

c3p0.autoCommitOnClose=false

c3p0.checkoutTimeout=10000

c3p0.acquireRetryAttempts=2
```

spring-mybatis.xml配置如下：

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/tx
       http://www.springframework.org/schema/tx/spring-tx.xsd">

    <!-- 扫描service包下所有使用注解的类型 -->
    <context:component-scan base-package="ssm.service"/>

    <!-- 配置数据库相关参数properties的属性：${url} -->
    <context:property-placeholder location="classpath:jdbc.properties"/>

    <!-- 数据库连接池 -->
    <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <property name="driverClass" value="${jdbc.driver}"/>
        <property name="jdbcUrl" value="${jdbc.url}"/>
        <property name="user" value="${jdbc.username}"/>
        <property name="password" value="${jdbc.password}"/>
        <property name="maxPoolSize" value="${c3p0.maxPoolSize}"/>
        <property name="minPoolSize" value="${c3p0.minPoolSize}"/>
        <property name="autoCommitOnClose" value="${c3p0.autoCommitOnClose}"/>
        <property name="checkoutTimeout" value="${c3p0.checkoutTimeout}"/>
        <property name="acquireRetryAttempts" value="${c3p0.acquireRetryAttempts}"/>
    </bean>

    <!-- 配置SqlSessionFactory对象 -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource"/>
        <!-- 扫描model包 使用别名 -->
        <property name="typeAliasesPackage" value="ssm.model"/>
        <!-- 扫描sql配置文件:mapper需要的xml文件 -->
        <property name="mapperLocations" value="classpath:mapper/*.xml"/>
    </bean>

    <!-- 配置扫描Dao接口包，动态实现Dao接口，注入到spring容器中 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <!-- 注入sqlSessionFactory -->
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>
        <!-- 给出需要扫描Dao接口包 -->
        <property name="basePackage" value="ssm.dao"/>
    </bean>

    <!-- 配置事务管理器 -->
    <bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource"/>
    </bean>

    <!-- 配置基于注解的声明式事务 -->
    <tx:annotation-driven transaction-manager="transactionManager"/>

</beans>
```

## 2. mapper文件配置

在mapper文件夹下，我们通过xml文件来实现对数据库的增删该查，一个示例文件如下：

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<mapper namespace="ssm.dao.IProblemDao">

    <select id="getAllProblems" resultType="Problem">SELECT * from problem</select>
    <select id="getProblemByID" resultType="Problem" parameterType="String">SELECT * from problem where uuid=#{uuid}</select>

</mapper>
```

namespace对应的是dao包中的接口，id是接口中方法的名称，parameterType是方法参数类型，returnType对应return值的类型，如果返回类型是List，这里returnType依然是List中元素的类型。

在这里，有一个小问题，我们写的SQL语句无法直观的看到对错，在这里，我们可以使用IDEA为我们提供的工具进行检查。

点击右侧的database，新建一个数据库连接，这样，我们就可以在配置文件中看到我们写的SQL语句是否合法了。

![](https://img-blog.csdnimg.cn/20190329211710190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

## 3. 连接云端数据库

显然对于一个正式的项目，配置一个远程数据库是十分有必要的，在Ubuntu服务器上配置MySQL可以参考这篇博文：[服务器配置MySQL](https://blog.csdn.net/qq_39542027/article/details/79396965)，但是按照教程配置好之后，本机依然连接不上MySQL。查阅了很多资料才明白，这是阿里云本身端口保护的原因，可以参考这篇博文的第三条——[远程连接阿里云MySQL失败解决办法](https://blog.csdn.net/hohaizx/article/details/78370333)。要在阿里云控制台进行相关的配置。

![](https://img-blog.csdn.net/20171027230824378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaG9oYWl6eA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 三. 配置报错

## 1. MySQL版本不匹配报错

报错信息：'caching_sha2_password' cannot be loaded

报错原因：在本机，我使用的是MySQL 8.0+的版本，但是在pom.xml中我却用了5.0+的包，造成了版本的不匹配，修改相关配置如下：

```
<!-- 数据库 -->
    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
      <version>8.0.11</version>
      <scope>runtime</scope>
    </dependency>
```

## 2. controller与service忘记加@

这里的报错很不小心，单纯的因为在controller与service类上面忘加@controller与@service

## 3. mapper中的sql语句区分大小写

报错内容：Cause: java.sql.SQLSyntaxErrorException: Table 'codejudger.Problem' doesn't exist

报错原因：最开始看到这个报错的时候感觉十分的违反常识，怎么可能找不到？并且IDEA也没有提示错误，在这篇博文：[映射mybatis *mapper.xml文件中的SQL时需要区分大小写](https://blog.csdn.net/quan20111992/article/details/81088034)中，作者提到这里的SQL是区分大小写的，如果想不区分，应该使用接口方式映射SQL。因此我将所有的sql查询都改成了小写。

## 4. 生成的项目没有clean

在帮同学配置这个项目的时候，我发现我所有的改动都没有起到效果，因此怀疑是没有清理旧项目导致的，因此选择build，clean之后，果然，后来的配置全部生效了。这个问题我没有遇到过，但是依然提醒大家在遇到此类问题的时候，试着清理一下项目，会不会有奇效。

## 5. 找不到对应的Artifact

这个问题同样是在同学的电脑上遇见的。在配置了Tomcat之后，报错是没有找到生成的artifact。这个错误很好定位，肯定是生成的artifact名字不统一导致的。通过查看tomcat的configuration我发现，其寻找的包的名字和实际要导入的包的名字不一致。

![](https://img-blog.csdnimg.cn/20190329214345277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

我们都知道，使用别人的maven项目可以使用IDEA的import功能，这个问题的出现和import有关系。它继承了原来项目使用的artifact的名字，但是新项目在build的时候，它却生成了新名字的artifact。

## 四. 单元测试

要想做一个成功的项目，软件测试必不可少，这是我无数血泪的教训总结出来的经验，因此这次项目我一定要测试这个环节完成。我目前使用的是Junit工具，目前只写了对dao的测试，相关博文可以参考[使用spring配合Junit进行单元测试的总结](https://www.cnblogs.com/jiaoyiping/p/4251759.html)。

其中的测试例子如下：

```java
package daoTest;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.transaction.annotation.Transactional;
import ssm.dao.IUserInformationDao;
import ssm.model.User;
import ssm.model.UserInformation;

import javax.annotation.Resource;

import static junit.framework.TestCase.assertNotNull;
import static junit.framework.TestCase.assertTrue;
import static org.junit.Assert.assertEquals;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = {"classpath:spring-mvc.xml","classpath:spring-mybatis.xml"})
@Transactional
public class UserInformationDaoTest {
    @Resource
    private IUserInformationDao iUserInformationDao;

    @Test
    public void testSelect()
    {
        UserInformation userInformation = iUserInformationDao.getUserInformationByUserID("1");
        assertNotNull(userInformation);
    }

    @Test
    public void testUpdate()
    {
        UserInformation userInformation= iUserInformationDao.getUserInformationByUserID("1");
        userInformation.setTotalSubmitCount(100);
        iUserInformationDao.updateUserInformation(userInformation);
        UserInformation selectUserInformation = iUserInformationDao.getUserInformationByUserID("1");
        assertEquals(selectUserInformation.getTotalSubmitCount(),100);
    }


}
```

通过单元测试，我发现了大量的疏漏，这证明完善的测试必不可少。

![](https://img-blog.csdnimg.cn/20190329215144604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

全部测试通过，dao测试完成！

# 五. 项目源码

项目正在进行中，欢迎大家指正！

[CodeJudger](https://github.com/caozixuan/CodeJudger)



