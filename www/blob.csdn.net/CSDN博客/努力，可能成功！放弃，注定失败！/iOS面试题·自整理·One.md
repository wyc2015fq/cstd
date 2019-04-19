# iOS面试题·自整理·One - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月18日 15:49:45[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1373
**——青灯素笺**
****◆**如何解决低内存问题？**
ForExample：将暂时没有展示在Window中的界面销毁，以获得足够的内存；
****◆**POST请求的数据类型有哪些？**
json、xml、二进制、参数拼接；
****◆**请简述你理解的面向对象思想。**
所谓面向对象，举个最简单的例子，就是这样的。
A先生需要一台电脑，但是A先生对电脑配置、性能、品牌等方方面面的知识一窍不通。而A先生恰巧有位经营电脑方面生意的朋友——C先生，C先生对电脑硬件方面很是了解。A先生告诉C先生所需要的电脑大概什么需求、什么价位。C先生就会提供给A先生其所需的电脑。至于中间筛选、购买、讨价还价的过程什么的，A先生完全不用管任何一个环节。只需要付费，就能拿到所需的电脑。
****◆**iOS中frame和bounds的区别？**
frame指的是：该view在父view坐标系统中的位置和大小。（参照点是父亲的坐标系统）
bounds指的是：该view在本身坐标系统中的位置和大小。（参照点是本身坐标系统）
****◆**请简述你理解的内存管理。**
如果你通过分配和初始化（比如[[XXXClass alloc] init]）的方式来创建对象，你就拥有这个对象，需要负责该对象的释放。这个规则在使用NSObject的便利方法new时，也同样适用。
如果你拷贝一个对象，你也拥有拷贝得到的对象，需要负责该对象的释放。
如果你保持一个对象，你就部分拥有这个对象，需要在以后都不再使用的时候释放该对象。
如果你从其他对象那里接收到一个对象，则你不拥有该对象，也不应该释放它。
****◆**如何设计一个缓存类？**
在程序中使用缓存可以极大的提高程序的运行速度,设计一个简单的缓存类并不需要太复杂的逻辑.
简单的只需要3个接口.
1.存对象
2.取对象
3.删除对象
读取对象
·读取对象的时候先读内存
·内存没有就读文件
保存对象
·保存对象的时候前台保存到内存里
·后台存入到文件里
·内存空间不足就需要先释放一块内存
·收到内存不足的消息的时候删除释放所有的内存
****◆**iOS9.0都有哪些新特性？**
1.网络适配-Https
2.后台定位
3.CoreSpotlightSearch
4.User-Interface-Testing用户交互
5.UIStackView
6.Contacts-Framework获取通讯录
7.白名单
8.多任务，iPad分屏
9.UIKit-Dynamics
10.MapKit
11.GameplayKit-Pathfinding
12.GameplayKit-Behaviors
13.CloudKit-Web-Service
****◆**什么是懒加载？**
懒加载就是懒汉模式，只有用到的时候才回去初始化。也可以理解为延时加载。
举个最简单的例子：tableView中图片的加载显示。
一个延时加载，避免内存过高；一个异步加载，避免线程阻塞。
****◆**如何进行网络消息推送？APNS推送(苹果推送服务)的实现过程？APSN和第三方相比有何区别？**
首先应用发送通知，系统弹出提示框询问用户是否允许，当用户允许后向苹果服务器（APNS）请求deviceToken，并由苹果服务器发送给自己的应用，自己的应用将DeviceToken发送给自己的服务器，自己的服务器想要发送网络推送时将deviceToken以及想要推送的信息发送给苹果服务器，苹果服务器将信息发送给应用。
推送信息内容，总容量不超过256个字节；
iOSSDK本身提供的APSN服务器推送，它可以直接推送给目标用户并根据您的方式弹出提示。
优点：不论应用是否开启，都会发送到手机端；
缺点：消息推送机制是苹果服务器端控制，个别时候可能会有延迟，因为苹果服务器也有队列来处理所有的消息请求；
第三方推送机制，普遍使用Socket机制来实现，几乎可以达到及时的发送到目标用户手机端，适用于即时通讯类应用。
优点：实时的，取决于心跳包的节奏；
缺点：iOS系统的限制，应用不能长时间的后台运行，所以应用关闭的情况下这种推送机制不可用。
****◆**添加NSNotificationCenter监听，其后面的object的意义是什么？**
用NSNotificationCenter添加监听者，其后面的object的意义是：监听同一条通知的多个观察者，在通知到达时，它们执行回调的顺序是不确定的，所以我们不能去假设操作的执行会按照添加观察者的顺序来执行。
****◆**论Socket和HTTP协议的区别。**
HTTP协议是基于TCP连接的，是应用层协议，主要是解决如何包装数据。
Socket是对TCP/IP协议的封装，Socket本身并不是协议，而是一个调用接口（API），通过Socket，我们才能使用TCP/IP协议。
HTTP连接：短连接，客户端向服务器发送一次请求，服务器响应后连接断开，节省资源。服务器不能主动给客户端响应（除非采用HTTP长连接技术），iPhone主要使用类NSURLConnection。
Socket连接：长连接，客户端跟服务器端直接使用Socket进行连接，没有规定连接后断开，因此客户端和服务器保持连接通道，双方可以主动发送数据，一般多用于游戏。
Socket默认连接超时时间是30秒，默认大小是8K（理解为一个数据包大小）。
****◆**iOS中数据的持久化存储方式有哪些？**
数据存储的核心都是写文件。
属性列表：只有NSString、NSArray、NSDictionary、NSData可writeToFile，存储依旧是plist文件。
Plis文件可以存储的7种数据类型：array、dictionary、string、bool、data、number。
对象序列化（对象归档）：对象序列化通过序列化的形式，键值关系存储到本地，转化成二进制流。通过runtime实现自动归档/解档。
1.编码（对象序列化）：把不能直接存储到plist文件中的数据，转化为二进制数据NSData，可以存储到本地；
2.解码（对象反序列化）：把二进制数据转化为本来的类型。
****◆**什么是KVC/KVO？它们之间有什么关系？**
数据存储的核心都是写文件。
kvc：键-值编码，是一种间接访问对象的属性使用字符串来标识属性，而不是通过调用存取方法，直接或通过实例变量访问的机制。
很多情况下可以简化程序代码。Apple文档其实给了一个很好的例子。
kvo：键值观察机制，它提供了观察某一属性变化的方法，极大的简化了代码。
具体看到用到过的一个地方是对于按钮点击变化状态的监控。
****◆**请简述服务器连接三次握手。**
第一次握手：客户端发送syn包（syn=j）到服务器，并进入SYN_SEND状态，等待服务器确认；
第二次握手：服务器收到syn包，必须确认客户的SYN（ack=j+1），同事自己也发送一个SYN包，即SYN+ACK包，此时服务器进入SYN+RECV状态；
第三次握手：客户端收到服务器的SYN+ACK包，向服务器发送确认包ACK（ack=k+1），发送完毕，客户端和服务器进入ESTABLISHED状态，完成三次状态。
****◆**层CALayer和UIView的区别是什么？**
两者最大的区别是：图层不会直接渲染到屏幕上。UIView是iOS系统中界面元素的基础，所有的界面元素都是继承自它。它本身完全是由CoreAnimation来实现的。它真正的绘图部分，是由一个CALayer类来管理。UIView本身更像是一个CALayer的管理器。一个UIView上可以有N个CALayer，每个layer显示一种东西，增强UIView的展现能力。
UIView可以响应用户事件，因为它继承自UIResponder。
1.其实UIView之所以能显示在屏幕上，完全是因为它内部的一个图层(即CALayer对象)；
2.UIView本身不具备显示功能，是它内部的层才有显示功能；
3.UIView创建对象时，UIVIew内部会自动创建一个图层
（CALayer对象）通过UIVIew的layer属性可以访问这个层。
4.当UIView需要显示到屏幕上时：
4.1.会调用drawRect：方法进行绘图，并且会将所有内容绘制在自己的图层layer属性上；
4.2.绘图完毕后，系统会将图层拷贝到屏幕上，于是完成了UIView的显示。
****◆**nil、Nil、NULL区别是什么？id和void *区别是什么？**
NULL：是对于C语言的指针而使用的，表示空指针；
nil：对于OC中的对象而使用的，表示对象为空；
Nil：对于OC中的类而使用的，表示类指向空。
id：OC中指针，可以指向所有类型的对象，它是泛型对象指针。
void*：C/C++中的泛型指针。
**◆Objective-C使用什么机制管理对象内存？**
通过retainCount的机制来决定对象是否需要释放。每次runloop的时候，都会检查对象的retainCount，如果retainCount为0，说明该对象没有地方需要继续使用了，可以释放掉了（会被系统自动释放）。
**◆ARC通过什么方式帮助开发者管理内存？**
编译时根据代码上下文，插入retain/release。
**◆不手动指定autoreleasepool的前提下，一个autorelease对象在什么时刻释放？(比如在一个ViewController的viewDidload中创建)**
分两种情况：手动干预释放时机、系统自动去释放。
1.手动敢于释放时机——指定autoreleasepool就是所谓的：当前作用于大括号结束时释放。
2.系统自动去释放——不手动指定autoreleasepool
Autorelease对象会在当前的runloop迭代结束时释放。
如果在一个ViewController的viewDidload中创建一个Autorelease对象，那么该对象会在viewDidAppear方法执行前就被销毁了。
**◆lldb(gdb)常用的调试命令？**
·breakpoint设置断点定位到某一个函数
·n断点指针下一步
·po打印对象
**◆InterfaceBuilder中UserDefinedRuntimeAttributes如何使用？**
它能够通过kvc的方式配置一些你在interfacebuilder中不能配置的属性。当你希望在InterfaceBuilder中做尽可能多的事情，这个特性能够帮助你编写更加轻量级的viewController。
****◆**这段代码有问题吗？如何修改？**
for (int i = 0; i < 10000; ++i) {
NSString *str = @"Abc";
str = [str lowercaseString];
str = [str stringByAppendingString:@"xyz"];
NSLog(@"%@", str);
}
从语法上看，没有任何问题。
但是，确实存在问题。这里考量的是iOS的自动释放池的原理或者说内存管理。
实际，这段代码的问题就是：内存得不到及时的释放。
为什么得不到及时地释放？
因为RunLoop是每个事件循环结束后才会自动释放池去使对象的引用计数减一，对于引用计数为零的对象才会真正被销毁、回收内存。
因此，对于此处的问题。一个for循环执行10000次，会产生10000个临时自动番话对象，一直放到自动释放池中管理，内存得不到回收。
然后，现象是：内存暴涨。
正确的写法是：
for (int i = 0; i < 10000; ++i) {
@autoreleasepool {
NSString *str = @"Abc";
str = [str lowercaseString];
str = [str stringByAppendingString:@"xyz"];
NSLog(@"%@", str);
}
}
****◆**UITableview是如何重用cell的？**
UITableview提供了一个属性：visibleCells，它是记录但潜在屏幕课件的cell，要想重用cell，我们需要明确指定重用标识（identifier）。
当cell滚动出TableView可视范围之外时，就会被放到可重用数组中。当有一个cell滚动出TableView可视范围之外时，同样也会有新的cell要显示到TableView可视区，因此这个新小时出来的cell就会先从可重用数组中通过所指定的identifier来获取，如果能够获取到，则直接使用之，否则创建一个新的cell。
****◆**如何更高效地显示列表？**
要更高效地显示列表（不考虑种种优化），可以通过以下方法处理（只是部分）：
·提前根据数据计算好高度并缓存起来；
·提前将数据处理、I/O计算异步处理好，并保存结果，在需要时直接拿来使用。
****◆**http状态码查询**
2XX成功状态码
200OK服务器成功处理了请求（这个是我们见到最多的）
4XX客户端错误状态码
400BadRequest（坏请求）  告诉客户端，它发送了一个错误的请求
403  Forbidden（禁止）请求被服务器拒绝了
404  NotFound（未找到）未找到资源
408  RequestTimeout（请求超时）如果客户端完成请求时花费的时间太长，服务器可以回送这个状态码并关闭连接
414  RequestURLTooLong（请求URL太长）  客户端发送的请求所携带的URL超过了服务器能够或者希望处理的长度
5XX服务器错误状态码
500  InternetSeverError（内部服务器错误）服务器遇到一个错误，使其无法为请求提供服务
501Not Implement（未实现）  客户端发起的请求超出服务的能力范围（比如，使用了服务器不支持的请求方法）时，使用此状态码
502  BadGateway（网关故障）代理使用的服务器遇到了上游的无效响应
503  ServiceUnavailable（未提供此服务）服务器目前无法为请求提供服务，但过一段时间就可以恢复服务
504GatewayTimeout（网关超时）与状态码408类似，但是响应来自网关或代理，此网关或代理在等待另一台服务器的响应时出现了超时服务器
505  HTTPVersionNotSupported（不支持的HTTP版本）服务器收到的请求使用了它不支持的HTTP协议版本。有些服务器不支持HTTP早期的HTTP协议版本，也不支持太高的协议版本
****◆**单例模式的理解**
单例模式是一种常用设计模式，单例模式是一个类在系统中只有一个实例对象。通过全局的一个入口点对这个实例对象进行访问；
iOS中单例模式的实现方式一般分为两种：非ARC和ARC+GCD。
****◆**如何理解MVVM设计模式**
ViewModel层，就是View和Model层的粘合剂，它是一个放置用户输入验证逻辑，视图显示逻辑，发起网络请求和其他各种各样的代码的记号的地方。说白了，就是把原来ViewController层的业务逻辑和页面逻辑等剥离出来放到ViewModel层。
View层，就是ViewController，它的任务就是从ViewModel层获取数据，然后显示。
****◆**关于Bitcode**
问题描述：iOS新建项目默认需要支持bitcode，而不支持bitcode的SDK会导致无法编译运行。大部分社交平台SDK不支持bitcode。
解决方案：
1、暂时关闭对bitcode的支持（建议），方法：选中工程文件-->选中project下的工程名-->Build Settings-->找到BuildOptions-->Enable Bitcode-->置为NO，即可。
2、移除不支持bitcode的平台SDK。
33.Objective-C中是否支持垃圾回收机制？
Objective-C是支持垃圾回收机制的（Garbagecollection简称GC），但是apple得移动终端中，是不支持GC的，Mac桌面系统开发中是支持的。
移动终端开发是支持ARC（AutomaticReferenceCounting的简称），ARC在iOS5之后推出的新技术，它与GC的机制是不同的。我们在编写代码时，不需要向对象发送release或者autorelease方法，也不可以调用delloc方法，编译器会在合适的位置自动给用户生成release消息（autorelease），ARC的特点是自动引用计数简化了内存管理的难度。
****◆**对于RunLoop的理解**
Runloop，是多线程的法宝，即一个线程一次只能执行一个任务，执行完任务后就会退出线程。主线程执行完即时任务时会继续等待接收事件而不退出。非主线程通常来说就是为了执行某一任务的，执行完毕就需要归还资源，因此默认是不运行Runloop的；
每一个线程都有其对应的Runloop，只是默认只有主线程的Runloop是启动的，其他子线程的Runloop默认是不启动的，若要启动则需要手动启动；
在一个单独的线程中，如果需要在处理完某个任务后不退出，继续等待接收事件，则需要启用Runloop；
NSRunloop提供了一个添加NSTimer的方法，可以指定Mode，如果要让任何情况下都回调，则需要设置Mode为Common模式；
实质上，对于子线程的Runloop默认是不存在的，因为苹果采用了懒加载的方式。如果我们没有手动调用[NSRunloopcurrentRunloop]的话，就不会去查询是否在当前线程的Runloop，也就不回去加载，更不会创建。
****◆**什么是安全释放？**
先释放再置空。
****◆**GCD内部怎么实现的？**
1.iOS和OSX的核心是XNU内核，GCD是基于XNU内核实现的
2.GCD的API全部在libdispatch库中
3.GCD的底层实现主要有DispatchQueue和DispatchSource
DispatchQueue：管理block（操作）
DispatchSource：处理事件
****◆**KVO内部实现原理？**
1.KVO是基于runtime机制实现的；
2.当某个类的对象第一次被观察时，系统就会在运行期动态的创建该类的一个派生类，在这个派生类中重写基类中任何被观察属性的setter方法。派生类在被重写setter方法中实现了真正的通知机制。（Person-->NSKVONotificationPerson）
****◆**iOS应用的调试技巧？**
1.如遇到crash，分析崩溃日志（symbolicatedrash工具的适用）保留崩溃版本的.dSYM文件；
2.在Xcode中进入断点管理窗口，然后点击右下方的+，增加新的ExceptionBreakpoint；
3.如遇到EXC_BAD_ACCESS，打开Scheme选项选择EditScheme。然后勾上EnableZombie和MallocStack那两项；
4.有效的日志管理。NSLog和加入一些开源的日志管理框架；
5.程序断点debug模式。
****◆**iOS应用程序性能的调优**
1.用ARC去管理内存；
2.适当的地方使用reuseIdentifier；
3.尽可能设置视图为不透明；
4.避免臃肿的Xibs文件；
5.不要阻塞主线程；
6.调整图像视图中的图像尺寸；
7.选择正确合集；
8.启用Gzip压缩；
9.重用和延迟加载视图；
10.使用缓存；
11.考虑使用绘图；
12.处理内存警告；
13.重（chong）用大开销对象；
14.使用SpriteSheets（sprite sheet主要是由N个图像组合成的一个大的图像，这个大的图像里面包括了一个完整动画的所有帧，使用一个大图片的好处就是减少读取次数，在一定数量的情况下，sprite sheet有很明显的优势：
1.更小的文件尺寸
2.更快的打开速度
3.在FP11的时代可以借助STARTLING以便得到GPU的加速支持
）；
15.避免重复处理数据；
16.选择正确的数据格式；
17.适当得设置背景图片；
18.减少你的网络占用；
19.设置阴影路径；
20.优化你的表格视图；
21.选择正确的数据存储方式；
22.加速启动时间；
23.使用自动释放池；
24.缓存图像；
25.尽可能避免日期格式化。
****◆**UIScrollView的contentSize、contentOffSet和contentInset属性的区别**
contentSize表示UIScrollView滚动区域的大小。UIScrollView的frame属性在设置好了以后不会随内容的变化而变化；
contentOffSet表示是UIScrollView当前显示区域定点相对于frame顶点的偏移量，一般用来设置UIScrollView显示的位置；
contentInset表示是scrollView的contentView的顶点相对于scrollView的位置，假设你的contentInset=（0，100），那么你的contentView就是从scrollView的（0，100）开始显示。一般都是（0，0）表示从scrollView的开始显示。
****◆**描述程序启动的顺序**
1.main.m是程序的入口；
2.UIApplicationMain（）创建应用程序对象，并且为此对象指定委托，检测程序的运行，同时开启事件循环，处理程序接收到的事件；
3.UIApplicationDeletegate方法的执行；
4.加载window；
5.指定根视图控制器；
6.在指定的视图控制器中添加控件，实现应用程序界面；
****◆**介绍一下XMPP?有什么优缺点吗?**
XMPP(Extensible Messaging and Presence Protocol,前称)是一种以XML为基础的开放式实时通信协议,是 经由互联网工程工作小组(IETF)通过的互联网标准。简单的说,XMPP就是一种协议,一种规定。就是说,在网络上传 东西,要建立连接,TCP/IP连接,建立后再传东西,而XMPP就是规定你传的东西的格式。XMPP是基于XML的协议。 优点开放:
XMPP协议是自由、开放、公开的,并且易于了解。 而且在客户端 、 服务器 、 组件 、 源码库等方面,都已经各自有多种实现。 标准:
互联网工程工作小组( IETF )已经将Jabber的核心XML流协议以XMPP之名,正式列为认可的实时通信及Presence技术。 而XMPP的技术规格已被定义在RFC 3920及RFC 3921 。 任何IM供应商在遵循XMPP协议下,都可与Google Talk实现连接。 证实可用:
第一个Jabber(现在XMPP)技术是Jeremie Miller在1998年开发的,现在已经相当稳定;数以百计的开发者为XMPP技术而努 力。 今日的互联网上有数以万计的XMPP服务器运作着,并有数以百万计的人们使用XMPP实时传讯软件。分散式:
XMPP网络的架构和电子邮件十分相像;XMPP核心协议通信方式是先创建一个stream,XMPP以TCP传递XML数据流,没有 中央主服务器。 任何人都可以运行自己的XMPP服务器,使个人及组织能够掌控他们的实时传讯体验。安全:
任何XMPP协议的服务器可以独立于公众XMPP网络(例如在企业内部网络中),而使用SASL及TLS等技术的可靠安全性,已自 带于核心XMPP技术规格中。可扩展:
XML 命名空间的威力可使任何人在核心协议的基础上建造定制化的功能;为了维持通透性,常见的扩展由XMPP标准基金会 。 弹性佳:
XMPP除了可用在实时通信的应用程序,还能用在网络管理、内容供稿、协同工具、文件共享、游戏、远程系统监控等。 多样性:
用XMPP协议来建造及布署实时应用程序及服务的公司及开放源代码计划分布在各种领域;用XMPP技术开发软件,资源及支持的 来源是多样的,使得使你不会陷于被“绑架”的困境。缺点数据负载太重:
随着通常超过70%的XMPP协议的服务器的数据流量的存在和近60%的被重复转发,XMPP协议目前拥有一个大型架空中存在的 数据提供给多个收件人。 新的议定书正在研究,以减轻这一问题。没有二进制数据:XMPP协议的方式被编码为一个单一的长的XML文件,因此无法提供修改二进制数据。 因此, 文件传输协议一样使用外部的 HTTP。 如果不可避免,XMPP协议还提供了带编码的文件传输的所有数据使用的Base64 。 至于其他二进制数据加密会话 (encrypted conversations)或图形图标(graphic icons)以嵌入式使用相同的方法。
****◆**isKindOfClass和isMemberOfClass的联系与区别**
联系：两者都能检测一个对象是否是某个类的成员
区别：isKindOfClass不仅用来确定一个对象是否是一个类的成员，也可以用来确定一个对象是否派生自该类的类的成员，而isMemberOfClass只能做到第一点
举例：如ClassA派生自NSObject类，ClassA*a= [[ClassA alloc] init];，[a isKindOfClass:[NSObject class]]可以检查出a是否是NSObject类派生类的成员，但isMemberOfClass做不到。
****◆**isKindOfClass、isMemberOfClass、selector作用分别是什么？**
isKindOfClass：某个对象属于某个类型，包括继承的类型；
isMemberOfClass：某个对象确切属于某个类型，是不是具体的实例；
selector：通过方法名，获取在内存中的函数的入口地址；
****◆**写一个便利构造器**
//id代表任意类型指针，这里代表Student *,类方法
+(id)studentWithName:(NSString *)newName  andAge:(int)newAge
{
Student *stu=[[Student alloc]initName:newName andAge:newAge];
return [stu autorelease];//自动释放
}
****◆**列举集中进程的同步机制，并比较其优缺点**
原子操作信号量机制；
自旋锁；
管程；
会合；
分布式系统；
****◆**进程之间通信的途径**
共享存储系统消息传递系统管道：以文件系统为基础；
****◆**进程死锁的原因**
资源竞争及进程推进顺序非法；
****◆**死锁的4个必要条件**
互斥；
请求保持；
不可剥夺；
环路；
****◆**死锁的处理**
鸵鸟策略；
预防策略；
避免策略；
检测与接触死锁；
****◆**使用block时什么情况会发生引用循环，如何解决？**
一个对象中强引用了block，在block中又强引用了该对象，就会发生循环引用；
解决方法是将该对象使用__weak或者__block修饰符修饰之后，再在block中使用；
1.id weak weakSelf = self;
或者weak__typeof(&*self)weakSelf = self该方法可以设置宏；
2.id __block weakSelf = self;
或者将一种以防强制制空xxx = nil;
检测代码中是否存在循环引用问题，可使用Facebook开源的一个检测工具FBRetainCycleDetector；
****◆**一个Objective-C对象的isa的指针指向什么？有什么作用？**
指向他的类对象，从而可以找到对象上的方法；
****◆**Objective-C中的类方法和实例方法有什么本质区别和联系？**
类方法：
1.类方法是属于类对象的；
2.类方法只能通过类对象调用；
3.类方法中的self是类对象；
4.类方法可以调用其他的类方法；
5.类方法中不能访问成员变量；
6.类方法中不能直接调用对象方法；
实例方法：
1.实例方法是属于实例对象的；
2.实例方法只能通过实例对象调用；
3.实例方法中的self是实例对象；
4.实例方法中可以访问成员变量；
5.实例方法中直接点用实例方法；
6.实例方法中也可以调用类方法（通过类名）；
文／青灯素笺（简书作者）
原文链接：http://www.jianshu.com/p/c91e04940262
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
