# Xcode8带来的新特性和坑 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月27日 16:29:28[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4277
##### 该文章属于<简书 — 刘小壮>原创，转载请注明：
[<简书 — 刘小壮> http://www.jianshu.com/p/c1904fd8db06](http://www.jianshu.com/p/c1904fd8db06)
> 
这段时间身边发生了很多事，工作和学习也有点静不下心来。同事的离开，公司的变化，生活的需要.....想做的事太多，可时间就是这么点，诸多无奈啊。总之一句话，无论做什么事，静下心来是非常重要的。
回到正题，`Xcode8`正式版在**9月13日**已经推送给开发者下载，我也在十一回来之后，就下载了新的`Xcode`。下载之后就出现了很多编译错误，之前的插件也不能用了，但是发现`Xcode8`把好多不错的插件功能整合到自身了，感觉这点也挺不错。
每个版本`Xcode`都会带来很多新特性，`Xcode8`也不例外，这些新特性会给我们的开发带来便利，也会带来很多坑。所以今天打算写一篇文章，详细讲一下`Xcode8`给我们带来的新特性以及带来的坑，以及我使用过程中遇到的一些问题，希望能帮助到其他朋友。
## Xcode8新特性
![](http://upload-images.jianshu.io/upload_images/270478-2e36dacb520895a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Xcode8
#### Interface Builder
随着14年的**iPhone6**和6P出来之后，**iPhone**的屏幕尺寸也越来越多，屏幕适配是一个需要解决的问题，以后不一定苹果又出什么尺寸的**iPhone**呢。
在**iPhone6**和**6P**发布的同一年，苹果推出的`Xcode6`中在原有的`Auto layout`的基础上，添加了`Size Classes`新特性，通过这个新特性可以使用一个`XIB`或者`SB`文件，适配不同的屏幕以及**iPhone**和**iPad**两种设备。
在`Xcode8`中，苹果推出了更加强大的可视化编辑工具预览功能，可以在不运行`App`的情况下，预览当前`XIB`或`SB`在不同屏幕尺寸下的显示。(这个功能我记得之前`Xcode`就有，只是隐藏的比较深，苹果现在给拿到外面了)
选择一个`XIB`文件进去，点击下面红框的位置，会出现从**3.5寸-5.5寸**一系列屏幕尺寸的选项。直接点击不同屏幕尺寸，以及横竖屏选项，切换不同的屏幕显示。在**iPad**上还可以选择是否分屏，功能非常强大。
![](http://upload-images.jianshu.io/upload_images/270478-10ee84feb6d27b99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Interface Builder
在右边有一个`Vary for Traits`选项，点击这个选项就可以同时显示所有可选的屏幕样式，功能和上面图片都一样，只是显示上看起来比较多。
![](http://upload-images.jianshu.io/upload_images/270478-b5530ad9123e4536.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Interface Builder
还有一点，新创建的`XIB`控件尺寸，不再是之前**600*600**的方块了，而是默认是**6s**的长方形`XIB`文件，看起来舒服多了。
#### Target中General的变化
在`Xcode8`之前，都需要自己设置证书和描述文件。如果设置出现错误的情况下，还可以通过点击`Fix issue`来修复这个错误。但这有个问题就在于，`Fix issue`选项并不是那么好用，有的时候设置是正确的这里也提示需要`Fix issue`。
可能苹果也意识到这个问题的存在，在`Xcode8`中可以通过`Automatically manage signing`选项，让苹果为我们管理证书和配置文件，设置也都是由苹果来完成的。在`Xcode8`中新建项目，这个选项默认是被勾选的。
![](http://upload-images.jianshu.io/upload_images/270478-cf9c731bd73b5d89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Automatically manage signing
从上面图中可以看到，苹果帮我们自动管理了证书和配置文件。而且在之前的项目中，如果想要设置安装后显示在手机上的`App`名字，还需要自己到`Info.plist`文件中，修改`Display Name`字段，而现在直接在`General`中就可以做修改，这个修改和`Info.plist`是同步的。
但是，如果我想自己管理证书和描述文件呢？只需要去掉`Automatically manage signing`选项。 
![](http://upload-images.jianshu.io/upload_images/270478-e20e67a56b48ee0b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Automatically manage signing
如果自己到`Build Settings`中手动设置证书和描述文件，可以发现`Provisioning Profile`选项已经被标明为**Deprecated**，也就是苹果并不推荐手动设置。
#### Xcode插件
升级`Xcode8`之后会发现，在`Xcode8`中所有第三方插件都失效了，并且连之前菜单栏的插件选项也不存在了。在之前很多**iOS**开发者，都是通过[Alcatraz](http://alcatraz.io/)来管理插件的，现在**Alcatraz**也是不可用的。但是`Xcode8`自身也对编译器进行了升级，将一些比较好的插件功能加入到`Xcode`中，例如单行高亮显示等。
在`Xcode8`中支持了开发插件工程，并且为我们提供了一个插件模板，开发的插件可以上传到**App Store**下载。苹果这么做有一个原因在于，之前`Xcode`和插件是运行在同一个进程的，所以插件的崩溃也会导致`Xcode`崩溃。苹果现在将插件作为一个单独的应用程序，分开进程运行，不会对`Xcode`带来其他影响。
![](http://upload-images.jianshu.io/upload_images/270478-17294f7b2cf34b76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Xcode Source Editor Extension
#### Runtime Issues
在开发过程中，因为语法或明显的代码错误(例如`Retain Cycle`)，编译器可以发现并报黄色或红色警告。但是一些因为代码逻辑导致的错误，编译器并没有办法找到。例如下面的这句代码，因为代码逻辑的问题导致两个数组相互引用，都不能释放。
![](http://upload-images.jianshu.io/upload_images/270478-f01ac2add89b5680.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
数组循环引用
这时候可以通过`Xcode8`提供的`Runtime Issues`新特性，查找到运行过程中出现的问题，并通过**Graph**的方式将问题可视化的展现给开发者。
![](http://upload-images.jianshu.io/upload_images/270478-7b90ab84fab271fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Runtime Issues
#### Debug Memory Graph
在`Xcode6`中出现了`Debug View Hierarchy`新特性，可以通过其调试当前`App`的视图层级，查找**UI**相关的`bug`非常方便。在`Xcode8`中苹果为开发者提供了`Debug Memory Graph`特性，通过这个新特性，可以直接选择一个对象，查看与其相关的内存关系。
![](http://upload-images.jianshu.io/upload_images/270478-1032a96564ebd880.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Debug Memory Graph
`Debug Memory Graph`和`Runtime Issues`可以配合使用，通过`Debug Memory Graph`分析内存关系完成后，点击`Runtime Issues`可以看到已经发现的内存问题。
#### Swift 3
`Xcode8`带来了新版本的`Swift3`，新版本的`Swift`变化较大，如果旧版的`Swift`项目在`Xcode8`上编译可能会失败。对此，苹果为开发者提供了`Swift`迁移工具，听说不太好用(我没用过这个工具)。
如果不想立刻就迁移到`Swift3`，可以在`Builder Settings`中进行设置，选择`Use Legacy Swift Language Version`设置为**YES**，就可以继续使用旧版本的`Swift2.3`。
![](http://upload-images.jianshu.io/upload_images/270478-ba61ed8f2c714769.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Use Legacy Swift Language Version
#### 其他更新
- `Xcode`新版字体，`SF Mono Regular`字体。更新`Xcode`之后我比较喜欢这种字体，看起来代码非常工整。
- 被编辑的行高亮显示。之前`Xcode`有个插件就是这个功能，`Xcode8`把高亮功能集成进来了，使用起来很方便。
- 最新版的**API**文档，展示样式发生了很大的改变。
- 更方便的生成文档(就是喵神写的`VVDocumenter`)，在`Xcode8`中可以将光标放在方法上面，通过`option + command + /`快捷键生成文档注释。
## Xcode8适配
#### XIB和Storeboard适配
在`Xcode8`之前，创建一个`XIB`或`SB`文件，都是一个**600*600**的方块`XIB`文件。在`Xcode8`之后，创建的`XIB`文件默认是**6s**尺寸的大小。
但是`Xcode8`打开之前旧项目的`XIB`或`SB`文件时，会弹出下面的弹框， 这时候一般直接选择**Choose Device**即可。
![](http://upload-images.jianshu.io/upload_images/270478-fd01f67b947f1dab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Choose an initial device view
但是这样有个问题，如果`Xcode8`打开过这个`XIB`文件，并选择**Choose Device**之后。其他的`Xcode8`以下版本的编译器，将无法再打开这个文件，会报以下错误：
> 
The document “ViewController.xib” requires Xcode 8.0 or later. This version does not support documents saved in the Xcode 8 format. Open this document with Xcode 8.0 or later.
有两种方法解决这个问题：
- 你同事也升级`Xcode8`，比较推荐这种方式，应该迎接改变。
- 右击`XIB`或`SB`文件 -> `Open as` -> `Source Code`，删除**xml**文件中下面一行字段。
```xml
<capability name="documents saved in the Xcode 8 format" minToolsVersion="8.0"/>
```
#### 编译错误
升级`Xcode`之后，`Xcode8`对之前的一些修饰符和语句不兼容，会导致一些编译错误。这种错误导致的原因很多，这里大致列几条，各位还是根据自身遇到的情况做修改吧。
- 之前一些泛型相关的修饰符，`nullable`之类的有的会报错。
- `CAAnimation`及其子类，设置代理属性后，必须在`@interface()`遵守代理，否则报错，等等。
#### 权限适配
这应该算**iOS10**系统适配的范畴，最近这两个都在弄，所以就直接和`Xcode8`适配一起写出来了。
在**iOS10**之后需要在`Info.plist`中，添加新的字段获取权限，否则在**iOS10**上运行会导致崩溃。下面是一些常用的字段，如果有缺少的麻烦各位评论区补充一下。
|Key|权限|
|----|----|
|Privacy - Camera Usage Description|相机|
|Privacy - Microphone Usage Description|麦克风|
|Privacy - Photo Library Usage Description|相册|
|Privacy - Contacts Usage Description|通讯录|
|Privacy - Bluetooth Peripheral Usage Description|蓝牙|
|Privacy - Location When In Use Usage Description|定位|
|Privacy - Location Always Usage Description|后台定位|
|Privacy - Calendars Usage Description|日历|
参考资料：[developer.apple](https://developer.apple.com/library/content/documentation/General/Reference/InfoPlistKeyReference/Articles/CocoaKeys.html)
#### 推送通知
苹果的推送在之前**iOS8**和**iOS9**的时候就发生过大的更新，推送功能越来越强大。在**iOS10**之后苹果推出了`UserNotifications`框架，可以通过这个框架更好的控制推送通知，可以更新、修改锁屏页面的推送消息，可以添加图片等功能。
但是在`Xcode8`打包时可能会出现一个问题，同一份代码在用`Xcode8`打包后，并且不对代码进行修改的情况下上传App Store后，会发现打包后苹果发来了一封邮件。这封邮件大概意思是如果需要使用推送通知，需要对代码做修改，否则将不能使用推送通知。
![](http://upload-images.jianshu.io/upload_images/270478-e45ddc7d39d3ccfb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Push Notifications
这是因为在`Xcode8`之后，如果需要使用`Push Notifications`的功能，需要勾选`Capabilities` ->
`Push Notifications`为**YES**，否则进行远程推送就会有问题，并且会收到苹果发来的这封邮件。
#### 删除系统log
升级`Xcode8`之后，在调试和运行过程中，发现控制台打印了很多不认识的**log**，这些**log**是系统打印的，和开发者没关系。但是这么多**log**看着比较乱，怎么屏蔽掉呢？
> 
subsystem: com.apple.UIKit, category: HIDEventFiltered, enable_level: 0, persist_level: 0, default_ttl: 0, info_ttl: 0, debug_ttl: 0, generate_symptoms: 0, enable_oversize: 1, privacy_setting: 2, enable_private_data: 0
在`Target` -> `Edit Scheme` -> `Run` -> 
```
Arguments
```
中，添加`OS_ACTIVITY_MODE`字段，并设置为**Disable**即可。
![](http://upload-images.jianshu.io/upload_images/270478-47b70077b3443c77.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
OS_ACTIVITY_MODE
顺便提一下，这两天在设置**log**选项的时候，发现可以通过在`Arguments`中设置参数，打印出`App`加载的时长，包括整体加载时长，动态库加载时长等。
在`Environment Variables`中添加`DYLD_PRINT_STATISTICS`字段，并设置为**YES**，在控制台就会打印加载时长。
![](http://upload-images.jianshu.io/upload_images/270478-22029fbbf21aab5d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
控制台打印信息
#### awakeFromNib报警告
老项目在`Xcode8`中，有些重写`awakeFromNib`方法的地方，会报下面的错误。这是因为没有调用`super`的方法导致的，还好我平时都是调用`super`的，我代码目前还没出问题。
`Method possibly missing a [super awakeFromNib] call`
文／刘小壮（简书作者）
原文链接：http://www.jianshu.com/p/c1904fd8db06
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
