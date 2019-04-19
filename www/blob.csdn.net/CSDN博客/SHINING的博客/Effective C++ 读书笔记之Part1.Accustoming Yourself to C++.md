# Effective C++ 读书笔记之Part1.Accustoming Yourself to C++ - SHINING的博客 - CSDN博客
2013年05月09日 16:04:15[snsn1984](https://me.csdn.net/snsn1984)阅读数：1428
1、View C++ as a federation of languages
C++的四个次语言：
1）C
2）Object-Oriented C++
3）Template C++
4）STL
2、Prefer consts, enums, and inlines to #defines
一方面是因为使用宏定义不利于调试的时候定位错误，另一方面主要是因为预处理器和编译器两者分工不同所导致的。
同时，宏定义太复杂的情况下很容易出现错误。
总结：
第一，对于单纯常量，最好以const对象或enums替换#defines.
第二，对于形似函数的宏（macros），最好改用inline函数替换#defines。
3、Use const whenever possible
const 语法虽然变化多端,但并不莫测高深。如果关键字const 出现在星号左
边,表示被指物是常量:如果出现在星号右边,表示指针自身是常量:如果出现在
星号两边,表示被指物和指针两者都是常量。

char greeting[] = "Hello";
char * p = greeting;                      //non-const pointer, non-const data
const char* p = greeting;             //non-const pointer, const data
char* const p = greeting;             //const pointer, non-const data
const char* const p = greeting;  //const pointer, const data
如果被指物是常量,有些程序员会将关键字const 写在类型之前,有些人会把
它写在类型之后、星号之前。两种写法的意义相同,所以下列两个函数接受的参数
类型是一样的：
void f1(const Widget* pw); 
void f2(Widget const * pw); 
两种形式都有人用,你应该试着习惯它们。
STL 选代器系以指针为根据塑模出来,所以迭代器的作用就像个?指针。声
明选代器为 const 就像声明指针为 const 一样(即声明一个T* const 指针) ,表
示这个迭代器不得指向不同的东西,但它所指的东西的值是可以改动的。如果你希
望迭代器所指的东西不可被改动(即希望STL 模拟一个 const T* 指针) ,你需要
的是 const _iterator:
std::vector<int> vec;
const std::vector<int>::iterator iter = vec.begin( );  //iter的作用就像个T* const
* iter = 10;                                                                    //没有问题，改变iter所指物
++iter;                                                                           //错误，iter是const
std: :vector<int>::const iterator clter = vec.begin( );   //cIter的作用像是个const T *
*clter = 10;                                                                       //错误，*cIter是const
++clter;                                                                             //没问题 
总结：
第一，某些东西声明为const 可帮助编译器侦测出错误用法。const 可被施加于任何作用域内的对象、函数参数、函数返回类型、成员函数本体。
第二，编译器强制实施 bitwise constness ,但你编写程序时应该使用"概念上的常量性"
(conceptual constness)。
第三，当 const 和 non-const 成员函数有着实质等价的实现时,令 non-const 版本调
用 const 版本可避免代码重复。
4、Make sure that objects are initialized before they're used
     所谓 static 对象,其寿命从被构造出来直到程序结束为止,因此 stack 和heap-based 对象都被排除。这种对象包括 global 对象、定义于 namespace 作用域内的对象、在 classes 内、在函数内、以及在 file 作用域内被声明为 static 的对象。函数内的 static 对象称为 local static 对象(因为它们对函数而言是 local) ,其他 static对象称为 non-local static 对象。程序结束时 static 对象会被自动销毁,也就是它们的析构函数会在main
 ()结束时被自动调用。
        所谓编译单元<translation unit)是指产出单一目标文件 (singleobject file) 的那些源码。基本上它是单一源码文件加上其所含入的头文件(#include files) 。
总结：
第一，内置型对象进行手工初始化,因为 C++不保证初始化它们。
第二，构造函数最好使用成员初值列 (member initialization list) ,而不要在构造函数本体内使用赋值操作(assignment) 。初值列列出的成员变量,其排列次序应该和它们在 class 中的声明次序相同。
第三，为免除"跨编译单元之初始化次序"问题,请以local static 对象替换 non-local static 对象。
感悟：
从这部分的内容大致可以看出整本书的风格，主要是在多种可以实现的情况下告诉你哪种情况更加合适，更加可以避免错误。从而让读者的水平从对C++的一般理解进入到更深的层次。所列的点都是平时容易遇到的。必须要有一定的C++实践经验才读出效果，否则看完了也记不住。跟实践经验很少的人直接去读设计模式是一个效果。

