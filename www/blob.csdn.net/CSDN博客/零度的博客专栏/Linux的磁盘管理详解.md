# Linux的磁盘管理详解 - 零度的博客专栏 - CSDN博客
2019年03月27日 11:12:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：96
原文转载自：[https://www.cnblogs.com/xiaoxiangyucuo/p/5573662.html](https://www.cnblogs.com/xiaoxiangyucuo/p/5573662.html)
硬盘：几个盘片，双面，磁性颗粒，
处理速率不同步：借助于一个中间层   
**文件系统(FileSystem)**
    可以实现对磁盘行的文件进行读写
    文件系统其实就是一个特殊软件，是直接安装到硬盘上的
**硬盘分类**
    机械硬盘：
        盘片组，磁头，磁头臂
        马达、空气过滤片
    固态硬盘
        类似与内存的结构
**机械硬盘简介**
    磁道(Track)：磁盘旋转时候的同心圆，磁道编号从外向内，从0开始编号。
    扇区(Sector)：将每个磁道分割成等大小的空间，每个扇区大小都是512字节
    柱面(Cylinder)：不同盘片的相同扇区。磁盘分区的时候，是以柱面为单位进行分区的。
寻道时间：
    移动磁道到目标磁道
    旋转磁盘
PS：外侧磁道的读写性能，优于内侧磁道，所以系统一般安装在C盘 （C盘靠外）
**保存分区信息**
    0号扇区：MBR:Master(main) Boot Record,主引导记录，512字节
    第一部分：前446字节，存放程序bootloader，作用完成系统的开机启动
    第二部分：中间64字节，Disk Patration Table，磁盘分区表，作用是记录磁盘分区
        16字节标识一个分区，64字节可标识4个分区
    第三部分：最后的2字节，Magic Number，标记当前MBR是否生效
**分区类型 **
    主分区
    扩展分区
**分区结构**
    全部都是主分区：1、2、3、4主
    主+扩展分区：1主+1扩展  2主+1扩展 3主+1扩展
        扩展分区只能有一个
        扩展分区必须制作成逻辑分区才能使用
        逻辑分区可以有无限多个
**设备文件**
    linux 一切皆为文件 
    设备文件：物理设备在系统中的映射 
            块设备文件 ：支持随机读写的设备，硬盘
            字符设备文件 ：支持线性读写，键盘 
    设备文件的默认位置： /dev 
    设备文件的设备号 ：
    主设备号(major number)：标识设备类型（相同类型的设备，主设备号相同）
    次设备好(ninor number)：标识相同类型的不同设备
**接口**
    IDE:并行的传输接口
    SATA:串行的传输接口 （串行的速度快于并行）
    SCSI:并行接口，企业使用，Small Computer System Interface
    SAS:串行接口，性能好（串行的SCSI）
    USB接口：
        usb1.0
        usb2.0
        usb3.0
CentOS 6.x硬盘识别   
    /dev/sd#
        #=12345...
    /dev/sda1
    /dev/sda2
Centos 5.x
    IDE
        /dev/hdx
    SATA
        /dev/sdx
**接口都是控制芯片 **
    网卡适配器 ：独立核心功能是转换数据格式
    usb控制器  ：集成的
**linux的磁盘对应的设备文件的命名**
    磁盘对应的设备文件依次/dev/sda sdb sdc .....
    磁盘分区对应的设备文件
        第一个主分区：/dev/sd#1
        第二个主分区：/dev/sd#2
        第三个主分区：/dev/sd#3
        第四个主分区：/dev/sd#4
        扩展分区
        第一个逻辑分区：/dev/sd#5
        【逻辑分区的编号是从5开始的】
**fdisk命令**
    作用：查看或者修改磁盘的分区
    格式：fdisk [选项] [设备文件]
    选项：
        -l：列举系统中的全部磁盘以及磁盘分区
            (centos6中，默认将逻辑卷也识别为一个磁盘)
    例子：只查看物理磁盘的分区
    fdisk -l /dev/sd[a-z]
    例子：只查看sda分区
    fdisk -l /dev/sda
fdisk -l返回结果解析
Disk /dev/sda: 42.9 GB, 42949672960 bytes
设备文件名称  空间      空间
255 heads,  63 sectors/track,     5221 cylinders
虚拟磁头数  平均每磁道上的扇区数  柱面数
Units = cylinders of 16065 * 512 = 8225280 bytes
每个存储单元的大小
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x0008cfb6
磁盘标识符【在磁盘数量多的情况下，非常有用】
   Device       Boot          Start      End      Blocks     Id        System
/dev/sda1       *             1          64       512000     83        Linux
分区的设备文件  标识系统分区  起始柱面   结束柱面 磁盘块大小 分区类型  系统类型
/dev/sda2                     64         5222     41430016   8e        Linux LVM
注意：直接输fdisk+磁盘，如果不使用任何选项，那么就是对磁盘进行分区操作
**分区操作实例**
fdisk /dev/sdc
分区操作命令 （标示注释的是能用的到的）
   a：toggle a bootable flag
   b：edit bsd disklabel
   c：toggle the dos compatibility flag
   d：delete a partition                     删除分区
   l：list known partition types             列举分区类型
   m：print this menu                        打印帮助
   n：add a new partition                    新建分区
   o：create a new empty DOS partition table
   p：print the partition table              打印分区信息
   q：quit without saving changes            退出
   s：create a new empty Sun disklabel
   t：change a partition's system id         修改分区类型id
   u：change display/entry units
   v：verify the partition table
   w：write table to disk and exit           保存修改
   x：extra functionality (experts only)
主+扩展<=4
分区以后的磁盘，需要安装文件系统才能使用
为什么分区？
    方便检索磁盘上的文件
    不是必须分区。  
注意：如果一个磁盘已经分区并且在使用，那么在对磁盘进行分区操作可能会出现系统无法识别该分区，此时需要让内核重新读取MBR中的分区表。系统识别的分区信息记录在/proc/partitions文件中，partx命令和kpartx命令可以让内核重新读取分区信息。例如#partx –a /dev/sdb
分区必须安装软件才能实现文件的读写操作
    --文件系统
常见文件系统
    NTFS
    FAT32
    EXT系列
vfs：Virtual File System 虚拟文件系统
    作用：将底层的各种各样的文件系统抽象成一个同一个文件系统，这样可以极大的简化上层应用在使用文件系统时候的复杂度。
**linux文件系统详解**
**一、基本文件系统**
1、ext系列文件系统
    ext2 ext3 ext4
    centos6 默认使用ext4
2、Reiserfs
    处理和保存小文件的处理非常优秀
3、xfs
    对大文件处理性能非常好。
    文件大小无上限。
4、JFS,日志文件系统
    IBM所开发
5、vfat，其实就是windows下的fat32
6、NTFS，linux到目前为止，对NTFS支持不好
   (如果必须使用NTFS，那么建议只用NTFS来读数据，不要执行写操作)
**二、集群文件系统**
1、GFS2：Global File System 2 （全局文件文件系统）
    由redhat所研发
2、OCFS2：Oracle Cluster File System 2 
    由Oracle所研发
**三、网络文件系统 **
1、NFS：
2、smbfs
3、windows磁盘映射 
**四、光盘文件系统**
1、iso9660
**五、分布式文件系统**
略
PS：磁盘分区也是文件，要使用磁盘分区需要将磁盘挂载到系统的一个目录中，这个目录就是该分区的访问入口。
**mke2fs命令** （make ext2 file system）
    作用：安装ex2文件系统
    格式：mke2fs [选项] 分区设备文件
    选项：-t ext3或ext4
【linux中一切皆文件，磁盘分区也是文件，要使用磁盘分区，需要将磁盘挂载到系统的一个目录中，这样，这个目录就是该分区的访问入口】 
PS：有的时候做实验对磁盘进行重复的格式化-挂载的时候，可能出现报错信息，包括挂载的时候提示已经挂载了，格式化的时候提示is apparently in use by the system; will not make a filesystem here!，具体错误信息如下
[root@localhost /]# mount /dev/sda1 /media/
mount: /dev/sda1 already mounted or /media/ busy
[root@localhost /]# mke2fs -t ext4 /dev/sda1
mke2fs 1.41.12 (17-May-2010)
/dev/sda1 is apparently in use by the system; will not make a filesystem here!
可以尝试使用dmset
    dmsetup  manages  logical  devices  that use the device-mapper driver.  Devices are created by loading a table that specifies a target for each sector (512 bytes) in the logical device.
    具体解决步骤如下
    #首先检查dm中记录信息
    [root@localhost /]# dmsetup status
    VolGroup-lv_swap: 0 4063232 linear
    VolGroup-lv_root: 0 36847616 linear
    sda3: 0 1044225 linear
    sda1: 0 2120517 linear
    #移除sda1和3
    [root@localhost /]# dmsetup remove sda1
    [root@localhost /]# dmsetup remove sda3
    #此时的系统就一切正常了
    [root@localhost /]# mke2fs -t ext4 /dev/sda1
**ext系列文件系统的(物理)构成**
block：磁盘块，磁盘存储数据的最小单元（默认4kb）
block bitmap：块位图，作用是实现快速检索出空闲的磁盘块
inode list：inode 列表，这是存放inode 的空间
inode：index inode，索引节点，索引节点其实也是一个小的磁盘空间，一个inode用来存放一个文件，在inode中存放内包括（大小、属主属组、时间戳、连接数、文件所在的磁盘快的编号），但是在inode中，没有文件名。
inode bitmap：inode位图，快速从inode列表中找出一个空闲的inode
磁盘块是用来存储文件中的数据的
    一般文件：磁盘块上所存储的就是文件中的内容
    目录：所存放的是该目录下文件名称列表 
**打开文件(找到文件)**    以打开cat /var/log/message 为例
    1、找到根（根是自引用）
    2、从根所在的磁盘块表中，找出var所在的inode编号
    3、根据var所在的inode编号，就可以找到var所在的磁盘块
    4、因为var是目录，所以var所在的磁盘块中存放的是一张表，在表中记录了log所在的inode
    5、根据log的inode编号，可以找到log目录所在的磁盘块
    6、从log的磁盘块的表中可以找到message文件所在的inode编号
    7、从message的inode中，可以找到该文件的磁盘块，最终读取出文件
**新建文件**    以在/etc/下新建文件zx.txt，大小是10K，为例
    1、扫描inode bitmap，检索出一个空闲的inode 。假设找出的1号inode
    2、找出目录etc所在的磁盘块，在该磁盘块的表中添加一个条目，内容是 zx.txt-->1
        【到此为止，就可以在磁盘上看到该文件，但是文件无法打开】
    3、扫描block bitmap，找出3个空闲磁盘块，然后就可以向磁盘块中写入数据。
ext系列文件文件系统的重要特性
    在向磁盘写入文件的时候，会提前锁定一个连续的空间
    如果后期发现空间不够，那么再锁定一个连续空间
    如果发现空间有剩余，那么释放剩余部分
    【避免磁盘碎片产生】   
**删除文件**     以在/etc/zx.txt为例
    1、找到etc目录所在的磁盘，将表中的索引删除
        【此时从磁盘上，就看不到该文件了】
    2、将1号inode对应的inode bitmap置为零
    3、将zx.txt所占用的磁盘块对应的block bitmap置为0
**复制文件**   以在/etc/zx.txt复制到home下为例
    1、先找出一个空闲inode （假设是6）
    2、找到home所在的磁盘块，在磁盘块的表中添加一个记录 zx.txt -->6
    3、找出空闲磁盘块
    4、将原文件磁盘块上的数据依次复制到新找出的磁盘块上
**剪切文件**  以在/etc/zx.txt剪切到home下为例
    1、找到etc所在的磁盘块，将磁盘块的表中的zx.txt的索引记录剪切下来
    2、找到home所在的磁盘块，在磁盘块的表中粘贴前面所剪切的索引记录
块组（Block Group）
    每个块组中，有独立的inode，位图
超级块（super block）
    记录包括
    1、块组的数量
    2、各个块中的磁盘数量、空闲数量、使用数量
    3、各个块组中的inode数量，空闲数量，使用数量
块组描述表（GDT）
    记录各个块组的范围
BOOT BLOCK
    引导块
    安装双系统的时候，第二个系统的引导程序安装在bootblock中
PS：下面画个图说明下。。。不过画的有点难看。。。将就看吧
![image](https://images2015.cnblogs.com/blog/974779/201606/974779-20160610144634371-669121391.png)
ext系列
    ext2
        可以自动检测出不完整的文件并删除，但是速度非常慢   
    ext3：日志文件系统（journal File System）
        在磁盘上创建一个日志空间，在创建文件的时候，先将文件的名称写入到日志空间，如果文件正常写入成功，那么从日志空间删除文件名称。
        如果在写入过程发生宕机等故障，那么文件名会保留在日志空间中，下次启动的时候，系统会扫描日志空间，将日志空间中的文件删除。
[root@node2 ~]# mke2fs /dev/sdc1
mke2fs 1.41.12 (17-May-2010)
Filesystem label=    标签（卷标）                    *
OS type: Linux       操作系统类型    
Block size=4096 (log=2)   磁盘块的大小，默认是4K     **
Fragment size=4096 (log=2)  偏移大小             
Stride=0 blocks, Stripe width=0 blocks
197600 inodes, 789185 blocks    inode和磁盘块的总量   **
39459 blocks (5.00%) reserved for the super user   给管理员用户预留的磁盘空间大小    ***
First data block=0       起始磁盘块编号  
Maximum filesystem blocks=809500672 
25 block groups
32768 blocks per group, 32768 fragments per group
7904 inodes per group   
Superblock backups stored on blocks:    超级块的备份所在的磁盘块编号     ***
    32768, 98304, 163840, 229376, 294912
Writing inode tables: done       done表示inode表写入成功写入成功                       
Writing superblocks and filesystem accounting information: done 
This filesystem will be automatically checked every 35 mounts or
180 days, whichever comes first.  Use tune2fs -c or -i to override.   
注意：如果是创建ext3文件系统，会多一个信息
Creating journal (16384 blocks): done
    表示创建日志空间成功
**创建文件系统命令**
mke2fs命令
    作用：是创建文件系统
    配置文件：/etc/mke2fs.conf （用来控制默认参数）
    格式：mke2fs [options] /dev/sd#n
    选项：
        -t {ext2|ext3|ext4}：指定新建的文件系统类型
        -j：同 -t ext3，作用创建ext3 文件系统
        -L ###：指定分区的卷标
        -b {1024|2048|4096}：设定磁盘块大小
        -g #：指定每个块组多少个磁盘块
        -G #：块组的总数
        -i bytes-per-inode：指定inode和字节的比例，默认是16384字节对应一个磁盘块
        -I inode-size：
        -U UUID：指定一个文件系统的UUID。  （通常用在磁盘数量较多的情况）
        -N number-of-inodes：指定文件系统中的inode总数
        -m #：指定保留空间的大小 
e2label命令
    作用：查看、修改卷标
    格式：e2label device [newlabel]
例子：
    [root@node2 ~]# e2label /dev/sdc1 myGame     <<<<<< 修改卷标
    [root@node2 ~]# e2label /dev/sdc1            <<<<<< 查看卷标
    myGame
blkid命令
    作用：查看磁盘的id信息和文件系统类型
    格式：blikd DEVICE
    [root@node2 ~]# blkid /dev/sdc1
    /dev/sdc1: LABEL="myGame" UUID="129c8699-9809-4e15-98a1-d232e87fceca" SEC_TYPE="ext2" 【TYPE="ext3"】
    设备文件   卷标            唯一标识                                    安全类型        文件系统类型
dumpe2fs命令
    作用：显示文件系统的分区信息
    选项：
        -h：显示超级块信息
    [root@node2 ~]# dumpe2fs -h /dev/sdc1
    dumpe2fs 1.41.12 (17-May-2010)
    Filesystem volume name:   myGame                         卷标名称
    Last mounted on:          <not available>                 最近一次的挂载点
    Filesystem UUID:          129c8699-9809-4e15-98a1-d232e87fceca
    Filesystem magic number:  0xEF53
    Filesystem revision #:    1 (dynamic)                ？？？？？？？？？？？？？？？
    Filesystem features:      has_journal ext_attr resize_inode dir_index filetype sparse_super large_file
    Filesystem flags:         signed_directory_hash
    Default mount options:    (none)
    Filesystem state:         clean                    文件系统的状态
    Errors behavior:          Continue
    Filesystem OS type:       Linux
    Inode count:              197600                     【这部分内容平时就应该经常关注的】
    Block count:              789185
    Reserved block count:     39459
    Free blocks:              759214
    Free inodes:              197589
    First block:              0
    Block size:               4096
    Fragment size:            4096
    Reserved GDT blocks:      192
    Blocks per group:         32768
    Fragments per group:      32768
    Inodes per group:         7904
    Inode blocks per group:   494
    Filesystem created:       Wed Mar 16 16:40:22 2016
    Last mount time:          n/a
    Last write time:          Wed Mar 16 17:10:07 2016
    Mount count:              0
    Maximum mount count:      21
    Last checked:             Wed Mar 16 16:40:22 2016     上一次检查文件系统时间
    Check interval:           15552000 (6 months)
    Next check after:         Mon Sep 12 16:40:22 2016
    Reserved blocks uid:      0 (user root)
    Reserved blocks gid:      0 (group root)
    First inode:              11
    Inode size:              256
    Required extra isize:     28
    Desired extra isize:      28
    Journal inode:            8
    Default directory hash:   half_md4
    Directory Hash Seed:      93c9bb64-26f5-48b2-b338-454f3474327f
    Journal backup:           inode blocks
    Journal features:         (none)
    Journal size:             64M
    Journal length:           16384
    Journal sequence:         0x00000001
    Journal start:            0
注意：以上内容，必须能够大部分可以读懂
tune2fs命令
    作用：查看、修改文件系统的属性
    可修改：
        预留空间、卷标
    不可修改：
        磁盘块
    选项：
        -l：查看超级块信息，类似于 dumpe2fs -h
        -L ###：修改分区的卷标，类似于 e2label DEVICE [newLabel]
        -m #：修改文件系统的保留空间百分比
        -j：将ext2文件系统升级为ext3。（内部数据不受影响）
fsck命令
    作用：检查、修复文件系统的命令
    选项：
        -t fstype DEVICE: 检查指定的文件系统
        -a：自动修复文件系统
        -r：交互式修复
e2fsck命令   
    作用：修改文件系统
    选项：
        -y：检查过程中，如果需要用户输入Y，则自动输入。
        -f：强制执行检查操作 
**虚拟内存**
    每个程序都使用一个独立的虚拟机内存空间，这个内存空间是物理内存的映射
    x86架构的主机中，每个程序都认为自己独享4G的内存空间，并且认为当前主机中，只有自己一个程序。
交换分区(swap)
    在windows中称为虚拟内存
    指的是在硬盘上规划处一个组成方式类似于内存的空间，当内存空间不足时，将内存中闲置数据暂时移动到交换分钟
    实现的效果就是可以实现内存的过载使用。
注意：
    尽量不要使用swap
    如果发现swap使用频繁，那么就说明需要添加内存了 
free命令
    作用：显示当前内存的使用情况
    格式：free [options]
    选项：
        -m：以MB为单位显示
        -g：以GB为单位显示
[root@node2 ~]# free -m
             total       used       free     shared    buffers     cached
Mem:          1869        290       1578          0         50         92
-/+ buffers/cache:        146       1722      <<<<<<真实内存空间的使用和空闲情况
Swap:         1983          0       1983
缓冲：存放元数据，inode中的数据
缓存: 存放数据
注意：只有缓存和缓冲中的数据可以清空，其他数据不能清空
伪文件相关
    /proc/partitions：记录系统中的分区信息
    /proc/meminfo：内存的使用情况
    /proc/cpuinfo：记录系统中的CPU的详细信息
dd命令
    作用：从底层逐字节复制文件。
    格式：dd if=原文件 of=目的文件 [bs=xK|M|G count=n]
    例子：将/etc/passwd复制到home下叫做a.txt
    dd if=/etc/passwd of=/home/a.txt
    例子：将/etc/passwd的前10K的内容复制到home下
    dd if=/etc/passwd of=/home/a.txt bs=1K count=10
    dd if=/etc/passwd of=/home/a.txt bs=10K count=1
    例子：备份sda的MBR
    dd if=/dev/sda of=/home/mbr.bak bs=512 count=1
    例子：实现硬盘对考（将sda上的数据全部复制到sdb上）
    dd if=/dev/sda of=/dev/sdb
**新建交换分区的流程**
一、新建一个分区
    分区格式id必须是 82
二、安装swap文件系统
    # mkswap -f /dev/sdc2
mkswap命令
    作用：创建swap文件系统
    格式：swap [-f -L 卷标] DEVICE
三、激活swap分区
    # swapon /dev/sdc2
swapon命令
    作用：激活交换分区
swapoff命令
    作用：关闭交换分区
特殊场景：磁盘无剩余空间（无法再分区），但是分区中还有大量剩余空间，此情况下新建交换分区，可以用 【磁盘镜像文件】 来实现
磁盘镜像文件：就是用一个文件，来模拟当中磁盘来使用。   
一、如何创建镜像文件
1、dd命令
2、特殊设备文件
    /dev/zero--泡泡机
案例：在tmp下创建一个1G的文件disk.img   
    # dd if=/dev/zero of=/tmp/disk.img bs=1M count=1024
二、将新建的镜像文件当做磁盘来创建交换分区
1、创建文件系统
# mkswap -f /tmp/disk.img
2、激活
# swapon /tmp/disk.img
/etc/fstab详解   
作用：实现开机自动挂载外部设备或者自动激活交换分区
注释
[root@node2 ~]# cat /etc/fstab
/dev/mapper/vg_zxf-lv_root          /                       ext4            defaults        1 1
UUID=265f1a54-7f15-46               /boot                   ext4            defaults        1 2
/dev/mapper/vg_zxf-lv_swap          swap                    swap            defaults        0 0
tmpfs                               /dev/shm                tmpfs           defaults        0 0
设备文件名称                        挂载点                  文件系统类型    挂载选项        转储频率   检查次序
/dev/sdc1                            /mnt                    ext3            defaults        0 0
注意：交换分区的自动激活稍有特殊
交换分区到底多大？
    物理内存2-4G，swap4G
    物理内存<2G，swap=物理内存*2
    物理内存>8G，swap=4G
mount命令
    作用：挂载外部设备到linux系统
    格式：mount [options] [-t fstype] [-o option] 设备名称 挂载点
用法1：直接执行mount，可以查看当前已经挂载的全部设备
    options
        -a：重读/etc/fstab，将其中的全部内容重新挂载
        -L label：以卷标的方式进行挂载
        -U UUID：以UUDI的方式进行挂载
        -n：在挂载外部设备的时候，不将挂载信息写入/etc/mtab中
        -t fstype：在挂载设备时候，必须指定文件系统类型。
            (在指定muont命令时候，会自动调用blkid来获取文件系统类型)
        -r：只读
        -rw：读写
    -o options
        sync：同步写入
            优点：安全
            缺点：导致系统性能变差
        async：异步写入
            优点：性能好
            缺点：容易丢失数据
        atime：自动更新时间戳
        noatime：不自动更新时间戳
        diratime：
        nodiratime：对目录而言，不自动更新访问时间戳
        defaults：
        exec：
        noexec：禁止分区中的可执行程序自动运行
        owner：允许普通用户挂载该文件系统      【通常不能使用此选项】   
        remount：重新挂载分区，在进行重新挂载的时候，可以指定额外的选项
            remount,rw  重新挂载可读写
            remount,rw,async,noatime,........
        ro：只读文件系统
        rw：读写文件系统
        acl：启动acl功能
挂载命令：mount
卸载命令：umount
fuser命令
    作用：查看指定的目录被哪个进程或者用户所使用
    格式：fuser [选项] 目录
    选项：
        -v：显示目录正被哪个进程所使用
        -k：杀死正在访问某个文件进程/用户
            如果是对挂载点进行操作，需要结合-m
        -m：是专用于挂载点的选项
df命令
    作用：检查分区的使用情况
    格式：df [-h]
PS：ls –ldh /mnt查看目录所在的磁盘块中的数据大小（不能看目录大小）
du命令
    作用：查看目录的大小
    选项：
        -h：
        -s：仅仅显示目标的大小
    组合：
        du -sh DIR
tree命令
    作用：显示目录以及子目录名称
    选项：
        -L n:显示N级目录 
**链接**
    硬链接：
        两个inode指向相同的磁盘块
        特点：
            硬链接和原始文件互为备份（修改一个文件，另一个随着改变）
            删除原始文件，链接文件可以正常使用
            【其实硬链接和原始文件就是同一个文件】
            每做一个硬链接，连接数加1
    软链接
        【同windows中的快捷方式】
        给原始文件创建一个快捷方式
        特点：
            软链接中，存放的是指向原始文件的路径
            软链接闪烁表示链接文件被损坏
            原始文件被删除，软连接文件无法访问
            软链接文件的权限是777,用户真正的权限要参考原始文件
注意：
    硬链接：
        只能对文件创建，不能对目录创建
        目录的连接数默认是2
        硬链接不能跨文件系统
    软链接：
        可作用于目录
        可以跨分区
        大小是所指向的原始文件的路径的字符个数
**linux文件压缩**
1、gzip命令
    后缀：.gz
    格式：gzip file1 file2 ....
    选项：
        -d：解压缩
        -#：指定压缩比，1-9
            gzip -7 passwd
        -c：将压缩结果输出到标准输出（通常用来保留原始文件）
        例子：压缩passwd，要求保留原始文件
        # gzip -c passwd > passwd.gz
    解压缩
    gunzip命令  
    zcat命令在不解压的情况下，查看压缩包中的内容 
2、bzip2命令
    后缀：.bz2
    格式： 同上
    选项：
        -d：解压缩
        -#：指定压缩比，1-9
            gzip -7 passwd
        -k：保留原始文件
    解压缩
    bunzip2   
    bzcat在不解压的情况下，查看压缩包中的内容 
3、xz命令
    后缀：xz
    格式：同上
    选项：
        -d：解压缩
        -#：指定压缩比，1-9
            gzip -7 passwd
        -k：保留原始文件       
    解压
    unxz 
注意：以上三个，只能压缩文件，不能压缩目录           
4、zip命令
    后缀：.zip
    特点：压缩比低，但是可以压缩目录，而且压缩不删除原文件
    【zip压缩文件，在windows和linux都支持】
    格式：zip [选项] xxx.zip file1 file2 .......
    选项：
        -r：递归，压缩目录以及子目录中的全部文件。【压缩目录时候，通常要使用】
    解压命令
    unzip
compress：压缩
decompress：解压缩
uncompress: 解压缩
archive：归档
    将多个文件或目录合并成一个文件，文件大小不变或稍微变大。
归档的命令tar
    后缀：.tar
    格式：tar  [选项]  -f xxxx.tar  file1 file2......
    选项：
        -f xxxx.tar：指定归档文件的文件名
        -c：创建归档
        -x：展开归档
        --xattrs：在创建归档的时候保留文件的扩展属性等。
        -t：在不展开归档的情况下，查看其中的内容
        -p：在创建归档的时候保留文件的权限和属性不变
        --exclude=FILE：在创建归档的时候不对某个文件进行归档
        -j：调用bzip2进行压缩和解压缩
        -z：调用gzip进行压缩和解压缩
        -J：调用xz进行压缩和解压缩
        -v：显示过程信息
        -C
    例子：对home目录创建归档，归档文件名是a.tar
    # tar -cf a.tar /home
    例子：展开归档
    # tar -xf a.tar
    例子：将a.tar使用gzip进行压缩
    # gzip -c a.tar > a.tar.gz
    例子：将a.tar使用xz进行压缩
    # xz -k a.tar
    例子：将a.tar.gz解压缩并展开归档
    # gzip -d a.tar.gz  ==>a.tar
    # tar -xf a.tar     ==>a
    例子：将etc init.d home归档压缩成all.tar.gz
    # tar -zcvf all.tar.gz  etc/ init.d/  home/
    例子：将etc init.d home归档压缩成all.tar.bz2
    # tar -jcvf all.tar.bz2 etc/  init.d/  home/ 
    注意：在创建或者展开归档的时候，可以不指定压缩方式
        如果不指定，那么系统自动根据文件后缀来选择压缩或解压缩方式
    例子：将all.tar.gz展开
    # tar -zxvf all.tar.gz
    也可以
    # tar -xvf all.tar.gz  
   PS：在创建或展开压缩的时候，不建议指定方式，让系统自己选择。
ln命令
    作用：创建软\硬链接
    格式：ln [-s] 原始文件 目的文件
    选项：
        -s：创建软链接
RAID整列
    Redundant Ayyays of Inexpensive Disk 廉价冗余磁盘阵列
    Redundant Ayyays of Independent Disk 独立冗余磁盘阵列
RAID
    将多个物理磁盘组合成逻辑磁盘提供给用户，并提升整体读写性能。 
**RAID级别 **
（级别仅仅表示磁盘的组织方式不同）
0：条带
    读写提升：读、写
    冗余能力：无
    磁盘数量：>=2
    磁盘使用率：100%
1：镜像
    读写提升：无
    冗余能力：有
    磁盘数量：>=2
    磁盘使用率：50%
3：校验码
    读写提升：读、写
    冗余能力：有
    最多损坏磁盘：1个
    磁盘数量：>=3
    磁盘使用率：(n-1)/n
    【使用固定的磁盘存放校验码】
5：校验码
    读写提升：读、写
    冗余能力：有
    最多损坏磁盘：1个
    磁盘数量：>=3
    磁盘使用率：(n-1)/n       
    【多个磁盘轮流存放校验码】
10：   
    读写提升：读、写
    冗余能力：有
    磁盘数量：>=4
    磁盘使用率：50%       
01：   
    读写提升：读、写
    冗余能力：有
    磁盘数量：>=4
    磁盘使用率：50%   
50:
    读写提升：读、写
    冗余能力：有
    磁盘数量：>=6
    磁盘使用率：(n-2)/n       
jobd   
    读写提升：无
    冗余能力：无
    磁盘数量：不限制
    磁盘使用率：100%
    【通过jobd可以将多个物理磁盘的空间整合成一个逻辑磁盘】
RAID的实现
    硬RAID
        集成
        独立
    软RAID
        使用CPU模拟生成一个RAID控制器
        会导致CPU更为繁忙
        要借助于MD模块
**md模块**
    multi disk
    作用：将多个物理磁盘整合成逻辑磁盘
命令实现
    mdadm
    特点：模式化的命令
1、创建模式
    作用：创建阵列
    选项: -C
    模式中的子选项：
        -l #：指定阵列的级别
        -n #：指定创建阵列所使用的磁盘数量
        -a {yes|no}：yes,表示自动创建需要的设备文件
        【阵列的设备文件是/dev/md#】
        -c #：指定所切割的数据块的大小，默认是512K，修改必须是512的整数倍
        【chunk】
        -x #：指定空闲(保留)磁盘的个数
2、管理模式
    操作
        添加：--add -a
        删除：--del -d
3、监控模式   
    选项：-F
4、增长模式
    选项：-G
5、装配模式
    选项：-A
【如果不指定，那么默认就是管理模式】 
**案例1：创建一个2G的raid0**
    【既可以使用磁盘，也可以用分区来实现，此案例用分区实现】
1、创建两个1G的分区
    注意：
        1、分区类型必须修改为fd
        2、不能在分区上安装文件系统
2、创建raid0   
    格式：mkadm -C /dev/DeviceName -a yes -l LEVEL -n 磁盘数量 磁盘1 磁盘2 ....
    # mdadm -c /dev/md0 -a yes -l 0 -n 2 /dev/sdc{1,2}
3、查看当前系统中的raid信息
    # cat /proc/mdstat
4、在raid0上安装文件系统
    # mke2fs /dev/md0
5、挂载
    # mkdir /myraid0
    # mount /dev/md0 /myraid0
**案例2：删除raid0**
1、查看raid0的基本信息   
    [root@test mkraid0]# cat /proc/mdstat
    Personalities : [raid0]
    md0 : active raid0 sdc2[1] sdc1[0]
          2119680 blocks super 1.2 512k chunks
2、卸载此raid0
    # umount /dev/md0
3、停止此RAID0
    # mdadm -S /dev/md0
4、清除此raid的超级块中的数据   
    # mdadm --zero-superblock /dev/sdc1 /dev/sdc2
**案例3：创建一个10G的raid1**
    【使用物理磁盘来实现，每个物理磁盘大小是10G】
1、使用sdb和sdc创建raid1
    # mdadm -C /dev/md9 -a yes -l 1 -n 2 /dev/sdc /dev/sdb
    【软raid不能当做启动分区来使用】
2、检查raid1的同步情况
    # cat /proc/mdstat
3、检查raid的详细信息
    # mdadm -D /dev/md9
    /dev/md9:
            Version : 1.2
      Creation Time : Wed Mar 23 17:23:44 2016
         Raid Level : raid1
         Array Size : 10477440 (9.99 GiB 10.73 GB)
      Used Dev Size : 10477440 (9.99 GiB 10.73 GB)
       Raid Devices : 2
      Total Devices : 2
        Persistence : Superblock is persistent
        Update Time : Wed Mar 23 17:24:37 2016
              State : clean
     Active Devices : 2
    Working Devices : 2
     Failed Devices : 0
      Spare Devices : 0
               Name : node2.openstack.com:9  (local to host node2.openstack.com)
               UUID : 766ecba3:768f11af:590df855:a3cfdc8f
             Events : 17
        Number   Major   Minor   RaidDevice State
           0       8       32        0      active sync   /dev/sdc
           1       8       16        1      active sync   /dev/sdb
4、安装文件系统
    # mke2fs /dev/md9
5、挂载
    # mount /dev/md9 /mnt
6、模拟raid1中一个磁盘损坏的情况【模拟sdb损坏】
    # mdadm /dev/md9 --fail /dev/sdb
    建议：此时检查md9的详细信息
    方式：mdadm -D /dev/md9
7、修复raid1
    (1)向raid1添加新磁盘
    # mdadm /dev/md9 -a /dev/sdd
    建议：此时检查md9的详细信息
    方式：mdadm -D /dev/md9
8、删除sdb
    # mdadm /dev/md9 -r /dev/sdb
**阵列的停止和启动**
1、停止
    # mdadm -S /dev/md9
2、停止后再次启动raid
    # mdadm -A /dev/md9 /dev/sdc /dev/sdd
mdadm -D --scan
    可以显示系统中的raid阵列以及所使用的磁盘信息（uuid）
mdadm的配置文件，记录系统raid信息，可以用来实现快速启动raid阵列
/etc/mdadm.conf
【此文件默认不存在】
用重定向的方式，可以将系统的阵列信息保存到mdadm.conf中
mdadm -D --scan > /etc/mdadm.conf
以后再启动阵列，直接用阵列设备文件就可以，如下
#mdadm -A /dev/md9    
**lvm **
    DM:Device Mapper 设备映射
对比MD和DM
    MD:将多个物理磁盘整合成一个逻辑磁盘
    DM:将一个或多个物理磁盘整合成一个或多个逻辑磁盘
DM的功能
    LVM2-->动态磁盘
    快照
    多路径
DM的核心原理
重要名词
    物理卷：Physical Volume PV,可以理解物理卷就是物理磁盘
    卷组：Volume Group VG,将所有物理磁盘的存储能力抽象成一个大的存储空间
    逻辑卷：Logical Volume LV,从卷组中取出一部分存储空间构成一个分区
    物理盘区：Physical Extend PE,只有将物理磁盘加入到卷组以后，才有PE的概念。
              PE可以理解为物理卷中的一个一个存储单元。【只有进入卷组才是PE，否则不是】
    逻辑盘区：Logical Extend LE.
              其实pe和le是相同的东西，只是在卷组中叫做PE，在逻辑卷中叫做LE
**动态磁盘的相关操作**
第一类：管理物理卷的命令(pv)
    创建：pvcreate
    删除：pvremove
    扫描：pvscan
    查看：pvs，pvdisplay
    移动pv上的数据：pvmove
        (通常用在要卸载的磁盘上)
第二类：管理卷组的命令(vg)
    创建：vgcreate
        -s #K|M|G|T|E：指定PE的大小，默认是4M
    删除：vgremove
    扫描：vgscan
    查看：vgs，vgdisplay
    扩大：vgextend  （其实就是向卷组中添加新的物理卷）
    缩小：vgreduce
第三类：管理逻辑卷的命令(lv)   
    创建：lvcreate
        -n xx：指定逻辑卷的名称
        -L #K|M|G|T|E：指定逻辑卷的大小   （其实就是指定分区的大小）
    删除：lvremove
    扫描：lvscan
    显示：lvs，lvdisplay
    扩大：lvextend  
    缩小：lvreduce
sdf 3 ，sdg 5 ，sdj 10 ，sdi 15， sdh 20   
**案例1：将以上硬盘做成逻辑卷**
1、创建逻辑卷
    # pvcreate /dev/sdf /dev/sdi......
2、查看当前系统中的逻辑卷
    # pvs
    # pvscan
    # pvdisplay
**例子2：前面的基础上，创建卷组**
    格式：
1、加入卷组   
    # vgcreate myvg001 /dev/sd{f,h,i}
2、查看
    # vgs
    # vgdisplay
    # vgscan
**案例3：将前面卷组中的sdh从卷组中删除**
1、移走sdh上的数据【必须执行】
    # pvmove /dev/sdh
2、从vg中剔除sdh
    # vgreduce myvg001 /dev/sdh
3、新建另一个卷组，名称为newvg，PE为8M
    # vgcraete newvg -s 8M /dev/sd{g,h}
**案例4：向newvg添加一个新的物理卷**
    # vgextend newvg /dev/sdj
**案例5：在前面的基础上，创建一个500M的逻辑卷，名称是mylv**
    # lvcreate -n mylv -L 500M newvg
    # lvs
    # lvdisplay
逻辑卷的设备文件名称
    /dev/卷组名/逻辑卷名   （只是链接） 
    /dev/mapper/卷组名-逻辑卷名
**案例6：使用此逻辑卷**
1、安装文件系统
    # mke2fs /dev/newvg/mylv
2、挂载   
    # mount /dev/newvg/mylv /media
**调整逻辑卷的大小**
扩大物理边界的命令
    lvextend
        -L [+]#：
            -L +5G: 在原来的基础上扩大5G的空间
            -L 5G：扩展到5G
扩大(调整)逻辑边界命令
    resize2fs
    格式：resize2fs lv的设备文件 NG
    选项：
        -p：将全部剩余的卷组都分给逻辑卷
        resize2fs /dev/newvg/mylv 100G  将逻辑分区调整为100G
        resize2fs -p /dev/newvg/mylv    有多少给多少
缩小逻辑分区
    resize2fs
缩小物理分区
    lvreduce [-]
**案例7：将前面的500M的逻辑卷扩展到800M**
1、检查卷组的剩余空间
    # vgs
2、调整物理边界
    # lvextend  -L 800M /dev/mapper/newvg-mylv
3、调整逻辑编辑
    # umount /dev/mapper/newvg-mylv
    # e2fsck -f /dev/mapper/newvg-mylv
    # resize2fs /dev/mapper/newvg-mylv 800M
    # mount /dev/mapper/newvg-mylv /media
PS：调大空间操作， 必须先卸载所挂载的逻辑卷 ，在系统异常的情况下，需要检查逻辑卷的文件文件   
**案例8：将前面的逻辑卷缩减为400M**
1、确认磁盘使用情况
    # df -h
2、卸载逻辑卷
    # umount /dev/mapper/newvg-mylv
3、检查文件系统
    # e2fsck -f /dev/mapper/newvg-mylv
4、缩减逻辑边界
    # resize2fs /dev/mapper/newvg-mylv 400M
5、缩减物理边界
    # lvreduce -L 400M /dev/mapper/newvg-mylv
6、重新挂载
**快照卷**
    可以对系统中的重要文件进行保护。
注意：
    1、新的快照卷当中是空
    2、一旦出现快照卷满了，那么快照卷自动自毁
    3、快照卷中的文件是只读状态的
创建快照卷的命令
    lvcreate
        -s：创建快照卷的专用选项
        -p r|w：设置快照卷中的文件的属性  【默认是只读】
    创建快照卷的命令格式：
    例子：创建一个100M的快照卷，快照卷的名称是myss，这个快照卷是针对/dev/mapper/newvg-mylv
    # lvcreate -s  -L 100M -n myss -p r /dev/mapper/newvg-mylv
