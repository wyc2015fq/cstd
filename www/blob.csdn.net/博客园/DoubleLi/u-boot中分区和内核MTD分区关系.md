# u-boot中分区和内核MTD分区关系 - DoubleLi - 博客园






**一、u-boot中环境变量与uImage中MTD的分区关系**

分区只是内核的概念，就是说A～B地址放内核，C～D地址放文件系统，(也就是规定哪个地址区间放内核或者文件系统)等等。

一般我们只需要分3-4个区，第一个为boot区，一个为boot参数区(传递给内核的参数),一个为内核区，一个为文件系统区。（但是有的内核就会有很多分区，比如内核参数会有两个，还有会Logo的地址）

而对于bootloader中只要能将内核下载到A~B区的A地址开始处就可以，C~D区的C起始地址下载文件系统…….这些起始地址在MTD的分区信息中能找到。所以bootloader对分区的概念不重要，只要它能把内核烧到A位置，把文件系统烧到C位置即可。

所以，在bootloader对Flash进行操作时，哪块区域放什么是以内核为主（内核中MTD的分区信息可以从内核的代码中看到）。传递给u-boot的参数只要和内核中MTD分区信息一致即可。

**而为了方便操作，bootloader类似也引入分区的概念**。例如，可以使用“nandwrite 0x3000000 kernel 200000”命令将uImage烧到kernel分区，而不必写那么长：nand write 3000000 A 200000,也就是用分区名来代替具体的地址。

这要对bootloader对内核重新分区：这需要重新设置一下bootloader环境参数，就可以同步更新内核分区信息

如：

setenv bootargs 'noinitrd console=ttySAC0root=/dev/mtdblock3 rootfstype=jffs2 mtdparts=nand_flash:128k(u-boot)ro,64k(u-bootenvs),3m(kernel),30m(root.jffs2),30m(root.yaffs)'

**解析**：在这里的挂载文件系统的地方mtdblock3，可以从mtdparts中看出来，第一个文件系统（jffs2格式）在第四个分区，所以使用mtdblock3,关于分区和文件系统的挂载在下面有解释。

在设置了mtdparts变量之后,就可以在nand read/write/erase命令中直接使用分区的名字而不必指定分区的偏移位置.**而这需要内核MTD最好没有规划分区。**

**如果你是通过uboot的内核命令行给MTD层传递MTD分区信息，这种情况下,内核读取到的分区信息始终和u-boot中的保持一致(推荐的做法)**

**如果你是把分区信息写在内核源代码MTD里定义好的方法,那最好保证它和u-boot中的保持一致,即同步修改uboot及内核的相关部分。**

**解析**：从分析的内容可以看出来，首先使用bootargs是可以重新设置内核分区的，使用的mtdparts，也就是说，如果内核中没有指定好mtd分区信息的话，使用uboot给与分区是很好的办法，如果内核中指定好了分区的信息，最好保证uboot中的分区和内核中的分区一直，如果不一致的话，自我感觉是使用uboot的分区信息，或者是uimage启动不成功。









Uboot中分区和内核MTD分区之间的关系理解:



首先觉得这两者是有关系的，但是关于在NAND分区过程中，是哪个依附于哪个，我觉得是uboot依附MTD，在内核flash device驱动中，如果有声明NAND分区信息的话，在uboot中可以再进行mtdparts分区，但是最终的结果是依照MTD进行挂载文件系统的，例如：

如果内核分区信息如下：

staticstruct mtd_partition smdk_default_nand_part[] = {

     [0] = {

          .name     = "uboot",

          .offset = 0x00000000,

          .size     = 0x00040000,

     },

     [1] = {

          .name     = "kernel",

          .offset = 0x00200000,

          .size     = 0x00300000,

     },

     [2] = {

          .name     = "yaffs2",

          .offset = 0x00500000,

          .size     = MTDPART_SIZ_FULL

     }

};

从中可以发现，uboot和kernel是存在间隙的，但是不管怎么说，在NAND的最低端可定是uboot的信息，那么这个开发板的uboot的分区信息如下：

**bootargs=noinitrdroot=/dev/mtdblock2  init=/linuxrc console=ttySAC0**

**mtdparts=mtdparts=nandflash0:256k@0(bios),128k(params),128k(toc),512k(eboot),1024k(logo),3m(kernel),-(root)**

从上面便可以看出来，挂载文件系统的块并不是按照mtdparts中顺序数出来的文件系统块，因为在内核代码中，文件系统是挂载在mtdblock2上，但是在uboot分区中不是这个样子，如果按照uboot的话应该是mtdblock5,但是内核中uboot和kernel之间是有间隙的，这个间隙正好别kernel之前的东西填充结束，总共2m,从内核信息中的mtd分区信息可知，在kernel声明中，偏移量offset=0x200000,也就是2m，但是uboot的大小是size=0x40000,也就是256K,

