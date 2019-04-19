# Linux下VSFTPD配置全方案 - 零度的博客专栏 - CSDN博客
2017年10月23日 10:02:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：374
### 一、VSFTPD的简述
VSFTPD（Very Secure FTP Daemon，非常安全的FTP服务器）。顾名思义，VSFTPD设计的出发点就是安全性。同时随着版本的不断升级，VSFTPD在性能和稳定性上也取得 了极大的进展。除了安全和性能方面很优秀的外，还有很好的易用性。Red Hat公司在自己的FTP服务器(ftp.redhat.com)上就使用了VSFTPD。
### 二、企业Linux下VSFTPD实战方案
要求：
1. 用户:netseek_com；FTP：ftp.netseek.com(192.168.0.21,我已经配置好了相关的DNS)
2. 禁止匿名登陆，禁止用户离开用户目录
3. 命令通道端口使用5021，数据通道端口使用5020（你可以改成你想设置的端口如1021,1020）
4. 允许使用被动模式使用端口范围为10010~10020.
5. 用户带宽设置为200kps；用户空间大小限制为300M，允许宽限期限内有10MB的缓冲
具体操作步骤如下：
为了更完全的描述安装全过程，我在我的另一台主机（netseek）上进行配置，在台机IP:192.168.0.20上绑定了192.168.0.21,也配置好了相关的DNS解析，在这里我就不多说了请参照“Linux全能web服务器架设”。
#### 1、 配置相关服务
启动服务：
[root@netseek root]# #rpm –qa | grep vsftpd ；查看是否安装vsftpd服务
#/etc/init.d/vsftpd start          ;启动服务
#chkconfig vsftpd on              ;让系统服务随着系统启动而启动
配置相关文档：
#vi /etc/vsftpd/vsftpd.conf
禁止匿名用户访问：
anonymous_enable=YES,将YES改为NO
在文件末尾加如下的设置：
# new added by netseek
listen_port=5021                      #命令通道端口，默认为21
listen_data_port=5020                   #数据通道端口，默认为20
pasv_enable=YES                      #允许被动模式
pasv_min_port=10000                   #被动模式使用端口范围
pasv_max_port=10010
local_max_rate=200000                    #用户宽带限制
chroot_local_user=YES                    #禁用户离开主目录
listen_address=192.168.0.21             #让他监听ip:192.168.0.21
#### 2、 添加虚拟主机用户
添加用户netseek_com,用户目录指定为/var/www/netseek.com,且此用户不能登陆系统.
#mkdir /var/www/netseek.com
#useradd –s /sbin/nologin –d /var/www/netseek.com netseek_com
注-s /sbin/nologin是让其不能登陆系统，-d 是指定用户目录为/var/www/netseek.com
#passwd netseek_com           (Do not Hack me 3ks) DnHm3ks 这是我设置的密码
#chown –R netseek_com:netseek_com /var/www/netseek.com
注:将用户目录及其子目录的所有和所属的组设置为netseek_com
#### 3、 配置磁盘配额
我在安装过程中磁盘了多个区，分别/home,/var,/，/www,tmp,swap,/boot多个分区。建议你将磁盘分多个独单的分区,这样可以降低磁盘分区损坏造成的数据丢失风险.
#[root@netseek root]# rpm -qa | grep quota
quota-3.06-9             #我的系统已经安装了磁盘配额工具
#vi /etc/fstab
找到类似
LABEL=/var              /var                    ext3 defaults        1 2    
在defaults后加入usrquota，改为如下：
LABEL=/var              /var                    ext3 defaults,usrquota        1 2
重新加载/var这个分区：
#mount –o remount /var
#cd /var   进入这个目录
#touch quota.user
#chmod 600 quota.user
#quotacheck –mf /var     对磁盘分区空间使用情况进行强制检测
[root@localhost var]# quotacheck -mf /var
quotacheck: WARNING – Quotafile /var/quota.user was probably truncated. Can’t save quota   settings…
重新启动系统
第一次运行时都会出现这个警告，再次运行的时候就不会啦
为指定用户定额：
#edquota netseek_com 为用户netseek_com设置磁盘配额。
Disk quotas for user netseek_com (uid 501):
   Filesystem                blocks    soft    hard     inodes     soft     hard
   /dev/sda5                   6616     307200       0       15        0        0
