# java初始化与构造方法执行顺序测试 - 小灰笔记 - CSDN博客





2017年02月21日 00:24:17[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1186








终于又深入一步，开始学习面向对象的编程了。这中间涉及到的很多概念以及思想我之前只是大致有那么一个概念轮廓，了解的十分肤浅。正好趁此机会好好学习了解一下。

属性以及方法是Java面向对象编程的两个很重要的概念，而两者分别又涉及多个知识点。这里只是想简单测试一下静态初始化块的一点特性，看一下静态初始化块、普通初始化块以及构造方法的执行顺序。

写测试代码如下；

  1 package study_01;

  2 

  3 publicclass Init {

  4 

  5 int var1;

  6 

  7 public Init(){

  8         System.out.println("构造方法执行");

  9     }

 10 

 11     {

 12         var1 = 1;

 13         System.out.println("普通初始化执行");

 14     }

 15 

 16 static{

 17         System.out.println("静态初始化执行");

 18     }

 19 

 20 publicstaticvoid main(String[] args) {

 21 // TODO Auto-generated method stub

 22         Init init1 =
new Init();

 23         Init init2 =
new Init();

 24     }

 25 

 26 }




程序运行的结果如下：

![](https://img-blog.csdn.net/20170221002434191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







    由上面的运行结果基本可以得出结论：程序运行时，静态初始化块首先被执行。然后，执行普通初始化块。最后，执行构造方法。静态初始化块只在类加载的时候执行一次，因此第二次创建对象的时候静态初始化块并没有被执行。