这两者中间是有间隙的，间隙的部分正好被填充满，但是挂载文件系统依旧是依照内核信息而不是依照uboot中的分区。

         但是也有人说，修改nand分区，直接修改uboot中的mtdparts就可以，也就是说，



**1.如何对nand 分区。修改mtdparts环境变量就可以了么？**

对于目前的U-boot而言,是的.而且, 设置了mtdparts变量之后,你可以在nand read/write/erase命令中直接使用分区的名字而不必指定分区的偏移位置.

**set bootargs noinitrd console=ttySAC0 root=/dev/mtdblock3 rootfstype=jffs2  mtdparts=nand_flash:128k(u-boot)ro,64k(u-boot envs),3m(kernel),30m(root.jffs2),30m(root.yaffs)**

**2 内核通过bootargs找到文件系统，bootargs中的mtdblockx即代表分区，block1，2，3代表哪个分区是如何确定的。**

事实上,bootargs中的"root=/dev/mtdblockx"只是告诉内核,root fs从第x个(x=0,1,2...)MTD分 区挂载,mtdblock0对应第一个分区,mtdblock1对应第二个分区,以此类推.至于这个分区对应MTD device(NAND Flash)的哪一段范围,取决于内核读到的MTD分区信息,这个分区信息可以通过:

1) 写死在MTD层的NAND Controller驱动或者内核其他部分代码里

2) 通过U-boot传递给内核的命令行中的mtdparts=...部分解析得出,解析的规则同u-boot中mtdparts变量的赋值规则

3) 其他可以让内核知道分区信息的任何办法

**3 在u－boot中给nand分区后是否要对应修改kernel的代码？**

如果你用的是通过内核命令行给MTD层传递u-boot中的MTD分区信息,那是不需要的,在这种情况下,内核读取到的分区信息始终和u-boot中的保持一致(推荐的做法)

如果你用的是把分区信息写死在内核源代码里的方法,那最好保证它和u-boot中的保持一致,即同步修改内核的相关部分代码

从上面这几个情况看出来，如果在uboot中进行nand分区，那么尽量保证和内核一致，如果内核中没有声明分区信息的话，在uboot中的分区就可以当做分区信息使用。从第二个问题的回答可以看出来，那么挂载文件系统的部分还是依靠内核的mtd分区的。

nand write0x3000000 kernel 200000 这条命令是说从内存中往flash中写内核，从内存地址为0x30000000的地方开始写，往flash的偏移地址为200000的地方写kernel这么多字节，也可以这么理解：从内存位置为0x30000000的地方读取kernel这么大的字节全部写到flash偏移地址为200000的地方

**nand read.jffs2 0x30007FC0 kernel;从nand读出内核：从哪里读？   从kernel分区放到哪里去？-0x30007FC0**

**二、常用的分区方法**



内核通过bootargs找到文件系统，bootargs中的mtdblockx即代表分区，block1，2，3代表哪个分区。

事实上,bootargs中的"root=/dev/mtdblockx"只是告诉内核,root fs从第x个(x=0,1,2...)MTD分区挂载,mtdblock0对应第一个分区,mtdblock1对应第二个分区,以此类推.


3：分区方法

1) MTD层的分区

2) 通过U-boot传递给内核的命令行中的mtdparts=...

3) 其他可以让内核知道分区信息的任何办法，（内核默认的命令参数）

下面说到mtdparts，及它的用法：

mtdparts

mtdparts=fc000000.nor_flash:1920k(linux),128k(fdt),20M(ramdisk),4M(jffs2),38272k(user),256k(env),384k(uboot)

要想这个参数起作用，内核中的mtd驱动必须要支持，即内核配置时需要选上Device Drivers  ---> Memory Technology Device (MTD) support  ---> Command line partition table parsing

mtdparts的格式如下：

mtdparts=<mtddef>[;<mtddef]

<mtddef>  := <mtd-id>:<partdef>[,<partdef>]

 <partdef> := <size>[@offset][<name>][ro]

 <mtd-id>  := unique id used in mapping driver/device

<size>    := standard linux memsize OR "-" to denote all remaining space

<name>    := (NAME)

因此你在使用的时候需要按照下面的格式来设置：

mtdparts=mtd-id:<size1>@<offset1>(<name1>),<size2>@<offset2>(<name2>)

这里面有几个必须要注意的：

a.  mtd-id 必须要跟你当前平台的flash的mtd-id一致，不然整个mtdparts会失效 怎样获取到当前平台的flash的mtd-id？

在bootargs参数列表中可以指定当前flash的mtd-id，如指定 mtdids:nand0=gen_nand.1，前面的nand0则表示第一个flash

b.  size在设置的时候可以为实际的size(xxM,xxk,xx)，也可以为'-'这表示剩余的所有空间。

