# windows 下cuda的配置 - 战斗蜗牛的专栏 - CSDN博客





2010年04月09日 16:29:00[vbskj](https://me.csdn.net/vbskj)阅读数：1164








VS2005及以上版本

支持CUDA的Geforce 8x/9x/1xx/2xx/3xx/4xx系列显卡，或MCP7x，ION芯片组

安装步骤为：

1.获取最新版本的driver, CUDA toolkit, CUDA SDK。无论显卡的CUDA计算能力如何，始终应该使用最新版本的驱动和**开发工具**。driver 和toolkit/SDK的版本应该与操作系统匹配。

2. 依次安装driver, toolkit 及SDK。安装完毕后进入SDK目录下的C/bin子目录下，根据操作系统不同进入Win32/release或者Win64/Release目录下，运行devicequery.exe，确认平台上的所有支持CUDA设备已被识别，信息正确无误。然后运行MatrixMul.exe确认显卡能够分配显存并调用kernel进行计算。 

3.配置生成规则。 将SDK目录下C/common目录下的cuda,rules拷贝到VS安装目录下的VC/VCProjectDefaults子目录下。建立一个Win32工程，并在项目上右击，选择custombuildrules,在CUDA build rule前打勾。在工程中新建一个.cu文件，右击属性后查看自定义生成规则是否已经是CUDA build rule。

4. 添加语法高亮。关闭VS, 将SDK目录下C/doc/syntax_highlighting/visual_studio_8子目录下的usertype.dat拷贝到Microsoft Visual Studio目录的/Common7/IDE子目录下。如果usertype.dat已经存在，则将其中的内容添加到已有文件中。打开visual  stuido，选择Tools->Options...中的text eidtor下的file extension，添加.cu，并将其编辑器设为Microsoft Visual C++，选择“add”后点"OK"，然后打开一个.cu文件，检查关键字是否已经高亮显示。



、、、、、、、、、、、、、、、

1.CUDA toolkit 和sdk


·[CUDA Toolkit version 1.1 for Win XP](http://developer.download.nvidia.com/compute/cuda/1_1/NVIDIA_CUDA_Toolkit_1.1_x86.exe) 
·[CUDA SDK version 1.1 for Win XP](http://developer.download.nvidia.com/compute/cuda/1_1/NVIDIA_CUDA_SDK_1.1_x86.exe)
ps: ·[NVIDIA Driver for Microsoft Windows XP with CUDA Support (169.21)](http://www.nvidia.com/object/winxp_169.21_whql.html) 在开发的时候，这个可以不用安装，如果是有支持CUDA的显卡的时候，安装这个才有效）

在安装过程中，会提醒你是否把sdk和toolkit的设置加入到VS的开发环境中，这里要选择是，但是CUDA windows 版本，目前只支持6.0. vs2003, vs2005的toolkit的bat，vs2008的好像还不支持。

2.设置系统环境变量

将安装的CUDA的sdk的路径加到系统环境变量中：

例如C:/Program Files/NVIDIA Corporation/NVIDIA CUDA SDK/bin/win32/
下的
├─Debug
├─EmuDebug
├─EmuRelease
└─Release

几个目录都加入到系统环境变量中，这样才能在运行程序的时候找到相应的dll库。

3.VS2005建立CUDA简单的工程

a.创建简单的空win32项目

b.加入新的项，自己修改为example.cu

c.右键点击exampl.cu 选择自定义编译选项

命令行中添加 "$(CUDA_BIN_PATH)/nvcc.exe" -ccbin "$(VCInstallDir)bin" -deviceemu -c -D_DEBUG -DWIN32 -D_CONSOLE -D_MBCS -Xcompiler /EHsc,/W3,/nologo,/Wp64,/Od,/Zi,/RTC1,/MTd -I"$(CUDA_INC_PATH)" -I./ -I../../common/inc -o $(ConfigurationName)/example.obj example.cu

输出里面修改为 $(ConfigurationName)/example.obj

依赖项 修改为 example.cu

ps:这里的编译选项是为了方便很多还没有支持CUDA开发的显卡的朋友。这里的用的编译选项是在模拟器上运行。如果到时候有了真的可以运行CUDA的硬件支持，可以自己修改编译选项；

下面就是自己写example.cu的程序。

d.右键点击example项目，选择最终编译选项为exe文件。

e.编译，执行，就会看到模拟器执行程序了：）


、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、





以下为VS2005的配置，VS2003和VS2008与此类似。

1、安装Visual Studio 2005环境。
2、安装开发助手Visual Assist X。
3、从 [http://www.nvidia.cn/object/cuda_get_cn.html](http://www.nvidia.cn/object/cuda_get_cn.html) 下载CUDA相关软件，并按次序安装。
CUDA 驱动--〉CUDA Toolkit --〉CUDA SDK

4、语法高亮：
将NVIDIA CUDA SDK/doc/syntax_highlighting/visual_studio_8里面的usertype.dat文件copy到Microsoft Visual Studio 8/Common7/IDE目录下面（如果已经存在，就追加到原来的后面）。

5、设置VS2005环境：
进入Tools|Options|Projects and Solutions|VC++Directories 添加:
Include files:d:/programming/cuda/toolkit/include和d:/programming/cuda/sdk/common/inc
Library files：d:/programming/cuda/toolkit/lib和d:/programming/cuda/sdk/common/lib
Source files：d:/programming/cuda/sdk/common/src

进入VC++ Project Settings：C/C++ File extensions：添加*.cu,在Text editor-File extension：添加cu 对应editor到Microsoft VC++ editor。

6、Visual Assist X设置：
关闭已经所有打开的Visual studio，之后进入注册表编辑器：HKEY_CURRENT_USER/Software/Whole Tomato/VANet8 找到右边的ExtSource项，将其值添加.cu;.cuh;之后关闭，再次打开VS2005即可。

7、在VS的Tools|Options|Projects and solutions|VC++ project setting里面的rules file search path里面添加 CUDA SDK安装目录的common目录的CUDA.rules文件所在路径。

8、新建工程->工程名上右键->选择custom build rules…->然后勾选新添加进去的Rule file。

9、在工程中添加.cu文件，右键点击cu文件->选择Properties->Configuration Properties->General->Tool的下拉列表中选择上一步勾选的Build Rule的名称，例如“CUDA Build Rule v2.1.1”->确定。



