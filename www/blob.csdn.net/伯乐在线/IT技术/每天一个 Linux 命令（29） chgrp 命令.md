# 每天一个 Linux 命令（29）: chgrp 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/12/03/2799003.html)
在lunix系统里，文件或目录的权限的掌控以拥有者及所诉群组来管理。可以使用chgrp指令取变更文件与目录所属群组，这种方式采用群组名称或群组识别码都可以。Chgrp命令就是change group的缩写！要被改变的组名必须要在/etc/group文件内存在才行。
1．命令格式：

Shell
```
chgrp [选项] [组] [文件]
```
2．命令功能：
chgrp命令可采用群组名称或群组识别码的方式改变文件或目录的所属群组。使用权限是超级用户。
3．命令参数：
必要参数:

Shell
```
-c 当发生改变时输出调试信息
-f 不显示错误信息
-R 处理指定目录以及其子目录下的所有文件
-v 运行时显示详细的处理信息
--dereference 作用于符号链接的指向，而不是符号链接本身
--no-dereference 作用于符号链接本身
```
选择参数:

Shell
```
--reference=<文件或者目录>
--help 显示帮助信息
--version 显示版本信息
```
4．使用实例：
实例1：改变文件的群组属性
命令：

Shell
```
chgrp -v bin log2012.log
```
输出：

Shell
```
[root@localhost test]# ll
---xrw-r-- 1 root root 302108 11-13 06:03 log2012.log
[root@localhost test]# chgrp -v bin log2012.log
“log2012.log” 的所属组已更改为 bin
[root@localhost test]# ll
---xrw-r-- 1 root bin  302108 11-13 06:03 log2012.log
```
说明：
将log2012.log文件由root群组改为bin群组
实例2：根据指定文件改变文件的群组属性
命令：

Shell
```
chgrp --reference=log2012.log log2013.log
```
输出：

Shell
```
[root@localhost test]# ll
---xrw-r-- 1 root bin  302108 11-13 06:03 log2012.log
-rw-r--r-- 1 root root     61 11-13 06:03 log2013.log
[root@localhost test]#  chgrp --reference=log2012.log log2013.log
[root@localhost test]# ll
---xrw-r-- 1 root bin  302108 11-13 06:03 log2012.log
-rw-r--r-- 1 root bin      61 11-13 06:03 log2013.log
```
说明：
改变文件log2013.log 的群组属性，使得文件log2013.log的群组属性和参考文件log2012.log的群组属性相同
实例3：改变指定目录以及其子目录下的所有文件的群组属性
命令：
输出：

Shell
```
[root@localhost test]# ll
drwxr-xr-x 2 root root   4096 11-30 08:39 test6
[root@localhost test]# cd test6
[root@localhost test6]# ll
---xr--r-- 1 root root 302108 11-30 08:39 linklog.log
---xr--r-- 1 root root 302108 11-30 08:39 log2012.log
-rw-r--r-- 1 root root     61 11-30 08:39 log2013.log
-rw-r--r-- 1 root root      0 11-30 08:39 log2014.log
-rw-r--r-- 1 root root      0 11-30 08:39 log2015.log
-rw-r--r-- 1 root root      0 11-30 08:39 log2016.log
-rw-r--r-- 1 root root      0 11-30 08:39 log2017.log
[root@localhost test6]# cd ..
[root@localhost test]# chgrp -R bin test6
[root@localhost test]# cd test6
[root@localhost test6]# ll
---xr--r-- 1 root bin 302108 11-30 08:39 linklog.log
---xr--r-- 1 root bin 302108 11-30 08:39 log2012.log
-rw-r--r-- 1 root bin     61 11-30 08:39 log2013.log
-rw-r--r-- 1 root bin      0 11-30 08:39 log2014.log
-rw-r--r-- 1 root bin      0 11-30 08:39 log2015.log
-rw-r--r-- 1 root bin      0 11-30 08:39 log2016.log
-rw-r--r-- 1 root bin      0 11-30 08:39 log2017.log
[root@localhost test6]# cd ..
[root@localhost test]# ll
drwxr-xr-x 2 root bin    4096 11-30 08:39 test6
[root@localhost test]#
```
说明：
改变指定目录以及其子目录下的所有文件的群组属性
实例4：通过群组识别码改变文件群组属性
命令：

Shell
```
chgrp -R 100 test6
```
输出：

Shell
```
[root@localhost test]# chgrp -R 100 test6
[root@localhost test]# ll
drwxr-xr-x 2 root users   4096 11-30 08:39 test6
[root@localhost test]# cd test6
[root@localhost test6]# ll
---xr--r-- 1 root users 302108 11-30 08:39 linklog.log
---xr--r-- 1 root users 302108 11-30 08:39 log2012.log
-rw-r--r-- 1 root users     61 11-30 08:39 log2013.log
-rw-r--r-- 1 root users      0 11-30 08:39 log2014.log
-rw-r--r-- 1 root users      0 11-30 08:39 log2015.log
-rw-r--r-- 1 root users      0 11-30 08:39 log2016.log
-rw-r--r-- 1 root users      0 11-30 08:39 log2017.log
[root@localhost test6]#
```
说明：
通过群组识别码改变文件群组属性，100为users群组的识别码，具体群组和群组识别码可以去/etc/group文件中查看
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
