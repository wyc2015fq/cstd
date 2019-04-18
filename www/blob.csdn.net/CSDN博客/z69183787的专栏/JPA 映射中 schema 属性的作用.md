# JPA 映射中 schema 属性的作用 - z69183787的专栏 - CSDN博客
2014年04月12日 11:39:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6754
这几天在迁移数据库，突然发现使用中的 A和 B 数据库 有相同的用户名，迁移后决定将A和B数据库放在1个C数据库，所以相同用户名stpt就被设置为
stpt1 stpt，启动应用后，发现sql错误，table or view does not exists。
检查后发现，原来数据对象Bo映射的 属性中，都标明了 schema，在oracle中 即指定 数据表所属用户，结果只能作罢。
注解：
```java
@Entity
/*     */ @Table(name="T_DOC_SEND", schema="STPT")
```
xml：
```
<class name="com.wonders.contact.deptContact.model.bo.TDeptContactTree" table="T_DEPT_CONTACT_TREE" schema="STPT" dynamic-update="true" dynamic-insert="true">
```
在hibernate与数据实体的映射文件*.hbm.xml里面指定了shcema文件，则在进行数据库的查询时，所有的查询都会翻译成schema.tablename
看样子之后 要统一定义每个bo 的 schema 了 不能再写在单独的 bo里了 ：
方案：
[http://blog.csdn.net/z69183787/article/details/23522571](http://blog.csdn.net/z69183787/article/details/23522571)
附上，每个数据库中 映射属性schema的不同含义：
转自:http://blog.sina.com.cn/s/blog_515015800100evtc.html
按照ＳＱＬ标准的解释，在ＳＱＬ环境下Catalog和Schema都属于抽象概念，可以把它们理解为一个容器或者数据库对象命名空间中的一个层次，主要用来解决命名冲突问题。从概念上说，一个数据库系统包含多个Catalog，每个Catalog又包含多个Schema，而每个Schema又包含多个数据库对象（表、视图、字段等），反过来讲一个数据库对象必然属于一个Schema，而该Schema又必然属于一个Catalog，这样我们就可以得到该数据库对象的完全限定名称从而解决命名冲突的问题了；例如数据库对象表的完全限定名称就可以表示为：Catalog名称.Schema名称.表名称。这里还有一点需要注意的是，ＳＱＬ标准并不要求每个数据库对象的完全限定名称是唯一的，就象域名一样，如果喜欢的话，每个ＩＰ地址都可以拥有多个域名。
　　从实现的角度来看，各种数据库系统对Catalog和Schema的支持和实现方式千差万别，针对具体问题需要参考具体的产品说明书，比较简单而常用的实现方式是使用数据库名作为Catalog名，使用用户名作为Schema名，具体可参见下表：
|供应商|Catalog支持|Schema支持|
|----|----|----|
|Oracle|不支持|Oracle User ID|
|MySQL|不支持|数据库名|
|MS SQL Server|数据库名|对象属主名，2005版开始有变|
|DB2|指定数据库对象时，Catalog部分省略|Catalog属主名|
|Sybase|数据库名|数据库属主名|
|Informix|不支持|不需要|
|PointBase|不支持|数据库名|
　　最后一点需要注意的是Schema这个单词，它在SQL环境下的含义与其在数据建模领域中的含义是完全不同的。在SQL环境下，Schema是一组相关的数据库对象的集合，Schema的名字为该组对象定义了一个命名空间，而在数据建模领域，Schema（模式）表示的是用形式语言描述的数据库的结构；简单来说，可以这样理解，数据建模所讲的Schema<也就是元数据>保存在SQL环境下相应Catalog中一个Schema<名叫DEFINITION_SCHEMA>下的表中，同时可以通过查询该Catalog中的另一个Schema<名叫INFORMATION_SCHEMA>下的视图而获取，具体细节不再赘述。
参考资料：
1. ISO/IEC 9075-2:2003 - Database Language SQL (SQL-2003)
2. [Database-specific Catalog and Schema Considerations](http://download.oracle.com/docs/cd/E13162_01/odsi/docs10gr3/datasrvc/Database-specific%20Catalog%20and%20Schema%20Considerations.html)
3. [What is a Schema in SQL Server 2005?](http://www.dbanotes.com/sqlserver-database/what-is-a-schema-in-sql-server-2005/)
个人想法（与上面转的无关）
如在sqlserver中有个数据库叫hibernate,其下有张表test,就可以这们select * from hibernate.dbo.test1
这里的hibernate相当于catagory,dbo就是catagory
