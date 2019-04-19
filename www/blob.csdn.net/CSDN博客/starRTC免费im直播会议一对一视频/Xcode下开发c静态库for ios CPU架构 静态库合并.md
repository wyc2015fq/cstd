# Xcode下开发c静态库for ios  CPU架构 静态库合并 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月15日 10:51:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：169
新建一个Cocoa Touch Static  Library工程
1，先在工程左侧删除“工程名Tests”下的文件与文件夹（从内往外删，最后删除"工程名Tests文件夹"） :Delete->Remove Reference
 然后在Finder中删除相应文件
2，删除TARGETS下的"工程名Tests"
先删除原来的.h和.m文件 （可不删除）
然后在codec上右键New File，新建一个c文件（同时创建其头文件）
在左侧要新建一个New Group
先生成iOS Device库，再选择其它的，如iPhone 4s
如何使用
New 一个Single View Application
在工程目录下建立一个codecLib文件夹，
里面放.h和.a
Build Phases里的 Link Binary...加上.a文件 
Build Settings里搜索Search
然后在下面的2项里加上
$(PROJECT_DIR)/codecLib(如果没有的话)

[https://developer.apple.com/library/ios/technotes/iOSStaticLibraries/Introduction.html](https://developer.apple.com/library/ios/technotes/iOSStaticLibraries/Introduction.html)
On iOS, static libraries are the only supported library type
Cocoa Touch Static Library。
静态库：.a
 和 .framework
动态库：.dylib
 和 .framework
静态库
[http://www.mamicode.com/info-detail-416411.html](http://www.mamicode.com/info-detail-416411.html)
**静态库文件的版本（4种）**
- 真机-Debug版本
- 真机-Release版本
- 模拟器-Debug版本
- 模拟器-Release版本
**设备的CPU架构**
模拟器：
4s~5 : i386
5s~6plus : x86_64
真机：
3gs~4s : armv7
5~5c : armv7s （静态库只要支持了armv7，就可以跑在armv7s的架构上）
5s~6plus : arm64
可用下面命令查看静态库支持的cpu架构
lipo -info xxx.a
同时支持真机与模拟器的静态库（需要通过lipo命令将库合并）
合并好坏：
好：开发过程中既可以在真机上调试，也可以在模拟器上调试
坏：如果静态库太大，合并打包后，会非常大
合并产生新的静态库：
lipo -createDebug-iphoneos/xxx.a
 Debug-iphonesimulator/xxx.a -outputxxx.a
fat binaries指合并后的通用库（(iphoneos & iphonesimulator）
