
# java入门教程-2.2 Java数据类型转换（自动转换和强制转换） -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 09:24:04[seven-soft](https://me.csdn.net/softn)阅读数：211


数据类型的转换，分为自动转换和强制转换。自动转换是程序在执行过程中“悄然”进行的转换，不需要用户提前声明，一般是从位数低的类型向位数高的类型转换；强制类型转换则必须在代码中声明，转换顺序不受限制。
## 自动数据类型转换
自动转换按从低到高的顺序转换。不同类型数据间的优先关系如下：
低--------------------------------------------->高
byte,short,char-> int -> long -> float -> double
运算中，不同类型的数据先转化为同一类型，然后进行运算，转换规则如下：|操作数1类型|操作数2类型|转换后的类型|
|byte、short、char|int|int|
|byte、short、char、int|long|long|
|byte、short、char、int、long|float|float|
|byte、short、char、int、long、float|double|double|
## 强制数据类型转换
强制转换的格式是在需要转型的数据前加上“( )”，然后在括号内加入需要转化的数据类型。有的数据经过转型运算后，精度会丢失，而有的会更加精确，下面的例子可以说明这个问题。publicclassDemo{
publicstaticvoidmain(String[]args){
intx;
doubley;
x=(int)34.56+(int)11.2;// 丢失精度
y=(double)x+(double)10+1;// 提高精度
System.out.println("x="+x);
System.out.println("y="+y);
}
}

运行结果：
x=45
y=56.0
仔细分析上面程序段：由于在 34.56 前有一个 int 的强制类型转化，所以 34.56 就变成了 34。同样 11.2 就变成了 11 了，所以 x 的结果就是 45。在 x 前有一个 double 类型的强制转换，所以 x
 的值变为 45.0，而 10 的前面也被强制成 double 类型，所以也变成 10.0，所以最后 y 的值变为 56。

