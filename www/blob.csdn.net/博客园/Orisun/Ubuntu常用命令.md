# Ubuntu常用命令 - Orisun - 博客园







# [Ubuntu常用命令](https://www.cnblogs.com/zhangchaoyang/articles/1932182.html)






.deb文件在ubuntu下直接双击即可安装软件

.tar.bz2和.tar.bz2

解压：tar jxvf FileName.tar.bz2

压缩：tar jcvf FileName.tar.bz2 DirName

.tar.gz 和 .tgz

解压：tar zxvf FileName.tar.gz

压缩：tar zcvf FileName.tar.gz DirName

.zip

解压：unzip FileName.zip

压缩：zip FileName.zip DirName

.rar

解压：rar x FileName.rar

压缩：rar a FileName.rar DirName

rar请到：http://www.rarsoft.com/download.htm 下载！

解压后请将rar_static拷贝到/usr/bin目录（其他由$PATH环境变量指定的目录也可以）：

[root@www2 tmp]# cp rar_static /usr/bin/rar

pwd当前路径

mkdir -p /home/subdir创建目录，选项-p表示要创建绝对路径，不要-p就是相对路径

rmdir data删除目录，该目录必须是空目录

mknod创建设备

rm -rf data删除目录/文件。-r是递归删除，-f是强制删除，删除文件时系统不会提示确认

chmod <a|u|g|o><+|-|=><权限字符串> <文件名>

a:allu:userg:groupo:other|:多选一

+:增加权限-：减少权限=：设定权限

如chmod g+wx install.pl

chmod 741 install.pl741二进制为111100001,等价于-rwxr----x

chown <新拥有者账号> <带路径的文件名>

ln /x/y /z硬链接。y文件在存储介质上只有一份，只不过是把它要路径x和z下都进行了登记。要删除文件y必须删除所有的链接，即rm /x/y与rm /z/y

ln -s <带路径的文件或子目录> <符号链接名>

ln -s /z/l/k/a /x/c这样一来/x/c相当于是/z/l/k/a的一个“快捷方式”

mount [-参数] [设备名称] [挂载点]

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

注意：挂载点必须是个已存在的目录，这个目录不能为空，但挂载后这个目录以前的内容将不可用，umount以后恢复正常。

mount -o loop linux.iso /mnt/iso直接挂载ISO文件

umount [-参数] 挂载点卸载命令

用户可能继续在终端上操作无需等待该进程完成。启动后台进程方式为：<命令> &

make dep>/tmp/dep.txt &在后台检查依赖关系，并将结果输出到/tmp/dep.txt中

ps（process state）查看瞬间正在运行的进程详细信息

-a所有进程

top同ps，但它是动态刷新的

kill 通过向进程发送信号来结束进程，没有指定信号默认为TERM信号。编号为9的SIGKILL信号几乎可以杀死所有的进程。

kill [-s 信号］ [-a] 信号

kill -l [信号]

kill -9 1234杀死进程号为1234的进程

w查看系统当前用户情况

磁盘分区工具fdisk。Linux给IDE设备分配了一个有hd前缀的文件，为SCSI设备分配一个有sd前缀的文件。

fdisk [选项] <磁盘对应的设备名>

-l列出指定磁盘的分区表

-v详细显示指令的执行过程

root@orisun-desktop:/home/orisun# fdisk /dev/sda

free显示RAM信息

df磁盘使用情况-h(--human-readable)-m指定单位为兆

du查看当前目录下各个文件所占的空间

du -s仅查看当前目录所占空间





ZZ 保存并退出 

:x 保存并退出 

w,W 跳至后一个字(W忽略标点) 

b,B 跳至前一个字（B忽略标点） 

$ 至行尾 

^ 至本行第一个非空字符 

0 至行首 

u 撤消最近一次对文本的改动。标准版的vi只能执行一级撤消，而vim则可以多级 

U 行撤消命令，用来撤消从光标最近一次移到该行时起对光标所在行做的全部改动 

很多大致是由指令与范围所构成。例如 dw 是由删除指 d 与范围 w 所组成，代表删除一个字 d(elete) w(ord) 。 



指令列表有： 

d 删除(delete) 

y 复制(yank) 

p 放置(put) 

c 修改(change) 

范围可以是下列几个： 

e 光标所在位置到该字的最后一个字母 

w 光标所在位置到下个字的第一个字母 

b 光标所在位置到上个字的第一个字母 

$ 光标所在位置到该列的最后一个字母 

0 光标所在位置到该列的第一个字母 

) 光标所在位置到下个句子的第一个字母 

( 光标所在位置到该句子的第一个字母 

} 光标所在位置到该段落的最后一个字母 

{ 光标所在位置到该段落的第一个字母 

/pattern 移至下一个包含pattern的行 

?pattern 移至上一个包含pattern的行 

/ 往下重复查找 

? 往上重复查找 

n 在同一方向重复查找 

N 在相反方向重复查找 

修改vim特性在.vimrc中：

" 自动缩进

set autoindent

set cindent



" Tab键的宽度

set tabstop=4



" 统一缩进为4

set softtabstop=4

set shiftwidth=4

" 显示行号

set number

" 高亮显示匹配的括号

set showmatch

" 为C程序提供自动缩进

set smartindent

" F5编译和运行C程序，F6编译和运行C++程序

" 请注意，下述代码在windows下使用会报错

" 需要去掉./这两个字符



" C的编译和运行

map <F5> :call CompileRunGcc()<CR>

func! CompileRunGcc()

exec "w"

exec "!gcc % -o %<"

exec "! ./%<"

endfunc



" C++的编译和运行

map <F6> :call CompileRunGpp()<CR>

func! CompileRunGpp()

exec "w"

exec "!g++ % -o %<"

exec "! ./%<"

endfunc





在 Ubuntu 系统中有两种设置环境变量 PATH 的方法。第一种适用于为单一用户设置 PATH，第二种是为全局设置 PATH。

第一种方法：

在用户主目录下有一个 .bashrc 文件，可以在此文件中加入 PATH 的设置如下：

export PATH=”$PATH:/your path1/:/your path2/…..”

注意：每一个 path 之间要用 “:“ 分隔。

注销重启 X 就可以了。

第二种方法：

在 /etc/profile中增加。

PATH="$PATH:/home/zhengb66/bin"

export PATH





locate在整个磁盘上查找文件

find在给定目录下查找文件，默认是递归查找，用-name指定文件名

grep搜索文本



apt-get update 更新本地所有软件。连接到apt服务器，更新本地软件包列表

在apt配置文件(/etc/apt/sources.list)下可以设置apt服务器

apt-get --purge remove sshfs--purge表示软件的配置文件也不并删除

apt-cache search firefox寻找可安装的资源

apt下载的安装包都保存在了/var/cache/apt/archives/下，使用apt-get clean清除这些.deb包



ping -c 3 www.baidu.com发送三个数据包后即停止

调用ifconfig后出来三个网络接口ath0是无线网卡，eth0是以太网卡，lo是环回

#ifconfig eth0 hw ether 00:14:CC:00:1A:00更改MAC，可见通过ifconfig显示的MAC都是虚拟的，可以随意更改





service ssh status查看服务运行状态

sudo /etc/init.d/ssh start启动服务

sudo /etc/init.d/ssh stop关闭服务

sudo /etc/init.d/ssh restart重启服务



command & > file将命令输出和标准错误输出一起重定向到file

command 2 > file将标准错误输出重定向到file

command1 | & command2连同标准错误输出一起送入管道 













