# ExecutorService的execute和submit方法区别 - 零度的博客专栏 - CSDN博客
2016年05月18日 09:38:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：4185
主要有三个区别：
       1、接收的参数不一样。
[ExecutorService]()的submit([Callable]()<[T]()>
 task)参数是一个Callable对象,submit(Runnable task)也可以接受Runnable对象；e[xecutor]()Service.execute([Runnable]()
 command)的参数是一个Runnable对象。
        2、submit有返回值，而execute没有。
        Method submit extends base method Executor.execute by creating and returning a Future that can be used to cancel execution and/or wait for completion. 用到返回值的例子，比如说我有很多个做validation的task，我希望所有的task执行完，然后每个task告诉我它的执行结果，是成功还是失败，如果是失败，原因是什么。然后我就可以把所有失败的原因综合起来发给调用者。个人觉得cancel
 execution这个用处不大，很少有需要去取消执行的，而最大的用处应该是第二点。
        3、submit方便Exception处理。
        There is a difference when looking at exception handling. If your tasks throws an exception and if it was
 submitted with `execute` this
 exception will go to the uncaught exception handler (when you don't have provided one explicitly, the default one will just print the stack trace to `System.err`).
 If you submitted the task with `submit`any thrown
 exception, checked or not, is then part of the task's return status. For a task that was submitted with `submit` and
 that terminates with an exception, the `Future.get` will
 rethrow this exception, wrapped in an `ExecutionException`.意思就是如果你在你的task里会抛出checked或者unchecked
 exception，而你又希望外面的调用者能够感知这些exception并做出及时的处理，那么就需要用到submit，通过捕获Future.get抛出的异常。
       例如：我有很多更新各种数据的task，我们希望如果其中一个task失败，其它的task就不需要执行了。那我们就需要catch Future.get()抛出的异常，然后终止其它task的执行。代码如下：
1、异常类
```
package zmx.crontab.test;
public class TaskException extends Exception {
    private static final long serialVersionUID = -446446001315434118L;
    public TaskException(String message) {  
        super(message);  
    } 	
}
```
2、任务类
```
package zmx.crontab.test;
import java.util.Random;
import java.util.concurrent.Callable;
public class TaskWithResult implements Callable<String>{
	
    private int id;  
  
    public TaskWithResult(int id) {  
        this.id = id;  
    } 
	
    /**
     * 任务的具体过程，一旦任务传给ExecutorService的submit方法，则该方法自动在一个线程上执行。
     */
    @Override
    public String call() throws Exception {
		
        System.out.println("call()方法被自动调用,干活！！！    " + Thread.currentThread().getName());  
        if (new Random().nextBoolean())  
            throw new TaskException("Meet error in task:" + Thread.currentThread().getName());  
        // 一个模拟耗时的操作  
        for (int i = 999999999; i > 0; ){
        	i--;
        }  
          
        return "call()方法被自动调用，任务的结果是：" + id + "    " + Thread.currentThread().getName();
    }
}
```
3、测试类
```
package zmx.crontab.test;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
public class ExecutorServiceTest {
	
	public static void main(String[] args) {
		
		ExecutorService executorService = Executors.newCachedThreadPool();
		List<Future<String>> resultList = new ArrayList<Future<String>>();
		
		for(int i=0;i<10;i++){
            // 使用ExecutorService执行Callable类型的任务，并将结果保存在future变量中  
            Future<String> future = executorService.submit(new TaskWithResult(i));  
            // 将任务执行结果存储到List中  
            resultList.add(future); 			
		}
		
		executorService.shutdown();  //以及提交的任务会继续执行		
		for (Future<String> fs : resultList) {  
	        try {  
		            System.out.println(fs.get()); // 打印各个线程（任务）执行的结果  
	            } catch (InterruptedException e) {  
		            e.printStackTrace();  
	            } catch (ExecutionException e) {  
	                executorService.shutdownNow();  //立即终止所有任务
	                e.printStackTrace();  
	                return;  
	            }  
        }
				
		
	}
}
```
执行的结果类似于：
```
call()方法被自动调用,干活！！！             pool-1-thread-2
call()方法被自动调用,干活！！！             pool-1-thread-3
call()方法被自动调用,干活！！！             pool-1-thread-6
call()方法被自动调用,干活！！！             pool-1-thread-1
call()方法被自动调用,干活！！！             pool-1-thread-4
call()方法被自动调用,干活！！！             pool-1-thread-5
call()方法被自动调用,干活！！！             pool-1-thread-7
call()方法被自动调用,干活！！！             pool-1-thread-9
call()方法被自动调用，任务的结果是：0    pool-1-thread-1
call()方法被自动调用,干活！！！             pool-1-thread-10
call()方法被自动调用,干活！！！             pool-1-thread-8
call()方法被自动调用，任务的结果是：1    pool-1-thread-2
call()方法被自动调用，任务的结果是：2    pool-1-thread-3
call()方法被自动调用，任务的结果是：3    pool-1-thread-4
java.util.concurrent.ExecutionException: zmx.crontab.test.TaskException: Meet error in task:pool-1-thread-5
	at java.util.concurrent.FutureTask$Sync.innerGet(FutureTask.java:252)
	at java.util.concurrent.FutureTask.get(FutureTask.java:111)
	at zmx.crontab.test.ExecutorServiceTest.main(ExecutorServiceTest.java:28)
Caused by: zmx.crontab.test.TaskException: Meet error in task:pool-1-thread-5
	at zmx.crontab.test.TaskWithResult.call(TaskWithResult.java:22)
	at zmx.crontab.test.TaskWithResult.call(TaskWithResult.java:1)
	at java.util.concurrent.FutureTask$Sync.innerRun(FutureTask.java:334)
	at java.util.concurrent.FutureTask.run(FutureTask.java:166)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
	at java.lang.Thread.run(Thread.java:724)
```
       可以看出：一旦某个task出错，其它的task就停止执行。
