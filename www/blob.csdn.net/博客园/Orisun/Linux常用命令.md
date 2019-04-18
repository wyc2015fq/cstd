# Linux常用命令 - Orisun - 博客园







# [Linux常用命令](https://www.cnblogs.com/zhangchaoyang/articles/1853587.html)





/**************************************************************************************************/

/*** 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  ***/

/***　　　　　　　　　　　　　　　　　　　　文件管理命令　　　　　　　　　　　　　　　　　　　　　　　***/

/***　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　   ***/

/**************************************************************************************************/

ls -l/usr显示文件详细信息，选项-l是list的意思，表示要列出详细信息

文件属性-rwxr-xr-x,第一个“-”表示文件的性质：

-一般文件

d目录文件

l符号链接文件

b块设备文件

c字符设备文件

pwd当前路径

cd/root更改当前路径

mkdir-p /home/subdir创建目录，选项-p表示要创建绝对路径，不要-p就是相对路径

rmdirdata删除目录，该目录必须是空目录

cathello查看文件内容

head--line=N hello查看文件的前N行

tail--line=N hello查看文件的末N行

cp/root/install.log install.log.bak

复制文件

rm -rfdata删除目录/文件。-r是递归删除，-f是强制删除，删除文件时系统不会提示确认

chmod<a|u|g|o><+|-|=><权限字符串><文件名>

a:allu:userg:groupo:other|:多选一

+:增加权限-：减少权限=：设定权限

如chmodg+wx install.pl

chmod741 install.pl741二进制为111100001,等价于-rwxr----x

chown<新拥有者账号><带路径的文件名>

ln/x/y /z硬链接。y文件在存储介质上只有一份，只不过是把它要路径x和z下都进行了登记。要删除文件y必须删除所有的链接，即rm/x/y与rm /z/y

ln -s<带路径的文件或子目录><符号链接名>

ln -s/z/l/k/a /x/c这样一来/x/c相当于是/z/l/k/a的一个“快捷方式”

mount[-参数] [设备名称][挂载点]

常用参数如下：

-r文件只读

-v详细显示安装信息

-w默认选项，文件可写

-t指定设备文件类型，常见的有：

ext3

vfat:FAT32

ntfs

nfs:网络文件系统

auto:自动检测文件系统

ISO9600:CD-ROM光盘

-o指定挂载文件系统时的选项，有的也可写在/etc/fstab中。常见的有：

defaults:使用所有的默认选项（auto,nouser,rw,suid）

exec/noexec:允许/不允许执行二进制代码

user/nouser:允许/不允许一般用户挂载

codepage=XXX:代码页

iocharset=XXX:字符集

ro:只读方式挂载

rw:读写方式挂载

注意：挂载点必须是个已存在的目录，这个目录不能为空，但挂载后这个目录以前的内容将不可用，umount以后恢复正常。

挂载Windows下的D盘，一般在Linux下这个分区对应/dev/hda5:

mount-t vfat -o iocharset=gdb2312 codepage=936 /dev/hda5/mnt/hda5如果编码还有问题，尝试用iocharset=utf8

mount后中文显示问号表明系统中没有可识别的中文字体，需要先安装中文字体。中文显示为乱码说明mount默认使用的文件编码与文件的实际编码不一致，codepage指定文件系统的代码页，中文简体是936,iocharset指定字符集，中文简体一般用cp936或gb2312。

mount-o umask=000,iocharset=cp936 /dev/hda5 /mnt/hda5普通用户可写

在计算机没有其他SCSI设备和USB外设的情况下，插入U盘的路径是/dev/sda1,用如下命令挂载：

mount/dev/sda1 /mnt/u

mount-o loop linux.iso /mnt/iso直接挂载ISO文件

把命令写/etc/fstab文件中，即可实现系统启动时的自动挂载：

/dev/hda5/mnt/hda5 vfat defaults,iocharset=cp936,rw 0 0

umount[-参数] 挂载点卸载命令

/**************************************************************************************************/

/*** 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  ***/

/***　　　　　　　　　　　　　　　　　　　　　　　　进程管理命令　　　　　　　　　　　　　　　　　***/

/*** 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  ***/

/**************************************************************************************************/

进程分为交互进程、批处理进程、守护进程。

