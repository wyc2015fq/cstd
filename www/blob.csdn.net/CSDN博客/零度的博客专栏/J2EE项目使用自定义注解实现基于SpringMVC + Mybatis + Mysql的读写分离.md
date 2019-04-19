# J2EE项目使用自定义注解实现基于SpringMVC + Mybatis + Mysql的读写分离 - 零度的博客专栏 - CSDN博客
2016年10月26日 11:15:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：1601
       1.为啥要读写分离？　　
       大家都知道最初开始，一个项目对应一个数据库，基本是一对一的，但是由于后来用户及数据还有访问的急剧增多，系统在数据的读写上出现了瓶颈，为了让提高效率，想读和写不相互影响，读写分离就诞生了。
       2.什么样的项目需要读写分离？
       并不是所有项目都适合读写分离，如果我把我自己的博客网站也搞成读写分离的，菜鸟觉得哇好高大上；砖家就会说SB，读写分离仅适合用在读写尤其频繁的项目，如淘宝这类访问量多，读写都很频繁的情况下。一般来说想中小型企业的erp，网站啥的都不需要，用了反而影响性能，因为读写时的切换也是要耗费资源的
3.读写分离的前提条件是什么？
       如2所说，如果确定了项目需要用到读写分离，那就得配置数据库同步了，数据不同步读写分离就没有意义，同步方式很多种，最简单的主从同步，可以参考前一篇文章 
       下面我讲的j2ee主从同步，用到的是技术框架： 基于注解的SpringMVC + Mybatis + Mysql，Spring mvc就很熟悉了，自己搭个框架一边玩吧，讲讲整合mybatis时候怎么读写分离的。
