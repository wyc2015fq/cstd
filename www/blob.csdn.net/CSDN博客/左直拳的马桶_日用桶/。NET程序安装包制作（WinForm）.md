# 。NET程序安装包制作（WinForm） - 左直拳的马桶_日用桶 - CSDN博客
2006年02月27日 23:54:00[左直拳](https://me.csdn.net/leftfist)阅读数：3579
**。****NET****程序安装包制作（****WinForm****）**
**一、新建项目—安装和部署项目—安装项目**
**二、选择安装文件。**
1、进入文件系统视图。方法为选定该项目，鼠标右键，弹出快捷菜单，选视图-文件系统。也可以直接点“解决方案资源管理器”上部的快捷图标。
2、在“应用程序文件夹”里添加程序需要用到的文件，例如EXE文件，配置文件，图标文件，等等。
3、用户的程序菜单创建一文件夹，命名，这个就是将来出现在用户的程序菜单中的文件夹。注意如果该文件夹的名字若与现有的程序菜单相同，则新安装的程序菜单项会与现有的挤在一块，如果子菜单项名字也一样，则会覆盖原有。
4、创建EXE文件快捷方式，改成适当的名字，剪切粘贴到“用户的程序菜单/文件夹”和“用户桌面”。鼠标右键看属性，ICON，从“应用程序文件夹”中选好对应的图标文件。这样安装后，程序将会在程序组以及桌面显示快捷方式。
**三、用户界面**
安装过程中，每个窗口上面会显示一幅图片。如果不指定，将会用默认图片代替。通常，应该用个性化的图片代替。同样，这张图片也应预先放在“应用程序文件夹”里。
1、进入用户界面视图。方法为选定该项目，鼠标右键，弹出快捷菜单，选视图-用户界面
2、将各个窗体的bannerBitmap改为个性化的图片。
**四、项目属性**
选定该项目，然后选菜单栏“视图”—“属性窗口”，将productname及title改为软件名称，它们在安装过程中会显示。注意，要用菜单栏中的“视图”—“属性窗口”，如果直接用鼠标右键，出来的是一个似乎没用的属性窗口。
拷贝productcode；切换到文件系统视图，选“用户的程序菜单”，找到卸载文件unInstall.exe的快捷方式，鼠标右键看属性，在属性Arguments里粘贴刚才复制的productcode。
UnInstall.exe是自行编写的一个程序，它的main()函数支持参数。将安装软件的productcode传给它，以保证准确地将其卸载。
**五、安装路径**
切换到文件系统视图，选“应用程序文件夹”，属性，将[ProgramFilesFolder][Manufacturer]/[ProductName]中的[ProductName]改成打算安装后的文件夹名称，并且去掉[Manufacturer]。因为在前面已经将ProductName改成了中文，为稳妥起见，避免安装后的文件夹为中文，在这里改回英文。
**六、启动条件**
1、.Net FrameWork
.Net程序需要.Net框架支持。因此系统会自动将.NET作为启动条件。这个不用怎么修改。
假如想安装包自带.NET框架，在目标机器没有安装.Net框架的情况下自动安装，则需要在我们自己的机器上安装引导程序，然后再制作安装包，这样做好的包里，会自动有一份dotnetfx.exe文件。
有关引导程序的信息，如下：
Visual Studio .NET 2003 中的 Setup 项目可以用来使 .NET 框架与应用程序一起安装，但必须首先在开发计算机上安装可选的Visual Studio .NET 框架引导程序插件。
**注**Visual Studio .NET 框架引导程序插件可以从[Download Center](http://go.microsoft.com/fwlink/?LinkId=23249)下载。
Visual Studio .NET 框架引导程序插件用于修改 Setup 项目的**Bootstrapper**属性的行为，以便包括 .NET 框架引导程序以及 Windows Installer 引导程序。然后，.NET 框架可重新分发软件包和适当的语言包将与应用程序一起打包；一个启动条件会在安装时检查是否有正确版本的 .NET 框架，如果需要，则在安装应用程序之前安装它。
**注**Visual Studio .NET 框架引导程序插件不会修改 Web 引导程序。将**Bootstrapper**属性设置为**Web Bootstrapper**将只安装 Windows Installer 引导程序。
此外，如果应用程序需要进行数据访问，则可以与安装程序一起安装 Microsoft 数据访问组件 (MDAC)。
Visual Studio .NET 框架引导程序插件还支持安装 .NET 框架的本地化版本，这些本地化版本可以是 Visual Studio .NET 所支持的八种语言中的任意一种。对于您希望支持的每种语言，必须安装单独的插件版本；它们可以从与英语版本相同的位置下载。Setup 项目的 Localization 属性用于确定将被引导的框架的版本。有关详细信息，请参阅[Installing Localized Applications](http://msdn.microsoft.com/library/en-us/dnnetdep/html/vsredistdeploy.asp?FRAME=true#vsredistdeploy_topic4)。
2、MDAC
如果程序需要用到数据库，也要将MDAC作为启动条件。
1）在“搜索目标计算机”里添加“注册表搜索”，命名为搜索MDAC。
在属性窗口中，选择 RegKey 属性，并键入 Software/Microsoft/DataAccess。
选择 Root 属性，并选择 vsdrrHKLM。
选择 Value 属性，并键入 FullInstallVer。
选择 Property 属性，并键入 MDACSEARCH。
2）在启动条件中添加一条件，命名为“MDAC”
选择该条件，在属性窗口中，选择 Condition 属性，并选择 MDACSEARCH>="2.6"。
