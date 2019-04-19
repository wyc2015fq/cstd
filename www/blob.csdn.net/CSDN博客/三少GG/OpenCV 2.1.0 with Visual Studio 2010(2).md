# OpenCV 2.1.0 with Visual Studio 2010(2) - 三少GG - CSDN博客
2010年11月09日 20:58:00[三少GG](https://me.csdn.net/scut1135)阅读数：2265标签：[2010																[installer																[windows																[vector																[path																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=installer&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
如果Debug下不能运行，则切换到Release版本下!!!!(pom 和CLEARMetrics两个程序都是如此！！！都用了opencv，release下才可以运行，否则出现各种错误问题，比如 “vector iterators incompatible”  0xc0150002 问题
说明：
[http://hi.baidu.com/octozi/blog/item/e82c7f510329db6c8435246b.html](http://hi.baidu.com/octozi/blog/item/e82c7f510329db6c8435246b.html)
**实践中可用的方案2：**
以下配置方法仅属于个人见解；若有什么不对的地方请指教啊。
在完成每一步骤后，记住点击“确定”，以确保你的设置成功。
在某些步骤中注意要在Debug 和 Release 分别进行配置。
1.首先是下载VS2010并安装，这里不作赘述；（安装目录：D://Program Files/Microsoft  Visual  Studio 10.0）
2.下载并安装OpenCV,我的版本是2.1（安装目录为D:/Program Files/OpenCV2.1）
////////////下面这步貌似不必要//////////////////
注意将路径添加至Path中
///////////////////////////////////////////
3.下载并安装CMake，下载版本为Windows（Win32 Installer），安装目录为D:/Program Files/CMake 2.8
4.运行cmake-gui(D:/Program
Files/CMake 2.8/bin)，设置路径为OpenCV安装路径（本文档假定安装位置为：D:/Program Files/OpenCV2.1），并创建子目录D:/Program
Files/OpenCV2.1/vs2010，用于存放编译结果。将数据填入指定文本框中
      5.点击“Configure”，在弹出对话框中选中Visual Studio 10.0;单选框中选项选择程序默认的；然后点击Fnish；程序会自动完成“Configure Done”；
      6.然后点击“configure”，接着点击“Generate”。程序完成“Generate Done”。
7.完成以上步骤后，将在D:/Program
Files/OpenCV2.1|vs2010目录下生成OpenCV.sln的VC Solution File，接着请用vs2010打开OpenCV.sln，完成以下操作：
      7.1在Debug下，选择“解决方案资源管理器”下的“解决方‘OpenCV’”，点击右键，运行“重新生成解决方案”，若编译无错，在选择“INSTALL”项目，运行“生成”。
      7.2在Release下，选择“解决方案资源管理器”下的“解决方‘OpenCV’”，点击右键，运行“重新生成解决方案”，若编译无错，在选择“INSTALL”项目，运行“生成”。
此时，OpenCV的*d.dll文件（for debug）和*.dll文件（for release）将出现在D:/Program Files/OpenCV2.1/vs2010/bin目录中；OpenCV的*d.lib文件（for
debug）和*.lib文件（for release）将出现在D:/Program Files/OpenCV2.1/vs2010/lib目录；头文件*.h出现在D:/Program Files/OpenCV2.1/vs2010/include/opencv中。 
可以被VS 
C++ 2010调用的OpenCV动态库生成完毕
8.配置Windows环境变量Path   
D:/Program
Files/OpenCV2.1/vs2010/bin
      8.接下来的工作很重要：
要将OpenCV和VS2010配置好，必须先创建一个工程：注意没有创建工程是不能进行配置的。
比如：我们创建一个MFC应用程序，名称为“Test”
点击确定，接下来
是一些设置问题：
单击完成。
      9.点击项目à属性à配置属性àVC++目录对 “常规”子菜单的“包含目录”和“库目录”进行编辑。
注意 在配置  Debug 和Release下进行相同操作，每次修改后记住保存。
在包含目录下添加
D:/Program Files/OpenCV2.1/vs2010/include/opencv
在库目录下添加D:/Program Files/OpenCV2.1/vs2010/lib
10.右键单击 “Test”选中“属性”，进入“Test属性页”，
为项目的Debug配置增加依赖的库：
cv210d.lib; cvaux210d.lib;cxcore210d.lib;
highgui210d.lib ;ml210d.lib;cxts210.lib    （ 注意！：别忘了分号）
为项目的Release配置增加依赖的库：
cv210.lib; cxcore210.lib;cxts210.lib; highgui210.lib
;ml210.lib ;cvaux210.lib
配置完成。
配置完成后，可以在Debug和Release系分别对程序进行调试。
若有什么问题，请邮箱联系：969691790@qq.com
