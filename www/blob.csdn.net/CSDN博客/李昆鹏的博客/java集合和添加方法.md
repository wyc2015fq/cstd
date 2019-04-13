
# java集合和添加方法 - 李昆鹏的博客 - CSDN博客


2018年03月17日 13:54:05[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：677


**-----------------------java集合和添加方法-----------------------------**
**复习数组及它的缺点**
**需求：把一个你班级的学生存储起来，使用数组。**
**数组的长度是固定的，无法满足动态的数据添加。**
**面试题：数组和集合的区别。**
**数组：长度固定**
**可以储存基本数据类型，也能存储对象。**
**如果使用****Object****数组，可以存储任意类型。**
**集合：长度可变**
**只能存储对象类型（由于有包装类的存在，集合可以存储任何类型）。**
**例子：**
**public****class**Test1 {
**public****static****void**main(String[]args)**throws**Exception{
//设置一个日期
//Stringstr="1999-10-23";
//Date date = newSimpleDateFormat("yyyy-MM-dd").parse(str);
//创建10个学生对象
//定义一个学生类数组
Student[]str1=**new**Student[10];
**for**(**int**i= 0;i< 10;i++) {
str1[i] =**new**Student(i+1,"张三",1 ,**new**Date());
}
//遍历学生类数组
**for**(**int**i= 0;i<str1.length;i++) {
System.**out**.println(str1[i]);
}
//如果现在又有一个学生加入怎么办？
//会发生数组越界，数组作为集合是有局限性，长度固定
//str[10] = new Student(i+1,"张三",1 , new Date());
//设置一个数组什么类型都能放
//使用数组可以储存任何数据类型
Object[]objs=**new**Object[10];
objs[0] = 1;
objs[1] ="李四";
objs[2] =**new**Student(1,"李昆鹏",1 ,**new**Date());
**for**(**int**i= 0;i<objs.length;i++) {
System.**out**.println(objs[i]);
}
}
}
结果：
Student [id=1, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=2, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=3, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=4, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=5, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=6, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=7, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=8, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=9, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
Student [id=10, name=张三, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
1
李四
Student [id=1, name=李昆鹏, gender=1, birthday=Sat Mar 17 11:55:26 CST 2018]
null
null
null
null
null
null
null


## java.util
## 接口 Collection<E>
**所有超级接口：**
Iterable<E>
**所有已知子接口：**
BeanContext,BeanContextServices,BlockingDeque<E>,BlockingQueue<E>,Deque<E>,List<E>,NavigableSet<E>,Queue<E>,Set<E>,SortedSet<E>
**所有已知实现类：**
AbstractCollection,AbstractList,AbstractQueue,AbstractSequentialList,AbstractSet,ArrayBlockingQueue,ArrayDeque,ArrayList,AttributeList,BeanContextServicesSupport,BeanContextSupport,ConcurrentLinkedQueue,ConcurrentSkipListSet,CopyOnWriteArrayList,CopyOnWriteArraySet,DelayQueue,EnumSet,HashSet,JobStateReasons,LinkedBlockingDeque,LinkedBlockingQueue,LinkedHashSet,LinkedList,PriorityBlockingQueue,PriorityQueue,RoleList,RoleUnresolvedList,Stack,SynchronousQueue,TreeSet,Vector
---

public interface**Collection<E>**extendsIterable<E>*Collection 层次结构*中的根接口。Collection表示一组对象，这些对象也称为 collection 的*元素*。一些 collection 允许有重复的元素，而另一些则不允许。一些 collection 是有序的，而另一些则是无序的。JDK 不提供此接口的任何*直接*实现：它提供更具体的子接口（如Set和List）实现。此接口通常用来传递 collection，并在需要最大普遍性的地方操作这些 collection。
*包 (bag)*或*多集合 (multiset)*（可能包含重复元素的无序 collection）应该直接实现此接口。
所有通用的Collection实现类（通常通过它的一个子接口间接实现Collection）应该提供两个“标准”构造方法：一个是 void（无参数）构造方法，用于创建空 collection；另一个是带有Collection类型单参数的构造方法，用于创建一个具有与其参数相同元素新的 collection。实际上，后者允许用户复制任何 collection，以生成所需实现类型的一个等效 collection。尽管无法强制执行此约定（因为接口不能包含构造方法），但是 Java 平台库中所有通用的Collection实现都遵从它。
此接口中包含的“破坏性”方法，是指可修改其所操作的 collection 的那些方法，如果此 collection 不支持该操作，则指定这些方法抛出UnsupportedOperationException。如果是这样，那么在调用对该 collection 无效时，这些方法可能，但并不一定抛出UnsupportedOperationException。例如，如果要添加的 collection 为空且不可修改，则对该 collection 调用`addAll(Collection)`方法时，可能但并不一定抛出异常。
一些 collection 实现对它们可能包含的元素有所限制。例如，某些实现禁止 null 元素，而某些实现则对元素的类型有限制。试图添加不合格的元素将抛出一个未经检查的异常，通常是NullPointerException或ClassCastException。试图查询是否存在不合格的元素可能抛出一个异常，或者只是简单地返回 false；某些实现将表现出前一种行为，而某些实现则表现后一种。较为常见的是，试图对某个不合格的元素执行操作且该操作的完成不会导致将不合格的元素插入 collection 中，将可能抛出一个异常，也可能操作成功，这取决于实现本身。这样的异常在此接口的规范中标记为“可选”。
由每个 collection 来确定其自身的同步策略。在没有实现的强烈保证的情况下，调用由另一进程正在更改的 collection 的方法可能会出现不确定行为；这包括直接调用，将collection 传递给可能执行调用的方法，以及使用现有迭代器检查 collection。
Collections Framework 接口中的很多方法是根据`equals`方法定义的。例如，`contains(Object o)`方法的规范声明：“当且仅当此 collection 包含至少一个满足(o==null ? e==null :o.equals(e))的元素e时，返回true。”*不*应将此规范理解为它暗指调用具有非空参数o的Collection.contains方法会导致为任意的e元素调用o.equals(e)方法。可随意对各种实现执行优化，只要避免调用equals即可，例如，通过首先比较两个元素的哈希码。（`Object.hashCode()`规范保证哈希码不相等的两个对象不会相等）。较为常见的是，各种 Collections Framework 接口的实现可随意利用底层`Object`方法的指定行为，而不管实现程序认为它是否合适。
此接口是JavaCollections Framework的一个成员。
-------------------------------------------------------
**集合的体系结构：**
集合也叫容器用于存储对象。
我们根据不同的需求和不同的数据结构来对集合做了不同的抽象。
![](https://img-blog.csdn.net/20180324211942427?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|构造方法摘要
|
|ArrayList|()
|构造一个初始容量为 10 的空列表。
|
|ArrayList|(|Collection|<? extends|E|> c)
|构造一个包含指定 collection 的元素的列表，这些元素是按照该  collection 的迭代器返回它们的顺序排列的。
|
|ArrayList|(int initialCapacity)
|构造一个具有指定初始容量的空列表。
|
**----------------集合的方法-------------------------**
添加：
|boolean
|add|(|E|e)
|确保此 collection 包含指定的元素（可选操作）。
|
|boolean
|addAll|(|Collection|<? extends|E|> c)
|将指定 collection 中的所有元素都添加到此 collection 中（可选操作）。
|
例子：
**public****class**CollectionTest1{
**public****static****void**main(String[]args) {
//创建一个集合
Collectioncoll=**new**ArrayList();
**for**(**int**i= 0;i< 10;i++) {
coll.add("张三"+i);
}
System.**out**.println(coll);
//创建第一个集合
Collectioncoll1=**new**ArrayList();
**for**(**int**i= 0;i< 10;i++) {
coll1.add("李四"+i);
}
//把一个集合中的内容加入到第一个集合中
coll.addAll(coll1);
System.**out**.println(coll1);
System.**out**.println(coll);
//集合中可以存储任何数据类型
Collectioncoll2=**new**ArrayList();
coll2.add(1);
coll2.add(**new**Student(1,"王五", 1,**new**Date()));
coll2.add(**new**Date());
System.**out**.println(coll2);
//集合中存储集合
Collectioncoll3=**new**ArrayList();
**for**(**int**i= 0;i< 3;i++) {
Collectioncoll4=**new**ArrayList();
**for**(**int**j= 0;j< 5;j++) {
coll4.add(i+"数据"+j);
}
//把内存集合加入到外层集合中
coll3.add(coll4);
}
System.**out**.println(coll3);
/*
*结果以下
*[[0数据0, 0数据1, 0数据2, 0数据3, 0数据4],
*  [1数据0, 1数据1, 1数据2, 1数据3, 1数据4],
*  [2数据0, 2数据1, 2数据2, 2数据3, 2数据4]]
*/
//创建一个班级对象
CollectionTest2c=**new**CollectionTest2();
c.setClazzId(1);
c.setStuNo(3);
Students=**new**Student(1,"李达", 1,**new**Date());
Students1=**new**Student(2,"李贵", 1,**new**Date());
Students2=**new**Student(3,"张飞", 1,**new**Date());
//创建一个存储学生对象的集合
Collectioncs=**new**ArrayList();
cs.add(s);
cs.add(s1);
cs.add(s2);
//把集合设置给班级对象
c.setCol(cs);
System.**out**.println(c);
/*
*结果以下
*[clazzId=1,
*stuNo=3,
*col=[
*Student [id=1, name=李达, gender=1, birthday=Sat Mar 17 13:46:42CST 2018],
*Student [id=2, name=李贵, gender=1, birthday=Sat Mar 17 13:46:42CST 2018],
*Student [id=3, name=张飞, gender=1, birthday=Sat Mar 17 13:46:42CST 2018]]]
*/
}
}
结果：
[张三0,张三1,张三2,张三3,张三4,张三5,张三6,张三7,张三8,张三9]
[李四0,李四1,李四2,李四3,李四4,李四5,李四6,李四7,李四8,李四9]
[张三0,张三1,张三2,张三3,张三4,张三5,张三6,张三7,张三8,张三9,李四0,李四1,李四2,李四3,李四4,李四5,李四6,李四7,李四8,李四9]
[1, Student [id=1,name=王五, gender=1, birthday=Sat Mar 17 13:46:42 CST2018], Sat Mar 17 13:46:42 CST 2018]
[[0数据0, 0数据1, 0数据2, 0数据3, 0数据4],
[1数据0, 1数据1, 1数据2, 1数据3, 1数据4],
[2数据0, 2数据1, 2数据2, 2数据3, 2数据4]]
CollectionTest2[clazzId=1, stuNo=3,
col=
[Student [id=1, name=李达, gender=1, birthday=Sat Mar 17 13:46:42 CST 2018],
Student [id=2, name=李贵, gender=1,birthday=Sat Mar 17 13:46:42 CST 2018],
Student [id=3, name=张飞, gender=1,birthday=Sat Mar 17 13:46:42 CST 2018]]]


