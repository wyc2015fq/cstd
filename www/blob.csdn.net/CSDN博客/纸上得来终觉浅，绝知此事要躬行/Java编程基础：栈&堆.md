# Java编程基础：栈&堆 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年03月01日 14:01:52[boonya](https://me.csdn.net/boonya)阅读数：691标签：[内存																[对象																[堆																[栈																[面向对象](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)





##### 原文地址：[http://blog.csdn.net/mazhimazh/article/details/16879055](http://blog.csdn.net/mazhimazh/article/details/16879055)

##### 1）栈是运行时的单位，而堆是存储的单位。




    栈解决程序的运行问题，即程序如何执行，或者说如何处理数据；堆解决的是数据存储的问题，即数据怎么放、放在哪儿。


    在Java中一个线程就会相应有一个线程栈与之对应，这点很容易理解，因为不同的线程执行逻辑有所不同，因此需要一个独立的线程栈。而堆则是所有线程共享的。栈因为是运行单位，因此里面存储的信息都是跟当前线程（或程序）相关信息的。包括局部变量、程序运行状态、方法返回值等等；而堆只负责存储对象信息。



##### 2）为什么要把堆和栈区分出来呢？栈中不是也可以存储数据吗？


    第一，从软件设计的角度看，栈代表了处理逻辑，而堆代表了数据。这样分开，使得处理逻辑更为清晰。分而治之的思想。这种隔离、模块化的思想在软件设计的方方面面都有体现。


    第二，堆与栈的分离，使得堆中的内容可以被多个栈共享（也可以理解为多个线程访问同一个对象）。这种共享的收益是很多的。一方面这种共享提供了一种有效的数据交互方式(如：共享内存)，另一方面，堆中的共享常量和缓存可以被所有栈访问，节省了空间。


    第三，栈因为运行时的需要，比如保存系统运行的上下文，需要进行地址段的划分。由于栈只能向上增长，因此就会限制住栈存储内容的能力。而堆不同，堆中的对象是可以根据需要动态增长的，因此栈和堆的拆分，使得动态增长成为可能，相应栈中只需记录堆中的一个地址即可。


    第四，面向对象就是堆和栈的完美结合。其实，面向对象方式的程序与以前结构化的程序在执行上没有任何区别。但是，面向对象的引入，使得对待问题的思考方式发生了改变，而更接近于自然方式的思考。当我们把对象拆开，你会发现，对象的属性其实就是数据，存放在堆中；而对象的行为（方法），就是运行逻辑，放在栈中。我们在编写对象的时候，其实即编写了数据结构，也编写的处理数据的逻辑。不得不承认，面向对象的设计，确实很美。



##### 3）堆中存什么？栈中存什么？


    堆中存的是对象。栈中存的是基本数据类型和堆中对象的引用。一个对象的大小是不可估计的，或者说是可以动态变化的，但是在栈中，一个对象只对应了一个4btye的引用（堆栈分离的好处）。


    为什么不把基本类型放堆中呢？因为其占用的空间一般是1~8个字节——需要空间比较少，而且因为是基本类型，所以不会出现动态增长的情况——长度固定，因此栈中存储就够了，如果把他存在堆中是没有什么意义的（还会浪费空间，后面说明）。可以这么说，基本类型和对象的引用都是存放在栈中，而且都是几个字节的一个数，因此在程序运行时，他们的处理方式是统一的。但是基本类型、对象引用和对象本身就有所区别了，因为一个是栈中的数据一个是堆中的数据。最常见的一个问题就是，Java中参数传递时的问题。

##### 4）Java中的参数传递时传值呢？还是传引用？（引用类型包括：类类型，接口类型和数组）


    在运行栈中，基本类型和引用的处理是一样的，都是传值，所以，如果是传引用的方法调用，也同时可以理解为“传引用值”的传值调用，即引用的处理跟基本类型是完全一样的。但是当进入被调用方法时，被传递的这个引用的值，被程序解释（或者查找）到堆中的对象，这个时候才对应到真正的对象。如果此时进行修改，修改的是引用对应的对象，而不是引用本身，即：修改的是堆中的数据。所以这个修改是可以保持的了。



**[java]**[view
 plain](http://blog.csdn.net/mazhimazh/article/details/16879055#)[copy](http://blog.csdn.net/mazhimazh/article/details/16879055#)[print](http://blog.csdn.net/mazhimazh/article/details/16879055#)[?](http://blog.csdn.net/mazhimazh/article/details/16879055#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- publicclass test2 {  
- publicvoid change(StringBuffer a, int b,String c) {  
-         a.append("xxxx");  
-         b = 2;  
-         c+="nnn";  
-                 System.out.println(c);//mmmmnnn
-     }  
- publicstaticvoid main(String[] args) {  
-         StringBuffer a=new StringBuffer("yyyy");  
-         String c="mmmm";  
- int b=3;  
- new test2().change(a, b,c);  
-         System.out.println(a);// yyyyxxxx
-         System.out.println(b);// 3
-         System.out.println(c);// mmmm
-     }  
- 
- }  


 Java中，栈的大小通过-Xss来设置，当栈中存储数据比较多时，需要适当调大这个值，否则会出现java.lang.StackOverflowError异常。常见的出现这个异常的是无法返回的递归，因为此时栈中保存的信息都是方法返回的记录点。](https://so.csdn.net/so/search/s.do?q=栈&t=blog)](https://so.csdn.net/so/search/s.do?q=堆&t=blog)](https://so.csdn.net/so/search/s.do?q=对象&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)




