# mysql : utf8mb4 的问题 - z69183787的专栏 - CSDN博客
2018年04月24日 12:11:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：107
微信呢称和QQ呢称上有很多火星文和emoji表情图片，这些数据，如果直接insert到mysql数据库，一般会报错，设置成utf8都不好使，必须改成utf8mb4编码，这二者的区别见： [mysql utf8mb4与emoji表情](http://my.oschina.net/wingyiu/blog/153357) ，网上的解决办法大多是修改my.cnf参数，设置mysql的编码为utf8mb4，这种方法虽然彻底，但是通常要重启mysql，会造成生产系统临时当机。
下面是影响相对更小的处理方法：
前提：mysql的版本不能太低，低于5.5.3的版本不支持utf8mb4编码。
将表中的对应字段，比如会员表的呢称字段，其字符集修改成utf8mb4。
然后在java客户端，将mysql-connector-java升级成最新版（目前最新版是5.1.37），最后修改druid数据源的配置，增加一行：
`<property name="connectionInitSqls" value="set names utf8mb4;"/>`
完整配置参考下面：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" init-method="init" destroy-method="close">
        <property name="driverClassName" value="${jdbc-driver}"/>
        <property name="url" value="${jdbc-url}"/>
        <property name="username" value="${jdbc-user}"/>
        <property name="password" value="${jdbc-password}"/>
        <property name="filters" value="stat"/>
        <property name="maxActive" value="20"/>
        <property name="initialSize" value="1"/>
        <property name="maxWait" value="60000"/>
        <property name="minIdle" value="1"/>
        <property name="timeBetweenEvictionRunsMillis" value="3000"/>
        <property name="minEvictableIdleTimeMillis" value="300000"/>
        <property name="validationQuery" value="SELECT 'x'"/>
        <property name="testWhileIdle" value="true"/>
        <property name="testOnBorrow" value="false"/>
        <property name="testOnReturn" value="false"/>
        <property name="poolPreparedStatements" value="true"/>
        <property name="maxPoolPreparedStatementPerConnectionSize" value="20"/>
        <property name="connectionInitSqls" value="set names utf8mb4;"/>
    </bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
基本上应该就可以了，如果还不行，检查下jdbc连接串的设置：
jdbc:mysql://localhost:3306/db名称?useUnicode=true&characterEncoding=utf8
再不行的话，把最后的?useUnicode=true&characterEncoding=utf8 去掉
