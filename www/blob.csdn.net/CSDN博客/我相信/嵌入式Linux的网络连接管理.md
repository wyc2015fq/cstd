# 嵌入式Linux的网络连接管理 - 我相信...... - CSDN博客





2018年03月06日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1512









连接管理器(ConnMan)是一个连接管理守护进程 , 用于管理运行 Linux 操作系统中设备的互联网连接。 它以快速、连贯、同步的方式对不断变化的网络条件提供了低内存消耗。

ConnMan拥有各种插件, 是一个完全模块化的系统, 可以扩展支持各种有线或无线技术，允许对各种用例进行简单的适配和修改。 例如DHCP 和DNS 这样的配置方法就是使用插件实现的。所有的应用程序都使用 D-Bus 来与 ConnMan 通信，与嵌入式 Yocto 构建系统一起使用, 是 GENIVI 规范的一部分。

ConnMan遵循 GPL v2.0许可， 英特尔是该项目的主要贡献者。其内部架构如下：

![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2jt4uX4qbnn3uhkiaB6CFN6td8V4gjD4tjcia4IpNDTnPVXg8vbN0pCcQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)

图1

## 功能特性

以下特性嵌入到ConnMan中:
- 
通用插件基础结构

- 
设备和网络抽象(带有基本存储支持)

- 
IPv4, IPv4-LL (link-local) 和DHCP以及IPv6, DHCPv6，V6到V4的 隧道等

- 
高级路由和 DNS 配置

- 
内置 DNS 代理和智能缓存

- 
内置的 WISPr 热点登录和门户检测

- 
时区配置(手动和自动的 NTP)

- 
代理处理(手动和自动的WPAD)

- 
网络支持(USB, 蓝牙和 WiFi AP 模式)

- 
详细统计处理(家居及漫游)


可以启用各种插件来支持网络:
- 
以太网插件

- 
Wifi 插件 WEP40 / WEP128及 WPA/WPA2(个人及企业)

- 
蓝牙插件(使用 BlueZ)

- 
2g / 3g / 4g 插件(使用 oFono)


还提供了附加功能的插件:
- 
回路接口设置

- 
PACrunner 代理

- 
PolicyKit 授权工具


## D-Bus接口

应用程序都使用 D-Bus 来与 ConnMan 通信，通过管理器对象来实现应用入口。管理器对象包含服务对象的引用(接口)，其他接口包括 VPN、时钟、任务、技术、会话、通知、对等、代理和计数器等。 

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2kQWRn7CkicRiaYXnQ66ia933Q2viba95ibpjbrQ1FFsqEsicNKeDSD52uFnA/640?wx_fmt=png)

图2

### D-Bus Manager 接口

D-Bus Manager 解释了所有的方法和属性。 

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib28SwMXn8VjE43wXBDnoQ5wdSEPiaoM6UJ8NticzoKhPUIzaESb6WbHz9A/640?wx_fmt=png)

图3 




检索所有全局属性的简单方法如下:

```
bus = dbus.SystemBus()
manager = dbus.Interface(bus.get_object("net.connman", "/"), "net.connman.Manager")
properties = manager.GetProperties()
```

改变一个全局属性也非常简单。 例如, 启用所谓的飞行模式(AKA flight mode)可以以设置该属性: manager.SetProperty ("OfflineMode", dbus.Boolean (1))。 

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2BRQ8nibTXUVwiaoC494O19KkLBYmtsW8YbSQEvxoUqYIliaY22AkdsvXg/640?wx_fmt=png)

图4

### 服务接口

高级用户接口易于访问网络细节和用户选择的偏好。 这是服务列表和接口（图5）， 以一个单一的平面和排序列表来维护所有可用的服务。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2fKeCx9nlxnEyTFpnbcjaGwrI7xRJSyNj08r1rHGicSlGrZgvaLg46hA/640?wx_fmt=png)

图5

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2xKQcEcRwKZTzvNmv31kxvFicleMSar9ZSFnXXQialcRVia9brffSsIbrw/640?wx_fmt=png)

图6

### 服务状态

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2YtIuSGp47eJDAAgPCOkXngdSNtbQmNkt69iabjtSDic0sX4xKKjNNYCA/640?wx_fmt=png)

图7
- 
每个服务的基本状态都是"空闲"，这意味着这项服务目前根本没有使用。 它也不是试图连接或做任何其他事情。

- 
"关联"状态表明该服务试图建立与网络的低级连接 -- 例如, 与 WiFi 接入点相关联/连接。

- 
在"配置"状态下, 表示服务正在尝试检索/配置 IP 设置。

