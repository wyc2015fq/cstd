# 必学LINUX命令 - 深之JohnChen的专栏 - CSDN博客

2006年03月13日 18:43:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2085


指令：fstab

使用权限 : 超级使用者

使用方式 : 使用编辑器来修改 /etc/fstab (eg. vi /etc/fstab)

说明 ： 存放档案系统与目录结构对应资料的档案

fstab 栏位说明：

第一栏(fs_spec)： 实际的 device 名称
第二栏(fs_file)： 对应到的目录结构(mount point)
第三栏(fs_vfstype)：该 partition 的档案系统，常见的有：

minix、ext、ext2、msdos、iso9660、nfs、swap
第四栏(fs_mntops)： 在 mount 时的参数
第五栏(fs_freq)： 在使用 dump 时是否记录，不需要则输入0
第六栏(fs_passno)： 决定在开机时执行 fsck 的先后顺序

例子 :
IDE 硬盘分成两个 partition 与一个 swap，还有一台光碟机跟一台软碟机的情形 :

/dev/hda1 / ext2 defaults 1 1
/dev/hda5 /home ext2 defaults 1 2
/dev/cdrom /mnt/cdrom iso9660 noauto,user,ro 0 0
/dev/hda6 swap swap defaults 0 0
/dev/fd0 /mnt/floppy ext2 noauto,owner 0 0
none /proc proc defaults 0 0
none /dev/pts devpts gid=5,mode=620 0 0

名称：su
使用权限：所有使用者
使用方式：su [-fmp] [-c command] [-s shell] [--help] [--version] [-] [USER [ARG]]
说明：变更为其他使用者的身份，除 root 外，需要键入该使用者的密码
参数：
-f 或 --fast 不必读启动档（如 csh.cshrc 等），仅用于 csh 或 tcsh
-m -p 或 --preserve-environment 执行 su 时不改变环境变数
-c command 或 --command=command 变更为帐号为 USER 的使用者并执行指令（command）后再变回原来使用者
-s shell 或 --shell=shell 指定要执行的 shell （bash csh tcsh 等），预设值为 /etc/passwd 内的该使用者（USER） shell
--help 显示说明文件
--version 显示版本资讯
- -l 或 --login 这个参数加了之后，就好像是重新 login 为该使用者一样，大部份环境变数（HOME SHELL USER等等）都是以该使用者（USER）为主，并且工作目录也会改变，如果没有指定 USER ，内定是 root
USER 欲变更的使用者帐号
ARG 传入新的 shell 参数
范例：
su -c ls root 变更帐号为 root 并在执行 ls 指令后退出变回原使用者。
su root -f 变更帐号为 root 并传入 -f 参数给新执行的 shell。
su - clsung 变更帐号为 clsung 并改变工作目录至 clsung 的家目录（home dir）。

名称 : shutdown

使用权限 : 系统管理者

使用方式 : shutdown [-t seconds] [-rkhncfF] time [message]

说明 : shutdown 可以用来进行关机程序，并且在关机以前传送讯息给所有使用者正在执行的程序，shutdown 也可以用来重开机。

参数 :

-t seconds : 设定在几秒钟之后进行关机程序
-k : 并不会真的关机，只是将警告讯息传送给所有只用者
-r : 关机后重新开机
-h : 关机后停机
-n : 不采用正常程序来关机，用强迫的方式杀掉所有执行中的程序后自行关机
-c : 取消目前已经进行中的关机动作
-f : 关机时，不做 fcsk 动作(检查 Linux 档系统)
-F : 关机时，强迫进行 fsck 动作
time : 设定关机的时间
message : 传送给所有使用者的警告讯息

名称：reboot
使用权限：系统管理者
使用方式：reboot [-n] [-w] [-d] [-f] [-i]
说明：若系统的 runlevel 为 0 或 6 ，则重新开机，否则以 shutdown 指令（加上 -r 参数）来取代
参数：
-n : 在重开机前不做将记忆体资料写回硬盘的动作
-w : 并不会真的重开机，只是把记录写到 /var/log/wtmp 档案里
-d : 不把记录写到 /var/log/wtmp 档案里（-n 这个参数包含了 -d） -f : 强迫重开机，不呼叫 shutdown 这个指令
-i : 在重开机之前先把所有网络相关的装置先停止
范例：
reboot 重开机。
reboot -w 做个重开机的模拟（只有纪录并不会真的重开机）。

