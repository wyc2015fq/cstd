# vsftpd设置被动模式 - z69183787的专栏 - CSDN博客
2017年09月18日 15:27:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：793
个人分类：[FTP-Ftp/Ftps/Sftp](https://blog.csdn.net/z69183787/article/category/6550544)

完整配置
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
listen=yes
listen_port=21
max_clients=100
max_per_ip=10
local_max_rate=5120000
anonymous_enable=no
local_enable=yes
write_enable=no
chroot_local_user=yes
chroot_list_enable=yes
chroot_list_file=/etc/vsftpd/chroot_list
guest_enable=yes
guest_username=kingsoft
virtual_use_local_privs=yes
user_config_dir=/etc/vsftpd/user_config
pasv_enable=yes
pasv_min_port=4500
pasv_max_port=5000
tcp_wrappers=yes
xferlog_enable=yes
xferlog_file=/var/log/ftp/vsftpd.log
idle_session_timeout=600
data_connection_timeout=120
accept_timeout=60
connect_timeout=60
connect_from_port_20=no
local_umask=022
pam_service_name=vsftpd.vu
pasv_address=本机ip
pasv_addr_resolve=yes
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
说明：
```
主动模式
Port_enable=YES               开启主动模式
Connect_from_port_20=YES      当主动模式开启的时候 是否启用默认的20端口监听
Ftp_date_port=%portnumber%    上一选项使用NO参数是 指定数据传输端口
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
被动模式
PASV_enable=YES   开启被动模式
PASV_min_port=%number% 被动模式最低端口
PASV_max_port=%number% 被动模式最高端口
iptables中开放这段端口
service iptables start 打开防火墙
iptables -I INPUT  -p tcp  --dport 10020:10040  -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 21 -j ACCEPT
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 在被动模式，服务器做了NAT，例如云主机，这时候我们用特定的IP访问机器，其实还转了一层。FTP客户端访问机器可能会没响应。具体情况为登录成功，但是list目录和文件的时候卡住。
这时候我们用lsof -i:21
```
vsftpd   22411   nobody    0u  IPv4  68905      0t0  TCP 10.140.41.65:ftp->10.10.10.98:43380 (ESTABLISHED)
vsftpd   22411   nobody    1u  IPv4  68905      0t0  TCP 10.140.41.65:ftp->10.10.10.98:43380 (ESTABLISHED)
```
这时候可以看到机器的真正IP。
我们需要设置
```
pasv_address=本机ip【就是我们能访问的外网IP】
pasv_addr_resolve=yes
```
这样ftp客户端就可以解析IP，访问成功
