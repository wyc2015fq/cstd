# WPA Supplicant 用法 - DoubleLi - 博客园






本文译至：[https://wiki.archlinux.org/index.php/WPA_Supplicant_%28%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87%29](https://wiki.archlinux.org/index.php/WPA_Supplicant_%28%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87%29)



[wpa_supplicant](http://hostap.epitest.fi/wpa_supplicant/) 是跨平台的 WPA[supplicant](http://en.wikipedia.org/wiki/Supplicant_%28computer%29)，支持 WEP, WPA 和 WPA2 ([IEEE 802.11i](http://en.wikipedia.org/wiki/IEEE_802.11i) / RSN (Robust Secure Network)). 可以在桌面、笔记本甚至嵌入式系统中使用。

*wpa_supplicant* 是在客户端使用的 IEEE 802.1X/WPA 组件， 支持与 WPA Authenticator 的交互，控制漫游和无线驱动的 IEEE 802.11 验证和关联。 

## 安装

从 [官方软件仓库](https://wiki.archlinux.org/index.php/Official_repositories) 中安装软件包 [wpa_supplicant](https://www.archlinux.org/packages/?name=wpa_supplicant)。

此外软件包 [wpa_supplicant_gui](https://www.archlinux.org/packages/?name=wpa_supplicant_gui) 提供了图形界面*wpa_gui*。 

## 启动

本节介绍启动wpa_supplicant的常用方法，选择一个最适合你的。

### systemd

wpa_supplicant提供多种服务的文件：
- `wpa_supplicant.service` - 使用 [D-Bus](https://wiki.archlinux.org/index.php/D-Bus), 推荐 [NetworkManager](https://wiki.archlinux.org/index.php/NetworkManager) 的用户.
- `wpa_supplicant@.service` - 接受接口名作为参数，并为该接口启动wpa_supplicant守护进程。它读取/etc/wpa_supplicant/wpa_supplicant-interface.conf的配置文件
- `wpa_supplicant-nl80211@.service` -  也是接口特定的,但明确强制nl80211驱动程序 (见下文). 配置文件路径是`/etc/wpa_supplicant/wpa_supplicant-nl80211-*interface*.conf`
- `wpa_supplicant-wired@.service` - 也是接口特定的, 使用 `wired` 驱动. 配置文件路径是`/etc/wpa_supplicant/wpa_supplicant-wired-*interface*.conf`

### dhcpcd

dhcpcd包含了一个钩子（默认为启用）来自动启动对应无线接口的wpa_supplicant。它只在如下情况下启动：
- 没有wpa_supplicant进程在该接口在监听。
- 存在一个wpa_supplicant的配置文件。dhcpcd 默认检查 /etc/wpa_supplicant.conf 和 /etc/wpa_supplicant/wpa_supplicant.conf，但可以通过在/etc/dhcpcd.conf设置env wpa_supplicant_conf=configuration_file_path来添加自定义路径。

### 手动

wpa_supplicant接受多个命令行参数，特别是：
- `-B` - 在后台执行
- `-c *文件名*` -路径配置文件
- `-i *接口*` - 监听的接口

关于完整的参数列表，参考 man 8 wpa_supplicant。例如，常见的用法是：
# wpa_supplicant -B -i *interface* -c *configuration_file*
## 配置

wpa_supplicant提供了一个参考配置文件/etc/wpa_supplicant/wpa_supplicant.conf，其中包含了所有可用的选项及其用法和例子的详细文档。考虑先把它备份起来，因为下面描述的自动添加网络配置到wpa_supplicant.conf的方法下删除了文件中的所有注释。

在其最简单的形式中，一个配置文件，只需要一个网络配置块。例如：


/etc/wpa_supplicant/foobar.confnetwork={
    ssid="..."
}
一旦你有一个配置文件，如前节所述，就可以启动wpa_supplicant守护进程，并使用一个静态IP或DHCP连接到无线网络。

### wpa_passphrase

网络配置可以使用wpa_passphrase工具自动生成并添加到配置文件中。这在连接到需要密码的安全网络时是有用的。例如：
$ wpa_passphrase *essid passphrase*network={
    ssid="*essid*"
    #psk="*passphrase*"
    psk=f5d1c49e15e679bebe385c37648d4141bc5c9297796a8a185d7bc5ac62f954e3
}
一些异常复杂的口令可能需要从文件输入：
$ wpa_passphrase *essid* < *passphrase.txt*
**提示**：wpa_supplicant和wpa_passphrase可以结合起来关联到几乎所有的WPA2（个人）网络：

# wpa_supplicant -B -i *interface* -c <(wpa_passphrase *essid passphrase*)


### 指定驱动

您可能需要指定一个驱动来使用。关于支持的驱动程序的列表，请参考wpa_supplicant -h的输出结果。
- `nl80211` 是当前的标准，但不是所有的无线芯片的模块支持。
- `wext` 目前已过时，但仍得到广泛支持。

使用 `-D` 开关来指定驱动:
# wpa_supplicant -B -i *interface* -c *configuration_file* -D *driver*
## 使用 wpa_cli

wpa_supplicant可以通过使用wpa_cli命令，在运行时手动进行控制。要启用wpa_cli，wpa_supplicant守护进程必须被配置为通过在wpa_supplicant的配置文件（默认位置：/etc/wpa_supplicant/wpa_supplicant.conf）设置ctrl_interface变量来创建一个“控制接口”（套接字）。

用户也将需要通过指定可以访问它的组来获准访问该套接字。一个新的组可能为此被创建，并且用户添加到它，或已存在的组可以使用 - 通常是 wheel。

下面的设置将在/run/wpa_supplicant/中创建套接字并允许wheel组的成员进行访问：
ctrl_interface=DIR=/run/wpa_supplicant GROUP=wheel

可以通过wpa_cli修改的wpa_supplicant配置文件自身。这在手动添加新的网络到配置文件，而无需重新启动wpa_supplicant守护进程时是有用的。为了实现这一目标，在配置文件中设置update_config变量设置为1：
update_config=1

在wpa_cli开始之前，wpa_supplicant守护进程必须处于运行状态。（见#Starting了解详情）。然后启动
$ wpa_cli

它会寻找配置文件中给定位置的控制套接字，位置也可以使用p选项手动设置）。您可以使用-i选项配置的接口，不然的话将使用wpa_supplicant所管理的第一个被找到的无线接口。



当wpa_cli被调用时，将出现交互式提示符（>）。提示包括tab 补全和已完成命令的说明。



### 使用wpa_cli添加一个新的网络

扫描可用网络，在>提示符之后输入“scan”。在扫描完成后将显示通知：
> scan
OK
<3>CTRL-EVENT-SCAN-RESULTS
>

然后输入 "scan_results" 来显示结果:
> scan_results
bssid / frequency / signal level / flags / ssid
00:00:00:00:00:00 2462 -49 [WPA2-PSK-CCMP][ESS] MYSSID
11:11:11:11:11:11 2437 -64 [WPA2-PSK-CCMP][ESS] ANOTHERSSID
>

为了与MYSSID相关联，需要告知wpa_supplicant。在配置文件中的各个网络是以零开始按数值进行索引。如果您添加了一个新的网络，它会被相应地分配一个新数字。
> add_network
0
>

使用这个数字来指定你的设置应用到哪个网络。对于一个新的网络，在引号里设置其SSID：
> set_network 0 ssid "*MYSSID*"
OK
>

即使你的无线接入点没有被保护，cli也显式地需要一个PSK，同样在引号内。密码必须是8-63个字符：
> set_network 0 psk "*passkey*"
OK
>

使能：
> enable_network 0
OK
>

将修改写入配置文件：
> save_config
OK
>

### 动作脚本

wpa_cli可以在后台模式下运行，并执行基于wpa_supplicant事件的指定脚本。可以支持两类事件：连接和断开。一些环境变量可用于脚本，细节请参考man 8wpa_cli。

下面的例子将使用桌面通知，通知有关事件的用户：
#!/bin/bash

case "$2" in
    CONNECTED)
        notify-send "WPA supplicant: connection established";
        ;;
    DISCONNECTED)
        notify-send "WPA supplicant: connection lost";
        ;;
esac

记住修改脚本为可执行，然后使用-a来传递脚本路径给wpa_cli：
$ wpa_cli -a *path_to_script*
## 另请参阅
- [Kernel.org wpa_supplicant documentation](http://wireless.kernel.org/en/users/Documentation/wpa_supplicant)









