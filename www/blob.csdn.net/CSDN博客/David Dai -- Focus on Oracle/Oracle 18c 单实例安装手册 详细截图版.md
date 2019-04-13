
# Oracle 18c 单实例安装手册 详细截图版 - David Dai -- Focus on Oracle - CSDN博客


2018年08月27日 23:25:30[Dave](https://me.csdn.net/tianlesoftware)阅读数：4286



##### 1. 安装前准备工作
##### 1.1.   系统要求
Linux下Oracle数据库安装的最低服务器配置要求：
|系统要求|说明|
|---|---|
|内存|DB：至少少1G，建议大小8G。
|Grid：至少8G。|
|临时空间|/tmp目录中至少有1 GB的空间|
|Swap空间|1 GB~2 GB：内存的1.5倍
|2 GB~16 GB：与内存相同
|超过16 GB：16 GB
|注意：如果为Linux服务器启用HugePages，则应在计算交换空间之前从可用内存中扣除分配给HugePages的内存。|
|Oracle软件的最小本地磁盘存储空间|对于Linux x86-64：
|对于独立服务器安装，Oracle Grid Infrastructure至少为6.8 GB。
|Oracle数据库企业版至少7.5 GB。
|Oracle Database Standard Edition 2至少为7.5 GB。|
|1.2.   检查和配置Oracle数据库的服务器硬件
|1）检查物理内存大小：
|[root@www|.cndba|.cn|~]|\# grep MemTotal /proc/meminfo|MemTotal:|3882124|kB|2）检查交换空间的大小：
|[root@www|.cndba|.cn|~]|\# grep SwapTotal /proc/meminfo|SwapTotal:|4194300|kB|3）检查/tmp目录大小：
|[root|@www|.cndba.cn ~]|\# df -h /tmp|Filesystem                   Size  Used Avail Use% Mounted|on|/dev/mapper/rhel_host1-root   46G   22G   25G  47% /|如果/ tmp目录中的可用空间小于所需的可用空间，请完成以下步骤之一：
|1. 1) 从/ tmp目录中删除不需要的文件以满足磁盘空间要求。
|2. 2) 设置Oracle用户的环境时，还要将TMP和TMPDIR环境变量设置为要使用的目录而不是/tmp。
|4）检查系统上的可用内存和磁盘交换空大小：
|[root@www|.cndba|.cn|~]|\# free|total        used        free      shared  buff/cache   available|Mem:|3882124|1249092|307064|1217640|2325968|1141792|Swap:|4194300|33592|4160708|5）确定系统架构：
|[root@www|.cndba|.cn|~]|\# uname -m|x86_64|验证处理器架构是否与要安装的Oracle软件版本匹配。如果没有看到预期的输出，则无法在此系统上安装该软件。
|6）验证是否正确安装了足够大小的共享内存（/dev/shm）：
|[root|@www|.cndba.cn ~]|\# df -h /dev/shm|Filesystem|Size|Used|Avail|Use|%|Mounted|on
tmpfs|1.9|G|1.2|G|712|M|63|%|/dev/shm|df-h命令显示挂载/dev/shm的文件系统，并以GB显示共享内存的总大小和可用大小。
|1.3.   配置安装环境
|1.3.1. 通过Oracle的RPM包来配置操作系统
|如果你的操作系统是Oracle Linux或者RedHat Linux，那么你可以通过Oracle RPM包来完成绝大部分的操作系统的配置Oracle Linux 6和Oracle Linux 7有该RPM包。挂载Oracle Linux 6和Oracle Linux 7镜像执行：
|\# yum install oracle-database-preinstall-18c|或者下载rpm包手动安装例如Oracle Linux 7包是oracle-database-preinstall-18c-1.0-4.el7.x86_64.rpm。
|Oracle Linux自动创建标准（非角色分配）Oracle安装所有者和组，并根据Oracle安装的要求设置其他内核配置设置。
|检查RPM日志文件以查看系统配置更改。例如，在Oracle Linux 7上：
|/|var|/|log|/oracle|-database|-preinstall|-|18|c/backup/timestamp/orakernel|.|log|1.3.2. 手动配置
|1.3.2.1.   配置本地yum源
|[root@www|.cndba|.cn|yum|.repos|.d|]|\# cat cndba.repo|[www|.cndba|.cn|]
name=CNDBA|.CN|YUM Server
baseurl=file:///mnt
gpgcheck=|0|enabled=|1|1.3.2.2.   挂载镜像
|[root@www|.cndba|.cn|yum|.repos|.d|]|\# mount -t auto -o ro /dev/cdrom /mnt/|[root@www|.cndba|.cn|yum|.repos|.d|]|\#  yum clean all|Loaded plugins: langpacks, product-id, search-disabled-repos, subscription-manager
This system is not registered to Red Hat Subscription Management. You can use subscription-manager to register.
Cleaning repos: www|.cndba|.cn|Cleaning up everything|1.3.2.3.   配置/etc/hosts
|[root@www|.cndba|.cn|~]|\# vim /etc/hosts|192.168|.1|.51|cndba|1.3.2.4.   安装操作系统必备的软件包
|yum install binutils gcc gcc-c++  compat-libstdc++-|33|glibc  glibc|.i|686  glibc-devel  ksh libgcc|.i|686   libstdc++-devel  libaio  libaio|.i|686  libaio-devel  libaio-devel|.i|686  libXtst  libXtst|.i|686  libX11  libX11|.i|686 libXau  libXau|.i|686  libxcb  libxcb|.i|686  libXi  libXi|.i|686  make  sysstat  compat-libcap1 –|y|RedHat7.*的镜像文件中没有compat-libstdc包，需要手动安装
|[root|@www|.cndba.cn software]|\# rpm -ivh compat-libstdc++-33-3.2.3-72.el7.x86_64.rpm|warning|: compat-libstdc++-|33|-|3.2|.3|-|72.|el7.x86_64.|rpm|: Header V3 RSA/SHA256 Signature, key ID|f4a80eb5|: NOKEY
Preparing...|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\# [100%]
Updating / installing...
   1:compat-libstdc++-33-3.2.3-72.el7 \#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|\#\#\#\#\#\#|[|100|%]|1.3.2.5.   配置Linux的系统参数
