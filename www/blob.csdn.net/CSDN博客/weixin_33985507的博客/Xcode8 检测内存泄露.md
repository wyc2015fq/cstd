# Xcode8 检测内存泄露 - weixin_33985507的博客 - CSDN博客
2016年06月17日 18:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
### Xcode8 检测内存泄露
Xocde8的beta版本，我最近几天用了，增强了一些功能，感觉还是特别好用的。[官方文档](https://link.jianshu.com?t=https://developer.apple.com/library/prerelease/content/documentation/DeveloperTools/Conceptual/WhatsNewXcode/introduction.html)里面有简要说明
![120158-62b0f5d4aa6993dc.png](https://upload-images.jianshu.io/upload_images/120158-62b0f5d4aa6993dc.png)
xcode8_new.png
Source Editing 主要可说的而是官方集成XCode插件开发，总算有了一个具体的规范，苹果拿来主义也是毫不含糊，在
> 
XCode的/Applications/Xcode-beta.app/Contents/PlugIns/XCDocumenterExtension.appex/Contents/Resources/VVDocumenter-Xcode-Info.plist,直接拿了猫神的VVDocument！！！
本文主要说明Xcode8 的Debuging 新功能，Xcode8 改进了ViewDebugging，能查看autolayout并提示autolayout约束的问题，这个具体用就很清楚了，主要还是看Debug Memory Graph这个功能。
#### 开启Debug Meory Graph：
- 配置Edit Scheme，如下图
![120158-1a06e314ecd994e5.png](https://upload-images.jianshu.io/upload_images/120158-1a06e314ecd994e5.png)
malloc_1.png
这里 Malloc Stack 有两个选项，一个是 All allocations,另一个如图中 Live Allocations Only所示。如果不开启这个，在最终的生成内存泄露的列表中将无法查看具体leak的函数堆栈信息。
- Xcode 8 运行一个App
- 点击ViewDebug 旁边类似分享的按钮,如下图：
![120158-51be2acd8f804442.png](https://upload-images.jianshu.io/upload_images/120158-51be2acd8f804442.png)
debug1.png
然后Xcode会生成leak列表，如下图：
![120158-6f99277ef0f7668a.png](https://upload-images.jianshu.io/upload_images/120158-6f99277ef0f7668a.png)
memory2.png
![120158-22e21dc167af1010.png](https://upload-images.jianshu.io/upload_images/120158-22e21dc167af1010.png)
memory_graphy_2.png
具体的引用次数和循环引用的引用计数图都清晰表名，点击具体函数能查看代码为什么存在内存泄露，这样大大方便开发者发现bug。
如果想要了解更多，请参看WWDC的session：[Visual Debugging with Xcode](https://link.jianshu.com?t=https://developer.apple.com/videos/play/wwdc2016/410/)
