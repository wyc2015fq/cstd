# wifi配置常用命令总结 - DoubleLi - 博客园





1:iwlist eth1 scanning 查看无线路由 2:iwconfig eth1 essid "无线路由的名称" 3: ifconfig eth1 IP 4: route add default gw 网关对于带密码的路由器,设置如下:1:iwconfig eth1 key s:密码2:iwconfig eth1 key open3:ifconfig eth1 essid "名称"4:ifconfig eth1 IP5:route add default gw 网关


还在搞8634，看不到头了。。唉。wireless_tools的缺点是不支持WPA认证，所以有必要把wpa_supplicant也移植过来。无线网卡是基于zydas芯片的，正好可以在网上搜到wpa_supplicant的zydas专用包：wpa_supplicant-0.4.7_zydas_pre3.tar.gz

    移植很简单，解包直接修改Makefile，把CC改成mipsel-linux-gcc，然后编译直接通过。编译完成后可以发现有几个可执行程序：wpa_supplicant，wpa_cli等。wpa_supplicant是核心程序，它和wpa_cli的关系就是服务和客户端的关系：后台运行wpa_supplicant，可以使用wpa_cli来搜索、设置、和连接网络等，大部分情况下编辑好配置文件，运行wpa_supplicant就可以连通网络了，创建一个wpa_supplicant.conf，键入以下内容：

#可有可无，没有的话无法用wpa_cli terminate来关闭wpa_supplicant
ctrl_interface=/var/run/wpa_supplicant

# 确保只有root用户能读取WPA的配置，可有可无，尤其是在嵌入式设备上
ctrl_interface_group=0

# 使用wpa_supplicant来扫描和选择AP，也可以设置成0或2，但只有1才能正确工作
ap_scan=1

network={
#AP的名字
   ssid="my_network"
#AP的key
   psk="my_key"
}

配置文件弄好后就可以运行wpa_supplicant了。在那之前当然要先插好无线网卡，载入驱动：

*bash# insmod zd1211b.kobash# ifconfig eth1 upbash# wpa_supplicant -B -ieth1 -Dzydas -c/etc/wpa_supplicant.conf*

   参数-B代表后台运行，-ieth1代表使用设备eth1，-Dzydas代表网卡芯片是zydas，-c指定配置文件。
   接下来就可以看下网络是否连通了：

*bash# iwconfigbash# udhcpc -i eth1 -n*zd1211b的驱动使用wpa_supplicant来配置的时候会有隔段时间就锁死系统一会的情况，很奇怪。









1. 此无线网络不设置安全机制，换句话说，么有设置密码，OK，我们该怎么做呢？命令行伺候

# iwlist scanning            # 搜索无线网络，我们假设搜索到了 oceanboo-wireless
# iwconfig interface essid oceanboo-wireless 
# dhcpcd interface


够简单了吧，这样，你的机器就连接上 oceanboo-wireless 这个无线网络了，dhcp 获得了 ip 地址。不过不给无线路由器设置密码是很愚蠢的做法，假如你不是开咖啡厅、快餐店，还是设置一个密码吧。

2. 此无线网络设置了安全机制，使用 WEP 加密认证。

# iwlist scanning           # 还是一样的搜索，记住这个命令。
# iwconfig  interface essid oceanboo-wireless key XXXX-XXXX-XXXX-XXXX or XXXXXXXX.   # XXX多了点，不过如果你设置过无线路由，你一定知道这是啥玩意，我就不多说了。
# dhcpcd interface


哈，还是一样的简单。
关于 WEP 认证加密的输入还有下列这些方式，按需要索取吧。

引用

# iwconfig interface key 0123-4567-89
# iwconfig interface key [3] 0123-4567-89
# iwconfig interface key s:password [2]
# iwconfig interface key [2]
# iwconfig interface key open
# iwconfig interface key off
# iwconfig interface key restricted [3] 0123456789
# iwconfig interface key 01-23 key 45-67 [4] key [4]



AP基本命令

iwconfig配置接口

wlanconfig创建或销毁VAP

iwpriv设置属性

iwlist搜索无线热点

brctl网桥操作



笔记本的无线网卡搞了好久都没弄好，最后还是madwifi好使   O(∩_∩)O哈哈~

给大家分享下

madwifi配置wep atheros无线网卡

从madwifi.org获取包后

 tar -xzvf madwifi-0.9.4.tar.gz        #最新的0.9.4包编译老是报错，就换0.9.3的包了……
cd madwifi-0.9.4
make
make install
modprobe ath_pci            #无线模块支持
modprobe wlan_scan_sta      #无线扫描支持
wlanconfig ath0 destroy     #取消所有ath0配置，可能多此一举
wlanconfig ath0 create wlandev wifi0 wlanmode sta
iwconfig ath0 key xxxx      #wep password,如果用ascii则s:xxxxx
iwconfig ath0 essid "xxxx"  #广播域
ifconfig ath0 up
dhclient ath0               #or route add xxxxx

如果加密方式为wpa则需要用wpa_supplicant来配置，需要写配置文件。继续研究









