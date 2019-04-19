# iOS微信内存监控 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年03月21日 21:58:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：99标签：[iOS 资源大全																[iOS学习资料																[内存																[ios](https://so.csdn.net/so/search/s.do?q=ios&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
**作者：杨津，腾讯移动客户端开发 高级工程师**
商业转载请联系腾讯WeTest获得授权，非商业转载请注明出处。 
**原文链接：[wetest.qq.com/lab/view/36…](https://link.juejin.im?target=http%3A%2F%2Fwetest.qq.com%2Flab%2Fview%2F367.html%3Ffrom%3Dcontent_juejin)**
## WeTest 导读
目前iOS主流的内存监控工具是Instruments的Allocations，但只能用于开发阶段。本文介绍如何实现离线化的内存监控工具，用于App上线后发现内存问题。
FOOM（Foreground Out Of Memory），是指App在前台因消耗内存过多引起系统强杀。对用户而言，表现跟crash一样。Facebook早在2015年8月提出FOOM检测办法，大致原理是排除各种情况后，剩余的情况是FOOM，具体链接：[code.facebook.com/posts/11469…](https://link.juejin.im?target=https%3A%2F%2Fcode.facebook.com%2Fposts%2F1146930688654547%2Freducing-fooms-in-the-facebook-ios-app%2F%E3%80%82)
微信自15年年底上线FOOM上报，从最初数据来看，每天FOOM次数与登录用户数比例接近3%，同期crash率1%不到。而16年年初某东老大反馈微信频繁闪退，在艰难拉取2G多日志后，才发现kv上报频繁打log引起FOOM。接着16年8月不少外部用户反馈微信启动不久后闪退，分析大量日志还是不能找到FOOM原因。微信急需一个有效的内存监控工具来发现问题。
## 一、实现原理
微信内存监控最初版本是使用Facebook的FBAllocationTracker工具监控OC对象分配，用fishhook工具hook malloc/free等接口监控堆内存分配，每隔1秒，把当前所有OC对象个数、TOP 200最大堆内存及其分配堆栈，用文本log输出到本地。该方案实现简单，一天内完成，通过给用户下发TestFlight，最终发现联系人模块因迁移DB加载大量联系人导致FOOM。
**不过这方案有不少缺点：**
1、监控粒度不够细，像大量分配小内存引起的质变无法监控，另外fishhook只能hook自身app的C接口调用，对系统库不起作用；
2、打log间隔不好控制，间隔过长可能丢失中间峰值情况，间隔过短会引起耗电、io频繁等性能问题；
3、上报的原始log靠人工分析，缺少好的页面工具展现和归类问题。
所以二期版本以Instruments的Allocations为参考，着重四个方面优化，分别是数据收集、存储、上报及展现。
### 1.数据收集
16年9月底为了解决ios10 nano crash，研究了libmalloc源码，无意中发现这几个接口： 
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec5b45d5cf95?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
当malloc_logger和__syscall_logger函数指针不为空时，malloc/free、vm_allocate/vm_deallocate等内存分配/释放通过这两个指针通知上层，这也是内存调试工具malloc stack的实现原理。有了这两个函数指针，我们很容易记录当前存活对象的内存分配信息（包括分配大小和分配堆栈）。分配堆栈可以用backtrace函数捕获，但捕获到的地址是虚拟内存地址，不能从符号表dsym解析符号。所以还要记录每个image加载时的偏移slide，这样符号表地址=堆栈地址-slide。
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec5e2eea773d?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
另外为了更好的归类数据，每个内存对象应该有它所属的分类Category，如上图所示。对于堆内存对象，它的Category名是“Malloc ”+分配大小，如“Malloc 48.00KiB”；对于虚拟内存对象，调用vm_allocate创建时，最后的参数flags代表它是哪类虚拟内存，而这个flags正对应于上述函数指针__syscall_logger的第一个参数type，每个flag具体含义可以在头文件找到；对于OC对象，它的Category名是OC类名，我们可以通过hook OC方法+[NSObject
 alloc]来获取：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F68da04c7-642d-4010-bf65-b7e0a51f9b30.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec855fc59a0d?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
但后来发现，NSData创建对象的类静态方法没有调用+[NSObject alloc]，里面实现是调用C方法NSAllocateObject来创建对象，也就是说这类方式创建的OC对象无法通过hook来获取OC类名。最后在苹果开源代码CF-1153.18找到了答案，当**CFOASafe=true并且**CFObjectAllocSetLastAllocEventNameFunction!=NULL时，CoreFoundation创建对象后通过这个函数指针告诉上层当前对象是什么类型：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Fe6b851d0-2736-4790-ad26-e92a286720ae.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec87f3d44550?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
通过上面方式，我们的监控数据来源基本跟Allocations一样了，当然是借助了私有API。如果没有足够的“技巧”，私有API带不上Appstore，我们只能退而求其次。修改malloc_default_zone函数返回的malloc_zone_t结构体里的malloc、free等函数指针，也是可以监控堆内存分配，效果等同于malloc_logger；而虚拟内存分配只能通过fishhook方式。
### **2.数据存储**
**存活对象管理**
APP在运行期间会大量申请/释放内存。以上图为例，微信启动10秒内，已经创建了80万对象，释放了50万，性能问题是个挑战。另外在存储过程中，也尽量减少内存申请/释放。所以放弃了sqlite，改用了更轻量级的平衡二叉树来存储。
伸展树（Splay Tree），也叫分裂树，是一种二叉排序树，不保证树是平衡，但各种操作平均时间复杂度是O(logN)，可近似看作平衡二叉树。相比其他平衡二叉树（如红黑树），其内存占用较小，不需要存储额外信息。伸展树主要出发点是考虑到局部性原理（某个刚被访问的结点下次又被访问，或者访问次数多的结点下次可能被访问），为了使整个查找时间更少，被频繁查询的结点通过“伸展”操作搬移到离树根更近的地方。大部分情况下，内存申请很快又被释放，如autoreleased对象、临时变量等；而OC对象申请内存后紧接着会更新它所属Category。所以用伸展树管理最适合不过了。
传统二叉树是用链表方式实现，每次添加/删除结点，都会申请/释放内存。为了减少内存操作，可以用数组实现二叉树。具体做法是父结点的左右孩子由以往的指针类型改成整数类型，代表孩子在数组的下标；删除结点时，被删除的结点存放上一个被释放的结点所在数组下标。
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F7467582f-cba1-412f-87b8-f97468c32286.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec901822ea85?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
**堆栈存储**
据统计，微信运行期间，backtrace的堆栈有成百万上千万种，在捕获最大栈长64情况下，平均栈长35。如果36bits存储一个地址（armv8最大虚拟内存地址48bits，实际上36bits够用了），一个堆栈平均存储长度157.5bytes，1M个堆栈需要157.5M存储空间。但通过断点观察，实际上大部分堆栈是有共同后缀，例如下面的两个堆栈后7个地址是一样的：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F4ef00822-e16f-4476-8390-fb8e31b9f8aa.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec92c22fcb70?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
为此，可以用Hash Table来存储这些堆栈。思路是整个堆栈以链表的方式插入到table里，链表结点存放当前地址和上一个地址所在table的索引。每插入一个地址，先计算它的hash值，作为在table的索引，如果索引对应的slot没有存储数据，就记录这个链表结点；如果有存储数据，并且数据跟链表结点一致，hash命中，继续处理下一个地址；数据不一致，意味着hash冲突，需要重新计算hash值，直到满足存储条件。举个例子（简化了hash计算）：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F88d996f5-387b-409f-845e-6f33787aab46.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec95631b7f06?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
1）Stack1的G、F、E、D、C、A、依次插入到Hash Table，索引1～6结点数据依次是(G, 0)、(F, 1)、(E, 2)、(D, 3)、(C, 4)、(A, 5)。Stack1索引入口是6
2）轮到插入Stack2，由于G、F、E、D、C结点数据跟Stack1前5结点一致，hash命中；B插入新的7号位置，(B, 5)。Stack2索引入口是7
3）最后插入Stack3，G、F、E、D结点hash命中；但由于Stack3的A的上一个地址D索引是4，而不是已有的(A, 5)，hash不命中，查找下一个空白位置8，插入结点(A, 4)；B上一个地址A索引是8，而不是已有的(B, 5)，hash不命中，查找下一个空白位置9，插入结点(B, 9)。Stack3索引入口是9
经过这样的后缀压缩存储，平均栈长由原来的35缩短到5不到。而每个结点存储长度为64bits（36bits存储地址，28bits储存parent索引），hashTable空间利用率60%+，一个堆栈平均存储长度只需要66.7bytes，压缩率高达42%。
**性能数据**
经过上述优化，内存监控工具在iPhone6Plus运行占用CPU占用率13%不到，当然这是跟数据量有关，重度用户（如群过多、消息频繁等）可能占用率稍微偏高。而存储数据内存占用量20M左右，都用mmap方式把文件映射到内存。有关mmap好处可自行google之。
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F0842c9a8-e092-4449-b506-9ede78d931bd.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec9a0163c389?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
### **3.数据上报**
由于内存监控是存储了当前所有存活对象的内存分配信息，数据量极大，所以当出现FOOM时，不可能全量上报，而是按某些规则有选择性的上报。
首先把所有对象按Category进行归类，统计每个Category的对象数和分配内存大小。这列表数据很少，可以做全量上报。接着对Category下所有相同堆栈做合并，计算每种堆栈的对象数和内存大小。对于某些Category，如分配大小TOP N，或者UI相关的（如UIViewController、UIView之类的），它里面分配大小TOP M的堆栈才做上报。上报格式类似这样：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Faded6983-d17d-4189-a20c-d1b7e984e2bf.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ec9e49d51387?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
### **4.页面展现**
页面展现参考了Allocations，可看出有哪些Category，每个Category分配大小和对象数，某些Category还能看分配堆栈。
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F8a2421ed-d604-4b07-ba7b-063d7288009b.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecb410da22db?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
为了突出问题，提高解决问题效率，后台先根据规则找出可能引起FOOM的Category（如上面的Suspect Categories），规则有：
● UIViewController数量是否异常
● UIView数量是否异常
● UIImage数量是否异常
● 其它Category分配大小是否异常，对象个数是否异常
接着对可疑的Category计算特征值，也就是OOM原因。特征值是由“Caller1”、“Caller2”和“Category, Reason”组成。Caller1是指申请内存点，Caller2是指具体场景或业务，它们都是从Category下分配大小第一的堆栈提取。Caller1提取尽量是有意义的，并不是分配函数的上一地址。例如：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Fcd2aa491-bdb0-40d1-9348-c92c2ef8e221.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecb903622042?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
所有report计算出特征值后，可以对它们进行归类了。一级分类可以是Caller1，也可以是Category，二级分类是与Caller1/Category有关的特征聚合。效果如下：
**一级分类**
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Fde23e735-040a-40a0-b6d2-1fcc5eee9887.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecbc23f88ac7?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
**二级分类**
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Faabebbd3-f030-4978-9b5a-aeee49babdf1.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecbe89033397?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
### **5.运营策略**
上面提到，内存监控会带来一定的性能损耗，同时上报的数据量每次大概300K左右，全量上报对后台有一定压力，所以对现网用户做抽样开启，灰度包用户/公司内部用户/白名单用户做100%开启。本地最多只保留最近三次数据。
## 二、降低误判
先回顾Facebook如何判定上一次启动是否出现FOOM：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F3a16be64-b1e8-4e7a-98bb-cd396821bf96.jpg!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecc422139270?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
1.App没有升级
2.App没有调用exit()或abort()退出
3.App没有出现crash
4.用户没有强退App
5.系统没有升级/重启
6.App当时没有后台运行
7.App出现FOOM
1、2、4、5比较容易判断，3依赖于自身CrashReport组件的crash回调，6、7依赖于ApplicationState和前后台切换通知。**微信自上线FOOM数据上报以来，出现不少误判，主要情况有：**
**ApplicationState不准**
部分系统会在后台短暂唤起app，ApplicationState是Active，但又不是BackgroundFetch；执行完didFinishLaunchingWithOptions就退出了，也有收到BecomeActive通知，但很快也退出；整个启动过程持续5～8秒不等。解决方法是收到BecomeActive通知一秒后，才认为这次启动是正常的前台启动。这方法只能减少误判概率，并不能彻底解决。
**群控类外挂**
这类外挂是可以远程控制iPhone的软件，通常一台电脑可以控制多台手机，电脑画面和手机屏幕实时同步操作，如开启微信，自动加好友，发朋友圈，强制退出微信，这一过程容易产生误判。解决方法只能通过安全后台打击才能减少这类误判。
**CrashReport组件出现crash没有回调上层**
微信曾经在17年5月底爆发大量GIF crash，该crash由内存越界引起，但收到crash信号写crashlog时，由于内存池损坏，组件无法正常写crashlog，甚至引起二次crash；上层也无法收到crash通知，因此误判为FOOM。目前改成不依赖crash回调，只要本地存在上一次crashlog（不管是否完整），就认为是crash引起的APP重启。
**前台卡死引起系统watchdog强杀**
也就是常见的0x8badf00d，通常原因是前台线程过多，死锁，或CPU使用率持续过高等，这类强杀无法被App捕获。为此我们结合了已有卡顿系统，当前台运行最后一刻有捕获到卡顿，我们认为这次启动是被watchdog强杀。同时我们从FOOM划分出新的重启原因叫“APP前台卡死导致重启”，列入重点关注。
## 三、成果
微信自2017年三月上线内存监控以来，解决了30多处大大小小内存问题，涉及到聊天、搜索、朋友圈等多个业务，FOOM率由17年年初3%，降到目前0.67%，而前台卡死率由0.6%下降到0.3%，效果特别明显。
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2F0081e8b8-9968-4d06-aa14-1d46dbf4e184.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecca29284131?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621eccc47b5223f?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
## 四、常见问题
**UIGraphicsEndImageContext**
UIGraphicsBeginImageContext和UIGraphicsEndImageContext必须成双出现，不然会造成context泄漏。另外XCode的Analyze也能扫出这类问题。
**UIWebView**
无论是打开网页，还是执行一段简单的js代码，UIWebView都会占用APP大量内存。而WKWebView不仅有出色的渲染性能，而且它有自己独立进程，一些网页相关的内存消耗移到自身进程里，最适合取替UIWebView。
**autoreleasepool**
通常autoreleased对象是在runloop结束时才释放。如果在循环里产生大量autoreleased对象，内存峰值会猛涨，甚至出现OOM。适当的添加autoreleasepool能及时释放内存，降低峰值。
**互相引用**
比较容易出现互相引用的地方是block里使用了self，而self又持有这个block，只能通过代码规范来避免。另外NSTimer的target、CAAnimation的delegate，是对Obj强引用。目前微信通过自己实现的MMNoRetainTimer和MMDelegateCenter来规避这类问题。
**大图片处理**
举个例子，以往图片缩放接口是这样写的：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Ff4b9815e-f34f-4be1-bf9a-7f4ec9271851.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecd0eb53aa05?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
但处理大分辨率图片时，往往容易出现OOM，原因是-[UIImage drawInRect:]在绘制时，先解码图片，再生成原始分辨率大小的bitmap，这是很耗内存的。解决方法是使用更低层的ImageIO接口，避免中间bitmap产生：
[](https://link.juejin.im?target=https%3A%2F%2Ftesterhome.com%2Fuploads%2Fphoto%2F2018%2Fa5596daa-28a5-45ca-b005-177d4462ee67.png!large)
![](https://user-gold-cdn.xitu.io/2018/3/13/1621ecdbeb114f14?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

**大视图**
大视图是指View的size过大，自身包含要渲染的内容。超长文本是微信里常见的炸群消息，通常几千甚至几万行。如果把它绘制到同一个View里，那将会消耗大量内存，同时造成严重卡顿。最好做法是把文本划分成多个View绘制，利用TableView的复用机制，减少不必要的渲染和内存占用。
> 
### 推荐文章
最后推荐几个iOS内存相关的链接：
● [Memory Usage Performance Guidelines](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.apple.com%2Flibrary%2Fcontent%2Fdocumentation%2FPerformance%2FConceptual%2FManagingMemory%2FManagingMemory.html%23%2F%2Fapple_ref%2Fdoc%2Fuid%2F10000160-SW1)
● [No pressure, Mon!](https://link.juejin.im?target=http%3A%2F%2Fwww.newosxbook.com%2Farticles%2FMemoryPressure.html)
## **腾讯WeTest iOS预审工具**
为了提高IEG苹果审核通过率，腾讯专门成立了苹果审核测试团队，打造出iOS预审工具这款产品。经过1年半的内部运营，腾讯内部应用的iOS审核通过率从平均35%提升到90%+。
现将腾讯内部产品的过审经验，以线上工具的形式共享给各位。在WeTest腾讯质量开放平台上可以在线使用。**点击[wetest.qq.com/product/ios](https://link.juejin.im?target=http%3A%2F%2Fwetest.qq.com%2Fproduct%2Fios%3Ffrom%3Dcontent_juejin) 即可立即体验！**
作者：腾讯WeTest
链接：https://juejin.im/post/5aa79eeaf265da2392360487
来源：掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
