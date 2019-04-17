# OpenCV的安装   环境配置 - 家家的专栏 - CSDN博客





2010年08月25日 19:49:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：702标签：[application																[dependencies																[list																[library																[include																[insert](https://so.csdn.net/so/search/s.do?q=insert&t=blog)
个人分类：[openCV](https://blog.csdn.net/yihaizhiyan/article/category/715111)





OpenCV的安装时，在c++环境中的配置：



打开VC++.NET应用程序,在菜单项里选择:Tool->Options

在列表中,选择Projects->VC++ Directories

首先,从”Show Directories for”的list box 中选择”Library files”

单击”Insert New”图标,定位你安装的文件夹

假设它安装于”D:/program Files/OpenCV”

在库文件列表中,定位并添加:

”D:/program Files/OpenCV/lib”

然后在list box中选择include文件,定位并添加以下路径:

D:/program Files/OpenCV/cv/include

D:/program Files/OpenCV/cxcore/include

D:/program Files/OpenCV/otherlibs/highgui

D:/program Files/OpenCV/cvaux/include

D:/program Files/OpenCV/otherlibs/cvcam/include

接下来,选择list box中的source文件, 定位并添加下下路径:

D:/program Files/OpenCV/cv/src

D:/program Files/OpenCV/cxcore/src

D:/program Files/OpenCV/cvaux/src

D:/program Files/OpenCV/otherlibs/highgui

D:/program Files/OpenCV/otherlibs/cvcam/src/windows

单击选项对话框的”OK”

现在你就正确配置好了全局设置.



创建新的项目

在Developer Studio中创建新的application

从菜单中选择”File”->”New……”->”Projects”

选择”win32 Application”或者”win32 console application”,后者更加容易,示例都采取这种方式

输入项目名称现则位置

单击OK,在向导中的直接单击finish.



以上步骤之后developer studio将创建一个项目文件夹(缺省与项目同名)、<project name>.vcproj文件、solution<project name>.sln以及三个源文件：<project name>.cpp、stdafx.cpp和stdafx.h。stdafx文件是预编译头文件，对于消减编译时间很有用处。

例如，假设我们创建了一个新的叫”hello”的项目。打开hello.cpp,包含opencv相关的#include路径：

#include <cv.h>

#include <cxcore.h>

#include <highgui.h>

然后键入一些opencv的代码，按F7进行Build，会有些Link错误。

添加项目的depencency

从菜单：”Projecet”->”Properties”

选择”Link”tab->”Input”->”Addition Dependencies”。添加所有需要的库

（cxcore[d].lib cv[d].lib highgui[d].lib cvaux[d].lib cvcam[d].lib）这就OK了。。](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=dependencies&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)




