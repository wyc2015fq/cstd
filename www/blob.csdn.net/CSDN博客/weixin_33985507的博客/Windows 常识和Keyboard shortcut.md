# Windows 常识和Keyboard shortcut - weixin_33985507的博客 - CSDN博客
2015年12月21日 00:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
Win 8 实现了 App 和桌面软件的统一，是 Windows 发展史上一个很好的里程碑。不论哪种输入方式，触摸、键盘、鼠标，界面是一致的。
- 
配置多个IP地址
以太网属性->Internet 协议版本 4（TCP/IPv4）
![596516-910017d7ffc7a44e.png](https://upload-images.jianshu.io/upload_images/596516-910017d7ffc7a44e.png)
IPV4配置
- 
[How to change the system language across your whole Windows 10 PC](https://www.windowscentral.com/how-properly-change-system-default-language-windows-10)？
修改系统语言，显示语言。
##### Screen
- Desktop
- Start Screen
- Apps View
*Screen 切换*
[Win] 在 Start Screen 和 Desktop 之间切换。
[**↓**] [**↑**] 在 Start Screen 和 Apps View 之间切换。
##### 快捷键
[Windows 8 tutorial: Getting around your PC](http://windows.microsoft.com/en-us/windows-8/getting-around-tutorial?ocid=W8_apps_helptips)
- 
[Win] + X ; 常用的功能都出现在列表中
- 
[Win] + C ; Open the charms
- 
[Win] + I ; Open the Settings
- 
Alt + F4 ; 关闭任何 App 都适用。
- 
[Win] + D ; show Desktop)
- 
[Win] + E ; show Explorer)
- 
[Win] + R ; Open Run Command Prompt
- 
[Win] + Space ; To switch input methods（切换输入法）。更多查看 input language settings。
- 
CTRL + SHIFT + ESC: Opens Windows Task Manager
- 
CTRL + Tab: 在一个应用的内部各窗口 TAB 切换，比如浏览器的各个 TAB 间切换。
- 
ALT + TAB: Switch to another running program (hold down the ALT key and then press the TAB key to view the task-switching window)，即在各个 App 窗口之间切换。
- 
ALT + D：选中资源管理器的地址栏、浏览器的地址栏；
- 
Restore Down: [Win] + Down Arrow
- 
Maximize: [Win] + Up Arrow
- 
Minimize: ([Win] + Down Arrow)*2，两次则最小化
- 
Minimize: [Win] + M，全部最小化
- 
[Additional Windows 8 keyboard shortcuts](https://technet.microsoft.com/en-us/windows/jj728757.aspx)
- 
[Keyboard shortcuts for Windows](https://support.microsoft.com/en-us/kb/126449)
- 
[Win] or [CTRL + ESC]
> 
Start Search works almost exactly like Start Menu Search from Windows 7. From the desktop, just tap **WINKEY** (or**CTRL + ESC**) and start typing. The standard Search experience appears, with a search pane on the right featuring Apps, Settings, and Files entries, plus icons for each Metro-style app that also supports searching.
##### 设置鼠标滚轮方向保持和 Mac 触摸设备一致
- [Inverting direction of mouse scroll wheel](http://superuser.com/questions/310681/inverting-direction-of-mouse-scroll-wheel)
> - 找到 mouse 的 HID 值；
Control panel > Mouse > Hardware tab > HID-compliant mouse Properties > Details tab > Device Instance Path Property.
其值类似：HID\VID_046D&PID_C534&MI_01&COL01\8&E98EFF4&0&0000。
- 利用 regedit 设置一下 FlipFlopWheel 的值为 1；
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\HID\VID_*\Device Parameters\
- 重新拔插一下鼠标即可生效；
- 在 Windows XP 上，你只需要简单地在 regedit 里面搜索 FlipFlopWheel（可能很多个），把其值改为 1 即可；
- 滚动条保持不变
##### [Quickly Open a Command Prompt from the Windows Explorer Address Bar](http://lifehacker.com/5989434/quickly-open-a-command-prompt-from-the-windows-explorer-address-bar)
有时候你在资源管理器里面打开了一个目录，想直接转到命令行状态下，怎么办？
> 
It turns out Windows has a built-in way to do this. Simply type in "cmd" in the address bar and it'll open the command prompt with the path to your current folder already set.
So you can hit **Alt+D** to highlight what's in the address bar, type in "**cmd**" and you're all set.
##### [常用命令（Command-Line Reference）](https://technet.microsoft.com/en-us/library/cc754340.aspx)
- 
通常，使用 ***<command> /?***　可以查看便捷帮助
- 
set
set /?
Displays, sets, or removes cmd.exe environment variables.
比如：set 列出全部环境变量值，set appdata 则列出 appdata 变量值。
[List all environment variables from command line?](http://stackoverflow.com/questions/5327495/list-all-environment-variables-from-command-line)
> 
echo 命令则比较严格，echo %path% 列出 path 变量值，`SET prefix` 则可以列出所有以 prefix 开头的变量值。
修改环境变量：Control Panel > System > Advanced system settings > EnvironmentVariables，比如：编辑 path 变量。
只要你把一个应用程序的路径加入了 path 环境变量，则就可以通过 [Win]+R 启动应用程序了（你可以在应用程序路径里面增加各种缩写的快捷方式），而不必放到 Windows 系统目录下。比如：jmeter 应用程序加各种缩写。
- 
color 设置 command prompt 窗口的背景和前景色；
- 
cls：clears the screen
- 
[tasklist](https://technet.microsoft.com/en-us/library/cc730909.aspx) /fi "imagename eq nginx.exe"
查看 nginx 是否启动。
- 
fc or comp
文件比较（file compare），类似于 Linux 下的 diff。
- 
[nslookup](https://technet.microsoft.com/en-us/library/cc725991.aspx)
命令行下执行命令进入交互模式。在交换模式下输入 help 或 ? 查看帮助，exit 则退出。
```
C:\Users\windows7>nslookup
Default Server:  3qdc-wj1.soho3q.com
Address:  10.11.101.200
> set type=any
> set timeout=10
> example.com # 查看域名解析情况
Server:  3qdc-wj1.soho3q.com
Address:  10.11.101.200
Non-authoritative answer:
example.com     internet address = 93.184.216.34
example.com     AAAA IPv6 address = 2606:2800:220:1:248:1893:25c8:1946
> www.example.com # 查看主机解析情况
```
- 
ipconfig
`ipconfig /?` # Display help message
`ipconfig /flushdns` # Purges the DNS Resolver cache. 清空DNS缓存。
- 
`find` | `findstr`
find: Searches for a text string in a file or files.
findstr: Searches for patterns of text in files.
find /?
find /I : Ignores the case of characters when searching for the string.
- 
sc.exe （service control）
sc query state= all | find /i "vpn"
sc query OpenVPNService 查看状态
sc start OpenVPNService 启动
sc stop OpenVPNService 停止
- 
net.exe
net start OpenVPNService 启动
net stop OpenVPNService 停止
注：Win 7 请以 Administrator 权限打开命令行窗口。
- 
Remote Desktop Connection
**mstsc.exe** (Remote Desktop Services (Terminal Services) Command)
Creates connections to Remote Desktop Session Host (RD Session Host) servers or other remote computers.
- 
Services
`"Control Panel > Administrative Tools > Services"` start the Services Console.
Search & Run: `services.msc` 打开服务窗口
.msc File Extension: [Microsoft Management Console](https://msdn.microsoft.com/en-us/library/bb742441.aspx) files (Management Saved Console). mmc.exe 可以编辑 msc 文件。
- 
Telnet
因安全原因，Win 7 以上默认不安装 Telnet。进入 Control Panel -> Programs and Features -> Turn Windows features on or off，Select 'Telnet Client' check box 进行安装 [install Telnet Client](http://technet.microsoft.com/en-us/library/cc771275(v=ws.10).aspx)。
telnet 10.11.132.184 3306 检测 3306 MySQL 服务是否可访问。
参考：[Test TCP port connectivity for Apple software](https://support.apple.com/en-us/HT203875)。
- 
netstat
netstat -an -p tcp 检测链接和端口使用情况。Displays protocol statistics and current TCP/IP network connections.
-a: Displays all connections and listening ports.
-n: Displays addresses and port numbers in numerical form.
-p proto: Shows connections for the protocol specified by proto.
示例：netstat -an -p tcp | find /i "1080" | find /i "listening" 查看 1080 端口是否绑定成功；
- 
ping
ping 命令检测和指定主机域名或IP是否连通。
- 
[查看 java 版本](https://java.com/en/download/help/version_manual.xml)
> 
Control Panel > Programs and Features
- 
`type NUL > filename`
creates a 0 bytes file. 类似于 Linux 下的 touch 命令。
- 
`cmd /C mklink /D User h:\baiduyunebooks\sublime3\Packages\User`
创建一个符号链接 User 指向最后的文件目录参数；
- 
截屏（screen capture）
To take a screen capture of the entire screen：Press *Print Screen*（PrtScn）.
To capture an active window：Press Alt+PrtScn.
- 
Change Drive Letter（改变驱动器字母）
Control Panel > Administrative Tools > Computer Management > Disk Management > Change Drive Letter and Paths
- 
how to close the laptop lid without sleep?
Control Panel > Power Options > Choose what closing the lid does
##### Windows - Uptime（开机时间）
Your Windows system’s uptime is displayed in [the Task Manager](http://www.howtogeek.com/108742/how-to-use-the-new-task-manager-in-windows-8/). Right-click the taskbar and select Task Manager or press Ctrl+Shift+Escape to open it.
On Windows 8, click the Performance tab and look under “Up time” at the bottom of the window.
##### Installation Date
> `systeminfo | find /i "Original"` 或者 `systeminfo | find /i "初始安装日期"`
```
Original Install Date:     6/8/2015, 9:45:09 AM
```
##### Visual Studio Community
[https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs)
### 了解 slmgr（软件授权管理）
- [How to Use Slmgr to Change, Remove, or Extend Your Windows License](https://www.howtogeek.com/245445/how-to-use-slmgr-to-change-remove-or-extend-your-windows-license/)
##### 查找当下的实际情况
```
slmgr.vbs -xpr
slmgr /dlv
slmgr /dli
```
##### 更换产品密钥
```
slmgr /skms kms.chinancce.com
slmgr /ipk DCPHK-NFMTC-H88MJ-PFHPY-QJ4BJ
slmgr /ato
```
##### 依据错误码，查找错误描述
`slui.exe 0x2a 0xC004E016`
其中 0xC004E016 为错误码。
##### 几个术语
- KMS：Key Management Service
- LTSB：Long Term Servicing Branch
##### 可试用密钥
```
企业版2015LTSB - Enterprise 2015 LTSB WNMTR-4C88C-JK8YV-HQ7T2-76DF9
企业版2015LTSB N - Enterprise 2015 LTSB N 2F77B-TNFGY-69QQF-B8YKP-D69TJ
企业版2016LTSB - Enterprise 2016 LTSB DCPHK-NFMTC-H88MJ-PFHPY-QJ4BJ
企业版2016LTSB N - Enterprise 2016 LTSB N QFFDN-GRT3P-VKWWX-X7T3R-8B639
```
##### slmgr.vbs /dlv
```
Windows Software Licensing Management Tool, slmgr.vbs is a command line licensing tool. It is a visual basic script used to configure licensing in Windows and also helps you see the licensing status of your Windows 10/8/7 installation.
```
![596516-738aff4259b39b10.png](https://upload-images.jianshu.io/upload_images/596516-738aff4259b39b10.png)
slmgr.vbs /dlv
##### 常用文件
C:\WINDOWS\system32\drivers\etc\hosts
##### Taskbar
Taskbar 就是 ***任务栏***，看到她，就感觉亲切。
**Windows logo key**
![596516-8f8206d54c79c00a.png](https://upload-images.jianshu.io/upload_images/596516-8f8206d54c79c00a.png)
Windows key.png
> 
Windows logo key 在行文中 以 [Win] 表示，比如 [Win] + C。
##### 网络
- [Can I choose WiFi over Ethernet LAN](https://superuser.com/questions/436256/can-i-choose-wifi-over-ethernet-lan)？
如何优先使用 WiFi网络？
- [自动跃点数](https://www.pc841.com/Win10/201608-71547_2.html)；
- [How to manage wireless networks using Command Prompt in Windows 10](https://www.windowscentral.com/how-manage-wireless-networks-using-command-prompt-windows-10)？
- [Change Network Adapter Priorities in Windows 10](https://www.ghacks.net/2016/12/02/change-network-adapter-priorities-in-windows-10/)；
##### 建立 VPN 链接
Change connection properties：Control Panel / Network and Sharing Center / Change adapter settings。
Type of VPN：Automatic，好像不大可靠，最好改为 PPTP。错误 691表明你的 用户名/密码错误，详见 [Windows VPN not Workin on Windows 8.1](http://answers.microsoft.com/en-us/windows/forum/windows8_1-networking/windows-vpn-not-workin-on-windows-81/de62794b-8bb6-4898-9d54-5c1f7e9af4d6?auth=1)。
![596516-1292d949d87ae322.png](https://upload-images.jianshu.io/upload_images/596516-1292d949d87ae322.png)
connection properties.png
##### 如何把公用网络切换为专用网络？
![596516-fca10eb5f708653d.png](https://upload-images.jianshu.io/upload_images/596516-fca10eb5f708653d.png)
公用切换为专用
##### APP ?
- 
Windows UI application
就是你在 Start Screen 看到的 App，是从应用商店（Store）里面下载安装的 App，通常可以在平板、手机上安装。
- 
Windows Desktop application
就是传统方式安装在 PC 上的桌面软件。
> 
[App 卸载问题](http://answers.microsoft.com/en-us/windows/forum/windows_8-windows_install/how-to-remove-an-app-from-all-apps-screen-of/904ceace-1c11-4b54-894c-b6ec60a71aa7?auth=1)，试试： sfc 命令处理一下系统文件一致性问题。
[Win]+X / Windows PowerShell (Admin) / Type **sfc /ScanNow** /
扫描结果见：C:\Windows\Logs\CBS\CBS.log，看不懂。sfc 也没解决问题。sfc: System File Checker command.
sfc: Scans the integrity of all protected system files and replaces incorrect versions with correct Microsoft versions.
![596516-420840a37125bba5.png](https://upload-images.jianshu.io/upload_images/596516-420840a37125bba5.png)
Start Screen -> Apps View.png
