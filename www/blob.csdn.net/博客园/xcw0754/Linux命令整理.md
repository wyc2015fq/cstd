# Linux命令整理 - xcw0754 - 博客园
# [Linux命令整理](https://www.cnblogs.com/xcw0754/p/4921025.html)
　　附：[vim的入门书](http://www.jianshu.com/p/bcbe916f97e1)，感觉比vimtutor还要赞一点。
　　看完[《每天一个linux命令系列 》](http://www.cnblogs.com/peida/archive/2012/12/05/2803591.html)基本上就差不多了，但是还是有一些需要补上的。
 日常+生癖命令：
（1）删除软件：sudo apt-get autoremove --purge 软件名
（2）更新源：sudo apt-get update
（3）复制文件：cp 源文件 目标位置+名称
（4）查看c/c++文件的汇编代码：编译时加-g选项，再 objdump -S 文件名 即可。
（5）[Linux添加/删除用户和用户组](http://www.cnblogs.com/xd502djj/archive/2011/11/23/2260094.html)
（6）[linux下常用FTP命令 1. 连接ftp服务器](http://www.cnblogs.com/xiaochaohuashengmi/archive/2011/10/24/2222686.html)
（7）[Ubuntu使用FTP时显示500 Illegal PORT command](http://blog.csdn.net/simmerlee/article/details/8269029)
（8）[LINUX的FTP使用方法](http://www.cnblogs.com/xcw0754/p/4963158.html)
（9）[objdump命令的使用](http://blog.csdn.net/beyondioi/article/details/7796414) （用于反汇编二进制文件）
（10）以树形显示某个文件夹下所有文件：tree 
（11）以树形显示进程之间的关系：pstree
（12）命令行抓包工具（大部分是ip包）：tcpdump
常用命令：
（1）pwd命令：
　　打印当前所在目录的完整路径。这个命令比较简单，只有两个选项-L或-P。
　　-L　　表示显示逻辑路径。即你从哪里一层层来的，就打印这个。
　　-P　　表示显示物理路径。即你可能从很复杂的软链接中飞来飞去，但是所进目录并不是物理存在的，加此选项会显示你真实所在目录的路径。
（2）man命令：
　　用于查看各种手册或者命令使用方法。下面这篇文章讲得不错。
[Linux man命令的使用方法](http://www.cnblogs.com/hnrainll/archive/2011/09/06/2168604.html)
（3）cp命令：
　　复制东西所需要的命令。格式：cp [源头] [目的]
　　[注意]顶多可以帮你创建一个目录，不能多个。下面选项中冲突的会产生覆盖，即前者总是被覆盖，如-i和-n选项。
 　　-a：　　此参数的效果和同时指定"-dpR"参数相同； 
	　　-d：　　当复制符号连接时，把目标文件或目录也建立为符号连接，并指向与源文件或目录连接的原始文件或目录； 
	　　-f：　　强行复制文件或目录，不论目标文件或目录是否已存在； 
	　　-i：　　覆盖既有文件之前先询问用户； 
	　　-l：　　对源文件建立硬连接，而非复制文件； 
	　　-p：　　保留源文件或目录的属性； 
	　　-R/r：　递归处理，将指定目录下的所有文件与子目录一并处理； 
	　　-s：　　对源文件建立符号连接，而非复制文件； 
	　　-u：　　使用这项参数后只会在源文件的更改时间较目标文件更新时或是名称相互对应的目标文件并不存在时，才复制文件； 
	　　-S：　　在备份文件时，用指定的后缀“SUFFIX”代替文件的默认后缀； 
	　　-b：　　覆盖已存在的文件目标前将目标文件备份； 
	　　-v：　　详细显示命令执行的操作。
（4）ln命令（link命令）：
　　用于创建链接。格式：ln oldfile newfile
　　硬链接：定义了文件的别名，直接对原文件进行操作。目录不允许被创建硬链接，而目录..就是硬链接。
　　软链接：创建一个数据块来保存真实文件的路径。
　　见图即可理解软硬链接的区别了。
![图 2. 软链接的访问](http://www.ibm.com/developerworks/cn/linux/l-cn-hardandsymb-links/image002.jpg)
　　-b或--backup：　　删除，覆盖目标文件之前的备份； 
　　-d或-F或--directory：　　建立目录的硬连接； 
　　-f或--force：　　强行建立文件或目录的连接，不论文件或目录是否存在； 
　　-i或--interactive：　　覆盖既有文件之前先询问用户； 
　　-n或--no-dereference：　　把符号连接的目的目录视为一般文件； 
　　-s或--symbolic：　　对源文件建立符号连接，而非硬连接； 
　　-S<字尾备份字符串>或--suffix=<字尾备份字符串>：　　用"-b"参数备份目标文件后，备份文件的字尾会被加上一个备份字符串，预设的备份字符串是符号“~”，用户可通过“-S”参数来改变它； 
　　-v或--verbose：　　显示指令执行过程； 
　　-V<备份方式>或--version-control=<备份方式>：　　用“-b”参数备份目标文件后，备份文件的字尾会被加上一个备份字符串，这个字符串不仅可用“-S”参数变更，当使用“-V”参数<备份方式>指定不同备份方式时，也会产生不同字尾的备份字符串；
（5）touch命令：
　　用于更新访问文件的时间（也可以直接touch新文件名用于创建空文件）。格式：touch 选项 参数
　　-a：或--time=atime或--time=access或--time=use 只更改存取时间； 
　　-c：或--no-create 不建立任何文件； 
　　-d：<时间日期> 使用指定的日期时间，而非现在的时间； 改了“最近改动时间”。
　　-m：或--time=mtime或--time=modify 只更该变动时间；改了全部时间为当前时间。 
　　-r：<参考文件或目录> 把指定文件或目录的日期时间，统统设成和参考文件或目录的日期时间相同； 
　　-t：<日期时间> 使用指定的日期时间，而非现在的时间； 
（6）mv命令：
　　用于移动文件or目录（也可以对文件更名），有一个目标文件就行了。格式：mv 选项 源文件 [源文件] 目标文件 
　　--backup=<备份模式>：若需覆盖文件，则覆盖前先行备份； 模式请使用help查看。
　　-b：当文件存在时，覆盖前，为其创建一个备份； 
　　-f：若目标文件或目录与现有的文件或目录重复，则直接覆盖现有的文件或目录； 
　　-i：交互式操作，覆盖前先行询问用户，如果源文件与目标文件或目标目录中的文件同名，则询问用户是否覆盖目标文件。用户输入”y”，表示将覆盖目标文件；输入”n”，表示取消对源文件的移动。这样可以避免误将文件覆盖。 
　　-n：不覆盖已存在文件，即若已存在相同文件名，不进行mv操作。[ifn]中只能有一个存在。
　　--strip-trailing-slashes：删除源文件中的斜杠“/”； 
　　-S<后缀>：为备份文件指定后缀，而不使用默认的后缀； 
　　--target-directory=<目录>：指定源文件要移动到目标目录； 
　　-u：当源文件比目标文件新或者目标文件不存在时，才执行移动操作。
　　-v：打印移动的情况。
（7）file命令：
　　用于打印某些文件的类型。一个文件中会保存其真实的文件类型，即使把后缀给改了也可以显示出其真实类型。参数不多，能用得到的也不多。格式：file [参数] 文件名。
　　-b：　　列出辨识结果时，不显示文件名称； 
　　-c：　　详细显示指令执行过程，便于排错或分析程序执行的情形； 
　　-f<名称文件>：　　指定名称文件，其内容有一个或多个文件名称时，让file依序辨识这些文件，格式为每列一个文件名称； 
　　-L：　　直接显示符号连接所指向的文件类别； 
　　-m<魔法数字文件>：　　指定魔法数字文件； 
　　-v：　　显示版本信息； 
　　-z：　　尝试去解读压缩文件的内容。
（8）whereis命令：
　　以程序名查找文件，只搜3种文件，与find命令的功能有点区别。格式：whereis [参数] 程序名
　　-b   定位可执行文件。
　　-m   定位帮助文件。
　　-s   定位源代码文件。
　　-u   搜索默认路径下除可执行文件、源代码文件、帮助文件以外的其它文件。
　　-B   指定搜索可执行文件的路径。
　　-M   指定搜索帮助文件的路径。
　　-S   指定搜索源代码文件的路径。
　　更详细请见[whereis命令](http://www.cnblogs.com/peida/archive/2012/11/09/2761928.html)。
（9）top命令：
　　用于查看当前进程所占CPU及内存的一些动态信息，还支持交互。功能类似于任务管理器。格式：top 参数。
　　-p　　指定进程号。
　　-u　　指定用户。
　　-d　　指定刷新的周期。
　　交互命令（只需要按下单个字母）：
　　s　　 改变刷新的周期。
　　k　　终止一个进程。
　　b和x　　配合显示当前运行的进程，以及排序所用的第一关键列，shift+>或<可以改变这项。
top指令也可以将结果重定向到文件的模式：
-b 用batch模式，即可重定向
-n 取多少次数据
-d 每次刷新的时间间隔
　　更加详细的请看[top命令](http://www.cnblogs.com/peida/archive/2012/12/24/2831353.html)。
（10）crontab命令：
　　没有试用过，不知道是不是一个因系统而异的命令。详情可看[crontab命令](http://man.linuxde.net/crontab)。
（11）killall命令：
　　用于杀死具有同样特征的多个进程，而kill是用于发信号给单个进程。格式：killall 选项 参数 程序名。
　　此命令可以用于杀死（或发送信号[选项-s]）一组进程，所杀进程即具有相同标识。比如同个用户，同个进程组等。可以精确匹配程序名，也可以忽略大小写匹配，还可以在发送信号前进行询问再确定发或不发。此命令没有怎么试，我怕一下就得关机了，选项比较少，直接man killall即可。
（12）pkill命令：
　　也是用于给进程发送信号的，也支持群发，选项很多，比如发给某个终端所创建的所有进程，指定ppid的所有进程，进程组ID等等。
　　具体看得看man pkill。
（13）grep命令：
　　这命令主要是正则表达式的运用，可以搜文件和目录，也可以搜文本（直接用管道符|）。
 　　-E, --extended-regexp PATTERN 　　是正则表达式(缩写为 ERE)
  　　-G, --basic-regexp        PATTERN 　　　　是一个基本正则表达式(缩写为 BRE)
  　　-e, --regexp=PATTERN 　　　　　　     用 PATTERN 来进行匹配操作（采用多PATTERN时有用）
  　　-f, --file=FILE           PATTERN 　　　　　　在指定的FILE中
  　　-i, --ignore-case 　　　　　　　　　　        忽略大小写
  　　-w, --word-regexp 　　　　　　　　        强制 PATTERN 仅完全匹配单词
  　　-x, --line-regexp 　　　　　　　　        强制 PATTERN 仅完全匹配一行
  　　-v, --invert-match 　　　　　　　　       选中不匹配的行
  　　-m, --max-count=NUM 　　　　      NUM 次匹配后停止
  　　-b, --byte-offset 　　　　 　　 　　      输出的同时打印字节偏移
  　　-n, --line-number 　　　　　　        打印匹配项在文件中的行号
      　　--line-buffered 　　　　　　　　      每行输出清空　　
  　　-H, --with-filename 　　　　　　      为每一匹配项打印文件名
  　　-o, --only-matching　　　　　　       每行只显示匹配的PATTERN
  　　-r, --recursive 　　　　　　　　          递归下去，file为目录时有效。
  　　-R, --dereference-recursive 　　 likewise, but follow all symlinks
      　　--include=FILE_PATTERN 　　　　只查找匹配FILE_PATTERN 的文件
      　　--exclude=FILE_PATTERN 　　跳过匹配FILE_PATTERN 的文件和目录
      　　--exclude-from=FILE 　　　　  跳过所有除FILE 以外的文件
      　　--exclude-dir=PATTERN 　　　　跳过所有匹配PATTERN 的目录。
  　　-L, --files-without-match　　　　 只打印不匹配FILEs 的文件名
  　　-l, --files-with-matches 　　　　 只打印匹配FILES 的文件名
  　　-c, --count 　　　　　　　　　　              只打印每个FILE 中的匹配行数目　　
  　　-Z, --null 　　　　　　　　　　　　               在FILE 文件最后打印空字符
  　　-B, --before-context=NUM 　　 打印以搜到文本为结束的NUM 行
  　　-A, --after-context=NUM 　　  打印以搜到文本为起始的NUM 行
  　　-C, --context=NUM 　　　　        打印输出文本NUM行
（14）find命令：
　　用于查找文件（包括目录，链接文件等），可以使用多种条件进行查找，比如按文件大小，文件类型，文件时间，递归层数，。找到之后还能有多种操作，比如删除掉，打印出文件名等。
　　更详细的请看[find命令](http://man.linuxde.net/find)，以及查看find手册。
（15）time命令：
　　用于统计指定命令所花时间，比如用户时间，系统时间等。格式：time [time的参数] 命令 [命令的参数]
　　要注意在命令后的参数全部归于命令的，而不是time的。结果还可以格式化输出到文件中。
　　-o 将time命令的结果输出到某个文件中
　　-a	在-o的基础上，将结果append在原文件尾
　　-f	指定输出的格式，有多种可选，类似于printf函数那样。
 　　更详细的请看[time命令](http://man.linuxde.net/time)。
（16）tr命令：
　　将标准输入进行字符串替换后输出到标准输出。格式：tr s1 s2
      将s1全部替换成s2，举例: tr '\t' '\n'
（16）tee命令：
　　将标准输入逐个复制到指定文件中，并输出到标准输出。可以指定追加参数-a。
命令详解：
（1）source命令：
```
1 source/etc/profile是让/etc/profile文件修改后立即生效，
 2 还有一种方法是：. /etc/profile
 3 注意：.和/etc/profile有空格
 4 linux中source命令
 5 source命令也称为“点命令”也就是一个符号（.）.source命令通常用于重新执行刚修改的初始或文件，使之立即生效，而不必注销并重新登录。
 6 用法： source filename 或. filename
 7 附：source命令除了上述的用途之外，还有一个用途。
 8 在对编译系统核心时常常需要输入一长串的命令，如：
 9 make mrproper
10 make menuconfig
11 make dep
12 make clean
13 make bzImage
14 …………
15 如果把这些命令做成一个文件，让它自动顺序执行，对于需要多次反复编译系统核心的用户来说会很方便，而用source命令就可以做到这一点，它的作用就是把一个文件的内容当成shell来执行，先在linux的源代码目录下（如/usr/src/linux-2.4.20）建立一个文件，如make_command，在其中输入一下内容：
16 make mrproper &&
17 make menuconfig &&
18 make dep &&
19 make clean &&
20 make bzImage &&
21 make modules &&
22 make modules_install &&
23 cp arch/i386/boot/bzImage /boot/vmlinuz_new &&
24 cp System.map /boot &&
25 vi /etc/lilo.conf &&
26 lilo -v
27 文件建立好之后，每次编译核心的时候，只需要在/usr/src/linux-2.4.20下输入：
28 source make_command
29 即可，如果你用的不是lilo来引导系统，可以把最后两行去掉，配置自己的引导程序来引导内核。
30 顺便补充一点，&&命令表示顺序执行由它连接的命令，但是只有它之前的命令成功执行完成了之后才可以继续执行它后面的命令。
source
```
（2）ulimit命令：选项-a可以显示本linux系统的程序的一些设置及限制，比如程序栈的大小。也可以直接加 -x 数字 来修改，x表示对应的选项，数字表示大小，也可以为unlimited。
（3）chmod命令：用于更改文件权限，用法如 chmod u+rwx test.cpp ，其中u可以替换为g或o，表示用户，组或其他人，+号表示加权限，减号则相反，后3个表示权限，最后是文件名了。
（4）umask命令：用于显示“新创建的文件的默认权限”。创建时，文件默认666，目录默认777，减去umask的位就是结果了，如文件的666-245=422=-r---w---w-。
（5）apt-get -f install 是修复依赖关系（depends）的命令，假如系统上有某个package不满足依赖条件，这个命令就会自动修复，安装那个package依赖的package。即缺少package时安装一下。
　　apt-get purge用于卸载并清除软件包的配置。
（7）wc命令：用于统计文件的数量、行数、字节数、字符数、最长行的长度、单词数等等。
（8）find命令：用于在当前路径下查找，比如find -name *.cpp 就是查找出所有以.cpp为后缀的文件。
（9）cat，more，less命令：这三个命令都用于显示文件。但是cat 文件如果太大的话前面的部分会被剪除。more是讲文件按页来显示的。less是滚屏显示（不会自动滚的）。
有用的操作：
（1）安装.deb的包：使用dpkg -i 包名  即可，但是可能会有依赖问题，即缺少其他安装包，那么直接apt-get install -f 全部给补上。
资源相关的操作：
（1）系统
# uname -a #查看内核/操作系统/CPU信息
# head -n 1 /etc/issue #查看操作系统版本
# cat /proc/cpuinfo #查看CPU信息
# hostname #查看计算机名
# lspci -tv #列出所有PCI设备
# lsusb -tv #列出所有USB设备
# lsmod #列出加载的内核模块
# env #查看环境变量
（2）资源
# free -m #查看内存使用量和交换区使用量
# df -h #查看各分区使用情况
# du -sh <目录名> #查看指定目录的大小
# grep MemTotal /proc/meminfo #查看内存总量
# grep MemFree /proc/meminfo #查看空闲内存量
# uptime #查看系统运行时间、用户数、负载
# cat /proc/loadavg #查看系统负载
（3）磁盘和分区
# mount | column -t #查看挂接的分区状态
# fdisk -l #查看所有分区
# swapon -s #查看所有交换分区
# hdparm -i /dev/hda #查看磁盘参数(仅适用于IDE设备)
# dmesg | grep IDE #查看启动时IDE设备检测状况
（4）网络
# ifconfig #查看所有网络接口的属性
# iptables -L #查看防火墙设置
# route -n #查看路由表
# netstat -lntp #查看所有监听端口
# netstat -antp #查看所有已经建立的连接
# netstat -s #查看网络统计信息
（5）进程
# ps -ef #查看所有进程
# top #实时显示进程状态
（6）用户
# w #查看活动用户
# id <用户名> #查看指定用户信息
# last #查看用户登录日志
# cut -d: -f1 /etc/passwd #查看系统所有用户
# cut -d: -f1 /etc/group #查看系统所有组
# crontab -l #查看当前用户的计划任务
（7）服务
# chkconfig --list #列出所有系统服务
# chkconfig --list | grep on #列出所有启动的系统服务
（8）程序
# rpm -qa #查看所有安装的软件包

