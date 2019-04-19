# 开源VoIP解决方案||inux构建功能强大的voip网络电话服务器 - 三少GG - CSDN博客
2012年11月25日 19:15:19[三少GG](https://me.csdn.net/scut1135)阅读数：4197
# [开源VoIP解决方案业界渐成潮流](http://net.zdnet.com.cn/network_security_zone/2010/1130/1953016.shtml)
**[适用于呼叫传统电话，IP电话]本文摘要**
开源VoIP解决方案已经变得越来越流行，对于企业的语音服务开始显现威力。平台建立在Asterrisk上，FreeSwich提供电话的软交换解决方案，已被证明易用、有效、便于维护。特别引以注意的是回音消除的重要性，回音在任何的电话网络都存在。
　　开源VoIP解决方案已经变得越来越流行，对于企业的语音服务开始显现威力。去年的一篇国外的调研报告指出，开源PBX占据2008年的20%的市场份额，并已每年40%的速度成长。目前已经超越了任何传统的PBX制造商。
平台建立在Asterrisk上，FreeSwich提供电话的软交换解决方案，已被证明易用、有效、便于维护。此外，VoIP的使用上，基于电脑或手机等设备，具有革命性的意义。这样可以确保开源解决方案的完整性。
　　特别引以注意的是回音消除的重要性，回音在任何的电话网络都存在。但是在VoIP，回音更加显著，持续时间较长。开源解决方案能够很好的处理这样问题。
　　另外，硬件的选用也十分关键。为确保开源VoIP的部署，服务器性能是首要问题。CPU和内存的占用率必须保证在60%以下。
# [linux构建功能强大的voip网络电话服务器](http://www.gaojinbo.com/linux%E6%9E%84%E5%BB%BA%E5%8A%9F%E8%83%BD%E5%BC%BA%E5%A4%A7%E7%9A%84voip%E7%BD%91%E7%BB%9C%E7%94%B5%E8%AF%9D%E6%9C%8D%E5%8A%A1%E5%99%A8.html)
Author:高进波 
Time:2009-11-11 
centos5.4下建立一个属于自己的功能强大的VOIP网络电话系统
环境： 
centos 5.4 
asterisk 1.6.1.9 
freePBX 2.6.0RC2
现如今经过几年的信息化建设，IP网络已经走进绝大部分大型企业，多数企业内部均已经建立了内部数据网络。并且实现与INTERNET网互联。在基础网络建设完成以后，为更好的利用现有的IP网络资源，提高企业信息化水平和办公自动化水平，节省语音通话费用，在企业内实现语音数据的统一传输，紧跟下一步电信发展的潮流和趋势，建立基于IP网络的数据语音综合服务将是企业大客户在信息化建设、提高综合竞争能力等方面需要着重考虑问题。
**VOIP系统架构图 **
![voip](http://www.gaojinbo.com/wp-content/uploads/2009/11/voip-300x182.jpg)
1.下载地址： 
cd /usr/src
wget [http://downloads.asterisk.org/pub/telephony/asterisk/releases/asterisk-1.6.1.9.tar.gz](http://downloads.asterisk.org/pub/telephony/asterisk/releases/asterisk-1.6.1.9.tar.gz)
wget [http://downloads.asterisk.org/pub/telephony/dahdi-linux-complete/releases/dahdi-linux-complete-2.2.0.2+2.2.0.tar.gz](http://downloads.asterisk.org/pub/telephony/dahdi-linux-complete/releases/dahdi-linux-complete-2.2.0.2+2.2.0.tar.gz)
wget [http://downloads.asterisk.org/pub/telephony/libpri/releases/libpri-1.4.10.2.tar.gz](http://downloads.asterisk.org/pub/telephony/libpri/releases/libpri-1.4.10.2.tar.gz)
wget [http://downloads.asterisk.org/pub/telephony/asterisk/asterisk-addons-1.6.1-current.tar.gz](http://downloads.asterisk.org/pub/telephony/asterisk/asterisk-addons-1.6.1-current.tar.gz)
wget [http://downloads.sourceforge.net/project/lame/lame/3.98.2/lame-398-2.tar.gz?use_mirror=nchc](http://downloads.sourceforge.net/project/lame/lame/3.98.2/lame-398-2.tar.gz?use_mirror=nchc)
wget [http://downloads.asterisk.org/pub/telephony/sounds/asterisk-extra-sounds-en-gsm-current.tar.gz](http://downloads.asterisk.org/pub/telephony/sounds/asterisk-extra-sounds-en-gsm-current.tar.gz)
wget [http://mirror.freepbx.org/freepbx-2.6.0RC2.tar.gz](http://mirror.freepbx.org/freepbx-2.6.0RC2.tar.gz)
Sounds 
$ make menuselect
2.安装依赖包
yum install e2fsprogs-devel keyutils-libs-devel krb5-devel libogg libselinux-devel libsepol-devel libxml2-devel libtiff-devel gmp php-pear php-pear-DB php-gd php-mysql php-pdo kernel-devel ncurses-devel audiofile-devel libogg-devel openssl-devel mysql-devel
 zlib-devel perl-DateManip sendmail-cf sox php-process-5.2.10
cd /usr/src 
tar zxvf lame-398-2.tar.gz 
cd lame-398-2 
./configure 
make && make install
3.安装asterisk和freePBX
cd /usr/src
tar xvzf dahdi-linux-complete-2.2.0.2+2.2.0.tar.gz 
tar xvzf asterisk-1.6.1.9.tar.gz 
tar xvzf libpri-1.4.10.2.tar.gz 
tar xvzf asterisk-addons-1.6.1-current.tar.gz 
tar xvzf freepbx-2.6.0RC2.tar.gz
mkdir -p /var/lib/asterisk/sounds 
cd /var/lib/asterisk/sounds 
tar xvzf /usr/src/asterisk-extra-sounds-en-gsm-current.tar.gz
cd /usr/src/dahdi-linux-complete-2.2.0.2+2.2.0 
make && make install 
make config
cd /usr/src/libpri-1.4.10.2 
make clean 
make && make install
cd /usr/src/asterisk-1.6.1.9 
useradd -c "Asterisk PBX" -d /var/lib/asterisk asterisk 
mkdir /var/run/asterisk 
mkdir /var/log/asterisk 
chown -R asterisk:asterisk /var/run/asterisk 
chown -R asterisk:asterisk /var/log/asterisk 
chown -R asterisk:asterisk /var/lib/php/session/ 
./configure 
make && make install 
make samples
cd /usr/src/freepbx-2.6.0RC2 
mysqladmin -uroot -p create asterisk 
mysqladmin -uroot -p create asteriskcdrdb 
mysql -uroot -p asterisk < SQL/newinstall.sql 
mysql -uroot -p asteriskcdrdb < SQL/cdr_mysql_table.sql
mysql -uroot -p 
GRANT ALL PRIVILEGES ON asteriskcdrdb.* TO asteriskuser@localhost IDENTIFIED BY ‘SOMEPASSWORD’;
GRANT ALL PRIVILEGES ON asterisk.* TO asteriskuser@localhost IDENTIFIED BY ‘SOMEPASSWORD’;
flush privileges; 
\q
cd /usr/src/asterisk-addons-1.6.1.1 
./configure 
make && make install
cd /usr/src/freepbx-2.5.2 
./start_asterisk start 
./install_amp –username=asteriskuser –password=SOMEPASSWORD
访问web进行配置
完成！
