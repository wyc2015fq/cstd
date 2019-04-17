# gdb 调试及优化 - 在思索中前行！ - CSDN博客





2014年11月18日 17:00:02[_Tham](https://me.csdn.net/txl16211)阅读数：1913








调试程序时，在gdb内p var，会提示

No symbol "var" in current context.

即使没有使用任何编译优化选项，仍然不能查看，可能是这些变量被优化到寄存器中，gdb无法读取。

g++编译时局部变量被优化到寄存器里了，此时是无法在内存中查看变量的值的。


解决方案：

在编译是加入 ‘-gstabs+’  选项，比如:

g++ -g -Wall  -gstabs+ -o main.o main.cpp

gdb main，就可以查看局部变量了。



## [使用GDB的源代码查看功能](http://www.cnblogs.com/frydsh/p/3388828.html)

## 在调试程序的过程中，可以自由地查看相关的源代码（如果有源代码的话）是一项最基本的特性。       一些IDE在这方面做得相当好，GDB当然也提供了这项特性，虽然不如IDE直观，但在一定程度上要比IDE更加灵活和快捷。       GDB之所以能够知道对应的源代码，是因为调试版的可执行程序中记录了源代码的位置；因为源代码的位置在编译之后可能会移动到其它地方，所以GDB还会在当前目录中查找源代码，另外GDB也允许明确指定源代码的搜索位置。默认情况下，GDB在编译时目录中搜索，如果失败则在当前目录中搜索，即$cdir:$cwd，其中$cdir指的是编译时目录（compilation directory），$cwd指的是当前工作目录（current working directory）。

##        在GDB中使用查看源代码相关的命令时，有一个当前文件的概念，当命令的位置参数没有限定一个文件的时候（不论是明确限定还是隐含限定），将使用当前文件。当前文件默认是main函数所在文件，如果程序当前正处于断点位置，则断点所在文件即为当前文件。       与当前文件的概念类似，还存在一个当前行的概念，它默认为main函数的开始处。如果使用gdb载入一个可执行文件，然后单单执行一条简单的list命令，你会发现输出的源代码并非是从第一行开始的，这是因为当前行默认在main函数附近处的缘故。      1.设置和获取源代码显示数量：         默认情况下，GDB显示指定位置处以及其前后的10行代码，但是这是一个可设置的值。         set listsize count：设置list命令显示的源代码数量最多为count行，0表示不限制行数。         show listsize：显示listsize的值。      2.编辑源代码：         在一些情况下，我们希望在编辑器中显示或者编辑源代码，GDB允许我们使用自己喜欢的编辑器。         可在环境变量EDITOR中指定GDB使用的编辑器，例如：EDITOR=/usr/bin/gedit;export EDITOR;gdb         edit location：在编辑器中编辑位置location处的源代码，如果省略location，则编辑当前位置。      3.搜索源代码：         有的时候，我们希望在当前文件中进行搜索，GDB提供了这样的命令。         search regexp：从当前行的下一行开始向前搜索。         rev regexp ：从当前行的上一行开始向后搜索。         有的时候，你会发现search命令总是提示“Expression not found”，这是因为当前行可能已经是最后一行了，特别是文件很短的时候。这里需要注意的是，任何list命令都会影响当前行的位置，并且由于每次都是多行输出，所以对当前行的影响并非简单地向前一行或者向后一行。          search命令本身也会影响当前行的位置。      4.源代码位置：         GDB之所以可以查看到源代码，是因为它知道源代码放在哪里。         在一个调试会话中，GDB维护了一个源代码查找目录列表，默认值是编译目录和当前工作目录。当GDB需要一个源文件的时候，它依次在这些目录中查找，直到找到一个或者抛出错误。         GDB还维护了一个路径替换规则，将要搜索的原始路径按照找到的第一个规则做前缀替换，然后再在源码搜索目录中查找文件。         GDB允许明确指定源代码位置，或者路径替换规则，以应付源代码位置迁移的情况。         directory path-list：将一个或者多个源代码搜索目录加入到当前源码搜索目录列表的前面，目录之间使用空格间隔。         directory：不带参数的directory将源码搜索目录恢复为默认值。         set directories path-list：将源码目录设置为path-list，但是会补上默认目录。         show directories：显示源码搜索目录列表。         set substitute-path from to：设置目录替换规则，放置在规则列表的末端。         unset substitute-path [path]：删除path对应的替换规则，或者删除所有的替换规则。         show substitute-path [path]：显示path对应的替换规则，或者显示所有的替换规则。      5.查看机器码：        在一些必要的时候，我们需要查看汇编代码来诊断问题。GDB提供了这种可能。GDB提供了两种能力：显示源代码位置与指令地址之间的映射；显示指定位置的汇编代码。        info line linespec：显示源代码linespec处对应的汇编地址范围。        info line *addr：显示地址addr处对应的源代码位置。        disassemble，disassemble /m，disassemble /r：显示指定地址范围内的汇编代码，有4种使用形式，

##         第一种不带参数，显示当前正在执行的函数的汇编代码；

##         第二种是一个参数，显示该地址所在函数的汇编代码；

##         第三种是两个参数的disassemble start,end，显示地址［start，end）内的汇编代码；

##         第四种是两个参数的disassemble start,+length，显示地址［start，start+length）内的汇编代码。

##         参数可以是16进制的地址，也可以是函数名。/m表示混合输出源代码和汇编代码，/r表示混合输出二进制和汇编代码。        set disassembly-flavor instruction-set：设置显示汇编代码时使用的风格，目前只针对intel x86系列，可取的值为att和intel，默认是att。        show disassembly-flavor：显示disassembly-flavor设置        set disassemble-next-line on|off|auto：当程序停止下来的时候，是否显示下一行源代码的汇编代码，默认为off。        show disassemble-next-line：显示disassemble-next-line设置。      6.显示指定位置的源代码：        list命令可用于显示指定位置处的源代码。list命令会影响当前行和当前文件。        list命令有多种方式指定要显示的源代码范围，可以是行号，函数名，甚至是指令地址。        常用的如下：        list linenum：显示指定行数附近的代码。        list function：显示指定函数附近的代码。        list ＊addr：显示指定地址附近的代码。

## [利用gdb在汇编指令级调试C程序](http://blog.csdn.net/mergerly/article/details/8538272)

关于[GDB](http://lenky.info/tag/gdb/)[调试](http://lenky.info/tag/%E8%B0%83%E8%AF%95/)[C程序](http://lenky.info/tag/c%E7%A8%8B%E5%BA%8F/)的[常用命令与手段](http://sourceware.org/gdb/documentation/)就不多说了，这里主要介绍一下如何对C程序做到[汇编](http://lenky.info/tag/%E6%B1%87%E7%BC%96/)指令级别的调试。


首先是获取汇编代码，这可以通过disassemble命令或x命令或类似的命令：






```cpp
[root@localhost test]# gdb ./a.out -q
(gdb) list
1   #include<stdio.h>
2   #include<malloc.h>
3
4   int callee(int a, int b, int c, int d, int e)
5   {
6       return 1;
7   }
8
9   int main(){
10      callee(1,2,3,4,5);
(gdb) disassemble main
Dump of assembler code for function main:
0x0000000000400463 <main+0>:  push   %rbp
0x0000000000400464 <main+1>:  mov    %rsp,%rbp
0x0000000000400467 <main+4>:  mov    $0x5,%r8d
0x000000000040046d <main+10>: mov    $0x4,%ecx
0x0000000000400472 <main+15>: mov    $0x3,%edx
0x0000000000400477 <main+20>: mov    $0x2,%esi
0x000000000040047c <main+25>: mov    $0x1,%edi
0x0000000000400481 <main+30>: callq  0x400448 <callee>
0x0000000000400486 <main+35>: mov    $0x2,%eax
0x000000000040048b <main+40>: leaveq
0x000000000040048c <main+41>: retq
End of assembler dump.
(gdb) x/10i main
0x400463 <main>:  push   %rbp
0x400464 <main+1>:    mov    %rsp,%rbp
0x400467 <main+4>:    mov    $0x5,%r8d
0x40046d <main+10>:   mov    $0x4,%ecx
0x400472 <main+15>:   mov    $0x3,%edx
0x400477 <main+20>:   mov    $0x2,%esi
0x40047c <main+25>:   mov    $0x1,%edi
0x400481 <main+30>:   callq  0x400448 <callee>
0x400486 <main+35>:   mov    $0x2,%eax
0x40048b <main+40>:   leaveq
(gdb)
```
接着，利用display命令自动显示当前正要执行的汇编指令，[display命令](http://sourceware.org/gdb/current/onlinedocs/gdb/Auto-Display.html#Auto-Display)可以在每次程序暂停时自动打印指定变量的值。而我们要显




示的汇编指令在ip寄存器内（当然，ip寄存器内存储的是机器码），我们可以看看（先得把程序执行起来）：





```cpp
(gdb) b main
Breakpoint 1 at 0x400467: file t3.5.c, line 10.
(gdb) r
Starting program: /root/test/a.out 
 
Breakpoint 1, main () at t3.5.c:10
10      callee(1,2,3,4,5);
(gdb) info reg
rax            0x3cd2153a60 261222644320
rbx            0x3cd101bbc0 261204589504
rcx            0x4004a0 4195488
rdx            0x7fffc5f6fa38   140736514685496
rsi            0x7fffc5f6fa28   140736514685480
rdi            0x1  1
rbp            0x7fffc5f6f940   0x7fffc5f6f940
rsp            0x7fffc5f6f940   0x7fffc5f6f940
r8             0x3cd21522d0 261222638288
r9             0x3cd0e0d620 261202433568
r10            0x0  0
r11            0x3cd1e1d8a0 261219276960
r12            0x0  0
r13            0x7fffc5f6fa20   140736514685472
r14            0x0  0
r15            0x0  0
rip            0x400467 0x400467 <main+4>
eflags         0x246    [ PF ZF IF ]
cs             0x33 51
ss             0x2b 43
ds             0x0  0
es             0x0  0
fs             0x0  0
gs             0x0  0
fctrl          0x37f    895
fstat          0x0  0
ftag           0xffff   65535
fiseg          0x0  0
fioff          0x0  0
foseg          0x0  0
fooff          0x0  0
fop            0x0  0
mxcsr          0x1f80   [ IM DM ZM OM UM PM ]
(gdb)
```

**看汇编指令：**


```cpp
(gdb) p $rip
$2 = (void(*)()) 0x400467 <main+4>
(gdb) x/i $rip
0x400467 <main+4>:    mov    $0x5,%r8d
(gdb)
```

我们还可以利用一个名为[pc](http://sourceware.org/gdb/current/onlinedocs/gdb/Registers.html#Registers)的gdb内部变量：



```cpp
(gdb) p $pc
$3 = (void(*)()) 0x400467 <main+4>
(gdb) x/i $pc
0x400467 <main+4>:    mov    $0x5,%r8d
(gdb)
```

结合display命令和寄存器或pc内部变量，我们做如下设置：



```cpp
(gdb) display /i $pc
1: x/i $pc
0x400467 <main+4>:    mov    $0x5,%r8d
(gdb)
```
同时显示多条汇编，比如3条：


```cpp
(gdb) display /3i $pc
(gdb) b main
Breakpoint 1 at 0x400467: file t3.5.c, line 10.
(gdb) r
Starting program: /root/test/a.out 
 
Breakpoint 1, main () at t3.5.c:10
10      callee(1,2,3,4,5);
1: x/3i $pc
0x400467 <main+4>:    mov    $0x5,%r8d
0x40046d <main+10>:   mov    $0x4,%ecx
0x400472 <main+15>:   mov    $0x3,%edx
(gdb)
```
接下来，利用[ni](http://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html#Continuing-and-Stepping)（nexti）或si（stepi）命令进行汇编指令级的调试，如下所示可以看到参数是如何传递的：


```cpp
(gdb) display /i $pc
1: x/i $pc
0x400467 <main+4>:    mov    $0x5,%r8d
(gdb) ni
0x000000000040046d  10      callee(1,2,3,4,5);
1: x/i $pc
0x40046d <main+10>:   mov    $0x4,%ecx
(gdb) ni
0x0000000000400472  10      callee(1,2,3,4,5);
1: x/i $pc
0x400472 <main+15>:   mov    $0x3,%edx
(gdb) ni
0x0000000000400477  10      callee(1,2,3,4,5);
1: x/i $pc
0x400477 <main+20>:   mov    $0x2,%esi
(gdb) ni
0x000000000040047c  10      callee(1,2,3,4,5);
1: x/i $pc
0x40047c <main+25>:   mov    $0x1,%edi
(gdb)
```
更简单直接的方法是利用[layout](http://sourceware.org/gdb/current/onlinedocs/gdb/TUI-Commands.html#TUI-Commands)显示汇编代码窗口：


```cpp
(gdb) help layout
Change the layout of windows.
Usage: layout prev | next | <layout_name>
Layout names are:
   src   : Displays source and command windows.
   asm   : Displays disassembly and command windows.
   split : Displays source, disassembly and command windows.
   regs  : Displays registerwindow. If existing layout
           is source/command or assembly/command, the
           registerwindow is displayed. If the
           source/assembly/command (split) is displayed,
           theregisterwindow is displayed with
           the window that has current logical focus.
 
(gdb) layout asm   lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqk
   x0x400463 <main>                 push   %rbp                                                x
   x0x400464 <main+1>               mov    %rsp,%rbp                                           x
   x0x400467 <main+4>               mov    $0x5,%r8d                                           x
   x0x40046d <main+10>              mov    $0x4,%ecx                                           x
   x0x400472 <main+15>              mov    $0x3,%edx                                           x
   x0x400477 <main+20>              mov    $0x2,%esi                                           x
   x0x40047c <main+25>              mov    $0x1,%edi                                           x
   x0x400481 <main+30>              callq  0x400448 <callee>                                   x
   x0x400486 <main+35>              mov    $0x2,%eax                                           x
   x0x40048b <main+40>              leaveq                                                     x
   x0x40048c <main+41>              retq                                                       x
   x0x40048d                        nop                                                        x
   x0x40048e                        nop                                                        x
   x0x40048f                        nop                                                        x
   x0x400490 <__libc_csu_fini>      repz retq                                                  x
   x0x400492                        nopl   0x0(%rax)                                           x
   x0x400499                        nopl   0x0(%rax)                                           x
   x0x4004a0 <__libc_csu_init>      mov    %r12,-0x20(%rsp)                                    x
   x0x4004a5 <__libc_csu_init+5>    mov    %r13,-0x18(%rsp)                                    x
   x0x4004aa <__libc_csu_init+10>   lea    0x2001bb(%rip),%r12        # 0x60066c               x
   x0x4004b1 <__libc_csu_init+17>   mov    %r14,-0x10(%rsp)                                    x
   x0x4004b6 <__libc_csu_init+22>   mov    %r15,-0x8(%rsp)                                     x
   x0x4004bb <__libc_csu_init+27>   mov    %rsi,%r14                                           x
   x0x4004be <__libc_csu_init+30>   mov    %rbx,-0x30(%rsp)                                    x
   x0x4004c3 <__libc_csu_init+35>   mov    %rbp,-0x28(%rsp)                                    x
   x0x4004c8 <__libc_csu_init+40>   sub    $0x38,%rsp                                          x
 mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj
exec No process In:                                                           Line: ??   PC: 0x0
(gdb)
```






