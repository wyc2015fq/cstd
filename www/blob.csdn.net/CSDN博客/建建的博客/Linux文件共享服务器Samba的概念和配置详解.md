# Linux文件共享服务器Samba的概念和配置详解 - 建建的博客 - CSDN博客
2017年03月01日 10:43:06[纪建](https://me.csdn.net/u013898698)阅读数：624
在[Linux](http://www.1987.name/t/linux)（Unix）主机之间可以使用[NFS](http://www.1987.name/t/nfs)服务实现文件共享。而且是现实来也十分方便，在Windows主机之间使用Windows文件共享同样可以实现包括网络文件在内的系统资源的共享。如果想在linux和Windows主机之间进行文件的共享，使用samba服务器是比较理想的解决方案。
## linux与Windows之间文件共享方式
在构建linux主机和Windows主机的混合应用系统中，linux主机用过使用samba服务器和客户机与Windows主机实现共享文件互相访问。主要有以下三种应用方式：
- linux主机运行samba服务器，Windows主机作为客户端。
- linux主机运行samba服务器，在linux主机中使用samba客户端程序对samba服务器进行测试，并访问服务器中的共享资源。
- Windows主机提供文件共享服务。linux主机中使用samba客户端程序访问Windows共享文件。
通过以上三种文件共享的使用方式可以看出，linux主机与Windows主机之间进行文件共享的互访主要涉及到samba服务器、samba客户端、和Windows服务器3个角色，其中Windows服务器既可以提供文件共享服务也可以作为客户机访问其他服务器中的共享文件，Windows与linux主机之间共享文件互访关系图：
![samba服务器](http://www.1987.name/wp-content/uploads/2013/01/033345Kkm.jpg)
## samba服务器的基本原理
samba服务器在linux系统上实现了SMB/CIFS协议，即可以在linux系统中为Windows主机提供网络文件共享服务。samba服务器中包括两个服务程序：
- smbd服务程序为客户机提供了服务器中共享资源（目录和文件等）的访问。
- nmbd服务程序提供了netbios主机名称解析，为Windows网络中或者工作组内的主机进行主机名称的解析。
samba服务器需要smbd和nmbd两个服务程序同时运行。
## samba服务器的安装
所需软件包：
- `samba-common`软件包：提供了samba服务器和客户机都必须使用的公共文件。
- `samba-client`软件包：提供了samba客户机所有的文件。
- `samba`软件包：提供了samba服务器所有的文件。
### 安装Samba
yum -y install samba samba-client samba-common
### 建立samba用户
useradd wwwroot
smbpasswd -a wwwroot
New SMB password:  输入密码
Retype new SMB password:  再次输入
关于smbpasswd命令选项：
- `-h`显示smbpasswd的命令格式帮助
- `-a`添加指定的用户帐号
- `-d`禁用指定的用户帐号
- `-e`启用指定的用户帐号
- `-x`删除指定的用户帐号
- 没有添加任何选项时候将改变用户密码。
### 建立共享目录
samba服务器主配文件是[smb.conf](http://www.1987.name/686.html)，位于`/etc/sabma/smb.conf`。下面是建立一个共享目录wwwroot的示例配置：
grep -v "^[#|;]" /etc/samba/smb.conf | tr -s 'n'
[global]
        workgroup = MYGROUP
        server string = Samba Server Version %v
        # logs split per machine
        # max 50KB per log file, then rotate
        security = user
        passdb backend = tdbsam
        # the login script name depends on the machine name
        # the login script name depends on the unix user used
        # disables profiles support by specifing an empty path
        load printers = yes
        cups options = raw
        #obtain list of printers automatically on SystemV
[homes]
        comment = Home Directories
        browseable = no
        writable = yes
[printers]
        comment = All Printers
        path = /var/spool/samba
        browseable = no
        guest ok = no
        writable = no
        printable = yes
[wwwroot]
        comment = Web Directories
        path = /data/wsdata/wwwroot
        public = yes
        writable = yes
        browseable = yes
更详细的smb.conf配置文件的中文解释请参考《[Linux
 Samba服务主配文件smb.conf中文详解](http://www.1987.name/686.html)》
### 启动samba服务器
service smb start- `start`启动
- `status`状态
- `stop`停止
- `restart`重启
### smbstatus命令
`smbstatus`命令用于显示主机中samba服务器的链接状态，包括链接服务器的客户端IP地址、主机名、登录用户名、锁定的文件等，通过smbstatus命令可以了解samba服务器的运行状态。
smbstatus
Samba version 3.0.33-3.39.el5_8
PID     Username      Group         Machine                      
-------------------------------------------------------------------
  305   wwwroot       wwwroot       2011-20121102ul (192.168.1.21)
Service      pid     machine       Connected at
-------------------------------------------------------
IPC$         305   2011-20121102ul  Tue Jan  8 10:18:07 2013
wwwroot      305   2011-20121102ul  Tue Jan  8 10:19:13 2013
Locked files:
Pid          Uid        DenyMode   Access      R/W        Oplock           SharePath   Name   Time
--------------------------------------------------------------------------------------------------
305          504        DENY_NONE  0x100001    RDONLY     NONE             /data/wsdata/wwwroot   .   Tue Jan  8 10:20:07 2013
iptables防火墙设置：
iptables -A INPUT -p tcp --dport 445 -j ACCEPT
## samba客户端使用
samba服务器的客户端软件包samba-client中包括了samba客户端管理命令，通过这些命令可以查看和访问samba服务器或Windows服务器中的文件共享，软件包中的很多命令中，我们只需要使用几条就可以应用samba客户端了。
### Linux命令行客户端
下面介绍几条samba客户端常用命令：
`smbclient`命令是以命令行方式访问samba服务器，类似于`telnet`、`ftp`命令，使用`-L`选项可以列出指定服务器中共享资源。
smbclient -L 192.168.1.110 -U wwwroot
Password:  输入密码
Domain=[LOCALHOST] OS=[Unix] Server=[Samba 3.0.33-3.39.el5_8]
        Sharename       Type      Comment
        ---------       ----      -------
        IPC$            IPC       IPC Service (Samba Server Version 3.0.33-3.39.el5_8)
        wwwroot         Disk      Web Directories
Domain=[LOCALHOST] OS=[Unix] Server=[Samba 3.0.33-3.39.el5_8]
        Server               Comment
        ---------            -------
        Workgroup            Master
        ---------            -------
        BONARF               XP-O0
        MSHOME               CHINACHINA
        MYGROUP              LOCALHOST
        WORKGROUP            XP-201110131805
使用`smbclient`命令登录samba服务器：
smbclient //192.168.1.110/wwwroot -U wwwroot
          
Password:  输入密码
Domain=[LOCALHOST] OS=[Unix] Server=[Samba 3.0.33-3.39.el5_8]
smb: > 
smb: >
smb: >
#可以使用help命令查看客户端支持的命令：
smb: >help
?              altname        archive        blocksize      cancel         
case_sensitive cd             chmod          chown          close          
del            dir            du             exit           get            
getfacl        hardlink       help           history        lcd            
link           lock           lowercase      ls             mask           
md             mget           mkdir          more           mput           
newer          open           posix          posix_open     posix_mkdir    
posix_rmdir    posix_unlink   print          prompt         put            
pwd            q              queue          quit           rd             
recurse        reget          rename         reput          rm             
rmdir          showacls       setmode        stat           symlink        
tar            tarmode        translate      unlock         volume         
vuid           wdel           logon          listconnect    showconnect    
!  
### Windows客户端
在Windows上访问samba服务器很简单，按`Windows徽标键+R`打开“运行”窗口：
![smb1](http://www.1987.name/wp-content/uploads/2013/01/smb1.jpg)
![smb2](http://www.1987.name/wp-content/uploads/2013/01/smb2.jpg)
#### 映射Windows网络驱动器
这样做的目的主要是为了跨平台开发搭建开发环境。现在我的开发环境为 Windows 7 + CentOS 5.8 (VMWare)，将 samba 共享的 Linux 目录，映射成 Windows 的一个驱动器盘符：
![smb3](http://www.1987.name/wp-content/uploads/2013/01/smb3.jpg)
![smb4](http://www.1987.name/wp-content/uploads/2013/01/smb4.jpg)
