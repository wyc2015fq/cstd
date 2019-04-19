# 每天一个 Linux 命令（31）: /etc/group 文件详解 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/12/05/2802419.html)
  Linux /etc/group文件与/etc/passwd和/etc/shadow文件都是有关于系统管理员对用户和用户组管理时相关的文件。linux /etc/group文件是有关于系统管理员对用户和用户组管理的文件,linux用户组的所有信息都存放在/etc/group文件中。具有某种共同特征的用户集合起来就是用户组（Group）。用户组（Group）配置文件主要有 /etc/group和/etc/gshadow，其中/etc/gshadow是/etc/group的加密信息文件。
    将用户分组是Linux系统中对用户进行管理及控制访问权限的一种手段。每个用户都属于某个用户组；一个组中可以有多个用户，一个用户也可以属于不 同的组。当一个用户同时是多个组中的成员时，在/etc/passwd文件中记录的是用户所属的主组，也就是登录时所属的默认组，而其他组称为附加组。
    用户组的所有信息都存放在/etc/group文件中。此文件的格式是由冒号(:)隔开若干个字段，这些字段具体如下：
    组名:口令:组标识号:组内用户列表
    具体解释：
**组名：**
    组名是用户组的名称，由字母或数字构成。与/etc/passwd中的登录名一样，组名不应重复。
**   口令：**
    口令字段存放的是用户组加密后的口令字。一般Linux系统的用户组都没有口令，即这个字段一般为空，或者是*。
** 组标识号：**
    组标识号与用户标识号类似，也是一个整数，被系统内部用来标识组。别称GID.
**组内用户列表：**
    是属于这个组的所有用户的列表，不同用户之间用逗号(,)分隔。这个用户组可能是用户的主组，也可能是附加组。
** 使用实例：**
**    输出：**
```
Shell
			
			
```
[root@localhost test6]# cat /etc/group
root:x:0:root,linuxsir
bin:x:1:root,bin,daemon
daemon:x:2:root,bin,daemon
sys:x:3:root,bin
```
```
**    说明：**
    我们以root:x:0:root,linuxsir 为例： 用户组root，x是密码段，表示没有设置密码，GID是0,root用户组下包括root、linuxsir以及GID为0的其它用户。
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
