# 每天一个 Linux 命令（21）：find命令之xargs - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/11/15/2770888.html)
在使用 find命令的-exec选项处理匹配到的文件时， find命令将所有匹配到的文件一起传递给exec执行。但有些系统对能够传递给exec的命令长度有限制，这样在find命令运行几分钟之后，就会出现溢出错误。错误信息通常是“参数列太长”或“参数列溢出”。这就是xargs命令的用处所在，特别是与find命令一起使用。
find命令把匹配到的文件传递给xargs命令，而xargs命令每次只获取一部分文件而不是全部，不像-exec选项那样。这样它可以先处理最先获取的一部分文件，然后是下一批，并如此继续下去。
在有些系统中，使用-exec选项会为处理每一个匹配到的文件而发起一个相应的进程，并非将匹配到的文件全部作为参数一次执行；这样在有些情况下就会出现进程过多，系统性能下降的问题，因而效率不高； 而使用xargs命令则只有一个进程。另外，在使用xargs命令时，究竟是一次获取所有的参数，还是分批取得参数，以及每一次获取参数的数目都会根据该命令的选项及系统内核中相应的可调参数来确定。
使用实例：
## 实例1： 查找系统中的每一个普通文件，然后使用xargs命令来测试它们分别属于哪类文件
命令：


```
find . -type f -print | xargs file
```
输出：


```
[root@localhost test]# ll
总计 312
-rw-r--r-- 1 root root 302108 11-03 06:19 log2012.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxrwx 2 root root   4096 11-12 19:32 test3
drwxrwxrwx 2 root root   4096 11-12 19:32 test4
[root@localhost test]# find . -type f -print | xargs file
./log2014.log: empty
./log2013.log: empty
./log2012.log: ASCII text
[root@localhost test]#
```
## 实例2：在整个系统中查找内存信息转储文件(core dump) ，然后把结果保存到/tmp/core.log 文件中
命令：


```
find / -name "core" -print | xargs echo "" >/tmp/core.log
```
输出：


```
[root@localhost test]# find / -name "core" -print | xargs echo "" >/tmp/core.log
[root@localhost test]# cd /tmp
[root@localhost tmp]# ll
总计 16
-rw-r--r-- 1 root root 1524 11-12 22:29 core.log
drwx------ 2 root root 4096 11-12 22:24 ssh-TzcZDx1766
drwx------ 2 root root 4096 11-12 22:28 ssh-ykiRPk1815
drwx------ 2 root root 4096 11-03 07:11 vmware-root
```
## 实例3:在当前目录下查找所有用户具有读、写和执行权限的文件，并收回相应的写权限
命令：


```
find . -perm -7 -print | xargs chmod o-w
```
输出：


```
[root@localhost test]# ll
总计 312
-rw-r--r-- 1 root root 302108 11-03 06:19 log2012.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxrwx 2 root root   4096 11-12 19:32 test3
drwxrwxrwx 2 root root   4096 11-12 19:32 test4
[root@localhost test]# find . -perm -7 -print | xargs chmod o-w
[root@localhost test]# ll
总计 312
-rw-r--r-- 1 root root 302108 11-03 06:19 log2012.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxr-x 2 root root   4096 11-12 19:32 test3
drwxrwxr-x 2 root root   4096 11-12 19:32 test4
[root@localhost test]#
```
说明：
执行命令后，文件夹scf、test3和test4的权限都发生改变
## 实例4：用grep命令在所有的普通文件中搜索hostname这个词
命令：


```
find . -type f -print | xargs grep "hostname"
```
输出：


```
[root@localhost test]# find . -type f -print | xargs grep "hostname"
./log2013.log:hostnamebaidu=baidu.com
./log2013.log:hostnamesina=sina.com
./log2013.log:hostnames=true[root@localhost test]#
```
## 实例5：用grep命令在当前目录下的所有普通文件中搜索hostnames这个词
命令：


```
find . -name * -type f -print | xargs grep "hostnames"
```
输出：


```
[root@peida test]# find . -name \* -type f -print | xargs grep "hostnames"
./log2013.log:hostnamesina=sina.com
./log2013.log:hostnames=true[root@localhost test]#
```
说明：
注意，在上面的例子中， 用来取消find命令中的*在shell中的特殊含义。
## 实例6：使用xargs执行mv
命令：


```
find . -name "*.log" | xargs -i mv {} test4
```
输出：


