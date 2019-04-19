# 【JM】电脑检测不到U盘 - 三少GG - CSDN博客
2011年04月22日 12:44:00[三少GG](https://me.csdn.net/scut1135)阅读数：1917标签：[工作																[flash																[磁盘																[优化																[ubuntu																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)
 竟然是刚哥的U盘 无意中救命...实在无语。难道是两个类似的U盘（供电电源类似），在ubuntu下驱动错乱，显示msdos文件系统？anyway，it's solved.[](http://help.360.cn/index.php?c=help&a=topic&bid=5030806&tid=24157670)
[http://help.360.cn/index.php?c=help&a=topic&bid=5030806&tid=24157670](http://help.360.cn/index.php?c=help&a=topic&bid=5030806&tid=24157670)
## 电脑检测不到U盘
我的电脑突然检测不到U盘,用瑞星和360对电脑进行过杀毒,但没有发现病毒.
插上其它的移动存储设备,电脑可以检测到.我想应该是我的U盘中了病毒,我想杀毒,但问题是,电脑根本检测到到U盘,怎么杀呢.请360专家帮我解决这个问题,谢谢!
## 楼主选择的最佳答案![](http://help.360.cn/img/bestdis.gif)
我们可以通过如下步骤重现丢失的托盘区图标。 
第1步：单击"开始→运行"，在运行对话框中键入"C:/WINDOWS/system32/systray.exe"命令，按下回车键执行命令，重新插拔一下USB设备，看图标是否出现。 
第2步：如果第1步无法重现"安全删除硬件"图标，那么则继续。在"运行"对话框中键入"regedit"，打开注册表编辑器。依次展开如下分支：[HKEY_CURRENT_USER/So 
ftware/Microsoft/Windows/CurrentVersion/ 
Run]，在右侧窗格中单击鼠标右键，新建一个字符串值并重命名为"systray.exe"，双击将其数值数据修改为"C:/WINDOWS/system32/ 
systray.exe"，确定之后退出注册表编辑器。 
第3步：在"运行"对话框中键入"devmgmt.msc"，回车后打开设备管理器。在"磁盘驱动器"下找到"Generic Flash USB Device"选项，这里是以U盘为例的，实际的应用过程中可能与此不同，但都很好识别。 
第4步：在"Generic Flash USB Device"选项上单击鼠标右键，选择"属性"，打开属性窗口。单击"策略"标签，打开策略标签页，选择"为提高性能而优化"，而不是选择"为快速删除而优化"，单击"确定"按钮。重新插拔一次USB设备，"安全删除硬件"图标应该出现了。 
第5步：如果还是不行，那么可以重新安装一下USB端口的驱动程序，如果你不清楚如何来安装，那么干脆直接重装一下主板驱动，虽然略耗时，但简单方便。重装主板驱动后，丢失的托盘区图标应该回来了。 
第6步：如果仍然没有出现，那么再次调出运行对话框，键入"SFC /scannow"，回车后对系统文件进行扫描，当提示插入Windows安装光盘时将安装光盘插入到光驱，修复完成后，重新启动计算机即可。 
"**安全删除硬件"图标不肯现身 很多人在使用USB设备过程中，会遇到这样的怪现象：将USB设备插入到计算机，USB设备上的指示灯在正常闪烁，但是托盘区"安全删除硬件"图标始终无法显示，在资源管理器当中也无法找到其盘符，USB设备无法使用!!!!!。**
发生这种情况后，首先将USB设备接入到其他计算机，看USB设备是否可以正常工作。如果不能正常工作，则是USB设备硬件出现故障，最好是送修处理。如果USB设备能够正常工作，那么则是计算机的设置问题了。这时，需要进入到注册表中操作。在"运行"对话框中输入"regedit"，回车后打开注册表编辑器。依次展开如下分支[HKEY_LOCAL_MACHINE/SYSTEM/
CurrentCntrolSet/Services/USBSTOR]，在右侧的窗格中找到名为"Start"的DWORD值。"Start"是USB设备的工作开关，有3种工作状态，设置为"2"表示自动，"3"表示手动，"4"表示停用。双击"Start"，在弹出的编辑对话框中将其数值数据修改为十六位进制数值"3"，最后单击"确定"按钮。关闭注册表编辑器，并重新启动计算机使设置生效，重启后故障即可排除。 
***答对了不忘回帖啊***
+_++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**计算机管理->磁盘管理里面可以检测到，无卷符，显示可用空间为100%**
/
1.
关闭了usb接口检测，或者禁用了usb端口。
在“我的电脑”点右键“属性”---“硬件”---“设备管理器”
里面有个“通用串型控制器”把里面所有的“小红叉”全部启用 
2. 
可能是连接线有问题.将接口的地方清理一下或拆开后,将接口紧固 
