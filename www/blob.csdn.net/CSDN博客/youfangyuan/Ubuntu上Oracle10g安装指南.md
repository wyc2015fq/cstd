# Ubuntu上Oracle10g安装指南 - youfangyuan - CSDN博客
2013年01月20日 19:40:10[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：662标签：[oracle																[Oracle																[ORACLE](https://so.csdn.net/so/search/s.do?q=ORACLE&t=blog)](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[坏境搭建](https://blog.csdn.net/youfangyuan/article/category/1330794)
                
安装体验版的Oracle 10g
体验版的Oracle10g比较适合在本子上或者开发的机器上使用，它提供了Oracle10g的大多数功能，并且仅需要更小的安装空间和运行环境。
[编辑] 下载体验版Oracle 10g的deb包
从下列地址下载：
http://www.oracle.com/technology/software/products/database/oracle10g/index.html
http://www.oracle.com/technetwork/database/express-edition/downloads/102xelinsoft-102048.html
根本不好下载，每次注册时都提示用户名已被使用。
用gmail的邮箱，密码8位大写小写数字——by zero_coder
[编辑] 安装下载的deb包
安装oracle很简单，比如下载的文件在/home/xxx/download目录，只要执行即可：
sudo dpkg -i /home/xxx/download/oracle-xe-universal_10.2.0.1-1.0_i386.deb
安装的时候需要注意控制台输出，在安装过程中需要设定监听端口和密码。 如果没有提示设置，可以手工设置 /etc/init.d/oracle-xe configure。 安装後如果一切正常的话Oracle应该会自动启动，是用ps命令可以查看当前运行的oracle进行，判断Oracle是否正确运行：
sudo ps aux |grep oracle
输出的结果应该类似如下：
oracle   16282  0.0  0.2  22076  6068 ?        Ss   22:08   0:00 /usr/lib/o
oracle   16290  0.0  0.4 671464 10248 ?        Ss   22:08   0:00 xe_pmon_XE
oracle   16292  0.0  0.4 670772  8828 ?        Ss   22:08   0:00 xe_psp0_XE
oracle   16294  0.0  0.9 670772 20028 ?        Ss   22:08   0:00 xe_mman_XE
oracle   16296  0.0  0.5 672844 11400 ?        Ss   22:08   0:00 xe_dbw0_XE
oracle   16298  0.0  1.3 686324 28228 ?        Ss   22:08   0:00 xe_lgwr_XE
oracle   16300  0.0  0.6 670780 14000 ?        Ss   22:08   0:00 xe_ckpt_XE
oracle   16302  0.0  1.4 671316 29752 ?        Ss   22:08   0:00 xe_smon_XE
oracle   16304  0.0  0.6 670776 14096 ?        Ss   22:08   0:00 xe_reco_XE
oracle   16306  0.0  1.1 672376 22916 ?        Ss   22:08   0:00 xe_cjq0_XE
oracle   16308  0.2  2.0 673520 43060 ?        Ss   22:08   0:00 xe_mmon_XE
oracle   16310  0.0  0.4 670772  9944 ?        Ss   22:08   0:00 xe_mmnl_XE
oracle   16312  0.0  0.4 671440  8952 ?        Ss   22:08   0:00 xe_d000_XE
oracle   16314  0.0  0.4 671324  8668 ?        Ss   22:08   0:00 xe_s000_XE
oracle   16316  0.0  0.4 671324  8676 ?        Ss   22:08   0:00 xe_s001_XE
oracle   16318  0.0  0.4 671320  8668 ?        Ss   22:08   0:00 xe_s002_XE
oracle   16320  0.0  0.4 671320  8676 ?        Ss   22:08   0:00 xe_s003_XE
oracle   16333  0.0  0.4 670772  9956 ?        Ss   22:08   0:00 xe_qmnc_XE
oracle   16353  0.0  0.5 670772 11000 ?        Ss   22:08   0:00 xe_q000_XE
oracle   16358  0.0  0.4 670772  9284 ?        Ss   22:08   0:00 xe_q001_XE
默认安装好以後，Oracle会自动运行，如果Oracle没有启动起来，需要手动去运行init.d下面的脚本：
sudo /etc/init.d/oracle-xe start
如果输出如下则表示启动成功：
Starting Oracle Net Listener.
Starting Oracle Database 10g Express Edition Instance.
[编辑] 使用网页浏览器访问Oracle的http管理界面
启动firefox或者其他浏览器，在地址栏输入地址(此处的8080端口是在安装时给定的)：
http://localhost:8080/apex
就可以进入到管理界面，在这个管理界面中，可以完成如创建数据库、用户管理、表管理和存储与运行时管理的诸多配置工作。注意默认的安装设定sys/system是同一个密码，这个要注意。
详细安装方式请参考Oracle的官方说明。
[编辑] 安装企业版的Oracle 10g
written by flexitime, 2007-10-15〔http://flexitime.blog.sohu.com/）
今天终于在Ubuntu7.04上成功安装Oracle10g。为了安装Oracle，花了不少时间到网上查资料，不过更多的时间用于自己的真正实践，当中走了不少的弯路，现在把安装过程记下来。（我一共在两台机器上安装了三次，这篇文章是在安装第三次的时候一边操作一边在另外的电脑上写的） 在网上有不少Oracle的安装教程，但这些教程更多的只是针对较低版本的Ubuntu（如6.X，5.X等等），而且安装Oracle的过程中也有些问题是没有描述和解决的。因此，我觉得有必要自己写一下安装我的实践过程，当然，我主要还是参考了孙高勇先生的《Oracle
 10g for Ubuntu安装指南》的内容。 现在开始吧！
[编辑] 第一步，我们需要一个Ubuntu
当然在网上可以免费下载得到，我用的是桌面版，而不是网站上提供的服务器版，很多刚由Windows转过来的朋友可能会认为服务器版总会比桌面版多些功能，为此而下载了服务版来用。其实Ubuntu网上提供的下载却不然，服务器版只不过比桌面版多了amp的软件（a-apache, m-mysql, p-php)，但却没有图形用户界面（可以另外再安装，但比较麻烦）。为此，我使用了桌面版的Ubuntu来完成这次的安装。安装的时候虽然有中文可以选择，但我还是选用了英文，这不是我的英文特别好（再好也好不过母语）的缘故，而是因为Ubuntu的汉化未如理想，汉字的支持也不怎么样（特别是在纯
 CUI模式下）。所以还是用英文会更好（不必担心，在英文版Ubuntu下一样可以用中文的输入法，同样也可以让Oracle支持中文），要安装 ubuntu，可以参考网上的其它教程。
[编辑] 第二步，安装必需的包
apt-get install gcc make binutils lesstif2 libc6 libc6-dev rpm libmotif3 libaio1 libstdc++6 alien
Note: 在我使用Ubuntu 10.10安装oracle时，会报一个makefile到错误。这是因为新版本到ubuntu默认安装了libstdc++6，而oracle安装程序要使用的是libstdc++5，所以如果遇到这个错误可以执行 sudo apt-get install libstdc++5 然后在安装程序的报错对话框中点retry就可以继续了。
64位：sudo apt-get install gcc libaio1 lesstif2 lesstif2-dev make libc6 libc6-i386 libc6-dev-i386 libstdc++5 lib32stdc++6 lib32z1 ia32-libs
[编辑] 第三步，创建用户
# adduser oracle
正在添加用户"oracle"...
正在添加新组"oracle" (1001)...
正在添加新用户"oracle" (1002) 到组"oracle"...
创建主目录"/home/oracle"...
正在从"/etc/skel"复制文件...
输入新的 UNIX 密码： 
重新输入新的 UNIX 密码： 
passwd：已成功更新密码
Changing the user information for oracle
Enter the new value, or press ENTER for the default
Full Name []: oracle
Room Number []: 
Work Phone []: 
Home Phone []: 
Other []: 
adduser 命令系统会自动创建oracle用户的文件
[编辑] 第四步，修改设置
[编辑] A.设置swap区
Oracle10g至少需要500M的内存和400M的交换空间，要查看swap区是否足够大小，用 fdisk -l 命令去查，如果小于400M的空间，那么就要增加swap的大小 重设交换分区可以使用如下操作:
dd if=/dev/zero of=tmp_swap bs=1k count=900000
chmod 600 tmp_swap
mkswap tmp_swap
swapon tmp_swap
完成安装以後,可以释放这个空间:
swapoff tmp_swap
rm tmp_swap
count 值是根据你需要调整的交换分区大小而定。
[编辑] B.修改 sysctl.conf
添加如下的行到/etc/sysctl.conf 中:
kernel.shmmax = 3147483648
kernel.shmmni = 4096
kernel.shmall = 2097152
kernel.sem = 250 32000 100 128
fs.file-max = 65536
net.ipv4.ip_local_port_range = 1024 65000
[编辑] C.修改 limits.conf
添加如下的行到/etc/security/limits.conf中：
* soft nproc 2407
* hard nproc 16384
* soft nofile 1024
* hard nofile 65536
[编辑] D.让修改生效
修改了以上文件後,必须让其生效,或重启系统,或切换到 root 用户下用以下的方式改变内核运行参数: sysctl -p
[编辑] E.产生相应的软连接
创建一个文件如 kk，内容如下：
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
创建後，切换到 root 用户去执行一下。
[编辑] F.创建RedHat的版本声明文件
在/etc/redhat-release中添加以下语句，以使安装程序认为正在一个RedHat的系统上安装：
Red Hat Linux release 3.1 (drupal)
[编辑] G.修改环境变量
编辑 /home/oracle/.bashrc，增加以下export 的内容。 (注意，在Ubnutu 7.04中用户的profile文件已改名为~/.profile，有很多安装教程都是用 ~/.bash_profile，在7.04中不行的)
export ORACLE_HOME=/opt/ora10
export ORACLE_OWNER=oracle
export ORACLE_SID=ora1
export ORACLE_TERM=xterm
export PATH=$ORACLE_HOME/bin:$ORACLE_HOME/Apache/Apache/bin:$PATH
[编辑] 第五步，开始安装
注销原来的用户，改用oracle用户登录。用env查看一下环境变量是否生效。 然後进行/ora_ins_disk中进行安装，执行安装脚本时还需要以root权限创建目录/opt/ora10
sudo mkdir /opt/ora10
sudo chown -R oracle:oracle /opt/ora10
sudo chmod -R 770 /opt/ora10 
cd/ora_ins_disk
./runInstaller
在安装过程中，请使用 Advanced Installation，然後一路按默认的设置进行往下设置，到窗单名为 “Specify Database Configuration Options”的时候，要修改以下设置： Database Character Set 中选择 Simplified Chinese ZHS16GBK 在安装的後期，系统提示需要用 root 用户去运行两个脚本文件orainstRoot.sh和root.sh，安装完毕後，Oracle是正常启动着的，你可以试一下连接数据库，同时也可以使用浏览器去设置一下Oracle，（url:http:
 //localhost:1158/em/）（Oracle 10g与之前的版本都不一样，使用WEB页的企业管理器来代替以前的C/S版JAVA企业管理器）
[编辑] 第六步，设置自启动
创建自启动脚本 创建 oracledb 脚本到/etc/init.d/oracledb,内容如下
#!/bin/bash
#
# /etc/init.d/oracledb
#
# Run-level Startup script for the Oracle Instance, Listener, and
# Web Interface
export ORACLE_HOME=/opt/ora10
export ORACLE_SID=ora1
export PATH=$ORACLE_HOME/bin:$ORACLE_HOME/Apache/Apache/bin:$PATH
ORA_OWNR="oracle"
# if the executables do not exist -- display error
if [ ! -f $ORACLE_HOME/bin/dbstart -o ! -d $ORACLE_HOME ]
then
echo "[Oracle Error] dbstart command does not exist, quitting..."
exit 1
else
echo "[Oracle Info] dbstart existance check OK"
fi
# depending on parameter -- startup, shutdown, restart
# of the instance and listener or usage display
case "$1" in
start)
# Oracle listener and instance startup
echo "[Oracle Info] Starting oracle"
echo "[Oracle Info] su $ORA_OWNR -c ""$ORACLE_HOME/bin/lsnrctl start"
su $ORA_OWNR -c "$ORACLE_HOME/bin/lsnrctl start"
if [ $? -ne 0 ] 
then
echo "[Oracle Error] listener failed to start"
exit 1
else 
echo "[Oracle Info] listener start OK"
fi
su $ORA_OWNR -c "$ORACLE_HOME/bin/dbstart $ORACLE_HOME"
echo "[Oracel Info] su $ORA_OWNR -c ""$ORACLE_HOME/bin/dbstart $ORACLE_HOME"
if [ $? -ne 0 ] 
then
echo "[Oracle Error] dbstart failed to start"
exit 1
else
echo "[Oracle Info] dbstart OK"
fi
touch /var/lock/oracle
echo "[Oracle Info]su $ORA_OWNR -c ""$ORACLE_HOME/bin/emctl start dbconsole"
su $ORA_OWNR -c "$ORACLE_HOME/bin/emctl start dbconsole"
if [ $? -ne 0 ] 
then
echo "[Oracle Error] dbconsole failed to start"
exit 1
else
echo "[Oracle Info] dbconsole start OK"
fi
echo "[Oracle Info] Starting oracle ended OK"
;;
stop)
# Oracle listener and instance shutdown
echo "[Oracle Info] Shutdown Oracle: "
echo "[Oracle Info] su $ORA_OWNR -c ""$ORACLE_HOME/bin/lsnrctl stop"
su $ORA_OWNR -c "$ORACLE_HOME/bin/lsnrctl stop"
echo "[Oracle Info] su $ORA_OWNR -c ""$ORACLE_HOME/bin/dbshut $ORACLE_HOME"
su $ORA_OWNR -c "$ORACLE_HOME/bin/dbshut $ORACLE_HOME"
rm -f /var/lock/oracle
echo "[Oracle Info]su $ORA_OWNR -c ""$ORACLE_HOME/bin/emctl stop dbconsole"
su $ORA_OWNR -c "$ORACLE_HOME/bin/emctl stop dbconsole"
echo "[Oracle Info] Shutdown Oracle ended OK"
;;
reload|restart)
echo "[Oracle Info]: $1 oracle"
$0 stop
$0 start
;;
*)
echo "[Oracle Error] Unknown context: $1"
echo "[Oracle Info]  Usage: `basename $0` start|stop|restart|reload"
exit 1
esac
exit 0
设置权限,放到启动脚本中去
chmod 755 /etc/init.d/oracledb
update-rc.d oracledb defaults 99
还需要编辑一下/etc/oratab，把其中的N改成Y，不然上述脚本中所调用的$ORACLE_HOME/bin/dbstart在执行时不会自动启动相应的数据库实例（这里的数据库实例是ora1）。文件内容如下：
#
# This file is used by ORACLE utilities.  It is created by root.sh
# and updated by the Database Configuration Assistant when creating
# a database.
# A colon, ':', is used as the field terminator.  A new line terminates
# the entry.  Lines beginning with a pound sign, '#', are comments.
#
# Entries are of the form:
#   $ORACLE_SID:$ORACLE_HOME:<N|Y>:
#
# The first and second fields are the system identifier and home
# directory of the database respectively.  The third filed indicates
# to the dbstart utility that the database should , "Y", or should not,
# "N", be brought up at system boot time.
#
# Multiple entries with the same $ORACLE_SID are not allowed.
#
#
ora1:/opt/ora10:Y
我的 oralce 的安装盘中可能有些问题，所以如果只用上述的脚本是不能启动 listener的，要修改一下： 修改 /opt/ora10/bin/dbstart文件 查找：（本步骤修改后需重新安装？——（不需要重新安装呀Sutra 2010年6月10日 (四) 21:31 (CST)））
# Set this to bring up Oracle Net Listener
ORACLE_HOME_LISTNER=/ade/vikrkuma_new/oracle
将其改为：
# Set this to bring up Oracle Net Listener
ORACLE_HOME_LISTNER=$ORACLE_HOME
设置後，测试一下
/etc/init.d/./oracledb reload
如果一切正常的话，会关闭一次Oracle又再重新打开。
            
