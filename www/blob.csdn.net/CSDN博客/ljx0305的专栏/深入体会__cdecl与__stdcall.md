# 深入体会__cdecl与__stdcall  - ljx0305的专栏 - CSDN博客
2011年03月18日 10:16:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：495标签：[汇编																[编译器																[callback																[security																[windows																[winapi](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=security&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)
[深入体会__cdecl与__stdcall](http://www.cnblogs.com/sober/archive/2009/09/01/1558178.html)
    在学习C++的过程中时常碰到WINAPI或者CALLBACK这样的调用约定，每每觉得十分迷惑。究竟这些东西有什么用？不用他们又会不会有问题？经过在网上的一番搜寻以及自己动手后，整理成以下的学习笔记。
1.WINAPI与CALLBACK
    其实这两者在Windows下是相同的，在windef.h中定义如下： 
```
Code
#ifdef _MAC
#define CALLBACK    PASCAL
#define WINAPI      CDECL
#define WINAPIV     CDECL
#define APIENTRY    WINAPI
#define APIPRIVATE  CDECL
#ifdef _68K_
#define PASCAL      __pascal
#else
#define PASCAL
#endif
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif
```
    这里根据不同的系统版本选择不同的定义，Windows的话对应#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)后面的那一段，可以看得出都为__stdcall。至于为什么Windows要对应__stdcall可能与系统本身的内存处理方式有关。现在问题就在于__stdcall是什么？
2.__stdcall与__cdecl
    在网上找到的资料如下：
    1）采用__cdecl约定时，函数参数按照从右到左的顺序入栈，并且由调用函数者把参数弹出栈以清理堆栈。因此，实现可变参数的函数只能使用该调用约定。由于每一个使用__cdecl约定的函数都要包含清理堆栈的代码，所以产生的可执行文件大小会比较大。
      2）采用__stdcall约定时，函数参数按照从右到左的顺序入栈，被调用的函数在返回前清理传送参数的栈，函数参数个数固定。由于函数体本身知道传进来的参数个数，因此被调用的函数可以在返回前用一条ret n指令直接清理传递参数的堆栈。
 在看完这些描述后，有些问题必须解决的，首先什么是可变参数的函数，还有为什么__stdcall就不能调用可变参数的函数。
3.汇编详解__stdcall与__cdecl
    1）准备  VS2008下查看程序反汇编代码的方法
    首先创建一个可运行的工程，接着在程序中设置一个断点，debug一下程序。在代码区域的任意地方右键-->"转到反汇编"即可看到程序的反汇编代码。
    2）VS2008一些默认的设置
    在VS2008下，全局函数的约定是__cdecl，类的成员函数的约定是__stdcall。还有一般的Win32 API函数都是__stdcall。一般来说可以用__stdcall的就不会去使用__cdecl，这样有更好的封装性，因为入栈和清空栈的代码在同一块地方。至于为什么全局函数会是__cdecl，我还没想出来。
    3）__cdecl约定的反汇编分析 
```
Code
 1 --- stdcalltest.cpp -------------------
 2 // StdcallTest.cpp : 定义控制台应用程序的入口点。
 3 //
 4 
 5 #include "stdafx.h"
 6 
 7 int Add(int a, int b)
 8 {
 9 004113A0  push        ebp  
10 004113A1  mov         ebp,esp 
11 004113A3  sub         esp,0C0h 
12 004113A9  push        ebx  
13 004113AA  push        esi  
14 004113AB  push        edi  
15 004113AC  lea         edi,[ebp-0C0h] 
16 004113B2  mov         ecx,30h 
17 004113B7  mov         eax,0CCCCCCCCh 
18 004113BC  rep stos    dword ptr es:[edi] 
19    return a + b;
20 004113BE  mov         eax,dword ptr [a] 
21 004113C1  add         eax,dword ptr [b] 
22 }
23 004113C4  pop         edi  
24 004113C5  pop         esi  
25 004113C6  pop         ebx  
26 004113C7  mov         esp,ebp 
27 004113C9  pop         ebp  
28 004113CA  ret              
29  
30 --- stdcalltest.cpp -------------------
31 
32 int _tmain(int argc, _TCHAR* argv[])
33 {
34 004113E0  push        ebp  
35 004113E1  mov         ebp,esp 
36 004113E3  sub         esp,0CCh 
37 004113E9  push        ebx  
38 004113EA  push        esi  
39 004113EB  push        edi  
40 004113EC  lea         edi,[ebp-0CCh] 
41 004113F2  mov         ecx,33h 
42 004113F7  mov         eax,0CCCCCCCCh 
43 004113FC  rep stos    dword ptr es:[edi] 
44     int sum = Add(1, 2);
45 004113FE  push        2    
46 00411400  push        1    
47 00411402  call        Add (41108Ch) 
48 00411407  add         esp,8 
49 0041140A  mov         dword ptr [sum],eax 
50     return 0;
51 0041140D  xor         eax,eax 
52 }
53 0041140F  pop         edi  
54 00411410  pop         esi  
55 00411411  pop         ebx  
56 00411412  add         esp,0CCh 
57 00411418  cmp         ebp,esp 
58 0041141A  call        @ILT+325(__RTC_CheckEsp) (41114Ah) 
59 0041141F  mov         esp,ebp 
60 00411421  pop         ebp  
61 00411422  ret
```
    这里实现的是int Add(int a, int b)，然后调用Add(1, 2)。从44行开始看，先push 2，再push 1，可见入栈是从右到左。接着call 41108Ch的内存，但在这段代码中是找不到这个内存的。在call处按F11，会看到如下代码：Add: 0041108C  jmp         Add (4113A0h) ，这里的4113A0h在上面代码就可以找到在第9行了。至于为什么要这样子？你可以认为VS在编译的时候把函数的地址放在一段内存中比较好管理。而事实上函数的地址也确实是这样子。为了更清楚，我把这段代码也展示一下。 
```
Code
 1 0041100F  jmp         wmain (4113E0h) 
 2 00411014  jmp         _RTC_GetErrDesc (411BE0h) 
 3 00411019  jmp         __p__fmode (412796h) 
 4 0041101E  jmp         __security_check_cookie (4132F0h) 
 5 00411023  jmp         IsDebuggerPresent (4134AAh) 
 6 00411028  jmp         _RTC_Terminate (412760h) 
 7 0041102D  jmp         WideCharToMultiByte (4134BCh) 
 8 00411032  jmp         _RTC_AllocaHelper (411550h) 
 9 00411037  jmp         _RTC_GetErrorFuncW (411CA0h) 
10 0041103C  jmp         _RTC_NumErrors (411BD0h) 
11 00411041  jmp         __setusermatherr (412706h) 
12 00411046  jmp         Sleep (41349Eh) 
13 0041104B  jmp         GetModuleFileNameW (413510h) 
14 00411050  jmp         __security_init_cookie (412930h) 
15 00411055  jmp         SetUnhandledExceptionFilter (4134DAh) 
16 0041105A  jmp         _cexit (412A4Eh) 
17 0041105F  jmp         _CrtDbgReportW (412CD6h) 
18 00411064  jmp         VirtualQuery (413516h) 
19 00411069  jmp         atexit (4128F0h) 
20 0041106E  jmp         MultiByteToWideChar (4134C2h) 
21 00411073  jmp         _RTC_SetErrorType (411C00h) 
22 00411078  jmp         wmainCRTStartup (4117E0h) 
23 0041107D  jmp         _except_handler4 (412CF0h) 
24 00411082  jmp         _lock (413320h) 
25 00411087  jmp         GetProcAddress (4134CEh) 
26 0041108C  jmp         Add (4113A0h) 
27 00411091  jmp         _RTC_CheckStackVars (4114D0h) 
28 00411096  jmp         __report_gsfailure (413340h) 
29 0041109B  jmp         terminate (413302h) 
30 004110A0  jmp         _exit (412A42h)
```
    你会发现，在不知不觉中，编译器已经给你生成了这么多的函数，而Add仅仅在第26行出现过。看得出编译器对编程有多重要了。闲话少说，回到代码。在第二段代码展示的28行处的指令是ret，这里没有执行清栈。而在48行是add esp,8，这个操作将栈的指针修正到有参数入栈之前，这里的8就刚好是两个int的大小。
    4）__stdcall约定的反汇编分析
    这里只做一点点改动，将Add的定义改为int __stdcall Add(int a, int b)。在反汇编代码中不同的地方只有两处，一是add esp,8这条语句没有了，二是ret变为了ret 8。可见，清栈的工作变到了在函数里面。
    5）何为可变参数的函数
    我觉得必须先知道的是它的形式：type funcname(type para1, type para2, ...)。这里的"..."不是省略的意思，而是可变参数的函数必须这样声名。具体说明可参照[http://hi.baidu.com/sunlit88/blog/item/272460da3f360f61d1164ea7.html](http://hi.baidu.com/sunlit88/blog/item/272460da3f360f61d1164ea7.html)。下面是我改了一些地方的代码。 
```
Code
 1 // StdcallTest.cpp : 定义控制台应用程序的入口点。
 2 //
 3 
 4 #include "stdafx.h"
 5 #include <stdarg.h>
 6 
 7 int Add(int a, ![](http://www.cnblogs.com/Images/dot.gif))
 8 {
 9     int count = 0, sum = 0, i = a;
10     va_list marker;
11     va_start(marker, a); //初始化
12     while(i != -1)
13     {
14         sum += i; //先加第一个参数
15         ++count;
16         i = va_arg(marker, int);//取下一个参数
17     }
18     va_end(marker);
19     return sum;
20 }
21 
22 int _tmain(int argc, _TCHAR* argv[])
23 {
24     int sum = Add(1, 2, 3, -1);
25     return 0;
26 }
```
    查看反汇编代码也可以看出清栈的操作是在Add(1, 2, 3, -1)后执行的。本来我想试试写成int __stdcall Add(int a, ...)会有什么后果的，谁知道VS在编译的时候硬是把__stdcall方式改成__cdecl方式，看来编译器也不笨啊，知道这种方式肯定会出问题，就把你的改过来了。不过这也是一个好的编译器所需要做的事情，有时候你会发现自己写的代码与实际运行会有点点差别，那可能就是编译器把自己觉得需要优化的东西优化后的结果。这时候我又想起了volatile这个关键字，它就是让编译器不要去优化的时候使用的。
    6）可变参函数为什么不能用__stdcall
    我觉得这个问题应该从编译时和运行时来说，因为函数的代码是在编译的时候就已经在内存中写好的，而当程序在编译的时候，可变参不能告知代码的ret n的n是多少。而add esp,n是在运行时执行的，所以知道n是多少。
4.写在后面
    相对__cdecl和__stdcall还有很多约定，这里就不细说了。以前学汇编没细学，现在才发现只有从最最低层的代码才能看到程序的原貌。C++那层有时候还看不出问题，中间还有个编译器在搞鬼。
引用:http://www.cnblogs.com/sober/archive/2009/09/01/1558178.html
