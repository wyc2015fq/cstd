
# java final关键字到底修饰了什么 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:33:23[seven-soft](https://me.csdn.net/softn)阅读数：207


final使得被修饰的变量"不变"，但是由于对象型变量的本质是“引用”，使得“不变”也有了两种含义：引用本身的不变，和引用指向的对象不变。
引用本身的不变：
final StringBuffer a=new StringBuffer("immutable");
final StringBuffer b=new StringBuffer("not immutable");
a=b;//编译期错误
引用指向的对象不变：
final StringBuffer a=new StringBuffer("immutable");
a.append(" broken!"); //编译通过
可见，final只对引用的“值”(也即它所指向的那个对象的内存地址)有效，它迫使引用只能指向初始指向的那个对象，改变它的指向会导致编译期错误。至于它所指向的对象的变化，final是不负责的。这很类似==操作符：==操作符只负责引用的“值”相等，至于这个地址所指向的对象内容是否相等，==操作符是不管的。
理解final问题有很重要的含义。许多程序漏洞都基于此----final只能保证引用永远指向固定对象，不能保证那个对象的状态不变。在多线程的操作中,一个对象会被多个线程共享或修改，一个线程对对象无意识的修改可能会导致另一个使用此对象的线程崩溃。一个错误的解决方法就是在此对象新建的时候把它声明为final，意图使得它“永远不变”。其实那是徒劳的。


