# EMoji表情 Java+Mysql 存储 - z69183787的专栏 - CSDN博客
2017年05月05日 14:11:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1844
具体修改方式网上都有，
背景：我们的数据库是5.6+版本，如果5.7+版本的话，mysql connector只要高于 **5.1.34** 及以上就性了
而我们只能指定 mysql connector5.1.13 这个 jdbc connector版本，才能做到在5.6+的数据库情况下存入emoji表情
** 低版本的不支持utf8mb4高版本的修复了bug  需要db的 character-set-server是 utf8mb4这个版本刚好是默认用utf8mb4连。**
前端接口请求：
![](https://img-blog.csdn.net/20170505141022331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
数据库中存储后形式为：
![](https://img-blog.csdn.net/20170505140942351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
返回：
![](https://img-blog.csdn.net/20170505141113067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 如何支持utf8mb4字符集？druid连接池池中的一个参数connectionInitSqls该如何在zebra中对应或者处理？
A1: 检查连接mysql的版本 如果是**mysql5.7或mysqlserver的collation_server为****utf8_general_ci**,将mysql-connector-java升级到5.1.33及以上版本后可以直接支持。
A2: 一般情况下，业务方在使用这个参数时，都是设置编码方式："set names utf8mb4"。要求mysql-connector-java要在5.1.20以上，poolType不能是c3p0，其他的类型都能支持，这里以Druid为例
connectionInitSqls：物理连接初始化的时候执行的sql
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" init-method="init" destroy-method="close"><property name="driverClassName" value="${jdbc-driver}"/><property name="url" value="${jdbc-url}"/><property name="username" value="${jdbc-user}"/><property name="password" value="${jdbc-password}"/><property name="filters" value="stat"/><property name="maxActive" value="20"/><property name="initialSize" value="1"/><property name="maxWait" value="60000"/><property name="minIdle" value="1"/><property name="timeBetweenEvictionRunsMillis" value="3000"/><property name="minEvictableIdleTimeMillis" value="300000"/><property name="validationQuery" value="SELECT 'x'"/><property name="testWhileIdle" value="true"/><property name="testOnBorrow" value="false"/><property name="testOnReturn" value="false"/><property name="poolPreparedStatements" value="true"/><property name="maxPoolPreparedStatementPerConnectionSize" value="20"/>**<property name="connectionInitSqls" value="set names utf8mb4"/>**</bean>
