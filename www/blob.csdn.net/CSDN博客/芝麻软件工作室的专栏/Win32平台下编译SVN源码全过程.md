
# Win32平台下编译SVN源码全过程 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月03日 08:54:20[seven-soft](https://me.csdn.net/softn)阅读数：864


前段时间曾经总结过一些[在win32平台下基于SVN开发的一些注意事项](http://www.windameister.org/blog/index.php/2009/05/15/win32-svn-dev-some-notes/)，主要是在利用svn官方发布的二进制库进行开发过程中使用的方法和一些值得注意的问题。
由于svn官方发布的win32平台下的二进制文件是基于vc6编译的，在使用vc2005进行开发时，会遇到因CRT冲突而引起的link错误。因此，如果是使用vc2005（我推测使用VC2003也会遇到同样的问题，尚未验证）附带的CRT库与svn官方发布的binary进行link，那么无论如何都会出现crash的问题。最为彻底的解决方案，还是自行编译svn源码。
在win32下编译svn源码说明：
首先需要从官方下载一份SVN源码，版本可以根据需要选取，比如最新的Release 1.6.3([目前已经更新到1.6.6](http://subversion.tigris.org/))可以在这个地址下载到：
[http://subversion.tigris.org/downloads/subversion-1.6.3.zip](http://subversion.tigris.org/downloads/subversion-1.6.3.zip)
以及svn所需的依赖包：
[http://subversion.tigris.org/downloads/subversion-deps-1.6.3.zip](http://subversion.tigris.org/downloads/subversion-deps-1.6.3.zip)
如果需要支持ssl的话，还需要下载openssl（根据实际需要选择相应版本）：
[http://www.openssl.org/source/](http://www.openssl.org/source/)
如果需要BerkeleyDB的话，需要下载WindowsBDB(BDB是可选的，如果不使用BDB，则默认使用FSFS)：
[http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=688&expandFolder=688&folderID=2627](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=688&expandFolder=688&folderID=2627)
此外还有Windows libintl：
[http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=2627&expandFolder=2627&folderID=8100](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=2627&expandFolder=2627&folderID=8100)

解压svn源码包，可以在subversion-1.6.3目录中找到这么一个官方发布的说明文件INSTALL。该文件详述了安装通过源码编译SVN所需依赖的工具及第三方库，并且给出了详细的步骤。
网上同样有一个INSTALL的说明，可以在这里访问到:
[http://svn.collab.net/repos/svn/trunk/INSTALL](http://svn.collab.net/repos/svn/trunk/INSTALL)
编译开源项目的话，其附带的INSTALL说明都是最重要也是最全面的参考。网上搜索的其他资料，也会有相应的参考价值，但无论如何，其信息的来源也是INSTALL。因此编译开源项目时，认真阅读INSTALL是最重要且效率最高的。
从Google上进行一下简单的搜索的话，可以找到一篇介绍svn源码编译的文章，在这里：[http://rocksun.cn/?p=103](http://rocksun.cn/?p=103)
为了后续的步骤方便，我们需要先准备编译所必须的一些东西：
包括Perl：[http://www.activestate.com/activeperl/](http://www.activestate.com/activeperl/)
Python：[http://www.python.org/download/](http://www.python.org/download/)

安装Python以及Perl，比如分别安装到D:/Python26以及D:/Perl。安装好之后，将python以及perl的bin目录设为系统目录并重新启动使之生效。
编译svn源码第一步，将下载的svn源码包解压到X:/SVN/svn—trunk下。X可以是任意一个盘符。在我的机器上，我使用了F盘，下文中皆以F盘举例。
在F:/SVN/svn-trunk中解压了刚才下载的svn源码包以及依赖之后，可以在目录
F:/SVN/src-trunk/subversion-1.6.3中看到以下文件及目录：
![](http://www.windameister.org/attachments/svn_compile_log/svn.compile.log.1.jpg)
以及文件：
![](http://www.windameister.org/attachments/svn_compile_log/svn.compile.log.2.jpg)
Svn的编译需要依赖libapr以及libapr-util，SQLite，zlib，libintl可选，libneon/libserf二则择一，openssl可选，BerkeleyDB可选，libsasl可选，对Python,Perl,Java,Ruby支持的模块可选，以及KDELibs,GNOME Keyring可选。
我们依次先编译依赖项：首先进入到subversion-1.6.3/apr目录中。可以看到存在apr.dsw以及apr.dsp文件，这是VC6的工程文件。我们如果想在2005下编译的话，需要将其转换成sln及vcproj文件，简单的用vc2005打开该文件并保存即可。该目录下还有Makefile.win文件，是win下的makefile，我们打开makefile.win文件查看一下说明：可以得知如果需要编译.sln文件的话，需要置USESLN=1。
在VC2005的命令行中输入nmake -f makefile.win buildall checkall USESLN=1便可以开始编译apr了。Checkall表示编译完成后会去运行所有的测试用例。
编译完成后，当前目录下会多出2个文件夹，分别是LibR – StaticRelease，Release – DllRelease。如果选择Debug编译，则会生成LibD – StaticDebug， Debug – DllDebug。
类似的，我们将apr-util以及apr-iconv也编译好。
编译zlib：
进入zlib目录后，使用以下命令编译zlib库
nmake -f win32/Makefile.msc
编译openssl：
将此前下载的openssl解压到F:/SVN/openssl
阅读其INSTALL文档（INSTALL.W32）
使用VC编译openssl首先需要运行configure：
perl Configure VC-WIN32
接着运行
ms/do_masm
这里的do_masm是一个bat脚本，该脚本会生成nt.mak以及ntdll.mak分别是Release版本的静态和动态的库的make文件。如果想生成debug版本的make文件，可以通过修改do_masm.bat中的调用mk1mf.pl脚本处的参数实现，具体参数可以参考mk1mf.pl文件自身的说明。
接下来，创建动态链接库版本的ssl库用nmake -f ms/ntdll.mak，以及静态版本使用：nmake -f ms/nt.mak
生成的结果文件位于out32dll文件夹，以及out32文件夹中。
编译neon
进入F:/SVN/src-trunk/subversion-1.6.3/neon目录
nmake –f neon.mak
默认生成的是release版的libneon.lib (debug版为libneonD.lib)
可以用nmake –f neon.mak DEBUG_BUILD=1生成debug版的lib。
回到F:/SVN/src-trunk/subversion-1.6.3
运行python gen-make.py –help可以了解如何使用gen-make.py生成我们所需的svn编译文件。
由于在此，我打算选用neon, libintl, openssl(本例中并不打算使用BDB，如果需要BDB则需要增加—with-berkeley-db=DIR参数)进行编译，目前需要关注的几个重要参数如下：
–with-apr=DIR
–with-apr-util=DIR
–with-apr-iconv=DIR
–with-neon=DIR
–with-libintl=DIR
–with-openssl=DIR
–with-zlib=DIR
–vsnet-version=VER
运行
F:/SVN/src-trunk/subversion-1.6.3>python gen-make.py -t vcproj
–with-apr=apr –with-apr-iconv=apr-iconv –with-apr-util=apr-util –with-libintl=svn-win32-libintl –with-openssl=../../openssl –with-zlib=zlib –vsnet-version=2005
(其中的libintl需要解压到当前文件夹中)
即可生成vc2005的sln(subversion_vcnet.sln)文件了。
打开vc2005，选择Debug编译选项，对项目ALL进行编译。如果一切顺利，则会生成一个F:/SVN/src-trunk/subversion-1.6.3/Debug目录。内容包括svn的所有的lib及可执行文件。
![](http://www.windameister.org/attachments/svn_compile_log/svn.compile.log.3.jpg)
将svn/svn.exe以及所有目录下的.dll文件拷贝到一个新建的bin目录下。将openssl的dll，apr, apr-util, apr-iconv的dll拷贝到同样的bin目录下，如下图：
![](http://www.windameister.org/attachments/svn_compile_log/svn.compile.log.4.jpg)
运行svn –version看看结果吧～
最后，当我们有了自行编译的svn可以做什么？你可以做任何你想做的事——比如自己基于svn的接口进行开发（可以参考开源项目rapidsvn以及TortoiseSVN的源码）

