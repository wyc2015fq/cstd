# 软件安装时Error 1335 - cannot copy错误的解决方法 - xqhrs232的专栏 - CSDN博客
2010年03月01日 15:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3267
原文地址：：[http://www.jysls.com/thread-232584-1-1.html](http://www.jysls.com/thread-232584-1-1.html)
相关文章地址：：[http://www.hackbase.com/subject/2009-07-14/14558.html](http://www.hackbase.com/subject/2009-07-14/14558.html)
[http://wenwen.soso.com/z/q165169212.htm?pid=wenwen.autologin](http://wenwen.soso.com/z/q165169212.htm?pid=wenwen.autologin)
在安装Office 2003以及其他软件（如Adobe Acrobat）时出现Error 1335，上网搜索查到M$的帮助信息，按帮助进行操作，发现按方法2进行操作可以使原来出错的软件正常安装，但对于Office 2003还是无法解决问题。考虑中机器增加过内存，把增加的内存拔下来，此时安装就没有问题了。更换内存，问题解决。
下面为M$对Error 1335的详细解释：
症状
当您尝试安装 Microsoft Office 2000 或 Microsoft Office XP 时，可能会收到以下错误信息：
Error 1335-Cannot copy cab file Office1.cab.The file may be corrupt.
当您以安全模式启动计算机并尝试安装 Office 时，可能会收到以下错误信息：
The Windows Installer Service could not be accessed.This can occur if you are running Windows in Safe mode or if the Windows Installer is not correctly installed.
即使 Office 光盘程序包能够在其他计算机上正常工作，您仍可能收到此错误信息。
在使用管理权限登录计算机并尝试安装 Office 时，您会收到相同的错误信息。
原因
导致此错误的原因可能是媒体不洁净、存在 CD-ROM 读取问题或者存在内存分配问题。
替代方法
要变通解决此问题，请尝试使用下列方法。
方法 1：清洁光盘
使用软布小心地清洁光盘。
方法 2：更改安装的内存设置
要更改安装的内存设置，请按照下列步骤操作：
1. 单击开始，然后单击运行。
2. 在打开框中，键入 msconfig，然后单击确定。
3. 在 BOOT.INI 选项卡上，单击高级选项。
4. 在 BOOT.INI 高级选项对话框中，选中 /MAXMEM= 复选框，然后将设置更改为 256（如果计算机具有较少的 RAM，应当设置为更小的值）。
5. 单击确定关闭 BOOT.INI 高级选项对话框。
6. 在系统配置实用工具对话框中，单击应用。
7. 单击确定关闭系统配置实用工具对话框。
8. 单击重新启动以响应以下消息：
您必须重新启动您的计算机以便某些由系统配置所作的更改生效。
9. 重新启动计算机后，将 Microsoft Office 作为完全安装进行安装。
10. 安装 Microsoft Office 后，执行步骤 1 到 8 以清除 /MAXMEM= 复选框。
方法 3：使用平面文件夹安装 Office
要使用平面文件夹安装 Office，请按照下列步骤操作： 1. 干净启动计算机。有关如何对 Windows XP 执行干净启动的其他信息，请单击下面的文章编号，以查看 Microsoft 知识库中相应的文章：
310353 如何在 Windows XP 中执行全新启动
2. 在计算机的硬盘上保存 Microsoft Office 光盘的副本。为此，请按照下列步骤操作： a. 启动 Windows 资源管理器。为此，请右键单击开始，然后单击快捷菜单上的资源管理器。
b. 在硬盘卷的根目录下创建一个新文件夹，然后将其命名为“Flatfile”（例如 C:/Flatfile）。
c. 浏览到包含 Microsoft Office 光盘的驱动器，然后将 Microsoft Office 光盘的全部内容复制到 C:/Flatfile 文件夹中。为此，请按照下列步骤操作： 1. 在 Windows 资源管理器中，单击包含 Microsoft Office 光盘的 CD-ROM 图标（或 DVD-ROM 图标）。
2. 在编辑菜单上，单击全选。
3. 将所有文件粘贴到新的文件夹中。为此，请将资源管理器窗口右侧的选定项拖动到左侧的 C:/Flatfile 文件夹中。
注意：如果收到错误信息
This Folder contains "X" hidden files
请按照下列步骤操作： a. 在“工具”菜单上，单击“文件夹选项”。
b. 单击“查看”选项卡。
c. 打开“显示隐藏文件和文件夹”。
d. 关闭“隐藏受保护的操作系统文件”。
e. 单击“应用”，然后单击“确定”。
注意：如果在复制过程中发生错误，可能表明光盘媒体有问题、CD-ROM 驱动器已脏或者硬盘有问题。
3. 以正常方式重新启动计算机，然后从 C:/Flatfile 文件夹将 Microsoft Office 安装到硬盘上。
