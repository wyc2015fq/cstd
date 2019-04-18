# C 语言预处理命令总结大全 - 文章 - 伯乐在线
原文出处： [zhenhuaqin](http://blog.chinaunix.net/uid-21411227-id-3071612.html)
C程序的源代码中可包括各种编译指令，这些指令称为预处理命令。虽然它们实际上不是C语言的一部分，但却扩展了C程序设计的环境。本节将介绍如何应用预处理程序和注释简化程序开发过程，并提高程序的可读性。ANSI标准定义的C语言预处理程序包括下列命令：
#define，#error，＃include，#if，#else，#elif，#endif，#ifdef，#ifndef，#undef，#line，#pragma等。
非常明显，所有预处理命令均以符号#开头，下面分别加以介绍。
#### 一 #define
命令#define定义了一个标识符及一个串。在源程序中每次遇到该标识符时，均以定义的串代换它。ANSI标准将标识符定义为宏名，将替换过程称为宏替换。命令的一般形式为：


```
#define identifier string
```
注意：
1该语句没有分号。在标识符和串之间可以有任意个空格，串一旦开始，仅由一新行结束。
2宏名定义后，即可成为其它宏名定义中的一部分。
3 宏替换仅仅是以文本串代替宏标识符，前提是宏标识符必须独立的识别出来，否则不进行替换。例如：


```
#define XYZ this is a tes
```
使用宏printf(“XYZ”)；//该段不打印”this is a test”而打印”XYZ”。因为预编译器识别出的是”XYZ”
4如果串长于一行，可以在该行末尾用一反斜杠’ \’续行。


```
#defineLONG_STRING"this is a very long\
string that is used as an example"
```
5 C语言程序普遍使用大写字母定义标识符。
6 用宏代换代替实在的函数的一大好处是宏替换增加了代码的速度，因为不存在函数调用的开销。但增加速度也有代价：由于重复编码而增加了程序长度。
#### 二 #error
命令#error强迫编译程序停止编译，主要用于程序调试。
#error指令使预处理器发出一条错误消息，该消息包含指令中的文本.这条指令的目的就是在程序崩溃之前能够给出一定的信息。
#### 三 #include
命令＃i nclude使编译程序将另一源文件嵌入带有＃include的源文件，被读入的源文件必须用双引号或尖括号括起来。例如：
＃include”stdio.h”或者＃include
这两行代码均使用C编译程序读入并编译用于处理磁盘文件库的子程序。
将文件嵌入＃i nclude命令中的文件内是可行的，这种方式称为嵌套的嵌入文件，嵌套层次依赖于具体实现。
如果显式路径名为文件标识符的一部分，则仅在那些子目录中搜索被嵌入文件。否则，如果文件名用双引号括起来，则首先检索当前工作目录。如果未发现文件，则在命令行中说明的所有目录中搜索。如果仍未发现文件，则搜索实现时定义的标准目录。
如果没有显式路径名且文件名被尖括号括起来，则首先在编译命令行中的目录内检索。如果文件没找到，则检索标准目录，不检索当前工作目录。
#### 四 条件编译命令
有几个命令可对程序源代码的各部分有选择地进行编译，该过程称为条件编译。商业软件公司广泛应用条件编译来提供和维护某一程序的许多顾客版本。
#if、#else，#elif及#endif
#if的一般含义是如果#if后面的常量表达式为true，则编译它与#endif之间的代码，否则跳过这些代码。命令#endif标识一个#if块的结束。


```
#if constant-expression
statement sequence
#endif
```
Eg:


```
#define MAX 91
#include 
using namespace std;
int main()
{
#if MAX > 99
       cout<<"MAX is bigger than 99"<
#elif MAX > 90
       cout<<"MAX is bigger than 90"<
#else
       cout<<"MAX is smaller than 90"<
#endif
       return 0;
}
```
跟在#if后面的表达式在编译时求值，因此它必须仅含常量及已定义过的标识符，不可使用变量。表达式不许含有操作符sizeof（sizeof也是编译时求值）。
#else命令的功能有点象C语言中的else；#else建立另一选择（在#if失败的情况下）。注意，#else属于#if块。
#elif命令意义与ELSE IF 相同，它形成一个if else-if阶梯状语句，可进行多种编译选择。#elif 后跟一个常量表达式。如果表达式为true，则编译其后的代码块，不对其它#elif表达式进行测试。否则，顺序测试下一块。


```
#if expression
statement sequence
#elif expression1
statement sequence
#endif
```
在嵌套的条件编译中#endif、#else或#elif与最近#if或#elif匹配。


```
# ifdef 和# ifndef
```
条件编译的另一种方法是用#ifdef与#ifndef命令，它们分别表示”如果有定义”及”如果无定义”。# ifdef的一般形式是：


```
# ifdef macroname
statement sequence
#endif
```
#ifdef与#ifndef可以用于#if、#else，#elif语句中，但必须与一个#endif。


```
#define MAX 91
#include 
using namespace std;
 
int main()
{
#ifdef MAX
       cout<<"hello,MAX!"<
#else
       cout<<"where is MAX?"<
#endif
#ifndef LEO
       cout<<"LEO is not defined"<
#endif
       return 0;
}
```
命令#undef 取消其后那个前面已定义过有宏名定义。一般形式为：


```
#undef macroname
```
命令#line改变__LINE__与__FILE__的内容，它们是在编译程序中预先定义的标识符。命令的基本形式如下：


```
#line number["filename"]
```
其中的数字为任何正整数，可选的文件名为任意有效文件标识符。行号为源程序中当前行号，文件名为源文件的名字。命令#line主要用于调试及其它特殊应用。注意：在#line后面的数字标识从下一行开始的数字标识。


```
#line 100 "jia"
       cout<<"#line change line and filename!"<
       cout<<__LINE__<
       cout<<__FILE__<
```
#### 五 #pragma
命令#pragma 为实现时定义的命令，它允许向编译程序传送各种指令。
#pragma的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。#pragma指令对每个编译器给出了一个方法,在保持与C和C++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器或操作系统专有的,且对于每个编译器都是不同的。
其格式一般为: #Pragma Para
##### 1 message 参数。
Message 参数能够在编译信息输出窗口中输出相应的信息，这对于源代码信息的控制是非常重要的。其使用方法为：
#pragma message(“消息文本”)
当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。
当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法


```
#ifdef _X86
#pragma message(“_X86 macro activated!”)
#endif
```
当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_
X86 macro activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了。
##### 2 code_seg 参数。
格式如：


```
#pragma code_seg( ["section-name"[,"section-class"] ] )
```
它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。
##### 3 #pragma once (比较常用）
只要在头文件的最开始加入这条指令就能够保证头文件被编译一次。这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。
#### 4 #pragma hdrstop
表示预编译头文件到此为止，后面的头文件不进行预编译。BCB可以预编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。
有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用#pragma startup指定编译优先级，如果使用了#pragma package(smart_init) ，BCB就会根据优先级的大小先后编译。
##### 5 #pragma resource “*.dfm”
表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体外观的定义。
##### 6 #pragma warning( disable : 4507 34; once : 4385; error : 164 )
等价于：


```
#pragma warning(disable:4507 34) /* 不显示4507和34号警告信息。如果编译时总是出现4507号警告和34号警告,  而认为肯定不会有错误，可以使用这条指令。*/
#pragma warning(once:4385) // 4385号警告信息仅报告一次
#pragma warning(error:164) // 把164号警告信息作为一个错误。
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
//.......
#pragma warning( pop )
```
在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。
##### 7 pragma comment(…)
该指令将一个注释记录放入一个对象文件或可执行文件中。
常用的lib关键字，可以帮我们连入一个库文件。
##### 8 progma pack（n）
指定结构体对齐方式。#pragma pack(n)来设定变量以n字节对齐方式。
n 字节对齐就是说变量存放的起始地址的偏移量有两种情况：
第一、如果n大于等于该变量所占用的字节数，那么偏移量必须满足默认的对齐方式，
第二、如果n小于该变量的类型所占用的字节数，那么偏移量为n的倍数，不用满足默认的对齐方式。
结构的总大小也有个约束条件，分下面两种情况：如果n大于所有成员变量类型所占用的字节数，那么结构的总大小必须为占用空间最大的变量占用的空间数的倍数； 否则必须为n的倍数。
下面举例说明其用法。


```
#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐
struct test
{
char m1;
double m4;
int m3;
};
#pragma pack(pop)//恢复对齐状态
```
为测试该功能，可以使用sizeof（）测试结构体的长度！
