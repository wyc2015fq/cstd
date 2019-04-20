# windows7系统笔记本设置成虚拟WiFi热点（即“无线路由器”） - 纯洁的微笑 - CSDN博客
2012年03月11日 18:09:57[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：15
                如果家中只有一根网线，却有两台或更多台笔记本需要连接到网络，借助软件，就可以将笔记本变身为无线路由器，即“WiFi热点”。
   以windows7为例，其实它本身就自带了WiFi和SoftAp（即虚拟无线AP）功能，仅需激活他们，你就可以让自己的笔记本变身为一个“WiFi热点”，让其他笔记本都连入你的笔记本上网。
   再打开你的笔记本的无线网卡“开关”后，以管理员身份进行快捷键win+R→输入"cmd”→回车的操作。然后在已打开的命令提示符中输入"netsh wlan set hostednetwork mode=allow ssid=binglanPC key=binglanWiFi"的命令并按回车，这样就初步启用和设定了虚拟WiFi网卡。
PS:此命令有三个参数。mode：是否启用虚拟WiFi网卡，改为disallow则为禁用；ssid：无线网络名称，最好用英文（以binglanPC为例）；key：无线网络密码，8个以上字符（本文以binglanWiFi为例）。
      当你在“网络连接”中，看到多出一个名为“Microsoft Virtual WiFi Miniport Adapter”的无线连接2，表示开启成功。为了和其他网卡区分开，将其重命名为"虚拟WiFi”。
   接着，就是连接Internet了。在“网络连接”中，右键单击已经连接到Internet的网络连接，依次选择“属性”→ “共享" → ”允许其他……连接(N)" → “虚拟WiFi”即可。而在你点击“确定”之后，那个提供共享的网卡图标旁会出现“共享”的字样。
PS：此时可能出现“无法启用共享访问 错误代码：0x80004005：未指定错误 ”，遇到此情况时，右键单击桌面上的“计算机”，依次选择“管理”→在左边的菜单栏中双击打开“服务和应用程序”菜单→选择“服务”→在右边找到并选择"Windows Firewall” → 点击“启用此服务”，然后再进行一次上边这段中的操作即可。
   接下来，在命令提示符中运行：netsh wlan start hostednetwork，从而开启无线网络。
PS：当将"start”改为"stop"时，即可关闭该无线网络。
最后，这个虚拟WiFi的红叉会消失，这表示“WiFi基站”已建好。然后，其他笔记本搜索无线网络"binglanPC”、输入密码"binglanWiFi"，就能共享上网了啦！
PS：很多人说这个办法行不通，Please先查一下你的网卡是否支持
cmd命令行输入netsh wlan show drivers
“支持的承载网络”后面写的是“是”还是“否”。否的话这个方法就行不通了，            
