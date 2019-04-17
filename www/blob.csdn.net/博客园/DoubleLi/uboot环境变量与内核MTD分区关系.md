# uboot环境变量与内核MTD分区关系 - DoubleLi - 博客园






**uboot 与系统内核中MTD分区的关系：**

分区只是内核的概念，就是说A～B地址放内核，C～D地址放文件系统，(也就是规定哪个地址区间放内核或者文件系统)等等。

1：在内核MTD中可以定义分区A~B，C~D。。。。。。并予以绝对的地址赋值给每个分区。我们可以来看看在内核中是怎样来对MTD进行分区的：arch/arm/plat-s3c24xx/common-smdk.c

static struct mtd_partition smdk_default_nand_part[] = {
 [0] = {
  .name = "Boot",
  .size = SZ_16K,
  .offset = 0,
 },
 [1] = {
  .name = "S3C2410 flash partition 1",
  .offset = 0,
  .size = SZ_2M,
 },
 [2] = {
  .name = "S3C2410 flash partition 2",
  .offset = SZ_4M,
  .size = SZ_4M,
 },
 [3] = {
  .name = "S3C2410 flash partition 3",
  .offset = SZ_8M,
  .size = SZ_2M,
 },
 [4] = {
  .name = "S3C2410 flash partition 4",
  .offset = SZ_1M * 10,
  .size = SZ_4M,
 },

......
 };

一般我们只需要分3-4个区，第一个为boot区，一个为boot参数区(传递给内核的参数),一个为内核区，一个为文件系统区。

而对于bootloader中只要能将内核下载到A~B区的A地址开始处就可以，C~D区的C起始地址下载文件系统。。。这些起始地址在MTD的分区信息中能找到。所以bootloader对分区的概念不重要，只要它能把内核烧到A位置，把文件系统烧到C位置。
所以，在bootloader对Flash进行操作时，哪块区域放什么是以内核为主。

**而为了方便操作，bootloader类似也引入分区的概念**，如，可以使用“nand write 0x3000000 kernel 200000”命令将uImage烧到kernel分区，而不必写那么长：nand write 3000000 A 200000,也就是用分区名来代替具体的地址。

这要对bootloader对内核重新分区：这需要重新设置一下bootloader环境参数，就可以同步更新内核分区信息

如：

setenv bootargs 'noinitrd console=ttySAC0 root=/dev/mtdblock3 rootfstype=jffs2

                           mtdparts=nand_flash:128k(u-boot)ro,64k(u-boot envs),3m(kernel),30m(root.jffs2),30m(root.yaffs)'

内核配置时选上Device Drivers  ---> Memory Technology Device (MTD) support  ---> Command line partition table parsing

在设置了mtdparts变量之后,就可以在nand read/write/erase命令中直接使用分区的名字而不必指定分区的偏移位置.**而这需要内核MTD最好没有规划分区。**

**如果你是通过uboot的内核命令行给MTD层传递MTD分区信息，这种情况下,内核读取到的分区信息始终和u-boot中的保持一致(推荐的做法)**

**如果你是把分区信息写在内核源代码MTD里定义好的方法,那最好保证它和u-boot中的保持一致,即同步修改uboot及内核的相关部分。**

2：

内核通过bootargs找到文件系统，bootargs中的mtdblockx即代表分区，block1，2，3代表哪个分区。

事实上,bootargs中的"root=/dev/mtdblockx"只是告诉内核,root fs从第x个(x=0,1,2...)MTD分区挂载,mtdblock0对应第一个分区,mtdblock1对应第二个分区,以此类推.

3：分区方法

1) MTD层的分区

2) 通过U-boot传递给内核的命令行中的mtdparts=...

3) 其他可以让内核知道分区信息的任何办法，（内核默认的命令参数）

下面说到mtdparts，及它的用法：

**mtdparts**

mtdparts=fc000000.nor_flash:1920k(linux),128k(fdt),20M(ramdisk),4M(jffs2),38272k(user),256k(env),384k(uboot)

要想这个参数起作用，内核中的mtd驱动必须要支持，即内核配置时需要选上Device Drivers  ---> Memory Technology Device (MTD) support  ---> Command line partition table parsing



mtdparts的格式如下：

mtdparts=<mtddef>[;<mtddef]

<mtddef>  := <mtd-id>:<partdef>[,<partdef>]

 <partdef> := <size>[@offset][<name>][ro]

 <mtd-id>  := unique id used in mapping driver/device

<size>    := standard linux memsize OR "-" to denote all remaining space

<name>    := (NAME)

因此你在使用的时候需要按照下面的格式来设置：

mtdparts=mtd-id:<size1>@<offset1>(<name1>),<size2>@<offset2>(<name2>)

这里面有几个必须要注意的：

a.  **mtd-id ****必须要跟你当前平台的flash的mtd-id一致，不然整个mtdparts会失效 怎样获取到当前平台的flash的mtd-id？**

