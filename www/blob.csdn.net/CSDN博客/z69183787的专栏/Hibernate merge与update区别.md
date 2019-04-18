# Hibernate merge与update区别 - z69183787的专栏 - CSDN博客
2014年06月13日 11:31:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2197
首先，对于一个处于detatch状态的实体对象来说，要将其中的修改，合并到数据库中，有两种方法，一种方式，是调用update()，另一种方式是调用merge()
当调用update()时，首先要取保目标session中，不包含同样id的实体对象的引用，如果有的话，会抛出异常，完成update()方法后，这个实体对象从detatch状态，转换为persistent状态，在session提交前，后续对其的修改，都会被合并到数据库中。
当调用merge()对象时，无需考虑session中是否已经包含同样id的实体对象，如果session中没有同样id的实体对象，hibernate会通过select语句，从数据库中查询出对应对象，如果数据库中没有对应对象，就新建一个。同时，完成merge()操作后，会返回数据库中对应的persistent状态对象，而原有的，作为参数传入的实体对象，仍然是detatch状态，后续代码对其的修改，无法合并到数据库中。
merge与update区别
注:就因为这2个方法的区别还得我花了太多时间项目迟迟不能做完
    ,但是让我解决了,学到了东西了.
这是一段代码
public void updateData(Object obj) {
  try {
   tx = this.getSession().beginTransaction();
   // 执行修改操作
  //this.getSession().update(obj);
   this.getSession().merge(obj);
   tx.commit();
  } catch (Exception e) {
   System.out.println("===修改信息出现异常===");
   e.printStackTrace();
   tx.rollback();
  } 
 }
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
如果没有对应的记录，merge会把该记录当作新的记录来插入。此处我很疑惑，因为我传得person实体对象里写明了id值的，它为什么还会做插入的动作呢？
update方法打印出的日志如下: 
Hibernate: update person set name=? where id=? 
2009-11-22 20:59:55,359 ERROR [org.hibernate.jdbc.AbstractBatcher] - Exception executing batch: 
org.hibernate.StaleStateException: Batch update returned unexpected row count from update [0]; actual row count: 0; expected: 1 
以下的内容摘抄自网上： 
　　当我们使用update的时候，执行完成后，我们提供的对象A的状态变成持久化状态。 
　　但当我们使用merge的时候，执行完成，我们提供的对象A还是脱管状态，hibernate或者new了一个B，或者检索到 一个持久对象B，并把我们提供的对象A的所有的值拷贝到这个B，执行完成后B是持久状态，而我们提供的A还是托管状态。
网上朋友的总结：
merge（oldObj） 操作如下：
if(oldObj没有id) {
      添加操作   
} else {
     if(根据oldObj.id在Hibernate Context中没有找到对象) {
          加载得到newObj；
          if(newObj == null) {
               添加操作
        }
     }else{
         newObj
     }
    if(oldObj里面的属性 和newObj不一致) {
      更新操作
}
}
Hibernate参考书中hibernate对象三种状态
Hibernate定义并支持下列对象状态(state):
- 
*瞬时(Transient)* - 由`new`操作符创建，且尚未与Hibernate `Session` 关联的对象被认定为瞬时(Transient)的。瞬时(Transient)对象不会被持久化到数据库中，也不会被赋予持久化标识(identifier)。 如果瞬时(Transient)对象在程序中没有被引用，它会被垃圾回收器(garbage
 collector)销毁。 使用Hibernate`Session`可以将其变为持久(Persistent)状态。(Hibernate会自动执行必要的SQL语句)
- 
*持久(Persistent)* - 持久(Persistent)的实例在数据库中有对应的记录，并拥有一个持久化标识(identifier)。 持久(Persistent)的实例可能是刚被保存的，或刚被加载的，无论哪一种，按定义，它存在于相关联的`Session`作用范围内。 Hibernate会检测到处于持久(Persistent)状态的对象的任何改动，在当前操作单元(unit
 of work)执行完毕时将对象数据(state)与数据库同步(synchronize)。 开发者不需要手动执行`UPDATE`。将对象从持久(Persistent)状态变成瞬时(Transient)状态同样也不需要手动执行`DELETE`语句。
- 
*脱管(Detached)* - 与持久(Persistent)对象关联的`Session`被关闭后，对象就变为脱管(Detached)的。 对脱管(Detached)对象的引用依然有效，对象可继续被修改。脱管(Detached)对象如果重新关联到某个新的`Session`上， 会再次转变为持久(Persistent)的(在Detached其间的改动将被持久化到数据库)。
 这个功能使得一种编程模型，即中间会给用户思考时间(user think-time)的长时间运行的操作单元(unit of work)的编程模型成为可能。 我们称之为*应用程序事务*，即从用户观点看是一个操作单元(unit of work)。
