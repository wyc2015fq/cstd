# Java中使用Runnable接口创建并启动多线程 - 小灰笔记 - CSDN博客





2017年03月24日 00:07:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1703








       使用Runnable接口创建多线程的方法暂时还没有体验到有什么优点，感觉整体上比使用Thread直接创建稍微麻烦一点。而且，创建的过程中本身也用到了Thread构造方法。写一段测试代码：

class RunnableDemoimplements Runnable

{

       public void run()

       {

              int i = 0;

              for(i = 0;i < 10;i++)

              {

                     System.out.println("run" + (i + 1) + " times");

              }

       }



       public static void main(String[] args)

       {

              Runnable runnable_demo1 = newRunnableDemo();

              Runnable runnable_demo2 = newRunnableDemo();

              Runnable runnable_demo3 = newRunnableDemo();



              Thread thread_demo1 = newThread(runnable_demo1);

              Thread thread_demo2 = newThread(runnable_demo2);

              Thread thread_demo3 = new Thread(runnable_demo3);



              thread_demo1.start();

              thread_demo2.start();

              thread_demo3.start();

       }

}

       编译运行的结果如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>javac RunnableDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>java RunnableDemo

run 1 times

run 2 times

run 3 times

run 4 times

run 5 times

run 6 times

run 7 times

run 8 times

run 9 times

run 10 times

run 1 times

run 2 times

run 1 times

run 3 times

run 2 times

run 4 times

run 3 times

run 5 times

run 6 times

run 7 times

run 8 times

run 9 times

run 4 times

run 10 times

run 5 times

run 6 times

run 7 times

run 8 times

run 9 times

run 10 times

       从乱序的特点能够看出多线程的功能已经启动。

       大致考虑两种创建多线程的方法，找找异同点：

       1，都需要创建一个类，只是一个集成类而另一个实现接口；

       2，根据创建的类创建实例化对象；

       3，第2不完成之后，集成Thread类的对象已经可以启动线程，而实现接口的对象则还需要通过Thread方法创建新的线程对象。



