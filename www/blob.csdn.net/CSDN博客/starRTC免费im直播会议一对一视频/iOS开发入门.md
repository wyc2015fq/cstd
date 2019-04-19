# iOS开发入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月21日 14:11:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：169
[https://github.com/qinjx/30min_guides/blob/master/ios.md](https://github.com/qinjx/30min_guides/blob/master/ios.md)
- 任何C源程序，不经修改，即可通过Objective-C编译器成功编译
- Objective-C源程序中可以直接使用任何C语言代码
除了面向对象有语法是SmallTalk风格的（下面会讲到），其它非面向对象的语法、数据类型，与C完全相同
SmallTalk的消息传递语法风格
Objective-C的面向对象语法源自SmallTalk，消息传递（Message Passing）风格。在源码风格方面，这是它差别最大的地方。
在Java、C++世界，我们调用一个对象的某方法，在Objective-C里，这称作给类型发送一个消息，这可不仅仅是文字游戏，他们的技术细节也是不同的。
在Java里，对象和方法关系非常严格，一个方法必须属于一个类/对象，否则编译是要报错的。而在Objective-C里，类型和消息的关系比较松散，消息处理到运行时（runtime）才会动态决定，给类型发送一个它无法处理的消息，也只会抛出一个异常，而不会挂掉。
[obj undefinedMethod];
在代码里调用没定义的方法（这是Java世界的习惯说法啊，专业的叫法是，给obj对象传递它无法处理的消息），Xcode会警告，但编译能成功，运行的时候会出错。它会输出这样一个错误：
Terminating app due to uncaught exception 'NSInvalidArgumentException', reason: '-[NSObject undefinedMethod]: unrecognized
 selector sent to instance 0x8871710'
字符串
Objective-C里有字符串是由双引号包裹，并在引号前加一个@符号，例如：
title = @"Hello";if(title == @"hello") {}
PHP程序员要注意，在这里不能用单引号，即使只有一个字符也不能用。
函数调用
不涉及面向对象时，它和C是完全一样的。以下是几个函数调用的示例：
不带参数
startedBlock();
带参数
NSLog(@"decrypted string: %@", str);CGRectMake(0,0,0,0);

传递消息给类/实例方法
不带参数
[obj method];
对应的Java版本
obj.method();
带一个参数：
[counter increase:1];
对应的Java版本
counter.increase(1);
带多个参数
对C Family程序员来说，这是最难接受的，最反人类的：
- (void) setColorToRed: (float)red Green: (float)green Blue:(float)blue {...} //定义方法[myObj setColorToRed: 1.0 Green:
 0.8 Blue: 0.2]; //调用方法
对应的Java版
public void setColorToRedGreenBlue(float red, float green, float blue) {...}myObj.setColorToRedGreenBlue(1.0, 0.8, 0.2);
消息嵌套
UINavigationBar *bar = [[[UINavigationBar alloc] init] autorelease];
对应的Java版
UINavigationBar bar = UINavigationBar.alloc().init().autorelease();//Java没有指针，所以星号去掉了
类
分为接口定义和实现两个部分。接口定义（Interface）放在头文件中，文件扩展名是.h，实现（implementation）放在实现文件中，文件扩展名是.m（也有.mm的扩展名，表示Objective-C和C++混编的代码）。
interface可以定义成员属性，Java里不可以

Interface
@interface MyClass {int memberVar1;id memberVar2;}-(return_type) instance_method1; -(return_type) instance_method2:
 (int) p1;-(return_type) instance_method3: (int) p1 andPar: (int) p2;@end
Implementation
@implementation MyClass {int memberVar3;}-(return_type) instance_method1 {....}-(return_type) instance_method2: (int)
 p1 {....}-(return_type) instance_method3: (int) p1 andPar: (int) p2 {....}@end
接口和实现以@interface、@implementation开头，都以@end结束。“@”符号在Objective-C中是个很神奇的符号。
冒号也是方法名的一部分，method和method:是两个不同的方法名，不是overload，第二个带参数。
上述代码对应的Java版：
public class MyClass {protected int memberVar1;protected pointer memberVar2;private int memberVar3;public (return_type)
 instance_method1() {....}public (return_type) instance_method2(int p1) {....}public (return_type) instance_method3andPar(int
 p1, int p2) {....}}
私有方法和公开方法
写在.h头文件里的方法都是公开的，Objective-C里没有私有方法的概念
类方法就是Java、PHP里的Static Method，不用实例化就能调。类方法有一个加号前缀。
 示例：
类定义
@interface MyClass+(void) sayHello;@end@implementation MyClass+(void) sayHello {NSLog(@"Hello, World");}@end
使用
[MyClass sayHello];

实例方法有一个减号前缀。 示例：
类定义
@interface MyClass : NSObject-(void) sayHello;@end@implementation MyClass-(void) sayHello {NSLog(@"Hello, World");}@end
使用
mycls = [MyClass
new];[mycls sayHello];
继承是写在Interface定义里面的。语法为：子类名在左，父类名在右，中间用冒号分隔。 示例：
@interface MyClass : NSObject@end

