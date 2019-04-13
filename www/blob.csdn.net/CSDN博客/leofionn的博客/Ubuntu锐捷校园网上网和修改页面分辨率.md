
# Ubuntu锐捷校园网上网和修改页面分辨率 - leofionn的博客 - CSDN博客


2018年05月26日 21:15:43[leofionn](https://me.csdn.net/qq_36142114)阅读数：44


下载linux锐捷校园网客户端，解压
cd /home/jarvanl/rjsupplicant/
su
./rjsupplicant.sh -a 1 -d 1 -u 账号名  -p  密码   -s 1 -n eth0

需要保证的参数：
修改eth0参数
vi  /etc/network/interfaces
auto lo
iface lo inet loopback
auto eth0
iface eth0 inet loopback
停止networkmanager再启动，然后重启
sudo stop network-manager
sudo NetworkManager
如果上面不起作用
sudo start network-manager
重启!!一定得重启

分辨率修改--
右键-更改桌面背景-全部设置(左上角)-系统 软件和更新(最下面第二个)
--附加驱动--选择fglrx-updates(专有)--重启！！！--
右键-更改桌面背景-全部设置(左上角)--硬件 显示--可以选择分辨率率

