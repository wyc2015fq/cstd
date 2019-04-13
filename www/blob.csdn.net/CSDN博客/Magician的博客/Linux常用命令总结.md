
# Linux常用命令总结 - Magician的博客 - CSDN博客


置顶2017年09月23日 22:06:51[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：386标签：[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[常用命令																](https://so.csdn.net/so/search/s.do?q=常用命令&t=blog)[总结																](https://so.csdn.net/so/search/s.do?q=总结&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=常用命令&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=linux&t=blog)个人分类：[Linux																](https://blog.csdn.net/qq_16633405/article/category/6721984)
[
																					](https://so.csdn.net/so/search/s.do?q=linux&t=blog)所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)[
							](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




### 文章目录
[系统目录结构](#_3)
[命令格式：命令 -选项 参数 如：ls -la /usr](#___ls_la_usr_25)
[1、常用指令](#1_29)
[2、系统管理命令](#2_114)
[3、打包压缩相关命令](#3_290)
[4、关机/重启机器](#4_328)
[5、Linux管道](#5Linux_345)
[6、Linux软件包管理](#6Linux_354)
[7、vim使用](#7vim_391)
[8、用户及用户组管理](#8_421)
[9、文件权限管理](#9_470)
[补充：](#_519)

# 系统目录结构
![这里写图片描述](https://img-blog.csdn.net/20171002154425217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
bin  (binaries)存放二进制可执行文件
sbin  (super user binaries)存放二进制可执行文件，只有root才能访问
etc (etcetera)存放系统配置文件
usr  (unix shared resources)用于存放共享的系统资源
home 存放用户文件的根目录
root  超级用户目录
dev (devices)用于存放设备文件
lib  (library)存放跟文件系统中的程序运行所需要的共享库及内核模块
mnt  (mount)系统管理员安装临时文件系统的安装点
boot 存放用于系统引导时使用的各种文件
tmp  (temporary)用于存放各种临时文件
var  (variable)用于存放运行时需要改变数据的文件
掌握好一些快捷键
ctrl + c（停止当前进程）
ctrl + r（查看命令历史）
ctrl + l（清屏，与clear命令作用相同）
## 命令格式：命令 -选项 参数 如：ls -la /usr
## 1、常用指令
ls　　        显示文件或目录(list)
`-l           列出文件详细信息l(list)
 -a          列出当前目录下所有文件及目录，包括隐藏的a(all)`mkdir         创建目录
`-p           创建目录，若无父目录，则创建p(parent)`cd               切换目录（change directory）
`cd -：切换到上次工作的目录
 cd .. ：切换到上一级目录`touch          创建空文件
echo            创建带有内容的文件。
`#输出字符串到某文件
echo /root/apps/test.txt >hello word`cat              查看文件内容（catenate）
cp                拷贝
`-r  		   （recursive）拷贝整个文件夹`mv               移动或重命名
rm               删除文件（remove）
`-r            （recursive）递归删除，可删除子目录及文件
 -f            强制删除`find              在文件系统中搜索某文件//find /etc/ -name “aaa”
wc                统计文本中行数、字数、字符数（word count）
grep             在文本文件中查找某个字符串
rmdir           删除空目录
tree             树形结构显示目录，需要安装tree包
`yum -y install tree`pwd              显示当前目录（print working directory）
ln                  创建链接文件
`#为start.sh创建软连接start。主要用于添加某个模块到所需的地方
ln -s /root/app/start.sh   start`more、less  分页显示文本文件内容
head、tail    显示文件头、尾内容
ctrl+alt+F1  命令行全屏模式
yum
`yum install filename#安装软件
yum search keyname#根据关键字搜索软件包
yum remove filename#卸载软件包`
## 2、系统管理命令
stat              显示指定文件的详细信息，比ls更详细
who               显示在线登陆用户
whoami          显示当前操作用户
hostname      显示主机名
uname           显示系统信息
top                动态显示当前耗费资源最多进程信息
**ps                  显示瞬间进程状态**
`ps -aux
ps -ef | grep Hive   查看包含“hive”关键字的程序
功能：显示系统中进程的信息。包括进程ID、控制进程终端、执行时间和命令。 
    选项： 
  -a 显示所有进程信息 
  -u uidlist 列出这个用户的所有进程 
          -e 显示当前运行的每一个进程信息 
          -f 显示一个完整的列表 
  -x 显示包括没有终端控制的进程状况 。 
  -f：显示UID，PPIP，C与STIME栏位。
    注释： 
    grep全称是Global Regular Expression Print，表示全局正则表达式版本，它的使用权限是所有用户。
    例如：% ps -ef 
  % ps -aux 然后再利用一个管道符号导向到grep去查找特定的进程,然后再对特定的进程进行操作。
  
 **UID，PPIP，C与STIME栏位说明** 
 
UID：程序被该 UID 所拥有
PID：就是这个程序的 ID
PPID：则是其上级父程序的ID
C：CPU使用的资源百分比
STIME：系统启动时间
TTY ：登入者的终端机位置
TIME：使用掉的CPU时间。
CMD：所下达的是什么指令`**du**查看目录大小**du -h /home带有单位显示目录信息**
`-h文件大小以K，M，G为单位显示（human-readable）
-s只显示各档案大小的总合（summarize）`**ifconfig**查看网络情况
**ping**测试网络连通
netstat          显示网络状态信息如：netstat -anp | grep 10000   查看进程号运行的详细信息
`-a 显示所有连接和监听端口
-t (tcp)仅显示tcp相关选项
-u (udp)仅显示udp相关选项
-n 拒绝显示别名，能显示数字的全部转化成数字。
-p 显示建立相关链接的程序名`man                命令不会用了，找男人  如：man ls
clear              清屏
alias               对命令重命名 如：alias showmeit=“ps -aux” ，另外解除使用unaliax showmeit
**kill**杀死进程，可以先用ps 或 top命令查看进程的id，然后再用kill命令杀死进程。
curl[ifconfig.me](http://ifconfig.me)查询外网ip
**查看系统的配置：**
lscpu：显示cpu架构信息
`[xxx@localhost ~]$ lscpu
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                4　　　　　　　　　　#总处理器核心数量
On-line CPU(s) list:   0-3
Thread(s) per core:    1　　　　　　　　　　#每个核心支持的线程数量。1表示只支持一个线程，即不支持超线程
Core(s) per socket:    1　　　　　　　　　　#每个处理器的核心数量
Socket(s):             4　　　　　　　　　　#处理器数量
NUMA node(s):          1
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 63
Stepping:              0
CPU MHz:               2599.998
BogoMIPS:              5199.99
Hypervisor vendor:     VMware　　　　　　　#管理程序供应商
Virtualization type:   full
L1d cache:             32K
L1i cache:             32K
L2 cache:              256K
L3 cache:              30720K
NUMA node0 CPU(s):     0-3`总处理器核心数量=处理器数量*每个处理器的核心数量*每个核心支持的线程数量。即：CPU(s) = Socket(s) * Core(s) * Thread(s)。
`# 查看物理CPU个数
cat /proc/cpuinfo | grep "physical id" | sort | uniq
# 查看每个物理CPU中core的个数(即核数)
cat /proc/cpuinfo | grep "cpu cores" | uniq
# 查看每个物理CPU中线程的个数
cat /proc/cpuinfo | grep "siblings" | uniq
# 查看逻辑CPU的个数
cat /proc/cpuinfo| grep "processor"
# 查看CPU型号
cat /proc/cpuinfo | grep "model name" | uniq`**内存**
free命令就是从meminfo中获取的信息。一般情况下，使用free就能得到我们想知道的信息：-h添加单位
`[xxx@localhost ~]$ free -h
             total       used       free     shared    buffers     cached
Mem:          7868        954       6914          0         64        619
-/+ buffers/cache:        271       7597
Swap:         4031          0       4031`可以看出，内存大小是8G。
**硬盘**
lsblk：blk是block的缩写。列出块设备
`[xxx@localhost ~]$ lsblk
NAME                        MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sr0                          11:0    1 1024M  0 rom  
sdb                           8:16   0  100G  0 disk 
└─sdb1                        8:17   0  100G  0 part /data
sda                           8:0    0   60G  0 disk 
├─sda1                        8:1    0  500M  0 part /boot
└─sda2                        8:2    0 59.5G  0 part 
  ├─VolGroup-lv_root (dm-0) 253:0    0   50G  0 lvm  /
  ├─VolGroup-lv_swap (dm-1) 253:1    0    4G  0 lvm  [SWAP]
  └─VolGroup-lv_home (dm-2) 253:2    0  5.6G  0 lvm  /home`其中，TYPE=disk表示硬盘。可以看出，硬盘分为sda和sdb，一共160G。
df：查看硬盘使用情况。**df -h 带有单位显示磁盘信息**
`[xxx@localhost ~]$ df -h
Filesystem                    Size  Used Avail Use% Mounted on
/dev/mapper/VolGroup-lv_root   50G  1.7G   46G   4% /
tmpfs                         3.9G     0  3.9G   0% /dev/shm
/dev/sda1                     485M   39M  421M   9% /boot
/dev/mapper/VolGroup-lv_home  5.5G  165M  5.1G   4% /home
/dev/sdb1                      99G  188M   94G   1% /data`**lsb_release -a**:显示当前系统信息。 前提是lsb_release已正确安装
**cat /etc/issue**：显示当前的系统版本。
## 3、打包压缩相关命令
gzip：
bzip2：
**tar:                打包压缩**
`-c              归档（打包）文件（create）
 -x              解压缩文件（extract）
 -z              gzip压缩文件
 -j              bzip2压缩文件
 -v              显示压缩或解压缩过程 v（verbose）
 -f              使用档名``例：
tar -cvf /home/abc.tar /home/abc              只打包，不压缩
tar -zcvf /home/abc.tar.gz /home/abc        打包，并用gzip压缩
tar -jcvf /home/abc.tar.bz2 /home/abc      打包，并用bzip2压缩
tar -zxvf /home/abc.tar.gz /home/abc        解压tar包到对应的目录下`当然，如果想解压缩，就直接替换上面的命令  tar -cvf  / tar -zcvf  / tar -jcvf 中的“c” 换成“x” 就可以了。
## 4、关机/重启机器
shutdown
`-r             关机重启
 -h             关机不重启
 now          立刻关机`halt               关机
**reboot**重启
## 5、Linux管道
将一个命令的标准输出作为另一个命令的标准输入。也就是把几个命令组合起来使用，后一个命令除以前一个命令的结果。
例：grep -r “close” /home/* | more       在home目录下所有文件中查找，包括close的文件，并分页输出。
## 6、Linux软件包管理
dpkg (Debian Package)管理工具，软件包名以.deb后缀。这种方法适合系统不能联网的情况下。
比如安装tree命令的安装包，先将tree.deb传到Linux系统中。再使用如下命令安装。
sudo dpkg -i tree_1.5.3-1_i386.deb         安装软件
sudo dpkg -r tree                                     卸载软件
注：将tree.deb传到Linux系统中，有多种方式。VMwareTool，使用挂载方式；使用winSCP工具等；
APT（Advanced Packaging Tool）高级软件工具。这种方法适合系统能够连接互联网的情况。
依然以tree为例
sudo apt-get install tree                         安装tree
sudo apt-get remove tree                       卸载tree
sudo apt-get update                                 更新软件
sudo apt-get upgrade
将.rpm文件转为.deb文件
.rpm为RedHat使用的软件格式。在Ubuntu下不能直接使用，所以需要转换一下。
sudo alien abc.rpm
## 7、vim使用
（详细使用请参考：
[http://blog.csdn.net/qq_16633405/article/details/55214939](http://blog.csdn.net/qq_16633405/article/details/55214939)
vim三种模式：命令模式、插入模式、编辑模式。使用ESC或i或：来切换模式。
命令模式下：
:q                      退出
:q!                     强制退出
:wq                   保存并退出
:set number     显示行号
:set nonumber  隐藏行号
/apache            在文档中查找apache 按n跳到下一个，shift+n上一个
yyp                   复制光标所在行，并粘贴
h(左移一个字符←)、j(下一行↓)、k(上一行↑)、l(右移一个字符→)
## 8、用户及用户组管理
/etc/passwd    存储用户账号
/etc/group       存储组账号
/etc/shadow    存储用户账号的密码
/etc/gshadow  存储用户组账号的密码
useradd 用户名
userdel 用户名
adduser 用户名
groupadd 组名
groupdel 组名
**passwd root     给root设置密码**
su root
su - root
/etc/profile     系统环境变量
bash_profile     用户环境变量
.bashrc              用户环境变量
su user              切换用户，加载配置文件.bashrc
su - user            切换用户，加载配置文件/etc/profile ，加载bash_profile
更改文件的用户及用户组
sudo chown [-R] owner[:group] {File|Directory}
例如：还以jdk-7u21-linux-i586.tar.gz为例。属于用户hadoop，组hadoop
要想切换此文件所属的用户及组。可以使用命令。
sudo chown root:root jdk-7u21-linux-i586.tar.gz
## 9、文件权限管理
三种基本权限
R           读         数值表示为4
W          写         数值表示为2
X           可执行  数值表示为1
![这里写图片描述](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示，jdk-7u21-linux-i586.tar.gz文件的权限为-rw-rw-r–
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)-rw-rw-r–一共十个字符，分成四段。
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)第一个字符“-”表示普通文件；这个位置还可能会出现**“l”链接；“d”表示目录**
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)第二三四个字符“rw-”表示当前所属用户的权限。   所以用数值表示为4+2=6
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)第五六七个字符“rw-”表示当前所属组的权限。      所以用数值表示为4+2=6
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)第八九十个字符“r–”表示其他用户权限。              所以用数值表示为2
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)所以操作此文件的权限用数值表示为662
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**更改权限**
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)sudo chmod [u所属用户  g所属组  o其他用户  a所有用户]  [+增加权限  -减少权限]  [r  w  x]   目录名
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)例如：有一个文件filename，权限为“-rw-r----x” ,将权限值改为"-rwxrw-r-x"，用数值表示为765
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)sudo chmod u+x g+w o+r  filename
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)上面的例子可以用数值表示
[
](https://img-blog.csdn.net/20170923220456390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)sudo**chmod 765 filename**
常用的指令：
`#给文件添加执行权限
chmod +x filename
#给文件卸去执行权限
chmod -x filename`chmod +x 和chmod a+x 是一样的，一般没有明确要求，可以就用chmod +x
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113155247215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
## 补充：
Linux命令大全离线版：[http://download.csdn.net/download/qq_16633405/10033409](http://download.csdn.net/download/qq_16633405/10033409)
修改系统日期与时间：date -s “2012-11-03 10:25:25”
scp命令：
scp /home/space/music/1.mp3[root@www.cumt.edu.cn](mailto:root@www.cumt.edu.cn):/home/root/others/music
scp+要传输的文件的地址+用户名+@+主机名+文件要存放的地址
`-r:传输整个目录`tar命令：
tar -cvf /root/jdk1.7.0_45.jar-C
/root
tar+"-cvf"+你要压缩的文件所在的绝对路径（注意要加一个.jar）+  “-C”+压缩后存放的绝对路径
查看端口被占用的指令
4.1、lsof -i:端口号查看某个端口是否被占用
![这里写图片描述](https://img-blog.csdn.net/20171104153522750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.2、使用netstat -anp|grep 80 （查看端口号包含80的端口，最后一列就是进程号）
![这里写图片描述](https://img-blog.csdn.net/20171104153533810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5、后台运行程序并将控制台信息输出到文本中
[
](https://img-blog.csdn.net/20171104153533810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)\#将python程序运行到后台并将控制台信息打印到run.log文件中
nohup python flask_data.py >/root/apps/intelligent_custom_service/run.log &[
            ](https://img-blog.csdn.net/20171104153533810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[
  ](https://img-blog.csdn.net/20171104153522750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)