# webrtc所有平台下载编译步骤详细说明 - DoubleLi - 博客园






# webrtc所有平台下载编译步骤详细说明



## 1、安装depot tools


Windows：
国外下载：[https://storage.googleapis.com/chrome-infra/depot_tools.zip](https://storage.googleapis.com/chrome-infra/depot_tools.zip)
下载完把压缩包解压，然后把解压目录加入PATH环境变量
[Linux](http://lib.csdn.net/base/linux)（[Android](http://lib.csdn.net/base/android)）/Mac（[iOS](http://lib.csdn.net/base/ios)）：
安装[Git](http://lib.csdn.net/base/git)
国外：git clone [https://chromium.googlesource.com/chromium/tools/depot_tools.git](https://chromium.googlesource.com/chromium/tools/depot_tools.git)
国内：git clone [https://source.codeaurora.org/quic/lc/chromium/tools/depot_tools](https://source.codeaurora.org/quic/lc/chromium/tools/depot_tools)
把depot_tools目录加入PATH：export PATH=`pwd`/depot_tools:"$PATH"



## 2.安装依赖软件


Windows：
a、系统locale最好设置成English，就是控制面板里面的Region，看下图

![](http://img.blog.csdn.net/20160901151701606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
b、安装”Visual Studio 2015 Update 2“，其他版本都不受官方支持。
c、[操作系统](http://lib.csdn.net/base/operatingsystem)必须是Windows 7 x64及以上版本，x86操作系统都不支持。
d、安装VS2015时必须有下列组件：
•Visual C++, which will select three sub-categories including MFC
•Universal Windows Apps Development Tools > Tools
•Universal Windows Apps Development Tools > Windows 10 SDK (10.0.10586)
e、新开个cmd中运行set DEPOT_TOOLS_WIN_TOOLCHAIN=0，之后所以脚本都在这个cmd中运行
f、编译是用ninja而不是VS！

Linux：看后面

Android：
安装[Java](http://lib.csdn.net/base/java)OpenJDK：
$ sudo apt-get install openjdk-7-jdk
$ sudo update-alternatives --config javac
$ sudo update-alternatives --config [Java](http://lib.csdn.net/base/java)
$ sudo update-alternatives --config javaws
$ sudo update-alternatives --config javap
$ sudo update-alternatives --config jar
$ sudo update-alternatives --config jarsigner

Mac（IOS）：
安装最新XCode



## 3.下源码


先创建目录
mkdir webrtc-checkout
cd webrtc-checkout

Windows：
fetch --nohooks webrtc
gclient sync

Linux：
export GYP_DEFINES="OS=linux"
fetch --nohooks webrtc_android
gclient sync

cd src
./build/install-build-deps.sh

Android：
export GYP_DEFINES="OS=android"
fetch --nohooks webrtc_android
gclient sync

cd src
. build/install-build-deps-android.sh

Mac：
export GYP_DEFINES="OS=mac"
fetch --nohooks webrtc_ios
gclient sync

IOS：
export GYP_DEFINES="OS=ios"
fetch --nohooks webrtc_ios
gclient sync



## 4、生成ninja项目文件


Windows/Linux：
方式一：使用gn生成：
生成debug版ninja项目文件：gn gen out/Default
生成release版ninja项目文件：gn gen out/Default --args='is_debug=false'
清空ninja项目文件：gn clean out/Default
方式二：使用gyp生成（已过期的方式，但是目前还可以用）
[Python](http://lib.csdn.net/base/python) webrtc/build/gyp_webrtc.py

Android：
使用gn生成：
gn gen out/Default --args='target_os="android" target_cpu="arm"'
生成ARM64版：gn gen out/Default --args='target_os="android" target_cpu="arm64"'
生成32位 x86版：gn gen out/Default --args='target_os="android" target_cpu="x86"'
生成64位 x64版：gn gen out/Default --args='target_os="android" target_cpu="x64"'

Mac:
使用gn生成：
gn gen out/Debug-mac --args='target_os="mac" target_cpu="x64" is_component_build=false'

IOS：
生成ARM版：gn gen out/Debug-device-arm32 --args='target_os="ios" target_cpu="arm" is_component_build=false'
生成ARM64版：gn gen out/Debug-device-arm64 --args='target_os="ios" target_cpu="arm64" is_component_build=false'
生成32位模拟器版：gn gen out/Debug-sim32 --args='target_os="ios" target_cpu="x86" is_component_build=false'
生成64位模拟器版：gn gen out/Debug-sim64 --args='target_os="ios" target_cpu="x64" is_component_build=false'



## 5.编译源码


Windows/Linux/Android/Mac/IOS：
ninja -C out/Default

好了，这样就编译出来所有相关的库和[测试](http://lib.csdn.net/base/softwaretest)程序。









