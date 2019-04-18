# spring的DriverManagerDataSource与apache的BasicDataSource（转） - z69183787的专栏 - CSDN博客
2012年12月28日 16:10:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：922
情况是这样的。。。
2008-3-18 1:08:26 org.apache.tomcat.util.threads.ThreadPool logFull
严重: All threads (150) are currently busy, waiting. Increase maxThreads (150) or check the servlet status
重新启动服务器之后，问题依然存在。
分析得出以下可能情况
1.连接数达到了150的最大上限
2.服务器端响应用户请求的时间过长导致
3.连接池的配置数出了问题
分析：
1.1个用户访问是OK的，当用2个用户对localhost进行访问，所以根本不可能达到并发访问线程的150的上限，所以应该不是数据库的原因,排除了第一种可能
2.之前访问的JSP已经经过多次访问，所以不可能是第一次访问编译，运行而导致的请求时间过长的原因，第二情况也否定。
到此，经过分析可以确定的是连接池出了问题...
首先有2个知识点需要再弄的更清楚一些
DriverManager与DataSource  连接数据库有何区别?
        DriverManager传统的jdbc连接，通过Class.forName("XXX"),的办法注册之后，就可以DriverManager.getConnection()获得连接了。
        DataSource是建立在JNDI服务基础上的,需要application server配置datasource.首先需要注册一个DataSource(一般在/META-INF/context.xml下)然后在web.xml文件中引用这个DataSource,就可以DataSource.getConnection()获得连接,具体操作参考(tomcat目录里的JNDI Resources小节)
DataSource 与 DBCP pool(连接池) 的区别？
        J2EE 服务器启动时会建立一定数量的池连接，并一直维持不少于此数目的池连接。客户端程序需要连接时，池驱动程序会返回一个未使用的池连接并将其表记为忙。如果当前没有空闲连接，池驱动程序就新建一定数量的连接，新建连接的数量有配置参数决定。当使用的池连接调用完成后，池驱动程序将此连接表记为空闲，其他调用就可以使用这个连接。
相当于是优化了DataSource的一种工具
跟数据库连接的部分是通过Spring的DataSource JDBC连接的,配置的XML内容如下:
<bean id="propertyConfigurer" 
class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
<!--PropertyPlaceholderConfigurer类来读取xxx.properties配置文件信息,以key和value的形式-->
<property name="locations">
    <list>
        <value>
          /WEB-INF/classes/config/pkm/environment/jdbc.properties
        </value>
        <value>
          <!--多个xxx.properties文件-->
        </value>
    </list>
</property>
</bean>
<!--事实上是因为DriverManagerDataSource建立连接是只要有连接就新建一个connection,根本没有连接池的作用-->
<!--两种不同的DataSource-->
<!--单纯的DataSource-->
<bean id="pkmDataSource" 
class="org.springframework.jdbc.datasource.DriverManagerDataSource">
    <property name="driverClassName">
        <value>${pkm.jdbc.driverClassName}</value>
        <!--${pkm.jdbc.driverClassName}是jdbc.properties文件 中的key-->
    </property>
    <property name="url">
        <value>${pkm.jdbc.url}</value>
    </property>
    <property name="username">
        <value>${pkm.jdbc.username}</value>
    </property>
    <property name="password">
        <value>${pkm.jdbc.password}</value>
    </property>
</bean>
<!--连接池-->
<bean id="pkmDataSource" 
class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close" lazy-init="false">
    <property name="driverClassName" value="${pkm.jdbc.driverClassName}"/>
    <property name="url" value="${pkm.jdbc.url}"/>
    <property name="username" value="${pkm.jdbc.username}"/>
    <property name="password" value="${pkm.jdbc.password}"/>
    <property name="initialSize" value="5"/>
    <property name="maxActive" value="10"/>
    <property name="maxWait" value="60000"/>
    <property name="poolPreparedStatements" value="true"/>  
</bean>
        所以问题就出在这里,当使用BasicDataSource后问题不在出现,所以是连接数量造成的,在访问数量大,并发的情况下,毫无疑问是要选择连接池的,因为有连接池的功能,无论是效率还是在资源利用率上都优于DriverManagerDataSource
        从而从根本上了解了spring的DriverManagerDataSource与apacheBasicDataSource之间的区别.
有时候可能需要配置Jndi服务
<bean id="pkmDataSource" 
class="org.springframework.jndi.JndiObjectFactoryBean">
    <property name="jndiName" value="pkmDataSource"/> 
</bean>
<!--
这样的话部署的时候，需要在容器中（tomcat,weblogic）配置JDBC Connection Pool(DBCP)连接池
-->
这三种连接方式常常使用，也容易混淆，选择其中的某一种，就需要看具体环境来配置了。
PS：jdbc.properties文件中的配置如下
pkm.jdbc.driverClassName=oracle.jdbc.OracleDriver
pkm.jdbc.url=jdbc\:oracle\:thin\:@109.52.20.31\:1521\:orcl<!--把符号做转译-->
pkm.jdbc.name=pkmuser
pkm.jdbc.password=dbl0gin
pkm.jdbc.dataSource=pkmDataSource
