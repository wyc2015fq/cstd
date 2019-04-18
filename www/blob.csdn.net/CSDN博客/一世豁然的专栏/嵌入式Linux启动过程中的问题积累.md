# 嵌入式Linux启动过程中的问题积累 - 一世豁然的专栏 - CSDN博客





2015年07月01日 14:21:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：524标签：[方法论																[Linux启动](https://so.csdn.net/so/search/s.do?q=Linux启动&t=blog)](https://so.csdn.net/so/search/s.do?q=方法论&t=blog)
个人分类：[方法论](https://blog.csdn.net/Explorer_day/article/category/3135631)








本博客转载于：[http://www.cnblogs.com/yyangblog/archive/2011/06/14/2080773.html](http://www.cnblogs.com/yyangblog/archive/2011/06/14/2080773.html)




1.Bad Magic Number 
## Booting image at 33000000 ...
Bad Magic Number
OMAP5912 OSK # （tftp下载好kernel的uImage后就停止在这，不能启动kernel）

问题原因：启动参数设置错误,0x30000000处不可以执行。
有
的开发板sdram不是在0x33000000，所以不能把kernel
uImage下载到0x33000000中运行。如我之前的bootcmd参数为:setenv bootcmd tftpboot 33000000
uImage\; bootm 33000000。但板子Omap5912的sdram地址在0x100000000，将参数改为setenv
bootcmd tftpboot 10000000 uImage\; bootm 10000000后便可以启动kernel了。

2.启动停止在"Starting kernel ..."
TFTP from server 192.168.167.170; our IP address is 192.168.167.15
Filename 'uImage'
Load address: 0x10000000
Loading: #################################################################
         #################################################################
done
Bytes transferred = 2025908 (1ee9b4 hex)
## Booting image at 10000000 ...
   Image Name:   Linux-2.6.18-mh8_pro500-versatil
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    2025844 Bytes =  1.9 MB
   Load Address: 30008000
   Entry Point:  30008000
   Verifying Checksum ... OK
OK
Starting kernel ...   （卡在这里）

问题原因：多半是kernel没编译成功。
确认configure参数是否配置正确，是否选择了正确的目标编译平台，如smdk2410等。

3.不能启动kernel
Starting kernel ...
Uncompressing Linux.........................................................
.................... done, booting the kernel.

问题原因：可能是Bootargs参数设置错误,确认bootargs设置是否正确。

4.不能挂载nfs
eth0: link up
IP-Config: Complete:
      device=eth0, addr=192.168.167.15, mask=255.255.255.0, gw=192.168.167.254,
     host=192.168.167.15, domain=, nis-domain=(none),
     bootserver=192.168.167.170, rootserver=192.168.167.170, rootpath=
Looking up port of RPC 100003/2 on 192.168.167.170
Root-NFS: Unable to get nfsd port number from server, using default
Looking up port of RPC 100005/1 on 192.168.167.170
Root-NFS: Unable to get mountd port number from server, using default
mount: server 192.168.167.170 not responding, timed out
Root-NFS: Server returned error -5 while mounting /work/nfs/rootfs_bluetooth_omap
VFS: Unable to mount root fs via NFS, trying floppy.
VFS: Cannot open root device "nfs" or unknown-block(2,0)
Please append a correct "root=" boot option
Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(2,0)

问题原因：这种情况通常是nfs配置问题。
确
认uboot的bootargs参数里和nfs相关的ip地址信息设置是否正确，以及Host机/etc/exports配置无误，重起nfs服务,重新
尝试连接。另外还需要注意bootargs内console和mem两个参数的设置。kernel2.6后console最好设置为
ttySAC0，mem也要根据开发板实际情况设置正确。

5.文件系统不能启动问题
eth0: link up
IP-Config: Complete:
      device=eth0, addr=192.168.167.15, mask=255.255.255.0, gw=192.168.167.254,
     host=192.168.167.15, domain=, nis-domain=(none),
     bootserver=192.168.167.170, rootserver=192.168.167.170, rootpath=
Looking up port of RPC 100003/2 on 192.168.167.170
Looking up port of RPC 100005/1 on 192.168.167.170
VFS: Mounted root (nfs filesystem).
Freeing init memory: 128K
/sbin/initKernel panic - not syncing: Attempted to kill init!

问题原因：制作的文件系统缺少运行busybox所需的libcrypt.so库，新版本会有缺库提示，老版本(1.60)没有。
注：运行一个busybox文件系统至少需要如下几个库：
ld-linux.so.x
libc.so.6
libcrypt.so.x
较新版本的busybox可能还需要
libm.so.6
libgcc_s.so.x
(x为版本号）

6.文件系统不能启动问题2
eth0: link up
IP-Config: Complete:
      device=eth0, addr=192.168.167.15, mask=255.255.255.0, gw=192.168.167.254,
     host=192.168.167.15, domain=, nis-domain=(none),
     bootserver=192.168.167.170, rootserver=192.168.167.170, rootpath=
Looking up port of RPC 100003/2 on 192.168.167.170
Looking up port of RPC 100005/1 on 192.168.167.170
VFS: Mounted root (nfs filesystem).
Freeing init memory: 128K
Kernel panic - not syncing: No init found.  Try passing init= option to kernel.

问题原因：对比一个可用的文件系统后发现，缺少了ld-linux.so.x库，文件系统里只有ld-linux.so.x的连接文件，少拷了库文件。

8.不能获得帐户UID信息
Could not get password database information for UID of current process: User "???" unknown or no memory to allocate password entry 
Unknown username "root" in message bus configuration file
Could not get password database information for UID of current process: User "???" unknown or no memory to allocate password entry
Failed to start message bus: Could not get UID and GID for username "root"

问题原因：
情况一：系统帐户验证出现问题.怀疑是调用getuid、getguid时并没有返回正确值,可能是缺少帐户验证相关库,实际排查后发现，缺少libnss_files库。拷贝交叉编译器的libnss_files库到文件系统后，启动文件系统成功。
情况二：系统没有root帐号。可以由whoami命令看出。
手动创建帐号。
#vi /etc/passwd 
root:x:0:0:root:/root:/bin/sh
kyo:x:500:500:kyo:/home/kyo:/bin/bash
添加组
#vi group
root:x:0:root

9.
Freeing init memory: 128K
init started: BusyBox v1.6.1 (2007-08-27 14:33:15 CST) multi-call binary
starting pid 834, tty '': '/etc/init.d/rcS'
Cannot run '/etc/init.d/rcS': No such file or directory
Please press Enter to activate this console. 
发现没有/etc/init.d/rcS文件系统一样能正常启动。看来rcS只是用来设置一些随机启动的参数，对文件能否正常运行关系不大。
注：这个不是错误,是偶然发现! :) 





