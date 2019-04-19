# Xcode 调试方法总结 - xqhrs232的专栏 - CSDN博客
2019年01月04日 16:29:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：94
原文地址::[https://www.cnblogs.com/holyday/p/7987314.html](https://www.cnblogs.com/holyday/p/7987314.html)
相关文章
1、Xcode调试方法汇总----[https://www.jianshu.com/p/98a776e54bbe](https://www.jianshu.com/p/98a776e54bbe)
编写代码过程中出现错误、异常是不可避免的。通常我们都需要进行大量的调试去寻找、解决问题。这时，熟练掌握调试技巧将很大程度上的提高工作效率。接下来就说说开发过程中Xcode的调试方法。
**1. Enable NSZombie Objects (开启僵尸对象)。**
这个技巧主要用来追终重复释放的问题。个人认为，ARC推出以来。项目的基本是基于ARC环境。不用开发者主动去调用release去释放对象，所以不用太在意这个方法。这里就不多做介绍了。想了解该方法的同学请 [坐飞机](http://www.xuebuyuan.com/2174541.html)
**2. 断点调试（全局断点、条件断点）**
一、全局断点：
```
NSArray *aa = @[@2,@4];
NSLog(@"%@",aa[3]);
```
- 1
- 2
这两行代码，没有添加全局断点时，运行crash，直接就跳到了mian函数，如下图：
![这里写图片描述](https://img-blog.csdn.net/20160323102036348)
接下来添加全局断点，方法如下图：
![添加全局断点的方法](https://img-blog.csdn.net/20160323100932412)
添加之后运行，奔溃后，程序停留在了crash那行代码。
![这里写图片描述](https://img-blog.csdn.net/20160323102513105)
是不是很方便，很省事。哈哈！（ps 不过有的crash，这种方式定位不到）
二、条件断点：设置断点触发的条件，方便开发者对特定情况进行调试 
如下图： 
在for循环中添加一个断点。右击断点选择”Edit BreakPoint”,然后设置断点触发条件。 
![这里写图片描述](https://img-blog.csdn.net/20160323161603299)
这个例子当 “i==5”时，断点触发，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20160323162300349)
**3. Static Analyzer （静态分析）**
Static Analyzer主要用于分析内存，避免内存泄漏。主要对以下情况进行分析。 
未使用的实例变量、未初始化的实例变量、类型不兼容、无法达到的路径、引用空指针 
使用：command + shift +B，如下图就能轻松找到可能内存泄漏的代码，然后我们根据代码环境进行修复就可以了（ps：有的内存泄漏可能检测不出来，还是需要我们在写代码时对内存这块多留点心。）
![这里写图片描述](https://img-blog.csdn.net/20160323173156956)
**4. LLDB调试器**
这个方法是我今天主推的方法。比较高级，也更加灵活、方便。 
随着Xcode5，LLDB调试器已经取代了GDB，成为了Xcode工程中默认的调试器。其实Xcode已经帮我们完成了大部分工作，而且很多东西也可以在Xcode中直接看到。所以这里我们只列举常用的命令。 
**打印**：p,print的缩写：该命令如果打印的是简单类型则会列出简单类型的的类型和值，如果是对象会打印出对象的地址。 
po,print Object 的缩写，用于输出OC对象 
如下如，当运行到断点处时，控制台就会出现LLDB的调试命令行。我们只需在这里进行调试。 
![这里写图片描述](https://img-blog.csdn.net/20160323175410172)
**expr**:expression的缩写，可以在调试时动态执行指定表达式，并将结果打印出来。常用于在调试过程中修改变量的值。 
如上图，你在控制台输入  
`expr a=2`
你就能看到 
`(NSInteger) $11 = 2`
这是a的值就被动态改成了2 
除此之外，还可以使用这个命令生成一个新的对象，如： 
`expr int $b = 0`
`p $b` 这条命令用于输出新申明对象的值（注意要加$）
**image**: image命令可用于寻址，有多个组合命令，在控制台输入`help image`可查看image的用法。比较实用的用法是用于寻找栈地址对应的代码位置，下面我们来举个例子：
```
NSArray *array = @[@1,@2];
NSLog(@"%@",array[2]);
```
- 1
- 2
这段代码很明显会crash，运行之后抛出下面的异常
```
2016-03-23 22:26:11.014 Test[3631:136626] *** Terminating app due to uncaught exception 'NSRangeException', reason: '*** -[__NSArrayI objectAtIndex:]: index 2 beyond bounds [0 .. 1]'
*** First throw call stack:
(
    0   CoreFoundation                      0x0000000104f28f45 __exceptionPreprocess + 165
    1   libobjc.A.dylib                     0x00000001049a2deb objc_exception_throw + 48
    2   CoreFoundation                      0x0000000104e17b14 -[__NSArrayI objectAtIndex:] + 164
    3   Test                                0x00000001044a5829 -[ViewController viewDidLoad] + 265
    4   UIKit                               0x0000000105467cc4 -[UIViewController loadViewIfRequired] + 1198
    5   UIKit                               0x0000000105468013 -[UIViewController view] + 27
    6   UIKit                               0x000000010534151c -[UIWindow addRootViewControllerViewIfPossible] + 61
    7   UIKit                               0x0000000105341c05 -[UIWindow _setHidden:forced:] + 282
    8   UIKit                               0x00000001053534a5 -[UIWindow makeKeyAndVisible] + 42
    9   UIKit                               0x00000001052cd396 -[UIApplication _callInitializationDelegatesForMainScene:transitionContext:] + 4131
    10  UIKit                               0x00000001052d39c3 -[UIApplication _runWithMainScene:transitionContext:completion:] + 1750
    11  UIKit                               0x00000001052d0ba3 -[UIApplication workspaceDidEndTransaction:] + 188
    12  FrontBoardServices                  0x0000000107c83784 -[FBSSerialQueue _performNext] + 192
    13  FrontBoardServices                  0x0000000107c83af2 -[FBSSerialQueue _performNextFromRunLoopSource] + 45
    14  CoreFoundation                      0x0000000104e55011 __CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE0_PERFORM_FUNCTION__ + 17
    15  CoreFoundation                      0x0000000104e4af3c __CFRunLoopDoSources0 + 556
    16  CoreFoundation                      0x0000000104e4a3f3 __CFRunLoopRun + 867
    17  CoreFoundation                      0x0000000104e49e08 CFRunLoopRunSpecific + 488
    18  UIKit                               0x00000001052d04f5 -[UIApplication _run] + 402
    19  UIKit                               0x00000001052d530d UIApplicationMain + 171
    20  Test                                0x00000001044a5baf main + 111
    21  libdyld.dylib                       0x000000010764c92d start + 1
    22  ???                                 0x0000000000000001 0x0 + 1
)
libc++abi.dylib: terminating with uncaught exception of type NSException
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
现在我怀疑出错的地址是`0x00000001044a5829`（可根据执行文件名或最小的栈地址判断）为进一步精确定位我们可输入以下命令`image lookup --address 0x00000001044a5829`
命令执行后返回结果如下：
```
Address: Test[0x0000000100001829] (Test.__TEXT.__text + 265)
Summary: Test`-[ViewController viewDidLoad] + 265 at ViewController.m:21
```
- 1
- 2
由此，我们可以看出出错的地方是ViewController.m文件的第21行。 
我们还可以使用image lookup命令查看具体的类,如下：
```
(lldb) image lookup --type UIView
Best match found in /Users/jamalping/Library/Developer/Xcode/DerivedData/Test-gviuudbzlyhssmanjxpwhchdbscz/Build/Products/Debug-iphonesimulator/Test.app/Test:
id = {0x00001e8d}, name = "UIView", byte-size = 8, decl = UIView.h:144, clang_type = "@interface UIView : UIResponder
@property ( getter = isUserInteractionEnabled,setter = setUserInteractionEnabled:,assign,readwrite,nonatomic ) BOOL userInteractionEnabled;
@property ( getter = tag,setter = setTag:,assign,readwrite,nonatomic ) NSInteger tag;
@property ( readonly,getter = layer,setter = <null selector>,nonatomic ) CALayer * layer;
@property ( readonly,getter = isFocused,setter = <null selector>,nonatomic ) BOOL focused;
@property ( getter = semanticContentAttribute,setter = setSemanticContentAttribute:,assign,readwrite,nonatomic ) UISemanticContentAttribute semanticContentAttribute;
@end
"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
**call**
call：即调用，如我们在`viewDidLoad：` 设置一个断点，在程序中断的时候输入`call self.view.backgroudColor = [UIColo redColor]`继续运行程序，view就变成红色了，在调试的时候灵活运用call命令可以达到事半功倍的效果。
最近一直没有更新简书是因为在开发和测试阶段，有任务，没有进行学习，不过在做任务的时候也遇到了一些技术点，在这里总结一下。
      今天我们来谈一谈**�Xcode调试的技巧。**�就像玩游戏，有些玩家他们知道怎么操作，会放技能会走路，但是他们不知道买装备，玩了一局下来，鞋子小刀都没有买，这样行走江湖很危险啊！所以我们出门要把装配佩戴好，学会装备自己才是王道！
      本文将简书和各大博客上边涉及该方面的内容进行汇总，并试图进行全方位的总结，如果有什么不足之处还希望大家给提出来，我会进行补充和修改。现在我们开始吧。
       总体看来，关于调试的方法包括以下几个：**�日志输出&LLDB、断点、性能、一些小技巧**等几个大的方面。我们一一进行学习和总结。
**尝试接受新鲜事物和方法，方法都是熟能生巧的，各种方法综合运用，用好了会事半功倍。**
1、**�日志输出**&LLDB
       关于日志输出，我们最先想到的是NSLog，但是弊端在于我们需要在想要打印的位置添加NSLog代码并重新运行项目，这样耽误时间，所以我们平时用的比较多的是打断点，然后po一下。这个“po”就是LLDB里面的一句命令。
       那么，什么是**LLDB**呢：它是一个有着 REPL 的特性和 C++ ,Python 插件的开源调试器。LLDB 绑定在 Xcode 内部，存在于主窗口底部的控制台中。调试器允许你在程序运行的特定时暂停它，你可以查看变量的值，执行自定的指令，并且按照你所认为合适的步骤来操作程序的进展。我们可以简单的理解成它是一个**调试器**。
**    （1）LLDB命令行**
       像下图中，我们打断点后，控制台右边里面会出现一个“lldb”，我们平时不怎么关注它，但我们一直在使用它。
![](https://upload-images.jianshu.io/upload_images/1440398-e92f26ca5680c7a9.png)
      <1>  **help命令**
      断点的时候，我们在控制台右边lldb后边输入一个“help”，然后敲回车，就会看到所有关于lldb的命令以及各自的介绍，如下图：
![](https://upload-images.jianshu.io/upload_images/1440398-81f01a068ac674f1.png)
![](https://upload-images.jianshu.io/upload_images/1440398-442ee1805a180595.png)
      <2>  **print命令**
       print很好理解，就是打印，使用过程中我们可以直接用p来代替print。
       <3>  **expression命令**
        该命令可以**改变程序实际参数的值**，目的是方便了调试：不用重新运行项目。例如下图中，我们简单的做一个测试，令苹果=1，橘子=2，all应该=3，在断点过程中，我们用expression命令修改了橘子的值，令橘子=5，结果再打印all的时候，all=6（亲测好使）。使用过程中我们可以直接用 expr来代替expression。
![](https://upload-images.jianshu.io/upload_images/1440398-1566b6a669ade87f.png)
        这里我们注意到一个“$9”，这里的9是我们使用lldb命令的次数，例如下图，我们expression一次橘子，po了一次all，再print橘子的时候，显示的是“$11”，说明我们print命令是$10。这个不用管，只是提醒我们一下而已，作用应该不是很大。
![](https://upload-images.jianshu.io/upload_images/1440398-eb4bbff8be615aeb.png)
       <4> ** po命令**
        现在我们来看看平时用的比较多的“po”，它是“print object”的简写。po一下，我们就可以看到对象的详细信息。po使用的比较多，使用起来也比较简单、方便，这里不做多余的介绍。
        <5>  **image命令**
> 
image list 查看工程中使用的库
image lookup --address  0x000000010e0979ac  程序崩溃的时候定位，查看具体报错位置
       这个其实我们可以想办法用在我们崩溃日志的收集里面，这样的话我们就可以直接定位到崩溃信息的具体位置了。（亲测，不好使。感觉xcode反馈的崩溃信息不准确。）
       <6>  **call命令**
        call即**调用**的意思。上述的po和p也有调用的功能。所以一般只在不需要显示输出，或方法无返回值的时候使用call。 和上面的命令一样，我们依然在viewDidLoad:里面设置断点，然后在程序中断的时候输入下面的命令：
> 
call  [self.view setBackgroundColor:[UIColor redColor]]
        继续运行程序，看看view的背景颜色是不是变成红色的了！在调试的时候灵活运用call命令可以起到事半功倍的作用（亲测好使）。
（2）**LLDB调试栏**
![](https://upload-images.jianshu.io/upload_images/1440398-81fbd739bef06a95.png)
      一般的按钮和功能我们用的比较多，也比较熟悉，这里我们着重介绍一下**Debug View Hierachy**和xcode8新增的**memory graph**功能。
      <1>**Debug View Hierachy**
![](https://upload-images.jianshu.io/upload_images/1440398-b9d9d222a76fe3a0.png)
 Debug View Hierachy，翻译过来就是：调试视图层次。除了点击控制台出的图标，也可以从菜单中选择Debug > View Debugging > Capture View Hierarchy 来启动视图调试。（我们可以看出xcode开发人员的用心之处：重叠在一起的长方形，我们大概就明白这个按钮是表示层级关系的）在断点或者不是断点的情况下都可以通过点击这个按钮查看视图层级关系。点击按钮，我们会在xcode最顶端的地方看到下图的一个信息：
![](https://upload-images.jianshu.io/upload_images/1440398-d98c8832be3992e7.png)
       capture user interface for YourAppName：capture是捕获的意思，interface，face我们知道是脸，inter是进入的意思，interface就是进入脸，我们大概能够明白这句话的意思就是“**为你的app捕获用户交界面**”。
![](https://upload-images.jianshu.io/upload_images/1440398-a19fac9cc93102fb.png)
从左到右控件排序：（上图中也简单解释了各个功能）
> 
调整视图间距：调整不同视图间的间距。
展示被剪切的内容：当前展示视图中被剪切的部分。
展示约束：展示选中视图的约束。
重置查看区域：将3D渲染透视图恢复至默认状态。
调整查看模式：选择性地展示3D渲染透视图，比如仅展示内容，仅展示框架以及同时展示内容和框架。
缩小：缩小3D渲染透视图
恢复：将3D渲染透视图恢复至默认尺寸。
放大：放大3D渲染透视图
调整可视视图范围：隐藏视图或展示视图，一步步解析3D渲染视图，向左或者向右滑动滑块儿有相反的效果。
       有了这个图层关系，我们可以很清楚的知道页面上边的各个控件的位置关系，因为我们在开发阶段�尤其是测试阶段，某个控件上边的字不显示，或者控件的字被遮挡，我们可以用视图调试器查看，是否控件是frame设置的不合理。
        <2>**memory graph**
**【经验1】**
        这个是xcode8新增的功能，翻译过来的意思就是：**内存图**。有了内存图我们就可以解决**闭包引用循环**问题了。举个栗子，我们写个循环引用，如下：
![](https://upload-images.jianshu.io/upload_images/1440398-57d2c75748844907.png)
![](https://upload-images.jianshu.io/upload_images/1440398-e08fdd919a9f2516.png)
![](https://upload-images.jianshu.io/upload_images/1440398-741bf149fd5ab3bf.png)
     （说实在的，这几个命令我在终端不知道怎么调用，试了半天，还是没有搞出来，应该就是内存图调试的树状结构。如果有谁在终端里面知道怎么搞出来，烦请告诉我一下具体怎么操作，谢谢了！）所以，这里我们直接看这个**memory graph**按钮点击后的效果。
**      【经验2】**
**       说明1**：这个功能是xcode8新增的功能，那么xcode7上边肯定找不到！而xcode8还要10.11.5以上的系统，所以，建议大家先把升级电脑系统，然后安装xcode8。
**说明2**：  真机的话还需要iOS9或者10的系统。
**  说明3**：查找当前默认Xcode.app的developer路径---终端命令行：xcode-select -p。
                    如果安装了多个版本的xcode工具，可以使用xcode-select命令指定命令行指令使用哪个版本xcode下的developer目录下的调试工具，即**修改路径**：xcode-select --switch /Applications/Xcode2.app/Contents/Developer。
**  踩过的坑：**
      <1>本来我用的是xcode7.2，挺好用，结果在xcode7上边显然没有这个按钮，升级到xcode8吧。
      <2>同事airdrop传来的xcode8.0和xcode8.1，结果提示安装不上，需要升级电脑的系统。
      <3>升级好了系统，安装好了同事airdrop传来的xcode8.0不显示。（！准备开始抓狂  ！）难道需要8.1才行？！
      <4>安装好了同事airdrop传来的xcode8.1，依旧不显示。（！！！抓狂！！！）难道是�打开xcode时候的路径不对？！
      <5>修改打开xcode的路径为xcode8.1，依旧不显示。（早已经料到是这个结果了）难道是xcode安装的太多了？！
      <6>卸载掉xcode7.0.1、7.2、7.3、8.0，只剩下一个8.1，依旧不显示。（淡定的接受这个结果）难道是同事传的xcode包有问题？！在App Store上自己下载！！
        <7>�下载好了，安装好了，依旧不显示。（�生无可恋了。。。）
        就在此时，我不知道是被逼疯了还是灵光一闪，拿了同事的真机运行项目，结果**居然**有了memory graph按钮！！！！
> 
当你认为最困难的时候，恰恰也就是你最接近成功的时候！
当你放弃的时候，你永远不会知道你离成功是那么的接近！
成功很简单，就是在你坚持不住的时候，再坚持一下！
所以建议使用**memory graph**功能之前确保：
> 
升级电脑系统并安装xcode8.1。（8.1比8.0更稳定）（这里其实并不非得是App Store自己下载，别人用airdrop传的也行）
真机iOS系统在9.0以上。
**      【经验3】**
       当你点击这个按钮以后，xcode上边的状态条会显示下图：
![](https://upload-images.jianshu.io/upload_images/1440398-aca4071c0db1c0f7.png)
       Capturing memory graph for YourAppName：翻译过来就是正在捕捉你的app内存图。
       来来来，我们先来看看这狗东西......不是，是点击后的效果图😂😂😂
![](https://upload-images.jianshu.io/upload_images/1440398-0fc9b569994020a1.png)
      我想说，这是啥玩意？！
![](https://upload-images.jianshu.io/upload_images/1440398-4ae1ba8c463bb8dd.png)
**    【经验4】**
     在解释上边图结构和各个图标表示的含义之前我们先来看看这几个**叹号**分别代表什么。
![](https://upload-images.jianshu.io/upload_images/1440398-5c408de97bd8bbf9.png)
> 
**红色叹号**：这个最常见，**Error=错误**。
**黄色叹号**：这个也常见，**Warning=警告**。
**紫色叹号**：这个不常见，不过使用了memory graph我们就会经常见到，**Runtime Issue=运行时问题**。
**蓝色箭头**：这个是**检查内存泄露**是见到的，静态分析**Command+Shift+B**就可以看见。**Static Analyzer Issue=静态分析的问题**。
**红色叉叉**：这个实在instruments里面Leaked用法的时候见到过。**UI Test Failed=UI测试失败**。
在memory graph状态下我们点击左边的叹号，就会看到RunTime Issue：
![](https://upload-images.jianshu.io/upload_images/1440398-c8043863b976173f.png)
**      【经验5】**
       在解决这个紫色叹号之前我们先来看看右边灰色三棱锥、蓝色正方体、绿色圆圈等等都代表的是什么。
> 
绿色的一般都是 UIKit 控件及其子类
蓝色一般 NSObject 类及其子类
黄色一般都是容器类型及其子类
灰色括号是指 block
      当然还有很多一些其他的类型,具体的大家去看右上角的** Memory Inspect 界面**就好,上面都会有详细的信息。
**    【经验6】**
      当我们在某一个块上边点击右键的时候，会弹出一个选项框，里面有5个选项：
![](https://upload-images.jianshu.io/upload_images/1440398-a8e225f8cd6ce9e0.png)
![](https://upload-images.jianshu.io/upload_images/1440398-a9b93034d53d6add.png)
> 
Quick Look：快速查看，和上图中的小眼睛功能一样。
Print Description：打印详细信息，和上图小眼睛右边的按钮功能一样。
Jump To Definition：跳至代码区。
Reveal in Debug Navigator：在左边的内存树状结构中标蓝色。
Focus on Node：在节点上关注，点击后只剩下跟自己前后箭头相关的节点node。
        一开始进入首页的时候，只展示tabbar上边的四个controller。而当我们想要看某个页面的memory graph的时候，我们需要在真机上边跳到那个页面，然后再点击memory graph按钮，才能在左边的树状结构中找到想要看的页面的controller。
**      【经验7】**
        和红色、黄色叹号一样，紫色叹号出来了，我们就要想办法解决掉，那么我们看看项目中的紫色叹号都标记在哪里了呢？
![](https://upload-images.jianshu.io/upload_images/1440398-3e2c38a9ea35e416.png)
       各种Reachability啊！！！好不容易找到这个memory graph按钮，好不容易看明白点了内存图，好不容易找到了紫色叹号，你踏玛德告诉我这个错误是因为苹果自己的Reachability造成的！！！（生无可恋。。。。。。。。。。。。。。。。。）紫着吧。
2、断点
断点里面根据作用和功能也有很多种类：**普通断点、条件断点、异常断点、符号断点**等。我们一一学习介绍。
（1）**普通断点**（不带技能就出去闯荡江湖的）
        当程序运行到断点处时会暂停运行。比如断点打在11行，那么程序就会停在11行（注意：程序只运行到了前10行，第11行其实还没有被执行。）。只要在代码行旁边点击，就能添加一个断点，再次点击，断点变成浅蓝色，就能让断点不可用（disable了，仍然存在，只是不起作用了）。
![](https://upload-images.jianshu.io/upload_images/1440398-85e7dd2a336f2d0c.png)
（2）**条件断点**（带上技能再闯荡江湖）
打上断点之后，对断点进行编辑，设置相应过滤条件。单击右键会弹出选项框：
![](https://upload-images.jianshu.io/upload_images/1440398-7b10fe9cf0401aa6.png)
> 
Edit BreakPoint... ：编辑断点。
Disable  BreakPoint ：断点失效。（相当于上边说到的单击断点变成浅蓝色，断点失效）
Delete BreakPoint ：删除断点。
Reveal in BreakPoint Navigator :在左边的断点树状结构表明该断点。
这里我们主要用到的是第一个：Edit BreakPoint。这里面设置断点的筛选条件（双击断点也可以快速进入编辑断点的对话框）。
![](https://upload-images.jianshu.io/upload_images/1440398-1ec91790b78e921a.png)
> 
【1】Condition：返回一个布尔值，当布尔值为真触发断点，一般里面我们可以写一个表达式。
【2】Ignore：      忽略前N次断点，到N+1次再触发断点。
【3】Action：      断点触发事件，分为六种：
           <1>  AppleScript：执行脚本。
           <2>  Capture GPU Frame：用于OpenGL ES调试，捕获断点处GPU当前绘制帧。 
           <3>  Debugger Command：和控制台中输入LLDB调试命令一致。
           <4>  Log Message：输出自定义格式信息至控制台。
           <5>  Shell Command：接收命令文件及相应参数列表，Shell Command是异步执行的，只有勾选“Wait until done”才会等待Shell命令执行完在执行调试。
            <6>  Sound：断点触发时播放声音。
【4】Options(Automatically continue after evaluating actions选项)：选中后，表示断点不会终止程序的运行。
【1】Condition：这里我设置i==6，我们看LLDB控制台打印结果：
![](https://upload-images.jianshu.io/upload_images/1440398-10f126571d936611.png)
这里打印了0-5，然后断点断了。这样做的目的就是我们不用在循环里面一个一个的点击下一步，直接跳至我们想要看到的那一步。
【2】Ignore：这里我把Condition的条件取消，设置Ignor的条件为3，我们看LLDB控制台打印结果：
![](https://upload-images.jianshu.io/upload_images/1440398-62efb20154dca31a.png)
结果是将0-2的循环直接忽略，而后边的循环依旧每次在断点的位置断一次。
(3)  **异常断点 **Exception Breakpoint**（全局断点）**
      异常断点可以快速定位不满足特定条件的异常，比如常见的数组越界，这时候很难通过异常信息定位到错误所在位置。这个时候异常断点就可以发挥作用了。
     添加异常断点：
![](https://upload-images.jianshu.io/upload_images/1440398-6cc6d12c9e6f6522.png)
        同样的，全局断点也是可以编辑的，单击右键或者双击断点就会弹出编辑框，编辑的项目和上述是一样的。
**（4）符号断点 Symbolic Breakpoint**
        符号断点的创建也同异常断点。一般符号断点可以在你指定的[类名 方法名]时中断执行**。**
![](https://upload-images.jianshu.io/upload_images/1440398-b5cd6c324fb16066.png)
3、**性能检测：**
（1）静态分析：通过对代码静态分析，找出代码潜在的错误，如内存泄漏、空引用、未使用函数等。
                方法：菜单“Product"->"Analyze"或者Shift+Command+B，然后想办法**消灭蓝箭头**。
（2）动态分析：通过Instruments工具跟踪分析程序运行时的数据
                方法：参考《[Instruments性能检测](https://www.jianshu.com/p/9e94e42cfb01)》
4、其他小技巧
       （1）**模拟器调试：FPS**
        在《[Instruments性能检测](https://www.jianshu.com/p/9e94e42cfb01)》一文中我们就介绍了**FPS=Frame Per Sencond：�一秒钟渲染多少帧**。
        根据苹果全球开发者大会WWDC的说法，**当FPS低于45时，用户就会察觉滑动有卡顿。**
        编译并运行应用程序，选中模拟器，从 Debug菜单中选择Color Blended Layers选项。
![](https://upload-images.jianshu.io/upload_images/1440398-3be231a1381256e8.png)
![](https://upload-images.jianshu.io/upload_images/1440398-caa5b2bba7ff8560.png)
> 
**红色**：表示这些layer是透明的，系统在渲染这些像素点的时候，需要将该view及view一下的其他view混合之后才能得到实际的颜色。红色越深，表明系统在渲染的时候越费劲。
**绿色**：表示这些layer是不透明的，易于渲染。
**黄色**：表示这里的点无法直接绘制在屏幕上，此时系统需要对相邻的像素点做反锯齿计算，增加了图形负担。产生的原因是这个控件的背景是通过图片拉伸得到的。
**所以推荐尽可能地使用不透明的图层。**
**（2） 真机调试：截图。**
       当我们在模拟器上边运行项目的时候，想要给产品或者测试人员看一下页面效果如何，qq截图就可以了，如果在真机上呢怎么截图呢？一般我们会拿着真机给产品或者测试人员看看，但是如果来回折腾很麻烦，我们也可以用自己的手机照相然后发图片给他们，这里还有一个更好的办法对真机进行截图：运行项目，**选择Debug--View Debugging--Take Screenshot of 真机名字**，这样在你的电脑桌面上边就会有一张你的真机上边选好页面的截图。
![](https://upload-images.jianshu.io/upload_images/1440398-86eb47779287a470.png)
参考：1、[iOS各种调试技巧豪华套餐](https://www.jianshu.com/p/92d949a6d4e7)
           2、[iOS调试之LLDB](https://www.jianshu.com/p/073979bccd2f)
           3、[Xcode8调试黑科技：Memory Graph实战解决闭包循环引用问题](https://www.jianshu.com/p/f792f9aa2e45)
           4、[iOS开发之Xcode常用调试技巧总结](https://www.jianshu.com/p/d8bc3d74dc3e?utm_campaign=hugo&utm_medium=reader_share&utm_content=note&utm_source=qq)
           5、[Xcode8的Debug新特性---WWDC 2016 Session 410 & 412 学习笔记](https://www.jianshu.com/p/074072c33916)
           6、[iOS 开发之个人调试技巧](https://www.jianshu.com/p/0b9f0b142aed)
**一、Xcode调试技巧之：NSLog**
上面也提到了,在我们日常的开发过程中最常见的Debug方式就是打Log。而在OC语言中，打Log是采用NSLog方法。但是NSLog效率低下，具体原因可以看这篇博客（《[NSLog效率低下的原因及尝试lldb断点打印Log](http://blog.sunnyxx.com/2014/04/22/objc_dig_nslog/)》）。所以在平时的开发过程中，能不打Log就不打Log。实在想打Log网上也有对NSLog的一些优化方法，可以阅读王巍的《[宏定义的黑魔法 - 宏菜鸟起飞手册](https://onevcat.com/2014/01/black-magic-in-macro/)》如下代码便出自其中：
|1234567|`#define NSLog(format, ...) do {                                             \``fprintf(stderr, ``" %s\n"``,                                           \``[[[NSString stringWithUTF8String:__FILE__] lastPathComponent] UTF8String],  \``__LINE__, __func__);                                                        \``(NSLog)((format), ``##__VA_ARGS__);                                           \``fprintf(stderr, ``"-------\n"``);                                               \``} ``while``(0)`|
另外在使用NSLog的时候应当注意，release版本中应该要去掉NSLog。
**二、Xcode调试技巧之：LLDB**
1、po：print object的缩写，表示显示对象的文本描述，如果对象不存在则打印nil。
简单的打印一个对象我们就不说了，我们来说说特殊的应用场景吧！
应用场景：你想知道一个视图包含了哪些子视图。当然你可以循环打印子视图，但是下面只需要一个命令即可解决。
输出视图层级关系（这是一个被隐藏的命令）:po [[self view] recursiveDescription]
还有个常见的调试场景，比如你要打印一个model。你直接用NSLog或po对象处理的结果是model的地址，这并不是我们想要的。怎么办？有没有解决方法呢？
答案是有的。你可以重写model里面的description方法。但是，如果model里属性非常多，这样就不适用了。你不可能说在description方法里面拼接属性返回。这样不仅麻烦，而且可读性非常差。到这里，我们可以利用runtime动态获取属性并返回。不过我并不建议你重写description方法，我推荐你重写debugDescription方法（至于详细的介绍以及如何重写[请点击此处](http://ms.csdn.net/share/14C7310B9DBCF3E650907B00695DBD79_1_IPHONE_APP)），因为debugDescription方法和description方法效果一样，区别在于debugDescription方法是在你使用po命令时调用的，实际上也是调用了description方法。
2、p:可以用来打印基本数据类型。
3、call:执行一段代码
|1|`call NSLog(@``"%@"``, @``"yang"``)`|
4、expr:动态执行指定表达式
expr i = 101
输出：(int)$0 = 101
5、bt:打印当前线程堆栈信息
如果要打印所以线程堆栈信息，使用:bt all即可。
6、image:常用来寻找栈地址对应代码位置：
举个栗子：
应用场景（数组越界）模拟代码：
|12|`NSArray *array = @[@``"yang"``,@``"she"``,@``"bing"``];``NSLog(@``"%@"``,array[3]);`|
错误信息如下：
|12345678910111213141516|`*** Terminating app due to uncaught exception ``'NSRangeException'``, reason: ``'*** -[__NSArrayI objectAtIndex:]: index 3 beyond bounds [0 .. 2]'``*** First ``throw``call stack:``(``0   CoreFoundation                      0x000000010579734b __exceptionPreprocess + 171``1   libobjc.A.dylib                     0x00000001051f821e objc_exception_throw + 48``2   CoreFoundation                      0x00000001056d1eeb -[__NSArrayI objectAtIndex:] + 155``3   BGMultimediaDemo                    0x0000000104c25550 -[ViewController viewDidLoad] + 192``4   UIKit                               0x0000000105d5c06d -[UIViewController loadViewIfRequired] + 1258``......``......``......``21  BGMultimediaDemo                    0x0000000104c25adf main + 111``22  libdyld.dylib                       0x000000010857268d start + 1``23  ???                                 0x0000000000000001 0x0 + 1``)``libc++abi.dylib: terminating ``with``uncaught exception of type NSException`|
这个时候我们如果怀疑出错的地址是0x0000000104c25550，那么我们可以使用下面命令来找出错误代码的位置：
image lookup --address 0x0000000104c25550
执行命令后输出结果如下：
Address: BGMultimediaDemo[0x0000000100001550] (BGMultimediaDemo.__TEXT.__text + 192)
Summary: BGMultimediaDemo`-[ViewController viewDidLoad] + 192 at ViewController.m:30
从上面输出结果中可以看出，错误位置应该是ViewController.m文件中的30行。是不是超级好用？反正我觉得好用。
**三、Xcode调试技巧之：断点（Breakpoint）**
断点，程序员Debug必备技之一。
1、条件断点
打上断点之后，对断点进行编辑，设置相应过滤条件。下面简单的介绍一下条件设置：
Condition：返回一个布尔值，当布尔值为真触发断点，一般里面我们可以写一个表达式。
Ignore：忽略前N次断点，到N+1次再触发断点。
Action：断点触发事件，分为六种：
- 
AppleScript：执行脚本。
- 
Capture GPU Frame：用于OpenGL ES调试，捕获断点处GPU当前绘制帧。
- 
Debugger Command：和控制台中输入LLDB调试命令一致。
- 
Log Message：输出自定义格式信息至控制台。
- 
Shell Command：接收命令文件及相应参数列表，Shell Command是异步执行的，只有勾选“Wait until done”才会等待Shell命令执行完在执行调试。
- 
Sound：断点触发时播放声音。
这些功能平时在调试程序的过程中都可以进行尝试，说实话我用的设置Condition项会较多些。
Options(Automatically continue after evaluating actions选项)：选中后，表示断点不会终止程序的运行。
![38.jpg](http://cc.cocimg.com/api/uploads/20161101/1477988185595536.jpg)
2、异常断点
异常断点可以快速定位不满足特定条件的异常，比如常见的数组越界，这时候很难通过异常信息定位到错误所在位置。这个时候异常断点就可以发挥作用了。
添加异常断点：
![39.jpg](http://cc.cocimg.com/api/uploads/20161101/1477988201696957.jpg)
编辑异常断点：
![40.jpg](http://cc.cocimg.com/api/uploads/20161101/1477988213410670.jpg)
Exception：可以选择抛出异常对象类型：OC或C++。
Break：选择断点接收的抛出异常来源是Throw还是Catch语句。
3、符号断点
符号断点的创建方式和异常断点一样一样的，在符号断点中可以指定要中断执行的方法：
举个例子，常见的场景，我想让它执行到ViewController类中的viewWillAppear方法就中断执行：
![42.png](http://cc.cocimg.com/api/uploads/20161101/1477988250339451.png)
Symbol:[ViewController viewWillAppear:]即[类名 方法名]可以执行到指定类的指定方法中开始断点。如果只有viewWillAppear:即方法名，它会执行到所以类中的viewWillAppear:方法中开始断点。
**四、Xcode调试技巧之：EXC_BAD_ACCESS**
1、开启僵尸对象
开启Zombie模式之后会导致内存上升，因为所以已经被释放（引用计数为0）的对象被僵尸对象取代，并未真的释放掉。这个时候再给僵尸对象发送消息，就会抛出异常，并打印出异常信息，你可以轻松的找到错误代码位置，结束Zombies时会释放。它的主要功能是检测野指针调用。
使用方法：
“Edit Scheme…” —> “Run” —> “Diagnostics” —> “Zombie Objects”
打开”Edit Scheme…”窗口：
![43.png](http://cc.cocimg.com/api/uploads/20161101/1477988279182828.png)
开启Zombie模式：
![44.png](http://cc.cocimg.com/api/uploads/20161101/1477988284639020.png)
注意：Zombie模式不能再真机上使用，只能在模拟器上使用。
2、Address Sanitizer（地址消毒剂）
在Xcode7之后新增了AddressSanitizer工具，为我们调试EXC_BAD_ACCESS错误提供了便利。当程序创建变量分配一段内存时，将此内存后面的一段内存也冻结住，标识为中毒内存。程序访问到中毒内存时（访问越界），立即中断程序，抛出异常并打印异常信息。你可以根据中断位置及输出的Log信息来解决错误。当然，如果变量已经释放了，它所占用的内存也会被标识为中毒内存，这个时候访问这片内存空间同样会抛出异常。
使用方法：
“Edit Scheme…” —> “Run” —> “Diagnostics” —> “Zombie Objects”
![45.png](http://cc.cocimg.com/api/uploads/20161101/1477988306785766.png)
开启AddressSanitizer之后，在调试程序的过程中，如果有遇到EXC_BAD_ACCESS错误，程序则会自动终端，抛出异常。
**五、结语**
文中提到的这些只是iOS开发过程中比较常见的一部分Debug方式。其他的还有比如说：Profile，Analyze分析，View Hierarchy（在调试视图显示异常时用的比较多）等，有兴趣的可以自行了解。
本文内容中部分来自网络，后续会不断更新完善。欢迎一起学习交流！
**六、参考博客地址：**
- 
[iOS 开发调试技巧](http://www.jianshu.com/p/9c4c92a38468)
- 
[与调试器共舞 - LLDB 的华尔兹](https://objccn.io/issue-19-2/)
- 
[Xcode7新特性AddressSanitizer](http://blog.csdn.net/xbenlang/article/details/49490563)
- 
[在Xcode 7上直接使用Clang Address Sanitizer](http://www.cocoachina.com/ios/20150730/12830.html)
- 
[Xcode 调试技巧](http://qingbo.info/2016/08/17/xcode-debugging/)
- 
[Xcode Debug技巧](http://dev.guanghe.tv/2015/12/iOSXcodeDebug.html)
