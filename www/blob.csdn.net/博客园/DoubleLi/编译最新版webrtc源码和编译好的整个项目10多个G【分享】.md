# 编译最新版webrtc源码和编译好的整个项目10多个G【分享】 - DoubleLi - 博客园






# 编译最新版webrtc源码和编译好的整个项目10多个G【分享】



参考[https://webrtc.org/native-code/development/](https://webrtc.org/native-code/development/)编译最新版webrtc源码：

[Git](http://lib.csdn.net/base/git) clone [https://chromium.googlesource.com/external/webrtc](https://chromium.googlesource.com/external/webrtc)

gclient config [https://chromium.googlesource.com/external/webrtc](https://chromium.googlesource.com/external/webrtc) --name=src

set DEPOT_TOOLS_WIN_TOOLCHAIN=0
set GYP_MSVS_VERSION=2015
set GYP_GENERATORS=ninja,msvs-ninja
gclient sync
cd src
[Python](http://lib.csdn.net/base/python) webrtc/build/gyp_webrtc.py

上图哈：

![](http://img.blog.csdn.net/20160428173344624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：
1.depot_tools的使用参考：[https://bitbucket.org/chromiumembedded/cef/wiki/MasterBuildQuickStart#markdown-header-windows-setup](https://bitbucket.org/chromiumembedded/cef/wiki/MasterBuildQuickStart#markdown-header-windows-setup)
2.webrtc官方源码我copy到github上了：[https://github.com/JumpingYang001/webrtc](https://github.com/JumpingYang001/webrtc)
3.webrtc编译好的整个项目我传到百度云了：[http://pan.baidu.com/s/1nuT0MV3](http://pan.baidu.com/s/1nuT0MV3) (注：由于刚压缩好在上传中，有10多G，请稍等，等上传完成后再下载)



------------------------ **2016-9-29加入安装depot_tools说明(红色字体：python和其他工具会在运行gclinet时自己下载安装)** -----------------------------------------

Install depot_tools


|Chromium and Chromium OS use a package of scripts called [**depot_tools**](http://dev.chromium.org/developers/how-tos/depottools) to manage checkouts and code reviews.  The depot_tools package includes `gclient`, `gcl`, `git-cl`, `repo`, and others.## Installing on Linux and Mac- Confirm `git` and python are installed. git 2.2.1+ recommended. python 2.7+ recommended.- Fetch depot_tools: `$ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git`- Add `depot_tools` to your [PATH](http://www.lmgtfy.com/?q=PATH+environment):`$ export PATH=`pwd`/depot_tools:"$PATH"`- Yes, you want to put depot_tools ahead of everything else, otherwise gcl will refer to the GNU Common Lisp compiler.- You may want to add this to your `.bashrc` file or your shell's equivalent so that you don’t need to reset your $PATH manually each time you open a new shell.### Installing on Windows### PreambleChromium is mostly designed to be run using the native Windows tools and the[Msys (Git for Windows)](http://msysgit.github.io/) toolchain. Cygwin is not recommended, and likely things will fail in cryptic ways.### Instructions- Download [depot_tools.zip](https://storage.googleapis.com/chrome-infra/depot_tools.zip) and decompress it.- **Do not use drag-n-drop or copy-n-paste extract **from Explorer, this will not extract the hidden ".git" folder which is necessary for depot_tools to autoupdate itself. You can use "Extract all..." from the context menu though.- **Do not extract to a path containing spaces**. If you do, gclient will produce the error "update_depot_tools.bat was not expected at this time" or similar.- Add `depot_tools` to the **start** (not end!) of your PATH:- With Administrator access:- Control Panel > System and Security > System > Advanced system settings- Modify the PATH system variable to include depot_tools- Without Administrator access:- Control Panel > User Accounts > User Accounts > Change my environment variables- Add a PATH user variable: C:\path\to\depot_tools;%PATH%- Run `gclient`**from the cmd**** shell**. The first time it is run, it will install its own copy of various tools. If you run gclient from a non-cmd shell, it may appear to run properly, but python, and other tools may not get installed correctly (while it should work fine from a msys bash shell, you may still trip over bugs from time to time).- If you see strange errors with the file system on the first run of gclient, you may want to [disable Windows Indexing](http://tortoisesvn.tigris.org/faq.html#cantmove2).- If you see errors like "The system cannot execute the specified program", try [installing "Microsoft Visual C++ 2008 Redistributable Package"](http://code.google.com/p/chromium/issues/detail?id=75886).- If it complains that it can't find python, make sure you don't already have a .gclient file in the same directory.- After running gclient open a command prompt and type where python and confirm that the depot_tools python.bat comes ahead of any copies of python.exe. Failing to ensure this can lead to overbuilding when using gn - see [crbug.com/611087](http://crbug.com/611087)See also [Howto: depot tools](http://dev.chromium.org/developers/how-tos/depottools)------------------------2016-9-29加入安装depot_tools说明-----------------------------------------|
|----|











