# LINUX下的tty，console与串口分析 - DoubleLi - 博客园






1、LINUX下TTY、CONSOLE、串口之间是怎样的层次关系？具体的函数接口是怎样的？串口是如何被调用的？ 

2、printk函数是把信息发送到控制台上吧？如何让PRINTK把信息通过串口送出？或者说系统在什么地方来决定是将信息送到显示器还是串口？ 


3、start_kernel中一开始就用到了printk函数（好象是printk(linux_banner什么的），在 这个时候整个内核还没跑起来呢那这时候的printk是如何被调用的？在我们的系统中，系统启动是用的现代公司的BOOTLOADER程序，后来好象跳到了LINUX下的head-armv.s， 然后跳到start_kernel，在bootloader 里串口已经是可用的了，那么在进入内核后是不是要重新设置？ 


以上问题可能问的比较乱，因为我自己脑子里也比较乱，主要还是对tty,console,serial之间的关系，特别是串口是如何被调用的没搞清这方面的资料又比较少（就情景分析中讲了一点），希望高手能指点一二，非常谢！


我最近也在搞这方面的东西，也是写一个串口设备的驱动 
搞了将近一个月了，其中上网找资料，看源代码，什么都做了 
但还是一蹋糊涂的，有些问题还是不明白，希望一起讨论讨论 

在/proc/device（没记错应该是这个文件） 
里面有一个叫serial的驱动，其主设备号是4，次设备号是64-12X（没记错应该是这个范围） 
大家都知道，串口的次设备号是从64开始的，串口1 /dev/ttyS0就对应次设备号64，串口2就对应65 
问题是现在我机上只有两个串口，它注册这么多次设备号来干什么？ 

对于一个接在串口1的设备，在我注册驱动的时候 
我是需要自己找一个主设备号呢？ 
还是就用主设备号4，次设备号从上面12X的后面选？ 
还是就用主设备号4，次设备号64？ 

在linux的内核中有一个tty层，我看好像有些串口驱动是从这里开始的 
例如调用tty_register_driver()来注册驱动 
就像在pci子系统里调用pci_register_driver()那样的 
那么，用这种机制来注册的驱动， 
它是直接对串口的端口操作呢（例如用inb(),outb()....之类的） 
还是某些更底层的驱动接口呢？ 

这些问题缠了我很久都没解决，搞得最后不得不放弃 
现在转向用户空间的应用程序，看能不能有些更高效的方法来实现 
（在用户空间只能用open("/dev/ttyS0", O_RDWR)来实现了）


另外还有，系统里已经为我们实现了串口的驱动 
所以我们在用户空间的程序里直接open("/dev/ttyS0")就可用了 
但是现在要写的是接在串口上的设备的驱动 
在内核模块中可不可以包含某个头文件，然后就可以直接用串口驱动中的接口呢？


看到你们的问题后，感觉很有典型性，因此花了点工夫看了一下，做了一些心得贴在这里，欢迎讨论并指正： 
1、LINUX下TTY、CONSOLE、串口之间是怎样的层次关系？具体的函数接口是怎样的？串口是如何被调用的？ 
tty和console这些概念主要是一些虚设备的概念，而串口更多的是指一个真正的设备驱动Tty实际是一类终端I/O设备的抽象，它实际上更多的是一个管理的概念，它和tty_ldisc（行规程）和tty_driver（真实设备驱动）组合在一起，目的是向上层的VFS提供一个统一的接口通过file_operations结构中的tty_ioctl可以对其进行配置[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)查tty_driver，你将得到n个结果，实际都是相关芯片的驱动因此，可以得到的结论是（实际情况比这复杂得多）：每个描述tty设备的tty_struct在初始化时必然挂如了某个具体芯片的字符设备驱动（不一定是字符设备驱动），可以是很多，包括显卡或串口chip不知道你的ARM Soc是那一款，不过看情况你们应该用的是常见的chip，这些驱动实际上都有而console是一个缓冲的概念，它的目的有一点类似于tty实际上console不仅和tty连在一起，还和framebuffer连在一起，具体的原因看下面的键盘的中断处理过程Tty的一个子集需要使用console(典型的如主设备号4，次设备号1―64)，但是要注意的是没有console的tty是存在的
而串口则指的是tty_driver举个典型的例子： 
分析一下键盘的中断处理过程： 
keyboard_interrupt―>handle_kbd_event―>handle_keyboard_event―>handle_scancode 
void handle_scancode(unsigned char scancode, int down) 
{ 
…….. 
tty = ttytab? ttytab[fg_console]: NULL; 
if (tty && (!tty->driver_data)) { 
…………… 
tty = NULL; 
} 
…………. 
schedule_console_callback(); 
} 
这段代码中的两个地方很值得注意，也就是除了获得tty外（通过全局量tty记录），还进行了console 回显schedule_console_callbackTty和console的关系在此已经很明了！！！ 

2、printk函数是把信息发送到控制台上吧？如何让PRINTK把信息通过串口送出？或者说系统在什么地方来决定是将信息送到显示器还是串口？ 
具体看一下printk函数的实现就知道了，printk不一定是将信息往控制台上输出，设置kernel的启动参数可能可以打到将信息送到显示器的效果。函数前有一段英文，很有意思： 
/*This is printk. It can be called from any context. We want it to work. 
* 
* We try to grab the console_sem. If we succeed, it's easy - we log the output and 
* call the console drivers. If we fail to get the semaphore we place the output 
* into the log buffer and return. The current holder of the console_sem will 
* notice the new output in release_console_sem() and will send it to the 
* consoles before releasing the semaphore. 
* 
* One effect of this deferred printing is that code which calls printk() and 
* then changes console_loglevel may break. This is because console_loglevel 
* is inspected when the actual printing occurs. 
*/ 
这段英文的要点：要想对console进行操作，必须先要获得console_sem信号量如果获得console_sem信号量，则可以“log the output and call the console drivers”，反之，则“place the output into the log buffer and return”，实际上，在代码： 
asmlinkage int printk(const char *fmt, ...) 
{ 
va_list args; 
unsigned long flags; 
int printed_len; 
char *p; 
static char printk_buf[1024]; 
static int log_level_unknown = 1; 
if (oops_in_progress) { /*如果为1情况下，必然是系统发生crush*/ 
/* If a crash is occurring, make sure we can't deadlock */ 
spin_lock_init(&logbuf_lock); 
/* And make sure that we print immediately */ 
init_MUTEX(&console_sem); 
} 
/* This stops the holder of console_sem just where we want him */ 
spin_lock_irqsave(&logbuf_lock, flags); 
/* Emit the output into the temporary buffer */ 
va_start(args, fmt); 
printed_len = vsnprintf(printk_buf, sizeof(printk_buf), fmt, args);/*对传入的buffer进行处理，注意还不是 
真正的对终端写，只是对传入的string进行格式解析*/ 
va_end(args); 
/*Copy the output into log_buf. If the caller didn't provide appropriate log level tags, we insert them here*/ 
/*注释很清楚*/ 
for (p = printk_buf; *p; p++) { 
if (log_level_unknown) { 
if (p[0] != '<' || p[1] < '0' || p[1] > '7' || p[2] != '>') { 
emit_log_char('<'); 
emit_log_char(default_message_loglevel + '0'); 
emit_log_char('>'); 
} 
log_level_unknown = 0; 
} 
emit_log_char(*p); 
if (*p == '\n') 
log_level_unknown = 1; 
} 
if (!arch_consoles_callable()) { 
/*On some architectures, the consoles are not usable on secondary CPUs early in the boot process.*/ 
spin_unlock_irqrestore(&logbuf_lock, flags); 
goto out; 
} 
if (!down_trylock(&console_sem)) { 
/*We own the drivers. We can drop the spinlock and let release_console_sem() print the text*/ 
spin_unlock_irqrestore(&logbuf_lock, flags); 
console_may_schedule = 0; 
release_console_sem(); 
} else { 
/*Someone else owns the drivers. We drop the spinlock, which allows the semaphore holder to 
proceed and to call the console drivers with the output which we just produced.*/ 
spin_unlock_irqrestore(&logbuf_lock, flags); 
} 
out: 
return printed_len; 
} 
实际上printk是将format后的string放到了一个buffer中，在适当的时候再加以show,这也回答了在start_kernel中一开始就用到了printk函数的原因 

3、start_kernel中一开始就用到了printk函数（好象是printk(linux_banner什么的)，在这个时候整个内核还没跑起来呢。那这时候的printk是如何被调用的？在我们的系统中，系统启动是用的现代公司的BOOTLOADER程序，后来好象跳到了LINUX下的head-armv.s， 然后跳到start_kernel，在bootloader 里串口已经是可用的了，那么在进入内核后是不是要重新设置？ 
Bootloader一般会做一些基本的初始化，将kernel拷贝物理空间，然后再跳到kernel去执行。可以肯定的是kernel肯定要对串口进行重新设置，原因是Bootloader有很多种，有些不一定对串口进行设置，内核不能依赖于bootloader而存在。




多谢楼上大侠，分析的很精辟。我正在看printk函数。

我们用的CPU是hynix的hms7202。在评估板上是用串口0作 
控制台，所有启动过程中的信息都是通过该串口送出的。 
在bootloader中定义了函数ser_printf通过串口进行交互。

但我还是没想明白在跳转到linux内核而console和串口尚未 
初始化时printk是如何能够工作的？我看了start_kernel 
的过程（并通过超级终端作了一些跟踪），console的初始化 
是在console_init函数里，而串口的初始化实际上是在1号 
进程里（init->do_basic_setup->do_initcalls->rs_init)， 
那么在串口没有初始化以前prink是如何工作的？特别的，在 
start_kernel一开始就有printk(linux_banner)，而这时候 
串口和console都尚未初始化呢。