|1)  配置Linux的内核参数
|[root@www|.cndba|.cn|~]|\# vim /etc/sysctl.conf|添加以下内容
fs|.aio|-max-nr =|1048576|fs|.file|-max =|6815744|kernel|.shmall|=|2097152|kernel|.shmmax|=|4294967295|kernel|.shmmni|=|4096|kernel|.sem|=|250|32000|100|128|net|.ipv|4|.ip|_local_port_range =|9000|65500|net|.core|.rmem|_default =|262144|net|.core|.rmem|_max =|4194304|net|.core|.wmem|_default =|262144|net|.core|.wmem|_max =|1048576|shmall大于或等于shmmax的值，以页为单位。shmmax物理内存大小的一半（以字节为单位）
|用root用户执行sysctl -p 使修改生效。
|[root@cndba ~]|\# sysctl -p|fs|.aio|-max-nr =|1048576|fs|.file|-max =|6815744|kernel|.shmall|=|2097152|kernel|.shmmax|=|4294967295|kernel|.shmmni|=|4096|kernel|.sem|=|250|32000|100|128|net|.ipv|4|.ip|_local_port_range =|9000|65500|net|.core|.rmem|_default =|262144|net|.core|.rmem|_max =|4194304|net|.core|.wmem|_default =|262144|net|.core|.wmem|_max =|1048576|2)  设置 Oracle 用户的 Shell 限制
|[root|@www|.cndba.cn ~]\# vim /etc/security/limits.conf
添加以下内容
oracle|soft|nproc|2047|oracle  hard  nproc|16384|oracle|soft|nofile|1024|oracle  hard  nofile|65536|oracle|soft|stack|10240|oracle  hard  stack|32768|oracle|soft|memlock|3145728|oracle hard memlock|3145728|注意：
|limits.conf 文件发生更改时，这些更改立即生效。但是，如果 Oracle 用户已登录，
|则这些更改将在注销这些用户并重新登录之后生效。必须先完成此操作，然后再使用这些帐户进行安装。
|3)  设置PAM
|确保加载了最新版本的 PAM，然后在 /etc/pam.d/login 文件中添加或编辑以下行：
|[root@www|.cndba|.cn|~]|\# vim /etc/pam.d/login|添加以下内容
session required pam_limits|.so|4)  关闭防火墙/SELINUX
|--关闭防火墙
[root@www|.cndba|.cn|~]|\# systemctl stop firewalld|[root@www|.cndba|.cn|~]|\# systemctl disable firewalld|Removed symlink /etc/systemd/system/dbus-org|.fedoraproject|.FirewallD|1|.service|.
Removed symlink /etc/systemd/system/basic|.target|.wants|/firewalld|.service|.
--关闭SELINUX
[root@www|.cndba|.cn|~]|\# vi /etc/selinux/config|[root@www|.cndba|.cn|~]|\# cat /etc/selinux/config|\# This file controls the state of SELinux on the system.|\# SELINUX= can take one of these three values:|\#     enforcing - SELinux security policy is enforced.|\#     permissive - SELinux prints warnings instead of enforcing.|\#     disabled - No SELinux policy is loaded.|SELINUX=disabled|\# SELINUXTYPE= can take one of three two values:|\#     targeted - Targeted processes are protected,|\#     minimum - Modification of targeted policy. Only selected processes are protected.|\#     mls - Multi Level Security protection.|SELINUXTYPE=targeted|1.4.   创建用户和组
|[root@www|.cndba|.cn|~]|\# /usr/sbin/groupadd -g 54321 oinstall|[root@www|.cndba|.cn|~]|\# /usr/sbin/groupadd -g 54322 dba|[root@www|.cndba|.cn|~]|\# /usr/sbin/groupadd -g 54323 oper|[root@www|.cndba|.cn|~]|\# /usr/sbin/useradd -u 54321 -g oinstall -G dba,oper oracle|[root@www|.cndba|.cn|software]|\# passwd oracle|然后，可以使用ID命令验证要使用的Oracle安装所有者是否将Oracle Inventory组作为其主要组。例如：
|[oracle|@cndba|~]|$|id oracle
uid=|54321|(oracle) gid=|54321|(oinstall) groups=|54321|(oinstall),|54322|(dba),|54323|(oper)|1.5.   创建目录
|创建Oracle Home
|[root@www|.cndba|.cn|~]|\# mkdir -p /u01/app/oracle/product/18.3.0/dbhome_1|[root@www|.cndba|.cn|~]|\# chown -R oracle:oinstall /u01|[root@www|.cndba|.cn|~]|\# chmod -R 775 /u01/|1.6.   设置环境变量
|切换到oracle用户下执行vim .bash_profile编辑.bash_profile文件添加以下内容：
|[oracle@cndba ~]$ vim .bash_profile
ORACLE_SID=cndba;|export|ORACLE_SID  
ORACLE_UNQNAME=cndba;|export|ORACLE_UNQNAME
JAVA_HOME=/usr/local/java;|export|JAVA_HOME
ORACLE_BASE=/u01/app/oracle;|export|ORACLE_BASE
ORACLE_HOME=|$ORACLE_BASE|/product/|18.3|.|0|/dbhome_1;|export|ORACLE_HOME
ORACLE_TERM=xterm;|export|ORACLE_TERM
NLS_DATE_FORMAT=|"YYYY:MM:DDHH24:MI:SS"|;|export|NLS_DATE_FORMAT|\#NLS_LANG=american_america.ZHS16GBK; export NLS_LANG|export|NLS_LANG=AMERICAN_AMERICA.UTF8
TNS_ADMIN=|$ORACLE_HOME|/network/admin;|export|TNS_ADMIN
ORA_NLS11=|$ORACLE_HOME|/nls/data;|export|ORA_NLS11
PATH=.:|${JAVA_HOME}|/bin:|${PATH}|:|$HOME|/bin:|$ORACLE_HOME|/bin:|$ORA_CRS_HOME|/bin
PATH=|${PATH}|:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin|export|PATH
LD_LIBRARY_PATH=|$ORACLE_HOME|/lib
LD_LIBRARY_PATH=|${LD_LIBRARY_PATH}|:|$ORACLE_HOME|/oracm/lib
LD_LIBRARY_PATH=|${LD_LIBRARY_PATH}|:/lib:/usr/lib:/usr/local/lib|export|LD_LIBRARY_PATH
CLASSPATH=|$ORACLE_HOME|/JRE
CLASSPATH=|${CLASSPATH}|:|$ORACLE_HOME|/jlib
CLASSPATH=|${CLASSPATH}|:|$ORACLE_HOME|/rdbms/jlib
CLASSPATH=|${CLASSPATH}|:|$ORACLE_HOME|/network/jlib|export|CLASSPATH
THREADS_FLAG=native;|export|THREADS_FLAG|export|TEMP=/tmp|export|TMPDIR=/tmp
umask|022|使设置的环境变量生效
|[oracle|@cndba|~]|$|source .bash_profile|2. 安装数据库软件
|注意：将安装文件解压到Oracle Home下，这里和Oracle 18c之前的版本不同，之前的版本解压的安装文件可以在任何位置，Oracle 解压的安装文件必须在Oracle Home下。因为仅从Oracle home运行runInstaller命令。不要从任何其他位置运行runInstaller。
|[root@www|.cndba|.cn|software]|\# chown oracle:oinstall LINUX.X64_180000_db_home.zip|[root@www|.cndba|.cn|software]|\# su - oracle|[oracle@cndba software]$ ll
total|4457668|-rw-r--r--.|1|oracle oinstall|4564649047|Jul|30|10|:|14|LINUX|.X|64_180000_db_home|.zip|[oracle@cndba software]$ unzip -d /u01/app/oracle/product/|18.1|.0|/dbhome_1/ LINUX|.X|64_180000_db_home|.zip|切换到Oracle用户进去Oracle Home目录执行./runInstaller安装Oracle 18c数据库软件
|[oracle|@cndba|dbhome_1]|$|cd /u01/app/oracle/product/|18.1|.|0|/dbhome_1/
[oracle|@cndba|dbhome_1]|$|./runInstaller|2.1.   选择仅安装类型
|这里选择安装数据库软件，软件安装成功之后在通过执行DBCA命令创建数据库，如果选择第一个则数据库会一起被创建。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382082758.png)
|2.2.   选择安装数据库类型
|这里选择安装单实例
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382131395.png)
|2.3.   选择数据库版本
|选择企业版
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382145970.png)
|2.4.   Oracle 软件安装位置
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382157065.png)
|2.5.   创建Inventory
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382193075.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382193075.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382203667.png)
|2.6.   执行先决条件检查
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382221232.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382221232.png)
|根据提示信息可以发现Oracle 建议使用8G内存，当前只有4G，忽略继续下一步。
|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382221232.png)|2.7.   执行安装
|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382221232.png)![](https://www.cndba.cn:8080/cndba/20180827/png/1535382234965.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382234965.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382241741.png)
|2.8.   在操作系统root用户下执行以下两个脚本
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382254025.png)
|[root@www.cndba.cn ~]|\# /u01/app/oraInventory/orainstRoot.sh|Changing permissions|of|/u01/app/oraInventory.
Adding|read|,|write|permissions|for|group.
Removing|read|,|write|,execute permissions|for|world.
Changing groupname|of|/u01/app/oraInventory|to|oinstall.
The execution|of|the|script|is|complete.
[root@www.cndba.cn ~]|\# /u01/app/oracle/product/18.1.0/dbhome_1/root.sh|Performing root user operation.
The following environment variables are|set|as|:
    ORACLE_OWNER= oracle
    ORACLE_HOME=  /u01/app/oracle/product/|18.1|.0|/dbhome_1
