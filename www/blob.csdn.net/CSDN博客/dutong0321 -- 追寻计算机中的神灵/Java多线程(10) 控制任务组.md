# Java多线程(10) 控制任务组 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月26日 19:33:17[dutong0321](https://me.csdn.net/dutong0321)阅读数：136
#### 概述
执行器服务作为线程池，不仅仅可以提高执行任务的效率，还可以控制一组相关任务。例如，可以在执行器中使用shutdownNow方法取消所有的任务。
#### 控制任务组
```
java.util.concurrent.ExecutorService 
T invokeAny(Collection<Callable<T>> tasks)
T invokeAny(Collection<Callable<T>> tasks, long timeout,TimeUnit unit)
// 执行给定的任务，返回其中一个任务的结果（程序任意一个已完成的结果）。
// 第二个方法若发生超时，抛出一个TimeOutException异常
List<Future<T>> invokeAll(Collection<Callable<T>> tasks)
List<Future<T>> invokeAll(Collection<Callable<T>> tasks,long timeout, TimeUnit unit)
// 执行给定的任务，返回所有的结果。第二个方法若发生超时，抛出一个Timeout Exception异常
java.util.concurrent.ExecutorCompletionService<V>
ExecutorCompletionService(Executor e)
// 构建一个执行器完成服务来收集给定执行器的结果
Future<V> submit(Callable<V> task)
Future<V> submit(Runnable task,V result) 
// 提交一个任务给底层的执行器
Future<V> take()
// 移除下一个已完成的结果，如果没有任何已完成的结果可用则阻塞
Future<V> poll()
Future<V> poll(long time,TimeUnit unit)
//移除下一个已完成的结果，如果没有已完成的结果可用则返回null。第二个方法将等待给定的时间。
```
PS：其中invokeAll有个特殊的地方就是：如果其创建3个任务，他会新建2个子线程，然后剩下的任务会继续在本线程去执行。
