
# Linux Android 常用shell命令汇总 不断更新 - 嵌入式Linux - CSDN博客

2018年09月06日 15:30:11[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：168所属专栏：[微信公众号:嵌入式linux](https://blog.csdn.net/column/details/26885.html)



## 文章目录
## [@[toc]
01 前言](#toc
01__1)
## [02 正文](#02__22)

## 01 前言
## Linux shell命令应该算是非常入门的东西，但是实际上在使用的时候，会遇到各种各样的问题，前几天我在我们的项目上需要做一个功能，根据进程名字杀死这个进程，下面是过程
## 1、我们正常需要的操作是
## ps |grep xxx
## kill -9 xx
## 2、kill命令是常用的，但是用killall命令会更快
## killall -9 xxx
## 3、令我困扰的是我用的那个没有killall然后我做了下面的操作
## adb shell ps |grep speech |awk ‘{print $2}’ > 1.txt cat 1.txt |xargs
## -n1 adb shell kill -9
## 我用这个的原因是用一条指令重定向总是各种问题，正常情况下用这条就可以了
## adb shell ps |grep speech |awk ‘{print $2}’|xargs -n1 kill -9
## 02 正文
## \#\#repo 管理git
## repo是一个管理工具，用来管理所有git仓库
## 比如repo forall -c ‘pwd;git log 001dev…002dev’
## 上面这个命令用来比较所有仓库这两个分支的差别
## \#\#关机/重启
## 关机（必须用root用户）
## shutdown -h now  \#\# 立刻关机
## shutdown -h +10  \#\#  10分钟以后关机
## shutdown -h 12:00:00  \#\#12点整的时候关机
## halt   \#  等于立刻关机
## 重启
## shutdown -r now
## reboot   \# 等于立刻重启
## \#\#echo
## root@ubuntu:~\# a=“hello,world”
## root@ubuntu:~\# echo a
## a
## root@ubuntu:~\# echo &a
## [1] 3091
## a: command not found
## [1]+  Done                    echo
## root@ubuntu:~\# echo $a
## hello,world
## root@ubuntu:~\#
## \#\#vim文本编辑器
## \#\#\#最基本用法
## vi  somefile.4
## 1 首先会进入“一般模式”，此模式只接受各种快捷键，不能编辑文件内容
## 2 按i键，就会从一般模式进入编辑模式，此模式下，敲入的都是文件内容
## 3 编辑完成之后，按Esc键退出编辑模式，回到一般模式；
## 4 再按：，进入“底行命令模式”，输入wq命令，回车即可
## \#\#\#常用快捷键
## 一些有用的快捷键（在一般模式下使用）：
## a   在光标后一位开始插入
## A   在该行的最后插入
## I   在该行的最前面插入
## gg   直接跳到文件的首行
## G    直接跳到文件的末行
## dd    删除一行
## 3dd   删除3行
## yy    复制一行
## 3yy   复制3行
## p     粘贴
## u     undo
## v        进入字符选择模式，选择完成后，按y复制，按p粘贴
## ctrl+v   进入块选择模式，选择完成后，按y复制，按p粘贴
## shift+v  进入行选择模式，选择完成后，按y复制，按p粘贴
## \#\#\#查找并替换
## \#\#\#\#1 显示行号
## :set nu
## \#\#\#\#2 隐藏行号
## :set nonu
## \#\#\#\#3 查找关键字
## :/you       \#\# 效果：查找文件中出现的you，并定位到第一个找到的地方，按n可以定位到下一个匹配位置（按N定位到上一个）
## \#\#\#\#4 替换操作
## 😒/sad/bbb    查找光标所在行的第一个sad，替换为bbb
## :%s/sad/bbb      查找文件中所有sad，替换为bbb
## 拷贝/删除/移动/更换文件名字
## cp  somefile.1   /home/hadoop/
## rm /home/hadoop/somefile.1
## rm -f /home/hadoop/somefile.1
## mv /home/hadoop/somefile.1 …/
## mv a.txt b.txt
## [//把a.txt更名为b.txt](//xn--a-g84b.xn--txtb-xh5fn22a1p8a.txt)
## \#\#打包压缩
## \#\#\#1、gzip压缩
## gzip a.txt
## \#\#\#2、解压
## gunzip a.txt.gz
## gzip -d a.txt.gz
## \#\#\#3、bzip2压缩
## bzip2 a
## \#\#\#4、解压
## bunzip2 a.bz2
## bzip2 -d a.bz2
## \#\#\#5、打包：将指定文件或文件夹
## tar -cvf bak.tar  ./aaa
## 将/etc/password追加文件到bak.tar中
## tar -rvf bak.tar /etc/password
## \#\#\#6、解压
## tar -xvf bak.tar
## \#\#\#7、打包并压缩
## tar -zcvf a.tar.gz  aaa/
## \#\#\#8、解包并解压缩(重要的事情说三遍!!!)
## tar  -zxvf  a.tar.gz
## 解压到/usr/下
## tar  -zxvf  a.tar.gz  -C  /usr
## \#\#\#9、查看压缩包内容
## tar -ztvf a.tar.gz
## zip/unzip
## \#\#\#10、打包并压缩成bz2
## tar -jcvf a.tar.bz2
## \#\#\#11、解压bz2
## tar -jxvf a.tar.bz2
## \#\#常用查找命令的使用
## \#\#\#1、查找可执行的命令所在的路径：
## which ls
## \#\#\#2、查找可执行的命令和帮助的位置：
## whereis ls
## \#\#\#3、从某个文件夹开始查找文件
## find / -name “hadooop*”
## find / -name “hadooop*” -ls
## \#\#\#4、查找并删除
## find / -name “hadooop*” -ok rm {} ;
## find / -name “hadooop*” -exec rm {} ;
## \#\#\#5、查找用户为hadoop的文件
## find  /usr  -user  hadoop  -ls
## \#\#\#6、查找用户为hadoop的文件夹
## find /home -user hadoop -type d -ls
## \#\#\#7、查找权限为777的文件
## find / -perm -777 -type d -ls
## \#\#\#8、在指定目录不分大小写查找某个文件
## find ./sound/ -iname ft56Q.c
## \#\#\#9、显示命令历史
## history
## \#\#grep命令
## \#\#\#最常用的-全字匹配
## grep -wrn  weiqifa ./sound/
## \#\#\#查找 某个字符串但是不在指定文件夹查找
## grep -E “http”  ./ -R --exclude-dir=./sound/
## grep -E “http”  . -R --exclude-dir={.git,res,bin}
## 排除扩展名为 java 和 js 的文件
## grep -E “http”  . -R --exclude=*.{java,js}
## \#\#\#1 基本使用
## 查询包含hadoop的行
## grep hadoop /etc/password
## grep aaa  ./*.txt
## \#\#\#2 cut截取以:分割保留第七段
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\# grep dsl /etc/passwd | cut -d: -f7
## /bin/bash
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\# grep dsl /etc/passwd
## dsl❌1000:1000:dsl,:/home/dsl:/bin/bash
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\#
## \#\#\#3 查询不包含hadoop的行
## grep -v hadoop /etc/passwd
## \#\#\#4 正则表达包含hadoop
## grep ‘hadoop’ /etc/passwd
## \#\#\#5 正则表达(点代表任意一个字符)
## grep ‘h.*p’ /etc/passwd
## \#\#\#6 正则表达以hadoop开头
## grep ‘^hadoop’ /etc/passwd
## \#\#\#7 正则表达以hadoop结尾
## grep ‘hadoop$’ /etc/passwd
## 规则：
## .  : 任意一个字符
## a* : 任意多个a(零个或多个a)
## a? : 零个或一个a
## a+ : 一个或多个a
## .* : 任意多个任意字符
## . : 转义.
## o{2} : o重复两次
## 查找不是以\#开头的行
## grep -v ‘^\#’ a.txt | grep -v ‘^$’
## 以h或r开头的
## grep ‘
## [1](#fn1)
## ’ /etc/passwd
## 不是以h和r开头的
## grep ‘
## [
## hr]’ /etc/passwd
## 不是以h到r开头的
## grep ‘
## [
## h-r]’ /etc/passwd
## linux文件权限的描述格式解读
## drwxr-xr-x      （也可以用二进制表示  111 101 101  -->  755）
## d：标识节点类型（d：文件夹   -：文件  l:链接）
## r：可读   w：可写    x：可执行
## 第一组rwx：  \#\# 表示这个文件的拥有者对它的权限：可读可写可执行
## 第二组r-x：  \#\# 表示这个文件的所属组用户对它的权限：可读，不可写，可执行
## 第三组r-x：  \#\# 表示这个文件的其他用户（相对于上面两类用户）对它的权限：可读，不可写，可执行
## \#\#修改文件权限
## chmod g-rw haha.dat         \#\# 表示将haha.dat对所属组的rw权限取消
## chmod o-rw haha.dat         \#\# 表示将haha.dat对其他人的rw权限取消
## chmod u+x haha.dat         \#\# 表示将haha.dat对所属用户的权限增加x
## chmod a-x haha.dat               \#\# 表示将haha.dat对所用户取消x权限
## 也可以用数字的方式来修改权限
## chmod 664 haha.dat
## 就会修改成   rw-rw-r–
## 如果要将一个文件夹的所有内容权限统一修改，则可以-R参数
## chmod -R 770 aaa/
## \#\#修改文件所有权
## <只有root权限能执行>
## chown angela  aaa        \#\# 改变所属用户
## chown :angela  aaa        \#\# 改变所属组
## chown angela:angela aaa/    \#\# 同时修改所属用户和所属组
## \#\#统计文件或文件夹的大小
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\# du -sh sound/
## 33M    sound/
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\#
## \#\#\#df -h    查看磁盘的空间
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\# df -h
## Filesystem      Size  Used Avail Use% Mounted on
## udev            1.9G     0  1.9G   0% /dev
## tmpfs           394M  6.4M  388M   2% /run
## /dev/sda1       294G   93G  186G  34% /
## tmpfs           2.0G  204K  2.0G   1% /dev/shm
## tmpfs           5.0M  4.0K  5.0M   1% /run/lock
## tmpfs           2.0G     0  2.0G   0% /sys/fs/cgroup
## tmpfs           394M   80K  394M   1% /run/user/1000
## root@ubuntu:~/kernel_rk3399_yan4_dev/kernel\#
## \#\#生成patch和应用patch
## \#\#\#生成patch
## 最常用
## git diff > 1.patch
## 使用git format-patch生成所需要的patch:
## 当前分支所有超前master的提交：
## git format-patch -M master
## 某次提交以后的所有patch:
## git format-patch 4e16 --4e16指的是commit名
## 从根到指定提交的所有patch:
## git format-patch --root 4e16
## 某两次提交之间的所有patch:
## git format-patch 365a…4e16 --365a和4e16分别对应两次提交的名称
## 某次提交（含）之前的几次提交：
## git format-patch –n 07fe --n指patch数，07fe对应提交的名称
## 故，单次提交即为：
## git format-patch -1 07fe
## git format-patch生成的补丁文件默认从1开始顺序编号，并使用对应提交信息中的第一行作为文件名。如果使用了-- numbered-files选项，则文件名只有编号，不包含提交信息；如果指定了–stdout选项，可指定输出位置，如当所有patch输出到一个文件；可指定-o
## 指定patch的存放目录；
## \#\#\#应用patch
## patch -p1 < 0001-Added-liuxingde-test.patch
## 先检查patch文件：git apply --stat newpatch.patch
## 检查能否应用成功：git apply --check newpatch.patch
## 打补丁：git am --signoff < newpatch.patch
## 喜欢可以关注微信公众号：嵌入式Linux
## ​
---

## hr
## [↩︎](#fnref1)


