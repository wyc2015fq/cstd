# CentOS静默安装Oracle12c详细过程 - 零度的博客专栏 - CSDN博客
2019年04月12日 11:27:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：67
转载出处：[https://blog.csdn.net/zwl18210851801/article/details/80774980](https://blog.csdn.net/zwl18210851801/article/details/80774980)
本人亲测没有问题，强烈推荐好文！
对上一篇文章的补充（增加，远程连接、自启动等），
转载一篇文章：[https://www.jianshu.com/p/309bb3504285](https://www.jianshu.com/p/309bb3504285)
大家可以对比看，相信会受益匪浅!
> 
说明：原创，经过一天的折腾，终于搞清楚centos7.2上静默安装oracle数据库，并且经过安装验证。由于之前都是在桌面环境中安装，一般依赖包安装完，安装过程和Windows上相似。但是如果服务器中的Linux安装了桌面环境，因为桌面环境运行还是比较占用资源，会拖慢服务器的速度，所以现在大多数Linux服务器都是无桌面版的，安装都是最小化环境。
当然，可以在安装最小化环境中的Linux中安装桌面环境，安装好后再次安装oracle数据库。最后又将桌面环境卸载。但是这种方法麻烦费时。
今天，花了一天时间，折腾安装桌面再次安装数据库，系统都做了好几次了。最后决定尝试静默安装数据库，终于尝试两次之后安装成功。
> 
只要尝试，不断总结，肯定会成功的。原来，静默安装步骤也就比较简单。主要还是在响应文件的配置。其他的配置基本和桌面环境安装一样。
`说明：为了安装方便，最好使用Xshell类似的工具连接到centos终端`
### 1、创建用户及组
> 
groupadd oinstall #创建oinstall
groupadd dba # 创建dba组
useradd -g oinstall -G dba oracle #创建oracle用户
passwd oracle # 修改oracle用户的登录密码
[root@rainbow oracle]# id oracle #查看oracle用户信息
uid=1000(oracle) gid=1000(oinstall) groups=1000(oinstall),1001(dba)
### 2、创建目录
> 
mkdir -p /u01/app/oracle
chown -R oracle:oinstall /u01/app/oracle
chmod -R 775 /u01/app/oracle
### 3、安装依赖包
> 
依赖包是安装oracle的必须，如果某些需要的依赖包没有安装上，安装过程中会出现各种各样的错误。所以，依赖包安装完整，安装过程一般会比较顺利。
直接执行如下命令，安装依赖包。
`yum -y install binutils compat-libcap1 compat-libstdc++-33 compat-libstdc++-33*i686 compat-libstdc++-33*.devel compat-libstdc++-33 compat-libstdc++-33*.devel gcc gcc-c++ glibc glibc*.i686 glibc-devel glibc-devel*.i686 ksh libaio libaio*.i686 libaio-devel libaio-devel*.devel libgcc libgcc*.i686 libstdc++ libstdc++*.i686 libstdc++-devel libstdc++-devel*.devel libXi libXi*.i686 libXtst libXtst*.i686 make sysstat unixODBC unixODBC*.i686 unixODBC-devel unixODBC-devel*.i686`
- *参考官方：[http://docs.oracle.com/cd/E11882_01/install.112/e24326/toc.htm#BHCCADGD](https://link.jianshu.com/?t=http://docs.oracle.com/cd/E11882_01/install.112/e24326/toc.htm#BHCCADGD)*
- The following or later version of packages for Oracle Linux 7, and Red Hat Enterprise Linux 7 must be installed:
`binutils-2.23.52.0.1-12.el7.x86_64 compat-libcap1-1.10-3.el7.x86_64 compat-libstdc++-33-3.2.3-71.el7.i686 compat-libstdc++-33-3.2.3-71.el7.x86_64 gcc-4.8.2-3.el7.x86_64 gcc-c++-4.8.2-3.el7.x86_64 glibc-2.17-36.el7.i686 glibc-2.17-36.el7.x86_64 glibc-devel-2.17-36.el7.i686 glibc-devel-2.17-36.el7.x86_64 ksh libaio-0.3.109-9.el7.i686 libaio-0.3.109-9.el7.x86_64 libaio-devel-0.3.109-9.el7.i686 libaio-devel-0.3.109-9.el7.x86_64 libgcc-4.8.2-3.el7.i686 libgcc-4.8.2-3.el7.x86_64 libstdc++-4.8.2-3.el7.i686 libstdc++-4.8.2-3.el7.x86_64 libstdc++-devel-4.8.2-3.el7.i686 libstdc++-devel-4.8.2-3.el7.x86_64 libXi-1.7.2-1.el7.i686 libXi-1.7.2-1.el7.x86_64 libXtst-1.2.2-1.el7.i686 libXtst-1.2.2-1.el7.x86_64 make-3.82-19.el7.x86_64 sysstat-10.1.5-1.el7.x86_64 unixODBC-2.3.1-6.el7.x86_64 or later unixODBC-2.3.1-6.el7.i686 or later unixODBC-devel-2.3.1-6.el7.x86_64 or later unixODBC-devel-2.3.1-6.el7.i686 or later`
### 4、更改hosts文件
> 
hosts文件中，要讲主机名添加到配置文件中，及添加到127.0.0.1后面，如果没有加，安装过程中会出错。
- 首先查看主机名：使用hostname命令
`[root@rainbow oracle]# hostname rainbow`
- 主机名为rainbow，则把rainbow添加到/etc/hosts文件中：
`[root@rainbow oracle]# vi /etc/hosts`
`127.0.0.1 localhost localhost.localdomain localhost4 localhost4.localdomain4 rainbow ::1 localhost localhost.localdomain localhost6 localhost6.localdomain6`
- 将自己的主机名添加到文件中，保存退出。
### 5、配置内核参数/etc/sysctl.conf
> 
编辑sysctl.conf文件
`[root@rainbow oracle]# vi /etc/sysctl.conf
- 
加入如下配置
	fs.aio-max-nr = 1048576
	fs.file-max = 6815744
	kernel.shmall = 2097152
	kernel.shmmax = 536870912
	kernel.shmmni = 4096
	kernel.sem = 250 32000 100 128
	net.ipv4.ip_local_port_range = 9000 65500
	net.core.rmem_default = 262144
	net.core.rmem_max = 4194304
	net.core.wmem_default = 262144
	net.core.wmem_max = 1048576
	vm.hugetlb_shm_group = 1001 #此处与id oracle中的dba中一致
- 
让内核参数生效，执行：/sbin/sysctl -p
### 6、修改用户限制
> 
vi /etc/secruity/limits.conf
#在末尾添加
oracle soft nproc 2047
oracle hard nproc 16384
oracle soft nofile 1024
oracle hard nofile 65536
oracle soft stack 10240
- 保存退出。
### 7、修改 /etc/pam.d/login
> 
vi /etc/pam.d/login
打开编辑此文件，在末尾加入如下配置：
`session required /lib64/security/pam_limits.so session required pam_limits.so`
`说明：此处，要使用lib64,如果使用lib，则可能会出现无法登陆系统的情况，使用lib是在32位的情况下，如果是64位系统，则应该使用lib64`
- 保存退出。
### 8、修改系统配置文件/etc/profile
> 
vi /etc/profile
加入如下配置：
`if [[ $USER = "oracle" ]]; then if [[ $SHELL = "/bin/ksh" ]]; then ulimit -p16384 ulimit -n65536 else ulimit -u16384 -n 65536 fi fi`
- 注意空格
- 保存退出
- 使其生效 source /etc/profile
### 9、修改oracle用户环境变量
> 
`vi /home/oracle/.bash_profile`
加入如下配置文件：
`#oracle
export TMP=/tmp;
export TMPDIR=$TMP;
export ORACLE_BASE=/u01/app/oracle;
export ORACLE_HOME=$ORACLE_BASE/product/12.1.0/db_1;
export ORACLE_SID=orcl;
export ORACLE_TERM=xterm;
export PATH=/usr/sbin:$PATH;
export PATH=$ORACLE_HOME/bin:$PATH;
export LD_LIBRARY_PATH=$ORACLE_HOME/lib:/lib:/usr/lib;
export CLASSPATH=$ORACLE_HOME/JRE:$ORACLE_HOME/jlib:$ORACLE_HOME/rdbms/jlib;`
- 保存退出。
- 使其生效：source /home/oracle/.bash_profile
### 10、上传、解压oracle安装文件
> 
使用sftp将安装文件上传至服务器。
- 解压文件夹
`[root@rainbow oracle]# unzip linuxamd64_12102_database_1of2.zip [root@rainbow oracle]# unzip linuxamd64_12102_database_2of2.zip`
	加压之后，得到一个database的文件夹，将其移动到/u01/app/oracle目录下
	mv database /u01/app/oracle
- 对文件授权
	chown -R oracle:oinstall /u01/app/oracle/database
### 11、编辑、安装数据库响应文件db_install.rsp
> 
最终的安装主要在与三个响应文件的配置：dbca.rsp,netca.rsp,db_install.rsp，分别对应于创建数据库，创建监听，安装数据库。三个文件的路径在database/response 文件夹下。
为了防止出错，我们把三个响应文件复制到oracle用户路径下，出错之后可以删除再复制更改即可。
- 
在oracle用户目录下，创建一个etc文件夹
`[oracle@rainbow /]$ mkdir /home/oracle/etc`
- 
复制响应文件
`[oracle@rainbow /]$ cp /u01/app/oracle/database/response/* /home/oracle/etc`
`[oracle@rainbow /]$ ls /home/oracle/etc/ dbca.rsp db_install.rsp netca.rsp`
- 
授权
`[oracle@rainbow /]$ su - root [root@rainbow ~]# chmod 700 /home/oracle/etc/*.rsp`
- 
编辑安装响应文件
`[root@rainbow ~]# vi /home/oracle/etc/db_install.rsp`
- 
找到如下的配置，将配置更改如下：
	`oracle.install.option=INSTALL_DB_SWONLY // 安装类型
	ORACLE_HOSTNAME=rainbow // 主机名称（hostname查询）
	UNIX_GROUP_NAME=oinstall // 安装组
	INVENTORY_LOCATION=/u01/app/oracle/oraInventory //INVENTORY目录（不填就是默认值）
	SELECTED_LANGUAGES=en // 选择语言
	ORACLE_HOME=/u01/app/oracle/product/12.1.0/db_1 //oracle_home
	ORACLE_BASE=/u01/app/oracle //oracle_base
	oracle.install.db.InstallEdition=EE 　　　　// oracle版本
	oracle.install.db.isCustomInstall=false 　　//自定义安装，否，使用默认组件
	oracle.install.db.DBA_GROUP=dba // dba用户组
	oracle.install.db.OPER_GROUP=dba // oper用户组
	oracle.install.db.config.starterdb.type=GENERAL_PURPOSE //数据库类型
	oracle.install.db.config.starterdb.globalDBName=orcl //globalDBName
	oracle.install.db.config.starterdb.SID=orcl //SID
oracle.install.db.config.starterdb.password.ALL=admin123 //设定所有数据库用户使用同一个密码
SECURITY_UPDATES_VIA_MYORACLESUPPORT=false //（手动写了false）
DECLINE_SECURITY_UPDATES=true 　　//设置安全更新（貌似是有bug，这个一定要选true，否则会无限提醒邮件地址有问题，终止安装。PS：不管地址对不对）`
- 配置好后。保存退出。
- 开始安装数据库。切换到oracle用户下
`[root@rainbow ~]# su - oracle [oracle@rainbow database]$ /u01/app/oracle/database/runInstaller -silent -responseFile /home/oracle/etc/db_install.rsp`
- 安装过程如下，出现如下结果，说明安装成功
	Starting Oracle Universal Installer...
Checking Temp space: must be greater than 500 MB. Actual 17997 MB Passed
Checking swap space: must be greater than 150 MB. Actual 2999 MB Passed
Preparing to launch Oracle Universal Installer from /tmp/OraInstall2017-06-04_07-51-31PM. Please wait ...[oracle@rainbow database]$ [WARNING] [INS-32055] The Central Inventory is located in the Oracle base.
ACTION: Oracle recommends placing this Central Inventory in a location outside the Oracle base directory.
[WARNING] [INS-13014] Target environment does not meet some optional requirements.
CAUSE: Some of the optional prerequisites are not met. See logs for details. /tmp/OraInstall2017-06-04_07-51-31PM/installActions2017-06-04_07-51-31PM.log
ACTION: Identify the list of failed prerequisite checks from the log: /tmp/OraInstall2017-06-04_07-51-31PM/installActions2017-06-04_07-51-31PM.log. Then either from the log file or from installation manual find the appropriate configuration to meet the prerequisites and fix it manually.
You can find the log of this install session at:
/u01/app/oracle/oraInventory/logs/installActions2017-06-04_07-51-31PM.log
The installation of Oracle Database 12c was successful.
Please check '/u01/app/oracle/oraInventory/logs/silentInstall2017-06-04_07-51-31PM.log' for more details.
As a root user, execute the following script(s):
1. /u01/app/oracle/oraInventory/orainstRoot.sh
2. /u01/app/oracle/product/12.1.0/db_1/root.sh
Successfully Setup Software
- 使用root用户登录 ，分别执行
`/u01/app/oracle/oraInventory/orainstRoot.sh /u01/app/oracle/product/12.1.0/db_1/root.sh`
- 到此，数据库已成功安装上。
### 12、配置、启动监听
> 
监听器配置响应文件为：/home/oracle/etc/netca.rsp,此文件使用默认即可。
- 
执行配置监听
`[oracle@rainbow database]$ /u01/app/oracle/product/12.1.0/db_1/bin/netca -silent -responseFile /home/oracle/etc/netca.rsp`
- 
出现如下信息，说明监听配置成功。
	[oracle@rainbow database]$ /u01/app/oracle/product/12.1.0/db_1/bin/netca -silent -responseFile /home/oracle/etc/netca.rsp
	Parsing command line arguments:
	Parameter "silent" = true
	Parameter "responsefile" = /home/oracle/etc/netca.rsp
	Done parsing command line arguments.
	Oracle Net Services Configuration:
	Profile configuration complete.
	Oracle Net Listener Startup:
	Running Listener Control:
	/u01/app/oracle/product/12.1.0/db_1/bin/lsnrctl start LISTENER
	Listener Control complete.
	Listener started successfully.
	Listener configuration complete.
	Oracle Net Services configuration successful. The exit code is 0
	[oracle@rainbow database]$
- 
启动监听
> 
[oracle@rainbow database]$ lsnrctl start
- 查看监听状态
> 
[oracle@rainbow database]$ lsnrctl status
LSNRCTL for Linux: Version 12.1.0.2.0 - Production on 04-JUN-2017 20:15:07
Copyright (c) 1991, 2014, Oracle. All rights reserved.
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=localhost)(PORT=1521)))
STATUS of the LISTENER
Alias LISTENER
Version TNSLSNR for Linux: Version 12.1.0.2.0 - Production
Start Date 04-JUN-2017 20:08:08
Uptime 0 days 0 hr. 7 min. 0 sec
Trace Level off
Security ON: Local OS Authentication
SNMP OFF
Listener Parameter File /u01/app/oracle/product/12.1.0/db_1/network/admin/listener.ora
Listener Log File /u01/app/oracle/diag/tnslsnr/rainbow/listener/alert/log.xml
Listening Endpoints Summary...
(DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=localhost)(PORT=1521)))
(DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1521)))
The listener supports no services
The command completed successfully
- 重启监听
> 
[oracle@rainbow database]$ lsnrctl reload
### 13、静默创建数据库实例
> 
编辑应答文件
`[oracle@rainbow database]$ vi /home/oracle/etc/dbca.rsp`
- 
将其更改如下：
`[GENERAL] RESPONSEFILE_VERSION = "12.1.0" OPERATION_TYPE = "createDatabase" [CREATEDATABASE] GDBNAME = "dbsrv2" SID = "dbsrv2" TEMPLATENAME = "General_Purpose.dbc" CHARACTERSET = "AL32UTF8"`
- 
保存退出
> 
执行安装数据库实例
`[oracle@rainbow database]$ /u01/app/oracle/product/12.1.0/db_1/bin/dbca -silent -responseFile /home/oracle/etc/dbca.rsp`
- 执行安装后，依次输入sys和system的密码，然后耐性等待，会显示安装的进度。
- 查看日志信息：
`[oracle@rainbow database]$ cat /u01/app/oracle/cfgtoollogs/dbca/orcl/orcl.log
Unique database identifier check passed.
/ has enough space. Required space is 6140 MB , available space is 12658 MB.
File Validations Successful.
Copying database files
DBCA_PROGRESS : 1%
DBCA_PROGRESS : 3%
DBCA_PROGRESS : 11%
DBCA_PROGRESS : 18%
DBCA_PROGRESS : 26%
DBCA_PROGRESS : 37%
Creating and starting Oracle instance
DBCA_PROGRESS : 40%
DBCA_PROGRESS : 45%
DBCA_PROGRESS : 50%
DBCA_PROGRESS : 55%
DBCA_PROGRESS : 56%
DBCA_PROGRESS : 60%
DBCA_PROGRESS : 62%
Completing Database Creation
DBCA_PROGRESS : 66%
DBCA_PROGRESS : 70%
DBCA_PROGRESS : 73%
DBCA_PROGRESS : 85%
DBCA_PROGRESS : 96%
DBCA_PROGRESS : 100%
Database creation complete. For details check the logfiles at:
/u01/app/oracle/cfgtoollogs/dbca/orcl.
Database Information:
Global Database Name:orcl
System Identifier(SID):orcl
`
- 日志显示，数据库实例已经成功创建。
- 此时，查看监听，orcl数据库实例已经正常运行。
	`[oracle@rainbow database]$ lsnrctl status
LSNRCTL for Linux: Version 12.1.0.2.0 - Production on 04-JUN-2017 20:54:52
Copyright (c) 1991, 2014, Oracle. All rights reserved.
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=localhost)(PORT=1521)))
STATUS of the LISTENER
Alias LISTENER
Version TNSLSNR for Linux: Version 12.1.0.2.0 - Production
Start Date 04-JUN-2017 20:08:08
Uptime 0 days 0 hr. 46 min. 44 sec
Trace Level off
Security ON: Local OS Authentication
SNMP OFF
Listener Parameter File /u01/app/oracle/product/12.1.0/db_1/network/admin/listener.ora
Listener Log File /u01/app/oracle/diag/tnslsnr/rainbow/listener/alert/log.xml
Listening Endpoints Summary...
(DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=localhost)(PORT=1521)))
(DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1521)))
Services Summary...
Service "orcl" has 1 instance(s).
Instance "orcl", status READY, has 1 handler(s) for this service...
Service "orclXDB" has 1 instance(s).
Instance "orcl", status READY, has 1 handler(s) for this service...
The command completed successfully
`
### 14、数据库的启动与停止
> 
进入到oracle安装目录的bin目录下，执行 dbstop即可停止数据库。
### 15、开放端口（使用root用户）
> 
由于数据库安装了一般需要外网访问，所以需要开放数据库连接端口。
[root@rainbow /]# firewall-cmd --zone=public --add-port=1521/tcp --permanent
success
[root@rainbow /]# firewall-cmd --zone=public --add-port=5500/tcp --permanent
success
[root@rainbow /]# firewall-cmd --reload
success
- 命令含义：
	--zone #作用域
	--add-port=80/tcp #添加端口，格式为：端口/通讯协议
	--permanent #永久生效，没有此参数重启后失效
- 重启防火墙
	firewall-cmd --reload
### 16、设置oracle数据库随系统开机自启动。
> 
大多数情况下，我们都需要让oracle数据库随服务器开机自启动。
- 1、编辑/etc/oratab
	orcl:/u01/app/oracle/product/12.1.0/db_1:N
- 将以上配置改为：
	orcl:/u01/app/oracle/product/12.1.0/db_1:Y
- 保存退出
- 2、编辑/etc/rc.d/rc.local
	vi /etc/rc.d/rc.local
- 加入如下配置：
	su oracle -lc "/u01/app/oracle/product/12.1.0/db_1/bin/lsnrctl start"
	su oracle -lc /u01/app/oracle/product/12.1.0/db_1/bin/dbstart
- 保存退出
- 重启系统，再次连接数据库，将正常连接上。
