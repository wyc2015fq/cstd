# Windows QT环境搭建流程 - L_Andy的专栏 - CSDN博客

2017年08月18日 10:11:47[卡哥](https://me.csdn.net/L_Andy)阅读数：197
个人分类：[Qt](https://blog.csdn.net/L_Andy/article/category/5882119)



**Windows QT环境搭建流程（QT4.8.6 + QT Creator + MInGW）**

1、先安装IDE编译环境qt-creator-opensource-windows-x86-3.1.1.exe。请使用默认路径C:\Qt\qtcreator-3.1.1。

2、解压缩MinGW编译器工具i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z。

请把里面的文件夹mingw拷贝到路径C:\Qt\mingw32。

3、最后安装库文件qt-opensource-windows-x86-mingw482-4.8.6-1.exe。

   QT默认的安装路径是C:\Qt\4.8.6，这里建议修改为C:\Qt\4.8.6_MinGW，此举是为了与其他Qt版本库区分开。避免把已安装的其他QT版本覆盖并破坏掉。(注：支持windows环境的QT库有mingw，vs2010，vs2008等版本)

   当安装提示Find installedMinGW时，请用户指向路径C:\Qt\mingw32。

4、以上步骤完成后，执行桌面快捷方式QtCreator。

   (1)进入软件->菜单->工具->选项->构建和运行->Qt版本->手动设置，添加qmake路径C:\Qt\4.8.6_MinGW\bin\qmake.exe。

   (2)进入软件->菜单->工具->选项->构建和运行->编译器->手动设置，添加MinGW，并设置相应路径。

   编译器路径C:\Qt\mingw32\bin\g++.exe。

   (3)进入软件->菜单->工具->选项->构建和运行->调试器->手动设置，添加MinGW Debugger，并设置相应路径。

   调试器路径C:\Qt\mingw32\bin\gdb.exe。

   (4)进入软件->菜单->工具->选项->构建和运行->构建套件(Kit)，把编译器选择为"MinGW"，调试器选择"MinGW Debugger"。

   Qt版本选择为"Qt4.8.6(4.8.6_MinGW)"。

