# 浅析 C++ 的封装性 - 文章 - 伯乐在线
原文出处： [chen825919148](http://blog.csdn.net/chen825919148/article/details/7964953)
本专题，我们讲述封装性。封装性是C++的入门特性，要想学习C++语言，封装性是首先要掌握的。下面我们进入正题:
### 一、 类与对象
早在本系列第一节课（理解程序中的数据）的时候讲述过数据类型与数据的区别和联系当时得出的结论如下:
Ø 数据类型规定了数据的大小和表现形式
Ø 数据就是电脑中存放的数。
Ø 每个数据都有自己的地址，而这些地址可以有变量来代替
Ø 因此，数据通常存放于变量（或常量）中
这个结论在C++中仍然同样适用，类就是我们自己定义的复杂的数据类型，而对象则就是由类声明的变量。下面我们进入纯语法层面。
#### 1、 类的定义方法
我相信，大家都还记得我在第一节课的时候讲述的结构体的课程，也相信大家没有忘记怎么定义一个结构体。下面我给出类的定义方法:


```
class CExample // 是不是很像定义一个结构体
{
private: // 权限控制，相关内容在下面的小节中详细讲述
int m_nFirstNum; //定义成员变量。也叫属性
int m_nSecNum;
public:
int GetSum(){return m_nFirstNum} // 成员函数
bool SetNum(int nFirst,int nSec)
{
m_nFirstNum = nFirst;
m_nSecNum = nSec ;
return true;
}
CExample(){m_nFirstNum = 0; m_nSecNum = 0;} //构造函数
~CExample(){} // 空析构
};
```
当然，上面这个类的定义是不是很像定义一个结构体？只不过多了个private和public还有一些函数。是的，C++里面，将结构体升级了，结构体里面可以有函数成员了，为了兼容，换了个关键字，当然，上面的这个class完全可以改成struct，一点问题都没有。
好奇的朋友会问：如果函数体的语句太多，逻辑复杂了，函数很大，那这个类岂不是很难看，太臃肿了吧。
是的，为了方便类的组织，也为了协调项目工程中文件的组织。上面的类还可以写成如下的形式：


```
// .h文件中写如下的声明部分
class CExample   // 是不是很像定义一个结构体
{
private: // 权限控制，相关内容在下面的小节中详细讲述
       int   m_nFirstNum;  // 定义成员变量。也叫属性
       int   m_nSecNum;
public:
       int   GetSum(); // 成员函数
       bool  SetNum(int nFirst,int nSec);
       CExample();   //构造函数
       ~CExample();               // 空析构
};
// .cpp 文件中写如下的定义及实现部分
CExample::CExample()
{
}
CExample::~CExample()
{
}
 
int CExample::GetFirstNum()
{
    return m_nFirstNum;
}
 
int CExample::GetSecNum()
{
    return m_nSecNum;
}
 
bool CExample::SetNum(int nFirst, int nSec)
{
m_nFirstNum  =  nFirst; 
m_nSecNum  =  nSec ;
return true;
}
int CExample::GetSum()
{
       return m_nFirstNum+m_nSecNum;
}
```
上面两种写法也是有区别的，第一种方法写的函数具有Inline函数的特性。后一种则没有。
#### 2、 属性和方法的使用
C++中定义一个对象跟定义一个函数没有什么区别。


```
#include <stdio.h>
#include "Example.h"
int main(int argc, char* argv[])
{
    CExample obj_Exp;      // 定义一个对象
    obj_Exp.SetNum(10,20); // 调用一个方法
    printf("%d+%d = %d\r\n", obj_Exp.GetFirstNum(),
                          obj_Exp.GetSecNum(),
                          obj_Exp.GetSum());
       return 0;
}
```
由此，我们就可以通过一个函数间接的来操作我们的变量，用户在给我们的变量赋值时，我们可以通过Set函数来对输入的内容作检测，在获取一个变量的内容时，我们可以通过一个函数来取得，这样都提高了程序安全性。
从程序设计的角度来讲，如果我们以类为单位编码的话，每个模块都是独立的我们只要关注与本类相关操作，比如人这个类，它一般情况下有两个眼睛、一个嘴巴等之类的属性，人可以使用工具，可以行走，可以跳跃等方法。我们编写的所有的函数都是针对这些展开的。而不用去关心谁要使用这个类。因此，类/对象概念的加入，不单单是给编码方式做了改变，主要是设计思路的改变，程序模块化的改变等等。
### 二、 解析对象的内存结构
现在，我相信，如果习惯了我这种学习方式的朋友一定会很好奇，类定义对象的内存格式是怎样的，它是不是像一个普通变量那样，或者是不是像一个结构体变量那样在内存里连续的将各个成员组织到一起，它又是怎样将各个成员变量还有函数绑定到一起的？变量和函数在一起它是怎么组织的？本小节让我们来解决这些问题。
为节省篇幅，我们仍旧使用上面的代码。我们用VC的调试器，调试这个代码:
![](http://www.cppblog.com/images/cppblog_com/besterchen/Study/6/1.JPG)
注意看我们的变量监视区，我们定义的对象的内容跟结构体成员的内容格式差不多，（是按照定义的顺序连续存放的，这点跟普通的局部变量不一样，普通的局部变量在内存中的顺序与定义顺序相反）内存中只存放了成员变量，它并没有标出SetNum的位置，那它是怎么找到SetNum这个函数的呢？
根据我们先前调试C函数的经验，我们知道，函数的代码是被放在可执行文件的代码区段中的。在这个代码中，也有调用SetNum的代码，我们详细的跟一下它的汇编代码：


```
10:       CExample obj_Exp;
004011ED   lea         ecx,[ebp-14h]
004011F0   call        @ILT+15(CExample::CExample) (00401014)
004011F5   mov         dword ptr [ebp-4],0
11:       obj_Exp.SetNum(10,20);
004011FC   push        14h
004011FE   push        0Ah
00401200   lea         ecx,[ebp-14h]
00401203   call        @ILT+0(CExample::SetNum) (00401005)
```
这段代码又给我们带来了新的问题，我们只用类定义了一个对象（变量），它自动的调用了一个函数，根据注释我们知道它调用的是构造函数。我们跟进去看下:


```
11:   CExample::CExample()
12:   {
00401050   push        ebp
00401051   mov         ebp,esp
00401053   sub         esp,44h
00401056   push        ebx
00401057   push        esi
00401058   push        edi
00401059   push        ecx                               ; 保存寄存器环境
0040105A   lea         edi,[ebp-44h]
0040105D   mov         ecx,11h
00401062   mov         eax,0CCCCCCCCh        
00401067   rep stos    dword ptr [edi]    ; 将栈空间清为CC（Release编译就没有这部分代码了。）
00401069   pop         ecx                        
0040106A   mov         dword ptr [ebp-4],ecx   ; 将 ECX中的内容给局部变量
13:   }
0040106D   mov         eax,dword ptr [ebp-4]   ; 将ECX的内容返回
00401070   pop         edi
00401071   pop         esi
00401072   pop         ebx
00401073   mov         esp,ebp
00401075   pop         ebp
00401076   ret
```
这段代码，首次看还真看不出个所以然来，源码的构造函数中，我们什么都没写，是个空函数，而这里做的是返回ECX的值，可是这个函数也没有对ECX做什么特别的操作，而是直接使用进函数时ECX的值。那只能说明在调用这个函数前，ECX发生了变化。我们再回头看下调用构造函数的代码：


```
10:       CExample obj_Exp;
004011ED   lea         ecx,[ebp-14h]
004011F0   call        @ILT+15(CExample::CExample) (00401014)
004011F5   mov         dword ptr [ebp-4],0
```
![](http://www.cppblog.com/images/cppblog_com/besterchen/Study/6/2.JPG)
哈哈，它是把我们obj_Exp对象的地址给了ECX，然后调用构造返回的，也就是说，构造的返回值是我们对象的首地址。哎，迷糊了，真搞不懂这是在干什么。先不管他，我们继续看怎么调用的SetNum这个函数吧：


```
11:       obj_Exp.SetNum(10, 20);
004011FC   push        14h                        ; 传递参数
004011FE   push        0Ah
00401200   lea         ecx,[ebp-14h]           ; 也有这句，还是把我们的对象首地址给ECX
00401203   call        @ILT+0(CExample::SetNum) (00401005)
29:   bool CExample::SetNum(int nFirst, int nSec)
30:   {
00401130   push        ebp
00401131   mov         ebp,esp
00401133   sub         esp,44h
00401136   push        ebx
00401137   push        esi
00401138   push        edi
00401139   push        ecx
0040113A   lea         edi,[ebp-44h]
0040113D   mov         ecx,11h
00401142   mov         eax,0CCCCCCCCh
00401147   rep stos    dword ptr [edi]
00401149   pop         ecx
0040114A  mov         dword ptr [ebp-4],ecx    ; 备份一下我们的对象首地址
31:       m_nFirstNum  =  nFirst;
0040114D   mov         eax,dword ptr [ebp-4]   ; 取出对象首地址
00401150   mov         ecx,dword ptr [ebp+8]    ; 取出nFirst参数
00401153   mov         dword ptr [eax],ecx       ; 给对象首地址指向的内容赋值为nFirst的内容
32:       m_nSecNum  =  nSec ;
00401155   mov         eax,dword ptr [ebp-4]   ; 取出对象首地址
00401158   mov         ecx,dword ptr [ebp+0Ch]; 取出nSec参数
0040115B   mov         dword ptr [eax+4],ecx   ; 给对象首地址+4指向的你内容赋值
          return true;
0040115E   mov         al,1          ; 返回1
34:   }
00401160   pop         edi
00401161   pop         esi
00401162   pop         ebx
00401163   mov         esp,ebp
00401165   pop         ebp
00401166   ret         8
```
我简要的注释下来一下上面的代码。通过分析上面的代码，我们可以得出这样的结论:
A、 函数通过ecx传递了我们对象的首地址。
B、 函数通过ecx传递的对象首地址定位对象的每个成员变量。
这样，很明显，ECX起到了传递参数的作用，这时ecx中的地址有个专业术语，叫做this指针。
OK，这就是一个新的知识点，我们成员函数的调用方式。
#### 1、 成员函数的调用方式: __thiscall
记得在前面章节介绍函数时，讲过一些调用方式，但是没有提到过这种调用方式。下面我做一个简要的总结：
A、 参数也通过栈传递。
B、 它用一个寄存器来传递this指针。
C、 本条特性摘自《加密与解密》（第三版）非原文:
a) 对于VC++中传参规则：
i. 最左边两个不大于4字节的参数分别用ECX和EDX传参数.
ii. 对于浮点数、远指针、__int64类型总是通过栈来传递的。
b) 对于BC++|DELPHI中的传递规则:
i. 最左边三个不大于DWORD的参数，依次使用EAX,ECX,EDX传递，其它多的参数依次通过PASCAL方式传递。
这样，函数的地址还是在代码区域，对象的内存中只存放数据成员，当我们要调用成员函数时，就通过一个寄存器将函数操作的对象的首地址（也就是this指针）传递过去就可以了，传递不同的对象指针，就操作不同的数据。哈哈，太巧妙了。
#### 2、 浅谈构造与析构函数
OK，继续调试代码:


```
13:       printf("%d+%d = %d\r\n", obj_Exp.GetFirstNum(),
14:                                obj_Exp.GetSecNum(),
15:                                obj_Exp.GetSum());
00401208   lea         ecx,[ebp-14h]
0040120B   call        @ILT+30(CExample::GetSum) (00401023)            ; 调用GetSum函数
00401210   push        eax
00401211   lea         ecx,[ebp-14h]
00401214   call        @ILT+5(CExample::GetSecNum) (0040100a)
00401219   push        eax
0040121A   lea         ecx,[ebp-14h]
0040121D   call        @ILT+10(CExample::GetFirstNum) (0040100f)
00401222   push        eax
00401223   push        offset string "%d+%d = %d\r\n" (0042501c)
00401228   call        printf (00401290)
0040122D   add         esp,10h
16:
17:       return 0;
00401230   mov         dword ptr [ebp-18h],0
00401237   mov         dword ptr [ebp-4],0FFFFFFFFh
0040123E   lea         ecx,[ebp-14h]
00401241   call        @ILT+20(CExample::~CExample) (00401019); 调用析构函数
00401246   mov         eax,dword ptr [ebp-18h]
```
我们至始至终都没有调用过构造和析构函数。但是，通过这次调我们知道，在创建一个对象（变量）的时候，我们的程序会自动的调用我们的构造函数，在要出对象作用域的时候，会自动的调用析构函数。
这样，我们很容易就能想象出，构造和析构的用途：构造就做初始化对象的各个成员，申请空间等初始化工作。析构就做一些释放申请的空间啊之类的清理工作。
就这样，C++将数据跟函数封装到了一起，这样我们每个类产生的对象都是一个独立的个体，它有一个自己的运作方式，几乎完全独立。在我们使用它的时候，根本不需要它是怎么实现了，只要知道怎么使用即可。
### 三、 浅谈类的静态成员
通过前面几节的学习，我们大概的能理解类的封装性及其运作过程，但是，如果我们继续深入的学习C++，我们很快就能发现一个问题：我们上面说的所有的成员都是属于对象的，也就是说，我们必须先通过类来定义一个对象才可以操作。但是有的时候，我们需要一些属于类的成员，比如:人都有一个脑袋，这一个脑袋属于人类共有的特性。不需要具体到哪一个人，我们都可以确定人只有一个脑袋。
放到类中也一样，比如我们需要知道当前这个类创建了几个对象的时候，我们不必在创建一个新的对象只需要使用类的相关函数或者直接访问类的某些属性就可以了，而这些函数或者变量，它肯定不可能属于某个对象，它应该属于这个类本身。
OK,下面就来体验一下静态带给我们的一些好处。同样，我们将前面的代码添加点儿东西（见Exp02）:


```
public:
    static int m_nCount; //统计产生对象的
    static int print(constchar *szFormat, ...);   // 让我们的类有自己的输出函数
       // .cpp文件中
int CExample::m_nCount = 0; // 初始化静态成员变量
CExample::CExample()
{
    m_nCount++;       // 当创建一个对象的时候，这个变量加1
}
 
CExample::~CExample()
{
    if (m_nCount > 0)
    {
        m_nCount--;  // 当对象销毁时，这个变量减1
    }
}
/************************************************************************/
/*让我们的CExample可以打印自己的信息
/*支持多参，同printf用法相同
/************************************************************************/
int CExample::print(constchar *szFormat, ...)
{
    if (!szFormat)
    {
        return 0;
    }
 
    va_list   pArgs;
    char      szBuffer[256 * 15] = {0};
    va_start(pArgs,szFormat);  
    vsprintf(szBuffer,szFormat, pArgs);
    va_end(pArgs);
 
    printf(szBuffer);
    return strlen(szFormat);
}
```
好，有了这些，我们可以编写如下的测试代码:


```
#include "stdafx.h"
#include <stdio.h>
#include "Example.h"
 
int main(int argc, char* argv[])
{
    CExample obj_Exp1;
    CExample::print("当前对象的数量为:%d\r\n", CExample::m_nCount);
 
    if (1)
    {
        CExample obj_Exp2;// 该对象属于if作用域，出了if，对象自动销毁
        CExample::print("当前对象的数量为:%d\r\n", CExample::m_nCount);
    }
 
    CExample::print("当前对象的数量为:%d\r\n", CExample::m_nCount);
 
       return 0;
}
```
我想大家应该能想象出来运行的结果:
![](http://www.cppblog.com/images/cppblog_com/besterchen/Study/6/3.JPG)
好，我们调试一下这段程序:


```
11:       CExample::print("当前对象的数量为:%d\r\n", CExample::m_nCount);
004012EC   mov         eax,[CExample::m_nCount (0042ae6c)]       ;这明显告诉我们，静态就是全局
004012F1   push        eax
004012F2   push        offset string "当前对象的数量为:%d\r\n"
004012F7   call        @ILT+30(CExample::print) (00401023)    ;调用该静态函数没有传递this指针
004012FC   add         esp,8
```
多了不用看了，通过这段代码，我们很明显就可以清楚，静态变量，不属于类对象，它存放于全局数据区，同全局变量在一个地方（更多关于静态变量的相关说明见我发的《static学习笔记》一文）。
静态函数，跟全局函数一样，它虽然在源码中书写与类内，但是它其实就是一个全局函数，不传递this指针，因此，在使用静态函数时需要知道，静态函数中不能调用其它普通的成员函数也不能引用普通的成员变量。但是反过来，在其它的成员函数中可以调用静态函数也可以使用静态变量。
### 四、 打破类封装性的棒槌 —— 友元
当我们的工程总类越来越多的时候，我们很难避免类与类之间相互操作。倘若每个类之间相互操作都通过函数进行，那可想我们编写代码是多么的繁琐，想投机取巧走小路么？是的，C++提供了这种捷径，让我们直接对一个类的私有成员进行操作——当然，我极力反对这种做法，因为它打破了类的封装性。
我不推荐使用友元在这里仍然讲述友元并不是我无聊也不是我写它装门面。因为我们再后面学习用全局函数运算符的时候，不可避免的要使用到它。
声明友元的关键字是friend，OK，看代码，我们还是在上面的代码中加点东西(见Exp03):


```
// Example.h
       friend extern void ShowPrivMem(CExample tmpExpObj);
```
声明ShowPrivMem函数是CExample类的友元函数，这时ShowPrivMem中就可以随便访问CExample类中任何一个成员了。
编写测试代码如下:


```
void ShowPrivMem(CExample tmpExpObj)
{
    printf("%d+%d = %d\r\n", tmpExpObj.m_nFirstNum,
                             tmpExpObj.m_nSecNum,
                             tmpExpObj.GetSum());
}
int main(int argc,char* argv[])
{
    CExample obj_Exp;
    obj_Exp.SetNum(10,20);
    ShowPrivMem(obj_Exp);
    return 0;
}
```
当然，这是将一个函数声明为友元，将另外一个类声明为自己的友元也是一样的。由于没有什么特别之处，我就不为此特别的写一个代码了。
另外，需要注意的是，将一个类声明为自己的友元，并不是自己可以访问那个类的私有成员而是被声明为友元的类可以访问自己的私有成员。注意顺序别弄反了……。
### 五、 学习小结
本节讲的东西很少，但是很关键，调试C++程序，摸清楚this指针对摸清程序结构，摸清程序数据格式都起着非常重要的作用。
学习本小节的方法，不是理解，而是多写，多调试。
祝大家成功。
