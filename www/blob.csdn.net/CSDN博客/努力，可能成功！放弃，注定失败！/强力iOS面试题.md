# 强力iOS面试题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年07月30日 16:27:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：131标签：[iOS面试题](https://so.csdn.net/so/search/s.do?q=iOS面试题&t=blog)
个人分类：[关于iOS面试](https://blog.csdn.net/qq_30513483/article/category/6180887)
网上乱逛看到的强力面试题list，问别人问自己都是好干货，存了。
## 以下是题目
[※]@property中有哪些属性关键字？
> 
属性相关的关键字，可以按照一些功能来分类
线程相关
atomic：默认为该属性，功能为在多线程的情况下，编译器自动生成相应的线程安全代码，达到互斥加锁的目的，避免所对应的变量出现读写不同步的问题。
nonatomic：当该变量不需要考虑多线程的情况时使用该关键字，能够提高性能和效率。
> 
> 
atomic主要用于线程保护的，可以避免当一个线程在对一个地址进行访问时防止另一个线程对此地址同时进行访问，防止了数据错误。众所周知，线程锁会耗费系统资源，所以在实际使用时，如果没有线程相关的内容，建议使用nonatomic。
目前大部分的iOS app中，nonatomic的普遍使用也是基于性能考虑的。
> 
读写相关
readwrite：默认类型，会自动生成相应的读写方法。
readonly：只生成相应的get方法，不生成set方法。
> 
> 
主要使用目的：控制外部类对该属性的访问权限。
> 
内存引用相关
这里又根据系统版本分为两类，一类是iOS 5.0之前，一类是iOS 5.0加入了ARC机制之后。
ARC之前
assgin：默认类型，set方法中为直接赋值，即直接指向了一个地址，没有任何引用计数的更改。
retain：set方法中，首先释放（release）之前的对象，即去除对于旧对象的引用计数，并直接指向新的对象的地址，并使新对象的引用计数加一。
copy：set方法中，与retain类似，先release旧对象，然后对于新的对象进行copy，实质为使用新的内存空间和地址，引用计数唯一，并将目标对象的内容拷贝过来。可以认为是内容拷贝。
ARC之后
weak：弱引用，与assgin类似，只是当对象释放后，weak类型会自动置空，即将指针设为nil，避免野指针。
strong：强引用，默认类型，与retain类似。
> 
> 
内存计数方面的关键字是objc主要内容。
[※]weak属性需要在dealloc中置nil么？
> 
不需要，可以参考上面的答案，weak类型的属性会在对象释放后自动将指针设为nil，防止野指针。
[※※]@synthesize和@dynamic分别有什么作用？
> 
@synthesize
该关键字会在编译期间自动生成set和get方法。当自定义了set或get方法时，会自动覆盖编译器生产的方法。
@dynamic
该关键字不会自动生成set和get方法，同时避免了编译警告，但是需要我们自己去实现set和get方法，一般在动态绑定或某些需要特别定义读写方法时使用该关键字。
[※※※]ARC下，不显式指定任何属性关键字时，默认的关键字都有哪些？
> 
ARC下默认的关键字为：atomic，weak，strong（经[@Chrisss](https://www.jianshu.com/users/3b06e48136fa)同学指正，谢谢），readwrite。
非ARC下默认的关键字为：atomic，retain，readwrite。
> 
其实说起来就是线程相关，内存相关，读写相关。
[※※※]用@property声明的NSString（或NSArray，NSDictionary）经常使用copy关键字，为什么？如果改用strong关键字，可能造成什么问题？
> 
这里主要思考的是copy关键字使用场景，首先要理解copy关键字的实质，copy相当于是进行了一次内容拷贝，其指针地址和内存空间都与被拷贝的对象不同，但是内容是相同的。那么，copy关键字的使用场景就得到了：
> 
当需要对于一个被赋值的对象内容进行修改，并且不能修改原对象内容时，就使用copy关键字。
> 
回到原题，NSString、NSArray等类都是用于封装数据的结构类，这些类的对象经常是需要进行内容修改的。所以，为了避免对于原本赋值对象内容的修改，而只是针对当前类中的对象内容进行修改，就需要使用copy关键字了。当然，如果确定使用过程中都不需要对对象内容进行修改，那么根据ARC环境使用retain或者strong都是可以的，在一定程度上还能减少对于内存的使用。
[※※※]@synthesize合成实例变量的规则是什么？假如property名为foo，存在一个名为_foo的实例变量，那么还会自动合成新变量么？
> 
以下内容是根据个人的理解和经验。
@synthesize合成实例变量的规则是：
> 
> 
在代码的实现文件（.m文件）里面自动生成指定实例的get和set方法。
> 
如果在代码的声明文件（.h文件）中未定义ivar申明，在当前Xcode 6.0版本中，只要定义了property类型，不需要写@synthesize关键字，编译器会动态创建相对应的ivar变量，如：`@synthesize foo = _foo;`。
在之前的Xcode版本不会动态生成相应的合成代码，需要手写，完整的写法如下：
.h文件
```
#import <Foundation/Foundation.h>
@interface TestClass : NSObject
{
    NSObject *_object;
}
@property (nonatomic, retain) NSObject *object;
@end
```
.m文件
```
#import "TestClass.h"
@implementation TestClass
@synthesize object = _object; //这里写成@synthesize object;也是可以的，但是相应的属性名称和ivar名称就是相同的了。在调用时，如果直接写object，就是ivar调用；如果是self.object则是调用get方法。
@end
```
> 
当存在一个名为_foo的实例变量，那么还会自动合成新变量么？
不会。
![](https://upload-images.jianshu.io/upload_images/670128-3634d6175691e60d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
程序员都无所谓的warning众
[※※※※※]在有了自动合成属性实例变量之后，@synthesize还有哪些使用场景？
> 
这里有一个答案，看起来挺合适的。
> 
[http://stackoverflow.com/questions/19784454/when-should-i-use-synthesize-explicitly](https://link.jianshu.com?t=http://stackoverflow.com/questions/19784454/when-should-i-use-synthesize-explicitly)
> 
具体说起来就是：
> 
不会自动生成的地方，又不想自己写get和set方法，就用这个。
[※※]objc中向一个nil对象发送消息将会发生什么？
> 
不会发生什么。这个答案讲的很详细：
> 
[http://blog.csdn.net/hanangellove/article/details/45013581](https://link.jianshu.com?t=http://blog.csdn.net/hanangellove/article/details/45013581)
> 
具体原因如下：
> 
objc是动态语言，每个方法在运行时会被动态转为消息发送，即：`objc_msgSend(receiver, selector)`。
> 
那么，为了方便理解这个内容，还是贴一个objc的源代码：
runtime.h（类在runtime中的定义）
```
struct objc_class {
  Class isa OBJC_ISA_AVAILABILITY; //isa指针指向Meta Class，因为Objc的类的本身也是一个Object，为了处理这个关系，runtime就创造了Meta Class，当给类发送[NSObject alloc]这样消息时，实际上是把这个消息发给了Class Object
  #if !__OBJC2__
  Class super_class OBJC2_UNAVAILABLE; // 父类
  const char *name OBJC2_UNAVAILABLE; // 类名
  long version OBJC2_UNAVAILABLE; // 类的版本信息，默认为0
  long info OBJC2_UNAVAILABLE; // 类信息，供运行期使用的一些位标识
  long instance_size OBJC2_UNAVAILABLE; // 该类的实例变量大小
  struct objc_ivar_list *ivars OBJC2_UNAVAILABLE; // 该类的成员变量链表
  struct objc_method_list **methodLists OBJC2_UNAVAILABLE; // 方法定义的链表
  struct objc_cache *cache OBJC2_UNAVAILABLE; // 方法缓存，对象接到一个消息会根据isa指针查找消息对象，这时会在method Lists中遍历，如果cache了，常用的方法调用时就能够提高调用的效率。
  struct objc_protocol_list *protocols OBJC2_UNAVAILABLE; // 协议链表
  #endif
  } OBJC2_UNAVAILABLE;
```
objc在向一个对象发送消息时，runtime库会根据对象的isa指针找到该对象实际所属的类，然后在该类中的方法列表以及其父类方法列表中寻找方法运行，然后在发送消息的时候，objc_msgSend方法不会返回值，所谓的返回内容都是具体调用时执行的。
那么，回到本题，如果向一个nil对象发送消息，首先在寻找对象的isa指针时就是0地址返回了，所以不会出现任何错误。
[※※※]objc中向一个对象发送消息[obj foo]和objc_msgSend()函数之间有什么关系？
> 
同上题：`[obj foo];`在objc动态编译时，会被转意为：`objc_msgSend(obj, @selector(foo));`。
[※※※]什么时候会报unrecognized selector的异常？
> 
这里也有一篇文章讲的很清楚。
> 
[http://tech.glowing.com/cn/objective-c-runtime/](https://link.jianshu.com?t=http://tech.glowing.com/cn/objective-c-runtime/)
> 
简单的流程如下，在上一题中也提到过：
> 
objc是动态语言，每个方法在运行时会被动态转为消息发送，即：`objc_msgSend(receiver, selector)`。
> 
objc在向一个对象发送消息时，runtime库会根据对象的isa指针找到该对象实际所属的类，然后在该类中的方法列表以及其父类方法列表中寻找方法运行，如果，在最顶层的父类中依然找不到相应的方法时，程序在运行时会挂掉并抛出异常***unrecognized selector sent to XXX ***。但是在这之前，objc的运行时会给出三次拯救程序崩溃的机会：
- Method resolution
	objc运行时会调用`+resolveInstanceMethod:`或者 `+resolveClassMethod:`，让你有机会提供一个函数实现。如果你添加了函数并返回 YES，那运行时系统就会重新启动一次消息发送的过程，如果 resolve 方法返回 NO ，运行时就会移到下一步，**消息转发（Message Forwarding）**。
- Fast forwarding
	如果目标对象实现了`-forwardingTargetForSelector:`，Runtime 这时就会调用这个方法，给你把这个消息转发给其他对象的机会。
	只要这个方法返回的不是nil和self，整个消息发送的过程就会被重启，当然发送的对象会变成你返回的那个对象。否则，就会继续**Normal Fowarding**。
	这里叫Fast，只是为了区别下一步的转发机制。因为这一步不会创建任何新的对象，但下一步转发会创建一个NSInvocation对象，所以相对更快点。
- Normal forwarding
	这一步是Runtime最后一次给你挽救的机会。首先它会发送`-methodSignatureForSelector:`消息获得函数的参数和返回值类型。如果`-methodSignatureForSelector:`返回nil，Runtime则会发出`-doesNotRecognizeSelector:`消息，程序这时也就挂掉了。如果返回了一个函数签名，Runtime就会创建一个NSInvocation对象并发送`-forwardInvocation:`消息给目标对象。
[※※※※]一个objc对象如何进行内存布局？（考虑有父类的情况）
> 
首先发一篇网上最多的答案：
> 
> 
[https://github.com/ChenYilong/iOSInterviewQuestions/commit/0e7b5e2b13a5aec84c2d976bfe4f35b4d292383b](https://link.jianshu.com?t=https://github.com/ChenYilong/iOSInterviewQuestions/commit/0e7b5e2b13a5aec84c2d976bfe4f35b4d292383b)
> 
这个答案这几天思考了很多，也参考了不少别人的答案，这里写一下自己的理解。
还是要把objc中class定义的源代码贴上来：
runtime.h（类在runtime中的定义）
```
struct objc_class {
  Class isa OBJC_ISA_AVAILABILITY; //isa指针指向Meta Class，因为Objc的类的本身也是一个Object，为了处理这个关系，runtime就创造了Meta Class，当给类发送[NSObject alloc]这样消息时，实际上是把这个消息发给了Class Object
  #if !__OBJC2__
  Class super_class OBJC2_UNAVAILABLE; // 父类
  const char *name OBJC2_UNAVAILABLE; // 类名
  long version OBJC2_UNAVAILABLE; // 类的版本信息，默认为0
  long info OBJC2_UNAVAILABLE; // 类信息，供运行期使用的一些位标识
  long instance_size OBJC2_UNAVAILABLE; // 该类的实例变量大小
  struct objc_ivar_list *ivars OBJC2_UNAVAILABLE; // 该类的成员变量链表
  struct objc_method_list **methodLists OBJC2_UNAVAILABLE; // 方法定义的链表
  struct objc_cache *cache OBJC2_UNAVAILABLE; // 方法缓存，对象接到一个消息会根据isa指针查找消息对象，这时会在method Lists中遍历，如果cache了，常用的方法调用时就能够提高调用的效率。
  struct objc_protocol_list *protocols OBJC2_UNAVAILABLE; // 协议链表
  #endif
  } OBJC2_UNAVAILABLE;
```
> 
首先，其实我也不能说清楚内存布局到底是个什么意思，这个希望有朋友可以帮我解答一下
[※※※※]一个objc对象的isa的指针指向什么？有什么作用？
[※※※※]下面的代码输出什么？
```
@implementation Son : Father
- (id)init
{
    self = [super init];
    if (self) {
        NSLog(@"%@", NSStringFromClass([self class]));
        NSLog(@"%@", NSStringFromClass([super class]));
    }
    return self;
}
@end
```
[※※※※]runtime如何通过selector找到对应的IMP地址？（分别考虑类方法和实例方法）
[※※※※]使用runtime Associate方法关联的对象，需要在主对象dealloc的时候释放么？
[※※※※※]objc中的类方法和实例方法有什么本质区别和联系？
[※※※※※]_objc_msgForward函数是做什么的，直接调用它将会发生什么？
[※※※※※]runtime如何实现weak变量的自动置nil？
[※※※※※]能否向编译后得到的类中增加实例变量？能否向运行时创建的类中添加实例变量？为什么？
[※※※]runloop和线程有什么关系？
[※※※]runloop的mode作用是什么？
[※※※※]以+scheduledTimerWithTimeInterval...的方式触发的timer，在滑动页面上的列表时，timer会暂定回调，为什么？如何解决？
[※※※※※]猜想runloop内部是如何实现的？
[※]objc使用什么机制管理对象内存？
[※※※※]ARC通过什么方式帮助开发者管理内存？
[※※※※]不手动指定autoreleasepool的前提下，一个autorealese对象在什么时刻释放？（比如在一个vc的viewDidLoad中创建）
[※※※※]BAD_ACCESS在什么情况下出现？
[※※※※※]苹果是如何实现autoreleasepool的？
[※※]使用block时什么情况会发生引用循环，如何解决？
[※※]在block内如何修改block外部变量？
[※※※]使用系统的某些block api（如UIView的block版本写动画时），是否也考虑引用循环问题？
[※※]GCD的队列（dispatch_queue_t）分哪两种类型？
[※※※※]如何用GCD同步若干个异步调用？（如根据若干个url异步加载多张图片，然后在都下载完成后合成一张整图）
[※※※※]dispatch_barrier_async的作用是什么？
[※※※※※]苹果为什么要废弃dispatch_get_current_queue？
[※※※※※]以下代码运行结果如何？
```
- (void)viewDidLoad
{
    [super viewDidLoad];
    NSLog(@"1");
    dispatch_sync(dispatch_get_main_queue(), ^{
        NSLog(@"2");
    });
    NSLog(@"3");
}
```
[※※]addObserver:forKeyPath:options:context:各个参数的作用分别是什么，observer中需要实现哪个方法才能获得KVO回调？
[※※※]如何手动触发一个value的KVO
[※※※]若一个类有实例变量NSString *_foo，调用setValue:forKey:时，可以以foo还是_foo作为key？
[※※※※]KVC的keyPath中的集合运算符如何使用？
[※※※※]KVC和KVO的keyPath一定是属性么？
[※※※※※]如何关闭默认的KVO的默认实现，并进入自定义的KVO实现？
[※※※※※]apple用什么方式实现对一个对象的KVO？
[※※]IBOutlet连出来的视图属性为什么可以被设置成weak?
[※※※※※]IB中User Defined Runtime Attributes如何使用？
[※※※]如何调试BAD_ACCESS错误
[※※※]lldb（gdb）常用的调试命令？
来源：[http://blog.sunnyxx.com/2015/07/04/ios-interview/](https://link.jianshu.com?t=http://blog.sunnyxx.com/2015/07/04/ios-interview/)
***后续我会把每个相应的答案填上来。***
***PS：所有答案都是个人见解，有问题请留言大家一起探讨～***
From:[强力iOS面试题](https://link.jianshu.com?t=http://www.wangxinn.com/2015/07/07/qiang-li-iosmian-shi-ti-list/)
作者：wxin9
链接：https://www.jianshu.com/p/2e7ae4457083
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
