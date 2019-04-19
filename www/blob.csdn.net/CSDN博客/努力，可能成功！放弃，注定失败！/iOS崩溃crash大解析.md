# iOS崩溃crash大解析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月15日 14:16:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9047
## 前言
iOS崩溃是让iOS开发人员比较头痛的事情，app崩溃了，说明代码写的有问题，这时如何快速定位到崩溃的地方很重要。调试阶段是比较容易找到出问题的地方的，但是已经上线的app并分析崩溃报告就比较麻烦了。
之前我总是找到一个改一个，并靠别人测试重现来找出问题的地方，这样往往比较耗费时间。而且比较难找到原因的时候每次都是到网上找各种资源搜索，解决了之后也没有认真分析原因及收集，时间长了之后就会忘记原来解决过的问题，别人来问我的时候我也不能很快找到答案。所以这里写一个关于崩溃的文章，以便之后自己查询用。
这里也会开始写一个demo，争取把所有的崩溃原因都写进demo里。
## 1.崩溃分析
### 1.1.崩溃日志（crash log）
#### 1.1.1.xcode中查看崩溃信息
xcode->Window->Organizer->Crashes
![](http://upload-images.jianshu.io/upload_images/101810-a463b8636f8ca5b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 1.1.2.根据符号表来监测奔溃位置
**什么是符号表**
> 
符号表就是指在Xcode项目编译后，在编译生成的二进制文件.app的同级目录下生成的同名的.dSYM文件。
.dSYM文件其实是一个目录，在子目录中包含了一个16进制的保存函数地址映射信息的中转文件，所有Debug的symbols都在这个文件中(包括文件名、函数名、行号等)，所以也称之为调试符号信息文件。
**符号表有什么用**
符号表就是用来符号化 crash log（崩溃日志）。crash log中有一些方法16进制的内存地址等，通过符号表就能找到对应的能够直观看到的方法名之类。
**如何得到.dsYM文件**
> 
我们在Archive的时候会生成.xcarchive文件，然后显示包内容就能够在里面找到.dsYM文件和.app文件。
**如何使用.dsYM**
1.友盟.dsYM分析
> 
如果是使用友盟的话，我们能在错误列表里看到一些错误，然后可以导出奔溃信息，导出的文件为.csv文件。友盟有一个分析工具，使用那个工具可以看到一些错误的函数，行号等。但是很容易分析失败，不知道为什么？
注意：使用的时候要确保你的.xcarchive在 ~/Library/Developer/Xcode/或该路径的子目录下。
.xcarchive里的.dsYM文件和.app文件是有对应的UUID的。然后你的错误详情里也是有UUID，只有当UUID相等时才能分析对。
我犯的错误：因为我们是两个人开发，Archive的时候都是在另一个人的电脑上Archive的，所以我的电脑里根本没有对应的.xcarchive文件。所以我在我电脑上用友盟的分析工具分析是时候是监测不出来错误的。
2.第三方小工具.dsYM分析
> 
或者自己找到.xcarchive文件和错误内存地址(友盟错误详情里标绿色的为错误内存地址)。然后通过一个小应用来分析出对应的函数。[应用下载地址](http://pan.baidu.com/s/1bnkxPvT),具体可参考文章[dSYM
 文件分析工具](http://www.cocoachina.com/ios/20141219/10694.html)
下图是我友盟里的错误信息，可以分析的内存地址就是标绿的地方，图中zhefengle就是你的app名，这部分后面的地址就是可以解析符号化的地址：
![](http://upload-images.jianshu.io/upload_images/101810-d90596bf25ea614b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下图是分析工具分析上面的错误内存地址：
![](http://upload-images.jianshu.io/upload_images/101810-638189391ebdb929.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
分析工具
3.命令行工具symbolicatecrash
> 
symbolicatecrash是xcode的一个符号化crash log的命令行工具。使用方法也就是导出.crash文件（crash log）和找到.dsYM文件，然后进行分析。
如何使用查看[[iOS]使用symbolicatecrash分析crash文件](http://www.cnblogs.com/ningxu-ios/p/4141783.html)
4.还有命令行工具atos
> 
如果你有多个“.ipa”文件，多个".dSYMB"文件，你并不太确定到底“dSYM”文件对应哪个".ipa"文件，那么，这个方法就非常适合你。
特别当你的应用发布到多个渠道的时候，你需要对不同渠道的crash文件，写一个自动化的分析脚本的时候，这个方法就极其有用。
简单使用方法[命令行工具atos](http://blog.sina.com.cn/s/blog_76a1980f0102wjcf.html)
#### 1.1.3.奔溃日志分析
参考[iOS应用崩溃日志分析](http://www.cocoachina.com/industry/20130725/6677.html)里面有很详细的分析介绍。
![](http://upload-images.jianshu.io/upload_images/101810-7ed917ccc7fcb6b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
崩溃日志
以上是一个完整的崩溃日志。其实友盟错误详情里的就是上图的第4部分。
**如何得到崩溃日志**
1.把设备连上电脑，得到自己设备的崩溃日志
> 
崩溃日志可以从xcode里打开Devices看到对应手机的一些奔溃信息。点击下图的View Device Logs就能看到崩溃日志。
![](http://upload-images.jianshu.io/upload_images/101810-1a9874bbbbf26dfb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.使用第三方崩溃管理工具
> 
我暂时只使用过友盟，友盟里面有错误分析，就是截取的崩溃日志。
3.自己截取崩溃日志
> 
自己写入代码，然后截取到崩溃日志，把崩溃日志发送到开发者邮箱里。
[iOS Crash（崩溃）调试技巧](http://www.xingcheng.me/?p=305)这篇文章中有介绍如何截取崩溃日志并发送到邮箱。
**分析崩溃日志**
崩溃日志中的（3）异常
> 
**Exception Type异常类型**
通常包含1.7中的Signal信号和EXC_BAD_ACCESS。
**Exception Codes:异常编码**
0x8badf00d: 读做 “ate bad food”! (把数字换成字母，是不是很像 :p)该编码表示应用是因为发生watchdog超时而被iOS终止的。 通常是应用花费太多时间而无法启动、终止或响应用系统事件。
0xbad22222: 该编码表示 VoIP 应用因为过于频繁重启而被终止。
0xdead10cc: 读做 “dead lock”!该代码表明应用因为在后台运行时占用系统资源，如通讯录数据库不释放而被终止 。
0xdeadfa11: 读做 “dead fall”! 该代码表示应用是被用户强制退出的。根据苹果文档, 强制退出发生在用户长按开关按钮直到出现 “滑动来关机”, 然后长按 Home按钮。强制退出将产生 包含0xdeadfa11 异常编码的崩溃日志, 因为大多数是强制退出是因为应用阻塞了界面。
崩溃日志中的（4）线程回溯
> 
这部分提供应用中所有线程的回溯日志。 回溯是闪退发生时所有活动帧清单。它包含闪退发生时调用函数的清单。看下面这行日志:
![](http://upload-images.jianshu.io/upload_images/101810-6187f8f349e67a8b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
它包括四列:
帧编号—— 此处是2。（数子从大到小为发生的顺序）
二进制库的名称 ——此处是 XYZLib.
调用方法的地址 ——此处是 0x34648e88.
第四列分为两个子列，一个基本地址和一个偏移量。此处是0×83000 + 8740, 第一个数字指向文件，第二个数字指向文件中的代码行。
### 1.2.野指针分析方法(Enable Malloc Scribble)
**介绍**
> 
因为野指针的原因发生崩溃是常常出现的事，而且比较随机。关于一些原因及概念后面我们会讲到。所以我们要提高野指针的崩溃率好来帮我们快速找到有问题的代码。
对象释放后只有出现被随机填入的数据是不可访问的时候才会必现Crash。
这个地方我们可以做一下手脚，把这一随机的过程变成不随机的过程。对象释放后在内存上填上不可访问的数据，其实这种技术其实一直都有，xcode的Enable Scribble就是这个作用。
![](http://upload-images.jianshu.io/upload_images/101810-a0f7debbceefbe18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
enter image description here
更加详细的介绍可以参考：[如何定位Obj-C野指针随机Crash](http://bugly.qq.com/bbs/forum.php?mod=viewthread&tid=31&highlight=%E5%A6%82%E4%BD%95%E5%AE%9A%E4%BD%8DObj-C%E9%87%8E%E6%8C%87%E9%92%88%E9%9A%8F%E6%9C%BACrash)。
[DSCrashDemo](https://github.com/walkdianzi/DSCrashDemo)这个demo里有上面这篇文章里写的关于提高野指针崩溃率的例子。
### 1.3.僵尸模式(NSZombieEnabled)
**介绍**
> 
启用了NSZombieEnabled的话，它会用一个僵尸来替换默认的dealloc实现，也就是在引用计数降到0时，该僵尸实现会将该对象转换成僵尸对象。僵尸对象的作用是在你向它发送消息时，它会显示一段日志并自动跳入调试器。
所以当启用NSZombieEnabled时，一个错误的内存访问就会变成一条无法识别的消息发送给僵尸对象。僵尸对象会显示接受到得信息，然后跳入调试器，这样你就可以查看到底是哪里出了问题。
所以这时一般崩溃的原因是：调用了已经释放的内存空间，或者说重复释放了某个地址空间。
**如何找出问题**
1.NSZombieEnabled
> 
打开NSZombieEnabled之后，如果遇到对应的崩溃类型既调用了已经释放的内存空间，或者说重复释放了某个地址空间。那么就能在GDB中看到对应的输出信息。
比如会出现如下这样的问题：
`[__NSArrayM addObject:]: message sent to deallocated instance 0x7179910`
2.MallocStackLoggingNoCompact
> 
如果崩溃是发生在当前调用栈，通过上面的做法，系统就会把崩溃原因定位到具体代码中。但是，如果崩溃不在当前调用栈，系统就仅仅只能把崩溃地址告诉我们，而没办法定位到具体代码，这样我们也没法去修改错误。这时就可以修改scheme，让xcode记录每个地址alloc的历史，这样我们就可以用命令把这个地址还原出来。
如图：(跟设置`NSZombieEnabled`一样，添加`MallocStackLoggingNoCompact`，并且设置为YES)
![](http://upload-images.jianshu.io/upload_images/101810-281721610095f76e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
这样，当出现崩溃原因是message sent to deallocated instance 0x7179910，我们可以使用以下命令，把内存地址还原：
`(gdb) nfo malloc-history 0x7179910`
也可以使用下面的命令
`(gdb) shell malloc_history {pid/partial-process-name} {address}`
这篇文章中有介绍[MallocStackLoggingNoCompact](http://www.cnblogs.com/vicstudio/p/3150557.html)的使用。
**总结**
还有官方文档[Enabling the Malloc Debugging Features](https://developer.apple.com/library/mac/documentation/Performance/Conceptual/ManagingMemory/Articles/MallocDebug.html)介绍了类似`NSZombieEnabled`和`MallocStackLoggingNoCompact`这类的环境变量的作用。
TODO：翻译Enabling the Malloc Debugging Features这篇文章，写对应的demo测试这类变量设置后如何找出内存出错问题。
### 1.4.Enable Address Sanitizer（地址消毒剂）
![](http://upload-images.jianshu.io/upload_images/101810-573c94b7416a1063.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置这个参数后就能看到一些更详细的错误信息提示，甚至会有内存使用情况的展示。
![](http://upload-images.jianshu.io/upload_images/101810-161383d2e8086cb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
C语言是一门危险的语言，内存安全是一个主要的问题。C语言中根本没有内存安全可言。像下面的代码，会被正常的编译，而且可能正常运行：
```
char *ptr = malloc(5);
ptr[12] = 0;
```
对于内存安全的验证已经有一些解决方案了。如Clang的静态代码分析，可以从代码中查找特定类型的内存安全问题。如Valgrind之类的程序可以在运行时检测到不安全的内存访问。
Address Sanitizer是另外一种解决方案。它使用了一种新的方法，有利有弊。但仍不失为一个查找代码问题的有力工具。
这类工具的理论依据是：**访问内存时，通过比较访问的内存和程序实际分配的内存，验证内存访问的有效性，从而在bug发生时就检测到它们，而不会等到副作用产生时才有所察觉。**
malloc函数总是最少分配16个字节。为了储存针对标准malloc的内存的保护，需要分配内存到16字节的范围内，因此，若分配的内存大小不是16字节的整数倍，余出的几个字节将不受保护。
Address Sanitizer会追踪受限内存，使用了一种简单但是很巧妙的方法：**它在进程的内存空间上保存了一个固定的区域，叫做“影子内存区”。用内存消毒剂的术语来说，一个被标记为受限的内存被称作“中毒”内存。“影子内存区”会记录哪些内存字节是中毒的。**通过一个简单的公式，可以将进程中的内存空间映射到“影子内存区”中，即：每8字节的正常内存块映射到一个字节的影子内存上。在影子内存上，会跟踪这8字节的“中毒状态”。
[Address Sanitizer](https://mikeash.com/pyblog/friday-qa-2015-07-03-address-sanitizer.html)这篇文章详细介绍了Enable Address Sanitizer，对应的中文翻译[在Xcode
 7上直接使用Clang Address Sanitizer](http://www.cocoachina.com/ios/20150730/12830.html)
### 1.5.Static Analyzer（静态分析）
Static Analyzer是一个非常好的工具去发现编译器警告不会提示的问题和一些个人的内错泄露和死存储（不会用到的赋了值的变量）错误。这个方法可能大大的提高内存使用和性能，以及提升应用的整体稳定性和代码质量。
打开方式：Xcode->Product-Analyze
然后我们就能看到如下蓝色箭头所示的一些有问题的代码。
![](http://upload-images.jianshu.io/upload_images/101810-6799040d6d4f1d20.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 1.6.unrecognized selector send to instancd 快速定位
在debug navigator的断点栏里添加Create Symbolic Breakpoint。
![](http://upload-images.jianshu.io/upload_images/101810-ec4bc480374be1a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在Symbolic中填写如下方法签名：
`-[NSObject(NSObject) doesNotRecognizeSelector:]`
![](http://upload-images.jianshu.io/upload_images/101810-83c153b213bef260.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置完成后再遇到类似的错误就会定位到具体的代码。
### 1.7.Signal和EXC_BAD_ACCESS错误分析
#### 1.7.1.Signal
**什么是Signal**
> 
在计算机科学中，信号（英语：Signals）是Unix、类Unix以及其他POSIX兼容的操作系统中进程间通讯的一种有限制的方式。它是一种异步的通知机制，用来提醒进程一个事件已经发生。当一个信号发送给一个进程，操作系统中断了进程正常的控制流程，此时，任何非原子操作都将被中断。如果进程定义了信号的处理函数，那么它将被执行，否则就执行默认的处理函数。
在iOS中就是未被捕获的Objective-C异常（NSException），导致程序向自身发送了SIGABRT信号而崩溃。
**Signal信号的类型**
> 
SIGABRT–程序中止命令中止信号
SIGALRM–程序超时信号
SIGFPE–程序浮点异常信号
SIGILL–程序非法指令信号
SIGHUP–程序终端中止信号
SIGINT–程序键盘中断信号
SIGKILL–程序结束接收中止信号
SIGTERM–程序kill中止信号
SIGSTOP–程序键盘中止信号
SIGSEGV–程序无效内存中止信号
SIGBUS–程序内存字节未对齐中止信号
SIGPIPE–程序Socket发送失败中止信号
[iOS异常捕获](http://www.iosxxx.com/blog/2015-08-29-iosyi-chang-bu-huo.html)这篇文章中有对各种信号的解释。
**SIGABRT**
> 
就crash而言，SIGABRT是一个比较好解决的，因为他是一个可掌控的crash。App会在一个目的地终止，因为系统意识到app做了一些他不能支持的事情。
通常, SIGABRT 异常是由于某个对象接收到未实现的消息引起的。 或者，用简单的话说，在某个对象上调用了不存在的方法。
**SIGSEGV**
> 
SIGSEGV程序无效内存中止信号，一般是表示内存不合法,
**SIGBUS**
> 
SIGBUS程序内存字节未对齐中止信号,
**截取Signal和Exception从容的崩溃**
> 
[DSSignalHandlerDemo](https://github.com/walkdianzi/DSSignalHandlerDemo)
这是一个防止奔溃的源码，可以使一些原本会奔溃的操作弹出UIAlertView。里面写了两种信号量的崩溃：SIGSEGV、SIGABRT，还有一些信号大家可以写上去提个PR给我。
下图为源码的运行图，其中Signal中的Signal(EGV)第一次点击的时候能弹出alert，如果第二次点击就没有崩溃和alert，感觉像卡死一样。
而Signal(BRT)中的这种信号错误多次点击也是没有问题的还是能继续下去。个人猜测可能是SIGSEGV这种信号错误会导致了整个进程挂了。
注意：测试的时候如果测试Signal类型的崩溃，不要在xcode下的debug模式进行测试。因为系统的debug会优先去拦截。应该build好应用之后直接点击运行app进行测试。
![](http://upload-images.jianshu.io/upload_images/101810-cee27904180e7c4a.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 1.7.2.EXC_BAD_ACCESS
> 
EXC_BAD_ACCESS是一个比较难处理的crash了，当一个app进入一种毁坏的状态，通常是由于内存管理问题而引起的时，就会出现出现这样的crash。通常1.7.1中的Signal信号错误都会提醒EXC_BAD_ACCESS。
文中1.3就介绍了用一些变量设置来找出这类错误。
## 2.崩溃类型收集
### 2.1.新老操作系统兼容
**原因**
> 
开发人员在进行开发的时候，常常使用的是某个操作系统版本，所以在开发人员进行开发测试的那个系统版本上基本不会出现问题。但在其他版本上开发人员无法进行完全的测试，这就导致了在新系统上运行正常，但在旧系统上却崩溃的情况。
在新 iOS 上正常的应用，到了老版本 iOS 上秒退最常见原因是系统动态链接库或Framework无法找到。这种情况通常是由于 App 引用了一个新版操作系统里的动态库（或者某动态库的新版本）或只有新 iOS 支持的 Framework，而又没有对老系统进行测试，于是当 App 运行在老系统上时便由于找不到而秒退。
还有就是有些方法是新版操作系统才支持的，而又没有对该方法是否存在于老系统中做出判断。这种情况其实还是比较难出现的，除非开发人员太low了，因为这类方法在xcode编码时编辑器都会有提醒的。
**解决**
> 
这种问题一般就是用户升级操作系统或者开发人员修改问题以兼容老系统。
### 2.2.本地存储的数据结构改变
**原因**
> 
程序在升级时，修改了本地存储的数据结构，但是对用户既存的旧数据没有做好升级，结果导致初始化时因为无法正确读取用户数据而秒退。
**解决**
第一种：是把服务端传过来的一些信息保存在本地，使用的时候从本地数据库取。
> 
刚开始的时候我是第一次从服务端得到数据的时候直接解析成对应的model然后存入plist文件里面。这时就有一个问题，比如服务端新传了字段`newId`，但是我旧版model里面没有定义过，存入本地的数据还是没有这个字段。
然后等我升级了程序，新程序里model，定义了这个`newId`字段，但是旧版里面数据已经保存过一遍了没有这个字段。这时再去取就取不到了。
所以后来我就把存储时解析数据改成了读取时解析数据。就是不管服务端传什么数据都把它存下来，然后在使用的时候再把它解析成对应的model，这样就不会丢失字段了。
第二种：自己的一些数据存储在本地SQLlite，新版的时候表结构改了。
SQLlite只支持更改一个表的名字，或者向表中增加一个字段（列），但是我们不能删除一个已经存在的字段，或者更改一个已经存在的字段的名称、数据类型、限定符等等。
> 
这种就是有时候新版又添加字段了，或者改变了字段的名称了。一般来说原有的字段名称不应该改变，但是添加新字段是常有的事。
一般做法是在第一次创建表的时候加一些冗余字段，以防后面不时之需。但是如果真没办法需要在旧表上增加新字段了，那就要做数据迁移了。
这里有一个库在FMDB基础上管理SQLlite数据库了，可以用来做数据迁移用。[FMDBMigrationManager](https://github.com/layerhq/FMDBMigrationManager)
TODO：做一个数据迁移的demo
### 2.3.访问的数据为空或访问数据类型不对
**原因**
> 
这类情况是比较常见的，后端传回了空数据，客户端没有做对应的判断继续执行下去了，这样就产生了crash。或者自己本地的某个数据为空数据而去使用了。还有就是访问的数据类型不是期望的数据类型而产生崩溃。
**解决**
第一种：
> 
服务端都加入默认值，不返回空内容或无key，但是服务端往往会不太愿意改，还有就是有些确实应该无值的话key也不用传，减少数据量的传输。
第二种：
> 
这种就是客户端自己做判断，如果每次都是自己去if判断是否为空或格式是否正确那肯定是比较麻烦的。所以这里用到了NSArray和NSDictionary的Category。一般我们访问的数据都是NSArray或NSDictionary，所以在取值方法里面做一下判断，返回正确的数据类型或默认值即可。
[DSCategories](https://github.com/walkdianzi/DSCategories/blob/master/README.md)这里面有两个Category`NSArray+SafeAccessM`和`NSDictionary+SafeAccess`可以看一下。
### 2.4.操作了不该操作的对象，野指针之类
#### 2.4.1野指针介绍
> 
iOS中有空指针和野指针两种概念。
空指针是没有存储任何内存地址的指针。如`Student *s1 = NULL;*`和`Student s2 = nil;`
而野指针是指指向一个已删除的对象（"垃圾"内存既不可用内存）或未申请访问受限内存区域的指针。野指针是比较危险的。因为野指针指向的对象已经被释放了，不能用了，你再给被释放的对象发送消息就是违法的，所以会崩溃。
[空指针和野指针](http://www.cnblogs.com/mjios/archive/2013/04/22/3034788.html#label1)这篇文章介绍了空指针和野指针的概念。
#### 2.4.2野指针崩溃情况
> 
野指针访问已经释放的对象crash其实不是必现的，因为dealloc执行后只是告诉系统，这片内存我不用了，而系统并没有就让这片内存不能访问。
所以野指针的奔溃是比较随机的，你在测试的时候可能没发生crash，但是用户在使用的时候就可能发生crash了。
注意：arc环境比非arc环境更少出现野指针。
现实出现问题大概是下面几种可能的情况：
> - 对象释放后内存没被改动过，原来的内存保存完好，可能不Crash或者出现逻辑错误（随机Crash）。
- 对象释放后内存没被改动过，但是它自己析构的时候已经删掉某些必要的东西，可能不Crash、Crash在访问依赖的对象比如类成员上、出现逻辑错误（随机Crash）。
- 对象释放后内存被改动过，写上了不可访问的数据，直接就出错了很可能Crash在objc_msgSend上面（必现Crash，常见）。
- 对象释放后内存被改动过，写上了可以访问的数据，可能不Crash、出现逻辑错误、间接访问到不可访问的数据（随机Crash）。
- 对象释放后内存被改动过，写上了可以访问的数据，但是再次访问的时候执行的代码把别的数据写坏了，遇到这种Crash只能哭了（随机Crash，难度大，概率低）！！
- 对象释放后再次release（几乎是必现Crash，但也有例外，很常见）。
参考下面这张图：
![](http://upload-images.jianshu.io/upload_images/101810-c08213d8d1dbb8d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
enter image description here
### 2.5.内存处理不当
> 
说到因为内存处理不当崩溃就要涉及到内存管理问题了。内存管理是软件开发中一个重要的课题。iOS自从引入ARC机制后，对于内存的管理开发者好像轻松了很多，但是还会发生一些内存泄露之类的问题。
对于这一块知识点需要了解ARC的一些机制，还有用instruments排查内存泄露问题等。这部分我后面会专门写一篇文章进行阐述。
### 2.6.主线程UI长时间卡死，被系统杀掉
> 
主线程被卡住是非常常见的场景，具体表现就是程序不响应任何的UI交互。这时按下调试的暂停按钮，查看堆栈，就可以看到是到底是死锁、死循环等，导致UI线程被卡住。
这部分需要研究多线程，还有如何看调试栏里的线程的信息。
[GCD死锁](http://www.saitjr.com/ios/ios-gcd-deadlock.html)这篇文章介绍了GCD使用多线程时的死锁问题。
还有这篇[iOS多线程中，队列和执行的排列组合结果分析](http://www.cnblogs.com/dsxniubility/p/4296937.html)也值得参考
TODO：写死锁demo
### 2.7.多线程之间切换访问引起的crash
> 
多线程引起的崩溃大部分是因为使用数据库的时候多线程同时读写数据库而造成了crash。
[多线程导致的iOS闪退分析](http://blog.csdn.net/lixing333/article/details/42149893)这篇文章就是关于多线程crash的调试。
## 3.参考文章及源码
**文章**
[iOS SQLite数据库迁移](http://www.jianshu.com/p/c19dd08697bd)
[如何定位Obj-C野指针随机Crash(一)：先提高野指针Crash率](http://bugly.qq.com/bbs/forum.php?mod=viewthread&tid=31&highlight=%E5%A6%82%E4%BD%95%E5%AE%9A%E4%BD%8DObj-C%E9%87%8E%E6%8C%87%E9%92%88%E9%9A%8F%E6%9C%BACrash)
[iOS如何查看崩溃信息](http://www.jianshu.com/p/a7027c69e2ce)
[分析iOS Crash文件：符号化iOS Crash文件的3种方法](http://wufawei.com/2014/03/symbolicating-ios-crash-logs/)
[使用SIGNAL让APP能够在从容崩溃](http://www.cnphp6.com/archives/109952)
[Segmentation fault](https://en.wikipedia.org/wiki/Segmentation_fault)对于Signal崩溃的解释
[Exception Types in iOS crash logs](http://stackoverflow.com/questions/7446655/exception-types-in-ios-crash-logs) stackoverflow上关于Signal的问题
**源码**
[如何从容的崩溃源码](https://github.com/walkdianzi/DSSignalHandlerDemo)截获异常崩溃弹出alert
[DSCrashDemo](https://github.com/walkdianzi/DSCrashDemo)关于一些崩溃例子的收集
文／齐滇大圣（简书作者）
原文链接：http://www.jianshu.com/p/1b804426d212
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
