# JPA 不在 persistence.xml 文件中配置每个Entity实体类的2种解决办法 - xqhrs232的专栏 - CSDN博客
2018年11月08日 13:58:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：281
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/taven/p/3351841.html](https://www.cnblogs.com/taven/p/3351841.html)
相关文章
1、[Class is managed, but is not listed in the persistence.xml file](https://stackoverflow.com/questions/24040208/class-is-managed-but-is-not-listed-in-the-persistence-xml-file)----[https://stackoverflow.com/questions/24040208/class-is-managed-but-is-not-listed-in-the-persistence-xml-file](https://stackoverflow.com/questions/24040208/class-is-managed-but-is-not-listed-in-the-persistence-xml-file)
2、[Class “model.Address” is listed in the persistence.xml file but not mapped](https://stackoverflow.com/questions/2242637/class-model-address-is-listed-in-the-persistence-xml-file-but-not-mapped)----[https://stackoverflow.com/questions/2242637/class-model-address-is-listed-in-the-persistence-xml-file-but-not-mapped](https://stackoverflow.com/questions/2242637/class-model-address-is-listed-in-the-persistence-xml-file-but-not-mapped)
3、java EE 貌似注解出问题了（Class is listed in the persistence.xml file, but is not annotated）----[https://zhidao.baidu.com/question/455195802099390085.html](https://zhidao.baidu.com/question/455195802099390085.html)
4、Class "" is listed in the persistence.xml file , but is not annotated ----[http://elaine0111.iteye.com/blog/2250404](http://elaine0111.iteye.com/blog/2250404)
在Spring 集成 Hibernate 的JPA方式中，需要在persistence配置文件中定义每一个实体类，这样非常地不方便，远哥目前找到了2种方法。
这2种方式都可以实现不用persistence.xml文件，免去每个Entity都要在persistence.xml文件中配置的烦恼，但是这种方式Entity实体类的主键字段注解@ID要放到 getXXX()方法上，否则不认。
**方式1：**
**修改“LocalContainerEntityManagerFactoryBean”的配置，如下：**
<bean id="entityManagerFactory" class="org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean">
<property name="packagesToScan" value="com.pilicat.data.entity" />
<property name="jpaVendorAdapter">
<bean class="org.springframework.orm.jpa.vendor.HibernateJpaVendorAdapter"/>
</property>
<property name="jpaProperties">
            <props>
                <prop key="hibernate.connection.driver_class">${jdbc.driverClassName}</prop>
                <prop key="hibernate.connection.url">${jdbc.url}</prop>
                <prop key="hibernate.connection.username">${jdbc.username}</prop>
                <prop key="hibernate.connection.password">${jdbc.password}</prop>
                <prop key="hibernate.c3p0.min_size">10</prop>
                <prop key="hibernate.hbm2ddl.auto">true</prop>
                <prop key="hibernate.dialect">${hibernate.dialect}</prop>
            </props>
        </property>
</bean>
方式1没有使用 persistence 配置文件，注意咯！
**方式2：**
**修改“LocalContainerEntityManagerFactoryBean”的配置，如下：**
<bean id="entityManagerFactory" class="org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean">
<property name="dataSource" ref="dataSource" />
<property name="persistenceXmlLocation" value="classpath:persistence-app.xml" />
<!-- <property name="persistenceUnitName" value="pilicat_app_jpa" /> -->
<property name="packagesToScan" value="com.pilicat.data.entity" />
<property name="jpaVendorAdapter">
<bean class="org.springframework.orm.jpa.vendor.HibernateJpaVendorAdapter">
<property name="databasePlatform" value="${hibernate.dialect}" />
<property name="generateDdl" value="false" />
</bean>
</property>
</bean>
persistence-app.xml配置文件内容：
<?xml version="1.0" encoding="UTF-8"?>
<persistence version="2.0" xmlns="http://java.sun.com/xml/ns/persistence" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xsi:schemaLocation="http://java.sun.com/xml/ns/persistence 
http://java.sun.com/xml/ns/persistence/persistence_2_0.xsd">
<persistence-unit name="pilicat_app_jpa" transaction-type="RESOURCE_LOCAL">
<properties>
<property name="hibernate.max_fetch_depth" value="3" />
<property name="hibernate.jdbc.fetch_size" value="50" />
<property name="hibernate.jdbc.batch_size" value="50" />
<property name="hibernate.show_sql" value="true" />
<property name="hibernate.format_sql" value="false" />
</properties>
</persistence-unit>
</persistence>
方式2使用了 persistence 配置文件，去掉“persistenceUnitName”属性，添加“packagesToScan”属性，persistence.xml配置文件中的persistence-unit名字照样保留，但是 persistence 配置文件中不需要对实体类进行配置，会自动识别。
为什么去掉“persistenceUnitName”属性就可以自动识别实体了呢？看一下Spring的源码就知道了：
类名：org.springframework.orm.jpa.persistenceunit.DefaultPersistenceUnitManager
代码段：
`private``List<SpringPersistenceUnitInfo> readPersistenceUnitInfos() {`
`    ``List<SpringPersistenceUnitInfo> infos = ``new``LinkedList<SpringPersistenceUnitInfo>();`
`    ``boolean``buildDefaultUnit = (``this``.packagesToScan != ``null``|| ``this``.mappingResources != ``null``);`
`    ``PersistenceUnitReader reader = ``new``PersistenceUnitReader(``this``.resourcePatternResolver, ``this``.dataSourceLookup);`
`    ``SpringPersistenceUnitInfo[] readInfos = reader.readPersistenceUnitInfos(``this``.persistenceXmlLocations);`
`    ``for``(SpringPersistenceUnitInfo readInfo : readInfos) {`
`        ``infos.add(readInfo);`
`        ``if``(``this``.defaultPersistenceUnitName != ``null``&&`
`                ``this``.defaultPersistenceUnitName.equals(readInfo.getPersistenceUnitName())) {`
`            ``buildDefaultUnit = ``false``;`
`        ``}`
`    ``}`
`    ``if``(buildDefaultUnit) {`
`        ``infos.add(buildDefaultPersistenceUnitInfo());`
`    ``}`
`    ``return``infos;`
`}`
注意看这个源码的方法，defaultPersistenceUnitName 变量如果不为空，并且等于 persistence 配置文件中的持久化单元名称，则buildDefaultUnit就为false，buildDefaultUnit 如果为 false，是不会执行 buildDefaultPersistenceUnitInfo() 方法的，而 buildDefaultPersistenceUnitInfo() 方法是根据我们定义的 packagesToScan 去自动扫描Entity实体类的。　　
注：我使用的是 Spring 3.2.4
以上2种方法都测试通过，还有没有更简单的办法呢？你也可以将您的方式告诉远哥，或在远哥的博客下方留言，欢迎大家交流分享，谢谢。
