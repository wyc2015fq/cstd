# java关键字含义 - zkq_1986的博客 - CSDN博客





2016年12月23日 15:02:45[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：393










# 1.  transient



transient，英文含义是即时、暂时，在java中充当关键字，表示不跟类一起序列化，其目的是为了安全性考虑，以防当前字段通过序列化被非法读取。

举例：

class A extends Serializable{

       transientint a;

   int b;

}

当类A序列化时，字段a不会序列化，字段b会序列化，存到磁盘。




# 2.  volatile



volatile，英文含义是易变的，在java中充当关键字，表示当前字段每次操作前都要从内存中读取最新的值，操作完后马上写回内存，主要用在多线程上。

举例：

class A extends Runnable{

volatileint a;

   int b;

}

需要注意的是，即使加了volatile关键字还是不能保证线程安全，它只能保证每次操作前会去内存中读取最新的值。要想保证线程安全，还是需要加对象锁。