- 
"准备好"状态标志着一个成功连接的设备，并不意味着它有默认的路由, 但是基本的 IP操作将会成功。

- 
"断开"状态，表明服务将终止当前连接并返回到"空闲"状态。

- 
"失败"状态表明一种错误的行为，类似于"空闲"状态, 服务没有连接。


### 服务的使用

下面是如何获得一个服务对象:
`service = dbus.Interface(bus.get_object("net.connman", path), "net.connman.Service")`
它还包含连接或断开特定服务的方法，ConnMan可以根据策略或通过外部事件(如在以太网电缆中插入)自动连接服务。



![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib2D8Ml6ByZJPGrd853xZQjpHKNcHxnoDtqKSbR867BL5KLsW1tkiaMwew/640?wx_fmt=png)

图8

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib25uGrzC96dEIibUIIIltRLtVmzAHE1aaSBIFcAztzj8OLsASibVmBicwxQ/640?wx_fmt=png)

图9

### 技能接口

基本的设备配置任务是通过技能接口完成的，例如, 通过技术接口来切换设备(例如通过 RFKILL)。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqHAF50t5L7oPyyalmdhuib21oFMiatOzVFjWCb5UTH74fGq6pEOZ4e8WDBeicAiciaKKnXloBbFY2l4Rw/640?wx_fmt=png)

图10

用户可以通过 OfflineMode 属性激活离线(飞行)模式。 在离线模式下, 包括以太网在内的所有技能都被关闭。 在离线模式下, 用户可以通过使用 rfkill 命令来临时激活单个技能。

## ConnMan 的一些细节

可以通过命令行参数来启动ConnMan：

```
-c, --config=FILE             Load the specified configuration file instead of /usr/local/etc/connman/main.conf
-d, --debug=DEBUG             Specify debug options to enable
-i, --device=DEV              Specify networking device or interface
-I, --nodevice=DEV            Specify networking interface to ignore
-p, --plugin=NAME,...         Specify plugins to load
-P, --noplugin=NAME,...       Specify plugins not to load
-W, --wifi=NAME               Specify driver for WiFi/Supplicant
-n, --nodaemon                Don't fork daemon to background
-r, --nodnsproxy              Don't enable DNS Proxy
--nobacktrace                 Don't print out backtrace information
```

使用示例如下：
`connmand -i wlan0 -I eth0 --nodnsproxy --nodaemon >& connman.log`
主配置文件（main.conf)如下:

```
InputRequestTimeout = 120 (default)---------> input request ( ex. passphrase) timeout
BrowserLaunchTimeout = 300 ( default) -----> The request for launching a browser for portal pages
BackgroundScanning = true --------------------> option for background scanning
FallbackTimeservers = --------------------------> List of fallback timeservers ( used by NTP sync) separated by ","
FallbackNameservers = -------------------------> List of fallback nameservers
DefaultAutoConnectTechnologies = -----------> List of technologies that are marked autoconnectable by default.
PreferredTechnologies = ------------------------> List of preferred technologies from - most preferred one to least preferred
NetworkInterfaceBlacklist = vmnet,vboxnet,virbr,ifb,ve-,vb-   ---> List of blacklisted network interfaces 
AllowHostnameUpdates = true -----------------> Allow connman to change the system hostname ( ex. dhcp hostname option)
SingleConnectedTechnology = false -----------> Keep only a single connected technology at any time
TetheringTechnologies = wifi,bluetooth,gadget
PersistentTetheringMode = false --------------> Restore earlier tethering status when returning from offline mode
Enable6to4 = false ------------------------------> Automatically enable Anycast 6to4 if possible (not recommended. see RFC6343 )
EnableOnlineCheck = false --------------------> Enable use of http get as on online status check
AlwaysConnectedTechnologies = -------------> List of technologies with AutoConnect = true which are always connected regardless of PreferredTechnologies setting
```

ConnMan 使用配置文件来提供现有的服务，在 /var/lib/Connman/目录下 。 配置文件名不能包含字母或数字以外的其他字符, 并且必须有一个 Config后缀. 例如：

```
Ex.      #cat /var/lib/connman/example.config
[global]
Name = Example
Description = Example network configuration

[service_home_ethernet]
Type = ethernet
IPv4 = 192.168.1.42/255.255.255.0/192.168.1.1
IPv6 = 2001:db8::42/64/2001:db8::1
MAC = 01:02:03:04:05:06
Nameservers = 10.2.3.4,192.168.1.99
SearchDomains = my.home,isp.net
Timeservers = 10.172.2.1,ntp.my.isp.net
Domain = my.home

[service_home_wifi]
Type = wifi
Name = my_home_wifi
Passphrase = secret
IPv4 = 192.168.2.2/255.255.255.0/192.168.2.1
MAC = 06:05:04:03:02:01
```

