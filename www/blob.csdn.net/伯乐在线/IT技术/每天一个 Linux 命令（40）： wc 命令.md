# 每天一个 Linux 命令（40）： wc 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/12/18/2822758.html)
Linux系统中的wc(Word Count)命令的功能为统计指定文件中的字节数、字数、行数，并将统计结果显示输出。
1．命令格式：

Shell
```
wc [选项]文件...
```
2．命令功能：
统计指定文件中的字节数、字数、行数，并将统计结果显示输出。该命令统计指定文件中的字节数、字数、行数。如果没有给出文件名，则从标准输入读取。wc同时也给出所指定文件的总统计数。
3．命令参数：

Shell
```
-c 统计字节数。
-l 统计行数。
-m 统计字符数。这个标志不能与 -c 标志一起使用。
-w 统计字数。一个字被定义为由空白、跳格或换行字符分隔的字符串。
-L 打印最长行的长度。
-help 显示帮助信息
--version 显示版本信息
```
4．使用实例：
实例1：查看文件的字节数、字数、行数
命令：

Shell
```
wc test.txt
```
输出：

Shell
```
[root@localhost test]# cat test.txt
hnlinux
peida.cnblogs.com
ubuntu
ubuntu linux
redhat
Redhat
linuxmint
[root@localhost test]# wc test.txt
7  8 70 test.txt
[root@localhost test]# wc -l test.txt
7 test.txt
[root@localhost test]# wc -c test.txt
70 test.txt
[root@localhost test]# wc -w test.txt
8 test.txt
[root@localhost test]# wc -m test.txt
70 test.txt
[root@localhost test]# wc -L test.txt
17 test.txt
```
说明：

Shell
```
7     8     70     test.txt
行数 单词数 字节数 文件名
```
实例2：用wc命令怎么做到只打印统计数字不打印文件名
命令：
输出：

Shell
```
[root@localhost test]# wc -l test.txt
```
7 test.txt

Shell
```
[root@localhost test]# cat test.txt |wc -l
7[root@localhost test]#
```
说明：
使用管道线，这在编写shell脚本时特别有用。
实例3：用来统计当前目录下的文件数
命令：

Shell
```
ls -l | wc -l
```
输出：

Shell
```
[root@localhost test]# cd test6
[root@localhost test6]# ll
总计 604
---xr--r-- 1 root mail  302108 11-30 08:39 linklog.log
---xr--r-- 1 mail users 302108 11-30 08:39 log2012.log
-rw-r--r-- 1 mail users     61 11-30 08:39 log2013.log
-rw-r--r-- 1 root mail       0 11-30 08:39 log2014.log
-rw-r--r-- 1 root mail       0 11-30 08:39 log2015.log
-rw-r--r-- 1 root mail       0 11-30 08:39 log2016.log
-rw-r--r-- 1 root mail       0 11-30 08:39 log2017.log
[root@localhost test6]# ls -l | wc -l
8
[root@localhost test6]#
```
说明：
数量中包含当前目录
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（15）：tail 命令](http://blog.jobbole.com/108995/)
[每天一个 Linux 命令（16）：which命令](http://blog.jobbole.com/109262/)
[每天一个 Linux 命令（17）：whereis 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（18）：locate 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（19）：find 命令概览](http://blog.jobbole.com/109265/)
[每天一个 Linux 命令（20）：find命令之exec](http://blog.jobbole.com/108996/)
[每天一个 Linux 命令（21）：find命令之xargs](http://blog.jobbole.com/108997/)
[每天一个 Linux 命令（22）：find 命令的参数详解](http://blog.jobbole.com/109268/)
[每天一个 Linux 命令（23）：Linux 目录结构](http://blog.jobbole.com/109506/)
[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)
[每天一个 Linux 命令（25）：Linux 文件属性详解](http://blog.jobbole.com/105269)
[每天一个 Linux 命令（26）：用 SecureCRT 来上传和下载文件](http://blog.jobbole.com/109539)
[每天一个 Linux 命令（27）：linux chmod 命令](http://blog.jobbole.com/109614)
[每天一个 Linux 命令（28）：tar 命令](http://blog.jobbole.com/109674/)
[每天一个 Linux 命令（29）: chgrp 命令](http://blog.jobbole.com/109696/)
[每天一个 Linux 命令（30）: chown 命令](http://blog.jobbole.com/109750/)
[每天一个 Linux 命令（31）: /etc/group 文件详解](http://blog.jobbole.com/109781/)
[每天一个 Linux 命令（32）：gzip 命令](http://blog.jobbole.com/109797/)
[每天一个 Linux 命令（33）：df 命令](http://blog.jobbole.com/109827/)
[每天一个 Linux 命令（34）： du 命令](http://blog.jobbole.com/109846/)
[每天一个 Linux 命令（35）： ln 命令](http://blog.jobbole.com/109869/)
[每天一个 Linux 命令（36）： diff 命令](http://blog.jobbole.com/109875/)
[每天一个 Linux 命令（37）： date 命令](http://blog.jobbole.com/109881/)
[每天一个 Linux 命令（38）： cal 命令](http://blog.jobbole.com/109886/)
[每天一个 Linux 命令（39）： grep 命令](http://blog.jobbole.com/109907/)
