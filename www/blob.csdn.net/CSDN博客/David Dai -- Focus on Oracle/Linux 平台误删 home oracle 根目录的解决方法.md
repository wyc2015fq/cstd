
# Linux 平台误删 home oracle 根目录的解决方法 - David Dai -- Focus on Oracle - CSDN博客


2015年02月13日 14:49:41[Dave](https://me.csdn.net/tianlesoftware)阅读数：16246






# 1  背景说明
正常情况下，我们在安装Oracle数据库的时候，都会创建一个oracle的用户。 其根目录就是/home/oracle.
并且根据Oracle 的OFA的架构，也是建议使用/u01 这样的目录来单独安装oracle的软件，包括存放数据文件。
关于OFA可以参考如下连接：
**Oracle OFA****（****Optimal Flexible Architecture****）****说明**
[http://www.cndba.cn/Dave/article/1111](http://www.cndba.cn/Dave/article/1111)

但实际上，总会存在那么一些不按照官网的建议来创建和管理的数据库，这些年见过太多。Dave 是一直建议严格按照Oracle 的标准来，因为这个是通用的标准，如果每家公司都有自己的标准，那每次换人管理，都会是很麻烦的，也是容易出问题的。
俗话说：铁打的营盘，流水的兵，DBA的岗位总会有波动，规则不一样，就会埋下坑，至于最后是谁踩上，就是人品了。
昨晚一朋友就出现这个问题，单实例的数据库，非归档，无备份。删除了整个/home/oracle 目录，如果是按照OFA来的，这个倒也不是很严重的事情，关键是/home/oracle 下还有几个数据文件，折腾了很久，把数据还原出来了。
马上就过年了，防火，防盗，防宕库，居然还犯下这么低级的错误，这是不想要年终奖的节奏啊，这个就不多说，DBA 就是一个心细的活，需要慢慢历练。
这篇Blog 主要看下误删除/home/oracle 目录的解决方法。这里不涉及数据文件恢复问题，以后在整理。

# 2  linux用户创建说明
在安装oracle 数据库的时候，我们都会创建用户和组。
Oracle 安装参考：
[64位 linux 平台下Oracle 安装文档](http://blog.csdn.net/tianlesoftware/article/details/6062816)
[http://www.cndba.cn/dave/article/211](http://www.cndba.cn/dave/article/211)

groupadd oinstall
groupadd dba
groupadd oper
useradd -g oinstall -G dba oracle
这里是用linux的命令来创建的。 和数据库创建一样，我们一般都是通过OUI来创建实例，其实也是可以通过静默的方式来创建。
所以我们在讲这个故障的处理之前，需要了解linux 手工创建用户和组的方法。

每个用户的创建，都会涉及到如下几个文件：
/etc/passwd （用户）
/etc/group （用户组）
/etc/shadow （密钥文件）
/home/username （家目录）
/etc/skel/.* （骨架文件）

在创建用户的时候，会在/etc/passwd,/etc/group,/etc/shadow三个文件中添加用户的信息，创建一个/home 下的用户根目录，然后把所有的骨架文件复制到用户的根目录下。
[oracle@dg1 ~]$ cat /etc/passwd|grep oracle
oracle:x:502:507::/home/oracle:/bin/bash
[oracle@dg1 ~]$
[oracle@dg1 ~]$ cat /etc/group |grepoinstall
oinstall:x:507:
[oracle@dg1 ~]$ cat /etc/group |grep dba
dba:x:502:oracle
asmdba:x:506:oracle
[oracle@dg1 ~]$
[root@dg1 ~]\# cat /etc/shadow |grep oracle
oracle:$6$7xDE3ZWD$jOnuSzTwfdWoR9JMVr33IIJhDJ/vGnCL5XGpCEMBx3XPiov0NnxRBvDIhflBTWPzXSNZ.HbqEoIt7PAxuF9R70:15595:0:99999:7:::
[root@dg1 ~]\#
当我们删除/home/oracle这个用户的根目录之后，用户的配置信息还存在系统的配置文件中，所以只需要还原骨架文件，在修改一下环境变量的信息，就可以了。

# 3  示例

--数据库正常运行：
[oracle@dg1 ~]$ ps -ef|grep ora
root     1388  1347  0 Aug26 ?        00:00:37 hald-addon-storage: polling/dev/sr0 (every 2 sec)
oracle   9946     1  0 00:49 ?        00:00:05 ora_pmon_dave
oracle   9948     1  0 00:49 ?        00:00:11 ora_psp0_dave
oracle   9950     1  2 00:49 ?        00:16:17 ora_vktm_dave
oracle   9954     1  0 00:49 ?        00:00:01 ora_gen0_dave
oracle   9956     1  0 00:49 ?        00:00:01 ora_diag_dave
oracle   9958     1  0 00:49 ?        00:00:01 ora_dbrm_dave
oracle   9960     1  0 00:49 ?        00:00:14 ora_dia0_dave
oracle   9962     1  0 00:49 ?        00:00:01 ora_mman_dave
oracle   9964     1  0 00:49 ?        00:00:02 ora_dbw0_dave

直接把/home/oracle 目录删掉，我这里直接move。
[root@dg1 ~]\# cd /home
[root@dg1 home]\# ls
oracle
[root@dg1 home]\# mv oracle oracle.bak
[root@dg1 home]\# ls
oracle.bak
[root@dg1 home]\#
[root@dg1 home]\# su - oracle
su: warning: cannot change directory to/home/oracle: No such file or directory
-bash-4.1$
-bash-4.1$ sqlplus / as sysdba
-bash: sqlplus: command not found
-bash-4.1$
-bash-4.1$ whoami
oracle
-bash-4.1$ pwd
/home
-bash-4.1$


开始恢复:
--创建目录：
[root@dg1 home]\# pwd
/home
[root@dg1 home]\# mkdir oracle
[root@dg1 home]\# chown oracle:oinstalloracle
[root@dg1 home]\# ll
total 8
drwxr-xr-x  2 oracle oinstall 4096 Aug 27 14:17 oracle
drwx------. 25 oracle oinstall 4096 Aug 2623:08 oracle.bak
[root@dg1 home]\#
--复制骨架文件：
[root@dg1 ~]\# cp /etc/skel/.* /home/oracle/
cp: omitting directory `/etc/skel/.'
cp: omitting directory `/etc/skel/..'
cp: omitting directory `/etc/skel/.gnome2'
cp: omitting directory `/etc/skel/.mozilla'
[root@dg1 ~]\#
[root@dg1 home]\# ls -la /home/oracle
total 24
drwxr-xr-x 2 root root 4096 Aug 27 14:47 .
drwxr-xr-x. 4 root root 4096 Aug 27 14:47..
-rw-r--r-- 1 root root   18 Aug 27 14:47.bash_logout
-rw-r--r-- 1 root root  176 Aug 27 14:47.bash_profile
-rw-r--r-- 1 root root  124 Aug 27 14:47.bashrc
-rw-r--r-- 1 root root  121 Aug 27 14:47.kshrc
[root@dg1 home]\#
[root@dg1 home]\# chown -R oracle:oinstall/home/oracle
[root@dg1 home]\# cd /home/oracle
[root@dg1 oracle]\# ls -la
total 24
drwxr-xr-x 2 oracle oinstall 4096 Aug 27 14:47 .
drwxr-xr-x. 4 root   root    4096 Aug 27 14:47 ..
-rw-r--r-- 1 oracle oinstall   18 Aug 2714:47 .bash_logout
-rw-r--r-- 1 oracle oinstall  176 Aug 2714:47 .bash_profile
-rw-r--r-- 1 oracle oinstall  124 Aug 2714:47 .bashrc
-rw-r--r-- 1 oracle oinstall  121 Aug 2714:47 .kshrc
[root@dg1 oracle]\#

//修改oracle用户的环境变量：
[oracle@dg1 ~]$ cat .bash_profile
\# .bash_profile
\# Get the aliases and functions
if [ -f ~/.bashrc ]; then
.~/.bashrc
fi
\# User specific environment and startupprograms
PATH=$PATH:$HOME/bin
export PATH

\# Oracle Settings
TMP=/tmp; export TMP
TMPDIR=$TMP; export TMPDIR
ORACLE_HOSTNAME=dave; exportORACLE_HOSTNAME
ORACLE_UNQNAME=dave; export ORACLE_UNQNAME
ORACLE_BASE=/u01/app/oracle; exportORACLE_BASE
ORACLE_HOME=$ORACLE_BASE/product/11.2.0/db_1;exportORACLE_HOME
ORACLE_SID=dave; export ORACLE_SID
PATH=/u01:/usr/sbin:$PATH; export PATH
PATH=$ORACLE_HOME/bin:$PATH; export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib:/lib:/usr/lib;exportLD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/jlib:$ORACLE_HOME/rdbms/jlib;exportCLASSPATH
[oracle@dg1 ~]$

恢复正常。 但如果有数据文件，那么恢复就比这里更复杂。




--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

