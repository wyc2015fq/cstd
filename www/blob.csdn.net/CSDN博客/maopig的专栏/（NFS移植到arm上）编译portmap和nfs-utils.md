# （NFS移植到arm上）编译portmap和nfs-utils - maopig的专栏 - CSDN博客
2017年08月15日 10:22:58[maopig](https://me.csdn.net/maopig)阅读数：1415
为了在播放机上实现NFS服务器的功能，我们已经在uClibc中打开了完整RPC支持，并且在新编译的内核中打开了NFS服务器支持。此外还有两个软件包也是提供NFS服务所必需的：[portmap](http://en.wikipedia.org/wiki/Portmap)和[nfs-utils](http://nfs.sourceforge.net/)。portmap为RPC程序提供端口映射服务，nfs-utils则是使用内核NFS服务器的支持程序。
编译portmap
1. 下载portmap_5beta: [ftp://ftp.porcupine.org/pub/security/portmap_5beta.tar.gz](ftp://ftp.porcupine.org/pub/security/portmap_5beta.tar.gz)
2. 打这个补丁: [portmap_5beta.patch.zip](http://www.cnitblog.com/Files/gouzhuang/portmap_5beta.patch.zip) （补丁来自buildroot-2009.11，我只是把多个补丁合并成一个）
3. $ make CC=mipsel-linux-gcc
4. $ mipsel-linux-strip portmap
编译nfs-utils
1. 下载nfs-utils-1.1.1: http://nchc.dl.sourceforge.net/project/nfs/nfs-utils/1.1.1/nfs-utils-1.1.1.tar.gz
2. 打这个补丁: [nfs-utils-1.1.1-uclibc.patch.zip](http://www.cnitblog.com/Files/gouzhuang/nfs-utils-1.1.1-uclibc.patch.zip) （在网上找到的，来源记不清了，我稍加了修改）
3. 运行配置脚本：
1./configure --build=i686-linux --host=mipsel-linux --disable-nfsv4 --disable-gss --disable-uuid --disable-mount --without-tcp-wrappers --with-gnu-ld CC=mipsel-linux-gcc CPP=mipsel-linux-cpp AR=mipsel-linux-ar STRIP=mipsel-linux-strip RANLIB=mipsel-linux-ranlib LD=mipsel-linux-ld
4. $ make
5. 安装到/home/user/dist/nfs-utils目录
1$ make DESTDIR=/home/user/dist/nfs-utils install-strip
在制作固件时，我们只需要几个编译好的程序：portmap, rpc.statd, rpc.nfsd, rpc.mountd, exportfs。其中portmap 放到/sbin下，其余的放到/usr/sbin下。
此外还需要一个NFS服务启动脚本S60nfs，放在/etc/init.d目录下。下载脚本[S60nfs.zip](http://www.cnitblog.com/Files/gouzhuang/S60nfs.zip) (来自buildroot，我把portmap的启动加进去了）
 1#!/bin/sh                                
 2#                                        
 3# nfs           This shell script takes care of starting and stopping
 4#               the NFS services. Stolen from RedHat FC5.            
 5
 6[ -x /sbin/portmap ] || exit 0
 7[ -x /usr/sbin/rpc.statd ] || exit 0
 8[ -x /usr/sbin/rpc.nfsd ] || exit 0
 9[ -x /usr/sbin/rpc.mountd ] || exit 0
10[ -x /usr/sbin/exportfs ] || exit 0
11
12# Don't fail if /etc/exports doesn't exist; create a bare-bones version and continue.
13[ -r /etc/exports ] || \                                                             
14    { touch /etc/exports && chmod u+rw,g+r,o+r /etc/exports ; } || \                 
15    { echo "/etc/exports does not exist" ; exit 0 ; }                                
16
17# The /var/lib/nfs directory is actually on a tmpfs filesystem.                      
18mkdir -p /var/lib/nfs/sm                                                             
19mkdir -p /var/lib/nfs/sm.bak                                                         
20touch /var/lib/nfs/etab                                                              
21touch /var/lib/nfs/rmtab                                                             
22touch /var/lib/nfs/state                                                             
23touch /var/lib/nfs/xtab                                                              
24
25start() {
26        # Start daemons.
27        echo -n "Starting port mapper: "
28        portmap                         
29        echo "done"
30
31        echo -n "Starting NFS statd: "
32        rpc.statd                     
33        touch /var/lock/subsys/nfslock
34        echo "done"
35
36        echo -n "Starting NFS services: "
37/usr/sbin/exportfs -r            
38        rpc.statd                        
39        echo "done"
40
41        echo -n "Starting NFS daemon: "
42        rpc.nfsd 2
43        echo "done"
44
45        echo -n "Starting NFS mountd: "
46        rpc.mountd                     
47        echo "done"
48        touch /var/lock/subsys/nfs     
49}                                      
50
51stop() {
52        # Stop daemons.
53        echo -n "Shutting down NFS mountd: "
54        killall -q rpc.mountd               
55        echo "done"
56
57        echo "Shutting down NFS daemon: "
58        kill -9 `pidof nfsd` 2>/dev/null
59        echo "done"
60
61        echo -n "Shutting down NFS services: "
62/usr/sbin/exportfs -au                
63        rm -f /var/lock/subsys/nfs            
64        killall -q rpc.statd                  
65        echo "done"
66
67        echo -n "Stopping NFS statd: "
68        killall -q rpc.statd
69        echo "done"
70        rm -f /var/lock/subsys/nfslock
71
72        echo -n "Stopping port mapper: "
73        killall -q portmap
74        echo "done"
75}
76
77# See how we were called.
78case"$1"in
79  start)
80        start
81        ;;
82  stop)
83        stop
84        ;;
85  restart)
86        stop
87        start
88        ;;
89  reload)
90/usr/sbin/exportfs -r
91        touch /var/lock/subsys/nfs
92        ;;
93*)
94        echo "Usage: nfs {start|stop|reload}"
95        exit 1
96esac
97
98exit 0
