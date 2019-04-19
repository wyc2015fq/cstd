# iOS里的动态库和静态库 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月11日 09:08:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：26标签：[iOS动态库																[iOS静态库](https://so.csdn.net/so/search/s.do?q=iOS静态库&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS动态库&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
## 介绍
- 
**动态库形式**：.dylib和.framework
- 
**静态库形式**：.a和.framework
- 
**动态库和静态库的区别**
静态库：链接时，静态库会被完整地复制到可执行文件中，被多次使用就有多份冗余拷贝（图1所示）
系统动态库：链接时不复制，程序运行时由系统动态加载到内存，供程序调用，系统只加载一次，多个程序共用，节省内存（图2所示）
![](https://upload-images.jianshu.io/upload_images/101810-24cd90dffd5c68fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
上图中的绿框表示app的可执行文件。
## **动态库的作用**
应用插件化:
> 
每一个功能点都是一个动态库，在用户想使用某个功能的时候让其从网络下载，然后手动加载动态库，实现功能的的插件化
> 
虽然技术上来说这种动态更新是可行的，但是对于AppStore上上架的app是不可以的。iOS8之后虽然可以上传含有动态库的app，但是苹果不仅需要你动态库和app的签名一致，而且苹果会在你上架的时候再经过一次AppStore的签名。所以你想在线更新动态库，首先你得有苹果APPStore私钥，而这个基本不可能。
> 
除非你的应用不需要通过AppStore上架，比如企业内部的应用，通过企业证书发布，那么就可以实现应用插件化在线更新动态库了。
共享可执行文件:
> 
在其它大部分平台上，动态库都可以用于不同应用间共享，这就大大节省了内存。从目前来看，iOS仍然不允许进程间共享动态库，即iOS上的动态库只能是私有的，因为我们仍然不能将动态库文件放置在除了自身沙盒以外的其它任何地方。
> 
不过iOS8上开放了App Extension功能，可以为一个应用创建插件，这样主app和插件之间共享动态库还是可行的。（还需了解下App Extension）
## Xcode6之后支持创建动态库工程
Xcode6之后苹果在iOS上开放了动态库。
创建：File->New->Project
![](https://upload-images.jianshu.io/upload_images/101810-7bcba1c9c25f10dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
创建
我们上面说过Framework即可以是动态库，也可以是静态库。那么我们上图中默认创建的是动态库，那么如何创建动态库呢？比如我创建的framework叫`testLib`，然后在`build setting`中设置动态库或静态库。如下图，创建framework的时候默认是`Dynamic Library`，我们可以修改为`Static Library`。
![](https://upload-images.jianshu.io/upload_images/101810-23fb2d5de62567de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
如果我们创建的framework是动态库，那么我们直接在工程里使用的时候会报错：`Reason: Image Not Found`。需要在工程的`General`里的`Embedded Binaries`添加这个动态库才能使用。
因为我们创建的这个动态库其实也不能给其他程序使用的，而你的`App Extension`和`APP`之间是需要使用这个动态库的。这个动态库可以`App Extension`和`APP`之间共用一份（App 和 Extension 的 Bundle 是共享的），因此苹果又把这种 Framework 称为 `Embedded Framework`，而我把这个动态库称为**伪动态库**。
具体创建静态库和Framework可以参考：[Xcode7创建静态库和Framework](https://link.jianshu.com?t=https://www.gitbook.com/book/xianjun/xcode7_framework_and_static_lib/details)。
## 自己创建的动态库
我们创建的动态库和系统的动态库有什么区别呢？我们创建的动态库是在我们自己应用的.app目录里面，只能自己的`App Extension`和`APP`使用。而系统的动态库是在系统目录里面，所有的程序都能使用。
可执行文件和自己创建的动态库位置：
> 
一般我们得到的iOS程序包是.ipa文件。其实就是一个压缩包，解压缩.ipa。解压缩后里面会有一个payload文件夹，文件夹里有一个.app文件，右键显示包内容，然后找到一个一般体积最大跟.app同名的文件，那个文件就是可执行文件。
而我们在模拟器上运行的时候用`NSBundle *bundel = [[NSBundle mainBundle] bundlePath];`就能得到.app的路径。可执行文件就在.app里面。
> 
而我们自己创建的动态库就在.app目录下的Framework文件夹里。
> 
下图就是测试工程`DFCUserInterface.app`的目录
![](https://upload-images.jianshu.io/upload_images/101810-3192fca129b2c4c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
我这里用了一个测试工程，即有系统的动态库(WebKit)，又有自己的动态库(DFCUserInterface)，我们可以看一下可执行文件中对动态库的链接地址。用[MachOView](https://link.jianshu.com?t=http://ofcckdrlc.bkt.clouddn.com/MachOView.app.zip)查看可执行文件。其中@rpth这个路径表示的位置可以查看[Xcode 中的链接路径问题](https://www.jianshu.com/p/cd614e080078)，而现在表示的其实就是.app下的Framework文件夹。
![](https://upload-images.jianshu.io/upload_images/101810-f6ddbdbc8c60a458.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/101810-c14139095774258a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
下图表示了静态库，自己创建的动态库和系统动态库：
![](https://upload-images.jianshu.io/upload_images/101810-561f3b45e2bbaf30.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/567/format/webp)
## 签名
系统在加载动态库时，会检查 framework 的签名，签名中必须包含 TeamIdentifier 并且 framework 和 host app 的 TeamIdentifier 必须一致。
我们在Debug测试的时候是不会报错的，在打包时如果有动态库，那么就会检查`TeamIdentifier`。
如果不一致，否则会报下面的错误：
```
Error loading /path/to/framework: dlopen(/path/to/framework, 265): no suitable image found. Did find:/path/to/framework: mmap() error 1
```
此外，如果用来打包的证书是 iOS 8 发布之前生成的，则打出的包验证的时候会没有 TeamIdentifier 这一项。这时在加载 framework 的时候会报下面的错误：
```
[deny-mmap] mapped file has no team identifier and is not a platform binary:/private/var/mobile/Containers/Bundle/Application/5D8FB2F7-1083-4564-94B2-0CB7DC75C9D1/YourAppNameHere.app/Frameworks/YourFramework.framework/YourFramework
```
可以通过 codesign 命令来验证。
```
codesign -dv /path/to/YourApp.app
或
codesign -dv /path/to/youFramework.framework
```
如果证书太旧，输出的结果如下：
```
Executable=/path/to/YourApp.app/YourApp
Identifier=com.company.yourapp
Format=bundle with Mach-O thin (armv7)
CodeDirectory v=20100 size=221748 flags=0x0(none) hashes=11079+5 location=embedded
Signature size=4321
Signed Time=2015年10月21日 上午10:18:37
Info.plist entries=42
TeamIdentifier=not set
Sealed Resources version=2 rules=12 files=2451
Internal requirements count=1 size=188
```
注意其中的 TeamIdentifier=not set。
我们在用`cocoapods`的`use_framework!`的时候生成的动态库也可以用`codesign -dv /path/to/youFramework.framework`查看到`TeamIdentifier=not set`。关于动态库的签名`TeamIdentifier`等之前没接触过，可以再去查看一下资料。
## 关于Framework
- **framework为什么既是静态库又是动态库？**
> 
系统的.framework是动态库，我们自己建立的.framework一般都是静态库。但是现在你用xcode创建Framework的时候默认是动态库，一般打包成SDK给别人用的话都使用的是静态库，可以修改`Build Settings`的`Mach-O Type`为`Static Library`。
- **什么是framework**
> 
Framework是Cocoa/Cocoa Touch程序中使用的一种资源打包方式，可以将代码文件、头文件、资源文件、说明文档等集中在一起，方便开发者使用。一般如果是静态Framework的话，资源打包进Framework是读取不了的。静态Framework和.a文件都是编译进可执行文件里面的。只有动态Framework能在.app下面的Framework文件夹下看到，并读取.framework里的资源文件。
> 
Cocoa/Cocoa Touch开发框架本身提供了大量的Framework，比如Foundation.framework/UIKit.framework/AppKit.framework等。需要注意的是，这些framework无一例外都是动态库。
> 
平时我们用的第三方SDK的framework都是静态库，真正的动态库是上不了AppStore的(iOS8之后能上AppStore，因为有个App Extension，需要动态库支持)。
### 创建静态Framework
**1.选择Framework**
![](https://upload-images.jianshu.io/upload_images/101810-7bcba1c9c25f10dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
创建
**2.选择为静态库**
![](https://upload-images.jianshu.io/upload_images/101810-23fb2d5de62567de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
**3.生成对应版本的静态库**
静态库的版本（4种）
- 真机-Debug版本
- 真机-Release版本
- 模拟器-Debug版本
- 模拟器-Release版本
这里debug或release是否生成符号表，是否对代码优化等可以在[如何加快编译速度](https://link.jianshu.com?t=https://www.zybuluo.com/qidiandasheng/note/587124)查看。
我们选择Release版本。编译模拟器和真机的所有CPU架构。
![](https://upload-images.jianshu.io/upload_images/101810-4a904cd260398739.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/101810-63a5f02380e88767.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
然后选择模拟器或者`Generic iOS Device`运行编译就会生成对应版本的`Framework`了。
![](https://upload-images.jianshu.io/upload_images/101810-ed90f2a51b0a203a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/101810-d5e0f456000bb54a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
**4.合成包含真机和模拟器的Framework**
终端cd到`Products`，然后执行以下代码，就会在`Products`目录下生成新的包含两种的执行文件，然后复制到任何一个`testLib.framework`里替换掉旧的`testLib`就可以了。
```
lipo -create Release-iphoneos/testLib.framework/testLib  Release-iphonesimulator/testLib.framework/testLib  -output testLib
```
或者在工程的`Build Phases`里添加以下脚本，真机和模拟器都Build一遍之后就会在工程目录下生成`Products`文件夹，里面就是合并之后的Framework。
```
if [ "${ACTION}" = "build" ]
then
INSTALL_DIR=${SRCROOT}/Products/${PROJECT_NAME}.framework
DEVICE_DIR=${BUILD_ROOT}/${CONFIGURATION}-iphoneos/${PROJECT_NAME}.framework
SIMULATOR_DIR=${BUILD_ROOT}/${CONFIGURATION}-iphonesimulator/${PROJECT_NAME}.framework
if [ -d "${INSTALL_DIR}" ]
then
rm -rf "${INSTALL_DIR}"
fi
mkdir -p "${INSTALL_DIR}"
cp -R "${DEVICE_DIR}/" "${INSTALL_DIR}/"
#ditto "${DEVICE_DIR}/Headers" "${INSTALL_DIR}/Headers"
lipo -create "${DEVICE_DIR}/${PROJECT_NAME}" "${SIMULATOR_DIR}/${PROJECT_NAME}" -output "${INSTALL_DIR}/${PROJECT_NAME}"
#open "${DEVICE_DIR}"
#open "${SRCROOT}/Products"
fi
```
### Framework目录
![](https://upload-images.jianshu.io/upload_images/101810-621181580415cc07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/422/format/webp)
- 
Headers
	表示暴露的头文件，一般都会有一个和Framework同名的.h文件，你在创建Framework的时候文件夹里也会默认生成这样一个文件。有这个和Framework同名的.h文件@import导入库的时候编译器才能找到这个库（@import导入头文件可参考[iOS里的导入头文件](https://link.jianshu.com?t=https://www.zybuluo.com/qidiandasheng/note/602118)）。
- 
info.plist
	主要就是这个Framework的一些配置信息。
- 
Modules
	这个文件夹里有个`module.modulemap`文件，我们看到这里面有这样一句`umbrella header "testLib.h"`，umbrella有保护伞、庇护的意思。
	也就是说`Headers`中暴露的`testLib.h`文件被放在umbrella雨伞下保护起来了，所以我们需要将其他的所有需要暴露的.h文件放到`testLib.h`文件中保护起来，不然会出现警告。`@import`的时候也只能找到umbrella雨伞下保护起来的.h文件。
![](https://upload-images.jianshu.io/upload_images/101810-b75dd180ed68209e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/828/format/webp)
- 
二进制文件
	这个就是你源码编译而成的二进制文件，主要的执行代码就在这个里面。
- 
.bundle文件
	如果我们在`Build Phases -> Copy Bundle Resources`里加入.bundle文件，那么创建出来的.Framework里就会有这个.bundle的资源文件夹。
### Framework的资源文件
**CocoaPods如何生成Framework的资源文件**
> 
我们能看到用cocoapods创建Framework的时候，Framework里面有一个.bundle文件，跟Framework同级目录里也有一个.bundle文件。这两个文件其实是一样的。
> 
那这两个.bundle是怎么来的呢？我们能看到用`use_frameworks!`生成的pod里面，pods这个PROJECT下面会为每一个pod生成一个`target`，比如我有一个pod叫做`testLib`，那么就会有一个叫`testLib`的`target`，最后这个target生成的就是`testLib.framework`。
那么如果这个pod有资源文件的话，就会有一个叫`testLib-bundleName`的`target`，最后这个target生成的就是`bundleName.bundle`。
**上面创建静态Framework例子里生成资源文件**
> 
在`testLib`的`target`的`Build Phases -> Copy Bundle Resources`里加入这个这个.bundle，在Framework里面就会生成这样一个bundle。
在`testLib`的`target`的`Build Phases -> Target Dependencies`里加入这个target：`testLib-bundleName`，就会在Framework的同级目录里生成这样一个bundle。
**静态Framework里不需要加入资源文件**
> 
一般如果是静态Framework的话，资源打包进Framework是读取不了的。静态Framework和.a文件都是编译进可执行文件里面的。只有动态Framework能在.app的Framework文件夹下看到，并读取.framework里的资源文件。
> 
你可以用`NSBundle *bundel = [[NSBundle mainBundle] bundlePath];`得到.app目录，如果是动态库你能在Framework目录下看到这个动态库以及动态库里面资源文件。然后你只要用`NSBundle *bundle = [NSBundle bundleForClass:<#ClassFromFramework#>];`得到这个动态库的路径就能读取到里面的资源了。
但是如果是静态库的话，因为编译进了可执行文件里面，你也就没办法读到这个静态库了，你能看到.app下的Framework目录为空。
[在framework或子工程中使用xib](https://link.jianshu.com?t=https://blog.cnbluebox.com/blog/2014/12/08/xib-in-frameworks/)
### 问题
- 
如果静态库中有category类，则在使用静态库的项目配置中【Other Linker Flags】需要添加参数【-ObjC]或者【-all_load】。
- 
如果使用framework的使用出现【Umbrella header for module 'XXXX' does not include header 'XXXXX.h'】,是因为错把xxxxx.h拖到了public中。
- 
如果出现【dyld: Library not loaded:XXXXXX】，是因为打包的framework版本太高。比如打包framework时，选择的是iOS 9.0，而实际的工程环境是iOS 8开始的。需要到`iOS Deployment Target`设置对应版本。
- 
如果创建的framework类中使用了.dylib或者.tbd，首先需要在实际项目中导入.dylib或者.tbd动态库，然后需要设置【Allow Non-modular Includes ....】为YES，否则会报错"Include of non-modular header inside framework module"。
- 
有时候我们会发现在使用的时候加载不了动态`Framework`里的资源文件，其实是加载方式不对，比如用pod的时候使用的是`use_frameworks!`，那么资源是在`Framework`里面的，需要使用以下代码加载（具体可参考[给pod添加资源文件](https://link.jianshu.com?t=https://www.zybuluo.com/qidiandasheng/note/392639#%E7%BB%99pod%E6%B7%BB%E5%8A%A0%E8%B5%84%E6%BA%90%E6%96%87%E4%BB%B6)）：
```
NSBundle *bundle = [NSBundle bundleForClass:<#ClassFromFramework#>];
[UIImage imageWithContentsOfFile:[bundle pathForResource:@"imageName@2x"(@"bundleName.bundle/imageName@2x") ofType:@"png"]];
```
## Swift 支持
跟着 iOS8 / Xcode 6 同时发布的还有 Swift。如果要在项目中使用外部的代码，可选的方式只有两种，一种是把代码拷贝到工程中，另一种是用动态 Framework。使用静态库是不支持的。
造成这个问题的原因主要是 Swift 的运行库没有被包含在 iOS 系统中，而是会打包进 App 中（这也是造成 Swift App 体积大的原因），静态库会导致最终的目标程序中包含重复的运行库（这是苹果[自家的解释](https://link.jianshu.com?t=https://github.com/ksm/SwiftInFlux#static-libraries)）。同时拷贝 Runtime 这种做法也会导致在纯 ObjC 的项目中使用 Swift 库出现问题。苹果声称等到 Swift 的 Runtime 稳定之后会被加入到系统当中，到时候这个限制就会被去除了（参考[这个问题](https://link.jianshu.com?t=https://stackoverflow.com/questions/25020783/how-to-distribute-swift-library-without-exposing-the-source-code)的问题描述，也是来自苹果自家文档）。
## CocoaPods 的做法
在纯 ObjC 的项目中，CocoaPods 使用编译静态库 .a 方法将代码集成到项目中。在 Pods 项目中的每个 target 都对应这一个 Pod 的静态库。
当不想发布代码的时候，也可以使用 Framework 发布 Pod，CocoaPods 提供了 `vendored_framework` 选项来使用第三方 Framework。
对于 Swift 项目，CocoaPods 提供了动态 Framework 的支持。通过 `use_frameworks!` 选项控制。对于 Swift 写的库来说，想通过 CocoaPods 引入工程，必须加入 `use_frameworks!` 选项。
### 关于 use_frameworks!
在使用`CocoaPods`的时候在`Podfile`里加入`use_frameworks!` ，那么你在编译的时候就会默认帮你生成动态库，我们能看到每个源码Pod都会在Pods工程下面生成一个对应的动态库Framework的`target`，我们能在这个`target`的`Build Settings -> Mach-O Type`看到默认设置是`Dynamic Library`。也就是会生成一个动态Framework，我们能在`Products`下面看到每一个Pod对应生成的动态库。
这些生成的动态库将链接到主项目给主工程使用，但是我们上面说过动态库需要在主工程target的`General -> Embedded Binaries`中添加才能使用，而我们并没有在`Embedded Binaries`中看到这些动态库。那这是怎么回事呢，其实是`cocoapods`已经执行了脚本把这些动态库嵌入到了.app的Framework目录下，相当于在`Embedded Binaries`加入了这些动态库。我们能在主工程target的`Build Phase -> Embed Pods Frameworks`里看到执行的脚本。
所以Pod默认是生成动态库，然后嵌入到.app下面的Framework文件夹里。我们去Pods工程的target里把`Build Settings -> Mach-O Type`设置为`Static Library`。那么生成的就是静态库，但是`cocoapods`也会把它嵌入到.app的Framework目录下，而因为它是静态库，所以会报错：`unrecognized selector sent to instanceunrecognized selector sent to instance 。`
## 参考
[创建一个 iOS Framework 项目](https://link.jianshu.com?t=http://www.samirchen.com/create-a-framework/)
[Xcode7创建静态库和Framework](https://link.jianshu.com?t=https://www.gitbook.com/book/xianjun/xcode7_framework_and_static_lib/details)
[iOS 静态库开发](https://www.jianshu.com/p/8f5b9855efb8)
[静态库与动态库的使用](https://link.jianshu.com?t=https://www.gitbook.com/book/leon_lizi/-framework-/details)
[iOS 静态库，动态库与 Framework](https://link.jianshu.com?t=https://skyline75489.github.io/post/2015-8-14_ios_static_dynamic_framework_learning.html)
[签名](https://link.jianshu.com?t=http://nixwang.com/2015/11/09/ios-dynamic-update/)
作者：齐滇大圣
链接：https://www.jianshu.com/p/42891fb90304
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
