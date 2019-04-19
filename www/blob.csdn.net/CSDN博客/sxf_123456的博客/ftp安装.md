# ftp安装 - sxf_123456的博客 - CSDN博客
2018年05月12日 19:54:32[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：253
                1、vsftpd 安装
 下载vsftpd包
 wget https://olex-secure.openlogic.com/content/openlogic/vsftpd/3.0.2/vsftpd-3.0.2.tar.gz
 # tar zxf vsftpd-3.0.2.tar.gz
 # cd vsftpd-3.0.2
 # make && make install
注：x86_64位系统在make的时候可能会提示错误： 
/usr/bin/ld: cannot find -lcap
这是因为其只会去/lib/或/usr/lib/下查找，而x86_64应该去/lib64/和/usr/lib64/中查找才对，所以需要修改vsf_findlibs.sh文件： 
# cp vsf_findlibs.sh vsf_findlibs.sh.default
# vim vsf_findlibs.sh
// 统一把/lib/改成/lib64/，而/usr/lib则改成/usr/lib64
修改完之后再来编译安装： 
2、修改配置文件
# mkdir /etc/vsftpd/
# cp /usr/local/src/vsftpd-3.0.2/vsftpd.conf /etc/vsftpd/vsftpd.conf
# cp /etc/vsftpd/vsftpd.conf /etc/vsftpd/vsftpd.conf.default
3、设置vsftpd安全配置
# yum install pam
# yum install pam-devel
# yum install db4-utils
1）首先创建虚拟用户口令明文文件，使用前面安装的db4-utils组件生成口令认证文件： 
# vim /etc/vsftpd/access.txt
brishenzhou
brishenzhou_pwd
# db_load -T -t hash -f /etc/vsftpd/access.txt /etc/vsftpd/access.db
注：access.txt中一行用户名+一行密码，保持这样。 
2）编辑vsftpd的PAM认证文件： 
# vim /etc/pam.d/vsftpd
auth required /lib64/security/pam_userdb.so db=/etc/vsftpd/access
account required /lib64/security/pam_userdb.so db=/etc/vsftpd/access
注：这里使用的就是/etc/vsftpd/access.db文件。 
3）所有的虚拟用户都需要使用一个系统本地用户，所以这里创建一个不需要登录的系统本地用户，并且设定它的主目录是/data/vsftpd： 
# id vsftpd
id: vsftpd：无此用户
# groupadd vsftpd
# mkdir -p /data/vsftpd/pub
# useradd -g vsftpd -d /data/vsftpd -s /sbin/nologin vsftpd
# id vsftpd
uid=504(vsftpd) gid=504(vsftpd) 组=504(vsftpd)
# chown -R vsftpd:vsftpd /data/vsftpd
# chmod a-w /data/vsftpd
# chmod 777 /data/vsftpd/pub
注：由于需要启用chroot，这里的根目录/data/vsftpd必须不可写，所以不能上传文件，可以新增一个pub的目录来放上传的文件。 
4）配置vsftpd开启虚拟用户选项： 
# vim /etc/vsftpd/vsftpd.conf
#禁止匿名用户
anonymous_enable=NO
local_enable=YES
write_enable=YES
#不启动锁定用户名单，所有的用户都将被锁定不允许访问上级目录，只允许访问其主目录
chroot_local_user=YES
chroot_list_enable=NO
#启动log
xferlog_enable=YES
xferlog_std_format=YES
xferlog_file=/etc/vsftpd/vsftpd.log
#开启虚拟用户
guest_enable=YES
#FTP虚拟用户对应的系统用户
guest_username=vsftpd
#PAM认证文件/etc/pam.d/vsftpd
pam_service_name=vsftpd
virtual_use_local_privs=YES
4、虚拟用户登录
# ftp 12.24.67.13
Connected to 12.24.67.13 (12.24.67.13).
220 (vsFTPd 3.0.2)
Name (12.24.67.13:root): brishenzhou
331 Please specify the password.
Password:
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> nlist
227 Entering Passive Mode (12,24,67,13,54,136).
150 Here comes the directory listing.
226 Directory send OK.
ftp> bye
# make clean
# make && make install
# vsftpd -v
vsftpd: version 3.0.2
1、下载ftp包
下载到指定目录
yum install --downloadonly --downloaddir=/tmp <package-name>
运行时出现Command line error:no such option:--downloadonly
执行下边安装命令
yum install yum-plugin-downloadonly            
