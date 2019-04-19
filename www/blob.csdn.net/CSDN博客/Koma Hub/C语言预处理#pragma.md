# C语言预处理#pragma - Koma Hub - CSDN博客
2017年11月30日 20:01:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：257
**1.#pragma message**
如果已经宏定义的PI，
```cpp
/**#pragma message(): 在编译信息输出窗口中输出相应的信息
 *                      这对于源代码信息的控制是非常重要的*/
#ifdef PI
#pragma message("Already define PI.")
#endif
```
编译的时候就会出现：
```
-bash-4.1$ gcc -o b Demo.c
Demo.c: 在函数‘main’中:
Demo.c:14: 附注：#pragma message：Already define PI.
```
**2.#pragma code_seg   [点击打开code_seg原文链接](http://st251256589.blog.163.com/blog/static/164876449201110562212822/)**
```cpp
/* 另一个使用得比较多的pragma 参数是code_seg。格式如：
 * #pragma code_seg( ["section-name"[,"section-class"] ] )
 * #pragma code_seg( [ [ { push | pop}, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] )
 * 它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。*/
//默认情况下,函数被存放在.text节中
void fun1(){  // stored in .text
}
//将函数存放在.my_data1节中
#pragma code_seg(".my_data1")
void fun2() { // stored in my_data1
}
//r1为标识符,将函数放入.my_data2节中
#pragma code_seg(push, r1, ".my_data2")
void fun3() { // stored in my_data2
}
```
**3.#pragma once**
使头文件只编译一次
```cpp
//只要在头文件的最开始加入这条指令就能够保证头文件被编译一次
#pragma once 
#ifndef _HEAD_
#define _HEAD_
void f(void);
#endif
void f(){
    printf("head.h:f()\n");
}
```
**4.#pragma warning**[点击打开code_seg原文链接](http://st251256589.blog.163.com/blog/static/164876449201110562212822/)****
有选择性地修改编译器的警告消息
```cpp
#pragma warning( warning-specifier : warning-number-list [; warning-specifier : warning-number-list...] )
#pragma warning( push[ ,n ] )//1,2,3,4:四个警告级别
#pragma warning( pop )
//主要用到的警告表示有如下几个:
//once:只显示一次(警告/错误等)消息
//default:重置编译器的警告行为到默认状态
//disable:禁止指定的警告信息
//error:将指定的警告信息作为错误报告
//如果大家对上面的解释不是很理解,可以参考一下下面的例子及说明
#pragma warning( disable : 4507 34; once : 4385; error : 164 )
//等价于：
#pragma warning(disable:4507 34)   // 不显示4507和34号警告信息
#pragma warning(once:4385)         // 4385号警告信息仅报告一次
#pragma warning(error:164)         // 把164号警告信息作为一个错误。
//同时这个pragma warning 也支持如下格式：
#pragma warning( push [ ,n ] )
#pragma warning( pop )
//这里n代表一个警告等级(1---4)。
#pragma warning( push )保存所有警告信息的现有的警告状态。
#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。
#pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。例如：
#pragma warning( push )
#pragma warning( disable : 4705 )
#pragma warning( disable : 4706 )
#pragma warning( disable : 4707 )
#pragma warning( pop )
//在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)
//在使用标准C++进行编程的时候经常会得到很多的警告信息,
//而这些警告信息都是不必要的提示,
//所以我们可以使用#pragma warning(disable:4786)来禁止该类型
//的警告在vc中使用ADO的时候也会得到不必要的警告信息,
//这个时候我们可以通过#pragma warning(disable:4146)
//来消除该类型的警告信息
```
**5.#pragma hdrstop**
表示预编译头文件到此为止，后面的头文件不进行预编译。如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。
**6.#pragma comment  **[点击打开code_seg原文链接](http://st251256589.blog.163.com/blog/static/164876449201110562212822/)****
该指令的格式为：   
```cpp
#pragma comment( "comment-type" [, commentstring] )
```
该指令将一个注释记录放入一个对象文件或可执行文件中,comment-type(注释类型):可以指定为五种预定义的标识符的其中一种。
五种预定义的标识符为:
1、compiler:将编译器的版本号和名称放入目标文件中,本条注释记录将被编译器忽略
如果你为该记录类型提供了commentstring参数,编译器将会产生一个警告
例如:
```cpp
#pragma comment( compiler )
```
2、exestr:将commentstring参数放入目标文件中,在链接的时候这个字符串将被放入到可执行文件中,当操作系统加载可执行文件的时候,该参数字符串不会被加载到内存中.但是,该字符串可以被dumpbin之类的程序查找出并打印出来,你可以用这个标识符将版本号码之类的信息嵌入到可执行文件中!
3、lib:这是一个非常常用的关键字,用来将一个库文件链接到目标文件中常用的lib关键字，可以帮我们连入一个库文件。
例如: 
```cpp
#pragma comment(lib, "user32.lib")
```
该指令用来将user32.lib库文件加入到本工程中
4、linker:将一个链接选项放入目标文件中,你可以使用这个指令来代替由命令行传入的或者在开发环境中设置的链接选项,你可以指定/include选项来强制包含某个对象,例如:
```cpp
#pragma comment(linker, "/include:__mySymbol")
```
你可以在程序中设置下列链接选项
/DEFAULTLIB
/EXPORT
/INCLUDE
/MERGE
/SECTION
这些选项在这里就不一一说明了,详细信息请看msdn!
5、user:将一般的注释信息放入目标文件中commentstring参数包含注释的文本信息,这个注释记录将被链接器忽略
例如:
```cpp
#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ )
```
```cpp
/**
#pragma comment(...) 
该指令将一个注释记录放入一个对象文件或可执行文件中。
常用的lib关键字，可以帮我们连入一个库文件。
如：#pragmacomment(lib, "comctl32.lib")
*/
#pragma comment(lib, "comctl32.lib")
```
**7.#pragma pack [点击打开原文链接](http://c.biancheng.net/cpp/html/469.html)**
这里重点讨论内存对齐的问题和#pragma pack（）的使用方法。
什么是内存对齐？先看下面的结构：
```cpp
struct TestStruct1
{
   char c1;
   short s;
   char c2;
   int i;
};
```
假设这个结构的成员在内存中是紧凑排列的，假设c1 的地址是0，那么s 的地址就应该是1，c2 的地址就是3，i 的地址就是4。也就是c1 地址为00000000, s 地址为00000001, c2地址为00000003, i 地址为00000004。
可是，我们在Visual C++6.0 中写一个简单的程序：
```cpp
struct TestStruct1 a;
printf("c1 %p, s %p, c2 %p, i %p\n",
(unsigned int)(void*)&a.c1 - (unsigned int)(void*)&a,
(unsigned int)(void*)&a.s - (unsigned int)(void*)&a,
(unsigned int)(void*)&a.c2 - (unsigned int)(void*)&a,
(unsigned int)(void*)&a.i - (unsigned int)(void*)&a);
```
运行，输出：
```cpp
c1 00000000, s 00000002, c2 00000004, i 00000008。
```
为什么会这样？这就是内存对齐而导致的问题。
1、为什么会有内存对齐？
字，双字，和四字在自然边界上不需要在内存中对齐。（对字，双字，和四字来说，自然边界分别是偶数地址，可以被4 整除的地址，和可以被8 整除的地址。）无论如何，为了提高程序的性能，数据结构（尤其是栈）应该尽可能地在自然边界上对齐。原因在于，为了访问未对齐的内存，处理器需要作两次内存访问；然而，对齐的内存访问仅需要一次访问。
一个字或双字操作数跨越了4 字节边界，或者一个四字操作数跨越了8 字节边界，被认为是未对齐的，从而需要两次总线周期来访问内存。一个字起始地址是奇数但却没有跨越字边界被认为是对齐的，能够在一个总线周期中被访问。某些操作双四字的指令需要内存操作数在自然边界上对齐。如果操作数没有对齐，这些指令将会产生一个通用保护异常。
双四字的自然边界是能够被16 整除的地址。其他的操作双四字的指令允许未对齐的访问（不会产生通用保护异常），然而，需要额外的内存总线周期来访问内存中未对齐的数据。
缺省情况下，编译器默认将结构、栈中的成员数据进行内存对齐。因此，上面的程序输出就变成了：c1 00000000, s 00000002, c2 00000004, i 00000008。编译器将未对齐的成员向后移，将每一个都成员对齐到自然边界上，从而也导致了整个结构的尺寸变大。尽管会牺牲一点空间（成员之间有部分内存空闲），但提高了性能。也正是这个原因，我们不可以断言sizeof(TestStruct1)的结果为8。在这个例子中，sizeof(TestStruct1)的结果为12。
2、如何避免内存对齐的影响？
那么，能不能既达到提高性能的目的，又能节约一点空间呢？有一点小技巧可以使用。比如我们可以将上面的结构改成：
```cpp
struct TestStruct2
{
   char c1;
   char c2;
   short s;
   int i;
};
```
这样一来，每个成员都对齐在其自然边界上，从而避免了编译器自动对齐。在这个例子中，sizeof(TestStruct2)的值为8。这个技巧有一个重要的作用，尤其是这个结构作为API的一部分提供给第三方开发使用的时候。第三方开发者可能将编译器的默认对齐选项改变，从而造成这个结构在你的发行的DLL 中使用某种对齐方式，而在第三方开发者哪里却使用另外一种对齐方式。这将会导致重大问题。
比如，TestStruct1 结构，我们的DLL 使用默认对齐选项，对齐为c1 00000000, s 00000002, c2 00000004, i 00000008，同时sizeof(TestStruct1)的值为12。
而第三方将对齐选项关闭，导致c1 00000000, s 00000001, c2 00000003, i 00000004，同时sizeof(TestStruct1)的值为8。
除此之外我们还可以利用#pragma pack（）来改变编译器的默认对齐方式（当然一般编译器也提供了一些改变对齐方式的选项，这里不讨论）。
使用指令#pragma pack (n)，编译器将按照n 个字节对齐。
使用指令#pragma pack ()，编译器将取消自定义字节对齐方式。
在#pragma pack (n)和#pragma pack ()之间的代码按n 个字节对齐。但是，成员对齐有一个重要的条件,即每个成员按自己的方式对齐.也就是说虽然指定了按n 字节对齐,但并不是所有的成员都是以n 字节对齐。其对齐的规则是,每个成员按其类型的对齐参数(通常是这个类型的大小)和指定对齐参数(这里是n 字节)中较小的一个对齐，即：min( n, sizeof( item )) 。并且结构的长度必须为所用过的所有对齐参数的整数倍,不够就补空字节。看如下例子：
```cpp
#pragma pack(8)
struct TestStruct4
{
   char a;
   long b;
};
struct TestStruct5
{
   char c;
   TestStruct4 d;
   long long e;
};
#pragma pack()
```
问题：
A)
sizeof(TestStruct5) = ?
B)
TestStruct5 的c 后面空了几个字节接着是d?
TestStruct4 中,成员a 是1 字节默认按1 字节对齐,指定对齐参数为8,这两个值中取1,a
按1 字节对齐;成员b 是4 个字节,默认是按4 字节对齐,这时就按4 字节对齐,所以sizeof(TestStruct4)应该为8;TestStruct5 中,c 和TestStruct4 中的a 一样,按1 字节对齐,而d 是个结构,它是8 个字节,它
按什么对齐呢?对于结构来说,它的默认对齐方式就是它的所有成员使用的对齐参数中最大的一个, TestStruct4 的就是4.所以,成员d 就是按4 字节对齐.成员e 是8 个字节,它是默认按8字节对齐,和指定的一样,所以它对到8 字节的边界上,这时,已经使用了12 个字节了,所以又添加了4 个字节的空,从第16 个字节开始放置成员e.这时,长度为24,已经可以被8(成员e 按8字节对齐)整除.这样,一共使用了24 个字节.内存布局如下（*表示空闲内存，1 表示使用内存。单位为1byete）：
a b
TestStruct4 的内存布局：1***,1111,
c
TestStruct4.a TestStruct4.b d
TestStruct5 的内存布局： 1***, 1***, 1111, ****，11111111
这里有三点很重要：
首先，每个成员分别按自己的方式对齐,并能最小化长度。
其次，复杂类型(如结构)的默认对齐方式是它最长的成员的对齐方式,这样在成员是复杂类型时,可以最小化长度。
然后，对齐后的长度必须是成员中最大的对齐参数的整数倍,这样在处理数组时可以保证每一项都边界对齐。
补充一下,对于数组,比如:char a[3];它的对齐方式和分别写3 个char 是一样的.也就是说它还是按1 个字节对齐.如果写: typedef char Array3[3];Array3 这种类型的对齐方式还是按1个字节对齐,而不是按它的长度。
但是不论类型是什么,对齐的边界一定是1,2,4,8,16,32,64....中的一个。
另外，注意别的#pragma pack 的其他用法：
```cpp
#pragma pack(push) //保存当前对其方式到packing stack
#pragma pack(push,n) 等效于
#pragma pack(push)
#pragma pack(n) //n=1,2,4,8,16 保存当前对齐方式，设置按n 字节对齐
#pragma pack(pop) //packing stack 出栈，并将对其方式设置为出栈的对齐方
```