名称：adduser
使用权限：系统管理员
使用方式：adduser [-c comment] [-d home_dir]
[-e expire_date] [-f inactive_time]
[-g initial_group] [-G group[,...]]
[-m [-k skeleton_dir] | -M] [-p passwd]
[-s shell] [-u uid [ -o]] [-n] [-r] loginid
或 adduser -D [-g default_group] [-b default_home]
[-f default_inactive]
[-e default_expire_date]
[-s default_shell]
附注：adduser 与 useradd 指令为同一指令（经由符号连结 symbolic link）
说明：新增使用者帐号或更新预设的使用者资料
参数：
-c comment 新使用者位于密码档（通常是 /etc/passwd）的注解资料
-d home_dir 设定使用者的家目录为 home_dir ，预设值为预设的 home 后面加上使用者帐号 loginid
-e expire_date 设定此帐号的使用期限（格式为 YYYY-MM-DD），预设值为永久有效
-f inactive_time 范例：

adduser是增加使用者.
相对的,也有删除使用者的指令,userdel.
语法:userdel [login ID]

名称 : mount
使用权限 : 系统管理者或/etc/fstab中允许的使用者
使用方式 :
mount [-hV]
mount -a [-fFnrsvw] [-t vfstype]
mount [-fnrsvw] [-o options [,...]] device | dir
mount [-fnrsvw] [-t vfstype] [-o options] device dir
说明 :

将某个档案的内容解读成档案系统，然后将其挂在目录的某个位置之上。当这个命令执行成功后，直到我们使用 umnount 将这个档案系统移除为止，这个命令之下的所有档案将暂时无法被调用。

这个命令可以被用来挂上任何的档案系统，你甚至可以用 -o loop 选项将某个一般的档案当成硬盘机分割挂上系统。这个功能对于 ramdisk,romdisk 或是 ISO 9660 的影像档之解读非常实用。

参数

-V

显示程序版本
-h

显示辅助讯息
-v

显示较讯息，通常和 -f 用来除错。
-a
将 /etc/fstab 中定义的所有档案系统挂上。

-F
这个命令通常和 -a 一起使用，它会为每一个 mount 的动作产生一个行程负责执行。在系统需要挂上大量 NFS 档案系统时可以加快挂上的动作。

-f
通常用在除错的用途。它会使 mount 并不执行实际挂上的动作，而是模拟整个挂上的过程。通常会和 -v 一起使用。

-n
一般而言，mount 在挂上后会在 /etc/mtab 中写入一笔资料。但在系统中没有可写入档案系统存在的情况下可以用这个选项取消这个动作。

-s-r
等于 -o ro

-w
等于 -o rw

-L
将含有特定标签的硬盘分割挂上。

-U
将档案分割序号为 的档案系统挂下。-L 和 -U 必须在/proc/partition 这种档案存在时才有意义。

-t
指定档案系统的型态，通常不必指定。mount 会自动选择正确的型态。

-o async
打开非同步模式，所有的档案读写动作都会用非同步模式执行。

-o sync
在同步模式下执行。

-o atime
-o noatime
当 atime 打开时，系统会在每次读取档案时更新档案的『上一次调用时间』。当我们使用 flash 档案系统时可能会选项把这个选项关闭以减少写入的次数。

-o auto
-o noauto
打开/关闭自动挂上模式。

-o defaults
使用预设的选项 rw, suid, dev, exec, auto, nouser, and async.

-o dev
-o nodev-o exec
-o noexec
允许执行档被执行。

-o suid
-o nosuid
允许执行档在 root 权限下执行。

-o user
-o nouser
使用者可以执行 mount/umount 的动作。

-o remount
将一个已经挂下的档案系统重新用不同的方式挂上。例如原先是唯读的系统，现在用可读写的模式重新挂上。

-o ro
用唯读模式挂上。

-o rw
用可读写模式挂上。

-o loop=
使用 loop 模式用来将一个档案当成硬盘分割挂上系统。

范例

将 /dev/hda1 挂在 /mnt 之下。

#mount /dev/hda1 /mnt

将 /dev/hda1 用唯读模式挂在 /mnt 之下。

