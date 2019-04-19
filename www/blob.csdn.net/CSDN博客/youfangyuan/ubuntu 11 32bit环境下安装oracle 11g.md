# ubuntu 11 32bit环境下安装oracle 11g - youfangyuan - CSDN博客
2013年01月20日 19:41:12[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：411
                
上周在自己的ubuntu环境里安装好了oracle 11g, 特此把安装过程纪录下来，留作备忘录。
事前准备:
系统更新sudo
Console代码  
sudo apt-get update    
sudo apt-get upgrade   
安装系统所需要的packagesudo
Console代码  
sudo su -  
apt-get install build-essential  
apt-get install libaio1  
apt-get install libaio-dev  
apt-get install unixODBC  
apt-get install unixODBC-dev  
apt-get install pdksh  
apt-get install expat  
apt-get install sysstat  
apt-get install libelf-dev  
apt-get install elfutils  
apt-get install lsb-cxx  
apt-get install ksh   
apt-get install libmotif3   
apt-get install alien   
apt-get install libtool   
apt-get install lsb-rpm  
为避免linking ctx/lib/ins_ctx.mk的编译错误，下载libstdc++5_3.3.6-25ubuntu1_i386.deb，并进行安装。
 为避免编译中link错误，执行下面命令
Console代码  
sudo ln -s /usr/bin/awk /bin/awk   
sudo ln -s /usr/bin/rpm /bin/rpm   
sudo ln -s /usr/bin/basename /bin/basename  
sudo ln -s /usr/lib/i386-linux-gnu/libc_nonshared.a /usr/lib  
sudo ln -s /usr/lib/i386-linux-gnu/libpthread_nonshared.a /usr/lib  
sudo ln -s /usr/lib/i386-linux-gnu/libstdc++.so.5 /usr/lib  
sudo ln -s /usr/lib/i386-linux-gnu/libstdc++.so.6 /usr/lib  
sudo ln -s /lib/i386-linux-gnu/libgcc_s.so.1 /lib  
 修改系统参数
Console代码  
sudo su -  
#backup  
cp /etc/sysctl.conf /etc/sysctl.original  
vi /etc/sysctl.conf, 添加以下内容  
    # Oracle 11gR2 entries  
    fs.aio-max-nr=1048576  
    fs.file-max=6815744  
    kernel.shmall=2097152  
    kernel.shmmni=4096  
    kernel.sem=250 32000 100 128  
    net.ipv4.ip_local_port_range=9000 65500  
    net.core.rmem_default=262144  
    net.core.rmem_max=4194304  
    net.core.wmem_default=262144  
    net.core.wmem_max=1048586  
    kernel.shmmax=2147483648  
Note: kernel.shmmax = max possible value, e.g. size of physical memory in bytes  
#启动新的系统参数 Load new kernel parameters  
sysctl -p  
创建用户和组
Console代码  
sudo groupadd oinstall  
sudo groupadd dba  
sudo groupadd nobody  
sudo usermod -g nobody nobody  
sudo useradd -g oinstall -G dba -d /home/oracle -s /bin/bash oracle  
sudo password oracle  
mkdir /home/oracle  
chown -R oracle:dba /home/oracle  
修改/etc/security/limits.conf，添加以下内容
Console代码  
oracle soft nproc 2047  
oracle hard nproc 16384  
oracle soft nofile 1024  
oracle hard nofile 65536  
创建oracle安装目录
Console代码  
mkdir -p /u01/app/oracle  
chown -R oracle:oinstall /u01/app/oracle  
chmod -R 775 /u01/app/oracle  
mkdir -p /u01/app/oraInventory  
chown -R oracle:oinstall /u01/app/oraInventory  
chmod -R 775 /u01/app/oraInventory  
开始正式安装:
以上边创建的oracle 用户login
执行以下命令开始安装
Console代码  
export LANG=C  
#在安装目录下  
./runInstaller  
安装过程中，可能会遇到以下的编译错误
Console代码  
Error in invoking target 'agent nmhs' of makefile  
'/u01/app/oracle/product/11.2.0/db_1/sysman/lib/ins_emagent.mk.'  
See  
'/u01/app/oraInventory/logs/installActions2012-03-11_06-59-43PM.log'  
for details.  
或者还有  
Error in invoking target 'all_no_orcl' of makefile  
'/u01/app/oracle/product/11.2.0/db_1/rdbms/lib/ins_rdbms.mk.'  
See  
'/u01/app/oraInventory/logs/installActions2012-03-11_06-59-43PM.log'  
for details.  
为解决上述编译错误，修改如下文件，然后再执行编译
Console代码  
sysman/lib/ins_emagent.mk：  
变更前:  
LDFLAGS=-Wl -o $@...  
变更后：  
LDFLAGS=-Wl,--copy-dt-needed-entries -o $@...  
bin/genorasdksh:  
变更前:  
$LD SLD_RUNTIME -Wl $D_OPT $LD_OUT ...  
变更后:  
$LD SLD_RUNTIME -Wl,--no-as-needed $D_OPT $LD_OUT ...  
srvm/lib/ins_srvm.mk:  
变更前:  
GETCRSHOME_LINKLINE=$(LINK) $(LDPATHFLAG)$(RDBMSLIB) $(CDEBUG) $(OPT) $(GETCRSHOME_OBJ1) -Wl  
变更后:  
GETCRSHOME_LINKLINE=$(LINK) $(LDPATHFLAG)$(RDBMSLIB) $(CDEBUG) $(OPT) $(GETCRSHOME_OBJ1) -Wl,--no-as-needed  
network/lib/env_network.mk:  
变更前:  
TNSLSNR_LINKLINE=$(LINK) $(TNSLSNR_OFILES) -Wl $(LINKTTLIBS) ...  
变更后:  
TNSLSNR_LINKLINE=$(LINK) $(TNSLSNR_OFILES) -Wl,--no-as-needed $(LINKTTLIBS) ...  
rdbms/lib/env_rdms.mk：  
变更前:  
ORACLE_LINKER=$(ADE_DEL_FILE_CMD) $(PURECMDS) gcc -Wl $(OLAPPRELINKOPTS) $(LDFLAGS) $(COMPSOBJS)  
变更后:  
ORACLE_LINKER=$(ADE_DEL_FILE_CMD) $(PURECMDS) gcc -Wl, --no-as-needed $(OLAPPRELINKOPTS) $(LDFLAGS) $(COMPSOBJS)  
按照安装程序的提示，以root权限login，然后执行命令 /u01/app/oracle/product/11.2.0/db_1/root.sh
创建启动程序 vi /etc/init.d/oracledb, 加入以下内容
Shell代码  
#!/bin/bash  
export ORACLE_BASE=/u01/app/oracle  
export ORACLE_HOME=/u01/app/oracle/product/11.2.0/dbhome_1  
export ORACLE_OWNR=oracle  
export PATH=$PATH:$ORACLE_HOME/bin  
if [ ! -f $ORACLE_HOME/bin/dbstart -o ! -d $ORACLE_HOME ]  
then  
echo "Oracle startup: cannot start"  
exit 1  
fi  
case "$1" in  
start)  
# Oracle listener and instance startup  
echo -n "Starting Oracle: "  
su $ORACLE_OWNR -c "$ORACLE_HOME/bin/lsnrctl start"  
su $ORACLE_OWNR -c "$ORACLE_HOME/bin/dbstart $ORACLE_HOME"  
touch /var/lock/oracle  
echo "OK"  
;;  
stop)  
# Oracle listener and instance shutdown  
echo -n "Shutdown Oracle: "  
su $ORACLE_OWNR -c "$ORACLE_HOME/bin/lsnrctl stop"  
su $ORACLE_OWNR -c "$ORACLE_HOME/bin/dbshut $ORACLE_HOME"  
rm -f /var/lock/oracle  
echo "OK"  
;;  
reload|restart)  
$0 stop  
$0 start  
;;  
*)  
echo "Usage: `basename $0` start|stop|restart|reload"  
exit 1  
esac  
exit 0  
