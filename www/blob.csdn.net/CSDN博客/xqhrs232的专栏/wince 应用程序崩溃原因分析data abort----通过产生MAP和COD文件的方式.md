# wince 应用程序崩溃原因分析data abort----通过产生MAP和COD文件的方式 - xqhrs232的专栏 - CSDN博客
2013年04月16日 11:00:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：947
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://justtotry.blog.163.com/blog/static/44630812201171510614697/](http://justtotry.blog.163.com/blog/static/44630812201171510614697/)
VS2005使用map文件查找程序崩溃原因
一般程序崩溃可以通过debug，找到程序在那一行代码崩溃了，最近编一个多线程的程序，都不知道在那发生错误，多线程并发，又不好单行调试，终于 找到一个比较好的方法来找原因，通过生成map文件，由于2005取消map文件生成行号信息（vc6.0下是可以生成行号信息的，不知道 microsoft怎么想的，在2005上取消了），只能定位在那个函数发生崩溃。这里可以通过生成cod文件，即机器码这一文件，具体定位在那一行崩 溃。
   首先配置vc2005生成map文件和cod文件：(1).map文件:property->Configuration Properties->Linker->Debugging 中的Generate Map File选择Yes(/MAP);
(2).cod文件:property->Configuration Properties->C/C++->output Files中Assembler OutPut中选择Assembly,Maching Code and Source(/FAcs),生成机器，源代码。
简单例子:
(1) #include "stdafx.h"
void errorFun(int * p)
{
*p=1;
}
int _tmain(int argc, _TCHAR* argv[])
{
int * p=NULL;
errorFun(p);
return 0;
}
在errorFun中函数中，*p=1这一行出错，由于p没有申请空间，运行时出错,弹出
Unhandled exception at 0x004113b1 in testError.exe: 0xC0000005: Access violation writing location 0x00000000.
在0x004113b1程序发生崩溃。
（2）debug文件下打开map文件，定位崩溃函数.
map文件开头是一些链接信息，然后我们要找函数和实始地址信息。地址是函始的开始地址
Address         Publics by Value              Rva+Base       Lib:Object
0000:00000000       ___safe_se_handler_count   00000000     <absolute>
0000:00000000       ___safe_se_handler_table   00000000     <absolute>
0000:00000000       ___ImageBase               00400000     <linker-defined>
0001:00000000       __enc$textbss$begin        00401000     <linker-defined>
0001:00010000       __enc$textbss$end          00411000     <linker-defined>
0002:00000390       [?errorFun@@YAXPAH@Z](mailto:?errorFun@@YAXPAH@Z)        00411390 f   testError.obj
0002:000003d0       _wmain                     004113d0 f   testError.obj
0002:00000430       __RTC_InitBase             00411430 f   MSVCRTD:init.obj
0002:00000470       __RTC_Shutdown             00411470 f   MSVCRTD:init.obj
0002:00000490       __RTC_CheckEsp             00411490 f   MSVCRTD:stack.obj
0002:000004c0       @_RTC_CheckStackVars@8     004114c0 f   MSVCRTD:stack.obj
0002:00000540       @_RTC_AllocaHelper@12      00411540 f   MSVCRTD:stack.obj
....
程序崩溃地址0x004113b1,我们找到第一个比这个地址大的004113d0,前一个是00411390,地址是函数的开始地址，所以发生的崩溃的的函数是errorFun,这个函数的初始地址00411390.
(3)找出具体崩溃行号.
由(2)可知，发生错误函数是errorFun,在testError.obj，打开testError.cod文件，找到errorFun函数生成的机器码.
[?errorFun@@YAXPAH@Z](mailto:?errorFun@@YAXPAH@Z) PROC    ; errorFun, COMDAT
; 7    : {
00000 55   push ebp
00001 8b ec   mov ebp, esp
00003 81 ec c0 00 00
00   sub esp, 192 ; 000000c0H
00009 53   push ebx
0000a 56   push esi
0000b 57   push edi
0000c 8d bd 40 ff ff
ff   lea edi, DWORD PTR [ebp-192]
00012 b9 30 00 00 00 mov ecx, 48   ; 00000030H
00017 b8 cc cc cc cc mov eax, -858993460 ; ccccccccH
0001c f3 ab   rep stosd
; 8    : *p=1;
0001e 8b 45 08 mov eax, DWORD PTR _p$[ebp]
00021 c7 00 01 00 00
00   mov DWORD PTR [eax], 1
; 9    : }
00027 5f   pop edi
00028 5e   pop esi
00029 5b   pop ebx
0002a 8b e5   mov esp, ebp
0002c 5d   pop ebp
0002d c3   ret 0
（说明: 7,8,9是表示在源代码的行号。
00000 55   push ebp，000000是相对偏移地地，55是机器码号，push ebp,000000是汇编码。）
通过（2）我们计算相对偏移地址，即崩溃地址-函数起始地址,0x004113b1-0x00411390=0x21(16进制的计数)
找到0x21这一行对应的机器码是 00021 c7 00 01 00 00,向上看它是由第8行*p=1；生成的汇编码，由此可见是这一行程序发生崩溃。
结束语：当然这只是一个简单的例子，实际上一运行便知道是这一行出错，但是对于一个比较大的工程，特别是在多线程并发情况下，要找出那一行出错比较困难，便可以使用map和cod文件找到程序崩溃原因。

//================================================================
备注：：
1》这篇文章提到了VS2005 APP下怎么产生MAP跟COD文件，很方便调试！！！
