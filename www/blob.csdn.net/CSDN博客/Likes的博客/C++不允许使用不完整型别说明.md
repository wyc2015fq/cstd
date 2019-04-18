# C++不允许使用不完整型别说明 - Likes的博客 - CSDN博客
2018年11月01日 11:48:29[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：100
[https://blog.csdn.net/qq_36038987/article/details/77747507?locationNum=5&fps=1](https://blog.csdn.net/qq_36038987/article/details/77747507?locationNum=5&fps=1)
在讲述柔性数组成员之前，首先要介绍一下不完整类型（incomplete type）。不完整类型是这样一种类型，它缺乏足够的信息例如长度去描述一个完整的对象。
incomplete types (types that describe objects but lack information needed to determine their sizes).
C与C++关于不完整类型的语义是一样的。
基本上没有什么书介绍过不完整类型，很多人初次遇到这个概念时脑袋会一片空白。事实上我们在实际的工程设计中经常使用不完整类型，只不过不知道有这么个概念而已。前向声明就是一种常用的不完整类型：
```cpp
class base;
struct test;
```
base和test只给出了声明，没有给出定义。不完整类型必须通过某种方式补充完整，才能使用它们进行实例化，否则只能用于定义指针或引用，因为此时实例化的是指针或引用本身，不是base或test对象。
一个未知长度的数组也属于不完整类型：
```cpp
extern int a[];
```
extern不能去掉，因为数组的长度未知，不能作为定义出现。不完整类型的数组可以通过几种方式补充完整才能使用，大括号形式的初始化就是其中一种方式：
```cpp
int a[] = { 10, 20 };
```
柔性数组成员（flexible array member）也叫伸缩性数组成员，它的出现反映了C程序员对精炼代码的极致追求。这种代码结构产生于对动态结构体的需求。在日常的编程中，有时候需要在结构体中存放一个长度动态的字符串，一般的做法，是在结构体中定义一个指针成员，这个指针成员指向该字符串所在的动态内存空间，例如：
```cpp
struct test{
       int a;
       double b;
       char *p;
};
```
 p指向字符串。这种方法造成字符串与结构体是分离的，不利于操作，如果把字符串跟结构体直接连在一起，不是更好吗？于是，可以把代码修改为这样：
char a[] = “hello world”;
struct test *PntTest = ( struct test* )malloc( sizeof( struct test ) + strlen( a ) + 1 );
strcpy( PntTest + 1, a );
这样一来，( char* )( PntTest + 1 )就是字符串“hello world”的地址了。这时候p成了多余的东西，可以去掉。但是，又产生了另外一个问题：老是使用( char* )( PntTest + 1 )不方便。如果能够找出一种方法，既能直接引用该字符串，又不占用结构体的空间，就完美了，符合这种条件的代码结构应该是一个非对象的符号地址，在结构体的尾部放置一个0长度的数组是一个绝妙的解决方案。不过，C/C++标准规定不能定义长度为0的数组，因此，有些编译器就把0长度的数组成员作为自己的非标准扩展，例如：
struct test
{
       int a;
       double b;
       char c[0];
};
c就叫柔性数组成员，如果把PntTest指向的动态分配内存看作一个整体，c就是一个长度可以动态变化的结构体成员，柔性一词来源于此。c的长度为0，因此它不占用test的空间，同时PntTest->c就是“hello world”的首地址，不需要再使用( char* )( PntTest + 1 )这么丑陋的语法了。 
鉴于这种代码结构所产生的重要作用，C99甚至把它收入了标准中：
6.7.2.1 Structure and union specifiers
As a special case, the last element of a structure with more than one named member may have an incomplete array type; this is called a flexible array member.
C99使用不完整类型实现柔性数组成员，标准形式是这样的： 
struct test
{     int a;
       double b;
       char c[];
};
 c同样不占用test的空间，只作为一个符号地址存在，而且必须是结构体的最后一个成员。柔性数组成员不仅可以用于字符数组，还可以是元素为其它类型的数组，例如： 
struct test
{
       int a;
       double b;
       float c[];
};
应当尽量使用标准形式，在非C99的场合，可以使用指针方法。有些人使用char a[1]，这是非常不可取的，把这样的a用作柔性数组成员会发生越界行为，虽然C/C++标准并没有规定编译器应当检查越界，但也没有规定不能检查越界，为了一个小小的指针空间而牺牲移植性，是不值得的。
C++错误--不允许使用不完整的类类型--     
在一个文件中定义了一个类CLASS A，在另一个文件中使用时说不允许使用不完整的类型。使用方法如下：
CLASS  A;
CLASS B{
public:
A *point；
}
分析问题：使用该类的文件看不到该类。
解决问题：将类的定义放到.h头文件中，在使用该类的文件中#include该头文件。
