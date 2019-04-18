# HQL模糊查询非varChar类型字段 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月13日 19:30:54[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：120



今天使用hql进行模糊查询，like条件的字段类型为bigint，导致报错 ，说是String类型不能够被强制转换成Long。最后发现就是因为我用 query.setParameter("id", "%"+param+"%"); 这个方法给sql参数赋值的时候，认为我们传的是一个String，而数据库的字段是long，所以报错，但是最恶心的是，你调用setParameter的时候并不会报错，而是当你query.list()或 query.uniqueResult()的时候才会报错。

后来网上搜索发现一个解决办法，就是通过convert将非varchar类型的数据转换成varchar，使用方法如下。

String hql = "from 实体类名（查询的表） where 1=1 and convert(varchar,**实体类变量名(模糊查询的字段)**,长度) like  :id;

Query query= session.createQuery(hql);

query.setParameter("id", "%"+param+"%");

