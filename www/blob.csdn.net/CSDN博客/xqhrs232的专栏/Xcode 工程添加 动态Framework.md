# Xcode 工程添加 动态Framework - xqhrs232的专栏 - CSDN博客
2019年02月27日 17:45:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：50
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/b04911e422bb](https://www.jianshu.com/p/b04911e422bb)
相关文章
1、[Xcode添加库文件framework （转）](https://www.cnblogs.com/seer/p/5703813.html)----[https://www.cnblogs.com/seer/p/5703813.html](https://www.cnblogs.com/seer/p/5703813.html)
自己记录一下，参考：[https://juejin.im/post/5c03c8b651882546150abeef](https://juejin.im/post/5c03c8b651882546150abeef)
手动方式
在 Xcode 工程中选中 app 对应的 target，然后在 General -> Embedded Binaries 下点击加号，如图 1，在弹出的窗口选择 Add Other...，最后在 Finder 中选择你要添加的“动态 framework”，并勾选 Copy if needed 即可。需要注意的是，你不能直接在 Finder 中把 .framework 文件拖拽到 Embedded Binaries 中，否则会报错。
![](https://upload-images.jianshu.io/upload_images/1786632-eaa09c06961ac263.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
但是！这种方式看似很方便，其实有个坑是：我们上一条小集提到，一般动态二进制文件都会包含很多处理器架构，例如：i386, x86_64, armv7, armv7s, arm64 等，然后 Xcode 在编译链接时，对动态二进制文件是直接拷贝到 .ipa 包中，并不会像链接静态库那样筛选掉未用到 architecture，而苹果又不允许把包含 i386, x86_64 等模拟器架构的包上传到 App Store Connect 后台，会报错。因此，我们在打 Release 正式包时往往需要手动通过 lipo 命令或者编写脚本移除掉这些 Invalid Architectures。（除非你的开发工程只通过真机来调试，不准备在模拟器里运行，且添加的动态库刚好又不包含 i386、x86_64）
首先先查看一下framework中包含的结构：
打开mac终端，cd到静态库的位置，输入命令
```
lipo -info ./XXXX.framework/XXXX
```
这时候会显示出该framework中所包含的架构
```
Architectures in the fat file: ./XXXX.framework/XXXX are: i386 x86_64 armv7 arm64
```
执行拆分的命令，这样就把armv7、arm64架构从静态库中剥离出来了，同理其他的CPU结构
```
lipo XXXX.framework/XXXX -thin armv7 -output XXXX_armv7 
lipo XXXX.framework/XXXX -thin arm64 -output XXXX_arm64
```
执行合并的命令，把拆分出来的xxxx_armv7、xxxx_arm64合并成命名为xxxx的静态库
```
lipo -create XXXX_armv7 XXXX_arm64 -output XXXX
```
具体的拆分合并命令如下
```
mkdir ./bak
cp -r XXXX.framework ./bak
lipo XXXX.framework/XXXX -thin armv7 -output XXXX_armv7
lipo XXXX.framework/XXXX -thin arm64 -output XXXX_arm64
lipo -create XXXX_armv7 XXXX_arm64 -output XXXX
mv XXXX XXXX.framework/
```
### 使用 CocoaPods 集成
同样地，通过 CocoaPods 集成动态库时，也会在工程中自动帮我们添加一个 Shell 脚本用于做这件事，如图 2 中的 [CP] Embed Pods Frameworks，大家可以自行查阅该 [Pods-xxx-frameworks.sh](https://link.juejin.im?target=http%3A%2F%2FPods-xxx-frameworks.sh) 脚本的内容，里面有个函数 `strip_invalid_archs()` 就是用于在打包时移除无用的处理器架构。
![](https://upload-images.jianshu.io/upload_images/1786632-f48b53e42ad0f3d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
因此，我们可以把自己开发的或者他人提供的动态 framework，通过 CocoaPods 来集成到工程中：创建一个 Pods 私有 git 库（相信大家已经很熟悉了），在 git 库中添加相关动态 .frameworks 文件，然后其 Podspec 文件的写法大致如图 3 所示，最后在你的工程中 pod install 即可。
![](https://upload-images.jianshu.io/upload_images/1786632-f51a1aac2203d165.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
作者：lyking
链接：https://www.jianshu.com/p/b04911e422bb
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