Enter|the|full pathname|of|the|local|bin directory: [/usr/|local|/bin]: 
   Copying dbhome|to|/usr/|local|/bin ...
   Copying oraenv|to|/usr/|local|/bin ...
   Copying coraenv|to|/usr/|local|/bin ...

Creating /etc/oratab|file|...
Entries will be added|to|the|/etc/oratab|file|as|needed|by|Database Configuration Assistant when a database|is|created
Finished|running|generic part|of|root|script|.
Now product-specific root actions will be performed.
Do you want|to|setup Oracle Trace File Analyzer (TFA) now ? yes|[no] : 
yes
Installing Oracle Trace File Analyzer (TFA).
Log File: /u01/app/oracle/product/|18.1|.0|/dbhome_1/install/root_cndba_2018-|07|-|30|_12-|58|-|38|-|721171897.|log|Finished installing Oracle Trace File Analyzer (TFA)|安装成功退出即可。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382289489.png)
|3. DBCA创建数据库
|可以通过手动执行命令方式来创建数据库，也可通过DBCA来创建。推荐是使用DBCA方式，方便而且不容易出错。
|3.1.   执行DBCA命令创建数据库
|在Oracle用户下执行DBCA命令创建数据库
|[oracle|@cndba|dbhome_1]|$|dbca![](https://www.cndba.cn:8080/cndba/20180827/png/1535382658814.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382658814.png)
|第一次安装的时候只能选择上面两项，如果已经创建数据库，则Delete database选择可以选择执行删除数据库。
|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382658814.png)|3.2.   选择数据库创建方式
|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382658814.png)|选择高级配置，很多选项可以自定义。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382676199.png)
|3.3.   选择数据库开发类型
|选择一般用途
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382688841.png)
|3.4.   指定数据库标识
|这里可以选择创建CDB数据库，可以选择创建空CDB或者可以选择创建CDB和一个或多个PDB并输入PDB名称。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382696547.png)
|3.5.   指定数据文件存放位置
|因为这里单实例所以放在本地文件系统。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382704933.png)
|3.6.   选择闪回区选项
|指定闪回区位置及大小及是否开启归档
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382711973.png)
|3.7.   选择网络配置
|这里如果选择创建新的监听，则数据库会自动创建个新监听。不用创建完数据库在用netca手动创建监听。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382719400.png)
|3.8.   选择Oracle Data Vault 配置选项
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382727163.png)
|3.9.   配置内存进程字符集等选项
|配置内存大小
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382735506.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382735506.png)
|在典型安装期间，使用Oracle Database Configuration Assistant（DBCA）创建数据库，并启用自动内存管理。如果选择“高级安装”，则可以手动指定内存分配，也可以启用自动内存管理。
|如果数据库实例的总物理内存大于4 GB，则在数据库安装和创建期间无法选择Oracle自动内存管理选项。而是使用自动共享内存管理。自动共享内存管理可根据需要自动在各种组件之间分配可用内存，从而允许系统最大限度地使用所有可用的SGA内存。
|通过自动内存管理，Oracle数据库实例可以自动为你管理和调整内存。通过自动内存管理，可以选择memory target,，实例会自动在系统全局区域（SGA）和实例程序全局区域（实例PGA）之间分配内存。随着内存需求的变化，实例会动态地在SGA和实例PGA之间重新分配内存。
|可以在数据库安装期间或之后启用自动内存管理。安装后启用自动内存管理涉及关闭并重新启动数据库。
|注意：
|默认情况下，在具有4 GB以上内存的节点上执行典型安装时，将禁用自动内存管理。
|配置进程数
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382746753.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382746753.png)
|配置字符集
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382758350.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535382758350.png)
|当根的字符集是AL32UTF8时，插入(plugged into)CDB或克隆的PDB可以具有与根不同的字符集。从CDB seed创建的PDB从其继承AL32UTF8，但可以将PDB迁移到其他字符集。
|当根的字符集不是AL32UTF8时，CDB中的所有PDB都使用root的字符集。
|Oracle建议CDB数据库字符集使用AL32UTF8，CDB国家字符集使用AL16UTF16，因为它们提供了最大的灵活性。
|为CDB选择数据库字符集时，必须考虑要合并（插入）到此CDB的数据库的当前字符集。
|配置连接模式
|此处选择专用服务器模式
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382767733.png)
|3.10.  是否开启EM
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382778049.png)
|3.11.  配置管理用户密码
|这里为管理用户设置统一密码，因为密码简单所以提示以下信息。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382792145.png)
|3.12.  选择创建数据库选项
|选择创建数据库
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382808789.png)
|3.13.  创建数据库
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382820061.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382830823.png)
|3.14.  数据库创建成功
![](https://www.cndba.cn:8080/cndba/20180827/png/1535382844422.png)
|4. 配置监听
|4.1.   创建监听服务
|在DBCA创建数据库的时候如果勾选创建新的监听则会自动创建监听，不需要在创建数据库之后在使用netca手动创建监听。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383137923.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383137923.png)
|在命令行执行DBCA手动创建监听
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383147839.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383147839.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383157495.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383157495.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383166462.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383166462.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383175291.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383175291.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383187052.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383187052.png)
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383198011.png)
|4.2.   启用监听服务
|在Linux 操作系统登录Oracle执行命令netmgr，选择要添加的监服务的监听，这里是默认创建的监听。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383207762.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383207762.png)
|在右上角现在数据库服务。单机左下角的添加地址，界面入如下。修改对应的全局数据库名，Oracle Home,SID
|对应修改为数据库对应信息。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383217950.png)|[](https://www.cndba.cn:8080/cndba/20180827/png/1535383217950.png)
|单机文件-保存网络配置，即可完成网络配置。
![](https://www.cndba.cn:8080/cndba/20180827/png/1535383227802.png)
|4.3.   启动和停止监听
|–停止监听
|在Linux操作系统上执行命令lsnrctl stop
|[oracle@cndba ~]$ lsnrctl stop
LSNRCTL|for|Linux: Version|18.0|.0|.0|.0|- Production|on|31|-JUL-|2018|00|:|59|:|01|Copyright (c)|1991|,|2018|, Oracle.|All|rights reserved.
Connecting|to|(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=cndba)(|PORT|=|1521|)))
The command completed successfully|–启动监听
|在Linux操作系统上执行命令lsnrctl start
|[oracle@cndba ~]$ lsnrctl|start|LSNRCTL|for|Linux: Version|18.0|.0|.0|.0|- Production|on|31|-JUL-|2018|00|:|59|:|03|Copyright (c)|1991|,|2018|, Oracle.|All|rights reserved.
Starting /u01/app/oracle/product/|18.1|.0|/dbhome_1/bin/tnslsnr: please wait...
TNSLSNR|for|Linux: Version|18.0|.0|.0|.0|- Production
System parameter file|is|/u01/app/oracle/product/|18.1|.0|/dbhome_1/network/admin/listener.ora
Log messages written|to|/u01/app/oracle/diag/tnslsnr/cndba/listener/alert/log.xml
Listening|on|: (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=cndba)(PORT=|1521|)))
Listening|on|: (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(|KEY|=EXTPROC1521)))
Connecting|to|(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=cndba)(PORT=|1521|)))
STATUS|of|the LISTENER
------------------------
Alias                     LISTENER
Version                   TNSLSNR|for|Linux: Version|18.0|.0|.0|.0|- Production|Start|Date|31|-JUL-|2018|00|:|59|:|03|Uptime|0|days|0|hr.|0|min|.|0|sec
Trace|Level|off
Security|ON|:|Local|OS Authentication
SNMP                      OFF
Listener Parameter File   /u01/app/oracle/product/|18.1|.0|/dbhome_1/network/admin/listener.ora
Listener Log File         /u01/app/oracle/diag/tnslsnr/cndba/listener/alert/log.xml
Listening Endpoints Summary...
  (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=cndba)(PORT=|1521|)))
  (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(|KEY|=EXTPROC1521)))