#mount -o ro /dev/hda1 /mnt

将 /tmp/image.iso 这个光碟的 image 档使用 loop 模式挂在 /mnt/cdrom之下。用这种方法可以将一般网络上可以找到的 Linux 光 碟 ISO 档在不烧录成光碟的情况下检视其内容。

#mount -o loop /tmp/image.iso /mnt/cdrom

相关命令umount

指令：fdisk

用途：观察硬盘之实体使用情形与分割硬盘用。

使用方法：

　　　　　　一、在 console 上输入 fdisk -l /dev/sda ，观察硬盘之实体使用情形。

　　　　　　二、在 console 上输入 fdisk /dev/sda，可进入分割硬盘模式。

　　　　　　　　1. 输入 m 显示所有命令列示。

　　　　　　　　2. 输入 p 显示硬盘分割情形。

　　　　　　　　3. 输入 a 设定硬盘启动区。

　　　　　　　　4. 输入 n 设定新的硬盘分割区。

　　　　　　　　　4.1. 输入 e 硬盘为[延伸]分割区(extend)。

　　　　　　　　　4.2. 输入 p 硬盘为[主要]分割区(primary)。

　　　　　　　　5. 输入 t 改变硬盘分割区属性。

　　　　　　　　6. 输入 d 删除硬盘分割区属性。

　　　　　　　　7. 输入 q 结束不存入硬盘分割区属性。

　　　　　　　　8. 输入 w 结束并写入硬盘分割区属性。

指令：e2fsck

使用权限 : 超级使用者

使用方式 : e2fsck [-pacnydfvFV] [-b superblock] [-B blocksize] [-l|-L bad_blocks_file] [-C fd] device

说明 ： 检查使用 Linux ext2 档案系统的 partition 是否正常工作

参数 ：

device ： 预备检查的硬盘 partition，例如：/dev/sda1
-a : 对 partition 做检查，若有问题便自动修复，等同 -p 的功能
-b : 设定存放 superblock 的位置
-B : 设定单位 block 的大小
-c : 检查该partition 是否有坏轨
-C file : 将检查的结果存到 file 中以便查看
-d : 列印 e2fsck 的 debug 结果
-f : 强制检查
-F : 在开始检查前，将device 的 buffer cache 清空，避免有错误发生
-l bad_blocks_file : 将有坏轨的block资料加到 bad_blocks_file 里面
-L bad_blocks_file : 设定坏轨的block资料存到 bad_blocks_file 里面，若无该档则自动产生
-n : 将档案系统以[唯读]方式开启
-p : 对 partition 做检查，若有问题便自动修复
-v : 详细显示模式
-V : 显示出目前 e2fsck 的版本
-y : 预先设定所有检查时的问题均回答[是]

例子 :
检查 /dev/hda5 是否正常，如果有异常便自动修复，并且设定若有问答，均回答[是] :
e2fsck -a -y /dev/hda5

注意 :

大部份使用 e2fsck 来检查硬盘 partition 的情况时，通常都是情形特殊，因此最好先将该 partition umount，然后再执行 e2fsck 来做检查，若是要非要检查 / 时，则请进入 singal user mode 再执行。

名称:df
使用权限: 所有使用者>
使用方式: df [选项]... [FILE]...

显示档案系统的状况，或是看所有档案系统的状况(预设值)

-a, --all 包含所有的具有 0 Blocks 的档案系统
--block-size={SIZE} 使用 {SIZE} 大小的 Blocks
-h, --human-readable 使用人类可读的格式(预设值是不加这个选项的...)
-H, --si 很像 -h, 但是用 1000 为单位而不是用 1024
-i, --inodes 列出 inode 资讯，不列出已使用 block
-k, --kilobytes 就像是 --block-size=1024
-l, --local 限制列出的档案结构
-m, --megabytes 就像 --block-size=1048576
--no-sync 取得资讯前不 sync (预设值)
-P, --portability 使用 POSIX 输出格式
--sync 在取得资讯前 sync
-t, --type=TYPE 限制列出档案系统的 TYPE
-T, --print-type 显示档案系统的形式
-x, --exclude-type=TYPE 限制列出档案系统不要显示 TYPE
-v (忽略)
--help 显示这个帮手并且离开
--version 输出版本资讯并且离开

