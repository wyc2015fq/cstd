# 由于ptrace.h文件导致的内核编译出错的解决方法 - xqhrs232的专栏 - CSDN博客
2016年06月16日 16:38:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：893
原文地址::[http://www.cnblogs.com/woshizyl/archive/2013/03/11/2953554.html](http://www.cnblogs.com/woshizyl/archive/2013/03/11/2953554.html)
编译内核到了make bzImage时出现如下错误：
arch/x86/kernel/ptrace.c:1472:17: error: conflicting types for ‘syscall_trace_enter’
In file included from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/vm86.h:130:0, 
                  from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/processor.h:10, 
                  from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/thread_info.h:22, 
                  from include/linux/thread_info.h:56, 
                  from include/linux/preempt.h:9, 
                  from include/linux/spinlock.h:50, 
                  from include/linux/seqlock.h:29, 
                  from include/linux/time.h:8, 
                  from include/linux/timex.h:56, 
                  from include/linux/sched.h:56, 
                  from arch/x86/kernel/ptrace.c:11: 
/home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/ptrace.h:146:13: note: previous declaration of ‘syscall_trace_enter’ was here 
arch/x86/kernel/ptrace.c:1517:17: error: conflicting types for ‘syscall_trace_leave’ 
In file included from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/vm86.h:130:0, 
                  from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/processor.h:10, 
                  from /home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/thread_info.h:22, 
                  from include/linux/thread_info.h:56, 
                  from include/linux/preempt.h:9, 
                  from include/linux/spinlock.h:50, 
                  from include/linux/seqlock.h:29, 
                  from include/linux/time.h:8, 
                  from include/linux/timex.h:56, 
                  from include/linux/sched.h:56, 
                  from arch/x86/kernel/ptrace.c:11: 
/home/rex/Downloads/linux-2.6.32.60/arch/x86/include/asm/ptrace.h:147:13: note: previous declaration of ‘syscall_trace_leave’ was here 
make[2]: *** [arch/x86/kernel/ptrace.o] Error 1 
make[1]: *** [arch/x86/kernel] Error 2 
make: *** [arch/x86] Error 2
注意红色标注的两行，说明ptrace.c 文件中第1472行的函数syscall_trace_leave和ptrace.h文件中该函数的声明有冲突，只要将ptrace.h文件中的函数声明改得和ptrace.c中的相同即可。另外，有人专门针对这个错误写了个补丁，地址为[https://patchwork.kernel.org/patch/1301031/](https://patchwork.kernel.org/patch/1301031/) 
 。
PS ：以前像遇到这种问题的时候，总会认为是自己哪儿出了问题。这个问题让我懂得linux也是像我一样的程序员写出来的，他们也会犯一些常人会犯的错误，所以Linux需要全世界的程序员一起改进  。
