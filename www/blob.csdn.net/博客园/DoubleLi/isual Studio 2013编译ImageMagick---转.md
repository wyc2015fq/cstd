# isual Studio 2013编译ImageMagick---转 - DoubleLi - 博客园









- “该文引用自 CruiseYoung的：Visual Studio 2013编译ImageMagick  
- http://blog.csdn.net/fksec/article/details/36008343”  



# 1 前期准备

> 1.1 C++11特性支持

> 
> Visual C++ Compiler November 2013 CTP (for  Visual Studio 2013)

> 下载页面：http://www.microsoft.com/en-us/download/details.aspx?id=41151

> 直接地址：http://download.microsoft.com/download/3/C/2/3C271B79-6354-4B66-9014-C6CEBC14C5C4/vc_CompilerCTP.Nov2013.exe

> 注1：

> Visual C++ Compiler November 2012 CTP (for  Visual Studio 2012)

> 下载页面：http://www.microsoft.com/en-us/download/details.aspx?id=35515

> 直接地址：http://download.microsoft.com/download/5/1/6/5169AA1E-D7A4-4DC2-A9AE-0A9DFD2601AB/vc_compilerCTPNov2012.exe


> 1.2 “OpenCL”支持：

> 
> 请参见：《Visual Studio 2013、TortoiseSVN、TortoiseGit、msysgit编译Tesseract(tesseract-ocr)》一文中的4.7节 地址：http://blog.csdn.net/fksec/article/details/32714639


# 2 官方网站

> ImageMagick官网：http://www.imagemagick.org/
ImageMagick中文站：http://www.imagemagick.com.cn/

# 3 参考文档

> ImageMagick-6.8.9\Install-windows.txt

# 4 代码下载

> 下载地址：http://www.imagemagick.org/download/
Windows编译，请点击“源码下载地址”页面中的“windows”目录连接进入；下载带“-windows”的压缩包，“zip”和“7z”任选其一即可。
最新稳定版本源码直接地址：
http://www.imagemagick.org/download/windows/ImageMagick-windows.7z
或
http://www.imagemagick.org/download/windows/ImageMagick-windows.zip

# 5 Win32(x86)平台编译

> 5.1 将源代码解压到当前目录，并进入ImageMagick-6.8.9\VisualMagick\configure目录，只有windows版本才有VisualMagick及其子目录
5.2 在当前目录找到“configure.exe”文件，如果没有，进行以下操作：

> 
> 用Visual Studio 2013打开configure.sln文件，直接编译；

> 友情提示1：此处工程编译可以不考虑“解决方案平台”是“WIN32”或“x64”，也不需考虑“解决方案配置”是“Debug”或“Release”。只需择一即可，建议“解决方案平台”选择“WIN32”，“解决方案配置”选择“Release”；

> 友情提示2：此处工程“configure”只有“WIN32”的平台选项，没有“x64”平台选项，如果您要添加“x64”平台选项，其详细过程如下：

> 工程“configure”-->右键“属性”(-->配置属性)-->“配置管理器(O)”-->“项目上下文(选定要生成的或部署的项目配置)(R):”-->“平台”-->下拉“新建”-->“新建平台”选择“x64”-->“从此处复制设置(S):”选择“Win32”-->勾选“创建信的解决方案平台(C)”-->“确定”。

> 解决：模块计算机类型“x64”与目标计算机类型“X86”冲突 问题：工程“configure”-->右键“属性”(-->配置属性)-->“配置(C)”选择“所有配置”-->“平台(P)”选择“所有平台”

> -->“配置属性”-->“链接器”-->“命令行”-->“其他选项(D)”删除“/MACHINE:I386 ”-->“确定”

> 或

> -->“配置属性”-->“链接器”-->“所有选项”-->“附加选项”删除“/MACHINE:I386 %(AdditionalOptions)”-->“确定”。


> 5.3 双击“configure.exe”文件，进行“编译类型”以及“编译选项”的选择。

> 
> 5.3.1 “编译类型”：在静态类型的选择共有3种，首先应排除“Static Single-threaded runtimes”，因为现在几乎不用了， Visual Studio 2010以上的版本好像都不提供该选项了。

> 剩余的两项，个人比较偏好“Static Multi-threaded DLL runtimes”，这也是Visual Studio创建“static”工程的默认选项。

> 5.3.2 “编译选项”：如果勾选“Generate utility projects with full paths instead of relative”项，则必须保证整个工程中不包含“中文路径”；个人建议该选项任何时候都不需要选择。

> 5.3.3 “编译选项”中不要勾选“Build 64-bit distribution”


> 5.4 解决方案升级：

> 
> 5.4.1 复制“UpgradeToVS2012.cmd”为“UpgradeToVS2013.cmd”并将其内容中的替换为如下内容：



