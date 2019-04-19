# 【温故知新C/C++】02:string::size_type ||vector - 三少GG - CSDN博客
2011年03月28日 10:41:00[三少GG](https://me.csdn.net/scut1135)阅读数：1036标签：[string																[vector																[存储																[iterator																[数据结构																[basic](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
1. [http://www.newsmth.net/pc/pccon.php?id=10002714&nid=359771](http://www.newsmth.net/pc/pccon.php?id=10002714&nid=359771)
**之所以抛弃char*的字符串而选用C++标准程序库中的string类**，是因为他和前者比较起来，不必 担心内存是否足够、字符串长度等等，而且作为一个类出现，他集成的操作函数足以完成我们大多数情况下(甚至是100%)的需要。我们可以用 = 进行赋值操作，== 进行比较+ 做串联（是不是很简单?）。我们尽可以把它看成是C++的基本数据类型。 
首先，为了在我们的程序中使用string类型，我们必须包含头文件 <string>。如下： 
#include <string> //注意这里不是string.h string.h是C字符串头文件 
string 是C++中的字符串。 字符串对象是一种特殊类型的容器,专门设计来操作的字符序列。 不像传统的c-strings,只是在数组中的一个字符序列，我们称之为字符数组，而C + +字符串对象属于一个类，这个类有很多内置的特点，在操作方式,更直观，另外还有很多有用的成员函数。 string 的定义为:typedef basic_string<char> string;
转成C时：**c_str**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.关于string::size_type 在 c++ primer有这么段解释
string::size_type类型  
从逻辑上来讲，size()成员函数似乎应该返回整型数值，或如2.2节“建议”中所述的无符号整数。但事实上，size操作返回的是string::size_type类型的值。我们需要对这种类型做一些解释。
string类类型和许多其他库类型都定义了一些伙伴类型（companion types）。这些伙伴类型使得库类型的使用是机器无关的(machine-independent)。size_type就是这些伙伴类型中的一种。它定义为与unsigned型（unsigned int或unsigned long）具有相同的含义，而且可以保证足够大可存储任意string对象的长度。为了使用由string类型定义的size_type类型，程序员必须加上作用域操作符来说明所使用的size_type类型是由string类定义的。
任何存储string的size操作结果的变量必须为string::size_type类型。特别重要的是，不要把size的返回值赋给一个int变量。
虽然我们不知道string::size_type的确切类型，但可以知道它是unsigned型（2.1.1节）。对于任意一种给定的数据类型，它的unsigned型所能表示的最大正数值比对应的signed要大一倍。这个事实表明size_type存储的string长度是int所能存储的两倍。
使用int变量的另一个问题是，有些机器上int变量的表示范围太小，甚至无法存储实际并不长的string对象。如在有16位int型的机器上，int类型变量最大只能表示32767个字符的string对象。而能容纳一个文件内容的string对象轻易就会超过这个数字。**因此，为了避免溢出，保存一个string对象size的最安全的方法就是使用标准库类型string:: size_type。**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
3. vector
 vector是C++[标准模板库](view/3097879.htm)中的部分内容，中文偶尔译作“容器”，但并不准确。它是一个多功能的，能够操作多种[数据结构](view/9900.htm)和算法的模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，**简单地说，vector是一个能够存放任意类型的**[**动态数组**](view/675652.htm)**，能够增加和压缩数据。 **
　　简单的使用方法如下: 
　　vector<int> test;//建立一个vector 
　　test.pushback(1);//把1和2压入vector 这样test[0]就是1,test[1]就是2 
　　test.pushback(2); 
　　我们可以用一个[迭代器](view/925158.htm)： 
　　vector<int>::iterator iter=text.begin();//定义一个可以迭代int型vector的迭代器iter，它指向text的首位 
　　for(;iter!=text.end();iter++) cout<<(*iter);//iter++指的是向前迭代一位，直到iter到超出末端迭代器为止，输出迭代器指向的值
