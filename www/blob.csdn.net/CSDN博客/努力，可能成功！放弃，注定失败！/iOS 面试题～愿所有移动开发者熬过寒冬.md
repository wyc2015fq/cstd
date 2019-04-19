# iOS 面试题～愿所有移动开发者熬过寒冬 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月20日 17:48:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1790
众所周知，由于种种原因，导致今年移动开发的工作比较难找，本菜鸟也深感找工作的不易，所以总结了些面试题，希望可以帮助到哪些迷茫的人，愿大家都找到好工作
## 01：MRC/ARC的内存管理机制？
`OC的内存管理机制分为两种：一种为MRC手动引用计数，一种为ARC自动引用计数，从iOS5开始都是用ARC开发。`
### 原理：
```cpp
```cpp
在MRC中当使用new，alloc和copy方法创建一个对象时，该对象的引用计数器的值为1，当不再使用该对象时， 需要手动  向对象发送一条release或autorelease消息，这样对象的引用计数器会 -1，当retaincount的值为0时，该对象就会被销毁，从而释放内存。
在ARC中通过是否有强指针指向对象，若至少有一个强指针指向对象，则不销毁对象，当没有强指针指向对象时，对象被销毁，其本质是编译器在编译的时候，插入了类似内存管理的代码。
```
```
### 缺点：
**MRC：**
1.模块化操作的时候，对象可能被多个模块创建和使用，不能确定最后由谁去释放；
2.多线程操作的时候，不确定哪个线程最后使用完毕，总而言之，MRC内存泄露和坏内存的问题。
**ARC：**
1 循环引用的时候会导致内存泄露，（我们常见的delegate往往是assign方式的属性而不是retain方式 的属性；
2 Objective-C对象与CoreFoundation对象进行桥接的时候如果管理不当也会造成内存泄露；
3 CoreFoundation中的对象不受ARC管理，需要开发者手动释放
## 02：MVC、单例、观察者、代理等设计模式？
设计模式：并不是一种新技术，而是一种编码经验，使用比如：iOS中的协议，继承关系等基本手段，用比较成熟的逻辑去处理某一种类型的事情，总结为所谓设计模式。
**MVC：**
M:
``数据管理者--模型，给ViewController提供数据；给ViewController存储数据提供接口；提供经过抽象的业务基本组件，供Controller调度``
V:
``数据展示者--View，界面控件的展示；响应与业务无关的事件，加载动画``
C:
``数据加工者--控制器，管理ViewController的生命周期；负责生成所有的View实例，并放入ViewController中；监听来自View与业务有关的事件，通过与模型的合作，完成业务逻辑处理``
**单例模式：**
说白了就是一个类不通过alloc方式创建对象，而是用一个静态方法返回这个类的对象。系统只需要拥有一个的全局对象，这样有利于我们协调系统整体的行为，比如想获得[UIApplication sharedApplication];任何地方调用都可以得到 UIApplication的对象，这个对象是全局唯一的。
**观察者模式：**
当一个物体发生变化时，会通知所有观察这个物体的观察者让其做出反应。实现起来无非就是把所有观察者的对象给这个物体，当这个物体的发生改变，就会调用遍历所有观察者的对象调用观察者的方法从而达到通知观察者的目的
**代理模式：**
代理模式给某一个对象提供一个代理对象，并由代理对象控制对源对象的引用.比如一个工厂生产了产品，并不想直接卖给用户，而是搞了很多代理商，用户可以直接找代理商买东西，代理商从工厂进货.常见的如QQ的自动回复就属于代理拦截，代理模式在iphone中得到广泛应用.
## 03：KVO，delegate，block，NSNotification等消息通知机制？
**KVO：**
`KVO,即：Key-Value Observing，它提供一种机制，当指定的对象的属性被修改后，则对象就会接受到通知。简单的说就是每次指定的被观察的对象的属性被修改后，KVO就会自动通知相应的观察者了。`
**KVO的使用步骤：**
1：添加监听者
2：移除监听者
3：属性改变时，通知监听者
**KVO的底层实现：**
KVO是基与runtime机制实现的，当某个类的对象第一次被观察时，系统就会在运行期动态的创建该类的一个派生类，在这个派生类中重写基类中任何被观察属性的setter方法，派生类在重写的setter方法中实现真正的通知机制
**delegate：**
**代理的使用场景：**
1：当A对象想监听B对象的变化时，就可以使用代理，让A成为B的代理
2：当B对象想通知A对象的时候，就可以使用代理，让A成为B的代理
**代理的使用步骤：**
1：遵守代理协议
2：实现代理方法
3：将遵守了协议的对象设置为代理
**代理的注意点：**
代理的属性一般为id，属性一般为weak，避免循环引用
**手写代理：**
```
1：声明婴儿代理
#import <Foundation/Foundation.h>
@class Baby;
@protocol BabyProtocol <NSObject>
// 喂婴儿吃东西
- (void)feedFood:(Baby *)baby;
// 哄婴儿睡觉
- (void)hongBaby:(Baby *)baby;
@end
2：让保姆成为婴儿的代理
// 如果使用id类型来接收保姆, 如果将来换保姆了, 婴儿类不用修改代码
@property (nonatomic, strong) id<BabyProtocol> nanny
3：保姆遵守协议，并实现协议方法
#import <Foundation/Foundation.h>
#import "BabyProtocol.h"
@class Baby;
@interface Nanny : NSObject <BabyProtocol>
#import "Nanny.h"
#import "Baby.h"
@implementation Nanny
- (void)feedFood:(Baby *)baby
{
    baby.hungry -= 10;
    NSLog(@"喂婴儿吃东西 %i", baby.hungry);
}
- (void)hongBaby:(Baby *)baby
{
    baby.sleep += 10;
    NSLog(@"哄婴儿睡觉 %i", baby.sleep);
}
@end
```
**block：**
block在使用过程中容易产生循环引用
```cpp
```cpp
1.默认情况下, block是放在栈里面的
 2.一旦block进行了copy操作, block的内存就会放在堆里面
 3.堆里面的block(被copy过的block)有以下现象
 1> block内部如果通过外面声明的强引用来使用某个对象, 那么block内部会自动额外产生一个强引用指向所使用的对象
 2> block内部如果通过外面声明的弱引用来使用某个对象, 那么block内部会自动额外产生一个弱引用指向所使用的对象
