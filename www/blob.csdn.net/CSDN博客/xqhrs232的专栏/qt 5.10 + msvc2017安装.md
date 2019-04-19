# qt 5.10 + msvc2017安装 - xqhrs232的专栏 - CSDN博客
2018年09月03日 13:53:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：686
原文地址::[https://blog.csdn.net/liji_digital/article/details/79233443](https://blog.csdn.net/liji_digital/article/details/79233443)
1）从visual studio官网下载免费版msvc2017并安装
2）从qt官网下载qt-unified-windows-x86-3.0.2-online.exe并运行
3）安装程序将询问安装哪一版qt?我选择的是基于msvc2017-64bit编译器的qt5.10.0
4）接下来的步骤是从https://jingyan.baidu.com/article/020278115d99b31bcc9ce53f.html学来的
5）启动VS2017，然后在弹出的主界面菜单栏中，点击“工具 > 扩展和更新”；
6）在弹出的“扩展和更新窗口”中，选择列表中的“联机”，然后在右侧的搜索框中输入“qt”并回车。搜索结束后，可以在返回的搜索列表中见到“Qt Visual Studio Tools”。选中它，并点击“下载按钮”下载该插件（注意：QtPackage插件仅支持到VS2015）；
7）等待插件信息下载完毕后，在“扩展和更新窗口”的底部会出现“更新已列入计划…”的提示信息。直接点击底部的“关闭按钮”关闭“扩展和更新窗口”；
8）关闭VS2017软件后，会自动弹出“VSIX安装器窗口”。等待它初始化完毕后，在出现的“计划任务界面”中，可以见到“Qt Visual Studio Tools”安装任务，直接点“修改按钮”确定安装；
9）安装过程中，我遇到警告：要关闭进程vctip.exe才能继续安装；
10）在命令行输入：tasklist | findstr vctip    获取vctip的进程号9140.然后再输入  taskkill /pid 9140 /F  杀死vctip；
![](https://img-blog.csdn.net/20180201213019420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaV9kaWdpdGFs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
11）安装得以完成。
12）等待“Qt Visual Studio Tools”安装结束后，会出现“修改完成界面”，点击该界面中的“关闭按钮”结束安装；
13）重新启动VS2017，会发现菜单栏中多了一个“Qt VS Tools菜单”，这个菜单就是之前安装的Qt插件提供的功能；
14）点击菜单栏上的“Qt VS Tools > Qt Options”打开Qt选项窗口；
15）在弹出的“Qt选项窗口”中，点击 “Add按钮”打开“Add New Qt Version窗口”。在该窗口中，写入自定义的Qt版本名称并指定Qt工具集所在目录（比如：C:\Qt\Qt5.9.0\msvc2017_64），然后点击“OK按钮”完成Qt版本添加；
16）然后可以用Qt VS Tools>Open Qt Project File 导入已有的qt pro文件，并编译，运行
17）编译过程可能遇到报错：找不到windows SDK8.1。这时鼠标右键点击解决方案，选择“重定解决方案目标”，在下图所示界面里重新指定SDK版本。然后重新编译。假如仍然报错，就在下图所示的下拉框里toggle几个选项，反复几次就好了。
![](https://img-blog.csdn.net/20180201221248912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaV9kaWdpdGFs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
//===============================================================================================备注::
1>qt下载网址----[http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)
