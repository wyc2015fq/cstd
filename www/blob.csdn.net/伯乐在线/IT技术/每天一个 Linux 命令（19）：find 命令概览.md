# 每天一个 Linux 命令（19）：find 命令概览 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/11/13/2767374.html)
Linux下find命令在目录结构中搜索文件，并执行指定的操作。Linux下find命令提供了相当多的查找条件，功能很强大。由于find具有强大的功能，所以它的选项也很多，其中大部分选项都值得我们花时间来了解一下。即使系统中含有网络文件系统( NFS)，find命令在该文件系统中同样有效，只你具有相应的权限。 在运行一个非常消耗资源的find命令时，很多人都倾向于把它放在后台执行，因为遍历一个大的文件系统可能会花费很长的时间(这里是指30G字节以上的文件系统)。
## 1．命令格式：


```
find pathname -options [-print -exec -ok ...]
```
## 2．命令功能：
用于在文件树种查找文件，并作出相应的处理
## 3．命令参数：


```
pathname: find命令所查找的目录路径。例如用.来表示当前目录，用/来表示系统根目录。
-print： find命令将匹配的文件输出到标准输出。
-exec： find命令对匹配的文件执行该参数所给出的shell命令。相应命令的形式为'command' {  } ;，注意{   }和；之间的空格。
-ok： 和-exec的作用相同，只不过以一种更为安全的模式来执行该参数所给出的shell命令，在执行每一个命令之前，都会给出提示，让用户来确定是否执行。
```
## 4．命令选项：


```
-name   按照文件名查找文件。
-perm   按照文件权限来查找文件。
-prune  使用这一选项可以使find命令不在当前指定的目录中查找，如果同时使用-depth选项，那么-prune将被find命令忽略。
-user   按照文件属主来查找文件。
-group  按照文件所属的组来查找文件。
-mtime -n +n  按照文件的更改时间来查找文件， - n表示文件更改时间距现在n天以内，+ n表示文件更改时间距现在n天以前。find命令还有-atime和-ctime 选项，但它们都和-m time选项。
-nogroup  查找无有效所属组的文件，即该文件所属的组在/etc/groups中不存在。
-nouser   查找无有效属主的文件，即该文件的属主在/etc/passwd中不存在。
-newer file1 ! file2  查找更改时间比文件file1新但比文件file2旧的文件。
-type  查找某一类型的文件，诸如：
b - 块设备文件。
d - 目录。
c - 字符设备文件。
p - 管道文件。
l - 符号链接文件。
f - 普通文件。
-size n：[c] 查找文件长度为n块的文件，带有c时表示文件长度以字节计。-depth：在查找文件时，首先查找当前目录中的文件，然后再在其子目录中查找。
-fstype：查找位于某一类型文件系统中的文件，这些文件系统类型通常可以在配置文件/etc/fstab中找到，该配置文件中包含了本系统中有关文件系统的信息。
-mount：在查找文件时不跨越文件系统mount点。
-follow：如果find命令遇到符号链接文件，就跟踪至链接所指向的文件。
-cpio：对匹配的文件使用cpio命令，将这些文件备份到磁带设备中。
```
另外,下面三个的区别:


```
-amin n   查找系统中最后N分钟访问的文件
-atime n  查找系统中最后n*24小时访问的文件
-cmin n   查找系统中最后N分钟被改变文件状态的文件
-ctime n  查找系统中最后n*24小时被改变文件状态的文件
-mmin n   查找系统中最后N分钟被改变文件数据的文件
-mtime n  查找系统中最后n*24小时被改变文件数据的文件
```
## 5．使用实例：
实例1：查找指定时间内修改过的文件
命令：


```
find -atime -2
```
输出：


```
[root@peidachang ~]# find -atime -2
.
./logs/monitor
./.bashrc
./.bash_profile
./.bash_history
```
说明：
超找48小时内修改过的文件
实例2：根据关键字查找
命令：


```
find . -name "*.log"
```
输出：


```
[root@localhost test]# find . -name "*.log" 
./log_link.log
./log2014.log
./test4/log3-2.log
./test4/log3-3.log
./test4/log3-1.log
./log2013.log
./log2012.log
./log.log
./test5/log5-2.log
./test5/log5-3.log
./test5/log.log
./test5/log5-1.log
./test5/test3/log3-2.log
./test5/test3/log3-3.log
./test5/test3/log3-1.log
./test3/log3-2.log
./test3/log3-3.log
./test3/log3-1.log
```
说明：
在当前目录查找 以.log结尾的文件。 “. “代表当前目录
实例3：按照目录或文件的权限来查找文件
命令：


```
find /opt/soft/test/ -perm 777
```
输出：


```
[root@localhost test]# find /opt/soft/test/ -perm 777
/opt/soft/test/log_link.log
/opt/soft/test/test4
/opt/soft/test/test5/test3
/opt/soft/test/test3
```
说明：
查找/opt/soft/test/目录下 权限为 777的文件
实例4：按类型查找
命令：


```
find . -type f -name "*.log"
```
输出：


```
[root@localhost test]# find . -type f -name "*.log"
./log2014.log
./test4/log3-2.log
./test4/log3-3.log
./test4/log3-1.log
./log2013.log
./log2012.log
./log.log
./test5/log5-2.log
./test5/log5-3.log
./test5/log.log
./test5/log5-1.log
./test5/test3/log3-2.log
./test5/test3/log3-3.log
./test5/test3/log3-1.log
./test3/log3-2.log
./test3/log3-3.log
./test3/log3-1.log
[root@localhost test]#
```
说明：
查找当目录，以.log结尾的普通文件
实例5：查找当前所有目录并排序
命令：


```
find . -type d | sort
```
输出：


```
[root@localhost test]# find . -type d | sort
.
./scf
./scf/bin
./scf/doc
./scf/lib
./scf/service
./scf/service/deploy
./scf/service/deploy/info
./scf/service/deploy/product
./test3
./test4
./test5
./test5/test3
[root@localhost test]#
```
实例6：按大小查找文件
命令：


```
find . -size +1000c -print
```
输出：


```
[root@localhost test]#  find . -size +1000c -print
.
./test4
./scf
./scf/lib
./scf/service
./scf/service/deploy
./scf/service/deploy/product
./scf/service/deploy/info
./scf/doc
./scf/bin
./log2012.log
./test5
./test5/test3
./test3
[root@localhost test]#
```
说明：
查找当前目录大于1K的文件
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
