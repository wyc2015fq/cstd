# 如何在Linux中添加新的系统调用  - ljx0305的专栏 - CSDN博客
2008年07月30日 16:46:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：675标签：[linux																[linux内核																[parameters																[image																[dos																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=dos&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
系统调用是应用程序和操作系统内核之间的功能接口。其主要目的是使得用户
可以使用操作系统提供的有关设备管理、输入/输入系统、文件系统和进程控制、
通信以及存储管理等方面的功能，而不必了解系统程序的内部结构和有关硬件细
节，从而起到减轻用户负担和保护系统以及提高资源利用率的作用。
　　Linux操作系统作为自由软件的代表，它优良的性能使得它的应用日益广泛，
不仅得到专业人士的肯定，而且商业化的应用也是如火如荼。在Linux中，大部分
的系统调用包含在Linux的libc库中，通过标准的C函数调用方法可以调用这些系统
调用。那么，对Linux的发烧友来说，如何在Linux中增加新的系统调用呢？
1 Linux系统调用机制
　　在Linux系统中，系统调用是作为一种异常类型实现的。它将执行相应的机器
代码指令来产生异常信号。产生中断或异常的重要效果是系统自动将用户态切换为
核心态来对它进行处理。这就是说，执行系统调用异常指令时，自动地将系统切换
为核心态，并安排异常处理程序的执行。
　　Linux用来实现系统调用异常的实际指令是：
　　Int  $0x80
　　这一指令使用中断/异常向量号128（即16进制的80）将控制权转移给内核。为
达到在使用系统调用时不必用机器指令编程，在标准的C语言库中为每一系统调用
提供了一段短的子程序，完成机器代码的编程工作。事实上，机器代码段非常简
短。它所要做的工作只是将送给系统调用的参数加载到CPU寄存器中，接着执行
int  $0x80指令。然后运行系统调用，系统调用的返回值将送入CPU的一个寄存器
中，标准的库子程序取得这一返回值，并将它送回用户程序。
　　为使系统调用的执行成为一项简单的任务，Linux提供了一组预处理宏指令。
它们可以用在程序中。这些宏指令取一定的参数，然后扩展为调用指定的系统调用
的函数。
　　这些宏指令具有类似下面的名称格式：
　　_syscallN（parameters）
　　其中N是系统调用所需的参数数目，而parameters则用一组参数代替。这些参
数使宏指令完成适合于特定的系统调用的扩展。例如，为了建立调用setuid（）系
统调用的函数，应该使用：
　　_syscall1（ int， setuid， uid_t， uid ）
　　syscallN（ ）宏指令的第1个参数int说明产生的函数的返回值的类型是整
型，第2个参数setuid说明产生的函数的名称。后面是系统调用所需要的每个参
数。这一宏指令后面还有两个参数uid_t和uid分别用来指定参数的类型和名称。
　　另外，用作系统调用的参数的数据类型有一个限制，它们的容量不能超过四个
字节。这是因为执行int  $0x80指令进行系统调用时，所有的参数值都存在32位的
CPU寄存器中。使用CPU寄存器传递参数带来的另一个限制是可以传送给系统调用的
参数的数目。这个限制是最多可以传递5个参数。所以Linux一共定义了6个不同的
_syscallN（）宏指令，从_syscall0（）、_syscall1（）直到_syscall5（）。
　　一旦_syscallN（）宏指令用特定系统调用的相应参数进行了扩展，得到的结
果是一个与系统调用同名的函数，它可以在用户程序中执行这一系统调用。
2 添加新的系统调用
　　如果用户在Linux中添加新的系统调用，应该遵循几个步骤才能添加成功，下
面几个步骤详细说明了添加系统调用的相关内容。
（1） 添加源代码
　　第一个任务是编写加到内核中的源程序，即将要加到一个内核文件中去的一个
函数，该函数的名称应该是新的系统调用名称前面加上sys_标志。假设新加的系统
调用为mycall(int number)，在/usr/src/linux/kernel/sys.c文件中添加源代
码，如下所示：
　　asmlinkage int sys_mycall(int number)
　　{
　　return number;
　　}
　　作为一个最简单的例子，我们新加的系统调用仅仅返回一个整型值。
（2） 连接新的系统调用
　　添加新的系统调用后，下一个任务是使Linux内核的其余部分知道该程序的存
在。为了从已有的内核程序中增加到新的函数的连接，需要编辑两个文件。
　　在我们所用的Linux内核版本（RedHat 6.0，内核为2.2.5-15）中，第一个要
修改的文件是：
　　/usr/src/linux/include/asm-i386/unistd.h
　　该文件中包含了系统调用清单，用来给每个系统调用分配一个唯一的号码。文
件中每一行的格式如下：
　　#define __NR_name NNN
　　其中，name用系统调用名称代替，而NNN则是该系统调用对应的号码。应该将
新的系统调用名称加到清单的最后，并给它分配号码序列中下一个可用的系统调用
号。我们的系统调用如下：
　　#define __NR_mycall 191
　　系统调用号为191，之所以系统调用号是191，是因为Linux-2.2内核自身的系
统调用号码已经用到190。
　　第二个要修改的文件是：
　　/usr/src/linux/arch/i386/kernel/entry.S
　　该文件中有类似如下的清单：
　　.long SYMBOL_NAME（）
　　该清单用来对sys_call_table[]数组进行初始化。该数组包含指向内核中每个
系统调用的指针。这样就在数组中增加了新的内核函数的指针。我们在清单最后添
加一行：
　　.long SYMBOL_NAME(sys_mycall)
（3） 重建新的Linux内核
　　为使新的系统调用生效，需要重建Linux的内核。这需要以超级用户身份登
录。
　　#pwd
　　/usr/src/linux
　　#
　　超级用户在当前工作目录（/usr/src/linux）下，才可以重建内核。
　　#make config
　　#make dep
　　#make clearn
　　#make bzImage
　　编译完毕后，系统生成一可用于安装的、压缩的内核映象文件：
　　/usr/src/linux/arch/i386/boot/bzImage　
（4） 用新的内核启动系统
　　要使用新的系统调用，需要用重建的新内核重新引导系统。为此，需要修
改/etc/lilo.conf文件，在我们的系统中，该文件内容如下：
　　boot=/dev/hda
　　map=/boot/map
　　install=/boot/boot.b
　　prompt
　　timeout=50
　　image=/boot/vmlinuz-2.2.5-15
　　label=linux
　　root=/dev/hdb1
　read-only
　　other=/dev/hda1
　　label=dos
　　table=/dev/had
　　首先编辑该文件，添加新的引导内核：
　　image=/boot/bzImage-new
　　label=linux-new
　　root=/dev/hdb1
　　read-only
　　添加完毕，该文件内容如下所示：
　　boot=/dev/hda
　　map=/boot/map
　　install=/boot/boot.b
　　prompt
　　timeout=50
　　image=/boot/bzImage-new
　　label=linux-new
　　root=/dev/hdb1
　　read-only
　　image=/boot/vmlinuz-2.2.5-15
　　label=linux
　　root=/dev/hdb1
　　read-only
　　other=/dev/hda1
　　label=dos
　　table=/dev/hda
　　这样，新的内核映象bzImage-new成为缺省的引导内核。
　　为了使用新的lilo.conf配置文件，还应执行下面的命令：
　　#cp /usr/src/linux/arch/i386/boot/zImage /boot/bzImage-new
　　其次配置lilo:
　　# /sbin/lilo
　　现在，当重新引导系统时，在boot:提示符后面有三种选择：linux-new 、
linux、dos，新内核成为缺省的引导内核。
　　至此，新的Linux内核已经建立，新添加的系统调用已成为操作系统的一部
分，重新启动Linux，用户就可以在应用程序中使用该系统调用了。
（5）使用新的系统调用
　　在应用程序中使用新添加的系统调用mycall。同样为实验目的，我们写了一个
简单的例子xtdy.c。
　　/* xtdy.c */
　　#include
　　_syscall1(int,mycall,int,ret)
　　main()
　　{
　　printf("%d n",mycall(100));
　　}
　　编译该程序：
　　# cc -o xtdy xtdy.c
　　执行：
　　# xtdy
　　结果：
　　# 100
　　注意，由于使用了系统调用，编译和执行程序时，用户都应该是超级用户身
份。
引用：[http://www.chinalinuxpub.com/read.php?wid=23](http://www.chinalinuxpub.com/read.php?wid=23)
