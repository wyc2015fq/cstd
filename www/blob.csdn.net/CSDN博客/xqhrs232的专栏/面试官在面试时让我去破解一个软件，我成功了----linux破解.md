# 面试官在面试时让我去破解一个软件，我成功了----linux破解 - xqhrs232的专栏 - CSDN博客
2016年10月18日 21:45:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4098
原文地址::[http://www.techug.com/crack-a-program](http://www.techug.com/crack-a-program)

最近我在面试中被要求破解一个程序，并且成功拿到了工作。
大家好，开了一个新博客我真是非常激动呐，于是我计划每周都要更新几篇文章。看了标题各位应该知道本文的大概内容了，这里我会讲述一个在土耳其的安卡拉面试的故事。
“软件安全工程师（Software Security Engineer）”——我申请的是这样一个职位，面试时候他们问了我一些低级的问题，有些我能答上来，还有些却不行。
然后他们给我发了一封邮件，其中包含了一个加密的程序（CRACK MEEE!）。回家之后我下载了它，打开发现需要一个密码来解锁，心想原来他们是要我找到这个密码。:)
乍看之下这是相当困难的一件事，不过下面我会介绍破解过程中一些主要的概念。:)
第一件事就是在终端中执行程序
root@lisa:~# ./CrackTheDoor
*** DOOR CONTROL SYSTEM ***
PASSWORD:
我随手尝试了几个愚蠢的密码，3次之后，它就退出了。:)
再尝试用工具去分析它，比如获取文件信息：
root@lisa:~# file CrackTheDoor
CrackTheDoor: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV),
dynamically linked (uses shared libs), for GNU/Linux 2.6.15,
BuildID[sha1]=0x9927be2fe310bea01d412164103b9c8b2d7567ea, not stripped
root@lisa:~#
于是我们就得到了关于这个二进制包更详细的信息:)
接下来：
root@lisa:~# ldd CrackTheDoor
linux-gate.so.1 =>(0xf777b000)
libc.so.6 => /lib32/libc.so.6 (0xf760c000)
/lib/ld-linux.so.2 (0xf777c000)
root@lisa:~#
哎呀，顺手就打了这个命令。我稍微解释一下，linux-gate.so这个文件应该可以在你的电脑上找到。从ldd命令的结果可以看出它是一个共享库对吧？那么你们听说过Virtual DSO（Virtual Dynamic Shared Object）吗？
这里建议阅读一下关于的linux-gate.so[详细介绍](http://www.trilithium.com/johan/2005/08/linux-gate/)。
libc.so.6是GNU系统上一个通用的C语言库，这个你们可能都知道。
ld-linux.so是linux的动态加载器。
到目前为止一切都还算顺利，那么使用调试器来运行这个程序试试：
root@lisa:~# gdb CrackTheDoor
GNU gdb (GDB) 7.4.1-debian
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law. Type “show copying”
and “show warranty” for details.
This GDB was configured as “x86_64-linux-gnu”.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>…
Reading symbols from /root/CrackTheDoor…(no debugging symbols found)…done.
(gdb) r
Starting program: /root/CrackTheDoorProgram received signal SIGSEGV, Segmentation fault.
0x080484fb in __do_global_dtors_aux ()(gdb)
结果程序自己就挂掉了，应该是我们使用调试器造成的，看来有些反调试的小花招集成在里面了。那么……
我们来重新运行它，并且找到程序是从哪开始执行的：
root@lisa:~# gdb CrackTheDoor
GNU gdb (GDB) 7.4.1-debian
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law. Type “show copying”
and “show warranty” for details.
This GDB was configured as “x86_64-linux-gnu”.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>…
Reading symbols from /root/CrackTheDoor…(no debugging symbols found)…done.
(gdb) info file
Symbols from “/root/CrackTheDoor”.
Local exec file:
`/root/CrackTheDoor’, filetype elf32-i386.
Entry point: 0x804762c
…
…
这样就得到了程序的入口，在这里设置一个断点再调试看看：
b * 0x804762c
按r运行，应该会停在入口的第一行：
gdb) x/30i $pc=> 0x804762c: pusha
0x804762d: mov $0xaa,%dl
0x804762f: mov $0×8048480,%edi
0×8047634: mov $0x8048cbc,%ecx
0×8047639: mov %edi,0x80476f3
0x804763f: mov %ecx,0x80476f7
0×8047645: sub %edi,%ecx
0×8047647: mov $0x804762f,%esi
0x804764c: push $0x80476c1
0×8047651: pusha
0×8047652: mov $0×55,%al
0×8047654: xor $0×99,%al
0×8047656: mov $0×8047656,%edi
0x804765b: mov $0x80476e5,%ecx
0×8047660: sub $0×8047656,%ecx
0×8047666: repnz scas %es:(%edi),%al
0×8047668: je 0x804770a
0x804766e: mov %edi,0x80476eb
0×8047674: popa
0×8047675: add 0x80476eb,%edx
0x804767b: ret
结果应该上面那样的，这里语法系统是AT&A，你也可以切换为Intel语法体系。我个人而言，更偏向Intel语法。
在地址0×8047654处，我们首先吧0×55放到AL寄存器，然后将它和0×99异或，得到了0xCC。
0xCC非常重要，因为它会中断你的进程，正如评论中所说，这在x86架构中是断点指令（译注：INT 3）。当调试器想要中断程序时，会将0xCC这个字节放到需要断点的地方。
在0×8047666处有个指令repnz scas，意思是在段寄存器ES和偏移量EDI范围的内存中寻找AL里的值（0xCC）。
简单来说，这行就是会搜寻内存中的0xCC，如果找到就让程序挂掉。
不过我不想在这花费太多时间，我们来执行strace命令：
root@lisa:~# strace ./CrackTheDoor
execve(“./CrackTheDoor”, ["./CrackTheDoor"], [/* 17 vars */])= 0
[ Process PID=31085 runs in 32 bit mode. ]
brk(0)= 0×9972000
access(“/etc/ld.so.nohwcap”, F_OK)= -1 ENOENT (No such file or directory)
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)= 0xfffffffff7715000
access(“/etc/ld.so.preload”, R_OK)= -1 ENOENT (No such file or directory)
open(“/etc/ld.so.cache”, O_RDONLY)= 3
fstat64(3, {st_mode=S_IFREG|0644, st_size=35597, …})= 0
mmap2(NULL, 35597, PROT_READ, MAP_PRIVATE, 3, 0)= 0xfffffffff770c000
close(3)= 0
access(“/etc/ld.so.nohwcap”, F_OK)= -1 ENOENT (No such file or directory)
open(“/lib32/libc.so.6″, O_RDONLY)= 3
read(3, “\177ELF\1\1\1\3\3\1\300o\1004″…, 512)= 512
fstat64(3, {st_mode=S_IFREG|0755, st_size=1441884, …})= 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)= 0xfffffffff770b000
mmap2(NULL, 1456504, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0)= 0xfffffffff75a7000
mprotect(0xf7704000, 4096, PROT_NONE)= 0
mmap2(0xf7705000, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x15d)= 0xfffffffff7705000
mmap2(0xf7708000, 10616, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0)= 0xfffffffff7708000
close(3)= 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)= 0xfffffffff75a6000
set_thread_area(0xffe4d864)= 0
mprotect(0xf7705000, 8192, PROT_READ)= 0
mprotect(0×8049000, 4096, PROT_READ)= 0
mprotect(0xf7733000, 4096, PROT_READ)= 0
munmap(0xf770c000, 35597)= 0
ptrace(PTRACE_TRACEME, 0, 0×1, 0)= -1 EPERM (Operation not permitted)
ptrace(PTRACE_TRACEME, 0, 0×1, 0)= -1 EPERM (Operation not permitted)
看到最后一行了吧，我们的程序又挂掉了，是ptrace这个函数的系统调用引起的。
在Linux中，ptrace是”Process Trace”的缩写。有了它，你就可以控制另一个进程，并且改变执行状态，就像调试器中一样。
调试器经常使用ptrace:)，毕竟这就是它们的任务嘛。
我们可以想象一下这段代码，可能是下面这样的：
int main(){if(ptrace(PTRACE_TRACEME, 0, 1, 0)< 0){
        printf("DEBUGGING... Bye\n");return 1;}
    printf("Hello\n");return 0;}
顺便说一句，你只能使用一次ptrace[PTRACE_TRACEME]。因此如果调试器在这之前使用ptrace，那么我们的调用就会返回false，于是就能知道还有其他东西在控制程序了。
现在需要做的是绕过ptrace保护机制，使程序无法检测到自己运行在调试器中。
所以一个可能的策略是改变这个系统调用的返回值。
系统调用是用户态与核心态之间的桥梁，于是可以确定ptrace中一定用到了一些系统调用来控制进程。
我们希望检测到程序使用ptrace，并且设置它的返回值为0:)
我在home目录下建立了一个文件叫.gdbinit，这样一来，当运行gdb的时候，其中的配置信息就会被自动加载。
~/.gdbinit
set disassembly-flavor intel # Intel syntax is betterset disassemble-next-line on
catch syscall ptrace #Catch the syscall.
commands 1
set($eax)= 0
continue
end
寄存器EAX会保存系统调用的返回值，这样它就永远都是0了，或者说是true。
通过这种方式，我们绕过了反调试机制，那就回到gdb继续吧：
eren@lisa:~$ gdb ./CrackTheDoor
GNU gdb (GDB) 7.4.1-debian
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law. Type “show copying”
and “show warranty” for details.
This GDB was configured as “x86_64-linux-gnu”.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>…
Catchpoint 1 (syscall ‘ptrace’ [26])
Reading symbols from /home/eren/CrackTheDoor…(no debugging symbols found)…done.
(gdb) r
Starting program: /home/eren/CrackTheDoorCatchpoint 1 (call to syscall ptrace), 0×08047698 in ?? ()=> 0×08047698: 3d 00 f0 ff ff cmp eax,0xfffff000Catchpoint 1 (returned from syscall ptrace), 0×08047698 in ?? ()=> 0×08047698: 3d 00 f0 ff ff cmp eax,0xfffff000*** DOOR CONTROL SYSTEM ***PASSWORD:
好了，至少目前为止我们能够随心所欲地来调试了。:)
我在函数PJeGPC4TIVaKFmmy53DJ处又设置了一个断点：
Breakpoint 2, 0×08048534 in PJeGPC4TIVaKFmmy53DJ ()=> 0×08048534 <PJeGPC4TIVaKFmmy53DJ+0>: 1e push ds
(gdb) x/40i $pc=> 0×8048534 <PJeGPC4TIVaKFmmy53DJ>: push ds
0×8048535 <PJeGPC4TIVaKFmmy53DJ+1>: mov ebp,esp
0×8048537 <PJeGPC4TIVaKFmmy53DJ+3>: sub esp,0×20
0x804853a <PJeGPC4TIVaKFmmy53DJ+6>: mov BYTE PTR [ebp-0x1],0xe4
0x804853e <PJeGPC4TIVaKFmmy53DJ+10>: mov BYTE PTR [ebp-0x2],0×87
0×8048542 <PJeGPC4TIVaKFmmy53DJ+14>: mov BYTE PTR [ebp-0x3],0xfb
0×8048546 <PJeGPC4TIVaKFmmy53DJ+18>: mov BYTE PTR [ebp-0x4],0xbe
0x804854a <PJeGPC4TIVaKFmmy53DJ+22>: mov BYTE PTR [ebp-0x5],0xc9
0x804854e <PJeGPC4TIVaKFmmy53DJ+26>: mov BYTE PTR [ebp-0x6],0×93
0×8048552 <PJeGPC4TIVaKFmmy53DJ+30>: mov BYTE PTR [ebp-0x7],0×84
0×8048556 <PJeGPC4TIVaKFmmy53DJ+34>: mov BYTE PTR [ebp-0x8],0xfc
0x804855a <PJeGPC4TIVaKFmmy53DJ+38>: mov BYTE PTR [ebp-0x9],0x8d
0x804855e <PJeGPC4TIVaKFmmy53DJ+42>: mov BYTE PTR [ebp-0xa],0xe5
0×8048562 <PJeGPC4TIVaKFmmy53DJ+46>: mov BYTE PTR [ebp-0xb],0xbf
0×8048566 <PJeGPC4TIVaKFmmy53DJ+50>: mov BYTE PTR [ebp-0xc],0x5c
0x804856a <PJeGPC4TIVaKFmmy53DJ+54>: mov BYTE PTR [ebp-0xd],0xe2
0x804856e <PJeGPC4TIVaKFmmy53DJ+58>: mov BYTE PTR [ebp-0xe],0×76
0×8048572 <PJeGPC4TIVaKFmmy53DJ+62>: mov BYTE PTR [ebp-0xf],0×21
0×8048576 <PJeGPC4TIVaKFmmy53DJ+66>: mov BYTE PTR [ebp-0x10],0xb8
0x804857a <PJeGPC4TIVaKFmmy53DJ+70>: mov DWORD PTR [ebp-0x14],0×0
0×8048581 <PJeGPC4TIVaKFmmy53DJ+77>: mov eax,DWORD PTR [ebp-0x14]
0×8048584 <PJeGPC4TIVaKFmmy53DJ+80>: add eax,DWORD PTR [ebp+0x8]
0×8048587 <PJeGPC4TIVaKFmmy53DJ+83>: movzx eax,BYTE PTR [eax]
0x804858a <PJeGPC4TIVaKFmmy53DJ+86>: test al,al
0x804858c <PJeGPC4TIVaKFmmy53DJ+88>: je 0×8048808 <PJeGPC4TIVaKFmmy53DJ+724>
0×8048592 <PJeGPC4TIVaKFmmy53DJ+94>: mov eax,DWORD PTR [ebp-0x14]
0×8048595 <PJeGPC4TIVaKFmmy53DJ+97>: add eax,DWORD PTR [ebp+0x8]
0×8048598 <PJeGPC4TIVaKFmmy53DJ+100>: mov edx,DWORD PTR [ebp-0x14]
0x804859b <PJeGPC4TIVaKFmmy53DJ+103>: add edx,DWORD PTR [ebp+0x8]
0x804859e <PJeGPC4TIVaKFmmy53DJ+106>: movzx edx,BYTE PTR [edx]
0x80485a1 <PJeGPC4TIVaKFmmy53DJ+109>: xor dl,BYTE PTR [ebp-0x1]
0x80485a4 <PJeGPC4TIVaKFmmy53DJ+112>: mov BYTE PTR [eax],dl
0x80485a6 <PJeGPC4TIVaKFmmy53DJ+114>: add DWORD PTR [ebp-0x14],0×1
0x80485aa <PJeGPC4TIVaKFmmy53DJ+118>: mov eax,DWORD PTR [ebp-0x14]
0x80485ad <PJeGPC4TIVaKFmmy53DJ+121>: add eax,DWORD PTR [ebp+0x8]
0x80485b0 <PJeGPC4TIVaKFmmy53DJ+124>: movzx eax,BYTE PTR [eax]
0x80485b3 <PJeGPC4TIVaKFmmy53DJ+127>: test al,al
0x80485b5 <PJeGPC4TIVaKFmmy53DJ+129>: je 0x804880b <PJeGPC4TIVaKFmmy53DJ+727>
0x80485bb <PJeGPC4TIVaKFmmy53DJ+135>: mov eax,DWORD PTR [ebp-0x14]
0x80485be <PJeGPC4TIVaKFmmy53DJ+138>: add eax,DWORD PTR [ebp+0x8]
0x80485c1 <PJeGPC4TIVaKFmmy53DJ+141>: mov edx,DWORD PTR [ebp-0x14]
0x80485c4 <PJeGPC4TIVaKFmmy53DJ+144>: add edx,DWORD PTR [ebp+0x8]
0x80485c7 <PJeGPC4TIVaKFmmy53DJ+147>: movzx edx,BYTE PTR [edx]
0x80485ca <PJeGPC4TIVaKFmmy53DJ+150>: xor dl,BYTE PTR [ebp-0x2]
终于到有乐子的步骤了。
我看到有些常量在内存中移动，并且和我输入的值进行了异或运算。
继续深入研究……
(gdb) x/30i X1bdrhN8Yk9NZ59Vb7P2
0×8048838 <X1bdrhN8Yk9NZ59Vb7P2>: sbb ecx,DWORD PTR [ecx+0x20ec83e5]
0x804883e <X1bdrhN8Yk9NZ59Vb7P2+6>: mov DWORD PTR [ebp-0x18],0×0
0×8048845 <X1bdrhN8Yk9NZ59Vb7P2+13>: mov BYTE PTR [ebp-0x1],0xd9
0×8048849 <X1bdrhN8Yk9NZ59Vb7P2+17>: mov BYTE PTR [ebp-0x2],0xcd
0x804884d <X1bdrhN8Yk9NZ59Vb7P2+21>: mov BYTE PTR [ebp-0x3],0xc9
0×8048851 <X1bdrhN8Yk9NZ59Vb7P2+25>: mov BYTE PTR [ebp-0x4],0xe5
0×8048855 <X1bdrhN8Yk9NZ59Vb7P2+29>: mov BYTE PTR [ebp-0x5],0x9e
0×8048859 <X1bdrhN8Yk9NZ59Vb7P2+33>: mov BYTE PTR [ebp-0x6],0xd0
0x804885d <X1bdrhN8Yk9NZ59Vb7P2+37>: mov BYTE PTR [ebp-0x7],0xe8
0×8048861 <X1bdrhN8Yk9NZ59Vb7P2+41>: mov BYTE PTR [ebp-0x8],0xa5
0×8048865 <X1bdrhN8Yk9NZ59Vb7P2+45>: mov BYTE PTR [ebp-0x9],0xaf
0×8048869 <X1bdrhN8Yk9NZ59Vb7P2+49>: mov BYTE PTR [ebp-0xa],0×87
0x804886d <X1bdrhN8Yk9NZ59Vb7P2+53>: mov BYTE PTR [ebp-0xb],0xd2
0×8048871 <X1bdrhN8Yk9NZ59Vb7P2+57>: mov BYTE PTR [ebp-0xc],0×79
0×8048875 <X1bdrhN8Yk9NZ59Vb7P2+61>: mov BYTE PTR [ebp-0xd],0xa9
0×8048879 <X1bdrhN8Yk9NZ59Vb7P2+65>: mov BYTE PTR [ebp-0xe],0x5d
0x804887d <X1bdrhN8Yk9NZ59Vb7P2+69>: mov BYTE PTR [ebp-0xf],0×7
0×8048881 <X1bdrhN8Yk9NZ59Vb7P2+73>: mov BYTE PTR [ebp-0x10],0×81
0×8048885 <X1bdrhN8Yk9NZ59Vb7P2+77>: mov DWORD PTR [ebp-0x14],0×0
0x804888c <X1bdrhN8Yk9NZ59Vb7P2+84>: mov eax,DWORD PTR [ebp-0x14]
0x804888f <X1bdrhN8Yk9NZ59Vb7P2+87>: add eax,DWORD PTR [ebp+0x8]
0×8048892 <X1bdrhN8Yk9NZ59Vb7P2+90>: movzx eax,BYTE PTR [eax]
0×8048895 <X1bdrhN8Yk9NZ59Vb7P2+93>: cmp al,BYTE PTR [ebp-0x1]
0×8048898 <X1bdrhN8Yk9NZ59Vb7P2+96>: je 0x80488a2 <X1bdrhN8Yk9NZ59Vb7P2+106>
0x804889a <X1bdrhN8Yk9NZ59Vb7P2+98>: mov eax,DWORD PTR [ebp-0x18]
和之前得到的结果类似，现在我在其中增加一些常量：
下面是这个函数后面运行的情况：
0x804889d <X1bdrhN8Yk9NZ59Vb7P2+101>: jmp 0x8048a20 <X1bdrhN8Yk9NZ59Vb7P2+488>
0x80488a2 <X1bdrhN8Yk9NZ59Vb7P2+106>: add DWORD PTR [ebp-0x14],0×1
0x80488a6 <X1bdrhN8Yk9NZ59Vb7P2+110>: mov eax,DWORD PTR [ebp-0x14]
0x80488a9 <X1bdrhN8Yk9NZ59Vb7P2+113>: add eax,DWORD PTR [ebp+0x8]
0x80488ac <X1bdrhN8Yk9NZ59Vb7P2+116>: movzx eax,BYTE PTR [eax]
0x80488af <X1bdrhN8Yk9NZ59Vb7P2+119>: cmp al,BYTE PTR [ebp-0x2]
0x80488b2 <X1bdrhN8Yk9NZ59Vb7P2+122>: je 0x80488bc <X1bdrhN8Yk9NZ59Vb7P2+132>
0x80488b4 <X1bdrhN8Yk9NZ59Vb7P2+124>: mov eax,DWORD PTR [ebp-0x18]
0x80488b7 <X1bdrhN8Yk9NZ59Vb7P2+127>: jmp 0x8048a20 <X1bdrhN8Yk9NZ59Vb7P2+488>
0x80488bc <X1bdrhN8Yk9NZ59Vb7P2+132>: add DWORD PTR [ebp-0x14],0×1
0x80488c0 <X1bdrhN8Yk9NZ59Vb7P2+136>: mov eax,DWORD PTR [ebp-0x14]
0x80488c3 <X1bdrhN8Yk9NZ59Vb7P2+139>: add eax,DWORD PTR [ebp+0x8]
0x80488c6 <X1bdrhN8Yk9NZ59Vb7P2+142>: movzx eax,BYTE PTR [eax]
0x80488c9 <X1bdrhN8Yk9NZ59Vb7P2+145>: cmp al,BYTE PTR [ebp-0x3]
0x80488cc <X1bdrhN8Yk9NZ59Vb7P2+148>: je 0x80488d6 <X1bdrhN8Yk9NZ59Vb7P2+158>
0x80488ce <X1bdrhN8Yk9NZ59Vb7P2+150>: mov eax,DWORD PTR [ebp-0x18]
0x80488d1 <X1bdrhN8Yk9NZ59Vb7P2+153>: jmp 0x8048a20 <X1bdrhN8Yk9NZ59Vb7P2+488>
0x80488d6 <X1bdrhN8Yk9NZ59Vb7P2+158>: add DWORD PTR [ebp-0x14],0×1
0x80488da <X1bdrhN8Yk9NZ59Vb7P2+162>: mov eax,DWORD PTR [ebp-0x14]
0x80488dd <X1bdrhN8Yk9NZ59Vb7P2+165>: add eax,DWORD PTR [ebp+0x8]
—Type <return> to continue, or q <return> to quit—
0x80488e0 <X1bdrhN8Yk9NZ59Vb7P2+168>: movzx eax,BYTE PTR [eax]
0x80488e3 <X1bdrhN8Yk9NZ59Vb7P2+171>: cmp al,BYTE PTR [ebp-0x4]
0x80488e6 <X1bdrhN8Yk9NZ59Vb7P2+174>: je 0x80488f0 <X1bdrhN8Yk9NZ59Vb7P2+184>
0x80488e8 <X1bdrhN8Yk9NZ59Vb7P2+176>: mov eax,DWORD PTR [ebp-0x18]
0x80488eb <X1bdrhN8Yk9NZ59Vb7P2+179>: jmp 0x8048a20 <X1bdrhN8Yk9NZ59Vb7P2+488>
0x80488f0 <X1bdrhN8Yk9NZ59Vb7P2+184>: add DWORD PTR [ebp-0x14],0×1
0x80488f4 <X1bdrhN8Yk9NZ59Vb7P2+188>: mov eax,DWORD PTR [ebp-0x14]
0x80488f7 <X1bdrhN8Yk9NZ59Vb7P2+191>: add eax,DWORD PTR [ebp+0x8]
0x80488fa <X1bdrhN8Yk9NZ59Vb7P2+194>: movzx eax,BYTE PTR [eax]
0x80488fd <X1bdrhN8Yk9NZ59Vb7P2+197>: cmp al,BYTE PTR [ebp-0x5]
0×8048900 <X1bdrhN8Yk9NZ59Vb7P2+200>: je 0x804890a <X1bdrhN8Yk9NZ59Vb7P2+210>
0×8048902 <X1bdrhN8Yk9NZ59Vb7P2+202>: mov eax,DWORD PTR [ebp-0x18]
0×8048905 <X1bdrhN8Yk9NZ59Vb7P2+205>: jmp 0x8048a20 <X1bdrhN8Yk9NZ59Vb7P2+488>
0x804890a <X1bdrhN8Yk9NZ59Vb7P2+210>: add DWORD PTR [ebp-0x14],0×1
0x804890e <X1bdrhN8Yk9NZ59Vb7P2+214>: mov eax,DWORD PTR [ebp-0x14]
你们看出其中运行的模式了吗？如果没看出来也没关系……
在这个函数中，程序同样将我的输入和一些常量进行了异或运算。
现在来看看输入，首先输入会和这些常量进行异或，然后将结果和另一些常量进行对比。
因此这两个函数应该是这样的：
void PJeGPC4TIVaKFmmy53DJ (int * p){
    int array[]={0xe4,0x87,0xfb,0xbe,0xc9,0x93,0x84,0xfc,
        0x8d,0xe5,0xbf,0x5c,0xe2,0x76,0x21,0xb8}
    for(i=0;i<16;i++){
        p[i]= p[i] ^ array[i]}}
int X1bdrhN8Yk9NZ59Vb7P2(int * p){
    int array ={0xd9,0xcd,0xc9,0xe5,0x9e,0xd0,0xe8,0xa5,
        0xaf,0x87,0xd2,0x79,0xa9,0x5d,0x7,0x81}
    for(i=0;i<16;i++){
        if(p[i]!= array[i])returnfalse; // fail..}returntrue}
于是写了一个简单的python脚本将上面两个常量进行异或操作，就得到密码了：
#!/usr/bin/python
firstConst =[0xe4,0x87,0xfb,0xbe,0xc9,0x93,0x84,0xfc,0x8d,0xe5,0xbf,0x5c,0xe2,0x76,0x21,0xb8]
secondConst =[0xd9,0xcd,0xc9,0xe5,0x9e,0xd0,0xe8,0xa5,0xaf,0x87,0xd2,0x79,0xa9,0x5d,0x7,0x81]
ret =”"
for x in range(16):
ret+=chr(firstConst[x] ^ secondConst[x])
print ret
eren@lisa:~$ ./CrackTheDoor*** DOOR CONTROL SYSTEM ***PASSWORD: =J2[WClY”bm%K+&9*** ACCESS GRANTED ****** THE DOOR OPENED ***
看！成功了！
这个公司又给我发来了第二轮的破解题目，也一样很有趣，下次我会在文章中写到的。
译注：原文评论中较多的疑问是如何定位到函数PJeGPC4TIVaKFmmy53DJ，有人给出了合理的解释，作者也默认了：其实很简单，不停地尝试。也许写下这篇文章之前有很多很多次失败，但是读者最感兴趣的部分是成功的，因此那些都被省略了。
