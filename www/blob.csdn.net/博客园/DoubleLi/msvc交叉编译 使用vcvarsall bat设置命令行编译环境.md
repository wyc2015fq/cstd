# msvc交叉编译:使用vcvarsall.bat设置命令行编译环境 - DoubleLi - 博客园






一直以来我只知道vc设置命令行编译环境的批处理命令是%VS140COMNTOOLS%/Common7/Tools下的vsvars32.bat,(%VS140COMNTOOLS%为定义vs2015公共工具程序位置的环境变量，命令行窗口输入set就能找到,vs2010的对应定义为%VS100COMNTOOLS%) 
微软的官网上有这个命令的说明: 
[https://technet.microsoft.com/zh-cn/library/1700bbwd.aspx](https://technet.microsoft.com/zh-cn/library/1700bbwd.aspx)

但在执行这个命令生成的命令行环境下用nmake只能编译32位版本的代码，我一直都不知道如何用nmake编译64位的代码。 
今天才搞明白vsvars32.bat已经过时了，正确的打开方式是vcvarsall.bat 。 
至少从vs2010开始， 
%VS140COMNTOOLS%/VC下就有vcvarsall.bat,用于生成命令行编译环境。 
如果要在命令行生成 32位代码，就执行`vcvarsall x86`
如果要在32位系统下生成64位代码，就执行`vcvarsall x86_amd64`
如果要在64位系统下生成32位代码，就执行`vcvarsall x86`或`vcvarsall amd64_x86`
到了VS2015，已经支持arm平台了，所以如果要生成arm平台的代码，就执行`vcvarsall x86_arm` 如果你的[操作系统](http://lib.csdn.net/base/operatingsystem)是64位的也可以 `vcvarsall amd64_arm`

前面一个名字代表你的当前电脑的体系结构，后面的这个名字代表你要生成的代码的体系结构。如果两个名字一样，就简化为一个名字。 
搞清楚了这个，今天终于顺利在命令行下实现nmake编译32位和64位版本代码：

生成用于编译32位代码的命令行编译环境

```
C:\Program Files (x86)\Microsoft Visual Studio 14.0>cd VC
C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC>vcvarsall x86
```



执行cmake生成NMake格式的Makefile,指定处理器为x86，然后执行nmake编译所有代码，并安装

```
>cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DTARGET_PROCESSOR=x86 ..\facecl 
>nmake install
```

安装成功的输出

```
Install the project...
-- Install configuration: "RELEASE"
-- Installing: C:/Program Files (x86)/facecl/lib/static/detect_cl.lib
-- Installing: C:/Program Files (x86)/facecl/bin/detect_cl.dll
-- Installing: C:/Program Files (x86)/facecl/lib/static/img_tool.lib
-- Installing: C:/Program Files (x86)/facecl/bin/img_tool.dll
-- Installing: C:/Program Files (x86)/facecl/bin/test_detect.exe
-- Up-to-date: C:/Program Files (x86)/facecl/./README_utf8.txt
-- Up-to-date: C:/Program Files (x86)/facecl/include/detect_cl_types.h
-- Up-to-date: C:/Program Files (x86)/facecl/include/detect_cl.h
-- Up-to-date: C:/Program Files (x86)/facecl/include/img_tool.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/test_detect.cpp
-- Up-to-date: C:/Program Files (x86)/facecl/sample/utility.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/raii.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/assert_macros.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/cmdline.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/dirent.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/file_utilits.h
-- Up-to-date: C:/Program Files (x86)/facecl/sample/time_utilits.h
```
- 

如果要在32位系统下生成64位代码，也如法炮制

关于vcvarsall.bat更详细的说明，参见微软的官方文档： 
[https://msdn.microsoft.com/zh-cn/library/f2ccy3wt.aspx](https://msdn.microsoft.com/zh-cn/library/f2ccy3wt.aspx)









