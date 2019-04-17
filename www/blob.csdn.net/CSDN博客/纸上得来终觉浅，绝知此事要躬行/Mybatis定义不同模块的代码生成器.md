# Mybatis定义不同模块的代码生成器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年09月28日 14:17:45[boonya](https://me.csdn.net/boonya)阅读数：652










前提：eclipse已安装MBG（MyBatis Generator）。

Mybatis Generator插件怎么使用请参考：[https://my.oschina.net/boonya/blog/719502](https://my.oschina.net/boonya/blog/719502)。

更多代码生成器介绍请参考：[http://www.mybatis.org/generator/index.html](http://www.mybatis.org/generator/index.html)


根据项目模块来生成对应的Mapper，这里采用Maven构建项目。首先将需要的数据库连接jar放置到系统C盘下面，如：C:\mysql-connector-java-5.1.12.jar。

# **Maven依赖配置**



```
<dependency>
			<groupId>org.mybatis</groupId>
			<artifactId>mybatis</artifactId>
			<version>3.1.1</version>
		</dependency>
		<dependency>
			<groupId>org.mybatis.generator</groupId>
			<artifactId>mybatis-generator-maven-plugin</artifactId>
			<version>1.3.2</version>
		</dependency>
```



# **Mapper生成配置**

文件放置于Resource目录下：

1.以库存模块为例的配置:mybatisGenerator-stock.xml
```xml

```




```
<?xml version="1.0" encoding="UTF-8"?>   
<!DOCTYPE generatorConfiguration   
  PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"  
  "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">

<generatorConfiguration>

	<classPathEntry location="C:\mysql-connector-java-5.1.12.jar" />

	<context id="MySqlTables" targetRuntime="MyBatis3">

		<commentGenerator>
			<property name="suppressAllComments" value="true" />
		</commentGenerator>

		<jdbcConnection driverClass="com.mysql.jdbc.Driver" connectionURL="jdbc:mysql://192.168.200.126:3306/wms?characterEncoding=utf8" userId="root" password="wms123456">
		</jdbcConnection>

		<javaTypeResolver>
			<property name="forceBigDecimals" value="false" />
		</javaTypeResolver>

		<javaModelGenerator targetPackage="com.wlyd.wms.persistence.beans.stock" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
			<property name="trimStrings" value="true" />
			<property name="rootInterface" value="BaseBean" /> 
		</javaModelGenerator> 


		<sqlMapGenerator targetPackage="com.wlyd.wms.persistence.mapper.stock" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
		</sqlMapGenerator>


		<javaClientGenerator type="XMLMAPPER" targetPackage="com.wlyd.wms.persistence.mapper.stock" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
			<property name="rootInterface" value="BaseMapper" />
		</javaClientGenerator>


		 <table tableName="wm_stock_10000" domainObjectName="WmStock"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
		<table tableName="wm_stock_transfer_10000" domainObjectName="WmStockTransfer"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
		<table tableName="wm_stock_adjust_10000" domainObjectName="WmStockAdjust"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
		<table tableName="wm_stock_assigned_10000" domainObjectName="WmStockAssigned"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
		<table tableName="wm_stock_freeze_10000" domainObjectName="WmStockFreeze"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
		<table tableName="wm_stock_release_10000" domainObjectName="WmStockRelease"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
	</context>

</generatorConfiguration>
```


2.以入库模块为例的配置:mybatisGenerator-into.xml
```xml

```





```
<?xml version="1.0" encoding="UTF-8"?>   
<!DOCTYPE generatorConfiguration   
  PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"  
  "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">

<generatorConfiguration>

	<classPathEntry location="C:\mysql-connector-java-5.1.12.jar" />

	<context id="MySqlTables" targetRuntime="MyBatis3">

		<commentGenerator>
			<property name="suppressAllComments" value="true" />
		</commentGenerator>

		<jdbcConnection driverClass="com.mysql.jdbc.Driver" connectionURL="jdbc:mysql://192.168.200.126:3306/wms?characterEncoding=utf8" userId="root" password="wms123456">
		</jdbcConnection>

		<javaTypeResolver>
			<property name="forceBigDecimals" value="false" />
		</javaTypeResolver>

		<javaModelGenerator targetPackage="com.wlyd.wms.persistence.beans.into" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
			<property name="trimStrings" value="true" />
			<property name="rootInterface" value="BaseBean" /> 
		</javaModelGenerator> 


		<sqlMapGenerator targetPackage="com.wlyd.wms.persistence.mapper.into" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
		</sqlMapGenerator>


		<javaClientGenerator type="XMLMAPPER" targetPackage="com.wlyd.wms.persistence.mapper.into" targetProject="ebwms\src\main\java">
			<property name="enableSubPackages" value="true" />
			<property name="rootInterface" value="BaseMapper" />
		</javaClientGenerator>


		<table tableName="wm_into_order_10000" domainObjectName="WmIntoOrder"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>

       <table tableName="wm_into_orderinfo_10000" domainObjectName="WmIntoOrderinfo"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
		</table>
		
	    <table tableName="wm_receipt_info_10000" domainObjectName="WmReceiptInfo"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
	    </table>
		
		 <table tableName="wm_shelve_info_10000" domainObjectName="WmShelveInfo"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
	    </table>
		
		 <table tableName="wm_into_order_codeno_10000" domainObjectName="WmIntoOrderCodeno"  enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false" enableSelectByExample="false"
			selectByExampleQueryId="false">
	    </table>
		
	</context>

</generatorConfiguration>
```



# **注意事项**

配置文件中不能使用<!-- -->注释。执行生成脚本前请先创建对应的代码路径。

更多MyBatis Generator的说明请查看：http://www.mybatis.org/generator/index.html

项目生成效果：

![](https://img-blog.csdn.net/20160928141533354)



示例demo：[Mybatis Mapper生成器示例](http://download.csdn.net/detail/boonya/9642055)。












