# SSM实战——秒杀系统之创建项目、管理依赖、设计数据库 - weixin_33985507的博客 - CSDN博客
2017年05月25日 20:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
注：本项目使用Myeclipse开发。
一：项目创建
1：使用Myeclipse创建一个web project，命名为MySeckill，并转换为Maven项目。
2：创建项目文件目录如下：
![](https://images2015.cnblogs.com/blog/1018541/201705/1018541-20170525201024888-1827569123.png)
上面四个包分别管理：源代码、源代码资源（配置文件）、测试代码、测试代码资源（配置文件）
3：在pom.xml添加SSM框架所需依赖包：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>MySeckill</groupId>
  <artifactId>MySeckill</artifactId>
  <version>0.0.1-SNAPSHOT</version>
    <dependencies>
    <!-- 单元测试依赖：使用4.11版本，该版本提供基于注解的测试配置 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.11</version>
            <scope>test</scope>
        </dependency>
    <!-- 1：日志依赖:slf4j:日志的规范、接口 + logback：日志接口的实现 + 二者整合依赖包-->
   <dependency>
       <groupId>org.slf4j</groupId>
          <artifactId>slf4j-api</artifactId>
       <version>1.7.7</version>
   </dependency>
    <dependency>
        <groupId>ch.qos.logback</groupId>
        <artifactId>logback-core</artifactId>
        <version>1.1.1</version>
    </dependency>
    <!-- 实现slf4j接口并整合 -->
    <dependency>
        <groupId>ch.qos.logback</groupId>
        <artifactId>logback-classic</artifactId>
        <version>1.1.1</version>
    </dependency>
    
    <!-- 2:数据库依赖：mysql驱动 + c3p0连接池 -->
    <dependency>
       <groupId>mysql</groupId>
       <artifactId>mysql-connector-java</artifactId>
       <version>5.1.37</version>
       <scope>runtime</scope>
   </dependency>
    <dependency>
        <groupId>c3p0</groupId>
        <artifactId>c3p0</artifactId>
        <version>0.9.1.2</version>
    </dependency>
        
    <!-- 3:Mybatis相关依赖：mybatis自身依赖 + mybatis整合spring依赖 -->
    <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.3.0</version>
        </dependency>
    <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.2.3</version>
    </dependency>    
    
    <!-- 4: Servlet web相关依赖-->
    <!-- 标签依赖 + jstl标签库 -->
    <dependency>
            <groupId>taglibs</groupId>
            <artifactId>standard</artifactId>
            <version>1.1.2</version>
        </dependency>
    <dependency>
            <groupId>jstl</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version>
    </dependency>
    <!-- json处理工具：jackson依赖 -->    
    <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.5.4</version>
    </dependency>    
    <!-- servlet本身依赖 -->
    <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
    </dependency>
    
    <!-- 5：spring相关依赖 -->
    <!-- 1)spring核心依赖：core、beans、context -->
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    
    <!-- 2)spring dao层依赖：jdbc依赖 + 事务管理依赖 -->
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    
    <!-- 3)spring web层依赖：web依赖 + mvc依赖 -->
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    
    <!--4)spring测试依赖  -->
    <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>4.1.7.RELEASE</version>
    </dependency>
    </dependencies>
    <build /> 
</project>
```
二：项目分析
1：秒杀操作
秒杀操作其实就是对数据库库存的减操作，并记录这个动作以及结果。这个“减”+“记录”是原子的，所以需要事务来管理。
2：动作信息
用户执行秒杀操作，所需信息主要有：谁？什么时候？结果如何？
3：高并发瓶颈
整个秒杀过程归根结底就是对库存的“减”操作的执行。一个减操作，对应库存表一行商品记录的修改；多个用户秒杀这个商品，则对应多个对这行的操作。这些同时发起的对同一行的操作该如何安排顺序？
数据库本身采用行级锁来管理这些并发的操作：对同一行，某一时刻只能有一个事务操作它。
那么一旦事务多了起来，比如上万条秒杀操作该如何调度？这就是高并发的瓶颈所在。
三：功能实现
1：秒杀网址暴露
到了秒杀时间才开放秒杀网址
2：执行秒杀
3：秒杀后的信息查询
四：数据库设计
```
CREATE DATABASE seckill;
USE seckill;
CREATE TABLE seckill(
`seckill_id` BIGINT NOT NULL AUTO_INCREMENT COMMENT '商品库存id',
`name` VARCHAR(120) NOT NULL COMMENT '商品名称',
`number` INT NOT NULL COMMENT '库存数量',
`create_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
`start_time` TIMESTAMP NOT NULL COMMENT '秒杀开启时间',
`end_time` TIMESTAMP NOT NULL COMMENT '秒杀结束时间',
PRIMARY KEY(seckill_id),
KEY idx_start_time(start_time),
KEY idx_end_time(end_time),
KEY idx_create_time(create_time)
)ENGINE=INNODB AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 COMMENT='秒杀库存表';
`success_killed`
CREATE TABLE success_killed(
`seckill_id` BIGINT NOT NULL COMMENT '秒杀商品id',
`user_phone` BIGINT NOT NULL COMMENT '用户手机号',
`state` TINYINT NOT NULL DEFAULT -1 COMMENT '状态标识：-1：无效 0：成功 1：已付款',2：已发货
`create_time` TIMESTAMP NOT NULL COMMENT '创建时间',
PRIMARY KEY(seckill_id,user_phone),/*联合主键*/
KEY ids_create_time(create_time)
)ENGINE=INNODB DEFAULT CHARSET = utf8 COMMENT='秒杀成功明细表';
```
