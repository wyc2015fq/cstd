# 关于Future.cancel(false) - z69183787的专栏 - CSDN博客
2019年02月26日 11:56:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：33
个人分类：[多线程](https://blog.csdn.net/z69183787/article/category/2176989)
[https://blog.csdn.net/StefanTimber/article/details/73823689](https://blog.csdn.net/StefanTimber/article/details/73823689)
前言
在学习Future接口的过程中，注意到它具有一个cancel()方法，用于取消异步的任务，它传入一个boolean类型的参数，传入true会中断线程停止任务，而传入false则会让线程正常执行至完成，并返回false。 
由此让我产生了疑问，false参数并不会停止任务，那么cancel(false)有什么用呢？
Future简介
Future接口用于获取异步计算的结果，可通过get()获取结果、cancel()取消、isDone()判断是否完成等操作。
```java
V get()： 获取结果，若无结果会阻塞至异步计算完成
V get(long timeOut, TimeUnit unit)：获取结果，超时返回null
boolean isDone()：执行结束（完成/取消/异常）返回true
boolean isCancelled()：任务完成前被取消返回true
boolean cancel(boolean mayInterruptRunning)：取消任务，未开始或已完成返回false，参数表示是否中断执行中的线程
cancel（）中的false参数
```
如上面所介绍的，传入true会中断线程停止任务，传入false则会让线程正常执行至完成，刚开始我难以理解传入false的作用，既然不会中断线程，那么这个cancel方法不就没有意义了吗？后来查阅了许多资料，在stackoverflow上找到了一个比较好的解释，终于恍然大悟。
简单来说，传入false参数只能取消还没有开始的任务，若任务已经开始了，就任由其运行下去。
当创建了Future实例，任务可能有以下三种状态：
**等待状态。**此时调用cancel()方法不管传入true还是false都会标记为取消，任务依然保存在任务队列中，但当轮到此任务运行时会直接跳过。
**完成状态。**此时cancel()不会起任何作用，因为任务已经完成了。
**运行中。**此时传入true会中断正在执行的任务，传入false则不会中断。
总结
Future.cancel(true)适用于： 
1. 长时间处于运行的任务，并且能够处理interruption
Future.cancel(false)适用于： 
1. 未能处理interruption的任务 
2. 不清楚任务是否支持取消 
3. 需要等待已经开始的任务执行完成
