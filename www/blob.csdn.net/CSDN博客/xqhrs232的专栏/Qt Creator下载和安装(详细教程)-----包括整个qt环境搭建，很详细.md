# Qt Creator下载和安装(详细教程)-----包括整个qt环境搭建，很详细 - xqhrs232的专栏 - CSDN博客
2017年07月27日 17:20:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6706
原文地址::[http://blog.csdn.net/win_turn/article/details/50465127](http://blog.csdn.net/win_turn/article/details/50465127)
# 简介
Qt是跨平台的图形开发库，目前由Digia全资子公司 Qt Company 独立运营，官方网址： 
[http://www.qt.io/](http://www.qt.io/)
也可以访问Qt项目域名：[http://qt-project.org/](http://qt-project.org/)
Qt本身支持众多[操作系统](http://lib.csdn.net/base/operatingsystem)。从通用操作系统[Linux](http://lib.csdn.net/base/linux)、Windows，到手机系统[Android](http://lib.csdn.net/base/android)、[iOS](http://lib.csdn.net/base/ios)、WinPhone，[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)系统支持QNX、VxWorks，应用非常广泛。 
基于Qt的软件非常多，其中最知名的要数[linux](http://lib.csdn.net/base/linux)桌面系统KDE（涵盖无数以K打头的应用软件）。国内WPS for Linux版本、360 for Linux也是使用Qt库开发的界面。只要有C++基础，Qt是很容易学的，而且开发跨平台的程序也容易。目前主流使用的Qt库有Qt4和Qt5两个大版本，下面介绍其在Windows系统里的下载和安装。
# 一、Qt4开发环境下载安装
首先进入下载页面 [http://www.qt.io/download/](http://www.qt.io/download/)
根据提示点击选项。 
![这里写图片描述](https://img-blog.csdn.net/20160105201442796)
![这里写图片描述](https://img-blog.csdn.net/20160105201456137)
![这里写图片描述](https://img-blog.csdn.net/20160105201506370)
最后点击 “Get started”按钮后，进入开源版页面： 
[http://www.qt.io/download-open-source/](http://www.qt.io/download-open-source/)
![这里写图片描述](https://img-blog.csdn.net/20160105202040923)
上面大按钮Download Now是下载在线安装版，其实只是一个下载工具，并不是真正的Qt开发库。可以自己下载离线安装包，注意页面的“View All Downloads”，点开之后就能看到离线包。目前看到的全是Qt5.4.0版本的，那Qt4的在哪呢？ 
把上面页面拖到最下面： 
![这里写图片描述](https://img-blog.csdn.net/20160105202155611)
旧版本包就是最后一个链接：[http://download.qt.io/archive/](http://download.qt.io/archive/)
![这里写图片描述](https://img-blog.csdn.net/20160105202222313)
打开之后可以看到四个目录：vsaddin是VisualStudio集成插件，qtcreator是Qt官方的集成开发环境，qt就是各种新老版本下载位置，online_installers就是在线安装工具。 
在Qt4的时候，Qt开发环境包括3个基本部分：Qt Framework（Qt库）、QtCreator（IDE）和MinGW（编译调试），都要分别下载安装并配置，比较麻烦。进入Qt5之后，Qt公司将三者打包成一个安装文件，比如Qt5.4.0的离线安装包，这样方便许多。这里先讲旧的Qt 4.8.6的开发环境下载安装配置方式。
## 1、Qt 4.8.6下载
从刚才页面进入目录 
[http://download.qt.io/archive/qt/4.8/4.8.6/](http://download.qt.io/archive/qt/4.8/4.8.6/)
![这里写图片描述](https://img-blog.csdn.net/20160105202312298)头两个是VS2010和VS2008编译而成的Qt库，第三个是MinGW版本Qt库，就是要下载MinGW版本的来使用。dmg结尾的两个是MacOS系统里使用的Qt库，qt-everywhere-opensource-src-4.8.6是Qt源码包，有zip和tar.gz两个压缩格式的，两个内容是一样的，只是zip一般在Windows下比较流行，tar.gz是Linux上流行的压缩格式。md5sums-4.8.6是以上文件的校验和，changes-4.8.6是Qt版本日志文件。 
下载qt-opensource-windows-x86-mingw482-4.8.6-1.exe： 
[http://download.qt.io/archive/qt/4.8/4.8.6/qt-opensource-windows-x86-mingw482-4.8.6-1.exe](http://download.qt.io/archive/qt/4.8/4.8.6/qt-opensource-windows-x86-mingw482-4.8.6-1.exe)
下载完成后不要着急装，还有别的需要下载。
## 2、QtCreator下载
在[http://download.qt.io/archive/qtcreator/](http://download.qt.io/archive/qtcreator/)页面有一些旧版本的qtcreator可以下载，不过有新版还是用新版的好些。 
进入刚才的 [http://www.qt.io/download-open-source/](http://www.qt.io/download-open-source/) ，点开“View All Downloads”，找到 
![这里写图片描述](https://img-blog.csdn.net/20160105202355689)
这里面讲的很清楚，Qt5.4.0安装包已经带有QtCreator，如果是装Qt5.4.0就不需要下载。我们装4.8.6的，这里要下载“Qt Creator 3.3.0 for Windows (71 MB)”， 
点击下载即可。我们仔细看一下下载链接： 
[http://mirrors.hust.edu.cn/qtproject/official_releases/qtcreator/3.3/3.3.0/qt-creator-opensource-windows-x86-3.3.0.exe](http://mirrors.hust.edu.cn/qtproject/official_releases/qtcreator/3.3/3.3.0/qt-creator-opensource-windows-x86-3.3.0.exe)
这个其实用的是国内华中科大的软件镜像，我们可以尝试进入： 
[http://mirrors.hust.edu.cn/qtproject/official_releases/](http://mirrors.hust.edu.cn/qtproject/official_releases/)
这个与Qt官方软件源一样，可以看到有 
![这里写图片描述](https://img-blog.csdn.net/20160105202419607)
下软件一定不要局限在一个文件链接上面，打开下载链接上层目录可以找到一堆好东西的。自己打开各个子目录看看： 
①gdb是针对[android](http://lib.csdn.net/base/android)开发的调试器。 
②jom可以百度一下，[http://qt-project.org/wiki/jom](http://qt-project.org/wiki/jom) ，是nmake克隆版，支持多条独立命令并行构建软件，加快软件构建速度。 
③online_installers是在线安装器。 
④Pyside是用于支持[Python](http://lib.csdn.net/base/python)语言编写Qt程序的。 
⑤qbs是Qt Build Suite (QBS) ，[http://qt-project.org/wiki/qbs](http://qt-project.org/wiki/qbs)，QBS是新创的Qt程序生成工具，因为Qt一般使用qmake来生成项目进行编译，qmake必须根据qt版本变化而变化，不便于管理，qt开发者对qmake又爱又恨，因此创建了不依赖Qt版本的生成工具QBS。 
⑥qt，当然是qt库本身了，从4.8到5.4都有。 
⑦qt-installer-framework，百度一下是什么，[http://blog.csdn.net/cnsword/article/details/8740393](http://blog.csdn.net/cnsword/article/details/8740393)，是专门用于发布开发好的程序用的，生成程序安装器，可以为Linux、Windows、MacOS生成安装器。Qt自己的QtCreator、QtSDK等安装程序就是使用qt-installer-framework打包的。 
⑧qtcreator是集成开发环境，从2.5到3.3版本都有。 
⑨vsaddin，是针对VisualStudio的Qt开发插件。
上面啰嗦这么多，看起来不着边际的东西，我为什么要讲呢？ 
现在许多娃子脑子被教死了、教蠢了，下载文件也不看看链接，看了链接也不管是不是有别的资源可以下载利用。等到用的时候就问：旧版Qt 4去哪里下载？主页怎么只有Qt5.4.0，上哪找Qt5.3.1？ 
这些都是不用脑子的低级问题。能下载到Qt 5.4.0版本，用屁股想一下，Q.5.3.1和Qt 4.8.6能有多远呢？ 
很多资源都是放在一块的，用心去看看下载链接上级目录、搜一搜就出来的。 
请记住：下载文件一定不要局限，学东西一定要有探索和发现的意识！
另外Qt软件镜像除了华中科大有，国内其他院校也是有的，中国科大的源还要快一些：[http://mirrors.ustc.edu.cn/qtproject/](http://mirrors.ustc.edu.cn/qtproject/)
## 3、MinGW 4.8.2
为什么是4.8.2？我们下载的qt-opensource-windows-x86-mingw482-4.8.6-1.exe 里面的mingw482，就是指这个Qt库使用MinGW 里面g++ 4.8.2版本编译生成的，所以需要同样版本的MinGW来编译程序，避免其他新旧版本兼容之类的问题。 
我们尝试运行qt-opensource-windows-x86-mingw482-4.8.6-1.exe的时候（先不要装，就看看），这个安装程序自己就会提示去哪里找这个MinGW版本，而且这个MinGW不是官方原版： 
![这里写图片描述](https://img-blog.csdn.net/20160105202459292)
是一个私人定制版，在sourceforge上面下载： 
[http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download)
下载链接比较长，去sourceforge浏览该目录可以看到： 
[http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/)
![这里写图片描述](https://img-blog.csdn.net/20160105202524468)
那个下载数目最多的就是Qt安装包里面指出的文件链接，当然还有更新的修订版，可以下载***rev3.7z也可以下载***rev4.7z，这里下载新的rev4：i686-4.8.2-release-posix-dwarf-rt_v3-rev4.7z
## 4、安装
有三个文件，顺序是这样的，Qt库需要MinGW，而qtcreator需要配置前面二者，所以先安装MinGW，再安装qt-opensource-windows-x86-mingw482-4.8.6-1，最后装QtCreator。
### （1）MinGW安装
这个是绿色版的，直接解压缩到C盘根目录就行了，放到其他分区也是可以的。只要后面指定了正确位置就行了，这里解压后路径是：C:\mingw32，g++位置是C:\mingw32\bin\g++.exe。
### （2）Qt库安装
运行qt-opensource-windows-x86-mingw482-4.8.6-1.exe，这个包比较大，解压时间长一些，等一阵子会进入安装界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105202603767)
点击Next进入： 
![这里写图片描述](https://img-blog.csdn.net/20160105202657952)
点击Next进入协议同意页面： 
![这里写图片描述](https://img-blog.csdn.net/20160105202716399)
选择accept协议条款，点击Next进入组建选择界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105202732000)
全部选中，然后点击Next，进入目标目录选择： 
![这里写图片描述](https://img-blog.csdn.net/20160105202758044)
如果C盘空间不够就选别的分区，然后点击Next进入开始菜单设置： 
![这里写图片描述](https://img-blog.csdn.net/20160105202832751)
点击Next进入MinGW选择页面： 
![这里写图片描述](https://img-blog.csdn.net/20160105202911000)
设置正确的MinGW文件夹，如果路径设置出错点击Install时会提示 
![这里写图片描述](https://img-blog.csdn.net/20160105202928025)
这时候选择“否”，重新设置正确的MinGW路径，点击Install之后会开始实际的安装。等待安装结束，取消下面两个复选框再点Finish： 
![这里写图片描述](https://img-blog.csdn.net/20160105202938420)
上面第一个是例子示范，第二个是Qt文档。 
在开始菜单里面Qt by Digia v4.8.6 (MinGW 4.8.2 OpenSource)就是Qt库的快捷菜单项： 
①Assistant，帮助文档查看 
②Designer，图形化编辑ui文件，就是Qt的图形界面设计程序 
③Examples and Demos，Qt程序例子 
④Linguist，国际化语言翻译工具 
⑤Qt 4.8.6 (Build Debug Libraries)，创建Debug版本库，这个可以不管。 
⑥Qt 4.8.6 Command Prompt，Qt命令行开发环境设置，这个命令行工具会设置好Qt位置和MinGW位置，可以从命令行编译C++和Qt程序。 
⑦Qt Readme，自述文件，描述Qt信息 
⑧qt.digia.com，Qt官网链接 
⑨Uninstall Qt OpenSource 4.8.6，卸载Qt库。 
点开Qt 4.8.6 Command Prompt，可以尝试运行命令看看版本信息： 
g++ –version 
qmake –version
![这里写图片描述](https://img-blog.csdn.net/20160105202949468)
这个命令行工具对应命令是： 
%COMSPEC% /k “C:\Qt\4.8.6\bin\qtvars.bat” 
也就是说，如果想自己配置MinGW版本和Qt库，可以学习 qtvars.bat 文件来编写自己的 bat文件比如
@echo off 
rem 
rem This file is generated 
rem
echo Setting up a MinGW/Qt only environment… 
echo – QTDIR set to C:\Qt\4.8.6 
echo – PATH set to C:\Qt\4.8.6\bin 
echo – Adding C:\mingw32\bin to PATH 
echo – Adding %SystemRoot%\System32 to PATH 
echo – QMAKESPEC set to win32-g++-4.6
set QTDIR=C:\Qt\4.8.6 
set PATH=C:\Qt\4.8.6\bin 
set PATH=%PATH%;C:\mingw32\bin 
set PATH=%PATH%;%SystemRoot%\System32 
set QMAKESPEC=win32-g++-4.6
%COMSPEC% /k
上面第一段是关闭命令回显，表示执行命令时不打印命令。 
第二段是显示想要显示的信息，打印了五行。 
第三段是真正的设置环境变量的命令，就是提示的五条： 
QTDIR指示Qt 4.8.6根目录，中间三句设置PATH，包括Qt工具bin路径、MinGW bin路径和操作系统的System32路径，QMAKESPEC是指编译Qt程序的目标平台。 
%COMSPEC% /k 就是cmd.exe执行命令后继续保持运行的意思。
### （3）QtCreator安装
如果想从命令行编译Qt程序，上面环境已经够了。当然现在都是可视化开发的，就要装QtCreator集成开发环境，因为QtCreator使用起来方便快捷。 
运行qt-creator-opensource-windows-x86-3.3.0.exe，
![这里写图片描述](https://img-blog.csdn.net/20160105202959718)
点击“下一步”，进入目录指定界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105203009816)
设置安装位置后点“下一步”，进入许可同意界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105203040224)
选择接受许可，点击“下一步”： 
![这里写图片描述](https://img-blog.csdn.net/20160105203051823)
点击“下一步”， 
![这里写图片描述](https://img-blog.csdn.net/20160105203106467)
点击安装， 
![这里写图片描述](https://img-blog.csdn.net/20160105203116205)
点击“下一步”， 
![这里写图片描述](https://img-blog.csdn.net/20160105203128815)
点击“完成”，会打开QtCreator： 
![这里写图片描述](https://img-blog.csdn.net/20160105203139718)
### （4）QtCreator配置
因为QtCreator还没有配置，这时候是不能编译程序的。 
打开QtCreator菜单“工具”–>“选项”，在对话框左边选“构建和运行”：
![这里写图片描述](https://img-blog.csdn.net/20160105203203608)
编译Qt程序必须配置好几个东西才行，需要配置的是： 
构建套件（Kit）、QtVersions、编译器、Debuggers。
#### ①Debuggers
![这里写图片描述](https://img-blog.csdn.net/20160105203222562)
点击Add，进入新的Debugger设置界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105203233993)
把Name设为GDB，然后点击Path一行最右边的“浏览”，找到gdb.exe的位置， 
设置好后点击右下角的“Apply”，就设置了调试器。
#### ②编译器
![这里写图片描述](https://img-blog.csdn.net/20160105203243291)
点击右边“添加”，弹出菜单有MinGW、GCC、Clang、Custom和QCC，选择MinGW， 
![这里写图片描述](https://img-blog.csdn.net/20160105203304114)
名称设为MinGW，对于编译器路径设置，就浏览找到g++.exe，设置好这两条就够了，然后点击右下角“Apply”。
#### ③Qt Versions
![这里写图片描述](https://img-blog.csdn.net/20160105203312401)
点击“添加”，弹出的文件查找框，是要选择qmake.exe位置，之前装的位置是： 
C:\Qt\4.8.6\bin\qmake.exe，选中qmake.exe
![这里写图片描述](https://img-blog.csdn.net/20160105203330971)
点击打开，然后Qt版本会自动检测： 
![这里写图片描述](https://img-blog.csdn.net/20160105203340283)
点击“Apply”就配置好Qt库了。
#### ④构建套件（Kit）
![这里写图片描述](https://img-blog.csdn.net/20160105203348577)
之前配置的三个最后都是为构建套件服务的，需要在这里面选择之前配好的三个：编译器选择“MinGW”、调试器选择“GDB”、Qt版本选择“Qt 4.8.6(4.8.6)”，Qt mkspec可以不用管，因为qmake.exe可以自动定。配置好编译器、调试器和Qt版本之后，点击“Apply”，再点击“OK”。终于配置好了。
然后就可以新建一个项目，选择“Qt Widgets Application”，都用默认的项目设置，一直点击下一步，然后点“完成”，看到代码：
![这里写图片描述](https://img-blog.csdn.net/20160105203357604)
点击左下角倒数第三个图标，就是纯三角形那个： 
![这里写图片描述](https://img-blog.csdn.net/20160105203407981)
程序就能正常编译跑起来了。
# 二、Qt 5.4.0 下载和安装
看完上面Qt4开发环境的安装配置，相比之下Qt 5.4.0安装（不需要配置）就特别简单了。就是一个傻瓜包，注意选择组件时把组件全部选中，然后一直点“下一步”就完事了。下面简单介绍一下。
## 1、下载Qt5.4.0开发环境
[http://www.qt.io/download-open-source/](http://www.qt.io/download-open-source/)
点开“View All Downloads”，找到“Offline Installers”离线安装包， 
有Linux、MacOS和Windows的，关于Windows系统里Qt安装包：
![这里写图片描述](https://img-blog.csdn.net/20160105203416185)
头两个是VS2013 64位版本的Qt库，有OpenGL版本和非OpenGL版本，OpenGL版本就是指底层绘图用OpenGL驱动，非OpenGL底层是用DirectX驱动来绘图。第三、第四个是VS2013 32位版本，第五个第六个是VS2012和VS2010的Qt库。需要注意的是微软VS2012之后编译的程序默认没法在WindowsXP上面跑（可以打补丁支持XP）。不推荐使用Visual Studio开发Qt程序，因为VS没有Qt Creator好用，VS还总是有莫名其妙的问题出现。 
最好的就是下载 Qt 5.4.0 for Windows 32-bit (MinGW 4.9.1, 852 MB) 。 
下载列表第八个是针对Android开发的（比较麻烦，网上有教程）。 
第九个是针对Windows RT操作系统（一般用在Windows平板电脑）的Qt库。 
下载MinGW版本Qt文件是: 
qt-opensource-windows-x86-mingw491_opengl-5.4.0.exe 
运行后等待exe文件自己启动和解压，这需要的时间可能比较长，因为文件很大。
## 2、安装
![这里写图片描述](https://img-blog.csdn.net/20160105203431880)
点击下一步， 
![这里写图片描述](https://img-blog.csdn.net/20160105203441025)
点击下一步，进入组件选择界面： 
![这里写图片描述](https://img-blog.csdn.net/20160105203450302)
这里要注意要把东西全部勾选上，图里面第一个MinGW 4.9.1是指用采用MinGW编译而成的Qt库，第二个MinGW才是真正的编译调试环境。 
全部选择后点击下一步：
![这里写图片描述](https://img-blog.csdn.net/20160105203459029)
选择agree然后点击下一步：
![这里写图片描述](https://img-blog.csdn.net/20160105203513452)
点击下一步： 
![这里写图片描述](https://img-blog.csdn.net/20160105203521350)
点击安装，等待安装好，点击“完成”。 
![这里写图片描述](https://img-blog.csdn.net/20160105203531364)
因为安装包把东西都带齐了，所以可以直接使用QtCreator新建、编译、生成程序。如果本机存在多个Qt构建套件，在新建项目时，Kit Selection： 
![这里写图片描述](https://img-blog.csdn.net/20160105203554668)
可以选择多个Qt构建套件，不同套件会生成不同程序。编译时在左下角点开Debug或Release字样，可以选择不同的Qt库生成各自的Debug版程序或Release版程序。 
![这里写图片描述](https://img-blog.csdn.net/20160105203603940)
# 三、开发好的可执行程序如何发布
一般用Qt官方开发环境，生成的exe都是动态链接版本，如果要放到其他电脑上运行，需要复制一大堆依赖的dll，而且数量比较多，是个麻烦。
如果想用一个exe干净利落的发布程序，就需要使用静态编译版的Qt库，网上有教程，我就不多说了，可以教大家用现成的，在[Git](http://lib.csdn.net/base/git)-hub上正好有静态Qt库：
~~https://github.com/fostersoftware/QtStaticWindows~~
~~https://github.com/fostersoftware/QtStaticLinux~~
第二个是Linux的，Windows系统下载第一个QtStaticWindows，最好使用[git](http://lib.csdn.net/base/git)工具来下载，新建一个QtSatic文件夹，在该文件夹里执行：
~~git clone [https://github.com/fostersoftware/QtStaticWindows.git](http://blog.csdn.net/win_turn/article/details/50465127)~~
下载完成后，添加Qt静态库的方法参考第一部分的4.(4)QtCreator配置，只需要在QtCreator里面添加新的 Qt Version和新的构建套件（Kit）。 
注意上面的只有Release版本Qt库，所以要在使用Qt官方的动态库版开发调试完成之后，最后使用静态Release版Qt库生成独立的exe程序，就可以放到其他机器上跑了。

