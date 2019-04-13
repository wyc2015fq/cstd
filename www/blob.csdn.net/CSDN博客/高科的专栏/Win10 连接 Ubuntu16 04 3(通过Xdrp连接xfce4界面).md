
# Win10 连接 Ubuntu16.04.3(通过Xdrp连接xfce4界面) - 高科的专栏 - CSDN博客

2018年09月28日 09:44:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：205


Win10 连接 Ubuntu16.04.3(通过Xdrp连接xfce4界面)
sudo apt-get install xrdp
sudo apt-get install vnc4server
sudo apt-get install xubuntu-desktop
echo "xfce4-session" >~/.xsession
sudo apt-get install dconf editor
sudo dconf editor(或者在搜索栏里找dconf编辑)
依次展开：org->gnome->desktop->remote-access 关掉require-encryption
sudo service xrdp restart
在Ubuntu系统开启桌面共享
1.在Ubuntu软件列表找到远程桌面共享（英文：Desktop Sharing）
2.勾选共享（第一个选项）,关掉第二列选项的每次登陆询问，否则你每次登陆都需要服务端确认才可以登陆，密码最好设置，安全一些！
步骤八：通过Win10的远程桌面软件连接Ubuntu
1.先查询Ubuntu的IP地址：在终端输入ifconfig
2.在win10下按win+R键并输入mstsc（启动远程桌面）
输入Ubuntu的IP地址，输入Ubuntu系统的用户名及密码，连接登陆即可！

