# Hibernate 自动生成bo 映射是 default_schema 引发的问题 - z69183787的专栏 - CSDN博客
2014年04月12日 11:49:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7806
最近在做一个项目，使用Spring+Struts2+JPA(Hibernate)的框架，项目接近尾声，现在要为测试和以后的正式运营开始做准备了。但是突然发现了一个小问题，发现使用MyEclipse自动生成的持久化对象所对应的Schema信息都是直接在类内部使用注解声明的方式来设置的，类似如下的格式@Table(name = "xxx", schema="xxx")。
这样子就带来一个坏处，就是数据库的schema肯定是会有变更需求的，而把schema直接写死在持久化类中，等于每次变化schema的名字都要修改一遍所有的PO对象的注解声明，还要对类进行重新编译，非常麻烦的事情。
想到以前的hibernate也是有分表配置方式和系统统一配置方式的，当然hibernate的配置都是在xml配置文件中，修改都不需要重新编译类文件，相对来说问题不大。但是对于JPA的注解方式就影响太大了。而这次开发也正好使用的Hibernate作为JPA的实现，所以，考虑可以使用Hibernate统一配置方式，给JPA的配置加一个hibernate.default_schema的property属性。
<property name="hibernate.default_schema" value="myschema"/>
然后去掉了PO类中的注解，测试一下，一切OK。
使用JPA还是非常方便的，很多时候就是把作为Provider的原框架包的元素变为property的属性就可以了。
要把Provider变为其他的比如Toplink、OpenJPA也同样处理即可。
而使用接口编程的优势，在这里又一次被印证了。
当然，JPA还提供了另外一种方式来定义，就是使用可选的orm.xml文件，可以在该文件中指定默认的schema。
Oralce数据库环境下，有两个Scheme ，如Scheme 1与Scheme 2；并且Scheme 1的user具有DBA权限，能够访问到Scheme 2中的T_CONFIG表(假设T_CONFIG表在Scheme 2中已经存在)。那么在Scheme 1中使用Hibernate的自动创建表T_CONFIG时，可能出现在Scheme 1中不能创建T_CONFIG表的情况。
无意中发现这个问题的，启动项目时，没有正常打印出INFO org.hibernate.tool.hbm2ddl.TableMetadata - foreign keys: [] 日志，本来以为是Hibernate不能自动创建表的原因，于是重新建了个用户，只给予此用户Resouce与connect角色权限，配置好后重启应用发现能够自动创建表(但是还是没看到校验的日志信息)；诧异了中看了下Hibernate的源码，发现如果不配置default_schema那么程序将使用默认的Environment.DEFAULT_SCHEMA(从hibernate.properties文件中获取，hibernate.properties没有配置就是null)。
解决方法，如果是用了spring+hibernate配置如下:
<bean name="hibernateProperties" class="org.springframework.beans.factory.config.PropertiesFactoryBean">
<property name="properties">
<props>
<prop key="hibernate.default_schema">${hibernate.default_schema}</prop>
<!--其他配置-->
</props>
</property>
</bean>
至于校验信息不能打印出来是由于slf4j两个包冲突了，去掉slf4j-log4j12就可以正常显示了。
如果不想使用Hibernate的自动创建表功能，只要不设置此参数局可以了，spring默认注入此参数是false
<property name="schemaUpdate" value="${hibernate.schema_update}"/>
