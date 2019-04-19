# Linux /dev目录详解和Linux系统各个目录的作用 - maopig的专栏 - CSDN博客
2012年01月11日 23:08:40[maopig](https://me.csdn.net/maopig)阅读数：103738标签：[linux																[tcp																[function																[class																[firefox																[timer](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
# Linux /dev目录详解
　　　　在linux下，/dev目录是很重要的，各种设备都在下面。下面简单总结一下：
　　dev是设备(device)的英文缩写。/dev这个目录对所有的用户都十分重要。因为在这个目录中包含了所有Linux系统中使用的外部设备。但是这里并不是放的外部设备的驱动程序，这一点和**[windows](http://www.ltesting.net/html/75/category-catid-375.html)**,dos操作系统不一样。它实际上是一个访问这些外部设备的端口。我们可以非常方便地去访问这些外部设备，和访问一个文件，一个目录没有任何区别。
　　Linux沿袭**[Unix](http://www.ltesting.net/html/76/category-catid-376.html)**的风格，将所有设备认成是一个文件。
　　关于一些特殊设备，我曾经总结过一些：
　　/dev/null和/dev/zero：http://www.51testing.com/index.php?uid-225738-action-viewspace-itemid-235339
　　/dev/tty、/dev/console、/dev/pty、/dev/pts等：http://www.51testing.com/index.php?uid-225738-action-viewspace-itemid-212419
设备文件分为两种：块设备文件(b)和字符设备文件(c)
　　设备文件一般存放在/dev目录下，对常见设备文件作如下说明：
　　/dev/hd[a-t]：IDE设备
　　/dev/sd[a-z]：SCSI设备
　　/dev/fd[0-7]：标准软驱
　　/dev/md[0-31]：软raid设备
　　/dev/loop[0-7]：本地回环设备
　　/dev/ram[0-15]：内存
　　/dev/null：无限数据接收设备,相当于黑洞
　　/dev/zero：无限零资源
　　/dev/tty[0-63]：虚拟终端
　　/dev/ttyS[0-3]：串口
　　/dev/lp[0-3]：并口
　　/dev/console：控制台
　　/dev/fb[0-31]：framebuffer
　　/dev/cdrom => /dev/hdc
　　/dev/modem => /dev/ttyS[0-9]
　　/dev/pilot => /dev/ttyS[0-9]
　　/dev/random：随机数设备
　　/dev/urandom：随机数设备
　　(PS：随机数设备，后面我会再写篇博客总结一下)
　　/dev目录下的节点是怎么创建的?
　　devf或者udev会自动帮你创建得。
　　kobject是sysfs文件系统的基础，udev通过监测、检测sysfs来获取新创建的设备的。
Linux系统各个目录的作用
　/
　　这就是根目录。对你的电脑来说，有且只有一个根目录。所有的东西，我是说所有的东西都是从这里开始。举个例子：当你在终端里输入“/home”，你其实是在告诉电脑，先从/（根目录）开始，再进入到home目录。
/root
　　这是系统管理员（root user）的目录。对于系统来说，系统管理员就好比是上帝，它能对系统做任何事情，甚至包括删除你的文件。因此，请小心使用root帐号。
/bin
　　这里存放了标准的（或者说是缺省的）linux的工具，比如像“ls”、“vi”还有“more”等等。通常来说，这个目录已经包含在你的“path”系统变量里面了。什么意思呢？就是：当你在终端里输入ls，系统就会去/bin目录下面查找是不是有ls这个程序。
/etc
　　这里主要存放了系统配置方面的文件。举个例子：你安装了samba这个套件，当你想要修改samba配置文件的时候，你会发现它们（配置文件）就在/etc/samba目录下。
/dev
　　这里主要存放与设备（包括外设）有关的文件（unix和linux系统均把设备当成文件）。想连线打印机吗？系统就是从这个目录开始工作的。另外还有一些包括磁盘驱动、USB驱动等都放在这个目录。
/home
　　这里主要存放你的个人数据。具体每个用户的设置文件，用户的桌面文件夹，还有用户的数据都放在这里。每个用户都有自己的用户目录，位置为：/home/用户名。当然，root用户除外。
/tmp
　　这是临时目录。对于某些程序来说，有些文件被用了一次两次之后，就不会再被用到，像这样的文件就放在这里。有些linux系统会定期自动对这个目录进行清理，因此，千万不要把重要的数据放在这里。
/usr
　　在这个目录下，你可以找到那些不适合放在/bin或/etc目录下的额外的工具。比如像游戏阿，一些打印工具拉等等。/usr目录包含了许多子目录：/usr/bin目录用于存放程序；/usr/share用于存放一些共享的数据，比如音乐文件或者图标等等；/usr/lib目录用于存放那些不能直接运行的，但却是许多程序运行所必需的一些函数库文件。你的软件包管理器（应该是“新立得”吧）会自动帮你管理好/usr目录的。
/opt
　　这里主要存放那些可选的程序。你想尝试最新的firefox测试版吗？那就装到/opt目录下吧，这样，当你尝试完，想删掉firefox的时候，你就可以直接删除它，而不影响系统其他任何设置。安装到/opt目录下的程序，它所有的数据、库文件等等都是放在同个目录下面。
　　举个例子：刚才装的测试版firefox，就可以装到/opt/firefox_beta目录下，/opt/firefox_beta目录下面就包含了运行firefox所需要的所有文件、库、数据等等。要删除firefox的时候，你只需删除/opt/firefox_beta目录即可，非常简单。
/usr/local
　　这里主要存放那些手动安装的软件，即不是通过“新立得”或apt-get安装的软件。它和/usr目录具有相类似的目录结构。让软件包管理器来管理/usr目录，而把自定义的脚本（scripts）放到/usr/local目录下面，我想这应该是个不错的主意。
　/media
　　有些linux的发行版使用这个目录来挂载那些usb接口的移动硬盘（包括U盘）、CD/DVD驱动器等等。
Linux中/proc目录下文件详解（一）
--------------------------------------------------------------------------------
**/proc**文件系统下的多种文件提供的系统信息不是针对某个特定进程的，而是能够在整个系统范围的上下文中使用。可以使用的文件随系统配置的变化而变化。命令procinfo能够显示基于其中某些文件的多种系统信息。以下详细描述/proc下的文件。
--------------------------------------------------------------------------------
**/proc/cmdline文件**
这个文件给出了内核启动的命令行。它和用于进程的cmdline项非常相似。
**示例： **
[root@localhost proc]# cat cmdline
ro root=LABEL=/ rhgb quiet
--------------------------------------------------------------------------------
**/proc/cpuinfo文件**
这个文件提供了有关系统CPU的多种信息。这些信息是从内核里对CPU的测试代码中得到的。文件列出了CPU的普通型号（386，486，586，686等），以及能得到的更多特定信息（制造商，型号和版本）。文件还包含了以bogomips表示的处理器速度，而且如果检测到CPU的多种特性或者bug，文件还会包含相应的标志。这个文件的格式为：文件由多行构成，每行包括一个域名称，一个冒号和一个值。
**示例： **
[root@localhost proc]# cat cpuinfo
processor : 0
vendor_id : AuthenticAMD
cpu family : 6
model : 8
model name : AMD Athlon(tm) XP 1800+
stepping : 1
cpu MHz : 1530.165
cache size : 256 KB
fdiv_bug : no
hlt_bug : no
f00f_bug : no
coma_bug : no
fpu : yes
fpu_exception : yes
cpuid level : 1
wp : yes
flags : fpu vme de pse tsc msr pae mce cx8 apic mtrr pge mca cmov pat pse36 mmx fxsr sse syscall mmxext 3dnowext 3dnow
bogomips : 2998.27
--------------------------------------------------------------------------------
**/proc/devices文件**
这个文件列出字符和块设备的主设备号，以及分配到这些设备号的设备名称。
**示例： **
[root@localhost /]# cat /proc/devices
Character devices:
1 mem
4 /dev/vc/0
4 tty
4 ttyS
5 /dev/tty
5 /dev/console
5 /dev/ptmx
6 lp
7 vcs
10 misc
13 input
14 sound
29 fb
36 netlink
116 alsa
128 ptm
136 pts
180 usb
Block devices:
1 ramdisk
2 fd
3 ide0
9 md
22 ide1
253 device-mapper
254 mdp 
--------------------------------------------------------------------------------
/proc/dma文件 
这个文件列出由驱动程序保留的DMA通道和保留它们的驱动程序名称。casade项供用于把次DMA控制器从主控制器分出的DMA行所使用；这一行不能用于其它用途。
示例：
[root@localhost ~]# cat /proc/dma
4: cascade
--------------------------------------------------------------------------------
/proc/filesystems文件
这个文件列出可供使用的文件系统类型，一种类型一行。虽然它们通常是编入内核的文件系统类型，但该文件还可以包含可加载的内核模块加入的其它文件系统类型。
示例：
[root@localhost proc]# cat /proc/filesystems 
nodev sysfs
nodev rootfs
nodev bdev
nodev proc
nodev sockfs
nodev binfmt_misc
nodev usbfs
nodev usbdevfs
nodev futexfs
nodev tmpfs
nodev pipefs
nodev eventpollfs
nodev devpts
ext2
nodev ramfs
nodev hugetlbfs
iso9660
nodev mqueue
nodev selinuxfs
ext3
nodev rpc_pipefs
nodev autofs 
--------------------------------------------------------------------------------
/proc/interrupts文件
这个文件的每一行都有一个保留的中断。每行中的域有：中断号，本行中断的发生次数，可能带有一个加号的域（SA_INTERRUPT标志设置），以及登记这个中断的驱动程序的名字。可以在安装新硬件前，像查看/proc/dma和/proc/ioports一样用cat命令手工查看手头的这个文件。这几个文件列出了当前投入使用的资源（但是不包括那些没有加载驱动程序的硬件所使用的资源）。
示例：
[root@localhost SPECS]# cat /proc/interrupts
CPU0
0: 7039406 XT-PIC timer
1: 6533 XT-PIC i8042
2: 0 XT-PIC cascade
3: 0 XT-PIC uhci_hcd
5: 108 XT-PIC VIA8233, uhci_hcd
8: 1 XT-PIC rtc
9: 0 XT-PIC acpi
10: 0 XT-PIC ehci_hcd
11: 17412 XT-PIC uhci_hcd, eth0
12: 140314 XT-PIC i8042
14: 37897 XT-PIC ide0
15: 60813 XT-PIC ide1
NMI: 0
ERR: 1 
--------------------------------------------------------------------------------
/proc/ioports文件
这个文件列出了诸如磁盘驱动器，以太网卡和声卡设备等多种设备驱动程序登记的许多I/O端口范围。
示例：
[root@localhost SPECS]# cat /proc/ioports
0000-001f : dma1
0020-0021 : pic1
0040-0043 : timer0
0050-0053 : timer1
0060-006f : keyboard
0070-0077 : rtc
0080-008f : dma page reg
00a0-00a1 : pic2
00c0-00df : dma2
00f0-00ff : fpu
0170-0177 : ide1
01f0-01f7 : ide0
0376-0376 : ide1
0378-037a : parport0
037b-037f : parport0
03c0-03df : vga+
03f6-03f6 : ide0
03f8-03ff : serial
0800-0803 : PM1a_EVT_BLK
0804-0805 : PM1a_CNT_BLK
0808-080b : PM_TMR
0810-0815 : ACPI CPU throttle
0820-0823 : GPE0_BLK
0cf8-0cff : PCI conf1
dc00-dcff : 0000:00:12.0
dc00-dcff : via-rhine
e000-e0ff : 0000:00:11.5
e000-e0ff : VIA8233
e400-e41f : 0000:00:10.0
e400-e41f : uhci_hcd
e800-e81f : 0000:00:10.1
e800-e81f : uhci_hcd
ec00-ec1f : 0000:00:10.2
ec00-ec1f : uhci_hcd
fc00-fc0f : 0000:00:11.1
fc00-fc07 : ide0
fc08-fc0f : ide1 
--------------------------------------------------------------------------------
/proc/kcore文件
这个文件是系统的物理内存以core文件格式保存的文件。例如，GDB能用它考察内核的数据结构。它不是纯文本，而是/proc目录下为数不多的几个二进制格式的项之一。
示例：
暂无 
--------------------------------------------------------------------------------
/proc/kmsg文件
这个文件用于检索用printk生成的内核消息。任何时刻只能有一个具有超级用户权限的进程可以读取这个文件。也可以用系统调用syslog检索这些消息。通常使用工具dmesg或守护进程klogd检索这些消息。
示例：
暂无 
--------------------------------------------------------------------------------
/proc/ksyms文件
这个文件列出了已经登记的内核符号；这些符号给出了变量或函数的地址。每行给出一个符号的地址，符号名称以及登记这个符号的模块。程序ksyms,insmod和kmod使用这个文件。它还列出了正在运行的任务数，总任务数和最后分配的PID。
示例：
暂无 
--------------------------------------------------------------------------------
/proc/loadavg文件
这个文件给出以几个不同的时间间隔计算的系统平均负载，这就如同uptime命令显示的结果那样。前三个数字是平均负载。这是通过计算过去1分钟，5分钟，15分钟里运行队列中的平均任务数得到的。随后是正在运行的任务数和总任务数。最后是上次使用的进程号。
示例：
[root@localhost ~]# cat /proc/loadavg
0.11 0.16 0.14 3/126 3912 
--------------------------------------------------------------------------------
/proc/locks文件
这个文件包含在打开的文件上的加锁信息。文件中的每一行描述了特定文件和文档上的加锁信息以及对文件施加的锁的类型。内核也可以需要时对文件施加强制性锁。
示例：
[root@localhost redhat]# cat /proc/locks
1: POSIX ADVISORY READ 3822 03:0a:1067117 0 EOF
2: POSIX ADVISORY READ 3822 03:0a:1067138 0 EOF
3: POSIX ADVISORY WRITE 3326 03:0a:2326540 0 EOF
4: POSIX ADVISORY WRITE 2639 03:0a:2966595 0 EOF
5: FLOCK ADVISORY WRITE 2591 03:0a:2966586 0 EOF
6: POSIX ADVISORY WRITE 2540 03:0a:2966578 0 EOF
7: POSIX ADVISORY WRITE 2530 03:0a:2966579 0 EOF
8: POSIX ADVISORY WRITE 2402 03:0a:2966563 0 EOF
9: POSIX ADVISORY WRITE 2371 03:0a:2966561 0 EOF 
proc/mdstat文件
这个文件包含了由md设备驱动程序控制的RAID设备信息。
示例：
[root@localhost ~]# cat /proc/mdstat
Personalities :
unused devices: <none>
--------------------------------------------------------------------------------
/proc/meminfo文件
这个文件给出了内存状态的信息。它显示出系统中空闲内存，已用物理内存和交换内存的总量。它还显示出内核使用的共享内存和缓冲区总量。这些信息的格式和free命令显示的结果类似。
示例：
[root@localhost ~]# cat /proc/meminfo
MemTotal:    223812 kB
MemFree:       3764 kB
Buffers:       9148 kB
Cached:       92112 kB
SwapCached:        364 kB
Active:       183640 kB
Inactive:        17196 kB
HighTotal:           0 kB
HighFree:          0 kB
LowTotal:    223812 kB
LowFree:       3764 kB
SwapTotal:    626524 kB
SwapFree:    620328 kB
Dirty:              12 kB
Writeback:           0 kB
Mapped:       142880 kB
Slab:          12668 kB
Committed_AS: 376732 kB
PageTables:    2336 kB
VmallocTotal:   3907576 kB
VmallocUsed:    2968 kB
VmallocChunk:   3904224 kB
HugePages_Total:     0
HugePages_Free:    0
Hugepagesize:     4096 kB
--------------------------------------------------------------------------------
/proc/misc文件
这个文件报告用内核函数misc_register登记的设备驱动程序。
示例：
[root@localhost ~]# cat /proc/misc
63 device-mapper
175 agpgart
135 rtc 
--------------------------------------------------------------------------------
/proc/modules文件
这个文件给出可加载内核模块的信息。lsmod程序用这些信息显示有关模块的名称，大小，使用数目方面的信息。
示例：
[root@localhost /]# cat /proc/modules
md5 4033 1 - Live 0x10a7f000
ipv6 232577 8 - Live 0x10b0c000
parport_pc 24705 1 - Live 0x10a8b000
lp 11565 0 - Live 0x10a7b000
parport 41737 2 parport_pc,lp, Live 0x10a55000
autofs4 24005 0 - Live 0x10a74000
i2c_dev 10433 0 - Live 0x109d2000
i2c_core 22081 1 i2c_dev, Live 0x10a6d000
sunrpc 160421 1 - Live 0x10a9d000
ipt_REJECT 6465 1 - Live 0x109da000
ipt_state 1857 5 - Live 0x109eb000
ip_conntrack 40693 1 ipt_state, Live 0x10a62000
iptable_filter 2753 1 - Live 0x10896000
ip_tables 16193 3 ipt_REJECT,ipt_state,iptable_filter, Live 0x109ed000
dm_mod 54741 0 - Live 0x109f8000
button 6481 0 - Live 0x10905000
battery 8517 0 - Live 0x109d6000
ac 4805 0 - Live 0x10908000
uhci_hcd 31449 0 - Live 0x109dd000
ehci_hcd 31557 0 - Live 0x10949000
snd_via82xx 27237 2 - Live 0x10953000
snd_ac97_codec 64401 1 snd_via82xx, Live 0x10912000
snd_pcm_oss 47609 0 - Live 0x1093c000
snd_mixer_oss 17217 2 snd_pcm_oss, Live 0x1090c000
snd_pcm 97993 2 snd_via82xx,snd_pcm_oss, Live 0x10923000
snd_timer 29765 1 snd_pcm, Live 0x108ec000
snd_page_alloc 9673 2 snd_via82xx,snd_pcm, Live 0x108bd000
gameport 4801 1 snd_via82xx, Live 0x108a6000
snd_mpu401_uart 8769 1 snd_via82xx, Live 0x108b9000
snd_rawmidi 26725 1 snd_mpu401_uart, Live 0x108e4000
snd_seq_device 8137 1 snd_rawmidi, Live 0x1083b000
snd 54053 11 snd_via82xx,snd_ac97_codec,snd_pcm_oss,snd_mixer_oss,snd_pcm,snd_timer,snd_mpu401_uart,snd_rawmidi,snd_seq_device, Live 0x108f6000
soundcore 9889 2 snd, Live 0x1089b000
via_rhine 23497 0 - Live 0x1089f000
mii 4673 1 via_rhine, Live 0x10893000
floppy 58609 0 - Live 0x108a9000
ext3 116809 1 - Live 0x10875000
jbd 74969 1 ext3, Live 0x10861000
lsmod命令显示结果如下：
[root@localhost /]# lsmod
Module                Size   Used by
md5                   4033   1
ipv6                232577   8
parport_pc          24705   1
lp                   11565   0
parport             41737   2 parport_pc,lp
autofs4             24005   0
i2c_dev             10433   0
i2c_core             22081   1 i2c_dev
sunrpc             160421   1
ipt_REJECT              6465   1
ipt_state             1857   5
ip_conntrack           40693   1 ipt_state
iptable_filter       2753   1
ip_tables              16193   3 ipt_REJECT,ipt_state,iptable_filter
dm_mod                 54741   0
button                6481   0
battery                 8517   0
ac                   4805   0
uhci_hcd             31449   0
ehci_hcd             31557   0
snd_via82xx          27237   2
snd_ac97_codec       64401   1 snd_via82xx
snd_pcm_oss          47609   0
snd_mixer_oss       17217   2 snd_pcm_oss
snd_pcm             97993   2 snd_via82xx,snd_pcm_oss
snd_timer              29765   1 snd_pcm
snd_page_alloc       9673   2 snd_via82xx,snd_pcm
gameport             4801   1 snd_via82xx
snd_mpu401_uart       8769   1 snd_via82xx
snd_rawmidi          26725   1 snd_mpu401_uart
snd_seq_device       8137   1 snd_rawmidi
snd                    54053   11 snd_via82xx,snd_ac97_codec,snd_pcm_oss,snd_mixer_oss,snd_pcm,snd_timer,snd_mpu401_uart,snd_rawmidi,snd_seq_device
soundcore             9889   2 snd
via_rhine              23497   0
mii                   4673   1 via_rhine
floppy                 58609   0
ext3                116809   1
jbd                    74969   1 ext3 
--------------------------------------------------------------------------------
/proc/mounts文件
这个文件以/etc/mtab文件的格式给出当前系统所安装的文件系统信息。这个文件也能反映出任何手工安装从而在/etc/mtab文件中没有包含的文件系统。
示例：
[root@localhost /]# cat /proc/mounts
rootfs / rootfs rw 0 0
/proc /proc proc rw,nodiratime 0 0
none /dev tmpfs rw 0 0
/dev/root / ext3 rw 0 0
none /dev tmpfs rw 0 0
none /selinux selinuxfs rw 0 0
/proc /proc proc rw,nodiratime 0 0
/proc/bus/usb /proc/bus/usb usbfs rw 0 0
/sys /sys sysfs rw 0 0
none /dev/pts devpts rw 0 0
none /dev/shm tmpfs rw 0 0
none /proc/sys/fs/binfmt_misc binfmt_misc rw 0 0
sunrpc /var/lib/nfs/rpc_pipefs rpc_pipefs rw 0 0 
--------------------------------------------------------------------------------
/proc/pci文件
这个文件给出PCI设备的信息。用它可以方便地诊断PCI问题。你可以从这个文件中检索到的信息包括诸如IDE接口或USB控制器这样的设备，总线，设备和功能编号，设备延迟以及IRQ编号。
示例：
[root@localhost /]# cat /proc/pci
PCI devices found:
   Bus   0, device 0, function   0:
Class 0600: PCI device 1106:3116 (rev 0).
   Master Capable.   Latency=8.
   Prefetchable 32 bit memory at 0xe0000000 [0xe7ffffff].
   Bus   0, device 1, function   0:
Class 0604: PCI device 1106:b091 (rev 0).
   Master Capable.   No bursts.   Min Gnt=12.
   Bus   0, device   16, function   2:
Class 0c03: PCI device 1106:3038 (rev 12.
   IRQ 5.
   Master Capable.   Latency=32.
   I/O at 0xec00 [0xec1f].
   Bus   0, device   16, function   1:
Class 0c03: PCI device 1106:3038 (rev 12.
   IRQ 3.
   Master Capable.   Latency=32.
   I/O at 0xe800 [0xe81f].
   Bus   0, device   16, function   0:
Class 0c03: PCI device 1106:3038 (rev 12.
   IRQ 11.
   Master Capable.   Latency=32.
   I/O at 0xe400 [0xe41f].
   Bus   0, device   16, function   3:
Class 0c03: PCI device 1106:3104 (rev 130).
   IRQ 10.
   Master Capable.   Latency=32.
   Non-prefetchable 32 bit memory at 0xdfffff00 [0xdfffffff].
   Bus   0, device   17, function   0:
Class 0601: PCI device 1106:3177 (rev 0).
   Bus   0, device   17, function   1:
Class 0101: PCI device 1106:0571 (rev 6).
   IRQ 255.
   Master Capable.   Latency=32.
   I/O at 0xfc00 [0xfc0f].
   Bus   0, device   17, function   5:
Class 0401: PCI device 1106:3059 (rev 80).
   IRQ 5.
   I/O at 0xe000 [0xe0ff].
   Bus   0, device   18, function   0:
Class 0200: PCI device 1106:3065 (rev 116).
   IRQ 11.
   Master Capable.   Latency=32.   Min Gnt=3.Max Lat=8.
   I/O at 0xdc00 [0xdcff].
   Non-prefetchable 32 bit memory at 0xdffffe00 [0xdffffeff].
   Bus   1, device 0, function   0:
Class 0300: PCI device 5333:8d04 (rev 0).
   IRQ 11.
   Master Capable.   Latency=32.   Min Gnt=4.Max Lat=255.
   Non-prefetchable 32 bit memory at 0xdfe80000 [0xdfefffff].
   Prefetchable 32 bit memory at 0xd0000000 [0xd7ffffff]. 
--------------------------------------------------------------------------------
/proc/stat文件 
这个文件包含的信息有CPU利用率，磁盘，内存页，内存对换，全部中断，接触开关以及赏赐自举时间（自1970年1月1日起的秒数）。
示例：
[root@localhost /]# cat /proc/stat
cpu   31994 3898 7161 381600 15254 451 0
cpu0 31994 3898 7161 381600 15254 451 0
intr 4615930 4404290 3364 0 0 12 0 7 0 2 0 0 12618 112114 0 44142 39381
ctxt 1310498
btime 1148891913
processes 4249
procs_running 4
procs_blocked 0 
--------------------------------------------------------------------------------
/proc/uptime文件
这个文件给出自从上次系统自举以来的秒数，以及其中有多少秒处于空闲。这主要供uptime程序使用。比较这两个数字能够告诉你长期来看CPU周期浪费的比例。
示例：
[root@localhost /]# cat /proc/uptime
4477.04 4021.10 
--------------------------------------------------------------------------------
/proc/version文件
这个文件只有一行内容，说明正在运行的内核版本。可以用标准的编程方法进行分析获得所需的系统信息。
示例：
[root@localhost /]# cat /proc/version
Linux version 2.6.9-1.667 ([bhcompile@tweety.build.redhat.com](mailto:bhcompile@tweety.build.redhat.com)) (gcc version 3.4.2 20041017 (Red Hat 3.4.2-6.fc3)) #1 Tue Nov 2 14:41:25 EST 2004
proc/net子目录
此目录下的文件描述或修改了联网代码的行为。可以通过使用arp,netstat,route和ipfwadm命令设置或查询这些特殊文件中的许多文件。
示例：
[root@localhost /]# ls /proc/net
anycast6    ip_conntrack       mcfilter6   rt6_stats     tcp
arp          ip_conntrack_expect   netlink rt_acct    tcp6
dev          ip_mr_cache       netstat rt_cache    udp
dev_mcast    ip_mr_vif          packet     snmp       udp6
dev_snmp6    ip_tables_matches psched     snmp6       unix
if_inet6    ip_tables_names    raw        sockstat    wireless
igmp           ip_tables_targets raw6    sockstat6
igmp6       ipv6_route           route    softnet_stat
ip6_flowlabel   mcfilter          rpc        stat 
--------------------------------------------------------------------------------
以下摘要介绍此目录下文件的功能：
arp
转储每个网络接口的arp表中dev包的统计
dev
来自网络设备的统计
dev_mcast
列出二层（数据链路层）多播组
igmp
加入的IGMP多播组
netlink
netlink套接口的信息
netstat
网络流量的多种统计。第一行是信息头，带有每个变量的名称。接下来的一行保存相应变量的值
raw
原始套接口的套接口表
route
静态路由表
rpc
包含RPC信息的目录
rt_cache
路由缓冲
snmp
snmp agent的ip/icmp/tcp/udp协议统计；各行交替给出字段名和值
sockstat
列出使用的tcp/udp/raw/pac/syc_cookies的数量
tcp
TCP连接的套接口
udp
UDP连接的套接口表
unix
UNIX域套接口的套接口表 
--------------------------------------------------------------------------------
示例：[root@localhost /]# cat /proc/net/route
Iface Destination     Gateway       Flags RefCnt   Use     Metric   Mask MTU    Window   IRTT
eth0 0035C2DA        00000000        0001 0    0    0    80FFFFF0
eth0 0000FEA9        00000000        0001 0    0    0    0000FFF0
eth0 00000000        0135C2DA        0003 0    0    0    00000000 
--------------------------------------------------------------------------------
[root@localhost /]# cat /proc/net/tcp
   sl   local_address rem_address st tx_queue rx_queue tr tm->when retrnsmt uid   timeout inode
0: 00000000:8000 00000000:0000 0A 00000000:00000000 00:00000000 00000000 29        0 9525 1 0dde7500 3000 0 0 2 -1
1: 00000000:006F 00000000:0000 0A 00000000:00000000 00:00000000 00000000 0        0 9484 1 0dde79e0 3000 0 0 2 -1
2: 0100007F:0277 00000000:0000 0A 00000000:00000000 00:00000000 00000000 0        0 10049 1 0a8e3a00 3000 0 0 2 -1
3: 0100007F:14D7 00000000:0000 0A 00000000:00000000 00:00000000 00000000 99        0 9847 1 0dde7020 3000 0 0 2 -1
4: 0100007F:0019 00000000:0000 0A 00000000:00000000 00:00000000 00000000 0        0 10286 1 0a8e3520 3000 0 0 2 -1
--------------------------------------------------------------------------------
[root@localhost /]# cat /proc/net/arp
IP address    HW type     Flags    HW address          Mask     Device
218.194.53.1     0x1       0x2       00:0D:BC:78:07:3F     *        eth0 
--------------------------------------------------------------------------------
[root@localhost /]# cat /proc/net/udp
   sl   local_address rem_address st tx_queue rx_queue tr tm->when retrnsmt uid   timeout inode
0: 00000000:8000 00000000:0000 07 00000000:00000000 00:00000000 00000000 29        0 9520 2 0b4ef7c0
105: 00000000:14E9 00000000:0000 07 00000000:00000000 00:00000000 00000000 99        0 10284 2 0b4ef040
111: 00000000:006F 00000000:0000 07 00000000:00000000 00:00000000 00000000 0        0 9483 2 0b4efcc0
116: 00000000:02F4 00000000:0000 07 00000000:00000000 00:00000000 00000000 0        0 9511 2 0b4efa40
119: 00000000:0277 00000000:0000 07 00000000:00000000 00:00000000 00000000 0        0 10050 2 0b4ef2c0
--------------------------------------------------------------------------------
/proc/scsi子目录
此目录下包含一个列出了所有检测到的SCSI设备的文件，并且为每种控制器驱动程序提供一个目录，在这个目录下又为已安装的此种控制器的每个实例提供一个子目录。
示例：
由于本人的机器没有SCSI设备，顾暂时无法提供示例。 
--------------------------------------------------------------------------------
/proc/sys子目录
在此目录下有许多子目录。此目录中的许多项都可以用来调整系统的性能。这个目录包含信息太多，无法介绍全部。只在示例中展示目录下的一些文件。
示例：[root@localhost /]# ls /proc/sys
debug   dev   fs   kernel   net   proc   sunrpc   vm 
--------------------------------------------------------------------------------
[root@localhost ~]# ls /proc/sys/fs
aio-max-nr dentry-state    file-nr    lease-break-time   overflowgid
aio-nr    dir-notify-enable   inode-nr     leases-enable     overflowuid
binfmt_misc   file-max           inode-state   mqueue          quota 
--------------------------------------------------------------------------------
[root@localhost ~]# ls /proc/sys/kernel
acct                hotplug    panic                sem
cad_pid             modprobe     panic_on_oops           shmall
cap-bound              msgmax    pid_max                 shmmax
core_pattern           msgmnb    print-fatal-signals     shmmni
core_uses_pid       msgmni    printk                sysrq
ctrl-alt-del           ngroups_max   printk_ratelimit        tainted
domainname          osrelease printk_ratelimit_burst   threads-max
exec-shield          ostype    pty                   vdso
exec-shield-randomize   overflowgid   random                version
hostname             overflowuid   real-root-dev 
--------------------------------------------------------------------------------
[root@localhost ~]# ls /proc/sys/net
core   ethernet   ipv4   ipv6   unix 
--------------------------------------------------------------------------------
[root@localhost sys]# ls /proc/sys/vm
block_dump                 laptop_mode          nr_pdflush_threads
dirty_background_ratio     legacy_va_layout    overcommit_memory
dirty_expire_centisecs     lower_zone_protection   overcommit_ratio
dirty_ratio             max_map_count       page-cluster
dirty_writeback_centisecs   min_free_kbytes        swappiness
hugetlb_shm_group       nr_hugepages           vfs_cache_pressure 
--------------------------------------------------------------------------------
[root@localhost sys]# ls /proc/sys/net/ipv4
conf                            tcp_fack
icmp_echo_ignore_all             tcp_fin_timeout
icmp_echo_ignore_broadcasts        tcp_frto
icmp_ignore_bogus_error_responses   tcp_keepalive_intvl
icmp_ratelimit                   tcp_keepalive_probes
icmp_ratemask                   tcp_keepalive_time
igmp_max_memberships             tcp_low_latency
igmp_max_msf                       tcp_max_orphans
inet_peer_gc_maxtime             tcp_max_syn_backlog
inet_peer_gc_mintime             tcp_max_tw_buckets
inet_peer_maxttl                tcp_mem
inet_peer_minttl                tcp_moderate_rcvbuf
inet_peer_threshold             tcp_no_metrics_save
ip_autoconfig                   tcp_orphan_retries
ip_conntrack_max                tcp_reordering
ip_default_ttl                   tcp_retrans_collapse
ip_dynaddr                      tcp_retries1
ip_forward                      tcp_retries2
ipfrag_high_thresh                 tcp_rfc1337
ipfrag_low_thresh                tcp_rmem
ipfrag_secret_interval          tcp_sack
ipfrag_time                      tcp_stdurg
ip_local_port_range             tcp_synack_retries
ip_nonlocal_bind                tcp_syncookies
ip_no_pmtu_disc                    tcp_syn_retries
neigh                            tcp_timestamps
netfilter                          tcp_tso_win_divisor
route                            tcp_tw_recycle
tcp_abort_on_overflow              tcp_tw_reuse
tcp_adv_win_scale                tcp_vegas_alpha
tcp_app_win                      tcp_vegas_beta
tcp_bic                         tcp_vegas_cong_avoid
tcp_bic_fast_convergence           tcp_vegas_gamma
tcp_bic_low_window                 tcp_westwood
tcp_dsack                          tcp_window_scaling
tcp_ecn                         tcp_wmem 
--------------------------------------------------------------------------------
[root@localhost sys]# cat /proc/sys/kernel/shmall
2097152 
--------------------------------------------------------------------------------
[root@localhost sys]# cat /proc/sys/kernel/osrelease
2.6.9-1.667
--------------------------------------------------------------------------------
总结：/proc文件系统包含了大量的有关当前系统状态的信息。proc的手册页中也有对这些文件的解释文档。把文件和分析这些文件的工具产生的输出进行比较能够更加清晰地了解这些文件。
