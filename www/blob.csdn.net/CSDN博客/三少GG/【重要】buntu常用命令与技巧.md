# 【重要】buntu常用命令与技巧 - 三少GG - CSDN博客
2010年05月02日 16:16:00[三少GG](https://me.csdn.net/scut1135)阅读数：989标签：[ubuntu																[kill																[linux																[search																[access](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
**Ubuntu常用命令与技巧(w1) **
**sudo apt－get install 软件名 安装软件命令 **
sudo nautilus 打开文件（有root权限） 
**su root 切换到“root” **
ls 列出当前目录文件（不包括隐含文件） 
**ls -a 列出当前目录文件（包括隐含文件） ls -l 列出当前目录下文件的详细信息 **
cd .. 回当前目录的上一级目录 
cd - 回上一次所在的目录 
**cd ~ 或 cd 回当前用户的宿主目录 **
mkdir 目录名 创建一个目录 
rmdir 空目录名 删除一个空目录 
**rm 文件名 文件名 删除一个文件或多个文件 rm -rf 非空目录名 删除一个非空目录下的一切 **
mv 路经/文件 /经/文件 移动相对路经下的文件到绝对路经下 
mv 文件名 新名称 在当前目录下改名 
**find 路经 -name “字符串” 查找路经所在范围内满足字符串匹配的文件和目录 **
**fdisk fdisk -l 查看系统分区信息 **
fdisk fdisk /dev/sdb 为一块新的SCSI硬盘进行分区 
chown chown root /home 把/home的属主改成root用户 
chgrp chgrp root /home 把/home的属组改成root组 
**Useradd 创建一个新的用户 **
Groupadd 组名 创建一个新的组 
Passwd 用户名 为用户创建密码 
Passwd -d用户名 删除用户密码也能登陆 
Passwd -S用户名 查询账号密码 
Usermod -l 新用户名 老用户名 为用户改名 
Userdel–r 用户名 删除用户一切 
**tar -c 创建包 –x 释放包 -v 显示命令过程 –z 代表压缩包 **
tar –cvf benet.tar /home/benet 把/home/benet目录打包 
tar –zcvf benet.tar.gz /mnt 把目录打包并压缩 
tar –zxvf benet.tar.gz 压缩包的文件解压恢复 
**tar –jxvf benet.tar.bz2 解压缩 **
**make 编译 **
**make install 安装编译好的源码包 **
reboot Init 6 重启LINUX系统 
Halt Init 0 Shutdown –h now 关闭LINUX系统 
**uname -a 查看内核版本 cat /etc/issue 查看ubuntu版本 lsusb 查看usb设备 **
sudo ethtool eth0 查看网卡状态 
cat /proc/cpuinfo 查看cpu信息 
**lshw 查看当前硬件信息 sudo fdisk -l 查看磁盘信息 df -h 查看硬盘剩余空间 **
free -m 查看当前的内存使用情况 
ps -A 查看当前有哪些进程 
kill 进程号(就是ps -A中的第一列的数字)或者 killall 进程名( 杀死一个进程) 
kill -9 进程号 强制杀死一个进程 
**常用apt命令：**
apt-cache search package 搜索包 
apt-cache show package 获取包的相关信息，如说明、大小、版本等 **sudo apt-get install package 安装包 **
sudo apt-get install package - - reinstall 重新安装包 
sudo apt-get -f install 修复安装”-f = –fix-missing” **sudo apt-get remove package 删除包 sudo apt-get remove package - - purge 删除包，包括删除配置文件等 **
**sudo apt-get update 更新源 **
**sudo apt-get upgrade 更新已安装的包 **
sudo apt-get dist-upgrade 升级系统 
sudo apt-get dselect-upgrade 使用 dselect 升级 
apt-cache depends package 了解使用依赖 
apt-cache rdepends package 是查看该包被哪些包依赖 
sudo apt-get build-dep package 安装相关的编译环境 
**apt-get source package 下载该包的源代码 sudo apt-get clean && sudo apt-get autoclean 清理无用的包 sudo apt-get check 检查是否有损坏的依赖 **
清理所有软件缓存（即缓存在/var/cache/apt/archives目录里的deb包 ） 
sudo apt-get clean 
**删除系统不再使用的孤立软件 sudo apt-get autoremove**
2.
buntu创建、删除文件及文件夹，强制清空回收站方法
mkdir目录名创建一个目录
rmdir空目录名删除一个空目录
rm文件名文件名删除一个文件或多个文件
rm -rf非空目录名删除一个非空目录下的一切
touch文件名创建一个空文件
## 强制清空回收站
ubuntu8.04以前的版本
sudo rm -fr $HOME/.Trash/*
ubuntu 8.04
sudo rm -fr $HOME/.local/share/Trash/files/*
下面是touch
命令用法
-a                     只更改访问时间
-c, --no-create        不创建任何文件
-d, --date=字符串        使用<字符串>表示时间而非当前时间
-f                     (忽略)
-m                     只更改修改时间
-r, --reference=文件   使用指定<文件>的时间属性而非当前时间
-t STAMP               使用 [[CC]YY]MMDDhhmm[.ss]格式的时间而非当前时间
--time=WORD            使用 WORD指定的时间：access、atime、use都等于-a
                           选项的效果，而 modify、mtime 等于 -m 选项的效果
      --help     显示此帮助信息并离开
      --version 显示版本信息并离开
请注意，-d 和 -t 选项可接受不同的时间/日期格式。
[root@linux ~]# cp [options] source1 source2 source3 .... directory
参数：
-a  ：相当于 -pdr 的意思；
-d  ：若来源档为连结档的属性(link file)，则复制连结档属性而非档案本身；
-f  ：为强制 (force) 的意思，若有重复或其他疑问时，不会询问使用者，而强制复制；
-i  ：若目的档(destination)已经存在时，在覆盖时会先询问是否真的动作！
-l  ：进行硬式连结 (hard link) 的连结档建立，而非复制档案本身；
-p  ：连同档案的属性一起复制过去，而非使用预设属性；
**-r  ：递回持续复制，用于目录的复制行为；**
-s  ：复制成为符号连结档 (symbolic link)，亦即‘捷径’档案；
-u  ：若 destination 比 source 旧才更新 destination ！
最后需要注意的，如果来源档有两个以上，则最后一个目的档一定要是‘目录’才行！
范例：
范例一：将家目录下的 .bashrc 复制到 /tmp 下，并更名为 bashrc
[root@linux ~]# cd /tmp
[root@linux tmp]# cp ~/.bashrc bashrc
[root@linux tmp]# cp -i ~/.bashrc bashrc
cp: overwrite `basrhc'? n
# 重复作两次动作，由于 /tmp 底下已经存在 bashrc 了，加上 -i 参数，
# 则在覆盖前会询问使用者是否确定！可以按下 n 或者 y 呢！
# 但是，反过来说，如果不想要询问时，则加上 -f 这个参数来强制直接覆盖！
范例二：将 /var/log/wtmp 复制到 /tmp 底下
[root@linux tmp]# cp /var/log/wtmp . <==想要复制到目前的目录，最后的 . 不要忘
[root@linux tmp]# ls -l /var/log/wtmp wtmp
-rw-rw-r--  1 root utmp 71808 Jul 18 12:46 /var/log/wtmp
-rw-r--r--  1 root root 71808 Jul 18 21:58 wtmp
# 注意到了吗？！在不加任何参数的情况下，档案的所属者会改变，连权限也跟着改变了～
# 这是个很重要的特性！要注意喔！还有，连档案建立的时间也不一样了！
# 如果您想要将档案的所有特性都一起复制过来，可以加上 -a 喔！
[root@linux tmp]# cp -a /var/log/wtmp wtmp_2
[root@linux tmp]# ls -l /var/log/wtmp wtmp_2
-rw-rw-r--  1 root utmp 71808 Jul 18 12:46 /var/log/wtmp
-rw-rw-r--  1 root utmp 71808 Jul 18 12:46 wtmp_2
# 了了吧！整个资料特性完全一模一样ㄟ！真是不赖～这就是 -a 的特性！
**范例三：复制 /etc/ 这个目录下的所有内容到 /tmp 底下[root@linux tmp]# cp /etc/ /tmpcp: omitting directory `/etc'   <== 如果是目录，不能直接复制，要加上 -r 的参数[root@linux tmp]# cp -r /etc/ /tmp# 还是要再次的强调喔！ -r 是可以复制目录，但是，档案与目录的权限会被改变～# 所以，也可以利用 cp -a /etc /tmp 来下达指令喔！**
范例四：将范例一复制的 bashrc 建立一个连结档 (symbolic link)
[root@linux tmp]# ls -l bashrc
-rw-r--r--  1 root root 395 Jul 18 22:08 bashrc
[root@linux tmp]# cp -s bashrc bashrc_slink
[root@linux tmp]# cp -l bashrc bashrc_hlink
[root@linux tmp]# ls -l bashrc*
-rw-r--r--  2 root root 395 Jul 18 22:08 bashrc
-rw-r--r--  2 root root 395 Jul 18 22:08 bashrc_hlink
lrwxrwxrwx  1 root root   6 Jul 18 22:31 bashrc_slink -> bashrc
# 那个 bashrc_slink 是由 -s 的参数造成的，建立的是一个‘捷径’，
# 所以您会看到在档案的最右边，会显示这个档案是‘连结’到哪里去的！
# 至于那个 bashrc_hlink 有趣了！建立了这个档案之后， bashrc 与 bashrc_hlink 
# 所有的参数都一样，只是，第二栏的 link 数改变成为 2 了～而不是原本的 1 喔！
# 这两种连结的方式的异同，我们会在下一章里面进行介绍的！
范例五：若 ~/.bashrc 比 /tmp/bashrc 才复制过来
[root@linux tmp]# cp -u ~/.bashrc /tmp/bashrc
# 这个 -u 的特性，是在目标档案与来源档案有差异时，才会复制的。
# 所以，比较常被用于‘备份’的工作当中喔！ ^_^
范例六：将范例四造成的 bashrc_slink 复制成为 bashrc_slink_2
[root@linux tmp]# cp bashrc_slink bashrc_slink_2
[root@linux tmp]# ls -l bashrc_slink*
lrwxrwxrwx  1 root root   6 Jul 18 22:31 bashrc_slink -> bashrc
-rw-r--r--  1 root root 395 Jul 18 22:48 bashrc_slink_2
# 这个例子也是很有趣喔！原本复制的是连结档，但是却将连结档的实际档案复制过来了
# 也就是说，如果没有加上任何参数时，复制的是原始档案，而非连结档的属性！
# 若要复制连结档的属性，就得要使用 -d 或者 -a 的参数了！
范例七：将家目录的 .bashrc 及 .bash_history 复制到 /tmp 底下
[root@linux tmp]# cp ~/.bashrc ~/.bash_history /tmp
# 可以将多个资料一次复制到同一个目录去！
## Ubuntu更改文件夹权限
Ubuntu的许多操作是在终端中进行的，通过sudo命令管理的文件是由root持有权限的，一般用户是无法改变的。在图形界面上，我们可以通过属性中的权限选项夹进行操作。但是一旦文件的属性显示当前用户没有读写权力时，无法在图形界面上修改权限。
常用方法如下：
sudo chmod 600 ××× （只有所有者有读和写的权限）
sudo chmod 644 ××× （所有者有读和写的权限，组用户只有读的权限）
sudo chmod 700 ××× （只有所有者有读和写以及执行的权限）
sudo chmod 666 ××× （每个人都有读和写的权限）
**sudo chmod 777 ××× （每个人都有读和写以及执行的权限）**
其中×××指文件名（也可以是文件夹名，不过要在chmod后加-ld）。
解释一下，其实整个命令的形式是
sudo chmod -（代表类型）×××（所有者）×××（组用户）×××（其他用户）
三位数的每一位都表示一个用户类型的权限设置。取值是0～7，即二进制的[000]~[111]。
这个三位的二进制数的每一位分别表示读、写、执行权限。
如000表示三项权限均无，而100表示只读。这样，我们就有了下面的对应：
0 [000] 无任何权限
4 [100] 只读权限
6 [110] 读写权限
7 [111] 读写执行权限
现在看上面的几个常用用法就非常清楚了。试着自己来修改权限吧
最后同时附上查询文件（或文件夹）权限的命令
ls -l 文件名称 （文件夹将-l改为-ld）。
原文地址： [http://liufeng2008.blogspot.com/2008/08/ubuntu.html](http://liufeng2008.blogspot.com/2008/08/ubuntu.html)
-----------------------------------------------------------------------------------------------------------------
chmod用法 
指令名称 : chmod 
使用权限 : 所有使用者 
使用方式 : chmod [-cfvR] [--help] [--version] mode file... 
说明 : Linux/Unix 的档案存取权限分为三级 : 档案拥有者、群组、其他。
利用 chmod 可以藉以控制档案如何被他人所存取。 
把计 : 
mode : 权限设定字串，格式如下 : [ugoa...][[+-=][rwxX]...][,...]，
其中u 表示该档案的拥有者，
g 表示与该档案的拥有者属于同一个群体(group)者，
o 表示其他以外的人，
a 表示这三者皆是。 
+ 表示增加权限、- 表示取消权限、= 表示唯一设定权限。 
r 表示可读取，w 表示可写入，x 表示可执行，X 表示只有当该档案是个子目录或者该档案已经被设定过为可执行。 
-c : 若该档案权限确实已经更改，才显示其更改动作 
-f : 若该档案权限无法被更改也不要显示错误讯息 
-v : 显示权限变更的详细资料 
-R : 对目前目录下的所有档案与子目录进行相同的权限变更(即以递回的方式逐个变更) 
--help : 显示辅助说明 
--version : 显示版本 
范例 :将档案 file1.txt 设为所有人皆可读取 : 
chmod ugo+r file1.txt 
-
批量将文件夹及子文件夹文件
chmod -R 755 /var/www/thinkphp
将档案 file1.txt 设为所有人皆可读取 : 
chmod a+r file1.txt 
将档案 file1.txt 与 file2.txt 设为该档案拥有者，与其所属同一个群体者可写入，但其他以外的人则不可写入 : 
chmod ug+w,o-w file1.txt file2.txt 
将 ex1.py 设定为只有该档案拥有者可以执行 : 
chmod u+x ex1.py 
将目前目录下的所有档案与子目录皆设为任何人可读取 : 
chmod -R a+r * 
此外chmod也可以用数字来表示权限如 chmod 777 file 
语法为：chmod abc file 
其中a,b,c各为一个数字，分别表示User、Group、及Other的权限。 
r=4，w=2，x=1 
若要rwx属性则4+2+1=7； 
若要rw-属性则4+2=6； 
若要r-x属性则4+1=7。 
范例： 
chmod a=rwx file 
和 
chmod 777 file 
效果相同 
chmod ug=rwx,o=x file 
和 
chmod 771 file 
效果相同 
若用chmod 4755 filename可使此程式具有root的权限 
指令名称 : chown 
使用权限 : root 
使用方式 : chmod [-cfhvR] [--help] [--version] user[:group] file... 
说明 : Linux/Unix 是多人多工作业系统，所有的档案皆有拥有者。利用 chown 可以将档案的拥有者加以改变。
一般来说，这个指令只有是由系统管理者(root)所使用，一般使用者没有权限可以改变别人的档案拥有者，也没有权限可以自己的档案拥有者改设为别人。
只有系统管理者(root)才有这样的权限。 
把计 : 
user : 新的档案拥有者的使用者 
IDgroup : 新的档案拥有者的使用者群体
(group)-c : 若该档案拥有者确实已经更改，才显示其更改动作
-f : 若该档案拥有者无法被更改也不要显示错误讯息
-h : 只对于连结(link)进行变更，而非该 link 真正指向的档案
-v : 显示拥有者变更的详细资料
-R : 对目前目录下的所有档案与子目录进行相同的拥有者变更(即以递回的方式逐个变更)
--help : 显示辅助说明
--version : 显示版本 
范例 : 
将档案 file1.txt 的拥有者设为 users 群体的使用者 jessie : 
chown jessie:users file1.txt 
将目前目录下的所有档案与子目录的拥有者皆设为 users 群体的使用者 lamport : 
chmod -R lamport:users * 
-rw------- (600) -- 只有属主有读写权限。 
-rw-r--r-- (644) -- 只有属主有读写权限；而属组用户和其他用户只有读权限。 
-rwx------ (700) -- 只有属主有读、写、执行权限。 
-rwxr-xr-x (755) -- 属主有读、写、执行权限；而属组用户和其他用户只有读、执行权限。 
-rwx--x--x (711) -- 属主有读、写、执行权限；而属组用户和其他用户只有执行权限。 
-rw-rw-rw- (666) -- 所有用户都有文件读、写权限。这种做法不可取。 
-rwxrwxrwx (777) -- 所有用户都有读、写、执行权限。更不可取的做法。 
以下是对目录的两个普通设定: 
drwx------ (700) - 只有属主可在目录中读、写。 
drwxr-xr-x (755) - 所有用户可读该目录，但只有属主才能改变目录中的内容。 
