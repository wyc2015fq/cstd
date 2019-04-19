# 如何使用QString::arg() - fanyun的博客 - CSDN博客
2016年09月30日 09:02:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：556

在Qt中QString::arg的定义：
```cpp
QString QString::arg ( const QString & a, int fieldWidth = 0, const QChar & fillChar = QLatin1Char( ' ' ) ) const
```
QString也重载的+和+=运算符，
在QT的QString中，arg方法类似于C中的printf中使用的格式输出符（只是有点类似）。 
在QT5的帮助文档中，可以看出以下几点： 
使用arg(str1, str2, str3)这种方法进行替换。 
使用arg(str1).arg(str2).arg(str3)这种方法进行替换。 
使用arg(int, int, int)这种方式进行替换。 
解释以下两种方法： 
一：使用arg(str1, str2, str3) 
这种方法要求str1、str2、str3都为const QString &类型，即： 
```cpp
QString QString::arg(constQString & a1,constQString & a2,constQString & a3)const
```
同时，arg( )里的参数实现从1个到9个，也就是说最多只能有9个！即在实现中，arg的定义中参数最多的为： 
QString QString::arg(constQString & a1,constQString & a2,constQString & a3,constQString & a4,constQString & a5,constQString & a6,constQString & a7,constQString & a8,constQString & a9)const 
这也就意味着，如果有如下代码： 
```cpp
QString str = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").
            arg("1","2","3","4","5","6","7","8","9","10","11",
)
```
str将只会输出： 
"1 2 3 4 5 6 7 8 9 %10 %11" 
解决方法为在后面再加一个arg方法，放上后面两个对应的参数： 
```cpp
QString str = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").
            arg("1","2","3","4","w","6","7","8","9").arg("a","b");
```
//输出为： 
// "1 2 3 4 5 6 7 8 9 a b" 
注意：在QT5帮助文档中，给出了如下代码： 
```cpp
QString str;
str ="%1 %2";
str.arg("%1f","Hello");       // returns "%1f Hello"
str.arg("%1f").arg("Hello");   // returns "Hellof %2"
```
如果你copy这个代码去测试一下，会发现，输出结果还是："%1 %2" 。根本没有其给出的结果。（没找到源码，有谁知道怎么查看QT的源码，求帮助！说找src文件夹的就不要来了，根本木有） 
所以在使用的时候一定要注意一下。要紧跟字符串使用arg( ) 。 
二.使用arg(str1).arg(str2).arg(str3) 
这种方法其实前面用到了，如下面例子： 
```cpp
QString str=QString("%1 %2 %3 %4").arg("A").arg("B").arg("C").arg("D");
//str=="A B C　D"
```
arg( )里的参数在进行替换时，只会按照数字从小到大的顺序进行替换，只有比当前数字小的所有数字都替换完成了，才会替换当前数字，否则将当前数字和%按字符串处理。且数字为自然数！ 
三. 使用arg(int, int, int) 
原型【1】为： 
QString QString::arg(inta,intfieldWidth = 0,intbase = 10, QChar fillChar = QLatin1Char(' '))const 
a：表示要处理的数字；fieldWidth：字宽；base：数字基数 
如下列： 
str = QString("十进制 63 的十进制为 %1") 
            .arg(63,0,10); 
//输出："十进制 63 的十进制为 63" 
str = QString("十进制 63 的十六进制为 %1") 
            .arg(63,0,16); 
//输出："十进制 63 的十六进制为 3f" 
str = QString("%1 %L2 %L3") 
            .arg(12345) 
            .arg(1987654321) //根据结果，可知道这个L的作用 
            .arg(12345,0,8);//但使用这种方式时，L失效 
//输出为：12345 1,987,654,321 30071 
//这里%L3的L没有任何作用 
在原型【1】中的a的类型还可以是uint、long、double、udouble等数字的类型！ 
这里当a的类型为double时，原型为： 
QString QString::arg(doublea,intfieldWidth = 0,charformat ='g',intprecision = -1, QChar fillChar = QLatin1Char(' '))const 
多了两个参数：format和precision，一个指明规格，一个指明精度。规格默认为 'g' 或 'G'，指示后面的精度为整数和小数部分总和的长度；规格为'e'或 'E' 时，表示为科学记数法形式，精度部分指示的为小数部分的位数长度；规格为其他字符时，无作用，输出原double数字。如： 
```cpp
doubled = 12.345689;
str = QString("delta: %1").arg(d, 0,'g',4);
//输出为："delta: 12.35"     ---这里进行了四舍五入
str = QString("delta: %1").arg(d, 0,'G',5);
//输出为："delta: 12.346"     ---这里进行了四舍五入
str = QString("delta: %1").arg(d, 0,'e',4);
//输出为："delta: 1.234e+01"
str = QString("delta: %1").arg(d, 0,'E',4);
//输出为："delta: 1.234E+01"
str = QString("delta: %1").arg(d, 0,'r',4);
//输出为："delta: 12.345689"
str = QString("delta: %1").arg(d, 0,'Q',4);
//输出为："delta: 12.345689"
```

