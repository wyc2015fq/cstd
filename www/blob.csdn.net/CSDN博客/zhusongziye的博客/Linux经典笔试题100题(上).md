# Linux经典笔试题100题(上) - zhusongziye的博客 - CSDN博客





2018年11月06日 21:43:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：57








本套笔试题共50题，每题1分，共50分。（参考答案在文章末尾）

1. cron 后台常驻程序 (daemon) 用于： 

A. 负责文件在网络中的共享 

B. 管理打印子系统

C. 跟踪管理系统信息和错误 

D. 管理系统日常任务的调度


2. 在大多数Linux发行版本中，以下哪个属于块设备 (block devices) ？ 

A. 串行口

B. 硬盘 

C. 虚拟终端

D. 打印机


3. 下面哪个Linux命令可以一次显示一页内容？ 

A. pause 

B. cat 

C. more 

D. grep 


4. 怎样了解您在当前目录下还有多大空间？ 

A. Use df 

B. Use du / 

C. Use du . 

D. Use df . 


5. 怎样更改一个文件的权限设置？ 

A. attrib 

B. chmod 

C. change 

D. file 


6. 假如您需要找出 /etc/my.conf 文件属于哪个包 (package) ，您可以执行： 

A. rpm -q /etc/my.conf 

B. rpm -requires /etc/my.conf 

C. rpm -qf /etc/my.conf 

D. rpm -q | grep /etc/my.conf 


7. 假如当前系统是在 level 3 运行，怎样不重启系统就可转换到 level 5 运行？ 

A. Set level = 5 

B. telinit 5 

C. run 5 

D. ALT-F7-5 


8. 那个命令用于改变 IDE 硬盘的设置？ 

A. hdparam 

B. ideconfig 

C. hdparm 

D. hddparm


9. 下面哪个命令可以列出定义在以后特定时间运行一次的所有任务？

A. atq

B. cron

C. batch

D. at


10.下面命令的作用是：set PS1="[\u\w\t]\\$" ; export PS1

A. 改变错误信息提示

B. 改变命令提示符

C. 改变一些终端参数

D. 改变辅助命令提示符


11.作为一个管理员，你希望在每一个新用户的目录下放一个文件 .bashrc ，那么你应该在哪个目录下放这个文件，以便于新用户创建主目录时自动将这个文件复制到自己的目录下。

A. /etc/skel/

B. /etc/default/

C. /etc/defaults/

D. /etc/profile.d/


12.在bash中，export命令的作用是：

A. 在子shell中运行命令

B. 使在子shell中可以使用命令历史记录

C. 为其它应用程序设置环境变量

D. 提供NFS分区给网络中的其它系统使用



13.在使用了shadow口令的系统中，/etc/passwd和/etc/shadow两个文件的权限正确的是：

A. -rw-r----- , -r--------

B. -rw-r--r-- , -r--r--r--

C. -rw-r--r-- , -r--------

D. -rw-r--rw- , -r-----r--


14．下面哪个参数可以删除一个用户并同时删除用户的主目录？

A. rmuser -r

B. deluser -r

C. userdel -r

D. usermgr -r


15．有一个备份程序mybackup，需要在周一至周五下午1点和晚上8点各运行一次，下面哪条crontab的项可以完成这项工作？

A. 0 13,20 * * 1,5 mybackup

B. 0 13,20 * * 1,2,3,4,5 mybackup

C. * 13,20 * * 1,2,3,4,5 mybackup

D. 0 13,20 1,5 * *  mybackup


16．如何从当前系统中卸载一个已装载的文件系统

A. umount

B. dismount

C. mount -u

D. 从 /etc/fstab 中删除这个文件系统项


17．如果你的umask设置为022，缺省的你创建的文件的权限为：

A. ----w--w-

B. -w--w----

C. r-xr-x---

D. rw-r--r--


18．在一条命令中如何查找一个二进制命令 Xconfigurator 的路径？

A. apropos Xconfigurator 

B. find Xconfigurator

C. where Xconfigurator

D. which Xconfigurator


19．哪一条命令用来装载所有在 /etc/fstab 中定义的文件系统？

A. amount

B. mount -a

C. fmount

D. mount -f


20．运行一个脚本，用户不需要什么样的权限？

A. read

B. write

C. execute

D. browse on the directory


21．在Linux中，如何标识接在IDE0上的slave硬盘的第2个扩展分区？

A. /dev/hdb2

B. /dev/hd1b2

C. /dev/hdb6

D. /dev/hd1b6


22．在应用程序起动时，如何设置进程的优先级？

A. priority

B. nice

C. renice

D. setpri


23．在 bash 中, 在一条命令后加入"1>&2" 意味着：

A. 标准错误输出重定向到标准输入

B. 标准输入重定向到标准错误输出

C. 标准输出重定向到标准错误输出

D. 标准输出重定向到标准输入


24．下面哪条命令可以把f1.txt复制为f2.txt?

A. cp f1.txt | f2.txt

B. cat f1.txt | f2.txt

C. cat f1.txt > f2.txt

