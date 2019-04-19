# windows7系统笔记本设置成虚拟WiFi热点（即“无线路由器”） - 三少GG - CSDN博客
2012年01月30日 00:20:56[三少GG](https://me.csdn.net/scut1135)阅读数：8869标签：[windows																[网络																[microsoft																[internet																[路由器																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=路由器&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)
[http://www.douban.com/note/142037908/](http://www.douban.com/note/142037908/)**比conncetify方便，稳定，网速好！ ****（不过麻烦的是要打开防火墙，不用wifi的时候估计还要关掉。每次需要输入wlan start指令）****此外，每次别忘检测笔记本无线按钮是否打开！！！否则会说“无法启动承载网络。连到系统上的设备没有发挥作用 ”** 如果家中只有一根网线，却有两台或更多台笔记本需要连接到网络，借助软件，就可以将笔记本变身为无线路由器，即“WiFi热点”。
   以windows7为例，其实它本身就自带了WiFi和SoftAp（即虚拟无线AP）功能，仅需激活他们，你就可以让自己的笔记本变身为一个“WiFi热点”，让其他笔记本都连入你的笔记本上网。
   再打开你的笔记本的无线网卡“开关”后，**以管理员身份进行快捷键win+R→输入"cmd”→回车的操作。然后在已打开的命令提示符中输入"netsh wlan set hostednetwork mode=allow ssid=binglanPC key=binglanWiFi"的命令并按回车，这样就初步启用和设定了虚拟WiFi网卡。**
PS:此命令有三个参数。mode：是否启用虚拟WiFi网卡，改为disallow则为禁用；ssid：无线网络名称，最好用英文（以binglanPC为例）；key：无线网络密码，8个以上字符（本文以binglanWiFi为例）。
      当你在“网络连接”中，看到多出一个名为“Microsoft Virtual WiFi Miniport Adapter”的无线连接2，表示开启成功。为了和其他网卡区分开，将其重命名为"虚拟WiFi”。
   接着，就是连接Internet了。**在“网络连接”中，右键单击已经连接到Internet的网络连接，依次选择“属性”→ “共享" → ”允许其他……连接(N)" → “虚拟WiFi”即可。而在你点击“确定”之后，那个提供共享的网卡图标旁会出现“共享”的字样。**
PS：此时可能出现“无法启用共享访问 错误代码：0x80004005：未指定错误 ”，遇到此情况时，右键单击桌面上的“计算机”，依次选择“管理”→在左边的菜单栏中双击打开“服务和应用程序”菜单→选择“服务”→在右边找到并选择"Windows Firewall” → 点击“启用此服务”，然后再进行一次上边这段中的操作即可。
   接下来，在命令提示符中运行：**netsh wlan start hostednetwork，从而开启无线网络。**当然可以新建一个记事本文件复制：netsh wlan start hostednetwork  然后另存为：启动WIFI热点.bat
**下次开机**的时候只需要以“管理员身份运行”就启动了。PS：当将"start”改为"stop"时，即可关闭该无线网络。
最后，这个虚拟WiFi的红叉会消失，这表示“WiFi基站”已建好。然后，其他笔记本搜索无线网络"binglanPC”、输入密码"binglanWiFi"，就能共享上网了啦！
PS：很多人说这个办法行不通，Please先查一下你的网卡是否支持
cmd命令行输入netsh wlan show drivers
“支持的承载网络”后面写的是“是”还是“否”。否的话这个方法就行不通了
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++第二篇  参考 [http://wenku.baidu.com/view/b3483baddd3383c4bb4cd22b.html](http://wenku.baidu.com/view/b3483baddd3383c4bb4cd22b.html)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
开启windows 7的隐藏功能：虚拟WiFi和SoftAP（即虚拟无线AP），就可以让电脑变成无线路由器，实现共享上网，节省网费和路由器购买费。iphone4亲测通过,比conncetify方便，稳定，网速好！
以操作系统为win7的笔记本或装有无线网卡的台式机作为主机。
主机设置如下：
**1、以管理员身份运行命令提示符：**
**“开始”---在搜索栏输入“cmd”----右键以“管理员身份运行”**
**2启用并设定虚拟WiFi网卡：**
运行命令：netsh wlan sethostednetwork mode=allow ssid=wuminPC key=wuminWiFi
(注意：上边命令"ssid"后红字为网络名称，自己随便命名，比如wuminPC可改为MyWiFi等等，自己喜欢怎么命名都行
                            "Key"后边红字为密码，自己随便命名，比如wuminWiFi 可以改为12345678，总之自己命名就可以了，不一定非得这个网络名称，这个密码，密码8位以上最好）
此命令有三个参数，mode：是否启用虚拟WiFi网卡，改为disallow则为禁用。
                  ssid：无线网名称，最好用英文(以wuminPC为例)，即要设定的wifi名称。
                  key：无线网密码，八个以上字符(以wuminWiFi为例），即你要设定的wifi密码。
以上三个参数可以单独使用，例如只使用mode=disallow可以直接禁用虚拟Wifi网卡。
相信以前用过DOS的人都知道怎么运行命令了，那就是输入按回车**netshwlan set hostednetwork mode=allow ssid=wuminPCkey=wuminWiFi**
运行之后看到以下内容：
然后再打开“网络和共享中心”--“更改适配器设置”看看是不是多了一项，若果有多出的这一项“Microsoft Virtual WiFi MiniportAdapter”，那么说明你前边的设置是真确的。
开启成功，网络连接中会多出一个网卡为“MicrosoftVirtual WiFi Miniport Adapter”的无线。
为方便起见，将其重命名为虚拟WiFi。若没有，只需更新无线网卡驱动就OK了。
**3、设置Internet连接共享：**
在“网络连接”窗口中，右键单击已连接到Internet的网络连接，选择“属性”→“共享”，勾上“允许其他······连接(N)”并选择“虚拟WiFi”。
确定之后，提供共享的网卡图标旁会出现“共享的”字样，表示“宽带连接”已共享至“虚拟WiFi。
**4、开启无线网络：**
继续在命令提示符中运行：netsh wlan start hostednetwork
当然可以新建一个记事本文件复制：netshwlan start hostednetwork  然后另存为：启动WIFI热点.bat
下次开机的时候只需要以“管理员身份运行”就启动了。
 至此，虚拟WiFi的红叉叉消失，WiFi基站已组建好，主机设置完毕。，iphone，touch,笔记本,带WiFi模块的手机等子机搜索到无线网络wuminPC，输入密码wuminWiFi，就能共享上网啦
网络名称和密码可以按照自己喜欢的写。
用这种方法建立的无线网络，比使用connetify要稳定，简单，而且网速好。
需要的朋友可以试试，好用的话不要忘记顶顶哦！
====================================================================================================
附：显示无线网络信息命令：netshwlan show hostednetwork
=====================================================================================================
更新：有网友说还可以桥接的方法建立共享，也一并总结了，希望对网友有帮助！
1.**删除[电脑](http://detail.zol.com.cn/desktop_pc_index/subcate27_list_1.html)之前已经建立所有的临时网络**：进入“网络与共享中心”——选择左边”管理无线网络“——选中无用的临时网络右键单击删除网络确定退出
2.**[无线网卡](http://detail.zol.com.cn/wireless_nic_index/subcate225_list_1.html)的IP设定**：“网络和共享中心”——无线网络连接选择属性——双击下面IPv4IP设置为自动获取，click确定。
3.**禁用本地连接和无线网络连接的连接共享。**分别右键单击“本地连接”和“无线网络连接”选择“属性”——
 单击共享选项卡取消允许.
4.**建立桥接（关键步骤，跟vista最大区别）**
 ”网络与共享中心“——”更改适配器设置“，进入网络连接界面——**（重中之重）拔掉连接到[笔记本](http://detail.zol.com.cn/notebook_index/subcate16_list_1.html)上的有线网卡网线**——按下Ctrl，同时选中
 ”本地连接“和”无线网络连接“ ——右键单击其中任意一个选择桥接——等待出现网桥图标以后，插上网线
**5.创建临时网络**网络与共享中心“——单击”管理无线网络“——单击中间的”添加“——在新打开的窗口选择”创建临时网络“点击”下一步"
进入如下步骤要求输入输入网络名和密码，网络名自己命名，密码自己设定，安全类型选WEP
click“下一步”等待关闭。
**6 开启笔记本的无线网络.左键点击右下角的网络图标,连接刚刚创建的wifi临时网络（比如我设置的“wifi for iPod”） 这样你就可以共享笔记本的网络了！**
