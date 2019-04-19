# iOS 开发报错 clang: error: unable to execute command: Segmentation fault: 11----本人备注 - xqhrs232的专栏 - CSDN博客
2019年01月04日 14:10:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：103
原文地址::[https://blog.csdn.net/zhouzhoujianquan/article/details/45098567](https://blog.csdn.net/zhouzhoujianquan/article/details/45098567)
相关文章
1、clang: error: unable to execute command: Segmentation fault: 11----[https://blog.csdn.net/sepnineth/article/details/56016087](https://blog.csdn.net/sepnineth/article/details/56016087)
os开发报错 
ld: can't link with a main executable file '/Users/apple/Library/Developer/Xcode/DerivedData/LePaiDemo-ftsuwgzvjdjwzghgkuefbzvdkvey/Build/Products/Debug-iphoneos/LePaiDemo.app/LePaiDemo' for architecture armv7
clang: error: unable to execute command: Segmentation fault: 11
clang: error: linker command failed due to signal (use -v to see invocation)
解决办法 把Architectures的debug改为NO就可以
//======================================================
备注：：
1》project/targets/build settings/Architectures/build active architecture only/debug/NO
2》把工程copy到Mac os系统下再去编译就可以了！！！
