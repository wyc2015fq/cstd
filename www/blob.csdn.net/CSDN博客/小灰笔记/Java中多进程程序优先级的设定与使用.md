# Java中多进程程序优先级的设定与使用 - 小灰笔记 - CSDN博客





2017年03月25日 23:24:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1291








       涉及到多线程，如果嵌入式处理时后我已经接触到的优先级问题，Java之中应该也是避免不了的。看了一下，Java中有针对多线程优先级的使用技术。

       为了验证，做一个对比使用，写一段使用默认优先级的代码如下：

public classPriorityDemo extends Thread

{

       PriorityDemo()

       {

       }



       PriorityDemo(String str)

       {

              super(str);

       }



       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("***");

              }

       }



       public static void main(String args[])

       {

              TdDemo1 td1 = new TdDemo1();

              TdDemo2 td2 = new TdDemo2();



              td1.start();

              td2.start();

       }

}



class TdDemo1extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 1-" + i);

              }

       }

}



class TdDemo2extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 2-" + i);

              }

       }

}

       代码的编译运行结果如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>javac PriorityDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>java PriorityDemo

task demo 1-0

task demo 1-1

task demo 1-2

task demo 1-3

task demo 1-4

task demo 1-5

task demo 1-6

task demo 1-7

task demo 2-0

task demo 2-1

task demo 2-2

task demo 2-3

task demo 2-4

task demo 2-5

task demo 1-8

task demo 2-6

task demo 1-9

task demo 2-7

task demo 1-10

task demo 2-8

task demo 2-9

task demo 1-11

task demo 2-10

task demo 1-12

task demo 2-11

task demo 2-12

task demo 1-13

task demo 2-13

task demo 1-14

task demo 2-14



       接下来修改代码如下：

public classPriorityDemo extends Thread

{

       PriorityDemo()

       {

       }



       PriorityDemo(String str)

       {

              super(str);

       }



       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("***");

              }

       }



       public static void main(String args[])

       {

              TdDemo1 td1 = new TdDemo1();

              TdDemo2 td2 = new TdDemo2();



              td1.setPriority(MIN_PRIORITY);

              td2.setPriority(MAX_PRIORITY);



              td1.start();

              td2.start();

       }

}



class TdDemo1extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 1-" + i);

              }

       }

}



class TdDemo2extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 2-" + i);

              }

       }

}

       代码的编译与执行结果如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>javac PriorityDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>java PriorityDemo

task demo 2-0

task demo 2-1

task demo 1-0

task demo 1-1

task demo 1-2

task demo 1-3

task demo 1-4

task demo 1-5

task demo 2-2

task demo 2-3

task demo 2-4

task demo 2-5

task demo 2-6

task demo 2-7

task demo 1-6

task demo 2-8

task demo 1-7

task demo 2-9

task demo 1-8

task demo 2-10

task demo 1-9

task demo 2-11

task demo 1-10

task demo 2-12

task demo 1-11

task demo 2-13

task demo 1-12

task demo 2-14

task demo 1-13

task demo 1-14

       虽说具体的情况难以看出但是从总体的执行结果可以看出一个趋势，那就是给了高优先级的td2的执行在整个过程中安排的更往前了一点。Java的优先级可以有10个优先级指定，但是在Windows系统中只能够支持3个。这是看教程的时候教程中提到的，为了看一下现在的情况是否有所改变，再次修改代码如下：

public classPriorityDemo extends Thread

{

       PriorityDemo()

       {

       }



       PriorityDemo(String str)

       {

              super(str);

       }



       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("***");

              }

       }



       public static void main(String args[])

       {

              TdDemo1 td1 = new TdDemo1();

              TdDemo2 td2 = new TdDemo2();



              td1.setPriority(10);

              td2.setPriority(1);



              td1.start();

              td2.start();

       }

}



class TdDemo1extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 1-" + i);

              }

       }

}



class TdDemo2extends PriorityDemo

{

       public void run()

       {

              for(int i = 0; i < 15;i++)

              {

                     System.out.println("taskdemo 2-" + i);

              }

       }

}

       代码的编译与执行结果如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>javac PriorityDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\PriorityDemo>java PriorityDemo

task demo 1-0

task demo 1-1

task demo 1-2

task demo 1-3

task demo 2-0

task demo 2-1

task demo 2-2

task demo 2-3

task demo 2-4

task demo 2-5

task demo 2-6

task demo 2-7

task demo 1-4

task demo 2-8

task demo 1-5

task demo 2-9

task demo 1-6

task demo 2-10

task demo 1-7

task demo 2-11

task demo 1-8

task demo 1-9

task demo 1-10

task demo 2-12

task demo 2-13

task demo 1-11

task demo 2-14

task demo 1-12

task demo 1-13

task demo 1-14

       整个过程中并没有出现编译或者执行的问题，而且优先级的主要趋势在执行的结果中也基本能够得到验证。由此看来，现在Windows平台中的Java进程的优先级应该至少也有10级的级别使用了。



