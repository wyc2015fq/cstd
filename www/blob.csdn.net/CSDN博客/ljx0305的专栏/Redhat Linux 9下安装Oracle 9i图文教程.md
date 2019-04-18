# Redhat Linux 9下安装Oracle 9i图文教程 - ljx0305的专栏 - CSDN博客
2008年08月02日 22:30:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1723标签：[oracle																[linux																[redhat																[disk																[数据库																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=disk&t=blog)](https://so.csdn.net/so/search/s.do?q=redhat&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)
在Liunx下安装Oracle数据库软件对于初学者来说是一件不容易的事，本菜鸟在折腾了许久后安装成功，中间过程不敢独享，公布出来分享一下．有问题的地方还请各位大侠指正． 
准备工作
　　系统版本：Redhat Linux Enterprise 4
　　内核版本：2.6.9-5
　　Oracle版本：9.2.4
　　内存容量：512M
　　硬盘容量：16G
一：安装Linux系统，放入Linux第一张光盘，设置BIOS从光盘引导，启动后进入如下界面： 
![](http://lh4.google.com/idwaring/Rx8LEnYGVqI/AAAAAAAAACQ/PQML0vQJ8ow/s400/Image00000.jpg)
　　按回车键进行图文方式安装．第二个选项是输入"linux test"进行文本方式安装． 
![](http://lh6.google.com/idwaring/Rx8LFHYGVrI/AAAAAAAAACc/2XefPRiJAcg/s400/Image00001.jpg)
　　是否对光盘进行测试，如是介质正常的话，建议选＂skip＂ 
![](http://lh3.google.com/idwaring/Rx8LFXYGVsI/AAAAAAAAACk/YEN17zTnJRo/s400/Image00002.jpg)
　　没什么好说的，＂Next＂ 
![](http://lh5.google.com/idwaring/Rx8LF3YGVtI/AAAAAAAAACs/oz0x4D95tSc/s400/Image00003.jpg)
　　选择系统的语言，可以选＂简体中文＂，但是在以后的安装中可能会出现乱码的现象，为了方便，选＂English＂
![](http://lh6.google.com/idwaring/Rx8LGHYGVuI/AAAAAAAAAC0/gN0tDkK3Vng/s400/Image00004.jpg)
　　配置键盘，一般选＂U.S.English＂ 
![](http://lh4.google.com/idwaring/Rx8LGnYGVvI/AAAAAAAAAC8/VlHQ3hjsIFQ/s400/Image00005.jpg)
　　系统的分区选项，这里选手动分区＂Manually partition with Disk Druid＂,因为我们要将Oracle安装在一个较大的分区内，如果自动分区的话，可能会出现空间不够的情况． 
![](http://lh6.google.com/idwaring/Rx8LHHYGVwI/AAAAAAAAADE/053dJZqayJQ/s400/Image00006.jpg)
![](http://lh3.google.com/idwaring/Rx8LHXYGVxI/AAAAAAAAADM/aiVBIQSZj0Q/s400/Image00007.jpg)
　　现在进入了具体的分区选项，有几点要注意的，＂/＂分区是必须的，因为所有的分区就是从它开始，类似Windows一定要有一个主的活动分区一样．我计划Oracle这个软件安装在＂/usr＂下，这个分区差不多要有8G的空间才行，我的解压都是在这下面进行，如果进行光盘安装就不需这么大了，＂swap＂交换分区设成1G．其它具体的设置如下： 
![](http://lh5.google.com/idwaring/Rx8LH3YGVyI/AAAAAAAAADU/yA8zC3JvTeU/s400/Image00008.jpg)
![](http://lh6.google.com/idwaring/Rx8LJHYGV0I/AAAAAAAAADk/UU5T3VXBa7I/s400/Image00010.jpg)
/boot: 300M/: 1500Mswap:1G　　/home: 2G
/tmp: 1G/usr:10.5G
![](http://lh4.google.com/idwaring/Rx8LJnYGV1I/AAAAAAAAADs/BZZ2wqLpDzw/s400/Image00011.jpg)
　　是否设置启动密码，一般不选，直接＂Next＂ 
![](http://lh6.google.com/idwaring/Rx8LKHYGV2I/AAAAAAAAAD0/QSvrgpOA7B0/s400/Image00012.jpg)
　　设置网卡，最好是手动指定，这里我选的是＂DHCP＂，可以看到我这里有两块网卡． 
![](http://lh4.google.com/idwaring/Rx8LKnYGV3I/AAAAAAAAAD8/CxsybExlrIM/s400/Image00013.jpg)![](http://lh6.google.com/idwaring/Rx8LLHYGV4I/AAAAAAAAAEE/mVnpDaHPNms/s400/Image00014.jpg)
　　是否启用防火墙及Linux的语法检测，为了方便起见，都设成不启用，如上图示．不过各位如果有安全需要，可以按需开启． 
![](http://lh5.google.com/idwaring/Rx8LL3YGV5I/AAAAAAAAAEM/sAetlLlO7RU/s400/Image00015.jpg)
　　选择系统所支持的语言，选上简体和繁体中文，这样在显示这些字体的文件时不会出现乱码的情况． 
![](http://lh3.google.com/idwaring/Rx8LMXYGV6I/AAAAAAAAAEU/txQlHRwGOR8/s400/Image00016.jpg)
　　选择所在的时区，这里选的是亚洲/上海＂Asia/Shanghai＂ 
![](http://lh5.google.com/idwaring/Rx8LM3YGV7I/AAAAAAAAAEc/nyKZhGNJRUQ/s400/Image00017.jpg)
　　设置Linux下管理员root的口令． 
![](http://lh3.google.com/idwaring/Rx8LNXYGV8I/AAAAAAAAAEk/sKr0Jatq6Og/s400/Image00018.jpg)
　　选择自定义安装软件包＂Customize software packages to be installed＂ 
![](http://lh5.google.com/idwaring/Rx8LN3YGV9I/AAAAAAAAAEs/lbZUbE2LD9A/s400/Image00019.jpg)
![](http://lh3.google.com/idwaring/Rx8LOXYGV-I/AAAAAAAAAE0/6t0L1pMqa34/s400/Image00020.jpg)
![](http://lh5.google.com/idwaring/Rx8LO3YGV_I/AAAAAAAAAE8/us_xafBDuqc/s400/Image00021.jpg)
![](http://lh3.google.com/idwaring/Rx8LPXYGWAI/AAAAAAAAAFE/MbluTiTNrIo/s400/Image00022.jpg)
![](http://lh5.google.com/idwaring/Rx8LP3YGWBI/AAAAAAAAAFM/iEuYPn6tx04/s400/Image00023.jpg)
![](http://lh6.google.com/idwaring/Rx8LQHYGWCI/AAAAAAAAAFU/j2Lu-_ZUSWg/s400/Image00024.jpg)
　　去掉＂GNOME＂选＂KDE＂这个除了个人喜好外，还有一点是如果有远程进行图文安装的话，KDE比较稳定，还有一点是软件开发工具要装，不然后面安装会重新安装组件，具体的软件包选择可以参考上面的图片．
前期工作：
　　接上期的Linux下oracle 9i图文安装一（[](http://waringid.blog.51cto.com/65148/47655)[http://waringid.blog.51cto.com/65148/47655](http://waringid.blog.51cto.com/65148/47655)），下面进行Oracle安装前期准备中的第一点，完成Linux的安装． 
　　在安装中要求准备光盘． 
![](http://lh4.google.com/idwaring/Rx8LQnYGWDI/AAAAAAAAAFc/OGFpvxwV66A/s400/Image00025.jpg)
　　安装过程中按要求更换光盘 
![](http://lh6.google.com/idwaring/Rx8LRHYGWEI/AAAAAAAAAFk/y_OmFuq7h8U/s400/Image00026.jpg)
![](http://lh3.google.com/idwaring/Rx8LRXYGWFI/AAAAAAAAAFs/Md6jIEIiZLw/s400/Image00027.jpg)
![](http://lh5.google.com/idwaring/Rx8LR3YGWGI/AAAAAAAAAF0/j66FzuP-vds/s400/Image00028.jpg)
　　完成安装，重启计算机 
![](http://lh6.google.com/idwaring/Rx8LSHYGWHI/AAAAAAAAAF8/gel4jEsxTgU/s400/Image00029.jpg)
　　登陆初始界面 
![](http://lh3.google.com/idwaring/Rx8LSXYGWII/AAAAAAAAAGE/G15tEQlIN3o/s400/Image00030.jpg)
![](http://lh5.google.com/idwaring/Rx8LS3YGWJI/AAAAAAAAAGM/ISiZGbhArGM/s400/Image00031.jpg)
　　设置日期和时间 
![](http://lh6.google.com/idwaring/Rx8LTHYGWKI/AAAAAAAAAGU/jC4QVNIhlVQ/s400/Image00032.jpg)
　　显示设置 
![](http://lh3.google.com/idwaring/Rx8LTXYGWLI/AAAAAAAAAGc/YME8L7VaF7c/s400/Image00033.jpg)
　　是否为Redhat的正版授权用户，如果不是请按图中显示进行选择．当然也可以注册一个用户，但是如果不是购买服务的用户这样没什么意义． 
![](http://lh5.google.com/idwaring/Rx8LT3YGWMI/AAAAAAAAAGk/4lxpCeRQ3QY/s400/Image00034.jpg)
![](http://lh6.google.com/idwaring/Rx8LUHYGWNI/AAAAAAAAAGs/G1AFurnGD8g/s400/Image00035.jpg)
　　设置系统用户 
![](http://lh3.google.com/idwaring/Rx8LUXYGWOI/AAAAAAAAAG0/-fkzxyTy1lU/s400/Image00036.jpg)
　　检测到声卡 
![](http://lh5.google.com/idwaring/Rx8LU3YGWPI/AAAAAAAAAG8/UOuungH40a0/s400/Image00037.jpg)
　　是否重新增加软件包，完成安装 
![](http://lh6.google.com/idwaring/Rx8LVHYGWQI/AAAAAAAAAHE/g11zxHliCiY/s400/Image00038.jpg)
![](http://lh3.google.com/idwaring/Rx8LVXYGWRI/AAAAAAAAAHM/jlyJ-cyUsXY/s400/Image00039.jpg)
　　登陆界面 
![](http://lh4.google.com/idwaring/Rx8LVnYGWSI/AAAAAAAAAHU/5QaO0XR7ahA/s400/Image00040.jpg)
![](http://lh5.google.com/idwaring/Rx8LV3YGWTI/AAAAAAAAAHc/GkUmYXM_2uc/s400/Image00041.jpg)
　　查看网卡是否正常及网络地址 
su - 
# ifconfig -a |more
![](http://lh4.google.com/idwaring/Rx8LWnYGWUI/AAAAAAAAAHk/GmSvQayhEsA/s400/Image00042.jpg)
　　查看系统的主机名，要设置好主机名，不然后面在其它系统上安装Oracle的客户端的时候会有问题，这里的主机名是redhat.test.com 
　　su - 
　　# cat /etc/hosts
![](http://lh5.google.com/idwaring/Rx8LW3YGWVI/AAAAAAAAAHs/Ny4UKGD12Qc/s400/Image00043.jpg)
前期工作：
　　前面已经讲到了安装Linux系统及加载相应的软件包，参考地址如下：
Linux下oracle 9i图文安装一（[](http://waringid.blog.51cto.com/65148/47655)[http://waringid.blog.51cto.com/65148/47655](http://waringid.blog.51cto.com/65148/47655)），
Linux下oracle 9i图文安装二（[](http://waringid.blog.51cto.com/65148/47759)[http://waringid.blog.51cto.com/65148/47759](http://waringid.blog.51cto.com/65148/47759)）这一节将配置Oracle安装的环境．其中包括安装必须的软件包，创建相应的用户和组，增加相应的系统参数，编辑必须的环境参数． 本节中需在计算机中输入的数据将以蓝色字体标记．
一：创建相应的用户（以root用户登陆） 
groupadd dba 
　　useradd -g dba -d /usr/Oracle -s /bin/bash oracle 
　　passwd oracle
注意：在图中没有＂passwd oracle＂这一条，因为忘记截图了，这个是必须的，因为后面用oracle这个用户登陆如果没有密码会登陆不了．　　 
![](http://lh6.google.com/idwaring/RyF4gPz0uUI/AAAAAAAAAIM/hu0f7t6D87Q/s400/Image00000.jpg)
二：设置系统参数：
　　编辑/etc/sysctl.conf文件，增加以下内容 
kernel.sem=250 32000 100 128 
　　kernel.shmmax=4294967295 
　　kernel.shmmni=128 
　　kernel.msgmnb=65536 
　　kernel.msgmni=2878 
　　fs.file-max=65536 
　　net.ipv4.ip_local_port_range=1024 65000
![](http://lh5.google.com/idwaring/RyF4h_z0uXI/AAAAAAAAAIk/5_CddPWPdgo/s400/Image00003.jpg)
　　输入命令"sysctl -p"或重启电脑使设置生效 
sysctl -p
![](http://lh3.google.com/idwaring/RyF4ifz0uYI/AAAAAAAAAIs/podlL5qupGY/s400/Image00004.jpg)
　　编辑/etc/security/limits.conf文件，增加以下内容 
* - nproc 16384 
　　* - nofile 16384
![](http://lh3.google.com/idwaring/RyGDmfz0uvI/AAAAAAAAAMU/Ah9mubFQat0/s400/Image00000.jpg)
![](http://lh6.google.com/idwaring/RyGDmPz0uuI/AAAAAAAAAMM/Zqgibutqy0k/s400/Image00001.jpg)
三：创建应用文件夹及设置相应的权限
mkdir -p /usr/Oracle/product/9.2.0 
　　chown -R oracle.dba /usr/Oracle/product/9.2.0 
　　chmod -R 777 /usr/Oracle/product/9.2.0 
![](http://lh4.google.com/idwaring/RyF4ivz0uZI/AAAAAAAAAI0/oF6Txmmat94/s400/Image00005.jpg)
　　注销＂root＂用户，以＂oracle＂用户登陆 
![](http://lh5.google.com/idwaring/RyF4i_z0uaI/AAAAAAAAAI8/kbrzz4AIWac/s400/Image00006.jpg)
![](http://lh6.google.com/idwaring/RyF4jPz0ubI/AAAAAAAAAJE/zOJ544j1-l8/s400/Image00007.jpg)
四：编辑.bash_profile文件（登陆进来后本地文件应是/usr/Oracle） 
　　具体文见附件． 
![](http://lh5.google.com/idwaring/RyF4k_z0ueI/AAAAAAAAAJc/Fnxpne_kEnE/s400/Image00010.jpg)
　　加载配置文件及查看已生效的配置 
source .bash_profile 
　　set |more
![](http://lh3.google.com/idwaring/RyF4lfz0ufI/AAAAAAAAAJk/7bBb2jtkudU/s400/Image00011.jpg)![](http://lh6.google.com/idwaring/RyF4mPz0ugI/AAAAAAAAAJs/7Dcm8rwNG5M/s400/Image00012.jpg)
五：下载和安装JAVA[](http://java.sun.com/products/archive/index.html)[http://java.sun.com/products/archive/index.html](http://java.sun.com/products/archive/index.html)
　　将下载下来的文件放至/usr下面． 
su - 
　　chmod +x j2re-1_3_1_19-linux-i586.bin 
　　./jere-1_3_1_19-linux-i586.bin 
　　mv jre1.3.1_19 /usr/jre1.3.1 
　　java -version 　
![](http://lh4.google.com/idwaring/RyF4mvz0uhI/AAAAAAAAAJ0/n8sVOuRh40s/s400/Image00013.jpg)
![](http://lh5.google.com/idwaring/RyF4n_z0ujI/AAAAAAAAAKE/7tQEkz-U6bY/s400/Image00015.jpg)
![](http://lh4.google.com/idwaring/RyF4ovz0ukI/AAAAAAAAAKM/s6akNowh-P0/s400/Image00016.jpg)
![](http://lh5.google.com/idwaring/RyF4o_z0ulI/AAAAAAAAAKU/9N0P3BSskKA/s400/Image00017.jpg)
六： 查询所需的软件包
su - 
　　 rpm -q compat-db compat-gcc-32 compat-gcc-32-c++ compat-libcom_err compat-libcwaitcompat-libgcc-296 compat-libstdc++-296 compat-libstdc++-33 gcc gcc-c++ glibc glibc-common glibc-devel glibc-headers glibc-kernheaders libgcc make 
![](http://lh4.google.com/idwaring/RyF4pvz0umI/AAAAAAAAAKc/iBMzsmbFhBo/s400/Image00018.jpg)
　　安装缺少的软件包，具体的安装各位自已搞定了，有很大一部份系统的安装光盘上有，给几个下载地址．[](http://ftp.chinalinuxpub.com/redhat/enterprise/os/i386/RedHat/RPMS/)[http://ftp.chinalinuxpub.com/redhat/enterprise/os/i386/RedHat/RPMS/](http://ftp.chinalinuxpub.com/redhat/enterprise/os/i386/RedHat/RPMS/)；[](http://mirror.centos.org/centos/4/updates/i386/RPMS/)[http://mirror.centos.org/centos/4/updates/i386/RPMS/](http://mirror.centos.org/centos/4/updates/i386/RPMS/)；[](http://oss.oracle.com/projects/compat-oracle/files/RedHat/)[http://oss.oracle.com/projects/compat-oracle/files/RedHat/](http://oss.oracle.com/projects/compat-oracle/files/RedHat/)；如果实在不行的话可以找我联系，其中除了上述的软件外，还要安装 compat-oracle-rhel4-1.0-5.i386.rpm．
![](http://lh3.google.com/idwaring/RyF4qfz0unI/AAAAAAAAAKk/-LwDqVD9b7Q/s400/Image00019.jpg)
![](http://lh5.google.com/idwaring/RyF4q_z0uoI/AAAAAAAAAKs/STsSeFgtcxo/s400/Image00020.jpg)
![](http://lh4.google.com/idwaring/RyF4rvz0upI/AAAAAAAAAK0/iPnpzhjLBUM/s400/Image00021.jpg)
![](http://lh6.google.com/idwaring/RyF4sPz0uqI/AAAAAAAAAK8/TP3afLbGZMo/s400/Image00022.jpg)
![](http://lh5.google.com/idwaring/RyF4s_z0urI/AAAAAAAAALE/MYUwoBYZSGg/s400/Image00023.jpg)
![](http://lh4.google.com/idwaring/RyF4tvz0usI/AAAAAAAAALM/dRwU9kE4gPA/s400/Image00024.jpg)
七：更改GCC
su - 
　　cd /usr/bin 
　　mv ./gcc ./gcc34 
　　mv ./gcc32 ./gcc
![](http://lh6.google.com/idwaring/RyF4uPz0utI/AAAAAAAAALU/lw5ziSodYrg/s400/Image00025.jpg)
前期工作：
　　前面已经讲到了安装Linux系统及加载相应的软件包，参考地址如下：
Linux下oracle 9i图文安装一
（[](http://waringid.blog.51cto.com/65148/47655)[http://waringid.blog.51cto.com/65148/47655](http://waringid.blog.51cto.com/65148/47655)）， 
Linux下oracle 9i图文安装二 
（[](http://waringid.blog.51cto.com/65148/47759)[http://waringid.blog.51cto.com/65148/47759](http://waringid.blog.51cto.com/65148/47759)） 
Linux下oracle 9i图文安装三 
（[](http://waringid.blog.51cto.com/65148/47849)[http://waringid.blog.51cto.com/65148/47849](http://waringid.blog.51cto.com/65148/47849)） 
这一节将将开始Oracle的安装．其中包括更改Oracle默认的JRE的路径，解决＂Error:You do not have sufficient provileges to write to the specified path. in component Database Configuration Assistant 9.2.0.1.0,Installation cannot continue for this component.＂的错误及安装中必须的步骤． 本节中需在计算机中输入的数据将以蓝色字体标记．
一：下载和解压Oracle软件包，将文件存到＂/usr/Oracle＂
[](http://www.oracle.com/technology/software/products/oracle9i/index.html)[http://www.oracle.com/technology/software/products/oracle9i/index.html](http://www.oracle.com/technology/software/products/oracle9i/index.html)
su oracle 
　　gunzip ship_9204_linux_disk1.cpio.gz 
　　gunzip ship_9204_linux_disk2.cpio.gz 
　　gunzip ship_9204_linux_disk3.cpio.gz 
　　cpio -idmv < ship_9204_linux_disk1.cpio 
　　cpio -idmv < ship_9204_linux_disk2.cpio 
　　cpio -idmv < ship_9204_linux_disk3.cpio
![](http://lh6.google.com/idwaring/RyLWb_z0uxI/AAAAAAAAAM4/a6JpGlU7CFY/s400/Image00001.jpg)
二：更改默认的jre路径
su oracle 
　　vi /usr/Oracle/Disk1/install/linux/oraparam.ini 
　　JRE_LOCATION=/usr/jre1.3.1
![](http://lh4.google.com/idwaring/RyLWcfz0uyI/AAAAAAAAANA/bGhMlK2puwY/s400/Image00002.jpg)
三：开始安装
su oracle 
　　cd /usr/Oracle/Disk1 
　　./runInstaller
![](http://lh3.google.com/idwaring/RyLWdPz0uzI/AAAAAAAAANI/fah7zqB_rxw/s400/Image00003.jpg)
![](http://lh5.google.com/idwaring/RyLWdvz0u0I/AAAAAAAAANQ/IO91t8IHwuk/s400/Image00004.jpg)
![](http://lh3.google.com/idwaring/RyLWePz0u1I/AAAAAAAAANY/boX7kZasBFI/s400/Image00005.jpg)
![](http://lh5.google.com/idwaring/RyLWevz0u2I/AAAAAAAAANg/oFMC3l9kHKc/s400/Image00006.jpg)
四：进入/tmp运行安装时必须的脚本
　　su - 
　　cd /tmp 
　　sh orainstRoot.sh
![](http://lh6.google.com/idwaring/RyLWe_z0u3I/AAAAAAAAANo/dyeTsT7o-Yo/s400/Image00007.jpg)
![](http://lh5.google.com/idwaring/RyLWfvz0u4I/AAAAAAAAANw/bcGlFPpY2zY/s400/Image00008.jpg)
![](http://lh3.google.com/idwaring/RyLWgPz0u5I/AAAAAAAAAN4/gU7a2zKSXL8/s400/Image00009.jpg)
五：继续安装
![](http://lh5.google.com/idwaring/RyLWgvz0u6I/AAAAAAAAAOA/rWo63zVGGKc/s400/Image00010.jpg)
![](http://lh3.google.com/idwaring/RyLWhPz0u7I/AAAAAAAAAOI/ufuWiPwjB5c/s400/Image00011.jpg)
![](http://lh6.google.com/idwaring/RyLWh_z0u8I/AAAAAAAAAOQ/oF3U6YySGLg/s400/Image00012.jpg)
![](http://lh4.google.com/idwaring/RyLWifz0u9I/AAAAAAAAAOY/FOhJLEvnYzU/s400/Image00013.jpg)
六：解决下一个画面中可能出现的错误＂Error:You do not have sufficient provileges to write to the specified path. in component Database Configuration Assistant 9.2.0.1.0,Installation cannot continue for this component.＂
su - 
　　cd / 
　　mkdir oradata 
　　chown oracle.dab /oradata 
![](http://lh6.google.com/idwaring/RyLWi_z0u-I/AAAAAAAAAOg/xmE_rUocyc0/s400/Image00014.jpg)
![](http://lh5.google.com/idwaring/RyLWjvz0u_I/AAAAAAAAAOo/QqzPz2NR3W0/s400/Image00015.jpg)
![](http://lh3.google.com/idwaring/RyLWkPz0vAI/AAAAAAAAAOw/9o18nyoYQNI/s400/Image00016.jpg)
![](http://lh4.google.com/idwaring/RyLWlfz0vCI/AAAAAAAAAPA/D4SnhjQysRg/s400/Image00018.jpg)
![](http://lh6.google.com/idwaring/RyLWl_z0vDI/AAAAAAAAAPI/QHp3VvtDHJQ/s400/Image00019.jpg)
这里有一点不明白，为什么数据库会默认安装在＂/oradata＂下，不知哪位大侠知道怎样更改这里的设置，还请提示一二．
前期工作：
　　前面已经讲到了安装Linux系统及加载相应的软件包，参考地址如下：
Linux下oracle 9i图文安装一
（[](http://waringid.blog.51cto.com/65148/47655)[http://waringid.blog.51cto.com/65148/47655](http://waringid.blog.51cto.com/65148/47655)）， 
Linux下oracle 9i图文安装二 
（[](http://waringid.blog.51cto.com/65148/47759)[http://waringid.blog.51cto.com/65148/47759](http://waringid.blog.51cto.com/65148/47759)） 
Linux下oracle 9i图文安装三 
（[](http://waringid.blog.51cto.com/65148/47849)[http://waringid.blog.51cto.com/65148/47849](http://waringid.blog.51cto.com/65148/47849)） 
Linux下oracle 9i图文安装四 
（[](http://waringid.blog.51cto.com/65148/47973)[http://waringid.blog.51cto.com/65148/47973](http://waringid.blog.51cto.com/65148/47973)） 
这一节将将继续Oracle的安装．其中包括重新链接Oracle默认的JRE的路径，解决＂Configuration Tools＂的错误及安装中必须的步骤． 本节中需在计算机中输入的数据将以蓝色字体标记．
一：运行root.sh
　　su - 
　　cd /usr/Oracle/product/9.2.0 
　　sh root.sh
![](http://lh4.google.com/idwaring/RyLWmfz0vEI/AAAAAAAAAPQ/e7Fu9JdF0cA/s400/Image00020.jpg)
![](http://lh6.google.com/idwaring/RyLWm_z0vFI/AAAAAAAAAPY/-9_eSFKBWNQ/s400/Image00021.jpg)
![](http://lh5.google.com/idwaring/RyLWnvz0vGI/AAAAAAAAAPk/ZV4O3cVp804/s400/Image00022.jpg)
二：解决＂Configuration Tools＂错误，重新链接JAVA
su oracle 
　　export LD_ASSUME_KERNEL=2.4.19 
　　cd /usr/Oracle/product/9.2.0 
　　rm JRE 
　　ln -s /usr/jre1.3.1 JRE 
　　su - 
　　cd /usr/Oracle/product/9.2.0/JRE/bin 
　　ln -s java jre 
　　cd i386/native_threads 
　　ln -s java jre
![](http://lh4.google.com/idwaring/RyLWofz0vHI/AAAAAAAAAPs/rBJ6Gb4crZQ/s400/Image00023.jpg)
![](http://lh6.google.com/idwaring/RyLWo_z0vII/AAAAAAAAAP0/SlfLT3o9Kqo/s400/Image00024.jpg)
![](http://lh3.google.com/idwaring/RyLWqPz0vKI/AAAAAAAAAQE/AEqzxASsfOg/s400/Image00026.jpg)
　　然后重新选＂Retry＂重新依次安装工具。
![](http://lh4.google.com/idwaring/RyLWpfz0vJI/AAAAAAAAAP8/DJhBVMWpIfc/s400/Image00025.jpg)
![](http://lh5.google.com/idwaring/RyLWqvz0vLI/AAAAAAAAAQM/N0QUValj_c8/s400/Image00027.jpg)
![](http://lh3.google.com/idwaring/RyLWrPz0vMI/AAAAAAAAAQU/GU-rQdeboIY/s400/Image00028.jpg)
![](http://lh6.google.com/idwaring/RyLWr_z0vNI/AAAAAAAAAQc/UVNRTxfYreo/s400/Image00029.jpg)
![](http://lh4.google.com/idwaring/RyLWsfz0vOI/AAAAAAAAAQk/ruOtYwMZv54/s400/Image00030.jpg)
![](http://lh6.google.com/idwaring/RyLWs_z0vPI/AAAAAAAAAQs/fanCd-CW_gQ/s400/Image00031.jpg)
![](http://lh5.google.com/idwaring/RyLWtvz0vQI/AAAAAAAAAQ0/mSWjeiajSaM/s400/Image00032.jpg)
![](http://lh4.google.com/idwaring/RyLWufz0vRI/AAAAAAAAAQ8/lsqcULMa7Iw/s400/Image00033.jpg)
![](http://lh6.google.com/idwaring/RyLWu_z0vSI/AAAAAAAAARE/-p2NYcZLq1c/s400/Image00034.jpg)
![](http://lh4.google.com/idwaring/RyLWvfz0vTI/AAAAAAAAARM/i1qtfXJcGXI/s400/Image00035.jpg)
![](http://lh3.google.com/idwaring/RyLWwPz0vUI/AAAAAAAAARU/uEQLKzdI8sg/s400/Image00036.jpg)
![](http://lh5.google.com/idwaring/RyLWwvz0vVI/AAAAAAAAARc/aNt_YR5JG5w/s400/Image00037.jpg)
![](http://lh3.google.com/idwaring/RyLWxPz0vWI/AAAAAAAAARk/RvIAj4RIGaQ/s400/Image00038.jpg)
![](http://lh5.google.com/idwaring/RyLWxvz0vXI/AAAAAAAAARs/ia_akoPxqnM/s400/Image00039.jpg)
![](http://lh3.google.com/idwaring/RyLWyPz0vYI/AAAAAAAAAR0/xUBN7Qwf5Mg/s400/Image00040.jpg)
![](http://lh5.google.com/idwaring/RyLWyvz0vZI/AAAAAAAAAR8/lVRjLrRSEac/s400/Image00041.jpg)
![](http://lh3.google.com/idwaring/RyLWzPz0vaI/AAAAAAAAASE/5QgunxX2zD8/s400/Image00042.jpg)
![](http://lh5.google.com/idwaring/RyLWzvz0vbI/AAAAAAAAASM/XB4LffMcdR8/s400/Image00043.jpg)
前期工作：
　　前面已经讲到了安装Linux系统及加载相应的软件包，参考地址如下： 
Linux下oracle 9i图文安装一 
（[](http://waringid.blog.51cto.com/65148/47655)[http://waringid.blog.51cto.com/65148/47655](http://waringid.blog.51cto.com/65148/47655)）， 
Linux下oracle 9i图文安装二 
（[](http://waringid.blog.51cto.com/65148/47759)[http://waringid.blog.51cto.com/65148/47759](http://waringid.blog.51cto.com/65148/47759)） 
Linux下oracle 9i图文安装三 
（[](http://waringid.blog.51cto.com/65148/47849)[http://waringid.blog.51cto.com/65148/47849](http://waringid.blog.51cto.com/65148/47849)） 
Linux下oracle 9i图文安装四 
（[](http://waringid.blog.51cto.com/65148/47973)[http://waringid.blog.51cto.com/65148/47973](http://waringid.blog.51cto.com/65148/47973)） 
Linux下oracle 9i图文安装五 
（[](http://waringid.blog.51cto.com/65148/47985)[http://waringid.blog.51cto.com/65148/47985](http://waringid.blog.51cto.com/65148/47985)） 
这一节将将完成Oracle的安装．其中包括重定向数据库的存放路径及打上安装完成后的几个必须的补丁；再加上在其它机器的客户端上远程联接Oracle． 本节中需在计算机中输入的数据将以蓝色字体标记．
一：继续安装Oracle
　　在安装中如果出现"ora-00604"，＂ora-01653＂的错误，是因为默认的SYSTEM表存放在＂/oradata＂下，空间不够的原因，本例中把它重新定位到＂/usr/Oracle＂下就没问题了．
![](http://lh5.google.com/idwaring/RyLW2vz0vgI/AAAAAAAAAS0/dBCaEWNTWuU/s400/Image00048.jpg)
![](http://lh6.google.com/idwaring/RyLW0_z0vdI/AAAAAAAAASc/3jjVdwgzAIM/s400/Image00045.jpg)
![](http://lh5.google.com/idwaring/RyLW1vz0veI/AAAAAAAAASk/o3x2fzOl6yk/s400/Image00046.jpg)
![](http://lh3.google.com/idwaring/RyLW2Pz0vfI/AAAAAAAAASs/lXXJnTki4oM/s400/Image00047.jpg)
![](http://lh4.google.com/idwaring/RyLW3fz0vhI/AAAAAAAAAS8/cO3SeylV_W4/s400/Image00049.jpg)
![](http://lh6.google.com/idwaring/RyLW3_z0viI/AAAAAAAAATE/mKJUpfX3H70/s400/Image00050.jpg)
　　输入数据库管理员的密码．
![](http://lh4.google.com/idwaring/RyLW4fz0vjI/AAAAAAAAATM/w0BU6qnvASw/s400/Image00051.jpg)
　　完成安装
![](http://lh6.google.com/idwaring/RyLW4_z0vkI/AAAAAAAAATU/XK-1Zde7Wjo/s400/Image00052.jpg)
　　重新检测已安装的组件
![](http://lh5.google.com/idwaring/RyLW5vz0vlI/AAAAAAAAATc/xlcmOoltDFg/s400/Image00053.jpg)
二：为完成安装的系统打上补丁
　　在安装配置时agent会起不来，可以先忽略，打上补丁后就可以了
su oracle 
　　cp p2617419_210_GENERIC.zip /tmp 
　　cd /tmp 
　　unzip p2617419_210_GENERIC.zip 
　　export PATH=$PATH:/tmp/OPatch 
　　export PATH=$PATH:/sbin 
　　cp p3238244_9204_LINUX.zip /usr/Oracle 
　　unzip /usr/Oracel/p3238244_9204_LINUX.zip 
　　opatch apply -no_inventory /usr/Oracle/3238244 
　　find $ORACLE_HOME -name "*.mk" | xargs grep -l dbsnmp 
　　cd $ORACLE_HOME/network/lib 
　　make -f ins_oemagent.mk install 
![](http://lh3.google.com/idwaring/RyLW6Pz0vmI/AAAAAAAAATk/BpAcQmEzfUo/s400/Image00054.jpg)
![](http://lh6.google.com/idwaring/RyLW6_z0vnI/AAAAAAAAATs/alP0yzUStGA/s400/Image00055.jpg)
![](http://lh4.google.com/idwaring/RyLW7fz0voI/AAAAAAAAAT0/MFTgL9vGz98/s400/Image00056.jpg)
![](http://lh5.google.com/idwaring/RyLW7vz0vpI/AAAAAAAAAT8/TJ93K1lTl3I/s400/Image00057.jpg)
![](http://lh4.google.com/idwaring/RyLW8fz0vqI/AAAAAAAAAUE/h4B1_SIgufA/s400/Image00059.jpg)
三：在本机测试数据库是否已启动
su oracle 
　　sqlplus "as /sysdba" 
　　exit 
　　lsnrctl 
　　stop 
　　start
![](http://lh6.google.com/idwaring/RyLW9_z0vsI/AAAAAAAAAUU/-ViiGlX4WQs/s400/Image00062.jpg)
![](http://lh4.google.com/idwaring/RyLW-fz0vtI/AAAAAAAAAUc/dvsTdkiQWdM/s400/Image00065.jpg)
![](http://lh6.google.com/idwaring/RyLW__z0vvI/AAAAAAAAAUs/_3DH2xU7lkQ/s400/Image00067.jpg)
四：远程查看和登陆Oracle
![](http://lh3.google.com/idwaring/RyLhgPz0vwI/AAAAAAAAAVM/lyx_Rgfbq-M/s400/Image00000.jpg)
![](http://lh4.google.com/idwaring/RyLhgfz0vxI/AAAAAAAAAVU/eBv1SSzlndc/s400/Image0001.jpg)
![](http://lh6.google.com/idwaring/RyLhg_z0vyI/AAAAAAAAAVc/Jr6QhqlF6Zw/s400/Image0002.jpg)
至此，Oracle的安装已经圆满完成，谢谢大家支持
引用于:http://waringid.blog.51cto.com/65148/47655

