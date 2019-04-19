# 编译FFmpeg for iOS - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 20:50:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2751
所属专栏：[](https://blog.csdn.net/column/details/.html)[编解码](https://blog.csdn.net/column/details/12721.html)
2项依赖：
- 
gas-preprocessor（见附录：gas-preprocessor简介）
- yasm 1.2.0
如果要集成x264和fdk_aac，需要先编译x264和fdk_aac。
**Usage（**用root执行**）**
- To build everything:
./build-ffmpeg.sh
- To build arm64 libraries:
./build-ffmpeg.sh arm64
- To build fat libraries for armv7 and x86_64 (x86_64 是64-bit simulator 模拟器):
./build-ffmpeg.sh armv7
 x86_64
- To build fat libraries from separately built thin libraries:
./build-ffmpeg.sh lipo
You should link your app with
- libz.dylib
- libbz2.dylib
- libiconv.dylib
最终输出FFmpeg-iOS（我已改成fat-FFmpeg），可以在脚本里指定需要的平台
#ARCHS="arm64 armv7 x86_64 i386"


问题：
1，编译失败，sh中有个
ruby -e "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/master/install)](https://raw.githubusercontent.com/Homebrew/install/master/install))"
 访问不了
执行到这一句说明没有安装yasm，而安装yasm，需要用到homebrew，因此执行上面语句安装homebrew时报错了。
解决：安装yasm，先安装homebrew，参见附录：homebrew的安装与使用
2，xcrun -sdk iphoneos clang is unable to create an executable file 
C compiler test failed.
解决：去掉bitcode，如下：
CFLAGS="$CFLAGS -mios-version-min=$DEPLOYMENT_TARGET~~-fembed-bitcode~~"
3，如果集成了fdk_aac，需要添加：enable-nonfree
4，gas-preprocessor.pl的位置在usr/local/bin/，之前在/usr/bin下。不然会报GNU assembler not found gas-preprocessor
附录：
一，homebrew的安装与使用
套件管理器
安装：
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
不能用root执行

常用命令：
查找软件包
brew search wget
安装软件包
brew install wget
删除软件包
brew remove wget
查看软件包信息
brew info wget
列出软件包的依赖关系
brew deps wget
更新brew
brew update
二，gas-preprocessor简介
[https://github.com/libav/gas-preprocessor](https://github.com/libav/gas-preprocessor)
Perl script that implements a subset of the GNU as preprocessor that Apple's as doesn't
To configure Libav for iOS:./configure --enable-cross-compile --arch=arm --target-os=darwin --cc='clang -arch armv7' --sysroot=$(xcrun
 --sdk iphoneos --show-sdk-path) --cpu=cortex-a8 --enable-picIf deploying to all generations, it's recommended to do separate out-of-treebuilds for each architecture, then
lipo
together the resulting libs. Forinstance, assuming separate builds in armv6 and armv7:lipo -create -arch armv6 armv6/libavcodec/libavcodec.a -arch armv7 armv7/libavcodec/libavcodec.a
 -output universal/libavcodec.aand similar for each library. Then in XCode, make sure to build for both armv6and armv7. If you only care about one generation (since the armv6 devices aretoo slow for instance), then lipo is unnecessary of course.
还一个原始版本[https://github.com/yuvi/gas-preprocessor](https://github.com/yuvi/gas-preprocessor)
Perl script that implements a subset of the GNU as preprocessor that Apple's as doesn't
To configure ffmpeg for the iPhone 3gs and iPod touch 3g:./configure --enable-cross-compile --arch=arm --target-os=darwin
 --cc='/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc -arch armv7' --sysroot=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.0.sdk
 --cpu=cortex-a8 --enable-picTo configure ffmpeg for all other
iPhones and iPods:./configure --enable-cross-compile --arch=arm --target-os=darwin --cc='/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc -arch
armv6' --sysroot=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.0.sdk --cpu=arm1176jzf-sMake
 sure to replace the iPhone SDK version with the version that you're using.Pre-3.0 versions
aren't supported and probably won't work.If deploying to all generations, it's recommended to do separate
 out-of-treebuilds for each architecture, then lipo together the resulting libs. Forinstance, assuming separate builds in armv6 and armv7:lipo -create -arch armv6 armv6/libavcodec/libavcodec.a -arch armv7 armv7/libavcodec/libavcodec.a -output universal/libavcodec.aand
 similar for each library. Then in XCode, make sure to build for both armv6and armv7. If you only care about one generation (since the armv6 devices aretoo slow for instance), then lipo is unnecessary of course.

我的脚本:

库裁剪：支持libfdk_aac+libx264 编码和aac+h264解码：
艺搜参考：
脚本：
[https://github.com/kewlbear/FFmpeg-iOS-build-script](https://github.com/kewlbear/FFmpeg-iOS-build-script)
Homebrew：[http://linfan.info/blog/2012/02/25/homebrew-installation-and-usage/](http://linfan.info/blog/2012/02/25/homebrew-installation-and-usage/)
