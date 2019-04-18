# RunTime.getRunTime().addShutdownHook用法 - z69183787的专栏 - CSDN博客
2016年11月03日 11:02:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：451
个人分类：[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)
今天在阅读Tomcat源码的时候，catalina这个类中使用了下边的代码，不是很了解，所以google了一下，然后测试下方法，Tomcat中的相关代码如下：
Runtime.getRuntime().addShutdownHook(shutdownHook);
   这个方法的含义说明：
 这个方法的意思就是在jvm中增加一个关闭的钩子，当jvm关闭的时候，会执行系统中已经设置的所有通过方法addShutdownHook添加的钩子，当系统执行完这些钩子后，jvm才会关闭。所以这些钩子可以在jvm关闭的时候进行内存清理、对象销毁等操作。
一、编写个测试类
  package com.test.hook;
public class TestShutdownHook {
 /**
  * @param args
  */
 public static void main(String[] args) {
  // 定义线程1
  Thread thread1 = new Thread() {
   public void run() {
    System.out.println("thread1...");
   }
  };
  // 定义线程2
  Thread thread2 = new Thread() {
   public void run() {
    System.out.println("thread2...");
   }
  };
  // 定义关闭线程
  Thread shutdownThread = new Thread() {
   public void run() {
    System.out.println("shutdownThread...");
   }
  };
  // jvm关闭的时候先执行该线程钩子
  Runtime.getRuntime().addShutdownHook(shutdownThread);
  thread1.start();
  thread2.start();
 }
}
打印结果：
thread2...
thread1...
shutdownThread...
或者：
thread2...
thread1...
shutdownThread...
结论：
无论是先打印thread1还是thread2，shutdownThread 线程都是最后执行的（因为这个线程是在jvm执行关闭前才会执行）。
