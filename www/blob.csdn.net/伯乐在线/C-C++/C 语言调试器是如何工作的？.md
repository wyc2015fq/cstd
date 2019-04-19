# C 语言调试器是如何工作的？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [SCaffrey](http://www.jobbole.com/members/caffrey) 翻译，[唐尤华](http://www.jobbole.com/members/tangyouhua) 校稿。未经许可，禁止转载！
英文出处：[blog.0x972.info](http://blog.0x972.info/?d=2014/11/13/10/40/50-how-does-a-debugger-work)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
当你用 GDB 的时候，可以看到它完全控制了应用程序进程。当你在程序运行的时候用 `Ctrl + C`，程序的运行就能够终止，而GDB能展示它的当前地址、堆栈跟踪信息之类的内容。
但是它是怎么办到的呢？
## 但是它们怎么不工作呢？
开始，让我们先研究它怎样才会不工作。它不能通过阅读和分析程序的二进制信息来模拟程序的运行。它其实能做，而那应该能起作用（`Valgrind `内存调试器就是这样工作的），但是这样的话会很慢。`Valgrind`会让程序慢1000倍，但是GDB不会。它的工作机制与Qemu虚拟机一样。
所以到底是怎么回事？黑魔法？……不，如果那样的话就太简单了。
另一种猜想？……？破解！是的，这里正是这样的。操作系统内核也提供了一些帮助。
首先，关于Linux的进程机制需要了解一件事：父进程可以获得子进程的附加信息，也能够`ptrace`它们。并且你可以猜到的是，调试器是被调试的进程的父进程（或者它会变成父进程，在Linux中进程可以将一个进程变为自己子进程:-)）
## Linux Ptrace API
Linux Ptrace API 允许一个（调试器）进程来获取低等级的其他（被调试的）进程的信息。特别的，这个调试器可以：
> - 读写被调试进程的内存 ：`PTRACE_PEEKTEXT`、`PTRACE_PEEKUSER`、`PTRACE_POKE`……
- 读写被调试进程的CPU寄存器 `PTRACE_GETREGSET、PTRACE_SETREGS`
- 因系统活动而被提醒：PTRACE_O_TRACEEXEC, PTRACE_O_TRACECLONE, PTRACE_O_EXITKILL, PTRACE_SYSCALL（你可以通过这些标识区分exec syscall、clone、exit以及其他系统调用）
- 控制它的执行：PTRACE_SINGLESTEP、PTRACE_KILL、PTRACE_INTERRUPT、PTRACE_CONT （注意，CPU在这里是单步执行）
- 修改它的信号处理`：PTRACE_GETSIGINFO、PTRACE_SETSIGINFO`
# Ptrace是如何实现的？
Ptrace的实现不在本文讨论的范围内，所以我不想进一步讨论，只是简单地解释它是如何工作的（我不是内核专家，如果我说错了请一定指出来，并原谅我过分简化:-)）
Ptrace [是Linux内核的一部分](https://github.com/torvalds/linux/blob/master/kernel/ptrace.c)，所以它能够获取进程所有内核级信息：
- 读写数据？Linux有[`copy_to/from_user`](http://www.fsl.cs.sunysb.edu/kernel-api/re256.html)`。`
- 获取CPU寄存器？用[copy_regset_to/from_user](https://github.com/torvalds/linux/blob/98959948a7ba33cf8c708626e0d2a1456397e1c6/include/linux/regset.h#L331)很轻松（这里没有什么复杂的，因为CPU寄存器在进程未被调度时保存在Linux的`struct task_struct *`调度结构中）。
- 修改信号处理？更新域[last_siginfo](https://github.com/torvalds/linux/blob/9a3c4145af32125c5ee39c0272662b47307a8323/include/linux/sched.h#L1494)
- 单步执行？在处理器出发执行前，设置进程task结构的right flag（[ARM](http://lxr.free-electrons.com/source/arch/arm/kernel/hw_breakpoint.c#L671)、[x86](http://lxr.free-electrons.com/source/arch/x86/kernel/step.c#L104)）
- Ptrace是在很多计划的操作中被Hooked（搜索[`ptrace_event`](https://github.com/torvalds/linux/search?utf8=%E2%9C%93&q=ptrace_event)函数），所以它可以在被询问时（`PTRACE_O_TRACEEXEC`选项和与它相关的），向调试器发出一个[`SIGTRAP`信号](https://github.com/torvalds/linux/blob/9a3c4145af32125c5ee39c0272662b47307a8323/include/linux/ptrace.h#L120)。
# 没有Ptrace的系统会怎么样呢？
这个解释超出了特定的Linux本地调试，但是对于大部分其他环境是合理的。要了解GDB在不同目标平台请求的内容，你可以看一下它在[目标栈](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/target.h;h=d363b61bc32bb856b85bc9d49cc97ca2577d7c5a;hb=HEAD#l395)里面的操作。
在这个目标接口里，你可以看到所有C调试需要的高级操作：

C
```
struct target_ops 
{
  struct target_ops *beneath;   /* To the target under this one.  */
  const char *to_shortname; /* Name this target type */
  const char *to_longname;  /* Name for printing */
  const char *to_doc;       /* Documentation.  Does not include trailing
               newline, and starts with a one-line descrip-
               tion (probably similar to to_longname).  */
 void (*to_attach) (struct target_ops *ops, const char *, int);
 void (*to_fetch_registers) (struct target_ops *, struct regcache *, int);
 void (*to_store_registers) (struct target_ops *, struct regcache *, int);
 int (*to_insert_breakpoint) (struct target_ops *, struct gdbarch *,
             struct bp_target_info *);
 int (*to_insert_watchpoint) (struct target_ops *,
             CORE_ADDR, int, int, struct expression *);
 ...
}
```
普通的GDB调用这些函数，然后目标相关的组件再实现它们。（概念上）这是一个栈，或者一个金字塔：栈顶的是非常通用的，比如：
> - 系统特定的[Linux](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/linux-fork.c;h=d7eaeb1d86999498b613f099b1b4cd50882184af;hb=HEAD)
- 本地或[远程](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/remote.c;h=4b9b0991c65ffeeb6a817824266d450a76bc406a;hb=HEAD)调试
- 调试方式特定的（[ptrace](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/inf-ptrace.c;h=6eb8080242349296e43dcc19df4a0896e6093fa8;hb=HEAD)、[ttrace](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/inf-ttrace.c;h=dceea421b3a47f8206dffab73bf3d9a8c4cc54a3;hb=HEAD)）
- 指令集特定的（Linux [ARM](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/arm-linux-nat.c;h=b9b99966bb45d41bace5a3524e34ecc612851cda;hb=HEAD)、Linux [x86](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/x86-linux-nat.c;h=b2141ebcd335b2fffde7041ee3501031dff61daa;hb=HEAD)）
那个[远程](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/remote.c;h=4b9b0991c65ffeeb6a817824266d450a76bc406a;hb=HEAD)目标很有趣，因为它通过一个连接协议（TCP/IP、串行端口）把两台“电脑”间的执行栈分离开来。
那个远程的部分可以是运行在另一台Linux机器上的`gdbserver`。但是它也可以是一个硬件调试端口的界面（JTAG） 或者一个虚拟的机器管理程序（比如 [Qemu](http://en.wikipedia.org/wiki/QEMU)），并能够代替内核和ptrace的功能。那个远程根调试器会查询管理程序的结构，或者直接地查询处理器硬件寄存器来代替对OS内核结构的查询。
想要深层次学习这个远程协议，Embecosm 写了一篇[一个关于不同信息的详细指南](http://www.embecosm.com/appnotes/ean4/embecosm-howto-rsp-server-ean4-issue-2.html)。Gdbserver的事件处理循环在[这](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/gdbserver/server.c;h=173a22ea06ddfe696f1360376ba2c28e23d10b22;hb=HEAD#l469)，而也可以在这里找到[Qemu gdb-server stub](https://github.com/qemu/qemu/blob/master/gdbstub.c)。
# 总结一下
我们能看到`ptrace`的API提供了这里所有底层机制被要求实现的调试器：
> - 获取exec系统调用并从调用的地方阻止它执行
- 查询CPU的寄存器来获得处理器当前指令以及栈的地址
- 获取`clone或fork事件`来检测新线程
- 查看并改变数据地址读取并改变内存的变量
但是这就是一个调试器的全部工作吗？不，这只是那些非常低级的部分……它还会处理符号。这是，链接源程序和二进制文件。被忽视可能也是最重要的的一件事：断点！我会首先解释一下断点是如何工作的，因为这部分内容非常有趣且需要技巧，然后回到符号处理。
# 断点不是Ptrace API的一部分
就像我们之前看到的那样，断点不是`ptrace` API的一部分。但是我们可以改动内存并获取被调试的程序信号。你看不到其中的相关之处？这是因为断点的实现比较需要技巧并且还要一点hack！让我们来检验一下如何在一个指定的地址设置一个断点。
1、这个调试器读取（ptrace追踪）存在地址里的二进制指令，并保存在它自己的数据结构中。
2、它在这个位置写入一个不合法的指令。不管这个指令是啥，只要它是不合法的。
3、当被调试的程序运行到这个不合法的指令时（或者更准确地说，处理器将内存中的内容设置好时）它不会继续运行（因为它是不合法的）。
4、在现代多任务系统中，一个不合法的指令不会使整个系统崩溃掉，但是会通过引发一个中断（或错误）把控制权交回给系统内核。
5、这个中断被Linux翻译成一个`SIGTRAP`信号，然后被发送到处理器……或者发给它的父进程，就像调试器希望的那样。
6、调试器获得信号并查看被调试的程序指令指针的值（换言之，是陷入 trap发生的地方）。如果这个IP地址是在断点列表中，那么就是一个调试器的断点（否则就是一个进程中的错误，只需要传过信号并让它崩溃）。
7、现在，那个被调试的程序已经停在了断点，调试器可以让用户来做任何他/她想要做的事，等待时机合适继续执行。
8、为了要继续执行，这个调试器需要 1、写入正确的指令来回到被调试的程序的内存； 2、单步执行（继续执行单个CPU指令，伴随着ptrace 单步执行）； 3、把非法指令写回去（使得这个执行过程下一次可以再次停止） ；4、让这个执行正常运行
很整洁，是不是？作为一个旁观的评论，你可以注意到，如果不是所有线程同时停止的话这个算法是不会工作的（因为运行的线程可能会在合法的指令出现时传出断点）。我不会详细讨论GDB是如何解决这个问题的，但在这篇论文里已经说得很详细了：[使用GDB不间断调试多线程程序](http://www.bmrtech.com/uploadfile/image/whitepaper/mentorpaper_multcore_db.pdf)。简要地说，他们把指令写到内存中的其他地方，然后把那个指令的指针指向那个地址并单步执行处理器。但是问题在于一些指令是和地址相关的，比如跳转和条件跳转……
# 处理符号和调试信息
现在，让我们回到信号和调试信息处理。我没有详细地学习这部分，所以只是大体地说一说。
首先，我们是否可以不使用调试信息和信号地址来调试呢？答案是可以。因为正如我们看到过的那样，所有的低级指令是对CPU寄存器和内存地址来操作的，不是源程序层面的信息。因此，这个到源程序的链接只是为了方便用户。没有调试信息的时候，你看程序的方式就像是处理器（和内核）看到的一样：二进制（汇编）指令和内存字节。GDB不需要进一步的信息来把二进制信息翻译成CPU指令：


```
(gdb) x/10x $pc # heXadecimal representation
0x402c60:   0x56415741  0x54415541  0x55f48949  0x4853fd89
0x402c70:   0x03a8ec81  0x8b480000  0x8b48643e  0x00282504
0x402c80:   0x89480000  0x03982484
(gdb) x/10i $pc # Instruction representation
=> 0x402c60:    push   %r15
0x402c62:   push   %r14
0x402c64:   push   %r13
0x402c66:   push   %r12
0x402c68:   mov    %rsi,%r12
0x402c6b:   push   %rbp
0x402c6c:   mov    %edi,%ebp
0x402c6e:   push   %rbx
0x402c6f:   sub    $0x3a8,%rsp
0x402c76:   mov    (%rsi),%rdi
```
现在，如果我们加上调试信息，GDB能够把符号名称和地址配对：


```
(gdb) $pc
$1 = (void (*)()) 0x402c60 <main>
```
你可以通过 `nm -a $file` 来获取ELF二进制的符号列表：


```
nm -a /usr/lib/debug/usr/bin/ls.debug | grep " main"
0000000000402c60 T main
```
GDB还会能够展示堆栈跟踪信息（稍后会详细说），但是只有感兴趣的那部分：


```
(gdb) where
#0  write ()
#1  0x0000003d492769e3 in _IO_new_file_write ()
#2  0x0000003d49277e4c in new_do_write ()
#3  _IO_new_do_write ()
#4  0x0000003d49278223 in _IO_new_file_overflow ()
#5  0x00000000004085bb in print_current_files ()
#6  0x000000000040431b in main ()
```
我们现在有了PC地址和相应的函数，就是这样。在一个函数中，你将需要对着汇编来调试！
现在让我们加入调试信息：就是DWARF规范下的`gcc -g`选项。我不是特别熟悉这个规范，但我知道它提供的：
> - 地址到代码行和行到地址的配对
- 数据类型的定义，包括typedef和structure
- 本地变量和函数参数以及它们的类型

```
$ dwarfdump /usr/lib/debug/usr/bin/ls.debug | grep 402ce4
0x00402ce4  [1289, 0] NS
$ addr2line -e /usr/lib/debug/usr/bin/ls.debug  0x00402ce4
/usr/src/debug/coreutils-8.21/src/ls.c:1289
```
试一试`dwarfdump`来查看二进制文件里嵌入的信息。`addr2line`也能用到这些信息：
很多源代码层的调试命令会依赖于这些信息，比如`next`命令，这会在下一行的地址设置一个断点，那个`print`命令会依赖于变量的类型来输出（`char`、`int`、`float`，而不是二进制或十六进制）。
# 最后总结
我们已经见过调试器内部的好多方面了，所以我只会最后说几点：
> - 这个堆栈跟踪信息也是通过当前的帧是向上“解开（unwinded）”的（`$sp`和`$bp`/`#fp`），每个堆栈帧处理一次。函数的名称和参数以及本地变量名可以在调试信息中找到。
- `监视点（&amp;amp;lt;code&amp;amp;gt;watchpoints`）是通过处理器的帮助（如果有）实现的：在寄存器里标记哪些地址应该被监控，然后它会在那内存被读写的时候引发一个异常。如果不支持这项功能，或者你请求的断点超过了处理器所支持的……那么调试器就会回到“手动”监视：一个指令一个指令地执行这个程序，并检查是否当前的操作到达了一个监视点的地址。是的，这很慢！
- 反向调试也可以这样进行，记录每个操作的效果，并反向执行。
- 条件断点是正常的断点，除非在内部，调试器在将控制权交给用户前检查当前的情况。如果当前的情况不满足，程序将会默默地继续运行。
还可以玩`gdb gdb`，或者更好的（好多了）`gdb --pid $(pid of gdb)`，因为把两个调试器放到同一个终端里是疯狂的:-)。还可以调试系统：


```
qemu-system-i386 -gdb tcp::1234
gdb --pid $(pidof qemu-system-i386)
gdb /boot/vmlinuz --exec "target remote localhost:1234"
```
但我会在另一篇文章里提到！
