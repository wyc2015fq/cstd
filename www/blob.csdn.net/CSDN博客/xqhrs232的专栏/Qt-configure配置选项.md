# Qt-configure配置选项 - xqhrs232的专栏 - CSDN博客
2016年06月12日 18:45:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：775
原文地址::[http://blog.csdn.net/dztaopao/article/details/9243269](http://blog.csdn.net/dztaopao/article/details/9243269)
相关文章
1、[Qt交叉编译配置选项](http://blog.csdn.net/miaolxroy/article/details/17246131)----[http://blog.csdn.net/miaolxroy/article/details/17246131](http://blog.csdn.net/miaolxroy/article/details/17246131)2
2、编译qt选项用法----[http://wenku.baidu.com/link?url=u05PajcxJfKb_Ss7tfv6pixsEvBJUiyEYwpm8Seh8VgaL7Cx09CW3R7MD4mE2R1HiotqX0LOBggzj75Ig38sED3E37fI-nmFKiEQXIMf1C7](http://wenku.baidu.com/link?url=u05PajcxJfKb_Ss7tfv6pixsEvBJUiyEYwpm8Seh8VgaL7Cx09CW3R7MD4mE2R1HiotqX0LOBggzj75Ig38sED3E37fI-nmFKiEQXIMf1C7)
这个页面给出了一个简短的，当使用configure脚本或configure.exe二进制构建Qt时的各种不同的可用选项。当使用默认选项构建Qt,只需如下所示的从命令行调用configure。
在Linux、Mac OS X和Unix平台下构建Qt:
. / configure
在Windows上,运行相应的可执行:
./configure.exe
如果你想定制的构建Qt,请使用下面的表中列出的选项。如要查看完整的选项列表,可调用./configure的- help命令行选项。
跨平台的选项:
编译和链接两个Qt库，调试的打开情况|Option|Description|Note|
|----|----|----|
|-buildkey<key>|用指定的库和插件来构建Qt||
|<key>|当库加载插件,它只会加载那些有一个匹配的<key>。||
|-release|在调试关掉的情况下编译和链接Qt||
|-debug|在调试打开的情况下编译和链接Qt|默认打开|
|-debug-and-release|这个选项代表一个默认值,需要评估。如果评价成功,特点是包括。| |
|-opensource|编译和链接开源版的Qt.||
|-commercial|编译和链接的商业版的Qt。||
|-developer-build|编译和链接Qt 与 Qt 开发选项Qt（包括自动测试出口）||
|-shared|创建和使用共享Qt库。|默认打开|
|-static|创建和使用静态Qt库。||
|-ltcg|生成链接时间代码。|只适用于发布版本。|
|-no-ltcg|不生成链接时间代码。|默认打开|
|-no-fast|通过生成makefile 为Qt所配置有项目文件。|默认打开|
|-fast|快速的生成makefile为Qt只配置库和子目录。|所有其他的makefile通过运行qmake来创建。|
|-no-exceptions|不支持交叉编译平台上||
|-exceptions|支持交叉编译|默认打开|
|-no-accessibility|不支持访问编译窗口||
|-accessibility|支持访问编译窗口|默认打开|
|-no-stl|不支持STL编译||
|-stl|支持STL编译|默认打开|
|-no-sql-<driver>|完全禁用SQL <驱动>,默认情况下没有打开。||
|-qt-sql-<driver>|在Qt库中启用SQL <驱动>.||
|-plugin-sql-<driver>|Enable SQL <driver> as a plugin to be linked to at run time.|Available values for <driver>: mysql, psql, oci, odbc, tds, db2, sqlite, sqlite2, ibase. Drivers marked with a '+' during configure have been detected as available on this system.|
|-system-sqlite|Use sqlite from the operating system.||
|-no-qt3support|Disables the Qt 3 support functionality.||
|-no-opengl|Disables OpenGL functionality||
|-opengl <api>|Enable OpenGL support with specified API version.|Available values for <api>: desktop - Enable support for Desktop OpenGL (Default), es1 - Enable support for OpenGL ES Common Profile, es2 - Enable support for OpenGL ES 2.0.|
|-no-openvg|Disables OpenVG functionality|Default value.|
|-openvg|Enables OpenVG functionality|Requires EGL support, typically supplied by an OpenGL or other graphics implementation.|
|-platform <spec>|The operating system and compiler you are building on.|The default value is %QMAKESPEC%.|
|-xplatform <spec>|The operating system and compiler you are cross compiling for.|See the README file for a list of supported operating systems and compilers.|
|-system-proxies|Use system network proxies by default.||
|-no-system-proxies|Do not use system network proxies by default.||
|-qtnamespace <namespace>|Wraps all Qt library code in 'namespace name {..}||
|-qtlibinfix <infix>|Renames all Qt* libs to Qt*<infix>||
|-D <define>|Add an explicit define to the preprocessor.||
|-I <includepath>|Add an explicit include path.||
|-L <librarypath>|Add an explicit library path.||
|-l <libraryname>|Add an explicit library name, residing in a librarypath.||
|-graphicssystem <sys>|Specify which graphics system should be used.|Available values for <sys>: * raster - Software rasterizer, opengl - Using OpenGL acceleration, experimental!, openvg - Using OpenVG acceleration, experimental!|
|-help, -h, -?|Display this information.||
### Third Party Libraries
|Option|Description|Note|
|----|----|----|
|-qt-zlib|Use the zlib bundled with Qt.||
|-system-zlib|Use zlib from the operating system.|See http://www.gzip.org/zlib|
|-no-gif|Do not compile GIF reading support.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-libpng|Do not compile PNG support.||
|-qt-libpng|Use the libpng bundled with Qt.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-system-libpng|Use libpng from the operating system.|See http://www.libpng.org/pub/png|
|-no-libmng|Do not compile MNG support.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-qt-libmng|Use the libmng bundled with Qt.||
|-system-libmng|Use libmng from the operating system.|See http://www.libmng.com|
|-no-libtiff|Do not compile TIFF support.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-qt-libtiff|Use the libtiff bundled with Qt.||
|-system-libtiff|Use libtiff from the operating system.|See http://www.libtiff.org|
|-no-libjpeg|Do not compile JPEG support.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-qt-libjpeg|Use the libjpeg bundled with Qt.||
|-system-libjpeg|Use libjpeg from the operating system.|See http://www.ijg.org. This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
### Qt for Windows only:
|Option|Description|Note|
|----|----|----|
|-no-dsp|Do not generate VC++ .dsp files.||
|-dsp|Generate VC++ .dsp files, only if spec "win32-msvc".|Default value.|
|-no-vcproj|Do not generate VC++ .vcproj files.||
|-vcproj|Generate VC++ .vcproj files, only if platform "win32-msvc.net".|Default value.|
|-no-incredibuild-xge|Do not add IncrediBuild XGE distribution commands to custom build steps.||
|-incredibuild-xge|Add IncrediBuild XGE distribution commands to custom build steps. This will distribute MOC and UIC steps, and other custom buildsteps which are added to the INCREDIBUILD_XGE variable.|The IncrediBuild distribution commands are only added to Visual Studio projects. This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-plugin-manifests|Do not embed manifests in plugins.||
|-plugin-manifests|Embed manifests in plugins.|Default value.|
|-no-qmake|Do not compile qmake.||
|-qmake|Compile qmake.|Default value|
|-dont-process|Do not generate Makefiles/Project files. This will override -no-fast if specified.||
|-process|Generate Makefiles/Project files.|Default value.|
|-no-rtti|Do not compile runtime type information.||
|-rtti|Compile runtime type information.|Default value.|
|-no-mmx|Do not compile with use of MMX instructions||
|-mmx|Compile with use of MMX instructions|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-3dnow|Do not compile with use of 3DNOW instructions||
|-3dnow|Compile with use of 3DNOW instructions|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-sse|Do not compile with use of SSE instructions||
|-sse|Compile with use of SSE instructions|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-sse2|Do not compile with use of SSE2 instructions||
|-sse2|Compile with use of SSE2 instructions|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-openssl|Do not compile in OpenSSL support||
|-openssl|Compile in run-time OpenSSL support|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-openssl-linked|Compile in linked OpenSSL support||
|-no-dbus|Do not compile in D-Bus support||
|-dbus|Compile in D-Bus support and load libdbus-1 dynamically.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-dbus-linked|Compile in D-Bus support and link to libdbus-1||
|-no-phonon|Do not compile in the Phonon module||
|-phonon|Compile the Phonon module.|Phonon is built if a decent C++ compiler is used. This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-phonon-backend|Do not compile the platform-specific Phonon backend-plugin||
|-phonon-backend|Compile in the platform-specific Phonon backend-plugin|Default value.|
|-no-multimedia|Do not compile the multimedia module||
|-multimedia|Compile in multimedia module|Default value.|
|-no-audio-backend|Do not compile in the platform audio backend into [QtMultimedia](http://qt-project.org/doc/qt-4.8/qtmultimedia.html)||
|-audio-backend|Compile in the platform audio backend into [QtMultimedia](http://qt-project.org/doc/qt-4.8/qtmultimedia.html)|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-webkit|Do not compile in the WebKit module||
|-webkit|Compile in the WebKit module|WebKit is built if a decent C++ compiler is used. This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-webkit-debug|Compile in the WebKit module with debug symbols.||
|-no-script|Do not build the [QtScript](http://qt-project.org/doc/qt-4.8/qtscript.html) module.||
|-script|Build the [QtScript](http://qt-project.org/doc/qt-4.8/qtscript.html) module.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-scripttools|Do not build the [QtScriptTools](http://qt-project.org/doc/qt-4.8/qtscripttools.html) module.||
|-scripttools|Build the [QtScriptTools](http://qt-project.org/doc/qt-4.8/qtscripttools.html) module.|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-declarative|Do not build the declarative module||
|-declarative|Build the declarative module|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-declarative-debug|Do not build the declarative debugging support||
|-declarative-debug|Build the declarative debugging support|Default value.|
|-arch <arch>|Specify an architecture.|Available values for <arch>: * windows, windowsce, symbian, boundschecker, generic.|
|-no-style-<style>|Disable <style> entirely.||
|-qt-style-<style>|Enable <style> in the Qt Library.|Available styles: * windows, + windowsxp, + windowsvista, * plastique, * cleanlooks, * motif, * cde, windowsce, windowsmobile, s60|
|-no-native-gestures|Do not use native gestures on Windows 7.||
|-native-gestures|Use native gestures on Windows 7.|Default value.|
|-no-mp|Do not use multiple processors for compiling with MSVC|Default value.|
|-mp|Use multiple processors for compiling with MSVC (-MP)||
|-loadconfig <config>|Run configure with the parameters from file configure_<config>.cache.||
|-saveconfig <config>|Run configure and save the parameters in file configure_<config>.cache.||
|-redo|Run configure with the same parameters as last time.||
### Qt for Windows CE only:
|Option|Description|Note|
|----|----|----|
|-no-iwmmxt|Do not compile with use of IWMMXT instructions||
|-iwmmxt|Do compile with use of IWMMXT instructions.|This is for Qt for Windows CE on Arm only. This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-no-crt|Do not add the C runtime to default deployment rules.|Default value.|
|-qt-crt|Qt identifies C runtime during project generation||
|-crt <path>|Specify path to C runtime used for project generation.||
|-no-cetest|Do not compile Windows CE remote test application||
|-cetest|Compile Windows CE remote test application|This option denotes a default value and needs to be evaluated. If the evaluation succeeds, the feature is included.|
|-signature <file>|Use file for signing the target project||
|-phonon-wince-ds9|Enable Phonon Direct Show 9 backend for Windows CE|Default value|
### Qt for Symbian OS only:
|Option|Description|Note|
|----|----|----|
|-no-freetype|Do not compile in Freetype2 support.|Default value.|
|-qt-freetype|Use the libfreetype bundled with Qt.||
|-fpu <flags>|VFP type on ARM, supported options: softvfp(default) |vfpv2 | softvfp+vfpv2||
|-no-s60|Do not compile in S60 support.||
|-s60|Compile with support for the S60 UI Framework|Default value.|
|-no-usedeffiles|Disable the usage of DEF files.||
|-usedeffiles|Enable the usage of DEF files.||
