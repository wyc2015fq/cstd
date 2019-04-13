
# 关于NFS服务器的原理总结和mount挂载 - 高科的专栏 - CSDN博客

2015年01月26日 16:54:57[高科](https://me.csdn.net/pbymw8iwm)阅读数：13267


NFS 是Network File System的缩写，即网络文件系统。一种使用于分散式文件系统的协定，由Sun公司开发，于1984年向外公布。功能是通过网络让不同的机器、不同的操作系统能够彼此分享个别的数据，让应用程序在客户端通过网络访问位于服务器磁盘中的数据，是在类Unix系统间实现磁盘文件共享的一种方法。
NFS 的基本原则是“容许不同的客户端及服务端通过一组RPC分享相同的文件系统”，它是独立于操作系统，容许不同硬件及操作系统的系统共同进行文件的分享。
NFS在文件传送或信息传送过程中依赖于RPC协议。RPC，远程过程调用 (Remote Procedure Call) 是能使客户端执行其他系统中程序的一种机制。NFS本身是没有提供信息传输的协议和功能的，但NFS却能让我们通过网络进行资料的分享，这是因为NFS使用了一些其它的传输协议。而这些传输协议用到这个RPC功能的。可以说NFS本身就是使用RPC的一个程序。或者说NFS也是一个RPC SERVER。所以只要用到NFS的地方都要启动RPC服务，不论是NFS SERVER或者NFS CLIENT。这样SERVER和CLIENT才能通过RPC来实现PROGRAM
 PORT的对应。可以这么理解RPC和NFS的关系：NFS是一个文件系统，而RPC是负责负责信息的传输。
其中，NFS服务器设定好了分享出来的/home/shares（可以是其他目录）这个目录后，其他的客户端就可以将这个目录挂载到自己系统上的/mnt/nfs挂载点（挂载点可以自定），只要在PC1系统中进入/mnt/nfs目录内，就可以看到NFS服务器系统内的/home/shares目录下的所有数据（要有相应的权限），/home/shares就好像自己PC中的一个分区（但不占用磁盘空间）。用户可以使用cp、cd、 mv、rm等磁盘或文件相关的指令进行操作。虽然NFS有属于自己的协议和端口号，但是在传送数据或其他相关信息时，NFS使用的是远程过程调用（Remote
 Procedure Call，RPC）协议来协助NFS本身的运作。RPC即远程进程调用。当使用某些服务来进行远程联机的时候，主机的IP地址、服务的端口号及对应到的服务PID等信息都需要管理与对应，管理端口的对应与服务相关性的工作就是RPC的任务。
NFS本身的服务并没有提供数据传递的协议，因此，NFS使用RPC来实现网络传输功能。NFS本身就是一个使用RPC的程序，换句话说，NFS是RPC服务器。当然，不但运行NFS的服务器需要启动RPC的服务，要挂载NFS文件系统的客户端，也需要同步启动RPC，这样服务器端与客户端才能由RPC的协议进程序端口的对应，Linux系统默认时启动这一服务。
NFS的安装是非常简单的，只需要两个软件包即可，而且在通常情况下，是作为系统的默认包安装的。
nfs-utils-* ：包括基本的NFS命令与监控程序
portmap-* ：支持安全NFS RPC服务的连接
1、包的安装
yum install -y portmap
yum install -y nfs-utils
通过yum或者rpm安装完portmap后，发现执行命令service portmap start时报如下错误：portmap: unrecognized service。
CentOS6（Linux Kernel 2.6.32）中，portmap已经被rpcbind代替了，仔细查看安装信息就会发现，执行命令yum install portmap时安装的就是rpcbind。

安装好之后然后就是修改配置文件
服务器端配置文件/etc/exports：指定要共享的目录及权限，例如：
/home/work 192.168.11.*（rw,sync,root_squash）
/home  192.168.1.105 (rw,sync)
/public  * (rw,sync)
配置文件每行分为两段：第一段为共享的目录，使用绝对路径，第二段为客户端地址及权限。
地址可以使用完整IP或网段，例如10.0.0.8或10.0.0.0/24，10.0.0.0/255.255.255.0当然也可以地址可以使用主机名，DNS解析的和本地/etc/hosts解析的都行，支持通配符，例如：*.chengyongxu.com
权限有：
rw：read-write，可读写；    注意，仅仅这里设置成读写客户端还是不能正常写入，还要正确地设置共享目录的权限，参考问题7
ro：read-only，只读；
sync：文件同时写入硬盘和内存；
async：文件暂存于内存，而不是直接写入内存；
no_root_squash：NFS客户端连接服务端时如果使用的是root的话，那么对服务端分享的目录来说，也拥有root权限。显然开启这项是不安全的。
root_squash：NFS客户端连接服务端时如果使用的是root的话，那么对服务端分享的目录来说，拥有匿名用户权限，通常他将使用nobody或nfsnobody身份；
all_squash：不论NFS客户端连接服务端时使用什么用户，对服务端分享的目录来说都是拥有匿名用户权限；
anonuid：匿名用户的UID值，通常是nobody或nfsnobody，可以在此处自行设定；
anongid：匿名用户的GID值。
我用的是：
vi /etc/exports
/home 192.168.11.*(rw)
2、设置NFS服务器的自动启动状态

对于实际的应用系统，每次启动LINUX系统后都手工启动nfs服务器是不现实的，需要设置系统在指定的运行级别自动启动portmap和nfs服务。
\# chkconfig --list portmap
\# chkconfig --list nfs
3、设置portmap和nfs服务在系统运行级别3和5自动启动。
\# chkconfig --level 35 portmap on
\# chkconfig --level 35 nfs on


**NFS系统守护进程**
nfsd：它是基本的NFS守护进程，主要功能是管理客户端是否能够登录服务器；
mountd：它是RPC安装守护进程，主要功能是管理NFS的文件系统。当客户端顺利通过nfsd登录NFS服务器后，在使用NFS服务所提供的文件前，还必须通过文件使用权限的验证。它会读取NFS的配置文件/etc/exports来对比客户端权限。
portmap：主要功能是进行端口映射工作。当客户端尝试连接并使用RPC服务器提供的服务（如NFS服务）时，portmap会将所管理的与服务对应的端口提供给客户端，从而使客户可以通过该端口向服务器请求服务。
**NFS服务器的启动与停止**
在对exports文件进行了正确的配置后，就可以启动NFS服务器了。

service rpcbind start
service nfs start
重启portmap 和nfs 服务
service portmap restart
service nfs restart
exportfs
这样一来，NFS服务器就搭建好了，如果需要一个客户端来共享他的目录，需要在客户端执行这样的命令：
mount 192.168.11.210:/home/wow    /mnt/wow/config
就相当于把192.168.11.210（NFS服务器）的/home/wow挂载到 /mnt/wow/config目录上，实现了文件共享
其中，NFS服务器设定好了分享出来的/home/shares（可以是其他目录）这个目录后，其他的客户端就可以将这个目录挂载到自己系统上的/mnt/nfs挂载点（挂载点可以自定），只要在PC1系统中进入/mnt/nfs目录内，就可以看到NFS服务器系统内的/home/shares目录下的所有数据（要有相应的权限），/home/shares就好像自己PC中的一个分区（但不占用磁盘空间）。用户可以使用cp、cd、 mv、rm等磁盘或文件相关的指令进行操作。虽然NFS有属于自己的协议和端口号，但是在传送数据或其他相关信息时，NFS使用的是远程过程调用（Remote
 Procedure Call，RPC）协议来协助NFS本身的运作。


