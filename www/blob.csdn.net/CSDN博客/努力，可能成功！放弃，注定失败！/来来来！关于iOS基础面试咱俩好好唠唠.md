# 来来来！关于iOS基础面试咱俩好好唠唠 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月20日 17:49:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3312
> 
笔者之前在魔都从事iOS相关工作，前段时间决定来到帝都发展，找工作的过程中准备了些基础的面试题目，现分享给大家。
#### 1.1 谈一谈GCD和NSOperation的区别？
- 首先二者都是多线程相关的概念，当然在使用中也是根据不同情境进行不同的选择；
- GCD是将任务添加到队列中（串行/并发/主队列），并且制定任务执行的函数（同步/异步），其性能最好，底层是C语言的API，也更轻量级。iOS4.0以后推出的，针对多核处理器的并发技术，只能设置某一个队列的优先级，其高级功能有一次性执行dispatch_once，延迟操作dispatch_after，调度组等等；
- NSOperation把操作（异步）添加到队列中（全局的并发队列），是OC框架，更加面向对象，是对GCD的封装，iOS2.0推出，苹果推出GCD之后，对NSOperation的底层全部重写，可以随时取消已经设定准备要执行的任务，已经执行的除外，可以设置队列中每一个操作的优先级，其高级功能可以设置最大操作并发数，继续/暂停/全部取消，可以快队列设置操作的依赖关系。
#### 1.2 谈谈多线程的应用
通常耗时的操作都放在子线程处理，然后到主线程更新UI，如
- 我们要从数据库提取数据还要将数据分组后显示，那么就会开个子线程来处理，处理完成后才去刷新UI显示。
- 拍照后，会在子线程处理图片，完成后才回到主线程来显示图片。拍照出来的图片太大了，因此要做处理。
- 音频、视频处理会在子线程来操作
- 文件较大时，文件操作会在子线程中处理
- 做客户端与服务端数据同步时，会在后台闲时自动同步
#### 2. 线程之间是如何通信的？
- 通过主线程和子线程切换的时候传递参数performSelecter:onThread:withObject:waitUntilDone:
#### 3. 网络图片处理问题怎么解决图片重复下载问题？（SDWebImage大概实现原理）
- 这个就需要用到字典，以图片的下载地址url为key，下载操作为value，所有的图片大概分成三类：已经下载好的，正在下载的和将要下载的；
- 当一张图片将要进行下载操作的时候，先判断缓存中是否有相同的图片，如果有的话就返回，没有的话就去沙盒中找，有的话就拿出来用，没有的话再去以图片的url为key去字典中找有没有正在进行的任务，最后去判断等待的下载操作任务里面的字典有无相同key，如果没有，就自己开启任务，记录一下，文件保存的名称是url的md5值
- 
这里建立了两个字典 :
1.iconCache:保存缓存的图片
2.blockOperation 用来保存下载任务
![](http://upload-images.jianshu.io/upload_images/1501122-e719042afe2fa99e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
653183AF-3074-47AD-8460-10B5CEF1323C.png
- 
每当进入或退出程序时，会进行图片文件的管理：超过一星期的文件会被清除，如果设置了最大缓存，超过这个缓存就会删除最旧的文件，直到当前缓存文件为最大缓存文件的一半大小；
- 一般app中大部分缓存都是图片的情况下，可以直接调用clear方法进行清除缓存，getSize()方法获取当前缓存大小。
#### 4. 多线程安全的几种解决方法？
- 1> 只有在主线程刷新访问UI
- 2> 如果要防止资源抢夺，需要用synchronize进行加锁保护
- 3> 如果是异步操作要保证线程安全等问题，尽量使用GCD（有些函数默认就是安全的）
#### 5. 原子属性
- 原子属性采用的是"多读单写"机制的多线程策略；"多读单写"缩小了锁范围,比互斥锁的性能好
- 规定只在主线程更新UI,就是因为如果在多线程中更新,就需要给UI对象加锁,防止资源抢占写入错误,但是这样会降低UI交互的性能,所以ios设计让所有UI对象都是非线程安全的(不加锁)
#### 6. 代理的作用、block
- 代理又叫委托，是一种设计模式（可以理解为java中回调监听机制），代理是对象与对象之间的通信交互，代理解除了对象之间的耦合性
- 改变或传递控制链，允许一个类在某些特定时刻通知到其他类，而不需要获取到那些类的指针，可以减少框架复杂度
- 代理的属性常是assign的原因：防止循环引用，以致对象无法得到正确的释放
block底层是根据函数指针和结构体结合实现的，block本身就是结构体，更加简洁，不需要定义繁琐的协议方法，但通信事件比较多的话，建议使用Delegate
- block就是一个数据类型，存放一段代码，编译的时候不会执行，只有用到的时候才会去执行里面的代码。声明的时候使用copy是因为要从栈区拷贝到堆区，在栈区会受到作用域的限制，超出所在的函数就会被销毁，就没办法进行传值回调等一系列操作了。应注意循环引用，**weak来修饰。如果一个变量是在block外部创建，需要在block内部修改，那么需要使用**block修饰这个变量（**block可以在ARC和MRC情况下使用，可以修饰对象和基本数据类型，**weak只能在ARC下使用，只能修饰对象，不能修饰基本数据类型）
- 最常用的是使用block作为参数传值，不同情况下回调不同的代码（如成功回调失败回调）
#### 7. 谈谈你对runTime运行时机制的了解（注意哦，这个很重要的）
- runtime是一套比较底层的纯C语言API，属于一个C语言库，包含了很多底层的C语言的API
- 平时编写的OC代码，在程序运行过程中，其实都是转成了runtime的C语言代码，runtime是OC的幕后工作者，底层语言，例如：
- OC--> [[WPFPerson alloc] init]
- runtime-->objc_msgSend(objc_msgSend("WPFPerson", "alloc"), "init")
- 利用runtime可以实现一些非常底层的操作（用OC不好实现）
- 在程序运行过程中，动态创建一个类(比如KVO底层实现:检测isa指针，发现是新建了一个类，当然Xcode7.0以前的版本才可以监听到isa指针)
- 遍历一个类的所有成员变量、方法，访问私有变量（先通过runtime的class_getInstanceVariable获取成员变量，再通过class_getIvar获取它的值）
- 在程序运行过程中，动态为某个类添加属性\方法，修改属性值\方法，比如产品经理需要跟踪记录APP中按钮的点击次数和频率等数据，可以通过集成按钮或者类别实现，但是带来的问题比如别人不一定去实例化你写的子类，或者其他类别也实现了点击方法导致不确定会调用哪一个，runtime可以这样解决：在按钮的分类里面，重写load方法，利用dispatch_once保证只执行一次，新建监控按钮点击的方法，先用class_addMethod方法，判断其返回的bool值，如果添加成功，就用class_replaceMethod将原来的方法移除，如果添加失败，就用method_exchangeImplementations方法进行替换
- 拦截并替换方法，比如由于某种原因，我们要改变这个方法的实现，但是又不能动它的源码（比如一些开源库出现问题的时候，这时候runtime就可以出场了）-->先增加一个tool类，然后写一个我们自己实现的方法-change，通过runtime的class_getInstanceMethod获取两个方法，在用class_replaceMethod方法进行替换。防止数组越界的方法：数组越界的时候报错的方法是add_object，做一个逻辑判断，越界的时候通过class_replaceMethod交换掉add_object（相当于重写了这个方法）
- 相关应用
- NSCoding(归档和解档)，如果一个模型有很多个属性，那么需要对每个属性都实现一遍encodeObject和decodeObjectForKey方法，十分麻烦，但是如果使用class_copyIvarList获取所有属性，然后循环遍历，使用[ivarName substringFromIndex:1]去掉成员变量下划线
- 字典转模型：像几个出名的开源库JSONModel、MJExtension等都是通过这种方式实现的(利用runtime的class_copyIvarList获取属性数组，遍历模型对象的所有成员属性，根据属性名找到字典中key值进行赋值，当然这种方法只能解决NSString、NSNumber等，如果含有NSArray或NSDictionary，还要进行第二步转换，如果是字典数组，需要遍历数组中的字典，利用objectWithDict方法将字典转化为模型，在将模型放到数组中，最后把这个模型数组赋值给之前的字典数组)
- Method Swizzling：OC中调用方法事实上就是向对象发送消息，而查找消息的唯一依据就是selector的名字，因此可以使用runtime运行时机制动态交换方法。在+load方法里面调换，因为method swizzling的影响范围是全局的，所以应该放在最保险的地方来处理，+load方法能够保证能在类初始化的时候一定能被调用，可以保证统一性，如果是在使用的时候才去调用，可能达不到全局处理的效果；使用dispatch_once保证只交换一次。[objc_getClass("__NSArrayM")
 swizzleSelector:@selector(addObject:)
withSwizzledSelector:@selector(hyb_safeAddObject:)];使用场景：addObject方法添加的值为nil的时候会崩溃。调用objectAtIndex:时出现崩溃提示empty数组问题
#### 8. 谈谈你对Run Loop的理解
- RunLoop是多线程的一个很重要的机制，就是一个线程一次只能执行一个任务，执行完任务后就会退出线程。主线程会通过do-while死循环让程序持续等待下一个任务不退出。通过mach_msg()让runloop没事时进入trap状态，节省CPU资源。非主线程通常来说就是为了执行某个任务而创建的，执行完就会归还资源，因此默认不开启RunLoop
- 实质上，对于子线程的runloop是默认不存在的，因为苹果采用了懒加载的方式，如果没有手动调用[NSRunLoop currentRunLoop]的话，就不会去查询当前线程的RunLoop，也不会创建、加载
- 当然如果子线程处理完某个任务后不退出，需要继续等待接受事件，需要启动的时候也可以手动启动，比如说添加定时器的时候就要手动开始RunLoop
> 
如何处理事件
- 
界面刷新： 当UI改变（ Frame变化、 UIView/CALayer 的继承结构变化等）时，或手动调用了 UIView/CALayer 的 setNeedsLayout/setNeedsDisplay方法后，这个 UIView/CALayer 就被标记为待处理。 苹果注册了一个用来监听BeforeWaiting和Exit的Observer，在它的回调函数里会遍历所有待处理的 UIView/CAlayer 以执行实际的绘制和调整，并更新 UI 界面。
- 
手势识别： 如果上一步的 _UIApplicationHandleEventQueue() 识别到是一个guesture手势，会调用Cancel方法将当前的touchesBegin/Move/End 系列回调打断。随后系统将对应的 UIGestureRecognizer 标记为待处理。 苹果注册了一个 Observer 监测 BeforeWaiting (Loop即将进入休眠) 事件，其回调函数为 _UIGestureRecognizerUpdateObserver()，其内部会获取所有刚被标记为待处理的
 GestureRecognizer，并执行GestureRecognizer的回调。 当有 UIGestureRecognizer 的变化(创建/销毁/状态改变)时，这个回调都会进行相应处理。
- 
网络请求：最底层是CFSocket层，然后是CFNetwork将其封装，然后是NSURLConnection对CFNetwork进行面向对象的封装，NSURLConnection是iOS7中新增的接口。当网络开始传输时，NSURLConnection创建了两个新线程：com.apple.NSURLConnectionLoader和com.apple.CFSocket.private。其中CFSocket线程是处理底层socket连接的。NSURLConnectionLoader这个线程内部会使用RunLoop来接受底层socket的事件，并添加到上层的Delegate
> 
应用
- 
滑动与图片刷新：当tableView的cell上有需要从网络获取的图片的时候，滚动tableView，异步线程回去加载图片，加载完成后主线程会设置cell的图片，但是会造成卡顿。可以设置图片的任务在CFRunloopDefaultMode下进行，当滚动tableView的时候，Runloop切换到UITrackingRunLoopMode，不去设置图片，而是而是当停止的时候，再去设置图片。（在viewDidLoad中调用self.imageView performSelector@selector(setImage)
 withObject:...afterDelay:...inModes@[NSDefayltRunLoopMode]）
- 
常驻子线程，保持子线程一直处理事件 为了保证线程长期运转，可以在子线程中加入RunLoop，并且给Runloop设置item，防止Runloop自动退出
#### 9. SQLite常用的SQL语句
- 创建表：create table 表名(字段名 字段数据类型 是否为主键, 字段名 字段数据类型, 字段名 字段数据类型...)
- 增：insert into 表名(字段1，字段2...) values(值1，值2...)
- 删：delete from 表名 where 字段=值
#### 10. 关于Socket，谈谈TCP/IP 和 UDP的理解
- Socket是一个用于传输网络数据的工具，TCP/IP 和 UDP都是传输协议，用于定义网络数据传输的格式，属于长连接
- TCP/IP 侧重可靠传输，传输速度慢，不会丢失数据，安全，聊天和下载文件时用到
- UDP：侧重快速传输，传输速度快，容易丢失数据包，不安全。局域网游戏和网络游戏，视频聊天的时候用到
- TCP更安全是因为有一个三次握手：第一次握手（客户端发送syn包到服务器，并进入SYN_SEND状态，等待服务器确认），第二次握手（服务器收到syn包，必须确认客户的SYN包，同时自己发送一个SYN+ACK包，此时服务器进入SYN_RECV状态），第三次握手（客户端收到服务器的SYN+ACK包，向服务器发送确认包ACK，发送完毕后服务器和客户端都进入ESTABLISHED状态，完成三次握手），三次握手之后才开始正式传输数据。因此使用TCP协议的MSN比采用UDP的QQ传输速度慢，但并不代表不安全，因为程序员可以手动对UDP的数据收发进行验证（比如发送方对每个数据包进行编号然后由接收方进行验证）
- Http：超文本传输协议，用于定义网络数据传输的格式(短链接）http1.0之前不支持短连接，1.1之后默认就是长连接，只要在服务器和客户端同时设置Connection为keep-alive即可
- 长连接是为了复用，长连接指的是TCP连接，也就是为了复用TCP连接，也就是说多个HTTP请求可以复用一个TCP连接，节省了很多TCP连接建立和断开的消耗
- 比如请求了一个网页，这个网页肯定还包含了CSS、JS等一系列资源，如果是短连接的话，每次打开一个网页，基本要建立几个甚至几十个TCP连接，浪费了大量资源
- 长连接不是永久连接，如果一段时间内，具体的时间长短，是可以在header当中进行设置的，也就是所谓的超时时间，这个连接没有HTTP请求发出的话，那么这个长连接就会被断掉
- socket连接是长连接，客户端与服务器保持通道，双方可以主动发送数据，一般多用于即时通讯，游戏，默认超时时间是30秒，默认大小是8k（一个数据包大小）
#### 11. 谈一谈内存管理
- iOS的内存管理分为 MRC 和 ARC，管理的是堆区动态产生的对象，基本数据类型就不是内存管理的范围
- 内存管理的核心概念是引用计数器：当对象被alloc、copy、new的时候，引用计数器+1，当被release的时候引用计数器—1，为0的时候就会被系统回收，调用dealloc方法
- 说道内存管理，就必须说说@property的内存管理参数：
- retain --> release 一次旧对象 retain 一次新对象 (适用于OC对象类型)
- copy --> release 一次旧对象 拷贝一个新对象出来
- 如何避免内存泄露 --> 使用Analyze进行代码的静态分析
- 当然使用block的时候最应该注意下循环引用，使用Leaks检测内存泄露，显示绿色的勾告知内存处理的不错，实际上内存得不到释放。一般我的方法是在控制器声明周期的viewDidAppear和dealloc方法里面打印日志[[self class] description]，如果没有打印出来，就说明没有被释放。使用**weak**typeof(self) weakSelf = self;解决。有一次我是直接使用成员变量，而不是属性，_age，我以为这样没有使用self就可以了，但是后来测试发现还是造成循环引用了，因为_age是控制器的成员变量，也就是强引用了控制器，也要改成弱引用**block**weak __typeof(_currentModel) weakModel = _currentModel;
#### 12. 常见的数据持久化有哪些
- 偏好设置（preference），利用NSUserDefaults
- 用来保存应用程序设置和属性、用户保存的数据。用户再次打开程序或开机后这些数据仍然存在
- NSUserDefaults可以存储的数据类型包括：NSData、NSString、NSNumber、NSDate、NSArray、NSDictionary。如果要存储其他类型，需要先转化为前面的类型，才能用NSUserDefault存储
- 偏好设置是专门用来保存应用程序的配置信息的，一般不要在偏好设置中保存其他数据
- 偏好设置会将所有数据保存到同一个文件中。即preference目录下的一个以此应用包名来命名的plist文件。
```
//1.获得NSUserDefaults文件
NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
//2.向文件中写入内容
[userDefaults setObject:@"AAA" forKey:@"a"];
[userDefaults setBool:YES forKey:@"sex"];
[userDefaults setInteger:21 forKey:@"age"];
//2.1立即同步
[userDefaults synchronize];
//3.读取文件
NSString *name = [userDefaults objectForKey:@"a"];
BOOL sex = [userDefaults boolForKey:@"sex"];
NSInteger age = [userDefaults integerForKey:@"age"];
NSLog(@"%@, %d, %ld", name, sex, age);
```
- 
归档(Archiver)、解档(unArchiver)，利用NSKeyedArchiver实现归档、利用NSKeyedUnarchiver反接的那个
- 归档及时将内存中的对象写入到磁盘文件中，归档也叫序列化，解档就是讲磁盘中文件中的对象读取出来
- 必须遵循NSCoding协议，只要遵循了NSCoding协议的对象都可以通过它实现序列化，两个协议方法必须实现
```
// 反归档
- (id)initWithCoder:(NSCoder *)aDecoder {
if ([super init]) {
    self.avatar = [aDecoder decodeObjectForKey:@"avatar"];
    self.name = [aDecoder decodeObjectForKey:@"name"];
    self.age = [aDecoder decodeIntegerForKey:@"age"];
}
return self;
}
// 归档
- (void)encodeWithCoder:(NSCoder *)aCoder {
[aCoder encodeObject:self.avatar forKey:@"avatar"];
[aCoder encodeObject:self.name forKey:@"name"];
[aCoder encodeInteger:self.age forKey:@"age"];
}
```
- 
归档，把对象归档时需要调用NSKeyedArchiver的工厂方法archiveRootObject: toFile: 方法
```
NSString *file = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject stringByAppendingPathComponent:@"person.data"];
Person *person = [[Person alloc] init];
person.avatar = self.avatarView.image;
person.name = self.nameField.text;
person.age = [self.ageField.text integerValue];
[NSKeyedArchiver archiveRootObject:person toFile:file];
```
- 
反归档
```
NSString *file = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject stringByAppendingPathComponent:@"person.data"];
Person *person = [NSKeyedUnarchiver unarchiveObjectWithFile:file];
if (person) {
self.avatarView.image = person.avatar;
self.nameField.text = person.name;
self.ageField.text = [NSString stringWithFormat:@"%ld", person.age];
}
```
- 
属性列表
- 
数据库：SQLite
- Core Data
[点击查看大神讲解](http://www.jianshu.com/p/6e048f7c5812)
- 属性列表
#### 这五种持久化操作不同点
- 
从存储数据大小来看，归档、偏好设置、属性列表三种方法适合存储数据量较小的数据，数据库、CoreData方法适合存储数据量较大的数据
- 
从加密性来看，其中归档会将数据进行加密，而偏好设置是直接保存到属性列表中，不会对数据进行加密
- 
从存储类型来看，属性列表只能存放固定的七种类型（可在plist文件中看到），归档对存储类型无限制
#### 13. KVC 和 KVO
- KVC(key-value-coding键值编码，跟多情况下会简化程序代码)的常见用法：
- 给私有变量（该变量不对外开放）赋值：[Person setValue: @"19" ForKeyPath:@"age"]
- 字典转模型：setValuesForKeyWithDictionary
- 取出私有变量：[Person valueForKey:@"age"]
- 没有找到对应的key会崩溃：重写setValueForUndefinedKey
- 
KVC缺点：一旦使用KVC，编译器无法检查出错误，即不会对设置的键、键路径进行错误检查，且执行效率低于自定义的setter和getter方法，因为使用KVC键值编值，必须先解析字符串，然后设置或访问对象的实例变量
- 
通过KVO（key-value-observing，典型的观察者模式，被观察的对象必须使用KVC键值编码来修改它的实例变量，这样才能被观察者观察到）监听person对象中name属性发生改变
- 给监听的属性设置一个观察者：
`[self.person addObserver:self forKeyPath:@"name" options:NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld context:nil];`
- 当person的name的值发生改变时,就会执行该方法
```
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context
{
  do something....
}
```
- 当一个类的属性被观察的时候，系统会通过runtime动态的创建一个该类的派生类，并且会在这个类中重写基类被观察的属性的setter方法，而且系统将这个类的isa指针指向了派生类（NSNotifying_类名），从而实现了给监听的属性赋值时调用的是派生类的setter方法。重写的setter方法会在调用原setter方法前后，通知观察对象值得改变。
#### 14. @synthesize和@dynamic区别是什么
- 这两个关键字都是@property对应的词
- @synthesize 语义是如果没有手动实现setter和getter方法，那么编译器会自动帮你加上这两个方法
- @dynamic告诉编译器，属性的setter和getter由用户自己实现，不自动生成（readOnly只实现getter即可），但是如果没有自己实现，编译的饿时候不会报错，运行的时候就会报错，这就是所谓的动态绑定
#### 15. 谈谈时间响应链的一般顺序
- 一般来说，第一响应者是个视图对象或者其他子类随想，当其被触摸以后事件遍交由他处理，如果他不处理，时间就会被传递给它的视图控制器对象viewController（如果存在），然后是它的父视图对象（superView）（如果存在），一次类推，知道顶层视图，接着沿着顶层视图（topView）到窗口（UIWindow对象）再到程序（Application对象）。如果整个过程都没有响应这个时间，该事件就会被抛弃。一般情况下，在响应链中只要有UI系那个处理事件，事件就会停止传递
#### 16. post和get方式的区别
- 
GET请求的数据会负载URL之后，即把数据放在HTTP协议头中，以？区分URL和传输数据，参数之间以&相连，英文字母/数字，原样发送，如果是空格，转化为+，如果是中文，把字符串用BASE64加密；POST就是把提交的数据放在HTTP包的包体中
- 
GET一般用于提交少量数据（最多提交1k，浏览器限制），POST用于提交大量数据（理论上无限制，收服务器限制）
- 
GET提交的数据可以在浏览器历史记录中看到，安全性不好，别人可以拿到账号密码
- 
Get是向服务器发索取数据的一种请求，而POST是向服务器发提交数据的一种请求，只是发送机制不同
- 
GET不可以设置书签，POST可以设置书签
- 
什么情况下用POST：
- 请求的结果具有持续性副作用，如数据库添加新的数据行
- 若使用get方法，则表单上手机的数据可能让URL过长
- 要传送的数据不是采用7位的ASCII编码
- 
什么情况下用GET：
- 请求是为了查找资源，HTML表单数据仅用来帮助搜索
- 请求结果无持续副作用性的副作用
- 手机的数据及HTML表单内的输入字段名称的总长不超过1024个字符
#### 17. 深复制和浅复制
- 非集合类对immutable对象进行copy操作，是指针复制，mutableCopy操作时内容复制
- 非集合类对mutable对象进行copy和mutableCopy都是内容复制。
- 在集合类对象中，对immutable对象进行copy，是指针复制，mutableCopy是内容复制
- 在集合类对象中，对mutable对象进行copy和mutableCopy都是内容复制。但是：集合对象的内容复制仅限于对象本身，对象元素仍然是指针复制。
- NSString *str = @"string"; str = @"newString"; 打印对象地址，发现是发生变化的，需要把@"newStirng"当做一个新的对象，将这段对象的内存地址赋值给str
#### 18. 关于项目中动画的使用
- 序列帧动画：self.imageView.animationImages = array;
- [UIView animateWithDuration] + CGAffinetransform
- 核心动画CABasicAnimation *anim = [CABasicAnimation animationWithKeyPath:@"position.y"]; anim.fromValue toValue repeatCount [btn.layer addAnimation]
- 关键帧动画CAKeyframeAnimation，anim.values = array，添加到layer上
- 组动画CAAnimationGroup，将以上动画组合起来
- 转场动画：CATransition，设置duration和type，然后添加到layer上。利用UIView 的类方法实现转场动画
[UIView transitionWithView: duration: options: animations:^{ } completion:nil];
- UIDynamicAnimator仿真者 、 UISnapBehavior吸附行为，设置damping来调节震动幅度 、 UIPushBehavior推动行为 、 UICollisionBehavior碰撞边缘检测行为 、 UIAttachmentBehavior附着行为 、 UIGravityBehavior重力行为
- POPSpringAnimation
- springBounciness[0,20]越大振幅越大。
- springSpeed速度
#### 19. 谈谈你对MVVM的认识
> 
其实MVVM这货有点被神化了，好吧，其实我项目中也没用到过，只是写过稍大点的demo
- 说到MVVM，就必须先说MVC，MVC就是Model显示数据，View呈现用户界面，Controller调节二者之间的交互，但是这个结构存在的问题就是模型的代码太少，而随着功能的增加控制器的代码会越来越多，而且不方便测试（比如办一个饭卡，默认一百元）；
- 在MVVM中，view和view controller正式联系在一起，我们将它视为一个组件，这两个都不能直接引用model，而是引用视图模型view model，里面放置着用户输入验证逻辑，视图显示逻辑，发送网络请求等代码，view model再去引用model
- 这个的优点呢，首先就是低耦合，view可以独立于model变化和修改，一个viewModel可以绑定到不同的view上，可重用性，也有利于独立开发，开发人员可以专注业务逻辑和数据的开发，设计人员可以专注于页面设计
- 
实际用例
- 楼市餐厅需要充餐卡，默认是100元，在模型层中，如果想保存这个值 --> let balanece = 100
- 但是展示给用户的时候，我们想呈现出“您的账户余额为：￥100”，在mvc中，这种代码只能放到视图控制器中，显得很臃肿，如果放在模型当中，会更丑，因为有许多进行格式化的代码挤在其中
- 
如果添加视图模型，仅仅需要映射一下原始数据
```
struct AccountViewModel {
let dispalyBalance: String
init(mode: cardAccount) {
  let formattedBalance = model.balance.currencyValue;
  displayBalance = "Your balance is \(formatterBalance)"
}
}
```
- 通过这种方式，视图模型实际上会读取数据模型，然后将其中的信息进行格式化，从而准备展现在视图当中，很容易测试，直接把带有账户信息的模型放进去，然后测试显示就可以了，在之前是特别复杂的
- 
在去年WWDC上，Andy（iOS4.1-8的UIKit框架维护者）演讲中有一个关于zoetrope（西洋镜）的例子
- 大概意思就是每一帧都是静态值，可以通过改变任务手部抬起的距离，或者任务头部倾斜的距离，来对字符进行编码。每一帧都是静态的，但是当把他们放在一起，然后一直看向一个中心的话，那么始终都有新的数据出现，这样就可以得到一个而美丽的、生动的动画（类比Tom猫）
- 我们可以使用相同的方式来实现值类型，视图控制器会跟随zoetrope的最后一个帧图像--也就是最新的一块活跃数据，然后将其展示给用户。只要您的模型发生了更新，视图就会根据最新的信息进行更改了
`var viewModel = ViewModel(model: Account)`
#### 20.0 关于Swift与OC的不同
> 
其实是个面试官八个不懂Swift，而且一般不懂得就问个Swift与OC的不同。。题主也只是在自学Swift，等到3.0出了之后再深入研究，而且项目中可能也要开始从混编逐渐向Swift靠拢了。
- 
Swift是一门更加现代化的语言，但是目前还在成长阶段，更新改动比较大，虽然说其底层思想不变，变的是API和接口，但是每次更新完Xcode看到自己的Swift项目还是有些淡淡的忧伤，而且目前Swift开发都要转成OC的runtime，包略大，因此题主认为成熟项目最好还是采用OC
- 
先记住一句话：OC底层面向对象，而Swift底层更加面向协议
- 
我们已经见识过Apple使用了大量协议，比如在tableView当中，我们可以通过协议来告诉Apple需要多少个表视图单元格，而不是每时每刻都要继承UITableViewController
- 
在这里以MVVM作为测试用例：比如现在需要建立一个类似设置界面的tableView，每个cell需要一个label和一个switch，自定义SwitchWithTextTableViewCell，在其内部建立一个configure方法中对label的title，titleFont，titleColor，switch的switchOn和switchColor等进行初始化，但这种方式非常累赘，比如添加一个副标题，就需要额外添加三个属性
- 
但是利用协议SwitchWithTextCellProtocol，让视图模型实现这个协议，然后在这里设置所有的属性
```
protocol SwitchWithTextCellProtocol {
  var title: String { get }
  var titleFont: UIFont { get }
  var titleColor: UIColor { get }
  var switchOn: Bool { get }
  var switchColor: UIColor { get }
  func onSwitchTogglenOn(onL Bool)
}
```
- 
通过swift2.0重点饿协议扩展，就可以通过默认值来做一些处理了，如果对于大多数单元格来说，可以确定某一种颜色的话，就可以对其建立扩展，然后设置颜色即可，所有实现此协议的视图就没有必要再去设置这个颜色了
- 
现在，我的configure方法里面只要实现此协议的值就可以了
```
// 这个方法只需要一个参数，相比于之前的多个参数简便了很多
class SwitchWithTextTableViewCell: UITableViewCell {
  func configure(withDelegate delagate: SwitchWithTextCellProtocol) {
      // 在这里配置方法
  }
}
```
- 
现在的视图模型
```
struct MinionModeViewController: SwitchWithTextCellProtocol {
  var title = "excellent!!"
  var switchOn = true
  var switchColor: UIColor {
      return .yellowColor()
  }
  func onSwitchToggleOn(on: Bool) {
      if on {
          print("The Minions are here to stay!")
      } else {
          print("The Minions went out to play!")
      }
  }
}
```
- 现在，cellForRowAtIndexPath()也变得非常简明了
```
let cell = tableView.dequeueReuseableCellWithIdentifier("SwitchWithTextTableViewCell", forIndexPath: indexPath) as! SwitchWithTextTableViewCell
cell.configure(withDelegate: MinionModeViewModel())
return cell
```
> 
再把模型放在视图模型层级，一遍对其进行跟踪，再视图模型中传递这些信息，这样单元格就可以生成了
- 但是在这个基础上，还可以再做进一步的深化，就是建立两个协议，一个作为实际编码的数据源，比如标题内容之类的实际数据，一个作为单元格委托，存储颜色、字体之类的并没有包含实际数据的信息，也就是仿照Apple中UITableView等集合视图之类的地方，按照这种思维去建立单元格存储和单元格委托
```
protocol SwitchWithTextCellDataSource {
    var title: String { get }
    var switchOn: Bool { get }
}
protocol SwitchWithTextCellDelegate {
    func onSwitchTogglenOn(on: Bool)
    var switchColor: UIColor { get }
    var textColor: UIColor { get }
    var font: UIFont { get }
}
```
- 
接下来，再让configure方法同时接受这两个协议。因为委托可以全部在协议扩展中使用默认值进行配置，比如说字体、颜色之类的信息，这样在理论上就可以不用向里面传递任何东西进去，只需要创建一个模型就可以了
```
// SwitchWithTextTableViewCell
func configure(withDataSource dataSource: SwitchWithTextCellDataSource, delegate: SwitchWithTextCellDelegate?) {
  // 在这里配置视图
}
```
- 
然后就要开始通过扩展来改进视图模型了，使用一个实际数据源的代码块，然后给定要传递的视图当中的原始信息
```
struct MinionModeViewModel: SwiftWithTextCellDataSource {
  var title = "Minion Mode!!"
  var switchOn = true
}
```
- 
接下来会在一个单独的视图模型的部分使用处理字体、颜色之类的委托，然后在其中进行相关的配置
```
extension MinionModeViewModel: SwitchWithTextCellDelegate {
  var switchColor: UIColor {
      return .yellowColor()
  }
  func onSwitchToggleOn(on: Bool) {
      if on {
          print("The Minions are here to stay!")
      } else {
          print("The Minions went out to play!")
      }
  }
}
```
- 
最终，表格视图单元格变得非常简单
```
// SettingViewController
let viewModel = MinionModeViewModel()
cell.configure(withDataSource:viewModel, delegate: viewModel)
return cell
```
> 
仅仅需要创建视图模型，然后将其传递到配置方法当中，最后返回单元格，就可以了
#### 20.1 Swift2.0中的 Minxin 和 Trait
- 
在游戏开发中通常会有一个很庞大的层级关系，以及一系列的继承，比如各种怪，继承在这里显得十分有意义，但是随着层级的扩展，这个项目就会变得凌乱起来
- 
比如说需要设计一个可以射击的怪物，但这时候塔防顶部的大炮也会射击，就需要把“射击辅助类”提取出来，但是如果一直这样提取子类，代码后面会一团乱麻
- 
将这个代码重构，不再去提取能够射击或者能够加血的子类，而是将其提取为协议，通过协议扩展来实现这个功能，代码更加简洁，更利于理解
```
// 一看这个对象的类型，就知道他有哪些功能，而不是一个个去查找她的实现
class ZapMonster: GameObject, GunTraint, HealthTraint, MovementTraint {
  ...
}
```
- 
虽然说这种设计模式是游戏方面的，但是我们平时的代码也可以参考这种设计模式：这样就不需要让实际的单元格实现这个协议了，只需要将其根更广泛的TextPresentable 联系在一起就可以了，这样，任何拥有标签的视图，而不仅仅是单元格，都可以实现这个协议来弯沉相关的功能。这样就可以说这个标签有什么样的温恩，什么样的颜色，以及什么样的字体
```
protocol TextPresentable {
    var text: String { get }
    var textColor: UIColor { get }
    var font: UIFont { get }
}
protocol SwitchPresentable {
    var switchOn: Bool { get }
    var switchColor: UIColor { get }
    func onSwitchToggleOn(on: Bool)
}
```
> 
这种情况下，比如需要一个图片框，只要一个iamgeProtocol就可以了，设计师要求改所有标签的颜色的话一行代码就可以搞定
- 现在单元格的模样
```
class SwitchWithTextTableViewCell<T where T: TextPresentable, T: SwitchPresentable>: UITableViewCell {
    private var delegate: T?
    // T是视图模型
    func configure(withDelegate delegate: T) {
        // 在这里配置视图
    }
}
```
> 
在这种情况下，它没有实现这些协议，但是会期待某种实现这些协议的东西传递进去，因此我们使用了泛型，这个单元格期待了一个实现了TextPresentableProtocol 的委托。就我们而言，传递进去的将是一个实现了这些协议的东西就可以了，现在要基于这些信息在单元格当中配置所有的东西了，现在就可以基于浙西而信息在单元格中配置所有的东西了
```
extension MinionModeViewModel: TextPresentable {
    var text: String { return "Minion Mode" }
    var textColor: UIColor { return .blackColor() }
    var font: UIFont { return .systemFontOfsize(17.0) }
}
```
- 
我们的视图模型将会有一个TextPresentable代码，在其中可以配置文本、颜色、字体，并且由于所有的这些协议扩展中都已经有默认值了，甚至不需要视图模型去实现这些具体的内容
- 
最后，视图模型当中的代码就只需要dequeue相应的单元格。然后通过视图模型对其进行配置，然后返回单元格即可
#### 20.2 Swift2.2随着iOS9.3一同登场，讲讲什么新调整？
- ++ 和 -- 被deprecate掉了，2.2版本会有一个警告，但是3.0必然会完全移除，使用 +=1 和 -=1 替代；
- 传统C 风格的for循环被干掉，只能用 for in；
- var 参数被废除了
- 使用var， 让你在函数内部修改参数
- 使用inout，可以让你的改变延续到函数结束后
- 允许更多的参数作为参数标签：for i 1.stride(to: 9, by: 2){print(i)} 从1开始，每次加2，但结果要一直小于9，代码的自解释性更好；
- 字符串作为 selector 也被 deprecated 掉了，可以通过 #selector(Swift方法名)来实现 selector，发生拼写错误也会得到提醒
- 还有一些没记住。。反正我的demo中需要用到的地方这几点比较多
#### 21. 优化tableViewCell高度
- 
一种是针对所有 Cell 具有固定高度的情况，通过：self.tableView.rowHeight = 88;
指定了一个所有 cell 都是 88 高度的 UITableView，对于定高需求的表格，强烈建议使用这种（而非下面的）方式保证不必要的高度计算和调用。
- 
另一种方式就是实现 UITableViewDelegate 中的：heightForRowAtIndexPath:需要注意的是，实现了这个方法后，rowHeight 的设置将无效。所以，这个方法适用于具有多种 cell 高度的 UITableView。
- 
iOS7之后出了了estimatedRowHeight，面对不同高度的cell，只要给一个预估的值就可以了，先给一个预估值，然后边滑动边计算，但是缺点就是
- 设置估算高度以后，tableView的contentSize.height是根据cell高度预估值和cell的个数来计算的，导致导航条处于很不稳定的状态，因为contentSize.height会逐渐由预估高度变为实际高度，很多情况下肉眼是可以看到导航条跳跃的
- 如果是设计不好的上拉加载或下拉刷新，有可能使表格滑动跳跃
- 估算高度设计初衷是好的，让加载速度更快，但是损失了流畅性，与其损失流畅性，我宁愿让用户加载界面的时候多等那零点几秒
- 
iOS8 WWDC 中推出了 self-sizing cell 的概念，旨在让 cell 自己负责自己的高度计算，使用 frame layout 和 auto layout 都可以享受到：
- self.tableView.estimatedRowHeight = 213;
self.tableView.rowHeight = UITableViewAutomaticDimension;
如果不加上估算高度的设置，自动算高就失效了
- 这个自动算高在 push 到下一个页面或者转屏时会出现高度特别诡异的情况，不过现在的版本修复了。
- 
相同的代码在 iOS7 和 iOS8 上滑动顺畅程度完全不同，iOS8 莫名奇妙的卡。很大一部分原因是 iOS8 上的算高机制大不相同,从 WWDC 也倒是能找到点解释，cell 被认为随时都可能改变高度（如从设置中调整动态字体大小），所以每次滑动出来后都要重新计算高度。
- dequeueReusableCellWithIdentifier:forIndexPath: 相比不带 “forIndexPath” 的版本会多调用一次高度计算
- iOS7 计算高度后有”缓存“机制，不会重复计算；而 iOS8 不论何时都会重新计算 cell 高度
- 
使用 UITableView+FDTemplateLayoutCell（百度知道负责人孙源） 无疑是解决算高问题的最佳实践之一，既有 iOS8 self-sizing 功能简单的 API，又可以达到 iOS7 流畅的滑动效果，还保持了最低支持 iOS6
- 
FDTemplateLayoutCell 的高度预缓存是一个优化功能，利用RunLoop空闲时间执行预缓存任务计算，当用户正在滑动列表时显然不应该执行计算任务影响滑动体验。
- 当用户正在滑动 UIScrollView 时，RunLoop 将切换到 UITrackingRunLoopMode 接受滑动手势和处理滑动事件（包括减速和弹簧效果），此时，其他 Mode （除 NSRunLoopCommonModes 这个组合 Mode）下的事件将全部暂停执行，来保证滑动事件的优先处理，这也是 iOS 滑动顺畅的重要原因
- 注册 RunLoopObserver 可以观测当前 RunLoop 的运行状态，并在状态机切换时收到通知：
- RunLoop开始
- RunLoop即将处理Timer
- RunLoop即将处理Source
- RunLoop即将进入休眠状态
- RunLoop即将从休眠状态被事件唤醒
- RunLoop退出
- 分解成多个RunLoop Source任务，假设列表有 20 个 cell，加载后展示了前 5 个，那么开启估算后 table view 只计算了这 5 个的高度，此时剩下 15 个就是“预缓存”的任务，而我们并不希望这 15 个计算任务在同一个 RunLoop 迭代中同步执行，这样会卡顿 UI，所以应该把它们分别分解到 15 个 RunLoop 迭代中执行，这时就需要手动向 RunLoop 中添加 Source 任务（由应用发起和处理的是 Source 0 任务）
#### 22. 列举一下你常用的第三方库
- 
Xcode插件
- Alcatraz：Xcode 插件管理工具
- ColorSense-for-Xcode：代码生成颜色预览，可视化编辑
- KSImageNamed-Xcode：引入图片自动提示，预览
- VVDocumenter-Xcode：规范化注释
- 
项目中常用的第三方库
- AFNetworking：网络库，通常会在AFN上面再封装一层，主要封装接口逻辑
- SDWebImage：下载网络图片，定时清除缓存
- Reachability：网络状态判断，AFN已经有这个功能
- WebViewJavaScriptBridge：Webview和cocoa之间消息传递
- fmdb：SQLite的封装，简单易用
- DTCoreText：CoreText库，支持HTML
- KissXML：XML解析，支持读取和修改，基于libxml
- ZXingObjC：二维码，支持编码解码
- GTMBase64：base64编解码
- GPIImage：图像处理
- JSONKit：json解析，性能最好
- Mansonry：辅助自动布局
- MJRefresh：上拉加载，下拉刷新
- MBProgressHUD：进度图，加载效果提示
#### 23. 为什么AFN显示图片不如SDWebImage流畅？同样是从网络上下载图片而不是从缓存取图片？
- 因为SDWebImage有一个decoder
- UIImage的imageWithData函数是每次画图的时候才将Data解压成ARGB的图像
- 所以每次画图的时候，会有一个解压操作，这样效率很低，但是只有瞬时的内存需求
- 为了提高效率通过SDWebImageDecoder将包装在Data的资源解压，然后画在另外一张图片上，这样新的图片就不再需要重复解压了
- 这是典型的空间换时间的做法
#### 24. 关于新特性
> 
不知为何每次面试官问这个问题我都感觉好low，虽然能吹牛逼吹好久。。
iOS7新特性
- 在iOS7当中，使用麦克风也需要取得用户同意了。如果用户不允许app使用麦克风的话，那么需要使用麦克风的app就不能接收不到任何声音
- [NSArray firstObject]的实现，iOS4之前只是一个私有的方法
- UIImage.renderingMode着色（Tint Color），可以设置一个UIImage在渲染时是否使用当前视图的Tint Color。
- UIScreenEdgePanGestureRecognizer可以从屏幕边界即可检测手势
- 使用Core Image来检测眨眼以及微笑iOS给Core Image增加了两种人脸检测功能：CIDetectorEyeBlink以及CIDetectorSmile。这也就是说你现在可以在照片中检测微笑以及眨眼。
> 
iOS8新特性
- 当使用iOS8定位的时候需要请求用户授权，且在info.plist里添加字段NSLocationAlwaysUsageDescription 请求用户授权的描述
- 
size classes是为了解决storyboard只能订制一种屏幕样式的问题，它不再是具体的尺寸，而是抽象尺寸通过宽/高 的compact、any、regular 组成了九种组合包含了所有苹果设备的尺寸。
- 
iOS8中，字体是Helvetica，中文的字体有点类似于“华文细黑”。只是苹果手机自带渲染，所以看上去可能比普通的华文细黑要美观。iOS9中，中文系统字体变为了专为中国设计的“苹方” 有点类似于一种word字体“幼圆”。字体有轻微的加粗效果，并且最关键的是字体间隙变大了！
> 
iOS9新特性
- iOS9系统发送的网络请求将统一使用HTTPs，将不再默认使用HTTP等不安全的网络协议，而默认采用TLS 1.2。服务器因此需要更新，以解析相关数据。如不更新，可通过在 info.plist 中声明，倒退回不安全的网络请求。
- 将允许出现这种场景：同一app中多个location manager：一些只能在前台定位，另一些可在后台定位
- 
bitcode的理解应该是把程序编译成的一种过渡代码，然后苹果再把这个过渡代码编译成可执行的程序。bitcode也允许苹果在后期重新优化我们程序的二进制文件，有类似于App瘦身的思想。
- 
stackView
- 
Multasking：多任务特性，三种形式
- 临时调出的滑动覆盖：Slide Over
- 视频播放的画中画模式（Picture in Picture）（AVPlayerViewController默认支持。MPMoviePlayerViewController被deprecated掉了，不支持）
- iPad真正同时使用两个App
- 
UI Test：iOS9.0之前加入异步代码测设和性能测试，可以说Xcode自带的测试框架已经能满足绝大部分单元测试的需求了，但是这并不够，因为开发一个iOS app从来都是很注重UI和用户体验的，之前UI测试使用KIF，Automating，iOS9.0的Xcode给出了自带的XCUITest的一系列工具，和大多数UI测试工具类似，XCUI使用Accessbility标记来确定view，但因为是Apple自家的东西，可以自动记录操作流程，所以只要书写最后的验证部分就好了，比其他UI测试工具方便多了
- Swift2
- APP Thinning：app为了后向兼容，都同时包含了32bit和64bit，在图片资源2X和3X的一应俱全，下载的时候只需要当前机型对应的一套资源，但是却要全部打包下载，现在只需要升级iOS9，就可以省很多流量
- 3D touch
- 地图显示实时的交通状况
- 人工智能siri更加智能，几个大城市的地铁及火车站入口都有详细的标识
- 手机电池的低功耗设置
- Spootlight，你的设备会向推荐最近通话过的联系人，使用过的APP以及你可能感兴趣的去处、信息呈现更精彩
#### 25. 我是怎样用两个imageView实现了无线轮播！
- 建立一个scrollView，设置contentsize为3*kWidth，contentOffSet为kWidth
- 接下来使用代理方法scrollViewDidScroll来监听scrollview的滚动，定义一个枚举变量来记录滚动的方向
- 使用KVO来监听direction属性值的改变-->[self addObserver:self forKeyPath:@"direction" options:NSKeyValueObservingOptionNew context:nil];
- 通过observeValueForKeyPath判断滚动的方向，当偏移量大于x，表示左移，则将otherImageView加在右边，偏移量小于x，表示右移，则将otherImageView加在左边。同时判断设置对应的索引，图片
- 通过代理方法scrollViewDidEndDecelerating来监听滚动结束，结束后，scrollview的偏移量为0或者2x，我们通过代码再次将scrollview的偏移量设置为x，并将currImageView的图片修改为otherImageView的图片，那么我们看到的还是currImageView，只不过展示的是下一张图片，如图，又变成了最初的效果
- ，然后设置自动轮播，添加计时器，利用setContentOffset方法里面setContentOffset:animated:方法执行完毕后不会调用scrollview的scrollViewDidEndDecelerating方法，但是会调用scrollViewDidEndScrollingAnimation方法，因此我们要在该方法中调用pauseScroll（即监听减速结束后由otherImageView切换到currImageView的方法）
- 添加计时器：self.timer = [NSTimer timerWithTimeInterval:self.time target:self selector:@selector(nextPage) userInfo:nil repeats:YES];
- 在scrollViewWillBeginDragging中停止计时器
- 在scrollViewDidEndDragging中开启计时器
- 判断外界传入的是图片还是路径，如果是图片，直接加入图片数组中，如果是路径，先添加一个占位图片，然后根据路径去下载图片
- 监听图片被点击
- 定义一个block属性暴露给外界void(^imageClickBlock)(NSInteger index)
（不会block的可以用代理，或者看这里）
- 设置currImageView的userInteractionEnabled为YES
- 给currImageView添加一个点击的手势
- 在手势方法里调用block，并传入图片索引
- NSTimer的两种形式
- scheduledTimerWithTimeInterval 是创建一个定时器，并加入到当前运行循环[NSRunLoop currentRunLoop]中
- 其他两个（[NSTimer timerWithTimeInterval:3 target:self selector:@selector(doSomeThing1) userInfo:nil repeats:YES]; [[NSTimer alloc] initWithFireDate:[NSDate dateWithTimeIntervalSinceNow:5] interval:3 target:self selector:@selector(doSomeThing2) userInfo:nil
 repeats:YES];）只是创建定时器，并未添加到当前运行循环中，所以如果是其他两种方式创建的定时器则需要手动添加到currentRunLoop中
- NSTimer是普通的定时器，如果系统繁忙，刷新可能会被延迟。但是CADisplaylink实时刷新，跟着屏幕的刷新频率实时刷新，60次/s，与屏幕刷新频率相同
#### 26. tableView的优化
> 
iOS平台因为UIKit本身的特性，需要将所有的UI操作都放在主线程执行，所以有时候就习惯将一些线程安全性不确定的逻辑，以及它线程结束后的汇总工作等等放到了主线程，所以主线程包含大量计算、IO、绘制都有可能造成卡顿。
- 可以通过监控runLoop监控监控卡顿，调用方法主要就是在kCFRunLoopBeforeSources和kCFRunLoopBeforeWaiting之间,还有kCFRunLoopAfterWaiting之后,也就是如果我们发现这两个时间内耗时太长,那么就可以判定出此时主线程卡顿.
- 使用到CFRunLoopObserverRef,通过它可以实时获得这些状态值的变化
- 监控后另外再开启一个线程,实时计算这两个状态区域之间的耗时是否到达某个阀值,便能揪出这些性能杀手.
- 监控到了卡顿现场,当然下一步便是记录此时的函数调用信息,此处可以使用一个第三方Crash收集组件PLCrashReporter,它不仅可以收集Crash信息也可用于实时获取各线程的调用堆栈
- 
当检测到卡顿时,抓取堆栈信息,然后在客户端做一些过滤处理,便可以上报到服务器,通过收集一定量的卡顿数据后经过分析便能准确定位需要优化的逻辑
- 
设置正确的 reuseidentifer 以重用 cell
- 
尽量将 View 设置为不透明,包括 cell 本身（backgroundcolor默认是透明的），图层混合靠GPU去渲染,如果透明度设置为100%，那么GPU就会忽略下面所有的layer，节约了很多不必要的运算。模拟器上点击“Debug”菜单，然后选择“color Blended Layers”，会把所有区域分成绿色和红色,绿色的好,红色的性能差（经过混合渲染的），当然也有一些图片虽然是不透明的，但是也会显示红色，如果检查代码没错的话，一般就是图片自身的性质问题了，直接联系美工或后台解决就好了。除非必须要用GPU加载的，其他最好要用CPU加载，因为CPU一般不会百分百加载，可以通过CoreGraphics画出圆角
- 
有时候美工失误，图片大小给错了，引起不必要的图片缩放（可以找美工去改，当然也可以异步去裁剪图片然后缓存下来），还是使用Instrument的Color Misaligned Images，黄色表示图片需要缩放，紫色表示没有像素对齐。当然一般情况下图片格式不会给错，有些图片格式是GPU不支持的，就还要劳烦CPU去进行格式转换。还有可以通过Color Offscreen-Rendered Yellow来检测离屏渲染（就是把渲染结果临时保存，等到用的时候再取出，这样相对于普通渲染更消耗内存，使用maskToBounds、设置shadow，重写drawRect方法都会导致离屏渲染）
避免渐变，cornerRadius在默认情况下，这个属性只会影响视图的背景颜色和 border，但是不会离屏绘制，不影响性能。不用clipsToBounds（过多调用GPU去离屏渲染），而是让后台加载图片并处理圆角，并将处理过的图片赋值给UIImageView。UIImageView 的圆角通过直接截取图片实现，圆角路径直接用贝塞尔曲线UIBezierPath绘制（人为指定路径之后就不会触发离屏渲染），UIGraphicsBeginImageContextWithOptions。UIView的圆角可以使用CoreGraphics画出圆角矩形，核心是CGContextAddArcToPoint
 函数。它中间的四个参数表示曲线的起点和终点坐标，最后一个参数表示半径。调用了四次函数后，就可以画出圆角矩形。最后再从当前的绘图上下文中获取图片并返回，最后把这个图片插入到视图层级的底部。
“Flash updated Regions”用于标记发生重绘的区域
- 如果 row 的高度不相同,那么将其缓存下来
- 如果 cell 显示的内容来自网络,那么确保这些内容是通过异步下载
- 使用 shadowPath 来设置阴影，图层最好不要使用阴影,阴影会导致离屏渲染(在进入屏幕渲染之前,还看不到的时候会再渲染一次,尽量不要产生离屏渲染)
- 减少 subview 的数量，不要去添加或移除view，要就显示，不要就隐藏
- 在 cellForRowAtIndexPath 中尽量做更少的操作,最好是在别的地方算好，这个方法里只做数据的显示，如果需要做一些处理,那么最好做一次之后将结果储存起来.
- 使用适当的数据结构来保存需要的信息,不同的结构会带来不同的操作代价
- 使用,rowHeight , sectionFooterHeight 和 sectionHeaderHeight 来设置一个恒定高度 , 而不是从代理(delegate)中获取
- cell做数据绑定的时候，最好在willDisPlayCell里面进行，其他操作在cellForRowAtIndexPath，因为前者是第一页有多少条就执行多少次，后者是第一次加载有多少个cell就执行多少次，而且调用后者的时候cell还没显示
- 读取文件,写入文件,最好是放到子线程,或先读取好,在让tableView去显示
- tableView滚动的时候,不要去做动画(微信的聊天界面做的就很好,在滚动的时候,动态图就不让他动,滚动停止的时候才动,不然可能会有点影响流畅度)。在滚动的时候加载图片，停止拖拽后在减速过程中不加载图片，减速停止后加载可见范围内图片
#### 27. 谈谈内存的优化和注意事项（使用Instrument工具的CoreAnimation、GPU Driver、I/O操作，检查fps数值）
- 
重用问题：比如UITableViewCell、UICollectionViewCell、UITableViewHeaderFooterViews等设置正确的reuseIdentifier，充分重用
- 
懒加载控件、页面：对于不是立刻使用的数据，都应该使用延迟加载的方式，比如网络连接失败的提示界面，可能一直都用不到
- 
使用Autorelease Pool：在某些循环创建临时变量处理数据时，自动释放池以保证能及时释放内存
- 
不要使用太多的xib/storyboard：载入时会将其内部的图片在内的所有资源载入内存，即使未来很久才会需要使用，相对于纯代码写的延迟加载，在性能和内存上就差了很多
- 
数据缓存：对于cell的行高要缓存起来，使用reloadData效率也极高，对于网络数据，不需要每次都请求的，应该缓存起来，可以写入数据库，也可以通过plist文件存储
- 
选择正确的数据结构：针对不同的业务场景选择最合适的数据结构是写出高效代码的基础
- 数组：有序的一组值，使用索引查询起来很快，使用值查询的很慢，插入/删除 很慢
- 字典：存储键值对对，用键查找比较快
- 集合：无序的一组值，用值来查找很快，插入/删除很快
- 
gzip/zip压缩：当从服务器下载相关附件时，可以通过 zip压缩后再下载，使得内存更小，下载速度也更快
- 
重大开销对象：一些objects的初始化很慢，比如NSDateFormatter和 NSCalendar，但是又无可避免的需要使用，通常作为属性存储起来，避免反复使用
- 
避免反复处理数据：需要应用需要从服务器加载数据，常为JSON或者XML格式的数据，在服务器端或者客户端使用相同的数据结构很重要
- 
选择图片时，要对图片进行压缩处理，根据不同的情况选择不同的图片加载方式，-imageNamed:读取到内存后会缓存下来，适合图片资源较小，使用很频繁的图片；-initWithContentsOfFiles:仅加载图片而不缓存，适合较大的图片。若是collectionView中使用大量图片的时候，可以用UIVIew.layer.contents=(__bridge id _Nullable)(model.clipedImage.CGImage)；这样就更轻量级一些
- 
当然有时候也会用到一些第三方，比如在使用UICollectionView和UITableView的时候，Facebook有一个框架叫AsyncDisplayKit，这个库就可以很好地提升滚动时流畅性以及图片异步下载功能（不支持sb和autoLayout，需要手动进行约束设置），AsyncDisplayKit用相关node类，替换了UIView和它的子类,而且是线程安全的。它可以异步解码图片，调整图片大小以及对图片和文本进行渲染，把这些操作都放到子线程，滑动的时候就流畅许多。我认为这个库最方便的就是实现图片异步解码。UIImage显示之前必须要先解码完成，而且解码还是同步的。尤其是在UICollectionView/UITableView
 中使用 prototype cell显示大图，UIImage的同步解码在滚动的时候会有明显的卡顿。另外一个很吸引人的点是AsyncDisplayKit可以把view层次结构转成layer。因为复杂的view层次结构开销很大，如果不需要view特有的功能（例如点击事件），就可以使用AsyncDisplayKit 的layer backing特性从而获得一些额外的提升。当然这个库还处于开发阶段，还有一些地方地方有待完善，比如不支持缓存，我要使用这个库的时候一般是结合Alamofire和AlamofireImage实现图片的缓存
> 
千万别打赏！俺不缺钱，缺赞！
文／si1ence（简书作者）
原文链接：http://www.jianshu.com/p/bc3f8424fad3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
