# Java线程(七)：Callable和Future - z69183787的专栏 - CSDN博客
2014年06月05日 16:54:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：739
个人分类：[多线程-专栏](https://blog.csdn.net/z69183787/article/category/6816123)

      20161024  接着上一篇继续并发包的学习，本篇说明的是Callable和Future，它俩很有意思的，一个产生结果，一个拿到结果。
        Callable接口类似于Runnable，从名字就可以看出来了，但是Runnable不会返回结果，并且无法抛出返回结果的异常，而Callable功能更强大一些，被线程执行后，可以返回值，这个返回值可以被Future拿到，也就是说，Future可以拿到异步执行任务的返回值，下面来看一个简单的例子：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7451464#)[copy](http://blog.csdn.net/ghsau/article/details/7451464#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74847/fork)
- publicclass CallableAndFuture {  
- publicstaticvoid main(String[] args) {  
-         Callable<Integer> callable = new Callable<Integer>() {  
- public Integer call() throws Exception {  
- returnnew Random().nextInt(100);  
-             }  
-         };  
-         FutureTask<Integer> future = new FutureTask<Integer>(callable);  
- new Thread(future).start();  
- try {  
-             Thread.sleep(5000);// 可能做一些事情
-             System.out.println(future.get());  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         } catch (ExecutionException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
        FutureTask实现了两个接口，Runnable和Future，所以它既可以作为Runnable被线程执行，又可以作为Future得到Callable的返回值，那么这个组合的使用有什么好处呢？假设有一个很耗时的返回值需要计算，并且这个返回值不是立刻需要的话，那么就可以使用这个组合，用另一个线程去计算返回值，而当前线程在使用这个返回值之前可以做其它的操作，等到需要这个返回值时，再通过Future得到，岂不美哉！这里有一个Future模式的介绍：[](http://caterpillar.onlyfun.net/Gossip/DesignPattern/FuturePattern.htm)[http://openhome.cc/Gossip/DesignPattern/FuturePattern.htm](http://openhome.cc/Gossip/DesignPattern/FuturePattern.htm)。
        下面来看另一种方式使用Callable和Future，通过ExecutorService的submit方法执行Callable，并返回Future，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7451464#)[copy](http://blog.csdn.net/ghsau/article/details/7451464#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74847/fork)
- publicclass CallableAndFuture {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService threadPool = Executors.newSingleThreadExecutor();  
-         Future<Integer> future = threadPool.submit(new Callable<Integer>() {  
- public Integer call() throws Exception {  
- returnnew Random().nextInt(100);  
-             }  
-         });  
- try {  
-             Thread.sleep(5000);// 可能做一些事情
-             System.out.println(future.get());  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         } catch (ExecutionException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
        代码是不是简化了很多，ExecutorService继承自Executor，它的目的是为我们管理Thread对象，从而简化并发编程，Executor使我们无需显示的去管理线程的生命周期，是JDK 5之后启动任务的首选方式。
如果要执行多个带返回值的任务，并取得多个返回值，可用CompletionService：
CompletionService相当于Executor加上BlockingQueue，使用场景为当子线程并发了一系列的任务以后，主线程需要实时地取回子线
程任务的返回值并同时顺序地处理这些返回值，谁先返回就先处理谁。
        执行多个带返回值的任务，并取得多个返回值，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7451464#)[copy](http://blog.csdn.net/ghsau/article/details/7451464#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74847/fork)
- publicclass CallableAndFuture {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService threadPool = Executors.newCachedThreadPool();  
-         CompletionService<Integer> cs = new ExecutorCompletionService<Integer>(threadPool);  
- for(int i = 1; i < 5; i++) {  
- finalint taskID = i;  
-             cs.submit(new Callable<Integer>() {  
- public Integer call() throws Exception {  
- return taskID;  
-                 }  
-             });  
-         }  
- // 可能做一些事情
- for(int i = 1; i < 5; i++) {  
- try {  
-                 System.out.println(cs.take().get());  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             } catch (ExecutionException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }        
        其实也可以不使用CompletionService，可以先创建一个装Future类型的集合，用Executor提交的任务返回值添加到集合中，最后遍历集合取出数据，代码略。        
区别：
Future集合方法，submit的task不一定是按照加入自己维护的list顺序完成的。从list中遍历的每个Future对象并不一定处于完成状态，
这时调用get()方法就会被阻塞住，如果系统是设计成每个线程完成后就能根据其结果继续做后面的事，这样对于处于list后面的但是
先完成的线程就会增加了额外的等待时间。
而CompletionService的实现是维护一个保存Future对象的BlockingQueue。只有当这个Future对象状态是结束的时候，才会加入到这
个Queue中，take()方法其实就是Producer-Consumer中的Consumer。它会从Queue中取出Future对象，如果Queue是空的，就会阻
塞在那里，直到有完成的Future对象加入到Queue中。
所以，先完成的必定先被取出。这样就减少了不必要的等待时间。
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[](http://blog.csdn.net/ghsau/article/details/7436458)[http://blog.csdn.net/ghsau/article/details/7451464](http://blog.csdn.net/ghsau/article/details/7451464)，转载请注明。