```
```
在一个类中调用另一个类的方法可以用通知来实现，
通知的使用步骤：
1.监听通知:
```
[[NSNotificationCenter defaultCenter] addObserver:p1 selector:@selector(gotNews:) name:@"军事新闻" object:cmp2]; // 0
    [[NSNotificationCenter defaultCenter] addObserver:p2 selector:@selector(gotNews:) name:nil object:nil];
2.创建及发布通知:
[[NSNotificationCenter defaultCenter] postNotificationName:@"军事新闻" object:nil userInfo:@{@"title" : @"453543"}];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"娱乐新闻" object:cmp2 userInfo:@{@"title" : @"453543"}];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"娱乐新闻" object:cmp1 userInfo:@{@"title" : @"453543"}];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"娱乐新闻" object:nil userInfo:@{@"title" : @"453543"}];
3.移除通知:
[[NSNotificationCenter defaultCenter] removeObserver:p1];
    [[NSNotificationCenter defaultCenter] removeObserver:p2];
```
### 如何选择合适的消息通知机制？
通知比较灵活(1个通知能被多个对象接收, 1个对象能接收多个通知),
代理比较规范，但是代码多(默认是1对1)
KVO性能不好(底层会动态产生新的类)，只能监听某个对象属性的改变, 不推荐使用(1个对象的属性能被多个对象监听, 1个对象能监听多个对象的其他属性)
**通知（NSNotificationCenter\NSNotification）**
任何对象之间都可以传递消息
使用范围
1个对象可以发通知给N个对象
1个对象可以接受N个对象发出的通知
必须得保证通知的名字在发出和监听时是一致的
**KVO**
仅仅是能监听对象属性的改变（灵活度不如通知和代理）
**代理**
使用范围
1个对象只能设置一个代理(假设这个对象只有1个代理属性)
1个对象能成为多个对象的代理
比通知规范
建议使用代理多于通知
## 04：GCD，NSOperation多线程？
略
## 05：屏幕适配？
通过判断版本来控制，来执行响应的代码
功能适配：保证同一个功能在不同的系统版本上都能用
UI适配：保证各自的显示风格
## 06：runtime？
runtime是一套比较底层的纯C语言的API， 属于一个C语言库，里面包含许多API， 简称运行时， 运行时中最重要的是消息机制。 
对于C语言来说，函数的调用会在编译的时候就决定调用哪个函数，而对于OC来说，在编译的时候并不能决定调用哪个函数，只有在真正运行的时候才会根据函数的名称找到对应的函数来调用
runtime有啥用？
1> 能动态产生一个类、一个成员变量、一个方法 （KVO，，class_addMethod）
2> 能动态修改一个类、一个成员变量、一个方法
3> 能动态删除一个类、一个成员变量、一个方法
消息机制：
# import <objc/message.h> : 消息机制
方法调用的本质就是让对象发送消息，只有对象才能发送消息，使用消息机制必须导入#import<objc/message.h>
消息机制原理：对象根据方法编号SEL去映射表查找对应的方法实现
常见的函数和应用：
# import <objc/runtime.h> : 成员变量、类、方法
Ivar * class_copyIvarList : 获得某个类内部的所有成员变量 （占位符颜色，MJ，归档）
Method * class_copyMethodList : 获得某个类内部的所有方法 （）
Method class_getInstanceMethod : 获得某个实例方法（对象方法，减号-开头）
Method class_getClassMethod : 获得某个类方法（加号+开头）
动态的添加方法：
当一个类方法非常多，加载类到内存的时候会比较消耗资源，此时可以使用运行时给某个类动态的添加方法，即：performSelector
class_addMethod
搅拌池：
method_exchangeImplementations : 交换2个方法的具体实现 （image图片）
```
// 获取imageWithName方法地址
Method imageWithName = class_getClassMethod(self, @selector(imageWithName:));
// 获取imageWithName方法地址
Method imageName = class_getClassMethod(self, @selector(imageNamed:));
// 交换方法地址，相当于交换实现方式
method_exchangeImplementations(imageWithName, imageName);
```
## 07：KVC字典转模型，模型中的属性和字典中的Key不一致如何处理？
KVC字典转模型弊端：必须保证，模型中的属性和字典中的key一一对应。
如果不一致，就会调用[<Status 0x7fa74b545d60> setValue:forUndefinedKey:]
报key找不到的错。
分析:模型中的属性和字典的key不一一对应，系统就会调用setValue:forUndefinedKey:报错。
解决:重写对象的setValue:forUndefinedKey:,把系统的方法覆盖，
就能继续使用KVC，字典转模型了。
```
- (void)setValue:(id)value forUndefinedKey:(NSString *)key
{
}
```
## 08：SDWebImage原理？
略
## 09：svn和git常见操作？
**SVN： **
svn status 查看当前工作空间内，所有有变化的文件的状态
svn log 查看当前版本的操作日志
**个人开发：**
1: 初始化项目：svn import path URL - username=用户民 - password=密码 - m“注释”
`svn checkout 初始化本地仓库`
2：从服务器下载代码 svn checkout URL -username -password
3：修改文件提交 svn commit -m“注释”
4：新建文件提交 svn add filename svn commit -m“注释”
5：删除文件提交 一定不要直接手动删除文件,先从本地代码仓库管理中移除，然后再删除 svn remove filename / svn delete filename
**多人开发：**
超时：当本地文件的版本号小于服务器文件版本号时，如果要提交本地文件，就会报out of date超时错误，原因是svn通过版本号管理每一个文件，如果一个文件被修改并提交到服务器，那么服务器上的版本号就会加1，如果你的版本号小于服务器的版本号，就说明，有人修改了那个文件
超时解决：更新，从服务器下载更新最新版本，保证本地与服务器保持版本号一致
冲突：如果几个人同时修改了同一个文件的同一行代码，那么就会产生冲突，此时版本控制器不会智能的识别使用那个版本，只能交给用户处理，此时会有三个选择，1：用我的；2：用服务器的；3：延迟处理
```
```
版本回退：
修改了未提交：
svn revert
修改了，并且提交了：
a:
svn update 
svn merge 文件名 -r 版本1：版本2
svn commit - m 
b:
svn update - r 指定版本号 （本地代码已经变化，服务器的没有变）
svn update (冲突，选择使用自己的代码即可)
Git:
svn checkout —— git clone
svn 只下载代码， git，会连同代码仓库一起下载下来
svn commit —— git commit
svn 是提交到服务器，git 中是提交到本地仓库，需要使用push才能提交到服务器
svn update - git pull
svn update 从服务器下载最新被修改的代码，
工作区：与.git文件夹同级的其他文件夹或者子文件夹
暂缓区：
个人开发：
创建文件并提交：
touch mian.c
git add.
git commit -m“注释”
修改文件并提交：
git add.
git commit - m“注释”
删除文件并提交：
git rm person.h
git cmmit -m“注释”
版本回退：
git reset -hard HEAD 重置到当前版本
git reset -hard HEAD^^ 重置到上上个版本
git reset -hard 七位版本号 重置到指定版本
团队开发：
共享库：远程共享库：Github/OSChina
下载代码到本地：
git clone
新增文件同步：
touch person.h
git add.
git commit -m“创建person.h”
git push
 git pull
