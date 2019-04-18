# JPA EntityManager的四个主要方法 ——persist,merge,refresh和remove - z69183787的专栏 - CSDN博客
2015年01月08日 11:39:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2968

![](http://dl.iteye.com/upload/attachment/0084/2955/8e05eb9a-18ed-3aed-bbdb-8fb270403014.png)
**public void persist(Object entity)**
persist方法可以将实例转换为managed(托管)状态。在调用flush()方法或提交事物后，实例将会被插入到数据库中。
对不同状态下的实例A，persist会产生以下操作:
1.       如果A是一个new状态的实体，它将会转为managed状态；
2.       如果A是一个managed状态的实体，它的状态不会发生任何改变。但是系统仍会在数据库执行INSERT操作；
3.       如果A是一个removed(删除)状态的实体，它将会转换为受控状态；
4.       如果A是一个detached(分离)状态的实体，该方法会抛出IllegalArgumentException异常，具体异常根据不同的JPA实现有关。
# public void merge(Object entity)
         merge方法的主要作用是将用户对一个detached状态实体的修改进行归档，归档后将产生一个新的managed状态对象。
对不同状态下的实例A，merge会产生以下操作:
1.       如果A是一个detached状态的实体，该方法会将A的修改提交到数据库，并返回一个新的managed状态的实例A2；
2.       如果A是一个new状态的实体，该方法会产生一个根据A产生的managed状态实体A2;
3.       如果A是一个managed状态的实体，它的状态不会发生任何改变。但是系统仍会在数据库执行UPDATE操作；
4.       如果A是一个removed状态的实体，该方法会抛出IllegalArgumentException异常。
# public void refresh(Object entity)
        refresh方法可以保证当前的实例与数据库中的实例的内容一致。
对不同状态下的实例A，refresh会产生以下操作:
1.       如果A是一个new状态的实例，不会发生任何操作，但有可能会抛出异常，具体情况根据不同JPA实现有关；
2.       如果A是一个managed状态的实例，它的属性将会和数据库中的数据同步；
3.       如果A是一个removed状态的实例，不会发生任何操作;
4.       如果A是一个detached状态的实体，该方法将会抛出异常。
# public void remove(Object entity)
        remove方法可以将实体转换为removed状态，并且在调用flush()方法或提交事物后删除数据库中的数据。
对不同状态下的实例A，remove会产生以下操作:
1.       如果A是一个new状态的实例，A的状态不会发生任何改变，但系统仍会在数据库中执行DELETE语句；
2.       如果A是一个managed状态的实例，它的状态会转换为removed；
3.       如果A是一个removed状态的实例，不会发生任何操作;
4.       如果A是一个detached状态的实体，该方法将会抛出异常。
