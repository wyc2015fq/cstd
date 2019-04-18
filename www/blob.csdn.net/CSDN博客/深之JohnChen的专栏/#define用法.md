# #define用法 - 深之JohnChen的专栏 - CSDN博客

2007年09月07日 15:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1788标签：[mfc																[编译器																[跨平台																[windows																[input																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[C/C++](https://blog.csdn.net/byxdaz/article/category/284144)


1.简单的define定义

#define MAXTIME 1000

一个简单的MAXTIME就定义好了，它代表1000，如果在程序里面写

if(i<MAXTIME){.........}

编译器在处理这个代码之前会对MAXTIME进行处理替换为1000。

这样的定义看起来类似于普通的常量定义CONST，但也有着不同，因为define的定义更像是简单的文本替换，而不是作为一个量来使用，这个问题在下面反映的尤为突出。

2.define的“函数定义”

define可以像函数那样接受一些参数，如下

#define max(x,y) (x)>(y)?(x):(y);

这个定义就将返回两个数中较大的那个，看到了吗？因为这个“函数”没有类型检查，就好像一个函数模板似的，当然，它绝对没有模板那么安全就是了。可以作为一个简单的模板来使用而已。 
但是这样做的话存在隐患，例子如下：
#define Add(a,b) a+b;
在一般使用的时候是没有问题的，但是如果遇到如：c * Add(a,b) * d的时候就会出现问题，代数式的本意是a+b然后去和c，d相乘，但是因为使用了define（它只是一个简单的替换），所以式子实际上变成了
c*a + b*d

另外举一个例子：
#define pin (int*);
pin a,b;
本意是a和b都是int型指针，但是实际上变成int* a,b;
a是int型指针，而b是int型变量。
这是应该使用typedef来代替define，这样a和b就都是int型指针了。

所以我们在定义的时候，养成一个良好的习惯，建议所有的层次都要加括号。

3.宏的单行定义
#define A(x) T_##x
#define B（x) #@x

#define C（x) #x
我们假设：x=1，则有：
A(1)------〉T_1
B(1)------〉'1'
C(1)------〉"1"

3.define的多行定义

define可以替代多行的代码，例如MFC中的宏定义（非常的经典，虽然让人看了恶心）

#define MACRO(arg1, arg2) do { /
/* declarations */ /
stmt1; /
stmt2; /
/* ... */ /
} while(0) /* (no trailing ; ) */
关键是要在每一个换行的时候加上一个"/"

4.在大规模的开发过程中，特别是跨平台和系统的软件里，define最重要的功能是条件编译。

就是：
#ifdef WINDOWS
......
......
#endif
#ifdef LINUX
......
......
#endif

可以在编译的时候通过#define设置编译环境

5.如何定义宏、取消宏

//定义宏
#define [MacroName] [MacroValue]
//取消宏
#undef [MacroName]
普通宏
#define PI (3.1415926)

带参数的宏
#define max(a,b) ((a)>(b)? (a),(b))
关键是十分容易产生错误，包括机器和人理解上的差异等等。

6.条件编译
#ifdef XXX…(#else) …#endif
例如 #ifdef DV22_AUX_INPUT
#define AUX_MODE 3
#else
#define AUY_MODE 3
＃endif
#ifndef XXX … (#else) … #endif

7.头文件(.h)可以被头文件或C文件包含；
重复包含（重复定义）
由于头文件包含可以嵌套，那么C文件就有可能包含多次同一个头文件，就可能出现重复定义的问题的。


