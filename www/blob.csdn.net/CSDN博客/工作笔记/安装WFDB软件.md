# 安装WFDB软件 - 工作笔记 - CSDN博客





2012年01月17日 14:13:17[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8160








# WFDB quick start for MS-Windows 95/98/NT/2000/ME/XP/Vista/7

**                                                                      MS-Windows用户WFDB快速入门**

**Under Windows Vista or Windows 7, it may be necessary to perform some or all of the steps below while logged into an account with "administrator" permissions.**

Windows Vista 或 Windows 7环境下，可能需要执行下面部分或全部步骤，同时要以管理员的权限登录。 


- **[Install Cygwin](http://www.physionet.org/physiotools/cygwin/)***in a location that does not have any spaces in its pathname*. The default and preferred location is
c:\cygwin. Do not use locations under
Program Files, Documents and Settings, or any other directory containing a space in its name. Be sure to leave the "Default Text File Type" as "Unix" (not "DOS"), and
 be sure to select and install the curl-devel,
diffutils, gcc,
gcc-mingw, make,
openssl-devel, sunrpc,
libX11-devel, and X-start-menu-icons packages (these are not installed by default in a minimal Cygwin installation).


安装Cygwin到一个路径中不含空格的地方。默认的首选位置是c:\cygwin。不要用Program Files,
Documents and Settings，或其它名字中含空格的目录。"Default Text File Type" 选项一定选中 "Unix" (不选 "DOS"),一定要选择安装
curl-devel, diffutils,
gcc, gcc-mingw,
make, openssl-devel,
sunrpc, libX11-devel,
X-start-menu-icons软件包（Cygwin默认最小安装中不含这些软件包）。

- **Install an HTTP client library** (optional). If you installed curl-devel together with Cygwin as specified above, you may go on to step 3. Otherwise, install
[**libcurl**](http://www.physionet.org/physiotools/libcurl/) or
[**libwww**](http://www.physionet.org/physiotools/libwww/cygwin/) now.






         安装 HTTP 客户端库（可选）。 如果你安装Cygwin时安装了上面指定的curl-devel，你可以跳至第三步，否则，现在安装libcurl 或libwww。
- You will be able to use the WFDB software to read files on local disk drives and CD-ROMs whether or not you install one of these libraries.


            不论你是否安装这些库，你可以使用WFDB软件读取本地磁盘，光盘上的文件。
- Direct access from WFDB applications to data files on remote web and FTP servers (NETFILES) is possible
*only* if you install libcurl or libwww. 

          只要安装了libcurl 或libwww，通过WFDB应用程序直接访问远程网络和FTP服务器上的数据文件(NETFILES 功能)。
- Direct access from WFDB applications to password-protected data files on remote web and FTP servers requires libcurl 7.12.0 or later.


         通过WFDB应用程序直接访问远程网络和FTP服务器上受密码保护的数据文件，需要安装libcurl7.12.0 或更高的版本。
- If you skip this step now, you may install libcurl or libwww and recompile the WFDB library later to enable NETFILES access for all of your WFDB applications. It will not be necessary to recompile the applications themselves.


         如果你现在跳过这一步，你以后可以安装libcurl 或libwww 并重新编译WFDB库，为你的WFDB应用程序启动NETFILES访问功能。




     3.   [**Install XView**](http://www.physionet.org/physiotools/xview/#windows) (optional).


           安装XView（可选）
- You will not be able to compile or use WAVE unless you have installed libX11-devel (in step 1 above) and XView, but none of the other WFDB applications require these packages. Other components of PhysioToolkit,
 such as [plt](http://www.physionet.org/physiotools/plt) and [SEMIA](http://www.physionet.org/physiobank/database/ltstdb/semia/), require X11 and XView.

          你不能编译和使用WAVE软件除非你已经安装了libX11-devel （上面第一步中）和 XView ,但是其他的WFDB 应用程序不需要这些软件包。PhysioToolkit 的其它组件，例如plt 和SEMIA，需要X11 和XView。
- Add /usr/openwin/bin (or the directory containing
textedit, if you have a non-standard installation of XView) to your
PATH*before attempting to compile WAVE*. (See the instructions
[here](http://www.physionet.org/physiotools/xview/#windows).) 

          在编译WAVE前，添加 /usr/openwin/bin ( 或包含textedit 的目录，如果你进行的是非标准的XView安装) 到你的PATH环境变量。（参见这里的说明。）




**         4. Download the current version of the WFDB software package** as
[sources](http://www.physionet.org/physiotools/wfdb.tar.gz) or [binaries](http://www.physionet.org/physiotools/binaries/). Binary packages are provided as a convenience and may not be up-to-date; we strongly recommend following the procedure described below for compiling the software from the sources instead. If you plan to compile WFDB applications that are not included
 in the binary package, please start with the sources, and read the [note](http://www.physionet.org/physiotools/wfdb-windows-quick-start.shtml#compiling) below.  

*If you download the sources (recommended):*

          以源码或二进制软件包形式下载当前版本的WFDB软件。二进制软件包使用方便但可能不是最新的版本；我们强烈建议按照下面描述的步骤从源代码编译这个软件。如果你打算编译 没有包含在二进制软件包中的WFDB应用程序，请从源代码开始，并阅读下面的注释。



- Choose a working directory for the installation. This directory must not have any spaces in its pathname. If your Windows username has no spaces, you can use your Cygwin home directory (c:\cygwin\home\*username*);
 otherwise, create a directory beneath c:\cygwin (for example,
c:\cygwin\home\wfdb) and use that as your working directory. Save the archive of sources (wfdb.tar.gz) in this working directory.





         为安装选择一个工作目录。这个目录的路径名中必须不含空格。如果你的Windows用户名不含空格，你可以使用你的Cygwin home目录（c:\cygwin\home\username); 否则，在c:\cygwin下创建一个目录用它作为你的工作目录（举个例子，c:\cygwin\home\wfdb）。 保存源代码文件（wfdb.tar.gz)到这个工作目录。 


- Open a Cygwin terminal emulator window. To open a Cygwin window, either click on the Cygwin desktop icon, or find and run
c:\cygwin\cygwin.bat. **Under Vista or Windows 7, right-click on the Cygwin icon and choose "Run as administrator"** (thanks to Albert Titus for this tip). By default, the current directory within the
 Cygwin window will be c:\cygwin\home\*username* initially. If you chose a different working directory, enter that directory by typing a command such as


         打开Cygwin终端模拟窗口。通过点击Cygwin桌面图标或运行c:\cygwin\cygwin.bat来打开Cygwin窗口。在Vista 或Windows 7系统，右键点击Cygwin图标，选择 “Run as administrator”(感谢Albert Titus 对此的提示)。默认的，Cygwin初始当前目录是c:\cygwin\home\username 。如果你选择其他不同的工作目录，输入类似命令切换到那个目录
- cd /home/wfdb

- (Omit c:\cygwin\ from this cd command, and use a forward slash (/) rather than a backslash (\) as a directory separator where needed.) Perform the remainder
 of the installation by typing the commands as shown below into this Cygwin window. Do not use an MS-DOS window for this purpose.


       （从cd命令中删除c:\cygwin\, 在需要的地方用正斜杠而不是反斜杠作为目录分隔符。）在Cygwin窗口中输入下面所示的命令完成安装。


- Unpack the archive of sources (using tar, included with Cygwin):
解压源代码文件（用Cygwin 中的tar命令） 
- tar xfvz wfdb.tar.gz

- If your browser decompressed the archive during the download, you will need to use this command instead:


           如果你的浏览器在下载的过程中解压了该文档文件，你需要换用这个命令： 
     tar xfv wfdb.tar- 
This creates a directory with a name of the form wfdb-10.*m.n* within your working directory. Look at the list of files printed by the
tar command to determine the name of the directory for the next step.


          这会在你的工作目录中创建一个名字形如wfdb-10.m.n的目录。查看tar 命令打印的文件列表来确定下一步要用的目录名。 
- Enter this directory and configure the package: 

          进入这个目录并完成配置： 
- cd wfdb-10.*m.n*
./configure

(In the cd command, replace wfdb-10.*m.n* with the name of the directory as printed by
tar in the previous step.) 

          （在cd命令中，用前一步tar打印输出的目录名替换wfdb-10.m.n）  
- *(Optional)* Compile and test the package without disturbing any previous WFDB installation:
make        ----（可选，编译并测试软件包，不会影响以前安装的WFDB）
- 
Skip this step on an initial installation. The checks performed by 'make check' (described below) are run on the test binaries if you do perform this step.


          初始安装跳过这一步。如果你一定要执行这一步，‘make check’验证命令（下面做了描述）会完成  对二进制编译结果的测试 
- Compile and install the package (as noted above, installation may require "administrator" permissions):


          编译并安装（如上面提到的，安装可能需要管理员权限） 
- make install

- The binaries will be installed in /usr/bin (accessible from outside the Cygwin environment as
c:\cygwin\usr\bin). Be sure that this
bin directory is in your PATH. 

          二进制代码将被安装到目录/usr/bin(从Cygwin 环境之外通过目录c:\cygwin\usr\bin可以访问到)。确保PATH中包含这个bin目录。


- *(Optional)* Check that the WFDB library and applications have been correctly compiled and installed:

      （可选） 验证 WFDB库和应用程序已经正确编译和安装： 
      make check

         This step compiles a short program that exercises the WFDB library, prints a summary of test results, and prompts you to press <Enter>. After you have done so, the WFDB applications are tested. The tests are very short (typically less than a second
 each), except that the last one (xform using NETFILES) may take up to a minute if you have a slow or inoperative Internet connection. If any application test fails, its output can be found in the
checkpkg subdirectory of the WFDB source tree; compare this output with the files of the same names that can be found in the
checkpkg/expected subdirectory. 

              这一步编译一个短的程序来试用WFDB库，打印测试结果，提示你输入回车。然后WFDB应用程序被测试。测试用时非常短（通常每一个少于一秒），除了最后一个（xform 使用NETFILES）可能会耗时达到一分钟，如果你的网络连接慢或是无法连接。如果任何一个应用程序测试失败，WFDB源代码目录树的checkpkg子目录中可以找到输出结果；将这次测试输出和checkpkg/expected 子目录中同名的文件进行比较。
- Make the WFDB library available for other applications: 
- 使其它应用程序也可以使用WFDB库 
- 
- ln -sf /usr/bin/libwfdb.dll.a /usr/lib



*If you download the binaries (not recommended):*
- Copy cygwfdb*dll and all of the
*.exe files from the [directory of MS-Windows binaries](http://www.physionet.org/physiotools/binaries/windows/wfdb/usr/bin/) into a directory in your PATH. If you have not installed Cygwin (especially not recommended), also copy the other
cyg*.dll files from the directory of MS-Windows binaries into the same directory as the
*.exe files. Note: you *must* install Cygwin and Xview in order to use WAVE.


如果你下载了二进制代码（不推荐）

        从MS-Windows 二进制文件目录中 拷贝cygwfdb*dll 和 所有的*.exe 文件 到 PATH 中的一个目录。如果你还没有安装Cygwin（特别不推荐），也像*.exe文件那样，从MS-Windows 二进制文件目录拷贝其它的cyg*.dll 文件到相同的目录。要使用WAVE，你必须安装Cygwin和XView 。




**          5   If XView is installed, test WAVE** by [starting the Cygwin X server](http://www.physionet.org/physiotools/cygwin/#using-x) and then running (in an xterm window, such as the one that opens when you start the X server) the command:

              如果XView 已经安装完成，测试WAVE程序，启动 Cygwin X 服务器然后运行命令（在xterm窗口，例如你启动X服务器时打开的那个）：
wave -r mitdb/200 -a atr
Note that WAVE's menus (marked with a ![nabla](http://www.physionet.org/physiotools/icons/nabla.png)) are opened using a right click. Annotation editing requires the use of the middle button; if your mouse has only two buttons, you may be able
 to simulate a middle button click by "chording" (press both buttons at the same time, then release them; see
[this note](http://physionet.org/physiotools/xview/#windows) for details if necessary).


If you have not used WAVE before, you may want to follow through the tutorial material in the beginning of the
[WAVE User's Guide](http://www.physionet.org/physiotools/wug/). 

注意到 WAVE 的菜单（带有![](http://www.physionet.org/physiotools/icons/nabla.png)标识）用右键点击打开。注释编辑需要使用鼠标中键；如果你的鼠标仅有两个按键，你可以通过“连击”模拟中键（同时按两个按键，然后释放按键，如果需要详见这个注释）。


-----------------------------------运行wave -r mitdb/200 -a atr 命令行时，出现如下错误：-------------------------------

assertion "ret != inval_id" failed: file "/usr/src/ports/xorg/libX11/libX11-1.3.3-1/src/libX11-1.3.3/src/xcb_io.c", line 385, function: _XAllocID Aborted (core dumped)


WAVE 不能使用！！

（1）查找错误原因

（2）从XView 源代码编译

（3）换用另一个环境（Fedora 8 虚拟机下OK）



**             6   Read the manuals**. Really! :-) If you want to write your own software to work with PhysioBank data, begin with the
[WFDB Programmer's Guide](http://www.physionet.org/physiotools/wpg/). To learn about the wide variety of existing software that can be used to study PhysioBank data, read the
[WFDB Applications Guide](http://www.physionet.org/physiotools/wag/) and the
[WAVE User's Guide](http://www.physionet.org/physiotools/wug/). 

                 阅读手册。按你的实际需要！:-) 如果你想要 写自己的软件用到PhysioBank 数据，从阅读WFDB 编程指南开始。想要学习已有的种类繁多的用来研究PhysioBank数据的软件，阅读WFDB应用程序指南和WAVE用户指南。



