# 使用BusyBox制作根文件系统 - DoubleLi - 博客园






1、BusyBox简介

BusyBox 是很多标准 Linux 工具的一个单个可执行实现。BusyBox 包含了一些简单的工具，例如 cat 和 echo，还包含了一些更大、更复杂的工具，例如 grep、find、mount 以及 telnet；有些人将 BusyBox 称为 Linux 工具里的“瑞士军刀”。

BusyBox 揭露了这样一个事实：很多标准 Linux 工具都可以共享很多共同的元素。例如，很多基于文件的工具（比如 grep 和 find）都需要在目录中搜索文件的代码。当这些工具被合并到一个可执行程序中时，它们就可以共享这些相同的元素，这样可以产生更小的可执行程序。实际上，BusyBox 可以将大约 3.5MB 的工具包装成大约 200KB 大小。这就为可引导的磁盘和使用 Linux 的嵌入式设备提供了更多功能。我们可以对 2.4 和 2.6 版本的 Linux 内核使用 BusyBox。

我们平时用的那些Linux命令就好比是分立式的电子元件,而BusyBox就好比是一个集成电路,把常用的工具和命令集成压缩在一个可执行文件里,功能基本不变,而大小却小很多倍,在嵌入式linux应用中,BusyBox有非常广的应用。



2、BusyBox的用法

　　可以这样用BusyBox

　　#busybox ls

　　他的功能就相当运行ls命令

　　最常用的用法是建立指向BusyBox的链接,不同的链接名完成不同的功能.

　　#ln -s busybox ls

　　#ln -s busybox rm

　　#ln -s busybox mkdir

　　然后分别运行这三个链接：

　　#./ls

　　#./rm

　　#./mkdir

　　就可以分别完成了ls、rm和mkdir命令的功能.虽然他们都指向同一个可执行程序BusyBox，但是只要链接名不同,完成的功能就不同, BusyBox就是这么的神奇。



3、配置编译BusyBox

? 下载