名称: dd
使用权限: 所有使用者dd 这个指令在 manual 里的定义是 convert and copy a file

使用方式:
dd [option]

如果你想要在线看 manual, 可以试试:
dd --help

或是
info dd

如果你想要看看这个版本如何:
dd --version

输入或输出
dd if=[STDIN] of=[STDOUT]

强迫输入或输出的Size为多少Bytes
bs: dd -ibs=[BYTE] -obs=[SIZE]

强迫一次只做多少个 Bytes
cbs=BYTES

跳过一段以后才输出
seek=BLOCKS

跳过一段以后才输入
skip=BLOCKS

当然你可以拿这个来方便的拷贝光碟(注意，你的光碟是标准的 iso9660格式才可以这么做唷！)

dd if=/dev/cdrom of=cdrom.iso
其中 if 后面以及 of 后面的内容依你的需求调整。

然后给系统这个指令就可以烧了:

cdrecord -v cdrom.iso
这篇不是在讲 cdrecord 的，所以上面的指令是最为简单但是不一定能符合您的硬件环境....

指令：clear

用途：清除萤幕用。

使用方法：在 console 上输入 clear。

名称：tr

### 1.比方说要把目录下所有的大写档名换为小写档名?

似乎有很多方式，/"tr/"是其中一种:

#!/bin/sh

dir=/"/tmp/testdir/";
files=`find $dir -type f`;
for i in $files
do
dir_name=`dirname $i`;
ori_filename=`basename $i`
new_filename=`echo $ori_filename | tr [:upper:] [:lower:]` > /dev/null;
#echo $new_filename;
mv $dir_name/$ori_filename $dir_name/$new_filename
done

### 2.自己试验中...lowercase to uppercase

tr abcdef...[del] ABCDE...[del]
tr a-z A-Z
tr /'[:lower:]/' /'[:upper:]/'

shell>> echo /"this is a test/" | tr a-z A-Z > www
shell>> cat www
THIS IS A TEST

### 3.去掉不想要的字串

shell>> tr -d this ### 去掉有关 t.e.s.t
this

man
man
test
e

### 4.取代字串

shell>> tr -s /"this/" /"TEST/"
this
TEST
th
TE

名称：top

使用权限：所有使用者

使用方式：top [-] [d delay] [q] [c] [S] [s] [i] [n] [b]

说明：实时显示 process 的动态

参数：

d : 改变显示的更新速度，或是在交谈式指令列( interactive command)按 s
q : 没有任何延迟的显示速度，如果使用者是有 superuser 的权限，则 top 将会以最高的优先序执行
c : 切换显示模式，共有两种模式，一是只显示执行档的名称，另一种是显示完整的路径与名称S : 累积模式，会将己完成或消失的子行程 ( dead child process ) 的 CPU time 累积起来
s : 安全模式，将交谈式指令取消, 避免潜在的危机
i : 不显示任何闲置 (idle) 或无用 (zombie) 的行程
n : 更新的次数，完成后将会退出 top
b : 批次档模式，搭配 /"n/" 参数一起使用，可以用来将 top 的结果输出到档案内

范例：
显示更新十次后退出 ;
top -n 10

使用者将不能利用交谈式指令来对行程下命令 :
top -s

将更新显示二次的结果输入到名称为 top.log 的档案里 :
top -n 2 -b < top.log

名称：ps
使用权限：所有使用者
使用方式：ps [options] [--help]
说明：显示瞬间行程 (process) 的动态
参数：
ps 的参数非常多, 在此仅列出几个常用的参数并大略介绍含义
-A 列出所有的行程
-w 显示加宽可以显示较多的资讯
-au 显示较详细的资讯
-aux 显示所有包含其他使用者的行程

au(x) 输出格式 :

USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
USER: 行程拥有者
PID: pid
%CPU: 占用的 CPU 使用率
%MEM: 占用的记忆体使用率
VSZ: 占用的虚拟记忆体大小
RSS: 占用的记忆体大小
TTY: 终端的次要装置号码 (minor device number of tty)
STAT: 该行程的状态:
D: 不可中断的静止 (通悸□□缜b进行 I/O 动作)
R: 正在执行中
S: 静止状态
T: 暂停执行
Z: 不存在但暂时无法消除
W: 没有足够的记忆体分页可分配
<: 高优先序的行程
N: 低优先序的行程
L: 有记忆体分页分配并锁在记忆体内 (实时系统或捱A I/O)
START: 行程开始时间
TIME: 执行的时间
COMMAND:所执行的指令

