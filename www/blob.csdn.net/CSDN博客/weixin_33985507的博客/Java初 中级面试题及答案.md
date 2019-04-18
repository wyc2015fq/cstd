# Java初/中级面试题及答案 - weixin_33985507的博客 - CSDN博客
2018年07月17日 07:07:05[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
**1.Java的HashMap是如何工作的？**
HashMap是一个针对数据结构的键值，每个键都会有相应的值，关键是识别这样的值。
HashMap 基于 hashing 原理，我们通过 put ()和 get ()方法储存和获取对象。当我们将键值对传递给 put ()方法时，它调用键对象的 hashCode ()方法来计算 hashcode，让后找到 bucket 位置来储存值对象。当获取对象时，通过键对象的 equals ()方法找到正确的键值对，然后返回值对象。HashMap 使用 LinkedList 来解决碰撞问题，当发生碰撞了，对象将会储存在 LinkedList 的下一个节点中。 HashMap 在每个 LinkedList 节点中储存键值对对象。
参考：
[浅谈Java中的hashcode方法](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D513)
[集合系列—HashMap源码分析](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D74)
**2.什么是快速失败的故障安全迭代器？**
快速失败的Java迭代器可能会引发ConcurrentModifcationException在底层集合迭代过程中被修改。故障安全作为发生在实例中的一个副本迭代是不会抛出任何异常的。快速失败的故障安全范例定义了当遭遇故障时系统是如何反应的。例如，用于失败的快速迭代器ArrayList和用于故障安全的迭代器ConcurrentHashMap。
**3.Java BlockingQueue是什么？**
Java BlockingQueue是一个并发集合util包的一部分。BlockingQueue队列是一种支持操作，它等待元素变得可用时来检索，同样等待空间可用时来存储元素。
**4.什么时候使用ConcurrentHashMap？**
在问题2中我们看到ConcurrentHashMap被作为故障安全迭代器的一个实例，它允许完整的并发检索和更新。当有大量的并发更新时，ConcurrentHashMap此时可以被使用。这非常类似于Hashtable，但ConcurrentHashMap不锁定整个表来提供并发，所以从这点上ConcurrentHashMap的性能似乎更好一些。所以当有大量更新时ConcurrentHashMap应该被使用。
**5.哪一个List实现了最快插入？**
LinkedList和ArrayList是另个不同变量列表的实现。ArrayList的优势在于动态的增长数组，非常适合初始时总长度未知的情况下使用。LinkedList的优势在于在中间位置插入和删除操作，速度是最快的。
LinkedList实现了List接口，允许null元素。此外LinkedList提供额外的get，remove，insert方法在LinkedList的首部或尾部。这些操作使LinkedList可被用作堆栈（stack），队列（queue）或双向队列（deque）。
ArrayList实现了可变大小的数组。它允许所有元素，包括null。 每个ArrayList实例都有一个容量（Capacity），即用于存储元素的数组的大小。这个容量可随着不断添加新元素而自动增加，但是增长算法并没有定义。当需要插入大量元素时，在插入前可以调用ensureCapacity方法来增加ArrayList的容量以提高插入效率。
**6.Iterator和ListIterator的区别**
ListIterator有add()方法，可以向List中添加对象，而Iterator不能。
ListIterator和Iterator都有hasNext()和next()方法，可以实现顺序向后遍历，但是ListIterator有hasPrevious()和previous()方法，可以实现逆向（顺序向前）遍历。Iterator就不可以。
ListIterator可以定位当前的索引位置，nextIndex()和previousIndex()可以实现。Iterator没有此功能。
都可实现删除对象，但是ListIterator可以实现对象的修改，set()方法可以实现。Iierator仅能遍历，不能修改。
**7.什么是CopyOnWriteArrayList，它与ArrayList有何不同？**
CopyOnWriteArrayList是ArrayList的一个线程安全的变体，其中所有可变操作（add、set等等）都是通过对底层数组进行一次新的复制来实现的。相比较于ArrayList它的写操作要慢一些，因为它需要实例的快照。
CopyOnWriteArrayList中写操作需要大面积复制数组，所以性能肯定很差，但是读操作因为操作的对象和写操作不是同一个对象，读之间也不需要加锁，读和写之间的同步处理只是在写完后通过一个简单的”=”将引用指向新的数组对象上来，这个几乎不需要时间，这样读操作就很快很安全，适合在多线程里使用，绝对不会发生ConcurrentModificationException ，因此CopyOnWriteArrayList适合使用在读操作远远大于写操作的场景里，比如缓存。
**8.迭代器和枚举之间的区别**
如果面试官问这个问题，那么他的意图一定是让你区分Iterator不同于Enumeration的两个方面：
Iterator允许移除从底层集合的元素。
Iterator的方法名是标准化的。
**9.Hashmap如何同步?**
当我们需要一个同步的HashMap时，有两种选择：
使用Collections.synchronizedMap（..）来同步HashMap。
使用ConcurrentHashMap的
这两个选项之间的首选是使用ConcurrentHashMap，这是因为我们不需要锁定整个对象，以及通过ConcurrentHashMap分区地图来获得锁。
**10.IdentityHashMap和HashMap的区别**
IdentityHashMap是Map接口的实现。不同于HashMap的，这里采用参考平等。
在HashMap中如果两个元素是相等的，则key1.equals(key2)
在IdentityHashMap中如果两个元素是相等的，则key1 == key2
**11.mybatis的优缺点？**
优点：SQL写在XML中，便于统一管理和优化
提供映射标签，支持对象和数据库的orm字段关系映射
可以对SQL进行优化
缺点： SQL工作量大
mybagtis移植姓不好
不支持级联
mybatis学习：
[Mybatis学习系列（一）入门简介](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1356)
[Mybatis学习系列（二）Mapper映射文件](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1361)
[Mybatis学习系列（三）动态SQL](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1364)
[Mybatis学习系列（四）Mapper接口动态代理](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1366)
[Mybatis学习系列（五）关联查询](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1368)
[Mybatis学习系列（六）延迟加载](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1370)
[Mybatis学习系列（七）缓存机制](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Fp%3D1375)
**12.谈谈SSH整合?**
struts(表示层)+spring(业务层)+hibernate(持久层)
struts是一个表示层的框架，主要用于接收请求，分发请求。struts其实属于MVC中的VC层次的
hibernate是一个持久层的框架，主要负责与关系数据库的操作
spring是一个业务层的框架，是一个整合的框架，能够很好的黏合表示层和持久层。
**13.maven是什么?有什么作用？**
是一个项目管理、构建工具
作用：帮助下载jar 寻找依赖，帮助下载依赖 热部署、热编译
**14.WEB 前端优化？**
减少HTTP请求的数量（合并css、js、图片）
利用浏览器的缓存机制
利用GZIP压缩机制：只针对文本类资源有效
把CSS文件放在HTML开头
把javascript文件放在HTML结尾
避免CSS表达式（判断浏览器）
使用javascript压缩
减少DNS查找
避免重定向
使用ajax
**15.安全性测试**
利用安全性测试技术，找到潜在的漏洞
**16.事务隔离级别（4种）**
Serializable(串行化)：一个事务在执行过程中完全看不到其他事物对数据库所做的更新（事务执行的时候不允许别的事务并发执行，事务只能一个接着一个地执行，而不能并发执行）
Repeatable Read（可重复读）：一个事务在执行过程中可以看到其它事务已经提交的新插入的记录，但是不能看到其它事务对已有记录的更新
Read Commited（读已提交数据）：一个事务在执行过程中可以看到其它事务已经提交的新插入的记录，而且能看到其它事务已经提交的对已有记录的更新。
Read Uncommitted（读未提交数据）：一个事务在执行过程中可以看到其它事务没有提交的新插入的记录的更新，而且能看其它事务没有提交到对已有记录的更新
**17.MYSQL存储引擎（4种）**
MyISAM它不支持事务，也不支持外键，尤其是访问速度快，对事务完整性没有要求或者以SELECT、INSERT为主的应用基本都可以使用这个引擎来创建表。
每个MyISAM在磁盘上存储成3个文件，其中文件名和表名都相同，但是扩展名分别为：
.frm(存储表定义)
MYD(MYData，存储数据)
MYI(MYIndex，存储索引)
InnoDB
InnoDB存储引擎提供了具有提交、回滚和崩溃恢复能力的事务安全。但是对比MyISAM的存储引擎，InnoDB写的处理效率差一些并且会占用更多的磁盘空间以保留数据和索引。
1)自动增长列2)外键约束
MEMORY
memory使用存在内存中的内容来创建表。每个MEMORY表实际对应一个磁盘文件，格式是.frm。MEMORY类型的表访问非常快，因为它到数据是放在内存中的，并且默认使用HASH索引，但是一旦服务器关闭，表中的数据就会丢失，但表还会继续存在。
MERGE
merge存储引擎是一组MyISAM表的组合，这些MyISAM表结构必须完全相同，MERGE表中并没有数据，对MERGE类型的表可以进行查询、更新、删除的操作，这些操作实际上是对内部的MyISAM表进行操作。
**18.事务传播特性**
1. PROPAGATION_REQUIRED: 如果存在一个事务，则支持当前事务。如果没有事务则开启
2. PROPAGATION_SUPPORTS: 如果存在一个事务，支持当前事务。如果没有事务，则非事务的执行
3. PROPAGATION_MANDATORY: 如果已经存在一个事务，支持当前事务。如果没有一个活动的事务，则抛出异常。
4. PROPAGATION_REQUIRES_NEW: 总是开启一个新的事务。如果一个事务已经存在，则将这个存在的事务挂起。
5. PROPAGATION_NOT_SUPPORTED: 总是非事务地执行，并挂起任何存在的事务。
6. PROPAGATION_NEVER: 总是非事务地执行，如果存在一个活动事务，则抛出异常
7. PROPAGATION_NESTED：如果一个活动的事务存在，则运行在一个嵌套的事务中. 如果没有活动事务,
**19.简述一下你了解的设计模式。**
所谓设计模式，就是一套被反复使用的代码设计经验的总结（情境中一个问题经过证实的一个解决方案）。使用设计模式是为了可重用代码、让代码更容易被他人理解、保证代码可靠性。设计模式使人们可以更加简单方便的复用成功的设计和体系结构。将已证实的技术表述成设计模式也会使新系统开发者更加容易理解其设计思路。
在GoF的《Design Patterns: Elements of Reusable Object-Oriented Software》中给出了三类（创建型[对类的实例化过程的抽象化]、结构型[描述如何将类或对象结合在一起形成更大的结构]、行为型[对在不同的对象之间划分责任和算法的抽象化]）共23种设计模式，包括：Abstract Factory（抽象工厂模式），Builder（建造者模式），Factory Method（工厂方法模式），Prototype（原始模型模式），Singleton（单例模式）；Facade（门面模式），Adapter（适配器模式），Bridge（桥梁模式），Composite（合成模式），Decorator（装饰模式），Flyweight（享元模式），Proxy（代理模式）；Command（命令模式），Interpreter（解释器模式），Visitor（访问者模式），Iterator（迭代子模式），Mediator（调停者模式），Memento（备忘录模式），Observer（观察者模式），State（状态模式），Strategy（策略模式），Template Method（模板方法模式）， Chain Of Responsibility（责任链模式）。
面试被问到关于设计模式的知识时，可以拣最常用的作答，例如：
**工厂模式：**工厂类可以根据条件生成不同的子类实例，这些子类有一个公共的抽象父类并且实现了相同的方法，但是这些方法针对不同的数据进行了不同的操作（多态方法）。当得到子类的实例后，开发人员可以调用基类中的方法而不必考虑到底返回的是哪一个子类的实例。
**代理模式：**给一个对象提供一个代理对象，并由代理对象控制原对象的引用。实际开发中，按照使用目的的不同，代理可以分为：远程代理、虚拟代理、保护代理、Cache代理、防火墙代理、同步化代理、智能引用代理。
**适配器模式：**把一个类的接口变换成客户端所期待的另一种接口，从而使原本因接口不匹配而无法在一起使用的类能够一起工作。
**模板方法模式：**提供一个抽象类，将部分逻辑以具体方法或构造器的形式实现，然后声明一些抽象方法来迫使子类实现剩余的逻辑。不同的子类可以以不同的方式实现这些抽象方法（多态实现），从而实现不同的业务逻辑。
除此之外，还可以讲讲上面提到的门面模式、桥梁模式、单例模式、装潢模式（Collections工具类和I/O系统中都使用装潢模式）等，反正基本原则就是拣自己最熟悉的、用得最多的作答，以免言多必失。
设计模式：
[23种设计模式及六大设计原则](https://link.juejin.im?target=http%3A%2F%2Fwww.javazhiyin.com%2F%3Ftag%3D%25E8%25AE%25BE%25E8%25AE%25A1%25E6%25A8%25A1%25E5%25BC%258F)