设置和配置文件是为用户经常连接的网络而自动创建的，例如：

```
# cat /var/lib/connman/settings 

[global]

OfflineMode=false


[WiFi]

Enable=true

Tethering=false


[Bluetooth]

Enable=false

Tethering=false
```

Vpn 设置可以在 /var/lib/connman-VPN/ 。

## 写一个插件

基本插件包含通过 CONNMAN 定义的插件描述, 以及通过该描述定义的 init / exit 回调。例如：

```
#include <connman/plugin.h>

static int example_init(void)
{
    return 0;
}

static void example_exit(void)
{
}

CONNMAN_PLUGIN_DEFINE(example, "Example plugin", CONNMAN_VERSION,
                        example_init, example_exit)
```

插件通过技术、网络和设备以及其他基础结构与ConnMan的核心功能相互作用。 以下是技术基础设施:

```
ex. bluetooth plugin
    static struct connman_technology_driver tech_driver = {
        .name           = "bluetooth",
        .type           = CONNMAN_SERVICE_TYPE_BLUETOOTH,
        .probe          = bluetooth_tech_probe,
        .remove         = bluetooth_tech_remove,
        .set_tethering  = bluetooth_tech_set_tethering,
    };
```

为了让连接管理器了解新的插件, 需要通过调用ConnMan驱动注册器来注册它的驱动程序。
`connman_technology_driver_register(&tech_driver);`
### 设备

设备代表给定技术的真实设备，每种技术都可能有很多设备。

```
static struct connman_device_driver device_driver = {
        .name           = "bluetooth",
        .type           = CONNMAN_DEVICE_TYPE_BLUETOOTH,
        .probe          = bluetooth_device_probe,
        .remove         = bluetooth_device_remove,
        .enable         = bluetooth_device_enable,
        .disable        = bluetooth_device_disable,
    };
```

注册驱动程序:
`connman_device_driver_register(&device_driver);`
此外, 需要为每个插件编写处理新设备的检测代码; 蓝牙插件通过为 BlueZ-d-bus 接口来注册观察者来实现。 一旦新的蓝牙设备出现, 插件需要通过调用 ConnMan 设备创建来通知 ConnMan 内涵。 对于蓝牙插件, 这个调用将是:

```
struct connman_device *device;
device = connman_device_create("bluetooth", CONNMAN_DEVICE_TYPE_BLUETOOTH)
```

### 网络

连接管理器为插件提供了一种手段, 用于处理为每种技术建立 / 处理连接的细节。 对于蓝牙插件, 需要注册一个connman*network*driver:

```
static struct connman_network_driver network_driver = {
        .name           = "bluetooth",
        .type           = CONNMAN_NETWORK_TYPE_BLUETOOTH_PAN,
        .probe          = bluetooth_pan_probe,
        .remove         = bluetooth_pan_remove,
        .connect        = bluetooth_pan_connect,
        .disconnect     = bluetooth_pan_disconnect,
    };
```

然后, 调用注册函数:
`connman_network_driver_register(&network_driver);`
### 源文件的结构解读

初始化函数将在 src/main.c 中在运行主循环之前调用。

```
src/util.c - /dev/urandom support
src/inotify.c - used by the core (config.c) and the session policy plugin
src/technology.c – used for technology interface
src/notifier.c – basic notifier infrastructure
src/agent.c – agent interface
src/service.c – used for service interface
src/peer_service.c – p2p peer service API
src/peer.c – p2p peer object’s core logic
src/provider.c – provider infrastructure
src/network.c – network infrastructure used by plug-ins
src/config.c – framework for configuration files
src/device.c – device infrastructure used by plug-ins
src/iptables.c – iptables support (netfilter chains and rules)
src/firewall-iptables.c, nftables.c (older firewall.c) – firewall infrastructure.
src/nat.c – used for NAT
src/tethering.c  – tethering infrastructure
src/manager.c – Manager interface
src/stats.c – used for storing service stats ( mmaped into memory)
src/ipconfig.c – IP configuration framework
src/rtnl.c – netlink support
src/session.c – Allows 3rd party applications to request a network session 
src/resolver.c – resolver framework
src/dhcp.c, dhcpv6.c – dhcp framework
src/rfkill.c – rfkill interface support
src/machine.c -  basic systemd-hostnamed ( machine-type- “chassis”) support
```

