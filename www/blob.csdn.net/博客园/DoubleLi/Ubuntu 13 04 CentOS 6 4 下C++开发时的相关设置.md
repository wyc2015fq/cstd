# Ubuntu 13.04/CentOS 6.4 下C++开发时的相关设置 - DoubleLi - 博客园






[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 13.04/[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 6.4 下C++开发时的相关设置

一、基本设置

首先，为了可以使我们的c++ 可以找到 iostream类，std标准库，我们需要在C/C++ General->Paths and Symbols 中添加include dictionarys.

![](http://www.linuxidc.com/upload/2014_01/140124110413541.png)

当然，从c/c++ Build ->settings->gcc c++ complier中也可以添加。这样，我们便可以解决include包含问题。

其次,On a project with two target architectures each with its own build configuration (the main target is an embedded AVR platform; the second target is my local Linux PC for running unit tests) I found it necessary to setPreferences -> C/C++ -> Indexer -> Use active build configuration as well as to add/usr/include/c++/4.7,/usr/includeand/usr/include/c++/4.7/x86_64-linux-gnu toProject Properties -> C/C++ General -> Paths and Symbols and then to rebuild the index.

另外，makefile也要让eclipse自动生成。

二、关于静态链接库的问题。

首先，我们自己先做好了一个静态链接库，然后我们新建一个文件夹mylib,把生成的*.a放进去，再在这个文件夹下新建一个include文件夹，把头文件放进去。

库文档在连接（静态库和共享库）和运行（仅限于使用共享库的程式）时被使用，其搜索路径是在系统中进行配置的。一般 Linux

系统把 /lib 和 /usr/lib

两个目录作为默认的库搜索路径，所以使用这两个目录中的库时无需进行配置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到

库的搜索路径之中。配置库文档的搜索路径有下列两种方式，可任选其一使用：

在环境变量 LD_LIBRARY_PATH中指明库的搜索路径。

在 /etc/ld.so.conf 文档中添加库的搜索路径。    将自己可能存放库文档的路径都加入到/etc/ld.so.conf中是明智的选择 ^_^

添加方法也极其简单，将库文档的绝对路径直接写进去就OK了，一行一个。例如：

/usr/X11R6/lib

/usr/local/lib

/opt/lib

最后，要在终端下执行/sbin/ldconfig,使配置生效。

需要注意的是：第二种搜索路径的配置方式对于程式连接时的库（包括共享库和静态库）的定位已足够了，但是对于使用了共享库的程式的执行还是不够的。这是因为为了加快程式执行时对共享库的定位速度，避免使用搜索路径查找共享库的低效率，所以是直接读取库列表文档/etc/ld.so.cache从中进行搜索的。/etc/ld.so.cache 是个非文本的数据文档，不能直接编辑，他是根据/etc/ld.so.conf中配置的搜索路径由 /sbin/ldconfig命令将这些搜索路径下的共享库文档集中在一起而生成的（ldconfig 命令要以root权限执行）。因此，为了确保程式执行时对库的定位，在 /etc/ld.so.conf中进行了库搜索路径的配置之后，还必须要运行/sbin/ldconfig 命令更新 /etc/ld.so.cache文档之后才能够。ldconfig,简单的说，他的作用就是将/etc/ld.so.conf列出的路径下的库文档缓存到/etc/ld.so.cache以供使用。因此当安装完一些库文档，(例如刚安装好glib)，或修改ld.so.conf增加新的库路径后，需要运行一下/sbin/ldconfig使任何的库文档都被缓存到ld.so.cache中，假如没做，即使库文档明明就在/usr/lib下的，也是不会被使用的，结果编译过程中抱错，缺少xxx库，去查看发现明明就在那放着，搞的想大骂computer蠢猪一个。^_^

在程式连接时，对于库文档（静态库和共享库）的搜索路径，除了上面的配置方式之外，还能够通过 -L 参数显式指定。因为用 -L配置的路径将被优先搜索，所以在连接的时候通常都会以这种方式直接指定要连接的库的路径。

前面已说明过了，库搜索路径的配置有两种方式：在环境变量LD_LIBRARY_PATH 中配置连同在/etc/ld.so.conf 文档中配置。其中，第二种配置方式需要 root权限，以改变 /etc/ld.so.conf 文档并执行/sbin/ldconfig 命令。而且，当系统重新启动后，任何的基于GTK2 的程式在运行时都将使用新安装的 GTK+ 库。不幸的是，由于GTK+版本的改变，这有时会给应用程式带来兼容性的问题，造成某些程式运行不正常。为了避免出现上面的这些情况，在GTK+及其依赖库的安装过程中对于库的搜索路径的配置将采用第一种方式进行。这种配置方式无需 root 权限，配置也简单：

$ exportLD_LIBRARY_PATH=/opt/gtk/lib:$LD_LIBRARY_PATH

能够用下面的命令查看 LD_LIBRAY_PATH的配置内容：

$ echo $LD_LIBRARY_PATH

至此，库的两种配置就完成了。

当敲完代码后，直接按project > run，系统提示错误：" launch failed. Binary file not found.” 后来查完资料后才知道原来在linux下必须build 完后才可以运行，所以每次要运行project时，必須先按ctrl + b，build时会提示有无错误。build 成功后程序才可以运行。

binary not found 其实主要是链接的问题，这里的方法比较简单。

projrct-》properties-》c/c++ build --》Environment--》PATH  选replace native environment with specified one

新建项目后，点击run>>run configurations,双击左侧一列的c/c++ local applation，新建一个运行配置，在main中点击Browse选择你的项目，在common中勾选“debug”和“run”，这样就好使了。

I even faced this error and searched a lot to fix. Fortunately, I figured it out myself.

The following are the things you need to make sure they are set:

Windows > Preferences > C/C++ > New CDT project wizard > Makefile Project > Binary Parsers

a) Set it to Windows PE Parser if you are on windows.

b) Set it to Cygwin PE Parser if you are using Cygwin

Windows > Preferences > C/C++ > Environment

Now click on "select" and select "Path" variable so that you add you path variables in Eclipse.

Now its all done!

All you have to do is write a test program and press Ctrl+B (Build all; there is a button on menu bar) and after that click "Run".

After all the setup is done.Go to eclipse project --> build all or ctrl + B then eclipse automatically build up the binaries and other stuffs now run your project

Linux平台下用Eclipse编写C++程序出错！

所用环境是[Fedora](http://www.linuxidc.com/topicnews.aspx?tid=5)16 + GCC + Eclipse CDT，可是我随便写个简单的程序，编译，总是提示：“Launch failed,binary not found”。请问如何解决？

1.已试过在“项目”——“属性”——“C++ Build”——“Settings”——“Binary Parses”里面勾选“GNU Elf Parses”和“PE Windows Parses”，仍然无用。

2.已试过在“窗口”——“首选项”——“C/C++”——“New CDT Project Wizard”——“Makefile Project”——“Binary Parses”里面勾选“GNU Elf Parses”和“PE Windows Parses”，还是无用。

3.已试过先按Ctrl+B编译后再运行，无用。

4.已试过文件名加上后缀.cpp和.c，无用。

Project->Properties->C/C++Build->Settings->Binary Parsers

勾选GNU Elf Parser和Elf Parser

然后千万千万别忘了Project->Build All (Ctrl+B), 再run（Ctrl+F11）

如果看到项目下面出来一项名为Binaries, Congradulations! 配置成功，可以run了！

3、还不行？

检查你的文件名是否为xxx.cpp? 而且xxx中不能含有“.”之类的符号

新建了一个项目后，Ctrl+F11可以运行，但只要 ，然后再Ctrl+F11运行，出现"launch failed.Binary not found"错误，解决方法如下：

新建项目后，点击run>>run configurations,双击左侧一列的c/c++ local applation，新建一个运行配置，在main中点击Browse选择你的项目，在common中勾选“debug”和“run”，这样就好使了。









