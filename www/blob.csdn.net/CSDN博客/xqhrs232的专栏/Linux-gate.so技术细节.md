# Linux-gate.so技术细节 - xqhrs232的专栏 - CSDN博客
2017年02月17日 14:16:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：243
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/vinozly/p/6159492.html](http://www.cnblogs.com/vinozly/p/6159492.html)
相关文章
1、What is linux-gate.so.1?-----[http://www.trilithium.com/johan/2005/08/linux-gate/](http://www.trilithium.com/johan/2005/08/linux-gate/)
2、[如何获取Linux-gate.so.1动态库](http://blog.csdn.net/mao0514/article/details/51220140)----[http://blog.csdn.net/mao0514/article/details/51220140](http://blog.csdn.net/mao0514/article/details/51220140)

1. linux-gate.so是什么
参考这里：http://www.trilithium.com/johan/2005/08/linux-gate/
简而言之，linux-gate.so是为了实现用户程序使用sysenter/sysexit进行
系统调用的辅助机制。为什么我们需要这么一种机制来完成sysenter/sysexit？
按照我们使用int 80进行系统调用的思维，我们期待sysenter/sysexit是这样的
一个过程：
          user app:                        kernel:
            /*things*/                     
            /*setup parameters*/
            movl $__NR_getpid, %eax
            sysenter                ------>
                                           movl current->pid, %eax
                                           sysexit
                                    <------
            /*%eax=pid*/
            /*other things*/
我们编写一个例子试试上面的想法：
    [root@w237 vdso.d]# cat pid.c
    #include <stdio.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/syscall.h>
    #define STRINGFY_(x) #x
    #define STRINGFY(x) STRINGFY_(x)
    int main()
    {
        pid_t pid;
        __asm__ volatile("movl $"STRINGFY(__NR_getpid)", %%eax\n"
                         "sysenter\n"
                         : "=a"(pid));
        printf("pid=%u\n", pid);
        return 0;
    }
编译，gdb调试：
    [root@w237 vdso.d]# gcc -g -o pid pid.c
    [root@w237 vdso.d]# gdb -q ./pid
    Using host libthread_db library "/lib/tls/libthread_db.so.1".
    (gdb) disassemble main
    Dump of assembler code for function main:
    0x08048368 <main+0>:    push   %ebp
    0x08048369 <main+1>:    mov    %esp,%ebp
    0x0804836b <main+3>:    sub    $0x8,%esp
    0x0804836e <main+6>:    and    $0xfffffff0,%esp
    0x08048371 <main+9>:    mov    $0x0,%eax
    0x08048376 <main+14>:   add    $0xf,%eax
    0x08048379 <main+17>:   add    $0xf,%eax
    0x0804837c <main+20>:   shr    $0x4,%eax
    0x0804837f <main+23>:   shl    $0x4,%eax
    0x08048382 <main+26>:   sub    %eax,%esp
    0x08048384 <main+28>:   mov    $0x14,%eax
    0x08048389 <main+33>:   sysenter
    0x0804838b <main+35>:   mov    %eax,0xfffffffc(%ebp)
    0x0804838e <main+38>:   sub    $0x8,%esp
    0x08048391 <main+41>:   pushl  0xfffffffc(%ebp)
    0x08048394 <main+44>:   push   $0x8048488
    0x08048399 <main+49>:   call   0x80482b0
    0x0804839e <main+54>:   add    $0x10,%esp
    0x080483a1 <main+57>:   mov    $0x0,%eax
    0x080483a6 <main+62>:   leave
    0x080483a7 <main+63>:   ret
    End of assembler dump.
    (gdb)
我们在sysenter一行设置断点，并且运行跟踪：
    (gdb) b *0x8048389
    Breakpoint 1 at 0x8048389: file pid.c, line 13.
    (gdb) r
    Starting program: /home/wensg/vdso.d/pid
    Reading symbols from shared object read from target memory...done.
    Loaded system supplied DSO at 0xffffe000
    Breakpoint 1, 0x08048389 in main () at pid.c:13
    13          __asm__ volatile("movl $"STRINGFY(__NR_getpid)", %%eax\n"
这时候gdb中断在sysenter这一行，用stepi单步运行这条指令：
    (gdb) stepi
    0xffffe424 in __kernel_vsyscall ()
看见了么？当sysenter执行完毕(也就是sysexit的结果)以后，程序是停在了0xffffe424这一行，
这个地址位于函数__kernel_vsyscall中！！为什么不是sysenter的下一行0x804838b？？？
2. sysenter/sysexit指令
参考IA32的文档。
sysenter/sysexit被冠以“Fast System Call facility”。至于是否如此，我现在不关心。
sysenter调用的过程为：
设置下面寄存器值(%msr[SYSENTER_CS]表示名为SYSENTER_CS的msr值，model specific 
register，一组特别的寄存器组):
    %cs   = %msr[SYSENTER_CS]
    %eip  = %msr[SYSENTER_EIP]
    %ss   = %msr[SYSENTER_SS] + 8
    %esp  = %msr[SYSENTER_ESP]
    %CPL  = 0
然后从%cs:%eip继续执行。
sysexit调用过程为：
设置下面寄存器值：
    %cs   = %msr[SYSENTER_CS] + 16
    %eip  = %edx
    %ss   = %msr[SYSENTER_CS] + 24
    %esp  = %ecx
    %CPL  = 3
然后从%cs:%eip继续执行。
我们看到sysenter调用进入内核时，CPU不会保存用户堆栈，返回地址和其它的寄存器，
那么sysexit怎么返回到正确的用户空间呢？
一种办法就是调用前把%eip, %esp（因为%cs, %ss只是内核用来糊弄MMU的，我们先不管了）
保存在别的寄存器中，不过这样需要2个寄存器才能完成任务。
另外一种办法就是sysexit总是返回到用户进程某个固定的地址！vdso就是作为
sysenter/sysexit的存根(stub)的。sysenter只会在某个固定的位置被调用，而sysexit
也只需要返回到调用sysenter+2的位置（sysenter的机器码占2个字节）。不过%esp还是
需要保存的。
这就是为什么我们在例子1中观察到了sysenter指令会跳转到了__kernel_vsyscall()函数中，
sysexit返回的固定地址就在这个__kernel_vsyscall中。
让我们看看__kernel_vsyscall的汇编代码：
    (gdb) disassemble __kernel_vsyscall
    Dump of assembler code for function __kernel_vsyscall:
    0xffffe414 <__kernel_vsyscall+0>:       push   %ecx
    0xffffe415 <__kernel_vsyscall+1>:       push   %edx
    0xffffe416 <__kernel_vsyscall+2>:       push   %ebp
    0xffffe417 <__kernel_vsyscall+3>:       mov    %esp,%ebp
    0xffffe419 <__kernel_vsyscall+5>:       sysenter
    0xffffe41b <__kernel_vsyscall+7>:       nop
    0xffffe41c <__kernel_vsyscall+8>:       nop
    0xffffe41d <__kernel_vsyscall+9>:       nop
    0xffffe41e <__kernel_vsyscall+10>:      nop
    0xffffe41f <__kernel_vsyscall+11>:      nop
    0xffffe420 <__kernel_vsyscall+12>:      nop
    0xffffe421 <__kernel_vsyscall+13>:      nop
    0xffffe422 <__kernel_vsyscall+14>:      jmp    0xffffe417 <__kernel_vsyscall+3>
    0xffffe424 <__kernel_vsyscall+16>:      pop    %ebp  ; sysexit返回到这里
    0xffffe425 <__kernel_vsyscall+17>:      pop    %edx
    0xffffe426 <__kernel_vsyscall+18>:      pop    %ecx
    0xffffe427 <__kernel_vsyscall+19>:      ret
    End of assembler dump.
    (gdb)
看到没有，在0xffffe424这一行的上方有一个sysenter指令。Linux的设计是：进程只应当
从一个地方调用sysenter, sysexit返回到这个调用下面的某个地方，这两个地址都是固定的。
__kernel_vsyscall的sysenter到sysexit返回的地址0xffffe424中间有数个nop和jmp指令
的作用，下面再解释。
3. 如何使用sysenter
从例1的例子来看，我们是无法直接使用sysenter的，因为我们无法知道这个返回地址和
调用的协议。实际上，这样的指令对于普通的程序员来说，完全是透明的。vdso是C库的开发
者关心的问题。
__kernel_vsyscall的设计目标是代替int 80, 也就是下面两种方式应该是等价的：
     /* int80 */                  /* __kernel_vsyscall */
     movl $__NR_getpid, %eax      movl $__NR_getpid, %eax
     int $0x80                    call __kernel_vsyscall
     /* %eax=getpid() */          /* %eax=getpid() %/
C库有怎么知道有__kernel_vsyscall呢？很简单，kernel告诉C库，kernel中存在
__kernel_vsyscall。至于C库选择int80，还是sysenter进行系统调用，那就是C库管了，
kernel已经提供了这样的一种机制，策略就不管是它管的了。
kernel告诉C库__kernel_vsyscall的位置，则是通过elf的interpreter的auxiliary vector
这个的具体细节看以参考elf的技术文档，我们可以通过下面的手段观察auxiliary vector
    [root@w237 vdso.d]# LD_SHOW_AUXV=1 /bin/ls
    AT_SYSINFO:      0xffffe414
    AT_SYSINFO_EHDR: 0xffffe000
    AT_HWCAP:    fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe
    AT_PAGESZ:       4096
    AT_CLKTCK:       100
    AT_PHDR:         0x8048034
    AT_PHENT:        32
    AT_PHNUM:        8
    AT_BASE:         0x0
    AT_FLAGS:        0x0
    AT_ENTRY:        0x8049cf0
    AT_UID:          0
    AT_EUID:         0
    AT_GID:          0
    AT_EGID:         0
    AT_SECURE:       0
    AT_PLATFORM:     i686
AT_SYSINFO就是__kernel_vsyscall函数的地址，AT_SYSINFO_EHDR是vdso加载的位置。
4. 总体的结构：
用下面的图来解释：
 这张图不清楚，贴一张真正的图：
![](https://img-blog.csdn.net/20151216160037421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
linux-gate.so(vdso)是内核镜像中的特定页，它是一个完整的elf share object，
因此在磁盘的任何位置都找不到一个它。它是由内核的某些文件编译生成的。
当使用exec()执行新的镜像时，内核把linux-gate.so的页面映射到程序的进程空间中。
内核把__kernel_vsyscall的地址以auxiliary vector的形式告诉interpreter
（C库）。
当C库要进入内核时，它就可以选择使用__kernel_vsyscall或者int80来进行系统调用。
5. 内核的细节
让我们想想内核需要做那些工作：
  5.1 生成vdso，并链接到内核中。
  5.2 设置MSR，以便sysenter能进入内核的正确位置，sysexit能返回到用户程序的正确位置。
  5.3 exec()时，将vdso映射到用户程序的地址空间中，找到__kernel_vsyscall的地址，
      传给interpreter。
  5.4 调用时，正确传递参数。
  5.5 sysenter的响应函数要正确解析参数，调用相应的系统函数完成服务；设置%ecx, %edx，
      用sysexit返回
  5.6 当程序exit时，解除vdso的映射。
当你理解上面的内容之后，理解内核的细节不过是把它们找出来而已。自己去翻内核看，也是理解
上面内容的一个很好的途径。
6. __kernel_vsyscall
前面还遗留了一个问题，那7个nop和jmp是干什么的呢？让我们再看看它的代码：
    0xffffe414 <__kernel_vsyscall+0>:       push   %ecx
    0xffffe415 <__kernel_vsyscall+1>:       push   %edx
    0xffffe416 <__kernel_vsyscall+2>:       push   %ebp
    0xffffe417 <__kernel_vsyscall+3>:       mov    %esp,%ebp
    0xffffe419 <__kernel_vsyscall+5>:       sysenter
    0xffffe41b <__kernel_vsyscall+7>:       nop
    0xffffe41c <__kernel_vsyscall+8>:       nop
    0xffffe41d <__kernel_vsyscall+9>:       nop
    0xffffe41e <__kernel_vsyscall+10>:      nop
    0xffffe41f <__kernel_vsyscall+11>:      nop
    0xffffe420 <__kernel_vsyscall+12>:      nop
    0xffffe421 <__kernel_vsyscall+13>:      nop
    0xffffe422 <__kernel_vsyscall+14>:      jmp    0xffffe417 <__kernel_vsyscall+3>
    0xffffe424 <__kernel_vsyscall+16>:      pop    %ebp  ; sysexit返回到这里
    0xffffe425 <__kernel_vsyscall+17>:      pop    %edx
    0xffffe426 <__kernel_vsyscall+18>:      pop    %ecx
    0xffffe427 <__kernel_vsyscall+19>:      ret
前面连个push %ecx和%edx是因为sysexit返回时，要用这两个寄存器来制定返回的eip和esp，因此先保存起来。
然后我们要把%esp的值保存在%ebp中，否则我们就无法获得当前的堆栈指针了，在覆盖%ebp前，先保存%ebp，
这是系统调用的第六个参数。
然后使用sysenter
然后一堆的nop和一个jmp，这里完全是一个死循环。这是干什么的？正常的sysexit又不会执行这里（直接到
jmp之后了）
这个问题linus在这封mail中讨论了：
http://lkml.org/lkml/2002/12/18/218
他的意思是jmp的设计是用来支持restarted system call的，如果一个system call需要restart，它只需要返
回到某个nop中，然后jmp到重新初始化%ebp的代码中，从而是sysenter再次执行。
不过什么情况下会使一个system call restart，征个人告诉我。
下面link也不错，可对照参考一下；
http://www.ibm.com/developerworks/cn/linux/kernel/l-k26ncpu/index.html
Linux 2.6 对新型 CPU 快速系统调用的支持
