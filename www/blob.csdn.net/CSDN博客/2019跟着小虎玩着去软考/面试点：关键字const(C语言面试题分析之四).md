# 面试点：关键字const(C语言面试题分析之四) - 2019跟着小虎玩着去软考 - CSDN博客
2011年05月23日 18:46:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2552标签：[面试																[语言																[string																[c																[编译器																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=面试&t=blog)
个人分类：[C/C++](https://blog.csdn.net/littletigerat/article/category/666612)
**面试点：关键字****const**
**C****语言面试题分析之四**
# 一. const声明的变量只能被读
const int i=5;
int j=0;
...
i=j;//非法，导致编译错误
j=i;//合法
# 二．必须初始化
const int i=5;//合法
const int j;//非法，导致编译错误
# 三．如何在另一.c源文件中引用const常量
extern const int i;//合法
extern const int j=10;//非法，常量不可以被再次赋值
# 四. 可以进行类型检查
用const方法可以使编译器对处理内容有更多了解。
#define I=10
const long &i=10;/*说明：由于编译器的优化，使
得在const long i=10时，i不被分配内存，而是已10直接代入
以后的引用中，以致在以后的代码中没有错误，为达到说教效
果，特别地用&i明确地给出了i的内存分配。不过一旦你关闭所
有优化措施，即使const long i=10也会引起后面的编译错误。*/
char h=I;//没有错
char h=i;//编译警告，可能由于数的截短带来错误赋值。
# 五．可以避免不必要的内存分配
#define STRING "abcdefghijklmn/n"
const char string[]="abcdefghijklm/n";
...
printf(STRING);//为STRING分配了第一次内存
printf(string);//为string一次分配了内存，以后不再分配
...
printf(STRING);//为STRING分配了第二次内存
printf(string);
... 
由于const定义常量从汇编的角度来看，只是给出了对应的内存地址，
而不是象#define一样给出的是立即数，所以，const定义的常量在
程序运行过程中只有一份拷贝，而#define定义的常量在内存中有
若干个拷贝。
# 六．可以通过函数对常量进行初始化
int value(); 
const int i=value();
假定对ROM编写程序时，由于目标代码的不可改写，本语句将会无效，不过可以变通一下：
const int &i=value();
只要令i的地址处于ROM之外，即可实现：i通过函数初始化，而其
值有不会被修改。
# 七．是不是const的常量值一定不可以被修改呢？
观察以下一段代码：
const int i=0;
int *p=(int*)&i;
*p=100;
通过强制类型转换，将地址赋给变量，再作修改即可以改变const常量值。
# 八．如何分清数值常量和指针常量：
int ii=0;
const int i=0;//i是常量，i的值不会被修改
const int *p1i=&i;//指针p1i所指内容是常量，可以不初始化
int* const p2i=ⅈ//指针p2i是常量，所指内容可修改
const int * const p3i=&i; //指针p3i是常量，所指内容也是常量
p1i=ⅈ//合法
*p2i=100;//合法
