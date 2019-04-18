# ExecutorService的execute和submit方法 - z69183787的专栏 - CSDN博客
2015年07月21日 16:07:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1515
三个区别：
1、接收的参数不一样
2、submit有返回值，而execute没有
Method submit extends base method Executor.execute by creating and returning a Future that can be used to cancel execution and/or wait for completion. 
用到返回值的例子，比如说我有很多个做validation的task，我希望所有的task执行完，然后每个task告诉我它的执行结果，是成功还是失败，如果是失败，原因是什么。然后我就可以把所有失败的原因综合起来发给调用者。
个人觉得cancel execution这个用处不大，很少有需要去取消执行的。
而最大的用处应该是第二点。
3、submit方便Exception处理
There is a difference when looking at exception handling. If your tasks throws an exception and if
 it was submitted with `execute` this
 exception will go to the uncaught exception handler (when you don't have provided one explicitly, the default one will just print the stack trace to `System.err`).
 If you submitted the task with `submit`any thrown
 exception, checked or not, is then part of the task's return status. For a task that was submitted with `submit` and
 that terminates with an exception, the `Future.get` will
 rethrow this exception, wrapped in an `ExecutionException`.
意思就是如果你在你的task里会抛出checked或者unchecked exception，而你又希望外面的调用者能够感知这些exception并做出及时的处理，那么就需要用到submit，通过捕获Future.get抛出的异常。
比如说，我有很多更新各种数据的task，我希望如果其中一个task失败，其它的task就不需要执行了。那我就需要catch Future.get抛出的异常，然后终止其它task的执行，代码如下：
51cto上有一篇非常好的文章“Java5并发学习”（[http://lavasoft.blog.51cto.com/62575/115112](http://lavasoft.blog.51cto.com/62575/115112)），下面的代码是基于它之上修改的。
- import java.util.ArrayList;  
- import java.util.List;  
- import java.util.Random;  
- import java.util.concurrent.Callable;  
- import java.util.concurrent.ExecutionException;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- import java.util.concurrent.Future;  
- 
- publicclass ExecutorServiceTest {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService executorService = Executors.newCachedThreadPool();  
-         List<Future<String>> resultList = new ArrayList<Future<String>>();  
- 
- // 创建10个任务并执行
- for (int i = 0; i < 10; i++) {  
- // 使用ExecutorService执行Callable类型的任务，并将结果保存在future变量中
-             Future<String> future = executorService.submit(new TaskWithResult(i));  
- // 将任务执行结果存储到List中
-             resultList.add(future);  
-         }  
-         executorService.shutdown();  
- 
- // 遍历任务的结果
- for (Future<String> fs : resultList) {  
- try {  
-                 System.out.println(fs.get()); // 打印各个线程（任务）执行的结果
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             } catch (ExecutionException e) {  
-                 executorService.shutdownNow();  
-                 e.printStackTrace();  
- return;  
-             }  
-         }  
-     }  
- }  
- 
- class TaskWithResult implements Callable<String> {  
- privateint id;  
- 
- public TaskWithResult(int id) {  
- this.id = id;  
-     }  
- 
- /**
-      * 任务的具体过程，一旦任务传给ExecutorService的submit方法，则该方法自动在一个线程上执行。
-      * 
-      * @return
-      * @throws Exception
-      */
- public String call() throws Exception {  
-         System.out.println("call()方法被自动调用,干活！！！             " + Thread.currentThread().getName());  
- if (new Random().nextBoolean())  
- thrownew TaskException("Meet error in task." + Thread.currentThread().getName());  
- // 一个模拟耗时的操作
- for (int i = 999999999; i > 0; i--)  
-             ;  
- return"call()方法被自动调用，任务的结果是：" + id + "    " + Thread.currentThread().getName();  
-     }  
- }  
- 
- class TaskException extends Exception {  
- public TaskException(String message) {  
- super(message);  
-     }  
- }  
执行的结果类似于：
- call()方法被自动调用,干活！！！             pool-1-thread-1
- call()方法被自动调用,干活！！！             pool-1-thread-2
- call()方法被自动调用,干活！！！             pool-1-thread-3
- call()方法被自动调用,干活！！！             pool-1-thread-5
- call()方法被自动调用,干活！！！             pool-1-thread-7
- call()方法被自动调用,干活！！！             pool-1-thread-4
- call()方法被自动调用,干活！！！             pool-1-thread-6
- call()方法被自动调用,干活！！！             pool-1-thread-7
- call()方法被自动调用,干活！！！             pool-1-thread-5
- call()方法被自动调用,干活！！！             pool-1-thread-8
- call()方法被自动调用，任务的结果是：0    pool-1-thread-1
- call()方法被自动调用，任务的结果是：1    pool-1-thread-2
- java.util.concurrent.ExecutionException: com.cicc.pts.TaskException: Meet error in task.pool-1-thread-3
-     at java.util.concurrent.FutureTask$Sync.innerGet(FutureTask.java:222)  
-     at java.util.concurrent.FutureTask.get(FutureTask.java:83)  
-     at com.cicc.pts.ExecutorServiceTest.main(ExecutorServiceTest.java:29)  
- Caused by: com.cicc.pts.TaskException: Meet error in task.pool-1-thread-3
-     at com.cicc.pts.TaskWithResult.call(ExecutorServiceTest.java:57)  
-     at com.cicc.pts.TaskWithResult.call(ExecutorServiceTest.java:1)  
-     at java.util.concurrent.FutureTask$Sync.innerRun(FutureTask.java:303)  
-     at java.util.concurrent.FutureTask.run(FutureTask.java:138)  
-     at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(ThreadPoolExecutor.java:886)  
-     at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:908)  
-     at java.lang.Thread.run(Thread.java:619)  
可以看见一旦某个task出错，其它的task就停止执行。
来自：http://blog.csdn.net/peachpi/article/details/6771946
