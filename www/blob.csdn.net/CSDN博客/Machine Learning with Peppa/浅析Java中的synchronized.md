# 浅析Java中的synchronized - Machine Learning with Peppa - CSDN博客





2018年05月08日 22:46:02[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：86标签：[Java																[并行																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[编程之美：Java](https://blog.csdn.net/qq_39521554/article/category/7599598)

所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)](https://so.csdn.net/so/search/s.do?q=并行&t=blog)




Java语言的关键字，当它用来修饰一个方法或者一个代码块的时候，能够保证在同一时刻最多只有一个线程执行该段代码。

     一、当两个并发线程访问同一个对象object中的这个synchronized(this)同步代码块时，一个时间内只能有一个线程得到执行。另一个线程必须等待当前线程执行完这个代码块以后才能执行该代码块。

     二、然而，当一个线程访问object的一个synchronized(this)同步代码块时，另一个线程仍然可以访问该object中的非synchronized(this)同步代码块。

     三、尤其关键的是，当一个线程访问object的一个synchronized(this)同步代码块时，其他线程对object中所有其它synchronized(this)同步代码块的访问将被阻塞。

     四、第三个例子同样适用其它同步代码块。也就是说，当一个线程访问object的一个synchronized(this)同步代码块时，它就获得了这个object的对象锁。

         结果，其它线程对该object对象所有同步代码部分的访问都被暂时阻塞。

     五、以上规则对其它对象锁同样适用.

举例说明：

     一、当两个并发线程访问同一个对象object中的这个synchronized(this)同步代码块时，一个时间内只能有一个线程得到执行。另一个线程必须等待当前线程执行完这个代码块以后才能执行该代码块。

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427113245642-563164129.png)

   结果：

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427113316892-189702904.png)

   二、然而，当一个线程访问object的一个synchronized(this)同步代码块时，另一个线程仍然可以访问该object中的非synchronized(this)同步代码块。

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427132946814-355358370.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427132956330-1970231592.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427133258158-119189911.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427133023595-117870443.png)

三、尤其关键的是，当一个线程访问object的一个synchronized(this)同步代码块时，其他线程对object中所有其它synchronized(this)同步代码块的访问将被阻塞。

  修改说明二的例子中genNotThread()方法，如下

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427133821298-2109430022.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427133850392-1033181966.png)

四、第三个例子同样适用其它同步代码块。也就是说，当一个线程访问object的一个synchronized(this)同步代码块时，它就获得了这个object的对象锁。结果，其它线程对该object对象所有同步代码部分的访        问都被暂时阻塞。

      修改说明二的例子中genNotThread()方法，如下

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427134117548-1149472425.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427134146580-221313013.png)

 五、以上规则对其它对象锁同样适用:

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427135844689-734584353.png)

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427135911673-235455753.png)

结果：

尽管线程t1获得了对Inner的对象锁，但由于线程t2访问的是同一个Inner中的非同步部分。所以两个线程互不干扰。

![](https://images2015.cnblogs.com/blog/903762/201604/903762-20160427135958720-80239874.png)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




