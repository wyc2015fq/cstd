# 【java】sting和stringbuilder与stringbuffer的区别辨析 - csdn_baotai的博客 - CSDN博客

2018年08月01日 20:40:47[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：99
个人分类：[Java](https://blog.csdn.net/csdn_baotai/article/category/7659813)



1.在执行速度上：Stringbuilder->Stringbuffer->String
- 
String是字符串常量 

Stringbuffer是字符串变量  

Stringbuilder是字符串变量

有可能我们会疑惑String怎么是字符串变量。看以下代码：

String str = adc；

str = str + “ef”；

System.out.println(str);

输出结果为:abcef;

在Java虚拟机中str为adc时是一个变量，当str被重新赋值为adcef时为另一个变量，被重新分配了一次内存，上次使用的内存会被gc在适当的时候回收掉。由于这种机制导致了如果有大量的String赋值操作时，会导致运行速度的缓慢，以及内存使用量的上升。

3、一个特殊的例子

```
String str = “this is only a” + “simple” + “test”；

StringBuffer stringBuffer  = new StringBuffer("this is only a").append("simple").append("test");
```

你会很惊讶的发现，生成str对象速度简直太快了，而这个时候Stringbuffer居然速度上根本一点都不占优势。其实这是JVM的一个把戏，实际上：

String str  =  ”this is only a“ + “simple” + “test”;

其实就是

String str = “this is only a simple test”；

所以不需要太多的是时间。但大家这里需要注意的是，如果你的字符串是来自另外的String对象的话。速度就没那么快了，譬如：

String str1 = “this is only a”；

String str2 = “simple”;

String str3 = “test”;

String str = str1 + str2 + str3;

这时候JVM会规规矩矩的按照原来的方式去做。

4、StringBuilder与StringBuffer

```
StringBuilder：线程非安全的
  StringBuffer：线程安全的
```

当我们在字符串缓冲区去被多个线程使用时，JVM不能保证StringBuilder的线程是安全的，虽然他的速度最快，但是可以保证StringBuffer是可以正确操作的。当然大多数情况是在单线程下进行的，所以大多数情况下是建议用StringBuilder而不用StringBuffer的，就是速度的原因。

5、对于三者的总结： 

        1）如果操作少量的数据用String 

        2）单线程下操作大量的数据用StringBuilder 

        3）多线程下操作大量的数据用StringBuffer**

