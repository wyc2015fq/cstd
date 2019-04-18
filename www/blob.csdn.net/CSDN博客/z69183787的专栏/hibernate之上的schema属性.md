# hibernate之上的schema属性 - z69183787的专栏 - CSDN博客
2014年04月12日 11:53:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9724
hibernate之<class>上的schema属性
----------
在oralce中，不同的用户，使用不同的schema. 但在sqlserver中，同一个用户可以有多个schema. 如果我们要使用非默认schema中的表如何处理？
我们可以设置默认的schema,如：
**[xhtml]**[view
 plain](http://blog.csdn.net/fhd001/article/details/6237019#)[copy](http://blog.csdn.net/fhd001/article/details/6237019#)
- <propertyname="Hibernate.default_schema">POI_BEIJING</property>
在使用不同schema的实例类的映射配置上使用schema属性，如例：
**[xhtml]**[view
 plain](http://blog.csdn.net/fhd001/article/details/6237019#)[copy](http://blog.csdn.net/fhd001/article/details/6237019#)
- <hibernate-mapping>
- <classname="com.csc.poimanager.dao.Poi"table="POI"schema="PPPP">
- <idname="poiId"type="java.lang.Long">
- <columnname="POI_ID"precision="10"scale="0"/>
- <generatorclass="increment"/>
- </id>
- <propertyname="cnName"type="java.lang.String">
- <columnname="CN_NAME"length="1000"/>
- </property>
- </class>
- </hibernate-mapping>
