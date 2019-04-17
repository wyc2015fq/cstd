# Linux下的iwpriv（iwlist、iwconfig）的简单应用 - DoubleLi - 博客园






无线网络飞速发展的今天，许多设备都提供了连接无线网络的功能。

那么Linux下的wifi到底该怎么配置、连接呢？？

开始配置之前，我们要说说iw家族。iw是linux下常用的wifi配置工具，网上有相应的库和源码。全名为wirelessTools。

配置wifi模块，并连接相应的无线网络过程：主要使用iwpriv命令

1. 扫描可用的无线网络：



**[cpp]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwlist wifi-name scanning  


其中wifi-name为无线网卡的名字，比如网卡eth0就是系统默认的网卡名字， wifi-name可以用ifconfig查看，一般为ra0。

2. 看扫描到的网络信息，按要连接的网络类型进行配置。以下为扫描到的网络：



**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwlist ra0 scanning  
- ===>rt_ioctl_giwscan. 11(11) BSS returned, data->length = 1427  
- ra0       Scan completed :  
-           Cell 01 - Address: C4:CA:D9:1D:9E:A0  
-                     Protocol:802.11b/g/n  
-                     ESSID:""  
-                     Mode:Managed  
-                     Frequency:2.412 GHz (Channel 1)  
- Quality=7/100  Signal level=-87 dBm  Noise level=-82 dBm  
-                     Encryption key:off  
-                     Bit Rates:54 Mb/s  
-           Cell 02 - Address: FC:75:16:A1:A9:16  
-                     Protocol:802.11b/g/n  
-                     ESSID:"jxj_rd"  
-                     Mode:Managed  
-                     Frequency:2.452 GHz (Channel 9)  
- Quality=94/100  Signal level=-53 dBm  Noise level=-92 dBm  
-                     Encryption key:on  
-                     Bit Rates:54 Mb/s  
-                     IE: WPA Version 1  
-                         Group Cipher : TKIP  
-                         Pairwise Ciphers (2) : TKIP CCMP  
-                         Authentication Suites (1) : PSK  
-                     IE: IEEE 802.11i/WPA2 Version 1  
-                         Group Cipher : TKIP  
-                         Pairwise Ciphers (2) : TKIP CCMP  
-                         Authentication Suites (1) : PSK  
-           Cell 03 - Address: C4:CA:D9:02:2A:70  
-                     Protocol:802.11b/g/n  
-                     ESSID:""  
-                     Mode:Managed  
-                     Frequency:2.462 GHz (Channel 11)  
- Quality=83/100  Signal level=-57 dBm  Noise level=-92 dBm  
-                     Encryption key:off  
-                     Bit Rates:54 Mb/s  
-           Cell 04 - Address: 14:D6:4D:75:CA:9C  
-                     Protocol:802.11b/g/n  
-                     ESSID:"liangym"  
-                     Mode:Managed  
-                     Frequency:2.472 GHz (Channel 13)  
- Quality=100/100  Signal level=-27 dBm  Noise level=-92 dBm  
-                     Encryption key:on  
-                     Bit Rates:54 Mb/s  
-                     IE: WPA Version 1  
-                         Group Cipher : CCMP  
-                         Pairwise Ciphers (1) : CCMP  
-                         Authentication Suites (1) : PSK  
-                     IE: IEEE 802.11i/WPA2 Version 1  
-                         Group Cipher : CCMP  
-                         Pairwise Ciphers (1) : CCMP  
-                         Authentication Suites (1) : PSK  


如上：ESSID项的值即为无线网络的名字，如上的“jxj_rd”等。如果值为空，说明在无线路由勾选了隐藏ESSID的选项。

ESSID是很关键的，如果被隐藏了，也不是不能连接，而是需要破解，用到其他的工具，非常麻烦。至于如何破解，不是本文叙述的内容，有兴趣的朋友可以网上搜索。

得到了网络的信息，接下来就是配置连接选项了。以连接"jxj_rd"为例，我们看看具体的过程：

（1） 设置要连接的网络类型：



**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 set NetworkType=Infra  



（2） 设置要连接的无线网络的安全模式：



**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 set AuthMode=WPA2PSK  



（3） 设置网络加密方式：（CCMP即为AES）



**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 set EncrypType=TKIP  



（4） 设置连接时的密码： ********为加密网络的密码



**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 set WPAPSK=*******  


（5） 连接该网络：有两种方式：

    <1> 用iwpriv命令

> 


**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 set SSID=jxj_rd  



    <2> 用iwconfig命令

> 


**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwconfig ra0 essid jxj_rd  



至此，如果密码正确，就可以连接上网络jxj_rd了。如果你不放心，可以用命令查看状态：

> 


**[html]**[view plain](http://blog.csdn.net/liangyamin/article/details/7209761)[copy](http://blog.csdn.net/liangyamin/article/details/7209761)



- iwpriv ra0 connStatus  



当然，你也可以用iwconfig命令查看状态，这里不再赘述。









