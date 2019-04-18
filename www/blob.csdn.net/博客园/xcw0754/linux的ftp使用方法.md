# linux的ftp使用方法 - xcw0754 - 博客园
# [linux的ftp使用方法](https://www.cnblogs.com/xcw0754/p/4963158.html)
[linux下常用FTP命令 1. 连接ftp服务器](http://www.cnblogs.com/xiaochaohuashengmi/archive/2011/10/24/2222686.html)
**以下是在服务器为U****buntu环境下操作：**
（1）首先需要安装vsftp软件，已经安装的可跳过。
　　1.更新软件源
　　　　#apt-get  update
　　2.安装vsftp
　　　　#apt-get install vsftpd -y
　　3.添加ftp帐号和目录（目录是为了限制用户端的活动范围的，可以不使用）
　　　　先检查一下nologin的位置，通常在/usr/sbin/nologin或者/sbin/nologin下。
　　　　使用下面的命令创建帐户，该命令指定了/alidata/www/wwwroot为**用户pwftp**的家目录，您可以自己定义帐户名和目录：
　　　　#useradd -d /alidata/www/wwwroot -s /sbin/nologin pwftp
　　　　修改该帐户密码:
　　　　#passwd pwftp
　　　　修改指定目录的权限　　
　　　　#chown -R pwftp.pwftp /alidata/www/wwwroot
　　4.配置vsftp
　　　　编辑vsftp配置文件，命令如下：
　　　　#vi /etc/vsftpd.conf
　　　　将配置文件中”anonymous_enable=YES “改为 “anonymous_enable=NO”
　　　　取消如下配置前的注释符号：
　　　　local_enable=YES
　　　　write_enable=YES
　　　　chroot_local_user=YES
　　　　chroot_list_enable=YES
　　　　chroot_list_file=/etc/vsftpd.chroot_list
　　　　保存退出
　　　　编辑/etc/vsftpd.chroot_list文件，将ftp的帐户名添加进去，保存退出。（如果没有这个文件，直接写pwftp（即用户名）然后保存即可。）
　　5.修改shell配置
　　　　vi编辑/etc/shells，如果该文件里没有/usr/sbin/nologin或者/sbin/nologin（具体看当前系统配置）则追加进去
　　6.重启vsftp服务并测试登录
　　　　使用命令启动vsftp服务：
　　　　#service vsftpd restart
　　　　然后用帐号pwftp在客户端测试下是否可以登陆ftp。目录是/alidata/www/wwwroot。

