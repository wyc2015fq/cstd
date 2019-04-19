# 2，libyuv 编译for ios - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月17日 14:27:06[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1919
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
这里有编译好的库
[https://bintray.com/yarr/ios/libyuv-ios#](https://bintray.com/yarr/ios/libyuv-ios#)
lipo -info libyuv.a 
Architectures in the fat file: libyuv.a are: armv7 i386 x86_64 arm64 
如何自己编译呢？
我找到个特别方便的方法：
git clone [https://github.com/yarrcc/libyuv-ios](https://github.com/yarrcc/libyuv-ios)
不要用root下载代码。不然xcode打开会报错。
https://yunpan.cn/cMAyU3LQruk7g 访问密码 7d0b
我发现里面有个yuv.xcodeproj，就用xcode打开了，结果一运行，就编译出来了

