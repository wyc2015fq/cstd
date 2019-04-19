# 【不成功】iPhone toolchain 4.0 (Ubuntu 12.04 开发环境，需联网) - 三少GG - CSDN博客
2013年02月28日 00:16:03[三少GG](https://me.csdn.net/scut1135)阅读数：1901

搭建开发环境的两种主流方法：
**1.在苹果机上用xcode做开发   好处：可以使用一整套apple工具，常规路子就是这个。2.使用第三方工具toolchain   好处:可以在windows和linux下开发。   坏处:无法debug**
**iPhone toolchain 4.0 介绍**⑴ 基于 iOS SDK 4.2, cctools 782 及 gcc 4.2.1
⑵ 支持 armv6 及 armv7
⑶ clang 支持 blocks
① 下载及安装 Ubuntu 桌面版 12.04 在虚拟机 (例如 VirtualBox, VMWare)
please make sure you use gcc-4.4 and g++-4.4 to compile cctools**注****意需要使用 gcc-4.4 (默认是4.6)   参见：[https://github.com/javacom/toolchain4/wiki](https://github.com/javacom/toolchain4/wiki)**
[复制代码]()
sudo apt-get install gcc-4.4 g++-4.4 gobjc-4.4
sudo update-alternatives --remove-all gcc  
(若可能会提示没有候选，忽略之)
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 44 --slave /usr/bin/g++ g++ /usr/bin/g++-4.4 --slave /usr/bin/gcov gcov /usr/bin/gcov-4.4
② Ubuntu  更新及安装以下软件包  
[复制代码]()
- sudo apt-get update
- sudo apt-get install bison flex build-essential wget patch
- sudo apt-get install git autoconf gawk libssl-dev libxml2-dev uuid-dev
③ 下载 更新toolchain4_v2.tgz (131.99M) 解压 (115 下载 : [http://u.115.com/file/f39885f794](http://u.115.com.sixxs.org/file/f39885f794))
[复制代码]()
- cd ~
- tar xzvf ~/Downloads/toolchain4_v2.tgz
*注: toolchain4_v2.tgz 更新了支持 codesign_allocate 及 ldid*
④ 安装头文件及编译 toolchain4
[复制代码]()
- cd ~/toolchain4
- ./toolchain.sh xar
- ./toolchain.sh buildsys
- ./toolchain.sh cctools
- ./toolchain.sh llvmgcc
- ./toolchain.sh ldid
第三步会有error, 解决办法:
if you get the print_objc.c for error objc/objc_runtime.h not found  add these to disable compilation in /toolchain4/src/cctools/otool/print_objc.c
**注意：是将整个print_objc.c文件用下面的方法注释掉, 即清空fprintf_objc.h的所有内容**
#if 0
..
..#endif
There are lots of **ld warning: bad symbol version: ....** when compiling
To disable this, edit the source code of /toolchain4/src/cctools/ld64/src/MachOReaderDylib.hpp
and comment out this line 
warning("bad symbol version: %s in dylib %s ...
Then rebuild cctools **without downloading source again. (再次执行./toolchain.sh cctools时会让你选择)**
⑤ 安装 as_driver
[复制代码]()
- cd ~/toolchain4/as_driver
- make
- cd /usr/bin
- sudo mv as i686-linux-gnu-as
- sudo cp -p ~/toolchain4/as_driver/as_driver as
- sudo ln -s ~/toolchain4/pre/bin/arm-apple-darwin9-as .
- sudo mv ld i686-linux-gnu-ld
- sudo cp -p ~/toolchain4/as_driver/as_driver ld
- sudo ln -s ~/toolchain4/pre/bin/arm-apple-darwin9-ld .
安装 clang
[复制代码]()
- sudo apt-get install clang
⑥ 测试编译 iOS 4 项目 (包含测试 blocks ^)
[复制代码]()
- cd ~/toolchain4/Projects/LocalPush
- make
- make dist
- (LocalPush.ipa 就在 build/4.0 文件夹内)
编译项目时，提示#include_next <stdarg.h>不见时，![](https://img-my.csdn.net/uploads/201302/28/1361989003_4255.jpg)
替换成
**#include </home/XXX/toolchain4/sdks/iPhoneOS4.2.sdk/usr/lib/gcc/arm-apple-darwin10/4.2.1/include/stdarg.h>**绝对路径
包括后面出现的#include_next <float.h>，同样的方法解决**。（按理说应该还有更好的解决办法，暂时就这样凑合了）**
1. 此外有个不应该的错误：
make dist 出现下面的问题，
cp LocalPush-Info.plist ./build/4.0/LocalPush.app/Info.plist
export CODESIGN_ALLOCATE=~/toolchain4/pre/bin/arm-apple-darwin9-codesign_allocate; ~/toolchain4/prep/bin/ldid -S LocalPush
/bin/sh: /home/ne/toolchain4/prep/bin/ldid: not found
make: *** [dist] Error 127 
查找发现ldid位于/home/ne/toolchain4/pre/bin/ldid
而不存在prep目录，所以出现上述错误，将
Makefile中的~/toolchain4/prep/bin/ldid -S LocalPush
修改为
~/toolchain4/pre/bin/ldid -S LocalPush
继续运行make dist.
注释： 出现这个问题是因为压缩包里面的例子楼主已经修改过makefile里面的一些路径，楼主可能没测试过就发布了吧，试试用官方的文件[https://github.com/javacom/toolchain4](https://github.com/javacom/toolchain4)
2. 当出现签名失败，
util/ldid.cpp(576): _assert(2:false)
 util/ldid.cpp(581): _assert(0:WIFEXITED(status))
 Trace/breakpoint trap
是**makefile**这句
export CODESIGN_ALLOCATE=$(TOOLCHAIN)/pre/bin/arm-apple-darwin9-codesign_allocate
没有正确执行所致（不清楚原因）
后来将
export CODESIGN_ALLOCATE=/home/XXX/toolchain4/pre/bin/arm-apple-darwin9-codesign_allocate
放入.bashrc文件，**将makefile中这句去掉**，然后注销账户重新登陆后就没问题了
LocalPush.ipa 安装在越狱后的 iPhone 真机测试~
⑦ BatteryStatus.zip 真机测试 ![](http://bbs.weiphone.com.sixxs.org/images/weiphone/file/zip.gif)[BatteryStatus.zip](http://bbs.weiphone.com.sixxs.org/job.php?action=download&aid=1872531) (56 K) 下载次数:648 
BatteryStatus 是苹果的示例源代码，我汉化了及修改可在 toolchain 编译
[复制代码]()
- cd BatteryStatus
- make
- make dist
- (BatteryStatus.ipa 就在 build/4.0 文件夹内)
BatteryStatus.ipa 安装在越狱后的 iPhone 真机测试
![](http://images.weiphone.com.sixxs.org/attachments/Day_110424/39_261697_f08fa140923bb65.png)
其他源代码 :
MPPlayer (MediaPlayer.framework)  ![](http://bbs.weiphone.com.sixxs.org/images/weiphone/file/zip.gif)[MPPlayer.zip](http://bbs.weiphone.com.sixxs.org/job.php?action=download&aid=1876960) (388 K) 下载次数:426 
![](http://images.weiphone.com.sixxs.org/attachments/Day_100227/36_456283_7ce9ed48b2d91c0.jpg)
下列下载地址均失效，最新： [http://code.google.com/p/ios-devel/downloads/list](http://code.google.com/p/ios-devel/downloads/list)  建议使用4.3试下
# The pkg files should be put in /toolchain4/sdks/
If you want to **./toolchain.sh buildsys**
    Download [iPhoneSDK4_2.pkg here](http://www.2shared.com/file/aLJSoCyd/iPhoneSDK4_2.html) or[here](http://www.mediafire.com/?4abcqifvbuvz373)
If you want to **./toolchain.sh buildsys50**
     Download [iPhoneSDK5_0.pkg here](http://www.2shared.com/file/jy6OuLZu/iPhoneSDK5_0.html) or[here](http://www.mediafire.com/?n2ui80qplp73pl4)
If you want to **./toolchain.sh buildsys43**
     Download [iPhoneSDK4_3.pkg here](http://www.2shared.com/file/WLNKkDws/iPhoneSDK4_3.html) or[here](http://www.mediafire.com/?u1u3eunj1thktu7)
If you want to **./toolchain.sh build313**
     Download [iPhoneSDKHeadersAndLibs.pkg (that  is iPhoneSDK3.1.3) here](http://www.2shared.com/file/nQ0LyW2f/iPhoneSDKHeadersAndLibs.html) or [here](http://www.mediafire.com/?w0ma1eks7cign20)
    Download [MacOSX10.5.pkg here](http://www.2shared.com/file/MyujtOVe/MacOSX105.html) or[here](http://www.mediafire.com/?a36dh6d49e3at6p)
If you want to **./toolchain.sh build32**
    Download [iPhoneSDKHeadersAndLibs_32.pkg (that is iPhoneSDK3.2) here](http://www.2shared.com/file/5pueVAUR/iPhoneSDKHeadersAndLibs_32.html) or [here](http://www.mediafire.com/?48btkk3q26aqwvm)
    and [MacOSX10.5.pkg](http://www.mediafire.com/?a36dh6d49e3at6p)
Old iPhone SDKs (requires developer account login)
[iPhone SDK 3.1.3 with XCode 3.2.1 for Snow Leopard (requires 10.6.0)](http://developer.apple.com/ios/download.action?path=/iphone/iphone_sdk_3.1.3__final/iphone_sdk_3.1.3_with_xcode_3.2.1__snow_leopard__10m2003a.dmg)
[iPhone SDK 3.2 Final with Xcode 3.2.2 for Snow Leopard (requires 10.6.0)](http://developer.apple.com/ios/download.action?path=/iphone/iphone_sdk_3.2__final/xcode_3.2.2_and_iphone_sdk_3.2_final.dmg)
[Xcode 3.2.3 and iPhone SDK 4 Final for Snow Leopard (requires 10.6.2)](http://developer.apple.com/ios/download.action?path=/iphone/iphone_sdk_4__final/xcode_3.2.3_and_iphone_sdk_4__final.dmg)
[Xcode 3.2.4 and iOS SDK 4.1 for Snow Leopard (requires 10.6.4)](http://developer.apple.com/ios/download.action?path=/ios/ios_sdk_4.1__final/xcode_3.2.4_and_ios_sdk_4.1.dmg)
[Xcode 3.2.5 and iOS SDK 4.2 for Snow Leopard (requires 10.6.4)](http://developer.apple.com/ios/download.action?path=/ios/ios_sdk_4.2__final/xcode_3.2.5_and_ios_sdk_4.2_final.dmg)
[Xcode 3.2.6 and iOS SDK 4.3.1 for Snow Leopard (requires 10.6.6)](http://adcdownload.apple.com/Developer_Tools/xcode_3.2.6_and_ios_sdk_4.3__final/xcode_3.2.6_and_ios_sdk_4.3.dmg)

----------------------------------------------------------------------------华丽丽的分割线-----------------------------------------------------------------------------------
一般搭建toolchain的朋友，肯定就已经看过了google的那篇搭建文章，原地址[http://code.google.com/p/iphone-dev/wiki/Building](http://code.google.com/p/iphone-dev/wiki/Building)，有兴趣的朋友可以去围观一下。
  然后以下是我搭建toolchain遇到的问题和解决方法，向大家阐述一下。
  我们跟着google原创的一起来。
。。。[http://blog.csdn.net/califan/article/details/6250073](http://blog.csdn.net/califan/article/details/6250073)
因为google的人一直没有对这篇文章进行维护，所以他提供的方法，其实大部分是基于iphone 2.0或以前的，但是现在基本上找不着还有开发2.0的人吧。。。。
