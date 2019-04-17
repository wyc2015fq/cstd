# CentOS配置smaba与Windows共享文件 - xiahouzuoxin - CSDN博客





2014年06月13日 10:40:13[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6036
个人分类：[Linux](https://blog.csdn.net/xiahouzuoxin/article/category/1286747)










操作环境：CentOS 6.5 64bit


Linux与Linux间通过什么共享文件呢——NFS，Windows与Windows之间呢——共享文件功能就OK了，那Windows与Linux之间呢？


这就是本段要讲的东东——samba。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E6%A3%80%E6%9F%A5%E6%98%AF%E5%90%A6%E5%AE%89%E8%A3%85samba)检查是否安装samba

```
# rmp -qa|grep samba
```


若安装了，则会显示如下，

```
samba4-libs-4.0.0-58.el6.rc4.x86_64
samba-common-3.6.9-168.el6_5.x86_64
samba-winbind-3.6.9-168.el6_5.x86_64
samba-client-3.6.9-168.el6_5.x86_64
samba-3.6.9-168.el6_5.x86_64
samba-winbind-clients-3.6.9-168.el6_5.x86_64
```


否则，使用

```
# yum install smaba
```


安装samba.

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E9%85%8D%E7%BD%AEsamba)配置samba
- 
设置开机启动

```
# chkconfig smb on
# chkconfig nmb on
```

也可以使用setup命令可通过界面配置开机启动服务,

```
# setup
```

![setup](https://github.com/xiahouzuoxin/notes/raw/master/images/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6/setup.png)

- 
新建smb用户用于访问Linux共享文件

```
# useradd smb       # 新建用户
# smbpasswd -a smb  # 修改密码
```

此时/home目录会增加一个smb的用户。该Linux用户目录将可直接共享到Windows下。若要共享其它文件，按步骤3配置文件。如果不用了，删除smb用户也是可以的，

```
# smbpasswd -x smb  # 删除smb用户
```

- 
samba配置文件。

```
[tmp]
comment = Tmp Directories
path = /tmp                         # 共享的Linux目录
public = no                         # 目录不公开
writeable = yes                     # 可写
browseable = yes                    # 可读
valid users = smb                   # 访问用户，上面新建的，也可以使用原来已有的
```


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E8%AE%A9smaba%E9%80%9A%E8%BF%87%E9%98%B2%E7%81%AB%E5%A2%99)让smaba通过防火墙


使用samba需要穿过防火墙，因此根据不同情况，这里提供3种方法：
- 
方法1

```
# system-config-firewall
```

![smb-firewall](https://github.com/xiahouzuoxin/notes/raw/master/images/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6/smb-firewall.png)

- 
方法2

配置/etc/sysconfig/iptables文件，添加

```
-A INPUT -m state --state NEW -m tcp -p tcp --dport 139 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 445 -j ACCEPT
-A INPUT -m state --state NEW -m udp -p udp --dport 137 -j ACCEPT
-A INPUT -m state --state NEW -m udp -p udp --dport 138 -j ACCEPT
```

允许139 445 137 138几个端口通过。配置完后重启防火墙，

```
# /etc/rc.d/init.d/iptables restart
```

- 
方法3

索性，把防火墙关了，

```
# service iptables stop    # 暂时关闭
# chkconfig iptables off  # 永久关闭
```


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E5%85%B3%E9%97%ADselinux)关闭SELINUX

```
# vim /etc/selinux/config
SELINUX=permissive
```


此时需要重启下系统，

```
# reboot
```

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E5%90%AF%E5%8A%A8samba)启动samba

```
# service samba start   # 启动
# service samba restart # 重启
```


可以通过

```
# service samba status
```


查看samba启动状态。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#windows%E8%AE%BF%E9%97%AE)Windows访问


先查看Linux虚拟机网络ip地址，

```
# ifconifg
eth0      Link encap:Ethernet  HWaddr 00:0C:29:FD:DC:43  
          inet addr:192.168.56.58  Bcast:192.168.56.255  Mask:255.255.255.0
          inet6 addr: fe80::20c:29ff:fefd:dc43/64 Scope:Link
```


在Windows下使用Win+R开启运行窗口，输入

```
\\192.168.56.58
```


即可访问Linux共享的目录。

![smb](https://github.com/xiahouzuoxin/notes/raw/master/images/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6/smb.png)


此时使用

```
# smbstatus
```


查看连接状态。

![smbstatus](https://github.com/xiahouzuoxin/notes/raw/master/images/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6/smbstatus.png)

> 
文件访问权限：

若无法从Windows中往共享目录中拷贝文件，可能由于共享目录对smb用户的写权限不足，Linux中使用chmod 777 -R dir可改变dir目录权限。由于权限原因，本文中使用/tmp目录共享，因为/tmp目录对所有用户的权限比较开放。


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/CentOS%E9%85%8D%E7%BD%AEsmaba%E4%B8%8EWindows%E5%85%B1%E4%BA%AB%E6%96%87%E4%BB%B6.md#%E5%8F%82%E8%80%83)参考
- [http://www.livingelsewhere.net/tag/unix-linux-samba-ftp-smb-cifs/](http://www.livingelsewhere.net/tag/unix-linux-samba-ftp-smb-cifs/)
- [http://www.tangrucheng.com/centos6-5-setup-configure-samba.html](http://www.tangrucheng.com/centos6-5-setup-configure-samba.html)



