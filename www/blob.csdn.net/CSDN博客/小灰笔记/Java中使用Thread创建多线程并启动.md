# Java中使用Thread创建多线程并启动 - 小灰笔记 - CSDN博客





2017年03月23日 23:49:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：158
个人分类：[Java](https://blog.csdn.net/grey_csdn/article/category/6673076)









       按照教程实现了一个单线程的创建，但是单线程的创建于启动并不是很有实用价值的。毕竟直接在main方法中放着相关的执行操作本身也就是一种单线程的实现。接下来在之前用过的代码基础上稍作修改，形成如下代码：

class ThreadDemoextends Thread

{

       ThreadDemo(){};



       ThreadDemo(String szName)

       {

              super(szName);

       }



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

              ThreadDemo demo1 = newThreadDemo();

              ThreadDemo demo2 = newThreadDemo();

              ThreadDemo demo3 = newThreadDemo();



              demo1.start();

              demo2.start();

              demo3.start();

       }

}

       代码编译运行结果如下;

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>javac ThreadDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>java ThreadDemo

run 1 times

run 2 times

run 3 times

run 4 times

run 5 times

run 6 times

run 7 times

run 1 times

run 2 times

run 1 times

run 3 times

run 8 times

run 4 times

run 2 times

run 5 times

run 9 times

run 6 times

run 7 times

run 8 times

run 3 times

run 9 times

run 10 times

run 10 times

run 4 times

run 5 times

run 6 times

run 7 times

run 8 times

run 9 times

run 10 times

从上面的结果中实际上看到了一定的乱序，看起来三个任务的执行顺序不是按顺序来的。其实，这正是创建出来的三个线程出现了执行上的竞争而造成的结果。

       之前我写程序的时候确实是感觉傻了一点，虽说这么久以来也好歹完成了任务，但是我之前所实现的都是单线程的工作。一个任务从头到尾一路执行下来，好在现在的电脑的执行速度都还不坏，否则我这中间可得出现多少执行时间上的浪费啊！

       虽说现在是在学Java，后期的Python等支持多线程的语言中尽量也该把这个功能利用起来。尽可能的挖掘一下CPU的潜力还是一件很有意思的事情的。



