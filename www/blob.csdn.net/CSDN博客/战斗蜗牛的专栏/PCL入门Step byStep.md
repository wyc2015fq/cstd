# PCL入门Step byStep - 战斗蜗牛的专栏 - CSDN博客





2012年07月27日 18:44:04[vbskj](https://me.csdn.net/vbskj)阅读数：13043








## 一、独立安装包自动安装

1.   安装Microsoft VisualStudio 2008；

2.   安装PCL-1.5.1-AllInOne-msvc2008-win32；

(安装过程注意选择“增加环境变量路径”)

3.   安装cmake-2.8.8-win32-x86；

(安装过程注意选择“增加环境变量路径”)



## 二、利用CMake获取VS2008工程文件

1.   启动VS自带的命令行窗口，cd到源代码所在的目录，例如：“C:\Program Files\PCL1.5.1\share\doc\pcl-1.5\tutorials\sources\pcd_write”；



2.   敲入命令“cmakeCMakeLists.txt”(该txt为cmake脚本文件)；

注：编译source目录下的CMakeLists.txt会编译ALL_Build.sln,可以对所有工程进行统一编译。



3.  在当前目录下生成VS2008或VS2010的工程文件，接着用VS打开进行编译。



## 三、利用VS2008编译PCL文件

1．  直接打开编译好的工程文件，选择相应项目为“启动项目”，无论debug或者release方式编译，运行时都会出错。错误描述为“无法定位程序输入点。。。”。

（由于cmake的脚本文件的编译方式为minisize）

2.   选择编译模式为“MiniRel”,编译运行皆通过；

3.   为了方便调试，下面介绍如何加入MiniDeb(minisize debug)模式：

（1）菜单 Build ->Configuration...，点击
 Add 按钮，从 Release版本复制一个编译选项，命名为“Release withdebug info”，当然名称随意。

（2）菜单 Project ->Settings...，在左上角选中刚才创建的“Release with debug info”，然后修改以下内容：

　　a、C++ |Category: General：Optimizations 设置为“Disable (debug)”；选中“Generate browse info”；Debug Info 设置为“Program Database”。

　　b、Link |Category: General：选中“Generate debug info”。

（3）菜单 Build -> Set ActiveConfiguration...，选中“Release with debuginfo”。

经过以上配置之后，编译，即可得到“带调试信息的Release程序”，可设置程序断点，可跟踪调试。这种版本程序既有 Debug 的一面，又有 Release 的一面，当然更多的倾向于 Release。



4.   后来发现生成的工程里面带有RelWithDebInfo编译选项，可以进行调试。

## 二、源代码手动编译

1.按照下列网址要求下载第三方库

[http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers](http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers)



2.从svn下载源代码

![SVN Checkout dialog](http://www.pointclouds.org/documentation/tutorials/_images/SVNCheckout_pcl_trunk.png)





3.  运行CMake-GUI，填写如下内容（可以填写自定义的目录）：


Where is the source code  : C:/PCL/trunk

Where to build the binaries: C:/PCL




4. 点击“configure”按钮，选择你所安装的编译器。（vc10:2010，vc9:2008），然后CMake会自动进行编译环境搜索，可以勾选“group,advanced”,然后分别可以查看下面的选项进行选择。

P.S.：如果这时没有安装第三方库，会报错，按照网页说明安装第三方库即可。



5. 此时在编译PCL_KeyPoints时出现错误，似乎是QT4的问题。



需要把QT的环境变量手动添加：

path： 添加“C:\qt\4.8.0\bin”

QTDIR:“C:\qt\4.8.0”

QMAKESPEC:” win32-msvc2010”

INCLUDE:” C:\Program Files\Microsoft VisualStudio 10.0\VC\include”

LIB:” C:\Program Files\Microsoft VisualStudio 10.0\VC\lib”



然后重启cmake-gui进行编译，发现成功(编译结果窗口显示的红色字体，表示有错误；如果使用GPU的童鞋，在编译之前需要安装CUDA，然后在Cmake-GUI中间的编译选项中选择GPU、CUDA即可，然后点击configure)



6.如果所有configure成功，点击“generate”按钮来产生VS2010的工程文件。所有编译的结果出现在“Where to build thebinaries”设置对应的目录中。



7.利用编译好的vs2010工程文件编译pcl源代码。

![PCL solution with project folders](http://www.pointclouds.org/documentation/tutorials/_images/pcl_solution_with_projects_folder.png)

![Build ALL_BUILD project](http://www.pointclouds.org/documentation/tutorials/_images/msvc_build_build_all1.jpg)




注意：需要编译Debug和Release两个模式！



8.如果需要编译成独立的第三方库和可执行程序，则编译“INSTALL”工程即可。

![Build INSTALL project](http://www.pointclouds.org/documentation/tutorials/_images/msvc_build_install1.jpg)





