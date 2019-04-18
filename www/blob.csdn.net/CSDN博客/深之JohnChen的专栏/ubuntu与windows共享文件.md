# ubuntu与windows共享文件 - 深之JohnChen的专栏 - CSDN博客

2017年07月02日 00:47:01[byxdaz](https://me.csdn.net/byxdaz)阅读数：5544


一、通过vmmare tool工具共享文件
Ubuntu系列10.04、11.04、12.04等虚拟机中安装VMware Tools参考下面两篇文章。
http://www.linuxidc.com/Linux/2012-05/60472p2.htm
http://lixiyu.blog.51cto.com/4136883/1324141/

我的电脑是ubuntu12.04系统，安装vmware tool工具失败。
解决方法：在软件中心中安装vmware client。通过mount命令可以共享，但是每次重启系统之后，就无法共享了，必须再输入mount命令。
打开终端(热键：Ctrl+Alt+T)，输入下面mount命令，这样打开window下share目录就可以共享文件了。
sudo mount -t vmhgfs .host:/share /mnt/hgfs
![](https://img-blog.csdn.net/20170702003934148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170702003951066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

二、Samba共享文件
1、Samba软件包的安装
使用源安装，在终端中输入如下命令：
sudo apt-get install samba
sudo apt-get install smbclient
2、配置Samba服务
Samba服务器主要配置文件为/etc/samba/smb.conf，并且可以将NetBIOS名与主机的对应关系写在/etc/samba/lmhosts文件中。
1）在ubuntu共享一个目录，新建一个共享目录（比如：/home/johnchen/smb-share）。
2）在终端中输入：chmod -R go+rwx smb-share/ 可使Windows下用户对该共享目录拥有写权限。
3）将建立好的目录的设置信息写入/etc/samba/smb.conf文件即可。
sudo cp /etc/samba/smb.conf /etc/samba/smb.conf.bak       //修改配置文件之前最好做个备份
使用 sudo gedit /etc/samba/smb.conf 打开配置文件
将文件中的内容做如下相应修改：
#security=user 后面添加：
security=share
在文件结尾添加如下行：
[share]
comment=this is Linux share directory
path=/home/johnchen/smb-share
public=yes
writable=yes

保存退出，启动Samba服务：
sudo service smbd start
如果想停止Samba服务：
sudo service smbd stop

在Windows下访问共享目录，运行窗口（热键：win+R）中输入
\\192.168.16.111\share
ubuntu查看ip地址，在终端中输入ifconfig
这样就能以匿名用户访问共享目录share了。

三、通过teamviewer、QQ工具共享文件。

