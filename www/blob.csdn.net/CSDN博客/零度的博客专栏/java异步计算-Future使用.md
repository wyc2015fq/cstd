# java异步计算-Future使用 - 零度的博客专栏 - CSDN博客
2016年05月20日 10:14:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：2158
#### 1、异步计算实现过程
       从jdk1.5开始我们可以利用Future来跟踪异步计算的结果。在此之前主线程要想获得工作线程（异步计算线程）的结果是比较麻烦的事情，需要我们进行特殊的程序结构设计，比较繁琐而且容易出错。有了Future我们就可以设计出比较优雅的异步计算程序结构模型：根据分而治之的思想，我们可以把异步计算的线程按照职责分为3类：
      1. 异步计算的发起线程（控制线程）：负责异步计算任务的分解和发起，把分解好的任务交给异步计算的work线程去执行，发起异步计算后，发起线程可以获得Futrue的集合，从而可以跟踪异步计算结果
      2. 异步计算work线程：负责具体的计算任务
      3. 异步计算结果收集线程：从发起线程那里获得Future的集合，并负责监控Future的状态，根据Future的状态来处理异步计算的结果。
#### 2、Future接口介绍
       如果需要设定代码执行的最长时间，即超时，可以用Java线程池ExecutorService类配合Future接口来实现。 Future接口是Java标准API的一部分，在java.util.concurrent包中。Future接口是Java线程Future模式的实 现，可以来进行异步计算。Future模式可以这样来描述：我有一个任务，提交给了Future，Future替我完成这个任务。期间我自己可以去做任何想做的事情。一段时 间之后，我就便可以从Future那儿取出结果。就相当于下了一张订货单，一段时间后可以拿着提订单来提货，这期间可以干别的任何事情。其中Future
 接口就是订货单，真正处理订单的是Executor类，它根据Future接口的要求来生产产品。
       Future接口提供方法来检测任务是否被执行完，等待任务执行完获得结果，也可以设置任务执行的超时时间。这个设置超时的方法就是实现Java程 序执行超时的关键。Future接口是一个泛型接口，严格的格式应该是Future<V>，其中V代表了Future执行的任务返回值的类型。 Future接口的方法介绍如下： 
      boolean **cancel** (boolean mayInterruptIfRunning) 取消任务的执行。参数指定是否立即中断任务执行，或者等等任务结束。
      boolean **isCancelled** () 任务是否已经取消，任务正常完成前将其取消，则返回 true
boolean **isDone** () 任务是否已经完成。需要注意的是如果任务正常终止、异常或取消，都将返回true
      V **get** () throws InterruptedException, ExecutionException  等待任务执行结束，然后获得V类型的结果。InterruptedException 线程被中断异常， ExecutionException任务执行异常，如果任务被取消，还会抛出`CancellationException`
`  `V **get** (long timeout,
 TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException 同上面的get功能一样，多了设置超时时间。参数timeout指定超时时间，uint指定时间的单位，在枚举类TimeUnit中有相关的定义。如果计 算超时，将抛出TimeoutException。
#### 3、示例代码
```
package zmx.future.jdk.test;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
public class AsyncCompute {
	
	//线程池
	private ExecutorService executorService;
	
	//保存异步计算的Future
	private FutureContext<String> context;
	
	public AsyncCompute(){
		this.executorService = Executors.newFixedThreadPool(100);
		this.context = new FutureContext<String>();
	}
	
	public static void main(String[] args) {
		//启动异步计算
		AsyncCompute asyncCompute = new AsyncCompute();
		asyncCompute.startAsyncCompution();
		
		//启动异步计算结果输出线程
		OutputResult output = new OutputResult();
		output.setContext(asyncCompute.getContext());
		new Thread(output).start();
		
	}
	
	public FutureContext<String> getContext() {
		return context;
	}
	public void setContext(FutureContext<String> context) {
		this.context = context;
	}
	private void startAsyncCompution() {
		
        /** 
         * 开启100个异步计算，每个异步计算线程随机sleep几秒来模拟计算耗时。 
         */  
        final Random random = new Random();  
        for (int i = 0; i < 100; i++) {  
            Future<String> future = this.executorService  
                    .submit(new Callable<String>() {  
                        @Override  
                        public String call() throws Exception {  
                            int randomInt = random.nextInt(5);  
                            Thread.sleep(randomInt * 1000);  
                            return "" + randomInt;  
                       }  
                    });  
            //每个异步计算的结果存放在context中  
            this.context.addFuture(future);  
        }
		
		
	}
	public static class FutureContext<T>{
		
	     private List<Future<T>> futureList  = new ArrayList<Future<T>>();
		
             public void addFuture(Future<T> future) {  
    	         this.futureList.add(future);  
             }  
             public List<Future<T>> getFutureList() {  
                 return this.futureList;  
             } 
		
	}
	
	public static class OutputResult implements Runnable{
		
		private FutureContext<String> context;		
		public FutureContext<String> getContext() {
			return context;
		}
		public void setContext(FutureContext<String> context) {
			this.context = context;
		}
		@Override
		public void run() {
			
	           System.out.println("start to output result:");  
	           List<Future<String>> list = this.context.getFutureList();  
	         
	           for (Future<String> future : list) {  
	                 this.outputResultFromFuture(future);  
	           }  	             
	           System.out.println("finish to output result.");
		}
		
	    private void outputResultFromFuture(Future<String> future) {  
              try {  
                   while (true) {  
                       if (future.isDone() && !future.isCancelled()) {  
                           System.out.println("Future:" + future + ",Result:"  
                                   + future.get());  
                           break;  
                       } else {  
                           Thread.sleep(1000);  
                      }  
                   }  
               } catch (Exception e) {  
                   e.printStackTrace();  
               }  
	    } 
		
	}
	
	
}
```
输出结果：
```
start to output result:
Future:java.util.concurrent.FutureTask@1cb20da,Result:3
Future:java.util.concurrent.FutureTask@2b6b0c24,Result:4
Future:java.util.concurrent.FutureTask@ea25c1,Result:1
Future:java.util.concurrent.FutureTask@c54a25f,Result:3
Future:java.util.concurrent.FutureTask@870114c,Result:4
Future:java.util.concurrent.FutureTask@757f98e7,Result:4
Future:java.util.concurrent.FutureTask@c0fa1f5,Result:0
Future:java.util.concurrent.FutureTask@5651e202,Result:3
......
finish to output result.
```
