# 自动修改网卡IP地址并连接指定WIFI脚本，方便网络切换 - 左直拳的马桶_日用桶 - CSDN博客
2018年06月29日 20:00:34[左直拳](https://me.csdn.net/leftfist)阅读数：1136
公司的网络分为内网和外网，二者不能同时上，只能切换。我用的是笔记本电脑，无线网络，每次切换，都要改IP，网关，连接不同的WIFI，特别的麻烦。
时间长，觉得不是办法，就上网搜了搜，为内网、外网分别写了2个脚本 `*.cmd` 。脚本内容包括：
```
1、修改网卡IP地址、子网掩码、网关、DNS
2、连接指定WIFI
```
当需要切换网络的时候，就运行相应脚本，一下子搞定，方便！
脚本如下：
1、内网：
```
//网卡名称：无线网络连接
//IP：192.168.1.112
//网关：192.168.1.1
netsh interface ipv4 set address name="无线网络连接" source=static gateway=192.168.1.1  gwmetric=1 addr=192.168.1.112  mask=255.255.255.0
//DNS:192.168.1.1
netsh interface ipv4 set dns name="无线网络连接" source=static addr=192.168.1.1
//WIFI名：LT_ZW
netsh wlan connect name=LT_ZW
```
2、外网
```
//DHCP,自动获取IP地址
netsh interface ip set address name="无线网络连接" source=dhcp
//自动获取DNS
netsh interface ip set dns name="无线网络连接" source=dhcp
//WIFI名：LT_WW
netsh wlan connect name=LT_WW
```
