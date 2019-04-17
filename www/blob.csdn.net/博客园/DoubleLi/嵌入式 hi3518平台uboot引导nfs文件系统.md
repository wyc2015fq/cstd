# 嵌入式 hi3518平台uboot引导nfs文件系统 - DoubleLi - 博客园






首先贴出来我的bootargs的设置(注没有换行符!!!)：

setenv bootargs noinitrd mem=64M root=/dev/nfs init=/linuxrc rw nfsroot=10.10.2.59:/opt/rootfs/ ip=10.10.1.156:10.10.2.59:10.10.1.1:255.255.255.0:skdkjzz:eth0:off console=ttyAMA0,115200



Linux编译生成zImage，但是uboot不能识别，uboot只能识别uImage的文件格式，uImage与zImage文件的区别是uImage比zImage多个文件头，我们现在利用uboot的mkimage工具来生成uImage文件。





通常，u－boot为kernel提供一些kernel无法知道的信息，比如ramdisk在RAM中的地址。Kernel也必须为U-boot提供必要的信息，如通过mkimage这个工具（在u-boot代码的tools目录中）可以给zImage添加一个header，也就是使得通常编译的内核zImage添加一个数据头，把添加头后的image通常叫uImage，uImage是可以被U-boot直接引导的内核镜像。那么如何使用mkimage工具而产生uImage的呢？下面将具体介绍mkimage工具的使用：

1.首先进入u-boot目录下tools文件夹下，查看mkimage的命令参数
[root@localhost tools]# ./mkimage 
Usage: ./mkimage -l image
          -l ==> list image header information
       ./mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file

[:data_file...] image
          -A ==> set architecture to 'arch'   //用于指定CPU类型，比如ARM
          -O ==> set operating system to 'os'  //用于指定操作系统，比如Linux
          -T ==> set image type to 'type'      //用于指定image类型，比如Kernel
          -C ==> set compression type 'comp'   //指定压缩类型
          -a ==> set load address to 'addr' (hex)  //指定image的载入地址
          -e ==> set entry point to 'ep' (hex)     //内核的入口地址，一般是：image的载入地址+0x40（信息头的大小）
          -n ==> set image name to 'name'          //image在头结构中的命名
          -d ==> use image data from 'datafile'    //无头信息的image文件名
          -x ==> set XIP (execute in place)        //设置执行位置

-a参数后是内核的运行地址，-e参数后是入口地址。

1）如果我们没用mkimage对内核进行处理的话，那直接把内核下载到0x40008000再运行就行，内核会自解压运行（不过内核运行需要一个tag来传递参数，而这个tag建议是由bootloader提供的，在u-boot下默认是由bootm命令建立的）。

2）如果使用mkimage生成内核镜像文件的话，会在内核的前头加上了64byte的信息，供建立tag之用。bootm命令会首先判断bootm xxxx 这个指定的地址xxxx是否与-a指定的加载地址相同。
(1)如果不同的话会从这个地址开始提取出这个64byte的头部，对其进行分析，然后把去掉头部的内核复制到-a指定的load地址中去运行之
(2)如果相同的话那就让其原封不同的放在那，但-e指定的入口地址会推后64byte，以跳过这64byte的头部。

执行：./mkimage -A arm -O linux -T kernel -C none -a 40008000 -e 40008040 -n linux-2.6.31 -d zImage uImage



生成文件之后，根据所要选择启动的文件系统，设置uboot的参数：

1、启动ramdisk文件系统

setenv bootargs console=ttyAM0,115200 initrd=0x40400000,0x800000 root=/dev/ram0

setenv bootcmd tftp 0x40008000 uImage;bootm 0x40008000

setenv serverip 192.167.10.24

setenv ipaddr 192.167.10.2

2、启动nfs文件系统

**x86 Linux主机开启nfs服务，步骤如下：**



1、软硬件环境

VMware 7.0.0，Ubuntu 9.04

2、ubuntu安装后默认是没有带nfs的，使用如下命令安装：

我直接进的root帐户。

apt-get install nfs-kernel-server

apt-get install nfs-common 

3、虚拟机配置选项里网卡使用的是桥接，IP地址为192.168.0.1,和主机的192.168.0.11在同一网段，主机是连到路由器的。

4、修改配置文件

在设置配置文件之前，先建立共享目录/home/lah/nfs，nfs共享目录。修改nfs配置文件/etc/exports，添加如下一行：

/home/lah/nfs   *(rw,sync,no_root_squash)

