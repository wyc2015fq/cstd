# Java类成员变量、普通成员变量、初始化块、构造方法的初始化和执行顺序 - z69183787的专栏 - CSDN博客
2014年05月09日 09:29:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9095
**序言**
　　关于类的static成员变量初始化、static初始化块、实例的普通成员变量初始化、实例的普通初始化块以及构造函数的执行顺序，我一直不是十分明确地知道，今天专门花了几个小时的时间参考网上资料设计出了比较明了的测试代码，有代码有结果有真相。总体而言，static部分执行早于普通初始化块早于构造函数，如果一个类继承了某个父类，则父类的static部分最先执行。
**正文**
　　测试代码设计思路：有三个主线类B、C和D，其中D继承C，C继承B，这三个类中均包含static块、普通初始化块和无参的构造方法；有两个辅助类E和F，B中包含E类和F类的成员变量，F类成员变量是static类型，E类的成员变量是普通类型；程序运行入口在A.java中，A中的main函数只用来创建D类的实例，其代码列表如下。
E.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class E 
 4 {
 5     E()
 6     {
 7         System.out.println("执行E的构造函数");
 8     }
 9     public void funcOfE()
10     {
11         System.out.println("执行E的函数");
12     }
13 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
F.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class F 
 4 {
 5     F()
 6     {
 7         System.out.println("执行F的构造函数");
 8     }
 9     public void funcOfF()
10     {
11         System.out.println("执行F的函数");
12     }
13 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
B.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class B 
 4 {
 5     E e=new E();
 6     static F f=new F();
 7     public String sb=getSb();
 8     static
 9     {
10         System.out.println("执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)");
11         f.funcOfF();
12     }
13     {
14         System.out.println("执行B实例的普通初始化块");
15     }
16     B()
17     {
18         System.out.println("执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)");
19         e.funcOfE();
20     }
21     
22     public String getSb()
23     {
24         System.out.println("初始化B的实例成员变量sb");
25         return "sb";
26     }
27 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
C.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class C extends B
 4 {
 5     static
 6     {
 7         System.out.println("执行C的static块(C继承B)");
 8     }
 9     {
10         System.out.println("执行C的普通初始化块");
11     }
12     C()
13     {
14         System.out.println("执行C的构造函数(C继承B)");
15     }
16 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
D.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class D extends C
 4 {
 5     public String sd1=getSd1();
 6     public static String sd=getSd();
 7     static
 8     {
 9         System.out.println("执行D的static块(D继承C)");
10         
11     }
12     {
13         System.out.println("执行D实例的普通初始化块");
14     }
15     D()
16     {
17         System.out.println("执行D的构造函数(D继承C);父类B的实例成员变量sb的值为："+sb+";本类D的static成员变量sd的值为："+sd+";本类D的实例成员变量sd1的值是："+sd1);
18     }
19     
20     static public String getSd()
21     {
22         System.out.println("初始化D的static成员变量sd");
23         return "sd";
24     }
25     public String getSd1()
26     {
27         System.out.println("初始化D的实例成员变量sd1");
28         return "sd1";
29     }
30 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
A.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class A 
 4 {
 5     public static void main(String[] args)
 6     {
 7         System.out.println("运行A中的main函数，创建D类实例");
 8         D d1=new D();
 9         //D d2=new D();
10         
11     }
12 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　执行A.java后运行结果如下图所示，
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 运行A中的main函数，创建D类实例
 2 执行F的构造函数
 3 执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
 4 执行F的函数
 5 执行C的static块(C继承B)
 6 初始化D的static成员变量sd
 7 执行D的static块(D继承C)
 8 执行E的构造函数
 9 初始化B的实例成员变量sb
10 执行B实例的普通初始化块
11 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
12 执行E的函数
13 执行C的普通初始化块
14 执行C的构造函数(C继承B)
15 初始化D的实例成员变量sd1
16 执行D实例的普通初始化块
17 执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：sb;本类D的static成员变量sd的值为：sd;本类D的实例成员变量sd1的值是：sd1
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　分析：由输出结果可知，
　　(1) 整体上先执行static部分(1至7行)后执行非static部分(8至17行)。
　　(2) 在static部分中先运行父类的后运行子类的。第2行执行F的构造函数是因为B中包含F类的static(静态)变量，在B.java的第6行调用了F的构造函数初始化static变量；第4行执行F的funcOfF是因为在B的　　　　       　 static初始化块中调用了这个函数；之后依次执行C和D的static部分。
　　(3) 在非static部分中也是先运行父类的后运行子类的，对于同一个类，其成员变量的初始化和普通初始化块的执行优先于构造函数。
　　对于同一个类，其成员变量初始化一定优先于初始化块的执行吗？对于非static的情况，我们将D.java中的第5行移动到普通初始化块的后面，如下面所示，
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package chloe.executeorder;
 2 
 3 public class D extends C
 4 {
 5     
 6     public static String sd=getSd();
 7     static
 8     {
 9         System.out.println("执行D的static块(D继承C)");
10         
11     }
12     {
13         System.out.println("执行D实例的普通初始化块");
14     }
15     public String sd1=getSd1();
16     D()
17     {
18         System.out.println("执行D的构造函数(D继承C);父类B的实例成员变量sb的值为："+sb+";本类D的static成员变量sd的值为："+sd+";本类D的实例成员变量sd1的值是："+sd1);
19     }
20     
21     static public String getSd()
22     {
23         System.out.println("初始化D的static成员变量sd");
24         return "sd";
25     }
26     public String getSd1()
27     {
28         System.out.println("初始化D的实例成员变量sd1");
29         return "sd1";
30     }
31 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
之后再运行A.java，结果如下，
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 运行A中的main函数，创建D类实例
 2 执行F的构造函数
 3 执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
 4 执行F的函数
 5 执行C的static块(C继承B)
 6 初始化D的static成员变量sd
 7 执行D的static块(D继承C)
 8 执行E的构造函数
 9 初始化B的实例成员变量sb
10 执行B实例的普通初始化块
11 执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
12 执行E的函数
13 执行C的普通初始化块
14 执行C的构造函数(C继承B)
15 执行D实例的普通初始化块
16 初始化D的实例成员变量sd1
17 执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：sb;本类D的static成员变量sd的值为：sd;本类D的实例成员变量sd1的值是：sd1
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
该结果的第15和16行的输出结果与改动之前的输出结果正好相反，可见成员变量的初始化与普通初始化块的执行顺序不是固定的，它与源代码中赋值语句及普通初始化块的放置顺序相关：成员变量的赋值语句在前则赋值先执行，普通初始化块在前则初始化块先执行。对于static成员变量和static初始化块的执行顺序也是类似的，修改B.java中的第6行的位置即可看到类似的输出结果，此处不再赘述。
**总结**
1.执行的大致顺序如下，
　　(1) 在一个不存在继承的类中：初始化static变量,执行static初始化快-->初始化普通成员变量(如果有赋值语句),执行普通初始化块-->构造方法
　　(2)在一个存在继承的类中：初始化父类static成员变量,运行父类static初始化块-->初始化子类static成员变量,运行子类static初始化块-->初始化父类实例成员变量(如果有赋值语句),执行父类普通初始化块-->父类构造方法-->初始化子类实例成员变量(如果有赋值语句)及普通初始化块-->子类构造方法。
　　注意：其中变量初始化(赋值)和初始化块的执行与相关语句在源码中的放置顺序一致，不过变量声明会最先执行，参考[http://www.189works.com/article-52232-1.html](http://www.189works.com/article-52232-1.html)。
2.static成员变量可以再定义的时候初始化也可以在static块中初始化，static块可以出现多次，当编译成.class文件时会将多个static块的内容合并；实例成员变量可以再定义时初始化也可以在普通初始化块或构造函数中初始化。
　基本数据类型的成员变量要在初始化后再使用，引用数据类型的成员变量在实例化后才能被使用。
3.类的加载时机：
  (1) 用new创建该类的实例时；
  (2) 使用java.lang.reflect进行反射调用的时候；
  (3) 之前没有加载该类，之后加载该类的子类的时候；
  (4) 当虚拟机启动时，初始化main函数所在的类。
4.JVM载入类时会执行static块，创建一个实例时会运行构造方法。static块和static成员变量都是属于类而非实例的；构造函数和普通成员变量是属于一个实例的。类的初始化(包括static块的运行和static成员变量的赋值)只运行一次，多次创建某个类的实例只会运行一次该类的static()块，但会运行多次其构造函数。将之前的A.java中第9行的注释去掉后再运行即可说明情况。