Services Summary...
Service|"cndba"|has|1|instance(s).
  Instance|"cndba"|, status|UNKNOWN|, has|1|handler|(s)|for|this service...
The command completed successfully|–查看监听状态
|在Linux操作系统上执行命令lsnrctl status
|[oracle@cndba ~]$ lsnrctl status
LSNRCTL|for|Linux: Version|18.0|.0|.0|.0|- Production|on|31|-JUL-|2018|00|:|59|:|06|Copyright (c)|1991|,|2018|, Oracle.|All|rights reserved.
Connecting|to|(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=cndba)(|PORT|=|1521|)))
STATUS|of|the LISTENER|------------------------|Alias|LISTENER
Version                   TNSLSNR|for|Linux: Version|18.0|.0|.0|.0|- Production
Start Date|31|-JUL-|2018|00|:|59|:|03|Uptime|0|days|0|hr.|0|min.|2|sec
Trace Level               off
Security|ON|: Local OS Authentication
SNMP                      OFF
Listener Parameter|File|/u01/app/oracle/product/|18.1|.0|/dbhome_1/network/admin/listener.ora
Listener Log|File|/u01/app/oracle/diag/tnslsnr/cndba/listener/alert/log.xml
Listening Endpoints Summary...
  (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=cndba)(|PORT|=|1521|)))
  (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1521)))
Services Summary...
Service|"cndba"|has|1|instance(s).
  Instance|"cndba"|, status UNKNOWN, has|1|handler(s)|for|this service...
The command completed successfully

