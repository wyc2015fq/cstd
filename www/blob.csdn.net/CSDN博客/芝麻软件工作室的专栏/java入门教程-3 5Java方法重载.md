
# java入门教程-3.5Java方法重载 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:44:13[seven-soft](https://me.csdn.net/softn)阅读数：142个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



在Java中，同一个类中的多个方法可以有相同的名字，只要它们的参数列表不同就可以，这被称为方法重载(method overloading)。
参数列表又叫参数签名，包括参数的类型、参数的个数和参数的顺序，只要有一个不同就叫做参数列表不同。
重载是面向对象的一个基本特性。
下面看一个详细的实例。publicclassDemo{
// 一个普通的方法，不带参数
voidtest(){
System.out.println("No parameters");
}
// 重载上面的方法，并且带了一个整型参数
voidtest(inta){
System.out.println("a: "+a);
}
// 重载上面的方法，并且带了两个参数
voidtest(inta,intb){
System.out.println("a and b: "+a+" "+b);
}
// 重载上面的方法，并且带了一个双精度参数
doubletest(doublea){
System.out.println("double a: "+a);
returna*a;
}
publicstaticvoidmain(Stringargs[]){
Demoobj=newDemo();
obj.test();
obj.test(2);
obj.test(2,3);
obj.test(2.0);
}
}

运行结果：
No parameters
a: 2
a and b: 2 3
double a: 2.0
通过上面的实例，读者可以看出，重载就是在一个类中，有相同的函数名称，但形参不同的函数。重载的结果，可以让一个程序段尽量减少代码和方法的种类。
说明：参数列表不同包括：个数不同、类型不同和顺序不同。
仅仅参数变量名称不同是不可以的。
跟成员方法一样，构造方法也可以重载。
声明为final的方法不能被重载。
声明为static的方法不能被重载，但是能够被再次声明。
方法的重载的规则：方法名称必须相同。
参数列表必须不同（个数不同、或类型不同、参数排列顺序不同等）。
方法的返回类型可以相同也可以不相同。
仅仅返回类型不同不足以成为方法的重载。
方法重载的实现：
方法名称相同时，编译器会根据调用方法的参数个数、参数类型等去逐个匹配，以选择对应的方法，如果匹配失败，则编译器报错，这叫做重载分辨。

