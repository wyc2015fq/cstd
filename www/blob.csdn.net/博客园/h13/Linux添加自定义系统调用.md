# Linux添加自定义系统调用 - h13 - 博客园
一、什么是系统调用
在Linux的世界里，我们经常会遇到系统调用这一术语，所谓系统调用，就是内核提供的、功能十分强大的一系列的函数。这些系统调用是在内核中实现的，再通过一定的方式把系统调用给用户，一般都通过门(gate)陷入(trap)实现。系统调用就是用户空间应用程序和内核提供的服务之间的一个接口。由于服务是在内核中提供的，因此无法执行直接调用；相反，您必须使用一个进程来跨越用户空间与内核之间的界限。在特定架构中实现此功能的方法会有所不同。因此，本文将着眼于最通用的架构—— i386。
二、系统调用的作用
系统调用在Linux系统中发挥着巨大的作用，如果没有系统调用，那么应用程序就失去了内核的支持。我们在编程时用到的很多函数，如fork、open等这些函数最终都是在系统调用里实现的，这里我们说到了两个函数，即fork和exit,这两函数都是glibc中的函数，但是如果我们跟踪函数的执行过程，看看glibc对fork和exit函数的实现就可以发现在glibc的实现代码里都是采用软中断的方式陷入到内核中再通过系统调用实现函数的功能的。具体过程我们在系统调用的实现过程会详细的讲到。
由此可见，系统调用是用户接口在内核中的实现，如果没有系统调用，用户就不能利用内核。
三、系统调用的现实及调用过程
详细讲述系统调用的之前也讲一下Linux系统的一些保护机制。
      Linux系统在CPU的保护模式下提供了四个特权级别，目前内核都只用到了其中的两个特权级别，分别为“特权级0”和“特权级3”,级别0也就是我们通常所讲的内核模式，级别3也就是我们通常所讲的用户模式。划分这两个级别主要是对系统提供保护。内核模式可以执行一些特权指令和进入用户模式，而用户模式则不能。
