# ubuntu linux学习笔记 - weixin_33985507的博客 - CSDN博客
2008年09月11日 21:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**1.ssh**
**安装ssh**
`sudo apt-get install ssh`
**or**
```
sudo apt-get install openssh-server
```
**重启ssh**
`sudo /etc/init.d/ssh restart`
ssh-server配置文件位于/etc/ssh/sshd_config
**2.启用root账户**
有两种方式：
a.当你需要 root 的权限时，使用 sudo + linux命令。
b.执行下面的操作：
`sudo passwd root`
此命令将会重新设置 root 的密码，按照提示输入新的密码，并加以确认。
如果你想要禁用 root 帐号，则执行下列命令：
`sudo passwd -l root`
**3.查看IP地址**
`ifconfig`
**4.配置IP地址**
**a.**
```
**ifconfig eth0/*网卡名称*/ 10.0.0.5/*ip地址*/ netmask 255.255.255.0/*掩码*/**
```
**b.**
`**route add default gw 10.0.0.1//网关**`
网卡不能正常启动时sudo dhclient eth0
**5.安装五笔**
```
sudo apt-get install scim-tables-zh
sudo apt-get install fcitx
```
**6.解压命令**
**.tar**
解包：tar xvf FileName.tar
打包：tar cvf FileName.tar DirName
（注：tar是打包，不是压缩！）
---------------------------------------------
**.gz**
解压1：gunzip FileName.gz
解压2：gzip -d FileName.gz
压缩：gzip FileName
**.tar.gz 和 .tgz**
解压：tar zxvf FileName.tar.gz
压缩：tar zcvf FileName.tar.gz DirName
---------------------------------------------
**.bz2**
解压1：bzip2 -d FileName.bz2
解压2：bunzip2 FileName.bz2
压缩： bzip2 -z FileName
**.tar.bz2**
解压：tar jxvf FileName.tar.bz2
压缩：tar jcvf FileName.tar.bz2 DirName
---------------------------------------------
**.bz**
解压1：bzip2 -d FileName.bz
解压2：bunzip2 FileName.bz
压缩：未知
**.tar.bz**
解压：tar jxvf FileName.tar.bz
压缩：未知
---------------------------------------------
**.Z**
解压：uncompress FileName.Z
压缩：compress FileName
**.tar.Z**
解压：tar Zxvf FileName.tar.Z
压缩：tar Zcvf FileName.tar.Z DirName
---------------------------------------------
**.zip**
解压：unzip FileName.zip
压缩：zip FileName.zip DirName
---------------------------------------------
**.rar**
解压：rar x FileName.rar
压缩：rar a FileName.rar DirName
rar请到：[http://www.rarsoft.com/download.htm](http://www.rarsoft.com/download.htm) 下载！
解压后请将rar_static拷贝到/usr/bin目录（其他由$PATH环境变量指定的目录也可以）：
[root@www2 tmp]# cp rar_static /usr/bin/rar
---------------------------------------------
**.lha**
解压：lha -e FileName.lha
压缩：lha -a FileName.lha FileName
lha请到：[http://www.infor.kanazawa-it.ac.jp/~ishii/lhaunix/](http://www.infor.kanazawa-it.ac.jp/~ishii/lhaunix/)下载！
>解压后请将lha拷贝到/usr/bin目录（其他由$PATH环境变量指定的目录也可以）：
[root@www2 tmp]# cp lha /usr/bin/
---------------------------------------------
**.rpm**
解包：rpm2cpio FileName.rpm | cpio -div
---------------------------------------------
**.deb**
解包：ar p FileName.deb data.tar.gz | tar zxf -
---------------------------------------------
**.tar .tgz .tar.gz .tar.Z .tar.bz .tar.bz2 .zip .cpio .rpm .deb .slp .arj .rar .ace .lha .lzh .lzx .lzs .arc .sda .sfx .lnx .zoo .cab .kar .cpt .pit .sit .sea**
解压：sEx x FileName.*
压缩：sEx a FileName.* FileName
sEx只是调用相关程序，本身并无压缩、解压功能，请注意！
sEx请到： [http://sourceforge.net/projects/sex](http://sourceforge.net/projects/sex)下载！
解压后请将sEx拷贝到/usr/bin目录（其他由$PATH环境变量指定的目录也可以）：
[root@www2 tmp]# cp sEx /usr/bin/
**7.其它**
更新文件夹权限
chmod 777 * -R
全部子目录及文件权限改为 777
基础命令sudo 
Ubuntu无法获得排他锁解决方案 
sudo rm /var/cache/apt/archives/lock
sudo rm var/lib/dpkg/lock
更新源
apt-get update 
SVN服务器安装
sudo apt-get install subversion 
sudo apt-get install libapache2-svn 
更多内容,参考[](http://tech.ccidnet.com/art/3089/20060418/510133_1.html)[http://tech.ccidnet.com/art/3089/20060418/510133_1.html](http://tech.ccidnet.com/art/3089/20060418/510133_1.html)
创建目录
sudo mkdir /home/svn 
删除目录
sudo rm -rf dirxxx