范例：

ps
PID TTY TIME CMD
2791 ttyp0 00:00:00 tcsh
3092 ttyp0 00:00:00 ps
% ps -A
PID TTY TIME CMD
1 ? 00:00:03 init
2 ? 00:00:00 kflushd
3 ? 00:00:00 kpiod
4 ? 00:00:00 kswapd
5 ? 00:00:00 mdrecoveryd
.......
% ps -aux
USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
root 1 0.0 0.7 1096 472 ? S Sep10 0:03 init [3]
root 2 0.0 0.0 0 0 ? SW Sep10 0:00 [kflushd]
root 3 0.0 0.0 0 0 ? SW Sep10 0:00 [kpiod]
root 4 0.0 0.0 0 0 ? SW Sep10 0:00 [kswapd]
........

名称：kill

使用权限：所有使用者

使用方式：

kill [ -s signal | -p ] [ -a ] pid ...
kill -l [ signal ]
说明：kill 送出一个特定的信号 (signal) 给行程 id 为 pid 的行程根据该信号而做特定的动作, 若没有指定, 预设是送出终止 (TERM) 的信号
参数：

-s (signal) : 其中可用的讯号有 HUP (1), KILL (9), TERM (15), 分别代表著重跑, 砍掉, 结束; 详细的信号可以用 kill -l
-p : 印出 pid , 并不送出信号
-l (signal) : 列出所有可用的信号名称
范例：

将 pid 为 323 的行程砍掉 (kill) :
kill -9 323

将 pid 为 456 的行程重跑 (restart) :
kill -HUP 456

名称：cat
使用权限：所有使用者
使用方式：cat [-AbeEnstTuv] [--help] [--version] fileName
说明：把档案串连接后传到基本输出（萤幕或加 > fileName 到另一个档案）
参数：
-n 或 --number 由 1 开始对所有输出的行数编号
-b 或 --number-nonblank 和 -n 相似，只不过对于空白行不编号
-s 或 --squeeze-blank 当遇到有连续两行以上的空白行，就代换为一行的空白行
-v 或 --show-nonprinting
范例：
cat -n textfile1 > textfile2 把 textfile1 的档案内容加上行号后输入 textfile2 这个档案里
cat -b textfile1 textfile2 >> textfile3 把 textfile1 和 textfile2 的档案内容加上行号（空白行不加）之后将内容附加到 textfile3 里。

范例：
把 textfile1 的档案内容加上行号后输入 textfile2 这个档案里
cat -n textfile1 > textfile2

把 textfile1 和 textfile2 的档案内容加上行号（空白行不加）之后将内容附加到 textfile3 里。
cat -b textfile1 textfile2 >> textfile3

cat /dev/null > /etc/test.txt 此为清空/etc/test.txt档案内容

cat 也可以用来制作 image file。例如要制作软碟的 image file，将软碟放好后打
cat /dev/fd0 > OUTFILE

相反的，如果想把 image file 写到软碟，请打
cat IMG_FILE > /dev/fd0

注：
1. OUTFILE 指输出的 image 档名。
2. IMG_FILE 指 image file。
3. 若从 image file 写回 device 时，device 容量需与相当。
4. 通常用在制作开机磁片。

指令名称 : chmod

使用权限 : 所有使用者

使用方式 : chmod [-cfvR] [--help] [--version] mode file...

说明 : Linux/Unix 的档案调用权限分为三级 : 档案拥有者、群组、其他。利用 chmod 可以藉以控制档案如何被他人所调用。

参数 :

mode : 权限设定字串，格式如下 : [ugoa...][[+-=][rwxX]...][,...]，其中
u 表示该档案的拥有者，g 表示与该档案的拥有者属于同一个群体(group)者，o 表示其他以外的人，a 表示这三者皆是。
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

若用chmod 4755 filename可使此程序具有root的权限

名称：cp

使用权限：所有使用者

使用方式：

cp [options] source dest
cp [options] source... directory

说明：将一个档案拷贝至另一档案，或将数个档案拷贝至另一目录。

参数：