D. copy f1.txt | f2.txt


25．显示一个文件最后几行的命令是：

A. tac

B. tail

C. rear

D. last



26.如何快速切换到用户John的主目录下？

A. cd @John

B. cd #John

C. cd &John

D. cd ~John


27.把一个流中所有字符转换成大写字符，可以使用下面哪个命令？

A. tr a-z A-Z

B. tac a-z A-Z 

C.sed /a-z/A-Z

D. sed --toupper


28.使用什么命令可以查看Linux的启动信息？

A. mesg -d

B. dmesg

C. cat /etc/mesg

D. cat /var/mesg



29.运行级定义在：

A. in the kernel

B. in /etc/inittab

C. in /etc/runlevels

D. using the rl command



30.如何装载(mount)上在 /etc/fstab 文件中定义的所有文件系统？

A. mount -a

B. mount /mnt/*

C. mount 

D. mount /etc/fstab



31.使用ln命令将生成了一个指向文件old的符号链接new，如果你将文件old删除，是否还能够访问文件中的数据？

A. 不可能再访问

B. 仍然可以访问

C. 能否访问取决于文件的所有者

D. 能否访问取决于文件的权限



32.xt2fs文件系统中，缺省的为root用户保留多大的空间？

A. 3%

B. 5%

C. 10%

D. 15%



33.哪个命令用来显示系统中各个分区中inode的使用情况？

A. df -i

B. df -H

C. free -b

D. du -a -c /


34.多数Linux发行版本中，图形方式的运行级定义为？

A. 1

B. 2

C. 3

D. 5



35.在系统文档中找到关于print这个单词的所有说明？

A. man print

B. which print

C. locate print

D. apropos print



36.man 5 passwd 含义是？

A. 显示 passwd 命令的使用方法

B. 显示 passwd 文件的结构

C. 显示 passwd 命令的说明的前五行

D. 显示关于passwd的前五处说明文档。



37.如何在文件中查找显示所有以"*"打头的行？

A. find \* file

B. wc -l * < file

C. grep -n * file

D. grep ‘^\*’ file



38.在ps命令中什么参数是用来显示所有用户的进程的？

A. a

B. b

C. u

D. x



39.显示二进制文件的命令是？

A. od

B. vil

C. view

D. binview



40.如何显示Linux系统中注册的用户数（包含系统用户）？

A. account -l

B. nl /etc/passwd |head

C. wc --users /etc/passwd

D. wc --lines /etc/passwd



41.在一行结束位置加上什么符号，表示未结束，下一行继续？

A. /

B. \

C. ;

D. |



42.命令 kill 9 的含义是：

A. kills the process whose PID is 9.

B. kills all processes belonging to UID 9.

C. sends SIGKILL to the process whose PID is 9.

D. sends SIGTERM to the process whose PID IS 9.



43.如何删除一个非空子目录/tmp？

A. del /tmp/*

B. rm -rf /tmp

C. rm -Ra /tmp/*

D. rm -rf /tmp/*



44.使用什么命令可以在今天午夜运行命令 cmd1 ？

A. at midnight cmd1

B. cron -at "00:00" cmd1

C. batch -t "00:00" < cmd1

D. echo "cmd1" | at midnight



45.你的系统使用增量备份策略，当需要恢复系统时，你需要按什么顺序恢复备份数据？

A. 最后一次全备份，然后从最早到最近的增量备份

B. 最后一次全备份，然后从最近到最早的增量备份

C. 最早到最近的增量备份，然后最后一次全备份

D. 最近到最早的增量备份，然后最后一次全备份



46.对所有用户的变量设置，应当放在哪个文件下？

A. /etc/bashrc

B. /etc/profile

C. ~/.bash_profile

D. /etc/skel/.bashrc



47.Linux系统中，一般把命令 ls 定义为 ls --color 的别名，以便以不同颜色来标识不同类型的文件。但是，如何能够使用原先的ls命令？

A. \ls

B. ;ls

C. ls $$

D. ls --noalias



48.在Linux系统中的脚本文件一般以什么开头？

A. $/bin/sh

B. #!/bin/sh

C. use /bin/sh

D. set shell=/bin/sh



49.下面哪种写法表示如果cmd1成功执行，则执行cmd2命令？

A. cmd1&&cmd2

B. cmd1|cmd2

C. cmd1;cmd2

D. cmd1||cmd2



50.在哪个文件中定义网卡的I/O地址？

A. cat /proc/modules

B. cat /proc/devices

C. cat /proc/ioports

D. cat /io/dma

参考答案：

01.D    02.B    03.C    04.C    05.B

06.C    07.B    08.C    09.A    10.B

11.A    12.C    13.C    14.C    15.B

16.A    17.D    18.D    19.B    20.B

21.C    22.B    23.C    24.C    25.B

26.D    27.A    28.B    29.B    30.A

31.A    32.C    33.A    34.D    35.D

36.B    37.D    38.A    39.A    40.D

41.B    42.D    43.B    44.D    45.B

46.B    47.A    48.B    49.A     50.C



