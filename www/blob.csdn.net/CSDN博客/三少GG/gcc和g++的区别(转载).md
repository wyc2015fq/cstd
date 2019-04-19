# gcc和g++的区别(转载)  - 三少GG - CSDN博客
2010年05月10日 00:45:00[三少GG](https://me.csdn.net/scut1135)阅读数：973
[http://user.qzone.qq.com/272508369/blog/1253588249](http://user.qzone.qq.com/272508369/blog/1253588249)
我们在编译c/c++代码的时候，有人用gcc，有人用g++，于是各种说法都来了，譬如c代码用gcc，而c++代码用g++，或者说编译用gcc，链接用g++，一时也不知哪个说法正确，如果再遇上个extern "C"，分歧就更多了，这里我想作个了结，**毕竟知识的目的是令人更清醒，而不是更糊涂。**
误区一:gcc只能编译c代码,g++只能编译c++代码
两者都可以，但是请注意：
1.**后缀为.c的，gcc把它当作是C程序，而g++当作是c++程序；后缀为.cpp的，两者都会认为是c++程序**，注意，虽然c++是c的超集，但是两者对语法的要求是有区别的，例如：
#include <stdio.h>
int main(int argc, char* argv[]) {
   if(argv == 0) return;
   printString(argv);
   return;
}
int printString(char* string) {
sprintf(string, "This is a test./n");
}
如果按照C的语法规则，OK，没问题，但是，一旦把后缀改为cpp，立刻报三个错：“printString未定义”；
“cannot convert `char**' to `char*”；
”return-statement with no value“；
分别对应前面红色标注的部分。可见C++的语法规则更加严谨一些。
2.编译阶段，g++会调用gcc，对于c++代码，两者是等价的，但是因为gcc命令不能自动和C＋＋程序使用的库联接，所以通常用g++来完成链接，为了统一起见，干脆编译/链接统统用g++了，这就给人一种错觉，好像cpp程序只能用g++似的。
误区二:gcc不会定义__cplusplus宏，而g++会
实际上，这个宏只是标志着编译器将会把代码按C还是C++语法来解释，如上所述，如果后缀为.c，并且采用gcc编译器，则该宏就是未定义的，否则，就是已定义。
误区三:编译只能用gcc，链接只能用g++
严格来说，这句话不算错误，但是它混淆了概念，应该这样说：编译可以用gcc/g++，**而链接可以用g++或者gcc -lstdc++**。因为gcc命令不能自动和C＋＋程序使用的库联接，所以通常使用g++来完成联接。但在编译阶段，g++会自动调用gcc，二者等价。
误区四:extern "C"与gcc/g++有关系
实际上并无关系，无论是gcc还是g++，用extern "c"时，都是以C的命名方式来为symbol命名，否则，都以c++方式命名。试验如下：
me.h：
extern "C" void CppPrintf(void);
me.cpp:
#include <iostream>
#include "me.h"
using namespace std;
void CppPrintf(void)
{
     cout << "Hello/n";
}
test.cpp:
#include <stdlib.h>
#include <stdio.h>
#include "me.h"        
int main(void)
{
    CppPrintf();
    return 0;
}
1. 先给me.h加上extern "C"，看用gcc和g++命名有什么不同
[root@root G++]# g++ -S me.cpp
[root@root G++]# less me.s
.globl _Z9CppPrintfv        //注意此函数的命名
        .type   CppPrintf, @function
[root@root GCC]# gcc -S me.cpp
[root@root GCC]# less me.s
.globl _Z9CppPrintfv        //注意此函数的命名
        .type   CppPrintf, @function
完全相同！
2. 去掉me.h中extern "C"，看用gcc和g++命名有什么不同
[root@root GCC]# gcc -S me.cpp
[root@root GCC]# less me.s
.globl _Z9CppPrintfv        //注意此函数的命名
        .type   _Z9CppPrintfv, @function
[root@root G++]# g++ -S me.cpp
[root@root G++]# less me.s
.globl _Z9CppPrintfv        //注意此函数的命名
        .type   _Z9CppPrintfv, @function
完全相同！
【结论】完全相同，可见extern "C"与采用gcc/g++并无关系，以上的试验还间接的印证了前面的说法：在编译阶段，g++是调用gcc的。
============**例子**：
[http://hi.baidu.com/mgqw/blog/item/1d65dcdfd6db69104854035f.html](http://hi.baidu.com/mgqw/blog/item/1d65dcdfd6db69104854035f.html)
gcc和g++到底有什么区别？
2007-11-12 09:33
今天编译一个很普通的C++程序，如下： 
```
1  #include 《iostream》 
2 
3  void function1(){ 
4     for(int i=0;i<1000000;i++); 
5  } 
6 
7  void function2(){ 
8     function1(); 
9     for (int i=0;i<2000000;i++); 
10 } 
11 
12 void function3(){ 
13    function1(); 
14    function2(); 
15    for (int i=0;i<3000000;i++); 
16       function1(); 
17 } 
18 
19 int main(){ 
20    for(int i=0;i<10;i++) 
21    function1(); 
22 
23    for (int i=0;i<5000000;i++); 
24 
25    for(int i=0;i<10;i++) 
26       function2(); 
27       for(int i=0; i<13;i++); 
28          { 
29          function3(); 
30          function2(); 
31          function1(); 
32          } 
33 }
```
编译命令行： gcc -pg -o sample3 sample3.cpp
结果是：
/tmp/cck9wDpZ.o: In function `__static_initialization_and_destruction_0(int, int)':
sample3.cpp:(.text+0x12c): undefined reference to `std::ios_base::Init::Init()'
/tmp/cck9wDpZ.o: In function `__tcf_0':
sample3.cpp:(.text+0x183): undefined reference to `std::ios_base::Init::~Init()'
/tmp/cck9wDpZ.o:(.eh_frame+0x11): undefined reference to `__gxx_personality_v0'
collect2: ld returned 1 exit status
以前因为工作内容的关系，没用gcc编译过c++的代码。或者是其他同事已经写好了Makefile，没怎么关心过这个问题。不过以前和一个同事争论过，他说c++的代码必须要用g++编译，否则编译不过去，因为gcc是编译c code的，而g++才是编译c++的。我当时就知道肯定不对，因为gcc是gnu compiler collection的缩写，而不是gnu C compiler。理论上所有的gcc支持的不同语言的代码都可以用gcc编译（如果我错了请纠正我）。当时没空争论这个问题，结果今天自己碰上了。google了一下，又试了试，确实用g＋＋可以编译过去 
g++ -pg -o sample3 sample3.cpp 不服不服，继续尝试发现用gcc也可以，不过要加上指定的标准c++库 如下： 
**gcc -pg -o sample3 sample3.cpp -lstdc++** 所以，gcc是可以编译c++ 代码的，g++不过是隐含链接某些库
**本人亲身体会：文件后缀名真的很重要！**
我的一个C++程序命名为：mgqw.c 用g++ mgqw.c -o mgqw编译没有问题，
而如果使用：gcc mgqw.c -o mgqw -lstdc++的话则会报类似下面的错误：
mgqw.c:68: 错误：expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘*’ token
mgqw.c: 在函数 ‘main’ 中：
mgqw.c:909: 错误：expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘*’ token
mgqw.c: 在函数 ‘SwitchGame’ 中：
mgqw.c:1351: 错误：expected expression before ‘static’
mgqw.c:1352: 错误：‘cjpsw’ 未声明 (在此函数内第一次使用)
mgqw.c:1352: 错误：(即使在一个函数内多次出现，每个未声明的标识符在其
mgqw.c:1352: 错误：所在的函数内只报告一次。)
mgqw.c:1358: 错误：expected expression before ‘static’
mgqw.c:1359: 错误：‘zjhsw’ 未声明 (在此函数内第一次使用)
mgqw.c:1365: 错误：expected expression before ‘static’
mgqw.c:1366: 错误：‘jjzxsw’ 未声明 (在此函数内第一次使用)
mgqw.c:1372: 错误：expected expression before ‘static’
mgqw.c:1373: 错误：‘hjcdsw’ 未声明 (在此函数内第一次使用)
mgqw.c:1379: 错误：expected expression before ‘static’
mgqw.c:1380: 错误：‘bbcysw’ 未声明 (在此函数内第一次使用)
mgqw.c:1386: 错误：expected expression before ‘static’
mgqw.c:1387: 错误：‘kymxsw’ 未声明 (在此函数内第一次使用)
mgqw.c:1392: 错误：expected expression before ‘static’
mgqw.c:1393: 错误：‘dqdzsw’ 未声明 (在此函数内第一次使用)
将mgqw.c文件改名为mgqw.cc或者mgqw.cpp(表示是c++文件)后再使用命令：
gcc mgqw.cc -o mgqw -lstdc++
程序编译成功！花了我大半天时间才弄明白这个问题，晕死。