-a 尽可能将档案状态、权限等资料都照原状予以复制。
-r 若 source 中含有目录名，则将目录下之档案亦皆依序拷贝至目的地。
-f 若目的地已经有相同档名的档案存在，则在复制前先予以删除再行复制。
范例：
将档案 aaa 复制(已存在)，并命名为 bbb :
cp aaa bbb

将所有的C语言程序拷贝至 Finished 子目录中 :
cp *.c Finished

名称 : crontab

使用权限 : 所有使用者

使用方式 :

crontab [ -u user ] file
crontab [ -u user ] { -l | -r | -e }

说明 :
crontab 是用来让使用者在固定时间或固定间隔执行程序之用，换句话说，也就是类似使用者的时程表。-u user 是指设定指定 user 的时程表，这个前提是你必须要有其权限(比如说是 root)才能够指定他人的时程表。如果不使用 -u user 的话，就是表示设定自己的时程表。

餐数 :

-e : 执行文字编辑器来设定时程表，内定的文字编辑器是 VI，如果你想用别的文字编辑器，则请先设定 VISUAL 环境变数来指定使用那个文字编辑器(比如说 setenv VISUAL joe)
-r : 删除目前的时程表
-l : 列出目前的时程表

时程表的格式如下 :
f1 f2 f3 f4 f5 program

其中 f1 是表示分钟，f2 表示小时，f3 表示一个月份中的第几日，f4 表示月份，f5 表示一个星期中的第几天。program 表示要执行的程序。
当 f1 为 * 时表示每分钟都要执行 program，f2 为 * 时表示每小时都要执行程序，其馀类推
当 f1 为 a-b 时表示从第 a 分钟到第 b 分钟这段时间内要执行，f2 为 a-b 时表示从第 a 到第 b 小时都要执行，其馀类推
当 f1 为 */n 时表示每 n 分钟个时间间隔执行一次，f2 为 */n 表示每 n 小时个时间间隔执行一次，其馀类推
当 f1 为 a, b, c,... 时表示第 a, b, c,... 分钟要执行，f2 为 a, b, c,... 时表示第 a, b, c...个小时要执行，其馀类推

使用者也可以将所有的设定先存放在档案 file 中，用 crontab file 的方式来设定时程表。
例子 :

每月每天每小时的第 0 分钟执行一次 /bin/ls :
0 7 * * * /bin/ls

在 12 月内, 每天的早上 6 点到 12 点中，每隔 20 分钟执行一次 /usr/bin/backup :
0 6-12/3 * 12 * /usr/bin/backup

周一到周五每天下午 5:00 寄一封信给 alex@domain.name :
0 17 * * 1-5 mail -s /"hi/" alex@domain.name < /tmp/maildata

每月每天的午夜 0 点 20 分, 2 点 20 分, 4 点 20 分....执行 echo /"haha/"
20 0-23/2 * * * echo /"haha/"

注意 :

当程序在你所指定的时间执行后，系统会寄一封信给你，显示该程序执行的内容，若是你不希望收到这样的信，请在每一行空一格之后加上 > /dev/null 2>&1 即可

名称：mail

使用权限：所有使用者

使用方式：mail [-iInv] [-s subject] [-c cc-addr] [-b bcc-addr] user1 [user 2 ...]

说明：
mail 不仅只是一个指令， mail 还是一个电子邮件程序，不过利用 mail 来读信的人应该很少吧！对于系统管理者来说 mail 就很有用，因为管理者可以用 mail 写成 script ，定期寄一些备忘录提醒系统的使用者。

参数：
i 忽略 tty 的中断讯号。 (interrupt)
I 强迫设成互动模式。 (Interactive)
v 列印出讯息，例如送信的地点、状态等等。 (verbose)
n 不读入 mail.rc 设定档。
s 邮件标题。
c cc 邮件地址。
b bcc 邮件地址。

范例：

将信件送给一个或以上的电子邮件地址，由于没有加入其他的选项，使用者必须输入标题与信件的内容等。而 user2 没有主机位置，就会送给邮件服务器的 user2 使用者。

mail user1@email.address
mail user1@email.address user2

名称： finger

使用权限： 所有使用者

使用方式： finger [options] user[@address]

说明：finger 可以让使用者查询一些其他使用者的资料。会列出来的资料有：

