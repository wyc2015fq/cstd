# 微服务配置中心实战：Spring + MyBatis + Druid + Nacos - 纯洁的微笑 - CSDN博客
2018年12月20日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：558
很多基于 Spring MVC 框架的 Web 开发中，Spring + MyBatis + Druid 是一个黄金组合，在此基础上如果融入一个配置中心，会发生什么特别的变化呢？
本文将通过一个用户信息查询示例，演示在 Spring Web 项目中如何将数据库连接池的配置存放到 Nacos 中，进行统一运维管控，达到配置治理与降低数据泄露风险的目的。
Nacos是今年开源的一款帮助构建云原生应用的动态服务发现、配置和服务管理平台，提供注册中心、配置中心和动态DNS服务三大功能。项目地址：https://github.com/alibaba/nacos
# 数据表
在测试数据库中新建 `user` 表，其中包含用户名称等字段，与接下来的 User model 类相对应。
```
CREATE TABLE `user` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(10) NOT NULL DEFAULT '' COMMENT '名字',
  `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户表';
```
添加一行测试数据：
```
INSERT INTO `user` (`name`, `create_time`, `update_time`) 
    VALUES ('Nacos', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);
```
# Spring
该示例是 Spring 常规的 Web 项目，项目结构如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnoDtKaCDxCHPvSLXeBVEydlJy4aTk4C0SXeg0U4lybqZIsyCvGxmCVicichMjYTFgmAicArWlnYSmSHg/640?wx_fmt=png)image.png | left | 301x454
## pom.xml
引入 Nacos Spring 的依赖包 `nacos-spring-context`：
```xml
<dependency>
    <groupId>com.alibaba.nacos</groupId>
    <artifactId>nacos-spring-context</artifactId>
    <version>${latest.version}</version>
</dependency>
```
笔者在撰写本文时，nacos-spring-context 的最新版本为：0.2.2-RC1
## dispatcher-servlet.xml
`dispatcher-servlet.xml` 为示例中 Spring MVC 的入口配置，在其中通过 `import`引入了 Nacos、Druid、MyBatis 的配置，其内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xmlns="http://www.springframework.org/schema/beans"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/mvc
       http://www.springframework.org/schema/mvc/spring-mvc.xsd">
    <mvc:annotation-driven/>
    <context:annotation-config/>
    <context:component-scan base-package="com.alibaba.nacos.example.spring"/>
    <import resource="nacos.xml"/>
    <import resource="datasource.xml"/>
    <import resource="spring-config-mybatis.xml"/>
</beans>
```
## nacos.xml
关键看 `nacos.xml` ，nacos-spring-context 的扩展了 Spring 的 XML Schema 机制，自定义了 `<nacos:property-source/>` 等元素，详细配置内容如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns="http://www.springframework.org/schema/beans"
       xmlns:nacos="http://nacos.io/schema/nacos"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://nacos.io/schema/nacos
       http://nacos.io/schema/nacos.xsd">
    <nacos:global-properties server-addr="127.0.0.1:8848" />
    <nacos:property-source data-id="datasource.properties"/>
</beans>
```
其中通过 `<nacos:global-properties />` 设置 Nacos Server 的连接地址，通过 `<nacos:property-source />` 从 Nacos 配置中心加载了 dataId 为 `datasource.properties` 的配置，nacos-spring-context 会解析获取到的配置内容并添加到 Spring Environment 的 PropertySources 中，使得后续初始化 Druid 连接池的时候能获取到数据库连接地址、账号密码、初始连接池大小等信息。这就是 Nacos 配置中心与 Spring 结合的关键点。
## datasource.xml
这是数据库连接池的配置，初始化了 `DruidDataSource` 的 Spring Bean，并将其通过 `DataSourceTransactionManager` 设置为 Spring 的数据库连接池。
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx.xsd
       http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"
          init-method="init" destroy-method="close">
        <property name="url" value="${datasource.url}"/>
        <property name="username" value="${datasource.username}"/>
        <property name="password" value="${datasource.password}"/>
        <property name="initialSize" value="${datasource.initial-size}"/>
        <property name="maxActive" value="${datasource.max-active}"/>
    </bean>
    <bean id="txManager"
          class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"/>
    </bean>
    <tx:annotation-driven transaction-manager="txManager"/>
</beans>
```
从上面的配置内容看一看到，数据库连接池不需要因为引入 Nacos 配置中做任何特殊的改变。
## 其他
User 的 Model、Service 等也跟不使用 Nacos 配置中心时完全一致，这里就不一一贴出，完整示例代码可以在 nacos-examples 获取：https://github.com/nacos-group/nacos-examples/tree/master/nacos-spring-example/nacos-spring-config-datasource-example
# Nacos
参照 Nacos 官网的快速开始：https://nacos.io/zh-cn/docs/quick-start.html
从 Nacos 的 github 上下载最新稳定版本的 nacos-server：https://github.com/alibaba/nacos/releases
解压后到 Nacos 的 bin 目录下，执行 startup 启动脚本以单机模式启动 Nacos Server， Windows 请使用 `cmd startup.cmd` ，Linux/Unix/Mac 环境请使用 `sh startup.sh -m standalone` 。
启动后，浏览器访问：http://localhost:8848/nacos/index.html 就可以来到 Nacos 的控制台，新增 dataId 为 `datasource.properties` 的配置，对应上面 Spring 的 `nacos.xml` 中的 dataId。
配置内容则与 Spring 的 `datasource.xml` 中的连接池属性一一对应，示例如下：
```
datasource.url=jdbc:mysql://localhost:3306/test
datasource.username=root
datasource.password=root
datasource.initial-size=1
datasource.max-active=20
```
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnoDtKaCDxCHPvSLXeBVEydlLdtDajuyl2khmaeYiavfsFfE4HB77nXRYMdDh3icXn7UuPqUQRxNBopQ/640?wx_fmt=png)image.png | left | 647x490
# 运行
示例中是 `UserController#get()` 通过 UserServce 调用 Mybatis 的 Mapper 类（UserMapper）从数据库中查询指定 ID 的用户信息，假设该示例是运行在端口为 8080 的 Tomcat 上，访问：http://localhost:8080/users?id=1 地址将返回：
```
{
  "id": 1,
  "name": "Nacos"
}
```
# 总结
本文通过一个示例演示在“原生” Spring 中如何使用 Nacos 配置中心，从示例可以看出，对原有的 Spring 项目基本没有任何侵入，只需在 pom.xml 中添加 nacos-spring-context 的依赖，然后再定义并引入 `nacos.xml` 配置，就可以将数据库连接池信息管控起来，做到统一运维，并降低数据泄露的风险。
试想，如果你有多个项目连接同一个数据库或一个项目部署很多实例，当数据库密码修改时，你不需要去修改每个项目的 `datasource.properties` 文件，再走一次应用的部署发布流程，而是到 Nacos 的控制台上修改一个配置项，再去重启应用实例即可。当然，如果你是自己管理数据库连接池，则可以做到连“重启应用实例”都不需要了，只需在监听到 Nacos 配置变化时重新初始化数据库连接池即可。
将 Spring 配置放置到 Nacos 配置中，还能用上“动态推送”、“版本管理”、“快速回滚”、“监听查询”，以及后续的 “灰度发布”、“配置加密”、“权限管控”等功能，为 Spring + MyBatis + Druid 插上“飞翔”的翅膀。
完整示例代码：
https://github.com/nacos-group/nacos-examples/tree/master/nacos-spring-example/nacos-spring-config-datasource-example
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
