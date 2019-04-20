# ARM中C和汇编混合编程及示例（转） - h13 - 博客园
在嵌入式系统开发中，目前使用的主要编程语言是C和汇编，C++已经有相应的编译器，但是现在使用还是比较少的。在稍大规模的嵌入式软件中，例如含有OS，大部分的代码都是用C编写的，主要是因为C语言的结构比较好，便于人的理解，而且有大量的支持库。尽管如此，很多地方还是要用到汇编语言，例如开机时硬件系统的初始化，包括CPU状态的设定，中断的使能，主频的设定，以及RAM的控制参数及初始化，一些中断处理方面也可能涉及汇编。另外一个使用汇编的地方就是一些对性能非常敏感的代码块，这是不能依靠C编译器的生成代码，而要手工编写汇编，达到优化的目的。而且，汇编语言是和CPU的指令集紧密相连的，作为涉及底层的嵌入式系统开发，熟练对应汇编语言的使用也是必须的。
单纯的C或者汇编编程请参考相关的书籍或者手册，这里主要讨论C和汇编的混合编程，包括相互之间的函数调用。下面分四种情况来进行讨论，暂不涉及C++。
**1． ****在C****语言中内嵌汇编**
在C中内嵌的汇编指令包含大部分的ARM和Thumb指令，不过其使用与汇编文件中的指令有些不同，存在一些限制，主要有下面几个方面：
a.       不能直接向PC寄存器赋值，程序跳转要使用B或者BL指令
b.       在使用物理寄存器时，不要使用过于复杂的C表达式，避免物理寄存器冲突
c.       R12和R13可能被编译器用来存放中间编译结果，计算表达式值时可能将R0到R3、R12及R14用于子程序调用，因此要避免直接使用这些物理寄存器
d.       一般不要直接指定物理寄存器，而让编译器进行分配
内嵌汇编使用的标记是 __asm或者asm关键字，用法如下：
__asm
{
       instruction [;  instruction]
       …
       [instruction]
}
asm(“instruction [;  instruction]”);
下面通过一个例子来说明如何在C中内嵌汇编语言，
#include <stdio.h>
void my_strcpy(const char *src, char *dest)
{
       char ch;
       __asm
       {
              loop:
              ldrb       ch, [src], #1
              strb       ch, [dest], #1
              cmp        ch, #0
              bne         loop
       }
}
int main()
{
       char *a = "forget it and move on!";
       char b[64];
       my_strcpy(a, b);
       printf("original: %s", a);
       printf("copyed:   %s", b);  
       return 0;
}
在这里C和汇编之间的值传递是用C的指针来实现的，因为指针对应的是地址，所以汇编中也可以访问。
**2． ****在汇编中使用C****定义的全局变量**
内嵌汇编不用单独编辑汇编语言文件，比较简洁，但是有诸多限制，当汇编的代码较多时一般放在单独的汇编文件中。这时就需要在汇编和C之间进行一些数据的传递，最简便的办法就是使用全局变量。
/*    cfile.c
 *    定义全局变量，并作为主调程序
 */
#include <stdio.h>
int gVar_1 = 12;
extern        asmDouble(void);
int main()
{
       printf("original value of gVar_1 is: %d", gVar_1);
       asmDouble();
       printf("       modified value of gVar_1 is: %d", gVar_1);
       return 0;
}
       对应的汇编语言文件
;called by main(in C),to double an integer, a global var defined in C is used.
       AREA asmfile, CODE, READONLY
       EXPORT       asmDouble
       IMPORT   gVar_1
asmDouble
       ldr r0, =gVar_1
       ldr          r1, [r0]
       mov        r2, #2      
       mul         r3, r1, r2
       str          r3, [r0]
       mov        pc, lr
       END
**3． ****在C****中调用汇编的函数**
在C中调用汇编文件中的函数，要做的主要工作有两个，一是在C中声明函数原型，并加extern关键字；二是在汇编中用EXPORT导出函数名，并用该函数名作为汇编代码段的标识，最后用mov        pc, lr返回。然后，就可以在C中使用该函数了。从C的角度，并不知道该函数的实现是用C还是汇编。更深的原因是因为C的函数名起到表明函数代码起始地址的左右，这个和汇编的label是一致的。
/*  cfile.c
 *  in C,call an asm function, asm_strcpy
 *       Sep 9, 2004
 */
#include <stdio.h>
extern void asm_strcpy(const char *src, char *dest);
int main()
{
       const        char *s = "seasons in the sun";
       char        d[32];
       asm_strcpy(s, d);
       printf("source: %s", s);
       printf("       destination: %s",d);
       return 0;
}
;asm function implementation
       AREA asmfile, CODE, READONLY
       EXPORT asm_strcpy
asm_strcpy
loop
       ldrb          r4, [r0], #1       ;address increment after read
       cmp         r4, #0
       beq           over
       strb          r4, [r1], #1
       b               loop
over
       mov           pc, lr
       END
       在这里，C和汇编之间的参数传递是通过ATPCS（ARM Thumb Procedure Call Standard）的规定来进行的。简单的说就是如果函数有不多于四个参数，对应的用R0-R3来进行传递，多于4个时借助栈，函数的返回值通过R0来返回。
**4． ****在汇编中调用C****的函数**
在汇编中调用C的函数，需要在汇编中IMPORT 对应的C函数名，然后将C的代码放在一个独立的C文件中进行编译，剩下的工作由连接器来处理。
;the details of parameters transfer comes from ATPCS
;if there are more than 4 args, stack will be used
       EXPORT asmfile
       AREA asmfile, CODE, READONLY
       IMPORT   cFun
       ENTRY
       mov        r0, #11
       mov        r1, #22
       mov        r2, #33
       BL       cFun
       END
/*C file,  called by asmfile */
int       cFun(int a, int b, int c)
{
       return a + b + c;
}
       在汇编中调用C的函数，参数的传递也是通过ATPCS来实现的。需要指出的是当函数的参数个数大于4时，要借助stack，具体见ATPCS规范。
**小结**
以上通过几个简单的例子演示了嵌入式开发中常用的C和汇编混合编程的一些方法和基本的思路，其实最核心的问题就是如何在C和汇编之间传值，剩下的问题就是各自用自己的方式来进行处理。以上只是抛砖引玉，更详细和复杂的使用方法要结合实际应用并参考相关的资料。
**说明**
以上代码在ADS 1.2的工程中编译，并在对应的AXD中软件仿真通过。
**参考资料**
1．  杜春雷，ARM体系结构与编程，清华大学出版社，2003
2．  UC/OS-II for ARM移植的相关启动代码
转自：http://blog.csdn.net/rockyqiu2002/article/details/100158
