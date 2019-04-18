# 从C++ int类型的变量范围谈起 - 心纯净，行致远 - CSDN博客





2018年11月13日 08:40:06[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：85








> 
     C/C++中，整型数据是用int来进行定义的，整型的范围就是指int类型所能表示的整数的范围。在32位或64位机器中，int占4个字节，即32位。

       int能表示的最大正整数为：0111 1111 1111 1111 1111 1111 1111 1111  (最高位表示符号位，正数符号位为0)对应的10进制数为2^31-1=2147483647，对应的十六进制表示为：0x7FFFFFFF。

       int能表示的最小负整数为：1000 0000 0000 0000 0000 0000 0000 0000  (最高位表示符号位，负数符号位为1)，负数在计算机中以补码的形式存在，所以对应的原码（补码的补码）也是1000 0000 0000 0000 0000 0000 0000 0000，对应的十六进制表示为0x80000000，而C/C++规定该值为-2^31=-2147483648。

       所以最终，int类型（整数类型）的范围为-2^31 ~ 2^31-1，即-2147483648~2147483647，十六进制表示：0x80000000~0x7FFFFFFF。


从学习C语言开始，int类型所占字节数，以及数值范围就是一个挥之不去的问题。一开始会死记硬背一个char 1个字节，一个字节8个bit。64位机器上面一个int 4个字节，32位机器上面不一样。那时候并不知道编译器也分很多种，每一种实现的细节不一样，也不知道各家编译器是遵循C++标准委员会的标准。后来学会像编译器求证，以其输出为准，也学会向标准求证。

在一篇博客上面看到用numeric_limits 类可以方便地了解各个类型的取值范围以及占用内存，于是满心欢喜地将代码运行，发现有些问题：

```cpp
#include<iostream> 
#include <limits> 
using namespace std; int main() { 
cout << "type: \t\t" << "------------------size-----------------------"<< endl; 
cout << "bool: \t\t" << "Bytes:" << sizeof(bool); 
cout << "\tMaxValue:" << (numeric_limits<bool>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<bool>::min)() << endl; 
cout << "char: \t\t" << "Bytes:" << sizeof(char); 
cout << "\tMaxValue:" << (numeric_limits<char>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<char>::min)() << endl; 
cout << "signed char: \t" << "Bytes:" << sizeof(signed char); 
cout << "\tMaxValue:" << (numeric_limits<signed char>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<signed char>::min)() << endl; 
cout << "unsigned char: \t" << "Bytes:" << sizeof(unsigned char); 
cout << "\tMaxValue:" << (numeric_limits<unsigned char>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<unsigned char>::min)() << endl; 
cout << "wchar_t: \t" << "Bytes:" << sizeof(wchar_t); 
cout << "\tMaxValue:" << (numeric_limits<wchar_t>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<wchar_t>::min)() << endl; 
cout << "short: \t\t" << "Bytes:" << sizeof(short); 
cout << "\tMaxValue:" << (numeric_limits<short>::max)(); 
cout << "\t\tMinValue:" << (numeric_limits<short>::min)() << endl;
 cout << "int: \t\t" << "Bytes:" << sizeof(int);
 cout << "\tMaxValue:" << (numeric_limits<int>::max)(); 
cout << "\tMinValue:" << (numeric_limits<int>::min)() << endl; 
cout << "unsigned: \t" << "Bytes:" << sizeof(unsigned); 
cout << "\tMaxValue:" << (numeric_limits<unsigned>::max)(); 
cout << "\tMinValue:" << (numeric_limits<unsigned>::min)() << endl; 
cout << "long: \t\t" << "Bytes:" << sizeof(long); 
cout << "\tMaxValue:" << (numeric_limits<long>::max)(); 
cout << "\tMinValue:" << (numeric_limits<long>::min)() << endl;
cout << "type: \t\t" << "************size**************"<< endl; return 0; }
```

运行结果：

![](https://images2015.cnblogs.com/blog/902342/201704/902342-20170411201924860-705894530.png)



常用的int，long等类型的取值范围确实如愿显示。但是，在关于 char的判断中，有显示的问题。从使用的角度而言，其实char用于表示字符，判断大小并不合理。当然，这只是一个“完形填空”式的推导。既然显示在屏幕上的输出有问题，本着钻研精神，要弄清楚为什么。

首先是假设：

1. cout无法处理这个numeric_limit<char>::max()函数

2.numeric_limit<char>::max()的返回值并不是可显示字符。



为了验证第一点，我找到C++标准委员会的网站，找到C++14的草稿标准：

![](https://images2015.cnblogs.com/blog/902342/201704/902342-20170411202651047-234750302.png)

从这里看出，返回的是模板类型 T的构造函数。似乎排除了第一种可能。但标准太不具体，我在Clion上面用debug模式直接找到库文件来确认。值得一提的是编译器版本：gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.2)。

第一步：

![](https://images2015.cnblogs.com/blog/902342/201704/902342-20170411203043579-198296230.png)

再往下一层：

![](https://images2015.cnblogs.com/blog/902342/201704/902342-20170411203136813-1937765808.png)

在limits的实现里面，都是再用宏定义的位操作来实现最大和最小值的获取。先看__glibcxx_digits宏定义。这里面的逻辑很简单，占用字节数乘以__CHAR_BIT__减去符号位（1位）。在char的例子里面，得出来的位数是7。从这里不妨猜测，编译器对一个字节大小的定义很有可能是基于__CHAR_BIT__的。在一篇博文看过对于古老的机器，char类型只有7bit。按照这个猜测，在描述内置int型大小的时候，就可以说一个int等于4个char。这个猜测挺有意思，不过有待考究。

 进一步分析，__glicxx_min宏定义，对于传入的类型_Tp，如果有符号，就执行(_Tp)1 << _glibcxx_digits(_Tp), 没有就是(_Tp)0.

对于char型，它居然被是认为有符号的！ 所以最小值的计算变成 (char)1 << 7，二进制表示10000000, -128.

一个被赋值为-128的char类型字符，其实现的动机难以理解，不过从某程度上说明标准与实现之间的距离，对于一个函数的实现，尤其是不常用的功能，最好还是自己进行确认。



