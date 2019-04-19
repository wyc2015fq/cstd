# Ubuntu下10个厉害的Indicator小程序 - 三少GG - CSDN博客
2012年11月14日 16:02:35[三少GG](https://me.csdn.net/scut1135)阅读数：2967
[http://www.ithome.com/html/soft/27762.htm](http://www.ithome.com/html/soft/27762.htm)
Indicator小程序是非常有用的，提供了一些快捷的操作和信息，为Ubuntu的整体的体验发挥了很大的作用。这里介绍10个非常有用的Indicator小程序。
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145202_969.jpg)
1. Weather Indicator Applet (天气预报)
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145210_310.png)
安装命令：
sudo apt-get install indicator-weather
2. ClassicMenu：经典的GNOME2菜单
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145214_222.png)
安装命令：
sudo apt-add-repository ppa:diesch/testing
sudo apt-get update
sudo apt-get install classicmenu-indicator
3. Ubuntu One（Ubuntu提供的类似网盘的服务）
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145216_366.png)
安装命令：
sudo add-apt-repository ppa:rye/ubuntuone-extras
sudo apt-get update
sudo apt-get install indicator-ubuntuone
4. System Load Indicator ( 系统状态指示器）
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145217_591.png)
安装命令：
sudo add-apt-repository ppa:indicator-multiload/stable-daily
sudo apt-get update
sudo apt-get install indicator-multiload
5. Touchpad Indicator (触摸板开关)
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145219_432.png)
安装命令：
sudo add-apt-repository ppa:atareao/atareao
sudo apt-get update
sudo apt-get install touchpad-indicator
6. Battery Status Indicator (电源状态指示器)
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145220_648.png)
安装命令：
sudo apt-add-repository ppa:iaz/battery-status
sudo apt-get update
sudo apt-get install battery-status
运行：
/usr/lib/battery-status/battery-status –indicator
7. Workspaces Indicator (快速切换工作区)
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145222_185.png)
安装命令：
sudo add-apt-repository ppa:geod/ppa-geod
sudo apt-get update
sudo apt-get install indicator-workspaces
8. Keylock Application Indicator （显示大写键状态）
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145222_800.png)
安装命令：
sudo add-apt-repository ppa:tsbarnes/indicator-keylock
sudo apt-get update
sudo apt-get install indicator-keylock
9. Caffeine Application Indicator （防止启动屏保和省电模式）
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145223_718.png)
安装命令：
sudo add-apt-repository ppa:caffeine-developers/ppa
sudo apt-get update
sudo apt-get install caffeine
10. Pastie Clipboard Manager Indicator （剪贴板管理工具）
![](http://img.ithome.com/newsuploadfiles/2012/10/20121015_145224_941.png)
安装命令：
sudo add-apt-repository ppa:hel-sheep/pastie
sudo apt-get update
sudo apt-get install pastie
# [十二条技巧助你提升Ubuntu桌面体验](http://www.ithome.com/html/it/27879.htm)
从桌面体验中获得更多：
Pithos：使用Pithos是在Ubuntu桌面用Pandora听音乐的最佳方式。它是个简单的有效的用户界面，不用与Flash和讨厌的广告打交道，让你轻松享受音乐带来的乐趣。
反观web页面应用里发现的功能，几乎所有的Pandora音乐功能都能在Pithos里使用。你也可以使用Pithos选择、禁止和跳过歌曲。
Dropbox：这是个大型web服务，Linux这个应用与Ubuntu桌面很好集成。可轻松添加、移除或创建目录，放置在Dropbox账户中。
最好的是，可与UbuntuOne一起使用。虽然它不是数据备份解决方案，但真的是在平台之间分享文件的便捷方式。
Synapse：我活在键盘世界里啊。现在，我尽量控制自己少用鼠标。有了Synapse启动器，我能轻松找到文件，还能发布应用，甚至运行不同的系统任务，以前都得靠键盘呢。
Parcelite：没了剪贴板管理器，Parcelite雄起了。使用装了Parcelite的剪贴板，可复制任何东西，还可以在复制其他东西后唤醒之前的动作。不同于标准的复制与粘贴，Parcelite允许用户“存储”多个复制条目到粘贴板。甚至，Parcelite能与其他的Ubuntu同步剪切复制的内容。
System Monitor applet：在使用GNOME 2的日子里，我需要依靠很多小程序。我经常使用的程序必须是System Monitor applet。虽然可以使用命令行做相同的事情，但我喜欢运行可视化的程序，让人能看到资源是如何被消耗的呢。
Jungle Disk：过去几年我用了好多备份工具。我是主目录的粉丝，但同时，站外备份也有必要。基于我的经验，使用JungleDisk很安全，它很便宜，依赖Amazon S3提供的弹性存储，本身就支持Linux桌面的备份客户端。
Synaptic：当谈及Ubuntu桌面，少有像Synaptic这样的GUI应用能轻松进行安装、更新或迁移包。它能提供的功能类似于Ubuntu Software Center，而且Synaptic还有额外惊喜，比如可对所有应用实行完全控制。
VLC：媒体播放器中的战斗机。我爱VLC是因为它不仅能播放所有格式，还可从Web流化内容。甚至，也能流化内容并在本地编译。另一个很酷的选项是捕捉网络摄像头视频的功能。总体来说，它是个用途广泛的应用。
Skype Call Recorder：虽然Skype封闭源码，但它仍是使用Ubuntu获得统一桌面体验的关键部分。其他工具，如谷歌自己的视频聊天很好，但只有Skype的用户体验最好，提供了受欢迎的视频软件与功能。
捆绑Skype与Skype Call Recorder，就设置成为Skype音频会议模式，可记录下会议内容。不过在使用记录软件之前，首先要确认一下本地的法律是否允许。
Y PPA Manager：与Synaptic一样好用，进行包处理很不错，但其PPA备份功能不是完美的。所以我推荐使用真正的PPA管理工具，如Y PPA Manager，能轻松添加、移除和输出Ubuntu PPA。还有个额外好处，能处理输出的PPA gpg key。
Gufw：与Ubuntu ufw（防火墙）一样简单，事实是用于新手，CLI选项很逆天。幸好，有个坚固的GUI叫做Gufw。该软件允许终端用户设置防火墙规则，甚至支持基于应用的防火墙控制。
OpenDNS：有了父级控制，更快的DNS方案和安全web浏览，OpenDNS带来大量好处。你所需要做的就是注册该服务，听从指挥并安装ddclient。
