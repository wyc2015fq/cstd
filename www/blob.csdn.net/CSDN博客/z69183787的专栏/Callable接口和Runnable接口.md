# Callable接口和Runnable接口 - z69183787的专栏 - CSDN博客
2014年06月05日 16:23:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：774
Callable接口和Runnable接口相似，区别就是Callable需要实现call方法，而Runnable需要实现run方法；并且，**call方法还可以返回任何对象**，无论是什么对象，JVM都会当作Object来处理。但是如果使用了泛型，我们就不用每次都对Object进行转换了。
Runnable和Callable都是接口
不同之处：
1.Callable可以返回一个类型V，而Runnable不可以
2.Callable能够抛出checked exception,而Runnable不可以。
3.Runnable是自从java1.1就有了，而Callable是1.5之后才加上去的
4.Callable和Runnable都可以应用于executors。而Thread类只支持Runnable.
上面只是简单的不同，其实这两个接口在用起来差别还是很大的。Callable与executors联合在一起，在任务完成时可立刻获得一个更新了的Future。而Runable却要自己处理
  Future接口，一般都是取回Callable执行的状态用的。其中的主要方法：
- cancel，取消Callable的执行，当Callable还没有完成时
- get，获得Callable的返回值
- isCanceled，判断是否取消了
- isDone，判断是否完成
用Executor来构建线程池，应该要做的事：
1).调用Executors类中的静态方法newCachedThreadPool(必要时创建新线程，空闲线程会被保留60秒)或newFixedThreadPool(包含固定数量的线程池)等，返回的是一个实现了ExecutorService接口的ThreadPoolExecutor类或者是一个实现了ScheduledExecutorServiece接口的类对象。
2).调用submit提交Runnable或Callable对象。
3).如果想要取消一个任务，或如果提交Callable对象，那就要保存好返回的Future对象。
4).**当不再提交任何任务时，调用shutdown方法**。
举2个例子如下：
Java代码   ![收藏代码](http://junlas.iteye.com/images/icon_star.png)
- package thread.test04;  
- import java.util.concurrent.*;  
- publicclass ThreadTestA {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService e=Executors.newFixedThreadPool(10);  
-         e.execute(new MyRunnableA());  
-         e.execute(new MyRunnableB());  
-        e.shutdown();  
-    }  
- 
- }  
- 
- class MyRunnableA implements Runnable{  
- 
- publicvoid run(){  
-         System.out.println("Runnable:run()....");  
- int i=0;  
- while(i<20){  
-             i++;  
- for(int j=0;j<1000000;j++);  
-             System.out.println("i="+i);  
-         }  
-     }  
- }  
- 
- class MyRunnableB implements Runnable{  
- publicvoid run(){  
- char c='A'-1;  
- while(c<'Z'){  
-             c++;  
- for(int j=0;j<1000000;j++);  
-             System.out.println("c="+c);  
-         }  
-     }  
- }  
Java代码   ![收藏代码](http://junlas.iteye.com/images/icon_star.png)
- package thread.test04;  
- 
- import java.util.concurrent.Callable;  
- import java.util.concurrent.ExecutionException;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- import java.util.concurrent.Future;  
- 
- publicclass ThreadTestB {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService e=Executors.newFixedThreadPool(10);  
-         Future f1=e.submit(new MyCallableA());  
-         Future f2=e.submit(new MyCallableA());  
-         Future f3=e.submit(new MyCallableA());  
-         System.out.println("--Future.get()....");  
- try {  
-             System.out.println(f1.get());  
-             System.out.println(f2.get());  
-             System.out.println(f3.get());  
-         } catch (InterruptedException e1) {  
-             e1.printStackTrace();  
-         } catch (ExecutionException e1) {  
-             e1.printStackTrace();  
-         }  
- 
-         e.shutdown();  
- 
-     }  
- 
- }  
- 
- class MyCallableA implements Callable<String>{  
- public String call() throws Exception {  
-         System.out.println("开始执行Callable");  
-         String[] ss={"zhangsan","lisi"};  
- long[] num=newlong[2];  
- for(int i=0;i<1000000;i++){  
-             num[(int)(Math.random()*2)]++;  
-         }  
- 
- if(num[0]>num[1]){  
- return ss[0];  
-         }elseif(num[0]<num[1]){  
- thrownew Exception("弃权!");  
-         }else{  
- return ss[1];  
-         }  
-     }  
- 
- }  
 来源：[http://junlas.iteye.com/blog/846457](http://junlas.iteye.com/blog/846457)
