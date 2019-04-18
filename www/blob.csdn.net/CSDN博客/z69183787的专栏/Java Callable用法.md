# Java Callable用法 - z69183787的专栏 - CSDN博客
2014年06月05日 16:22:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3609
Callable 和 Runnable 的使用方法大同小异， 区别在于： 
1.Callable 使用 call（） 方法， Runnable 使用 run() 方法 
2.call() 可以返回值， 而 run()方法不能返回。 
3.call() 可以抛出受检查的异常，比如ClassNotFoundException， 而run()不能抛出受检查的异常。 
Callable示例如下： 
Java代码  ![收藏代码](http://auguslee.iteye.com/images/icon_star.png)
- class TaskWithResult implements Callable<String> {  
- privateint id;  
- 
- public TaskWithResult(int id) {  
- this.id = id;  
-     }  
- 
- @Override
- public String call() throws Exception {  
- return"result of TaskWithResult " + id;  
-     }  
- }  
- 
- publicclass CallableTest {  
- publicstaticvoid main(String[] args) throws InterruptedException,  
-             ExecutionException {  
-         ExecutorService exec = Executors.newCachedThreadPool();  
-         ArrayList<Future<String>> results = new ArrayList<Future<String>>();    //Future 相当于是用来存放Executor执行的结果的一种容器
- for (int i = 0; i < 10; i++) {  
-             results.add(exec.submit(new TaskWithResult(i)));  
-         }  
- for (Future<String> fs : results) {  
- if (fs.isDone()) {  
-                 System.out.println(fs.get());  
-             } else {  
-                 System.out.println("Future result is not yet complete");  
-             }  
-         }  
-         exec.shutdown();  
-     }  
- }  
执行结果 
Java代码  ![收藏代码](http://auguslee.iteye.com/images/icon_star.png)
- result of TaskWithResult 0
- result of TaskWithResult 1
- result of TaskWithResult 2
- result of TaskWithResult 3
- result of TaskWithResult 4
- result of TaskWithResult 5
- result of TaskWithResult 6
- result of TaskWithResult 7
- result of TaskWithResult 8
- result of TaskWithResult 9
Runnable示例： 
Java代码  ![收藏代码](http://auguslee.iteye.com/images/icon_star.png)
- publicclass LiftOff implements Runnable {  
- 
- protectedint countDown = 10;  
- privatestaticint taskCount = 0;  
- privatefinalint id = taskCount++;  
- 
- public LiftOff() {  
- 
-     }  
- 
- public LiftOff(int countDown) {  
- this.countDown = countDown;  
-     }  
- 
- public String status() {  
- return"#" + id + "(" + (countDown > 0 ? countDown : "LiftOff! ") + ")";  
-     }  
- 
- @Override
- publicvoid run() {  
- while (countDown-- > 0) {  
-             System.out.print(status());  
-             Thread.yield();  
-         }  
-         System.out.println();  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         ExecutorService exec = Executors.newFixedThreadPool(1);  
- for (int i = 0; i < 5; i++) {  
-             exec.execute(new LiftOff());  
-         }  
-         exec.shutdown();  
-     }  
- }  
执行结果 
Java代码  ![收藏代码](http://auguslee.iteye.com/images/icon_star.png)
- #0(9)#0(8)#0(7)#0(6)#0(5)#0(4)#0(3)#0(2)#0(1)#0(LiftOff! )  
- #1(9)#1(8)#1(7)#1(6)#1(5)#1(4)#1(3)#1(2)#1(1)#1(LiftOff! )  
- #2(9)#2(8)#2(7)#2(6)#2(5)#2(4)#2(3)#2(2)#2(1)#2(LiftOff! )  
- #3(9)#3(8)#3(7)#3(6)#3(5)#3(4)#3(3)#3(2)#3(1)#3(LiftOff! )  
- #4(9)#4(8)#4(7)#4(6)#4(5)#4(4)#4(3)#4(2)#4(1)#4(LiftOff! )  
注意ExecutorService 在Callable中使用的是submit()， 在Runnable中使用的是 execute()
