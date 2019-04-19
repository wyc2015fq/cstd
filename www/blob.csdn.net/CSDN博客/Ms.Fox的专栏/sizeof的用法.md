# sizeof的用法 - Ms.Fox的专栏 - CSDN博客
2014年10月21日 19:44:38[lulu-lu](https://me.csdn.net/smbluesky)阅读数：370
个人分类：[C++：注意事项](https://blog.csdn.net/smbluesky/article/category/2641013)
## 1、sizeof的用法
a、针对对象
sizeof（object）；或者sizeof  object；
b、针对类型
sizeof（typename）；
## 2、内置类型sizeof运算结果
|数据类型|sizeof|
|----|----|
|bool|1|
|char|1|
|short|2|
|int|4|
|long|4|
|float|4|
|double|8|
|long double|8|
**signed和unsigned的sizeof结果相同。**
函数类型和指针类型都属于指针的范畴。指针主要用于存储地址，在32位系统下，任何类型的指针其所占用的
内存大小均为4字节；而函数类型，它以其返回类型作为自身类型进行sizeof取值。
double f2()
{
return 1.0;
}
sizeof（f2()）=8;
sizeof(int *)=4;
# **3、数组和char**
char a[]="abcdef";     //如果字符数组表示字符串的话，数组末尾自动插入‘\0’
char b[]={'a','b','c','d','e','f'};
int c[20]={3,4};
char d[2][3]={"aa","bb"};
sizeof(a)=7;sizeof(b)=6;sizeof(c)=80;sizeof(d)=2*3=6;
sizeof(*a)=1;sizeof(*b)=1;sizeof(*c)=4;sizeof(*d)=3;
# 4、struct数据结构的大小问题
对于struct数据结构，由于CPU的对齐问题导致struct的大小变得特别复杂：
struct tagPersonInfoA 
{
char * pszName;
int nSex;
double nHeight;
};
struct tagPersonInfoB
{
char * pszName;
        double nHeight;
int nSex;
};
sizeof（tagPersonInfoA ）=16
sizeof（tagPersonInfoB）=24
同样的存储嘻嘻，但由于数据的排列顺序不同，最终占用的空间大小也不同，这就是CPU的对齐问题。
对一个空的struct结构体取sizeof，结果为1。这是编译器**为了保证此空struct的存在而专门分配的一字节**。
**不含继承和static成员变量的类**，和struct复合类型一样，空的class同样也占用1字节；
采用对齐来计算对象的大小；
计算类对象的大小时，类的成员函数不占用对象空间。
在单继承情况下，只要class中存在virtual函数，在编译时编译器就会**自动插入一个指向虚函数表的指针vptr**（大小为4字节）。
VC编译器插入vptr的位置一般从数据成员开始。
**而static成员是分配在全局区为类的所有对象共享，sizeof 时，不应该计入static成员。**
