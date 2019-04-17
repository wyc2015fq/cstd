# wifi详解（一） - DoubleLi - 博客园






# 1        WLAN技术

WLAN是英文WirelessLAN的缩写，就是无线局域网的意思。无线以太网技术是一种基于无线传输的局域网技术，与有线网络技术相比，具有灵活、建网迅速、个人化等特点。将这一技术应用于电信网的接入网领域，能够方便、灵活地为用户提供网络接入，适合于用户流动性较大、有数据业务需求的公共场所、高端的企业及家庭用户、需要临时建网的场合以及难以采用有线接入方式的环境等。

# 2        802.11协议简述

### 2.1.1       概述

作为全球公认的局域网权威，IEEE802工作组建立的标准在过去二十年内在局域网领域独领风骚。这些协议包括了802.3Ethernet协议、802.5TokenRing协议、802.3z100BASE－T快速以太网协议。在1997年，经过了7年的工作以后，IEEE发布了802.11协议，这也是在无线局域网领域内的第一个国际上被认可的协议。

在1999年9月，他们又提出了802.11b"HighRate"协议，用来对802.11协议进行补充，802.11b在802.11的1Mbps和2Mbps速率下又增加了5.5Mbps和11Mbps两个新的网络吞吐速率。利用802.11b，移动用户能够获得同Ethernet一样的性能、网络吞吐率、可用性。这个基于标准的技术使得管理员可以根据环境选择合适的局域网技术来构造自己的网络，满足他们的商业用户和其他用户的需求。802.11协议主要工作在ISO协议的最低两层上，并在物理层上进行了一些改动，加入了高速数字传输的特性和连接的稳定性。

主要内容：
　　1.802.11工作方式
　　2.802.11物理层
　　3.802.11b的增强物理层
　　4.802.11数字链路层
　　5.联合结构、蜂窝结构和漫游

# 3        802.11四种主要物理组件

## 3.1      工作站（Station）

构建网络的主要目的是为了在工作站间传送数据。所谓工作站，是指配备无线网络接口的计算设备。

## 3.2      接入点（Access Point）

802.11网络所使用的帧必须经过转换，方能被传递至其他不同类型的网络。具备无线至有线的桥接功能的设备称为接入点，接入点的功能不仅于此，但桥接最为重要。

## 3.3      无线媒介（Wireless Medium）

802.11标准以无线媒介在工作站之间传递帧。其所定义的物理层不只是一种，802.11最初标准化了两种射频物理层以及一种红外线物理层。

## 3.4      分布式系统（Distribution System）

当几个接入点串联以覆盖较大区域时，彼此之间必须相互通信以掌握移动式工作站的行踪。分布式系统属于802.11的逻辑组件，负责将帧转送至目的地。

下图为802.11网络的基本服务集（basic service set），其中包含了这四种物理组件。





# 4        WIFI适配层

里面定义很多字符串变量和适配层的接口实现，是对wpa_supplicant程序通信的接口封装，用来完成上层和wpa_supplicant的通信， 头文件在libhardware/include/hardware下，这里的函数用来向JNI的本地实现提供调用接口。

这里的函数，我把它们分为四类函数：

一类是命令相关的（控制）函数，就是在JNI层android_XXX_Command()函数所调用 的Wifi_Command()函数，调用流程如下：

android_XXX_command()=>docommand()=>wifi_command()=>wifi_send_command()=>wpa_ctrl_require()。

二类是 监听函数，即Wifi_wait_for_event()函数，调用流程如下：android_net_wifi_Waitforevent()=>wifi_wait_for_event()=>wpa_ctrl_recv()。

三就是WPA_SUPPLICANT的启动，连接，关闭函数

四是驱动的加载和卸载函数

# 5        wpa_supplicant

## 5.1      wpa_ctrl的作用

定义了两类套接字和一个管道，并分别实现了和wpa_supplicant的通信，而在实际的实现中采用的都是套接字的方式，因此wpa_supplicant适配层和wpa_supplicant层 是通过socket通讯的。

要是从wifi.c中真的很难看出它和wpa_supplicant有什么关系，和它联系密切的就是 这个wpa_ctrl.h文件，这里面定义了一个类wpa_ctrl，这个类中声明了两个Socket套接口，一个是本地一个是要连接的套接 口，wpa_ctrl与wpa_supplicant的通信就需要socket来帮忙了，而wpa_supplicant就是通过调用 wpa_ctrl.h中定义的函数和wpa_supplicant进行通讯的，wpa_ctrl类（其实是其中的两个socket）就是他们之间的桥梁。

## 5.2      WPA_SUPPLICANT

### 5.2.1       概念

