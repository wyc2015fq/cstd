# ﻿﻿﻿﻿C字符串数组赋值 - maopig的专栏 - CSDN博客
2012年08月09日 21:38:59[maopig](https://me.csdn.net/maopig)阅读数：22974
### [C字符数组赋值](http://blog.csdn.net/jphaoren/article/details/5803583)
举例如下：
char a[10];
1、定义的时候直接用字符串赋值
char a[10]="hello";
注意：不能先定义再给它赋值，如
 char a[10];
 a[10]="hello";
这样是错误的！
2、对数组中字符逐个赋值
char a[10]={'h','e','l','l','o'};
3、利用strcpy
char a[10];
strcpy(a, "hello");
易错情况：
1、char a[10]; a[10]="hello";//一个字符怎么能容纳一个字符串？况且a[10]也是不存在的！
2、char a[10]; a="hello";//这种情况容易出现，a虽然是指针，但是它已经指向在堆栈中分配的10个字符空间，现在这个情况a又指向数据区中的hello常量，这里的指针a出现混乱，不允许！
还有：不能使用关系运算符“＝＝”来比较两个字符串，**只能用strcmp() 函数来处理。**
C语言的运算符根本无法操作字符串。在C语言中把字符串当作数组来处理，因此，对字符串的限制方式和对数组的一样，特别是，它们都不能用C语言的运算符进行复制和比较操作。直接尝试对字符串进行复制或比较操作会失败。例如，假定str1和str2有如下声明：
char str1[10], str2[10];
利用=运算符来把字符串复制到字符数组中是不可能的：
str1 = "abc";     /*** WRONG ***/
str2 = str1;       /*** WRONG ***/
C语言把这些语句解释为一个指针与另一个指针之间的（非法的）赋值运算。但是，使用=初始化字符数组是合法的：
char str1[10] = "abc";
这是因为在声明中，=不是赋值运算符。
试图使用关系运算符或判等运算符来比较字符串是合法的，但不会产生预期的结果：
if (str1==str2) ...    /*** WRONG ***/
这条语句把str1和str2作为指针来进行比较，而不是比较两个数组的内容。因为str1和str2有不同的地址，所以表达式str1 == str2的值一定为0。
strcpy和memcpy都是标准C库函数，它们有下面的特点。
strcpy提供了字符串的复制。即strcpy只用于字符串复制，并且它不仅复制字符串内容之外，还会复制字符串的结束符。
已知strcpy函数的原型是：char* strcpy(char* dest, const char* src);
memcpy提供了一般内存的复制。即memcpy对于需要复制的内容没有限制，因此用途更广。
**void *memcpy( void ****dest***, const void ****src***, size_t***count***);**
[?](http://www.cnblogs.com/stoneJin/archive/2011/09/16/2179248.html#)
```cpp
char
```
```cpp
*
```
```cpp
strcpy
```
```cpp
(
```
```cpp
char
```
```cpp
* dest,
```
```cpp
const
```
```cpp
char
```
```cpp
*
 src)
```
```cpp
// 实现src到dest的复制
```
```cpp
{
```
```cpp
```
```cpp
if
```
```cpp
((src == NULL) || (dest == NULL))
```
```cpp
//判断参数src和dest的有效性
```
```cpp
{
```
```cpp
```
```cpp
```
```cpp
return
```
```cpp
NULL;
```
```cpp
}
```
```cpp
```
```cpp
char
```
```cpp
*strdest = dest;
```
```cpp
//保存目标字符串的首地址
```
```cpp
```
```cpp
while
```
```cpp
((*strDest++ = *strSrc++)!=
```
```cpp
'\0'
```
```cpp
);
```
```cpp
//把src字符串的内容复制到dest下
```
```cpp
```
```cpp
return
```
```cpp
strdest;
```
```cpp
}
```
```cpp
void
```
```cpp
*
```
```cpp
memcpy
```
```cpp
(
```
```cpp
void
```
```cpp
*memTo,
```
```cpp
const
```
```cpp
void
```
```cpp
*memFrom,
```
```cpp
size_t
```
```cpp
size)
```
```cpp
{
```
```cpp
```
```cpp
if
```
```cpp
((memTo == NULL) || (memFrom == NULL))
```
```cpp
//memTo和memFrom必须有效
```
```cpp
```
```cpp
return
```
```cpp
NULL;
```
```cpp
```
```cpp
char
```
```cpp
*tempFrom = (
```
```cpp
char
```
```cpp
*)memFrom;
```
```cpp
//保存memFrom首地址
```
```cpp
```
```cpp
char
```
```cpp
*tempTo = (
```
```cpp
char
```
```cpp
*)memTo;
```
```cpp
//保存memTo首地址
```
```cpp
```
```cpp
while
```
```cpp
(size -- > 0)
```
```cpp
//循环size次，复制memFrom的值到memTo中
```
```cpp
```
```cpp
*tempTo++ = *tempFrom++ ;
```
```cpp
```
```cpp
return
```
```cpp
memTo;
```
```cpp
}
```
strcpy和memcpy主要有以下3方面的区别。
1、复制的内容不同。strcpy只能复制字符串，而memcpy可以复制任意内容，例如字符数组、整型、结构体、类等。
2、复制的方法不同。strcpy不需要指定长度，它遇到被复制字符的串结束符"\0"才结束，所以容易溢出。memcpy则是根据其第3个参数决定复制的长度。
3、用途不同。通常在复制字符串时用strcpy，而需要复制其他类型数据时则一般用memcpy
**memset**原型：extern void *memset(void *buffer, int c, int count);
用法：#include <string.h>
功能：把buffer所指内存区域的前count个字节设置成字符c。
说明：返回指向buffer的指针。用来对一段内存空间全部设置为某个字符。
举例：char a[100];memset(a, '\0', sizeof(a));
**memset可以方便的清空一个结构类型的变量或数组。**
如：
struct sample_struct
{
char   csName[16];
int   iSeq;
int   iType;
};
对于变量
struct sample_strcut stTest;
一般情况下，清空stTest的方法：
stTest.csName[0]='\0';
stTest.iSeq=0;
stTest.iType=0;
用memset就非常方便：
memset(&stTest,0,sizeof(struct sample_struct));
如果是数组：
struct sample_struct   TEST[10];
则
memset(TEST,0,sizeof(struct sample_struct)*10);
对这个问题有疑问，不是对函数的疑问，而是因为没有弄懂mem和str的区别。
mem是一段内存，他的长度，必须你自己记住
str也是一段内存，不过它的长度，你不用记，随时都可以计算出来
所以memcpy需要第三个参数，而strcpy不需要
(一)strcmp函数   
         strcmp函数是比较两个字符串的大小,返回比较的结果。一般形式是：  
                 i=strcmp(字符串，字符串);
         其中，字符串１、字符串２均可为字符串常量或变量；i   是用于存放比较结果的整型变量。比较结果是这样规定的：  
①字符串1小于字符串2,strcmp函数返回一个负值;
②字符串1等于字符串2,strcmp函数返回零;
③字符串1大于字符串2,strcmp函数返回一个正值;那么,字符中的大小是如何比较的呢?来看一个例子。
         实际上,字符串的比较是比较字符串中各对字符的ASCII码。首先比较两个串的第一个字符，若不相等，则停止比较并得出大于或小于的结果；如果相等就接着 比较第二个字符然后第三个字符等等。如果两上字符串前面的字符一直相等,像"disk"和"disks"   那样,   前四个字符都一样,   然后比较第 五个字符,   前一个字符串"disk"只剩下结束符'/0',后一个字符串"disks"剩下's','/0'的ASCII码小于's'的ASCII 码,所以得出了结果。因此无论两个字符串是什么样，strcmp函数最多比较到其中一个字符串遇到结束符'/0'为止，就能得出结果。
**注意:字符串是数组类型而非简单类型，不能用关系运算进行大小比较。  **
         if("ABC">"DEF")   /*错误的字符串比较*/
 if(strcmp("ABC","DEF")   /*正确的字符串比较*/
（二）strcpy函数  
         strcpy函数用于实现两个字符串的拷贝。一般形式是：  
                 strcpy（字符中1,字符串2)
         其中,字符串1必须是字符串变量,而不能是字符串常量。strcpy函数把字符串2的内容完全复制到字符串1中,而不管字符串1中原先存放的是什么。复制后，字符串2保持不变。  
例:    
   注意,由于字符串是数组类型,所以两个字符串复制不通过赋值运算进行。  
         t=s;   /*错误的字符串复制*/
         strcpy(t,s);   /*正确的字符串复制*/
昨天在用结构体指针给结构体赋初值的时候，犯了一个错误（main函数中被注释掉的那一句话）。而应该采用strcpy的方式把一个字符串放入字符数组中去，对此我将沿着我探讨这个问题根源的思路做一个分享
#include <stdio.h>
#include <string.h>
#include <malloc.h>
typedef struct student_info
{
       int number;
       char name[32];
}stu, *p_student;
int
main()
{
       stu a;                                  
 //注意要定义一个结构体出来
       p_student p = NULL;
       p = &a;
       p->number = 1;
//p->name = “aa”;  //错误
   strcpy(p->name, “aa”);
//正确
       printf("%d:%s\n", p->number, a.name);
       return 0;
}
在发现这个问题之后，我进行了如下的几个尝试：指针对这个数组成员不能赋值，那直接用结构体变量是否可以？（不行）；采用普通方式，对结构体变量a在定义的时候初始化，是否可行？（可行）
那么最终问题在于数组初始化和赋值的方面，对此进一步采用了几个程序，对该知识点进行了深入的理解，程序如下
#include <stdio.h>
#include <string.h>
int
main()
{
       char a[] = "abcdef";
       char *p = "abcdef";
       char *p_a = NULL;
       //p_a = a;
       //fun(&p_a);
       //a[0] = 'z';
       //a = "z";
       //a = 'z';
       strcpy(a, "zf");
       //p = "z";
       *p = 'z';
       printf("%s\n", a);
       printf("%s\n", p);
       //printf("%s\n", p_a);
       return 0;
}
int
fun(char **ar)
{
       //char **p_a = &ar;
       *ar = "zef";
       //ar[0] = 'z';
//     strcpy(ar, "z");
       return 1;
}
以上是我在做知识点归纳时用的源程序，比较混乱，敬请了解。一下我将解释一下，我到底做了那几个方面的工作和讨论
由于关系到字符数组的赋值问题，我想到了这个例子，实验了一下区别
把两个相同的字符串分别赋给一个字符数组，和字符型指针。结果：均可以打印出来
那么它们能否再被赋值修改呢？
对于字符数组而已只能通过a[0] = 'z';或者strcpy(a, "zf");这样的方式对其进行修改，总结的时候再归纳原因
对于字符型指针，p = "z";可以，但是注意这里的实质却并没有对p字符串**进行修改**。**而*p = 'z';是不可行的**
归纳总结一：
       参考网络知识和《c和指针》中关于字符数组的初始化部分
（注意：需要提前搞清楚，什么是赋值，什么是初始化，什么是定义。相关知识可以参考网络资料，eg：int a;(定义)，int a = 10;(初始化)，a = 10;(赋值)）
*1*对于字符数组：
       char a[15] = “abcdef”;
但不能做如下操作
       char a[15];
       a[15] = “abcdef”;
特别注意：第一种初始化的方式，看似左值是个字符串，其实不然，它其实是个初始化列表。最后列表包含\0
因此，**字符数组是不能将字符串赋给它的！**
所以在后续的赋值里必须对数组元素进行赋值
*2*对于字符串
       char *p = “abcdef”;
注意这里不是把一个字符串赋给了一个字符型的指针，而是把一个字符型的指针指向了字符串的首地址。
       所以上面p = "z"只是把指针指向了另一个地方而已
*3*对于数组与字符串在机器中的存储
*数组*：
                     数组在机器中，当被定义时eg：char a[10];就已经分配好了内存空间，放在了数据段中，其中的数值是可以进行修改的
*字符串*：
**字符串在内存中也被存放在了数据段中，但是字符串又称为字符串常量，是编译器“钉死”无法修改的，所以*p = 'z';想改变字符串的值是不被允许的**
之后我又进行了一些尝试，那就是将数组作为一个函数的参数进行传递，看变换，这个知识点，我不做操作上的过多复述。
归纳总结二：
       在此之前，我们首先得理解，数组名。数组名表示着这个数组的入口地址，这一点与函数名，结构体标识等类似。
       eg：char a[10];那么**a**表示的是这个数组第一个元素的地址，即&a[0];
**   而&a;表示的是这个数组的首地址**。
       估计不少人这个时候糊涂了，这两个有区别吗？数值不一样么？
它们两个的数值是一样的，因为地址只有一个，数组的一个元素的地址的值就是这个数组的地址的值，那么为什么还要分这么细致呢？下面举个例子
eg：
       char a[10];
       char *p = NULL;
       char (*p_a)[10] = NULL;
       p = a;
       p_a = &a;
如上面这个例子，a只能赋给一个char型的指针，而&a只能赋给一个指向一个数组的指针（注意这里的这个数组要与你定义的那个a数组同大小）
借助指针，我们不难理解，其实他们所代表的一个指针的类型是截然不同的（如果不能理解请参考c相关教程中指针的概念）
进入正文，所以当一个数组被传递如函数的时候可以把数组名传进去，也就是把这个数组的第一个元素的首地址传进去
作为指针被传进去的数组名这时和指针是完全一致的，可以作为指针使用，当然为直观化，可以用做数组
eg:
       char a[10];
       int fun(char *a)
       {
                     a[0] = ‘A’;
}
至于程序中出现的指针的指针只是在思考时做的讨论，上面的那么其实也可以看做普通指针，这里不做过多说明。