启动Linux进程的四种方式：

（1）前台启动

ls-a显示所有文件（夹），包括隐藏的文件（夹）。

必须等待该命令返回后才能输入下一条命令。

（2）后台进程

用户可能继续在终端上操作无需等待该进程完成。启动后台进程方式为：<命令>&

makedep>/tmp/dep.txt &在后台检查依赖关系，并将结果输出到/tmp/dep.txt中

（3）定时进程

下面的命令将在当天的11:15将文件/bin/bash以电子邮件的形式发给orisun用户：

#at11:15

at>cat /bin/bash | mail -s "bash" orisun

at><EOT>

"<EOT>"表示按下Ctrl+D组合键

at-l查看系统内准备定时执行的作业

at -r<作业号>删除正待执行的一项作业

（4）周期执行

at只能执行一次，cron可以周期重复执行

cron命令在系统启动时由一个shell脚本自动运行，进入后台。cron服务每分钟被唤醒一次，搜索/var/spool/cron目录，寻找以/etc/passwd中的用户命名的crontab文件，查看当前是否有命令需要运行，没有就继续进入休眠状态。命令执行结果以E-mail形式传送给crontab的所有者或指定的用户。

ps（processstate）查看瞬间正在运行的进程详细信息

-a所有进程

-w显示加宽以显示较多信息

-aux显示所有包含其他使用者的进程信息，x表示显示没有控制终端的进程

aux的格式输出如下：

USER进程拥有者

PID进程ID

%CPU占用CPU百分比

%MEM占用内存百分比

VSZ占用虚拟内存大小

RSS占用常驻内存大小

TTY终端编号

STAT进程状态

START进程开始时间

TIME进程执行时间

COMMAND该进程对应的命令

top同ps，但它是动态刷新的

kill通过向进程发送信号来结束进程，没有指定信号默认为TERM信号。编号为9的SIGKILL信号几乎可以杀死所有的进程。

kill[-s 信号］ [-a] 信号

kill-l [信号]

kill-9 1234杀死进程号为1234的进程

w查看系统当前用户情况

/**************************************************************************************************/

/*** 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  ***/

/***　　　　　　　　　　　　　　　　　　　　压缩和解压命令　　　　　　　　　　　　　　　　　　　　***/

/***　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 ***/

/**************************************************************************************************/

tar(tapearchive)

tar[主选项+辅选项]文件或目录列表

主选项：

-c创建新的档案文件

-r把要归档的文件追加到档案文件的末尾

-t列出档案文件的内容

-u用新文件取代原备份文件

-x从档案文件中释放文件

辅选项

-b设定磁带机

-f使用档案文件或设备，该选项是必选的

-k保存已存在的文件

-m在还原文件时把所有文件的修改时间设定为现在

-M创建多卷的档案文件，以便在多个磁盘上存放

-v详细报告tar处理文件的信息

-z用gzip来解压/压缩文件。压缩时用了该选项，解压时也要用该选项。使用了该选项时，归档文件的后缀不是.tar而是.gz

/**************************************************************************************************/

/***　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 ***/

/***　　　　　　　　　　　　　　　　　　磁盘分区命令　　　　　　　　　　　　　　　　　　　　　　***/

/*** 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　  ***/

/**************************************************************************************************/

磁盘分区工具fdisk。Linux给IDE设备分配了一个有hd前缀的文件，为SCSI设备分配一个有sd前缀的文件。

fdisk[选项] <磁盘对应的设备名>

-l列出指定磁盘的分区表

-v详细显示指令的执行过程

root@orisun-desktop:/home/orisun#fdisk /dev/sda



WARNING:DOS-compatible mode is deprecated. It's strongly recommended to

switchoff the mode (command 'c') and change display units to

sectors(command 'u').



Command(m for help): m

Commandaction

a  toggle a bootable flag

b  edit bsd disklabel

c  toggle the dos compatibility flag

d  delete a partition

l  list known partition types

m  print this menu

n  add a new partition

o  create a new empty DOS partition table

p  print the partition table

q  quit without saving changes

s  create a new empty Sun disklabel

t  change a partition's system id

u  change display/entry units

v  verify the partition table

w  write table to disk and exit

x  extra functionality (experts only) 






















