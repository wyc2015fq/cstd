# 在C/C++代码中使用SSE等指令集的指令(1)介绍 - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 01:30:52[vbskj](https://me.csdn.net/vbskj)阅读数：1769
个人分类：[SSE并行](https://blog.csdn.net/vbskj/article/category/2169499)










我们知道，在C/C++代码中，可以插入汇编代码提高性能。现在的指令集有了很多的高级指令，如果我们希望使用这些高级指令来实现一些高效的算法，就可以在代码中嵌入汇编，使用SSE等高级指令，这是可行的，但是如果对汇编不太熟悉，不愿意使用汇编的人来说，其实也是可以的，这就是Compiler Intrinsics（[http://msdn.microsoft.com/zh-cn/site/26td21ds](http://msdn.microsoft.com/zh-cn/site/26td21ds)）。


PS：下面的内容以Windows平台为主，对于Linux下，也有类似的方法。


（1）什么是Intrinsics


Intrinsics是对MMX、SSE等指令集的指令的一种封装，以函数的形式提供，使得程序员更容易编写和使用这些高级指令，在编译的时候，这些函数会被内联为汇编，不会产生函数调用的开销。在理解intrinsics指令之前，先理解intrinsics函数。


（3）#pragma intrinsic和#pragma function


#pragma intrinsic(function[,function][,function]...)：表示后面的函数将进行intrinsic，替换为内部函数，去掉了函数调用的开销，注意：有些地方解释为内联，但是和内联并不完全相同，对于内联，可以指定任意函数为内联，但是此pragma intrinsic只能适用于编译器规定的一部分函数，不是所有函数都能使用，而且，inline关键字一般用于指定自定义的函数，intrinsic则是系统库函数的一部分。参考[http://technet.microsoft.com/zh-cn/library/tzkfha43.aspx](http://technet.microsoft.com/zh-cn/library/tzkfha43.aspx)获取详细的说明。


下面分析这个例子：



- #include <math.h>
- void foo()  
- {  
- double var = cos(10);  
- }  

使用VS2010的32bit的command line编译：



cl /c test.c /FA



输出得到其汇编文件：



- ; Listing generated by Microsoft (R) Optimizing Compiler Version 16.00.30319.01   
- 
-     TITLE   C:\tempLab\test.c  
-     .686P  
-     .XMM  
-     include listing.inc  
-     .model  flat  
- 
- INCLUDELIB LIBCMT  
- INCLUDELIB OLDNAMES  
- 
- PUBLIC  __real@4024000000000000  
- PUBLIC  _foo  
- EXTRN   _cos:PROC  
- EXTRN   __fltused:DWORD
- ;   COMDAT __real@4024000000000000  
- ; File c:\templab\test.c  
- CONST   SEGMENT  
- __real@4024000000000000 DQ 04024000000000000r   ; 10  
- ; Function compile flags: /Odtp  
- CONST   ENDS  
- _TEXT   SEGMENT  
- _var$ = -8                      ; size = 8  
- _foo    PROC  
- ; Line 3  
-     push    ebp  
-     mov ebp, esp  
-     sub esp, 8  
- ; Line 4  
-     sub esp, 8  
-     fld QWORD PTR __real@4024000000000000  
-     fstp    QWORD PTR [esp]  
-     call    _cos  
-     add esp, 8  
-     fstp    QWORD PTR _var$[ebp]  
- ; Line 5  
-     mov esp, ebp  
-     pop ebp  
-     ret 0  
- _foo    ENDP  
- _TEXT   ENDS  
- END  

可以看到，这里调用了call_cos函数进行运算，下面代码修改如下：




- #include <math.h>
- #pragma intrinsic(cos)
- void foo()  
- {  
- double var = cos(10);  
- }  

同样的命令编译，得到汇编如下：




- ; Listing generated by Microsoft (R) Optimizing Compiler Version 16.00.30319.01   
- 
-     TITLE   C:\tempLab\test.c  
-     .686P  
-     .XMM  
-     include listing.inc  
-     .model  flat  
- 
- INCLUDELIB LIBCMT  
- INCLUDELIB OLDNAMES  
- 
- PUBLIC  __real@4024000000000000  
- PUBLIC  _foo  
- EXTRN   __fltused:DWORD
- EXTRN   __CIcos:PROC  
- ;   COMDAT __real@4024000000000000  
- ; File c:\templab\test.c  
- CONST   SEGMENT  
- __real@4024000000000000 DQ 04024000000000000r   ; 10  
- ; Function compile flags: /Odtp  
- CONST   ENDS  
- _TEXT   SEGMENT  
- _var$ = -8                      ; size = 8  
- _foo    PROC  
- ; Line 4  
-     push    ebp  
-     mov ebp, esp  
-     sub esp, 8  
- ; Line 5  
-     fld QWORD PTR __real@4024000000000000  
-     call    __CIcos  
-     fstp    QWORD PTR _var$[ebp]  
- ; Line 6  
-     mov esp, ebp  
-     pop ebp  
-     ret 0  
- _foo    ENDP  
- _TEXT   ENDS  
- END  

对比之后，它们的主要区别的代码段如下：




- sub esp, 8  
-     fld QWORD PTR __real@4024000000000000  
- 
-     fstp    QWORD PTR [esp]  
-     call    _cos  
-     add esp, 8  


- fld QWORD PTR __real@4024000000000000  
- call    __CIcos  

显然，使用了Intrinsics之后的cos函数的指令少了很多，其调用的内部函数是_CIcos([http://msdn.microsoft.com/zh-cn/library/ff770589.aspx](http://msdn.microsoft.com/zh-cn/library/ff770589.aspx))，此函数会计算对栈顶的元素直接进行cos运算，所以节省了很多函数调用参数传递等的指令。



仍然参考MSDN（[http://technet.microsoft.com/zh-cn/library/tzkfha43.aspx](http://technet.microsoft.com/zh-cn/library/tzkfha43.aspx)）可以看到其中一段话：


The floating-point functions listed below do not have true intrinsic forms. Instead they have versions that pass arguments directly to the floating-point chip rather than pushing them onto the program stack.



当然，这是描述其中一部分Intrinsics函数的，Intrinsics也有不同的方式进行优化/内联，具体参考MSDN查询哪些函数可以使用Intrinsics以及是如何工作的（[http://msdn.microsoft.com/zh-cn/site/26td21ds](http://msdn.microsoft.com/zh-cn/site/26td21ds)）。


#pragma function：使用格式和intrinsics一样，pragma function用于指定函数不进行intrinsics操作，也就是不生成内部函数。



最后，要知道的一个内容是一个相关的编译选项：/Oi

[http://technet.microsoft.com/zh-cn/library/f99tchzc.aspx](http://technet.microsoft.com/zh-cn/library/f99tchzc.aspx)



/Oi 仅作为对编译器的请求，用于将某些函数调用替换为内部函数；为产生更好的性能，编译器可能会调用函数（而不会将该函数调用替换为内部函数）。



简单的理解，就是告诉编译器尽量使用intrinsics版本的调用，当然，最终的实际调用依赖于编译器的判断。


也可以参考wiki中（[http://en.wikipedia.org/wiki/Intrinsic_function](http://en.wikipedia.org/wiki/Intrinsic_function)）关于intrinsic functions来帮助理解其作用。简单来说，可以理解为编译器的“内置函数”，编译器会根据情况进行一些优化。


（4）指令集相关的intrinsics介绍


上面介绍的是pragma对intrinsic函数的使用，其中介绍了cos，还有很多类似的“内置函数版本”。有时候将上面的这些称之为”intrinsics函数“，除此之外，intrinsics更广泛的使用是指令集的封装，能直接映射到高级指令集，从而使得程序员可以以函数调用的方式来实现汇编能达到的功能，编译器会生成为对应的SSE等指令集汇编。


1. 如何使用这类函数


在windows上，包含#include <**mmintrin.h>头文件即可（不同的指令集扩展的函数可能前缀不一样），也可以直接包含#include <intrin.h>（这里面会根据使用环境判断使用ADM的一些兼容扩展）。


2. 关于数据类型


这些和指令集相关的函数，一般都有自己的数据类型，不能使用一般的数据类型传递进行计算，一般来说，MMX指令是__m64（[http://msdn.microsoft.com/zh-cn/library/08x3t697(v=VS.90).aspx](http://msdn.microsoft.com/zh-cn/library/08x3t697(v=VS.90).aspx)）类型的数据，SSE是__m128类型的数据等等。


3. 函数名：


这类函数名一般以__m开头。函数名称和指令名称有一定的关系。


4. 加法实例：


下面使用SSE指令集进行加法运算，一条指令对四个浮点数进行运算：



- #include <stdio.h>
- #include <intrin.h>
- 
- int main(int argc, char* argv[])  
- {  
-     __m128  a;  
-     __m128  b;  
- 
-     a = _mm_set_ps(1,2,3,4);        // Assign value to a
-     b = _mm_set_ps(1,2,3,4);        // Assign value to a
- 
-     __m128 c = _mm_add_ps(a, b);    // c = a + b
- 
-     printf("0: %lf\n", c.m128_f32[0]);  
-     printf("1: %lf\n", c.m128_f32[1]);  
-     printf("2: %lf\n", c.m128_f32[2]);  
-     printf("3: %lf\n", c.m128_f32[3]);  
- 
- return 0;  
- }  

从代码看，好像很复杂，但是生成的汇编的效率会比较高。一条指令就完成了四个浮点数的加法，其运行结果如下：


![](http://hi.csdn.net/attachment/201111/24/0_132212010578Yq.gif)



（5）总结：


1. Intrinsics函数：能提高性能，会增大生成代码的大小，是编译器的”内置函数“。


2. Intrinsics对指令的封装函数：直接映射到汇编指令，能简化汇编代码的编写，另外，隐藏了寄存器分配和调度等。由于涉及到的数据类型、函数等内容较多，这里只是一个简单的介绍。



