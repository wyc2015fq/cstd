# printk()函数的总结 - DoubleLi - 博客园






我们在使用printk()函数中使用日志级别为的是使编程人员在编程过程中自定义地进行信息的输出，更加容易地掌握系统当前的状况。
对程序的调试起到了很重要的作用。
（下文中的日志级别和控制台日志控制级别是一个意思）

printk(日志级别 "消息文本")；这里的日志级别通俗的说指的是对文本信息的一种输出范围上的指定。
日志级别一共有8个级别，printk的日志级别定义如下（在linux26/includelinux/kernel.h中）：
#defineKERN_EMERG"<0>"/*紧急事件消息，系统崩溃之前提示，表示系统不可用*/
#defineKERN_ALERT"<1>"/*报告消息，表示必须立即采取措施*/
#defineKERN_CRIT"<2>"/*临界条件，通常涉及严重的硬件或软件操作失败*/
#defineKERN_ERR"<3>"/*错误条件，驱动程序常用KERN_ERR来报告硬件的错误*/
#defineKERN_WARNING"<4>"/*警告条件，对可能出现问题的情况进行警告*/
#defineKERN_NOTICE"<5>"/*正常但又重要的条件，用于提醒。常用于与安全相关的消息*/
#defineKERN_INFO"<6>"/*提示信息，如驱动程序启动时，打印硬件信息*/
#defineKERN_DEBUG"<7>"/*调试级别的消息*/

没有指定日志级别的printk语句默认采用的级别是 DEFAULT_ MESSAGE_LOGLEVEL（这个默认级别一般为<4>,即与KERN_WARNING在一个级别上），其定义在linux26/kernel/printk.c中可以找到。
下面是一个比较简单的使用
printk(KERN_INFO "INFO\n");  //这里可以使用数字代替 KERN_INFO，即可以写成printk(<6> "INFO\n");  
在这个格式的定义中，日志级别和信息文本之间不能够使用逗号隔开，因为系统在进行编译的时候，将日志级别转换成字符串于后面的文本信息进行连接。

在对系统输出进行控制时，主要是讨论控制台和伪终端的输情况，以及系统日志等。

下面是控制台日志级别的一些简要的介绍
控制台相应的日志级别定义如下：
#define MINIMUM_CONSOLE_LOGLEVEL　 1　 　/*可以使用的最小日志级别*/
#define DEFAULT_CONSOLE_LOGLEVEL 　7 /*比KERN_DEBUG 更重要的消息都被打印*/

int console_printk[4] = {
DEFAULT_CONSOLE_LOGLEVEL,/*控制台日志级别，优先级高于该值的消息将在控制台显示*/
/*默认消息日志级别，printk没定义优先级时，打印这个优先级以上的消息*/
DEFAULT_MESSAGE_LOGLEVEL,
/*最小控制台日志级别，控制台日志级别可被设置的最小值（最高优先级）*/
MINIMUM_CONSOLE_LOGLEVEL,
DEFAULT_CONSOLE_LOGLEVEL,/* 默认的控制台日志级别*/
};
在进行查看的时候，可以使用命令 cat /proc/sys/kernel/printk来查看这四个值
可以通过修改文件/proc/sys/kernel/printk中的第一个值来更改当前的控制台日志级别。

（声明：在下面的模块函数中控制台所使用的日志级别均为KERN_WARNING级别）当日志级别高于console_loglevel（控制台日志级别）时，消息才能在控制台显示出来。
假如我们写了一个如下的模块函数：
1 #include <linux/init.h>
2 #include <linux/module.h>
3 MODULE_LICENSE("Dual BSD/GPL");
4 static int book_init(void)
5 {
6   printk(KERN_EMERG "EMERG\n");
7   printk(KERN_ALERT "ALERT\n");
8   printk(KERN_CRIT " CRIT\n");
9   printk(KERN_ERR " ERR\n");
10  printk(KERN_WARNING ""WARNING\n");
11   printk(KERN_NOTICE "NOTICE\n");
12  printk(KERN_INFO "INFO\n");
13  printk(KERN_DEBUG "DEBUG\n");
14  return 0;
    }
15static void book_exit(void)
16{
17  printk(KERN_ALERT "Book module exit\n");
    }
18  module_init(book_init);
19  module_exit(book_exit);

在控制台（这里指的是虚拟终端  Ctrl+Alt+(F1~F6)）加载模块以后，控制台给出的信息为
6~9行中要求输出的信息，我们在伪终端（如果对伪终端不是很清楚可以看相关的内容）上运行命令tail -n 10 /var/log/messages查看日志文件刚才得到的运行记录
可以发现messages中的值为KERN_WARNING级别之后所要求输出到信息值。而如果我们在文件syslog和kern-log中查看系统日志文件，一般情况下可以得到所有的输出信息

Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637057] INFO
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637063] CRIT
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637066] WARNING
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637068] ERR
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637069] ALERT
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637070] EMERG
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637071]  NOTICE 
Jul 18 11:44:19 xiyoulinux-desktop kernel: [16100.637072] DEBUG
（不过在有些机器上运行得到的结果并不是这样的）
即一般情况下，syslog和kern.log两个文件中记录的内容从编程这个角度来看是基本一致的。
在目录/var/log/下有一下四个文件可以查看日志
syslog ，kern.log，messages ，DEBUG 。   
syslog和kern.log一般情况下可以得到所有的系统输出值，而messages得到的是比控制台日志级别低的输出值，DEBUG得到的仅仅是DEBUG级别的
输出值。
一般情况下，优先级高于控制台日志级别的消息将被打印到控制台。优先级低于控制台日志级别的消息将被打印到messages日志文件中，而在伪终端下不打印任何的信息。
我们在进行有关编程的时候，若使用到printk()这个函数，一般查看信息是在messages和虚拟终端下进行查看，而对于syslog和kern.log下是用来检验所有信息的输出情况。