1.在start_kernel一开始就有printk(linux_banner)，而这时候串口和console都尚未初始化? 
仔细分析printk可以对该问题进行解答代码中的： 
/* Emit the output into the temporary buffer */ 
va_start(args, fmt); 
printed_len = vsnprintf(printk_buf, sizeof(printk_buf), fmt, args); 
va_end(args); 
将输入放到了printk_buf中，接下来的 
for (p = printk_buf; *p; p++) { 
if (log_level_unknown) { 
if (p[0] != '<' || p[1] < '0' || p[1] > '7' || p[2] != '>') { 
emit_log_char('<'); 
emit_log_char(default_message_loglevel + '0'); 
emit_log_char('>'); 
} 
log_level_unknown = 0; 
} 
emit_log_char(*p); 
if (*p == '\n') 
log_level_unknown = 1; 
} 
则将printk_buf中的内容进行解析并放到全局的log_buf（在emit_log_char函数）中if (!down_trylock(&console_sem)) { 
/* 
* We own the drivers. We can drop the spinlock and let 
* release_console_sem() print the text 
*/ 
spin_unlock_irqrestore(&logbuf_lock, flags); 
console_may_schedule = 0; 
release_console_sem(); 
} else { 
/* 
* Someone else owns the drivers. We drop the spinlock, which 
* allows the semaphore holder to proceed and to call the 
* console drivers with the output which we just produced. 
*/ 
spin_unlock_irqrestore(&logbuf_lock, flags); 
} 
则是根据down_trylock(&console_sem)的结果调用release_console_sem()，在release_console_sem()中才真正的对全局的log_buf中的内容相应的console设备驱动进行处理[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)至此，可以得到如下的一些结论： 
（1）printk的主操作实际上还是针对一个buffer(log_buf),该buffer中的内容是否显示（或者说向终端输出），则要看是否可以获得console_sem（2）printk所在的文件为printk.c，是和体系结构无关的，因此对任何平台都一样[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)可以推测的结论是：
（1）kernel在初始化时将console_sem标为了locked，因此在start_kernel一开始的printk(linux_banner)中实际只将输入写入了缓冲，等在串口和console初始化后，对printk的调用才一次将缓冲中的内容向串口和console输出[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)（2）在串口和console的初始化过程中，必然有对console_sem的up操作[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)
（3）因此，在embedded的调试中，如果在console的初始化之前系统出了问题，不会有任何的输出[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)唯一可以使用的只能是led或jtag了[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)（4）因此，你的问题可以看出解答[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)2.console的初始化. 
不知道你用的是那一个内核版本，在我看的2.4.18和2.4.19中，都是在start_kernel中就对console进行的初始化[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)从前面的分析来看，console的初始化不应该太晚，否则log_buf有可能溢出[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)

