# HibernateTemplate 自动事务 - z69183787的专栏 - CSDN博客
2013年12月05日 10:54:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2947
使用HibernateTemplate时，不显示的声明事务管理，照样能对数据库做CURD。
这是因为在没有声明事务时，HibernateTemplate是默认自动提交事务的。
因此如果在业务代码中，只涉及到一次数据库操作时默认自动事务提交已经足够，如果多次数据库操作则需要做事务声明。
**hibernateTemplate有点类似事务中的propagation的required. 当你在spring配置了事务的时候,hibernateTemplate就会用当前spring配置的事务,如果spring没有配置事务,然而hibernateTemplate又是必须要一个事务,则它会自己创建一个事务开始并提交!!**
