# Linux 系统下 init 进程的前世今生 - 文章 - 伯乐在线
原文出处： [MagicBoy2010](http://blog.chinaunix.net/uid-23769728-id-3129443.html)
Linux系统中的init进程(pid=1)是除了idle进程(pid=0，也就是init_task)之外另一个比较特殊的进程，它是Linux内核开始建立起进程概念时第一个通过kernel_thread产生的进程，其开始在内核态执行，然后通过一个系统调用，开始执行用户空间的/sbin/init程序，期间Linux内核也经历了从内核态到用户态的特权级转变，/sbin/init极有可能产生出了shell，然后所有的用户进程都有该进程派生出来(目前尚未阅读过/sbin/init的源码)…
目前我们至少知道在内核空间执行用户空间的一段应用程序有两种方法：
1. call_usermodehelper
2. kernel_execve
它们最终都通过int $0x80在内核空间发起一个系统调用来完成，这个过程我在《深入Linux设备驱动程序内核机制》第9章有过详细的描述，对它的讨论最终结束在 sys_execve函数那里，后者被用来执行一个新的程序。现在一个有趣的问题是，在内核空间发起的系统调用，最终通过sys_execve来执行用户 空间的一个程序，比如/sbin/myhotplug，那么该应用程序执行时是在内核态呢还是用户态呢？直觉上肯定是用户态，不过因为cpu在执行 sys_execve时cs寄存器还是__KERNEL_CS，如果前面我们的猜测是真的话，必然会有个cs寄存器的值从__KERNEL_CS到 __USER_CS的转变过程，这个过程是如何发生的呢？下面我以kernel_execve为例，来具体讨论一下其间所发生的一些有趣的事情。
start_kernel在其最后一个函数rest_init的调用中，会通过kernel_thread来生成一个内核进程，后者则会在新进程环境下调 用kernel_init函数，kernel_init一个让人感兴趣的地方在于它会调用run_init_process来执行根文件系统下的 /sbin/init等程序：


```
static noinline int init_post(void)
{
        ...
        run_init_process("/sbin/init");
        run_init_process("/etc/init");
        run_init_process("/bin/init");
        run_init_process("/bin/sh");
        panic("No init found. Try passing init= option to kernel. "
              "See Linux Documentation/init.txt for guidance.");
}
```
run_init_process的核心调用就是kernel_execve，后者的实现代码是：


```
int kernel_execve(const char *filename,
                  const char *const argv[],
                  const char *const envp[])
{
        long __res;
        asm volatile ("int $0x80"
        : "=a" (__res)
        : "0" (__NR_execve), "b" (filename), "c" (argv), "d" (envp) : "memory");
        return __res;
}
```
里面是段内嵌的汇编代码，代码相对比较简单，核心代码是int $0x80，执行系统调用，系统调用号__NR_execve放在AX里，当然系统调用的返回值也是在AX中，要执行的用户空间应用程序路径名称保存在 BX中。int $0x80的执行导致代码向__KERNEL_CS:system_call转移(具体过程可参考[x86处理器中的特权级检查及Linux系统调用的实现](http://blog.chinaunix.net/uid-23769728-id-3077701.html)一帖). 此处用bx,cx以及dx来保存filename, argv以及envp参数是有讲究的，它对应着struct pt_regs中寄存器在栈中的布局，因为接下来就会涉及从汇编到调用C函数过程，所以汇编程序在调用C之前，应该把要传递给C的参数在栈中准备好。
system_call是一段纯汇编代码：


```
<arch/x86/kernel/entry_32.s>
ENTRY(system_call)
        RING0_INT_FRAME # can't unwind into user space anyway
        pushl_cfi %eax # save orig_eax
        SAVE_ALL
        GET_THREAD_INFO(%ebp)
                                        # system call tracing in operation / emulation
        testl $_TIF_WORK_SYSCALL_ENTRY,TI_flags(%ebp)
        jnz syscall_trace_entry
        cmpl $(nr_syscalls), %eax
        jae syscall_badsys
syscall_call:
        call *sys_call_table(,%eax,4)
        movl %eax,PT_EAX(%esp) # store the return value
syscall_exit:
        ...
restore_nocheck:
        RESTORE_REGS 4 # skip orig_eax/error_code
irq_return:
        INTERRUPT_RETURN #iret instruction for x86_32
```
system_call首先会为后续的C函数的调用在当前堆栈中建立参数传递的环境(x86_64的实现要相对复杂一点，它会将系统调用切换到内核栈 movq PER_CPU_VAR(kernel_stack),%rsp)，尤其是接下来对C函数sys_execve调用中的struct pt_regs *regs参数，我在上面代码中同时列出了系统调用之后的后续操作syscall_exit，从代码中可以看到系统调用int $0x80最终通过iret指令返回，而后者会从当前栈中弹出cs与ip，然后跳转到cs:ip处执行代码。正常情况下，x86架构上的int n指 令会将其下条指令的cs:ip压入堆栈，所以当通过iret指令返回时，原来的代码将从int n的下条指令继续执行，不过如果我们能在后续的C代码中改变regs->cs与regs->ip(也就是int n执行时压入栈中的cs与ip)，那么就可以控制下一步代码执行的走向，而 sys_execve函数的调用链正好利用了这一点，接下来我们很快就会看到。SAVE_ALL宏的最后为将ds, es, fs都设置为__USER_DS，但是此时cs还是__KERNEL_CS.
核心的调用发生在call *sys_call_table(,%eax,4)这条指令上，sys_call_table是个系统调用表，本质上就是一个函数指针数组，我们这里的系 统调用号是__NR_execve=11, 所以在sys_call_table中对应的函数为：


```
ENTRY(sys_call_table)
        .long sys_restart_syscall /* 0 - old "setup()" system call, used for restarting */
        .long sys_exit
        .long ptregs_fork
        .long sys_read
        .long sys_write
        .long sys_open /* 5 */
        .long sys_close
        ...
        .long sys_unlink /* 10 */
        .long ptregs_execve //__NR_execve
        ...
```
ptregs_execve其实就是sys_execve函数：


```
#define ptregs_execve sys_execve
```
而sys_execve函数的代码实现则是：


```
/*
 * sys_execve() executes a new program.
 */
long sys_execve(const char __user *name,
                const char __user *const __user *argv,
                const char __user *const __user *envp, struct pt_regs *regs)
{
        long error;
        char *filename;
        filename = getname(name);
        error = PTR_ERR(filename);
        if (IS_ERR(filename))
                return error;
        error = do_execve(filename, argv, envp, regs);
#ifdef CONFIG_X86_32
        if (error == 0) {
                /* Make sure we don't return using sysenter.. */
                set_thread_flag(TIF_IRET);
        }
#endif
        putname(filename);
        return error;
}
```
注意这里的参数传递机制！其中的核心调用是do_execve,后者调用do_execve_common来干执行一个新程序的活，在我们这个例子中要执 行的新程序来自/sbin/init，如果用file命令看一下会发现它其实是个ELF格式的动态链接库，而不是那种普通的可执行文件，所以 do_execve_common会负责打开、解析这个文件并找到其可执行入口点，这个过程相当繁琐，我们不妨直接看那些跟我们问题密切相关的代 码，do_execve_common会调用search_binary_handler去查找所谓的binary formats handler，ELF显然是最常见的一种格式：


```
int search_binary_handler(struct linux_binprm *bprm,struct pt_regs *regs)
{
       ...
       for (try=0; try<2; try++) {
                read_lock(&binfmt_lock);
                list_for_each_entry(fmt, &formats, lh) {
                        int (*fn)(struct linux_binprm *, struct pt_regs *) = fmt->load_binary;
                        ...
                        retval = fn(bprm, regs);
                        ...
               }
               ...
       }
}
```
代码中针对ELF格式的 fmt->load_binary即为load_elf_binary, 所以fn=load_elf_binary, 后续对fn的调用即是调用load_elf_binary，这是个非常长的函数，直到其最后，我们才找到所需要的答案：


```
static int load_elf_binary(struct linux_binprm *bprm, struct pt_regs *regs)
{
        ...
        start_thread(regs, elf_entry, bprm->p);
        ...
}
```
上述代码中的elf_entry即为/sbin/init中的执行入口点， bprm->p为应用程序新栈(应该已经在用户空间了)，start_thread的实现为：


```
void
start_thread(struct pt_regs *regs, unsigned long new_ip, unsigned long new_sp)
{
        set_user_gs(regs, 0);
        regs->fs = 0;
        regs->ds = __USER_DS;
        regs->es = __USER_DS;
        regs->ss = __USER_DS;
        regs->cs = __USER_CS;
        regs->ip = new_ip;
        regs->sp = new_sp;
        /*
         * Free the old FP and other extended state
         */
        free_thread_xstate(current);
}
```
在这里，我们看到了__USER_CS的身影，在x86 64位系统架构下，该值为0x33. start_thread函数最关键的地方在于修改了regs->cs= __USER_CS, regs->ip= new_ip，其实就是人为地改变了系统调用int $0x80指令压入堆栈的下条指令的地址，这样当系统调用结束通过iret指令返回时，代码将从这里的__USER_CS:elf_entry处开始执 行，也就是/sbin/init中的入口点。start_thread的代码与kernel_thread非常神似，不过它不需要象 kernel_thread那样在最后调用do_fork来产生一个task_struct实例出来了，因为目前只需要在当前进程上下文中执行代码，而不是创建一个新进程。关于kernel_thread，我在本版曾有一篇帖子分析过，当时基于的是ARM架构。
所以我们看到，start_kernel在最后调用rest_init，而后者通过对kernel_thread的调用产生一个新进程(pid=1)，新进程在其kernel_init()–>init_post()调用链中将通过run_init_process来执行用户空间的/sbin /init，run_init_process的核心是个系统调用，当系统调用返回时代码将从/sbin/init的入口点处开始执行，所以虽然我们知道 post_init中有如下几个run_init_process的调用：


```
run_init_process("/sbin/init");
run_init_process("/etc/init");
run_init_process("/bin/init");
run_init_process("/bin/sh");
```
但是只要比如/sbin/init被成功调用，run_init_process中的kernel_execve函数将无法返回，因为它执行int $0x80时压入堆栈中回家的路径被后续的C函数调用链给改写了，这样4个run_init_process只会有一个有机会被成功执行，如果这4个函数都失败 了，那么内核将会panic. 所以内核设计时必须确保用来改写int $0x80压入栈中的cs和ip的start_thread函数之后不会再有其他额外的代码导致整个调用链的失败，否则代码将执行非预期的指令，内核进入不稳定状态。
最后，我们来验证一下，所谓眼见为实，耳听为虚。再者，如果验证达到预期，也是很鼓舞人好奇心的极佳方法。验证的方法我打算采用“[Linux设备驱动模型中的热插拔机制及实验](http://blog.chinaunix.net/uid-23769728-id-3087574.html)” 中的路线，通过call_usermodehelper来做，因为它和kernel_execve本质上都是一样的。我们自己写个应用程序，在这个应用程序里读取cs寄存器的值，程序很简单：


```
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
int main()
{
    unsigned short ucs;
    asm(
        "movw %%cs, %0\n"
        :"=r"(ucs)
        ::"memory");
    syslog(LOG_INFO, "ucs = 0x%x\n", ucs);
    return 0;
}
```
然后把这个程序打到/sys/kernel/uevent_help上面(参照[Linux设备驱动模型中的热插拔机制及实验](http://blog.chinaunix.net/uid-23769728-id-3087574.html)一文），之后我们往电脑里插个U盘，然后到/var/log/syslog文件里看输出(在某些distribution上，syslog的输出可能会到/var/log/messages中)：
Mar 10 14:20:23 build-server main: **ucs = 0x33**
0x33正好就是x86 64位系统(我实验用的环境)下的__USER_CS.
所以第一个内核进程(pid=1)通过执行用户空间程序，期间通过cs的转变(从__KERNEL_CS到__USER_CS)来达到特权级的更替。
