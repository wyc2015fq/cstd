# hibernate中SQLQuery的addEntity()方法 - weixin_33985507的博客 - CSDN博客
2014年11月20日 16:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

如果使用原生sql语句进行query查询时，hibernate是不会自动把结果包装成实体的。所以要手动调用addEntity(Class class)等一系列方法。
如session.createSQLQuery(sql).addEntity(Class class);注意hibernate3.0.5不支持，单个参数的addEntity方法
另外，hibernate3.2可以对原生sql 查询使用ResultTransformer。这会返回不受Hibernate管理的实体。
session.createSQLQuery("SELECT NAME, BIRTHDATE FROM CATS")
       
.setResultTransformer(Transformers.aliasToBean(CatDTO.class))
或setResultTransformer(new **AliasToBeanResultTransformer**
(CatDTO.class))
上面的查询将会返回CatDTO的列表,它将被实例化并且将NAME和BIRTHDAY的值注射入对应的属性或者字段。
[http://bbs.xml.org.cn/blog/more.asp?name=lhwork&id=15351](http://bbs.xml.org.cn/blog/more.asp?name=lhwork&id=15351)
但必须注意，对每一个列都必须addScalar("列名")
既然如此，
那么setResultTransformer与addEntity的区别是什么呢？一个区别是前者支持查任意的列，后者必须用select
* from users的形式或select {a.*},{b.*} from a,b where ....。
