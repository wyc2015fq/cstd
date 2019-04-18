# Oracle安装（linux）总结 - ljx0305的专栏 - CSDN博客
2008年08月05日 18:07:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：683标签：[oracle																[linux																[disk																[优化																[数据库																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=disk&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)
linux下oracle安装： 
Oracle公司宣称在Linux下安装Oracle9i数据库至少要有512MB的内存和至少1GB或者两倍      
内存大小的交换空间，对于系统内存大于2GB的服务器，交换空间可以介于2GB—4GB之间。      
如果是为了在一台仅有256M内存的普通PC机上试用Oracle9i，在分配了1GB左右的交换空间的情况下，也可以正常运行Oracle数      
据库。      
要检查内存空间，登录进入Linux，在命令行方式（bash环境）下执行如下命令：      
grep MemTotal /proc/meminfo      
要检查交换空间，在命令行下执行如下命令：      
grep SwapTotal /proc/meminfo 
1.解压.cpio文件 
  #cpio -idvm < lnx_920_disk1.cpio 
  #cpio -idvm < lnx_920_disk2.cpio 
  #cpio -idvm < lnx_920_disk3.cpio 
  在当前目录下生成Disk1和Disk2,Disk3三个目录. 
2.打补丁 
  unzip oraclerpm.zip 
  #rpm -ivh *.i586.rpm --nodeps --force  
3.建立用户和组 
  #groupadd dba 
  #mkdir /opt/OracleHome 
  #adduser -u 499 -d /opt/OracleHome/ -g dba oracle 
  注:-u 后可以是1-500(系统用户) -g 添加到私有组dba中 
  #chown oracle.dba /opt/OracleHome/ 
  #mkdir /opt/OracleHome/Ora9i 
  #chown oracle.dba /opt/OracleHome/Ora9i 
  #touch /etc/rac_on 
  #mkdir /opt/tmp 
  #chmod 1777 /opt/tmp 
  #cp oracle.bashrc /opt/OraclHome/.bashrc 
4.linux系统优化 
  #dd if=/dev/zero of=/opt/tmpswap bs=1k count=1024000 
  #chmod 600 /opt/tmpswap 
  #mkswap /opt/tmpswap 
  #swapon -v /opt/tmpswap 
  #sysctl -w kernel.sem="250 32000 100 128"  
  #sysctl -w kernel.shmmax=`expr 512 /* 1024 /* 1024`  
  #echo "kernel.sem=250 32000 100 128" >;>; /etc/sysctl.conf 
  #echo "kernel.shmmax=`expr 512 /* 1024 /* 1024`" >;>; /etc/sysctl.conf 
  注:上两行命令执行后请查看/etc/sysctl.conf的变化. 
  #echo 65536 >;/proc/sys/fs/file-max 
  #ulimit -n 65536 
  #ulimit -u 16384 
  注:上三行命令是系统优化,建议也添加到/etc/rc.local文件中,每次启动系统自动优化 
5.安装 
  #xhost localhost 
  #su - oracle  
  #cd /opt/source/Disk1 
  #./runInstaller 
以下是几点注意事项 
选择software only 
然后用命令dbca创建数据库 
1.安装时字体问题的解决: 
   #unset LANG   
   如果执行上述指令不行的话再修改/root/.i18n和/etc/sysconfig/i18n 
   #vi /root/.i18n    
     注:root用户的语言环境,注释掉所有项后添加一行LANG="en_US" 
   #vi /etc/sysconfig/i18n    
     注:系统中新增加用户的语言环境(每增加一个用户把它拷贝到其宿主目录生成.i18n) 
2.安装过程中的报错: 
   前几个按报错信息的指示做, 
   只有最后一个/opt/OraclHome/Oracle9i/ctx/lib/下某一文件错,应修改 
   /opt/OraclHome/Oracle9i/ctx/lib/env_ctx.mk文件 
   在INSO_LINK= 这一行添加一条记录 $(LDLIBFLAG)dl  
3.oracle用户SHELL有问题 
  #userdel oracle 
  #adduser -u 499 -d /opt/OracleHome/ -g dba oracle 
小秘籍: 
 #hdparm -t /dev/hda 查看硬盘读写速度,如只有2M左右,则执行 
 #hdparm -d 1 /dev/hda 开启DMA功能,可把它添加到/etc/rc.local文件中.
引用:http://www.chinaunix.net/jh/19/446039.html
