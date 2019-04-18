# Hibernate里save(),saveOrUpdate(),merge(),update()的区别 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月25日 21:40:19[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：71


转自：[https://www.cnblogs.com/yangronglin/p/5826456.html](https://www.cnblogs.com/yangronglin/p/5826456.html)

save()方法用于将一个临时对象转变为持久化对象，也就是将一个新的业务实体保存到数据库中；

update()方法用于将一个游离对象重新转变为持久化对象，也就是更新一个已经存在的业务实体到数据库中；

saveOrUpdate()兼具了save()和update()方法的功能，该方法根据传入参数的状态执行不同的操作，当为临时状态时，调用save()方法；当为持久化状态时，则直接返回；当为游离状态时，调用update()方法。

merge()方法主要用于更新和保存实体，当实体不存在是，则执行保存操作，当实体已经存在时，执行更新操作，其实同saveOrUpdate()的功能是类似的。

1. update 和 merge的区别

首先在执行更新操作的时候，两者都必须要有id

update是直接执行update 语句，执行后状态为持久化状态

而merge则不一样：

1. 如果session中有与之对应的对象，也就是主键相同，则会把要保存的obj的值copy给session中的对象，然后update被复制后的session中的对象

2. 如果session中没有，则会先从数据库中select，然后把obj给查出来的对象copy，则update查出来的对象。

3. 所以merge会先select 再update

4. 所以merge后原来的对象obj状态为脱管。

二、save 和update区别

把这一对放在第一位的原因是因为这一对是最常用的。

save的作用是把一个新的对象保存

update是把一个脱管状态的对象或自由态对象（一定要和一个记录对应）更新到数据库

三、update 和saveOrUpdate区别

这个是比较好理解的，顾名思义，saveOrUpdate基本上就是合成了save和update,而update只是update;引用hibernate reference中的一段话来解释他们的使用场合和区别

通常下面的场景会使用update()或saveOrUpdate()： 

程序在第一个session中加载对象,接着把session关闭 

该对象被传递到表现层 

对象发生了一些改动 

该对象被返回到业务逻辑层最终到持久层 

程序创建第二session调用第二个session的update()方法持久这些改动

saveOrUpdate(po)做下面的事: 

如果该po对象已经在本session中持久化了，在本session中执行saveOrUpdate不做任何事 

如果savaOrUpdate(新po)与另一个与本session关联的po对象拥有相同的持久化标识(identifier)，抛出一个异常 

org.hibernate.NonUniqueObjectException: a different object with the same identifier value was already associated with the session: [org.itfuture.www.po.Xtyhb#5]

saveOrUpdate如果对象没有持久化标识(identifier)属性，对其调用save() ，否则update() 这个对象

