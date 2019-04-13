
# java入门教程-3.3Java变量的作用域 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 11:19:20[seven-soft](https://me.csdn.net/softn)阅读数：155个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



在Java中，变量的作用域分为四个级别：类级、对象实例级、方法级、块级。
类级变量又称全局级变量或静态变量，需要使用static关键字修饰，你可以与 C/C++ 中的 static 变量对比学习。类级变量在类定义后就已经存在，占用内存空间，可以通过类名来访问，不需要实例化。
对象实例级变量就是成员变量，实例化后才会分配内存空间，才能访问。
方法级变量就是在方法内部定义的变量，就是局部变量。
块级变量就是定义在一个块内部的变量，变量的生存周期就是这个块，出了这个块就消失了，比如 if、for 语句的块。块是指由大括号包围的代码，例如：{
intage=3;
Stringname="www.weixueyuan.net";
// 正确，在块内部可以访问 age 和 name 变量
System.out.println(name+"已经"+age+"岁了");
}
// 错误，在块外部无法访问 age 和 name 变量
System.out.println(name+"已经"+age+"岁了");

说明：方法内部除了能访问方法级的变量，还可以访问类级和实例级的变量。
块内部能够访问类级、实例级变量，如果块被包含在方法内部，它还可以访问方法级的变量。
方法级和块级的变量必须被显示地初始化，否则不能访问。
演示代码：publicclassDemo{
publicstaticStringname="微学苑";// 类级变量
publicinti;// 对象实例级变量
// 属性块，在类初始化属性时候运行
{
intj=2;// 块级变量
}
publicvoidtest1(){
intj=3;// 方法级变量
if(j==3){
intk=5;// 块级变量
}
// 这里不能访问块级变量，块级变量只能在块内部访问
System.out.println("name="+name+", i="+i+", j="+j);
}
publicstaticvoidmain(String[]args){
// 不创建对象，直接通过类名访问类级变量
System.out.println(Demo.name);
// 创建对象并访问它的方法
Demot=newDemo();
t.test1();
}
}

运行结果：
微学苑
name=微学苑, i=0, j=3

