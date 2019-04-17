# Linux操作系统Ubuntu部署Oracle篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月22日 16:30:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2311








ubuntu下安装oracle client：

1.下载安装包

命令：sudo wget [http://oss.oracle.com/debian/dists/unstable/non-free/binary-i386/oracle-xe-universal_10.2.0.1-1.1_i386.deb](http://oss.oracle.com/debian/dists/unstable/non-free/binary-i386/oracle-xe-univeral_10.2.0.1-1.1_i386.deb)

2.安装：以root身份执行

命令：sudo dpkg -i oracle-xe-universal_10.2.0.1-1.1_i386.deb

自动建立的oracle用户，以及自动修改了sysctl.conf文件，安装过程出现下面两个问题的处理：

1)问题一：交换分区不够，按照下面步骤提高交换区大小

  a.查看swap分区大小命令:free -m

  b.创建一个2G大小的Swap文件命令：

    mkdir /swapfile 

    cd swapfile

    sudo dd if=/dev/zero of=swap bs=1024 count=2000000

  c./swapfile下生成的文件转换成swap文件：sudo mkswap -f swap

  d.激活swap文件：sudo swapon swap

  e.查看交换区大小是否提高:free -m

  f.卸载swap文件：sudo swapoff swap

2)问题二：提示未安装软件包libaio\libaio1

  a.执行安装检查命令：apt-get -f install

  b.提示安装libaio，选择y继续；

  c.完成后，继续执行：sudo dpkg -i 安装命令；

3)启动时提示用户不具备dba组权限，到用户管理分配dba组给用户即可；

3.配置

1)修改/etc/environment文件,末尾加

　ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server

  LD_LIBRARY_PATH=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib

  TNS_ADMIN=/usr/lib/oracle

　PATH加上ORACLE_HOME/bin和LD_LIBRARY_PATH路径

2)配置TNS：sudo vi /usr/lib/oracle/tnsnames.ora添加连接内容，如

　　orcl =

　　(DESCRIPTION =

　　  (ADDRESS = (PROTOCOL = TCP)(HOST = 127.0.0.1)(PORT = 1521))

　　  (CONNECT_DATA =

　　    (SERVER = DEDICATED)

　　    (SERVICE_NAME = orcl)　

     )

　　)

3)运行sqlplus测试

　　$ sqlplus /nolog






