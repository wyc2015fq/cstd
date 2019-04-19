# 每天一个 Linux 命令（59）： rcp 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2013/03/14/2958685.html)
rcp代表“remote file copy”（远程文件拷贝）。该命令用于在计算机之间拷贝文件。rcp命令有两种格式。第一种格式用于文件到文件的拷贝；第二种格式用于把文件或目录拷贝到另一个目录中。
1．命令格式：

Shell
```
rcp [参数] [源文件] [目标文件]
```
2．命令功能：
rcp命令用在远端复制文件或目录，如同时指定两个以上的文件或目录，且最后的目的地是一个已经存在的目录，则它会把前面指定的所有文件或目录复制到该目录中。
3．命令参数：
各选项含义：

Shell
```
-r 递归地把源目录中的所有内容拷贝到目的目录中。要使用这个选项，目的必须是一个目录。
-p 试图保留源文件的修改时间和模式，忽略umask。
-k 请求rcp获得在指定区域内的远程主机的Kerberos 许可，而不是获得由krb_relmofhost⑶确定的远程主机区域内的远程主机的Kerberos许可。
-x 为传送的所有数据打开DES加密。这会影响响应时间和CPU利用率，但是可以提高安全性。如果在文件名中指定的路径不是完整的路径名，那么这个路径被解释为相对远程机上同名用户的主目录。如果没有给出远程用户名，就使用当前用户名。如果远程机上的路径包含特殊shell字符，需要用反斜线（\\）、双引号（”）或单引号（’）括起来，使所有的shell元字符都能被远程地解释。需要说明的是，rcp不提示输入口令，它通过rsh命令来执行拷贝。
directory 每个文件或目录参数既可以是远程文件名也可以是本地文件名。远程文件名具有如下形式：rname@rhost：path，其中rname是远程用户名，rhost是远程计算机名，path是这个文件的路径。
```
4．使用实例：
要使用 rcp，需要具备以下条件：
如果系统中有 /etc/hosts 文件，系统管理员应确保该文件包含要与之进行通信的远程主机的项。
/etc/hosts 文件中有一行文字，其中包含每个远程系统的以下信息：


```
internet_address   official_name   alias
```
例如：


```
9.186.10.***  webserver1.com.58.webserver
```
.rhosts 文件
.rhosts 文件位于远程系统的主目录下，其中包含本地系统的名称和本地登录名。
例如，远程系统的 .rhosts 文件中的项可能是：


```
webserver1 root
```
其中，webserver1 是本地系统的名称，root 是本地登录名。这样，webserver1 上的 root 即可在包含 .rhosts 文件的远程系统中来回复制文件。
配置过程:
只对root用户生效
1. 在双方root用户根目录下建立.rhosts文件,并将双方的hostname加进去.在此之前应在双方的 /etc/hosts文件中加入对方的IP和hostname
2. 把rsh服务启动起来,redhat默认是不启动的。
方法：用执行ntsysv命令,在rsh选项前用空格键选中,确定退出。然后执行：
service xinetd restart即可。
3. 到/etc/pam.d/目录下,把rsh文件中的auth required /lib/security/pam_securetty.so
一行用“#”注释掉即可。（只有注释掉这一行，才能用root用户登录）
命令使用:
将文件复制到远程系统
要将本地系统中的文件复制到远程系统，请使用以下命令：

Shell
```
rcplocal_fileremote_hostname:remote_fileEnter
```
注意，如果当前目录下没有 local_file，则除本地文件名外，还需要提供相对路径（自当前目录开始）或绝对路径名（自 / 开始）。
仅当希望将 remote_hostname 上的 remote_file 放到其他目录（远程主目录除外）下时，才需要为其指定完整的（绝对）路径。
使用实例1:将当前目录下的 test1 复制到名为 webserver1的远程系统
命令：

Shell
```
rcp test1 webserver1:/home/root/test3
```
说明：
在这种情况下，test1 被复制到远程子目录 test3下，名称仍为 test1 。如果仅提供了远程主机名，rcp 将把 test1 复制到远程主目录下，名称仍为 test1 。
还可以在目的目录中包含文件名。例如，将文件复制到名为 webserver1的系统中：