第一个参数是nfs共享目录，第二个是你允许的主机IP，这里设置成所有客户机都可共享该目录，括号里面的rw表示挂接此目录的客户机对该目录有读写的权限，no_root_squash  表示允许挂接此目录的客户机享有该主机的root 身份。

5、启动NFS服务并测试

/etc/init.d/portmap start

/etc/init.d/nfs-kernel-server start

现在roo_fs目录下放入一些文件，然后通过nfs挂载mnt目录测试。

mount localhost:/home/lah/nfs /mnt

       如果mnt下有/home/lah/nfs目录下的文件，则证明nfs服务已经配置好了。



**将nfs作为根文件系统时，开启的nfs服务的文件夹不能为空，应该放busybox制作的根文件系统。**



**uboot设置bootargs命令的方法**

**示例：**




**setenv bootargs console=ttyAM0,115200 noinitrd root=/dev/nfs nfsroot=192.167.10.6:/home/lah/nfs ip=192.167.10.2:192.167.10.6:::forlinux:eth0:**













|**#setenv bootargs noinitrd console=ttySAC0,115200 init=/linuxrc mem=64M root=/dev/nfs nfsroot=192.168.2.125:/home/hufei/nfsrootip=192.168.2.6:192.168.2.125:192.168.2.125:255.255.255.0:hufei.cublog.cn:eth0:off**|
|----|



**initrd, noinitrd:**
当你没有使用ramdisk启动系统的时候，你需要使用noinitrd这个参数，但是如果使用了的话，就需要指定initrd=r_addr,size, r_addr表示initrd在内存中的位置，size表示initrd的大小。



**console:**

console=tty  使用虚拟串口终端设备 .
console=ttyS[,options] 使用特定的串口，options可以是这样的形式bbbbpnx，这里bbbb是指串口的波特率，p是奇偶位（从来没有看过使用过），n是指的bits。
console=ttySAC[,options] 同上面。

看你当前的环境，有时用ttyS，有时用ttySAC，网上有人说，这是跟内核的版本有关，2.4用ttyS，2.6用ttySAC，但实际情况是官方文档中也是使用ttyS，所以应该是跟内核版本没有关联的。可以查看Documentation/serial-console.txt找到相关描述。



**init:**

init指定的是内核启起来后，进入系统中运行的第一个脚本，一般init=/linuxrc, 或者init=/etc/preinit，preinit的内容一般是创建console,null设备节点，运行init程序，挂载一些文件系统等等操作。请注意，很多初学者以为init=/linuxrc是固定写法，其实不然，/linuxrc指的是/目录下面的linuxrc脚本，一般是一个连接罢了。如果内核找不到linurc文件，将会依次搜索/sbin/init,/etc/init,/bin/init,/bin/sh.



**mem:**

指定内存大小，不是必须的



**root:**

用来指定rootfs的位置， 常见的情况有: 
    root=/dev/ram rw   
    root=/dev/ram0 rw
请注意上面的这两种设置情况是通用的，我做过测试甚至root=/dev/ram1 rw和root=/dev/ram2 rw也是可以的，网上有人说在某些情况下是不通用的，即必须设置成ram或者ram0，但是目前还没有遇到，还需要进一步确认，遇到不行的时候可以逐一尝试。

    root=/dev/mtdx rw
    root=/dev/mtdblockx rw
    root=/dev/mtdblock/x rw
    root=31:0x

上面的这几个在一定情况下是通用的，当然这要看你当前的系统是否支持，不过mtd是字符设备，而mtdblock是块设备，有时候你的挨个的试到底当前的系统支持上面那种情况下，不过root=/dev/mtdblockx rw比较通用。此外，如果直接指定设备名可以的话，那么使用此设备的设备号也是可以的。

**root=/dev/nfs**,并非真的设备，而是一个告诉内核经由网络取得根文件系统的旗标。 
在文件系统为基于nfs的文件系统的时候使用。当然指定root=/dev/nfs之后，还需要指定nfsroot,

nfsroot这个参数告诉内核以哪一台机器，哪个目录以及哪个网络文件系统选项作为根文件系统使用。参数的格式如下：

**nfsroot=[<server-ip>:]<root-dir>[,<nfs-options>]**

如果指令列上没有给定 nfsroot 参数，则将使用‘/tftpboot/%s’预设值。其它选项如下：