```
[root@localhost test]# ll
总计 316
-rw-r--r-- 1 root root 302108 11-03 06:19 log2012.log
-rw-r--r-- 1 root root     61 11-12 22:44 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:25 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxr-x 2 root root   4096 11-12 22:54 test3
drwxrwxr-x 2 root root   4096 11-12 19:32 test4
[root@localhost test]# cd test4/
[root@localhost test4]# ll
总计 0[root@localhost test4]# cd ..
[root@localhost test]# find . -name "*.log" | xargs -i mv {} test4
[root@localhost test]# ll
总计 12drwxr-xr-x 6 root root 4096 10-27 01:58 scf
drwxrwxr-x 2 root root 4096 11-13 05:50 test3
drwxrwxr-x 2 root root 4096 11-13 05:50 test4
[root@localhost test]# cd test4/
[root@localhost test4]# ll
总计 304
-rw-r--r-- 1 root root 302108 11-12 22:54 log2012.log
-rw-r--r-- 1 root root     61 11-12 22:54 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:54 log2014.log
[root@localhost test4]#
```
## 实例7：find后执行xargs提示xargs: argument line too long解决方法：
命令：


```
find . -type f -atime +0 -print0 | xargs -0 -l1 -t rm -f
```
输出：


```
[root@pd test4]#  find . -type f -atime +0 -print0 | xargs -0 -l1 -t rm -f
rm -f 
[root@pdtest4]#
```
说明：
-l1是一次处理一个；-t是处理之前打印出命令
## 实例8：使用-i参数默认的前面输出用{}代替，-I参数可以指定其他代替字符，如例子中的[]
命令：
输出：


```
[root@localhost test]# ll
总计 12drwxr-xr-x 6 root root 4096 10-27 01:58 scf
drwxrwxr-x 2 root root 4096 11-13 05:50 test3
drwxrwxr-x 2 root root 4096 11-13 05:50 test4
[root@localhost test]# cd test4
[root@localhost test4]# find . -name "file" | xargs -I [] cp [] ..
[root@localhost test4]# ll
总计 304
-rw-r--r-- 1 root root 302108 11-12 22:54 log2012.log
-rw-r--r-- 1 root root     61 11-12 22:54 log2013.log
-rw-r--r-- 1 root root      0 11-12 22:54 log2014.log
[root@localhost test4]# cd ..
[root@localhost test]# ll
总计 316
-rw-r--r-- 1 root root 302108 11-13 06:03 log2012.log
-rw-r--r-- 1 root root     61 11-13 06:03 log2013.log
-rw-r--r-- 1 root root      0 11-13 06:03 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxr-x 2 root root   4096 11-13 05:50 test3
drwxrwxr-x 2 root root   4096 11-13 05:50 test4
[root@localhost test]#
```
说明：
使用-i参数默认的前面输出用{}代替，-I参数可以指定其他代替字符，如例子中的[]
## 实例9：xargs的-p参数的使用
命令：


```
find . -name "*.log" | xargs -p -i mv {} ..
```
输出：


```
[root@localhost test3]# ll
总计 0
-rw-r--r-- 1 root root 0 11-13 06:06 log2015.log
[root@localhost test3]# cd ..
[root@localhost test]# ll
总计 316
-rw-r--r-- 1 root root 302108 11-13 06:03 log2012.log
-rw-r--r-- 1 root root     61 11-13 06:03 log2013.log
-rw-r--r-- 1 root root      0 11-13 06:03 log2014.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxr-x 2 root root   4096 11-13 06:06 test3
drwxrwxr-x 2 root root   4096 11-13 05:50 test4
[root@localhost test]# cd test3
[root@localhost test3]#  find . -name "*.log" | xargs -p -i mv {} ..
mv ./log2015.log .. ?...y
[root@localhost test3]# ll
总计 0[root@localhost test3]# cd ..
[root@localhost test]# ll
总计 316
-rw-r--r-- 1 root root 302108 11-13 06:03 log2012.log
-rw-r--r-- 1 root root     61 11-13 06:03 log2013.log
-rw-r--r-- 1 root root      0 11-13 06:03 log2014.log
-rw-r--r-- 1 root root      0 11-13 06:06 log2015.log
drwxr-xr-x 6 root root   4096 10-27 01:58 scf
drwxrwxr-x 2 root root   4096 11-13 06:08 test3
drwxrwxr-x 2 root root   4096 11-13 05:50 test4
[root@localhost test]#
```
说明：
-p参数会提示让你确认是否执行后面的命令,y执行，n不执行。
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
