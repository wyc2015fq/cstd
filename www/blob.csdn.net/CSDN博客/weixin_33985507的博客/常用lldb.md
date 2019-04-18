# 常用lldb - weixin_33985507的博客 - CSDN博客
2017年06月15日 16:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### Chapter 1 Gettting Started 概览
常用寄存器（x86_64)
• FirstArgument:RDI
• SecondArgument:RSI
• ThirdArgument:RDX
• FourthArgument:RCX
• FifthArgument:R8
• SixthArgument:R9
- 返回值RAX
常用寄存器 (arm64)
- x1
### 真机
```
[Tab 1]
$ iproxy 5000 22
[Tab 2]
$ iproxy 1234 1234
[Tab 3]
$ ssh root@localhost -p 5000
root# ps -e | grep Preferences
root# debugserver *:1234 -attach=40776
[Tab 4]
lldb
(lldb) process connect connect://localhost:1234
```
```
[Terminal 1]
$ tty
/dev/ttys003
[Terminal 2]
(lldb) file /Applications/Xcode.app/Contents/MacOS/Xcode
(lldb) process launch -e /dev/ttys003
(Ctrl + C)
(lldb) breakpoint set -n "-[NSView hitTest:]"
Breakpoint 1: where = AppKit`-[NSView hitTest:], address =
0x000000010338277b
(lldb) continue
Process 9532 resuming
(make sure the breakpoint is triggered)
Process 9532 resuming
Process 9532 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x00007fff817becfb AppKit`-[NSView hitTest:]
AppKit`-[NSView hitTest:]:
->  0x7fff817becfb <+0>: pushq  %rbp
    0x7fff817becfc <+1>: movq   %rsp, %rbp
    0x7fff817becff <+4>: pushq  %r15
    0x7fff817bed01 <+6>: pushq  %r14
