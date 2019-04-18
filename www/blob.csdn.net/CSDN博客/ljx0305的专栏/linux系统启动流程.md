# linux系统启动流程 - ljx0305的专栏 - CSDN博客
2012年10月04日 11:30:33[ljx0305](https://me.csdn.net/ljx0305)阅读数：484标签：[linux																[脚本																[initialization																[linux内核																[磁盘																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/ljx0305/article/category/364133)
系统的启动时指从计算机加电到显示用户登陆提示的整个过程,我们在这里对整个流程坐一个讨论...
对系统的流程先有一个大概的了解:开电源==>BIOS自检==>启动设备(磁盘orU盘or光驱)==>引导程序(lilo或grub)==>内核==> init程序(<==>rc.sysinit和rc)==>mingetty==>终端登录==>shell...
现在分为四个阶段来讲述:
第一阶段:BIOS启动引导阶段
               实现硬件的初始化以及查找启动介质;
               从MBR中装载启动引导管理器(GRUB)并运行该启动引导管理器.
第二阶段:GRUB启动引导管理器
               装载stage1;
               装载stage2;
               读取/boot/grub.conf文件并显示启动菜单;
               装载所选的kernel和initrd文件到内存中.
第三阶段:内核启动阶段
               运行内核启动参数;
               解压并挂载initrd文件系统,装载所需的驱动;
               挂载根文件系统.
第四阶段:init初始化阶段
               启动/sbin/init进程;
               运行rc.sysinit脚本,设置系统环境,启动swap分区,检查和挂载文件系统;
               读取/etc/inittab文件,运行在/etc/rc.d/rc<n>.d中定义的不同的运行级别服务初始化运行脚本;
               打开字符终端1--6号控制台or图形管理的7号控制台.
第一阶段:系统开机上电后,主板BIOS运行POST(Power on self test)代码,检测系统外围关键设备(如:CPU,内存,显卡,I/O,键盘,鼠标等),硬件配置信息及一些用户配置参数存储在主板的CMOS( Complementary Metal Oxide Semiconductor)上(一般64字节),实际上就是主板上一块可读写的RAM芯片,由主板上的电池供电,系统掉电后,信息不会丢失.
执行POST代码对系统外围关键设备检测通过后,系统启动自举程序, 根据我们在BIOS中设置的启动顺序搜索启动驱动器(比如的硬盘,光驱,网络服务器等).选择合适的启动器,通常是硬盘设备,BIOS会读取硬盘的第一个扇区(MBR,512字节),并执行其中的代码.实际上这里BIOS并不关心启动设备第一个扇区中是什么内容,它只是负责读取该扇区内容,并执行,BIOS的任务就完成了.此后将系统启动的控制权移交到MBR部分的代码.
第二阶段:BIOS通过下面两种方法之一来传递引导记录:
第一,将控制权传递给initial program loader(IPL)该程序安装在磁盘主引导记录(MBR)中
第二,将控制权传递给initial program loader(IPL),该程序安装在磁盘分区的启动引导扇区中
无论上面的哪种情况中,IPL都是MBR的一部分并应该存储于一个不大于446字节的磁盘空间中,因为MBR是一个不大于512字节的空间.因此IPL仅仅是GRUB的第一个部分(stage1),他的作用就是定位和装载GRUB的第二个部分(stage2);stage2对启动系统起关键作用,该部分提供了GRUB启动菜单和交互式的GRUB的shell.启动菜单在启动时候通过/boot/grub/grub.conf文件所定义的内容生成.在启动菜单中选择了kernel之后,GRUB会负责解压和装载kernel image并且将initrd装载到内存中.最后GRUB初始化kernel启动代码.完成之后后续的引导权被移交给kernel.
假设bootloader为grub,其引导系统的过程如下:
grub 分为stage1(stage1.5)和stage2,stage1可以看成是IPL,而stage2则是起到了grub的关键作用,包括对特定文件系统的支持(ext2,ext3等),grub自己的shell,以及一些内部程序(如:kernel,initrd,root)等.
stage1:MBR(0头0道1扇区),前446字节是存放stage1,后面跟的是硬盘分区表信息,BIOS将stage1载入内存的0x7c00处并跳转执行;
stage1(/stage1/start.S)的任务很单纯,就是将硬盘0头0道2扇区的读入内存.0头0道2扇区的内容是源代码中的/stage2/start.S,编译后512字节,是stage2或stage1.5的入口.
是关于grub常用的几个指令对应的函数：
grub>root (hd0,0)                                                                   --root指令为grub指定了一个根分区
grub>kernel /xen.gz-2.6.18-37.el5                                           --kernel指令将操作系统内核载入内存
grub>module /vmlinuz-2.6.18-37.el5xen ro root=/dev/sda2         --module指令加载指定的模块
grub>module /initrd-2.6.18-37.el5xen.img                             --指定initrd文件
grub>boot                                                                             --boot 指令调用相应的启动函数启动OS内核
第三阶段:grub>boot指令后,系统启动的控制权移交给kernel.Kernel会立即初始化系统中各设备并做相关配置工作,其中包括CPU,I/O,存储设备等.关于设备驱动加载,有两部分:一部分设备驱动编入Linux Kernel中,Kernel会调用这部分驱动初始化相关设备,同时将日志输出到kernel message buffer,系统启动后dmesg可以查看到这部分输出信息.另外有一部分设备驱动并没有编入Kernel,而是作为模块形式放在 initrd（ramdisk）中.
initrd是一种基于内存的文件系统,启动过程中,系统在访问真正的根文件系统/时,会先访问initrd文件系统.将initrd中的内容打开来看, 会发现有bin,devetc,lib,procsys,sysroot,init等文件（包含目录）.其中包含了一些设备的驱动模块,比如scsi ata等设备驱动模块,同时还有几个基本的可执行程序 insmod, modprobe, lvm,nash.主要目的是加载一些存储介质的驱动模块,如上面所说的scsi ideusb等设备驱动模块,初始化LVM,把/根文件系统以只读方式挂载.
initrd中的内容释放到rootfs中后,Kernel会执行其中的init文件,这里的init是一个脚本,由nash解释器执行.这个时候内核的控制权移交给init文件处理,我们查看init文件的内容,主要也是加载各种存储介质相关的设备驱动.
驱动加载后,会创建一个根设备,然后将根文件系统/以只读的方式挂载.这步结束后释放未使用内存并执行switchroot,转换到真正的根/上面去,同时运行/sbin/init程序.开启系统的1号进程,此后系统启动的控制权移交给 init 进程.关于switchroot是在nash中定义的程序.
实际上Linux Kernel仅是包含了基本的硬件驱动,在系统安装过程中会检测系统硬件信息,根据安装信息和系统硬件信息将一部分设备驱动写入 initrd .这样在以后启动系统时,一部分设备驱动就放在initrd中来加载.
第四阶段:init进程起来后,系统启动的控制权交给init进程./sbin/init是所有进程的父进程,当init起来后会先读取inittab文件,进行以下的工作:
a) 执行系统初始化脚本(/etc/rc.d/rc.sysinit),对系统进行基本的配置,以读写方式挂载根文件系统及其它文件系统,到此系统基本算运行起来了,后面需要进行运行级别的确定及相应服务的启动;
b) 确定启动后进入的运行级别;
c) 执行/etc/rc.d/rc,该文件定义了服务启动的顺序是先K后S,而具体的每个运行级别的服务状态是放在/etc/rc.d/rcn.d（n=0~6）目录下,所有的文件均链接至/etc/init.d下的相应文件.
d) 有关key sequence的设置
e) 有关UPS的脚本定义
 f)  启动虚拟终端/sbin/mingetty 
