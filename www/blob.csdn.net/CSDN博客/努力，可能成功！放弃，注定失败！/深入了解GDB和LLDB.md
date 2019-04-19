# 深入了解GDB和LLDB - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月19日 16:17:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：504

## 什么是GDB和LLDB
我们在开发iOS程序的时候常常会用到调试跟踪，如何正确的使用调试器来debug十分重要。xcode里有内置的Debugger，老版使用的是GDB，xcode自4.3之后默认使用的就是LLDB了。
> 
GDB:
UNIX及UNIX-like下的调试工具。
> 
LLDB:
LLDB是个开源的内置于XCode的具有REPL(read-eval-print-loop)特征的Debugger，其可以安装C++或者Python插件。
所以他们两个都是调试用的Debugger，只是LLDB是比较高级的版本，或者在调试开发iOS应用时比较好用，不然人家苹果为什么换成了LLDB了呢！
lldb与gdb命令名的对照表：[http://lldb.llvm.org/lldb-gdb.html](https://link.jianshu.com?t=http://lldb.llvm.org/lldb-gdb.html)
## LLDB的使用
在程序里你需要的地方设置断点。当断点断住的时候你就能看到我们进入LLDB调试器了。
![](https://upload-images.jianshu.io/upload_images/101810-bd1a33a28936c5d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
这时我们就可以使用一些LLDB命令来进行一些调试了。
## 调试快捷键：(Xcode常用快捷键)
> 
command+shift+Y 打开调试窗口
command+Y 调试运行程序
command+option+P 继续
command+shift+O 跳过
command+shift+I 进入
command+shift+T 跳出
## help命令
help会列出所有命令列表，用户加载的插件一般来说列在最后。
执行help 可以打印指定command的帮助信息。
比如：help print会打印内建命令print的使用帮助。
![](https://upload-images.jianshu.io/upload_images/101810-0d887e0c70c85bb8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
![](https://upload-images.jianshu.io/upload_images/101810-82a0144e35469104.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
## print命令
print命令的简化方式有prin pri p，唯独pr不能用来作为检查，因为会和process混淆，幸运的是p被lldb实现为特指print。
实际上你会发现，lldb对于命令的简称，是头部匹配方式，只要不混淆，你可以随意简称某个命令。
例如：
![](https://upload-images.jianshu.io/upload_images/101810-d0ada1bd64265c6c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
最前面的（int）是类型。$是命令结果的引用名，使用$0可以进行print $0 + 7这样打印出17。
**输出view 下 subview 的数量。**
> 
由于 subview 的数量是一个 int 类型的值，所以我们使用命令p：
`（lldb）p (int)[[[self view] subviews] count]`
**直接调用方法改变背景颜色之类**
> 
其实使用p，po，call都可以调用方法，只是p和po都是用于输出的有返回值的。call一般只在不需要显示输出，或是方法无返回值时使用。
`（lldb）p [self.view setBackgroundColor:[UIColor redColor]]`
`（lldb）p (void)[CATransaction flush]`
上述的p一般使用call比较好，因为方法是没有返回值的。
## po命令
命令po跟p很像。p输出的是基本类型，po输出的Objective-C对象。调试器会输出这个 object 的 description。
例如：
![](https://upload-images.jianshu.io/upload_images/101810-c72a9353c8fc6825.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
## expression命令
expression的简写就是e。可以用expression来声明新的变量，也可以改变已有变量的值。我们看到e声明的都是$开头的变量。我们在使用时也需要加上$符号。
例如：
**创建新的变量**
![](https://upload-images.jianshu.io/upload_images/101810-34fcc2cd43d75521.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
注意：如果上面这里输入以下命令，会发生错误。说明lldb无法判定某一步的计算结果是什么数据类型，这时需要强制类型转换来告诉lldb。
```bash
(lldb) p [[$array objectAtIndex:0] characterAtIndex:0]
error: no known method '-characterAtIndex:'; cast the message send to the method's return type
error: 1 errors parsing expression
(lldb) p (char)[[$array objectAtIndex:0] characterAtIndex:0]
'o'
```
**修改已有变量**
## image命令
image 命令可用于寻址，有多个组合命令。比较实用的用法是用于寻找栈地址对应的代码位置。 下面我写了一段代码
```
NSArray *arr=[[NSArray alloc] initWithObjects:@"1",@"2", nil];
NSLog(@"%@",arr[2]);
```
这段代码有明显的错误，程序运行这段代码后会抛出下面的异常：
```java
```java
*** Terminating app due to uncaught exception 'NSRangeException', reason: '*** -[__NSArrayI objectAtIndex:]: index 2 beyond bounds [0 .. 1]'
*** First throw call stack:
(
 0   CoreFoundation                      0x0000000101951495 __exceptionPreprocess + 165
 1   libobjc.A.dylib                     0x00000001016b099e objc_exception_throw + 43
2   CoreFoundation                      0x0000000101909e3f -[__NSArrayI objectAtIndex:] + 175
3   ControlStyleDemo                    0x0000000100004af8 -[RootViewController viewDidLoad] + 312
4   UIKit                               0x000000010035359e -[UIViewController loadViewIfRequired] + 562
5   UIKit                               0x0000000100353777 -[UIViewController view] + 29
6   UIKit                               0x000000010029396b -[UIWindow addRootViewControllerViewIfPossible] + 58
7   UIKit                               0x0000000100293c70 -[UIWindow _setHidden:forced:] + 282
8   UIKit                               0x000000010029cffa -[UIWindow makeKeyAndVisible] + 51
9   ControlStyleDemo                    0x00000001000045e0 -[AppDelegate application:didFinishLaunchingWithOptions:] + 672
10  UIKit                               0x00000001002583d9 -[UIApplication _handleDelegateCallbacksWithOptions:isSuspended:restoreState:] + 264
11  UIKit                               0x0000000100258be1 -[UIApplication _callInitializationDelegatesForURL:payload:suspended:] + 1605
12  UIKit                               0x000000010025ca0c -[UIApplication _runWithURL:payload:launchOrientation:statusBarStyle:statusBarHidden:] + 660
13  UIKit                               0x000000010026dd4c -[UIApplication handleEvent:withNewEvent:] + 3189
14  UIKit                               0x000000010026e216 -[UIApplication sendEvent:] + 79
15  UIKit                               0x000000010025e086 _UIApplicationHandleEvent + 578
16  GraphicsServices                    0x0000000103aca71a _PurpleEventCallback + 762
17  GraphicsServices                    0x0000000103aca1e1 PurpleEventCallback + 35
18  CoreFoundation                      0x00000001018d3679 __CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE1_PERFORM_FUNCTION__ + 41
19  CoreFoundation                      0x00000001018d344e __CFRunLoopDoSource1 + 478
20  CoreFoundation                      0x00000001018fc903 __CFRunLoopRun + 1939
21  CoreFoundation                      0x00000001018fbd83 CFRunLoopRunSpecific + 467
22  UIKit                               0x000000010025c2e1 -[UIApplication _run] + 609
23  UIKit                               0x000000010025de33 UIApplicationMain + 1010
24  ControlStyleDemo                    0x0000000100006b73 main + 115
25  libdyld.dylib                       0x0000000101fe95fd start + 1
26  ???                                 0x0000000000000001 0x0 + 1
)
libc++abi.dylib: terminating with uncaught exception of type NSException
```
```
现在，我们怀疑出错的地址是0x0000000100004af8（可以根据执行文件名判断，或者最小的栈地址）。为了进一步精确定位，我们可以输入以下的命令：
```
（lldb）image lookup --address 0x0000000100004af8
（lldb）im loo -a 0x0000000100004af8
```
命令执行后返回：
```
```
Address: ControlStyleDemo[0x0000000100004af8] (ControlStyleDemo.__TEXT.__text + 13288)
Summary: ControlStyleDemo`-[RootViewController viewDidLoad] + 312 at RootViewController.m:53
```
```
我们可以看到，出错的位置是RootViewController.m的第53行。
## 设置断点触发条件
![](https://upload-images.jianshu.io/upload_images/101810-c7d3ff6061b09779.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
## 参考
[LLDB使用篇](https://link.jianshu.com?t=http://www.dreamingwish.com/article/lldb-usage-a.html)
[LLDB调试命令初探](https://link.jianshu.com?t=http://www.starfelix.com/blog/2014/03/17/lldbdiao-shi-ming-ling-chu-tan/)
作者：齐滇大圣
链接：https://www.jianshu.com/p/087cd19d49ba
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
