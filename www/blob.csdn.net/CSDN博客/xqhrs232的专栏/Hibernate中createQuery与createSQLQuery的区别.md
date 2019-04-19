# Hibernate中createQuery与createSQLQuery的区别 - xqhrs232的专栏 - CSDN博客
2018年11月29日 18:15:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：31
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/h254931252/article/details/79032773](https://blog.csdn.net/h254931252/article/details/79032773)
相关文章
1、createQuery与createCriteria的用法及区别----[https://blog.csdn.net/gaoqiang1112/article/details/55002937](https://blog.csdn.net/gaoqiang1112/article/details/55002937)
1.前者用的hql语句进行查询，后者可以用sql语句查询。
2.前者以hibernate生成的Bean为对象装入list返回，后者则是以对象数组进行存储。
3.前者在查询时查询条件的类型需要和对象对应的属性的类型一致。例如查询时间类型的字段，前者需要用时间类型，而后者只要是时间格式的字符串即可。
使用createSQLQuery有时候也想以hibernate生成的Bean为对象装入list返回，这个方法可以直接转换对象
Query q = getSession().createSQLQuery(hql).addScalar("YY1").addScalar("YY2").addScalar("YY3");
q.setResultTransformer(Transformers.aliasToBean(XXXX.class));
XXXX 代表以hibernate生成的Bean的对象，也就是数据表映射出的Bean。YY表示查询出来的列对应到对象中的某个属性。
--------------------- 
作者：Memory小峰 
来源：CSDN 
原文：https://blog.csdn.net/h254931252/article/details/79032773 
版权声明：本文为博主原创文章，转载请附上博文链接！
