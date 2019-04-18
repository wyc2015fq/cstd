# Windbg核心调试之dump分析 - gauss的专栏 - CSDN博客
2013年01月04日 00:21:31[gauss](https://me.csdn.net/mathlmx)阅读数：339
当系统发生错误是，最常见的就是蓝屏(Bluescreen)，这时就会在系统目录下产生一个Dump文件，如MEMORY.DMP。这个文件的主要意义在于分析系统错误发生的原因，以作出解决的方法。
**一.Dump文件的产生，意义和类型**
　　当系统发生错误是，最常见的就是蓝屏(Bluescreen)，这时就会在系统目录下产生一个Dump文件，如MEMORY.DMP。这个文件的主要意义在于分析系统错误发生的原因，以作出解决的方法。
　　它可分为三种类型：
　　1.完全内存转储。这个文件比较大，和物理内存相当，包含了程序崩溃前系统及用户模式下的所有信息。
　　2.核心内存转储。这个文件大小约物理内存的三分之一，主要包含崩溃前系统内核的运[行情](http://quote.hexun.com/)况。一般为了分析内核错误，就选用这种文件。
　　3.小内存转储。这个文件小，只有64k，刚好一个页面文件大小。它包含了相对比较少的信息，主要可用于微软的在线分析。
　　以上三种形式的文件可以在我的电脑――〉鼠标右键――〉属性――〉高级――〉故障及恢复中设置。如下图：
**二。Dump文件的强迫产生**
　　由于我们也不知道何时会产生一个系统错误，从而得到dump文件，所以当练习分析时，可认为强迫产生一个。一般有以下两个办法。
　　1.双机联调。这里的双机可以是物理上的两台电脑，也可以是用虚拟机模拟。我想这里的大多数人应该选择后者，为啥?还不是money的问题~_^。当用windbg把被调试机联上以后，就可以用.crash命令产生一个蓝屏，当然之前要在被调试机里把dump产生的路径和类型设定好。还有另外一张办法，是通过修改注册表后，用键盘产生dump，但这种方法哪有第一种来的快，所以就不说了，感兴趣的可以查查windbg帮助文档看看。
　　2.单机驱动产生。这种方法，不必用双机联调，在本机上就可以办到。由于驱动深入到了内核，它的要求非常苛刻，一个简单的除零操作就可引发蓝屏。但是驱动的编写与普通win32api是有很大不同的，为了减轻负担，我直接运用一个现成的程序，是《MicrosoftWindowsInternals》作者写的Notmyfault(见附件)。它由Notmyfault.exe和Myfault.sys两部分组成。正如名字一样，引发蓝屏的不是Notmyfault.exe而是由他加载到内核中的Myfault.sys。如图:
　　我在这里两种方法都同时用了，先在虚拟机里执行Notmyfault，接着windbg立刻检测到了系统崩溃，并输出相关信息。
**三。Dump文件的分析**
　　当按上面的方法运行后，windbg输出了以下内容：
　　*** Fatal System Error: 0x000000d1
　　(0xE1147008,0x0000001C,0x00000000,0xFBE93403)
　　Break instruction exception - code 80000003 (first chance)
　　A fatal system error has occurred.
　　Debugger entered on first try; Bugcheck callbacks have notbeeninvoked.
　　A fatal system error has occurred.
　　*******************************************************************************
　　* *
　　* Bugcheck Analysis *
　　* *
　　*******************************************************************************
　　Use !analyze -v to get detailed debugging information.
　　2.BugCheck D1, e1147008, 1c, 0, fbe93403
　　*** ERROR: Module load completed but symbols could not beloadedfor myfault.sys
　　3.Probably caused by : myfault.sys ( myfault+403 )
　　Followup: MachineOwner
　　-
　　nt!RtlpBreakWithStatusInstruction:
　　80527da8 cc int 3
　　Kd:>
　　上面这一段，有用的信息，如1和2两段，说明的是一个问题，都指明了BugCheck是D1，并给了四个参数，这里的D1可以在windbg文档的BugCheckCode Reference中查出其具体含义，也可用!analyze?showD1命令查出。3说明引起的原因是myfault.sys模块。
　　接着在kd后输入!analyze ?v命令，这个命令是详细列出dump文件的信息。
　　windbg输出如下：
　　kd> !analyze -v
　　*******************************************************************************
　　* *
　　* Bugcheck Analysis *
　　* *
　　*******************************************************************************
　　DRIVER_IRQL_NOT_LESS_OR_EQUAL (d1) //指明Bugcheck D1,我们已看见过了
　　An attempt was made to access a pageable (or completelyinvalid)address at an
　　interrupt request level (IRQL) that is too high. Thisisusually
　　caused by drivers using improper addresses. //解释了错误的原因
　　If kernel debugger is available get stack backtrace.
　　Arguments:
　　Arg1: e1147008, memory referenced
　　Arg2: 0000001c, IRQL
　　Arg3: 00000000, value 0 = read operation, 1 = write operation
　　Arg4: fbe93403, address which referenced memory
　　//给出了相应的四个参数，第二列是代号，第三列是解释
　　Debugging Details:
　　--
　　READ_ADDRESS: e1147008 Paged pool //上面的Arg1.
　　CURRENT_IRQL: 1c //上面的Arg2
　　FAULTING_IP: //指出发生错误时所执行的指令
　　myfault+403
　　fbe93403 8b06 mov eax,dword ptr [esi]
　　DEFAULT_BUCKET_ID: DRIVER_FAULT //指出错误类型，是驱动错误
　　BUGCHECK_STR: 0xD1 //bugcheck索引，可查windbg文档，也可!analyze ?showD1
　　PROCESS_NAME: NotMyfault.exe //错误所属进程
　　TRAP_FRAME: f9357b80 --(trap fffffffff9357b80)//错误时各寄存器的内容
　　ErrCode = 00000000
　　eax=00000000 ebx=8111f330 ecx=000000d1 edx=0000001cesi=e1147008edi=00000000
　　eip=fbe93403 esp=f9357bf4 ebp=f9357c58 iopl=0 nv up ei pl zr napenc
　　cs=0008 ss=0010 ds=0023 es=0023 fs=0030 gs=0000 efl=00010246
　　myfault+0x403:
　　fbe93403 8b06 mov eax,dword ptr[esi]ds:0023:e1147008=????????
　　Resetting default scope
　　LAST_CONTROL_TRANSFER: from 804f880d to 80527da8
　　STACK_TEXT://反映了错误前堆栈中函数调用情况，最下面的0x7c801671处函数调用ntdll中的ZwDeviceIoControlFile，接着调用了ntdll中的KiFastSystemCallRet，再接着调用了nt(这里的nt指Ntoskrnl)中的KiFastCallEntry，一直到myfault+0x403，发生异常。
　　f9357734 804f880d 00000003 f9357a9000000000nt!RtlpBreakWithStatusInstruction
　　f9357780 804f93fa 00000003 e1147008fbe93403nt!KiBugCheckDebugBreak+0x19
　　f9357b60 80540853 0000000a e11470080000001cnt!KeBugCheck2+0x574
　　f9357b60 fbe93403 0000000a e1147008 0000001cnt!KiTrap0E+0x233
　　WARNING: Stack unwind information not available. Followingframesmay be wrong.
　　f9357c58 805759d1 ffb5c3b0 8111f318 811d9130 myfault+0x403
　　f9357d00 8056e33c 00000090 0000000000000000nt!IopXxxControlFile+0x5e7
　　f9357d34 8053d808 00000090 0000000000000000nt!NtDeviceIoControlFile+0x2a
　　f9357d34 7c92eb94 00000090 0000000000000000nt!KiFastCallEntry+0xf8
　　0012f9f0 7c92d8ef 7c801671 0000009000000000ntdll!KiFastSystemCallRet
　　0012f9f4 7c801671 00000090 0000000000000000ntdll!ZwDeviceIoControlFile+0xc
　　0012fa54 004018c2 00000090 83360018 00000000 0x7c801671
　　STACK_COMMAND: kb
　　FOLLOWUP_IP: //反汇编了发生错误指令的代码
　　myfault+403
　　fbe93403 8b06 mov eax,dword ptr [esi]
　　SYMBOL_STACK_INDEX: 4
　　FOLLOWUP_NAME: MachineOwner
　　MODULE_NAME: myfault
　　IMAGE_NAME: myfault.sys
　　DEBUG_FLR_IMAGE_TIMESTAMP: 43774e1d
　　SYMBOL_NAME: myfault+403
　　FAILURE_BUCKET_ID: 0xD1_myfault+403
　　BUCKET_ID: 0xD1_myfault+403
　　Followup: MachineOwner
　　//以上几段看名字就知道了，是以上信息的重复没有多大价值。
**四。总结**
　　通过以上的分析，知道了蓝屏的原因是BugcheckD1引起的，是由于驱动程序读操作了过高的IRQL引起的。也知道了这个引发蓝屏的驱动程序是myfault.sys，属于notmyfaulf.exe的进程。还知道了蓝屏前bug程序myfault.sys的调用情况等多个有用信息，接着就可以在myfault.sys源程序中进行bug修改了。