selector就是一个方法指针，类似PHP里的动态方法名：
<?phpclass Hello {public function sayHello() {}public function test() {$fun_name = "sayHello";$this->$fun_name();}}
在Objective-C里，selector主要用来做两类事情：
绑定控件触发的动作
@implementation DemoViewController- (void)downButtonPressed:(id)sender {//响应“按钮被按下事件”的方法UIButton *button = (UIButton*)sender;[button
 setSelected:YES];}- (void)drawAnButton {UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom]; btn.frame = _frame; btn.tag = 1;btn.backgroundColor = [UIColor clearColor];[btn addTarget: self action: @selector(downButtonPressed:) forControlEvents: UIControlEventTouchUpInside];//当这个按钮被按下时，触发downButtonPressed:方法}@end
延时异步执行
@implementation ETHotDealViewController- (void)viewDidLoad {//获取数据源HotDealDataSource *ds = [[HotDealDataSource alloc]init];[ds
 reload];_items = ds.items;[self performSelector: @selector(refreshTable) withObject: self afterDelay: 0.5];//延迟0.5秒调用refreshTable方法}-(void)refreshTable{[self.tableView reloadData];}@end
这个例子中，获取数据源是通过ASIHTTP组件异步调用服务端HTTP接口，refreshTable要用到数据源返回回来的数据，如果不延迟0.5秒，就会立刻执行，执行的时候数据还在路上呢，页面就要变空白了。

协议（Protocol）
就是Java、PHP里的Interface。
协议的定义用@protocol关键字：
@protocol Printable-(void)print:(NSString)str;@end
可以继承别的协议：
@protocol Printable <NSObject>-(void)print:(NSString)str;@end
协议可以包含可选方法，顾名思义，可选方法可以不被类实现：
@protocol Printable@optional-(void)print:(NSString)str;@end
加了@optional关键字，一个类在implements这个协议时，便可以不实现print:方法。
协议的实现
一个类实现某些协议是写在Interface定义里面的。语法为：协议名用尖括号包裹，多个协议名用逗号隔开，协议写在父类的右边（如果没有父类就直接写在子类右边）。
示例：
@interface class MyClass : NSObject
 <Printable, Drawable>@end
