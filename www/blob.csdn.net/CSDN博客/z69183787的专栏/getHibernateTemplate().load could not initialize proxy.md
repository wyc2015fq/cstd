# getHibernateTemplate().load could not initialize proxy - z69183787的专栏 - CSDN博客
2013年12月09日 14:01:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1011
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
```java
TBidPlan t = this.getHibernateTemplate().load(TBidPlan.class, id);
		if(!Hibernate.isInitialized(t)) 
			Hibernate.initialize(t); 
		
		TBidPlan t = this.getHibernateTemplate().get(TBidPlan.class, id);
		return t;
```
你把load()换成get()就可以
load()支持延迟加载，get()不支持
