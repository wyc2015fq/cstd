# cygwin介绍 - 工作笔记 - CSDN博客





2012年01月17日 14:11:13[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5621








What is Cygwin? 

Cygwin 是什么？ 



[Cygwin](http://cygwin.com/) is free software that provides a Unix-like environment and software tool set to users of any modern version of MS-Windows for x86 CPUs (NT/2000/XP/Vista/7) and (using an older version of Cygwin) some
 obsolete versions (95/98/ME) as well. 

Cygwin是自由软件，它为 所有当前x86 CPUs MS-Windows(NT/2000/XP/Vista/7)用户和一些过时的MS-Windows（95/98/ME)用户（用老版本的Cygwin） 提供类似Unix的环境和软件工具集




Cygwin consists of a Unix system call emulation library, 
cygwin1.dll, together with a [vast set](http://cygwin.com/packages/) of GNU and other free software applications organized into a large number of optional packages.


Cygwin包含一个Unix系统调用仿真库，cygwin1.dll，连同一个非常大的GNU和其他免费软件的由大量的可选软件包组成的应用程序集 



Among these packages are high-quality compilers and other software development tools, a complete X11 development toolkit, GNU emacs, TeX and LaTeX, OpenSSH (client and server), and much more, including everything needed to compile and use PhysioToolkit software
 under MS-Windows. 

  在这些软件包中，有高质量的编译器和其他软件开发工具，完整的X11开发工具包，GNU emacs,Tex ,LaTex,OpenSSH(客户端和服务器），等等，包含在MS-Windows环境下编译和使用PhysioToolkit软件所需的一切




Cygwin does **not** provide a means for running GNU/Linux or other Unix binary executables under MS-Windows.


  Cygwin不提供在MS-Windows环境下运行GNU/Linux 或其他Unix二进制可执行文件的方法 



In order to run such software using Cygwin, that software must be compiled from its sources. Cygwin provides all of the components needed to do this in most cases;


  用Cygwin运行这样的软件，必须从它的源码进行编译。大多数情况下，Cygwin都提供编译所需的全部组件； 



 most POSIX-compliant software, including X11 applications, can easily be ported to MS-Windows using Cygwin.


 大多数服从POSIX标准的软件，包括X11应用程序，利用Cygwin能够容易的移植到MS-Windows环境



This page is provided as a service to help PhysioNet users and others get started using Cygwin.


 这个网页提供的服务是帮助PhysioNet用户和其他人开始使用Cygwin



PhysioNet and the author are unaffiliated with the Cygwin project and cannot assist users in installing and using Cygwin.

 PhysioNet 和作者与Cygwin项目没有关联，不能辅助用户安装和使用Cygwin



 For general assistance on Cygwin beyond what is included here, please use the
[resources](http://www.physionet.org/physiotools/cygwin/#more) listed at the end of the page.


 要获得有关Cygwin的超出这里所包含的更一般性的帮助，请使用本页结尾处列出的资源

----------------------------------------------------------------------------------------------------------------------------------------------------------

Installing Cygwin

安装 Cygwin

Overview 

概述

Installing Cygwin begins with downloading and running the Cygwin installer (setup.exe), which is a small application that allows you to choose:


安装 Cygwin 从下载和运行Cygwin安装程序（setup.exe）开始，安装程序是一个小的应用程序来让你选择：

           the Cygwin root directory (where Cygwin will be installed on your system)

           Cygwin根目录（ 在你的系统上Cygwin将被安装的位置）

           which of the many Cygwin mirrors you will use while downloading

           在下载过程中你将使用的很多Cygwin镜像 

           which optional Cygwin packages should be installed 

            可选的Cygwin安装包

as well as a few other installation options. Cygwin packages consist of bzip2-compressed tar (.tar.bz2) archives. The installer downloads these from the mirror you have chosen into a temporary directory, validates them
 by computing their MD5 sums, and then unpacks them into the root directory you have chosen.


以及一些其它的安装选项。Cygwin安装包包含bzip2-压缩的tar(.tar.bz2)文件。安装程序通过你选择的镜像将这些软件包下载到一个临时目录中，通过计算它们的MD5和来验证它们，然后将它们解压到你选择的根目录中



If you have at least 5 Gb free disk space (half of which can be reclaimed after installation), you may choose to perform a
**full installation** of Cygwin with all of its optional packages. 

如果你有至少5GB 可用磁盘空间(其中的一半能够在安装完成后收回)，你可以选择执行Cygwin的完整安装，这将对全部可选软件包进行安装 



This choice is recommended for those who prefer to work in a Unix or GNU/Linux environment but who are constrained to work under MS-Windows.


  这种选择是推荐给那些喜欢在Unix或GNU/Linux环境下工作但是被限制在MS-Windows环境下工作的人 



 A full installation provides a large subset of the software provided as standard components of a typical GNU/Linux distribution.


  完整的安装会提供一个 典型GNU/Linux发行版标准组件的 大的子集   



Allow at least an hour to perform a full installation if you have a fast Internet connection, more otherwise.


  如果你网速快执行完整安装也至少需要一个小时，否则时间会更久 



If you wish only to compile and use existing software under MS-Windows, you may choose a
**custom installation** of Cygwin, consisting of the Base package and a few optional packages listed below; this choice requires less than 1 Gb, and proportionately less time than a full installation.


如果你只希望在MS-WIndows环境下编译和使用现有的软件，你可以选择Cygwin的用户安装，包含基本软件包和一些下面列出的可选软件包；这种选择需要不超过1GB的可用磁盘空间，和完全安装相比会成比例地减少安装时间






 A minimal installation may be upgraded easily at a later time, by following the same procedure as for initial installation. Packages that have previously been installed will not be changed unless newer versions have become available and you choose to install
 them. 

  一个最小安装以后容易升级，升级过程与初始的安装过程相同。先前安装过的软件包将不会发生变化，除非有新版本发行并且你选择安装它们 



When the installation is complete, the Cygwin root directory will resemble that of a typical Unix or GNU/Linux system, with subdirectories
bin (applications and DLLs), 
etc (configuration files), home (containing a subdirectory for each Windows user registered on your system),
lib (static libraries), 
tmp (temporary files), usr (containing additional software),
var (log files), and (depending on the optional packages you choose) perhaps other directories as well. The later sections of this page ([Using Cygwin](http://www.physionet.org/physiotools/cygwin/#using) and
[Using Cygwin/X](http://www.physionet.org/physiotools/cygwin/#using-x)) describe how to access the software once it has been installed.


 当安装完成时，Cygwin根目录将和典型的Unix或GNU/Linux系统类似，包含子目录bin（应用程序和动态链接库），etc（配置文件），home(包含在你的系统上注册过的每个Windows用户的子目录)，lib(静态库），tmp(临时文件），usr（包含额外的软件），var（日志文件），以及其它可能的目录（取决于你选择的可选软件包）。本页后面几节（Using Cygwin 和Using CygwinX）描述在安装完成后如何使用软件



Prerequisites

预备工作



1. Make sure that you are using a version of Windows supported by Cygwin, and that you have sufficient disk space and time available for the installation, as indicated above.


 1. 确保你正在使用的Windows版本支持Cygwin，并且确保你有足够的如前所述的可用磁盘空间和时间来进行安装



2. If your Windows login name contains a space character, consider changing it or creating a separate login for use with Cygwin. The Cygwin installer names your home directory according to your Windows login name. It is usually possible to work around problems
 caused by directory or file names that contain spaces, but they will be a constant irritation; it's better to avoid them.


 2. 如果你的WIndows登录名包含空格字符，考虑改变它或为使用Cygwin创建一个单独的登录。Cygwin安装程序根据你的Windows登录名命名你的home目录。通常由包含空格的目录或文件名引起的问题能够被解决，但是它们将会是一个没完没了的麻烦；最好避免它们。





3. Some virus scanners may interfere with Cygwin installation. If you encounter problems, consider disabling your virus scanner during Cygwin installation and re-enabling it afterwards.


 3. 某些病毒扫描程序可能会干扰Cygwin安装。如果你遇到问题，考虑在安装Cygwin时停用你的病毒扫描程序，等安装结束后重新启用



Cygwin 1.5.x  vs.  Cygwin 1.7.x

Cygwin 1.5.x  和 Cygwin 1.7.x 的对比



 Cygwin 1.7 is the current version as of December 2009. The installation procedure for Cygwin 1.7 has not changed significantly from the procedure for Cygwin 1.5. MS-Windows 95, 98, and ME are incompatible with Cygwin 1.7; if you must use one of these older
 versions of Windows, install Cygwin 1.5. 

 Cygwin 1.7 是2009 年12月发行的目前的版本。Cygwin 1.7 的安装过程和Cygwin 1.5的安装过程相比没有大的变化。MS-Windows 95,98,ME不兼容Cygwin 1.7；如果你必须使用某个老的Windows版本，安装Cygwin 1.5.



  Running the Cygwin installer

运行Cygwin安装程序

1. Click on the "Install Cygwin now" icon at right (or do so on the [Cygwin web site](http://cygwin.com/)). Save the link (setup.exe) to your desktop, then double-click on the saved icon to begin installation.

点击右侧“Install Cygwin now”图标（或在Cygwin网站上做这件事）。将链接（setup.exe）保存到桌面，然后双击保存的图标开始安装



2. A window titled **Cygwin Net Release Setup Program** appears. Click
**Next** to get started. 



[](http://www.physionet.org/physiotools/cygwin/#more)

 标题为Cygwin Net Release Setup Program 的窗口会出现，点击Next



**3.  Choose A Download Source**: Accept the default ("Install from Internet") and click
**Next**. 

选择下载源：使用默认（“Install from Internet”）并点击Next

Choosing the default "Install from Internet" causes the files you will choose in a later step to be downloaded first and then validated and installed. The other choices allow you to perform this procedure in two steps.

选择默认“Install from Internet”会导致在后面步骤中你选择的文件首先被下载然后验证和安装。其它选项会让你分两步执行这一个过程



4. **Select Root Install Directory**: Accept the defaults ("C:/cygwin", All Users, Unix) and click
**Next**. 

选择安装根目录：使用默认（"C:/cygwin", All Users, Unix）并点击Next

*Important:* The default root directory for Cygwin applications is C:/cygwin. Change it if you wish (using '/' rather than '\' as a directory separator, as shown)`, but
*do not choose a location that contains spaces anywhere in its path name*. All files installed by the Cygwin installer go into this directory; to uninstall Cygwin at a later date, simply remove this directory and all of its contents.


If you are installing Cygwin 1.7, you won't see an option to choose the default text file type. If you are installing Cygwin 1.5, you will have a choice as shown above, but don't change the default text file type ("Unix").


重要事项：Cygwin应用程序默认的根目录是C:/cygwin. 可按你的意愿改变它（使用‘/’而不是‘\’作为目录分隔符，如图所示------此处叙述和图形提示有矛盾），但是不要选择含有空格的路径。由Cygwin安装程序安装的所有文件都在这个目录中；在以后要想卸载Cygwin，简单地移除这个目录和它包含的所有内容

如果你正在安装Cygwin 1.7，你将看不到选择默认文本文件类型的选项。如果你正在安装Cygwin 1.5，你将有上图所示的选择，但是不要改变默认的文本文件类型（“Unix”）。



5. **Select Local Package Directory**: Accept the default or change it to any temporary directory of your choice, but make a note of it.


选择本地安装包目录：接受默认或改成你选择的任何一个临时目录， 但你要记住它



After the installation is complete, you may delete the 
.tar.gz package files that will have been written there. If you have chosen a full installation, these files may occupy over a gigabyte, and they are no longer needed once their contents have been installed. If you plan to install Cygwin on another PC,
 however, you may be able to save time by copying the local package directory and the Cygwin installer to the second PC; follow these instructions to install Cygwin on the second PC, but choose "Install from Local Directory" in step 3.

安装完成后，你可以删除本地安装包目录中那些被写入的.tar.gz包文件。如果你选择的是完全安装，这些文件可能占用超过1G的磁盘空间，安装完成后就不再需要它们。如果你打算在其它的PC上安装Cygwin，然而，你也许能够节省时间通过将本地安装包目录和Cygwin安装程序拷贝到待安装的PC上；同样遵循这些安装说明，但是在第三步中选择“从本地目录安装”



6. **Select Your Internet Connection**: The default should be correct for most users. Change it only if you encounter problems.


选择你的网络连接： 对大多数用户接受默认应该就会正确。只有在你遇到问题时才去改变它。



7. **Choose A Download Site**: Select a nearby mirror site from which to download the Cygwin packages. Speeds may vary considerably from site to site.


At this time, the installer downloads a list of available packages from the site you have chosen. Normally, this takes only a few seconds; if there is a lengthy pause, you may wish to start over and choose a different mirror site

选择下载网站：选择一个附近的镜像站点下载Cygwin元件包。不同站点的下载速度可能变化很大

在这个时候，安装程序从你选择的网站下载一系列的可用软件包。正常情况下，这个过程仅需花费几秒的时间；若果出现长时间的停顿，你可能会希望重新开始并选择一个不同的镜像站点



8. **Select Packages**: If you wish to do a **full installation**, click on the rotating selector next to "All" (at the top of the Category list) so that the indicator to its right changes from "Default" to "Install". Click
**Next** and skip ahead to step 10. 

 8 . 选择安装包：如果你愿意做完整安装，点击“ALL”附近的旋转选择器（在类别列表顶部），于是右侧的指示器从“Default”变成“Install”。点击Next 并跳至步骤10




9. If you wish to do a **custom installation**, click the **View** button so that the indicator to its right changes from "Category" to "Full".


You may resize the dialog box as needed until the package names are visible. 

如果你希望做一个自定义安装，点击View 按钮，使得在它右端的指示器从“Category”变成“Full”。你可以按你的需要调整对话框的大小直到可以看到软件包的名字



To select a package for installation, click on its rotating selector (in the **New** column) to cycle through the available choices until a version number appears. In most cases, you should choose the highest version number from those that are available (this is usually the first alternative offered).

 要选择一个用于安装的软件包，点击它的旋转选择器（在名为New的列）来循环显示可用的选项直到一个版本号出现。大多情况下，你应该从可用的选项中选择最高的版本号（这常常是提供的多个选择中的第一个）



In the example below, the **sunrpc** package, version 4.0-2, has been selected for installation. Note that the entry in the
**Bin?** (Binary) column changes from n/a to a checked box when you select a numbered version of the package. Leave this box checked. The entry in the
**Src?** (Source) column changes from n/a to an unchecked box, which you may check if you would also like to download the sources for the package.


在下面的例子中，软件包sunrpc，版本4.0-2，被选择安装。注意到当你选择安装包的带编号的版本时，在Bin？（Binary）列中的条目从n/a变成一个已被选中的框。保持此框的选中状态。在Src？（Source）列中的条目从n/a变成一个未被选中的框，如果你还想下载这个软件包的源码，你可以选中它。



If you plan to use PhysioToolkit software, be sure to choose at least the following packages for installation:


如果你打算使用PhysioToolkit软件，至少要选择安装以下软件包：

**bc** (Math, Utils) 

**diffutils** (Utils) 

**gcc** (Devel) 

**gcc-g77** (Devel) 

**gcc-mingw** (Devel)

**gv** (Graphics, Text, X11)

**ImageMagick** (Graphics) 

**libcurl-devel** (Devel, Net, Web)

(this package is named **curl-devel** in Cygwin 1.5)

**libX11-devel** (X11) 

**make** (Devel) 

**sunrpc** (Libs) 

**twm** (X11) 

**X-start-menu-icons** (X11) 



(The categories to which each package belongs are shown following the package names above, for reference.) You may choose any other desired packages, either at this time or later on (by running Cygwin setup again). The Cygwin installer will automatically download
 any additional packages needed to satisfy dependencies of those you select. When you have completed your selections, click
**Next**.

（上面软件包名称后面显示的是每个软件包所属的类别，以供参考。）你可以选择任何其他想要的软件包，或者就在这个时候或者在以后（通过再次运行Cygwin安装程序）



10. On Cygwin 1.7, setup may show a window with a warning ("Unmet Dependencies Found"), with a list of additional packages required by those you have already selected. At the bottom of the window, be sure that "Install these packages to meet dependencies (RECOMMENDED)"
 is checked, then click **Next**. 

关于Cygwin 1.7，安装程序可能会显示一个警告窗口（"发现未满足的依赖关系" ），带有一系列 已选择的软件包必需的 其他软件包。在窗口的底部， 确保“安装这些软件包以满足依赖关系（推荐）”被选中，然后点击Next。



The downloading process begins once the packages have been selected. The installer indicates its progress.


 一旦选择完软件包，下载过程便开始。安装程序提示安装进度



Be patient; the downloading and installation process may take an hour or more (depending not only on the speed of your Internet connection, but also on the load on the Cygwin mirror site from which you are downloading).

耐心点，下载和安装过程可能要花费一个小时甚至更久（不仅依赖于你的网速，还依赖于你选的Cygwin镜像站点的负载）。 



11. Once all selected package files have been downloaded and checked, they are unpacked into the Cygwin root install directory.


 11. 一旦所有选择的软件包文件下载完成并通过验证，它们会被解压到Cygwin安装根目录。



12. Following package installation, any installation-dependent configuration scripts are run to complete the setup process. (There may be lengthy pauses during this step, without any indication of progress. Be patient!)


 伴随着软件包的安装，任何与安装相关的配置脚本被运行，用来完成安装过程。（在这个步骤中可能有长时间的暂停，没有任何进度的提示。耐心等待！）

13. **Create Icons**: Unless these icons already exist from a previous Cygwin installation, make sure the boxes are checked and click
**Finish**. 

13. 创建图标： 除非这些图标 因为先前曾安装过Cygwin 已经存在，确保选框被选中并点击Finish按钮。

That's all! 

 结束！



You may always run Cygwin setup again to obtain additional or updated packages without reinstalling packages that remain current; if you do this, be sure to shut down any running Cygwin applications first.


 你总是可以再次运行Cygwin安装程序获得另外的或更新了的软件包，而不用重新安装已有的软件包；  如果你这样做，一定要先关闭所有正在运行的Cygwin应用程序。