相关信息可以查看drivers/mtd/cmdlinepart.c中的注释找到相关描述。

U-boot的环境变量值得注意的有两个： bootcmd 和bootargs。

引用：
u       bootcmd

    前面有说过bootcmd是自动启动时默认执行的一些命令，因此你可以在当前环境中定义各种不同配置，不同环境的参数设置，然后设置bootcmd为你经常使用的那种参数。

u       bootargs

    bootargs是环境变量中的重中之重，甚至可以说整个环境变量都是围绕着bootargs来设置的。bootargs的种类非常非常的多，我们平常只是使用了几种而已，感兴趣的可以看看这篇文章说的很全：[http://www.linuxidc.com/Linux/2011-03/33599p4.htm](http://www.linuxidc.com/Linux/2011-03/33599p4.htm)。bootargs非常的灵活，内核和文件系统的不同搭配就会有不同的设置方法，甚至你也可以不设置bootargs,而直接将其写到内核中去（在配置内核的选项中可以进行这样的设置），正是这些原因导致了bootargs使用上的困难。

    下面介绍一下bootargs常用参数，bootargs的种类非常的多，而且随着kernel的发展会出现一些新的参数，使得设置会更加灵活多样。

A. root

用来指定rootfs的位置， 常见的情况有:

    root=/dev/ram rw  

    root=/dev/ram0 rw

  请注意上面的这两种设置情况是通用的，我做过测试甚至root=/dev/ram1 rw和root=/dev/ram2 rw也是可以的，网上有人说在某些情况下是不通用的，即必须设置成ram或者ram0，但是目前还没有遇到，还需要进一步确认，遇到不行的时候可以逐一尝试。

    root=/dev/mtdx rw

    root=/dev/mtdblockx rw

    root=/dev/mtdblock/x rw

    root=31:0x

上面的这几个在一定情况下是通用的，当然这要看你当前的系统是否支持，不过mtd是字符设备，而mtdblock是块设备，有时候你的挨个的试到底当前的系统支持上面那种情况下，不过root=/dev/mtdblockx rw比较通用。此外，如果直接指定设备名可以的话，那么使用此设备的设备号也是可以的。

    root=/dev/nfs

在文件系统为基于nfs的文件系统的时候使用。当然指定root=/dev/nfs之后，还需要指定nfsroot=serverip:nfs_dir，即指明文件系统存在那个主机的那个目录下面。

B. rootfstype

    这个选项需要跟root一起配合使用，一般如果根文件系统是ext2的话，有没有这个选项是无所谓的，但是如果是jffs2,squashfs等文件系统的话，就需要rootfstype指明文件系统的类型，不然会无法挂载根分区.

C. console


console=tty<n>  使用虚拟串口终端设备 <n>.

console=ttyS<n>[,options] 使用特定的串口<n>，options可以是这样的形式bbbbpnx，这里bbbb是指串口的波特率，p是奇偶校验位，n是指的bits。

console=ttySAC<n>[,options] 同上面。


看你当前的环境，有时用ttyS<n>，有时用ttySAC<n>，网上有人说，这是跟内核的版本有关，2.4用ttyS<n>，2.6用ttySAC<n>，但实际情况是官方文档中也是使用ttyS<n>，所以应该是跟内核版本没有关联的。可以查看Documentation/serial-console.txt找到相关描述。


D. mem

mem=xxM 指定内存的大小，不是必须的

E. ramdisk_size

ramdisk=xxxxx           不推荐  

ramdisk_size=xxxxx   推荐

上面这两个都可以告诉ramdisk 驱动，创建的ramdisk的size，默认情况下是4m(s390默认8M)，你可以查看Documentation/ramdisk.txt找到相关的描述，不过ramdisk=xxxxx在新版的内核都已经没有提了，不推荐使用。

F. initrd, noinitrd

当你没有使用ramdisk启动系统的时候，你需要使用noinitrd这个参数，但是如果使用了的话，就需要指定initrd=r_addr,size, r_addr表示initrd在内存中的位置，size表示initrd的大小。

G. init

init指定的是内核启起来后，进入系统中运行的第一个脚本，一般init=/linuxrc, 或者init=/etc/preinit，preinit的内容一般是创建console,null设备节点，运行init程序，挂载一些文件系统等等操作。请注意，很多初学者以为init=/linuxrc是固定写法，其实不然，/linuxrc指的是/目录下面的linuxrc脚本，一般是一个连接罢了。

H. ip

指定系统启动之后网卡的ip地址，如果你使用基于nfs的文件系统，那么必须要有这个参数，其他的情况下就看你自己的喜好了。设置ip有两种方法：

 ip = ip addr

 ip=ip addr:server ip addr:gateway:netmask::which netcard:off

这两种方法可以用，不过很明显第二种要详细很多，请注意第二种中which netcard 是指开发板上的网卡，而不是主机上的网卡。

说完常见的几种bootargs，那么我们来讨论平常我经常使用的几种组合：

1). 假设文件系统是ramdisk，且直接就在内存中，bootargs的设置应该如下：

