# Java中的线程创建基础 - 小灰笔记 - CSDN博客





2017年03月23日 23:36:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：301








       为了能够让程序更加实时高效，线程的使用时一个很好的选择，而多线程是Java的一个很重要的特征。

       关于多线程技术我一直感兴趣但是从来没有研究过，但是每次看到类似的文章都会看一下。比如说，一直比较火的Python和Java有人做过对比。单线程下，Java的运行速度很有优势，但是多线程下两个速度差不多。虽说那时候还没有接触线程的概念，但是大致的结论还是习惯性记了一下。今天先总结一下多线程的基础——创建单个线程。

       写代码如下：

public classThread

{

       public static void main(String args[])

       {

              run();

       }

       public static void run()

       {

              for(int i = 0;i < 10;i++)

              {

                     System.out.println("run" + (i + 1) + " times");

              }

       }

}

       代码的编译执行结果：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>javac Thread.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>java Thread

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

       这是最初学习Java的代码，但是一直没有意识到的是其实上述的代码实现了一个单线程操作。只不过，线程的入口是main方法，执行完所有的代码后自然结束。把上述代码修改一下：

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

              ThreadDemo demo = newThreadDemo();

              demo.start();

       }

}





       代码编译运行结果如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>javac ThreadDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\thread_demo>java ThreadDemo

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

       从上面的结果看出，程序也得到了正确的执行。不过，不同于之前直接调用run方法，这里的方法是调用了一个线程对象的start方法。而这中线程的构建方式是Java中的线程创建基础。



