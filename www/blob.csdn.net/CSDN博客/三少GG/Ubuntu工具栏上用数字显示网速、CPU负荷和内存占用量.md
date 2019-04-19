# Ubuntu工具栏上用数字显示网速、CPU负荷和内存占用量 - 三少GG - CSDN博客
2013年07月09日 13:34:41[三少GG](https://me.csdn.net/scut1135)阅读数：3093
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# [HOW TO DISPLAY NETWORK UPLOAD / DOWNLOAD SPEED ON THE PANEL IN UBUNTU 11.04](http://www.webupd8.org/2011/05/how-to-display-network-upload-download.html)
|||||
![Indicator sysmon](http://4.bp.blogspot.com/_1QSDkzYY2vc/TcKsgUGTWdI/AAAAAAAAET4/zilLQMIdlT8/s800/indicator-sysmon2.png)
[Indicator-Sysmonitor
 (System Monitor Indicator)](http://www.webupd8.org/2011/03/system-monitor-indicator-puts-cpu-and.html) is a really interesting Ubuntu indicator because it allows you to display pretty much anything on the Unity (or GNOME) panel by using a simple command or a bash script.
(Speaking of Indicator-Sysmonitor: a recent update brought sensors support (make sure you install hddtemp and lm-sensors), but this post isn't about that)
Currently there is no AppIndicator to display the actual network usage as in the actual values on the Unity panel in Ubuntu 11.04. We have [Indicator
 Multiload](http://www.webupd8.org/2011/05/network-memory-and-cpu-usage-indicator.html) but it only displays a graph so I wrote a script for Indicator-Sysmonitor to display the network upload/download speed as well as CPU and Memory usage (Indicator Sysmonitor already has CPU and Memory usage but if you want to use a script,
 you must get your script to display the CPU and Memory usage independently).
Update: I've replaced "up" and "down" in the AppIndicator with up/down arrows (thanks to manequin for the tip!) so this is how it looks now:
![System monitor indicator](http://4.bp.blogspot.com/_1QSDkzYY2vc/TcRM6wQTtXI/AAAAAAAAEWk/s9OZ-5Mz00w/s800/sysmon2.png)
## Getting Indicator-Sysmonitor to display network upload / download speed
Before getting started, please note that while this worked just fine on my computer, 
1. Firstly, install Indicator-Sysmonitor. For Ubuntu 11.04:
```
sudo add-apt-repository ppa:alexeftimie/ppa
sudo apt-get update
sudo apt-get install indicator-sysmonitor
```
If you use Ubuntu 10.10 (not working in Ubuntu 10.04!), download .deb from [here](https://launchpad.net/indicator-sysmonitor/+download).
2. Install dstat (this is required by my script):
`sudo apt-get install dstat`
3. Now you must download the script. Let's create a folder (if it doesn't exists already) called "scripts" in your home directory and download the script there:
```
mkdir -p ~/scripts && cd ~/scripts
wget http://webupd8.googlecode.com/files/sysmon_0.2.tar.gz && tar -xvf sysmon_0.2.tar.gz
```
4. Setting Up System Monitor Indicator to use the script.
Now open Indicator-Sysmonitor (it should show up as System Monitor Indicator in the menu), then click it on the panel and select "Preferences" and under "Use this command", enter this:
`$HOME/scripts/sysmon`
![Indicator Sysmon](http://4.bp.blogspot.com/_1QSDkzYY2vc/TcKtP2GAAFI/AAAAAAAAET8/P7i3SJS2O90/s800/indicator-sysmon3.png)
Then save. For me, each time I save the Indicator-Sysmon preferences, the applet freezes. If that's the case for you too, run this in a terminal:
`killall indicator-sysmonitor`
Then start it again.
4. Configuring the script
![Indicator sysmon](http://4.bp.blogspot.com/_1QSDkzYY2vc/TcKsgRZNmfI/AAAAAAAAET0/7NOVHgUQWfE/s800/indicator-sysmon1.png)
By default, the script displays the RAM and network usage. You can also display the CPU usage or any other combination (display the network usage only, display all: the CPU, ram and network, etc.). For
 this, open the script (run the following command in a terminal):
`gedit ~/scripts/sysmon`
And on top of the file you'll see something like this:
```
#settings:
netspeed=true
ram=true
cpu=false
```
What these do is pretty much obvious - change the items you want to be displayed on the panel to true and the rest to false. Feel free to tweak the script to display other stuff if you want.

## 在Ubuntu 11.10工具栏上用数字显示网速、CPU负荷和内存占用量『译』
基本上照抄了《[How To Display Network Upload / Download Speed On
 The Panel In Ubuntu 11.04](http://www.webupd8.org/2011/05/how-to-display-network-upload-download.html)》，只不过我的实践环境是Ubuntu 11.10而已。
![](http://www.lirui.name/upload/224.indicator-sysmonitor.00.jpeg)
indicator-sysmonitor是一个能在Unity的工具栏上用数字显示系统状况的实用工具。如上图。一般装完以后只能显示CPU和内存，无法显示网速。[Andrew](http://www.webupd8.org/p/contact.html) 写了个脚本解决这个问题。
由于比较懒，没有完整翻译原文。只把必要的写下来了。
在开始之前先声明一下，以下所有步骤在我的电脑上都运行的很好。
**1. 首先在Ubuntu 11.10上安装 Indicator-Sysmonitor。**
sudo add-apt-repository ppa:alexeftimie/ppa 
sudo apt-get update 
sudo apt-get install indicator-sysmonitor
**2. 安装 dstat (运行定制脚本必备):**
sudo apt-get install dstat
**3. 下载 [Andrew](http://www.webupd8.org/p/contact.html) 编写的脚本**
下面我们在主文件夹里面创建一个 “.scripts” 文件夹，并把脚本下载到这个文件夹内：
mkdir -p ~/.scripts && cd ~/.scripts
wget http://webupd8.googlecode.com/files/sysmon_0.2.tar.gz && tar -xvf sysmon_0.2.tar.gz
**4. 设置System Monitor，让它使用脚本里的配置。**
下面 **Alt+F2** 运行indicator-sysmonitor。在工具栏点击Indicator-Sysmonitor 并选择 "**Preferences**" ，并在 "**Use this command**" 里输入：
$HOME/.scripts/sysmon
![](http://www.lirui.name/upload/224.indicator-sysmonitor.01.jpeg)
勾选“**Run on startup**” 后保存退出。在我机器上运行的时候，经常在修改配置的过程中 Indicator-Sysmon 会僵死。如果发生了这种情况请在终端用这个命令杀死任务：
killall indicator-sysmonitor
然后重新运行它。
**4. 自行配置脚本**
![](http://www.lirui.name/upload/224.indicator-sysmonitor.00.jpeg)
默认情况下，这个脚本显示内存、CPU负荷、内存使用情况。您可以可以去掉您不关心的项目。在终端里运行：
gedit ~/.scripts/sysmon
在脚本开头的位置有下面的几个布尔值：
#settings: 
netspeed=true 
ram=true 
cpu=true
 总之您自己看着改吧。也可以鼓捣这个脚本显示点别的东西。
顺便再提供两个很有用的插件：
**Touchpad Indicator** —— 这个插件可以轻松控制触控板状态(启用/禁用),并可在插入鼠标时自动禁用触控板。支持快捷键启用/禁用。
sudo add-apt-repository ppa:atareao/atareao
sudo apt-get update
sudo apt-get install touchpad-indicator
**ClassicMenu Indicator** —— 这个插件为 Unity 提供了类似 GNOME 风格的应用程序菜单
sudo add-apt-repository ppa:diesch/testing
sudo apt-get update
sudo apt-get install classicmenu-indicator
以上两段感谢[笨兔兔](http://www.bentutu.com/)分享
--------------------------------------
『Over』
