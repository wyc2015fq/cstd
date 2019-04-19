# bootcmd 和bootargs - maopig的专栏 - CSDN博客
2011年11月30日 23:31:17[maopig](https://me.csdn.net/maopig)阅读数：1845标签：[flash																[脚本																[平台																[服务器																[linux																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)
个人分类：[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)

看到这个标题，可能觉得这个并没有什么的，其实不然，编好了u-boot了，但是如何来使用确不是那么简单的，想当初我将uboot制作出来后以为全部都搞定了，屁颠屁颠的烧到板子上后可系统就是起不来，为什么了，折腾了很久以后才发现是u-boot的环境变量设置错误，在后来的工作中，我发现一些有一定经验的人在这方面也是模模糊糊的扯不清，所以这里要讨论一下。
   u-boot的环境变量是使用u-boot的关键，它可以由你自己定义的，但是其中有一些也是大家经常使用，约定熟成的，有一些是u-boot自己定义的，更改这些名字会出现错误，下面的表中我们列出了一些常用的环境变量：
|环境变量|描述|
|----|----|
|bootdelay|执行自动启动的等候秒数|
|baudrate|串口控制台的波特率|
|netmask|以太网接口的掩码|
|ethaddr|以太网卡的网卡物理地址|
|bootfile|缺省的下载文件|
|bootargs|传递给内核的启动参数|
|bootcmd|自动启动时执行的命令|
|serverip|服务器端的ip地址|
|ipaddr|本地ip地址|
|stdin|标准输入设备|
|stdout|标准输出设备|
|stderr|标准出错设备|
上面只是一些最基本的环境变量，请注意，板子里原本是没有环境变量的，u-boot的缺省情况下会有一些基本的环境变量，在你执行了saveenv之后，环境变量会第一次保存到flash中，之后你对环境变量的修改,保存都是基于保存在flash中的环境变量的操作。
   U-boot的环境变量值得注意的有两个：bootcmd和bootargs。
u      bootcmd
   前面有说过bootcmd是自动启动时默认执行的一些命令，因此你可以在当前环境中定义各种不同配置，不同环境的参数设置，然后设置bootcmd为你经常使用的那种参数。
u      bootargs
   bootargs是环境变量中的重中之重，甚至可以说整个环境变量都是围绕着bootargs来设置的。bootargs的种类非常非常的多，我们平常只是使用了几种而已，感兴趣的可以看看这篇文章说的很全：http://blog.chinaunix.net/u2/79570/showart_1675071.html。bootargs非常的灵活，内核和文件系统的不同搭配就会有不同的设置方法，甚至你也可以不设置bootargs,而直接将其写到内核中去（在配置内核的选项中可以进行这样的设置），正是这些原因导致了bootargs使用上的困难。
   下面介绍一下bootargs常用参数，bootargs的种类非常的多，而且随着kernel的发展会出现一些新的参数，使得设置会更加灵活多样。
A. root
用来指定rootfs的位置， 常见的情况有:
   root=/dev/ram rw  
   root=/dev/ram0 rw
 请注意上面的这两种设置情况是通用的，我做过测试甚至root=/dev/ram1 rw和root=/dev/ram2 rw也是可以的，网上有人说在某些情况下是不通用的，即必须设置成ram或者ram0，但是目前还没有遇到，还需要进一步确认，遇到不行的时候可以逐一尝试。
   root=/dev/mtdx rw
   root=/dev/mtdblockx rw
   root=/dev/mtdblock/x rw
   root=31:0x
上面的这几个在一定情况下是通用的，当然这要看你当前的系统是否支持，不过mtd是字符设备，而mtdblock是块设备，有时候你的挨个的试到底当前的系统支持上面那种情况下，不过root=/dev/mtdblockx rw比较通用。此外，如果直接指定设备名可以的话，那么使用此设备的设备号也是可以的。
   root=/dev/nfs
在文件系统为基于nfs的文件系统的时候使用。当然指定root=/dev/nfs之后，还需要指定nfsroot=serverip:nfs_dir，即指明文件系统存在那个主机的那个目录下面。
B. rootfstype
   这个选项需要跟root一起配合使用，一般如果根文件系统是ext2的话，有没有这个选项是无所谓的，但是如果是jffs2,squashfs等文件系统的话，就需要rootfstype指明文件系统的类型，不然会无法挂载根分区.
C.console
console=tty 使用虚拟串口终端设备.
console=ttyS[,options] 使用特定的串口，options可以是这样的形式bbbbpnx，这里bbbb是指串口的波特率，p是奇偶校验位，n是指的bits。
console=ttySAC[,options] 同上面。
看你当前的环境，有时用ttyS，有时用ttySAC，网上有人说，这是跟内核的版本有关，2.4用ttyS，2.6用ttySAC，但实际情况是官方文档中也是使用ttyS，所以应该是跟内核版本没有关联的。可以查看Documentation/serial-console.txt找到相关描述。
D. mem
mem=xxM指定内存的大小，不是必须的
E. ramdisk_size
ramdisk=xxxxx          不推荐  
ramdisk_size=xxxxx  推荐
上面这两个都可以告诉ramdisk驱动，创建的ramdisk的size，默认情况下是4m(s390默认8M)，你可以查看Documentation/ramdisk.txt找到相关的描述，不过ramdisk=xxxxx在新版的内核都已经没有提了，不推荐使用。
F. initrd, noinitrd
当你没有使用ramdisk启动系统的时候，你需要使用noinitrd这个参数，但是如果使用了的话，就需要指定initrd=r_addr,size, r_addr表示initrd在内存中的位置，size表示initrd的大小。
G. init
init指定的是内核启起来后，进入系统中运行的第一个脚本，一般init=/linuxrc,或者init=/etc/preinit，preinit的内容一般是创建console,null设备节点，运行init程序，挂载一些文件系统等等操作。请注意，很多初学者以为init=/linuxrc是固定写法，其实不然，/linuxrc指的是/目录下面的linuxrc脚本，一般是一个连接罢了。
H. mtdparts
mtdparts=fc000000.nor_flash:1920k(linux),128k(fdt),20M(ramdisk),4M(jffs2),38272k(user),256k(env),384k(uboot)
要想这个参数起作用，内核中的mtd驱动必须要支持，即内核配置时需要选上Device Drivers ---> Memory Technology Device (MTD) support ---> Command line partition table parsing
mtdparts的格式如下：
mtdparts=[;< font>
 := :[,]
 := [@offset][][ro]
  := unique id used in mapping driver/device
   := standard linux memsize OR "-" to denote all remaining space
   := (NAME)
因此你在使用的时候需要按照下面的格式来设置：
mtdparts=mtd-id:@(),@()
这里面有几个必须要注意的：
a. mtd-id必须要跟你当前平台的flash的mtd-id一致，不然整个mtdparts会失效
b. size在设置的时候可以为实际的size(xxM,xxk,xx)，也可以为''-''这表示剩余的所有空间。
举例：
假设flash的mtd-id是sa1100，那么你可以使用下面的方式来设置：
mtdparts=sa1100:-    → 只有一个分区
mtdparts=sa1100:256k(ARMboot)ro,-(root) → 有两个分区
可以查看drivers/mtd/cmdlinepart.c中的注释找到相关描述。
I. ip
指定系统启动之后网卡的ip地址，如果你使用基于nfs的文件系统，那么必须要有这个参数，其他的情况下就看你自己的喜好了。设置ip有两种方法：
 ip = ip addr
 ip=ip addr:server ip addr:gateway:netmask::which netcard:off
这两种方法可以用，不过很明显第二种要详细很多，请注意第二种中which netcard是指开发板上的网卡，而不是主机上的网卡。
说完常见的几种bootargs，那么我们来讨论平常我经常使用的几种组合：
1).假设文件系统是ramdisk，且直接就在内存中，bootargs的设置应该如下：
setenv bootargs ‘initrd=0x32000000,0xa00000 root=/dev/ram0 console=ttySAC0 mem=64Minit=/linuxrc’
2).假设文件系统是ramdisk，且在flash中，bootargs的设置应该如下：
setenv bootargs ‘mem=32M console=ttyS0,115200 root=/dev/ram rw init=/linuxrc’
注意这种情况下你应该要在bootm命令中指定ramdisk在flash中的地址，如bootm kernel_addr ramdisk_addr (fdt_addr)
3).假设文件系统是jffs2类型的，且在flash中，bootargs的设置应该如下
setenv bootargs ‘mem=32M console=ttyS0,115200 noinitrd root=/dev/mtdblock2 rwrootfstype=jffs2 init=/linuxrc’
4).假设文件系统是基于nfs的，bootargs的设置应该如下
setenv bootargs‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5:192.168.0.3:192.168.0.3:255.255.255.0::eth0:off’
或者
setenv bootargs‘noinitrd mem=64M console=ttySAC0 root=/dev/nfs nfsroot=192.168.0.3:/nfs ip=192.168.0.5’
上面就是我们经常使用的几种bootargs的组合，老实说，bootargs非常非常的灵活，所以设置的方法有很多中形式，具体的还应该根据你的平台具体的情况来设置

