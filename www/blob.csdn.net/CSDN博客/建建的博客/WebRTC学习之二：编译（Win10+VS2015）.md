# WebRTC学习之二：编译（Win10+VS2015） - 建建的博客 - CSDN博客
2017年12月13日 15:54:07[纪建](https://me.csdn.net/u013898698)阅读数：609
一.准备
1.操作系统
Win7 64位及以上，必须是64位的。我用的Win10，64位。
2.VS版本
Visual Studio 2015 Update 3及以上。我用的Visual Studio 2015 Update 3。
[http://download.microsoft.com/download/7/c/f/7cf151c3-b735-4e35-a1bb-9a48224f4a95/vs2015.3.ent_chs.iso](http://download.microsoft.com/download/7/c/f/7cf151c3-b735-4e35-a1bb-9a48224f4a95/vs2015.3.ent_chs.iso)
3.Python
编译过程中需要，我用的2.7.9版本，需添加到Path环境变量。
[https://www.python.org/ftp/python/2.7.9/python-2.7.9.amd64.msi](https://www.python.org/ftp/python/2.7.9/python-2.7.9.amd64.msi)
4.Microsoft DirectX SDK (June 2010)
如果安装出错，参考：[安装DirectX SDK时出现Error
 Code:s1023 的解决方案](http://blog.csdn.net/caoshangpa/article/details/53433052)
以上环境是2016年3月11号以后的WebRTC版本必须的。
二.下载
WebRTC需要翻墙才能下载，下载方法略，这里分享一个2016年6月22号的版本，只含源码，比较小。
[http://pan.baidu.com/s/1bpHvsZX](http://pan.baidu.com/s/1bpHvsZX)
我分享的这个版本已经带all.sln了，并且移除了test、unittest和demo工程。打开all.sln直接编译就行。
如果只是需要WebRTC静态库，看到这里就可以了。
三.编译
毕竟test、unittest和demo工程是学习WebRTC最好的资料，如果想编译它们，得自己重新生成all.sln。
1.VS工程文件生成
将源码解压，我解压到了D:\webrtc20160622
![](https://img-blog.csdn.net/20161126203735774)
输入上图指令就能在源码目录中生成all.sln，包含所有的工程。
注意：
a）要用 set GYP_GENERATORS=msvs， 不要用set GYP_GENERATORS=ninja,msvs-ninja，否则会报大量的编译错误。
b）除了我分享的版本，网上下载的各种未生成工程文件的源码也可以这个方法进行工程文件生成。
c）在编译test、unittest和demo工程时，会遇到一些问题，如下所示。
2.一些错误处理
问题1：error C2220: 警告被视为错误 - 没有生成“object”文件
原因：该文件的代码页为英文，而我们系统中的代码页为中文。
解决方法：
双击该错误打开对应文件，选择“文件”菜单中的“高级保存选项”菜单项，如下图所示。
![](https://img-blog.csdn.net/20170108183600446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后将编码方式选择为中文，如下图所示。
![](https://img-blog.csdn.net/20170108183850184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题2：LINK : fatal error LNK1104: 无法打开文件“D:\webrtc_test\build\Debug\lib\gtest_prod.lib”
原因：gtest_prod这个工程没有导出类，所以其就不生成lib。
解决方法：
右键该工程，添加一个类，类名随便取，比如说Test123，然后将该类头文件改成如下所示。
Test123.h
[cpp][view
 plain](http://blog.csdn.net/caoshangpa/article/details/53353681#)[copy](http://blog.csdn.net/caoshangpa/article/details/53353681#)
- #pragma once
- extern"C"__declspec(dllexport) class Test123  
- {  
- public:  
-     Test123();  
-     ~Test123();  
- };  
Test123.cpp
[cpp][view
 plain](http://blog.csdn.net/caoshangpa/article/details/53353681#)[copy](http://blog.csdn.net/caoshangpa/article/details/53353681#)
- #include "Test123.h"
- 
- Test123::Test123()  
- {  
- }  
- 
- Test123::~Test123()  
- {  
- }  
![](https://img-blog.csdn.net/20170108184555355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
需要注意的是，需要将项目类型选择为静态库，如下图所示。
![](https://img-blog.csdn.net/20170108191350654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果其他项目还提示缺少gtest_prod.lib，将其拷贝到指定位置。
问题3：LINK : fatal error LNK1181: 无法打开输入文件“D:\webrtc_test\build\Release\lib\system_wrappers_default.lib”
原因：同问题2
解决方法：同问题2，这里类名为Test456
![](https://img-blog.csdn.net/20170108191516390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题4：error MSB3721: 命令“call python "..\..\tools\isolate_driver.py" "check" "--isolated" "..\..\build\Release\peerconnection_unittests.isolated" "--isolate" "peerconnection_unittests.isolate" "--path-variable"
 "DEPTH" "..\.." "--path-variable" "PRODUCT_DIR" "..\..\build\Release\ " "--config-variable" "CONFIGURATION_NAME=Release" "--config-variable" "OS=win" "--config-variable" "asan=0" "--config-variable" "branding=Chromium" "--config-variable" "chromeos=0" "--config-variable"
 "component=static_library" "--config-variable" "disable_nacl=0" "--config-variable" "enable_pepper_cdms=1" "--config-variable" "enable_plugins=1" "--config-variable" "fastbuild=0" "--config-variable" "icu_use_data_file_flag=1" "--config-variable" "internal_gles2_conform_tests=0"
 "--config-variable" "kasko=0" "--config-variable" "lsan=0" "--config-variable" "msan=0" "--config-variable" "target_arch=ia32" "--config-variable" "tsan=0" "--config-variable" "use_custom_libcxx=0" "--config-variable" "use_instrumented_libraries=0" "--config-variable"
 "use_prebuilt_instrumented_libraries=0" "--config-variable" "use_ozone=0" "--config-variable" "use_x11=0" "--config-variable" "v8_use_external_startup_data=1" "--config-variable" "msvs_version=2015"”已退出，返回代码为 1。
原因：未知
解决方法：双击该错误，定位到所在文件的所在行，将该行删除。如下图所示，删除光标所在的行。
![](https://img-blog.csdn.net/20170108192043397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fvc2hhbmdwYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考链接：[http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=26000296&id=5746543](http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=26000296&id=5746543)
参考链接：[https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md](https://chromium.googlesource.com/chromium/src/+/master/docs/windows_build_instructions.md)
![](https://img-blog.csdn.net/20161126222158001)
