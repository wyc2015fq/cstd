# Sharding-JDBC - 为分库分表而生的数据库访问层微服务框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月09日 16:58:40[boonya](https://me.csdn.net/boonya)阅读数：2719








Sharding-JDBC - 为分库分表而生的数据库访问层微服务框架。从 3.0 开始，Sharding-JDBC 将更名为 Sharding-Sphere。

Sharding-JDBC是一个开源的分布式数据库中间件，它无需额外部署和依赖，完全兼容JDBC和各种ORM框架。Sharding-JDBC作为面向开发的微服务云原生基础类库，完整的实现了分库分表、读写分离和分布式主键功能，并初步实现了柔性事务。

在经历了整体架构的数次精炼及稳定性打磨后，如今它已积累了足够的底蕴，相信可以成为开发者技术选型时的一个参考。

官方中文地址：[http://shardingjdbc.io/index_zh.html](http://shardingjdbc.io/index_zh.html)

快速入门教程：[http://shardingjdbc.io/docs_cn/00-overview/](http://shardingjdbc.io/docs_cn/00-overview/)


Github源码地址：[https://github.com/sharding-sphere/sharding-sphere](https://github.com/sharding-sphere/sharding-sphere)

Github源码示例：[https://github.com/sharding-sphere/sharding-sphere-example](https://github.com/sharding-sphere/sharding-sphere-example)


OSChina码云：[https://gitee.com/shardingjdbc/sharding-jdbc](https://gitee.com/shardingjdbc/sharding-jdbc)

# 概述

Sharding-JDBC定位为轻量级java框架，使用客户端直连数据库，以jar包形式提供服务，未使用中间层，无需额外部署，无其他依赖，DBA也无需改变原有的运维方式，可理解为增强版的JDBC驱动，旧代码迁移成本几乎为零。

# 功能列表

## 1. 分库分表
- SQL解析功能完善，支持聚合，分组，排序，LIMIT，TOP等查询，并且支持级联表以及笛卡尔积的表查询
- 支持内、外连接查询
- 分片策略灵活，可支持=，BETWEEN，IN等多维度分片，也可支持多分片键共用，以及自定义分片策略
- 基于Hint的强制分库分表路由

## 2. 读写分离
- 一主多从的读写分离配置，可配合分库分表使用
- 基于Hint的强制主库路由

## 3. 柔性事务
- 最大努力送达型事务
- TCC型事务(TBD)

## 4. 分布式主键
- 统一的分布式基于时间序列的ID生成器

## 5. 兼容性
- 可适用于任何基于java的ORM框架，如：JPA, Hibernate, Mybatis, Spring JDBC Template或直接使用JDBC
- 可基于任何第三方的数据库连接池，如：DBCP, C3P0, BoneCP, Druid等
- 理论上可支持任意实现JDBC规范的数据库。目前支持MySQL，Oracle，SQLServer和PostgreSQL

## 6. 灵活多样的配置
- Java
- YAML
- Inline表达式
- Spring命名空间
- Spring boot starter

## 7. 分布式治理能力 (2.0新功能)
- 配置集中化与动态化，可支持数据源、表与分片策略的动态切换(2.0.0.M1)
- 客户端的数据库治理，数据源失效自动切换(2.0.0.M2)
- 基于Open Tracing协议的APM信息输出(2.0.0.M3)

# Architecture

![Architecture](http://ovfotjrsi.bkt.clouddn.com/docs/img/architecture_v2.png)

# [Release Notes](https://github.com/shardingjdbc/sharding-jdbc/releases)

# [Roadmap](https://gitee.com/shardingjdbc/sharding-jdbc/blob/master/ROADMAP.md)

# 快速入门

## 引入maven依赖


<!-- 引入sharding-jdbc核心模块 --><dependency><groupId>io.shardingjdbc</groupId><artifactId>sharding-jdbc-core</artifactId><version>${latest.release.version}</version></dependency>


## 规则配置

Sharding-JDBC的分库分表通过规则配置描述，请简单浏览配置全貌：


// 配置真实数据源Map<String,DataSource>dataSourceMap=newHashMap<>();// 配置第一个数据源BasicDataSourcedataSource1=newBasicDataSource();dataSource1.setDriverClassName("com.mysql.jdbc.Driver");dataSource1.setUrl("jdbc:mysql://localhost:3306/ds_0");dataSource1.setUsername("root");dataSource1.setPassword("");dataSourceMap.put("ds_0",dataSource1);// 配置第二个数据源BasicDataSourcedataSource2=newBasicDataSource();dataSource2.setDriverClassName("com.mysql.jdbc.Driver");dataSource2.setUrl("jdbc:mysql://localhost:3306/ds_1");dataSource2.setUsername("root");dataSource2.setPassword("");dataSourceMap.put("ds_1",dataSource2);// 配置Order表规则TableRuleConfigurationorderTableRuleConfig=newTableRuleConfiguration();orderTableRuleConfig.setLogicTable("t_order");orderTableRuleConfig.setActualDataNodes("ds_${0..1}.t_order_${[0, 1]}");// 配置分库策略orderTableRuleConfig.setDatabaseShardingStrategyConfig(newInlineShardingStrategyConfiguration("user_id","ds_${user_id % 2}"));// 配置分表策略orderTableRuleConfig.setTableShardingStrategyConfig(newInlineShardingStrategyConfiguration("order_id","t_order_${order_id % 2}"));// 配置分片规则ShardingRuleConfigurationshardingRuleConfig=newShardingRuleConfiguration();shardingRuleConfig.getTableRuleConfigs().add(orderTableRuleConfig);// 省略配置order_item表规则...// 获取数据源对象DataSourcedataSource=ShardingDataSourceFactory.createDataSource(dataSourceMap,shardingRuleConfig);


或通过YAML方式配置，与以上配置等价：


dataSources:ds_0:!!org.apache.commons.dbcp.BasicDataSourcedriverClassName:com.mysql.jdbc.Driverurl:jdbc:mysql://localhost:3306/ds_0username:rootpassword:ds_1:!!org.apache.commons.dbcp.BasicDataSourcedriverClassName:com.mysql.jdbc.Driverurl:jdbc:mysql://localhost:3306/ds_1username:rootpassword:shardingRule:tables:t_order:actualDataNodes:ds_${0..1}.t_order_${0..1}databaseStrategy:inline:shardingColumn:user_idalgorithmExpression:ds_${user_id % 2}tableStrategy:inline:shardingColumn:order_idalgorithmExpression:t_order_${order_id % 2}t_order_item:actualDataNodes:ds_${0..1}.t_order_item_${0..1}databaseStrategy:inline:shardingColumn:user_idalgorithmExpression:ds_${user_id % 2}tableStrategy:inline:shardingColumn:order_idalgorithmExpression:t_order_item_${order_id % 2}



DataSourcedataSource=ShardingDataSourceFactory.createDataSource(yamlFile);


规则配置包括数据源配置、表规则配置、分库策略和分表策略组成。这只是最简单的配置方式，实际使用可更加灵活，如：多分片键，分片策略直接和表规则配置绑定等。

## 使用原生JDBC接口

通过ShardingDataSourceFactory工厂和规则配置对象获取ShardingDataSource，ShardingDataSource实现自JDBC的标准接口DataSource。然后可通过DataSource选择使用原生JDBC开发，或者使用JPA, MyBatis等ORM工具。以JDBC原生实现为例：


DataSourcedataSource=ShardingDataSourceFactory.createDataSource(dataSourceMap,shardingRuleConfig);Stringsql="SELECT i.* FROM t_order o JOIN t_order_item i ON o.order_id=i.order_id WHERE o.user_id=? AND o.order_id=?";try(Connectionconn=dataSource.getConnection();PreparedStatementpreparedStatement=conn.prepareStatement(sql)){preparedStatement.setInt(,);preparedStatement.setInt(,);try(ResultSetrs=preparedStatement.executeQuery()){while(rs.next()){System.out.println(rs.getInt());System.out.println(rs.getInt());}}}


## 使用Spring命名空间配置


<?xml version="1.0"encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"xmlns:sharding="http://shardingjdbc.io/schema/shardingjdbc/sharding"xsi:schemaLocation="http://www.springframework.org/schema/beans 
                        http://www.springframework.org/schema/beans/spring-beans.xsd
                        http://www.springframework.org/schema/context 
                        http://www.springframework.org/schema/context/spring-context.xsd 
                        http://shardingjdbc.io/schema/shardingjdbc/sharding 
                        http://shardingjdbc.io/schema/shardingjdbc/sharding/sharding.xsd 
                        "><context:property-placeholderlocation="classpath:conf/conf.properties"ignore-unresolvable="true"/><beanid="ds_0"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"><propertyname="driverClassName"value="com.mysql.jdbc.Driver"/><propertyname="url"value="jdbc:mysql://localhost:3306/ds_0"/><propertyname="username"value="root"/><propertyname="password"value=""/></bean><beanid="ds_1"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"><propertyname="driverClassName"value="com.mysql.jdbc.Driver"/><propertyname="url"value="jdbc:mysql://localhost:3306/ds_1"/><propertyname="username"value="root"/><propertyname="password"value=""/></bean><sharding:inline-strategyid="databaseStrategy"sharding-column="user_id"algorithm-expression="ds_${user_id % 2}"/><sharding:inline-strategyid="orderTableStrategy"sharding-column="order_id"algorithm-expression="t_order_${order_id % 2}"/><sharding:inline-strategyid="orderItemTableStrategy"sharding-column="order_id"algorithm-expression="t_order_item_${order_id % 2}"/><sharding:data-sourceid="shardingDataSource"><sharding:sharding-ruledata-source-names="ds_0,ds_1"><sharding:table-rules><sharding:table-rulelogic-table="t_order"actual-data-nodes="ds_${0..1}.t_order_${0..1}"database-strategy-ref="databaseStrategy"table-strategy-ref="orderTableStrategy"/><sharding:table-rulelogic-table="t_order_item"actual-data-nodes="ds_${0..1}.t_order_item_${0..1}"database-strategy-ref="databaseStrategy"table-strategy-ref="orderItemTableStrategy"/></sharding:table-rules></sharding:sharding-rule></sharding:data-source></beans>





# 详细功能列表

## 功能性需求

### 分库分表
- SQL解析功能完善，支持聚合，分组，排序，LIMIT，TOP等查询，并且支持级联表以及笛卡尔积的表查询
- 支持内、外连接查询
- 分片策略灵活，可支持=，BETWEEN，IN等多维度分片，也可支持多分片键共用，以及自定义分片策略
- 基于Hint的强制分库分表路由

### 读写分离
- 独立使用读写分离支持SQL透传
- 一主多从的读写分离配置，可配合分库分表使用
- 基于Hint的强制主库路由

### 柔性事务
- 最大努力送达型事务
- TCC型事务(TBD)

### 分布式主键
- 统一的分布式基于时间序列的ID生成器

### 兼容性
- 可适用于任何基于java的ORM框架，如：JPA, Hibernate, Mybatis, Spring JDBC Template或直接使用JDBC
- 可基于任何第三方的数据库连接池，如：DBCP, C3P0, BoneCP, Druid等
- 理论上可支持任意实现JDBC规范的数据库。目前支持MySQL，Oracle，SQLServer和PostgreSQL

### 灵活多样的配置
- Java
- YAML
- Inline表达式
- Spring命名空间
- Spring boot starter

### 分布式治理能力 (2.0新功能)
- 配置集中化与动态化，可支持数据源、表与分片策略的动态切换(2.0.0.M1)
- 客户端的数据库治理，数据源失效自动切换(2.0.0.M2)
- 基于Open Tracing协议的APM信息输出(2.0.0.M3)

## 性能
- 半理解”理念的SQL解析引擎
- 多线程处理结果归并
- 性能损失率约6%

## 稳定性
- 完善的疲劳测试，普通查询无Full GC（GROUP BY除外）
- 多数据库、分片策略和语句的完整单元测试



参考示例：[https://blog.csdn.net/u011116672/article/details/78374724](https://blog.csdn.net/u011116672/article/details/78374724)

注意：参考Sharding-JDBC使用限制，很多语法还是不支持的，用的时候仍然可能需要自己去开发。




