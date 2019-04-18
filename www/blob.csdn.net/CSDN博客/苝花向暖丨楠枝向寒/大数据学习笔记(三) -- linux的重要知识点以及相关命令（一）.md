# 大数据学习笔记(三) -- linux的重要知识点以及相关命令（一） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月23日 14:29:38[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：82标签：[重要知识点																[常用命令](https://so.csdn.net/so/search/s.do?q=常用命令&t=blog)](https://so.csdn.net/so/search/s.do?q=重要知识点&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


**Table of Contents**

[linux特点](#linux%E7%89%B9%E7%82%B9%EF%BC%9A)

[根下目录含义](#%E6%A0%B9%E4%B8%8B%E7%9B%AE%E5%BD%95%E5%90%AB%E4%B9%89)

[ls -l  输出的内容解释](#ls%20-l%C2%A0%20%E8%BE%93%E5%87%BA%E7%9A%84%E5%86%85%E5%AE%B9%E8%A7%A3%E9%87%8A)

[type](#type)

[file](#file)

[yum](#yum)

[man](#man)

[help](#help)

[ehco](#ehco)

[df](#df)

[mount和umout](#mount%E5%92%8Cumout)

[du](#du)

[useradd](#useradd)

[cd](#cd)

[mkdir](#mkdir)

[rm](#rm)

[cp](#cp%C2%A0)

[mv](#mv)

[ln](#ln)

[stat](#stat)

# **linux特点**：

linux中 一切皆文件，一个进程也会被映射成文件，一个硬盘也会被映射成文件。

linux 是一个虚拟的目录树结构，每个分区会被对应一个目录。

window中我们执行一个程序是用鼠标点击exe，linux中是通过命令，其实输入一个命令就相当于windows中打开一个程序。

像下面这种显示方式，然后接收用户输入命令-->解析出命令-->执行命令-->返回结果 就是由bash程序为我们提供的服务

bash的工作原理 : 根据空格切割，第一个元素作为命令，判断命令是内部命令还是外部命令，在环境变量中以从左到右的顺序寻找，找到了就去执行。

```bash
[root@node001 ~]#
```

# **根下目录含义**

proc  内核运行状态的映射目录

proc/进程号/fd    存放该进程的 io 流 ，0代表标准输出流，1代表标准输入流，2代表错误输出流

bin  可执行文件目录

dev  设备目录

home 用户目录

lib 和 lib64 库目录

opt  用户软件安装目录

usr  一些规范的软件会自动安装在usr目录下

etc   配置文件目录

var  存放数据，如数据库数据或war包

mnt 挂载点目录，额外的临时文件系统

media 挂载点目录，移动设备

# **ls -l  输出的内容解释**

![](https://img-blog.csdn.net/20180923135716387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意 ，**上面的文件硬连接的次数是针对文件来说的，如果是一个目录，该目录下没有其他目录则为2，如果有其他目录，数字代表 2+目录数。**

如下

```bash
[root@node001 ~]# ll
total 32
drwxr-xr-x.  3 root root 4096 Sep 21 16:27 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
[root@node001 ~]# cd abc/
[root@node001 abc]# ll
total 8
drwxr-xr-x. 2 root root 4096 Sep 21 16:26 a
-rw-r--r--. 1 root root    4 Sep 21 16:27 b
[root@node001 abc]# cd a
[root@node001 a]# ll
total 0
```

 abc是一个目录，且目录下有一个目录 a ，有一个文件b ，所以abc 那里显示的是 2+1 = 3 ，而目录a是一个空目录，所以a显示的就是2，而普通文件显示的为1.

# **type**

通过type -a command 可以查看一个 命令所指向的程序所在的路径

如下

```bash
[root@node001 ~]# type -a ifconfig
ifconfig is /sbin/ifconfig   # 告诉我们ifconfig程序在 /sbin/ifconfig目录下
```

向ifconfig这种 返回一个路径，说明ifconfig是一个第三方的程序，并不是linux自带的。称为外部命令

```bash
[root@node001 ~]# type cd
cd is a shell builtin
```

它说 cd 命令 是一个 builtin ，意思就是说这是一个linux 集成的命令。称为内部命令

shell  代表该命令处于 人机交互层，比如这种命令行或windows下点击图标都称为shell

# **file**

用来查看文件类型

如下

```bash
[root@node001 ~]# file /sbin/ifconfig
/sbin/ifconfig: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.18, stripped
```

ELF 代表二进制可执行文件

```bash
[root@node001 ~]# type yum
yum is hashed (/usr/bin/yum)
[root@node001 ~]# file /usr/bin/yum 
/usr/bin/yum: a /usr/bin/python script text executable
```

通过上面命令可以看出 yum 是一个python脚本，那么一个python脚本并不是一个可执行程序，那么它是怎么运行的呢？

 我们 使用  vi /usr/bin/yum 命令来查看一下这个python脚本的内容，可以看到如下内容

![](https://img-blog.csdn.net/20180920232255145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意第一行，意思是使用 /usr/bin/下的python来执行该脚本，那么我们来看一下这个python到底 是不是一个二进制的可执行文件

```bash
[root@node001 bin]# file python
python: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for G
```

可以看出，是一个二进制可执行文件。 这个python程序会将 python脚本里的内容一边解释为cpu能读懂的东西，一边执行。 

# **yum**

安装外部程序

```bash
[root@node001 ~]# yum install man -y
```

```bash
[root@node001 ~]# yun install man-pages
```

-y  代表同意安装，没有 -y 还会询问一次是否确定安装 

# **man**

看一个外部程序的帮助文档

```bash
[root@node001 ~]# man ifconfig
```

 man命令可以查看下面几种类型的帮助文档

![](https://img-blog.csdn.net/20180921093240944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180921093517552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 可以通过 

```bash
[root@node001 ~]# man 8 ifconfig
```

 查看

# **help**

看一个内部程序的帮助文档

```bash
[root@node001 ~]# help cd
```

# **ehco**

在屏幕上显示字符串

$用于提取指定内容

下面 输出的是环境变量，环境变量配置的是可执行程序的所在的目录。bash会根据这些目录去找到可执行文件，从而响应用户输入的命令，要注意的是bash会从左向右去查找，查找到了就会执行。所以不存在相同的可执行程序想执行哪个就执行哪个。

```bash
[root@node001 bin]# echo $PATH
/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin
```

下面的命令可以输出bash程序的进程号 

```bash
[root@node001 dev]# echo $$
1557
```

# **df**

查看磁盘使用情况，以及磁盘挂载目录情况

```bash
[root@node001 grub]# df
Filesystem     1K-blocks   Used Available Use% Mounted on
/dev/sda3      100944296 834348  94982220   1% /
tmpfs             502204      0    502204   0% /dev/shm
/dev/sda1         198337  27810    160287  15% /boot
[root@node001 grub]# df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/sda3        97G  815M   91G   1% /
tmpfs           491M     0  491M   0% /dev/shm
/dev/sda1       194M   28M  157M  15% /boot
```

可以看出sda3 也就是我们之前 用来存放 自己安装的程序的磁盘挂载了在 根目录下

sda1 用来引导系统启动的磁盘挂载在了 根目录下的 boot 目录下

这里说一下 linux中的 目录和分区的对应关系，linux 根目录下的 所有文件夹是固定的，也就是你安装好以后就肯定存在那些文件夹，而我们安装centOS的时候进行的手动分区，实际上是将 分区指定挂载到某个目录下。

这种设计增强了可移植性。不像winows你写个代码，从E盘读取某个文件，把代码放别人电脑上，但别人电脑可能都没有E盘。

linux能够保证目录的一致性，就是所有人安装完linux系统 ，根目录下的目录都是一致的。

windows是在分区即不同磁盘中创建的目录，而linux是将分区挂载到目录下。然后目录又一样。

**linux系统启动的过程大致如下**

通电以后主板里有一个 bios(basic input output system) ，bios中有一段代码放入内存执行。 从而会指向引导系统的设备如优盘、硬盘 等。默认都是硬盘。读取硬盘前512字节（读的分区表），加载程序放入内存，执行程序 ， 找到活动分区 , 就是 boot 分区，分区起始位置读取固定字节数(把小程序再存入内存) ， 然后就把这第一个分区（boot分区）算作 根 ，从boot下再读取配置文件 、内核文件 读到内存里，从而给cpu reset指令，使内核接管硬件。内核接管硬件以后，cpu再读取的数据，就是从内核展开的指令里面读。然后重新挂载根目录，挂载的是sda3。然后会运行sbin中init程序（第一个程序）用来完成操作系统的初始化即安装各种软件，最终bash程序加载起来，我们就可以通过命令行进行人机交互了。

查看个文件来理解

```bash
[root@node001 boot]# vi /boot/grub/grub.conf
```

![](https://img-blog.csdn.net/20180921150335207?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个文件表明了 引导系统重要文件的位置， 它说是在根目录下，但实际这两个文件是在 boot目录下。之所以这样是因为在加载内核之前，boot分区就是根目录，只不过是加载完内核一个给cpu一个reset指令使其重新将sda3挂载到了根目录。然后再通过mount 将 sda1挂载到 boot目录下

# **mount和umout**

通过

```bash
[root@node001 /]# umount /boot/
```

将挂载移除

```bash
[root@node001 /]# df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/sda3        97G  815M   91G   1% /
tmpfs           491M     0  491M   0% /dev/shm
```

可以看到 sda1 的对应关系就没有了

```bash
[root@node001 /]# ll
total 92
dr-xr-xr-x.  2 root root  4096 Sep 21 03:27 bin
drwxr-xr-x.  2 root root  4096 Sep 21 03:25 boot
drwxr-xr-x. 18 root root  3700 Sep 21 06:01 dev
drwxr-xr-x. 61 root root  4096 Sep 21 09:29 etc
drwxr-xr-x.  2 root root  4096 Sep 23  2011 home
dr-xr-xr-x.  8 root root  4096 Sep 21 03:27 lib
dr-xr-xr-x.  9 root root 12288 Sep 21 03:27 lib64
drwx------.  2 root root 16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root  4096 Sep 23  2011 media
drwxr-xr-x.  2 root root  4096 Sep 23  2011 mnt
drwxr-xr-x.  2 root root  4096 Sep 23  2011 opt
dr-xr-xr-x. 85 root root     0 Sep 21 06:00 proc
dr-xr-x---.  2 root root  4096 Sep 21 07:56 root
dr-xr-xr-x.  2 root root 12288 Sep 21 03:27 sbin
drwxr-xr-x.  7 root root     0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root  4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root     0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root  4096 Sep 21 08:38 tmp
drwxr-xr-x. 13 root root  4096 Sep 21 03:26 usr
drwxr-xr-x. 17 root root  4096 Sep 21 03:26 var
[root@node001 /]# cd /boot/
[root@node001 boot]# ll
total 0
```

可以看到 boot 目录存在，但是里面的内容没了。

通过

```bash
[root@node001 dev]# mount /dev/sda1 /boot
```

将 dev 下的 sda1磁盘 挂载到 /boot 目录下 

```bash
[root@node001 boot]# df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/sda3        97G  815M   91G   1% /
tmpfs           491M     0  491M   0% /dev/shm
/dev/sda1       194M   28M  157M  15% /boot
```

```bash
[root@node001 dev]# cd /boot/
[root@node001 boot]# ll
total 21672
-rw-r--r--. 1 root root   105195 Nov 22  2013 config-2.6.32-431.el6.x86_64
drwxr-xr-x. 3 root root     1024 Sep 21 03:27 efi
drwxr-xr-x. 2 root root     1024 Sep 21 09:28 grub
-rw-------. 1 root root 15221168 Sep 21 03:28 initramfs-2.6.32-431.el6.x86_64.img
drwx------. 2 root root    12288 Sep 21 03:25 lost+found
-rw-r--r--. 1 root root   193758 Nov 22  2013 symvers-2.6.32-431.el6.x86_64.gz
-rw-r--r--. 1 root root  2518236 Nov 22  2013 System.map-2.6.32-431.el6.x86_64
-rwxr-xr-x. 1 root root  4128368 Nov 22  2013 vmlinuz-2.6.32-431.el6.x86_64
```

又可以看到内容了。

# **du**

查看目录占用空间

-h 单位换算、-s 求和      后面再接路径代表查看指定文件所占大小

```bash
[root@node001 etc]# du -hs ./*
```

# **useradd**

添加用户，添加的用户会存在home文件夹下，以文件的形式展示

```bash
[root@node001 etc]# useradd zzh
[root@node001 etc]# cd /home/
[root@node001 home]# ll
total 12
drwx------. 2 god  god  4096 Sep 21 13:24 god
drwx------. 2 good good 4096 Sep 21 13:24 good
drwx------. 2 zzh  zzh  4096 Sep 21 14:50 zzh
```

# **cd**

cd ~  进入当前目录

```bash
[root@node001 home]# cd ~
```

cd ~ 其他用户名  和  cd/home/其他用户名               进入其他用户目录

```bash
[root@node001 ~]# cd ~god
[root@node001 god]# cd /home/good/
[root@node001 good]#
```

# **mkdir**

创建目录

平行创建多个目录

```bash
[root@node001 ~]# mkdir ./x/1dir ./x/2dir ./x/3dir
[root@node001 ~]# ls
abc  anaconda-ks.cfg  install.log  install.log.syslog  x
[root@node001 ~]# cd x
[root@node001 x]# ls
1dir  2dir  3dir  y
```

花括号优化  ， 分号 分割命令

```bash
[root@node001 x]# cd;mkdir ./x/{a,b,c}dir
[root@node001 ~]# ls
abc  anaconda-ks.cfg  install.log  install.log.syslog  x
[root@node001 ~]# cd x
[root@node001 x]# ls
1dir  2dir  3dir  adir  bdir  cdir  y
```

-p 递归创建多个目录

```bash
[root@node001 ~]# ls
abc  anaconda-ks.cfg  install.log  install.log.syslog  x
[root@node001 ~]# cd x
[root@node001 x]# cd y
[root@node001 y]# cd z
```

# **rm**

删除

-r  递归删除

-f  删除不询问是否确定删除

```
[root@node001 tmp]# rm x
rm: cannot remove `x': Is a directory
[root@node001 tmp]# rm -r x
rm: descend into directory `x'? ^C
[root@node001 tmp]# rm -rf x
[root@node001 tmp]# ls
yum.log
```

# **cp **

拷贝 文件

-r 递归拷贝

```bash
[root@node001 ~]# cp x /tmp
cp: omitting directory `x'
[root@node001 ~]# cp -r x /tmp
```

# **mv**

移动文件且可以改名

```bash
[root@node001 zzh]# mv /tmp/p /home/zzh/x
[root@node001 zzh]# ls
x
```

```bash
[root@node001 zzh]# ls
x
[root@node001 zzh]# mv x p
[root@node001 zzh]# ls
p
```

# **ln**

添加链接，类似windows中的快捷方式，链接分为软连接和硬链接

**硬链接 **  ln  原文件名  链接名

```bash
[root@node001 ~]# ll
total 32
drwxr-xr-x.  2 root root 4096 Sep 21 15:00 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
[root@node001 ~]# ln profile xyz
[root@node001 ~]# ll -i
total 36
1310732 drwxr-xr-x.  2 root root 4096 Sep 21 15:00 abc
1310729 -rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
1310722 -rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
1310723 -rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
1310742 -rw-r--r--.  2 root root 1796 Sep 21 15:38 profile
1310733 drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
1310742 -rw-r--r--.  2 root root 1796 Sep 21 15:38 xyz
```

ls -l -i  通过 i  可以查看 文件地址， 可以看到 xyz 和 profile 的地址指向是一样的。以及 ll -i 输出内容中profile和xyz的第三个字段 是 2. 

**软连接    **

ln -s 源文件名 链接名

```bash
[root@node001 ~]# ln -s xyz aaa
[root@node001 ~]# ll -i
total 36
1310744 lrwxrwxrwx.  1 root root    3 Sep 21 16:33 aaa -> xyz
1310732 drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
1310729 -rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
1310722 -rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
1310723 -rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
1310742 -rw-r--r--.  2 root root 1796 Sep 21 15:38 profile
1310733 drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
1310742 -rw-r--r--.  2 root root 1796 Sep 21 15:38 xyz
```

可以发现 软连接的 第三个字段数字为1， 源文件和链接文件所指向的地址也不一样，一个是1310744一个是1310742.

并且软连接文件的 第一个字母是 l 。

删除源文件以后会提示 找不到源文件

![](https://img-blog.csdn.net/20180923140947730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 无论是软连接还是硬链接，改变链接文件中的内容或源文件内容，另一个都会跟着改变

# **stat**

**查看文件属性**

```bash
[root@node001 /]# stat /etc/profile
  File: `/etc/profile'
  Size: 1796      	Blocks: 8          IO Block: 4096   regular file
Device: 803h/2051d	Inode: 1179683     Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2018-09-21 03:34:13.380600126 +0800
Modify: 2013-10-02 17:06:18.000000000 +0800
Change: 2018-09-21 03:26:39.649999827 +0800
```

Access 访问该文件的最后一次时间

Modify 修改该文件内容的最后一次时间

Change  元文件被改变的最后一次时间，什么叫元文件？ 就是该文件的大小被改变，权限被改变等等都被称做元文件被改变。

**touch**

作用1 : 

touch 未存在文件名  创建该文件

```bash
[root@node001 /]# cd opt/
[root@node001 opt]# ll
total 0
[root@node001 opt]# touch test
[root@node001 opt]# stat test 
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: 803h/2051d	Inode: 2621442     Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2018-09-21 16:51:37.114888038 +0800
Modify: 2018-09-21 16:51:37.114888038 +0800
Change: 2018-09-21 16:51:37.114888038 +0800
```

作用2:

touch  已存在文件名  刷新 Acess、Modify、Change的三个时间到当前时间

```bash
[root@node001 opt]# stat test 
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: 803h/2051d	Inode: 2621442     Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2018-09-21 16:51:37.114888038 +0800
Modify: 2018-09-21 16:51:37.114888038 +0800
Change: 2018-09-21 16:51:37.114888038 +0800
[root@node001 opt]# touch test 
[root@node001 opt]# stat test 
  File: `test'
  Size: 0         	Blocks: 0          IO Block: 4096   regular empty file
Device: 803h/2051d	Inode: 2621442     Links: 1
Access: (0644/-rw-r--r--)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2018-09-21 16:54:03.108886546 +0800
Modify: 2018-09-21 16:54:03.108886546 +0800
Change: 2018-09-21 16:54:03.108886546 +0800
```

