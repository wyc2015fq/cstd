# java的若干问题（4）——java常用类库 - 刘炫320的博客 - CSDN博客
2016年05月25日 20:33:35[刘炫320](https://me.csdn.net/qq_35082030)阅读数：383
[](http://blog.csdn.net/qq_35082030/article/details/51487980)
今天，我们来介绍java中常用类库的一些问题。这些问题都是比较基础的，更加深入的探究在后面。
1、Runtime类
这个类没有构造方法，因为这个类在jvm运行时就自动构造了。它的构造方法是私有的，属于单例模式。
这个类的作用是观察整个jvm运行状态的。
不过他有个方法exec();这个方法是用来执行进程的。要和process类搭配使用。
2、StringBuffer类与String类的区别。
StringBuffer类更适合在频繁修改字符串的时候使用，因为它不需改变引用地址，就可以完成字符串的改变。
而String类的话，每次改变字符串，都是改变字符串的引用地址。
因此，如果在以后的使用中，如果出现经常改变字符串内容的地方，要用StringBuffer。
3、StringBuffer类常用方法
字符串追加append();
字符串查找indexOf();
字符串翻转reverse();
字符串替换replace();
字符串删除delete();
字符串截取substring();
4、对象克隆技术
针对java里引用类型，每次一改就改了2个地方，特地用的克隆。
5、list接口
list接口是java类集中最常用的一种类型，它就像是链表一样进行使用。
它主要有以下几个方法
添加：add();
删除：remove();
查询；contains();
取字串：subList();
获取某个元素get();
替换某个元素set();
6、针对sublist进行以下说明
其中这个sublist是个引用类型，也就是说它指向的对象只是原列表中的部分列表，对于sublist的操作，同样会影响到原列表里的内容。
7、list的子类
arraylist是最常用的类型，就像是一个数组列表一样。
vector类是比较老的类了，早在List出现以前，就已经有了。这点在C++里面STL里面体现的极为突出，他们称之为容器。但是jdk1.2以后随着arraylist的出现，vector和arraylist的特性基本上重合了，而且性能上还比不上arraylist，因此vector仅作了解即可。
linkedlist也是list的子类，它主要是用来做链表操作的，它可以用来实现QUEUE，也就是队列，因为他提供了一下一些接口
在列表中增加元素add();
在表头增加元素 addFirst();
在表尾增加元素 addLast();
从表头中取出元素 poll();
8、set接口
set接口也是Collection接口的子接口，但是它正如它名字一样，set,集合的意思，集合是无序的、互异的、确定的。因此，在里面是没有办法进行排序和索引的，也不可能会有重复的元素存在。
9、set接口的子类 
set接口里，经常有hashset和treeset这两种子类，其中treeset是可以排序的。
在进行排序时，需要注意equals和hashcode的重写。
