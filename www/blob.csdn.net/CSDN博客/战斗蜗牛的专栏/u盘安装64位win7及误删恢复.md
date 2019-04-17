# u盘安装64位win7及误删恢复 - 战斗蜗牛的专栏 - CSDN博客





2012年08月13日 20:46:34[vbskj](https://me.csdn.net/vbskj)阅读数：2551








1）使用u盘安装，发现pe不是64的；

2）利用ultraiso，写入硬盘映像，发现找不到驱动；

3）在win7下利用diskpart命令

不使用刻录U盘的方式，而是使用diskpart命令（vista、win7下都支持），将U盘做成引导盘，在用UltraISO将安装镜像中的文件提取到U盘，这时，U盘的内容可以完全访问，直接用U盘启动安装就可以了，就如同用光盘安装一样。我昨晚已采用这种方式在thinkpad x201上安装了win7 64旗舰版了。









win7下运行 CMD 进入命令提示行（记得用管理员身份）


C:\Windows\system32>diskpart




DISKPART> list disk

　　如果你的机器只有一块[硬盘](http://wenwen.soso.com/z/Search.e?sp=S%E7%A1%AC%E7%9B%98&ch=w.search.yjjlink&cid=w.search.yjjlink)，那么U盘应该显示为Disk
 1


　　DISKPART> select disk 1

　　选择U盘为当前[磁盘](http://wenwen.soso.com/z/Search.e?sp=S%E7%A3%81%E7%9B%98&ch=w.search.yjjlink&cid=w.search.yjjlink)


　　DISKPART> clean

　　清空磁盘


　　DISKPART> create partition primary

　　创建[主分区](http://wenwen.soso.com/z/Search.e?sp=S%E4%B8%BB%E5%88%86%E5%8C%BA&ch=w.search.yjjlink&cid=w.search.yjjlink)


　　DISKPART> select partition 1

　　选择分区


　　DISKPART> active

　　激活分区（一定要做，不然不能启动）


　　DISKPART> format fs=ntfs quick
[快速格式化](http://wenwen.soso.com/z/Search.e?sp=S%E5%BF%AB%E9%80%9F%E6%A0%BC%E5%BC%8F%E5%8C%96&ch=w.search.yjjlink&cid=w.search.yjjlink)为NTFS文件系统


　　DISKPART> assign letter=[ ]

　　指定[卷标](http://wenwen.soso.com/z/Search.e?sp=S%E5%8D%B7%E6%A0%87&ch=w.search.yjjlink&cid=w.search.yjjlink)，［］能不可与现存盘符重复，也可不加参数使用默认。


　　DISKPART> exit

　　退出Diskpart命令模式。


3、其次-复制安装文件到U盘


　　把Win7或Vista[镜像文件](http://wenwen.soso.com/z/Search.e?sp=S%E9%95%9C%E5%83%8F%E6%96%87%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink)中的文件直接[解压缩](http://wenwen.soso.com/z/Search.e?sp=S%E8%A7%A3%E5%8E%8B%E7%BC%A9&ch=w.search.yjjlink&cid=w.search.yjjlink)到U盘中，或者，用[虚拟光驱](http://wenwen.soso.com/z/Search.e?sp=S%E8%99%9A%E6%8B%9F%E5%85%89%E9%A9%B1&ch=w.search.yjjlink&cid=w.search.yjjlink)虚拟下镜像，打开虚拟光驱目录，把所有安装文件Copy进U盘。


4、BIOS中设置好U盘启动，插入U盘，启动[计算机](http://wenwen.soso.com/z/Search.e?sp=S%E8%AE%A1%E7%AE%97%E6%9C%BA&ch=w.search.yjjlink&cid=w.search.yjjlink)



- 










4）但是在安装win7过程中，错误的选择目标盘，结果把xp覆盖了，幸亏没有选择格式化，而是把原始系统文件放置在windows.old下面。下面给出，如何进行恢复：




## 利用windows.old还原旧系统




每次微软出来新系统都想尝尝鲜，但是兼容性又是一个纠结的问题。这烫手的山芋真的令人又爱又恨。难道就没有两全其美的办法了吗？答案是有的！！如果你在安装新操作系统时候不格式化c盘，细心的你一定发现了c盘内有一个windows.old文件夹。对了这就是之前存在c盘的所有文件拉！


下面从微软官方复制过来的恢复旧系统的方法，你也可以访问官方帮助：[http://support.microsoft.com/kb/971760](http://support.microsoft.com/kb/971760)直接阅读。




本文介绍如何在计算机上手动还原以前安装的 Windows 来替换当前安装的 Windows 7。

为此，必须使用命令提示符，并且必须在命令提示符处键入特定命令，以重命名文件夹并在不同版本的 Windows 之间移动文件夹。
**注意：**如果您的 Windows 7 当前已安装 Service Pack 1，则卸载步骤与没有安装 Service Pack 的 Windows 7 相同。如果您已经还原到以前安装的 Windows 7，则可能需要向该 Windows 7 安装 Service Pack 1。

本文适用于初级和中级计算机用户。


安装 Windows 7 的某个版本后将计算机还原到以前安装的 Windows 的步骤您可能会发现，事先将本文打印出来会更易于按步骤操作。步骤 1：确定是否存在 Windows.old 文件夹以及 Windows 硬盘上是否有足够的可用空间单击“开始”![](http://support.microsoft.com/library/images/support/kbgraphics/Public/EN-US/VistaStartButton.jpg)，然后单击“计算机”。在“查看”菜单上，单击“详细信息”。在“可用空间”列中，注意“硬盘”区域下面“本地磁盘(C:)”的可用空间大小。在“硬盘”区域，双击“本地磁盘(C:)”，然后确定是否存在
 Windows.old 文件夹。
**重要说明**：如果 Windows.old 文件夹不存在，则无法按照本文中的步骤来还原该计算机上以前安装的 Windows。您必须先备份然后再还原，或者将您的文件转移到以前的操作系统。右键单击“Windows.old”文件夹。Windows 7 将在几秒钟后确定该文件夹的大小。确定 Windows.old 文件夹是否小于在步骤 1.2 中看到的“本地磁盘 (C:)”的可用空间大小。
**注意**：如果 Windows.old 文件夹的大小是“本地磁盘 (C:)”项可用空间的两倍，您也许不能还原以前的 Windows 安装。


步骤 2：启动 Windows 恢复环境。将 Windows 7 安装光盘放入 DVD 驱动器中，然后重新启动计算机。在系统提示时按任意键，从光盘重新启动。在“安装 Windows”窗口中，选择语言、时间、货币、键盘输入方法或其他输入方法，然后单击“下一步”。在“安装 Windows”窗口中，单击“修复计算机”。在“系统恢复选项”窗口中，单击要修复的 Windows 7 操作系统的版本，然后单击“下一步”。在“系统恢复选项”窗口中，单击“命令提示符”。“命令提示符”窗口将会打开，其中显示了命令提示符。在命令提示符中键入以下步骤中描述的命令。


步骤 3：将 Windows 7 文件夹移动到新的 Win7 文件夹

**注意**当您在命令提示符下键入以下步骤中描述的一个或多个命令并按 Enter 后，可能会收到以下消息：系统找不到指定的文件。如果收到此消息，请转到本节中的下一个步骤，然后在此下一步骤中键入该命令。

键入以下命令，并在每个命令后按 Enter：C: 

Md Win7 

Move Windows Win7\Windows

Move "Program Files" "Win7\Program Files" 

Move Users Win7\Users 

Attrib –h –s –r ProgramData 

Move ProgramData Win7\ProgramData 

Rd "Documents and Settings"


步骤 4：复制或移动 Windows.old 文件夹中的内容**注意**当您在命令提示符下键入以下步骤中描述的一个或多个命令并按 Enter 后，可能会收到以下消息：系统找不到指定的文件。如果收到此消息，请转到本节中的下一个步骤，然后在此下一步骤中键入该命令。

键入以下命令然后在每个命令后按 Enter：


move /y c:\Windows.old\Windows c:\ 

move /y "c:\Windows.old\Program Files" c:\ 

move /y c:\Windows.old\ProgramData c:\ 

move /y c:\Windows.old\Users c:\ 

move /y "c:\Windows.old\Documents and Settings" c:\


步骤 5：还原以前安装的 Windows 的启动扇区根据您的情况，在命令提示符下键入以下命令之一。
**注意**：在以下命令中，D:代表 DVD 驱动器。如果计算机上的 DVD 驱动器由不同的驱动器号（例如E:）来表示，请在命令中使用该驱动器号。

**如果以前安装的 Windows 是 Windows Server 2003、Windows XP 或 Microsoft Windows 2000 **请键入以下命令，然后按 Enter：D:\boot\bootsect /nt52 c:

**如果以前安装的 Windows 是 Windows Vista **请键入以下命令，然后按 Enter：


D:\boot\bootsect /nt60 c:


步骤 6：还原以前安装的 Windows XP 或 Windows 2000 的 Boot.ini 文件**注意**只有当以前安装的是 Windows XP 或 Windows 2000 时才执行以下步骤。

键入以下命令并在每个命令后按 Enter：


Attrib –h –s –r boot.ini.saved 

Copy boot.ini.saved boot.ini


步骤 7：关闭命令提示符窗口，然后单击“重新启动”在命令提示符处键入下面的命令，然后按 Enter：exit


单击“重新启动”以重新启动计算机。**注意** 验证 Windows XP 正常工作后，如果 C:\Win7 文件夹不需要用于数据恢复，您可以将其删除。




哈哈！举一反三的机会到了，现在windows 8也出来了，用同样的方法也可以将系统还原为以前的系统哦！代码与步骤如下：


将 Windows 7 安装光盘放入 DVD 驱动器中，然后重新启动计算机。按shift+f10，启动命令提示符：

键入以下命令，并在每个命令后按 Enter：

(注C代表系统盘，D盘代表光驱所在盘符，可根据实际需要修改，建议对电脑比较熟的人操作，关于引号的添加，下述中文件名之间有空格的需加引号)**C: Md Win8 Move Windows Win8/WindowsMove "Program Files" "Win8/Program Files" Move Users Win/8Users Attrib –h –s –r ProgramData Move ProgramData Win8/ProgramDataRd "Documents and Settings"move /y c:Windows.old/Windows c: move /y "c:Windows.old/"Program Files" c:move /y c:Windows.old/ProgramData c: move /y c:Windows.old/Users c: move /y "c:Windows.old/Documents and Settings" c:D:\boot\bootsect\nt 60c:**







