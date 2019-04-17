# Java并发编程规则：构建封闭的线程限制 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月12日 16:55:25[boonya](https://me.csdn.net/boonya)阅读数：546标签：[java																[多线程																[并发																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[JavaConcurrent](https://blog.csdn.net/boonya/article/category/5810701)

所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)](https://so.csdn.net/so/search/s.do?q=并发&t=blog)




访问共享、可变的数据要求使用同步。一个可以避免同步的方式就是不提供共享数据。如果数据仅在单线程中访问，就不需要提供同步。线程封闭是实现线程安全的最简单的方式之一。当一个对象封闭在一个线程中时，这种做法会自动成为线程安全的，即使封闭的对象本身不是线程安全的。——《Java并发编程实践》

## 代码级别的Ad-hoc线程限制：

此种情况是，线程的维护任务全部落在实现上。通过可见性修饰符来控制目标对象的可见性，这是一种非常容易出错的方式，很大程度上依赖于编程人员的水平和实战经验。

线程限制的一种特例是将它用于volatile变量。只要确保单一的线程写入volatile变量，那么就可以实现线程安全的“读-改-写”，从而阻止了竞争条件的发生。可见性保证了volatile变量的修改，能够确保其他线程看到最新值。

鉴于Ad-hoc线程限制的易损性，因此应该有节制地使用它。如果可能的话，用一种线程限制的强制形式（栈限制或ThreadLocal）取代它。


## 基于栈内存的栈限制：

栈限制是线程限制的一种特例，在栈限制中，只能通过本地变量才可以触及对象。正如封装使不变约束更容易被保持，本地变量使对象更容易限制在线程本地中。本地变量本身就是被限制在执行线程中，它存在于执行线程栈。其它线程无法范文这个栈。栈限制（线程内部或线程本地用法，但是不要与核心库类ThreadLocal混淆）与ad-hoc线程限制相比，更易维护，更健壮。来看个示例：



```java
public int loadTheArk(Collection<Animal> candidates) {
        SortedSet<Animal> animals;
        int numPairs = 0;
        Animal candidate = null;

        // animals confined to method, don't let them escape!
        animals = new TreeSet<Animal>(new SpeciesGenderComparator());
        animals.addAll(candidates);
        for (Animal a : animals) {
            if (candidate == null || !candidate.isPotentialMate(a))
                candidate = a;
            else {
                ark.load(new AnimalPair(candidate, a));
                ++numPairs;
                candidate = null;
            }
        }
        return numPairs;
    }
```
上述例子animals = new TreeSet<Animal>(new SpeciesGenderComparator());限制在方法内部以防止对象逸出。




## 本地化线程ThreadLocal：

一种维护线程限制更规范的方式是使用ThreadLocal，它允许你将每个线程与持有数的对象关联在一起。ThreadLocal提供了get和set访问，为每个使用它的线程维护一份单独的拷贝。所以，get总是获取当前线程set设置的最新值。来看一个ThreadLocal的实例（获取JDBC数据库连接）：



```java
package net.jcip.examples;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

/**
 * ConnectionDispenser
 * <p/>
 * Using ThreadLocal to ensure thread confinement
 *
 * @author Brian Goetz and Tim Peierls
 */
public class ConnectionDispenser {
    static String DB_URL = "jdbc:mysql://localhost/mydatabase";

    private ThreadLocal<Connection> connectionHolder
            = new ThreadLocal<Connection>() {
                public Connection initialValue() {
                    try {
                        return DriverManager.getConnection(DB_URL);
                    } catch (SQLException e) {
                        throw new RuntimeException("Unable to acquire Connection, e");
                    }
                };
            };

    public Connection getConnection() {
        return connectionHolder.get();
    }
}
```
connectionHolder在执行线程完成之前保有了本地线程的JDBC连接对象，在线程结束时被垃圾回收机回收处理，如果是业务本地缓存数据，它将毫无价值。ThreadLocal是一种本地线程内的临时缓存机制。](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