修改文件同步：
git add. 
git commit -m“注释”
git push
git pull
删除文件同步：
git rm filename git commit -m“注释”
git push
git pull
忽略文件：
touch .gitignore open .gitignore 加入忽略文件名
git add.git commit -m“注释”
```
```
## 10：JSON、XML的解析？
JSON：
第三方框架：JSONKit，SBJson，TouchJSON（性能逐渐递减）
系统自带：NSJSONSerialization（性能最好）
常见方法：
NSJSONSerialization的常见方法
JSON数据 OC对象
```
+ (id)JSONObjectWithData:(NSData *)data options:(NSJSONReadingOptions)opt error:(NSError **)error;
OC对象  JSON数据 
+ (NSData *)dataWithJSONObject:(id)obj options:(NSJSONWritingOptions)opt error:(NSError **)error
```
DOM：一次将整个XML文档加载进内寸，比较适合解析小的文档
SAX：从根元素开速，按顺序一个元素一个元素往下解析，比较适合解析大的文档
系统自带：
NSXMLParser：SAX方式解析，使用简单
第三方：libxml2：纯C语言，默认包含在iOS SDK中，同时支持DOM和SAX
GDataXML：DOM方式解析，基于libxml2
区别:
(1)可读性方面:基本相同,xml的可读性比较好
(2)可扩展性方面:都具有很好的扩展性
(3)编码难度方面:相对而言:JSON的编码比较容易 (4)解码难度:json的解码难度基本为零,xml需要考虑子节点和父节点 (5)数据体积方面:json相对于xml来讲,数据体积小,传递的速度跟快些 (6)数据交互方面:json与JavaScript的交互更加方面,更容易解析处理,更好的数据交互 (7)数据描述方面:xml对数据描述性比较好
(8)传输速度方面:json的速度远远快于xml
JSON底层原理:遍历字符串中的字符,最终根据格式规定的特殊字符,比如{}号,[]号, : 号 等进行区分,{}号是一个字典 的开始,[]号是一个数组的开始, : 号是字典的键和值的分水岭,最终乃是将json数据转化为字典,字典中值可能是字典,数 组,或字符串而已。
XML底层原理: XML解析常用的解析方法有两种:DOM解析和SAX解析。DOM 采用建立树形结构的方式访问 XML 文档,而 SAX 采用的事件模型。 。DOM 解析把 XML 文档转化为一个包含其内容的树,并可以对树进行遍历。使用 DOM 解析器的时候需 要处理整个 XML 文档,所以对性能和内存的要求比较高。SAX在解析 XML 文档的时候可以触发一系列的事件,当发现给定的tag 的时候,它可以激活一个回调方法,告诉该方法制定的标签已经找到。SAX 对内存的要求通常会比较低,因为它让开发人员自己来决 定所要处理的tag。特别是当开发人员只需要处理文档中所包含的部分数据时,SAX
 这种扩展能力得到了更好的体现。 
