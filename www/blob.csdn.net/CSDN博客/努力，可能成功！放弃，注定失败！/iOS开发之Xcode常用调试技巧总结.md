# iOS开发之Xcode常用调试技巧总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月07日 17:28:24[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1442
NSLog,po命令和普通断点调试相信每个iOS开发者都会，这里就不作介绍了。
#### 一、Memory Graph
Xcode8新增：Memory Graph解决闭包引用循环问题
![](http://upload-images.jianshu.io/upload_images/1968278-4fa239dc98bba959.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个时候就进入了断点模式，可以查看issue面板，注意选择右边Runtime：
![](http://upload-images.jianshu.io/upload_images/1968278-5f7f21eac7b560b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
有很多叹号说明就有问题了。看内存中object的名字，有一条是Closure captures leaked。展开后点击就可以看到这个issue对应的内存图形展示在中间的面板中。当然了，我们更多的时候是在debug页面下查看：
![](http://upload-images.jianshu.io/upload_images/1968278-c7be1cd441c42075.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
所以，这里面引用循环了。点击紫色的叹号会出现Xcode分析出来的内存引用图形：
![](http://upload-images.jianshu.io/upload_images/1968278-62cd9bb2d174e5dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
有了这个图就很容易看出来了：这里有个引用循环
顺便提一下，通过在Arguments中设置参数，打印出App加载的时长，包括整体加载时长，动态库加载时长等。
在Environment Variables中添加DYLD_PRINT_STATISTICS字段，并设置为YES，在控制台就会打印加载时长。
![](http://upload-images.jianshu.io/upload_images/1968278-3b6f46441814a7e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Edit Scheme
![](http://upload-images.jianshu.io/upload_images/1968278-9d29c0c72f611d33.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
HYbfCQTYAJMgAkw
**二、Xcode调试技巧之：LLDB**
1、po：print object的缩写，表示显示对象的文本描述，如果对象不存在则打印nil。
简单的打印一个对象我们就不说了，我们来说说特殊的应用场景吧！
应用场景：你想知道一个视图包含了哪些子视图。当然你可以循环打印子视图，但是下面只需要一个命令即可解决。
输出视图层级关系（这是一个被隐藏的命令）:
> 
po [[self view] recursiveDescription]
还有个常见的调试场景，比如你要打印一个model。你直接用NSLog或po对象处理的结果是model的地址，这并不是我们想要的。怎么办？有没有解决方法呢？
答案是有的。你可以重写model里面的description方法。但是，如果model里属性非常多，这样就不适用了。你不可能说在description方法里面拼接属性返回。这样不仅麻烦，而且可读性非常差。到这里，我们可以利用runtime动态获取属性并返回。不过我并不建议你重写description方法，我推荐你重写debugDescription方法（至于详细的介绍以及如何重写[请点击此处](http://ms.csdn.net/share/14C7310B9DBCF3E650907B00695DBD79_1_IPHONE_APP)），因为debugDescription方法和description方法效果一样，区别在于debugDescription方法是在你使用po命令时调用的，实际上也是调用了description方法。
2、p:可以用来打印基本数据类型。
3、call:执行一段代码
> 
call NSLog(@
"%@"
, @
"yang"
)
4、expr:动态执行指定表达式
> 
expr i = 101
输出：(int)$0 = 101
5、bt:打印当前线程堆栈信息
如果要打印所以线程堆栈信息，使用:bt all即可。
6、image:常用来寻找栈地址对应代码位置：
举个栗子：
应用场景（数组越界）模拟代码：
> 
NSArray *array = @[@"yang",@"she",@"bing"];
NSLog(@"%@",array[3]);
错误信息如下：
> 
***Terminating app due to uncaught exception'NSRangeException', reason:'*** -[**NSArrayI objectAtIndex:]: index 3 beyond bounds [0 .. 2]'*** Firstthrowcall stack:(0 CoreFoundation 0x000000010579734b **exceptionPreprocess + 171
1 libobjc.A.dylib 0x00000001051f821e objc_exception_throw + 48
2 CoreFoundation 0x00000001056d1eeb -[__NSArrayI objectAtIndex:] + 155
3 BGMultimediaDemo 0x0000000104c25550 -[ViewController viewDidLoad] + 192
4 UIKit 0x0000000105d5c06d -[UIViewController loadViewIfRequired] + 1258
......
......
......
21 BGMultimediaDemo 0x0000000104c25adf main + 111
22 libdyld.dylib 0x000000010857268d start + 1
23 ??? 0x0000000000000001 0x0 + 1
)
libc++abi.dylib: terminating
with
uncaught exception of type NSException
这个时候我们如果怀疑出错的地址是0x0000000104c25550，那么我们可以使用下面命令来找出错误代码的位置：
> 
image lookup --address 0x0000000104c25550
执行命令后输出结果如下：
> 
Address: BGMultimediaDemo[0x0000000100001550] (BGMultimediaDemo.**TEXT.**text + 192)
Summary: BGMultimediaDemo`-[ViewController viewDidLoad] + 192 at ViewController.m:30
从上面输出结果中可以看出，错误位置应该是ViewController.m文件中的30行。是不是超级好用？反正我觉得好用。
**三、Xcode调试技巧之：断点（Breakpoint）**
断点，程序员Debug必备技之一。
1、条件断点
打上断点之后，对断点进行编辑，设置相应过滤条件。下面简单的介绍一下条件设置：
Condition：返回一个布尔值，当布尔值为真触发断点，一般里面我们可以写一个表达式。
Ignore：忽略前N次断点，到N+1次再触发断点。
Action：断点触发事件，分为六种：
AppleScript：执行脚本。
Capture GPU Frame：用于OpenGL ES调试，捕获断点处GPU当前绘制帧。
Debugger Command：和控制台中输入LLDB调试命令一致。
Log Message：输出自定义格式信息至控制台。
Shell Command：接收命令文件及相应参数列表，Shell Command是异步执行的，只有勾选“Wait until done”才会等待Shell命令执行完在执行调试。
Sound：断点触发时播放声音。
这些功能平时在调试程序的过程中都可以进行尝试，说实话我用的设置Condition项会较多些。
Options(Automatically continue after evaluating actions选项)：选中后，表示断点不会终止程序的运行。
![](http://upload-images.jianshu.io/upload_images/1968278-2219ee870c0a2585.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988185595536.jpg
2、异常断点
异常断点可以快速定位不满足特定条件的异常，比如常见的数组越界，这时候很难通过异常信息定位到错误所在位置。这个时候异常断点就可以发挥作用了。
添加异常断点：
![](http://upload-images.jianshu.io/upload_images/1968278-02c11b2668c621ea.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988201696957.jpg
编辑异常断点：
![](http://upload-images.jianshu.io/upload_images/1968278-fdccfeb59a8a8ba5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988213410670.jpg
Exception：可以选择抛出异常对象类型：OC或C++。
Break：选择断点接收的抛出异常来源是Throw还是Catch语句。
3、符号断点
符号断点的创建方式和异常断点一样一样的，在符号断点中可以指定要中断执行的方法：
举个例子，常见的场景，我想让它执行到ViewController类中的viewWillAppear方法就中断执行：
![](http://upload-images.jianshu.io/upload_images/1968278-dfd1811e946f7301.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988250339451.png
Symbol:[ViewController viewWillAppear:]即[类名 方法名]可以执行到指定类的指定方法中开始断点。如果只有viewWillAppear:即方法名，它会执行到所以类中的viewWillAppear:方法中开始断点。
4、监控断点
我们调试程序的大部分时候都是为了监控某个变量的变化，在代码中变量出现的地方添加断点不仅累而且还可能漏掉，事后还得一个一个删掉，实在很累。
我们可以通过为变量添加监控断点来简单地做到这一点。找到变量第一次出现的地方，添加一个普通断点，进入 debug 模式后在 Variables View 中右键变量，选择 Watch 变量名。这样，每一次该变量被改变都会触发断点告知我们。
![](http://upload-images.jianshu.io/upload_images/1968278-741c84425a185012.png-autofit?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2016-08-17_Screen Shot 2016-08-17 at 23.55.10.png
**四、Xcode调试技巧之：EXC_BAD_ACCESS**
1、开启僵尸对象
开启Zombie模式之后会导致内存上升，因为所以已经被释放（引用计数为0）的对象被僵尸对象取代，并未真的释放掉。这个时候再给僵尸对象发送消息，就会抛出异常，并打印出异常信息，你可以轻松的找到错误代码位置，结束Zombies时会释放。它的主要功能是检测野指针调用。
使用方法：
“Edit Scheme…” —> “Run” —> “Diagnostics” —> “Zombie Objects”
打开”Edit Scheme…”窗口：
![](http://upload-images.jianshu.io/upload_images/1968278-442559728571c82b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988279182828.png
开启Zombie模式：
![](http://upload-images.jianshu.io/upload_images/1968278-36e0bc45fb8c591b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988284639020.png
注意：Zombie模式不能再真机上使用，只能在模拟器上使用。
2、Address Sanitizer（地址消毒剂）
在Xcode7之后新增了AddressSanitizer工具，为我们调试EXC_BAD_ACCESS错误提供了便利。当程序创建变量分配一段内存时，将此内存后面的一段内存也冻结住，标识为中毒内存。程序访问到中毒内存时（访问越界），立即中断程序，抛出异常并打印异常信息。你可以根据中断位置及输出的Log信息来解决错误。当然，如果变量已经释放了，它所占用的内存也会被标识为中毒内存，这个时候访问这片内存空间同样会抛出异常。
使用方法：
“Edit Scheme…” —> “Run” —> “Diagnostics” —> “Address Sanitizer”
![](http://upload-images.jianshu.io/upload_images/1968278-5ac55075463125e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1477988306785766.png
开启AddressSanitizer之后，在调试程序的过程中，如果有遇到EXC_BAD_ACCESS错误，程序则会自动终端，抛出异常。
3.Analyze分析器
Analyze分析器是一种静态的工具，可以对我们的程序进行分析，找出我们未使用的变量，或一些死存储。执行Analyze如下：Product-->Analyze. 如下蓝色的标记就是静态分析的结果。
![](http://upload-images.jianshu.io/upload_images/1968278-97924e57e4eb9c25.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1968278-989093f061d81f47.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
当然，我们可以设置在编译程序的时候同时Analyze，把下列选项设为Yes即可。
![](http://upload-images.jianshu.io/upload_images/1968278-367a79cd7f3bc1a9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 5、视图调试
如今iOS开发的UI设计有很多种方式，比如storyboard，xib，代码实现。对于stoayboard,xib可视化实现是比较简单的，但是对于一些“iOS老程序员”而言，都喜欢使用代码实现UI，并且可能UI层次还比较复杂。这样就给我们新接手项目的开发者带来很多困扰。如何快速查看一个复杂UI的界面层次和布局，最快的方法就是用到视图调试。
当项目运行到某一个界面（可以是模拟器或真机）时，开启视图调试，点击按钮如图：
![](http://upload-images.jianshu.io/upload_images/1968278-84ca133ec3b9794d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样就会进入试图调试，你可以很方便的查看这个界面。这里可以看到控件之间的层次关系。
**六、参考博客地址：**
instruments这里不做介绍，请参考：
[instruments来检验你的app](http://www.cocoachina.com/industry/20140114/7696.html)
[Memory Graph](http://www.jianshu.com/p/f792f9aa2e45)
[iOS 开发调试技巧](http://www.jianshu.com/p/9c4c92a38468)
[在Xcode 7上直接使用Clang Address Sanitizer](http://www.cocoachina.com/ios/20150730/12830.html)
[Xcode 调试技巧](http://qingbo.info/2016/08/17/xcode-debugging/)
[Xcode Debug技巧](http://dev.guanghe.tv/2015/12/iOSXcodeDebug.html)
文／wu大维（简书作者）
原文链接：http://www.jianshu.com/p/d8bc3d74dc3e
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
