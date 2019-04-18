# chm文件打开看不到内容的问题 - 长歌行 - CSDN博客





2013年01月04日 10:48:34[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1920







下载下来的一些chm文件，打开的是否提示“打开此文件前总是询问”，不理睬直接打开后，只有目录，页面内容都显示“已取消到本页的导航”。原因是该CHM文件的发行商未知，将“打开此文件前总是询问”选项取消后，从此打开这个chm文件就没有问题了。

      查看chm文件的一些常见故障，抄录如下：
一、正确操作方法
1、鼠标右键单击该CHM文件，在右键快捷窗口中选择“打开方式”-“Microsoft HTML Help Executable”；
2、如果出现一个提示窗口，大意是说，该CHM文件的发行商未知等，是否确定打开。请一定要将“每次都提示该窗口”的复选框的勾去掉！否则就会出现“该页无法显示”的情况。
3、打开后，内容区就显示正常了！以后直接双击该CHM文件，就可以正常显示了！
**二、出现空白页面或该页无法显示的错误解决办法**：右键单击CHM文件的，选择“属性”，发现“常规”栏下多出几行字：“安全：此文件来自其他计算机，可能被阻止以帮助保护计算机”点击旁边的“解除锁定”即可解决


**三、“不能打开文件mk:@*.chm”发生原因**：

(1)安装了其他软件造成.chm和hh.exe失去关联

(2)区域设置有问题。（也可能是由于安装某个软件自动修改了区域语言）
**解决办法**：

(1)最简单的只需执行C:\WINNT\system32\dllcache\hh.exe即可。

(2)注册C:\WINNT\system32\hhctrl.ocx (regsvr32 C:\WINNT\system32\hhctrl.ocx)

(3)尝试调整 控制面板-区域设置-语言


**四、XP系统无法打开chm文件的解决方法 **

如果你的操作系统是英文版的，那么你只需在control panel 中将语言设置改为中文即可
**操作步骤**：start－control panel－data,time,language and regional options－regional and language options.在regional options 标签中将语言改为Chinese (PRC)，这样英文版的系统就可以看中文电子书了。



如果中文版系统，可以参考下面的方法：

1，在命令行运行regsvr32 itss.dll

2，在命令行运行regsvr32 hhctrl.ocx

3，右键关联chm文件的“打开方式”到\Windows\HH.exe

如果还没有解决的话，那可能试因为你的XP没有升级到 sp2，hhctrl.ocx版本太低，你可以到别人的系统上copy一个高版本的。

如果电子书能打开，但是右边窗口中显示的是“该页无法显示”，那么把下面的内容存为1.reg，然后双击导入注册表即可

REGEDIT4

[HKEY_LOCAL_MACHINESOFTWAREMicrosoftHTMLHelp][HKEY_LOCAL_MACHINESOFTWAREMicrosoftHTMLHelp1.xHHRestrictions]

"MaxAllowedZone"=dword:00000003[HKEY_LOCAL_MACHINESOFTWAREMicrosoftHTMLHelp1.xItssRestrictions]

"MaxAllowedZone"=dword:00000003

另外：在WINDOWS安装了安全更新 896358 后，下载的CHM文件无法打开最简单的办法是，点击CHM文件的属性，发现“常规”栏下多出几行字：“安全：此文件来自其他计算机，可能被阻止以帮助保护计算机”点击旁边的解除锁定即可解决

**五、在英文操作系统下无法打开**解决办法：解压缩以后，必须放在英文路径下，并且把chm文件本身也改用英文命名

**六、chm文件不能运行**

1，试试改文件名和所在目录为英文名称或是非法文件名  如：Performance Tuning for Linux? Servers.chm应改为Performance Tuning for Linux Servers.chm

2，在控制面板－区域和语言选项－高级　三个中文全选上

3，检查你下载的文件是否不完整或者已经受损，否则应该重新下载,试试把文件名和所在目录改成英文

4，控制面板-管理工具---服务----Help and Support

启用在此计算机上运行帮助和支持中心。如果停止服务，帮助和支持中心将不可用。如果禁用服务，任何直接依赖于此服务的服务将无法启动。

看看这个是否被关闭或禁用了，以及和它相关的服务

5，是HH.EXE的动态链接库文件受损所致。检查一下系统目录下是否有Hh.exe这个文件(如C:＼Windows下)，这个文件用来打开CHM文件。如果破坏，请从别的正常的XP系统下System32

文件夹下复制HHActiveX.dll、hhctrl.ocx、hhsetup.dll等三（或者两个）个文件，到你的机器上的相同下，即可解决

6，如果你确认你下载的文件没问题，系统目录下也有Hh.exe，但仍然打不开，那可能是程序所需链接库文件受损，请下载安装微软的Hhupd.exe就能解决问题。


**七、Hhctrl.ocx文件版本不对  **


解决方法分为两步：   

第一步，确定你的Hhctrl.ocx版本；

第二步，安装一个新版本。   

**具体做法**：
**第一步**：具体做法是：打开我的电脑-〉工具-〉文件夹选项，在查看中，选择显示所有文件，并把下面的两个“隐藏系统文件、隐藏已知的扩展文件”的复选框去掉。然后查找名为Hhctrl.ocx的文件，找到后，查看这个文件的属性，在版本栏中，你会看到这个文件的版本。   
**第二步**：在IE的安装盘上找到Hhupd.cab这个文件，把其中的Hhctrl.ocx解压到windows\system目录中。然后，在开始-〉运行中输入regsvr     c:\windows\system\hhctrl.ocx来注册文件，重启之后，问题解决。



