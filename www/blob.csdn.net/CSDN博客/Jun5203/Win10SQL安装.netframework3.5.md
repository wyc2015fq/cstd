# Win10SQL安装.netframework3.5 - Jun5203 - CSDN博客
2018年07月25日 09:26:21[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：316
##前言
前几天因为Windows.old，害的我迫不得已重装系统，然后问题就扑面而来，所有的软件重装，配置重配.这不又遇上了SQL Server问题。
主要问题有：
1、控制面板——程序——启用或关闭Windows功能——.net framework 3.5（不可用），显示如下图
![这里写图片描述](https://img-blog.csdn.net/20180725092138404?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、[安装下载.NET](http://xn--ghqt24aum9avvf.NET) Framework3.5（不可用），显示如下图
![这里写图片描述](https://img-blog.csdn.net/20180725092152520?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由此站在巨人的肩膀上，我找了出现以上问题错误代码0x800F081F的原因：
当指定了备用安装源且满足以下任一条件时，可能会出现此错误代码：
· 路径所指定的位置不包含安装该功能所需的文件。
· 尝试安装此功能的用户对该位置和文件不具有最起码的读取权限。
· 安装文件集已损坏、不完整或对您正在运行的 Windows版本无效。
若要解决此问题，请确保源的完整路径是正确的(x:\sources\sxs)，并且你对该位置至少拥有读取权限。 若要执行此操作，请尝试直接从受影响的计算机访问源。验证安装源包含有效且完整的一组文件。 如果问题仍然存在，请尝试使用一个不同的安装源。
##解决方案
解决方法有很多，但是并不一定都适用，下面我给大家阐述一下我用到的方法。
**方法一：**
1、“Windows+X”点击“命令提示符管理员”，输入“SFC/scannow”回车（please wait a minute…），再输入“dism /online
/cleanup-image /restorehealth”回车（please wait a minute…）
2、导致安装不上的原因还有可能是你的注册表里残留的无用注册表引起的（比如删除过的用户信息注册表、过期软件注册表…），可以去百度上download一个注册表清理工具，很快就能清理完。
3、以上两步都完成后，链接：[https://pan.baidu.com/s/1YcsXQepVIg0jqnB_aBKecw](https://pan.baidu.com/s/1YcsXQepVIg0jqnB_aBKecw) 密码：lc9v。解压ios镜像文件（打开介质文件）找到sxs文件夹，复制到C盘根目录下。
**“芝士暴击”：**根目录指逻辑驱动器的最上一级目录，它是相对子目录来说的。打开“我的电脑”，双击C盘就进入C盘的根目录，双击D盘就进入D盘的根目录。
**方法二：**
首先，你需要下载cab版的.net3.5文件：链接：[https://pan.baidu.com/s/1xuZF4TKtFMIpyosLet0qjg](https://pan.baidu.com/s/1xuZF4TKtFMIpyosLet0qjg) 密码：ueps。解压将文件复制到C盘的Windows文件夹里，然后打开“命令提示符（管理员）”或者“Wiondows PowerShell（管理员）”。输入以下命令，再按回车：dism /online/Enable-Feature /FeatureName:NetFx3 /Source:"%windir%"/LimitAccess
操作完成之后必须重启一次电脑。
**方法三：**
我的电脑——管理——服务和应用程序——服务——Windows Update-属性——启动类型（手动）——启动——确定
##心得体会
方法总比困难多，只要有解决问题的恒心，问题也不是问题。如果大家还有什么好的方法欢迎随时交流哈~
