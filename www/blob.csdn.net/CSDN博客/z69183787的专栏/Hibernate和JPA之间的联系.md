# Hibernate和JPA之间的联系 - z69183787的专栏 - CSDN博客
2014年04月17日 10:21:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：941
个人分类：[对象映射框架-Hibernate																[事务-Jpa实践](https://blog.csdn.net/z69183787/article/category/2175487)](https://blog.csdn.net/z69183787/article/category/2175489)
Hibernate分析：
> 
Hibernate是一个开源的代码的对象关系映射框架，它对JDBC进行了非常轻量级的对象封装，是Java程序员可以随心所欲的使用对象编程思想来操作数据库。
Hibernate的核心接口：
> 
Session：负责执行被持久化对象的CRUD，是线程不安全的
> 
**SessionFactory：负责初始化Hibernate，充当数据存储源的代理，并负责创建Session对象，用到了工厂模式。**
> 
Transaction：负责事务相关的操作。
> 
Query和Criteria：负责事务相关的操作。
> 
Configuration：负责配置并启动Hibernate，创建SessionFactory对象，在Hibernate的在启动的过程中，首先定位映射文档位置，读取文档和创建SessionFactory。
JPA：JavaPersistence API分析：
> 
JPA的宗旨是为了POJO提供持久化标准规范，JPA框架中支持大数据量。事务、并发等容器级别事务，JPA基于非侵入式原则设计，因此很融合框架或者容器。
JPA提供的技术 
> 
  (1)ORM映射元数据
> 
  JPA支持XML和JDK 5.0注解两种元数据的形式，元数据描述对象和表之间的映射关系，框架据此将实体对象持 久化到数据库表中；
> 
  (2)JPA 的API
> 
  用来操作实体对象，执行CRUD操作，框架在后台替我们完成所有的事情，开发者从繁琐的JDBC和SQL代码中解 脱出来。
> 
  (3)查询语言
> 
  通过面向对象而非面向数据库的查询语言查询数据，避免程序的SQL语句紧密耦合。 
JPA的优势
> 
  (1)标准化
  JPA是JCP 组织发布的JavaEE标准之一，因此任何声称符合JPA标准的框架都遵循同样的架构，提供相同的访问  API，这保证了基于JPA开发的企业应用能够经过少量的修改就能够在不同的JPA框架下运行。
  (2)对容器级特性的支持
  JPA框架中支持大数据集、事务、并发等容器级事务，这使得JPA超越了简单持久化框架的局限，在企业应用发挥更  大的作用。
  (3)简单易用，集成方便
  JPA的主要目标之一就是提供更加简单的编程模型：在JPA框架下创建实体和创建Java 类一样简单，没有任何的约束  和限制，只需要使用javax.persistence.Entity进行注释；JPA的框架和接口也都非常简单，没有太多特别的规则  和设计模式的要求，开发者可以很容易的 掌握。JPA基于非侵入式原则设计，因此可以很容易的和其它框架或者容  器集成。
 (4)可媲美JDBC的查询能力
 JPA的查询语言是面向对象而非面向数据库的，它以面向对象的自然语法构造查询语句，可以看成是HibernateHQL  的等价物。JPA定义了独特的JPQL（Java Persistence Query Language），JPQL是EJBQL的一种扩展，它是针 对实体的 一种查询语言，操作对象是实体，而不是关系数据库的表，而且能够支持批量更新和修改、JOIN、GROUP BY、HAVING等通常只有 SQL 才能够提供的高级查询特性，甚至还能够支持子查询。
  (5)支持面向对象的高级特性
  JPA中能够支持面向对象的高级特性，如类之间的继承、多态和类之间的复杂关系，这样的支持能够让开发者最大限度 的使用面向对象的模型设计企业应用，而不需要自行处理这些特性在关系数据库的持久化。
两者功能关系：
> 
从功能上来说，JPA现在就是Hibernate功能的一个子集。
例如：
> 
  (1)实体对象的状态，在Hibernate有自由、持久、游离三种，JPA里有new，managed，detached，  removed，而这些状态都是一一对应的。
> 
  (2)flush方法，都是对应的，
> 
  (3)Query query= manager.createQuery(sql)，它在Hibernate里写法上是session，而在JPA中变成了  manager
另一种关系：
> 
 JPA是标准接口，Hibernate是实现。
Hibernate通过三个组件来实现的，以及hibernate-annotation、hibernate-entitymanager和hibernate-core。
> 
(1)hibernate-annotation是Hibernate支持annotation方式配置的基础，它包括了标准的JPA annotation以及
> 
  Hibernate自身特殊功能的annotation。
> 
 (2)hibernate-core是Hibernate的核心实现，提供了Hibernate所有的核心功能。
> 
** (3)hibernate-entitymanager实现了标准的JPA，可以把它看成hibernate-core和JPA之间的适配器，它并不直接  提供ORM的功能，而是对hibernate-core进行封装，使得Hibernate符合JPA的规范。**
Hibernate和JPA类和实现
> 
 (1)HibernatePersistence.java，实现了JPA的PersistenceProvider接口，提供createEntityManagerFactory  和createContainerEntityManagerFactory两个方法来创建EntityManagerFactory对象，这两个方法底层都是调  用EJB3Configuration对象的buildEntityManagerFactory方法，来解析JPA配置文件persistence.xml,，并创建
 EntityManagerFactory对象。
** (2)EntityManagerFactory对象的实现是EntityManagerFactoryImpl类，这个类有一个最重要的private 属性就  是Hibernate的核心对象之一SessionFactory。这个类最重要的方法是createEntityManager，来返回  EntityMnagaer对象，而sessionFactory属性也传入了该方法。**
** (3)EntityManager对象的实现是EntityManagerImpl类，这个类继承自AbstractEntityManagerImpl 类，在 AbstractEntityManager类中有一个抽象方法getSession来获得Hibernate的Session对象，正是在这个 Session 对象的实际支持下，EntityManagerImpl类实现了JPA的EntityManager接口的所有方法，并完成实际的ORM操作。**
(4)QueryImpl类:QueryImpl类利用EntityManagerImpl的支持实现了JPA的 Query接口；
 (5)TransactionImpl利用EntityManagerImpl的支持实现了JPA的EntityTransaction接口。
