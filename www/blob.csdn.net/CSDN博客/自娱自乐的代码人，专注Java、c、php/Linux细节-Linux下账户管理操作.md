# Linux细节 - Linux下账户管理操作 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 16:12:05[initphp](https://me.csdn.net/initphp)阅读数：711标签：[linux																[networking																[user																[service																[apache](https://so.csdn.net/so/search/s.do?q=apache&t=blog)
个人分类：[Linux细节](https://blog.csdn.net/initphp/article/category/5942619)




- 账户文件存放地址

```
#用户存放位置
/etc/passwd
#密码存放位置
/etc/shadow
#用户组存放位置
/etc/group
#口令存放位置
/etc/gshadow
```
- 用户组管理

```
#添加用户组
groupadd test
#查看刚才添加的用户组
grep test /etc/group

#编辑用户组
groupmod -u tests test

#删除用户组
groupdel tests
```
- 用户管理

```
#添加用户
useradd test
useradd -g test test
useradd -d /www -M -G apache webmaster

#编辑用户
usermod -G staff newuser2
usermod -l newuser1 newuser

#锁定/解除账号
usermod -L newuser1
usermod -U newuser1

#删除用户
userdel webmaster
userdel -r webmaster #目录也删除
```
- 密码口令

```
#设置密码
passwd test

#锁定用户
passwd -l test

#解除用户
passwd -u test

#查看用户状态
passwd -S test
```
- 密码时效配置

```
#编辑 /etc/login.defs 默认配置登陆信息
PASS_MAX_DAYS 99999    #默认下次修改密码时间
PASS_MIN_DAYS 0        #下次修改所需的最小天数
PASS_MIN_LEN 5         #密码长度
PASS_WARN_AGE 7        #设定在口令失效前多少天开始通知用户更改口令
```
- 切换用户命令

```
#直接切换账号
su root

#使用sudo来执行超级管理员操作
sudo mkdir test

#sudo需要进行配置，配置文件 /etc/sudoers, 具体配置如下


# 让 osmond 用户和 wheel 组的成员可以在任何主机上以任何人的身份运行任何命令。
osmond ALL = (ALL) ALL
%wheel ALL = (ALL) ALL
# 专职系统管理员(millert,mikef和dowdy)可以在任何主机上执行任何命令而不需要进行身份验证。
User_Alias FULLTIMERS = millert, mikef, dowdy
FULLTIMERS ALL = NOPASSWD: ALL
# 兼职系统管理员(bostley,jwfox和crawl)可以在任何主机上运行任何命令,
# 但他们首先必须进行身份验证(因为这个条目没有NOPASSWD标签)。
User_Alias PARTTIMERS = bostley, jwfox, crawl
PARTTIMERS ALL = ALL
# 兼职管理员(jalala, sonar和huge)可以在任何主机上运行 BROWSE、PROCESSES、USERS 中的命令
# 同时可以修改除了 root 用户之外的所有用户口令
User_Alias PARTTIMERS2 = jalala, sonar , huge
Cmnd_Alias BROWSE = /bin/ls, /bin/cd, /bin/cat
Cmnd_Alias PROCESSES = /bin/nice, /bin/kill, /usr/bin/kill, /usr/bin/killall
Cmnd_Alias USERS = /usr/sbin/useradd [A-z]*,/usr/sbin/userdel -r [A-z]*
PARTTIMERS2 ALL=USERS,PROCESSES,BROWSE,/usr/bin/passwd [A-z]*, !/usr/bin/passwd root
# 允许sys 组的成员运行 networking, software, service 等管理命令
Cmnd_Alias NETWORKING = /sbin/route, /sbin/ifconfig, /bin/ping, \
/sbin/dhclient, /usr/bin/net, /sbin/iptables, /usr/bin/rfcomm, \
/usr/bin/wvdial, /sbin/iwconfig, /sbin/mii-tool
Cmnd_Alias SOFTWARE = /bin/rpm, /usr/bin/up2date, /usr/bin/yum
Cmnd_Alias SERVICES = /sbin/service, /sbin/chkconfig
Cmnd_Alias LOCATE = /usr/sbin/updatedb
Cmnd_Alias STORAGE = /sbin/fdisk, /sbin/sfdisk, /sbin/parted, /sbin/partprobe, \
/bin/mount, /bin/umount
Cmnd_Alias DELEGATING = /usr/sbin/visudo, /bin/chown, /bin/chmod, /bin/chgrp
Cmnd_Alias PROCESSES = /bin/nice, /bin/kill, /usr/bin/kill, /usr/bin/killall
Cmnd_Alias DRIVERS = /sbin/modprobe
%sys ALL = NETWORKING, SOFTWARE, SERVICES, STORAGE, DELEGATING, \
PROCESSES, LOCATE, DRIVERS
# WEBMASTERS(will, wendy,和 wim)中的用户都能够在主机www上
# 以www的身份执行任何命令，或者使用 su www 命令
User_Alias WEBMASTERS = will, wendy, wim
WEBMASTERS www = (www) ALL, (root) /usr/bin/su www
```](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=networking&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




