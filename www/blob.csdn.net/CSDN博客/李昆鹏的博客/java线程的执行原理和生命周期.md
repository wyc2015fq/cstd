
# java线程的执行原理和生命周期 - 李昆鹏的博客 - CSDN博客


2018年04月13日 22:47:36[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：49


--------------------------------------java线程的执行原理和生命周期------------------------------------------
**线程的执行原理**

![](https://img-blog.csdn.net/20180413224650341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

线程的并发执行通过多个线程不断切换CPU的资源，这个速度非常快，
我们都感知不到，我们能感知到的就是三个线程在并发执行。

**线程的生命周期**
1.新建：线程被new出来
2.准备就绪：线程具有执行的资格，即线程调用了start()方法，没有执行权利。
3.运行：具备执行的资格和具备执行的权利。
4.阻塞：没有执行的资格和执行的权利。
5.销毁：线程的对象变成垃圾，释放资源。
![](https://img-blog.csdn.net/20180413224709149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



