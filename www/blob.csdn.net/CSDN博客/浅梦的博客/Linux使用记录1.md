# Linux使用记录1 - 浅梦的博客 - CSDN博客





2017年03月01日 21:48:08[浅梦s](https://me.csdn.net/u012151283)阅读数：589








# The Linux Foundation
- Debian Family Systems (such as Ubuntu)
- SUSE Family Systems (such as openSUSE)
- Fedora Family Systems (such as CentOS)

# Linux Structure and Installation
- 
Linux支持的不同类型的文件系统
- 传统硬盘文件系统：ext2,ext3,ext4,XFS,Btrfs,JFS,NTFS,etc.
- 闪存文件系统：ubifs,JFFS2,YAFFS,etc.
- 数据库文件系统
- 特殊文件系统：procfs,sysfs,tmpfs,debugfs,etc.

- 
分区和文件系统

|_|Windos|Linux|
|----|----|----|
|分区|Disk1|/dev/sda1|
|文件系统类型|NTFS/FAT32|EXT3/EXT4/XFS..|
|Mounting Parameters|DriveLetter|MountPoint|
|OS所在根目录|C盘|/|

3. 文件系统层次 

Linux使用`/`分割路径，（Windows使用`\`），并且没有盘符。New drives作为目录挂载在单个文件系统中，通常再`/media`目录下。（举例来说一个CD-ROM `FEDORA`的目录可能是`/meida/FEDORA`） 

4. Boot Process 启动过程 
![这里写图片描述](https://img-blog.csdn.net/20170301214148261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 图形界面

## 会话管理

锁屏 `CTRL-ALT-L`

查看隐藏文件 `CTRL-H`

删除文件`.local/share/Trash/files`
## 安装和更新软件

![这里写图片描述](https://img-blog.csdn.net/20170301214616904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170301214627907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170301214638470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 查找Linux文档

## man

`man topic`
`man -f topic`列出包含topic的所有项相当于`whatis`
`man -k topic`列出关于topic的项目（可以不含topic这几个字本身）相当于`apropos`
`man 3 topic`列出第三章的相关内容 
`man -a topic`列出所有章的相关内容 
`cmd --help`列出关于某个指令的帮助
# 命令行操作

## 命令行模式选项
- sudo 
![这里写图片描述](https://img-blog.csdn.net/20170301224920400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170301225949759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 基本操作

![这里写图片描述](https://img-blog.csdn.net/20170301230155189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. Logging In and Out 

使用`ssh username@server.com`远程连接 

2. Rebooting and Shutting Down 
`shutdown`
`halt`和`poweroff`相当于`shutdown -h`
`reboot`相当于`shutdown -r`
```bash
$ sudo shutdown -h 10:00 "Shutting down for scheduled maintenance
```
- Locating Applications 搜索命令 

一般来说，可执行程序在/bin, /usr/bin,/sbin,/usr/sbin directories or under /opt.目录下。 

使用`which`可以定位应用，搜索命令所在路径和别名，如果`which`没有找到。可以是使用`whereis`，它会查找范围更大的目录。 
`whereis`搜索命令所在路径及帮助文档所在位置选项`-b`只查找可执行文件，`-m`只查找帮助文件。- 访问目录 

第一次登陆系统或打开终端时，默认目录是**家目录**。可以使用`echo $HOME`
|Command|Result|
|----|----|
|pwd|输出当前目录|
|cd ~或cd|转到家目录|
|cd..|转到上级目录|
|cd -|转到之前目录|

5. 绝对路径和相对路径 

6. 浏览文件系统 
`tree`命令可以浏览整个文件系统树，使用`tree -d`可以只看目录而不列出文件。|Command|Usage|
|----|----|
|cd/|更改到根目录|
|ls|展示当前目录内容|
|ls -a|展示所有文件（包括隐藏文件）|
|tree|展示文件系统树|

7. 硬链接和软（符号）链接 
`ln`可以用来创建硬链接和软链接（使用`-s`选项），也叫**symbolic**或**symlinks**。 

为file1创建一个硬链接 
`$ ln file1 file2`

使用命令`$ ls -li file1 file2`可以看到两个文件有相同的inode编号，说明只有一个文件，而另一个文件跟它链接在一起。 

符号链接 
`$ ln -s file1 file4`
`$ ls -li file1 file4`

file4不再是一个常规文件，它是一个链接文件，指向了file1，并且有不同的inode编号。 

符号链接不占文件系统的额外空间。它非常方便因为可以指向不同的地方。 

符号链接可以指向不同的甚至当前不存在的文件系统，当指向不存在的位置时，获得一个**dangling**link。 

导航目录历史 
`cd -`
`pushd`
`popd`
`dirs`显示目录信息
## 文件操作

查看文件
|Command|Usage|
|----|----|
|cat|查看不是很长的文件，不提供滚屏|
|tac|从文件末尾查看|
|less|用来查看更大的文件，Use / to search for a pattern in the forward direction and ? for a pattern in the backward direction.|
|tail|默认输出最后10行 使用-n参数更改行数|
|head|默认输出开头10行|

touch and mkdir 
`touch`创建文件或修改文件时间 
`$ touch <filename>`创建空文件 
`$ touch -t 03201600 myfile`设置特定时间戳 

mkdir创建目录 

rmdir删除目录，目录必须为空否则会失败。删除目录和目录中内容使用`rm -rf`

Removing a File|Command|Usage|
|----|----|
|mv|重命名|
|rm|删除文件|
|rm -f|强制删除|
|rm -i|交互式删除|

Renaming or Removing a Directory
|Command|Usage|
|----|----|
|mv|重命名目录|
|rmdir|删除空目录|
|rm -rf|强制递归删除目录|

![这里写图片描述](https://img-blog.csdn.net/20170302004350095?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 文件搜索

1.标准文件流 

文件流，描述符 

stdin 0 

stdout 1 

stderr 2 

2. I/O重定向 
`$ do_something < input-file`更改程序的输入，使得其读取file中的数据。 
`$ do_something > ouutput-file`，将程序的输出定向到file。 

因为stderrr和stdout不同，所以错误信息仍然会输出到终端。 

如果想要重定向stderr到一个不同的文件，需要使用stderr的文件描述符编号2， 
`$ do_something 2 > error-file`

将stdout和stderr的输出到相同文件 
`$ do_something > all-output-file 2>&1`
**bash**终端允许一个更简单的语法： 
`$ do_something >& all-output-file`

3.  管道pipes 

可以将一个指令的输出作为另一个指令的输入。 
`$ command1 | command2 | command3`

4. locate 
`locate`命令通过查询一个预先建立好的数据库搜索包含关键词的目录或文件，可以使用`grep`命令过滤查询结果 
`$ locate zip | grep bin`。 

locate命令所搜索的后台数据库路径 

/var/lib/mlocate 

locate命令使用的数据库是通过`updatedb`命令更新的，一般该命令一天执行一次，也可以手动执行。 
`/etc/updatedb.conf`配置文件 
![这里写图片描述](https://img-blog.csdn.net/20170305174447697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5. Wildcards 匹配文件名|Wildcard|Result|
|----|----|
|?|匹配单个字符|
|*|匹配任意字符串|
|[set]|匹配集合中的任意字符|
|[!set]|匹配任意不在集合中的字符|

6. 使用find在目录中查找 
`find [搜索范围] [搜索条件]`会列出所有当前目录和子目录下的文件。 

选项参数有 `-name`，只列出名字中含有特定模式的。 
`-iname`忽略含特定模式的。 
`-type`指定文件类型，d目录，l符号链接，f常规文件。 
`-user`按照所有者搜索 
`-nouser`按照没有所有者的文件 
`-mtion +10`查找10天前修改的文件 

-10 十天内修改的文件 

10 10天当天修改的文件 
`atime`文件访问时间 
`ctime`改变文件属性 
`-size`文件大小 
`-inum`i结点编号 
`find /etc -size +20k -a -size -50k`
`-a`逻辑与，`-o`逻辑或 

如果需要匹配，使用通配符匹配，通配符是完全匹配。 

7. 使用高级find选项 

find能够运行指令在其列出的结果上，使用`-exec`选项实现。 

查找并删除所有以.swp结尾的文件 
`$ find -name "*.swp" -exec rm {} ';'`
`-exec/-ok 命令 {}\;`对搜索结果执行操作 
`{}`花括号是一个占位符，用来填充find的结果。 

注意该指令要以`';'`(包括单引号)或`\;`结尾。 

也可以使用`-ok`选项，会提示确认再执行指令，更安全。 

8. 基于时间和大小查找文件 
![这里写图片描述](https://img-blog.csdn.net/20170302105110956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 安装软件

![这里写图片描述](https://img-blog.csdn.net/20170302105727866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 文件操作

## 文件系统
- 分区
- 挂载点 

在使用一个文件系统前，需要将其挂载到文件系统树上。如果挂载到一个非空的目录上，该目录内之前的内容将会被覆盖并且不可访问，直到该文件系统被解除挂载。 

所以通常挂载点都是空目录。 
`mount`可以将一个文件系（可以使本地的也可以是网络的）附加在文件系统树上面。 

参数包括`device node`和`mount point`
`$ mount /dev/sda5 /home`

单独使用`mount`可以列出当前已挂载的文件系统。 

命令`df -Th`(disk-free)将会展示关于挂载文件系统使用统计量的信息。- 网络文件系统 
**服务器端NFS**

使用`sudo service nfs start`命令启动NFS。 

文本文件`/etc/exports`包括了主机将要和其他系统通过NFS共享的目录和访问权限。每一项具有如下格式 
`/projects *.example.com(rw)`

该项允许目录`/projects`通过NFS以读写(rw)权限被挂载并与`example.com`域的主机共享。 

修改完配置文件后，使用`exportfs -av`命令通知Linux更新，或通过`sudo service nfs restart`重启NFS，但是这会比较慢。 
**客户机端NFS**

通过修改`/etc/fstab`文件完成系统启动后自动挂载远程文件系统。文件中的一项如下： 
`servername:/projects /mnt/nfs/projects nfs defaults 0 0`

也可以通过下命令一次性挂载而不重新启动 
`$ mount servername:/projects /mnt/nfs/projects`

如果`/etc/fstab`文件没有被修改，那么远程挂载在下次启动时将不会出现。
## 文件系统架构
- /bin和/sbin目录 

/bin目录包括可执行二进制，单用户模式下的基本命令和所有系统用户需要的基本命令。如`ps`,`ls`,`cp`。 

单用户模式下的非基本命令被放在`/usr/bin`目录，`/sbin`目录存放管理员所需的二进制文件,如`ifconfig`,`shutdown`。还有一个 
`/usr/bin`目录存放不太必要的管理员程序。 

现代Linux操作系统`/usr/bin`和`/bin`实际上是一样的。`/sbin`和`/usr/sbin`也是。- /dev目录 

包含设备结点。
- /var和/etc目录 
`/var`目录中包含可能改变大小和内容的文件。如 
- 系统日志文件/var/log
- 包和数据库文件/var/lib
- 打印队列/var/spool
- 临时文件/var/tmp

`/etc`目录是系统配置文件的存放地，不包含二进制程序，包含一些可执行脚本。 

4. /boot目录 

该目录包含很少的系统启动必需的文件。 

如vmlinz,initramfs,config,System.map 

5. /lib和/media目录 

/lib目录包括了/bin和/sbin中的应用程序运行所需的通用代码。库文件名以ld或lib开头。 

大部分是dll动态链接库，也叫做共享库，共享对象SO. 

/media目录包括可以移除的媒介（如CD,DVD,USB），linux自动挂载这些可移除媒介到/media 

6. /目录下其他目录|目录名|用途|
|----|----|
|/opt|可选应用软件包|
|/sys|Virtual pseudo-filesystem giving information about the system and the hardware. Can be used to alter system parameters and for debugging purposes|
|/srv|Site-specific data served up by the system. Seldom used.|
|/tmp|Temporary files; on some distributions erased across a reboot and/or may actually be a ramdisk in memory.|
|/usr|多用户应用，工具和数据|

7. /usr目录下的子目录 
![这里写图片描述](https://img-blog.csdn.net/20170302125910392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 比较文件和文件类型

`diff`用来比较文件和目录。选项参数如下
|diff Option|Usage|
|----|----|
|-c|Provides a listing of differences that include 3 lines of **context** before and after the lines differing in content|
|-r|Used to **recursively** compare subdirectories as well as the current directory|
|-i|忽略大小写|
|-w|忽略空格和制表符|

`diff <filename1> <filename2>`

2. 使用diff3 和 patch 
`diff3`可以同时将两个文件与同一个文件进行比较 
`$ diff3 myfile file1 file2`
![这里写图片描述](https://img-blog.csdn.net/20170302133611949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3. file命令查看文件类型
## 备份和压缩数据

使用`cp`或`rsync`可以备份数据。 
`rsync`效率更高，因为它会检查待复制的文件是否已经存在，而且只会拷贝改变的部分，相当于增量备份。 
`cp`只能在本地机器上进行拷贝工作（除非是一个通过NFS挂载的文件系统）。但是`rsync`可以在不同机器上进行复制，位置通过如下格式指定 
`target:path`，target格式可以是`[user@]host`，可选项是当远程机器和本地的用户不同时指定。 

1. 使用rsync 
`$ rsync -r project-X archive-machine:archives/project-X`

使用`-dry-run`参数来测试`rsync`命令是否产生想要的结果。 
`rsync sourcefile destinationfile`

2. 压缩数据|Command|Usage|
|----|----|
|gzip|最频繁使用的压缩工具|
|bzip2|产生比gzip更小的压缩结果|
|xz|最空间高效的压缩工具|
|zip|经常用来检查和解压来自其他系统的压缩文件|

`tar`工具通常用来打包和一次性解压缩。 

3. 使用gzip压缩数据
|Command|Usage|
|----|----|
|gzip *|将当前目录下所有文件分别压缩，添加.gz后缀|
|gzip -r projectx|将指定目录及其子目录下的所有文件压缩|
|gunzip foo|解压缩foo.gz中的foo。在底层，gunzip实际上和gzip -d一样|

4. 使用bzip2压缩数据 
`bzip2`使用了与`gzip`不同的压缩算法，可以产生更小的文件但需要更长的时间，通常用来压缩大文件。
|Command|Usage|
|----|----|
|bzip2 *|压缩当前目录下所有文件并添加后缀.bz2|
|bunzip2 *.bz2|解压以.bz2结尾的所有文件，和bzip2 -d 一样|

5. 使用xz压缩数据 
`xz`是一种最空间高效的压缩工具，牺牲了压缩速度。 
![这里写图片描述](https://img-blog.csdn.net/20170302141100575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6. 使用zip处理文件 
`zip`一般不用来在Linux中压缩文件，而用来校验和解压缩来自Windows的压缩文件。|Command|Usage|
|----|----|
|zip backup *|压缩当前目录下的所有文件到backup.zip中|
|zip -r backup.zip ~|将家目录及其子目录中的所有文件打包至backup.zip|
|unzip backup.zip|解压到当前目录下|

7. 使用tar压缩和解压缩 

格式 
`tar [主选项+辅选项] 文件或目录`

主选项 
`-c`，创建新的档案文件 
`-r`把要催当的文件追加到档案末尾 
`-t`列出档案文件内容 
`-u`更新文件 
`-x`从档案文件中释放文件 

辅助选项： 
`-f`使用档案文件或设备，通常必选。 
`-k`保存已经存在的文件，还原过程不进行替换。 
`-m`还原文件时，把所有文件的修改时间设定为现在。 
`-M`创建多卷的档案文件，以便在几个磁盘中存放。 
`-v`详细报告tar处理的文件信息。 
`-w`每一步都要求确认。 
`-z`用gzip来压缩或解压缩。|Command|Usage|
|----|----|
|`$ tar xvf mydir.tar`|将mydir.tar中的所有数据提取到mydir中|
|`$ tar zcvf mydir.tar.gz mydir`|用gzip创建归档并压缩|
|`$ tar jcvf mydir.tar.bz2 mydir`|用bz2创建归档并压缩|
|`$ tar jcvf mydir.tar.xz mydir`|用xz创建归档并压缩|
|`$ tar xvf mydir.tar.gz`|提取mydir.tar.gz中的所有文件到mydir中。注意：不需要告诉tar是用gzip格式|

也可以将归档和压缩的过程分开：

```bash
$ tar mydir.tar mydir; gzip mydir.tar
$ gunzip mydir.tar.gz; tar xvf mydir.tar
```

但是这样比较慢并且浪费了空间，创建了不必要的中间`.tar`中间文件。 

8. 硬盘之间的拷贝 
![这里写图片描述](https://img-blog.csdn.net/20170302141734395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 用户环境

## 账户，用户和组
- 确认当前用户 
`who`列出当前登陆用户 
`whoami`列出当前用户 
`who`的`-a`选项会列出详细信息- 添加和删除用户 
`$ sudo useradd newuser`
`$ sudo userdel newuser`

删除用户时使用`-r`选项可以同时删除该用户的家目录。 

使用`id`可以查看当前用户信息，使用 
`id username`查看指定用户信息。- 添加和删除用户组 
`$ sudo /usr/sbin/groupadd newgrooup`
`$ sudo /usr/sbin/groupdel newgroup`

向一个已经存在的组添加用户使用`usermode`，可以先查看用户已经所属的组`$ group username`

，然后使用`$ sudo /usr/sbin/usermod -G newgroup username`为用户添加组。 

删除一个用户所属的组要将其保留下来的组作为参数传递 
`$ sudo /usr/sbin/usermod -G newgroup username`

这样用户所属组只有`newgroup`存在。- 提升root权限 

使用`su`可以转换到管理员模式，使用`sudo`可以临时以管理员权限运行指令。 
**sudo**配置文件存储在`/etc/sudoers`文件和`/etc/sudoers.d`目录中。默认情况下sudoers.d目录为空。- Startup Files 

当第一次进入Linux系统时，`/etc/profile`会被读取，之后会按序查找以下文件 
`~/.bash_profile`,`~/.bash_login`,`~/.profile`。**Linux 登陆shell会加载搜索到的第一个文件并忽略后面的文件。**

但是，**每当开启一个新的shell或终端窗口，并没有执行一个完整的系统登陆**；只有`~/.bashrc`文件被读取加载，尽管这个文件在系统登陆时没有被读取。大多数发行版和用户将`~/.bashrc`文件包括进三个启动文件之一。在**Ubuntu,openSuse,CentOS**版本中，用户必须将更改`~/.bash_profile`文件来包括`~/.bashrc`文件。 

The `.bash_profile` will have certain extra lines, which in turn will collect the required customization parameters from `.bashrc`.
## 环境变量

使用`set`,`env`,`export`命令可以查看环境变量。 

1. 设置环境变量 

默认情况下，在一个脚本中创建的变量只能在当前的shell中使用，子进程不能访问。如果想让子进程访问，需要使用`export`命令。|Task|Command|
|----|----|
|查看特定变量的值|echo $SHELL|
|设置新变量|export VARIABLE=value (or VARIABLE=value; export VARIABLE)|
|永久添加变量|1.Edit ~/.bashrc and add the line export VARIABLE=value 2.Type source ~/.bashrc or just. ~/.bashrc (dot ~/.bashrc); or just start a new shell by typing  bash|

2. HOME变量 

代表家目录，也是用户登陆目录。不带参数的cd命令会跳转到HOME所指的目录。等同于~。 

3. PATH变量 

PATH是定义系统搜索命令的路径。路径用`:`分割，空路径或`./`代表当前目录。 

添加一个私有的`bin`目录到PATH：
```bash
$ export PATH=$HOME/bin:$PATH
$ echo $PATH
/home/me/bin:/usr/local/bin:/usr/bin:/bin/usr
```
- PS1变量 
![这里写图片描述](https://img-blog.csdn.net/20170302152911088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- SHELL变量 
![这里写图片描述](https://img-blog.csdn.net/20170302153045434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 使用历史命令

**bash**保留了历史命令，使用方向键可以查看，使用`history`命令也可以查看。历史命令存储在`~/.bash_history`中。 

1. 使用History环境变量 
`HISTFILE`存储历史文件位置 
`HISTFILESIZE`存储历史文件最大行数。 
`HISTSIZE`存储当前会话历史文件的最大行数。 

2. 寻找和使用历史命令|key|Usage|
|----|----|
|Up/Down arrow key|浏览历史命令列表|
|!!(bang-bang)|执行上一条命令|
|CTRL-R|搜索之前的命令|

3. 执行历史命令
|Syntax|Task|
|----|----|
|!|开始历史替代|
|!$|指向最后一个参数|
|!n|指向第n条命令|
|!string|指向最近的以string开头的命令|

4. 键盘快捷键 
![这里写图片描述](https://img-blog.csdn.net/20170302154657735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 创建别名

通常别名存在`~/.bashrc`文件中，因此在任何shell都可以使用他们。 
`alias`会列出当前定义的别名。注意别名定义中`=`两边没有空格，且定义要放在单引号或双引号包括的字符串中。

## 文件权限
|Command|Usage|
|----|----|
|chown|更改文件或目录的所有者|
|chgrp|更高组的所有权|
|chmod|更改文件的访问权限|

1. 文件访问权限和chmod 

2. chown 
`$ sudo chown user file-1`

3.chgrp





































