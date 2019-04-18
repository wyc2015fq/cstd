# Hibernate通过SQL查询常量时只能返回第一个字符的解决方法 - z69183787的专栏 - CSDN博客
2013年09月25日 15:53:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1283
在Hibernate中如果通过
**[java]**[view
 plain](http://blog.csdn.net/snowclash/article/details/6200768#)[copy](http://blog.csdn.net/snowclash/article/details/6200768#)
- session.createSQLQuery("select '合计' as name from dual").list()  
查询，返回值中只会有一个“合”字。
经查，常量在数据库中被认为是CHAR类型的，Hibernate取值时将其保存为Character类型，而Character只能保存一个字符，所以造成返回值中只存在一个字符。Hibernate官方已存在此[bug](http://opensource.atlassian.com/projects/hibernate/browse/HHH-2220) ，却从未修复。
该bug中给出2中解决方法：
1.继承一个Dialect，并注册CHAR类型的对应方式。代码如下
**[java]**[view
 plain](http://blog.csdn.net/snowclash/article/details/6200768#)[copy](http://blog.csdn.net/snowclash/article/details/6200768#)
- publicclass MySQLServerDialect extends org.hibernate.dialect.SQLServerDialect  
- {  
- public MySQLServerDialect()  
-     {  
- super();  
- //very important, mapping char(n) to String
-         registerHibernateType(Types.CHAR, Hibernate.STRING.getName());  
-     }  
- }  
2.设置字段对应的类型（缺点是如果查询很多字段，每个字段都必须设置，没设置类型的字段在结果集中是不存在的），代码如下
**[java]**[view
 plain](http://blog.csdn.net/snowclash/article/details/6200768#)[copy](http://blog.csdn.net/snowclash/article/details/6200768#)
- session.createSQLQuery("select '合计' as name from dual").addScalar("name", Hibernate.STRING).list()  
还有一种解决方法是在SQL中设置该字段的类型，代码如下
**[java]**[view
 plain](http://blog.csdn.net/snowclash/article/details/6200768#)[copy](http://blog.csdn.net/snowclash/article/details/6200768#)
- session.createSQLQuery("select cast('合计' as varchar(4)) as name from dual").list()  
该方法在Oracle和SQL Server中可用。
Hibernate中将SQL查询出来字段转换为对象的方法有两种
1.使用ResultTransformer，代码如下
**[java]**[view
 plain](http://blog.csdn.net/snowclash/article/details/6200768#)[copy](http://blog.csdn.net/snowclash/article/details/6200768#)
- query.setResultTransformer(new AliasToBeanResultTransformer(StudentDTO.class)).list()  
这种方法要求数据库的字段名和类的属性的大小写必须一致，但是Oracle查询返回的字段名却全是大写。
2.使用addEntity方法，这种方法的缺点是SQL必须写成select * from users的形式或select {a.*},{b.*} from a,b where...的格式。
