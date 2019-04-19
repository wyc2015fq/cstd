# RT3070 WIFI模块Android调试全记录 - xqhrs232的专栏 - CSDN博客
2013年01月24日 20:41:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：879
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/ccwwff/article/details/6535290#comments](http://blog.csdn.net/ccwwff/article/details/6535290#comments)
转载请注明来源：[http://blog.csdn.net/ccwwff/archive/2011/06/09/6535290.aspx](http://blog.csdn.net/ccwwff/archive/2011/06/09/6535290.aspx)
首先应该确定与CPU的USB Host接口驱动通信应该正常。
然后按以下顺序进行配置。
一、 在linux内核配置，加入以下wireless配置，然后编译烧录内核:
Drivers->Networking->Wireless->
CONFIG_IEEE80211
-*- Improved wireless configuration API
[*] nl80211 new netlink interface support
-*- Wireless extensions
<*> Generic IEEE 802.11 Networking Stack (mac80211)
Rate control algorithm selection --->
[ ] Enable LED triggers
Export mac80211 internals in DebugFS
[ ] Enable packet alignment debugging
[ ] Enable debugging output
{*} Generic IEEE 802.11 Networking Stack (DEPRECATED)
[*] Enable full debugging output
-*- IEEE 802.11 WEP encryption (802.1x)
<*> IEEE 802.11i CCMP support
<*> IEEE 802.11i TKIP encryption
<*> Software MAC add-on to the IEEE 802.11 networking stack
[*] Enable full debugging output
二、准备如下文件及工具, 其源码可以上网搜索：
iwpriv, iwlist, iwconfig, wget, wpa_supplicant, rt3070sta.ko, RT2870STA.DAT;
然后按以下顺序进行wifi调试环境配置：
1. 建立文件/system/etc/wifi/wpa_supplicant.conf, 内容如下, 以下主要配置ssid用，可根据当前环境调试用AP（无线路由器）热点的配置进行设置，这个是用来调试用的，在android环境，有其它配置方法：
ctrl_interface=DIR=/data/misc/wifi GROUP=system
update_config=1
ap_scan=1
network={
ssid="example_essid"
psk="password"
}
2．建目录dhcp, 及其空文件dhcpcd-tiwlan0.pid：
#mkdir /data/misc/dhcp
#cd /data/misc/dhcp
#vi dhcpcd-tiwlan0.pid
3. 设置网络接口：
#setprop wifi.interface "tiwlan0"
#setprop wlan.interface "tiwlan0"
4. 安装wifi驱动, RT2870STA.DAT拷贝至/etc/Wireless/RT2870
#insmod /sdcard/ rt3070sta.ko
6. 拷贝无线工具iwpriv, iwlist, iwconfig, wpa_supplicant至/system/bin目录
Iwpriv: 可查看网络节点，输入以下命令，如rt3070sta.ko安装成功，会显示我们想要的网络节点：
#iwpriv
7. ifconfig打开网络节点tiwlan0：
＃ifconfig tiwlan0 up
8. iwlist tiwlan0 scanning 搜索显示ssid：
＃iwlist tiwlan0 scanning
9.设置wifi essid, 并动态分配IP, dhcpcd是android一个dhcp库，注意要加&, 后台运行，否则后面的命令就输不了了：
# /system/bin/wpa_supplicant -dd -Dwext -itiwlan0 -c /system/etc/wifi/wpa_supplicant.conf &
#/system/bin/dhcpcd tiwlan0 &
10. ping , 检查连接情况, 如果出现连接响应，那就恭喜了：
＃ping 192.168.1.1 &
11. 下面就可以通过网络测试你的网络速度如何了，wget命令就可以简单搞定了，如下所示，如果是USB1.1 host, 就不要期待有多高的速度了，2-3Mbps就不错了，我这边用OTG(USB2.0), 连接54Mbps无线路由器，100MB的文件，50秒左右搞定了，平均16Mbps, 看起来还不错。将RT3070模块直接连接PC，下载速度基本在16Mbps左右，最高20Mbps左右。
＃/bin/busybox wget ftp://192.168.1.1/file -O filepath
12.推荐读取资料：rt3070驱动中Readme, iwpriv使用文档，iwpriv可以读取RT3070芯片寄存器的值，进行调试。
三、android wifi优化调试
1. （休眠/唤醒）调试，USB支持远程（休眠/唤醒），这个对于手持设备功耗至关重要。
2. init.rc需增加以下几行, 及其它必要的配置：
service wpa_supplicant /system/bin/wpa_supplicant -dd -Dwext -itiwlan0 -c /system/etc/wifi/wpa_supplicant.conf
group system wifi inet
disabled
oneshot
service dhcpcd /system/bin/dhcpcd tiwlan0
group system dhcp
disabled
oneshot
3. 可在Android setting应用程序里配置你的ssid了。
android ftp工具, 测试网络速度；
4. 可以安装一个FTP工具测试一下你的网速，推荐AndFTP.apk.
