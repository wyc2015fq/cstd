# Vmware Unity模式 - V__KING__的专栏 - CSDN博客





2017年01月22日 17:09:15[v__king__](https://me.csdn.net/V__KING__)阅读数：778








# ubuntu 12.04 之Vmware Unity模式
- 安装VMware Tool
- sudo add-apt-repository ppa:gnome3-team/gnome3
- sudo apt-get install gnome-shell
- 虚拟机重启
- 在用户登陆界面选择Gnome Classic - No Effects桌面

5. 

# ubuntu 14.04 之Vmware Unity模式

终端中输 

sudo apt-get update 

sudo apt-get install gnome-session-fallback 

下载vmware-xdg-detect-de：wget [https://raw.githubusercontent.com/graychan/notes/master/vmware/tools/vmware-xdg-detect-de](https://raw.githubusercontent.com/graychan/notes/master/vmware/tools/vmware-xdg-detect-de)

替换vmware-tools-distrib/lib/bin32/vmware-xdg-detect-de ：cp ~/vmware-xdg-detect-de ~/vmware-tools-distrib/lib/bin32/vmware-xdg-detect-de 

重新安装vmware-tools ：sudo ./vmware-install.pl 

更新完成后，注销，选择 Gnome Classic - No Effects桌面，就可以进入Unity模式了。 

如果有问题的话，重新安装 ： sudo apt-get install –reinstall gnome-session-flashback
# ubuntu 14.10 之Vmware Unity模式

终端中输 

sudo apt-get update 

sudo apt-get install gnome-session-fallback 

下载vmware-xdg-detect-de：wget [https://raw.githubusercontent.com/graychan/notes/master/vmware/tools/vmware-xdg-detect-de](https://raw.githubusercontent.com/graychan/notes/master/vmware/tools/vmware-xdg-detect-de)

替换vmware-tools-distrib/lib/bin32/vmware-xdg-detect-de ：cp ~/vmware-xdg-detect-de ~/vmware-tools-distrib/lib/bin32/vmware-xdg-detect-de 

重新安装vmware-tools ：sudo ./vmware-install.pl 

更新完成后，注销，选择GNOME Flashback(Metacity)桌面，就可以进入Unity模式了。 

如果有问题的话，重新安装 ： sudo apt-get install –reinstall gnome-session-flashback