**在bootargs参数列表中可以指定当前flash的mtd-id，如指定 mtdids:nand0=gen_nand.1，前面的nand0则表示第一个flash**

b.  size在设置的时候可以为实际的size(xxM,xxk,xx)，也可以为'-'这表示剩余的所有空间。

相关信息可以查看drivers/mtd/cmdlinepart.c中的注释找到相关描述。

U-boot的环境变量值得注意的有两个： bootcmd 和bootargs。

引用：

u       bootcmd

前面有说过bootcmd是自动启动时默认执行的一些命令，因此你可以在当前环境中定义各种不同配置，不同环境的参数设置，然后设置bootcmd为你经常使用的那种参数。

u       bootargs

    bootargs是环境变量中的重中之重，甚至可以说整个环境变量都是围绕着bootargs来设置的。bootargs的种类非常非常的多，我们平常只是使用了几种而已，感兴趣的可以看看这篇文章说的很全：[http://blog.chinaunix.net/u2/79570/showart_1675071.html](http://blog.chinaunix.net/u2/79570/showart_1675071.html)。bootargs非常的灵活，内核和文件系统的不同搭配就会有不同的设置方法，甚至你也可以不设置bootargs,而直接将其写到内核中去（在配置内核的选项中可以进行这样的设置），正是这些原因导致了bootargs使用上的困难。

下面介绍一下bootargs常用参数，bootargs的种类非常的多，而且随着kernel的发展会出现一些新的参数，使得设置会更加灵活多样。



A. root

用来指定rootfs的位置，常见的情况有:

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



C. console



console=tty<n>使用虚拟串口终端设备 <n>.

console=ttyS<n>[,options]使用特定的串口<n>，options可以是这样的形式bbbbpnx，这里bbbb是指串口的波特率，p是奇偶校验位，n是指的bits。

console=ttySAC<n>[,options]同上面。











看你当前的环境，有时用ttyS<n>，有时用ttySAC<n>，网上有人说，这是跟内核的版本有关，2.4用ttyS<n>，2.6用ttySAC<n>，但实际情况是官方文档中也是使用ttyS<n>，所以应该是跟内核版本没有关联的。可以查看Documentation/serial-console.txt找到相关描述。





D. mem

mem=xxM 指定内存的大小，不是必须的



E. ramdisk_size

ramdisk=xxxxx           不推荐

ramdisk_size=xxxxx   推荐

上面这两个都可以告诉ramdisk 驱动，创建的ramdisk的size，默认情况下是4m(s390默认8M)，你可以查看Documentation/ramdisk.txt找到相关的描述，不过ramdisk=xxxxx在新版的内核都已经没有提了，不推荐使用。



F. initrd, noinitrd

当你没有使用ramdisk启动系统的时候，你需要使用noinitrd这个参数，但是如果使用了的话，就需要指定initrd=r_addr,size, r_addr表示initrd在内存中的位置，size表示initrd的大小。



G. init

init指定的是内核启起来后，进入系统中运行的第一个脚本，一般init=/linuxrc, 或者init=/etc/preinit，preinit的内容一般是创建console,null设备节点，运行init程序，挂载一些文件系统等等操作。请注意，很多初学者以为init=/linuxrc是固定写法，其实不然，/linuxrc指的是/目录下面的linuxrc脚本，一般是一个连接罢了。



H. ip

指定系统启动之后网卡的ip地址，如果你使用基于nfs的文件系统，那么必须要有这个参数，其他的情况下就看你自己的喜好了。设置ip有两种方法：

 ip = ip addr

 ip=ip addr:server ip addr:gateway:netmask::which netcard:off

这两种方法可以用，不过很明显第二种要详细很多，请注意第二种中which netcard 是指开发板上的网卡，而不是主机上的网卡。



说完常见的几种bootargs，那么我们来讨论平常我经常使用的几种组合：

1). 假设文件系统是ramdisk，且直接就在内存中，bootargs的设置应该如下：

setenv bootargs ‘initrd=0x32000000,0xa00000 root=/dev/ram0 console=ttySAC0 mem=64M init=/linuxrc’



2). 假设文件系统是ramdisk，且在flash中，bootargs的设置应该如下：

setenv bootargs ‘mem=32M console=ttyS0,115200 root=/dev/ram rw init=/linuxrc’

注意这种情况下你应该要在bootm命令中指定ramdisk在flash中的地址，如bootm kernel_addr ramdisk_addr (fdt_addr)



3). 假设文件系统是jffs2类型的，且在flash中，bootargs的设置应该如下

setenv bootargs ‘mem=32M console=ttyS0,115200 noinitrd root=/dev/mtdblock2 rw rootfstype=jffs2 init=/linuxrc’



4). 假设文件系统是基于nfs的，bootargs的设置应该如下

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5:192.168.0.3:192.168.0.3:255.255.255.0::eth0:off’

或者

setenv bootargs ‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5’