延伸:SAX与DOM的区别: 1、SAX处理的优点非常类似于流媒体的优点。分析能够立即开始,而不是等待所有的数据被处理。而且由于应用程序只是 在读取数据时检查数据,因此不需要将数据存储在内存中。这对于大型文档来说是个巨大的优点。事实上,应用程序甚至不 必解析整个文档;它可以在某个条件得到 满足时停止解析。一般来说,SAX 还比它的替代者 DOM 快许多。另一方面,由 于应用程序没有以任何方式存储数据,使用 SAX 来更改数据或在数据流中往后移是不可能的。
2、DOM 以及广义的基于树的处理具有几个优点。首先,由于树在内存中是持久的,因此可以修改它以便应用程序能对数 据和结构作出更改。它还可以在任何时候在树中上下 导航,而不是像 SAX 那样是一次性的处理。DOM 使用起来也要简单 得多。另一方面,在内存中构造这样的树涉及大量的开销。大型文件完全占用系统内存容量的情况并不鲜见。此外,创建一 棵 DOM 树可能是一个缓慢的过程。
3、选择 DOM 还是选择 SAX,这取决于下面几个因素:
应用程序的目的:如果打算对数据作出更改并将它输出为 XML,那么在大多数情况下,DOM 是适当的选择。并不是说使 用 SAX 就不能更改数据,但是该过程要复杂得多,因为您必须对数据的一份拷贝而不是对数据本身作出更改。
数据容量: 对于大型文件,SAX 是更好的选择。数据将如何使用:如果只有数据中的少量部分会被使用,那么使用 SAX 来将该部分数据提取到应用程序中可能更好。 另一方面,如果您知道自己以后会回头引用已处理过的大量信息,那么 SAX 也许不是恰当的选择。
对速度的需要:SAX 实现通常要比 DOM 实现更快。
SAX 和 DOM 不是相互排斥的,记住这点很重要。您可以使用 DOM 来创建 SAX 事件流,也可以使用 SAX 来创建 DOM 树。事实上,用于创建 DOM 树的大多数解析器实际上都使用 SAX 来完成这个任务!
## 11：不等高Cell的高度计算？
iOS8之前：
面向对象：把数据布局到Cell上，拿到cell最底部控件的最大Y值
面向过程：把每个cell的高度算出来，保存到数组中，在heightForRow中返回每个cell的高度
iOS8之后：
新特性：估算高度
```
- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.estimatedRowHeight = 100;  //  随便设个不那么离谱的值
    self.tableView.rowHeight = UITableViewAutomaticDimension;
}
```
cell最下面的控件一定要粘着最底部，且不能实现高度的代理方法
注意：不要再高度的代理方法中计算cell的高度，这样会由于代理方法的频繁调用造成性能降低，最好在cellForRow中计算高度，或者请求到数据后直接调用。
问题：由于iOS8与iOS7上cell的高度计算原理不同，导致相同的代码在iOS7上顺滑，在iOS8上却莫名卡顿 ？
原因：
不开启高度估算时，UITableView 上来就要对所有 cell 调用算高来确定 contentSize
dequeueReusableCellWithIdentifier:forIndexPath: 相比不带 “forIndexPath” 的版本会多调用一次高度计算
iOS7 计算高度后有”缓存“机制，不会重复计算；而 iOS8 不论何时都会重新计算 cell 高度
**解决办法：孙源团队的第三方框架——UITableView+FDTemplateLayoutCell 具体见链接--[大神博客](http://blog.sunnyxx.com/2015/05/17/cell-height-calculation/)**
原理：
> 
和每个 UITableViewCell ReuseID 一一对应的 template layout cell
这个 cell 只为了参加高度计算，不会真的显示到屏幕上；它通过 UITableView 的 -dequeueCellForReuseIdentifier: 方法 lazy 创建并保存，所以要求这个 ReuseID 必须已经被注册到了 UITableView 中，也就是说，要么是 Storyboard 中的原型 cell，要么就是使用了 UITableView 的 -registerClass:forCellReuseIdentifier: 或 -registerNib:forCellReuseIdentifier:其中之一的注册方法。
根据 autolayout 约束自动计算高度
使用了系统在 iOS6 就提供的 API：-systemLayoutSizeFittingSize:
根据 index path 的一套高度缓存机制
计算出的高度会自动进行缓存，所以滑动时每个 cell 真正的高度计算只会发生一次，后面的高度询问都会命中缓存，减少了非常可观的多余计算。
自动的缓存失效机制
无须担心你数据源的变化引起的缓存失效，当调用如-reloadData，-deleteRowsAtIndexPaths:withRowAnimation:等任何一个触发 UITableView 刷新机制的方法时，已有的高度缓存将以最小的代价执行失效。如删除一个 indexPath 为 [0:5] 的 cell 时，[0:0] ~ [0:4] 的高度缓存不受影响，而 [0:5] 后面所有的缓存值都向前移动一个位置。自动缓存失效机制对 UITableView 的 9 个公有 API 都进行了分别的处理，以保证没有一次多余的高度计算。
预缓存机制
预缓存机制将在 UITableView 没有滑动的空闲时刻执行，计算和缓存那些还没有显示到屏幕中的 cell，整个缓存过程完全没有感知，这使得完整列表的高度计算既没有发生在加载时，又没有发生在滑动时，同时保证了加载速度和滑动流畅性，下文会着重讲下这块的实现原理。
利用RunLoop空闲时间执行预缓存任务
FDTemplateLayoutCell 的高度预缓存是一个优化功能，它要求页面处于空闲状态时才执行计算，当用户正在滑动列表时显然不应该执行计算任务影响滑动体验。
一般来说，这个功能要耦合 UITableView 的滑动状态才行，但这种实现十分不优雅且可能破坏外部的 delegate 结构，但好在我们还有RunLoop这个工具，了解它的运行机制后，可以用很简单的代码实现上面的功能。
空闲RunLoopMode
在曾经的 RunLoop 线下分享会中介绍了 RunLoopMode 的概念。
当用户正在滑动 UIScrollView 时，RunLoop 将切换到 UITrackingRunLoopMode 接受滑动手势和处理滑动事件（包括减速和弹簧效果），此时，其他 Mode （除 NSRunLoopCommonModes 这个组合 Mode）下的事件将全部暂停执行，来保证滑动事件的优先处理，这也是 iOS 滑动顺畅的重要原因。
当 UI 没在滑动时，默认的 Mode 是 NSDefaultRunLoopMode（同 CF 中的 kCFRunLoopDefaultMode），同时也是 CF 中定义的 “空闲状态 Mode”。当用户啥也不点，此时也没有什么网络 IO 时，就是在这个 Mode 下。
用RunLoopObserver找准时机
注册 RunLoopObserver 可以观测当前 RunLoop 的运行状态，并在状态机切换时收到通知： 
RunLoop开始
RunLoop即将处理Timer
RunLoop即将处理Source
RunLoop即将进入休眠状态
RunLoop即将从休眠状态被事件唤醒
RunLoop退出
因为“预缓存高度”的任务需要在最无感知的时刻进行，所以应该同时满足： 
RunLoop 处于“空闲”状态 Mode
当这一次 RunLoop 迭代处理完成了所有事件，马上要休眠时
使用 CF 的带 block 版本的注册函数可以让代码更简洁： 
```
CFRunLoopRef runLoop = CFRunLoopGetCurrent();
CFStringRef runLoopMode = kCFRunLoopDefaultMode;
CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler
(kCFAllocatorDefault, kCFRunLoopBeforeWaiting, true, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity _) {
    // TODO here
});
CFRunLoopAddObserver(runLoop, observer, runLoopMode);
```
在其中的 TODO 位置，就可以开始任务的收集和分发了，当然，不能忘记适时的移除这个 observer 
分解成多个RunLoop Source任务
假设列表有 20 个 cell，加载后展示了前 5 个，那么开启估算后 table view 只计算了这 5 个的高度，此时剩下 15 个就是“预缓存”的任务，而我们并不希望这 15 个计算任务在同一个 RunLoop 迭代中同步执行，这样会卡顿 UI，所以应该把它们分别分解到 15 个 RunLoop 迭代中执行，这时就需要手动向 RunLoop 中添加 Source 任务（由应用发起和处理的是 Source 0 任务）
Foundation 层没对 RunLoopSource 提供直接构建的 API，但是提供了一个间接的、既熟悉又陌生的 API：
```
- (void)performSelector:(SEL)aSelector
               onThread:(NSThread *)thr 
             withObject:(id)arg 
          waitUntilDone:(BOOL)wait 
                  modes:(NSArray *)array;
```
这个方法将创建一个 Source 0 任务，分发到指定线程的 RunLoop 中，在给定的 Mode 下执行，若指定的 RunLoop 处于休眠状态，则唤醒它处理事件，简单来说就是“睡你xx，起来嗨！”
于是，我们用一个可变数组装载当前所有需要“预缓存”的 index path，每个 RunLoopObserver 回调时都把第一个任务拿出来分发：
```
NSMutableArray *mutableIndexPathsToBePrecached = self.fd_allIndexPathsToBePrecached.mutableCopy;
CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler
(kCFAllocatorDefault, kCFRunLoopBeforeWaiting, true, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity _) {
    if (mutableIndexPathsToBePrecached.count == 0) {
        CFRunLoopRemoveObserver(runLoop, observer, runLoopMode);
        CFRelease(observer); // 注意释放，否则会造成内存泄露
        return;
    }
    NSIndexPath *indexPath = mutableIndexPathsToBePrecached.firstObject;
    [mutableIndexPathsToBePrecached removeObject:indexPath];
    [self performSelector:@selector(fd_precacheIndexPathIfNeeded:)
                 onThread:[NSThread mainThread]
               withObject:indexPath
            waitUntilDone:NO
                    modes:@[NSDefaultRunLoopMode]];
});
```
这样，每个任务都被分配到下个“空闲” RunLoop 迭代中执行，其间但凡有滑动事件开始，Mode 切换成 UITrackingRunLoopMode，所有的“预缓存”任务的分发和执行都会自动暂定，最大程度保证滑动流畅。
## 12:Socket简单介绍？
Socket又被成为套接字，当网络上的两端通过建立一个双向的通道来实现数据的交换，那么这个端口其实就是Socket。
网络通信需要满足一定的条件：
IP地址：（网络上主机设备的唯一标识）
端口号：用于标示进程的逻辑，不同进程的标识
传输协议：TCP、UDP
**TCP：**
传输数据时候，需要建立连接，形成传输数据的通道
数据的大小不受限制，可以进行大数据的传输
通过三次握手来完成连接，因此是安全的，可靠的协议
- 由于是可靠连接，所以导致数据传输的效率低
**UDP：**
传输数据是将数据以及源和目的封装到数据包中，不需要建立连接
因为不需要建立连接，所以是不可靠的连接
传输数据的大小被限制在64k以内
由于是不可靠连接，所以传输数据速度快
**http与Scoket的不同？**
http是客户端用http协议进行请求，发送请求时需要封装http请求头，服务端一般会有web端进行配合，其请求方式一般为客户端主动发送请求，服务端才能给响应，一次请求后则断开连接，以便以节省资源，除了长连接外，服务端不能给客户端响应。
Scoket是客户端与服务端用Scoket进行连接，连接后不断开，双方都可以发送数据，一般用在游戏等对数据即时性要求比较高的场合
## 13：XMPP的简单介绍？
XMPP是一个即时通讯的协议，其方便了用于即时通讯在网络上数据传输格式，比如登录，获取好友等格式。XMPP在网络中传输的内容格式为XML格式
XMPP是一个基于Socket的网络协议，这样其可以实现长连接，以方便实现即时通讯功能
XMPP的客户端是使用一个XMPPFramework框架来实现，服务器是使用一个开源的服务器Openfire
当客户端获取到服务器发送过来的好友消息，客户端需要对XML进行解析，使用的解析框架KissXML，而不是NSXMLParser
XMPP没有提供发送附件的功能，需要自己实现，把文件上传到文件服务器，上传成功后再把附件的路径发送给好友
## 14：环信的简单介绍？
环信是一个即时通信的服务提供商
环信使用的是XMPP协议，它是再XMPP的基础上进行二次开发，对服务器Openfire和客户端进行功能模型的添加和客户端SDK的封装，环信的本质还是使用XMPP，基本于Socket的网络通信
环信内部实现了数据缓存，会把聊天记录添加到数据库，把附件(如音频文件，图片文件)下载到本地，使程序员更多时间是花到用户即时体验上
环信内部已经实现了视频，音频，图片，其它附件发送功能
环信使用公司可以节约时间成本，不需要公司内部搭建服务器，客户端的开发，使用环信SDK比使用XMPPFramework更简洁方便
## 15：数据存储？
plist存储、偏好设置存储、归档、SQLite、Core Data
### plist存储：
plist存储，就是生成一个plist文件，用来储存字典或者数组，其不能储存自定义对象
### 偏好设置储存：
NSUserDefaults，其底层封装了一个字典，利用字典的方式生成plist，这样做的好处是不用关心文件名，快速的进行的键值配对
存储:
```
偏好设置NSUserDefaults
    底层就是封装了一个字典,利用字典的方式生成plist
     好处:不需要关心文件名,快速进行键值对存储
    // name xmg
    [[NSUserDefaults standardUserDefaults] setObject:@"xmg" forKey:@"name"];
    // age 18
    [[NSUserDefaults standardUserDefaults] setInteger:18 forKey:@"age"];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"isOn"];
 读取:
   NSString *name = [[NSUserDefaults standardUserDefaults] objectForKey:@"name"];
  BOOL ison =  [[NSUserDefaults standardUserDefaults] boolForKey:@"isOn"];
```
### 归档：
NSKeyedArchiver：专门用来做自定义对象归档
```
// 什么时候调用:当一个对象要归档的时候就会调用这个方法归档
// 作用:告诉苹果当前对象中哪些属性需要归档
- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeObject:_name forKey:@"name"];
    [aCoder encodeInt:_age forKey:@"age"];
}
// 什么时候调用:当一个对象要解档的时候就会调用这个方法解档
// 作用:告诉苹果当前对象中哪些属性需要解档
// initWithCoder什么时候调用:只要解析一个文件的时候就会调用
- (id)initWithCoder:(NSCoder *)aDecoder
{
    #warning  [super initWithCoder]
    if (self = [super init]) {
        // 解档
        // 注意一定要记得给成员属性赋值
      _name = [aDecoder decodeObjectForKey:@"name"];
      _age = [aDecoder decodeIntForKey:@"age"];
    }
    return self;
}
```
### SQLite:
```
```
SQlite是一款轻型的嵌入式数据库，其占用的资源非常低，因此十分适合在移动端使用。
基本用法：
增加表字段：
ALTER TABLE 表名 ADD COLUMN 字段名 字段类型；
删除表字段：
ALTER TABLE 表名 DROP COLUMN 字段名；
修改表字段：
ALTER TABLE 表名 COLUMN 旧字段名 TO 新字段名；
```
```
### Core Data:
```
1>  CoreData是对SQLite数据库的封装
2>  CoreData中的NSManagedObjectContext在多线程中不安全
3>  如果想要多线程访问CoreData的话，最好的方法是一个线程一个NSManagedObjectContext
4>  每个NSManagedObjectContext对象实例都可以使用同一个NSPersistentStoreCoordinator实例，这是因为NSManagedObjectContext会在便用NSPersistentStoreCoordinator前上锁
```
## 16:get与post的区别？
GET和POST的主要区别表现在数据传递上
GET：
```
在请求体URL后面以 ? 的的形式跟上发送给服务器的参数，多个参数之间以 & 隔开
由于服务器和浏览器对URL长度有限制，所以其后面附带的参数是有限制的
```
POST：
发送给服务器的所有参数都放在请求体中
理论上，POST传递的数据量没有限制
文／我是小菜鸟（简书作者）
原文链接：http://www.jianshu.com/p/2bc4fe6d35b7
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
