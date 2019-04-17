# 115 Java面试问题和答案 –终极列表 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 21:16:45[boonya](https://me.csdn.net/boonya)阅读数：1300









在本教程中我们会讨论在Java面试中可能被用到的各种类型的问题，以便面试官来测试你的Java和面向对象开发(OOP)方面的大概水平。


在接下来的章节我们会讨论面向对象编程(OOP)和它的特点，关于Java及其功能的常见问题，Java 集合框架(Collections), 垃圾回收，异常处理，Java applets, Swing, JDBC, 远程方法调用(RMI), Servlet 和JSP。让我们开始吧！



## 目录
- [面试对象编程(OOP)](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#1)
- [Java的一些常见问题](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#2)
- [Java线程](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#3)
- [Java集合](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#4)
- [垃圾回收](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#5)
- [异常处理](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#6)
- [Java Applets](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#7)
- [Swing](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#8)
- [JDBC](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#9)
- [远程方法调用(RMI)](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#10)
- [Servlets](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#11)
- [JSP](http://www.javacodegeeks.com/zh-hans/2015/02/java%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98%E5%92%8C%E7%AD%94%E6%A1%88.html#12)

## 面试对象编程(OOP)




Java是并发的，基于类和面向对象的编程语言。面向对象的软件开发具有以下优点：
- 
模块化的代码开发，使维护和修改更加容易。
- 
代码的复用性。
- 
提高代码的可靠性和灵活性。
- 
代码更易理解。


面向对象的编程具有一些重要的特征， 例如封装，继承，多多和抽象。我们会在以下章节逐个分析这些特征。

封装


封装为对象赋予了隐藏其内部特征和行为的能力。每一个对象都有一些方法，这些方法能被其他对象访问并改变对象内部数据。在Java中，有三种访问修饰符：public，private和protected。每一种修饰符赋予其他类一些访问权限，这些类可以是在同一个包里或者不同包。下面列出了使用封装的好处：
- 
每一个对象的内部状态都通过隐藏其属性来保护。
- 
提高了代码的复用性和可维护性，因为对象的行为可以被独立的改变或扩展。
- 
通过阻止对象间的非预期的相互访问，提高了模块化水平。


你可以参考一下文档[here](http://examples.javacodegeeks.com/java-basics/encapsulation-in-java/)来获得更多的关于封装的细节和例子。

多态


多态是编程语言用来为底层数据类型提供不同接口的能力。一个多态类型的操作也可以被应用到一些其他类型的值。

继承


继承为对象赋予了访问其他类的成员字段和方法的能力，这个其他类也被称作基类。继承提供了代码的可重用性，而且可以被用来为已存在的类增加新的功能，而不用改变基类。

抽象


抽象是分离概念(idea)和特定实例(specific instances)的过程，所以可以开发一些具有特定功能的类，而不是一些具体实现细节。Java支持抽象类，它们并不是所有方法都有具体实现。抽象技术的目的是分离类的行为和实现细节。

抽象和封装的区别


抽象和封装是互补的概念。一方面，抽象关注于一个对象的行为。另一方面，封装关注对象行为的实现细节。封装通常是通过隐藏对象内部状态来达到的，所以，可以视作一种提供抽象的策略。

## Java的一些常见问题

1. 什么是Java虚拟机？为什么java被称作平台独立的编程语言?  Java虚拟机是一个能处理Java字节码的进程。每一个Java源文件都会被编译成字节码文件，然后被Java虚拟机执行。从设计上看，Java允许程序在任何一个平台上执行，而不用程序员在每一个不同平台上进行重写或重编译。Java虚拟机能做到这一点，是因为它了解底层硬件平台的指令长度和其他细节。

2. JDK和JRE有什么区别?  JRE基本上就是java虚拟机，也就是Java程序执行的地方。它也包含浏览器的applet插件。JDK是java的全功能开发软件包(SDK)，包括了JRE，编译器和工具(比如JavaDoc, Java Debugger)。这些功能是为用户用作开发、编译和执行java程序的。

3. 关键字”static”是什么意思? 在Java中能够重写(override)私有或者静态方法吗? static关键字表示一个变量或方法在不用实例化它所属于的类的情况下就可以访问。在java中用户不能重写(override)静态方法，因为方法重写是基于运行时的动态绑定，而静态方法是在编译时绑定的。一个静态方法不能与这个类的任何实例关联，所以这种说法是不对的。

4. 能够在静态上下文中访问非静态的变量吗? Java中的静态变量属于类，它的值对这个类的所有实例都是一样的。静态变量在类被java虚拟机加载时就就被初始化。如果代码尝试访问没有实例的非静态变量，编译器会报错，因为这些变量还没有被创建，而且它们没有与任何实例关联。

5. Java支持哪些数据类型?自动装箱和自动拆箱是指什么? Java语言支持8中基本数据类型:
- 
byte
- 
short
- 
int
- 
long
- 
float
- 
double
- 
boolean
- 
char


自动装箱是指java编译器所进行的，在原始类型和与其对应的对象包装类之间的自动转换。比如，编译器将int转换为Integer，double转换为Double，等等。如果这个转换过程以相反的方法进行，这个操作就叫自动拆箱。

6. 在Java中什么叫方法重写和重载? Java中方法重载是指同一个类中有两个或多个相同名字的方法。而方法重写是指子类重新定义父类的相同方法。重写方法有相同的名字，参数列表和返回类型。重写方法不会限制对被重写方法的访问。

7. Java中什么是构造器，构造器重载和复制构造器？ 构造器在新建一个对象时被调用。每一个类都有一个构造器。程序员如果没有为一个类提供构造器，java编译器则会为这个类创建一个默认的构造器。Java中构造器重载与方法重载类似，不同的构造器可以在同一个类中被定义，每一个构造器都必须有不同的参数列表。另外，Java也支持与C++类似的复制构造器，但区别是，如果你不写自己的构造器，java并不会创建一个默认的复制构造器。

8. Java支持多重继承吗？ 不支持，java不支持多重继承。每一个类只能扩展一个类，但是可以实现多个接口。

9. 接口和抽象类的区别是什么？ Java提供并支持抽象类和接口，两种实现有一些相同的特点，但是他们有以下区别:
- 
接口中的所有方法都是隐含的抽象的。另一方面，抽象类可以包含抽象和非抽象方法。
- 
一个类可以实现多个接口，但只能扩展一个抽象类。
- 
一个类要实现一个接口，必须实现接口所声明的所有方法。但是，一个类可以不全部实现一个抽象类所定义的全部方法，但在这种情况下，子类也是抽象的。
- 
抽象类实现一个接口时，可以不提供这个接口的方法的实现。
- 
Java接口中的变量定义默认都是final的。抽象类可以包含非final的变量。
- 
Java接口的成员默认都是public的。抽象类的成员可以是private，protected或者public。
- 
接口是绝对抽象的，不能被实例化。抽象类也不能被实例化，但是可以在包含main方法时被调用。


请参考[Abstract class and Interface differences for JDK 8](http://www.javacodegeeks.com/2014/04/abstract-class-versus-interface-in-the-jdk-8-era.html).

10. 什么是按引用传递和按值传递？ 当一个对象以值方式传递时，表示这个对象的一份拷贝被传递。但是，即使这个对象被改变了，也不会影响原来的值。当一个对象是以按引用方式传递，表示实际对象没有被传递，而是这个对象的一个引用被传递。所以，外部方法的所有更改都可以产生影响。

## Java线程

11.进程和线程的区别是什么？ 进程是一个程序的运行时态，而线程则是进程中的一个执行序列。一个进程可以包含多个线程。线程有时候又被称为轻量级的进程。

12. 解释创建一个线程的不同方式。哪一种方式是你优先选择的，为什么？ 有三种方式可以用来创建线程:
- 
扩展Thread类。
- 
实现Runnable接口。
- 
使用Executor框架，来创建一个线程池。


Runnable接口是首选的，因为它不需要对象继承Thread类。如果你的应用设计需要多重继承，只有接口才能帮你。另外，线程池是非常高效的，也能非常容易的实现和使用。

13. 从高层次上解释可用的线程状态。 在执行过程中，线程可以处于以下的一些状态:
- [NEW](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#NEW):
 线程准备好执行了，但是还不一定会马上开始执行。
- [RUNNABLE](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#RUNNABLE):Java虚拟机正在执行这个线程的代码。
- [BLOCKED](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#BLOCKED):
 线程在等待监视器锁时，处于阻塞状态。
- [WAITING](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#WAITING):
 线程在等待另一个线程执行特定的一些操作。
- [TIMED_WAITING](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#TIMED_WAITING):
 线程在等待另一个线程执行一些特定的操作，并且最多等待指定的时间。
- [TERMINATED](https://docs.oracle.com/javase/8/docs/api/java/lang/Thread.State.html#TERMINATED):
 线程已经完成了执行。

14. Synchronized方法和synchronized块有什么区别? 在java编程中，每一个对象都有一个锁。一个线程可以通过使用synchronized关键字获得一个对象的锁。Synchronized关键字可以被用在方法级别上(粗粒度锁)或者块级别上(细粒度锁)。

15. 线程同步在监视器(monitor)里是怎么工作的？可以应用什么级别的同步？ JVM使用锁和监视器(monitor)。一个监视器基本上就是一个监护人，监护着一个同步代码序列并且保证在一个时刻仅有一个线程可以执行一段同步代码。每一个监视器都和一个对象引用关联，线程只有在获得锁之后才能执行代码。

16. 什么是死锁?当两个进程都在等待对方完成后才继续执行的情况下，两个进程都会永久得等待下去。

17. 怎样保证N个线程访问N个资源，而不产生死锁? 当使用N个线程时, 一个很简单的避免死锁的方法就是，强制对锁排序，每一个线程都遵循这个顺序。那么，如果所有线程都按照同样的顺序加锁和解锁，就不会产生死锁。

## Java集合

18. Java集合框架的基本接口是什么？Java集合框架提供了一个经过精心设计的一系列接口和类，它们支持对一批的对象的操作。Java集合框架的最基本的接口有：
- 
Collection,表示一组被称为元素的对象。
- 
Set, 是一个不可以包含重复元素的集合。
- 
List, 是一组排序的集合，而且可以包含重复元素。
- 
Map,是一个可以映射键到值的对象，而且不能包含重复键。

19. 为什么Collection不扩展Cloneable和Serializable接口? Collection接口指定了一组被称为元素的对象。每一个Collection的具体实现都可以选择它自己的方式来维护和排序这些元素。一些Collection允许重复的键，但另外的一些则不允许。拷贝和序列化的语义和隐含条件在处理具体实现时才起作用。所以，集合的具体实现才应该决定怎样被拷贝或序列化。

20. 什么是Iterator? Iterator接口提供了若干方法来对Collection进行迭代。每一个Java Collection 都包含iterator 方法来返回一个Iterator 实例。迭代器能够在进行迭代时删除底层集合的元素。

21. Iterator和ListIterator有什么区别？这两个接口的区别如下：
- 
一个[Iterator](http://docs.oracle.com/javase/7/docs/api/java/util/Iterator.html)可以用来遍历Set和List集合，而[ListIterator](http://docs.oracle.com/javase/7/docs/api/java/util/ListIterator.html) 只能用来迭代List。
- [Iterator](http://docs.oracle.com/javase/7/docs/api/java/util/Iterator.html)只可以正向遍历一个集合，而[ListIterator](http://docs.oracle.com/javase/7/docs/api/java/util/ListIterator.html) 可以双向遍历[List](http://docs.oracle.com/javase/7/docs/api/java/util/List.html) 。
- [ListIterator](http://docs.oracle.com/javase/7/docs/api/java/util/ListIterator.html)实现了[Iterator](http://docs.oracle.com/javase/7/docs/api/java/util/Iterator.html) 接口，并且包含其他的功能，比如增加新的元素，替换一个元素，获得前面和后面元素的索引位置，等等。

22. 快速失败(fail-fast)和安全失败(fail-safe)有什么区别？ Iterator的快速失败特性在拷贝底层集合时起作用，所以，它不受对集合的修改的影响。Java.util包中的所有集合类都是快速失败的，而java.util.concurrent包中的集合类都是安全失败的。快速失败的迭代器抛出ConcurrentModificationException，而安全失败的迭代器从不抛出这个异常。

23. Java中HashMap是如何工作的？Java中的HashMap用来存储键值对。HashMap需要哈希函数以及用hashCode和equals方法，来分别向集合插入元素和从集合查询元素。当put方法被调用时，HashMap计算键的哈希值并将键值对插入集合中的合适位置。如果键已经存在，它的值就被新的值更新。HashMap的一些重要特征是容量(capacity),加载因子(load
 factor)和改变大小的阈值(threshold resizing)。

24. hashCode()和equals()方法有什么重要性？ 在Java中，HashMap使用hashCode和equals方法来决定键值对的索引和检测重复。更具体的说，hashCode方法用来决定特定的键被存在什么位置。因为不同的键可能产生相同的哈希值，equals方法就被用来确定特定的键在集合中是否已存在。所以，这两个方法的实现都对HashMap的准确性和高效性有至关重要的作用。

25. HashMap和Hashtable之间有什么区别？ HashMap和Hashtable都实现了Map接口，所以有相似的特点。但是，它们也有以下区别：
- 
HashMap允许null键和值的存在，而Hashtable既不允许null键，也不允许null值。
- 
Hashtable是同步的，而HashMap不是。所以，HashMap更被常用在单线程环境中，而Hashtable更适合多线程环境。
- 
HashMap提供了一系列的键，java程序可以对它们进行迭代。所以，HashMap是快速失败的。而Hashtable为这些键提供了Enumeration接口。
- 
Hashtable类是遗产代码。

26. 数组和ArrayList之间有什么区别？什么时候选择数组而不是ArrayList？ 数组和ArrayList区别如下:
- 
数组可以包含原始类型或对象，而ArrayList只能包含对象。
- 
数组具有固定的大小，而ArrayList的大小是可变的。
- 
ArrayList提供了多个方法和功能，比如addAll, removeAll, iterator等。
- 
对于一组原始数据类型，集合使用自动装箱来减小代码量。但是，这种方法使得它们在处理固定尺寸的元素数据类型是更慢。

27. ArrayList和LinkedList之间有什么区别？ ArrayList和LinkedList类都实现了List接口，但是它们具有以下区别：
- 
Arraylist是基于索引的数据结构，底层使用数组存储的。它提供了对元素的随机访问，而时间复杂度度为O(1)。而LinkedList把数据存储在元素列表中，每一个元素都与它的前置和后置元素链接在一起。那么，对元素的查询操作时间复杂度为O(n)。
- 
与ArrayList相比，LinkedList的插入、增加和删除操作更快，因为不需要对数组进行改变尺寸或当一个元素被插入集合中某一个任意位置时更新索引。
- 
LinkedList比ArrayList消耗更多的内存，因为LinkedL中每一个节点都存在两个引用，一个指向前置元素，一个指向后置元素。


请参考文章[ArrayList vs. LinkedList](http://www.javacodegeeks.com/2013/12/arraylist-vs-linkedlist.html)。

28. Comparable 和 Comparator接口有什么区别？请列出它们的区别。 Java提供的Comparable 接口包含一个compareTo方法，它比较两个对象来为它们排序。更确切的说，它返回一个负整数，0或者一个正整数来表示输入的对象小于，等于或大于已存在的对象。Java还提供了Comparator 接口，它包含两个方法，compare
 和equals。第一个方法比较两个输入参数并对它们强制排序。它返回一个负整数，0或正整数来表示第一个参数小于，等于或大于第二个参数。第二个方法需要一个对象作为参数，目的是决定输入对象是否等于被比较对象。这个方法仅当指定的对象也是一个可比较的对象起作用,而且它们需要有相同的排序方法。

29. 什么是Java优先队列(Priority Queue)？PriorityQueue 是一个无限的队列，基于优先堆，它的元素用自然排序方式排序。在创建时，可以提供一个Comparator来负责对PriorityQueue的元素进行排序。PriorityQueue不允许null值，不支持自然排序的对象和没有相关联comparator的对象。最后，Java
 PriorityQueue不是线程安全的，它需要O(log(n))的时间复杂度来进行入队和出队的操作。

30. 你知道大O记法(big-O notation),可以给出一些关于不同数据结构的例子吗？ 大O记法(big-O notation)简单地描述了一个算法如何适应，或在最坏情况下即当数据结构中的元素数量增加时性能怎么样变化。大O记法(big-O notation)也可以用来描述其他行为，比如内存消耗。因为集合类实际上是数据结构，我们通常使用大O记法(big-O
 notation)来选择最好的实现来使用，而选择的根据是执行时间，内存和性能。大O记法(big-O notation)可以提供关于大量数据的处理性能的很好的指示。

31. 当使用一个无序数组而不是一个有序的数组时，牺牲的什么？ 排序数组的最大优势是查找时间复杂度为O(log n)，而无序数组的为O (n)。排序数组的弱点是，插入操作的时间复杂度为O(n)，因为具有更大值得元素必须向后移动来为新元素腾出空间。而无序的数组的插入操作的时间复杂度为常量的O(1)。

32. 关于Java集合框架的最佳实践有哪些？
- 
根据应用的需求选择正确的集合类型使用对性能是很关键的。例如，如果元素的尺寸是固定的而且有优先顺序，我们应该使用数组，而不是ArrayList。
- 
一些集合类允许我们指定初始容量。所以，如果我们有关于将要存储的元素数量的估计的话，我们可以使用这个初始容量来避免重哈希或改变尺寸。
- 
总是使用泛型来保证类型安全，可靠性和健壮性。而且，通过使用反省，可以避免运行时产生[ClassCastException](http://docs.oracle.com/javase/7/docs/api/java/lang/ClassCastException.html)异常。
- 
使用JDK提供的不可变类来作为Map中的键，这样可以避免在我们的自定义类中实现hashCode和equals方法。
- 
面向接口编程而不是面向实现。
- 
在集合中为空时，返回0个元素的集合或数组，而不是返回null值。

33. Enumeration和Iterator接口有什么区别？ Enumeration是Iterator的两倍块而且使用很少的内存。但是，Iterator比Enumeration更安全，因为其他线程在一个迭代器进行遍历时不能修改集合对象。而且，迭代器允许调用者从相关的集合移除元素，这在Enumeration 中是不可能的。

34. HashSet和TreeSet有什么区别？HashSet使用哈希表实现的，所以它的元素是无序的，HashSet的add，remove和contains方法都有常量的时间复杂度O(1)。 而TreeSet 是使用树数据结构来实现的，TreeSet中的元素是排序的，所以，add，remove和contains方法的时间复制度为O(logn)。

## 垃圾回收

35. Java中的垃圾回收有什么目的，什么时候使用？ 垃圾回收的目的是定位和移除不再被应用程序使用的对象，以便资源被回收重用。

36. System.gc()和Runtime.gc()方法是做什么的？ 这些方法可以用作对Java虚拟机的一个提示，来启动垃圾回收。但是，还是取决于Java虚拟机来决定是立即或稍后启动垃圾回收。

37. finalize()什么时候被调用？finalize()的目的是什么？finalize方法在即将释放这个对象的内存之前被垃圾回收器调用。通常建议在finalize方法中释放被这个对象所占用的资源。

38. 如果一个对象引用被设置为null，垃圾回收器会立即释放被这个对象所占用的内存吗？ 不会，垃圾回收器会在下一个周期对这个对象回收。

39. Java堆的结构是怎么样的？堆中的永久区(Perm Gen space)是什么？Java虚拟机中的堆是一个运行时的数据区域，其中可以分配所有类的实例和数组。它在Java虚拟机启动时创建。堆内存中的对象被自动的内存管理系统回收，也称为垃圾回收器。堆内存包含活的或已经死亡的对象，活对象能被应用程序访问，不会被垃圾回收。而死亡的对象是那些永远也不会被应用程序访问的对象，但是还没有被垃圾回收器回收，这些对象占据内存空间，直到它们最终被垃圾回收器回收。

40. 串行和吞吐量垃圾回收器有什么区别？ 吞吐量垃圾回收器使用并行式的新生代垃圾回收器，是用来对中大型数据集的应用程序进行使用的。另一方面，串行回收器通常更适合大多数小型应用程序(在现代处理器上需要大约100MB的堆内存)。

41. Java中对象在什么时候有条件被垃圾回收？一个对象在当它不能被使用它的程序访问到时，就能被垃圾回收。

42. Java虚拟机的永久带空间中会发生垃圾回收吗？ 永久带空间会发生垃圾回收，如果永久带空间被填满或者超过了某个阈值，它就会触发一个全局的垃圾回收。如果你仔细观察垃圾回收器的输出，你会发现永久带空间也会被垃圾回收。这就是为什么永久带空间的为什么需要分配合适的大小，这样可以避免频繁的全局内存回收。更多文章请查看 [Java
 8: PermGen to Metaspace](http://www.javacodegeeks.com/2013/02/java-8-from-permgen-to-metaspace.html).

## 异常处理

43. Java中两种异常是哪两种？它们之间的区别是什么？ Java有两种类型的异常：检查异常和(checked exceptions)和非检查异常(unchecked exceptions)。非检查异常如果有可能被方法或构造器抛出，并不需要再方法中或构造器中声明，而且向方法和构造器的上层传播。而检查异常必须在方法或构造器的throws子句中声明。请查看[Java
 exception handling](http://www.javacodegeeks.com/2013/07/java-exception-handling-tutorial-with-examples-and-best-practices.html)来获得更多提示。

44. Java中的Exception和Error有什么区别？ Exception和Error类都是Throwable 类的子类。Exception 类用在异常情况下，需要用户的程序进行捕获。Error 类定义的异常不需要被用户程序捕获。

45. Throw和throws有什么区别？ throw关键字用于显式的抛出一个异常。相反，throws子句用于表示那些需要被方法处理的异常。每一个方法都必须显式的指定哪些异常没有被处理，以便方法的调用者可以对可能的异常做出保护措施。最后，多个异常用逗号分隔。

45. 异常处理中的finally块有什么重要作用？ finally块总是会被执行，无论异常是否会被捕获。即使在catch语句不存在而且异常被抛出的的情况下，finally块也仍然会被执行。最后值得提的是，finally块常用来释放资源，比如I/O buffer，数据库连接等。

46. 异常处理后Exception对象会发生什么？Exception 会在下次垃圾回收时被回收掉。

47. Finally块和finalize方法有什么区别？finally块无论异常是否抛出都会被执行，用来释放被程序占用的资源。Finalize是Object类的一个保护性的方法，在对象即将被垃圾回收前，被Java虚拟机调用。

## Java Applets

48.什么是Applet? java applet是一个程序，它可以被包含在一个HTML页面中，在一个允许执行java程序的浏览器中执行。Applet程序被用来创建动态的和交互性的web应用程序。

49. 解释Applet的生命周期。一个Applet可能经历如下的状态:
- Init：Applet在每次被加载时都会初始化。
- Start：开始执行Applet。
- Stop：结束applet的执行。
- Destroy：在卸载applet前执行最终的清理工作。

50. 当applet加载时发生了什么？首先，一个applet的控制类的实例被创建。然后，applet初始化它自己，最终，它开始执行。

51. Applet和java程序有什么区别？Applet能在允许执行java的浏览器中执行，而java程序是一个独立的java程序，在浏览器外执行。但是，它们都需要有java虚拟机的存在。另外，java程序需要一个特定签名的main方法声明，用来开始执行。而Applet不需要这个方法来开始执行。最后，applet通常使用限制性的安全策略，而java程序通常使用更放松的安全策略。

52. Java Applet有哪些限制？ 通常由于安全原因，java  applet会有以下限制：
- 
Applet不能加载本地方法库。
- 
Applet通常不能读写宿主机器上的文件。
- 
Applet不能读一些系统属性。
- 
Applet不能创建除了到源主机之外的其他网络。
- 
Applet不能启动宿主机器上的任何程序。

53. 什么是不被信任的applet？ 不被信任的applet是那些不能访问或执行本地系统文件的applet。默认地，所有下载的applet都被认为是不被信任的。

54. 通过互联网下载的和通过文件系统下载的applet之间有什么区别？ 先考虑通过互联网下载applet的情况，applet被applet类加载器加载，会被applet安全管理器强加一些限制。而通过客户端本地磁盘下载的applet，applet被文件系统加载器加载。经过文件系统加载的applet允许读文件，写文件和加载客户端的库。而且，通过文件系统加载的applet允许执行一个进程。最后，通过文件系统加载的applet并不经过字节码校验器的校验。

55.什么是applet类加载器，它有什么功能？ 当applet是通过互联网下载的，就会被applet类加载器加载。这个类加载器强制了java命名空间的层级关系。而且，类加载器保证一个唯一的命名空间为来自本地文件系统的类，一个唯一的命名空间为每一个来自网络源的资源。当一个浏览器通过网络加载applet时，applet的类都被放在一个与该applet的来源关联的私有的命名空间中。那么，这些被类加载器加载的类都需要经过类校验器。校验器检查类文件是否符合java语言规范。
 另外，校验器确保没有栈溢出或者向下溢出，而且传给所有字节码指令的参数都是正确的。

56. 什么是applet安全管理器，它有什么功能？ applet安全管理器是为java applet强加限制的机制。浏览器可能仅包含一个安全管理器。安全管理期在初始化时建立，之后不能被替换，重载，覆盖或扩展。

## [Swing](http://examples.javacodegeeks.com/desktop-java/swing/)

57. Choice和List有什么区别？Choice以一种紧凑的方式显示，必须通过下拉才能显式全部的列表。从Choice中只能选择一项。List可以显示多项，它支持选择一个或多个列表项目。

58. 什么是布局管理器？ 布局管理器用来管理容器中的组件。

59. Scrollbar和JScrollPane有什么区别？ Scrollbar 是一个Component，而不是Container。ScrollPane是一个Container，ScrollPane 处理它自己的事件，并自己执行滑动动作。

60. 哪些swing方法是线程安全的？ 仅有三个线程安全的方法：repaint, revalidate, 和 invalidate。

61. 请列出支持painting的Component的子类。 Canvas, Frame, Panel和支持painting的Applet类。

62.什么是裁剪(clipping)?   裁剪(clipping)是指对于有限的区域或形状的受限绘画操作的过程。

63. MenuItem和CheckboxMenuItem有什么区别？CheckboxMenuItem 扩展了MenuItem类，并支持选择和去掉选择的菜单项。

64. BorderLayout的元素是如何管理的？BorderLayout 的元素是通过放置在容器的边界(North, South, East, and West))和中心来管理的。

65. GridBagLayout是如何管理元素的？ GridBagLayout 的元素是通过一个表格来管理的。不同大小的元素可能占据表格的多于一个的行或列。所以，行和列可能有不同的大小。

66. Window和Frame有和区别？ Frame 扩展自Window 类，并定义了一个可以包含菜单栏的主程序窗口。

67. 裁剪和重画有什么联系？当一个窗口被AWT绘画线程重画时，它指定剪切区域为窗口需要重画的区域。

68. 事件监听接口和事件适配器有什么联系？ 事件监听接口定义了一些方法，事件处理器必须针对特定的事件实现特定的方法。事件适配器提供了事件监听接口的默认实现。

69. GUI组件如何处理它的事件的？一个GUI组件通过实现相应的事件监听接口，并将自己作为自己的事件监听器来处理自己的事件。

70. Java的布局管理器相对于传统的窗口系统有什么优势？ 在所有的窗口平台上，Java一致性地使用布局管理器对组件进行布局。因为布局管理器没有使用绝对的尺寸或定位，它们可以适应各种窗口系统平台特定的差异。

71. Java对于所有的Swing组件使用了何种设计模式？ Java对于所有的Swing组件使用了MVC模式。

## JDBC

72. 什么是JDBC？JDBC是一个抽象层，它允许用户在数据库之间切换。JDBC 允许开发者不用考虑特定数据库的底层细节，就可以写出数据库应用程序。

73. 解释JDBC中Driver的角色。JDBC Driver提供了厂商针对JDBC API抽象类的特定实现。每一个Driver都必须提供以下java.sql包下的类的实现：Connection, Statement,PreparedStatement, CallableStatement, ResultSet 和 Driver。

74. Class.forName方法的目的是什么？这个方法用于加载用于建立与数据库的连接的驱动。

75. PreparedStatement相比Statement有什么优点？ PreparedStatements是经过预编译的，所以性能更好。而且，PreparedStatement对象可以被有不同的输入的查询所重用。

76. CallableStatement有什么作用？请说出用于准备一个CallableStatement的方法的名字。 CallableStatement 被用于执行存储过程。存储过程由数据库存储并执行。存储过程可以从用户处得到输入，然后返回结果。存储过程的使用是高度鼓励的，因为它提供了安全性和模块性。用于准备CallableStatement
 的方法是



|`1`|`CallableStament.prepareCall();`|
|----|----|




77. 连接池是指什么？与数据库的交互，打开和关闭连接是高成本的。特别地，当数据库的客户端数量增加时，成本非常高，大量的资源会被消耗。数据库连接池在应用服务器启动的时候就获得了连接，并进行维护。对于连接的请求会由连接池内部的连接来提供。在连接结束后，请求被返回给连接池以用于满足未来的请求。

## 远程方法调用(RMI)

78. 什么是RMI？Java远程方法调用(Java RMI)是Java的API，它执行与面向对象对等的远程过程调用(RPC)，支持序列化的java类和分布式的垃圾回收的直接传递。远程方法调用(RMI)也可以视为启动一个远程运行的对象的方法。RMI对位置透明，因为用户感觉方法是在一个本地运行的对象上执行。请看[RMI
 Tips](http://www.javacodegeeks.com/2013/11/two-things-to-remember-when-using-java-rmi.html).

79. RMI架构的基本原则是什么？ RMI架构基于一个很重要的原则，就是行为的定义和行为的实现时分离的。RMI允许定义行为的代码和执行行为的代码是分离的，可以在不同的JVM上运行。

80. RMI架构的分层是什么？ RMI架构包含以下的层：
- Stub和Skeleton层：本层刚好位于开发者的视角下，它负责拦截客户对接口方法的调用，并重定向这些调用到一个远程的RMI服务。
- 远程引用层：RMI架构的第二层处理从客户到服务器远程对象的引用的解释。这一层解释并管理客户端发起的到远程服务对象的引用，这个链接是一对一(单播)的链接。
- 传输层：这一层负责连接两个参与服务的JVM，它基于网络中计算机的TCP/IP连接，提供了基本的连通性和一些防火墙的渗透策略。

81. RMI中的远程接口有什么作用？ 远程接口用于确定接口方法，这些方法可能被不是本地机器的虚拟机调用。任何远程对象都必须直接或间接实现这个接口。一个实现远程接口的类应该声明实现了远程接口，定义每一个远程对象的构造器并为所有远程接口的每一个方法提供实现。

82.java.rmi.Naming Class有什么作用？ java.rmi.Naming 类提供了存储和获得到远程对象注册表中的远程对象的引用的方法。每一个Naming类的方法都有一个参数作为名字，这个名字是一个URL格式的String。

83. RMI中的绑定是什么意思？ 绑定就是为一个远程对象关联或注册一个名字，可以在以后的时间中被用来查询这个远程对象。远程对象可以使用Naming类的bind或rebind方法来与一个名字绑定。

84. 使用Naming类的bind和rebind方法有什么区别？ bind方法用于绑定一个特定的名字到远程对象上，而rebind方法用于重绑定指定的名字到远程对象上。如果这个名字的绑定已经存在，那么绑定就会被替换。

85. 使得一个RMI程序工作起来的步骤有哪些？要使一个RMI程序工作正常，必须有以下步骤：
- 
所有源文件的编译。
- 
使用rmic产生stub。
- 
启动rmiregistry。
- 
启动RMIServer。
- 
运行客户段程序。

86.RMI中的stub有什么作用？ A远程对象的stub作用是作为远程对象的本地代表或代理。调用者调用本地stub上的一个方法，用来执行远程对象上的方法。当一个stub方法被调用时，在底层执行了以下的步骤：
- 
它初始了一个到包含远程对象的远程JVM的连接。
- 
它编组到远程JVM的所有参数。
- 
它等待方法调用和执行的结果。
- 
它反编组返回值，或者是当方法没有被成功执行时的异常。
- 
它返回值到调用者。

87. 什么是DGC？它怎么样工作？ DGC指的是分布式内存回收。远程方法调用(RMI)使用DGC来自动垃圾回收。因为RMI包含跨JVM的对象引用，垃圾回收可能会非常困难。DGC使用引用计数算法来为远程对象提供自动内存管理。

88. RMI中使用RMISecurityManager的目的是什么？ RMISecurityManager提供了被使用下载的代码的RMI程序使用的安全管理器。如果安全管理器还没有设置，RMI的类加载器不会从远程位置下载任何类。

89. 解释编组和反编组。 当一个程序想要通过网络传输它自己的内存对象，到另一个网络主机或永久花到存储中时，内存中的形式必须被转换为另一种合适的格式。这一过程被称作编组，而反操作就被叫做反编组。

90. 解释序列化和反序列化。 Java提供了一种叫做序列化的机制，用在当一个对象可以用一串字节来表示并包含对象的数据，而且包含对象类型的信息，和存储在对象中的数据的类型的信息。所以，序列化可以被看作是压扁对象的方式，以便来存储在磁盘上。而且在以后可以读回来并复原。反序列化就是反过程，用来将一个对象从压扁的状态转换到存活的对象的过程。

## Servlets

91. 什么是Servlet？Servlet是java编程语言的一个类，用于处理客户端的请求并产生动态网页内容。Servlet通常用来处理或存储有HTML表单提交的数据，提供动态内容和管理在无状态HTTP协议中不能保存的状态信息。

92. 解释Servlet的架构。 所有servlet必须实现的核心抽象是javax.servlet.Servlet接口。每一个servlet都必须直接或间接地实现它，要么通过扩展javax.servlet.GenericServlet，要么扩展javax.servlet.http.HTTPServlet。最终，每一个servlet都可以用多线程的方式并行的处理多个请求。

93. Applet和Servlet有何区别？Applet是客户端的java程序，运行在客户端机器的web浏览器中。而servlet是服务器端的组件，运行在web服务器中。Applet可以使用UI类，而servlet没有UI接口。相反的，servlet等待客户端的HTTP请求并对每一个请求产生一个返回结果。

94. GenericServlet和HttpServlet有何区别？GenericServlet是一个广义的、协议无关的servlet，它实现了Servlet和ServletConfig接口。那些扩展了GenericServlet类的servlet应当覆盖service方法。最后，要开发一个HTTP Servlet来在Web上用于处理HTTP协议的请求，servlet必须扩展HttpServlet。请查看 [Servlet
 examples](http://examples.javacodegeeks.com/tag/servlet/).

95.解释Servlet的生命周期。每一次客户端的请求到达时，Servlet引擎都会加载servlet并调用它的init方法，用来初始化servlet。然后，Servlet对象对每一个请求都独立的调用service方法来处理所有来自客户端的后继请求。最后，通过调用servlet的destroy方法来移除servlet。

96. DoGet和doPost有何区别？ doGet: GET方法在请求URL后附加了名字-值对，所以，对于一个客户端的请求，字符数是有限制的，值的数量也是有限制的。更重要的是，请求的值是可见的，所以敏感信息不能通过这种方式传输。doPOST：POST方法通过在body内部传输值，而克服了GET请求所强制的限制。另外，也没有对于能传输的值的数量的限制。最后，通过POST请求传输的敏感信息也不对外部客户可见。

97. Web 应用程序是指什么？一个Web 应用程序是一个Web服务器或应用服务器的动态扩展。有两种类型的web应用程序：基于展现的和基于服务的。基于展现的web应用程序产生交互式的web页面，它们其中包含各种类型的标记语言和动态内容，用来对请求进行响应。另一方面，基于服务的web应用程序实现了web服务的一个端点。总之，web应用程序可以被视为安装在服务器URL命名空间的特定子集下的一系列servlet。

98. 什么是Server Side Include (SSI)？Server Side Includes (SSI) 是一个简单的服务器端的解释性的脚本语言，几乎只用于web，使用一个servlet标签来嵌入。SSI的最常见的用途就是将一个或多个文件的内容包含到一个web服务器上的web页面。当通过浏览器访问web页面时，web服务器使用由对应servlet产生的超文本来替换了这个web页面中的servlet标签。

99. 什么是servlet链？ servlet链是指将一个servlet的输出作为下一个servlet的输入。第二个servlet的输出也可以被发给第三个servlet，以此类推。链中的最后一个servlet负责将response返回给客户端。

100. 你怎么确定哪一个客户端机器发送的请求到你的servlet？ ServletRequest类有方法来查出客户端机器的IP地址或主机名。getRemoteAddr方法取得客户端机器的IP地址，getRemoteHost方法取得客户端机器的主机名。请看例子 [here](http://examples.javacodegeeks.com/enterprise-java/servlet/get-client-s-address-and-hostname-in-servlet/).

101. HTTP response的结构是怎么样的？ HTTP response有以下三个部分组成：
- Status Code: 描述response的状态。可以用来检查请求是否成功的完成。在请求失败的情况下，status code可以用来查找失败背后的原因。如果你的servlet没有返回status code，成功的status code也就是HttpServletResponse.SC_OK会作为默认的status code返回。
- HTTP Headers: 它们包含关于response的更多信息。比如，headers可以指定response过期的日期/时间，或者表单的编码用来安全的将实体传输到用户。请看 [how
 to retrieve headers in Servlet here](http://examples.javacodegeeks.com/enterprise-java/servlet/get-all-request-headers-in-servlet/).
- Body: 包含response的内容。Body可以包含HTML代码，图片等。在一个HTTP事务中，Body包含紧跟在headers后的二进制数据。

102. 什么是cookie？session和cookie有何区别？cookie是web服务器发送给浏览器的一小块信息。浏览器将每一个web服务器的cookie存储在一个本地文件中。在以后的请求中，浏览器会将为某个特定web服务器存储的cookie与请求一起传输。Session和cookie区别如下：
- 
无论客户端浏览器的设置如何，session都会工作。客户端可能选择禁止cookie的使用。但是，session仍然会工作，因为客户端没有能力禁止在服务器端使用它们。
- 
Session和cookie在所携带的信息的大小上也不同。HTTP session可以存储任何的java对象，而cookie只能存储一个String对象。

103. 浏览器和servlet使用何种协议来通信?  HTTP协议。

104. 什么是HTTP隧道(Tunneling)? HTTP隧道(Tunneling)是一种技术，使用不同网络协议进行的通信都可以使用HTTP或HTTPS协议来进行封装。那么，HTTP协议就作为通道的包装器，网络协议使用这个通道来进行通信。将其他协议的请求包装成HTTP请求就叫HTTP隧道(Tunneling)。

105. sendRedirect和forward方法有何区别？ sendRedirect方法创建一个新的请求，而forward方法只是将请求转发给一个新的目标。前置的request scope对象在redirect后就不再可用了，因为使用了一个新的request。另一方面，前者的request scope对象在转发后仍然可用。最后，总之，sendRedirect方法可以认为比forward方法要慢。

106. 什么是URL 编码(Encoding)和URL 解码(Decoding)？URL 编码(Encoding)过程是负责将URL中的空白字符和每一个其他的特殊字符替换为对应的HEX形式。对应地，URL 解码(Decoding)就是反过程。

## JSP

107. 什么是JSP？ JSP是一个包含两种类型文本的文本文档：静态数据和动态元素。静态数据可以用任何基于文本的格式来表示，比如HTML或XML。JSP是一种混合了静态内容和动态生成的内容的技术。请看[JSP
 example](http://examples.javacodegeeks.com/enterprise-java/jsp/sample-jsp-java-server-page/).

108. JSP请求是如何处理的？当JSP请求到达时，浏览器首先请求一个以.jsp为后缀的页面。然后，web服务器读取请求并使用JSP编译器，把JSP页面转换成servlet类。请注意JSP文件只在第一次页面请求时被编译，除非JSP文件改变了。生成的servlet类被调用，来处理浏览器的请求。一旦对于请求的执行完成，servlet就把response返回给客户端。请看 [how
 to get Request parameters in a JSP](http://examples.javacodegeeks.com/enterprise-java/jsp/get-request-parameter-in-jsp-page/).

109. JSP有什么优点？ 使用JSP技术的优点如下：
- 
JSP页面是动态编译成servlet的，所以，开发者可以容易的对展现代码进行更新。
- 
JSP页面可以被预编译。
- 
JSP页面可以容易的与静态模板结合，包含HTML或XML片段，和能够产生动态内容的代码。
- 
开发者可以开发自定义的JSP标签库，页面开发者能够使用类似XML的代码来访问它们。
- 
开发者可以在组件级别上对逻辑进行修改，而不用编辑使用应用逻辑的独立页面。

110. 什么是指令(Directive)?  JSP中可以使用的不同类型的指令(Directive)有哪些？ 指令(Directive)是当页面被编译成servlet时由JSP引擎处理的指令。指令被用于设置页面级别的指令，插入数据到外部文件，和指定自定义的标签库。指令被定义在< %@和% >之间。不同类型的指令有以下的类型：
- 
Include指令：用于包含一个文件，并将它的内容与当前页面合并。
- 
Page指令：用于定义JSP页面中的特定属性，比如错误页面和缓存。
- 
Taglib指令：用于声明页面使用的自定义标签库。

111. 什么是JSP action? JSP action使用XML代码中的结构来控制servlet引擎的行为。JSP action在JSP页面被请求时执行。它们可以被动态的插入一个文件中个，重用JavaBean组件， 转发用户到另一个页面，或者为Java插件生成HTMl。一些可用的action如下：
- 
jsp:include – 当JSP页面被请求时，用来包含一个文件。
- 
jsp:useBean – 查找或实例化一个JavaBean。
- 
jsp:setProperty – 设置JavaBean的属性。
- 
jsp:getProperty– 得到JavaBean的属性。
- 
jsp:forward– 转发请求到一个新页面。
- 
jsp:plugin– 生成浏览器特定的代码。

112. 什么是Scriptlet？ 在JSP技术中，Scriptlet是一段嵌入到JSP页面中的java代码。在标签之间的一切都是Scriptlet，用户可以添加任何有效的Scriptlet。

113. 什么是声明(Declarations)？声明(Declarations)类似Java中的变量声明。声明(Declarations)用于定义一个变量，可以用在随后的表达式或Scriptlet中。要添加一个声明，必须使用sequence来关闭声明。

114. 什么是表达式(expression)？JSP的表达式(expression)用来在脚本语言表达式中插入值，它被web服务器转换为字符串，返回到客户端的数据流。表达式被定义在介于 <% =和%>标签之间。

115. 什么是是隐含对象，有哪些？ JSP的隐含对象是指JSP容器在每一个页面中提供给开发者使用的Java对象。开发者可以直接的调用它们，而不用显示的声明。JSP隐含对象也被称为预定义的变量。以下的对象被认为是JSP页面中的隐含对象：
- `application`
- `page`
- `request`
- `response`
- `session`
- `exception`
- `out`
- `config`
- `pageContext`


噢，真是一篇够长的文章，包含了各种类型的Java面试题。如果您喜欢它, 请[订阅我们的 newsletter](http://eepurl.com/xRlkj) 来获得每周的更新和赠送的白皮书!
 另外，请查看[JCG Academy](http://academy.javacodegeeks.com/)


来获得更多高级的培训。


那么，还有哪些其他的Java面试题呢？请在comment中让我们知道吧，我们会把它包含进来的。祝您编码愉快！

|Translated by: Qiuxiang Yang|
|----|
|This post is a translation of [115 Java Interview Questions and Answers – The ULTIMATE List](http://www.javacodegeeks.com/2014/04/java-interview-questions-and-answers.html) from [Sotirios-Efstathios Maneas](http://www.javacodegeeks.com/author/stathis-maneas/)|





