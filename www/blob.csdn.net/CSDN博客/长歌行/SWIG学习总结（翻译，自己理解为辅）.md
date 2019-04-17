# SWIG学习总结（翻译，自己理解为辅） - 长歌行 - CSDN博客





2012年12月30日 17:18:15[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：5219
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









原文地址 http://program.upc.edu.cn/kaitian/?p=65



**SWIG介绍**

SWIG (Simplified Wrapper and Interface Generator) ，即简化包以及接口生成器，为脚本语言(tcl，perl，python等)提供了C和C++的接口。SWIG在1995年在Los Alamos National Laborator为开发一个用户接口应运而生，我的毕业设计外文翻译是其中一个科学家写的论文，在[这里](http://program.upc.edu.cn/kaitian/?p=68)可以看到。SWIG把科学家从繁杂的编写接口的工作中解脱出来，使他们把更多的时间投入到更加重要的部分上去。注意的是，貌似Google
 App Enginer也在用SWIG。同样的功能可以用C API ，CTypes，和C++的Boost库实现，本文介绍SWIG的使用。

SWIG是一个编译器，可以处理C++的声明等。

WINDOWS下，可以用VC或者是VS配置SWIG和Python，[这篇文章](http://realmike.org/blog/2010/07/18/python-extensions-in-cpp-using-swig/#custom)写的尽详细，尤其是windows的用户可以参考下。下载swig的windows版，为swig.exe增加环境变量，另外还要下载一个MinGW，或者是cygwin编译C或者是C++文件。

**脚本语言如何处理C的程序？**

脚本语言解释器可以通过高级控制来使用，底层的C++程序可以通过特殊的脚本语言命令来调用。脚本语言先把C++的原始函数转化为特殊的wrapper函数，其可以作为C++和脚本语言的胶水。Wrapper做了如下几件事：

1，收集函数参数确保他们都有效

2，调用C函数，

3 把函数的返回值转化为脚本语言可以识别的形式。

创建完wrapper函数，最后一步就是告诉脚本语言你这个函数。和一般就是在模块加载时在初始化函数里面实现。比方说，你创建了一个函数factor，那么系统就会为你增加一个factor的命令。

**变量链接**

Foo = 4

A = foo + 2.0

Foo = a + 2

为了实现这两个操作，变量一般通过一组getset函数来操纵。党变量被读取时，就调用get函数；变量值被改变时，set函数被调用。

常量：

#define RED   0xff0000，

为了确保变量可见，值在脚本语言中以RED的格式显示；实际上，脚本语对创建变量的支持已经很完美了。

**类和结构体**

脚本语言在处理简单的函数和变量时毫无压力，但是在处理类和变量时存在一些问题。最直接的技术就是去构造一些配套函数来隐藏一个结构体的底层展示。
struct Vector {            Vector();            ~Vector();            double x,y,z;};
被转化成如下几个函数
Vector *new_Vector();void delete_Vector(Vector *v);double Vector_x_get(Vector *v);double Vector_y_get(Vector *v);double Vector_z_get(Vector *v);void Vector_x_set(Vector *v, double x);void Vector_y_set(Vector *v, double y);void Vector_z_set(Vector *v, double z);
这些函数为获取object提供了解决机制，所以解释器不需要知道Vector的具体底层实现是什么（调用这些函数即可）

**5.SWIG基础**

这一节描述了SWIG的基础操作，输入文件的结构，如何处理标准C声明，
SWIG的命令：swig [ *options* ] filenameSwig –help可以获得SWIG的选项命令
输入格式：

SWIG期望文件包含标准CC++的声明，一个SWIG文件一般以.i或者是.swg为后缀。某些情况下，SWIG可以直接作为i源文件或者是图文件，但是这不是很标准的方式，这里有几个你不该这么做的原因。

SWIG标准输入文件；
%module mymodule%{#include "myheader.h"%}// Now list ANSI C/C++ declarationsint foo;int bar(int x)；...
%module表示了模块名字。

%{………….%}块里的东西直接被拷贝到SWIG创建的wrapper文件中。

这个section一般包括了头文件和一些声明，使wrapper文件正常编译。SWIG不去解析，也不去解释这个块。

**SWIG输出文件：**

SWIG的输出是CC++文件，包含了建立扩展模块需要的所有的wraper代码。根据目标语言，SWIG也会生成一些额外的文件。缺省状态下，file.i会被转化为file_wrap.c或者是file_wrap.cxx(取决于你是否使用了-c++选项)。输出文件名可以用-o选项来改变。

SWIG是一个很强大的编译器，你没有必要去修改输出文件。为了建立最终的扩展单元， SWIG输出文件和你其余的C++文件一起被编译链接，最终生成一个共享库。

**C预处理器**

像C语言，SWIG通过C预处理器的增强版本来预处理所有的入文件，所有标准的预处理特性都是支持的，包括文件包含，条件编译，和宏定义

**基本数据类型处理**

当一个整数从C语言传来时， 会有一些类型转换使它变为目标语言的格式。因此，16位的short可能被提升为32位整形。当整数太大时，它悄悄地被截断了。

有一个例外，就是unsigned char和signed char，他们被认为是8位的整形，是一个8位的ASCII码字符。当从脚本语言传递一个字符串给他时，他会自动街区第一个来作为这个字符。比如说，“foo”会被认为是字符’f’.。

如果目标语言没有bool类型，那么bool会被转化为0或者是1。

当使用大整数值的时候要特别注意，大部分脚本语言使用32位整数，所以64位整数可能会导致截断错误；同样的问题可能在unsigned int上发生（他可能会变为一个很大的负数）。

Int，char，short是很安全的，但是对于unsigned和long，你必须在SWIGwrap它提前检查

你源程序里的操作。

Char*类型被作为NULL终止的ASCII码字符串处理，SWIG在目标语言中把俺们映射为8位的字符串。SWIG在字符串送给C++之前，已经把它们作为NULL终止的字符串处理，所以传递二进制数据很不适合的。但是通过定义一个SWIG的typemaps就可以很好的解决这个问题。

**常量**

常量可以通过#define，枚举和%constant来定义，

注意define具有传递性

#define PI 3.1415

#define PI_4 PI/4

PI_4 也是一个常量！

注意，枚举类型一定在接口文件中有所体现才可以。

Char*的一点小提醒

当字符串从脚本语言传递到C++的char*时，这个指针通常指向一个解释器内部的区域，而修改这块区域是很不明智的行为。而且，一些语言是明令禁止这样做的，比方说python的string对象是不可变的。

问题的来源是一些可能改变字符串数据的函数，
char *strcat(char *s, const char *t)
SWIG会为他生成一个wrapper，但是其行为时未定义的，会导致你的程序崩溃，段错误或者是其他内存问题。原因就是s指向了目标语言的一块内部区域，而这些数据你是不该碰的。

最后一行：不要依赖char*，但是通过使用SWIG的typemaps可以改变这种行为。

**指针和复杂的对象**

绝大部分C程序都操纵数组，结构体和其他类型的对象，

**5.3.1简单指针**

Int*    char** double*** 这种指针全部被SWIG支持。SWIG没有把指针指向的对象变为脚本语言的格式，SWIG只是把指针转变为他的真实值。

SWIG处理指针是不透明的， 因此，一个指针可以从一个函数那里返回得到，也可以把它传递给另一个C函数。脚本语言的指针不能被直接操纵，虽然指针看上去是16进制的地址，而且，SWIG不把指针转换为更高级的对象，比如说数组和列表

派生类，结构体和类，对于他们来说，SWIG有一个非常简单的规则：所有对象都是指针，换句话说，SWIG通过引用来操纵object。很多C++程序大量使用指针，SWIG可以很好的处理指针与对象。

举个例子，
FILE *fopen(char *, char *);int fclose(FILE *);
SWIG并不知道FILE是什么东东，但是既然他是个指针，所以他是什么就不是哪么重要了，如果把这段代码包装成python，

F1 = open(“1.txt”,”r”)

Fclose(f1)

程序在并不知道FILE是啥的情况下依然能够工作正常。

**没有定义的类型**

当SWIG遇到一个不认识的类型时，他认为它是一个结构体或者是一个类，
void matrix_multiply(Matrix *a, Matrix *b, Matrix *c);
SWIG不知道Matrix是啥，但是很明显他是指向一个对象的指针，所以SWIG将他包装为一个指针。

不像C++，SWIG在使用一个变量之前不会去检查他是否被定义过，这个特性可以使SWIG通过有限的信息生成接口。

这样，所有没被声明的类型都被认为是指向结构体或者是类的指针，eg：
void foo(size_t num);
如果size_t没有定义的话，SWIG会把他当做size_t*,所以接口脚本有事会显得莫名其妙：
            foo(40);TypeError: expected a _p_size_t.
解决这个问题唯一的方法就是提前声明size_t

**Typedef**

C++用typedef来定义新的类型，typedef unsigned int size_t

Typedef必须在头文件或者是声明部分定义，SWIG会跟踪typedef的声明，时时刻刻根据这个信息来进行类型检查，
void foo(unsigned int *ptr);
他可以接受 size_t或者是unsigned int

**通过值来传递结构体**
double dot_product(Vector a, Vector b);
哪么SWIG又是怎么处理的呢？它使用指针把这个函数转化为下面格式：
double wrap_dot_product(Vector *a, Vector *b) {    Vector x = *a;    Vector y = *b;    return dot_product(x,y);}
目标语言用dot_product，现在他接受的是指针，而不是一个vector对象，大部分情况下，转变是透明的，所以你不一定知道。

**值返回**
Vector cross_product(Vector v1, Vector v2);
SWIG是这样做滴：
Vector *wrap_cross(Vector *v1, Vector *v2) {        Vector x = *v1;        Vector y = *v2;        Vector *result = new Vector(cross(x,y)); // Uses default copy constructor        return result;}
SWIG分配了一个新对象，返回这个对象的引用，这个对象不再使用时，用户可以将其删除。很明显，如果你没注意到隐式的内存分配，就会有内存泄露。

**结构体变量的链接**
类似于Vector unit_i;
这样的变量，都会被对应如下函数：
Vector *unit_i_get() {            return &unit_i;}void unit_i_set(Vector *value) {            unit_i = *value;}
Char*的链接
如果你有一个变量，类似于char *foo;
SWIG会生成如下代码：
void foo_set(char *value) {   if (foo) delete [] foo;   foo = new char[strlen(value)+1];   strcpy(foo,value);}
**数组**

SWIG完全支持数组，他也把数组当做指针。
int foobar(int a[40]);void grok(char *argv[]);void transpose(double a[20][20]);
会被当做
int foobar(int *a);void grok(char **argv);void transpose(double (*a)[20]);
可以注意到，SWIG也不做数组范围检查。

数组变量时提供的，但是缺省是只读状态。
int   a[100][200];读取变量a，会得到a[0][0]的地址，任何试图改变a的行为都会被认为是个错误，因为SWIG不知道怎么从目标语言拷贝值到数组。为了绕过这个限制，你可以写一个简单的帮助函数%inline %{void a_set(int i, int j, int val) {   a[i][j] = val;}int a_get(int i, int j) {   return a[i][j];}%}
字符串数组呗SWIG特殊处理，目标串可以在数组中储存，eg
char pathname[256];SWIG会为其生成如下几个函数char *pathname_get() {   return pathname;}void pathname_set(char *value) {   strncpy(pathname,value,256);}
**函数指针和回调函数**

SWIG支持函数指针，但是是C的函数而不是目标语言的函数。
如果有一个函数：int binary_op(int a, int b, int (*op)(int,int));
在python中 def add(a, b):return a + b

观察下binary_op会发生什么：应该是个错误吧，因为SWIG不知道怎么把python映射为C的回调函数。但是只要在接口文件中声明

%constant int add（int a, int b）;即可

注意，constant必不可少~~~

Unfortunately, by declaring the callback functions as constants, they are no longer accessible as functions. For example:

不幸的是，一旦声明回调函数是和常量，他再也不能作为函数被调用。

**结构体和联合变量**

当SWIG遇见结构体或者是联合，就会创建一些配套函数；如果没有构造函数和析构函数，他会在接口文件中创建他们。

注意要在接口文件中写上结构体的声明，并且python中结构体的成员可以任意扩张，比如说，在C函数中，node只有a,b,cc三个变量，但是python程序中你可以任意给一个对象加上任意变量名，只要你愿意！

**构造函数和析构函数**

对于创建和销毁物体有一个很好的机制是很有用的，SWIG会自动写一个函数如果没有的话。

**接口文件编写策略**

SWIG不需要你的代码做出任何改变，但是如果你给SWIG一大坨原始的C头文件和源代码，结果可能不是你想的那样好-事实上，这太可怕了！下面是编写C的接口文件遵循的步骤：
- 确定你想包装的函数。把所有函数都包装一般是不必要的，所以一些未雨绸缪可以简化接口，C头文件是寻找声明的好地方。 
- 为你的程序新建一个新的接口文件 
- 把那些生命拷贝文件里，或者是使用SWIG的 %include，  去包含整个头文件。 
- 明确所有的语法都是符合标准C++的 
- 明确所有typedef和其他类型信息在接口文件里都是可见的。另外，确保类型信息出现的次序是正确的。更为重要的是，在使用变量值钱，一定要定义它！ 
- 如果你的程序有main函数，记得给他改的名字吧 
- 运行SWIG，编译 

**关于main（）函数**

当你程序有main函数的时候，一定要删除或者重命名。绝大部分脚本语言都有自己的main函数程序。并且main在动态加载的时候也没啥用。
- 完全删除main函数 
- 重命名 
- 使用条件编译，只有不再使用脚本语言是才包含main函数。 

完全删除main函数会产生潜在的初始化问题，你可以考虑一个函数叫做progran_init()什么的，他可以在程序开始之前调用。

**注意：**

一些情况下，你会为一些脚本语言包装main函数，如果你这样做了，编译可能会进行，你的模块也可以成功加载。唯一的问题就是当你调用main函数时，你会发现，实际上它调用的是脚本语言自己的main函数！！！




