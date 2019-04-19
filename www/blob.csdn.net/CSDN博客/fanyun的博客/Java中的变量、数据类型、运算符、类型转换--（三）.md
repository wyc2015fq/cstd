# Java中的变量、数据类型、运算符、类型转换--（三） - fanyun的博客 - CSDN博客
2016年03月24日 17:46:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：741
个人分类：[Java](https://blog.csdn.net/fanyun_01/article/category/6140884)
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)
**       Java语言是一种强类型的语言，对各种数据类型都有明确的区分，而计算机使用内存来记忆大量运算时需要使用的数据，而当声明一个变量时，即在内存中划分一块空间存储数据，而变量类型决定划分内存空间的大小。**
**      Java中分为基本数据类型及引用数据类型：**
![](https://img-blog.csdn.net/20160325092522299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**** Java**基本数据类型：**
**![](https://img-blog.csdn.net/20160325092546302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
byte：java中最小的数据类型，在内存中占8位(bit)，即1个字节，取值范围-128~127
short：短整型，在内存中占2个字节，取值范围-32768~32717
int：整型，用于存储整数，在内在中占4个字节，取值范围-2147483648~2147483647
long：长整型，在内存中占8个字节
float：浮点型，在内存中占4个字节，用于存储带小数点的数字（与double的区别在于float类型有效小数点只有6~7位）
double：双精度浮点型，用于存储带有小数点的数字，在内存中占8个字节
char：字符型，用于存储单个字符，占2个字节
boolean：布尔类型，占8个字节，用于判断真或假（仅有两个值，即true、false）
引用数据类型：类，数据接口，数组
String：字符串型，用于存储一串字符
Java变量声明及使用：
数据类型变量名 = 值、表达式；
例：String name = "柯南";
    int a = 50;
注：“=”并不是数学中的“等号”，而是一个赋值运算符
**Java变量命名规则：**
1：必须以字母、下划线“_”、或“$”符号开头
2：可以包括数字、区分大小写
3：不能使用Java语言的关键字，例如int、class、public等
**Java中的六种运算符：**
算术运算符
赋值运算符
关系运算符
逻辑运算符
位运算符
三元运算符
算术运算符：
+：加法运算，求操作数的和
-：减法运算，求操作数的差
*：乘法运算，求操作数的乘积
/：除法运算，求操作数的商
%：求余运算，求操作数相除的余数
++：自增，操作数自加1
--：自减，操作数自减1
[](http://photo.blog.sina.com.cn/showpic.html#blogid=61e4a9dd0101emv7&url=http://album.sina.com.cn/pic/001N9eVLty6T9L4vDbi80)
赋值运算符：
=:将右边的值赋给左边，例：int a =1;
+=:左右两边的和相加赋给左边，例：int a= 1; a+=2;结果a的值为3
-=：左边减去右边的差赋给左边，例：int a=5;a-=2;结果a的值为3
*=：两边数值相乘的值赋给左边，例：int a= 2;a*=2;结果a的值为4
/=：左边除以右边的值赋给左边，例：int a= 6;a/=2;结果a的值为3
%=：左边除以右边的余数赋给左边，例：inta =7;a%=2;结果a的值为1
[](http://photo.blog.sina.com.cn/showpic.html#blogid=61e4a9dd0101emv7&url=http://album.sina.com.cn/pic/001N9eVLty6T9OLWOXjc5)
关系运算符：
>：大于，例：int a = 1;int b= 2;System.out.print(a > b);其结果为false
<：小于,例：int a =1;int b = 2;System.out.print(a < b);其结果为true
>=：大于等于,例：int a =1;int b = 2;System.out.print(a >= b);其结果为false
<=：小于等于,例：int a =1;int b = 2;System.out.print(a <= b);其结果为true
==：等于,例：int a =1;int b = 2;System.out.print(a == b);其结果为false
!=：不等于,例：int a =1;int b = 2;System.out.print(a != b);其结果为true
  其结果都是boolean类型，即要么是true要么是false
逻辑运算符:
&&：与、并且(短路)，   两个条件同时为真时，结果为真
||：或、或者(短路)，   两个条件有一个为真时，结果即为真
!：非，(!+条件)  条件为真时，结果为假
Java中的数据类型转换：
1：自动数据类型转换（放大转换）
满足自动数据类型转换条件：
1）两种类型要兼容：如数值类型（整型和浮点型）
2）目标类型大于源类型：例如int型数据可以自动转换为double类型
2：强制数据类型转换（缩小转换）
在变量前加上括号，在括号中指定要强制转换的类型
例：double a = 20.9;
    int b = (int)a;
注：强制转换会损失数值精度，例如double类型变量a，经强制转换为int类型后值变为20
