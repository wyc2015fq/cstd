# C++ 之预定义类型 IO 格式控制 - 文章 - 伯乐在线
原文出处： [Rookie_J](http://www.cnblogs.com/CaiNiaoZJ/archive/2011/08/17/2142010.html)
在C语言里，我们可以通过函数printf和scanf来进行格式化控制。而在C++中仍然包含了前者，但还提供了以下两种格式控制的方法：(1)使用流成员函数进行格式控制;(2)使用预定义操作符进行格式控制。下面我来一一介绍：
1.流成员函数主要是指ios类(流基类)中的，分别有：
(1).设置状态标志流成员函数setf
一般格式：long ios::setf(long flags),调用格式：流对象.setf(ios::状态标志)
ios类的状态标志有：
![](http://jbcdn2.b0.upaiyun.com/2016/10/2f5a1a8a226fbebeae1332fa8193400c.jpg)
因为状态标志在ios类中定义为枚举值，所以在引用这些值前要加上ios::，如果有多项标志，中间则用”|”分隔;
(2).清除状态标志流成员函数unsetf
一般格式：long ios::unsetf(long flags)，调用格式：流对象.unsetf(ios::状态标志);
(3).设置域宽流成员函数width
一般格式：int ios::width(int n)，调用格式：流对象.width(n);//注：它只对下一个流输出有效，输出完成后，恢复默认值0
(4).设置实数的精度流成员函数precision
一般格式：int ios::precision(int n)，调用格式：流对象.precision(n)；//注：参数n在十进制小数形式输出时代表有效数字。在以fixed形式和scientific形式输出时代表小数位数
(5).填充字符流成员函数fill
一般格式：char ios::fill(char ch)，调用格式：流对象.fill(ch);//注：当输出值不满宽域时用填充符来填充，默认填充符为空格，它与width函数搭配。
下面用示例来验证：

```
#include "stdafx.h"
#include <iostream>
#include <string>
int main()
{
    std::cout.setf(std::ios::left|std::ios::showpoint|std::ios::unitbuf);
    std::cout.precision(6);
    std::cout<<123.45678;
    std::cout.width(50);
    std::cout.fill('-');
    std::cout.unsetf(std::ios::left);//清除状态左对齐
    std::cout.setf(std::ios::right);
    std::cout<<"十进制小数输出,有效数字为6位"<<std::endl;
    std::cout.setf(std::ios::left|std::ios::fixed);
    std::cout.precision(6);
    std::cout<<123.45678;
    std::cout.width(50);
    std::cout.fill('-');
    std::cout.unsetf(std::ios::left|std::ios::fixed);//清除状态左对齐和定点格式
    std::cout.setf(std::ios::right);
    std::cout<<"固定小数位fixed,小数位为6位"<<std::endl;
    std::cout.setf(std::ios::left|std::ios::scientific);
    std::cout.precision(6);
    std::cout<<123.45678;
    std::cout.width(50);
    std::cout.fill('-');
    std::cout.unsetf(std::ios::left|std::ios::scientific);//清除状态左对齐和科学计数法格式
    std::cout.setf(std::ios::right);
    std::cout<<"科学计数法表示，小数位为6位"<<std::endl;
    std::cout.fill('');//设置填充符为默认空格
    std::cout.unsetf(std::ios::right);//清除状态靠右对齐
    std::cout.setf(std::ios::dec|std::ios::showpos|std::ios::internal);//设置状态基数为10，正整数前显示"+"和数据符号左对齐，数据本身右对齐，数据和符号之间为填充符' '
    std::cout.width(6);
    std::cout<<128<<std::endl;
    std::cout.unsetf(std::ios::dec);//清除状态基数为10
//在输出整数的八进制形式或十六进制形式之前
//先要把默认的十进制形式的标志清除std::cout.unsetf(std::ios::dec)
    std::cout.setf(std::ios::oct|std::ios::showbase);//设置状态基数为8,输出整数时显示基数符号
//std::ios::internal标志对八进制不起作用
    std::cout<<128<<std::endl;
    std::cout.unsetf(std::ios::oct);//清除状态基数为8
    std::cout.setf(std::ios::hex|std::ios::uppercase);//设置状态基数为16,输出整数时显示基数符号和科学计数法输出时E大写，十六进制字母大写
//std::ios::internal标志对十六进制不起作用
    std::cout<<255<<std::endl;
    std::cout.unsetf(std::ios::hex);//清除状态基数为16
return0;
}
```
结果：
![](http://jbcdn2.b0.upaiyun.com/2016/10/fde3af3c55eebd0999143b2ef517e600.jpg)
2.用ios类中的成员函数来进行IO格式的控制总需要写一条单独的语句，而不能直接嵌入到IO语句中去，显得很不方便。因此C++又提供了一种用操纵符来控制IO的格式。操纵符分为带参和不带参的两种，带参的定义在头文件iomanip.h中，不带参的定义在iostream.h中。下面分别是C++中的预定义操作符：
(1)dec:设置整数基数为10，用于输出和输入；
(2)hex:设置整数基数为16，用于输出和输入；
(3)oct:设置整数基数为8，用于输出和输入；
(4)ws:跳过输入的空格符，用于输入；
(5)endl:输出一个换行符并刷新输出流，用于输出；
(6)ends:插入一个空字符null，通常用来结束一个字符串，用于输出；
(7)flush:刷新一个输出流，用于输出；
(8)setbase(n):设置整数的基数为n(可取0或10代表十进制，8代表八进制和16代表十六进制，默认为0)，用于输入和输出；
(9)setfill(c):设置填充符(默认为空格)，用于输出；
(10)setprecision(n):设置实数精度n,原理和成员函数precision一样，用于输出；
(11)setw(n):设置域宽n，用于输出；
(12)setiosflags(flags):设置指定状态标志，多个用”|”分隔，用于输出和输入；
(13)resetiosflags(flags):清除指定状态标志，多个用”|”分隔，用于输出和输入；
操作符setiosflags(flags)和resetiosflags(flags)的部分状态标志：
![](http://jbcdn2.b0.upaiyun.com/2016/10/035792a668db9fe91c8aad0bdcf67e34.jpg)
下面用示例来验证：

```
#include "stdafx.h"
#include <iostream>
#include <iomanip>//带形参的操纵符必须含有该头文件
#include <string>
int main()
{
    std::string str="abcdefg";
    std::cout<<str<<std::ends<<std::endl;//std::ends用来结束一个字符串
    std::cout<<std::setiosflags(std::ios::left|std::ios::showpoint|std::ios::unitbuf)<<std::setprecision(6)
<<123.45678<<std::setw(50)<<std::setfill('-')<<std::resetiosflags(std::ios::left)
<<std::setiosflags(std::ios::right)<<"科学计数法表示，小数位为6位"<<std::endl;
    std::cout<<std::setiosflags(std::ios::left|std::ios::fixed)<<std::setprecision(6)
<<123.45678<<std::setw(50)<<std::setfill('-')<<std::resetiosflags(std::ios::left|std::ios::fixed)
<<std::setiosflags(std::ios::right)<<"固定小数位fixed,小数位为6位"<<std::endl;
    std::cout<<std::setiosflags(std::ios::left|std::ios::scientific)<<std::setprecision(6)
<<123.45678<<std::setw(50)<<std::setfill('-')<<std::resetiosflags(std::ios::left|std::ios::scientific)
<<std::setiosflags(std::ios::right)<<"科学计数法表示，小数位为6位"<<std::endl;
    std::cout<<std::setfill('')<<std::resetiosflags(std::ios::right)<<std::flush;//std::flush刷新一个输出流
    std::cout<<std::dec//或std::setbase(10或0)
<<std::setiosflags(std::ios::showpos|std::ios::internal)<<std::setw(6)<<128<<std::endl;
    std::cout<<std::setbase(8)//或std::oct
<<std::setiosflags(std::ios::showbase)<<128<<std::endl;
    std::cout<<std::setbase(16)//或std::hex
<<std::setiosflags(std::ios::showbase|std::ios::uppercase)<<255<<std::endl;
return0;
}
```
结果：
![](http://jbcdn2.b0.upaiyun.com/2016/10/e898c87d0ef6472f7f642cd13c74d929.jpg)
3.除了利用系统预定义的操纵符来进行IO格式的控制外，用户还可以自定义操纵符来合并程序中频繁使用的IO写操作。定义形式如下：
输出流自定义操纵符
ostream &操纵符名(ostream &s)
{
自定义代码
return s;
}
输入流自定义操纵符
istream &操纵符名(istream &s{
自定义代码
return s;
}
返回流对象s很关键，否则操纵符就不能用在流的IO操作序列中。
用示例来验证：

```
#include "stdafx.h"
#include <iostream>
#include <iomanip>
std::ostream& outputNo(std::ostream& s)//编号格式如：0000001
{
    s<<std::setw(7)<<std::setfill('0')<<std::setiosflags(std::ios::right);
return s;
}
std::istream& To16(std::istream& s)//要求输入的数为十六进制数
{
    s>>std::hex;
return s;
}
int main()
{
    std::cout<<outputNo<<8<<std::endl;
int a;
    std::cout<<"请输入十六进制的数：";
    std::cin>>To16>>a;
    std::cout<<"转化为十进制数："<<a<<std::endl;
return0;
}
```
结果:
![](http://pic002.cnblogs.com/images/2011/188149/2011081712155157.jpg)
