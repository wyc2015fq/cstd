# Linux上使用Qt Creator进行C/C++开发 - DoubleLi - 博客园








目录[-]

- [1、安装Qt Creator和基础构建环境(以Ubuntu为例)：](http://my.oschina.net/eechen/blog/166969#OSC_h2_1)
- [2、把自动完成快捷键"Ctrl+空格"改为"Alt+/"或者"Shift+Return"(Return表示Enter键)](http://my.oschina.net/eechen/blog/166969#OSC_h2_2)
- [3、在Qt Creator中使用make构建,运行,调试通用C/C++项目](http://my.oschina.net/eechen/blog/166969#OSC_h2_3)
- [4、在Qt Creator中使用cmake构建,运行,调试通用C/C++项目](http://my.oschina.net/eechen/blog/166969#OSC_h2_4)
- [5、Qt Creator会自动识别系统中安装的gcc、g++、gdb、make、cmake、valgrind,也可以自己到菜单"工具"->"选项"里进行一些自定义配置.](http://my.oschina.net/eechen/blog/166969#OSC_h2_5)



![](http://static.oschina.net/uploads/space/2014/0217/025140_OJwD_561214.png)

Qt Creator支持中文，启动速度比Eclipse、Netbeans更快，界面也更美观，跟输入法Fcitx协作良好（光标跟随）。

你既可以把Qt Creator当代码编辑器作为vim辅助，好处在于Qt Creator能够自动parse项目，形成友好快速的智能提示，而且你也同样可以使用Qt Creator里面的gdb前端(上图)和[valgrind前端(高清大图)](http://static.oschina.net/uploads/space/2015/0131/102504_xkNH_561214.png) 对项目进行调试和内存分析，不过Makefile就要自己写或者用autotools生成了，这对于使用vim进行编程的同学来说，应该是轻车熟路的。差点忘了，连续按两次Alt+V快捷键即可进入VIM编辑模式(FakeVim)，这应该很能吸引vim用户。当然，Qt Creator默认支持主流版本控制系统git/hg/svn等。

你也可以把Qt Creator当作IDE，这时可以作为Eclipse CDT、 Netbeans for C++这些重量级IDE的一个轻量替代品。你可以直接在Qt Creator的向导里新建一个非Qt使用CMake自动构建的项目，这意味着你不用自己写Makefile，你只要在Qt Creator里“执行CMake”就能自动生成。当然，你也可以导入一个make构建的项目到Qt Creator里编译调试，比如Nginx。

## 1、安装Qt Creator和基础构建环境(以Ubuntu为例)：

安装基础系统构建工具包：

sudo apt-get -y install build-essential gdb valgrind cmake

下载最新版二进制Qt Creator(建议使用基于Qt4构建的2.8系列的Qt Creator，能更好地兼容Fcitx输入法)：

[http://mirrors.ustc.edu.cn/qtproject/official_releases/qtcreator/2.8/2.8.1/](http://mirrors.ustc.edu.cn/qtproject/official_releases/qtcreator/2.8/2.8.1/)

不进行GUI编程，不需要安装Qt SDK，也就没有qmake和相关Qt开发库，只用下载那个60M的Qt Creator就好了，比如我下的是：

qt-creator-linux-x86_64-opensource-2.8.1.run

下载后给予执行权限，运行选择安装目录安装即可，可以安装在自己的家目录。

项目配置默认存放位置~/.config/QtProject

可以通过参数-settingspath自定义配置文件存放位置，比如：

/home/eechen/qtcreator-2.8.1/bin/qtcreator -settingspath /home/eechen/qtcreator-2.8.1

**注意：**如果你用的是基于Qt5构建的Qt Creator 3系列，其默认集成了iBus输入法插件，而对于Fcitx插件，则需要手动安装包fcitx-frontend-qt5， 然后复制libfcitxplatforminputcontextplugin.so到Qt Creator 3的 bin/plugins/platforminputcontexts 目录，比如： sudo apt-get install fcitx-frontend-qt5
ln -s /usr/lib/`arch`-linux-gnu/qt5/plugins/platforminputcontexts/libfcitxplatforminputcontextplugin.so /png/dev/qtcreator-3.2.1/bin/plugins/platforminputcontexts/
重启QtCreator,就可以正常使用Fcitx输入法了.

## 2、把自动完成快捷键"Ctrl+空格"改为"Alt+/"或者"Shift+Return"(Return表示Enter键)

工具-选项-环境-键盘: 键盘快捷键输入"Ctrl+空格",在键位顺序中点击x删除原来的"Ctrl+空格",输入新的"Alt+/",确定退出.

## 3、在Qt Creator中使用make构建,运行,调试通用C/C++项目

文件-新建文件或项目-导入项目-导入现有项目

这样导入的项目用的是make构建,需要自己写好Makefile文件后才能构建,支持自动完成,支持调试和内存分析.

## 4、在Qt Creator中使用cmake构建,运行,调试通用C/C++项目

文件-新建文件或项目-非Qt项目:新建一个CMake项目

在"执行CMake"这一步的参数中填入-DCMAKE_BUILD_TYPE=Debug

这样才可以在Qt Creator中调试程序,-DCMAKE_BUILD_TYPE的值可以设为Debug或者Release或者RelWithDebInfo.

cmake可以自动生成Makefile方便用make构建项目,但使用cmake构建项目也必须要会编写CMakeLists.txt文件.

Qt Creator支持使用qmake,make,cmake构建项目,但不能直接往cmake项目里添加文件,Qt Creator对CMake的支持还有待改进.

临时的做法是,切换到文件管理器来添加文件,然后右击项目"执行CMake"使parsing(语法解析)生效,并载入文件到在项目.

## 5、Qt Creator会自动识别系统中安装的gcc、g++、gdb、make、cmake、valgrind,也可以自己到菜单"工具"->"选项"里进行一些自定义配置.

配置vim外部编辑器:

工具->选项->环境->外部工具->使用vi进行编辑

把xterm改为konsole或者gnome-terminal

参数改为-e vim "%{CurrentDocument:FilePath}"

配置终端:

工具->选项->环境->概要

KDE环境默认为/usr/bin/konsole -e

如果要用gnome-terminal,则设为/usr/bin/gnome-terminal -x

设置在终端运行程序:

项目(侧边栏)-运行-勾选"在终端中运行"

调试(gdb)时出现"ptrace operation not permitted"问题解决办法:

临时设置: sudo sysctl kernel.yama.ptrace_scope=0

永久设置: 编辑/etc/sysctl.d/10-ptrace.conf改为kernel.yama.ptrace_scope = 0

系统默认设为1的目的是为了安全，防止用户程序(比如这里的gdb)访问其他进程(比如这里的konsole,gnome-terminal)内存的数据.

PS：附一张图，在Qt Creator中使用valgrind分析Nginx的内存使用：

![](http://static.oschina.net/uploads/space/2014/0217/025155_mCvP_561214.png)










