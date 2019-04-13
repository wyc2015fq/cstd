
# CentOS 配置swap交换区的方法 - 阳光岛主 - CSDN博客

2016年12月30日 14:35:51[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：15625


CentOS 作为linux世界里最稳定的服务器版本，rhas5一直有很大的应用面，之前一直关注的是freebsd，因为应用的需要，特别在配合mysql和oracle上red hat有着太多优势。
[米扑科技](http://mimvp.com)，作为一家快速发展的创业企业，购买了许多阿里云和亚马逊云，因此会配置swap交换区，具体方法如下。
用CentOS来代替red hat as的使用，因为CentOS5是利用rhas5的源码，去掉redhat的商标、图标之后编译发行的，是最一个源码编译版本，我用的是CentOS5.2
在安装centos 5.2的时候，忘设置swap交换分区，在内存小的情况下，增大swap的作用比较有效的。
配置swap之前，通过 free 和 df 查看 swap 和 磁盘空间情况，目的是通过配置swap后比较变化。
**free -m**
[root@ithomer ~]\#free -m
total       used       free     shared    buffers     cached
Mem:          1008        955         53          0         17        168
-/+ buffers/cache:        769        239
Swap:            0          0          0
**df -h**
[root@ithomer ~]\#df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/xvda1       20G   15G4.2G79% /
tmpfs           505M     0  505M   0% /dev/shm
/dev/xvdb       9.9G  5.1G  4.4G  54% /home/data
**第一步 创建文件块**
[root@ithomer ~]\#cd /var(进入一个文件夹)
[root@ithomer var]\#dd if=/dev/zero of=swapfile bs=1024 count=1024000（创建1GB的swap ,一般是内存的两倍）
1024000+0 records in
1024000+0 records out
1048576000 bytes (1.0 GB) copied, 23.3843 s, 44.8 MB/s
[root@ithomer var]\#/sbin/mkswap swapfile（创建swap文件，实际上dd已创建，此处不必再创建）
mkswap: swapfile: warning: don’t erase bootbits sectors
on whole disk. Use -f to force.
Setting up swapspace version 1, size = 1023996 KiB
no label, UUID=e878f535-ea6f-4516-8a34-f1458951aa36
[root@ithomer var]\#ll swapfile
-rw-r–r– 1 root root 1048576000 11月 27 00:55 swapfile
[root@ithomer var]\#/sbin/swapon swapfile(激活swap文件)
[root@ithomer var]\#/sbin/swapon -s(检查swap是否正确)
Filename                Type        Size    Used    Priority
/var/swapfile                           file        1023992    0    -1
[root@ithomer var]\#vim  /etc/fstab（加到fstab文件中让系统引导时自动启动）
在末尾增加以下内容：
/var/swapfile swap swap defaults 0 0
[root@ithomer var]\#reboot
重启之后，top命令查看，有swap分区的大小了
![centos-configure-swap-on-swap-method](http://blog.mimvp.com/wp-content/uploads/2014/11/centos-configure-swap-on-swap-method.png)
**free -m**
[root@ithomer ~]\#free -m
total       used       free     shared    buffers     cached
Mem:          1008        528        480          0         25        128
-/+ buffers/cache:        375        633
Swap:999          0        999
**df -h**
[root@ithomer ~]\#df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/xvda1       20G   16G3.2G84% /
tmpfs           505M     0  505M   0% /dev/shm
/dev/xvdb       9.9G  5.1G  4.4G  54% /home/data
根据上面 free 和 df 与配置 swap 之前比较，发现swap由0增加到了999(大约为1GB），而系统主盘（/dev/xvda1）由**4.2G**减小到了**3.2G**大约为1GB，即swap的空间来源于系统主盘（/dev/xvda1）
**删除swapfile文件**
$rm -f swapfile
rm: cannot remove ‘swapfile’: Permission denied
删除swapfile文件时，提示权限禁止，原因是swapfile已挂载，正在使用中，需要先卸载，注释掉\#
\#/var/swapfile swap     swap     defaults     0   0
\#/var/swapfile2 swap     swap     defaults     0   0
然后，sudo  -s  切换到root权限，rm -f swapfile删除成功
**/etc/fstab 文件参数**
cat /etc/fstab
LABEL=/     /           ext4    defaults,noatime  1   1
tmpfs       /dev/shm    tmpfs   defaults        0   0
devpts      /dev/pts    devpts  gid=5,mode=620  0   0
sysfs       /sys        sysfs   defaults        0   0
proc        /proc       proc    defaults        0   0
/var/swapfile swap     swap     defaults     0   0
/var/swapfile2 swap     swap     defaults     0   0
df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/xvda1       16G  6.2G  9.5G  40% /
devtmpfs        489M   56K  489M   1% /dev
tmpfs           498M     0  498M   0% /dev/shm
fstab中存放了与分区有关的重要信息，其中每一行为一个分区记录，每一行又可分为六个部份：
1. 第一项是您想要mount的储存装置的实体位置，如/或/var/swapfile2
2. 第二项就是您想要将其加入至哪个目录位置，如/home或/,这其实就是在安装时提示的挂入点
3. 第三项就是所谓的local filesystem，其包含了以下格式：如ext、ext2、msdos、iso9660、nfs、swap等，或如ext2，可以参见/prco /filesystems说明
4. 第四项就是您mount时，所要设定的状态，如ro（只读）或defaults（包括了其它参数如rw、suid、exec、auto、nouser、 async），可以参见mount nfs
5. 第五项是提供DUMP功能，在系统DUMP时是否需要备份BACKUP的标志位，其内定值是0
6. 第六项是设定此filesystem是否要在开机时做check的动作，除了root的filesystem其必要的check为1之外，其它皆可视需要 设定，内定值是0

**问题与解决**
**1、格式化及启动swap文件**
接下来执行mkswap命令,将myswap文件格式化成s文件系统,系统才能使用,切换到/tmp目录,并执行以下命令:
\# mkswap swapfree (将文件格式化为swap文件格式)
setting up swapspace version 1 , size = 262144 KB
\# swapon /tmp/swapfree (\#启动swap分区)
要停止使用新创建的swap文件,只要执行 swapoff /tmp/swapfree命令即可,如果swap交换文件不再使用，可以删除此文件。
**2、检查swap**
\#swapon -s
**3、 开机时自动启动新添加的swap分区**
如果每次开机后都要执行swapon命令启动swap分区或者文件,这太麻烦了.这时可以利用文字编辑器在/etc/fstab文件加一行,好让开机时自动启动swap分区及文件:
/dec/hdb5 swap swap defaults 0 0 (开机时启动此swap分区)
/tmp/swapfree swap swap defaults 0 0 (开机时启动此swap文件) .
swap空间大小：
通常情况下，Swap空间应大于或等于物理内存的大小，最小不应小于64M，通常Swap空间的大小应是物理内存的2-2.5倍。但根据不同的应用，应有不同的配置：如果是小的桌面系统，则只需要较小的Swap空间，而大的服务器系统则视情况不同需要不同大小的Swap空间。特别是数据库服务器和Web服务器，随着访问量的增加，对Swap空间的要求也会增加，具体配置参见各服务器产品的说明。
swap数量：
Swap分区的数量对性能也有很大的影响。因为Swap交换的操作是磁盘IO的操作，如果有多个Swap交换区，Swap空间的分配会以轮流的方式操作于所有的Swap，这样会大大均衡IO的负载，加快Swap交换的速度。如果只有一个交换区，所有的交换操作会使交换区变得很忙，使系统大多数时间处于等待状态，效率很低。用性能监视工具就会发现，此时的CPU并不很忙，而系统却慢。这说明，瓶颈在IO上，依靠提高CPU的速度是解决不了问题的。



