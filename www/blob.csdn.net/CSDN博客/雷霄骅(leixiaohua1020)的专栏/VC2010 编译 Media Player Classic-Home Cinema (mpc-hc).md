# VC2010 编译 Media Player Classic - Home Cinema (mpc-hc) - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月14日 22:01:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：36
个人分类：[MPC-HC](https://blog.csdn.net/leixiaohua1020/article/category/2286581)










Media Player Classic - Home Cinema (mpc-hc)播放器一个经典的影音播放器，免费软件，可播放CD、VCD、DVD、及MP3、MP4、AVI、AAC等多种影音格式。与此同时，它还是开源的。今天尝试着编译了一下它的源代码（还是第一次接触这么大的MFC工程）

![](https://img-blog.csdn.net/20130914215814328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第一步:：准备

1. 安装 Visual C++ 2010(不能是Express版本)
 2. 安装Visual Studio 2010 Service Pack 1 -> [http://www.microsoft.com/downloads/en/details.aspx?FamilyID=75568aa6-8107-475d-948a-ef22627e57a5&displaylang=en](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=75568aa6-8107-475d-948a-ef22627e57a5&displaylang=en)
 3. 安装DirectX SDK (June 2010) -> [http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=3021d52b-514e-41d3-ad02-438a3ba730ba](http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=3021d52b-514e-41d3-ad02-438a3ba730ba)

 第二步：准备GCC环境（这个主要是编译FFMPEG用的，FFMPEG只能用GCC编译）

1. 下载并解压 MSYS_MinGW-w64_GCC_481_x86-x64.7z 到 "C:\MSYS" 地址：[http://xhmikosr.1f0.de/tools/MSYS_MinGW-w64_GCC_481_x86-x64.7z](http://xhmikosr.1f0.de/tools/MSYS_MinGW-w64_GCC_481_x86-x64.7z)

（注意：就算自己电脑上有MSYS_MinGW，也建议下载这个）

2. 在 "C:\mpc-hc"创建一个"build.user.bat"，内容如下（有些参数根据自己系统确定）：

```
@ECHO OFF
SET "MPCHC_MSYS=C:\MSYS"
SET "MPCHC_MINGW32=%MPCHC_MSYS%\mingw"
SET "MPCHC_MINGW64=%MPCHC_MINGW32%"
REM Git is optional to set if you chose to add it in PATH when installing it（下面这条可以不要）
SET "MPCHC_GIT=C:\Program Files (x86)\Git"
```


 第三步：编译

1.使用Git 下载 MPC-HC's 到 "C:\mpc-hc" （其他地方也行）

Git命令（俩都行）：

git clone --recursive https://github.com/mpc-hc/mpc-hc.git

或：

git clone https://github.com/mpc-hc/mpc-hc.git
 git submodule update --init --recursive

2. 打开sln文件 "C:\mpc-hc\mpc-hc.sln"，编译之（注：如果Release编译不成功，可以试试编译Release-Lite）

3. 在"C:\mpc-hc\bin\mpc-hc_x86"下得到"mpc-hc.exe"
 4. 打开sln文件 "C:\mpc-hc\mpciconlib.sln"，编译之
 5. 得到"mpciconlib.dll"
 6. 打开sln文件 "C:\mpc-hc\mpcresources.sln"，编译之
 7. 得到 "mpcresources.XX.dll"
 注：直接运行"build.bat" 可以编译所有文件