多谢楼上，分析的很精彩！ 

我们用的内核版本是2.4.18，console的初始化确实是在 
start_kernel->console->init[。](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)关于tty和串口，我这里还想再问一下tty设备的操作的总入口 
是 

static struct file_operations tty_fops = { 
llseek: no_llseek, 
read: tty_read, 
write: tty_write, 
poll: tty_poll, 
ioctl: tty_ioctl, 
open: tty_open, 
release: tty_release, 
fasync: tty_fasync, 
}; 

而对串口的操作定义在： 

static struct tty_driver serial_driver 这个结构中
serial.c中的多数函数都是填充serial_driver中的函数指针
那么在对串口操作时，应该是先调用tty_fops中的操作（比如 
tty_open等），然后再分流到具体的串口操作（rs_open等）吧？ 
但tty_driver（对串口就是serial_driver)中有很多函数指针 
并不跟file_operations中的函数指针对应，不知道这些对应 
不上的操作是如何被执行的？比如put_char,flush_char,read_proc, 
write_proc,start,stop等。


以下是我对这个问题的一些理解： 
这实际上还是回到原先的老问题，即tty和tty_driver之间的关系。从实现上看，tty_driver实际上是tty机制的实现组件之一，借用面向对象设计中的常用例子，这时的tty_driver就象是tty这部汽车的轮胎，tty这部汽车要正常运行，还要tty_ldisc（行规程），termios，甚至struct tq_struct tq_hangup（看tty_struct）等基础设施。它们之间的关系并非继承。至于tty_driver中的函数指针，再打个C++中的比喻，它们实际上很象虚函数，也就是说，可以定义它们，但并不一定实现它们[、](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)实际上还不用说tty_driver，只要查一下serial_driver都会发现n多个具体的实现，但对各个具体的设备，其tty_driver中的函数不一定全部实现[、](http://www.wantso.com/bbs/h(71)ttp://www.wantso.com/bbs)所以put_char,flush_char,read_proc, write_proc,start,stop这些函数的情况是有可能实现，也有可能不实现 即使被实现，也不一定为上层(VFS层)所用.













