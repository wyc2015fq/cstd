# FW150U构建无线局域网【模拟AP功能设置指南】 - 三少GG - CSDN博客
2013年02月18日 17:48:40[三少GG](https://me.csdn.net/scut1135)阅读数：1694
迅捷网络： **FW150U 驱动**
**[http://www.fastcom.com.cn/Download/detail?pd=8](http://www.fastcom.com.cn/Download/detail?pd=8)**
软件简介：适用于FW150U 3.0,5.0版本无线网卡的驱动程序
[TL-WN727N 2.0模拟AP功能设置指南](http://service.tp-link.com.cn/detail_article_212.html)
TL-WN727N 2.0具有软件模拟AP功能，当网卡工作在“模拟AP”模式时可以实现无线接入点的功能。可以以此来快速组建基础结构无线网络。
下面介绍一下模拟AP模式的一个常用用法：配合电脑其他可以访问互联网的网络连接虚拟出一个“无线路由器”，网络拓扑结构如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102440404.jpg)
本地连接是可以访问互联网的网络连接，作为“虚拟无线路由器”WAN口，TL-WN727N安装之后的无线网络连接作为LAN口。其他Wi-Fi设备无线连接上这个“虚拟无线路由器”，就可以通过这个“虚拟无线路由器”实现多台电脑、手机、平板电脑等共享无线上网。下面我们来介绍该功能的配置方法。
**步骤1 TL-WN727N切换至模拟AP模式**
1. 首先安装TL-WN727N无线网卡的无线配置软件和驱动程序，安装完成之后右键点击桌面右下角的无线配置软件图标，如下图所示，在列表中选择“切换至模拟AP模式”。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102513241.jpg)
切换至模拟AP模式后，电脑上的本地连接会自动变为共享状态，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102547934.jpg)
**注意：若电脑上有多个已连接的网络连接，会弹出选择作为WAN口网络连接的列表。可以自行选择。宽带连接PPPOE拨号成功之后，也会在WAN口选择列表中。**
2. 切换成功之后弹出模拟AP的设置界面，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102614209.jpg)
主界面上有以下按钮：“配置AP”、“高级”、“连接控制列表”、“连接列表”以及“关于”，图中显示的IP地址和子网掩码为切换到模拟AP模式之后，无线网络连接自动配置的IP地址和子网掩码。
3. 手动指定无线网卡的IP地址和子网掩码，默认在切换成模拟AP模式的时候，无线网络连接会自动修改为使用下面的IP地址，并自动配置IP地址：192.168.123.1，子网掩码：255.255.255.0，可以保持默认，也可以手动修改（推荐保持默认即可）。如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102646267.jpg)
**注意：指定的IP地址将成为无线客户端的网关地址。**
**步骤2 配置AP无线参数**
1. 点击“配置AP”按钮，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102722991.jpg)
2. 在网络名称栏（SSID）定义模拟AP的无线网络名称，本例中保持默认的SoftAP-E0。点击下一步，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102748650.jpg)
3. 选择模拟AP的无线信道，可以选择任意信道1~13。点击下一步，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/2011112510289878.jpg)
4. 设置模拟AP的无线安全类型和加密类型，安全类型推荐“WPA-PSK/WPA2-PSK”，加密类型推荐“AES”。点击下一步继续，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102821714.jpg)
5. 设置模拟AP无线网络密码。密码可设置为8~63个ASCII码字符或8~64个16进制字符。更新群组密码间隔建议保持默认值。点击下一步设置完成。
**注意：ASCII码字符为键盘上的任意字符；16进制字符包括数字0~9，以及字母A~F、a~f。**
**步骤3 无线客户端连接至模拟AP**
1. 打开其他无线客户端（如笔记本、手机等），本例以XP系统笔记本为例，首先搜索无线网络，可以搜索到模拟AP的网络名称“SoftAP-E0”，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102845800.jpg)
2. 单击选中该无线网络，点击“连接”，开始无线连接。连接过程中会弹出无线网络密钥输入窗口，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102857595.jpg)
3. 请输入前述步骤中设定的无线网络密码。连接成功，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125102912640.jpg)
4. 此时查看无线网络连接的网络参数，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/2011112510305465.jpg)
从图中可以看出，无线网络自动获取到IP地址等网络参数。所获取到的IP地址为模拟AP指派，网关和DNS均为TL-WN727N工作在模拟AP模式时自动生成的无线IP地址，此时该笔记本即可通过TL-WN727N模拟AP功能共享上网了。
**附：连接控制功能应用设置**
使用此功能，您可以允许或者拒绝特定的MAC地址连入AP建立的无线网络。类似于无线路由器的“无线MAC地址过滤功能”。下面介绍一下该功能的设置方法。
首先点击“连接控制列表”按钮，打开如下图所示设置界面。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125103031291.jpg)
连接过滤方式中的规则将应用于列表中的MAC地址。首先选择连接过滤方式，这里有三种选项：
1. 不使用，即关闭连接控制功能。
2. 全部允许，即列表中MAC地址全部允许接入到无线网络，列表之外的全部禁止。
3. 全部拒绝，即列表中MAC地址全部拒绝接入到无线网络，列表之外的全部允许。
用户可以根据自己的实际情况进行选择。
MAC地址：为需要添加到列表中去MAC地址输入区域。
上图中以 “全部允许”为例，并将允许的MAC地址00-13-E8-D6-0E-4D添加到列表中。此时只有该MAC地址无线客户端才能连接，其他MAC地址无线客户端将不能连接到该无线网络。
**无线客户端列表**：主界面点击连接列表按钮，可以查看当前的连接到无线网络的客户端MAC地址，如下图所示。
![](http://service.tp-link.com.cn/pages/imageuploadfolder/20111125103039599.jpg)
