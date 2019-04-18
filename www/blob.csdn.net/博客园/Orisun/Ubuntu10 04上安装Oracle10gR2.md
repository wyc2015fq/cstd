# Ubuntu10.04上安装Oracle10gR2 - Orisun - 博客园







# [Ubuntu10.04上安装Oracle10gR2](https://www.cnblogs.com/zhangchaoyang/articles/1984161.html)





首先说明我为什么不装11g，因为11g对系统环境求太高（对于我这台四五年前买的机子而言），要求至少1G内存，虽然内存不到1G的情况下安装过程中可以忽略此条件继续安装，并且还可以安装成功并使用，但免不了会出一些使用和性能上的问题。10g则只要求一半的内存。

1.去Oracle官网上下载10201_database_linux32.zip，解压为database文件夹。

2.安装必需的包
apt-get install gcc make binutils lesstif2 libc6 libc6-dev rpm libmotif3 libaio1

3.我直接到当前用户orisun作为Oracle的用户，所以就没有再另外创建用户和组

4.我打算把oracle装到~/DB下面，所以mkdir ~/DB

5.修改 sysctl.conf
sudo gedit /etc/sysctl.conf
添加如下内容：
kernel.shmmax = 3147483648
kernel.shmmni = 4096
kernel.shmall = 2097152
kernel.sem = 250 32000 100 128
fs.file-max = 65536
net.ipv4.ip_local_port_range = 1024 65000

6.修改 limits.conf
sudo gedit /etc/security/limits.conf
添加如下内容：
soft nproc 2407
hard nproc 16384
soft nofile 1024
hard nofile 65536

7.让以上修改生效

sudo sysctl -p

8.创建软链接

创建并执行（以root身份）一个脚本文件：

#!/bin/bash
ln -s /usr/bin/awk /bin/awk
ln -s /usr/bin/rpm /bin/rpm
ln -s /usr/bin/basename /bin/basename
mkdir /etc/rc.d
ln -s /etc/rc0.d /etc/rc.d/rc0.d
ln -s /etc/rc2.d /etc/rc.d/rc2.d
ln -s /etc/rc3.d /etc/rc.d/rc3.d
ln -s /etc/rc4.d /etc/rc.d/rc4.d
ln -s /etc/rc5.d /etc/rc.d/rc5.d
ln -s /etc/rc6.d /etc/rc.d/rc6.d
ln -s /etc/init.d /etc/rc.d/init.d

9.创建RedHat版本声明

Oracle10g在安装时不承认Ubuntu版本，所以我们创建一个虚假的RedHat版本声明

sudo gedit /etc/redhat-release
添加如下内容：
Red Hat Linux release 3.1 (drupal)

10.设置语言并开始安装

set LANG=zh_CN

cd databse

./runInstaller

11.以root身份运行两个脚本文件orainstRoot.sh和root.sh

一路next下去后，中途会弹出一个警告框：

![](https://pic002.cnblogs.com/images/2011/103496/2011031420125236.png)

这时你要立刻去执行这两个脚本文件，否则下面还会报错：

![](https://pic002.cnblogs.com/images/2011/103496/2011031420143673.png)

我是安装完成后才去执行那两个脚本文件的，对于上面这个错误也直接忽略，暂时还没发现安装完成后有什么问题。

12.设置自动启动

创建自启动脚本/etc/init.d/oracledb,内容如下:
#!/bin/bash
#
# /etc/init.d/oracledb
#
# Run-level Startup script for the Oracle Instance, Listener, and
# Web Interface
####################下面引入与oracle相关的全局变量########################
export ORACLE_HOME=/home/orisun/DB/oracle/product/10.2.0/db_1
export ORACLE_SID=orcl
export PATH=$ORACLE_HOME/bin:$PATH
####################下面这句是设置oracle的用户########################
ORA_OWNR="orisun"
# if the executables do not exist -- display error
if [ ! -f $ORACLE_HOME/bin/dbstart -o ! -d $ORACLE_HOME ]
then
echo "Oracle startup: cannot start"
exit 1
fi
# depending on parameter -- startup, shutdown, restart
# of the instance and listener or usage display
case "$1" in
start)
# Oracle listener and instance startup
echo -n "Starting Oracle: "
su $ORA_OWNR -c "$ORACLE_HOME/bin/lsnrctl start"
su $ORA_OWNR -c "$ORACLE_HOME/bin/dbstart"
touch /var/lock/oracle
su $ORA_OWNR -c "$ORACLE_HOME/bin/emctl start dbconsole"
echo "OK"
;;
stop)
# Oracle listener and instance shutdown
echo -n "Shutdown Oracle: "
su $ORA_OWNR -c "$ORACLE_HOME/bin/lsnrctl stop"
su $ORA_OWNR -c "$ORACLE_HOME/bin/dbshut"
rm -f /var/lock/oracle
su $ORA_OWNR -c "$ORACLE_HOME/bin/emctl stop dbconsole"
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

13.设置权限,放到启动脚本中去
chmod 755 /etc/init.d/oracledb
update-rc.d oracledb defaults 99

14.修改/etc/oratab

注意只有以root执行了11步中所说的那两个脚本文件后才会生成/etc/oratab文件

编辑一下/etc/oratab，把其中的N改成Y，不然12步的脚本中所调用的$ORACLE_HOME /bin/dbstart在执行时不会 自动启动相应的数据库实例

orcl:/home/orisun/DB/oracle/product/10.2.0/db_1:Y

15.设置环境变量

在.bashrc中添加：

#Oracle
export ORACLE_HOME=/home/orisun/DB/oracle/product/10.2.0/db_1
export ORACLE_OWNER=orisun
export ORACLE_SID=orcl
export ORACLE_TERM=xterm
export PATH=$ORACLE_HOME/bin:$PATH

然后使生效source .bashrc

16.测试

$sqlplus /nolog
SQL> connect system/change_on_install as sysdba

或者登录：http://zcypc:1158/em

![](https://pic002.cnblogs.com/images/2011/103496/2011031420290527.png)

zcypc是我的计算机名，密码就是你安装数据库时设置的密码

17.安装Client

下载包，解压，runInstaller，就OK了，不需要改这改那的设置一大堆。












