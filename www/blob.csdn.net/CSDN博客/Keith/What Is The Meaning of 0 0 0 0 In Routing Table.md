# What Is The Meaning of 0.0.0.0 In Routing Table? - Keith - CSDN博客





2014年03月15日 23:30:57[ke1th](https://me.csdn.net/u012436149)阅读数：522









# What Is The Meaning of 0.0.0.0 In Routing Table?


by [NIX
 CRAFT](http://www.cyberciti.biz/tips/about-us) on JULY 16, 2009 · [1
 COMMENT](http://www.cyberciti.biz/faq/linux-unix-osx-bsd-windows-0-0-0-0-network-address/#comments)· LAST UPDATED JULY
 16, 2010


in [NETWORKING](http://www.cyberciti.biz/faq/category/networking/)



![](http://c.cyberciti.biz/cbzcache/3rdparty/networking.png)

My home Linux laptop is connected to ADSL router and whenever I run route -n command at the prompt it
 says 0.0.0.0 is set to my routers IP 192.168.1.1. What is the meaning of 0.0.0.0?


Each network host has a default route for each network card. This will create a 0.0.0.0 route for such card. The address 0.0.0.0 generally means "any address". If a packet destination doesn't match an individual address in the table, it must match a 0.0.0.0
 gateway address. In other words, default gateway is always pointed by 0.0.0.0:

```
route
 -n
```


Sample outputs:
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
10.0.31.18      0.0.0.0         255.255.255.255 UH    0      0        0 ppp0
38.96.196.94    192.168.1.2     255.255.255.255 UGH   0      0        0 wlan0
192.168.84.0    0.0.0.0         255.255.255.0   U     0      0        0 vmnet8
192.168.1.0     0.0.0.0         255.255.255.0   U     2      0        0 wlan0
192.168.110.0   0.0.0.0         255.255.255.0   U     0      0        0 vmnet1
192.168.122.0   0.0.0.0         255.255.255.0   U     0      0        0 virbr0
169.254.0.0     0.0.0.0         255.255.0.0     U     1000   0        0 wlan0
10.0.0.0        0.0.0.0         255.0.0.0       U     0      0        0 ppp0
0.0.0.0         192.168.1.2     0.0.0.0         UG    0      0        0 wlan0


192.168.1.2 is my ISP router i.e. default gateway indicated by 0.0.0.0. Other destination such as 10.0.0.0/8 network use ppp0 interface as gateway. If you are MS-Windows user use route print command.





from:http://www.cyberciti.biz/faq/linux-unix-osx-bsd-windows-0-0-0-0-network-address/