### 命令行客户端

ConnMan有一个标准的命令行客户端 connmanctl。 它可以有两种运行模式:
- 
在命令行模式中, 命令输入作为对 connmanctl 命令的参数, 就像 systemctl 一样

- 
交互式模式是在没有参数的情况下键入 connmanctl 来开始的。 将改变为 connmanctl 表示它正在等待用户命令, 就像 python 交互模式一样


connmanctl示例：

1.启用和禁用 WiFi

```
$ connmanctl technologies - check for the line that says Powered: True/False. 
    $ connmanctl enable wifi - To power the wifi on.
    $ connmanctl disable wifi – To power off the wifi.
```

2.连接到一个开放的访问点

本示例的命令显示如何在命令行模式下运行控制器。 为了扫描网络, connmanctl 接受简单的技术名参数。 扫描附近的 WiFi 网络:

```
$ connmanctl scan wifi To list the available networks found after a scan run (example output): 
$ connmanctl services 
*AO MyNetwork wifi_dc85de828967_68756773616d_managed_psk OtherNET wifi_dc85de828967_38303944616e69656c73_managed_psk AnotherOne wifi_dc85de828967_3257495245363836_managed_wep FourthNetwork wifi_dc85de828967_4d7572706879_managed_wep AnO6penNetwork wifi_dc85de828967_4d6568657272696e_managed_none 
$ connmanctl connect wifi_dc85de828967_4d6568657272696e_managed_none
```

3.连接到受保护的AP

对于受保护的AP, 提供一些信息给 ConnMan 守护进程, 例如一个密码。

```
$ connmanctl
connmanctl> scan wifi ----- To list services: 
connmanctl> services 
connmanctl> agent on --- register the agent to handle user requests.
connmanctl> connect wifi_dc85de828967_38303944616e69656c73_managed_psk
```

代理需要根据所连接的网络类型提供更多信息，代理还将打印关于它所需要信息的额外数据, 如下面的示例所示。
`Agent RequestInput wifi_dc85de828967_38303944616e69656c73_managed_psk Passphrase = [ Type=psk, Requirement=mandatory ] Passphrase?`
提供完请求的信息后, 然后键入 quit 退出。如果提供的信息是正确的, 那现在应该就连接到受保护的AP了。

各种硬件接口被在ConnMan中称为技术，要列出可用的技术, 可以运行:
`$ connmanctl technologies`
那些技术可以通过以下方式进行:

```
$ connmanctl enable technology_type

$ connmanctl disable technology_type
```

例如, 切换 WiFi:
`$ connmanctl disable wifi`
## 测试与调试

ConnMan同时也提供了基于 python 的测试脚本(ex. test-connman, list-services 等)。 通过这些脚本支持连接、断开、自动连接、服务、扫描、启用、禁用、非线性、状态等基本操作。

可以使用-d 命令行选项在 ConnMan 中激活调试打印。

```
-d Activate all normal debug prints 
-d src/service.c This prints debugging info from src/service.c file only
-d src/network.c:src/ipconfig.c This activates debug prints in src/network.c and src/ipconfig.c files. 
-d 'src/n*.c' This would activate debug print from all the C source files starting with letter 'n' in src directory. Note the quotation marks around option, that is to prevent shell expansion. 
-d '*/n*.c:*/i*.c' Activate debug prints for all C source files starting with letters 'n' or 'i' in any sub-directory.
```

Connman 的一些组件有基于环境变量启动的调试版本。 如果环境变量已设置, 则相应组件将打印一些额外的调试信息。可以使用下列环境变量:

```
* CONNMAN_DHCP_DEBUG: Dhcpv4相关调试信息
* CONNMAN_DHCPV6_DEBUG:  DHCPv6相关调试信息
* CONNMAN_IPTABLES_DEBUG: 使用 iptables 时的额外信息
* CONNMAN_RESOLV_DEBUG: 名称解析器调试打印。 这些调试信息用于当 ConnMan 解析主机名称以供自己使用。 请注意, DNS 代理调试版本不使用此环境变量。 为此, 我们可以使用"-d src / dnsproxy。 "c"命令行选项
* CONNMAN_SUPPLICANT_DEBUG :调试打印, 用于控制和 wpa 拼接过程之间的通信
* CONNMAN_WEB_DEBUG: 当 ConnMan 在 Wispr 进行互联网连接检查时的调试信息
```

例如：
`CONNMAN_SUPPLICANT_DEBUG=1 src/connmand -n`
*译自 http://www.embedded-computing.com/dev-tools-and-os/the-connman*