# po is usually more useful as it gives the NSObject’s description or debugDescription, if available.
(lldb) po $rdi
<_NSThemeCloseWidget: 0x11ca41f80>
# 给断点添加条件
(lldb) breakpoint modify 1 -c "(BOOL)[$rdi isKindOfClass:[NSTextView class]]"
（触发断点后）
(lldb) po [$rdi superclass]
# 用lldb查看私有API r-使用正则表达式；n-通过名字寻找函数或符号
(lldb) image lookup -rn 'DVTSourceTextView\ '
# 通过block注入来swizzle
(lldb) po @import Foundation
(lldb) po
Enter expressions, then terminate with an empty line to evaluate:
@import Cocoa;
id $class = [NSObject class];
SEL $sel = @selector(init);
void *$method = (void *)class_getInstanceMethod($class, $sel);
IMP $oldImp = (IMP)method_getImplementation($method);
id (^$block)(id) = ^id(id object) {
  if ((BOOL)[object isKindOfClass:[NSView class]]) {
    fprintf(stderr, "%s\n", (char *)[[[object class] description]
UTF8String]);
  }
  return object;
};
IMP $newImp = (IMP)imp_implementationWithBlock($block);
method_setImplementation($method, $newImp);
# 善于用help和apropos查看文档
(lldb) help breakpoint name
# will do a case-insensitive search for any word or string against the LLDB documentation and return any matching results.
(lldb) apropos swift
```
### Chapter 3: Attaching with LLDB
LLDB attaching实际是个误导人的词。`debugserver`才是真正负责attaching到进程的。
```
# attach到已存在进程
$ lldb -n Xcode
$ lldb -p `pgrep -x Xcode`
# attach到未存在进程
$ lldb -n Finder -w  # LLDB会等待直到一个叫Finder的进程下次启动
# attach到自己想要的进程。这种方式有个弊端是进程的stderr output (i.e. NSLog & company)回自动发送到Terminal里面去。
$ lldb -f /System/Library/CoreServices/Finder.app/Contents/MacOS/Finder
(lldb) process launch
# Options while launching
$ lldb -f /bin/ls
(lldb) process launch -w /Applications   # 改变当前工作目录，等于cd再ls
(lldb) process launch -- /Applications  # 给ls传参，等于 ls /Applications
(lldb) process launch -- ~/Desktop  # ls会报错，这个目录不存在
(lldb) process launch -X true -- ~/Desktop  # 用-X true来展开任何shell符号
(lldb) run ~/Desktop  # run 等价于 process launch -X true --
(lldb) process launch -o /tmp/ls_output.txt -- /Applications
(lldb) target delete
(lldb) target create /usr/bin/wc
$ echo "hello world" > /tmp/wc_input.txt
(lldb) process launch -i /tmp/wc_input.txt  # 等价于 wc < /tmp/wc_input.txt
(lldb) run  # 不想要stdin，程序会一直在那等着，输入内容，Ctrl+D结束输入
(lldb) process launch -n  # -n告诉LLDB不创建stdin，于是wc会立即结束突出
```
### Chapter 4: Stopping in Code
```
# 检查一个函数是否存在
(lldb) image lookup -n "-[UIViewController viewDidLoad]"
1 match found in /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk//System/Library/Frameworks/UIKit.framework/UIKit:
        Address: UIKit[0x00000000001ca654] (UIKit.__TEXT.__text + 1867588)
        Summary: UIKit`-[UIViewController viewDidLoad]
(lldb) image lookup -rn test  # 正则查找在任何包含有test（大小写匹配）的函数或框架
# 然后添加断点
(lldb) b -[UIViewController viewDidLoad]
(lldb) rb '\-\[UIViewController\ '  # rb = breakpoint set -r %1
(lldb) breakpoint delete  # 删除所有断点
(lldb) rb '\-\[UIViewController(\(\w+\))?\ '  # 找出包括category方法的有所UIVC的实例方法
(lldb) rb . -f DetailViewController.swift  # -f域限制，这个为所有的属性getter/setter，block，category，方法或函数添加断点
(lldb) rb .  # 这个是非常疯狂的，为所有一切添加断点
(lldb) rb . -s UIKit  # 限制在某个library，为这个库的所有东西添加断点
(lldb) rb . -s UIKit -o  # 断点只会被触发一次然后被删除
```
#### 修改或删除断点
```
(lldb) b main
  Breakpoint 1: 20 locations.
(lldb) breakpoint list
(lldb) breakpoint list 1
(lldb) breakpoint list 1 -b
(lldb) breakpoint list 1 3
(lldb) breakpoint list 1-3  # 以上也适用delete
(lldb) breakpoint delete 1
(lldb) breakpoint delete 1,1
```
### Chapter 5: Expression
关于`expression`命令的介绍，用它你可以在debugger中执行任意代码
#### Formatting p & po
'p'   = 'expression --'
'po' = 'expression -O  --'  # -O用来打印object的description
```
(lldb) p self
(Signals.MasterViewController) $R3 = 0x00007fc08860e960 {
  UIKit.UITableViewController = {
    baseUIViewController@0 = <extracting data from value failed>
    _tableViewStyle = 0
    _keyboardSupport = nil
    _staticDataSource = nil
    _filteredDataSource = 0x0000608000245e20
    _filteredDataType = 0
  }
  detailViewController = nil
}
(lldb) type summary add Signals.MasterViewController --summary-string "Wahoo!"
(lldb) p self
(Signals.MasterViewController) $R5 = 0x00007fc08860e960 Wahoo!
(lldb) type summary clear
```
#### Swift vs Objective-C debugging contexts
调试oc代码时，lldb便是oc语法；调试swift代码时，lldb便是swift语法。
有个例外时非断点停止而是主动点击暂停按钮(pause the application out of the blue)，进入的context一定是OC，不管app是不是swift
```
# 在swift环境强制使用oc语法
(lldb) expression -l objc -O -- [UIApplication sharedApplication]
```
#### User defined variables
一定要带上美元符号
oc环境创建的变量，最好还是不要用于swift环境，行为是未定义的，Apple的开发者正在开发这个语境
```
(lldb) po id $test = [NSObject new]
(lldb) po $test
<NSObject: 0x60000001d190>
(lldb) expression -l swift -O -- $test
<NSObject: 0x60000001d190>
(lldb) exppression -l swift -O -- $test.description
error: <EXPR>:3:1: error: use of unresolved identifier '$test'
$test.description
^~~~~
# 随便在一个类的实例方法里面设置一个断点（最好是ViewController的，这样易于观察实验结果），run并命中断点
(lldb) p self  # lldb会赋予一个变量 $R0 引用它，请恢复执行，并手动停止
(lldb) po $R0.title  # 记住上面的内容，主动停止会进入oc环境
error: use of undeclared identifier '$R0'
(lldb) expression -l swift -- $R0.title
(String?) $R1 = "Quarterback"  # 会打印出ViewController的title！
(lldb) expression -l swift -- $R0.title = "💩💩💩💩💩 "  # ⌘ + ⌃ + space然后输入poop可以找到这个emoji
# 恢复执行你就可以看到ViewController的title变成五坨翔啦。这个东西特别好用，当你需要观察给一个方法传一些特殊的值观察对应行为的时候
# 假设上面你下的断点在viewDidAppear函数里，暂停执行
(lldb) expression -l swift -O -- $R0.viewDidLoad()  
# 你会发现断点并没有命中！这是因为lldb默认忽视任何断点。你可以通过-i来打开
(lldb) expression -l swift -O -i 0 -- $R0.viewDidLoad()
# 现在断点可以正常的被触发了。这个技巧是一个很好的用来测试函数的方法。比如，你可以实现test-driven debugging，通过给一个函数传递不同的参数来观察它是如何处理不同的输入
```
#### Type formatting类型格式化
LLDB有一个很不错的功能是格式化基本类型数据，这使得LLDB是一个很好的
```
# 主动停止app
(lldb) expression -G x -- 10  # G代表GDB
(int) $0 = 0x0000000a
(lldb) p/x 10  # /x 指定用十六进制格式
(int) $1 = 0x0000000a
(lldb) p/t 10  # /t 指定用二进制格式
(int) $2 = 0b00000000000000000000000000001010
(lldb) p/t -10
(int) $3 = 0b11111111111111111111111111110110
(lldb) p/t 10.0
(double) $4 = 0b0100000000100100000000000000000000000000000000000000000000000000
(lldb) p/d 'D'  # /d 指定用十进制格式
(char) $5 = 68
(lldb) p/c 1430672467  # /c 指定用char格式
(int) $6 = STFU  # 将这个整型转换成二进制，并分成4个byte，把每个byte转换成ASCII码
```
完整的输出格式化列表如下：
• x: hexadecimal
• d: decimal
• u: unsigned decimal
• o: octal
• t: binary
• a: address
• c: character constant
• f: float
• s: string
如果这些格式化还不能够满足你，你可以使用lldb额外的formatters，但此时你不可以使用GDB的格式化语法了：
```
(lldb) expression -f Y -- 1430672467
(int) $0 = 53 54 46 55             STFU
```
### Chapter 6: Thread, Frame & Stepping Around
一个神奇的方法：Control+Shift+StepOver/StepIn
可以控制其他线程保持停止状态，这个在排查棘手的同步问题时特别有用
```
(lldb) thread backtrace
(lldb) bt  # 这两个其实是不一样的，用help验证
(lldb) frame info  # 打印出当前frame
frame #0: 0x000000010f5ce87a Signals`MasterViewController.viewWillAppear(animated=false, self=0x00007fcb3651be10) -> () at MasterViewController.swift:55
(lldb) frame select 1
frame #1: 0x000000010f5cee51 Signals`@objc MasterViewController.viewWillAppear(Bool) -> () at MasterViewController.swift:0
   1    /**
   2     * MasterViewController.swift
   3     *
   4     * Copyright (c) 2017 Razeware LLC
   5     *
   6     * Permission is hereby granted, free of charge, to any person obtaining a copy
   7     * of this software and associated documentation files (the "Software"), to deal
(lldb) run  # 重新启动app，Xcode不需要重编译
(lldb) next  # step over
(lldb) step  # step into Xcode默认next如果没有debug symbols
(lldb) settings show target.process.thread.step-in-avoid-nodebug
target.process.thread.step-in-avoid-nodebug (boolean) = true
(lldb) step -a0  # 忽视设置，不管有无debug symbols都进入
(lldb) finish  # step out 回车lldb会执行上一条指令
(lldb) next/step --run-mode/-m  # 控制其余线程是否停止
```
#### 查看栈上数据
一个非常有用的frame子命令是`frame variable`。这个命令会用在可执行文件头部中（或dSYM如果你的app被stripped了）的debug symbol信息来dump出关于那个栈帧的信息。有了这个debug信息，这个命令只要带上恰当的参数，你很容易就知道函数中所有变量（包括程序中的全局变量）的作用域
```
(lldb) frame variable  # 查看当前栈帧的变量
(Bool) animated = false
(Signals.MasterViewController) self = 0x00007f94de713760 {
  UIKit.UITableViewController = {
    baseUIViewController@0 = <extracting data from value failed>
    _tableViewStyle = 0
    _keyboardSupport = 0x000060800002aa00
    _staticDataSource = nil
    _filteredDataSource = 0x000060800005bcc0
    _filteredDataType = 0
  }
  detailViewController = nil
}
(CGFloat) bottomInset = 4.9406564584124654E-324
(lldb) frame info
  (lldb) frame variable -F self  # 查看所有私有变量，F是flat的意思
```
### Chapter 7: Image
block所在函数的参数是如何传给block的呢？
编译器能够分析出哪些参数是block需要的，然后创建一个以这些参数为参数的函数。但block被调用时，实际上是这个函数被调用了。
```
(lldb) image list
(lldb) image list Foundation
(lldb) image dump symtab UIKit -s address  # dump all the symbol table informaton available for UIKit
(lldb) image lookup -n "-[UIViewController viewDidLoad]"
1 match found in /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk//System/Library/Frameworks/UIKit.framework/UIKit:
        Address: UIKit[0x00000000001ca9a4] (UIKit.__TEXT.__text + 1868340)
        Summary: UIKit`-[UIViewController viewDidLoad]
(lldb) image lookup -rn UIViewController
(lldb) image lookup -rn '\[UIViewController\ '
(lldb) image lookup -rn '\[UIViewController\(\w+\)\ '  # 包括category的方法
(lldb) image lookup -rn _block_invoke
(lldb) image lookup -rn _block_invoke ImageName
# 给一个block下断点，观察block的frame variable
(lldb) frame variable
(__block_literal_5 *)  = 0x0000600000269c00
(int) sig = 23
(siginfo_t *) siginfo = 0x00007fff55a67588
(UnixSignalHandler *) self = 0x0000608000262c80
(UnixSignal *) unixSignal = 0x000000010eb3cd08
(__NSCFConstantString *) kSignalHandlerCountUpdatedNotification = 0x000000010a25e488 @"com.razeware.breakpoints.contentupdated"
(lldb) image dump symfile ImageName
# 很多内容出来，文本查找__block_literal_5
0x7fb975802760:   Type{0x100000ce6} , name = "__block_literal_5", size = 52, decl = UnixSignalHandler.m:123, compiler_type = 0x00007fb979082860 struct __block_literal_5 {
        void *__isa;
        int __flags;
        int __reserved;
        void (*__FuncPtr)();
        __block_descriptor_withcopydispose *__descriptor;
        UnixSignalHandler *const self;
        siginfo_t *siginfo;
        int sig;
    }
这个就是定义了block的东东啦
(lldb) po (__block_literal_5 *)0x0000600000269c00
<__NSMallocBlock__: 0x600000269c00>
(lldb) p/x ((__block_literal_5 *)0x0000600000269c00)->__FuncPtr
(void (*)()) $2 = 0x000000010a259810 (Commons`__38-[UnixSignalHandler appendSignal:sig:]_block_invoke_2 at UnixSignalHandler.m:123)
(lldb) image lookup -a 0x000000010a259810
      Address: Commons[0x0000000000002810] (Commons.__TEXT.__text + 2352)
      Summary: Commons`__38-[UnixSignalHandler appendSignal:sig:]_block_invoke_2 at UnixSignalHandler.m:123
(lldb) po ((__block_literal_5 *)0x0000600000269c00)->sig
23
```
**使用`image dump symfile ImageName`命令来探究一个未知的数据类型是如何工作的，这是一个很好的手段。这个命令也可以让我们了解编译器如何生成目标代码。除此之外，你也可以通过它来检查block是怎么引用block外部的指针来调试内存循环引用问题。**
```
(lldb) image lookup -rn __NSMallocBlock__  # 没有任何输出，说明它没有重写父类的任何方法
(lldb) po [__NSMallocBlock__ superclass]
__NSMallocBlock
(lldb) image lookup -rn __NSMallocBlock  # 输出的方法看起来都和内存管理有关
(lldb) po [__NSMallocBlock superclass]
NSBlock
(lldb) image lookup -rn 'NSBlock\ '
6 matches found in /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk//System/Library/Frameworks/CoreFoundation.framework/CoreFoundation:
        Address: CoreFoundation[0x00000000000400c0] (CoreFoundation.__TEXT.__text + 257696)
        Summary: CoreFoundation`-[NSBlock copy]        Address: CoreFoundation[0x0000000000078330] (CoreFoundation.__TEXT.__text + 487696)
        Summary: CoreFoundation`-[NSBlock copyWithZone:]        Address: CoreFoundation[0x00000000001892b0] (CoreFoundation.__TEXT.__text + 1605776)
        Summary: CoreFoundation`+[NSBlock allocWithZone:]        Address: CoreFoundation[0x00000000001892d0] (CoreFoundation.__TEXT.__text + 1605808)
        Summary: CoreFoundation`+[NSBlock alloc]        Address: CoreFoundation[0x00000000001892f0] (CoreFoundation.__TEXT.__text + 1605840)
        Summary: CoreFoundation`-[NSBlock invoke]        Address: CoreFoundation[0x0000000000189300] (CoreFoundation.__TEXT.__text + 1605856)
        Summary: CoreFoundation`-[NSBlock performAfterDelay:]
# 接下来我们想探究下invoke这个方法，但是我们调用invoke之后不希望这个block被内存管理给释放掉。可以这样处理
(lldb) po id $block = (id)0x0000600000269c00
(lldb) po [$block retain]
<__NSMallocBlock__: 0x600000269c00>
(lldb) po [$block invoke]
2017-07-11 10:38:05.075 Signals[507:5687492] Appending new signal: SIGIO
(lldb) image lookup -rn (?i)\ _\w+description\]  # (?i) means case insensitive
(lldb) image lookup -rn NSObject\(IvarDescription\)
这3个方法非常吸引人：
_ivarDescription
_propertyDescription 
_methodDescription
(lldb) po [[UIApplication sharedApplication] _ivarDescription]  # 可以发现一个叫UIStatusBar的私有类
(lldb) image lookup -rn '\[UIStatusBar\ set'
(lldb) po [[UIApplication sharedApplication] statusBar]
<UIStatusBar: 0x7fcb42809e00; frame = (0 0; 414 20); opaque = NO; autoresize = W+BM; layer = <CALayer: 0x608000032400>>
(lldb) po [0x7fcb42809e00 setBackgroundColor:[UIColor purpleColor]]  # statusBar就会变成紫色的啦
```
### Chapter 8: Persisting & Customizing Commands 持久化和自定义命令
#### 如何持久化
LLDB启动时会在几个目录里面寻找初始化文件，如果找到则会被加载到LLDB中，加载时机是在LLDB启动后attach到process之前。
LLDB将会在以下几个地方寻找初始化文件：
1.**`~/.lldbinit-[context]`**。其中context为Xcode或者lldb。即如果只想生效于Xcode中的lldb，用`~/.lldbinit-Xcode`，如果只想生效与命令行中的lldb，用`~/.lldbinit-lldb`
- 接下来LLDB就会搜索在`~/.lldbinit-[context]`中找到的内容。这个是最理想的文件，大部分情形我们还是希望两种场景下都使用
- LLDB将会搜索被启动时的目录。不幸的是，Xcode启动LLDB在`/`目录
#### 创建.lldbinit文件
```
# 注意这里的lldb context应该是Swift，所以才强制为oc语法
(lldb) command alias -- Yay_Autolayout expression -l objc -O -- [[[[[UIApplication sharedApplication] keyWindow] rootViewController] view] recursiveDescription]
(lldb) command alias cpo expression -l objc -O --
```
### Chapter 9: Regex Commands
alias只能方便我们处理静态的命令，如果命令有接受输入的话alias就显得很捉急了。
```
# 对新命令后面的内容进行正则替换。下面这个命令alias是无法完成的
(lldb) command regex rlook 's/(.+)/image lookup -rn %1/'  # %1指的是被匹配的内容
(lldb) rlook F00  # 等同于 image lookup -rn FOO, 正则表达式是从rlook之后开始匹配
(lldb) rl viewDidLoad  # 等同于 rlook viewDidLoad
(lldb) rl viewDidLoad Signals
# 来点高级点的
(lldb) command regex -- tv 's/(.+)/expression -l objc -O -- @import QuartzCore; [%1 setHidden:!(BOOL)[%1 isHidden]]; (void)[CATransaction flush];/'
(lldb) tv [[[UIApp keyWindow] rootViewController] view]  # 重复这个命令，手机就会黑屏、恢复黑屏一直循环
(lldb) command regex getcls 's/(([0-9]|\$|\@|\[).*)/cpo [%1 class]/'
(lldb) getcls @"hello world"
__NSCFString
(lldb) getcls @[@"hello world"] 
__NSSingleObjectArrayI
(lldb) getcls [UIDevice currentDevice] 
UIDevice
(lldb) cpo [UIDevice currentDevice] 
<UIDevice: 0x60800002b520>
(lldb) getcls 0x60800002b520
UIDevice
(lldb) command regex getcls 's/(([0-9]|\$|\@|\[).*)/cpo [%1 class]/' 's/(.+)/expression -l swift -O -- type(of: %1)/'
(lldb) getcls self
Signals.MasterViewController
```
