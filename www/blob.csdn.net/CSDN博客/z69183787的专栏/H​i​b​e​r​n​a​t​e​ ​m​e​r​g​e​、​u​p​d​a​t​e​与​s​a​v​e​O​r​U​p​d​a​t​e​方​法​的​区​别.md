# H​i​b​e​r​n​a​t​e​ ​m​e​r​g​e​、​u​p​d​a​t​e​与​s​a​v​e​O​r​U​p​d​a​t​e​方​法​的​区​别 - z69183787的专栏 - CSDN博客
2014年08月08日 09:13:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2031
通常下面的场景会使用update()或saveOrUpdate()： 
?程序在第一个session中加载对象 
?该对象被传递到表现层 
?对象发生了一些改动 
?该对象被返回到业务逻辑层 
?程序调用第二个session的update()方法持久这些改动 
saveOrUpdate()做下面的事: 
?如果对象已经在本session中持久化了，不做任何事 
?如果另一个与本session关联的对象拥有相同的持久化标识(identifier)，抛出一个异常 
?如果对象没有持久化标识(identifier)属性，对其调用save() 
?如果对象的持久标识(identifier)表明其是一个新实例化的对象，对其调用save() 
?如果对象是附带版本信息的（通过<version>或<timestamp>） 并且版本属性的值表明其是一个新实例化的对象，save()它。 
?否则update() 这个对象 
merge()可非常不同: 
?如果session中存在相同持久化标识(identifier)的实例，用用户给出的对象的状态覆盖旧有的持久实例 
?如果session没有相应的持久实例，则尝试从数据库中加载，或创建新的持久化实例 
?最后返回该持久实例 
?用户给出的这个对象没有被关联到session上，它依旧是脱管的 
update和merge方法,例子 
1. 数据库记录已存在，更改person的name为一个新的name。 
merge方法打印出的日志如下: 
Hibernate: select person0_.id as id0_0_, person0_.name as name0_0_ from person person0_ where person0_.id=?
 Hibernate: update person set name=? where id=? 
update方法打印出的日志如下: 
Hibernate: update person set name=? where id=? 
2. 数据库记录已存在，更改person的name和数据库里对应id记录的name一样的值。 
merge方法打印出的日志如下: 
Hibernate: select person0_.id as id0_0_, person0_.name as name0_0_ from person person0_ where person0_.id=?
 此处相对于第一种情形少了update的动作 
update方法打印出的日志如下: 
Hibernate: update person set name=? where id=? 
3. 数据库记录不存在时，也就是你传的实体bean的ID在数据库没有对应的记录。 
merge方法打印出的日志如下: 
Hibernate: select person0_.id as id0_0_, person0_.name as name0_0_ from person person0_ where person0_.id=?
 Hibernate: insert into person (name) values (?) 
如果没有对应的记录，merge会把该记录当作新的记录来插入。