g) 在运行级别5上运行X
这时呈现给用户的就是最终的登录界面.
/etc/rc.d/rc.sysint -- System Initialization Tasks 它的主要工作有:
配置selinux,
系统时钟,
内核参数（/etc/sysctl.conf）,
hostname,
启用swap分区, 
根文件系统的检查和二次挂载（读写）,
激活RAID和LVM设备,
启用磁盘quota,
检查并挂载其它文件系统,
等等。
比较简单的说法:
启动第一步:加载BIOS
当你打开计算机电源,计算机会首先加载BIOS信息,BIOS信息是如此的重要,以至于计算机必须在最开始就找到它.这是因为BIOS中包含了CPU的相关信息,设备启动顺序信息,硬盘信息,内存信息,时钟信息,PnP特性等等.在此之后,计算机心里就有谱了,知道应该去读取哪个硬件设备了,
启动第二步:读取MBR
硬盘上第0磁道第一个扇区被称为MBR,也就是Master Boot Record,即主引导记录,它的大小是512字节,别看地方不大,可里面却存放了预启动信息,分区表信息.
系统找到BIOS所指定的硬盘的MBR(stage1)后,就会将其复制到0×7c00地址所在的物理内存中,其实被复制到物理内存的内容就是Boot Loader(stage2),而具体到你的电脑,那就是lilo或者grub了.
启动第三步:Bootloader
Boot Loader 就是在操作系统内核运行之前运行的一段小程序.通过这段小程序,我们可以初始化硬件设备,建立内存空间的映射图,从而将系统的软硬件环境带到一个合适的状态,以便为最终调用操作系统内核做好一切准备.
Boot Loader有若干种,其中Grub,Lilo和spfdisk是常见的Loader.
我们以Grub为例来讲解吧,毕竟用lilo和spfdisk的人并不多.
系统读取内存中的grub配置信息（一般为menu.lst或grub.lst）,实际上是/etc/grub/grub.conf文件,并依照此配置信息来启动不同的kernel.
启动第四步:加载内核
根据grub设定的内核映像所在路径,系统读取内存映像,并进行解压缩操作.此时,屏幕一般会输出“Uncompressing Linux”的提示.当解压缩内核完成后,屏幕输出“OK, booting the kernel”.
系统将解压后的内核放置在内存之中,并调用start_kernel()函数来启动一系列的初始化函数并初始化各种设备,完成Linux核心环境的建立.至此,Linux内核已经建立起来了,基于Linux的程序应该可以正常运行了.
启动第五步:**用户层init依据inittab文件来设定运行等级**
内核被加载后,第一个运行的程序便是/sbin/init,该文件会读取/etc/inittab文件,并依据此文件来进行初始化工作.其实 /etc/inittab文件最主要的作用就是设定Linux的运行等级,其设定形式是“：id:5:initdefault:”,这就表明Linux需要运行在等级5上.Linux的运行等级设定如下:
0：关机
1：单用户模式
2：无网络支持的多用户模式
3：有网络支持的多用户模式
4：保留，未使用
5：有网络支持有X-Window支持的多用户模式
6：重新引导系统，即重启
启动第六步:**init进程执行rc.sysinit**
在设定了运行等级后,Linux系统执行的第一个用户层文件就是/etc/rc.d/rc.sysinit脚本程序,它做的工作非常多,包括设定 PATH,设定网络配置（/etc/sysconfig/network）,启动swap分区,设定/proc等等.如果你有兴趣,可以到/etc /rc.d中查看一下rc.sysinit文件.
启动第七步:启动内核模块
具体是依据/etc/modules.conf文件或/etc/modules.d目录下的文件来装载内核模块.
启动第八步:执行不同运行级别的脚本程序
根据运行级别的不同,系统会运行rc0.d到rc6.d中的相应的脚本程序,来完成相应的初始化工作和启动相应的服务.
启动第九步:执行/etc/rc.d/rc.local
rc.local就是在一切初始化工作后,Linux留给用户进行个性化的地方.你可以把你想设置和启动的东西放到这里.
启动第十步:执行/bin/login程序,进入登录状态
此时,系统已经进入到了等待用户输入username和password的时候了,你已经可以用自己的帐号登入系统了.
