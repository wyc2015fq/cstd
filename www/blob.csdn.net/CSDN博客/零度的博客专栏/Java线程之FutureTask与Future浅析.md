# Java线程之FutureTask与Future浅析 - 零度的博客专栏 - CSDN博客
2016年06月06日 17:44:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：4174
#### 一、Future使用
        FutureTask是Future和Callable的结合体。传统的代码是这样写的 
        Future f = executor.submit(new Callable());
        然后通过Future来取得计算结果。但是，若开启了多个任务，我们无从知晓哪个任务最先结束。因此，若要实现“当某任务结束时，立刻做一些事情，例如记录日志”这一功能，就需要写一些额外的代码。例如一般的Future使用。
多任务：
```
package zmx.multithread.test.reentrantlock;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
public class T1 {
	public static void main(String[] args) throws InterruptedException, ExecutionException {
			
		 ExecutorService executor2= Executors.newFixedThreadPool(5); 		 
		 class Task implements Callable<String>{
			@Override
			public String call() throws Exception {
				
				Random rand = new Random();  
		        TimeUnit.SECONDS.sleep(rand.nextInt(10));  
		        return  Thread.currentThread().getName();
			}			 
		 }
		 
		 List<Future<String>> results = new ArrayList<Future<String>>();
		 for(int i=0;i<5;i++){
			 Future<String> f =  executor2.submit(new Task());
			 results.add(f);
		 }
 
		 boolean flag =true; 
		 while(flag) {
			
			for(Iterator<Future<String>> iter  = results.iterator();iter.hasNext();){
				Future<String> f =iter.next();
				if(f.isDone()){
					System.out.println(f.get());
					iter.remove();
					
				}
			}
			if(results.size()==0){
				flag =false;
			}
			
		}
		
		System.out.println("执行完毕");
		 
		executor2.shutdownNow();
		 
		 
		 
	}
}
```
执行结果：
pool-1-thread-4
pool-1-thread-2
pool-1-thread-1
pool-1-thread-5
pool-1-thread-3
执行完毕
二、FutureTask
      上述使用遍历的方式解决多任务结果，但是不是最优的效果，FutureTask正是为此而存在，它有一个回调函数protected void done()，当任务结束时，该回调函数会被触发。因此，只需重载该函数，即可实现在线程刚结束时就做一些事情。
代码如下：
```
public class Test {
	public static void main(String[] args) {
		ExecutorService executor = Executors.newCachedThreadPool();
		for(int i=0; i<5; i++) {
			Callable<String> c = new Task();
			MyFutureTask ft = new MyFutureTask(c);
			executor.submit(ft);
		}
		executor.shutdown();
	}
		
}
class MyFutureTask extends FutureTask<String> {
	public MyFutureTask(Callable<String> callable) {
		super(callable);
	}
	@Override
	protected void done() {
		try {
			System.out.println(get() + " 线程执行完毕！~");
		} catch (InterruptedException | ExecutionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
}
class Task implements Callable<String> {
	@Override
	public String call() throws Exception {
		Random rand = new Random();
		TimeUnit.SECONDS.sleep(rand.nextInt(12));
		return Thread.currentThread().getName();
	}
}
```
结果如下：
pool-1-thread-4 线程执行完毕！
pool-1-thread-3 线程执行完毕！
pool-1-thread-5 线程执行完毕！
pool-1-thread-2 线程执行完毕！
pool-1-thread-1 线程执行完毕！