Login Name
User Name
Home directory
Shell
Login status
mail status
.plan
.project
.forward

其中 .plan ，.project 和 .forward 就是使用者在他的 Home Directory 里的 .plan ， .project 和 .forward 等档案里的资料。如果没有就没有。finger 指令并不限定于在同一服务器上查询，也可以寻找某一个远端服务器上的使用者。只要给一个像是 E-mail address 一般的地址即可。
参数：

-l
多行显示。

-s
单行显示。这个选项只显示登入名称，真实姓名，终端机名称，闲置时间，登入时间，办公室号码及电话号码。如果所查询的使用者是远端服务器的使用者，这个选项无效。

范例：下列指令可以查询本机管理员的资料：
finger root

其结果如下：
Login: root Name: root
Directory: /root Shell: /bin/bash
Never logged in.
No mail.
No Plan.

名称 : at

使用权限 : 所有使用者

使用方式 : at -V [-q queue] [-f file] [-mldbv] TIME

说明 : at 可以让使用者指定在 TIME 这个特定时刻执行某个程序或指令，TIME 的格式是 HH:MM其中的 HH 为小时，MM 为分钟，甚至你也可以指定 am, pm, midnight, noon, teatime(就是下午 4 点锺)等口语词。

如果想要指定超过一天内的时间，则可以用 MMDDYY 或者 MM/DD/YY 的格式，其中 MM 是分钟，DD 是第几日，YY 是指年份。另外，使用者甚至也可以使用像是 now + 时间间隔来弹性指定时间，其中的时间间隔可以是 minutes, hours, days, weeks。

另外，使用者也可指定 today 或 tomorrow 来表示今天或明天。当指定了时间并按下 enter 之后，at 会进入交谈模式并要求输入指令或程序，当你输入完后按下 ctrl+D 即可完成所有动作，至于执行的结果将会寄回你的帐号中。

参数 :

-V : 印出版本编号
-q : 使用指定的伫列(Queue)来储存，at 的资料是存放在所谓的 queue 中，使用者可以同时使用多个 queue，而 queue 的编号为 a, b, c... z 以及 A, B, ... Z 共 52 个
-m : 即使程序/指令执行完成后没有输出结果, 也要寄封信给使用者
-f file : 读入预先写好的命令档。使用者不一定要使用交谈模式来输入，可以先将所有的指定先写入档案后再一次读入
-l : 列出所有的指定 (使用者也可以直接使用 atq 而不用 at -l)
-d : 删除指定 (使用者也可以直接使用 atrm 而不用 at -d)
-v : 列出所有已经完成但尚未删除的指定

例子 :
三天后的下午 5 点锺执行 /bin/ls :
at 5pm + 3 days /bin/ls

三个星期后的下午 5 点锺执行 /bin/ls :
at 5pm + 2 weeks /bin/ls

明天的 17:20 执行 /bin/date :
at 17:20 tomorrow /bin/date

1999 年的最后一天的最后一分钟印出 the end of world !
at 23:59 12/31/1999 echo the end of world !

名称：mv

使用权限：所有使用者

使用方式：

mv [options] source dest
mv [options] source... directory
说明：将一个档案移至另一档案，或将数个档案移至另一目录。
参数：-i 若目的地已有同名档案，则先询问是否覆盖旧档。

范例：

将档案 aaa 更名为 bbb :
mv aaa bbb

将所有的C语言程序移至 Finished 子目录中 :
mv -i *.c

名称：locate
使用权限：所有使用者
使用方式： locate [-q] [-d ] [--database=]
locate [-r ] [--regexp=]
locate [-qv] [-o ] [--output=]
locate [-e ] [-f ] <[-l ] [-c]
<[-U ] [-u]>
locate [-Vh] [--version] [--help]
说明：

　　locate 让使用者可以很快速的搜寻档案系统内是否有指定的档案。其方法是先建立一个包括系统内所有档案名称及路径的数据库，之后当寻找时就只需查询这个数据库，而不必实际深入档案系统之中了。

　　在一般的 distribution 之中，数据库的建立都被放在 contab 中自动执行。一般使用者在使用时只要用

# locate your_file_name

的型式就可以了。 参数：
-u
-U

建立数据库，-u 会由根目录开始，-U 则可以指定开始的位置。

-e

将
排除在寻找的范围之外。