setenv bootargs ‘initrd=0x32000000,0xa00000 root=/dev/ram0 console=ttySAC0 mem=64M init=/linuxrc’

2). 假设文件系统是ramdisk，且在flash中，bootargs的设置应该如下：

setenv bootargs ‘mem=32M console=ttyS0,115200 root=/dev/ram rw init=/linuxrc’

注意这种情况下你应该要在bootm命令中指定ramdisk在flash中的地址，如bootm kernel_addr ramdisk_addr (fdt_addr)

3). 假设文件系统是jffs2类型的，且在flash中，bootargs的设置应该如下

setenv bootargs ‘mem=32M console=ttyS0,115200 noinitrd root=/dev/mtdblock2 rw rootfstype=jffs2 init=/linuxrc’

4). 假设文件系统是基于nfs的，bootargs的设置应该如下

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5:192.168.0.3:192.168.0.3:255.255.255.0::eth0:off’

或者

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5’


B. rootfstype

    这个选项需要跟root一起配合使用，一般如果根文件系统是ext2的话，有没有这个选项是无所谓的，但是如果是jffs2,squashfs等文件系统的话，就需要rootfstype指明文件系统的类型，不然会无法挂载根分区.

C. console


console=tty<n>  使用虚拟串口终端设备 <n>.

console=ttyS<n>[,options] 使用特定的串口<n>，options可以是这样的形式bbbbpnx，这里bbbb是指串口的波特率，p是奇偶校验位，n是指的bits。

console=ttySAC<n>[,options] 同上面。


看你当前的环境，有时用ttyS<n>，有时用ttySAC<n>，网上有人说，这是跟内核的版本有关，2.4用ttyS<n>，2.6用ttySAC<n>，但实际情况是官方文档中也是使用ttyS<n>，所以应该是跟内核版本没有关联的。可以查看Documentation/serial-console.txt找到相关描述。


D. mem

mem=xxM 指定内存的大小，不是必须的

E. ramdisk_size

ramdisk=xxxxx           不推荐  

ramdisk_size=xxxxx   推荐

上面这两个都可以告诉ramdisk 驱动，创建的ramdisk的size，默认情况下是4m(s390默认8M)，你可以查看Documentation/ramdisk.txt找到相关的描述，不过ramdisk=xxxxx在新版的内核都已经没有提了，不推荐使用。

F. initrd, noinitrd

当你没有使用ramdisk启动系统的时候，你需要使用noinitrd这个参数，但是如果使用了的话，就需要指定initrd=r_addr,size, r_addr表示initrd在内存中的位置，size表示initrd的大小。

G. init

init指定的是内核启起来后，进入系统中运行的第一个脚本，一般init=/linuxrc, 或者init=/etc/preinit，preinit的内容一般是创建console,null设备节点，运行init程序，挂载一些文件系统等等操作。请注意，很多初学者以为init=/linuxrc是固定写法，其实不然，/linuxrc指的是/目录下面的linuxrc脚本，一般是一个连接罢了。

H. ip

指定系统启动之后网卡的ip地址，如果你使用基于nfs的文件系统，那么必须要有这个参数，其他的情况下就看你自己的喜好了。设置ip有两种方法：

 ip = ip addr

 ip=ip addr:server ip addr:gateway:netmask::which netcard:off

这两种方法可以用，不过很明显第二种要详细很多，请注意第二种中which netcard 是指开发板上的网卡，而不是主机上的网卡。

说完常见的几种bootargs，那么我们来讨论平常我经常使用的几种组合：

1). 假设文件系统是ramdisk，且直接就在内存中，bootargs的设置应该如下：

setenv bootargs ‘initrd=0x32000000,0xa00000 root=/dev/ram0 console=ttySAC0 mem=64M init=/linuxrc’

2). 假设文件系统是ramdisk，且在flash中，bootargs的设置应该如下：

setenv bootargs ‘mem=32M console=ttyS0,115200 root=/dev/ram rw init=/linuxrc’

注意这种情况下你应该要在bootm命令中指定ramdisk在flash中的地址，如bootm kernel_addr ramdisk_addr (fdt_addr)

3). 假设文件系统是jffs2类型的，且在flash中，bootargs的设置应该如下

setenv bootargs ‘mem=32M console=ttyS0,115200 noinitrd root=/dev/mtdblock2 rw rootfstype=jffs2 init=/linuxrc’

4). 假设文件系统是基于nfs的，bootargs的设置应该如下

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5:192.168.0.3:192.168.0.3:255.255.255.0::eth0:off’

或者

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5’