<server-ip> --指定网络文件系统服务端的互联网地址(IP address)。如果没有给定此栏位，则使用由 nfsaddrs 变量（见下面）所决定的值。此参数的用途之一是允许使用不同机器作为反向地址解析协议(RARP) 及网络文件系统服务端。通常你可以不管它（设为空白）。

<root-dir> -- 服务端上要作为根挂入的目录名称。如果字串中有个‘%s’ 符记(token)，此符记将代换为客户端互联网地址之ASCII表示法。

<nfs-options> -- 标准的网络文件系统选项。所有选项都以逗号分开。如果没有给定此选项栏位则使用下列的预设值：

        port            = as given by server portmap daemon

**rsize           = 1024**

**        wsize           = 1024**

** timeo           = 7**

        retrans         = 3

        acregmin        = 3

        acregmax        = 60

        acdirmin        = 30

        acdirmax        = 60

        flags           = hard, nointr, noposix, cto, ac

参数nfsaddrs设定网络通讯所需的各种网络接口地址。如果没有给定这个参数，则内核核会试著使用反向地址解析协议以及／或是启动协议(BOOTP)以找出这些参数。其格式如下：

**ip:**

下面是U-boot官方文档提供的IP参数解析：
|**setenv bootargs ${bootargs}****ip=${ipaddr}:${serverip}:/****${gatewayip}:${netmask}:/****${hostname:${netdev}:off**|
|----|



注意,上面换行的地方均有空格。其中 192.168.2.6是开发板的IP,192.168.2.125

是PC端（或虚拟机）的 IP,上面的IP根据自己的实际情况修改,不要弄错了。



**nfsaddrs=<my-ip>:<serv-ip>:<gw-ip>:<netmask>:<name>:<dev>:<auto>**

<my-ip> -- 客户端的互联网地址。如果没设，此地址将由反向地址解析协议(RARP)或启动协议来决定。使用何种协议端视配置核心时打开的选项以及 参数而定。如果设定此参数，就不会使用反向地址解析协议或启动协议。

<serv-ip> -- 网络文件系统服务端之互联网地址。如果使用反向地址解析协议来决定客户端地址并且设定此参数，则只接受从指定之服务端传来的回应。要使用不同的机器作为反向地址解析与网络文件系统服务端的话，在此指定你的反向地址解析协议服务端（保持空白）并在 nfsroot 参数（见上述）中指定你的网络文件系统服务端。如果此项目空白则使用回答反向地址解析协议或启动协议之服务端的地址。

<gw-ip> -- 网关(gateway)之互联网地址，若服务端位於不同的子网络上时。如果此项目空白则不使用任何网关并假设服务端在本地的(local)网络上，除非由启动协议接收到值。

<netmask> -- 本地网络界面的网络掩码。如果为空白，则网络掩码由客户端的互联网地址导出，除非由启动协议接收到值。

<name> -- 客户端的名称。如果空白，则使用客户端互联网地址之 ASCII-标记法，或由启动协议接收的值。

<dev> -- 要使用的网络设备名称。如果为空白，所有设备都会用来发出反向地址解析请求，启动协议请求由最先找到的设备发出。网络文件系统使用接收到反向地址解析协议或启动协议回应的设备。如果你只有一个设备那你可以不管它。

<auto> -- 用以作为自动配置的方法。如果是 `rarp' 或是 `bootp' 则使用所指示的协议。如果此值为`both' 或空白，若配置核心时有打开这两种协议则都使用。 `none' 表示不使用自动配置。这种情况下你必须指定前述栏位中所有必要的值。

此参数可以作为 nfsaddrs 的参数单独使用（前面没有任何 `:` 字符），这种情况下会使用自动配置。然而，此种情况不能使用`none'作为值。



===========================================================================================================================



主机说明:
主机guest 为虚拟机redhat9: IP: 59.64.155.122  网关 59.64.155.1
redhat9上已经配置好NFS服务
也已经配置好TFTP服务
NFS服务, TFTP服务都已经启动
终端采用主机host XP的超级终端

实验板说明:
MPC8349itx开发板: IP: 59.64.155.244  网关 59.64.155.1
开发板eth0为vsc8201芯片，uboot支持驱动，另一芯片为交换交换芯片vsc7385，单独模块驱动。
U-Boot-1.1.3 ;    Linux kernel-2.6.13
内核支持NFS分区(即编译时在File system中选中[*] Root file system on NFS), 以及支持内核IP_PNP(即编译时在Networking中选中[*] IP: kernel level autoconfiguration)
    File systems  ---> 
        Network File Systems  ---> 
            <*> NFS file system support                        ## 必选
                [*]   Provide NFSv3 client support             ## 可选
            [*] Root file system on NFS                        ## 必选
    Networking  ---> 
        [*] Networking support 
            Networking options  ---> 
                [*]   IP: kernel level autoconfiguration       ## 必选

***************************************************************************

A: NFS启动挂载根文件系统

主机上操作:
$ cd /usr/local/mpc8349/
$ ./ltib --preconfig config/platform/mpc8349itx/defconfig-min-fs -f
    ## 依次进入如下选项选择(X) NFS only, 根据情况决定是否选择[ ] read-only root filesystem 
    --- Target Image Generation
        Options  --->
        --- Choose your root filesystem image type 
             Target image: (ext2.gz ramdisk)  --->         ## 改ramdisk文件系统为NFS
            (X) NFS only 
            [ ] read-only root filesystem
    ## 其他情况参考LTIB使用说明()
    ## 编译结束后生成的根文件系统是位于当前安装目录下的rootfs (/usr/local/mpc8349/small/rootfs)

$ ln -s /usr/local/mpc8349/small/rootfs  /home/liuby/rootfsln    ## 建立NFS服务目录软连接
$ su - root        ## 切换到root用户，需要密码
# echo "/usr/local/mpc8349/small/rootfs  59.64.155.244(rw,sync,no_root_squash)" > /etc/exports     ## NFS 服务配置
# service portmap restart
# service nfs restart
# exportfs -arv        ## 修改的NFS配置生效(exportfs参数顺序不一样，显示有所不同，此顺序显示结果明了)
# exit


实验板上操作:
uboot启动后按键进入uboot命令行环境:
=> cp.b fef50000 40000 20000; go 40004        ## 驱动vsc7385芯片
=> set serverip 59.64.155.122          ## 主机地址
=> set ipaddr 59.64.155.244            ## 实验板地址
=> set netmask 255.255.255.0
=> set netdev eth0                     ## eth0
=> ping 59.64.155.122                  ## 测试连通
=> set hostname PowerQUICC
=> set kernaddr fe810000            ## flash中内核起始地址
=> set rootpath /usr/local/mpc8349/small/rootfs        ## NFS服务根目录
=> setenv bootargs root=/dev/nfs rw nfsroot=$serverip:$rootpath ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=ttyS0,115200
=> bootm $kernaddr
            ## 如果一切正常，内核启动后会启动NFS文件系统作根文件系统。
            ## 测试: 在开发板上创建一个文件，在主机上会看到这个文件生成
同样可以简化操作(测试nfs服务软连接目录):
=> cp.b fef50000 40000 20000; go 40004        ## 驱动vsc7385芯片
=> setenv ipaddr 59.64.155.244
=> setenv bootargs root=/dev/nfs rw nfsroot=59.64.155.122:/home/liuby/rootfsln ip=59.64.155.244:59.64.155.122:255.255.255.0 console=ttyS0,115200 
=> bootm fe810000            ## 内核镜像uImage在FLASH中的存储地址

************************

实验中遇到一个问题，就是bootm之后内核启动，NFS启动挂载文件系统快完成时出现下面这个错误提示
RPC: sendmsg returned error 101
nfs: RPC call returned error 101
后来才发现是文件系统中的启动脚本在启动过程中修改了eth0的ip地址，导致连接不上NFS server
后来再测试又发现试图挂载非NFS文件系统目录时也会出现此问题，并且在主机上执行# showmount 命令结果会显示有ip地址(实验板ip地址)挂载。即: 在ltib编译文件系统时选择ext2.gz ramdisk (非NFS only), 最后使用rootfs目录，出现同样的问题。

还有遇到过一个问题，根本无法挂载文件系统，在启动时出现下述提示
Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(2,0)
 <0>Rebooting in 180 seconds..
后来发现是 /etc/exports 只有如下一句
 /home/liuby/rootfs  59.64.155.244(rw,sync,no_root_squash)
而没有像下面这样指定根目录的访问权限:
 /usr/local/mpc8349/small/rootfs  59.64.155.244(rw,sync,no_root_squash)
结论: 必须指定NFS文件系统所在目录或其对应的软连接做NFS服务根目录(见下分析)

在NFS文件系统中编译busybox时可以不选择编译mount命令(不建议如此)

nfs总结:
如上 /home/liuby/rootfsln是指向/usr/local/mpc8349/small/rootfs的软连接
# echo "/home/liuby/rootfsln 59.64.155.244(rw,sync,no_root_squash)" > /etc/exports
# exportfs -arv             ## 会弹出下面的提示
exporting 59.64.155.244:/usr/local/mpc8349/small/rootfs
可见 /etc/exports 中语句 /home/liuby/rootfsln 59.64.155.244(rw,sync,no_root_squash) 和 /usr/local/mpc8349/small/rootfs  59.64.155.244(rw,sync,no_root_squash) 是等效的.
故: 如果想用不同的文件系统，可以改变这个软连接指向新的文件系统，这比复制文件系统或者修改u-boot环境变量方便多了，并且还不用重新配置/etc/exports。
# echo "/home/liuby/rootfsln 59.64.155.244(rw, sync, no_root_squash)" > /etc/exports
$ rm -f /home/liuby/rootfsln
$ ln -s nfs_rootfs_dir_path  /home/liuby/rootfsln            ## 只需要建立和修改这个软连接即可
# exportfs -arv            ## 配置生效(必须)
在uboot环境变量中只需设置一次rootpath=/home/liuby/rootfsln, 每次只需在主机上修改此软连接的指向，即可
注意权限
***************************************************************************

B: TFTP下载内核和文件系统镜像到ram中启动

主机上操作:
在主机上TFTP根目录为/home/liuby/tftpboot/
$ mkdir /home/liuby/tftpboot/image_dir
$ cd /usr/local/mpc8349/small/
$ ./ltib --preconfig config/platform/mpc8349itx/defconfig-min-fs-modified -f --batch          ## 编译内核和文件系统镜像
$ cp -f rootfs/boot/uImage  rootfs.ext2.gz.uboot  ~/tftpboot/image_dir/        ## 复制内核镜像和文件系统镜像到~/tftpboot/image_dir
$ cd ~/tftpboot/
$ ln -s image_dir/ bootln


实验板上操作:
=> set serverip 59.64.155.122          ## 主机地址
=> set ipaddr 59.64.155.244            ## 实验板地址
=> set netmask 255.255.255.0 
=> set tftp_path bootln
=> ping $serverip                      ## 测试连通
=> cp.b fef50000 40000 20000; go 40004        ## 驱动vsc7385芯片
=> setenv loadkernaddr 1000000
=> setenv loadramdaddr 1200000
=> tftpboot $loadkernaddr image_dir/uImage    ## 下载内核镜像到ram
=> tftp $loadramdaddr $tftp_path/rootfs.ext2.gz.uboot    ## 下载文件系统镜像到ram
=> bootm $loadkernaddr $loadramdaddr    ## 启动ram中内核和文件系统镜像

下载不畅时，用CTRL+C来终止回到提示符


tftp总结:
在TFTP服务根目录下建立目录存储镜像文件，然后创建软连接指向需要下载的镜像目录，这样可以通过修改连接文件即可，而不用每次修改uboot环境变量了

***************************************************************************

C: TFTP下载内核镜像到ram，NFS挂载根文件系统(target image: NFS only)

主机上利用上面操作的结果，不进行配置。
实验板上操作:
=> set serverip 59.64.155.122          ## 主机地址
=> set ipaddr 59.64.155.244            ## 实验板地址
=> set netmask 255.255.255.0 
=> set bootargs root=/dev/nfs rw nfsroot=$serverip:/home/liuby/rootfsln ip=$ipaddr:$serverip:$netmask console=ttyS0,115200 
=> tftp 1000000 bootln/uImage          ## 下载内核镜像到ram 地址1000000
=> bootm 1000000
                ## 测试发现内核启动，NFS挂载成功

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

简便方法:
将下面附录的文件存储为.txt文件，例如取名mpc8349itx_uboot_env.txt
用linux命令unix2dos进行转换，然后打开windows XP 超级终端，启动实验板进入uboot命令行环境:
点击超级终端界面 "文件"->"属性"，弹出属性对话框，点击"设置"->"ASCII码设置"，设置行延迟20ms，字符延迟1ms；然后点击"发送"->"发送文本文件"，选择"mpc8349itx_uboot_env.txt"发送。
此操作相当于在uboot命令行输入uboot环境设置参数
执行完操作后保存了这些设置，以后每次在uboot启动时不用再敲入一行行的设置参数.

在uboot命令行下执行
=> run flashnfsboot
相当于 A 操作

在uboot命令行执行
=> run tftpramboot
相当于执行 B 操作

在uboot命令行执行
=> run tftpnfsboot
相当于执行 C 操作