Printable, Drawablw就是两个协议。
对应的Java版本是：
public class MyClass extends NSObject implements Printable, Drawable {}
分类（Category）
分类可以给一个已经存在的类增加方法，而不用去改它的源码。Java和PHP中都没有类似的特性。
比如说，NSObject是一个Objective-C内置的系统类，我们想给它增加toJson方法，就像这样：
头文件：NSObject+Json.h
@interface NSObject (Json)-(NSString)toJson;@end
实现文件：NSObject+Json.m
@implementation NSObject (Json)-(NSString)toJson {//...}@end
使用的时候，只要包含NSObject+Json.h，实例化NSObject类，就可以使用toJson方法了：
import "NSObject+Json.h"@implatementation XYZController-(void)test {NSObject *obj = [[NSObject alloc]init];NSString
 *str = [obj toJson];}@end
当然了，NSObject本来的那些方法依然还是可以用的，什么都没变，只是多了个toJson方法。看起来是不是和继承没太多差别呢（除了使用的时候实例化的是NSObject，而不是JsonObject）？再看一个继承实现不了的例子：
头文件：NSObject+Json+XML.h
@interface NSObject (Json)-(NSString)toJson;@end@interface NSObject (XML)-(NSString)toXML;@end
实现文件：NSObject+Json+XML.m
@implementation NSObject (Json)-(NSString)toJson {//...}@end@implementation NSObject (XML)-(NSString)toXML {//...}@end
使用：
import "NSObject+Json+XML.h"@implatementation XYZController-(void)test {NSObject *obj = [[NSObject alloc]init];
 NSString *json = [obj toJson];NSString *xml = [obj toXML];}@end

Cocoa Touch
Cocoa是Mac OS App的开发框架，Cocoa Touch是iOS开发用的框架，Cocoa
 Touch和Cocoa大部分是一样的，只是Cocoa Touch多了一些移动设备特有的东西，如：触摸屏、加速度传感器、GPS定位。Cocoa中多任务、多窗口的特性，在Cocoa Touch中也是没有的（或者跟Cocoa不完全一样的）。
学过Objective-C语言之后，也要再学习Cocoa Touch框架才能顺利地开发iOS应用。
最常用设计模式之Delegate
Cocoa Touch大量使用Delegate（委派）设计模式。

真机测试
拥有一个开发者账号
苹果的Developer Program分为个人开发者和公司开发者，分别是每年99美元和每年299美元，分别可以注册100台和500台苹果测试设备。这个台数限制在一个付费年度内不会清空，比如说，2013年4月1日付费成功的，付费会员资格在2014年3月31日之前有效，这期间，注册一台就少一个名额，哪怕这个设备注册进来用了之后一分钟马上又删掉了，减少的这个名额也不会回来。
在交钱之前，最好问一下，周围的同事，有没有已经交了钱的。如果有，你只需要注册一个免费的Apple ID，请他发个邀请邮件给你，把你的Apple
 ID加入他的团队就可以了，苹果会认为你们两个人是一个团队的，你们分别用自己的账号，共享100台设备的限额，
安装证书和私钥
进证书管理：
- 进入 [苹果开发者中心](https://developer.apple.com/)
- 点击iOS Dev Center
- 点蓝色“Login”按钮，用你的Apple ID登录，登录成功会跳到 [开发者首页](https://developer.apple.com/devcenter/ios/index.action)
- 点击右上角的[iOS Provisioning Portal](https://developer.apple.com/ios/manage/overview/index.action)（别找了，直接Command
 F搜索）
- 点左侧菜单栏里的[Certificates](https://developer.apple.com/ios/manage/certificates/team/index.action)
页面上有一个“Your Certificate”区域，下方有个Download圆角按钮，这是你的个人证书，下载下来。再下面一行，有一句“If
 you do not have the WWDR intermediate certificate installed, [click here to download now](https://developer.apple.com/certificationauthority/AppleWWDRCA.cer)”，这个是苹果的公共证书，也下下来。
双击下载回来的证书，装证书时，会提示你输入密码，这是【钥匙串访问工具】在问你要你的Mac OS账号开机密码（相当于linux里面的sudo），不是Apple ID的密码，不要搞错了。
安装私钥
如果你是和其它同事公用的账号，让他给你一个私钥即可，就是一个扩展名为p12的文件，双击之，钥匙串访问会自动出来，需要你输入一个密码，这个密码问给你p12文件的人要，不是你的Mac
 OS系统开机密码，也不是你的Apple ID密码。
将设备注册到Provisioning Portal
- 打开Xcode，从Xcode的Window菜单中找到Organizer，打开之（Shift Command 2）。
- 把iOS设备连上电脑，Organizer会自动识别出你的设备，并显示在左侧边栏。
- 在Organizer左侧边栏找到你的设备，右键，点击“Add Device to Provisioning Portal”，然后等Organizer提示你操作成功即可。（选中设备后，右边设备详情区域会显示一个按钮“Use for Development”，点它也可以）。
到iOS真机上运行测试版程序
回到Xcode主界面，在Stop按钮（Run按钮右边那个黑色正方形按钮）右边，有个下拉菜单，显示着 “ToolBarSearch > iPhone 5.0 Simulator” （即 你的应用英文名 > 当前选中的调试 ），点击这个下拉菜单，选中你的真机设备名，再按“Run”按钮，Xcode就会自动把当前正在编辑开发的应用编译并安装到真机上测试啦！
发布到App Store
打IPA包
IPA包本质上是一个ZIP压缩包，只不过它有着特殊的目录结构，扩展名是ipa，制作方法如下：
- 在Xcode中Build项目，快捷键Command B
- 在左侧项目导航器中，展开Products文件夹，找到你要打包的应用，你的应用名.app，右键，选择show in finder
- 到Finder中Copy这个.app目录（选中，按Command C），复制到一个你新建的名为Payload（区分大小写）的文件夹中
- 找到你的应用Logo，即一个512 * 512像素的PNG文件，copy到与Payload一起（与Payload并列，不要放进Payload了），并重命名为iTunesArtwork（区分大小写，没有扩展名）
- 将Payload目录、ItunesArtwork文件打成一个zip包，并更改扩展名为ipa
- 双击这个ipa文件，会用iTunes打开，如果打开成功，且在iTunes里有应用Logo显示，就成功了
批量自动打包
除App Store外，还有许多其它的iOS应用市场（如91助手，同步推等等），如果一个应用需要发布到很多个应用市场，且他们的代码略有不同（比如说，统计代码不同），按上述方法手工修改源码再打包，再还原，比较容易出错。好消息是，Xcode是有命令行的，我们可以写一个shell脚本，先用se自动修改源码，再调用Xcode的命令行来编译以得到your——app.app目录，最后调用zip、mv等命令把上一个章节讲的ipa打包动作自动执行。
Xcode中的代码结构与操作系统上的文件系统并不一致
推荐在Finder里建好目录再到Xcode的Project Navigator中点击“Add Files to”添加到项目中
开源代码
- [Apple官方的Sample Code](https://developer.apple.com/library/ios/navigation/#section=Resource%20Types&topic=Sample%20Code)
- [维基百科上的开源iOS App](http://en.wikipedia.org/wiki/List_of_free_and_open_source_iOS_applications)
- [iOS Opensource](http://www.iosopensource.com/) --Domain Parking了，以前可以下载Twitter和Wordpress客户端的
- [code 4 app](http://code4app.com/)
- [UI 4 app](http://ui4app.com/)， code4app的姐妹站
Objective-C教程
- [Apple官方教程](http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/ProgrammingWithObjectiveC/Introduction/Introduction.html)
- [Cocoa Dev Center](http://cocoadevcentral.com/d/learn_objectivec/)
- [维基上的Objective-C语言简介](http://zh.wikipedia.org/wiki/Objective-C) --中文，十分钟可读完，推荐
