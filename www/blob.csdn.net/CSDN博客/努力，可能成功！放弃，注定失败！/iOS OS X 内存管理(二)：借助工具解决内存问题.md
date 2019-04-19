# iOS/OS X 内存管理(二)：借助工具解决内存问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月22日 13:33:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：338标签：[iOSOS X内存管理																[内存管理																[iOS开发																[iOS](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=内存管理&t=blog)](https://so.csdn.net/so/search/s.do?q=iOSOS X内存管理&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
上一篇博客iOS/OS X内存管理(一)：基本概念与原理主要讲了iOS/OSX 内存管理中引用计数和内存管理规则，以及引入ARC新的内存管理机制之后如何选择ownership qualifiers(__strong、__weak、__unsafe_unretained和__autoreleasing)来管理内存。这篇我们主要关注在实际开发中会遇到哪些内存管理问题，以及如何使用工具来调试和解决。
![166109-00c90f0f030c3665.png](http://cc.cocimg.com/api/uploads/20160217/1455702106588624.png)
在往下看之前请下载实例[MemoryProblems](https://github.com/samlaudev/MemoryProblems)，我们将以这个工程展开如何检查和解决内存问题。
**悬挂指针问题**
悬挂指针([Dangling Pointer](https://en.wikipedia.org/wiki/Dangling_pointer))就是当指针指向的对象已经释放或回收后，但没有对指针做任何修改(一般来说，将它指向空指针)，而是仍然指向原来已经回收的地址。如果指针指向的对象已经释放，但仍然使用，那么就会导致程序crash。
当你运行MemoryProblems后，点击悬挂指针那个选项，就会出现EXC_BAD_ACCESS崩溃信息。
![166109-14751cda6424d749.png](http://cc.cocimg.com/api/uploads/20160217/1455702165562260.png)
我们看看这个NameListViewController是做什么的？它继承UITableViewController，主要显示多个名字的信息。它的实现文件如下：
```
`static NSString *const kNameCellIdentifier = @``"NameCell"``;`
`@interface NameListViewController ()`
`#pragma mark - Model`
`@property (strong, nonatomic) NSArray *nameList;`
`#pragma mark - Data source`
`@property (assign, nonatomic) ArrayDataSource *dataSource;`
`@end`
`@implementation NameListViewController`
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    ``self.tableView.dataSource = self.dataSource;`
`}`
`#pragma mark - Lazy initialization`
`- (NSArray *)nameList`
`{`
`    ``if``(!_nameList) {`
`        ``_nameList = @[@``"Sam"``, @``"Mike"``, @``"John"``, @``"Paul"``, @``"Jason"``];`
`    ``}`
`    ``return``_nameList;`
`}`
`- (ArrayDataSource *)dataSource`
`{`
`    ``if``(!_dataSource) {`
`        ``_dataSource = [[ArrayDataSource alloc] initWithItems:self.nameList`
`                                              ``cellIdentifier:kNameCellIdentifier`
`                                              ``tableViewStyle:UITableViewCellStyleDefault`
`                                          ``configureCellBlock:^(UITableViewCell *cell, NSString *item, NSIndexPath *indexPath) {`
`            ``cell.textLabel.text = item;`
`        ``}];`
`    ``}`
`    ``return``_dataSource;`
`}`
`@end`
```
要想通过tableView显示数据，首先要实现UITableViewDataSource这个协议，为了瘦身controller和复用data source，我将它分离到一个类ArrayDataSource来实现UITableViewDataSource这个协议。然后在viewDidLoad方法里面将dataSource赋值给tableView.dataSource。
解释完NameListViewController的职责后，接下来我们需要思考出现EXC_BAD_ACCESS错误的原因和位置信息。
一般来说，出现EXC_BAD_ACCESS错误的原因都是悬挂指针导致的，但具体是哪个指针是悬挂指针还不确定，因为控制台并没有给出具体crash信息。
**启用NSZombieEnabled**
要想得到更多的crash信息，你需要启动NSZombieEnabled。具体步骤如下：
1.选中Edit Scheme，并点击
![166109-f4e0337f766e1e89.png](http://cc.cocimg.com/api/uploads/20160217/1455702260463590.png)
2.Run -> Diagnostics -> Enable Zombie Objects
![166109-ae4f6b55212b75a9.png](http://cc.cocimg.com/api/uploads/20160217/1455702297971826.png)
设置完之后，再次运行和点击悬挂指针，虽然会再次crash，但这次控制台打印了以下有用信息：
![166109-9fe90d621bf6ce06.png](http://cc.cocimg.com/api/uploads/20160217/1455702322987711.png)
信息message sent to deallocated instance 0x7fe19b081760大意是向一个已释放对象发送信息，也就是已释放对象还调用某个方法。现在我们大概知道什么原因导致程序会crash，但是具体哪个对象被释放还仍然使用呢？
点击上面红色框的Continue program execution按钮继续运行，截图如下：
![166109-654444b25d8c5155.png](http://cc.cocimg.com/api/uploads/20160217/1455702351989090.png)
留意上面的两个红色框，它们两个地址是一样，而且ArrayDataSource前面有个_NSZombie_修饰符，说明dataSource对象被释放还仍然使用。
再进一步看dataSource声明属性的修饰符是assign
```
`#pragma mark - Data source`
`@property (assign, nonatomic) ArrayDataSource *dataSource;`
```
而assign对应就是__unsafe_unretained，它跟__weak相似，被它修饰的变量都不持有对象的所有权，但当变量指向的对象的RC为0时，变量并不设置为nil，而是继续保存对象的地址。
因此，在viewDidLoad方法中
```
`- (void)viewDidLoad {`
`    ``[``super``viewDidLoad];`
`    `
`    ``self.tableView.dataSource = self.dataSource;    `
`    ``/*  由于dataSource是被assign修饰，self.dataSource赋值后，它对象的对象就马上释放，`
`     ``*  而self.tableView.dataSource也不是strong，而是weak，此时仍然使用，所有会导致程序crash`
`     ``*/`
`}`
```
分析完原因和定位错误代码后，至于如何修改，我想大家都心知肚明了，如果还不知道的话，留言给我。
**内存泄露问题**
还记得上一篇iOS/OS X内存管理(一)：基本概念与原理的引用循环例子吗？它会导致内存泄露，上次只是文字描述，不怎么直观，这次我们尝试使用Instruments里面的子工具Leaks来检查内存泄露。
**静态分析**
一般来说，在程序未运行之前我们可以先通过Clang Static Analyzer(静态分析)来检查代码是否存在bug。比如，内存泄露、文件资源泄露或访问空指针的数据等。下面有个静态分析的例子来讲述如何启用静态分析以及静态分析能够查找哪些bugs。
启动程序后，点击静态分析，马上就出现crash
![166109-036d86de2b9e9424.png](http://cc.cocimg.com/api/uploads/20160217/1455702456834408.png)
此时，即使启用NSZombieEnabled，控制台也不能打印出更多有关bug的信息，具体原因是什么，等下会解释。
打开StaticAnalysisViewController，里面引用[Facebook Infer](http://fbinfer.com/)工具的代码例子，包含个人日常开发中会出现的bugs：
```
`@implementation StaticAnalysisViewController`
`#pragma mark - Lifecycle`
`- (void)viewDidLoad`
`{`
`    ``[``super``viewDidLoad];`
`    ``[self memoryLeakBug];`
`    ``[self resoureLeakBug];`
`    ``[self parameterNotNullCheckedBlockBug:nil];`
`    ``[self npeInArrayLiteralBug];`
`    ``[self prematureNilTerminationArgumentBug];`
`}`
`#pragma mark - Test methods from facebook infer iOS Hello examples`
`- (void)memoryLeakBug`
`{`
`     ``CGPathRef shadowPath = CGPathCreateWithRect(self.inputView.bounds, NULL);`
`}`
`- (void)resoureLeakBug`
`{`
`    ``FILE *fp;`
`    ``fp=fopen(``"info.plist"``, ``"r"``);`
`}`
`-(void) parameterNotNullCheckedBlockBug:(void (^)())callback {`
`    ``callback();`
`}`
`-(NSArray*) npeInArrayLiteralBug {`
`    ``NSString *str = nil;`
`    ``return``@[@``"horse"``, str, @``"dolphin"``];`
`}`
`-(NSArray*) prematureNilTerminationArgumentBug {`
`    ``NSString *str = nil;`
`    ``return``[NSArray arrayWithObjects: @``"horse"``, str, @``"dolphin"``, nil];`
`}`
`@end`
```
下面我们通过静态分析来检查代码是否存在bugs。有两个方式：
- 
手动静态分析：每次都是通过点击菜单栏的Product -> Analyze或快捷键shift + command + b
![166109-a890797a4457159d.png](http://cc.cocimg.com/api/uploads/20160217/1455702602889252.png)
- 
自动静态分析：在Build Settings启用Analyze During 'Build'，每次编译时都会自动静态分析
![166109-5c1dcdd871fcb891.png](http://cc.cocimg.com/api/uploads/20160217/1455702620911747.png)
静态分析结果如下：
![166109-6c032a57f0fef09b.png](http://cc.cocimg.com/api/uploads/20160217/1455702630773154.png)
通过静态分析结果，我们来分析一下为什么NSZombieEnabled不能定位EXC_BAD_ACCESS的错误代码位置。由于callback传入进来的是null指针，而NSZombieEnabled只能针对某个已经释放对象的地址，所以启动NSZombieEnabled是不能定位的，不过可以通过静态分析可得知。
**启动Instruments**
有时使用静态分析能够检查出一些内存泄露问题，但是有时只有运行时使用Instruments才能检查到，启动Instruments步骤如下：
1.点击Xcode的菜单栏的 Product -> Profile 启动Instruments
![166109-95b4ea305007d321.png](http://cc.cocimg.com/api/uploads/20160217/1455702712157999.png)
2.此时，出现Instruments的工具集，选中Leaks子工具点击
![166109-379b199e81584b16.png](http://cc.cocimg.com/api/uploads/20160217/1455702722661805.png)
3.打开Leaks工具之后，点击红色圆点按钮启动Leaks工具，在Leaks工具启动同时，模拟器或真机也跟着启动
![166109-03e04393903c0c6d.png](http://cc.cocimg.com/api/uploads/20160217/1455702752470838.png)
4.启动Leaks工具后，它会在程序运行时记录内存分配信息和检查是否发生内存泄露。当你点击引用循环进去那个页面后，再返回到主页，就会发生内存泄露
![166109-1148d40299015b5f.gif](http://cc.cocimg.com/api/uploads/20160217/1455702765336340.gif)
内存泄露.gif
![QQ截图20160217175300.png](http://cc.cocimg.com/api/uploads/20160217/1455702800976860.png)
如果发生内存泄露，我们怎么定位哪里发生和为什么会发生内存泄露？
**定位内存泄露**
借助Leaks能很快定位内存泄露问题，在这个例子中，步骤如下：
- 
首先点击Leak Checks时间条那个红色叉
![45.png](http://cc.cocimg.com/api/uploads/20160217/1455702906136270.png)
- 
然后双击某行内存泄露调用栈，会直接跳到内存泄露代码位置
![46.png](http://cc.cocimg.com/api/uploads/20160217/1455702899486042.png)
**分析内存泄露原因**
上面已经定位好内存泄露代码的位置，至于原因是什么？可以查看上一篇的[iOS/OS X内存管理(一)：基本概念与原理](http://www.jianshu.com/p/1928b54e1253)的循环引用例子，那里已经有详细的解释。
**难以检测Block引用循环**
大多数的内存问题都可以通过静态分析和Instrument Leak工具检测出来，但是有种block引用循环是难以检测的，看我们这个Block内存泄露例子，跟上面的悬挂指针例子差不多，只是在configureCellBlock里面调用一个方法configureCell。
```
`- (ArrayDataSource *)dataSource`
`{`
`    ``if``(!_dataSource) {`
`        ``_dataSource = [[ArrayDataSource alloc] initWithItems:self.nameList`
`                                              ``cellIdentifier:kNameCellIdentifier`
`                                              ``tableViewStyle:UITableViewCellStyleDefault`
`                                          ``configureCellBlock:^(UITableViewCell *cell, NSString *item, NSIndexPath *indexPath) {`
`                                              ``cell.textLabel.text = item;`
`                                              ``[self configureCell];`
`                                          ``}];`
`    ``}`
`    ``return``_dataSource;`
`}`
`- (void)configureCell`
`{`
`    ``NSLog(@``"Just for test"``);`
`}`
`- (void)dealloc`
`{`
`    ``NSLog(@``"release BlockLeakViewController"``);`
`}`
```
我们首先用静态分析来看看能不能检查出内存泄露：
![166109-c9f8a4c970462eb6.png](http://cc.cocimg.com/api/uploads/20160217/1455703034961910.png)
结果是没有任何内存泄露的提示，我们再用Instrument Leak工具在运行时看看能不能检查出：
![166109-68e795cea155fd8e.gif](http://cc.cocimg.com/api/uploads/20160217/1455703044559251.gif)
结果跟使用静态分析一样，还是没有任何内存泄露信息的提示。
那么我们怎么知道这个BlockLeakViewController发生了内存泄露呢？还是根据iOS/OS X内存管理机制的一个基本原理：当某个对象的引用计数为0时，它就会自动调用- (void)dealloc方法。
在这个例子中，如果BlockLeakViewController被navigationController pop出去后，没有调用dealloc方法，说明它的某个属性对象仍然被持有，未被释放。而我在dealloc方法打印release BlockLeakViewController信息：
```
`- (void)dealloc`
`{`
`    ``NSLog(@``"release BlockLeakViewController"``);`
`}`
```
在我点击返回按钮后，其并没有打印出来，因此这个BlockLeakViewController存在内存泄露问题的。至于如何解决block内存泄露这个问题，很多基本功扎实的同学都知道如何解决，不懂的话，自己查资料解决吧！
**总结**
一般来说，在创建工程的时候，我都会在Build Settings启用Analyze During 'Build'，每次编译时都会自动静态分析。这样的话，写完一小段代码之后，就马上知道是否存在内存泄露或其他bug问题，并且可以修bugs。而在运行过程中，如果出现EXC_BAD_ACCESS，启用NSZombieEnabled，看出现异常后，控制台能否打印出更多的提示信息。如果想在运行时查看是否存在内存泄露，使用Instrument Leak工具。但是有些内存泄露是很难检查出来，有时只有通过手动覆盖dealloc方法，看它最终有没有调用。