首先需要下载BusyBox源代码。可从[http://busybox.net](http://busybox.net/)上直接下载最新版BusyBox。例如当前最新版为1.5.1。



? 解压：

tar xvf busybox-1.5.1.tar.bz2



? 配置

BusyBox的配置程序和Linux内核菜单配置方式简直一模一样，十分方便易用。使用make menuconfig命令就可以进行配置。

![使用Busybox制作根文件系统 - Smart_zhao - 军惠博客](http://www.embeddedlinux.org.cn/emblinuxappdev/emblinuxappdev1_files/image055.jpg)



配置过程主要有几点需要修改的：

l 因为我们要将BusyBox交叉编译成ARM可执行程序放在开发板上执行，所以需要使用交叉编译器arm-linux-gcc来编译BusyBox。所以需要修改BusyBox根目录下的Makefile，找到

ARCH ?= $(SUBARCH)

CROSS-COMPILE ?=

修改成ARM的配置，如下：

ARM ?= arm

CROSS-COMPILE ?= arm-linux-

l 在BusyBox配置界面里修改几个配置：

Busybox Settings --->

Build Options

Build BusyBox as a static binary (no shared libs)

这个选项是一定要选择的，这样才能把BusyBox编译成静态链接的可执行文件，运行时才独立于其他函数库，否则必须要其他库文件才能运行。



Busybox Settings --->

Installation Options

Don’t use /usr

这个选项也是要选上的，否则make install 后BusyBox将安装在原系统的/usr下，这将覆盖系统原有的命令。选择这个选项后，make install后会在BusyBox目录下生成一个叫_install的目录，里面有BusyBox和指向他的链接。



Init Utilities --->

init

这个选项最好选上，这样BusyBox就可以初始化脚本inittab，可以用来初始化Linux系统。



如果要让BusyBox包含一个可以用于解释Linux命令的shell，需要配置BusyBox的Shells选项里的内容：

Shells --->

这里可选的shell有多种，包括ash，hush，lash，msh。最好使用ash，因为它是功能最全也最类似于一般Linux系统中的BASH的。同时注意第一行的：

Choose your default shell (none) --->

这里需要回车进去选择默认的Shell，例如选择了ash后，第一行的内容就会变成：

Choose your default shell (ash) --->

这样BusyBox才会生成sh的链接并且将这个sh指向对应的shell(ash)。



l 其他选项都是一些Linux基本命令选项，可以根据自己的需要选择配置，第一次的话用默认的设置即可。

? 编译

如果配置好了BusyBox，就可以使用make命令编译了。

#make

#make install



默认情况下，make install完成后会在BusyBox目录下创建一个新的本地子目录 _install，其中包含了基本的 Linux 环境。在这个目录中，会有一个链接到 BusyBox 的 linuxrc 程序。这个 linuxrc 程序在构建安装盘或急救盘（允许提前进行模块化的引导）时非常有用。同样在这个目录中，还有一个包含操作系统二进制文件的 /sbin 子目录。还有一个包含用户二进制文件的 /bin 目录。在构建软盘发行版或嵌入式初始 RAM 磁盘时，可以将这个 _install 目录迁移到目标环境的根目录中。



4、制作完整的根文件系统

BusyBox虽然为我们创建了Linux根系统中最基本的shell和一些常用命令，但是一个根文件系统还不只包含这些，还需要其它的一些内容。

? 创建一个比较完整的根文件系统目录结构

本章第一节已经介绍了根文件系统中的一些目录，这些目录是Linux正常运行时所必需的。我们可以在BusyBox的_install基础上创建完整的根文件系统目录，一般步骤如下：

l 在PC上创建一个目标根文件系统的目录，例如/rootfs，将BusyBox里的_install目录里所有内容复制到这个文件夹里：cp –r _install /rootfs

l 在/rootfs下创建目录etc/，dev/，lib/，tmp/，usr/，var/目录，同时var/目录里还需要创建var/run和var/log等目录。

l 生成etc/里的设备文件，例如tty，console，fb（FrameBuffer），mtdblock（Memory Technology Device）等，这些设备文件是Linux很多驱动程序及就用程序正常的工作的基础。这些设备文件都是与相应的硬件相联系的，主要包含几种信息：设备类型，主设备号，次设备号。

其中设备类型主要包括字符设备（Character Device）和块设备（Block Device），字符设备主要字符的输入输出设备如键盘、鼠标等，块设备主要指整块数据的输入输出设备，如FLASH、硬盘等存储设备，一般包含缓冲区机制。

主设备号用来区分不同种类的设备，而次设备号用来区分同一类型的多个设备。对于常用设备，Linux有约定俗成的编号，如硬盘的主设备号是3，而次设备对应到每个具体的设备上，一般在/proc/devices文件里可以找到相关信息。

对于一个已存在的设备文件可以通过ls –l 命令来获取它的设备相关信息。

ls 　–l　 /dev/console

crw-rw---- 1 root root 5,1 Apr 14 23:08 /dev/console

可以看出第一个字母为c，这代表/dev/console是字符设备，若第一个字母为b，则为块设备。而root之后的　5,1就分别为相应设备的主次设备号了。

这里需要强调一点，设备文件类似于配置文件，存储的是一些设备信息，里面不包含特定平台下的指令，所以设备文件本身是平台无关的，也就是说在I386上创建的设备文件可以放在ARM的根文件系统，而可以被Linux正确识别的。



基本了解设备后，还需要如何创建它们，一般情况下可以使用mknod生成相应设备文件。mknod是Linux中用来创建设备文件的命令，格式如下：

mknode [–m MODE] NAME TYPE [MAJOR MINOR]

其中MODE用于指定设备文件的访问权限。NAME为设备文件的文件名，TYPE为相应的设备类型（字符设备c，块设备b等），MAJOR和MINOR分别为主次设备号。

例如要创建刚才的那个console命令可用如下的命令：

mknode –m 660 console c 5 1



设备文件的创建除了使用mknod命令，还可以使用MAKEDEV命令，MAKEDEV可以较方便的创建一系统的设备文件，一般的Linux发行版都有自带，其基本格式如下：

MAKEDEV –d directory -m maxdevices device

其中directory为设备文件的目标存放文件夹，若不指定则为当前系统下的/dev里。maxdevices为最大的设备数，因为MAKEDEV一般会创建一种设备的一系列设备文件，一般从0开始编号，直到maxdevices，所以一般这个需要指定，要不会生成较多的相关设备文件，而一般我们是不需要这么多的。最后一个参数device为对应的设备文件名，包括tty，vt，mem，null，zero，fd，hd，audio，sound等。这些参数的详细内容以及更多的参数选项，可以参考man手册。

可以这样创建硬盘的设备文件hd

MAKEDEV –d /rootfs/dev -m 2 hda

这条命令就会在/rootfs/dev目录中创建hda和hda1两个设备文件，指向第一块硬盘和第一块硬盘的第一个分区。



对于目标根文件系统中的设备文件，一般都应放在etc/目录中，可以用如下几种方法来获取相应的设备文件：

ü 可以手动用mknod命令一个个的创建设备文件；

ü 可以使用MAKEDEV来创建设备文件；

ü 甚至可以直接拷贝PC系统中部分设备文件至目标根文件系统中。



l 构建lib/目录。lib/目录放的是Linux就用程序所需的库文件，其实也是目标平台的指令代码，所以这里的文件与etc/里不一样，必须与相应的硬件平台相对应，例如i386里的库文件放到ARM系统中就不能使用的，这点与Linux的可执行文件一样。在PC机上使用ls /lib命令就可以看到很多.so结尾的库文件，这些.so文件就是Linux的动态链接库（类似于Windows下的DLL文件）。要注意的是.so文件名后缀还可能加上一些版本号标志例如.so.1，.so.1.2等都是动态链接库。

ü 目标根文件系统中的库文件从哪里来？

一般这些库都是事先编译好的，而且跟编译器相关的(glibc等)，例如我们使用arm-linux-gcc进行编译则需要相应版本的一些库文件，这些库文件可以从编译器所在的目录里直接拷贝。对于ELDK开发包，可以从ELDK目录下的arm/lib/目录里复制相应文件。

ü 目标根文件系统需要哪些基本库文件？

库文件实际上是由其它可执行文件来调用的，所以库文件的取舍是由根文件系统中所包含的可执行文件来决定的。但是要运行可执行文件，一般有几个是系统必须的。它们是ld(ld-linux)，libc，几乎所有的可执行文件都需要调用到这两个库文件。

ld(ld-linux)：ld-linux.so 实际上就是一个可执行程序。这是负责执行动态装载的代码。它从可执行程序读取头信息(ELF格式的)，然后通过这些信息判断必要的库和需要装载的库。之后，执行动态链接，修改可执行程序和装载的库中的所有地址指针，使程序能够运行。一般的文件名可能为ld-2.3.2.so，ld-linux.so.2，这点与编译器和系统版本有关。

libc：libc.so.6 是以 ld-linux.so.2 为基础架构而完成的动态链接库，它几乎负责了所有常用的标准 C 函数库，例如 Linux 下写的 Socket 程序，其中的connect()、bind()、send() .....之类的函数，都是由 libc.so.6 所提供的。

所以一个最基本的lib/目录应该至少包含这ld-linux.so.2和libc.so.6这两个文件。

ü 应用程序需要哪些库文件？

前面已经说过动态链接库是由应用程序(可执行文件)调用的，那对于一个特定的可执行文件是如何判断它需要哪些库文件的？一般可以编译器的ldd命令来查看，例如arm-linux-gcc包含了arm-linux-ldd命令用来查看ARM可执行文件调用的动态链接库。arm-linux-ldd的功能就是列出可执行文件及动态链接库运行时需要的库文件，例如，对于刚才所指的libc.so.6可以查找出其需要的动态链接库。这里我们假设为ELDK里的libc。

# file /eldk/arm/libc.so.6

libc.so.6：symbolic link to `libc-2.3.5.so`

通过file命令看出这里的libc.so.6实际上是个符号连接，链接到libc-2.3.5.so，所以我们继续追查libc-2.3.5.so：

#file libc-2.3.5.so

libc-2.3.5.so: ELF 32-bit LSB shared object, ARM, version 1(ARM), stripped

从这里可以明显的看出这个是ARM的Shared Object，也就是ARM格式的动态链接库。到这里可以判断出libc.so.6是ARM指令的。下来看libc.so.6到底需要哪些库文件：

#arm-linux-ldd libc.so.6

ld-linux.so.2 => not found

可以看出libc.so.6库文件需要ld-linux.so.2这个动态链接库(not found 说明在当前系统中未找到相应的库，因为系统是i386而需要的是ARM格式，所以找不到)。

这样通过arm-linux-ldd命令就可以确定各个程序所需的动态链接库，然后根据需要放到lib/目录里，就组成目标根文件系统的动态链接库集合了。

l 若选择了BusyBox的init模块，则需要配置BusyBox的初始化文件。因为Linux系统加载根文件系统后需要执行一些配置以初始化整个Linux的工作环境及init程序和Shell等。这个文件就是etc/inittab。关于此文件的详细内容可以查看man inittab。但是BusyBox的inittab格式与一般Linux下的inittab的格式是不同，所以直接拷贝PC机上/etc/inittab文件到BusyBox制作的根文件系统中是不能用的，那怎么办呢？

BusyBox自带了符合它的格式的inittab样本文件，放在examples目录下，主要内容包括：

::sysinit:/etc/init.d/rcS



::askfirst:-/bin/sh



tty2::askfirst:-bin/sh

tty3::askfirst:-bin/sh

tty4::askfirst:-bin/sh



#Stuff to do when restarting the init process

::restart:/sbin/reboot



#Stuff to do before rebooting

::ctrlaltdel:/sbin/reboot

::shutdown:/bin/umount -a -r

::shutdown:/sbin/swapoff –a

BusyBox的inittab的每一行格式如下：

<id>:<runlevels>:<action>:<process>

总共包含四项，每项间以”：”隔开。

第一二项<id>和<runlevels>在BusyBox都是忽略掉的，所以可以看到BusyBox提供的inittab样本文件的所有项目都是以两个冒号”::”开头的。

第三项<action>为动作描述，可选项为sysinit, respawn, askfirst, wait, once, restart, ctrlaltdel 和 shutdown。其中大部分动作可以通过动作名直接理解它的作用。其中askfirst会在登录shell前提示用户，而respawn则不会提示。

第四项<process>指定了<action>动作应执行的脚本文件。

知道了格式，下面简单的分析一下inittab样本文件：

第一行::sysinit:/etc/init/rcS实际上指定了系统初始化(sysinit)时脚本为/etc/init/rcS，这个可以根据自己的需要更改的。

但是对于不同的终端设备的不同配置区别在于开头的标志，例如对于tty2终端，则有对应的操作 tty2::askfirst:-bin/sh。此行的意思指对于tty2使用shell为/bin/sh，同时对askfirst(有提示信息再要求登陆)。 若对于某个特定的终端设备可以直接将前面的设备标志去掉，例如ttyS0, ttyS1等。



第二行::askfirst:-/bin/sh指定了系统第一个终端在加载shell为/bin/sh，而且在进入shell前会提示用户。其它行请读者自行分析。

l 在etc/目录里除了inittab文件外，还需要其它的一些基本的文件，例如fstab、passwd、group、inputrc等，由于篇幅所限，不可能一一详解，读者可以参考其它书籍或者man手册，对于一些文件读者也可借用别的嵌入式根文件系统里的内容，然后在此基础上进行修改以符合自己的系统。这里简单介绍etc/里的几个文件：

ü fstab：这个文件描述系统中各种文件系统的信息。在这个文件中，每个文件系统用一行来描述，在每一行中，用空格或TAB符号来分隔各个字段，文件中以*开头的行是注释信息。一般内容可能如下：

/dev/mtdblock2 / jffs2 defaults 0 0

none /tmp ramfs defaults 0 0

none /proc proc defaults 0 0

第一列(字段)：设备名或者设备卷标名（一般为/dev里的对应的设备文件）

第二列(字段)：设备挂载目录 （例如上面的“/”或者“/tmp”）

第三列(字段)：设备文件系统 (例如上面的“ext3”或者“vfat”）

第四列(字段)：挂载参数 (具体可以查看帮助man mount）

对于已经挂载好的设备，例如上面的/dev/sda2，现在要改变挂载参数，这时可以不用卸载该设备，而可以使用下面的命令（没有挂载的设备，remount 这个参数无效）

#mount /mnt -o remount,ro （改defaults为ro）

关于其它参数请参考man手册。

第五列(字段)：指明是否要备份。（0为不备份，1为要备份）

第六列(字段)：指明自检顺序。 （0为不自检，1或者2为要自检，如果是根分区要设为1，其他分区只能是2）

ü passwd和group保存着Linux系统的用户组和用户名等，与硬件平台无关，为方便起见，可以从现有的Linux系统中拷贝过去即可。

etc/目录里的配置文件较多，不可能一一解释，请读者在创建时多参考已有的系统。

5、总结

上面已经介绍一个根文件系统的创建过程，如果完整的按照上面的步骤做下来，应该就会在/rootfs下得到了一个相对完整的根文件系统，这个根文件系统主要BusyBox的bin/、sbin/目录，etc/系统配置文件目录以及lib/动态链接库所在目录等，这样一个Linux应用程序可执行的最小环境基本已经搭成了。之后可以在这个根文件系统中添加所需的应用程序等等。

但是这个根文件系统又是怎么放到目标开发板里的呢？

通常的做法是将整个根文件系统打包成某种文件系统格式的映像，然后下载到目标开发板的存储设备里(如FLASH等)。

用什么程序可以打包？支持几种格式呢？

通常使用mkfs系统命令。mkfs命令可以生成指定文件系统类型的映像文件。对于不同的文件系统类型需要不同的mkfs命令，例如EXT2，EXT3类型的文件可以使用mkfs.ext2和mkfs.ext3等。而通常嵌入式开发板使用FLASH作为存储设备，所以对应的文件系统类型一般为JFFS2，所以使用命令mkfs.jffs2命令，这个命令一般在开发板提供的工具有，也可以从网上搜索下载，这个命令一般是运行在PC系统上的，所以一般为I386可执行文件。

mkfs.jffs2的基本命令格式如下：

mkfs.jffs2 -r DIR -o FILE -e SIZE --pad=PADSIZE



其中DIR为要打包的文件夹，FILE为输出的文件路径，SIZE为每次擦除的块大小(默认为64KB)。PADSIZE为填充大小，这个参数强制使目标文件大小至少为PADSIZE字节，若实际数据没有这么大，则使用0xFF填充，这个参数很重要，在将映像写入目标开发板时一般应与实际根文件系统大小相符(类似于总磁盘容量)，这样具有初始化的作用，若不相符合，在挂载这个JFFS2根文件系统时可能会出现一些问题。



下面给个简单的例子，这个例子将/rootfs的这个根文件系统打包成文件rootfs.img，且总大小为1M(0x100000字节)。

mkfs.jffs2 -r /rootfs -o rootfs.img -e 0x40000 --pad=0x100000



做好映像文件后就可以将这个映像文件写入目标板中，通常使用U-BOOT等BootLoader通过网卡下载到开发板内存中，然后再写入开发板的FLASH里。在U-BOOT里若网卡驱动可用，通常用tftp下载，再使用相关的FLASH操作命令写数据。