注：
ilesystem blocks soft hard inodes soft hard
filesystem-正在设置的文件系统，不要修改或删除
bolock-当前使用的碰盘空间，单位为KB
soft(第一个)-软磁盘空间限制，默认0，表示无限制，我在这里设为300M。
hard(第一个)-硬配额限制，0表示无限制。
inodes-当前文件数量
soft(第二个)-对文件数量的软限制,0表示无限制
hard(第二个)-对文件数量的硬限制,0表示无限制
让系统启动是自动加载:
#vi /etc/rc.d/rc.local ,加入以下:
/etc/init.d/vsftpd start
/sbin/quotacheck
/sbin/quotaon –avu
现在查看netseek_com 用户使用磁盘空间的情况
[root@localhost ~]# quota -u netseek_com
Disk quotas for user netseek_com (uid 501):
    Filesystem   blocks quota limit grace files quota limit grace
   /dev/sda5 6616   307200    0              15    0    0
注：关于更多的quota命令，请用man命令查看。
### 三， VSFTPD相关的配置详解
**1.匿名用户相关设置：**
anonymous_enable=YES              ,将YES改为NO, 禁止匿名用户登陆
#non_mkdir_write_enable=YES        ,将#注释去掉，允许匿名用户创建目录
#non_upload_enalbe=YES          ,将#去掉，允许匿名用户上传
anon_world_readable_only=YES    ,允许匿名用户下载，默认是禁止的，这个可以自行添加。
Anon_other_write_enable=YES        ,将其设为YES的话，就除了上传和创建目录外，还可以重命名，删除文件，默认是NO
no_anon_password=NO             ,将其设为YES,匿名用户不会查询用户密码直接登陆。
ftp_username=ftp                    ,匿名用户登陆系统的账号默认为ftp,此项最好不要改，否则设置不当会给系统的安全带来威胁。
**2.FTP服务端口的指定：**
listen_port=8021                   ,指定命令通道为8021,默认为21
listen_data_port=8020             ,指定数据通道为8020,默认为20
**3.上传模式的设置：**
**pasv_enable=YES                    ,是否允使用被动模式，默认是允许的。**
**pasv_min_port=10000                 ，指定使用被动模式时打开端口的最小值**
**pasv_max_port=10004             ，指定使用被动模式时打开端口的最大值。**
**4.vsftp服务器指定IP地址：**
listen_address=192.168.0.21           指定FTP服务器IP地址
注：只有当vsftp运行于独立模式时才允许使用指定IP,如果在/etc/xinetd.d目录下已经建立了vsfpd文件，就要将该文件中的disable设置为yes，方可。
**5. 锁定用户，禁止用户离开用户主目录：**
chroot_local_user=YES             将其设为YES，就锁定在用户主目录，设为NO，可以切换
将指定用户设置为锁定用户主目录:
#chroot_list_enable=YES
#chroot_list_file=/etc/vsftpd.chroot_list
将其改为如下:
chroot_list_enable=NO
chroot_list_file=/etc/vsftpd/vsftpd.chroot_list
将上面保存，再做如下操作：
#touch /etc/vsftpd/vsftpd.chroot_list
#vi /etc/vsftpd/vsftpd.chroot_list ,在该文件中加入用户名单，如：
netseek_com
**6.FTP服务器的流量控制：**
max_clients=100          ;允许的最大连接数，定义为100，默认为0，表没有限制
max_per_ip=5             ;每个IP允许的连接数，0表没有限制，需要运行于独立模式方可
anon_max_rate=50000        ;匿名用户最大带宽，单位为bps
local_max_rate=200000    ;系统用户最大带宽
如何对指定用户进行流量限制呢？
#vi /etc/vsftpd/vsftpd.conf,添加一行：
user_config_dir=/etc/vsftpd/userconf
#touch /etc/vsftpd/userconf/netseek_com   为netseek_com这个用户建立一个netseek_com文件
#vi /etc/vsftpd/userconf/netseek_com   添加以下内容
local_max_rate=100000         
保存重启服务即可。
**7.定制欢迎信息：**
目录说明设置
#vi /etc/vsftpd/vsftpd.conf
#dirmessage_enable=YES,前的#去掉。
然后我们定制一个.message,写上你想写的东西，然后把这个文件复制到各个用户的家目录中,就OK。
系统欢迎消息设置
ftpd_banner=Welcome to ftp.netseek.com , Yeah!!!
系统欢迎消息文件设置
banner_file=/etc/vsftpd/welcome 与ftpd_banner相类似，不同之处在于，banner_file指定欢迎文件。
