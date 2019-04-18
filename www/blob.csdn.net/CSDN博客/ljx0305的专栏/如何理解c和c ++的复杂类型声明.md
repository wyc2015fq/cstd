# 如何理解c和c ++的复杂类型声明 - ljx0305的专栏 - CSDN博客
2008年05月15日 19:59:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：512标签：[c																[pointers																[returning																[reference																[function																[parameters](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=returning&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
曾经碰到过让你迷惑不解、类似于int * (* (*fp1) (int) ) [10];这样的变量声明吗？本文将由易到难，一步一步教会你如何理解这种复杂的C/C++声明。
　　我们将从每天都能碰到的较简单的声明入手，然后逐步加入const修饰符和typedef，还有函数指针，最后介绍一个能够让你准确地理解任何C/C++声明的“右左法则”。
　　需要强调一下的是，复杂的C/C++声明并不是好的编程风格；我这里仅仅是教你如何去理解这些声明。注意：为了保证能够在同一行上显示代码和相关注释，本文最好在至少1024x768分辨率的显示器上阅读。
让我们从一个非常简单的例子开始，如下：
```
```
`int n;`
```
```
这个应该被理解为“declare n as an int”（n是一个int型的变量）。接下去来看一下指针变量，如下：
```
```
`int *p;`
```
```
这个应该被理解为“declare p as an int *”（p是一个int *型的变量），或者说p是一个指向一个int型变量的指针。我想在这里展开讨论一下：我觉得在声明一个指针（或引用）类型的变量时，最好将*（或 &）写在紧靠变量之前，而不是紧跟基本类型之后。这样可以避免一些理解上的误区，比如：再来看一个指针的指针的例子：
```
```
`char **argv;`
```
```
理论上，对于指针的级数没有限制，你可以定义一个浮点类型变量的指针的指针的指针的指针，再来看如下的声明：
```
```
```
int RollNum[30][4]; 
int (*p)[4]=RollNum; 
int *q[5];
```
```
```
这里，p被声明为一个指向一个4元素（int类型）数组的指针，而q被声明为一个包含5个元素（int类型的指针）的数组。另外，我们还可以在同一个声明中混合实用*和&，如下：
```
```
```
int **p1; 
// p1 is a pointer  to a pointer  to an int. 
int *&p2; 
// p2 is a reference to a pointer  to an int. 
int &*p3; 
// ERROR: Pointer  to a reference is illegal. 
int &&p4;
// ERROR: Reference to a reference is illegal.
```
```
```
注：p1是一个int类型的指针的指针；p2是一个int类型的指针的引用；p3是一个int类型引用的指针（不合法！）；p4是一个int类型引用的引用（不合法！）。
const修饰符
当你想阻止一个变量被改变，可能会用到const关键字。在你给一个变量加上const修饰符的同时，通常需要对它进行初始化，因为以后的任何时候你将没有机会再去改变它。例如：
```
```
```
const int n=5; 
int const m=10;
```
```
```
上述两个变量n和m其实是同一种类型的??都是const int（整形恒量）。因为C++标准规定，const关键字放在类型或变量名之前等价的。我个人更喜欢第一种声明方式，因为它更突出了const修饰符的作用。当const与指针一起使用时，容易让人感到迷惑。例如，我们来看一下下面的p和q的声明：
```
```
```
const int *p; 
int const *q;
```
```
```
他们当中哪一个代表const int类型的指针（const直接修饰int），哪一个代表int类型的const指针（const直接修饰指针）？实际上，p和q都被声明为const int类型的指针。而int类型的const指针应该这样声明： 
```
```
```
int * const r= &n;
// n has been declared as an int
```
```
```
这里，p和q都是指向const int类型的指针，也就是说，你在以后的程序里不能改变*p的值。而r是一个const指针，它在声明的时候被初始化指向变量n（即r=&n;）之后，r的值将不再允许被改变（但*r的值可以改变）。
组合上述两种const修饰的情况，我们来声明一个指向const int类型的const指针，如下：
```
```
```
const int * const p=&n 
// n has been declared as const int
```
```
```
下面给出的一些关于const的声明，将帮助你彻底理清const的用法。不过请注意，下面的一些声明是不能被编译通过的，因为他们需要在声明的同时进行初始化。为了简洁起见，我忽略了初始化部分；因为加入初始化代码的话，下面每个声明都将增加两行代码。
```
```
```
char ** p1; 
//    pointer to    pointer to    char 
const char **p2;
//    pointer to    pointer to const char 
char * const * p3;
//    pointer to const pointer to    char 
const char * const * p4;
//    pointer to const pointer to const char 
char ** const p5;
// const pointer to    pointer to    char 
const char ** const p6;
// const pointer to    pointer to const char 
char * const * const p7;
// const pointer to const pointer to    char 
const char * const * const p8;
// const pointer to const pointer to const char
```
```
```
注： p1是指向char类型的指针的指针；p2是指向const char类型的指针的指针；p3是指向char类型的const指针；p4是指向const char类型的const指针；p5是指向char类型的指针的const指针；p6是指向const char类型的指针的const指针；p7是指向char类型const指针的const指针；p8是指向const char类型的const指针的const指针。
typedef的妙用
typedef给你一种方式来克服“*只适合于变量而不适合于类型”的弊端。你可以如下使用typedef：
```
```
```
typedef char * PCHAR; 
PCHAR p,q;
```
```
```
这里的p和q都被声明为指针。（如果不使用typedef，q将被声明为一个char变量，这跟我们的第一眼感觉不太一致！）下面有一些使用typedef的声明，并且给出了解释：
```
```
```
typedef char * a;
// a is a pointer to a char 
typedef a b();
// b is a function that returns 
// a pointer to a char 
typedef b *c;
// c is a pointer to a function 
// that returns a pointer to a char 
typedef c d();
// d is a function returning 
// a pointer to a function 
// that returns a pointer to a char 
typedef d *e;
// e is a pointer to a function 
// returning a pointer to a 
// function that returns a 
// pointer to a char 
e var[10];
// var is an array of 10 pointers to 
// functions returning pointers to 
// functions returning pointers to chars.
```
```
```
typedef 经常用在一个结构声明之前，如下。这样，当创建结构变量的时候，允许你不使用关键字struct（在C中，创建结构变量时要求使用struct关键字，如 struct tagPOINT a；而在C++中，struct可以忽略，如tagPOINT b）。
```
```
```
typedef struct tagPOINT 
{ 
    int x; 
    int y; 
}POINT; 
POINT p; /* Valid C code */
```
```
```
函数指针
函数指针可能是最容易引起理解上的困惑的声明。函数指针在DOS时代写TSR程序时用得最多；在Win32和X-Windows时代，他们被用在需要回调函数的场合。当然，还有其它很多地方需要用到函数指针：虚函数表，STL中的一些模板，Win NT/2K/XP系统服务等。让我们来看一个函数指针的简单例子：
```
```
```
int (*p)(char);
```
```
```
这里p被声明为一个函数指针，这个函数带一个char类型的参数，并且有一个int类型的返回值。另外，带有两个float类型参数、返回值是char类型的指针的指针的函数指针可以声明如下：
```
```
```
char ** (*p)(float, float);
```
```
```
那么，带两个char类型的const指针参数、无返回值的函数指针又该如何声明呢？参考如下：
```
```
```
void * (*a[5])(char * const, char * const);
```
```
```
“右左法则”是一个简单的法则，但能让你准确理解所有的声明。这个法则运用如下：从最内部的括号开始阅读声明，向右看，然后向左看。当你碰到一个括号时就调转阅读的方向。括号内的所有内容都分析完毕就跳出括号的范围。这样继续，直到整个声明都被分析完毕。
对上述“右左法则”做一个小小的修正：当你第一次开始阅读声明的时候，你必须从变量名开始，而不是从最内部的括号。
下面结合例子来演示一下“右左法则”的使用。
```
```
```
int * (* (*fp1) (int) ) [10];
```
```
```
阅读步骤：
1. 从变量名开始fp1
2. 往右看，什么也没有，碰到了)，因此往左看，碰到一个*一个指针
3. 跳出括号，碰到了(int)一个带一个int参数的函数
4. 向左看，发现一个*（函数）返回一个指针
5. 跳出括号，向右看，碰到[10]一个10元素的数组
6. 向左看，发现一个*指针
7. 向左看，发现int int类型
总结：fp1被声明成为一个函数的指针,该函数返回指向指针数组的指针.
再来看一个例子：
```
```
```
int *( *( *arr[5])())();
```
```
```
阅读步骤：
1. 从变量名开始arr
2. 往右看，发现是一个数组一个5元素的数组
3. 向左看，发现一个*指针
4. 跳出括号，向右看，发现()不带参数的函数
5. 向左看，碰到*（函数）返回一个指针
6. 跳出括号，向右发现()不带参数的函数
7. 向左，发现*（函数）返回一个指针
8. 继续向左，发现int int类型
还有更多的例子：
```
```
```
float ( * ( *b()) [] )();
// b is a function that returns a 
// pointer to an array of pointers 
// to functions returning floats. 
void * ( *c) ( char, int (*)());
// c is a pointer to a function that takes 
// two parameters: 
// a char and a pointer to a 
// function that takes no 
// parameters and returns 
// an int 
// and returns a pointer to void. 
void ** (*d) (int &, 
char **(*)(char *, char **));
// d is a pointer to a function that takes 
// two parameters: 
// a reference to an int and a pointer 
// to a function that takes two parameters: 
// a pointer to a char and a pointer 
// to a pointer to a char 
// and returns a pointer to a pointer 
// to a char 
// and returns a pointer to a pointer to void 
float ( * ( * e[10]) 
  (int &) ) [5];
// e is an array of 10 pointers to 
// functions that take a single 
// reference to an int as an argument 
// and return pointers to 
// an array of 5 floats.
```
```
```
引用:http://oss.lzu.edu.cn/modules/newbb/viewtopic.php?viewmode=flat&topic_id=94&forum=13&type=
