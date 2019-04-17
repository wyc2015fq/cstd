# wpa_supplicant使用笔记 - DoubleLi - 博客园






还在搞8634，看不到头了。。唉。wireless_tools的缺点是不支持WPA认证，所以有必要把wpa_supplicant也移植过来。无线 网卡是基于zydas芯片的，正好可以在网上搜到wpa_supplicant的zydas专用包：wpa_supplicant- 0.4.7_zydas_pre3.tar.gz

    移植很简单，解包直接修改Makefile，把CC改成mipsel-linux-gcc，然后编译直接通过。编译完成后可以发现有几个可执行程 序：wpa_supplicant，wpa_cli等。wpa_supplicant是核心程序，它和wpa_cli的关系就是服务和客户端的关系：后台 运行wpa_supplicant，可以使用wpa_cli来搜索、设置、和连接网络等，大部分情况下编辑好配置文件，运行wpa_supplicant 就可以连通网络了，创建一个wpa_supplicant.conf，键入以下内容：
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

*bash# insmod zd1211b.kobash# ifconfig eth1 up*
*bash# wpa_supplicant -B -ieth1 -Dzydas -c/etc/wpa_supplicant.conf*

   参数-B代表后台运行，-ieth1代表使用设备eth1，-Dzydas代表网卡芯片是zydas，-c指定配置文件。
   接下来就可以看下网络是否连通了：

*bash# iwconfigbash# udhcpc -i eth1 -n以下 ： From http://blog.chinaunix.net/u1/38994/showart_1161421.html**bash# wpa_cli -ieth1 scan //搜索无线网bash# wpa_cli -ieth1 scan_results //显示搜索结果*
*bash# wpa_cli -ieth1 add_networkbash# iwconfig eth1 essid "you_wifi_net"bash# wpa_cli -ieth1 password 0 "password"bash# wpa_cli -ieth1 enable_networkluther.gliethttp add here-ieth1也是**wpa_cli和**wpa_supplicant进行unix方式进行进程见通信的文件名.*
*wpa_supplicant执行时会在*/data/system/wpa_supplicant**目录下创建***eth1文件,所以就能unix通信了*
下载地址：http://hostap.epitest.fi/releases/








