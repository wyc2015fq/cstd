# 【SSM框架】mybatis生成代码的坑还真不少 - Big Smile - CSDN博客
2017年08月28日 00:04:45[王啸tr1912](https://me.csdn.net/tr1912)阅读数：377
        有幸在公司搭建SSM框架，期间碰到了不少小问题，但是都如数解决了，可是到了最后这一个问题说什么都不行了，连着调试了一天才终于解决了，现在来分享一下。
# 一、mybatis代码生成工具generatorSqlmapCustom
        我们在项目中，用到了这个工具来生成代码，具体用处也不多讲了，直接上配置文件一目了然，他的主要功能就是可以根据连接的数据库生成pojo，dao和mapper.xml层，配置文件如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
  PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
  "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
<generatorConfiguration>
	<context id="testTables" targetRuntime="MyBatis3">
		<!-- 序列化插件 -->
		<plugin type="org.mybatis.generator.plugins.SerializablePlugin" />
		<commentGenerator>
			<!-- 是否去除自动生成的注释 true：是 ： false:否 -->
			<property name="suppressAllComments" value="true" />
		</commentGenerator>
		<!--数据库连接的信息：驱动类、连接地址、用户名、密码 -->
		<jdbcConnection driverClass="com.mysql.jdbc.Driver"
			connectionURL="jdbc:mysql://IP:PORT/yichuDev" userId="root"
			password="1234">
		</jdbcConnection>
		<!-- 默认false，把JDBC DECIMAL 和 NUMERIC 类型解析为 Integer，为 true时把JDBC DECIMAL 
			和 NUMERIC 类型解析为java.math.BigDecimal -->
		<javaTypeResolver>
			<property name="forceBigDecimals" value="true" />
		</javaTypeResolver>
		<!-- targetProject:生成PO类的位置 -->
		<javaModelGenerator targetPackage="com.cn.echuxianshengshop.pojo"
			targetProject=".\src">
			<!-- enableSubPackages:是否让schema作为包的后缀 -->
			<property name="enableSubPackages" value="false" />
			<!-- 从数据库返回的值被清理前后的空格 -->
			<property name="trimStrings" value="true" />
		</javaModelGenerator>
		<!-- targetProject:mapper映射文件生成的位置 -->
		<sqlMapGenerator targetPackage="mapper.UserMapper.xml"
			targetProject=".\src">
			<!-- enableSubPackages:是否让schema作为包的后缀 -->
			<property name="enableSubPackages" value="false" />
		</sqlMapGenerator>
		<!-- targetPackage：mapper接口生成的位置 -->
		<javaClientGenerator type="XMLMAPPER"
			targetPackage="com.cn.echuxianshengshop.dao" targetProject=".\src">
			<!-- enableSubPackages:是否让schema作为包的后缀 -->
			<property name="enableSubPackages" value="false" />
		</javaClientGenerator>
		<!-- 指定数据库表 -->
		<table schema="" tableName="t_bom"></table>
		<table schema="" tableName="t_acceptanceform"></table>
		<table schema="" tableName="t_abalancerepository"></table>
		<table schema="" tableName="t_examlogo"></table>				
	</context>
</generatorConfiguration>
```
        我们可以看到生成代码的一些条件都在上面能够配置，然后运行程序中的GeneratorSqlmap.java中的main函数就可以了，生成的包都在src的下面。
# 二、mapper报告异常
Caused by: org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'sqlSessionFactory' defined in class path resource [spring-mybatis.xml]: Invocation of init method failed; nested exception is
 org.springframework.core.NestedIOException: Failed to parse mapping resource: 'file [D:\Project\echuxianshengshop-parent\target\echuxianshengshop-parent\WEB-INF\classes\mapper\TOutputorderbMapper.xml]'; nested exception is org.apache.ibatis.builder.BuilderException:
 Error parsing Mapper XML. Cause: java.lang.IllegalArgumentException: Result Maps collection already contains value for com.cn.echuxianshengshop.dao.TOutputorderbDao.BaseResultMap
        首先映入眼帘的就是这样的一堆问题，日志处理的框框都放不下了，只是截取了一部分给我们，再仔细看，基本每一个大的条报告的错误都是查不多的，说有重复的XXX，然后不符合springFramework的规则，所以我们就开始排查，然后发现可能是分Mapper的原因（由于有自动生成的和自己写的两种，所以要分），所以把生成的就都删除了，结果，还是不行，到底是为什么呢？
        在网上苦苦搜寻，发现的都是说idea解析不了xml文件，需要引用什么东西在pom里面，但是依然不成功。后来搜索到了一篇博客，里面对于这种情况的发生原因描述的很是清楚：
[http://blog.csdn.net/zengdeqing2012/article/details/46340357](http://blog.csdn.net/zengdeqing2012/article/details/46340357)
        大概意思就是说mapping或者是resultmapper的ID有重复的地方，还有返回值设定的问题，我们根据日志还有这篇文档一排查，果然出现了问题，由于生成代码的工具的xml中写了两个一样的表名，所以导致了那个表的mapper.xml中有两套一模一样的代码，所以出现了很多的问题。
# 三、整合注入不恰当
        mybatis没有问题了之后，我们开始一条线的联调，由于service层分为接口和实现两个部分，所以在注入的时候只是注入了暴露的接口，却没有注入实现，所以在调用的时候到了service层就直接返回为空的错误了，最后在spring-mybatis中加入了service的实现类注入，方才起了作用。