-l
如果 是 1．则启动安全模式。在安全模式下，使用者不会看到权限无法看到的档案。这会始速度减慢，因为 locate 必须至实际的档案系统中取得档案的权限资料。

-f
将特定的档案系统排除在外，例如我们没有到理要把 proc 档案系统中的档案放在数据库中。

-q
安静模式，不会显示任何错误讯息。

-n
至多显示 个输出。

-r
使用正规运算式 做寻找的条件。

-o
指定数据库存的名称。

-d

指定数据库的路径

-h
显示辅助讯息

-v
显示更多的讯息

-V
显示程序的版本讯息 范例：

locate chdrv : 寻找所有叫 chdrv 的档案
locate -n 100 a.out : 寻找所有叫 a.out 的档案，但最多只显示 100 个
locate -u : 建立数据库

　　locate命令可以在搜寻数据库时快速找到档案，数据库由updatedb程序来更新，updatedb是由cron daemon周期性建立的，locate命令在搜寻数据库时比由整个由硬盘资料来搜寻资料来得快，但较差劲的是locate所找到的档案若是最近才建立或刚更名的，可能会找不到，在内定值中，updatedb每天会跑一次，可以由修改crontab来更新设定值。(etc/crontab)

　　locate指定用在搜寻符合条件的档案，它会去储存档案与目录名称的数据库内，寻找合乎范本样式条件的档案或目录录，可以使用特殊字元（如”*”或”?”等）来指定范本样式，如指定范本为kcpa*ner, locate会找出所有起始字串为kcpa且结尾为ner的档案或目录，如名称为kcpartner若目录录名称为kcpa_ner则会列出该目录下包括子目录在内的所有档案。

　　locate指令和find找寻档案的功能类似，但locate是透过update程序将硬盘中的所有档案和目录资料先建立一个索引数据库，在执行 loacte时直接找该索引，查询速度会较快，索引数据库一般是由操作系统管理，但也可以直接下达update强迫系统立即修改索引数据库。

　　不过第一次在执行update後再使用locate寻找档案常会失败，此时就要执行slocate ˉu该命令（也可执行updatedb指令，其效果相同）来更新slocate数据库，该命令会在/usr/sbin下产生slocate执行档，再由locate到此数据库寻找所要找的资料。

名称：rm

使用权限：所有使用者

使用方式：rm [options] name...

说明：删除档案及目录。

参数：

-i 删除前逐一询问确认。
-f 即使原档案属性设为唯读，亦直接删除，无需逐一确认。
-r 将目录及以下之档案亦逐一删除。
范例：
删除所有C语言程序档；删除前逐一询问确认 :
rm -i *.c

将 Finished 子目录及子目录中所有档案删除 :
rm -r Finished

指令名称 : ln

使用权限 : 所有使用者

使用方式 : ln [options] source dist，其中 option 的格式为 :

[-bdfinsvF] [-S backup-suffix] [-V {numbered,existing,simple}]
[--help] [--version] [--]
说明 : Linux/Unix 档案系统中，有所谓的连结(link)，我们可以将其视为档案的别名，而连结又可分为两种 : 硬连结(hard link)与软连结(symbolic link)，硬连结的意思是一个档案可以有多个名称，而软连结的方式则是产生一个特殊的档案，该档案的内容是指向另一个档案的位置。硬连结是存在同一个档案系统中，而软连结却可以跨越不同的档案系统。
ln source dist 是产生一个连结(dist)到 source，至于使用硬连结或软链结则由参数决定。

不论是硬连结或软链结都不会将原本的档案复制一份，只会占用非常少量的磁碟空间。

参数 :

-f : 链结时先将与 dist 同档名的档案删除-d : 允许系统管理者硬链结自己的目录-i : 在删除与 dist 同档名的档案时先进行询问-n : 在进行软连结时，将 dist 视为一般的档案-s : 进行软链结(symbolic link)-v : 在连结之前显示其档名-b : 将在链结时会被覆写或删除的档案进行备份-S SUFFIX : 将备份的档案都加上 SUFFIX 的字尾-V METHOD : 指定备份的方式--help : 显示辅助说明--version : 显示版本
范例 :
将档案 yy 产生一个 symbolic link : zz
ln -s yy zz

将档案 yy 产生一个 hard link : zz