直接贴代码：
```
[]()
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans" xmlns:context="http://www.springframework.org/schema/context"
    xmlns:p="http://www.springframework.org/schema/p" xmlns:tx="http://www.springframework.org/schema/tx" xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context-3.2.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx-3.2.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop-3.2.xsd">
    <context:component-scan base-package="com.xiao" />
    <aop:aspectj-autoproxy/>
    
    <!-- 引入属性文件 -->
    <context:property-placeholder location="classpath:application.properties" />
    <!-- 数据源配置,使用应用内的proxool数据库连接池 -->
    <bean id="business_write" class="org.logicalcobwebs.proxool.ProxoolDataSource">
        <!-- Connection Info destroy-method="close" -->
        <property name="driver" value="${jdbc.driver}" />
        <property name="driverUrl" value="${business.write.url}" />
        <property name="user" value="${business.write.username}" />
        <property name="password" value="${business.write.password}" />
        <property name="alias" value="master_db" />
        <!-- 测试的SQL执行语句 -->
        <property name="houseKeepingTestSql" value="select count(*) from dual" />
        <!-- 最少保持的空闲连接数 （默认2个） -->
        <property name="prototypeCount" value="${jdbc.prototypeCount}" />
        <!-- proxool自动侦察各个连接状态的时间间隔(毫秒),侦察到空闲的连接就马上回收,超时的销毁 默认30秒） -->
        <property name="houseKeepingSleepTime" value="${jdbc.hourseKeepingSleepTime}" />
        <!-- 最大活动时间(毫秒)(超过此时间线程将被kill,默认为5分钟) -->
        <property name="maximumActiveTime" value="${jdbc.maximumActiveTime}" />
        <!-- 连接最长时间(毫秒)(默认为4个小时) -->
        <property name="maximumConnectionLifetime" value="${jdbc.maximumConnectionLifetime}" />
        <!-- 最小连接数 （默认2个） -->
        <property name="minimumConnectionCount" value="${jdbc.minimumConnectionCount}" />
        <!-- 最大连接数 （默认5个） -->
        <property name="maximumConnectionCount" value="${jdbc.maximumConnectionCount}" />
    </bean>
    <bean id="business_read" class="org.logicalcobwebs.proxool.ProxoolDataSource">
        <!-- Connection Info destroy-method="close" -->
        <property name="driver" value="${jdbc.driver}" />
        <property name="driverUrl" value="${business.read.url}" />
        <property name="user" value="${business.read.username}" />
        <property name="password" value="${business.read.password}" />
        <property name="alias" value="slave_db" />
        <!-- 测试的SQL执行语句 -->
        <property name="houseKeepingTestSql" value="select count(*) from dual" />
        <!-- 最少保持的空闲连接数 （默认2个） -->
        <property name="prototypeCount" value="${jdbc.prototypeCount}" />
        <!-- proxool自动侦察各个连接状态的时间间隔(毫秒),侦察到空闲的连接就马上回收,超时的销毁 默认30秒） -->
        <property name="houseKeepingSleepTime" value="${jdbc.hourseKeepingSleepTime}" />
        <!-- 最大活动时间(毫秒)(超过此时间线程将被kill,默认为5分钟) -->
        <property name="maximumActiveTime" value="${jdbc.maximumActiveTime}" />
        <!-- 连接最长时间(毫秒)(默认为4个小时) -->
        <property name="maximumConnectionLifetime" value="${jdbc.maximumConnectionLifetime}" />
        <!-- 最小连接数 （默认2个） -->
        <property name="minimumConnectionCount" value="${jdbc.minimumConnectionCount}" />
        <!-- 最大连接数 （默认5个） -->
        <property name="maximumConnectionCount" value="${jdbc.maximumConnectionCount}" />
    </bean>
    <bean id="dataSource" class="com.xiao.weixin.common.jdbc.DynamicDataSource">
        <property name="targetDataSources">
            <map>
                <entry key="business_write" value-ref="business_write" />
                <entry key="business_read" value-ref="business_read" />
            </map>
        </property>
        <property name="defaultTargetDataSource" ref="business_write" />
    </bean>
    <!-- 线程池配置 -->
    <bean id="taskExecutor" class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor">
        <!-- 线程池活跃的线程数 -->
        <property name="corePoolSize" value="5" />
        <!-- 线程池最大活跃的线程数 -->
        <property name="maxPoolSize" value="10" />
        <!-- 队列的最大容量 -->
        <property name="queueCapacity" value="25" />
    </bean>
    <!-- myBatis文件 -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource" />
        <!-- 自动扫描entity目录, 省掉Configuration.xml里的手工配置 -->
        <property name="mapperLocations" value="classpath:com/xiao/**/*Mapper.xml" />
        
        <!--  <property name="configLocation" value="classpath:mybatisMapConfig.xml" />-->
    </bean>
    <!-- myBatis Mapper文件 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.xiao.**.dao" />
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />
    </bean>
    <!-- 事务管理器配置 -->
    <bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource" />
    </bean>
    <!-- 使用annotation定义事务 -->
    <tx:annotation-driven transaction-manager="transactionManager" order="2"/>
    <!-- Spring jdbcTemplate配置 -->
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="xiaoFilter" class="com.xiao.weixin.common.filter.XiaoFilter" />
    
    <import resource="spring-mybatis_quartz.xml"/>
    <!--  引入shiro 缓存配置 -->
    <import resource="applicationContext-shiro.xml"/>
    <!-- 引入国际化 配置-->
    <import resource="spring-i18n.xml"/>
</beans>
[]()
```
首先你得有配置好了的 读数据库和写数据库  不会的戳这里哈 [http://www.cnblogs.com/xiaochangwei/p/4824355.html](http://www.cnblogs.com/xiaochangwei/p/4824355.html)
配置好了读和写的数据源了后，仔细看
```
[]()
<bean id="dataSource" class="com.xiao.weixin.common.jdbc.DynamicDataSource">
        <property name="targetDataSources">
            <map>
                <entry key="business_write" value-ref="business_write" />
                <entry key="business_read" value-ref="business_read" />
            </map>
        </property>
        <property name="defaultTargetDataSource" ref="business_write" />
    </bean>
[]()
```
这里指定了两个数据源，并且默认是用的写数据库，
数据源之所以能切换是因为Spring提供了 org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource接口
默认不是写么
```
[]()
package com.xiao.weixin.common.jdbc;
import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
/**
 * 设置数据源
 */
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface DataSource {
    String value() default "business_read";
}
[]()
```
如果我们在serviceimpl中，某个方法上加上 @DataSource 如下：
```
[]()
@Override
    @DataSource
    public String getAccessToken() {
        String access_token = null;
        try {
            AccessToken token = weixinApiMapper.findToken();// 数据库查找没过期的token并返回
            if (token != null) {
                access_token = token.getAccessToken();
            }
        } catch (Exception e) {
            log.error("获取accessToken出错");
            e.printStackTrace();
        }
        log.debug("获取到的accessToken是：" + access_token);
        return access_token;
    }
[]()
```
就会连接到读数据库，不写则用默认的读数据库
不过 只是上面的代码是不行的 ，具体切换还得看Spring提供的接口
配置中关联的实现类代码如下
```
[]()
package com.xiao.weixin.common.jdbc;
import org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource;
public class DynamicDataSource extends AbstractRoutingDataSource {
    @Override
    protected Object determineCurrentLookupKey() {
        return JdbcContextHolder.getJdbcType();
    }
}
[]()
```
JdbcContextHolder 又是下面这样实现滴
```
[]()
package com.xiao.weixin.common.jdbc;
/**
 * 连接哪个数据源的环境变量
 */
public class JdbcContextHolder {
    private static final ThreadLocal<String> contextHolder = new ThreadLocal<String>();  
    
    public static void setJdbcType(String jdbcType) {    
        contextHolder.set(jdbcType);  
    }    
    
    /**
     * 在选用business_write数据源前，执行此方法
     */
    public static void setBusinessWrite(){  
        setJdbcType("business_write");  
    }  
    /**
     * 在选用business_read数据源前，执行此方法
     */
    public static void setBusinessRead(){  
        setJdbcType("business_read"); 
    }  
    public static String getJdbcType(){    
        return (String) contextHolder.get();   
    }   
    /**
     * 恢复成默认的数据源，即defaultTargetDataSource，执行此方法
     */
    public static void clearJdbcType() {    
        contextHolder.remove();    
    }    
}
[]()
```
就这样就能轻松搞定读写分离了。
