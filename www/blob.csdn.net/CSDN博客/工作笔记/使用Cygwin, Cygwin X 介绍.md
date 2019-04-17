# 使用Cygwin, Cygwin/X 介绍 - 工作笔记 - CSDN博客





2012年01月17日 13:39:37[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6127标签：[windows																[unix																[terminal																[服务器																[server																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)





## **Using Cygwin**

**As noted, Cygwin provides a Unix-like environment under Windows. The installation directory (by default,c:\cygwin) is the*root*of the Unix-like file system, which containsbin,etc,home,tmp, andusrdirectories as would be found on a GNU/Linux or other Unix system. Withinhomewill be one or more subdirectories, each allocated to a Windows user.**

**如前所述，Cygwin提供Windows下类Unix环境。安装目录（默认是c:\cygwin）是类Unix文件系统的root目录，像在GNU/Linux 或其它Unix系统上可以看到的那样包含bin，etc，home，tmp，及usr目录。home 目录中将会有一个或多个子目录，每一个分配给一个Windows用户。**



**To begin, click on the Cygwin desktop icon, or choose the Cygwin entry from your start menu, to open a Cygwin terminal window. Within this window, the GNUbashshell is running, with POSIX syntax (directory separators are '/', not '\'). Initially, the current (working) directory is/home/*user*, where*user*is your Windows login name. Don't use this directory if your Windows login name contains a space; make another and use that one instead, e.g., by typing these commands at thebashprompt:**

**首先，点击桌面上的Cygwin图标，或从开始菜单中选择Cygwin项，打开一个Cygwin终端窗口。在窗口中，GNU bash shell正在运行，用的是POSIX语法（目录分隔符是‘/’，不是‘\’）。刚开始，当前（工作）目录是 /home/user， 其中user是你的Windows登陆名。如果你的Windows登陆名中包含空格就不要使用这个目录；建立并换使用另外一个目录，例如，在bash中输入这些命令提示符：**
**mkdir /home/bob          ----------------------home 中创建bob目录    echo "export HOME=/home/bob" >>.bashrc ---------将双引号中的字符串附加到.bashrc中,不删除.bashrc原来内容         echo "export HOME=/home/bob" >>.bash_profile  -----------同上    cp .bashrc .bash_profile /home/bob            -----------拷贝文件.bashrc 和.bash_profile 到/home/bob    echo "cd" >>.bashrc                           -----------和上面echo命令行含义相同****Close your Cygwin terminal window and open another one; your current directory should now be/home/bob(or whatever you chose to call it). See the[Cygwin FAQ](http://cygwin.com/faq.html)(look for "My Windows logon name has a space in it") for other solutions to this common problem.****关闭Cygwin终端窗口并再次打开；你的当前目录现在应该是/home/bob（或任何一个你命名的目录）。参考Cygwin常见问题（找到"My Windows logon name has a space in it"）获得这个问题的其它解决方法。**
### **Finding Cygwin files with Windows applications, and vice versa**





**利用Windows应用程序查找Cygwin文件，反之亦然**



**Pathnames of files are often confusing to new Cygwin users. A pathname is simply a set of directions for finding a file. An*absolute*pathname begins at a fixed location (the*root*of the file system). The confusion arises because native Windows programs usec:\as their root, and Cygwin programs usec:\cygwin(unless you chose some other place to install Cygwin).**

**文件路径名常常令Cygwin新手感觉混淆。简单来讲路径名是用来查找文件的一组方向。绝对路径名从一个固定的位置开始（文件系统的根目录）。这种混淆的出现是由于本机Windows程序使用c:\作为它们的根目录，而Cygwin程序使用c:\cygwin作为根目录（除非你是选择某个其它地方安装的Cygwin）。**



**This means, for example, that a file created using a Cygwin program as/home/bob/foo.txtcan be read using a native Windows program asc:\cygwin\home\bob\foo.txt(the two pathnames refer to the same disk file).**

**举个例子，这意味着用Cygwin程序创建的路径是/home/bob/foo.txt 的文件能够被本机Windows程序通过路径c:\cygwin\home\bob\foo.txt读取（两个路径名指向相同的磁盘文件）。**

**---------------------------------------------------------上面讲的是Windows程序访问Cygwin文件----------------------------------------------------------------**

**If you need to use a Cygwin program to read or write a file located outside ofc:\cygwin, you can use a pathname beginning with/cygdrive/c/for this purpose. For example, a native Windows file calledc:\My Documents\hello.ccan be found by a Cygwin program at/cygdrive/c/My\ Documents/hello.c(note how the space character in "My Documents" must be prefixed by "\" to force it to be treated as part of the pathname). If you have a Windows d: drive, use/cygdrive/d/in the same way to access its contents from Cygwin programs.**

**如果你需要利用Cygwin程序读写位于c:\cygwin目录以外的文件，你可以使用以/cygdrive/c/开头的路径名实现这一目的。举个例子，一个本机Windows文件名为c:\My Documents\hello.c能够被Cygwin程序/cygdrive/c/My\ Documents/hello.c找到（注意"My Documents"中的空格字符必须加前缀“\”使它被当做路径名的一部分）。如果你有d盘，使用/cygdrive/d/前缀，Cygwin程序就可用同样的方式访问它的内容。**



**Unix/Linux files occasionally have names that differ only in case (a frequent example ismakefileandMakefile). Be careful: although Windows does preserve the original case of characters in filenames, it ignores case when looking for matches to file names. So, for example, if you write a file calledbaz.zipand then another calledBaz.zip, the first one will be overwritten. This behavior might be no surprise to Windows users, but it may be unexpected to Unix users.**

** Unix/Linux 文件名有时仅在大小写上不同（常见的例子是makefile和Makefile）。小心：虽然Windows确实会保留文件名中字符的原始大小写，当查找匹配的文件名时会忽略大小写。因此，举个例子，如果你写一个名为baz.zip的文件，然后又写一个名为Baz.zip的文件，第一个文件将被覆盖。这种行为可能对Windows用户司空见惯，但是对Unix用户可能是意想不到的。**

**---------------------------------------------------------------------------------------------------------------------------------------------------------------------------**

## **Using Cygwin/X**

**The X Window System, version 11 (often "X11", or simply "X") is the standard graphical environment under Unix and GNU/Linux; it is also available for other platforms, including Mac OS X and MS-Windows. X applications ("clients") exchange data with an X server (another application). The X server receives and interprets instructions from the clients for displaying the clients' windows, and it collects and transmits keyboard and mouse input events to the clients. Thexorgpackages available with Cygwin (collectively, "Cygwin/X") provide a high-quality X server, a large set of standard X clients, and a complete set of development tools that can be used to compile X clients that run under MS-Windows.**

**X Windows 系统，版本11（常称为“X11”，或简称为“X”）是Unix 和GNU/Linux下标准的图形环境；它也可用于其它平台，包括Mac OS X 和Windows。X 应用程序（“客户端”）与X服务器（另一个应用程序）交换数据。X服务器接收和解释来自客户端的用于显示客户端窗口的指令，它收集并发送键盘和鼠标输入事件到客户端。Cygwin 的xorg软件包（统称为“Cygwin/X”)提供高质量的X服务器，大批标准的X客户端，完整的能用来编译运行在MS-Windows上X客户端的开发工具。**



***Important: X clients**cannot**run unless they can connect to a running X server. The X server must be started first.*When you use Cygwin/X, its X server (Xwin) runs on your PC, under MS-Windows. Once Xwin is running, you can launch and interact with X clients on your PC or on any other networked computer (which can be running MS-Windows, GNU/Linux, Mac OS X, Unix, or any other OS that can run X client software).**

**重要事项：X客户端不能运行除非它们能连接到一个正在运行的X服务器。X服务器必须先被启动。当你使用Cygwin/X时， 它的X服务器（Xwin）在你的PC上MS-Windows环境下运行。一旦Xwin正在运行，你能启动和交互与你PC上的或任何其它联网计算机（能够运行MS-Windows，GNU/Linux, Mac OS X, Unix，或任意其它能运行X客户端软件的操作系统）上的X客户端。**



**Since Xwin is a complete window system, it has a "root" (background) window on which X clients' windows are displayed. Xwin can run in "rootless" mode, in which its root window is invisible, allowing the MS-Windows desktop and application windows to be visible, and allowing the X clients' windows to be moved, resized, closed, and reopened using the same methods as for native MS-Windows applications. Usestartxwinto run Xwin in rootless mode.**

**由于Xwin是完整的窗口系统，它有一个“root”（背景）窗口，X客户端的窗口在它上面显示。Xwin 能以“rootless”（无背景）模式运行，该模式下背景窗口不可见，MS-Windows 桌面和应用程序窗口能被看见，允许X客户端窗口可以被移动，调整大小，关闭，使用 和操作本机MS-Windows应用程序  相同的方法重新打开客户端窗口。使用startxwin 运行无背景模式的Xwin。**



**On Cygwin 1.7,startwinis/usr/bin/startwin.exe, and it must be started from a Cygwin window (not a DOS box or a desktop shortcut). On Cygwin 1.5,startwinisc:\cygwin\usr\X11R6\bin\startxwin.bat, and if you create a desktop shortcut to this batch file, you can click on it to launch Xwin.**

**在Cygwin 1.7上，startwin 对应/usr/bin/startwin.exe，它必须从Cygwin窗口启动（不是DOS窗口或桌面快捷方式）。**

**在Cygwin 1.5上，startwin 对应c:\cygwin\usr\X11R6\bin\startxwin.bat，若果你创建了这个批处理文件的桌面快捷方式，你可以点击启动Xwin。**



**If you prefer, you can run the commandstartxin a Cygwin window to launch Xwin; using this method, Xwin opens a root window on the desktop, and all X client windows are contained within Xwin's root window.**

**When you launch Xwin, it starts anxterm(terminal emulator) client:**

**如果你喜欢，你可以在Cygwin窗口中运行startx 命令来启动Xwin；使用这种方法，Xwin在桌面上打开一个背景窗口，所有X客户端窗口包含于Xwin 背景窗口。**

**当你启动Xwin，它启动了xterm（终端仿真器）客户端：**


**Within this window, just as in a Cygwin terminal window, you are running the GNUbashshell. An important difference is that you can launch other X clients from anxtermwithout additional preparation.**

**在这个窗口中，和在Cygwin终端窗口中一样，你正在运行GNU bash shell。一个重要的差别是你可以从xterm启动其它的X客户端不需要额外的准备。**



**You can accessxterm's menus by pressing and holding the**Ctrl**key while clicking the left, middle, or right mouse buttons. Use these menus to enable the scrollbar (as shown in the screen shot above) or to change the character size, among other possibilities.**

** 你能使用xterm的菜单，通过按住ctrl 键的同时单击鼠标左键，中键，右键。使用这些菜单能够使用滚动条（如上面的屏幕快照所示）或改变字符大小等。**



**While the X server is running, its icon ("X") appears in the Windows system tray (see above). You can click on this icon to force the X server to exit, but doing so will also force any X clients to exit; normally you should exit from all X clients before stopping the server.**

**在X服务器正在运行时，它的图标（“X”）出现在窗口系统托盘中（参看上面）。你可以点击这个图标强行退出X服务器，但是这么做也将强行退出所有的X客户端；正常来讲你应该在停止服务器之前 退出全部的X 客户端。**](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




