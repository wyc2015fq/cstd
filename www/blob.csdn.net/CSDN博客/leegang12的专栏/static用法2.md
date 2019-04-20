# static用法2 - leegang12的专栏 - CSDN博客
2011年05月21日 18:35:00[leegang12](https://me.csdn.net/leegang12)阅读数：434标签：[存储																[output																[多线程																[reference																[object																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[VC++语言](https://blog.csdn.net/leegang12/article/category/824576)
**static用法**
原文链接：[http://leader20.blog.163.com/blog/static/619861200753115237171/](http://leader20.blog.163.com/blog/static/619861200753115237171/)
static关键字是C, C++中都存在的关键字, 它主要有三种使用方式, 其中前两种只指在C语言中使用, 第三种在C++中使用(C,C++中具体细微操作不尽相同, 本文以C++为准).
(1)局部静态变量
(2)外部静态变量/函数
(3)静态数据成员/成员函数
下面就这三种使用方式及注意事项分别说明
一、局部静态变量
在C/C++中, 局部变量按照存储形式可分为三种auto, static, register
(<C语言程序设计(第二版)>谭浩强, 第174-175页)
与auto类型(普通)局部变量相比, static局部变量有三点不同
1. 存储空间分配不同
auto类型分配在栈上, 属于动态存储类别, 占动态存储区空间, 函数调用结束后自动释放, 而static分配在静态存储区, 在程序整个运行期间都不释放. 两者之间的作用域相同, 但生存期不同.
2. static局部变量在所处模块在初次运行时进行初始化工作, 且只操作一次
3. 对于局部静态变量, 如果不赋初值, 编译期会自动赋初值0或空字符, 而auto类型的初值是不确定的. (对于C++中的class对象例外, class的对象实例如果不初始化, 则会自动调用默认构造函数, 不管是否是static类型)
特点: static局部变量的”记忆性”与生存期的”全局性”
所谓”记忆性”是指在两次函数调用时, 在第二次调用进入时, 能保持第一次调用退出时的值. 
示例程序一
#include <iostream>
using namespace std;
void staticLocalVar()
{
static int a = 0; // 运行期时初始化一次, 下次再调用时, 不进行初始化工作
cout<<"a="<<a<<endl;
++a;
}
int main()
{
staticLocalVar(); // 第一次调用, 输出a=0
staticLocalVar(); // 第二次调用, 记忆了第一次退出时的值, 输出a=1
return 0;
}
应用:
利用”记忆性”, 记录函数调用的次数(示例程序一)
     利用生存期的”全局性”, 改善”return a pointer / reference to a local object”的问题. Local object的问题在于退出函数, 生存期即结束,. 利用static的作用, 延长变量的生存期.
示例程序二:
// IP address to string format
// Used in Ethernet Frame and IP Header analysis
const char * IpToStr(UINT32 IpAddr)
{
static char strBuff[16]; // static局部变量, 用于返回地址有效
const unsigned char *pChIP = (const unsigned char *)&IpAddr;
sprintf(strBuff, "%u.%u.%u.%u",    pChIP[0], pChIP[1], pChIP[2], pChIP[3]);
return strBuff;
}
注意事项:
1. “记忆性”, 程序运行很重要的一点就是可重复性, 而static变量的”记忆性”破坏了这种可重复性, 造成不同时刻至运行的结果可能不同.
2. “生存期”全局性和唯一性. 普通的local变量的存储空间分配在stack上, 因此每次调用函数时, 分配的空间都可能不一样, 而static具有全局唯一性的特点, 每次调用时, 都指向同一块内存, 这就造成一个很重要的问题 ---- 不可重入性!!!
这样在多线程程序设计或递归程序设计中, 要特别注意这个问题.
(不可重入性的例子可以参见<effective C++ (2nd)>(影印版)第103-105页)
下面针对示例程序二, 分析在多线程情况下的不安全性.(为方便描述, 标上行号)
① const char * IpToStr(UINT32 IpAddr)
② {
③    static char strBuff[16]; // static局部变量, 用于返回地址有效
④    const unsigned char *pChIP = (const unsigned char *)&IpAddr;
⑤    sprintf(strBuff, "%u.%u.%u.%u",    pChIP[0], pChIP[1], pChIP[2], pChIP[3]);
⑥    return strBuff;
⑦ }
假设现在有两个线程A,B运行期间都需要调用IpToStr()函数, 将32位的IP地址转换成点分10进制的字符串形式. 现A先获得执行机会, 执行IpToStr(), 传入的参数是0x0B090A0A, 顺序执行完应该返回的指针存储区内容是:”10.10.9.11”, 现执行到⑥时, 失去执行权, 调度到B线程执行, B线程传入的参数是0xA8A8A8C0, 执行至⑦, 静态存储区的内容是192.168.168.168. 当再调度到A执行时, 从⑥继续执行, 由于strBuff的全局唯一性, 内容已经被B线程冲掉, 此时返回的将是192.168.168.168字符串, 不再是10.10.9.11字符串.
二、外部静态变量／函数
在C中static有了第二种含义：用来表示不能被其它文件访问的全局变量和函数。, 但为了限制全局变量/函数的作用域, 函数或变量前加static使得函数成为静态函数。但此处“static”的含义不是指存储方式，而是指对函数的作用域仅局限于本文件(所以又称内部函数)。注意此时, 对于外部(全局)变量, 不论是否有static限制, 它的存储区域都是在静态存储区, 生存期都是全局的. 此时的static只是起作用域限制作用, 限定作用域在本模块(文件)内部.
使用内部函数的好处是：不同的人编写不同的函数时，不用担心自己定义的函数，是否会与其它文件中的函数同名。
示例程序三:
//file1.cpp
static int varA;
int varB;
extern void funA()
{
……
}
static void funB()
{
……
}
//file2.cpp
extern int varB; // 使用file1.cpp中定义的全局变量
extern int varA; // 错误! varA是static类型, 无法在其他文件中使用
extern vod funA(); // 使用file1.cpp中定义的函数
extern void funB(); // 错误! 无法使用file1.cpp文件中static函
C语言中用static关键字可以定义静态变量，亦可以定义静态函数。
一、用static定义静态变量
定义静态变量时，又可以分为定义静态局部变量和定义静态外部变量。静态变量的内存空间在编译时即被申请，直到程序结束时才被释放。如果在定义时没有被初始化，则自动初始化为0。静态变量仅会被初始化一次。
例1（定义静态局部变量）（来源：C Primer Plus,Fifth Edition）：
```
/* loc_stat.c -- using a local static variable */
#include <stdio.h>
void trystat(void);
int main(void)
{
    int count;
    for (count = 1; count <= 3; count++)
    {
        printf("Here comes iteration %d:/n", count);
         trystat();
    }
    return 0;
}
void trystat(void)
{
    int fade = 1;
    static int stay = 1;
    printf("fade = %d and stay = %d/n", fade++, stay++);
}
```
注意trystat()函数每次打印输出后增加每个变量的值。下面是运行结果：
Here comes iteration 1: 
fade = 1 and stay = 1 
Here comes iteration 2: 
fade = 1 and stay = 2 
Here comes iteration 3: 
fade = 1 and stay = 3
可见fade（int型）在每次调用trystat()时都要初始化一次，而stay（static int型）仅初始化一次。实际上，fade（int型）在每次调用trystat()时都经历定义->申请内存空间->初始化->释放内存，而stay（static int型）则仅在编译时申请内存空间然后初始化，程序执行过程中并不执行申请内存等过程，而其内存空间的释放要等到程序结束时才做。
例2（定义静态外部变量）：
```
/* This is a multiple-file program */
/* text1.c */
#include <stdio.h>
int dog = 8;
static int cat;
void output(void);
int main()
{
    printf("dog = %d cat = %d/n", dog, cat);
     output();
    return 0;
}
/* text2.c */
extern int dog;
extern int cat;
void output(void)
{
    printf("There is %d dog(s)./n", dog);
    printf("There is %d cat(s)./n", cat);
}
```
当编译后链接以上程序时会出现错误，原因就是static int cat限定了cat的使用范围是在text1.c内，即static定义的外部变量作用范围仅限于本文件内。去掉static int cat中的static关键字即可。从本程序也可以看出static定义的变量默认初始化为0。
以下摘自《C程序设计（第二版）》潭浩强 著：
static对局部变量和全局变量的作用不同。对局部变量来说，它使变量由动态存储方式改变为静态存储方式。而对全局变量来说，它使变量局部化（局部于本文件），但仍为静态存储方式。从作用域角度看，凡有static声明的，其作用域都是局限的，或者是局部于本函数内（静态局部变量），或者局限于本文件内（静态外部变量）。
二、用static定义静态函数
定义函数时在函数类型前加static关键字，刚该函数为静态函数。如：
`static int fun(int i, int j)`
静态函数的作用域仅限于所在文件，类似于静态外部变量。如果在多文件程序中不同的文件内存在同名的函数，则会出错。解决办法就是把这些函数定义为静态函数，使不同文件中的同名函数互不干扰。
