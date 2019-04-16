# Java JPA快速入门介绍 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年08月25日 22:29:27[gloomyfish](https://me.csdn.net/jia20003)阅读数：13153








**JPA快速入门介绍**

**一：什么是JPA**

JPA的英文全称是Java PersistenceAPI, 目的是给Java开发者提供对象关系映射工具用于在

Java应用程序开发中来管理关系数据(RDBMS)。JavaPersistence 包含下面三个部分：

a.      Java持久化API

b.      JPA查询语言

c.      对象关系映射元数据



**二：JPA有哪些框架提供了的实现**

当前JPA提供厂商有Hibernate, Apache, Eclipse Link等，Google云计算平台 AppEngine也使

用了JPA作为持久层。JPA作为持久层框架有如下优点：

1.      简单易用，帮助开发者提供了生产率

2.      便于维护，减低了维护成本

3.      学习成本相对比较低。



但是JPA的缺点也是显而易见，JPA作为持久层有如下缺点：

1.      将语言与数据库混在一起，导致数据改动以后，配置文件必须更新

2.      对与多数据与大数据量处理很容易产生性能问题。

3.      过度封装，导致错误查找相对与JDBC等传统开发技术而言更加困难



**三：标准的JPA规范JSR粗略解读**

JPA的最新规范为JSR Java PersistenceAPI Version 2.0

Entity Class – 实体类，必须使用注解@entity标明，同时必须有一个无参数的构造函数，而

且无参数构造函数必须为public或者protected，如果一个entity class被标记为final将导致

出错。

EntityManager – 实体管理者，管理Entity实例的整个生命周期，而且使用Query API来查询

实体与他们的persist状态。

Query Language – 基于字符串的查询语句，用来查询实体(Entity)与他们的状态。



MetaModel API – 通过EntityManagerFactory或者EntityManager的getMetamodel()方法获取，

查看persistence-unit的信息。



实体管理者与持久化上下文(Entity Manager and Persistence contexts) 

Persistence Contexts – 一个被管理的实体的实例集合，在一个持久化上下文中的所有实例都

由Entity Manager来管理它们整个生命周期。



持久化单元(Persistence-Unit) – 一个持久化单元是个逻辑分组包括以下部分：

-         一个实体管理者工厂及它的实体管理者

-         被管理的class集合，在persistence unit配置文件中定义

-         映射元数据– 注解定义或者xml定义匹配的类

-          

ORM元数据(MetaData forObject/Relational Mapping) – 坦白的说就annotation的各种解释与

使用。

详细了解请阅读Oracle官方文档 - 《persistence-2_0-final-spec》PDF文档。

**四：JPA简单实例说明**

1.      使用ObjectDB作为数据库，关于ObjectDB请参考[http://www.objectdb.com/](http://www.objectdb.com/)

2.      参考了objectDB的例子，代码几乎没有改动，文档说明参见这里：

[http://www.objectdb.com/tutorial/jpa/eclipse/web](http://www.objectdb.com/tutorial/jpa/eclipse/web)

3.      一个可运行的JPA实例源代码：

entity class:



```java
package com.gloomyfish.jpa.tutorial;

import java.io.Serializable;
import javax.persistence.*;
 
@Entity
public class Point implements Serializable {
    private static final long serialVersionUID = 1L;
 
    @Id @GeneratedValue
    private long id;
 
    private int x;
    private int y;
 
    public Point() {
    }
 
    Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
 
    public Long getId() {
        return id;
    }
 
    public int getX() {
         return x;
    }
 
    public int getY() {
         return y;
    }
 
    @Override
    public String toString() {
        return String.format("(%d, %d)", this.x, this.y);
    }
}
```
Main Test JPA;



```java
package com.gloomyfish.jpa.tutorial;

import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import javax.persistence.TypedQuery;

public class JPAMain {
	public static void main(String[] args) {
        // Open a database connection
        // (create a new database if it doesn't exist yet):
        EntityManagerFactory emf =
            Persistence.createEntityManagerFactory("$objectdb/db/points.odb");
        EntityManager em = emf.createEntityManager();
 
        // Store 1000 Point objects in the database:
        long startTime = System.currentTimeMillis();
        em.getTransaction().begin();
        for (int i = 0; i < 10; i++) {
            Point p = new Point(i, i);
            em.persist(p);
        }
        em.getTransaction().commit();
        long endTime = System.currentTimeMillis();
        System.out.println("save time = " + (endTime - startTime));
        // Find the number of Point objects in the database:
        Query q1 = em.createQuery("SELECT COUNT(p) FROM Point p");
        System.out.println("Total Points: " + q1.getSingleResult());
 
        // Find the average X value:
        Query q2 = em.createQuery("SELECT AVG(p.x) FROM Point p");
        System.out.println("Average X: " + q2.getSingleResult());
 
        // Retrieve all the Point objects from the database:
        TypedQuery<Point> query =
            em.createQuery("SELECT p FROM Point p", Point.class);
        List<Point> results = query.getResultList();
        for (Point p : results) {
            System.out.println(p);
        }
 
        // Close the database connection:
        em.close();
        emf.close();
    }
}
```