这里特别提出的是，内核模式与用户模式分别使用自己的堆栈，当发生模式切换的时候同时要进行堆栈的切换。每个进程都有自己的地址空间（也称为进程空间），进程的地址空间也分为两部分：用户空间和系统空间，在用户模式下只能访问进程的用户空间，在内核模式下则可以访问进程的全部地址空间，这个地址空间里的地址是一个逻辑地址，通过系统段面式的管理机制，访问的实际内存要做二级地址转换，即：逻辑地址、线性地址、物理地址。
系统调用对于内核来说就相当于函数，我们是关键问题是从用户模式到内核模式的转换、堆栈的切换以及参数的传递。
四、引起系统调用的两种途径
（1）int $0×80 , 老式linux内核版本中引起系统调用的唯一方式
（2）sysenter汇编指令
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/mlyy225/archive/2010/01/07/5148911.aspx](http://blog.csdn.net/mlyy225/archive/2010/01/07/5148911.aspx)
如何在Linux中添加新的系统调用
系统调用是应用程序和操作系统内核之间的功能接口。其主要目的是使得用户可以使用操作系统提供的有关设备管理、输入/输入系统、文件系统和进程控制、通信以及存储管理等方面的功能，而不必了解系统程序的内部结构和有关硬件细节，从而起到减轻用户负担和保护系统以及提高资源利用率的作用。
Linux操作系统作为自由软件的代表，它优良的性能使得它的应用日益广泛，不仅得到专业人士的肯定，而且商业化的应用也是如火如荼。在Linux中，大部分的系统调用包含在Linux的libc库中，通过标准的C函数调用方法可以调用这些系统调用。那么，对Linux的发烧友来说，如何在Linux中增加新的系统调用呢？
1 Linux系统调用机制
　　在Linux系统中，系统调用是作为一种异常类型实现的。它将执行相应的机器代码指令来产生异常信号。产生中断或异常的重要效果是系统自动将用户态切换为核心态来对它进行处理。这就是说，执行系统调用异常指令时，自动地将系统切换为核心态，并安排异常处理程序的执行。
Linux用来实现系统调用异常的实际指令是：
Int x80 
　　这一指令使用中断/异常向量号128（即16进制的80）将控制权转移给内核。为达到在使用系统调用时不必用机器指令编程，在标准的C语言库中为每一系统调用提供了一段短的子程序，完成机器代码的编程工作。事实上，机器代码段非常简短。它所要做的工作只是将送给系统调用的参数加载到CPU寄存器中，接着执行int x80指令。然后运行系统调用，系统调用的返回值将送入CPU的一个寄存器中，标准的库子程序取得这一返回值，并将它送回用户程序。
　　为使系统调用的执行成为一项简单的任务，Linux提供了一组预处理宏指令。
　　它们可以用在程序中。这些宏指令取一定的参数，然后扩展为调用指定的系统调用的函数。
　　这些宏指令具有类似下面的名称格式：
_syscallN（parameters）
　　其中N是系统调用所需的参数数目，而parameters则用一组参数代替。这些参数使宏指令完成适合于特定的系统调用的扩展。例如，为了建立调用setuid（）系统调用的函数，应该使用：
_syscall1（ int， setuid， uid_t， uid ）
syscallN（）宏指令的第1个参数int说明产生的函数的返回值的类型是整型，第2个参数setuid说明产生的函数的名称。后面是系统调用所需要的每个参数。这一宏指令后面还有两个参数uid_t和uid分别用来指定参数的类型和名称。
　　另外，用作系统调用的参数的数据类型有一个限制，它们的容量不能超过四个字节。这是因为执行int ?{GetProperty(Content)}x80指令进行系统调用时，所有的参数值都存在32位的CPU寄存器中。使用CPU寄存器传递参数带来的另一个限制是可以传送给系统调用的参数的数目。这个限制是最多可以传递5个参数。所以Linux一共定义了6个不同的_syscallN（）宏指令，从_syscall0（）、_syscall1（）直到_syscall5（）。
　　一旦_syscallN（）宏指令用特定系统调用的相应参数进行了扩展，得到的结果是一个与系统调用同名的函数，它可以在用户程序中执行这一系统调用。
2 添加新的系统调用
　　如果用户在Linux中添加新的系统调用，应该遵循几个步骤才能添加成功，下面几个步骤详细说明了添加系统调用的相关内容。
　　（1）添加源代码
　　第一个任务是编写加到内核中的源程序，即将要加到一个内核文件中去的一个函数，该函数的名称应该是新的系统调用名称前面加上sys_标志。假设新加的系统调用为mycall(int number)，在/usr/src/linux/kernel/sys.c文件中添加源代码，如下所示：
asmlinkage int sys_mycall(int number) 
{ 
return number; 
} 
　　作为一个最简单的例子，我们新加的系统调用仅仅返回一个整型值。
　　（2）连接新的系统调用
　　添加新的系统调用后，下一个任务是使Linux内核的其余部分知道该程序的存在。为了从已有的内核程序中增加到新的函数的连接，需要编辑两个文件。
　　在我们所用的Linux内核版本（RedHat 6.0，内核为2.2.5-15）中，第一个要修改的文件是：
/usr/src/linux/include/asm-i386/unistd.h 
　　该文件中包含了系统调用清单，用来给每个系统调用分配一个唯一的号码。文件中每一行的格式如下：
#define __NR_name NNN 
　　其中，name用系统调用名称代替，而NNN则是该系统调用对应的号码。应该将新的系统调用名称加到清单的最后，并给它分配号码序列中下一个可用的系统调用号。我们的系统调用如下：
#define __NR_mycall 191 
　　系统调用号为191，之所以系统调用号是191，是因为Linux-2.2内核自身的系统调用号码已经用到190。
　　第二个要修改的文件是：
/usr/src/linux/arch/i386/kernel/entry.S 
　　该文件中有类似如下的清单：
.long SYMBOL_NAME（）
　　该清单用来对sys_call_table[]数组进行初始化。该数组包含指向内核中每个系统调用的指针。这样就在数组中增加了新的内核函数的指针。我们在清单最后添加一行：
.long SYMBOL_NAME(sys_mycall) 
　　（3）重建新的Linux内核
　　为使新的系统调用生效，需要重建Linux的内核。这需要以超级用户身份登录。
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
　　（4）用新的内核启动系统
　　要使用新的系统调用，需要用重建的新内核重新引导系统。为此，需要修改/etc/lilo.conf文件，在我们的系统中，该文件内容如下：
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
　　现在，当重新引导系统时，在boot:提示符后面有三种选择：linux-new 、linux、dos，新内核成为缺省的引导内核。
　　至此，新的Linux内核已经建立，新添加的系统调用已成为操作系统的一部分，重新启动Linux，用户就可以在应用程序中使用该系统调用了。
　　（5）使用新的系统调用
　　在应用程序中使用新添加的系统调用mycall。同样为实验目的，我们写了一个简单的例子xtdy.c。
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
　　注意，由于使用了系统调用，编译和执行程序时，用户都应该是超级用户身份
[http://blog.chinaunix.net/u2/62213/showart_488383.html](http://blog.chinaunix.net/u2/62213/showart_488383.html)
**linux ****添加系统调用**
一.源码修改
1下载一个与所用系统内核版本接近的内核，放在/usr/src下，解压，作个链接ln -s linux-2.6.18.1 linux
2修改：修改三个地方
1）/usr/src/linux/kerner/sys.c中添加，
asmlinkage int sys_mysyscall(int a)
{
           return a;
}
2）定义系统调用号,/usr/src/linux/include/asm-i386/unistd.h
#define _NR_sysmycall 318 //不能与前面已有的重复
#define _NR_syscalls  319//修改系统中所用系统调用数目
3)在系统调用向量表里添加自定义的系统调用函数入口位置，
/usr/src/linux/arch/i386/kernel/syscall_table.S，以前老版本是entry.s
.long sys_mysyscall
二、内核编译
1.在/boot下复制配置文件，到/usr/src/linux下，改名位config，make menuconfig,可以不用修改，直接退出
2.make clean 清空以前的编译痕迹
3.make，编译出来的是bzImage
4.make modules,make modules-install//编译、安装config里配置的模块
如不执行次步骤，对于有的系统，制作不了initrd文件。系统就启动不了
5.如果直接make install，系统会自动制作initrd文件，并复制initrd和bzimage文件到/boot下，修改grub.conf文件，重启系统，选择进入新内核
6.不使用make install命令。复制bzImage到/boot下，改名位vmlinuz-2.6.18.1，手工制作initrd文件，/mkinitrd initrd-2.6.18.1.img 2.6.18.1,initrd文件名位initrd-2.6.18.1
7.修改grub.conf文件，复制原来已有的启动设置，把title和kernel和initrd改名为新制作的即可
三、编写代码测试
int main(void)
{
   int a=syscall(318,100);//318是系统调用号，100是参数
   printf("%d\n",a);
   return 0;
}
syscall是内核提供为用户程序的一个函数，
如果不使用syscall函数，也可以使用宏定义，但是在2.6.20以后的版本里，没有宏定义，需要自己从其他版本里复制过来添加。
来源：http://blog.csdn.net/yangdelong/archive/2010/04/12/5479070.aspx