Shell
```
rcp test1 webserver1:/home/root/test3
```
在这种情况下，将 test1 复制到远程目录root 下并将其命名为 test3。
使用实例2：从远程系统复制文件：要将远程系统中的文件复制到本地目录下
命令：

Shell
```
rcp remote_hostname:remote_file local_fileEnter
```
使用实例:3:将远程系统 webserver1中的 test2 复制到当前目录：
命令：

Shell
```
rcp webserver1:/home/root/test2 .Enter
```
说明：
点 (.) 是“当前目录”的简写形式。在这种情况下，远程目录中的 test2 被复制到当前目录下，名称仍为 test2 。
如果希望用新名称复制文件，请提供目标文件名。
如果希望将 test2 复制到本地系统中的其他目录下，请使用以下绝对或相对路径名：

Shell
```
rcp webserver1:/home/root/test2 otherdir/ Enter
```
或者，如果希望用其他文件名将文件复制到其他目录下：

Shell
```
rcp webserver1:/home/root/test2 otherdir/otherfile Enter
```
使用实例4：将目录复制到远程系统：要将本地目录及其文件和子目录复制到远程系统，请同时使用 rcp 和 -r（递归）选项。
命令：

Shell
```
rcp –r local_dir remote_hostname:remote_dir Enter
```
说明：
如果当前目录下没有 local_dir，则除本地目录名外，还需要提供相对路径名（自当前目录开始）或绝对路径名（自 / 顶级目录开始）。另外，如果主目录下没有 remote_dir，则 remote_dir 将需要一个相对路径（自主目录开始）或绝对路径（自 / 开始）。
使用实例5:
要将名为 work 的子目录完整地复制到 webserver1远程计算机中的主目录下名为 products 的目录，请键入以下内容：

Shell
```
rcp –r work webserver1:/home/root/products Enter
```
此命令在 webserver1:/home/root/products 下创建名为 work 的目录及其全部内容（假定 /home/root/products 已存在于 webserver1中）。
本示例假定用户处于包含 work 的本地目录下。否则，必须提供该目录的相对或绝对路径，如 /home/root/work。
使用实例6：从远程系统复制目录：
要将远程目录及其所有文件和子目录复制到本地目录，请在以下语法中使用 rcp 和 -r（递归）选项。
命令：

Shell
```
rcp –r remote_hostname:remote_dir local_dir Enter
```
要将名为 work 的远程目录复制到当前目录，请键入以下内容：

Shell
```
rcp –r webserver1:/home/root/work .Enter
```
点 (.) 表示当前目录。将在此目录下创建 work 目录。
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
[每天一个 Linux 命令（40）： wc 命令](http://blog.jobbole.com/109925/)
[每天一个 Linux 命令（41）： ps 命令](http://blog.jobbole.com/109948/)
[每天一个 Linux 命令（44）： top 命令](http://blog.jobbole.com/109951/)
[每天一个 Linux 命令（45）： free 命令](http://blog.jobbole.com/109965/)
[每天一个 Linux 命令（46）： vmstat 命令](http://blog.jobbole.com/109970/)
[每天一个 Linux 命令（47）： iostat 命令](http://blog.jobbole.com/109980/)
[每天一个 Linux 命令（48）： watch 命令](http://blog.jobbole.com/109994/)
[每天一个 Linux 命令（49）： at 命令](http://blog.jobbole.com/110001/)
[每天一个 Linux 命令（50）： crontab 命令](http://blog.jobbole.com/110005/)
[每天一个 Linux 命令（51）： lsof 命令](http://blog.jobbole.com/110011/)
[每天一个 Linux 命令（52）： ifconfig 命令](http://blog.jobbole.com/110016/)
[每天一个 Linux 命令（53）： route 命令](http://blog.jobbole.com/110019/)
[每天一个 Linux 命令（54）： ping 命令](http://blog.jobbole.com/110025/)
[每天一个 Linux 命令（55）： traceroute 命令](http://blog.jobbole.com/110032/)
[每天一个 Linux 命令（56）： netstat 命令](http://blog.jobbole.com/110037/)
[每天一个 Linux 命令（57）： ss 命令](http://blog.jobbole.com/110046/)
[每天一个 Linux 命令（58）： telnet 命令](http://blog.jobbole.com/110058/)
