# Objective-C 2.0编程语言(二) 成员变量与分类 - weixin_33985507的博客 - CSDN博客
2018年09月14日 16:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 成员变量和属性
#### 1. 成员变量
包含实例变量和类变量。一般也称实例变量。上一篇中介绍了类的数据结构。现在看一下成员变量。
```
struct objc_ivar {
    char *ivar_name;
    char *ivar_type;
    int ivar_offset;    // 基地址偏移量
    int space;          // 消耗空间
};
struct objc_ivar_list {
    int ivar_count;
    int space;
    struct objc_ivar ivar_list[1];
}
```
成员变量是何时分配内存，又储存在何处呢？
> 
时间：
同类对象一样，ivar是在runtime时期被确定的。类型，空间，位置，三者齐全。这也就是为什么分类不能简单地用@property来添加成员变量的原因。
存储：
按照类对象结构的角度来看，类中的成员变量的地址都是基于类对象自身地址进行偏移的。 实际上， 在给类添加成员变量时，会调用这个函数：
```
BOOL 
class_addIvar(Class cls, const char *name, size_t size, 
              uint8_t alignment, const char *type)
// alignment参数代表 内存对齐方式
// ivar_offset 由此算出：
/*
uint32_t offset = cls->unalignedInstanceSize();
      uint32_t alignMask = (1<<alignment)-1;
      offset = (offset + alignMask) & ~alignMask;
*/
// 因此，OC中类结构地址的偏移计算与结构体还是有不同的，只要是小于8bit长度的地址，统一归为8bit偏移
```
#### 2. 属性@property
Objective-C的“声明的属性”特征为实现对象的属性提供一个简便的途径。
属性主要有两个方面，声明属性的语法和访问属性的语法。此部分对这两个方面作一个简要的概述，另外还会讨论这种特征背后目的。
- **属性的目的：**
**访问实例变量**。你通常都使用一对访问器，也就是getter和setter方法。你可以把这种通过访问器风格的API来控制类状态的方式当作类的一种属性。通过使用访问器方法访问实例变量，这样你坚守了数据封装的原则。你可以通过对getter/setter对的严格控制，隐藏对实例变量状态的管理，同时让这些API的用户不了解类实现的变化。如果没有属性声明，那么编写这些访问器确实是一件恶心的事，你必须编写代码来支持内存管理的。而Objective-C的“声明的属性”特征提供了如下的优势：
■ 属性的声明为访问器的行为提供一个清晰的、显式的说明
■ 编译器可以为你解析访问器，它根据你在声明中提供的说明，这意味着你只需要少量的代码来编写和维护
■ 属性会和标识符一样被清晰地说明，并定义作用域，因此编译其能够探测到那些对没有声明的属性的使用。
■ 运行时环境会对类中定义的属性进行自省。
#### 属性的声明和实现
属性包括两个部分，声明和实现。
##### 1. 属性的声明
属性的声明以关键字@property开始。@property可以出现在类的接口部分中方法声明列表的任何地方。它还可以出现在协议和类别声明中：
`@property(attributes) type name;`
@property声明一个属性，可选的括号括起来的标志（Attribute）集为属性提供额外的语义上的和其它方面的行为。和其他Objective-C类型一样，每个属性都有一个类型说明和一个名字。
- 属性实现指示符
你可以在@implementaiton块中使用@synthesize和@dynamic指示符为编译器发出特定的指令。但要注意，它们并不是必须的。
> 
**@synthesize:**
你可以使用@synthesize关键字告诉编译器，如果在你的@implementation块中没有提供setter和getter方法，那么编译器必须解析它们，自动生成读取器和设置器方法。（iOS6之后不需要写了，@property声明默认解析生成存取器方法。）
**@Dynamic：**
告诉编译器不要创建 property 对应的成员变量（就是一般的 _someProperty），也不要自动生成 get/set 方法，同时不要报错，到在运行时我自己会来添加 get/set 方法。或者你需要在运行时采用别的机制例如动态加载或者动态方法方案（dynamic method resolution，主要用于在CoreData的实现NSManagedObject子类时使用,由Core Data框架在程序运行时的动态生成子类属性)。
##### 属性声明的标记
像weak和strong是直接作用在了成员变量上，而atomic，nonatomic，copy等等这些关键字则是编译器在setter和getter中实现。**如果你需要自己来实现访问器，就应该确保他们和这些声明匹配。**
- 原子性标记
> - atomic(默认)
默认为atomic，提供多线程安全，在多线程环境下，解析的访问器提供一个对属性的安全访问，同一时间只有个线程可以访问。
访问器方法实现如下：
```
// setter MRC模式 方便理解
    @synchronized(self){
    if(_car=!car){         
       [_car release];      
       _car=[car retain];
    }
// getter
    @synchronized(self) {
        return _car;
    }
```
> - nonatomic 不提供线程安全，提高性能。
atomic是Objc使用的一种线程保护技术，基本上来讲，是防止在写未完成的时候被另外一个线程读取，造成数据错误。而这种机制是耗费系统资源的，所以在iPhone这种小型设备上，如果无需多线程间的通讯编程，那么nonatomic是一个非常好的选择。
这也就是说，在多线程环境下，解析的访问器默认提供一个对属性的安全访问，从获取器得到的返回值或者通过设置器设置的值可以一次完成，即便是别的线程也正在对其进行访问。如果你不指定 nonatomic ，在自己管理内存的环境中，解析的访问器保留并自动释放返回的值，如果指定了 nonatomic ，那么访问器只是简单地返回这个值。
不过即便是atomic，也依然不能保证线程安全。如果线程A调用了getter，同时线程B和线程C都调用了setter，虽然原子性保证了同一时间只有一个线程访问访问器,但A线程getter得到的值，既可能是BC在set之前的原始值，也可能是B set的值或者C set的值。同时这个属性的值，也可能是B set的值或者C set的值。
所以，保证数据完整性不能简单靠一把锁来完成，毕竟这个是多线程编程最大的难点。[参见附录1 -多线程加锁]()
- 内存管理标记
> - **assign(默认)**
用于基础数据类型 （NSInteger，CGFloat）和C数据类型（int, float, double, char）等等。此标记说明setter将直接进行赋值,不做其他处理。
> - **strong(ARC)/retain(MRC)**
用于NSObject和其子类的对象类型。setter中将对参数进行release旧值，再retain新值。
此属性只能用于Objective-C对象类型，而不能用于Core Foundation对象。(原因很明显，retain/strong会增加对象的引用计数，而基本数据类型或者Core Foundation对象都没有引用计数——译者注)。
ps.注意: 把对象添加到数组中时，引用计数将增加对象的引用次数+1。
> - **weak(ARC)**
和strong是对应关键字，适用范围同strong一样，作用特征和assign类似 。 而weak真正的作用，实际是__weak修饰符。当我们声明一个weak属性foo时，实际是声明了一个__weak id _foo;如：
`id __weak obj1 = obj;`
假设obj已经附加__strong修饰符并被赋值。编译器会把上面这段代码转换成运行时代码。
`id obj1;`
`objc_initWeak(&obj1, obj); // 编译器会此函数初始化__weak修饰的变量`
`objc_destoryWeak(&obj1); // 当变量的作用域结束后调用，释放该变量。`
objc_initWeak函数实际干的活是：
`objc1 = 0; // 先将指针objc1置成0`
`objc_storeWeak(&obj1, obj); // 使obj1指向obj对象`
objc_destoryWeak函数的实际操作如下：
`objc_storeWeak(&obj1, 0);`
实际上，objc_storeWeak函数会把第二个参数的对象的地址作为key，并将第一个参数（__weak关键字修饰的指针的地址）作为值，注册到weak表中。当使用weak指针时，获取的变量时是通过weak表进行键值取的，如果第二个参数为0（说明对应的对象被释放了），则将weak表中对应的整个key-value键值对删除，这就是__weak关键字的核心思想！
> - **copy**
在setter赋值时使用传入值的一份拷贝，常用于string类型，因为传入值也许是可变的（NSMutableString），而你想要让你自己的对象保留一份不变的拷贝。对于可变字符串进行copy是就深拷贝。
（关于深浅拷贝: 对于不可变对象（如：NSString，NSArray实例），copy方法为浅拷贝，mutableCopy为深拷贝；而对于可变对象（如：NSMutableString，NSMutableArray实例）, 无论是copy还是mutableCopy都会创建一个新对象，属于深拷贝。）
但是关于容器对象，其中的元素对象无论是copy还是mutableCopy都是指针复制。如果要实现完全深拷贝，需要换种方式，
- 系统API如：
`NSArray *marray2 = [[NSArray alloc] initWithArray:marry1 copyItems:YES];`
- 归档解档.
`NSData *data = [NSKeyedArchiver archivedDataWithRootObject:marry1];`
`NSArray *marray2 = [NSKeyedUnarchiver unarchiveTopLevelObjectWithData:data error:nil];`
- 自定义实现对象的自定义拷贝，必须实现NSCopying，NSMutableCopying协议，实现该协议的copyWithZone方法和mutableCopyWithZone方法
> - unsafe_unretained ：
__unsafe_unretained和__weak一样，表示的是对象的一种弱引用关系，唯一的区别是：__weak修饰的对象被释放后，指向对象的指针会置空，也就是指向nil,不会产生野指针；而__unsafe_unretained修饰的对象被释放后，指针不会置空，而是变成一个野指针，那么此时如果访问这个对象的话，程序就会Crash，抛出BAD_ACCESS的异常。
- 其他标记
> - getter=<name>或setter=<name>
指定方法名，用来更改默认的存取成员变量的方法名称，一般只是在BOOL类型属性时常用。
#### 其他
> - 属性的重声明
你可以在子类中重新声明属性， 但在子类中你必须将超类属性的标记作为整体来声明，在类别和协议中声明的属性都是如此。如果你为一个属性声明为readonly，你可以在扩展类中将它重新声明为readwrite。这种情况下，设置器就会被解析。这种可以把一个只读的属性重新声明为读写属性的能力提供两种常见的实现方式：超类是不可变的，而子类是可变的；另一种方式是属性具有公共的只读API，而内部有一个私有的读写的方法。
> - 属性和“键/值”编程(key-value coding)
在Cocoa中，有两种访问属性的基本方式：静态地直接使用方法的唤醒和动态地使用“键/值”编程方式(KVC)。KVC定义通用的属性访问方法，valueForKey:和setValue:forKey: ，它们将属性和基于字符串的键值联系起来。
> - 属性自省
编译器在遇到属性声明时，它会产生和属性所在类、类别或协议相关联的描述性元数据。这些元数据可以通过函数来访问，这样的函数能够支持对一个类或者协议查询它们的属性，从而获得一个@encode类型的字符串，还可以得到一个C风格的，存储有属性的标记的字符串。对每个类和协议，都可以获得一个属性列表。Property结构体定义了一个句柄用来访问属性的元数据。
```
typedef struct objc_property *Property;
// 函数class_getProperty和protocol_getProperty分别用来在类和协议中查找一个命名的属性：
Property class_getProperty(Class cls, const char *name);
Property protocol_getProperty(Protocol *proto, const char *name);
// 函数class_copyPropertyList和protocol_copyPropertyList返回一个动态分配的指针数组，数字的成员是指向类或协议中声明的所有属性的指针。
Property* class_copyPropertyList(Class cls, uint32_t *count);
Property* protocol_copyPropertyList(Protocol *protocol, uint32_t *count);
// 函数property_getInfo function返回属性的名字和@encode类型的字符串：
void property_getInfo(Property *property, const char **name, const char **type);
// 函数property_copyAttributeList 返回一个动态分配的C风格的指针数组，它代表属性的编译时的标记列表：
const char **property_copyAttributeList(Property *property, unint32_t *count);
```
### Categories和Extensions
### 1. Categories 分类/类别
分类能够做到的事情主要是：**即使在你不知道一个类的源码情况下，向这个类添加扩展的方法。**类别添加的新方法称为类的一部份。会被子类继承。
objc所有类和对象都是c结构体，category当然也一样
```
struct _category_t {
    const char *name; // 并不是category小括号里写的名字，而是类的名字
    struct _class_t *cls; // cls要扩展的类对象，编译期间这个值是不会有的，runtime加载时才会根据name对应到类对象
    const struct _method_list_t *instance_methods; // 这个category所有的 - 方法
    const struct _method_list_t *class_methods; // 这个category所有的+方法
    const struct _protocol_list_t *protocols; // 这个category实现的protocol，比较不常用在category里面实现协议，但是确实支持的
    const struct _prop_list_t *properties; // 这个category所有的property，这也是category里面可以定义属性的原因，不过这个property不会@synthesize实例变量，需要runtime绑定，下面会说。
};
```
#### 使用
```
// UIViewController.h
 #import “UIViewController.h”
 @interface UIViewController（CustomView）
// 方式1. 添加扩展方法
 -（void) cus_MethodName；   // pExt_ 是前缀,防止覆写原类的同名方法. 调用优先级分类 > 本类 > 父类.
// 方式2. 添加属性 .使用runtime.
 @property(nonatomic,copy) NSString *nameWithSetterGetter;   
 @end
```
```
// UIViewController.m
static NSString *nameWithSetterGetterKey = @"nameWithSetterGetterKey";   //定义一个key值
@ implementation UIViewController （CustomView）
 -（void) cus_MethodName {
 }
//运行时实现setter方法
- (void)setNameWithSetterGetter:(NSString *)nameWithSetterGetter {
        objc_setAssociatedObject(self, &nameWithSetterGetterKey, nameWithSetterGetter, OBJC_ASSOCIATION_COPY);
}
//运行时实现getter方法
- (NSString *)nameWithSetterGetter {
    return objc_getAssociatedObject(self, &nameWithSetterGetterKey);
}
```
> - 根类的类别
类别可以用来为任何类添加方法，包括根类。为NSObject添加的方法在所有继承了NSObject的类中都可用。而且这通常很有用，但这样做往往很冒险。
尽管类别对根类的改变似乎很易懂而且影响也不大，但继承会给它一个很广的作用域。你可能正在为不可见的类作一些无意的改变。你可能并不知道你这样做的所有后果，而且那些不知道你做这些修改的人当然也不知道他们正在做什么。另外，在根类的方法时，已经要记住下面两种结果：
■向super发送消息是非法的，因为它没有超类。
■类对象可以执行在根类中定义的方法。
类对象只能执行类方法，然而根类中定义的方法确是例外，它们为运行时环境定义了所有类都继承的一些接口。类对象是具备所有功能的对象，并需要共享相同的接口。这种特性意味着，你必须考虑一种情况，在NSObject中定义的方法可能不但会被实例变量所执行，还可能被类对象执行。
### 2.  Extensions 类扩展
Extension是Category的一个特例。类扩展与分类相比只少了分类的名称，所以称之为“匿名类别”。
#### 使用
- 为一个类添加额外的原来没有变量，方法和属性
- 一般的类扩展写到.m文件中
- 一般的私有属性写到.m文件中的类扩展中
```
// XXX .m
@interface XXX ()
//私有属性
//私有方法（如果不实现，编译时会报警,Method definition for 'XXX' not found）
@end
```
#### 区别
类别与类扩展的区别：
①类别中原则上只能增加方法（能添加属性的的原因只是通过runtime解决无setter/getter的问题而已）；
②类扩展不仅可以增加方法，还可以增加实例变量（或者属性），只是该实例变量默认是@private类型的（
用范围只能在自身类，而不是子类或其他地方）；
③类扩展中声明的方法没被实现，编译器会报警，但是类别中的方法没被实现编译器是不会有任何警告的。这是因为类扩展是在编译阶段被添加到类中，而类别是在运行时添加到类中。
④类扩展不能像类别那样拥有独立的实现部分（@implementation部分），也就是说，类扩展所声明的方法必须依托对应类的实现部分来实现。
⑤定义在 .m 文件中的类扩展方法为私有的，定义在 .h 文件（头文件）中的类扩展方法为公有的。类扩展是在 .m 文件中声明私有方法的非常好的方式。
### 附录 线程锁
> - 时间片轮转调度算法
了解多线程加锁必须知道时间片轮转调度算法，才能深切理解其原理、性能瓶颈。
现代操作系统在管理普通线程时，通常采用时间片轮转算法(Round Robin，简称 RR)。每个线程会被分配一段时间片(quantum)，通常在 10-100 毫秒左右。当线程用完属于自己的时间片以后，就会被操作系统挂起，放入等待队列中，直到下一次被分配时间片，如果线程在时间片结束前阻塞或结束，则CPU当即进行切换。由于线程切换需要时间，如果时间片太短，会导致大量CPU时间浪费在切换上；而如果这个时间片如果太长，会使得其它线程等待太久。
> - 原子操作
狭义上的原子操作表示一条不可打断的操作，也就是说线程在执行操作过程中，不会被操作系统挂起，而是一定会执行完（理论上拥有CPU时间片无限长）。在单处理器环境下，一条汇编指令显然是原子操作，因为中断也要通过指令来实现，但一句高级语言的代码却不是原子的，因为它最终是由多条汇编语言完成，CPU在进行时间片切换时，大多都会在某条代码的执行过程中。
但在多核处理器下，则需要硬件支持。
> - 自旋锁和互斥锁
都属于CPU时间分片算法下的实现保护共享资源的一种机制。都实现互斥操作，加锁后仅允许一个访问者。
却别在于自旋锁不会使线程进入wait状态，而通过轮训不停查看是否该自旋锁的持有者已经释放的锁；对应的，互斥锁在出现锁已经被占用的情况会进入wait状态，CPU会当即切换时间片。
- OC中的同步锁
- 自旋锁 OSSpinLock
```
__block OSSpinLock oslock = OS_SPINLOCK_INIT;
    
//线程2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
   NSLog(@"线程2 befor lock");
   OSSpinLockLock(&oslock);
   NSLog(@"线程2");
   OSSpinLockUnlock(&oslock);
   NSLog(@"线程2 unlock");
});
//线程1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
   NSLog(@"线程1 befor lock");
   OSSpinLockLock(&oslock);
   NSLog(@"线程1 sleep");
   sleep(4);
   NSLog(@"线程1");
   OSSpinLockUnlock(&oslock);
   NSLog(@"线程1 unlock");
});
```
> 
OSSpinLock效率奇高，主要原因是：并没有进入系统kernel，使用它可以节省系统调用和上下文切换。
[不再安全的 OSSpinLock](http://blog.ibireme.com/2016/01/16/spinlock_is_unsafe_in_ios/) 低优先级线程拿到锁时，高优先级线程进入忙等(busy-wait)状态，消耗大量 CPU 时间，从而导致低优先级线程拿不到 CPU 时间，也就无法完成任务并释放锁。这种问题被称为优先级反转。
- 信号量 dispatch_semaphore
YY大神推荐使用信号量dispatch_semaphore作为自旋锁的替代方案。
```
dispatch_semaphore_t signal = dispatch_semaphore_create(1);
dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, 5.0f * NSEC_PER_SEC);
    
//线程1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
   NSLog(@"线程1 holding");
   dispatch_semaphore_wait(signal, timeout); //signal 值 -1
   NSLog(@"线程1 sleep");
   sleep(4);
   NSLog(@"线程1");
   dispatch_semaphore_signal(signal); //signal 值 +1
   NSLog(@"线程1 post singal");
});
    
//线程2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
   NSLog(@"线程2 holding");
   dispatch_semaphore_wait(signal, timeout);
   NSLog(@"线程2 sleep");
   sleep(4);
   NSLog(@"线程2");
   dispatch_semaphore_signal(signal);
   NSLog(@"线程2 post signal");
});
```
> 
dispatch_semaphore_create(1)为创建信号，（）中数字表示可以同时几个线程使用信号。为1表示同步使用。上述代码如果此处标2就和没设置信号量一样，并发自行运行。如果设置为0，则一律等待overTime时自动释放，所有代码都不执行，理论上也具有同步作用。
dispatch_semaphore_wait中传入的timeout表示最长加锁时间，自动释放锁后，其它线程可以获取信号并继续运行。
- pthread_mutex锁
pthread表示的是POSIX thread，定义的是一组跨平台线程相关的API。
pthread_mutex互斥锁是一个非递归锁，如果同一线程重复调用加锁会造成死锁。用法比较简单
```
static pthread_mutex_t pmutexLock;
pthread_mutex_init(&pLock, NULL);
    
//1.线程2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
   NSLog(@"线程2 befor lock");
   pthread_mutex_lock(&pLock);
   NSLog(@"线程2");
   pthread_mutex_unlock(&pLock);
});
    
//2.线程1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
   NSLog(@"线程1 before lock");
   pthread_mutex_lock(&pLock);
   sleep(3);
   NSLog(@"线程1");
   pthread_mutex_unlock(&pLock);
});
```
pthread_mutex(recursive) 递归锁，比较安全，同一线程有且仅有一次加锁，重复加锁不会死锁。无论加锁几次，只需解锁一次。
```
static pthread_mutex_t pLock;
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr); //初始化attr赋初值
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); //设置锁类型为递归锁
pthread_mutex_init(&pLock, &attr);
pthread_mutexattr_destroy(&attr);
    
//1.线程1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
   static void (^RecursiveBlock)(int);
   RecursiveBlock = ^(int value) {
       pthread_mutex_lock(&pLock);
       if (value > 0) {
           NSLog(@"value: %d", value);
           RecursiveBlock(value - 1);
       }
       
   };
   NSLog(@"线程1 before lock");
   RecursiveBlock(5);
   NSLog(@"线程1");
   pthread_mutex_unlock(&pLock);
   NSLog(@"线程1 unlock");
});
    
//2.线程2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
   NSLog(@"线程2 before lock");
   pthread_mutex_lock(&pLock);
   NSLog(@"线程2");
   pthread_mutex_unlock(&pLock);
   NSLog(@"线程2 unlock");
});
```
- Foundation框架NSLock NSRecursiveLock
> 
NSLock 只是在内部封装了一个 pthread_mutex，属性为PTHREAD_MUTEX_ERRORCHECK
NSRecursiveLock 与 NSLock 的区别在于内部封装的 pthread_mutex_t
对象的类型不同，NSRecursiveLock 的类型为 PTHREAD_MUTEX_RECURSIVE。
- 条件锁 NSConditionLock
和NSLock主要区别是增加了一个NSInteger类型的condition参数，api很简单，也很少。condition就是一个条件标识。在加锁和解锁时对NSConditionLock做条件判断和修改，相当于if语句。
实际的实现原理就是里面封装了一个NSCondition对象。
> 
NSCondition它通常用于标明共享资源是否可被访问或者确保一系列任务能按照指定的执行顺序执行。如果一个线程试图访问一个共享资源，而正在访问该资源的线程将其条件设置为不可访问，那么该线程会被阻塞，直到正在访问该资源的线程将访问条件更改为可访问状态或者说给被阻塞的线程发送信号后，被阻塞的线程才能正常访问这个资源。
NSConditionLock在lock时判断NSCondition对象的条件是否满足，不满足则wait，unlock时对发送NSCondition的broadcast，属于一个常见的生产者–消费者模型。
- 简单易用的@synchronized
@synchronized 实际上是把修饰对象当做锁来使用。这是通过一个哈希表来实现的，OC 在底层使用了一个互斥锁的数组(你可以理解为锁池)，通过对对象去哈希值来得到对应的互斥锁。
以上各种锁的实现原理可以参考[深入理解 iOS 开发中的锁](https://bestswifter.com/ios-lock/)
建议用互斥锁pthread_mutex（YYKit方案）或者信号量dispatch_semaphore（CoreFoundation和protobuf方案）作为替代。
