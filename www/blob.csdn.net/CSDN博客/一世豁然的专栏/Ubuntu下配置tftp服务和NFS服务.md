# Ubuntu下配置tftp服务和NFS服务 - 一世豁然的专栏 - CSDN博客





2015年07月01日 14:24:49[一世豁然](https://me.csdn.net/Explorer_day)阅读数：652标签：[TFTP																[NFS](https://so.csdn.net/so/search/s.do?q=NFS&t=blog)](https://so.csdn.net/so/search/s.do?q=TFTP&t=blog)
个人分类：[linux操作系统](https://blog.csdn.net/Explorer_day/article/category/2569249)








本博客转载于：[http://www.cnblogs.com/shenhaocn/archive/2011/03/13/1983042.html](http://www.cnblogs.com/shenhaocn/archive/2011/03/13/1983042.html)






**配置tftp**


方法一：（推荐方法）[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)10.04 测试通过


1.安装TFTP软件

sudo apt-get install tftp-hpa tftpd-hpa

tftp-hpa是客户端,tftpd-hpa是服务器端

2.建立tftpboot目录，作为服务器的目录
sudo mkdir ~/tftpboot

释放权限：(服务器目录,需要设置权限为777,chomd 777)
sudo chmod 777 ~/tftpboot





3.配置TFTP服务器

sudo gedit /etc/default/tftpd-hpa


将原来的内容为:


# /etc/default/tftpd-hpa


TFTP_USERNAME="tftp"
#TFTP_DIRECTORY="/var/lib/tftpboot"  #修改为自己需要的路径，与上面创建的目录一致

TFTP_ADDRESS="0.0.0.0:69"
#TFTP_OPTIONS="--secure"  #可以通过查看mantftpd,看各种参数的意义


#修改为

TFTP_DIRECTORY="/home/shenhao/tftpboot"

TFTP_OPTIONS="-l -c -s"


3.重新启动TFTP服务

sudo service tftpd-hpa restart


4.测试下


$ cd ~/tftpboot echo "hello tftp service">>a.txt


$ echo "hello tftp service,put to tftp serive">>b.txt


$ tftp localhost


tftp> get a.txt


tftp> put b.txt


tftp> quit 


其中get是取得文件,put是将文件上传到TFTP服务器上。

**方法二：**


配置tftp服务的步骤： 

1、安装相关软件包：tftpd（服务端），tftp（客户端），xinetd 
sudo apt-get install tftpd tftp xinetd


2、建立配置文件（蓝色的目录是可以更改为其他地址的，例如/home/user/tftpboot） 

在/etc/xinetd.d/下建立一个配置文件tftp 
sudo vi tftp 
在文件中输入以下内容： 

service tftp 

{ 

socket_type = dgram 

protocol = udp 

wait = yes 

user = root 

server = /usr/sbin/in.tftpd 

server_args = -s /tftpboot 

disable = no 

per_source = 11 

cps = 100 2 

flags = IPv4 

} 

保存退出


3、建立tftp服务文件目录（上传文件与下载文件的位置），并且更改其权限 
sudo mkdir /tftpboot

sudu chmod 777 /tftpboot


4、重新启动服务 

sudo /etc/init.d/xinetd restart 

至此tftp服务已经安装完成了，下面可以对其进行一下测试。（假设在当前目录下有一个测试文件test.txt） 

$tftp 192.168.16.2 (本机的ip地址) 
tftp> get test.txt 

tftp> quit 

$


通过get命令，可以把当前目录下的test.txt文件，通过tftp上传到它的服务文件目录。这时，在/tftpboot下面会出现test.txt文件。通过put命令，可以从/tftpboot下,下载test.txt文件。这样就验证了tftp服务配置的正确性。当文件上传与下载结束后，可以通过quit命令退出。


严格按照以上步骤配置tftp服务，一般都可以成功。如果出现无法get或者put的时候，可以查看一下防火墙是否关闭。


-----------------------------------------------------------------------------------------------------

**配置nfs服务器**



在ubuntu下安装、配置nfs服务的步骤如下： 

1、安装nfs 

Ubuntu上默认是没有安装nfs服务器的，因此我们首先安装nfs服务器端： 
$sudo apt-get install nfs-kernel-server 
在一些文档中，提出还需要使用apt-get来手动安装nfs的客户端nfs-common，以及端口映射器portmap，但其实这是没有必要的，因为在安装nfs-kernel-server时，apt会自动为我们把它们安装好。


2、配置/etc/exports 
nfs允许挂载的目录及权限在文件/etc/exports中进行了定义。 

例如，我们要将根目录下的rootfs目录共享出来，那么我们需要在/etc/exports文件末尾添加如下一行：

/rootfs  *(rw,sync,no_root_squash) 
其中： /rootfs是要共享的目录，


            *代表允许所有的网络段访问，


            rw是可读写权限,sync是资料同步写入内存和硬盘，


no_root_squash是nfs客户端分享目录使用者的权限，如果客户端使用的是root用户，那么对于该共享目录而言，该客户端就具有root权限。


其它nfs常用的参数有：

ro 

只读访问 

rw 读写访问sync 所有数据在请求时写入共享 

async 

nfs在写入数据前可以响应请求 

secure 

nfs通过1024以下的安全TCP/IP端口发送 

insecure 

nfs通过1024以上的端口发送 

wdelay 

如果多个用户要写入nfs目录，则归组写入（默认） 

no_wdelay 

如果多个用户要写入nfs目录，则立即写入，当使用async时，无需此设置。 

hide 在nfs共享目录中不共享其子目录 

no_hide 

共享nfs目录的子目录 

subtree_check 

如果共享/usr/bin之类的子目录时，强制nfs检查父目录的权限（默认） 

no_subtree_check 

和上面相对，不检查父目录权限 

all_squash 

共享文件的UID和GID映射匿名用户anonymous，适合公用目录。 

no_all_squash 

保留共享文件的UID和GID（默认） 

root_squash 

root用户的所有请求映射成如anonymous用户一样的权限（默认） 

no_root_squas 

root用户具有根目录的完全管理访问权限 

anonuid=xxx 

指定nfs服务器/etc/passwd文件中匿名用户的UID 

anongid=xxx 

指定nfs服务器/etc/passwd文件中匿名用户的GID


3、重启服务 
$sudo /etc/init.d/portmap restart 

$sudo /etc/init.d/nfs-kernel-server restart


4、测试nfs 

此时可以运行以下命令来显示一下共享出来的目录： 
$showmount -e 
或者可以使用以下命令把它挂载在本地磁盘上，例如将/rootfs挂载到/mnt下： 
$sudo mount -t nfs localhost:/rootfs /mnt


可以运行df命令查看是否挂载成功。查看后可以使用以下命令卸载： 
$ sudo umount /mnt


------------------------------------------------

**关于NFS配置错误**




ubuntu测试本机nfs出错：mount.nfs:access denied by server while mounting...


相信出现这种错误的应该有一些人吧，没关系，一会就帮你搞定。 

首先运行：sudo vim /etc/exports 

在其文本后添加 

/home/work 192.168.1.*（rw,sync,no_root_squash） 

然后保存退出。


注意，上面的主机IP可能不能使用＊来通配，否则在客户机上会出现访问拒绝，但是如果我们要设置局域网访问呢？怎么办，使用子网掩码例如：10.1.60.0/255.255.254.0即可让10.1.60.*和10.1.61.*都可以访问,还可以使用10.1.60/23这种方式类确定子网。


/home/work就表示共享目录，当然，你可以随便换成自己喜欢的目录，192.168.1.＊，前面三位是你主机的ip地址（怎么获得本机的ip地址？ifconfig命令就可以了） 

rw：读/写权限，只读权限的参数为ro； 

sync：数据同步写入内存和硬盘，也可以使用async，此时数据会先暂存于内存中，而不立即写入硬盘。 

no_root_squash：NFS 服务器共享目录用户的属性，如果用户是 root，那么对于这个共享目录来说就具有 root 的权限。


下面浅蓝色的文字，可略过去！ 
起用保护机制 

上面设置了只要在192.168.1.＊这个网段的所有IP地址用户都可以访问共享目录，但我只想让一个IP地址访问，比如192.168.1.101（把刚才192.168.1.＊直接换成192.168.1.101不行么？应该可以吧，没试过），那么就可以这样设置了。 

可以通过设定/etc/hosts.deny和/etc/hosts.allow文件来限制网络服务的存取权限。 

***/etc/hosts.deny*** 

portmap:ALL 

lockd:ALL 

mountd:ALL 

rquotad:ALL 

statd:ALL 

***/etc/hosts.allow*** 

portmap:192.168.1.101 

lockd:192.168.1.101 

mountd:192.168.1.101 

rquotad:192.168.1.101 

statd:192.168.1.101 

同时使用这两个文件就会使得只有ip为192.168.1.101的机器使用NFS服务。你的target 

board的ip地址设定为192.168.1.101，这样就可以了。 

浅蓝色文字可略过。


接着执行如下命令: 

启动端口映射： #sudo 

/etc/rc.d/init.d/portmap start（如果说找不到，就执行 #sudo /etc/init.d/portmap start） 

如果这一步不成功，最后也连接不上了。 

启动NFS 服务： #sudo /etc/rc.d/init.d/nfs start（又找不到，执行 #sudo 

/etc/init.d/nfs start或＃sudo /etc/init.d/nfs-kernel-server start） 

这两项执行成功 

的话，就会显示［ok］ 

最后执行：#sudo mount –t nfs 192.168.1.101:/home/work /mnt 

这里面的192.168.1.101是你自己主机的ip地址，也可以用127.0.0.1来代替自己的主机ip地址，同样可以连接上。/home/work就是刚才在etc/exports里添加的路径。 

如果在这一步又出错了，呵呵，重新#sudo vim /etc/exports，将“ 192.168.1.＊”换成“＊”，就这一个星星哦！ 

执行：# ls /mnt


恭喜你，mnt下是不是有了/home/work里的内容了。


卸载：#sudo umount /mnt/


如果上面操作做好了，还是不行，我觉得你应该重启一下机子试一下了！ 

更详细请点击：[linux.chinaunix.net/bbs/thread-985579-1-1.html](http://linux.chinaunix.net/bbs/thread-985579-1-1.html)

**提示：**挂载根文件时，提示“**Warning: unable to open an initial console.** ”启动信息问题的解决，以NFS为例。


清单51 **问题现象**


…


VFS: Mounted root (nfs filesystem).


Freeing init memory: 144K

**Warning: unable to open an initial console.**<启动停到此处>

**根据提示信息定位错误**


查找错误原因: 根据错误提示信息：”Warning: unable to open an initial 

console.”查看内核源代码。可定位错误提示在内核文件linux-2.6.26.2/init/main.c文件中。


清单52 **分析main.c的调用关系**


asmlinkage void __init start_kernel(void)


{


…


rest_init();


}


static void noinline __init_refok rest_init(void)


__releases(kernel_lock)


{


…


kernel_thread(kernel_init, NULL, CLONE_FS | CLONE_SIGHAND);


…


}


static int __init kernel_init(void * unused)


{


…


init_post();


return 0;


}


static int noinline init_post(void)


{


…


if (sys_open((const char __user *) "**/dev/console**", O_RDWR, 

0) < 0)


printk(KERN_WARNING "**Warning: unable to open an initial console.**\n");


…


run_init_process("/sbin/init");


…


}通过分析知道内核启动时需要成功打开/dev/console设备，否则给出"**Warning: unable to open an initial console.**"的错误提示。


//为目标机配置console设备文件节点


进入你的nfs文件系统的/dev/目录

**sudo mknod console c 5 1**


创建console文件，再重启即可。


如果还不可以可以试下以下操作：


内核启动显示：


Warning: unable to open an initial console. 

Failed to execute /linuxrc.  Attempting defaults... 

Kernel panic - not syncing: No init found.  Try passing init= option to kernel.


解决：


创建rootfs过程中，在/dev目录下手动创建如下节点：

**sudo mknod -m 660 null c 1 3**

**sudo mknod -m 660 console c 5 1**





**以下内容转载自：[http://hi.baidu.com/linuxcfan/blog/item/2c4c7e39723194c8d56225b4.html](http://hi.baidu.com/linuxcfan/blog/item/2c4c7e39723194c8d56225b4.html)**

**网络文件系统(NFS)**

**一、NFS简介**

1、NFS就是Network File System的缩写，它的最大功能就是可以通过网络让不同的机器，不同的操作系统彼此共享文件(share files)——可以通过NFS挂载远程主机的目录，访问该目录就像访问本地目录一样，所以也可以简单的将它看作一个文件服务器(File Server)。

**注意：**一般而言，使用NFS服务能够方便地使各unix-like系统之间实现共享，但如果需要在unix-like和windows系统之间共享，那就得使用samba了。

2、NFS是通过网络进行数据传输，那么NFS使用哪些端口呢，答案是……不知道，因为NFS传输数据时使用的端口是随机的，唯一的限制就是小于1024，客户端怎么知道服务器使用的是哪个端口，此时就要用到远程过程调用RPC。

其实，NFS运行在SUN的RPC（Remote Procedure Call，远程过程调用）基础上，RPC定义了一种与系统无关的方法来实现进程间通信，由此，NFS Server也可以看作是RPC Server。正因为NFS是一个RPC服务程序，所以在使用它之前，先要映射好端口——通过portmap设定。比如：某个NFS
 Client发起NFS服务请求时，它需要先得到一个端口(port)，所以它先通过portmap得到port number（不仅是NFS，所有的RPC服务程序启动之前，都需要先设定好portmap）。

**注意：**在启动RPC服务（比如NFS）之前，需要先启动portmap服务。

3、NFS允许系统将其目录和文件共享给网络上的其他系统。通过NFS，用户和应用程序可以访问远程系统上的文件，就像它们是本地文件一样。那么NFS最值得注意的优点有：

（1）本地工作站可以使用更少的磁盘空间，因为常用数据可以被保存在一台机器上，并让网络上的其他机器可以访问它。

（2）不需要为用户在每台网络机器上放一个用户目录，因为用户目录可以在NFS服务器上设置并使其在整个网络上可用。

（3）存储设备如软盘、光驱及USB设备可以被网络上其它机器使用，这可能可以减少网络上移动设备的数量。

**二、与NFS相关的几个文件和命令**

1、/etc/exports

对NFS服务的访问是由exports来批准，它枚举了若干有权访问NFS服务器上文件系统的主机名。

2、/sbin/exportfs

维护NFS的资源共享，可以通过它重新设定/etc/exports的共享目录，卸载NFS Server共享的目录或者重新共享等。

3、/usr/sbin/showmount 

上面的文件主要用在NFS Server端，而showmount则主要用在Client端，showmount可以用來查看NFS共享的目录资源。

4、/var/lib/nfs/xtab

NFS的记录文档：通过它可以查看有哪些Client连接到NFS主机的记录。

下面这几个并不直接负责NFS，实际上它们是负责所有的RPC。

5、/etc/default/portmap

实际上，portmap负责映射所有的RPC服务端口，它的内容非常非常之简单。

6、/etc/hosts.deny

设定拒绝portmap服务的主机，即禁止访问的客户端IP列表。

7、/etc/hosts.allow

设定允许portmap服务的主机，即允许访问的客户端IP列表。

**三、NFS安装**

在主机上安装NFS服务软件，因为Debian/Ubuntu上默认是没有安装的。

1、安装端口映射器portmap（可选）
$ sudo apt-get install portmap

2、在终端提示符后键入以下命令安装NFS服务器
$ sudo apt-get install nfs-kernel-server

3、安装NFS客户端（可选）

$ sudo apt-get install nfs-common

**注意：**nfs- kernel-server和nfs-common都依赖于portmap。另外，在一些文档中提出还需要使用apt-get来手动安装NFS的客户端
 nfs-common，以及端口映射器portmap，但其实这是没有必要的，因为在安装nfs-kernel-server时，apt会自动把它们安装好。

这样，宿主机就相当于NFS Server。同样地，目标系统作为NFS的客户端，需要安装NFS客户端程序。如果是Debian/Ubuntu系统，则需要安装nfs-common（第3步）。

**四、NFS配置**

1、配置portmap
方法1：编辑/etc/default/portmap，将"-i 127.0.0.1"去掉；

方法2：$ sudo dpkg-reconfigure portmap，出现“正在设定portmap”软件包设置界面，对Should portmap be bound to the loopback address？选择“否(No)”。

2、配置/etc/hosts.deny
禁止任何host（主机）能和你的NFS服务器进行NFS连接。在该文件中加入：

### NFS DAEMONS
portmap:ALL
lockd:ALL
mountd:ALL
rquotad:ALL
statd:ALL

3、配置/etc/hosts.allow
允许那些你想要的主机和你的NFS服务器建立连接。

下列步骤将允许任何IP地址以192.168.1开头的主机连接到NFS服务器上，具体要看你目标板的端口地址，也可以指定特定的IP地址。在该文件中加入：

### NFS DAEMONS
portmap: 192.168.1.
lockd: 192.168.1.
rquotad: 192.168.1.
mountd: 192.168.1.
statd: 192.168.1.

通过/etc/hosts.deny和/etc/hosts.allow设置对portmap的访问，采用这两个配置文件有点类似"mask"的意思。先在/etc/hosts.deny中禁止所有用户对portmap的访问，再在/etc/hosts.allow中允许某些用户对portmap的访问。

然后重启portmap daemon：
$ sudo /etc/init.d/portmap restart

4、配置/etc/exports

（1）共享的NFS目录在/etc/exports中列出，这个文件控制对目录的共享（NFS挂载目录及权限由该文件定义），书写规则是每个共享为一行）。

格式：[共享目录] [主机名或IP](参数,参数...)

第一个参数是要让客户机访问的目录，第二个是你允许的主机IP，最后的()内是访问控制方式。

**注意：**客户端可以使用主机名或者IP地址指定，在主机名中可以使用通配符(*)，IP地址后也可以跟掩码段(/24)，但出于安全原因这种情况应该尽量避免。客户端的说明后可在圆括号中加入一系列参数。很重要的一点，不要在最后一个客户端声明的后面留下任何空白或者没关闭括号，因为空白都被解释成客户端的分隔符。

例如我要将/opt/FriendlyARM/mini2440/root_nfs目录让用户的IP共享，则在该文件末尾添加下列语句：

/opt/FriendlyARM/mini2440/root_nfs *(rw,sync,no_root_squash)

其中：

/opt/FriendlyARM/mini2440/root_nfs 表示NFS共享目录，它可以作为开发板的根文件系统通过NFS挂接；

* 表示所有的客户机都可以挂接此目录；

rw 表示挂接此目录的客户机对该目录有读写的权力；

sync 表示所有数据在请求时写入共享，即数据同步写入内存和硬盘；

no_root_squash 表示允许挂接此目录的客户机享有该主机的root身份。

**注意：**可以用主机名来代替*，尽量指定主机名以便使那些不想其访问的系统不能访问NFS挂载的资源。另外，最好加上sync, 否则$ sudo exportfs -r时会给出警告，sync是NFS的默认选项。

（2）下面是一些NFS共享的常用参数：
ro 只读访问

rw 读写访问

sync 所有数据在请求时写入共享

async NFS在写入数据前可以相应请求

secure NFS通过1024以下的安全TCP/IP端口发送

insecure NFS通过1024以上的端口发送

wdelay 如果多个用户要写入NFS目录，则归组写入（默认）

no_wdelay 如果多个用户要写入NFS目录，则立即写入，当使用async时，无需此设置

hide 在NFS共享目录中不共享其子目录

no_hide 共享NFS目录的子目录

subtree_check 如果共享/usr/bin之类的子目录时，强制NFS检查父目录的权限（默认）

no_subtree_check 和上面相对，不检查父目录权限

all_squash 共享文件的UID和GID映射匿名用户anonymous，适合公用目录

no_all_squash 保留共享文件的UID和GID（默认）

root_squash root用户的所有请求映射成如anonymous用户一样的权限（默认）

no_root_squash root用户具有根目录的完全管理访问权限

anonuid=xxx 指定NFS服务器/etc/passwd文件中匿名用户的UID

anongid=xxx 指定NFS服务器/etc/passwd文件中匿名用户的GID

（3）查看NFS Server的export list：
$ sudo showmount -e

若更改了/etc/exports，运行以下命令进行更新：

$ sudo exportfs -r

然后重启NFS服务：

$ sudo /etc/init.d/nfs-kernel-server restart

**五、启动和停止NFS服务**

1、启动NFS的方法和启动其他服务器的方法类似，首先需要启动portmap和NFS这两个服务，并且portmap服务一定要先于NFS服务启动。

$ sudo /etc/init.d/portmap start
$ sudo /etc/init.d/nfs-kernel-server start

2、停止NFS服务

在停止NFS服务的时候，需要先停止NFS服务再停止portmap服务，如果系统中还有其他服务需要使用portmap服务，则可以不停止portmap服务。

$ sudo /etc/init.d/nfs-kernel-server stop
$ sudo /etc/init.d/portmap stop

3、重新启动portmap和NFS服务

$ sudo /etc/init.d/portmap restart
$ sudo /etc/init.d/nfs-kernel-server restart

4、检查portmap和NFS服务状态

$ sudo /etc/init.d/portmap status（不知原文是否有误，我的ubuntu上portmap貌似没status这个命令参数）

$ sudo /etc/init.d/nfs-kernel-server status

5、设置自动启动NFS服务

（1）检查NFS的运行级别：

$ sudo chkconfig --list portmap      （我的电脑也没chconfig这个工具，不过没多大关系，如果有问题再回头）

$ sudo chkconfig --list nfs-kernel-server

（2）在实际使用中，如果每次开启计算机之后都手工启动NFS服务是非常麻烦的，此时可以设置系统在指定的运行级别自动启动portmap和NFS服务。

$ sudo chkconfig --level 235 portmap on  （由于没有chkconfig工具，所以我就用services-admin（也就是图形界面的“系统─>“服务”）来代替，至于level就不管了，用默认设置）

$ sudo chkconfig --level 235 nfs-kernel-server on

**六、NFS客户端配置（NFS测试）**

1、在NFS服务器启动后，还需要检查Linux服务器的防火墙设置（一般需要关闭防火墙服务），确保没有屏蔽 NFS使用的端口和允许通信的主机，主要是检查Linux服务器iptables、ipchains等选项的设置，以及/etc/hosts.deny， /etc/hosts.allow文件。通常都是在内部局域网中进行开发，再安装系统时最好不要安装防火墙等网络安全软件，以方便使用时的配置。

如果你有防火墙，请确保32771、111和2049端口保持开放。

2、手动挂载
使用mount命令来挂载其他机器共享的NFS目录。

格式：$ sudo mount [Server IP]:/[share dir] [local mount point]

例如：

$ sudo mount -t nfs [-o nolock] localhost:/opt/FriendlyARM/mini2440/root_nfs /mnt/root_nfs 或

$ sudo mount -t nfs -o nolock 192.168.1.101:/opt/FriendlyARM/mini2440/root_nfs /mnt/root_nfs

其中，localhost可以是具体的IP地址，同时挂载点/mnt/root_nfs目录必须已经存在，而且在/mnt/root_nfs目录中没有文件或子目录。

3、自动挂载

（1）另一个挂载其他机器的NFS共享的方式就是在/etc/fstab文件中添加一行，该行必须指明NFS服务器的主机名、服务器输出的目录名以及挂载NFS共享的本机目录，同时必须是根用户才能修改/etc/fstab文件（目标板上可能没有fstab，需要自己创建一个）。

格式(参考PC上的fstab)：host_ip:/nfs_path /target_path nfsrsize=8192,wsize=8192,timeo=14,intr,nolock  0   0

**注意：**可以根据实际情况修改NFS服务器共享文件夹"servername.mydomain.com:/usr/local/pub"和在本机的挂载点"/pub"，同时挂载点/pub在客户端机器上必须存在。

（2）NFS常见挂载参数:

intr 允许通知中断一个NFS调用。当服务器没有应答需要放弃的时候有用处。


timeo 如果超时，客户端等待的时间，以十分之一秒计算

retrans 超时尝试的次数

bg 后台挂载（很有用）

hard 如果server端没有响应，那么客户端一直尝试挂载

rsize 读块大小

wsize 写块大小

4、使用autofs来挂载NFS

（1）挂载NFS共享的第三种方法是使用autofs，它使用automount守护进程来管理挂载点，只在文件系统被访问时才动态地挂载。

autofs访问主映射配置文件/etc/auto.master来决定要定义哪些挂载点，然后使用适用于各个挂载点的参数来启动automount守护进程。主映射配置中的每一行都定义一个挂载点，一个分开的映射文件定义在该挂载点下要挂载的文件系统。如/etc /auto.misc文件可能会定义/misc目录中的挂载点，这种关系在/etc/auto.master文件中会被定义。

（2）/etc/auto.master文件中的每个项目都有3个字段，第1个字段是挂载点；第2个字段是映射文件的位置；第3个字段可选，可以包括超时数值之类的信息。

例如：要在机器上的/misc/myproject挂载点上挂载远程机penguin.example.net中的/project52目录。

在/etc/auto.master文件中添加以下行：

/misc /etc/auto.misc --timeout 60

在/etc/auto.misc文件中添加以下行：

myproject -rw,soft,intr,rsize=8192,wsize=8192 penguin.example.net:/proj52

/etc/auto.misc中的第1个字段是/misc子目录的名称，该目录被automount动态地创建，它不应该在客户端机器上实际存在；第2个字段包括挂载选项，如rw代表读写访问权，第3个字段是要导出的NFS的位置，包括主机名和目录。

（3）autofs是一种服务，要启动这项服务，在shell提示下键入以下命令：

$ sudo /sbin/service autofs restart

要查看活跃的挂载点，在shell提示下键入以下命令：

$ sudo /sbin/service autofs status

如果在autofs运行时修改了/etc/auto.master配置文件，则必须在shell提示下键入以下命令来通知automount守护进程重新载入配置文件：

$ sudo /sbin/service autofs reload

5、可以运行df命令查看是否挂载成功：

$ sudo df

取消挂载的命令如下：

$ sudo umount /mnt/root_nfs

**七、目标板NFS配置操作**

主机IP：192.168.1.101

目标板IP：192.168.1.230

将USB转串口连接上，在终端输入minicom与板子连起，作为“超级终端”使用。

启动目标板并连通网络后，首先查看目标板kernel自身是否支持NFS，在minicom中输入cat /proc/filesystems命令查看其中是否有NFS一行，若没有则表示内核不支持NFS，就需要重新编译和烧写内核；有则OK，接下来就可以直接进行mount操作了。

具体命令是：
# mount -t nfs -o nolock 192.168.1.101:/opt/FriendlyARM/mini2440/root_nfs /mnt/root_nfs

无任何提示表示成功，这时可以进入/mnt/root_nfs目录，对文件进行cp、mv等操作。

但是如果使用命令mount -t nfs 192.168.1.101:/opt/FriendlyARM/mini2440/root_nfs /mnt/root_nfs，则会有如下的错误提示（也就是省去了"-o nolock"）：
# mount -t nfs 192.168.1.101:/opt/FriendlyARM/mini2440/root_nfs /mnt/root_nfs
portmap: server localhost not responding, timed out

RPC: failed to contact portmap (errno -5).

portmap: server localhost not responding, timed out

RPC: failed to contact portmap (errno -5).

lockd_up: makesock failed, error="-5"

portmap: server localhost not responding, timed out

RPC: failed to contact portmap (errno -5).

这时如果使用ls /mnt/root_nfs命令查看该目录内容时，你会发现此时NFS确确实实已经挂载成功了。

然后取消挂载的时候会出现如下的错误提示：
# umount /mnt/root_nfs/

lockd_down: no lockd running.

而成功取消挂载时没有任何提示，同样此时NFS也确实已经取消挂载了。

那么出现上面错误提示的可能原因是：
nfs mount默认选项包括文件锁，依赖于portmap提供的动态端口分配功能。

简单的解决方法：kill文件锁（lockd）或者使用mount -o nolock命令。

上面说的文件锁具体是什么原因，目前还没有找到更好的解决办法。不过，如果主机防火墙有设置时，也可能导致目标板无法正常访问。





