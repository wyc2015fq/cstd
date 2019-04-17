# Linux下Wi-Fi的实现：wireless_tools和wpa_supplicant - DoubleLi - 博客园






平台为hi35XX，在Liunx下借助wireless_tools和wpa_supplicant（因为现在的[无线](http://www.07net01.com/wireless/)[WIFI](http://www.wredian.com/tags-WIFI-0.html)[网络](http://www.wredian.com/tags-%E7%BD%91%E7%BB%9C-0.html)大多是wpa加密。所以需要移植wpa_supplicant工具）等工具实现wifi的过程。



**一、移植wireless_tools**

**1. 下载并[编译](http://www.07net01.com/linux/)wireless_tools**

    tar zxf  wireless_tools.29.tar.gz(网上下载)

    cd wireless_tools.29

    make

    成功后会在当前目录生成iwspy，iwpriv，iwlist以及iwconfig等文件。

**2. 拷贝文件**

    我们只需将这四个文件拷备到文件系统的s[bin](http://www.07net01.com/linux/)目录，然后将当前目录的libiw.so.29库文件拷到文件系统的lib目录，[无线网](http://www.07net01.com/wlan/)络工具即安装完成。





**二、加载无线[网卡](http://www.07net01.com/r_s/)驱动模块**

    网上有很多无线驱动模块，可移植性很好。呵呵，我的是利用XX无线网卡公司提供的网卡驱动[源码](http://www.07net01.com/linux/)，修改下Makefile（1.网卡选择 2.平台  3.[内核](http://www.07net01.com/linux/)路径和工具链）以及os/[linux](http://www.07net01.com/linux/)下的config.mk（一般不需要修改），make得到的。

    insmod rtutil5572sta.ko。
    insmod rt5572sta.ko
    insmod rtnet5572sta.ko



**三、指令的使用**

**1.  扫描可用的[无线网络](http://www.07net01.com/tags-%E6%97%A0%E7%BA%BF%E7%BD%91%E7%BB%9C-0.html)**

    iwlistwifi-name scanning 

    其中wifi-name为无线网卡的名字，比如网卡**eth0就是系统默认的网卡名字**， wifi-name可以用ifconfig查看，**一般为ra0(无线网卡)，不过，需要ifconfig ra0 up**。

**2.  扫描网络信息**

    iwlistra0 scan

===>rt_ioctl_giwscan.11(11) BSS returned, data->length = 1427 

ra0       Scan completed : 

          Cell 01 - Address:C4:CA:D9:1D:9E:A0 

                    Protocol:802.11b/g/n 

                    ESSID:"" 

                    Mode:Managed 

                    Frequency:2.412 GHz(Channel 1) 

                    Quality=7/100  Signal level=-87 dBm  Noise level=-82 dBm 

                    Encryption key:off 

                    Bit Rates:54 Mb/s 

    如上：ESSID项的值即为无线网络的名字，如上的“TXJ”等。如果值为空，说明在无线[路由](http://www.07net01.com/route/)勾选了隐藏**ESSID**的选项。ESSID是很关键的，如果被隐藏了，也不是不能连接，而是需要破解，用到其他的工具，非常麻烦。

**3. 设置要连接的网络类型**

      iwprivra0 set [Network](http://www.07net01.com/tags-Network-0.html)Type=Infra 

**4. 设置要连接的无线网络的[安全](http://www.07net01.com/security/)模式**

      iwprivra0 set AuthMode=WPA2PSK 

**5. 设置网络加密方式：（CCMP即为AES）**

      iwprivra0 set EncrypType=TKIP  

**6. 设置连接时的密码**

      iwprivra0 set WPAPSK=*******  //********为加密网络的密码

**7. 连接该网络**：有**两种**方式：

**<1> 用iwpriv命令**

    iwpriv ra0 set SSID=TXJ

**<2> 用iwconfig命令**

    iwconfigra0 essid TXJ

    至此，如果密码正确，就可以连接上网络TXJ了。 

**8. 查看状态**

    iwpriv ra0 connStatus 

    当然，你也可以用iwconfig命令查看状态。





**四、wpa_supplicant**

**wpa_supplicant**本是开源[项目](http://www.07net01.com/tags-%E9%A1%B9%E7%9B%AE-0.html)源码，被[谷歌](http://www.07net01.com/tags-%E8%B0%B7%E6%AD%8C-0.html)修改后加入[android](http://www.07net01.com/tags-android-0.html)移动平台，它主要是用来支持WEP，WPA/WPA2和WAPI无线协议和加密认证的，而实际上的工作内容是通过socket（不管是wpa_supplicant与上层还是 wpa_supplicant与驱动都采用socket通讯）与驱动交互上报数据给用户，而用户可以通过socket发送命令给wpa_supplicant调动驱动来对WiFi芯片操作。 简单的说，wpa_supplicant就是WiFi驱动和用户的中转站外加对协议和加密认证的支持。



**wpa_supplicant**由网卡公司提供，呵呵，同样，网上也有很多相关资料，如下：

**1.生成wpa_supplicant（网上下载，本人未试）**

**1.1下载并解压wpa_supplicant需要的源码**

    tarxvfz wpa_supplicant-0.7.2.tar.gz

    tarxvfz wpa_supplicant-0.5.8.tar.gz

    tarzxvf openssl-0.9.8e.tar.gz

**1.2编译openssl库**

将wpa_supplicant中的补丁拷贝到openssl中：

       cp wpa_supplicant-0.7.2/patches/openssl-0.9.8e-tls-extensions.patch  openssl-0.9.8e/

建立openssl编译目标路径：

      mkdir /usr/local/ssl

修改openssl-0.9.8e目录下的makefile文件，修改内容如下：

    CC=arm-none-linux-gnueabi-gcc

    AR=arm-none-linux-gnueabi-ar

    RANLIB=arm-none-linux-gnueabi-ranlib

    INSTALLTOP=/usr/local/ssl

    OPENSSLDIR=/usr/local/ssl

然后编译openssl:

    make & make install

正常编译后，在/usr/local/ssl中将会出现ssl库。

**1.3编译wpa_supplicant**

    cd  wpa_supplicant-0.5.8

    cp  defconfig .config

在.config中添加如下[代码](http://www.07net01.com/program/)：

    CC=arm-none-linux-gnueabi-gcc-L /usr/local/ssl/lib/

    CFLAGS+= -I/usr/local/ssl/include/

    LIBS+= -L/usr/local/ssl/lib/

然后编译:

    make

 经过编译后的wpa_supplicant源[程序](http://www.07net01.com/tags-%E7%A8%8B%E5%BA%8F-0.html)可以看到两个主要的可执行工具：**wpa_supplicant和wpa_cli**。wpa_supplicant是核心程序，它和wpa_cli的关系就是服务和[客户端](http://www.07net01.com/tags-%E5%AE%A2%E6%88%B7%E7%AB%AF-0.html)的关系：后台运行wpa_supplicant，使用wpa_cli来搜索、设置、和连接网络。



**2.把RT2870STA.dat文件复制到/etc/Wireless/RT2870STA 目录下**

    1.1 mkdir -p /etc/Wireless/RT2870STA **//创建多层目录。**

    1.2 cp /mnt/rt5370/RT2870STA.dat  /etc/Wireless/RT2870STA



**3.首先关闭本地网卡，再启动wifi网卡**

    #ifconfig eth0 down (如果非同一ip段，不需要关闭)

    ifconfig ra0 up

    启动网卡后，**会不断出现错误**：BIRIdx(1): RXDMALen not multiple of 4.[43507], BulkInBufLen = 416)
    修改./MODULE/include/iface/rtmp_usb.h问题就能够解决掉。
    #define RXBULKAGGRE_SIZE  12 该为8 



**4.设置网卡参数**

    ifconfig ra0  192.168.2.188

    #ifconfig ra0  broadcast 192.168.2.255 (广播地址，默认)

    #ifconfig ra0  netmask 255.255.255.0（子网掩码，默认）



**5.编写wpa_supplicant.conf[配置](http://www.07net01.com/r_s/)文件**

    ctrl_[interface](http://www.07net01.com/tags-interface-0.html)=/var/run/wpa_supplicant

    network={

             ssid="TXJ"  //[路由器](http://www.07net01.com/tags-%E8%B7%AF%E7%94%B1%E5%99%A8-0.html)名称

             scan_ssid=1

             key_mgmt=WPA-EAP WPA-PSK IEEE8021XNONE

             pairwise=TKIP CCMP

             group=CCMP TKIP WEP104 WEP40

             psk="xxxxx" //密码

            }



**6.启动命令**

    mkdir -p /var/run/wpa_supplicant

    /mnt/rt5370/**wpa_supplicant** -Dwext-ira0 -c /etc/**wpa_supplicant.conf**



**7.命令查看状态**

  /mnt/rt5370/**iwpriv** ra0 connStatus   **//iwpriv文件放在/mnt/rt5370/目录下，所以，需要这样运行指令。**

  成功 ra0  connStatus:Connected(AP:TXJ[E0:05:C5:CF:F0:56])

**然后，就可以ping通192.168.2.1，说明连接上路由器了。那么，工作就完成了。**

**附：**

    -i<ifname>: 网络[接口](http://www.07net01.com/tags-%E6%8E%A5%E5%8F%A3-0.html)名称 

    -c<conf>:[配置文件](http://www.07net01.com/tags-%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6-0.html)名称

    -C<ctrl_intf>:控制接口名称

    -D<driver>:驱动类型

    -p<driver_param>:驱动参数

    -b<br_ifname>:桥接口名称





**rt5370支持WPS，****rt3070****不支持。**











