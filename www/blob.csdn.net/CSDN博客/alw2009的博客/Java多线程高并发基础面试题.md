# Java多线程高并发基础面试题 - alw2009的博客 - CSDN博客





2016年09月19日 17:13:47[traveler_zero](https://me.csdn.net/alw2009)阅读数：3088








1.进程和线程有什么不同？ （个人理解， 不是标准答案）

           线程是CPU做任务调度的基本单元， 但是线程不能独立于进程运行，只能属于某个进程， 使用该进程的地址空间。

 一个进程至少拥有一个线程， 进程和线程之间是拥有和被拥有的关系。

参考：    [https://javarevisited.blogspot.com/2015/12/difference-between-thread-and-process.html](https://javarevisited.blogspot.com/2015/12/difference-between-thread-and-process.html)



2.多线程编程的好处。

          充分利用CPU， 相比于多进程， 多线程的的性能会更好， 因为同一个进程内做任务切换会更快。



3. 用户线程和守护线程有什么区别？


||目的|创建|地位|
|----|----|----|----|
|用户线程|干自己的事情|[java创建线程的方法三种方法及对比](http://blog.csdn.net/longshengguoji/article/details/41126119)|最后一个用户线程退出， JVM退出|
|守护线程			（天使线程）|为其他线程服务|public final void setDaemon(boolean on)|没地位|



4. 线程状态



     Runable<-->Running<---->Dead

                   \              |

                        Blocking/Wait



5。 让线程暂停

          Thread.sleep()