**[plain]**[view plain](http://blog.csdn.net/fksec/article/details/36008343#)[copy](http://blog.csdn.net/fksec/article/details/36008343#)

[print](http://blog.csdn.net/fksec/article/details/36008343#)[?](http://blog.csdn.net/fksec/article/details/36008343#)

- @echo off  
- REM This script upgrades the solution to VS2013 and fixes the MSB8012 error.  
- 
- FOR /F "tokens=3" %%A IN ('REG QUERY "HKLM\SOFTWARE\Microsoft\PowerShell\1" /v Install ^| FIND "Install"') DO SET PowerShellInstalled=%%A  
- 
- IF NOT "%PowerShellInstalled%"=="0x1" GOTO NOT_INSTALLED  
- 
- call "%vs120comntools%vsvars32.bat"  
- powershell -ExecutionPolicy Unrestricted .\build\UpgradeSolution.ps1 2013  
- 
- GOTO DONE  
- 
- :NOT_INSTALLED  
- ECHO "This script needs powershell"  
- 
- :DONE  
- PAUSE  



> 5.4.2 执行“UpgradeToVS2013.cmd”，等待其执行直到出现“请按任意键继续...”时，按任意键结束升级。


> 5.5 解决方案修正：

> 
> 5.5.1 字符集问题，即解决“pango-language-sample-table.h(551): error C2001: 常量中有换行符”问题：用“Notepad++”打开“ImageMagick-6.8.9\pango\pango\pango-language-sample-table.h”文件，在菜单“格式”中选择“以UTF-8格式编码”，保存。注意，不要用“转为UTF-8编码格式”！

> 5.5.2 项目依赖问题，即解决“无法打开输入文件“CORE_RL_pango_.lib””以及“无法打开输入文件“CORE_DB_pango_.lib””问题：

> 工程“UTIL_stream”-->右键“生成依赖项(B)”-->“项目依赖项(S)...”-->选项卡“依赖项”-->列表框“依赖于(D):”-->勾选工程“CORE_pango”-->“确定”


> 5.6 完整编译

> 
> 解决方案-->右键“批生成(T)...”-->“选定要生成的项目配置(K):”-->勾选“项目”值为“All”，“平台”值为“Win32”的行-->“重新生成(R)”；


# 6 x64平台编译

> 6.1 将下载的源代码重新解压一份到其它目录，来进行x64平台的编译：
6.2 以下步骤同“5.2”至“5.3.2”
6.3 在“编译选项”中勾选“Build 64-bit distribution”
6.4 以下步骤同“5.4”
6.5 升级后的解决方案不能用来编译Win32(x86)平台，否则会出现很多头文件找不到的错误，原因是在此解决方案中Win32平台中所有工程属性的“配置属性”-->“C/C++”-->“常规”-->“附加包含目录”项为空。
6.6 由于升级后的解决方案的工程“All”没有“x64”的“解决方案平台”选项，编译不方便，所以请按如下操作添加：

> 
> 6.6.1 工程“All”-->右键“属性”(-->配置属性)-->“配置管理器(O)”-->“项目上下文(选定要生成的或部署的项目配置)(R):”-->“项目”:“All”-->“平台”-->下拉“新建”-->“新建平台”选择“x64”-->“从此处复制设置(S):”选择“Win32”-->取消勾选“创建信的解决方案平台(C)”-->“确定”；

> 6.6.2 


> 
> 
> -->“配置(C)”选择“Release”-->“平台(P)”选择“x64”-->“配置管理器(O)”-->“项目”:“All”-->“平台”-->下拉选择“x64”-->勾选“生成”-->“关闭”；


> 
> -->“配置(C)”选择“Debug”-->“平台(P)”选择“x64”-->“配置管理器(O)”-->“项目”:“All”-->“平台”-->下拉选择“x64”-->勾选“生成”-->“关闭”；



> 
> 或


> 
> 
> -->“配置管理器(O)”-->“项目上下文(选定要生成的或部署的项目配置)(R):”-->“活动解决方案配置(C)”:“Release”-->“活动解决方案平台(P)”:“x64”-->“项目”:“All”-->“平台”-->下拉选择“x64”-->勾选“生成”-->“关闭”；


> 
> -->“配置管理器(O)”-->“项目上下文(选定要生成的或部署的项目配置)(R):”-->“活动解决方案配置(C)”:“Debug”-->“活动解决方案平台(P)”:“x64”-->“项目”:“All”-->“平台”-->下拉选择“x64”-->勾选“生成”-->“关闭”；



> 6.7 步骤同5.5
6.8 完整编译：

> 
> 解决方案-->右键“批生成(T)...”-->“选定要生成的项目配置(K):”-->勾选“项目”值为“All”，“平台”值为“x64”的行-->“重新生成(R)”；










