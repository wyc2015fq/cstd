# Ubuntu16.04 server安装Samba - tifentan的专栏 - CSDN博客





2016年12月09日 14:44:07[露蛇](https://me.csdn.net/tifentan)阅读数：1436










**1.首先安装samba**

sudo apt-get install samba


备份conf文件

sudo cp smb.conf smb.conf.bk


**2.编辑smb.conf**

**2.1配置无需验证共享文件夹**

在smb.conf文件最后添加

[share]comment = samba ##这个是说明，随意填

path = #你的文件夹目录

public = yes

create mask = 0775 #

directory mask = 0775 #

**2.2配置验证共享文件夹**


**2.2.1为系统添加一个用户**

sudo adduser usamba

**2.2.2将用户注册到samba**

sudo smbpasswd -a usamba

sudo smbpasswd -e usamba

**2.2.3在smb.conf文件最后添加**

[share]comment=samba

path = #你的文件夹目录

public = no

valid users = usamba

createmask
= 0775

directorymask
= 0775

**2.3配置写权限**


把smb.conf里面

;read only = yes

改成

read only = no

[share]里面配置改为



createmask
= 0777

directorymask
= 0777

**3.保存后重启samba**

sudo /etc/init.d/sambd restart







