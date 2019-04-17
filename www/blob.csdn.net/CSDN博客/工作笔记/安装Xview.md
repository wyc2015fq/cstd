# 安装Xview - 工作笔记 - CSDN博客





2012年01月17日 14:12:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5964








## MS-Windows 95/98/ME/NT/2000/XP

The file [xview-3.2p1.4-18c-cygwin.tar.bz2](http://www.physionet.org/physiotools/xview/cygwin/xview-3.2p1.4-18c-cygwin.tar.bz2) (in the cygwin directory) contains a set of XView binaries, header files, and standard XView clients for use under any modern version of MS-Windows (95/98/NT/2000/ME/XP).


文件[xview-3.2p1.4-18c-cygwin.tar.bz2](http://www.physionet.org/physiotools/xview/cygwin/xview-3.2p1.4-18c-cygwin.tar.bz2)（cygwin目录中）包含一系列XView库二进制文件，XView客户端，供在MS-Windows（95/98/NT/2000/ME/XP）下使用




It was compiled using the free [Cygwin development environment](http://www.physionet.org/physiotools/cygwin/), which includes an X11 server that must be running in order to interact with XView applications.


它是用Cygwin开发环境编译成的，Cygwin包含X11服务器，X11服务器用来与XView应用程序交互 



To install this package, first [install Cygwin](http://www.physionet.org/physiotools/cygwin/), including the optional sunrpc,
libX11-devel and X-start-menu-icons packages; then open a Cygwin terminal window, copy the tarball into your default (home) directory, and run these commands:


要安装这个软件波，首先安装Cygwin，选择安装sunrpc，libX11-devel ，X-start-menu-icons软件包；然后打开Cygwin终端窗口，拷贝压缩包到你的默认（home）目录（------默认目录是在home目录下，用Windows登陆名建立的Cygwin用户所对应的子目录），运行这些命令： 
    cd /                   ------切换到根目录，对应Windows下C:\cygwin    tar xfvj ~/xview-3.2p1.4-18c-cygwin.tar.bz2  ----解压默认目录下的压缩包 【tar命令选项： (tar --help)-x  ：　解压缩（ one ofrequired options, -f,-v,-j are optional)-f :   档案文件或设备-v :   显示处理的文件名。-j :   利用bzip2过滤档案文件】




These commands install the XView package into subdirectories of /usr/openwin. In order to link and use the libraries or to use the standard clients, you must add
/usr/openwin/bin to your PATH. You can do this automatically by adding the lines


这些命令安装XView 包 到/usr/openwin子目录。为连接和使用库或使用标准客户端，必须添加/usr/openwin/bin 到你的PATH。你可以自动完成此事，通过添加下面两行
    export PATH=/usr/openwin/bin:$PATH
    export DISPLAY=:0.0

to the text files named .bashrc and
.bash_profile (note the initial '.' in the names of these files) that should be located in your home directory. (Edit these files using any text editor, such as Windows Notepad, or create them if they don't exist;
 be sure to save them as plain text and without any suffix attached to the file names.)

到home目录下名为.bashrc 和 
.bash_profile 的文本文件（注意到这些文件名的开头带有‘.’）。（编辑这些文件，利用文本编辑器，例如Windows记事本，如果它们不存在就创建它们；确保将它们作为普通的文本进行保存并且文件名不带有任何后缀。---注意换行符Unix/Linux 是\n, Windows是\r\n可能给bash 带来问题）



You must also start the X server before attempting to run any X clients. 

你必须在运行X客户端前启动X服务器。 



One way to do this is via /usr/X11R6/bin/startxwin.bat.


一种方法是 通过 /usr/X11R6/bin/startxwin.bat. （ ----在/usr/X11R6/bin中没发现有 startxwin.bat）




The version of this script that comes with Cygwin's X-start-menu-icons package runs the X server with backing store disabled, which causes XView applications to open with blank (solid white) windows. To avoid
 this problem, open startxwin.bat in any text editor (Windows Notepad will work), and find the line that reads


这个脚本 会伴随Cygwin 安装 X-start-menu-icons包而出现，它运行X服务器时禁用了后备存储器，致使XView 应用程序打开时带有空白（纯白）窗口。为避免该问题，用文本编辑器打开startxwin.bat（如记事本），找到下面的一行



    run XWin -multiwindow -clipboard -silent-dup-error


Add the option "+bs" to the end of this line, so that it reads


在这行的末尾加入“+bs”选项，变成
    run XWin -multiwindow -clipboard -silent-dup-error +bs

If you have a two-button mouse, you will be able to simulate a middle button click by "chording" (clicking both buttons at approximately the same time) by using this form of the XWin command instead:


如果你用的是双按键的鼠标，你可以通过“连击”（几乎同时点击两个按键）模拟鼠标中键，通过换用下面形式的XWin命令： 
    run XWin -multiwindow -clipboard -silent-dup-error +bs -emulate3buttons

Save startwin.bat. If you make a desktop shortcut to this file, you can click on it to launch the X server and an
xterm window. 

You can then start XView clients either from the xterm window or (if you have set DISPLAY as shown above) from a Cygwin terminal window.


保存startwin.bat.(---是startxwin.bat)。如果你建立了这个文件的桌面快捷方式，你可以点击启动X服务器和xterm窗口。然后你可以从 xterm 窗口 或从Cygwin 终端窗口 启动XView客户端（如果你已经如上所示 设置了DISPLAY环境变量）。



