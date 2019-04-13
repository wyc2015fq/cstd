
# java入门教程-2.3Java运算符 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 09:25:35[seven-soft](https://me.csdn.net/softn)阅读数：203个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



Java中的运算符和C/C++相差无几。
#### 数学运算符
数学运算，结果为一个数值。见下表：
|运算符|说明|举例|
|+|加法|1 + 2|
|-|减法|4 - 3.4|
|*|乘法|7 * 1.5|
|/|除法|3.5 / 7|
|%|取余|7 % 2|
|++|自增|3++|
|--|自减|3--|
#### 关系运算符
关系运算符，结果为一个布尔值。见下表：
|运算符|说明|举例|
|>|大于|a > 4.2|
|>=|大于等于|3.4 >= b|
|<|小于|1.5 < 9|
|<=|小于等于|6 <= 1|
|==|等于|2 == 2|
|!=|不等于|2 != 2|
|&&|与|true && false|
||||或|(3 > 1) || (2 == 1)|
|!|非|!true|
#### 位运算符
位运算符对整数的二进制形式逐位进行逻辑运算，得到一个整数。见下表：
|运算符|说明|举例|
|&|与|1 & 4|
|||或|2 | 5|
|^|异或|2 ^ 3|
|~|非|~5|
|<<|左移|5 << 3|
|>>|右移|6 >> 1|
#### 条件运算符
Java中也有一个条件运算符（三目运算符）：
condition ? x1 : x2
condition为一个boolean值。根据condition，取x1或x2的值。
下面是关于运算符的一个综合举例：publicclassDemo{
publicstaticvoidmain(String[]args){
inta=10;
intb=10;
intx=10;
inty=21;
intz=10;
System.out.println("后自加 a="+(a++));
System.out.println("a的值 a="+a);
System.out.println("前自加 b="+(++b));
System.out.println("---------------------");
System.out.println("说 x>y,对吗？"+(x>y));
System.out.println("认为 x>y 并且 x<y,对吗？"+((x>y)&&(x<y)));
System.out.println("认为 x>=y 或者 x==y,对吗？"+((x>=y)||(x==y)));
System.out.println("认为 x<y 或者 x=z,对吗？"+((x<y)||(x==z)));
System.out.println("---------------------");
System.out.println("a&x 的结果是："+(a&x));
System.out.println("a|x 的结果是："+(a|x));
System.out.println("y^z 的结果是："+(y^z));
System.out.println("---------------------");
System.out.println("a 左移2位的结果是："+(a<<2));
System.out.println("y 右移3位的结果是："+(y>>3));
}
}

运行结果：
后自加 a=10
a的值 a=11
前自加 b=11
---------------------
说 x>y,对吗？false
认为 x>y 并且 x<y,对吗？false
认为 x>=y 或者 x==y,对吗？false
认为 x<y 或者 x=z,对吗？true
---------------------
a&x 的结果是：10
a|x 的结果是：11
y^z 的结果是：31
---------------------
a 左移2位的结果是：44
y 右移3位的结果是：2