wpa_supplicant本是开源项目源码，被谷歌修改后加入android移动平台，它主要是用来支持WEP，WPA/WPA2和WAPI无线协议和加密认证的，而实际上的工作内容是通过socket（不管是wpa_supplicant与上层还是wpa_supplicant与驱动都采用socket通讯）与驱动交互上报数据给用户，而用户可以通过socket发送命令给wpa_supplicant调动驱动来对WiFi芯片操作。 简单的说，wpa_supplicant就是WiFi驱动和用户的中转站外加对协议和加密认证的支持。

### 5.2.2       Wpa_supplicant与驱动的交互

#### 5.2.2.1        wpa_supplicant.c

首先定义一个驱动操作数组externstructwpa_driver_ops *wpa_supplicant_drivers[]，然后是系列wpa_supplicant_XXX()函数，很多函数里面调用 wpa_drv_XXX()函数，这些函数是wpa_supplicant_i.h中实现的函数。几乎每个函数都需要一个wpa_supplicant结 构，对其进行所有的控制和通信操作。

#### 5.2.2.2        Wpa_supplicant_i.h

其中定义了一个重要数据结构wpa_supplicant，其中有一个重要的driver成 员，它是wpa_driver_ops类型，可以被用来调用抽象层的接口。接下来是系列函数声明，这些函数声明在wpa_supplicant.c中实现，然后就是wpa_drv_XXX函数，这些函数就是在 wpa_supplicant.c中被wpa_supplicant_xxx函数调用的，而这些wpa_drv_xxx函数也都有一个 wpa_supplicant结构的变量指针，用来调用封装的抽象接口，而这些抽象接口的实现在driver_wext.c中（如果使用的汉斯WEXT驱动）。

这里要注意的是：在wpa_suppliant.c文件中定义的很多函数是在该头文件中声明的，而不是在wpa_supplicant.h中声明的。

#### 5.2.2.3        Driver_wext.c

对wpa_drvier_ops的个函数的具体实现，该结构指针在wpa_supplicant注册一个网络接口时会被初始化赋予指定的操作指针，wpa_supplicant.c中的wpa_supplicant_xxx函数通过wpa_supplicant结构中的该操作指针调用WEXT的实现接口。

就是在该文件中，创建了三个socket：ioctrl_socket，event_socket和mlme_socket，它们分别有自己的用途，如ioctrl_socket用于发送控制命令，event_socket用于监听驱动传来的event事件等。Wpa_supplicant通过这三个socket与wifi驱动关联，这里的socket同fd（文件描述符）类似。

# 6        Wpa_cli调试工具

## 6.1      启动wpa_supplicant

使用下面命令启动wpa_supplicant:

**wpa_supplicant-Dwext -iwlan0 -C/data/system/wpa_supplicant-c/data/misc/wifi/wpa_supplicant.conf**

 为了确保wpa_supplicant真的启动起来了，使用“ps”命令查看。

## 6.2        连接wpa_cli到wpa_supplicant

**wpa_cli-p/data/system/wpa_supplicant -iwlan0**

      然后，就可以使用wpa_cli调试工具进行wifi调试了，下面列出了一些常用的调试命令：

      >**scan**                            //扫描周围的AP

      >**scan_results**        //显示扫描结果

      >**status**                   //显示当前的连接状态信息

      >**terminate**             //终止wpa_supplicant

      >**quit**                        //退出wpa_cli

      >**add_network**       //返回可用network id

      >**set_network<network id> <variable> <value>** //设置网络

      >**select_network<network id>**          //选择网络，禁用其它网络

      >**disable_network<network id>**  //禁用网络

      >**enable_network<network id>**        //启用网络

## 6.3      示例

### 6.3.1       无密钥认证AP

             >add_network      (返回可用networkid, 假定返回0)

             >set_network 0 ssid “666”

             >set_network 0 key_mgmt NONE

             >enable_network 0

             >quit

如果上面的操作正确，我们会连接到一个AP，它的SSID为“666”，现在需要一个IP来访问internet：

             dhcpcd wlan0

成功获取IP后，即可连上internet。

### 6.3.2       WEP认证AP

             >add_network      (假设返回1)

             >set_network 1 ssid “666”

             >set_network 1 key_mgmt NONE

             >set_network 1 wep_key0 “ap passwork”

             >set_network 1 wep_tx_keyidx 0

             >select_network 1     （如果你已经连上了其它的AP，那么就需要这个命令来禁用其它的网络）

             >enable_network 1

       然后同上获取IP，连接到internet上。

### 6.3.3       WPA-PSK/WPA2-PSK认证AP

             >add_network      (假定返回2)

             >set_network 2 ssid “666”

             >set_network 2 psk “your pre-shared key”

             >select_network 2 

             >enable_network 2

还有其它的命令进一步设置网络，不过wpa_supplicant已经给了我们一些默认的配置。

### 6.3.4        隐藏藏AP

原则上应该只要在上面的基础上去set_network netid scan_ssid 1即可，测试过无加密的Hidden AP，WEP/WPA/WPA2应该道理一样。









