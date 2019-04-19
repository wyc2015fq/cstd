# iOS - Runtime 细致分析及具体运用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月02日 15:52:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：363
# [本文来自简书，原文地址:http://www.jianshu.com/p/c6cfe9f9972c](http://www.jianshu.com/p/c6cfe9f9972c)
# 前言
  运行时可以说是 Objective-C 这门语言的一个核心部分，看了许多博客，也反反复复看了多次源码([苹果开源的runtime源码](https://opensource.apple.com/tarballs/objc4/))，对于这方面研究的文章博客也比较多，有必要写这么一篇文章吗？质疑过！因为技术性文章雷同性无法避免的。同事的一句话让我觉得有必要，对技术怀有敬畏之心，工作一年跟三年的对字符串的理解都是不一样的，所有才决定写下这篇文章，因为也确实是每次来看这块理解都有所不同，便以此文章记录下此刻的认识与看法，如有误欢迎指正，如果看完觉得对你学习有帮助，关注一波帮忙点个喜欢❤️呗。
# 初识 Runtime - 运行时
runtime 简称运行时。其是系统运行的时候的一些机制，主要体现的是对象的消息机制。~~(读文章思考方式：看完上面一句话你脑海中应该有这个三个关键词运行时、OC中对象、消息发送 。本文也就是围绕着三个关键词进行的思考与阐述的)~~。
- Objective-C是基于C语言加入了面向对象特性 (面向对象贯穿整个操作系统，OC是重度对象语言) 和消息转发机制 (消息分发) 的动态语言
- Objective-C 是如何具有面向对象的能力的呢？OC中对象通过 C 语言的结构体来表示，对象的方法实现通过 C 函数来实现。
- Objective-C 的面向对象特性和消息转发机制动态性决定了其不仅需要一个编译器(疑问思考其编译的过程)，而且还需要运行时系统来动态创建类和对象，从而进行消息发送和转发。
- 下面上具体代码来感受下上面的几句话的含义，在一个控制器VC中对自定义继承自`NSObject`的`CCSomeClass`类进行实例化，然后调用其
```
-
 (void)sendMessage:(NSString *)str;
```
方法：
```
- (void)viewDidLoad {
    [super viewDidLoad];
    CCSomeClass *someClass = [[CCSomeClass alloc] init];
    [someClass sendMessage:@"zerocc"];
 }
```
下面的代码实现等同于上面的实现效果
```
- (void)viewDidLoad {
    [super viewDidLoad];
    CCSomeClass *someClass = objc_msgSend([CCSomeClass class], sel_registerName("alloc"));
    someClass = objc_msgSend(someClass, sel_registerName("init"));
    SEL sel = sel_registerName("sendMessage:");
    objc_msgSend(someClass,sel,@"zerocc");
}
```
# Objective-C 中类和对象
#### 类与对象的概念
- 类是对同一类事物高度的抽象，类中定义了这一类对象所应具有的静态属性（属性）和动态属性（方法）。
- 对象就是该类的一个实例，对象的创建就是类的实例化过程。
- 可以理解为类就是一种数据类型，它的变量就是对象。
#### Objective-C 中的类会是什么？
- 在Objective-C 类继承中，所有的类都是 `NSObject` (`NSProxy` 类抛开不考虑)
 的子类；也就是说 `NSObject` 是 Objective-C 继承中的根类,其他类都从 NSObject
 继承一套基本的接口到 Objective-C 运行时体系中。这些类的实例又都是从 NSObject 继承而获得 Objective-C 最根本的特性 - [NSObject特性详解](http://www.jianshu.com/p/e84cfd1e2e5e)。
- `NSObject` .h
 中类的定义(interface)文件，我们可以看到如下代码:
```
@interface NSObject <NSObject> {
    Class isa  OBJC_ISA_AVAILABILITY;
}
```
  每个`NSObject`类都拥有一个`Class`类作为成员变量,
 并且这个根类只有这么一个 `isa`属性；
- `Class isa`是什么呢？进入到 `objc.h`文件中，看到如下：
```cpp
```cpp
/// An opaque type that represents an Objective-C class.
typedef struct objc_class *Class;
```
```
  1. `Class` 是`objc_class`类型的结构体的指针变量
  2. 关键字`typedef`作用是给指向结构体的`objc_class`类型的指针起别名
  3. `Class isa` 等价于 
```
struct
 objc_class *isa
```
,所以从这里得出 `isa`是一个指针，是一个指向`objc_class`结构体的指针变量。
- `objc_class`类型结构体具体是?
 进入到`runtime.h`文件中，看其定义：
```cpp
```cpp
struct objc_class {
    Class isa  OBJC_ISA_AVAILABILITY;
#if !__OBJC2__
    Class super_class                                        OBJC2_UNAVAILABLE;  // 父类 是指向父类的
    const char *name                                         OBJC2_UNAVAILABLE;  // 类名
    long version                                             OBJC2_UNAVAILABLE;  // 类的版本信息，默认为0
    long info                                                OBJC2_UNAVAILABLE;  // 类信息，供运行期使用的一些位标识
    long instance_size                                       OBJC2_UNAVAILABLE;  // 该类的实例变量大小
    struct objc_ivar_list *ivars                             OBJC2_UNAVAILABLE;  // 该类的成员变量链表
    struct objc_method_list **methodLists                    OBJC2_UNAVAILABLE;  // 方法定义的链表
    struct objc_cache *cache                                 OBJC2_UNAVAILABLE;  // 方法缓存
    struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE;  // 协议链表
#endif
} OBJC2_UNAVAILABLE;
/* Use `Class` instead of `struct objc_class *` */
```
```
`objc_class`结构体描述了一个类的所有信息：父类、名称、信息、变量大小、变量列表、方法列表、协议列表等等；特别注意`objc_class`结构体中第一个数据又是指向另一个`Class`的`isa`指针（metaclass后面分析），所有的对象在内存里面都有一个isa。概况的来说这个`isa`指针对应的内存地址中存储了这些信息(类的信息)。
###### `objc_ivar_list`成员变量列表结构：
```cpp
```cpp
struct objc_ivar_list {
    int ivar_count                                           OBJC2_UNAVAILABLE;
#ifdef __LP64__
    int space                                                OBJC2_UNAVAILABLE;
#endif
    /* variable length structure */
    struct objc_ivar ivar_list[1]                            OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;
```
```
###### `objc_method_list`方法列表结构：
```cpp
```cpp
struct objc_method_list {
    struct objc_method_list *obsolete                        OBJC2_UNAVAILABLE;
    int method_count                                         OBJC2_UNAVAILABLE;
#ifdef __LP64__
    int space                                                OBJC2_UNAVAILABLE;
#endif
    /* variable length structure */
    struct objc_method method_list[1]                        OBJC2_UNAVAILABLE;
}
```
```
`**methodLists`指针的指针，可以动态修改`*methodLists`的值来添加成员方法,同样解释了Category不能添加属性的原因,二级指针
###### `objc_method`方法列表中的链表，它存储了方法名，方法类型和方法实现
```
struct objc_method {
    SEL method_name                                          OBJC2_UNAVAILABLE;
    char *method_types                                       OBJC2_UNAVAILABLE;
    IMP method_imp                                           OBJC2_UNAVAILABLE;
}
```
  1. 方法名类型为SEL，前面提到过相同名字的方法即使在不同类中定义，它们的方法选择器也相同。
  2. 方法类型method_types是个char指针，其实存储着方法的参数类型和返回值类型。
  3. method_imp指向了方法的实现，本质上是一个函数指针，后面会详细讲到
```cpp
```cpp
/// A pointer to the function of a method implementation. 
#if !OBJC_OLD_DISPATCH_PROTOTYPES
typedef void (*IMP)(void /* id, SEL, ... */ ); 
#else
typedef id (*IMP)(id, SEL, ...); 
#endif
```
```
  它就是一个函数指针，这是由编译器生成的。当你发起一个 ObjC 消息之后，最终它会执行的那段代码，就是由这个函数指针指定的。而IMP这个函数指针就指向了这个方法的实现。既然得到了执行某个实例某个方法的入口，我们就可以绕开消息传递阶段，直接执行方法，这在后面会提到。
  你会发现IMP指向的方法与objc_msgSend函数类型相同，参数都包含id和SEL类型。每个方法名都对应一个SEL类型的方法选择器，而每个实例对象中的SEL对应的方法实现肯定是唯一的，通过一组id和SEL参数就能确定唯一的方法实现地址；反之亦然。
  每个类都有一个方法列表，存放着selector的名字和方法实现的映射关系。IMP有点类似函数指针，指向具体的Method实现，SEL与IMP之间的关系图：
  获取方法地址IMP避开消息绑定而直接获取方法的地址并调用方法。这种做法很少用，除非是需要持续大量重复调用某方法的极端情况，避开消息发送泛滥而直接调用该方法会更高效。NSObject类中有个methodForSelector:实例方法，你可以用它来获取某个方法选择器对应的IMP，举个例子：
```
void (*setter)(id, SEL, BOOL);
        int i;
        setter = (void (*)(id, SEL, BOOL))[target
                                           methodForSelector:@selector(setFilled:)];
        for ( i = 0 ; i < 1000 ; i++ )
        setter(targetList[i], @selector(setFilled:), YES);
```
###### `objc_cache`方法缓存
```cpp
```cpp
struct objc_cache {
    unsigned int mask /* total = mask + 1 */                 OBJC2_UNAVAILABLE;
    unsigned int occupied                                    OBJC2_UNAVAILABLE;
    Method buckets[1]                                        OBJC2_UNAVAILABLE;
};
```
```
  Cache为方法调用的性能进行优化,通俗地讲,每当实例对象接收到一个消息时,它不会直接在isa指向的类的方法列表中遍历查找能够响应消息的方法，因为这样效率太低了，而是优先在Cache中查找。Runtime系统会把被调用的方法存到Cache中（理论上讲一个方法如果被调用，那么它有可能今后还会被调用）method_name作为key，method_imp作为value给存起来，下次查找的时候效率更高。这根计算机组成原理中学过的CPU绕过主存先访问Cache的道理挺像，高速缓存(cache) ->内存->虚拟内存->磁盘。
# Objective-C方法动态调用中涉及的术语介绍
 OC只是在编译阶段确定了要向接收者发送message这条消息，而receive将要如何响应这条消息，那就要看运行时发生的情况来决定了。通过发送消息来达到动态调用：
`[obj doSomething]    // 编译时runtime会将其转化为 objc_msgSend(obj,@selector(doSomething));`
也就是说 `objc_msgSend` 函数相当于入口；对象调用某个方法都将被编译器转化为：
`id objc_msgSend(id self, SEL op, ... );  // objc_msgSend(obj, selector, arg1, arg2, ...)`
如果消息的接收者能够找到对应的selector，那么就相当于直接执行了接收者这个对象的特定方法；否则，消息要么被转发，或是临时向接收者动态添加这个 selector 对应的实现内容，要么就崩溃掉。
下面将会逐渐展开介绍一些术语，及它们各自对应的数据结构。
#### id
- id 类型参数 `objc_msgSend`方法的第一个参数，`objc.h`文件中查看：
```
/// Represents an instance of a class.
struct objc_object {
  Class isa  OBJC_ISA_AVAILABILITY;
};
/// A pointer to an instance of a class.
typedef struct objc_object *id;
```
- 由此可知`id`是指向`objc_object`结构体的指针变量
- `objc_object`结构体代表一个类实例其包含了一个指向`objc_class`类型结构体的指针(上面已经分析过
```
Class
 isa
```
)；
- 总结`id`为指向类实例的指针，也就是他可以指向任何对象
#### SEL
- SEL 类型参数`objc_msgSend`方法的第一个参数，`objc.h`文件中查看：
```cpp
```cpp
/// An opaque type that represents a method selector.
typedef struct objc_selector *SEL;
```
```
- `SEL`是`selector`在Objc中的表示类型，selector是方法选择器
- `SEL`可以理解为方法编号，其实它就是个映射到方法的C字符串,`objc.h`文件中：
```
struct objc_method {
   SEL method_name;
   char *method_types;
   IMP method_imp;
};
```
- 可以用 Objc 编译器命令`@selector()`或者
 Runtime 系统的`sel_registerName`函数来获得一个SEL类型的方法选择器。
- 不同类中相同名字的方法所对应的方法选择器是相同的，即使方法名字相同而变量类型不同也会导致它们具有相同的方法选择器，于是 Objc 中方法命名有时会带上参数类型(NSNumber一堆抽象工厂方法)，Cocoa 中有好多长长的方法....。
#### ...是方法中的实参
```
objc_msgSend(receiver, selector, arg1, arg2,...)
// 例如 调someClass对象发送- (void)sendMessage:(NSString *)str;方法
objc_msgSend(someClass, @selector("sendMessage:"),@"zerocc");  //
```
# Objective-C 消息发送流程
调用方法的方式有两种：
- `[object message]` 的方式调用方法，如果一个对象无法按上述正常流程接受某一消息时，就会启动所谓“消息转发(message
 forwarding)”机制，通过这一机制，我们可以告诉对象如何处理未知的消息。默认情况下，对象接收到未知的消息，会导致程序崩溃，通过控制台，我们可以看到以下异常信息：这段异常信息实际上是由NSObject的`doesNotRecognizeSelector`方法抛出的。不过，我们可以采取一些措施，让我们的程序执行特定的逻辑，而避免程序的崩溃。
- 以 perform… 的形式来调用，则需要等到运行时才能确定object是否能接收message消息。如果不能，则程序崩溃。通常，当我们不能确定一个对象是否能接收某个消息时，会先调用respondsToSelector:来判断一下。如下代码所示：
```
if([self respondsToSelector:@selector(method)]){
      [self performSelector:@selector(method)];
  }
```
但是最终运行时都是调用了`objc_msgSend(receiver, selector, arg1, arg2,...)`函数，这个函数完成了动态绑定的所有事情：
- 检测这个selector是不是要忽略。比如Mac OS X开发，有了垃圾回收就不理会retain, release这些函数了。
- 检测这个target是不是nil对象。ObjC的特性是允许对一个nil对象执行任何一个方法不会Crash，因为会被忽略掉。
- 上面检测都通过则开始查找这个类的IMP,先从cache里面找,完了找得到就跳到对应的函数去执行。
- 如果cache找不到，通过对象的isa指针获取到类的结构体，然后在方法分发表里面查找方法的selector (方法分发表既是：class中的方法列表method_list，它将方法选择器和方法实现联系起来)。
- 如果分发表找不到，objc_msgSend 结构体中指向父类的指针找到其父类，并在父类的分发表去找方法的selector，会一直沿着类的继承体系到达NSObject类。一旦定位到selector，函数会就获取到了实现的入口点，并传入相应的参数来执行方法的具体实现,并将该方法添加进入缓存中。如果最后也没有定位到selector，则会走消息转发流程。
在异常抛出前，Objective-C 的运行时会给你三次拯救程序的机会：
- 动态方法解析
- 重定向接收者
- 完整的消息转发
#### 动态方法解析
对象在接收到未知的消息时，首先会调用所属类的类方法 +resolveInstanceMethod:(实例方法)或者 +resolveClassMethod:(类方法)。在这个方法中，我们有机会为该未知消息新增一个“处理方法”(或者说函数实现)，通过运行时class_addMethod函数动态添加到类里面就可以了。
- `+ (BOOL)resolveInstanceMethod:(SEL)sel;`解析实例方法
- `+ (BOOL)resolveClassMethod:(SEL)sel;`解析类方法
- 通过`class_addMethod(...)`的方式将缺少的selector动态创建出来，前提是有提前实现好的IMP（method_types一致)
- 如果resolveInstanceMethod：方法返回NO，运行时就会进行下一步：消息转发（Message Forwarding）。
- 用处：这种方案更多的是为@dynamic属性准备的，Core Data有效用到这个方法，NSManagedObjects中的属性的getter和setter就是在运行时动态添加的。
具体代码示例：
```
//  CCSomeClass.h
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import <Foundation/Foundation.h>
@interface CCSomeClass : NSObject
- (void)resolveMethod;
@end
//  CCSomeClass.m
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import "CCSomeClass.h"
#import <objc/runtime.h>
@implementation CCSomeClass
void addResolveMethod(id obj, SEL _cmd) {
    NSLog(@"resolveMethod was called ");
}
+ (BOOL)resolveInstanceMethod:(SEL)sel {
    if(sel == @selector(resolveMethod)){
        class_addMethod([self class], sel, (IMP)addResolveMethod, "v@:");
        return YES;
    }
    return [super resolveInstanceMethod:sel];
}
@end
```
#### 重定向接收者
如果上一步动态方法解析没有处理，runtime会调用以下方法
- `-(id)forwardingTargetForSelector:(SEL)aSelector;`
- 如果该方法返回非nil的对象，则使用该对象作为新的消息接收者
不能返回self，会出现无限循环
- 如果不知道该返回什么，应该使用
```
[super
 forwardingTargetForSelector:aSelector];
```
- 这种方法属于单纯的转发，无法对消息的参数和返回值进行处理
- 这一步合适于我们只想将消息转发到另一个能处理该消息的对象上。但这一步无法对消息进行处理，如操作消息的参数和返回值。
具体代码示例：
```
//  CCSomeClass.h
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import <Foundation/Foundation.h>
@interface CCSomeClass : NSObject
- (void)forwardMethod_arrayWithString:(NSString *)str;
@end
//  CCSomeClass.m
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import "CCSomeClass.h"
#import <objc/runtime.h>
#import "CCOtherClass.h"
@implementation CCSomeClass
#pragma mark - 重定向接收者
- (id)forwardingTargetForSelector:(SEL)aSelector
{
    //获取方法名
    NSString *selectorString = NSStringFromSelector(aSelector);
    //根据方法名添加方法
    if ([selectorString isEqualToString:@"forwardMethod_arrayWithString:"]) {
        CCOtherClass *otherClass = [[CCOtherClass alloc] init];
        return otherClass;
    }
    return [super forwardingTargetForSelector:aSelector];
}
@end
```
此例中备用接受者为自定义的`CCOtherclass`，其具体代码：
```
//  CCOtherClass.h
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import <Foundation/Foundation.h>
@interface CCOtherClass : NSObject
@end
//  CCOtherClass.m
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import "CCOtherClass.h"
@implementation CCOtherClass
/**
 *  把字符串转换为数组
 *
 *  @param str 需转换的字符串
 *
 *  @return 转换好的数组
 */
- (NSArray *)forwardMethod_arrayWithString:(NSString *)str
{
    if (str && (str != NULL) && (![str isKindOfClass:[NSNull class]]) && str.length > 0) {
        NSMutableArray *mArr = [NSMutableArray arrayWithCapacity:1];
        for (NSInteger index = 0; index < str.length; index++) {
            [mArr addObject:[str substringWithRange:NSMakeRange(index, 1)]];
        }
        NSLog(@"array:::%@",mArr);
        return mArr;
    }
    return nil;
}
@end
```
#### 完整的消息转发
- 消息转发的第一步重写方法：
```
-
 (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector;
```
为未有实现的方法的`selector`提供合适的方法签名然后返回这个aSelector的方法签名。
- 当对象发送一个unrecognized 的消息时，会使用从上面方法获取的方法签名等方法信息创建一个表示消息的 `NSInvocation` 对象，把与尚未处理的消息有关的全部细节都封装在`anInvocation`中，包括`selector`，目标(`target`)和参数。
- 然后调用
```
- (void)forwardInvocation:(NSInvocation
 *)anInvocation;
```
方法，在方法中又通过`- (void)invokeWithTarget:(id)target;`方法选择将消息转发给指定对象。其中
 forwardInvocation: 方法的实现有两个任务：
- 定位可以响应封装在anInvocation中的消息的对象。这个对象不需要能处理所有未知消息。
- 使用anInvocation作为参数，将消息发送到选中的对象。anInvocation将会保留调用结果，运行时系统会提取这一结果并将其发送 到消息的原始发送者。
具体代码样例分析：
```
//  CCSomeClass.h
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import <Foundation/Foundation.h>
@interface CCSomeClass : NSObject
- (void)usualMethod;
- (void)resolveMethod;
- (void)forwardMethod_arrayWithString:(NSString *)str;
- (void)signatureMethod_inverseWithString:(NSString *)str;
@end
#import "CCSomeClass.h"
#import <objc/runtime.h>
#import "CCOtherClass.h"
@implementation CCSomeClass
#pragma mark - 完整消息转发
//必须重写这个方法，为给定的selector提供一个合适的方法签名。
// 返回aSelector的方法签名
- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector
{
    NSMethodSignature *signature = [super methodSignatureForSelector:aSelector];
    if (!signature) {
        if ([CCOtherClass instancesRespondToSelector:aSelector]) {
            //获取方法签名
            signature = [CCOtherClass instanceMethodSignatureForSelector:aSelector];
        }
    }
    return signature;
}
// 当对self发送一个unrecoginzed的消息时，会创建一个NSInvocation，并调用这个方法。允许在这个方法中，通过[anInvocation invokeWithTarget:otherSelf];的方式进行消息转发。
- (void)forwardInvocation:(NSInvocation *)anInvocation
{
    //anInvocation选择将消息转发给其它对象
    if ([CCOtherClass instancesRespondToSelector:anInvocation.selector]) {
        [anInvocation invokeWithTarget:[[CCOtherClass alloc] init]];
    }
}
@end
```
```
//  CCOtherClass.h
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
//
#import <Foundation/Foundation.h>
@interface CCOtherClass : NSObject
@end
//  CCOtherClass.m
//  CCRuntime
//
//  Created by zerocc on 2017/4/25.
//  Copyright © 2017年 zerocc. All rights reserved.
#import "CCOtherClass.h"
@implementation CCOtherClass
/**
 *  逆置字符串
 *
 *  @param str 需逆置的字符串
 *
 *  @return 置换后的字符串
 */
- (NSString *)signatureMethod_inverseWithString:(NSString *)str
{
    if (str && (str != NULL) && (![str isKindOfClass:[NSNull class]]) && str.length > 0) {
        NSMutableString *mStr = [NSMutableString stringWithCapacity:1];
        for (NSInteger index = str.length; index > 0; index--) {
            [mStr appendString:[str substringWithRange:NSMakeRange(index - 1, 1)]];
        }
        NSLog(@"retureStr:::%@",mStr);
        return mStr;
    }
    return nil;
}
@end
```
# 运行时系统涉及的类与对象操作函数
- 属性相关操作函数
```cpp
```cpp
CCMyClass *myClass = [[CCMyClass alloc] init];
unsigned int outCount = 0;
Class cls = [myClass class];
// 属性操作
objc_property_t * properties = class_copyPropertyList(cls, &outCount);
for (int i = 0; i < outCount; i++) {
    objc_property_t property = properties[i];
    const char *propertyName =  property_getName(property);
    NSLog(@"property's name: %s", propertyName);
}
free(properties);
```
```
```cpp
```cpp
//成员变量操作函数
// 获取类中指定名称实例成员变量的信息
Ivar class_getInstanceVariable ( Class cls, const char *name );
// 获取类成员变量的信息
Ivar class_getClassVariable ( Class cls, const char *name );
// 添加成员变量
BOOL class_addIvar ( Class cls, const char *name, size_t size, uint8_t alignment, const char *types ); //这个只能够向在runtime时创建的类添加成员变量
// 获取整个成员变量列表
Ivar * class_copyIvarList ( Class cls, unsigned int *outCount ); //必须使用free()来释放这个数组
//属性操作函数
// 获取类中指定名称实例成员变量的信息
Ivar class_getInstanceVariable ( Class cls, const char *name );
// 获取类成员变量的信息
Ivar class_getClassVariable ( Class cls, const char *name );
// 添加成员变量
BOOL class_addIvar ( Class cls, const char *name, size_t size, uint8_t alignment, const char *types );
// 获取整个成员变量列表
Ivar * class_copyIvarList ( Class cls, unsigned int *outCount );
```
```
- 方法相关操作函数
```
//  获取一个类方法列表
Method *methods = class_copyMethodList(cls, &outCount);
for (int i = 0; i < outCount; i++) {
    Method method = methods[i];
    SEL method_name = method_getName(method);
    NSLog(@"method's signature - method_name: %s", method_name);
}
free(methods);
```
```
// 添加方法
//和成员变量不同的是可以为类动态添加方法。如果有同名会返回NO，修改的话需要使用method_setImplementation
BOOL class_addMethod(Class cls, SEL name, IMP imp, 
                                 const char *types) ;
// 替换原方法实现(偷梁换柱)
IMP class_replaceMethod(Class cls, SEL name, IMP imp, 
                                    const char *types) ;
// 交换两个方法的实现
void method_exchangeImplementations(Method m1, Method m2);
// 返回方法的具体实现
IMP class_getMethodImplementation ( Class cls, SEL name );
IMP class_getMethodImplementation_stret ( Class cls, SEL name );
// 类实例是否响应指定的selector
BOOL class_respondsToSelector ( Class cls, SEL sel );
```
- 协议相关操作函数
```cpp
```cpp
// 添加协议
BOOL class_addProtocol ( Class cls, Protocol *protocol );
// 返回类是否实现指定的协议
BOOL class_conformsToProtocol ( Class cls, Protocol *protocol );
// 返回类实现的协议列表
Protocol * class_copyProtocolList ( Class cls, unsigned int *outCount );
```
```
# Runtime黑魔法的具体运用
- 能获得某个类的所有成员变量
- 能获得某个类的所有属性
- 能获得某个类的所有方法
- 交换方法实现
- 能动态添加一个成员变量
- 能动态添加一个属性
- 字典转模型
- runtime归档/反归档
#### 交换方法
- 开发使用场景:系统自带的方法功能不够，给系统自带的方法扩展一些功能，并且保持原有的功能。
- 方式一:继承系统的类，重写方法
- 方式二:使用runtime,交换方法.
```
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    // 需求：给imageNamed方法提供功能，每次加载图片就判断下图片是否加载成功。
    // 步骤一：先搞个分类，定义一个能加载图片并且能打印的方法+ (instancetype)imageWithName:(NSString *)name;
    // 步骤二：交换imageNamed和imageWithName的实现，就能调用imageWithName，间接调用imageWithName的实现。
    UIImage *image = [UIImage imageNamed:@"123"];
}
@end
@implementation UIImage (Image)
// 加载分类到内存的时候调用
+ (void)load
{
    // 交换方法
    // 获取imageWithName方法地址
    Method imageWithName = class_getClassMethod(self, @selector(imageWithName:));
    // 获取imageWithName方法地址
    Method imageName = class_getClassMethod(self, @selector(imageNamed:));
    // 交换方法地址，相当于交换实现方式
    method_exchangeImplementations(imageWithName, imageName);
}
// 不能在分类中重写系统方法imageNamed，因为会把系统的功能给覆盖掉，而且分类中不能调用super.
// 既能加载图片又能打印
+ (instancetype)imageWithName:(NSString *)name
{
    // 这里调用imageWithName，相当于调用imageName
    UIImage *image = [self imageWithName:name];
    if (image == nil) {
        NSLog(@"加载空的图片");
    }
    return image;
}
@end
```
#### 动态添加方法
- 开发使用场景：如果一个类方法非常多，加载类到内存的时候也比较耗费资源，需要给每个方法生成映射表，可以使用动态给某个类，添加方法解决。
- 经典面试题：有没有使用performSelector，其实主要想问你有没有动态添加过方法。
代码示例：
```
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    Person *p = [[Person alloc] init];
    // 默认person，没有实现eat方法，可以通过performSelector调用，但是会报错。
    // 动态添加方法就不会报错
    [p performSelector:@selector(eat)];
}
@end
@implementation Person
// void(*)()
// 默认方法都有两个隐式参数，
void eat(id self,SEL sel)
{
    NSLog(@"%@ %@",self,NSStringFromSelector(sel));
}
// 当一个对象调用未实现的方法，会调用这个方法处理,并且会把对应的方法列表传过来.
// 刚好可以用来判断，未实现的方法是不是我们想要动态添加的方法
+ (BOOL)resolveInstanceMethod:(SEL)sel
{
    if (sel == @selector(eat)) {
        // 动态添加eat方法
        // 第一个参数：给哪个类添加方法
        // 第二个参数：添加方法的方法编号
        // 第三个参数：添加方法的函数实现（函数地址）
        // 第四个参数：函数的类型，(返回值+参数类型) v:void @:对象->self :表示SEL->_cmd
        class_addMethod(self, @selector(eat), eat, "v@:");
    }
    return [super resolveInstanceMethod:sel];
}
@end
```
#### 给分类添加属性
- 原理：给一个类声明属性，其实本质就是给这个类添加关联，并不是直接把这个值的内存空间添加到类存空间。
```
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // 给系统NSObject类动态添加属性name
    NSObject *objc = [[NSObject alloc] init];
    objc.name = @"zerocc";
    NSLog(@"%@",objc.name);
}
@end
// 定义关联的key
static const char *key = "name";
@implementation NSObject (Property)
- (NSString *)name
{
    // 根据关联的key，获取关联的值。
    return objc_getAssociatedObject(self, key);
}
- (void)setName:(NSString *)name
{
    // 第一个参数：给哪个对象添加关联
    // 第二个参数：关联的key，通过这个key获取
    // 第三个参数：关联的value
    // 第四个参数:关联的策略
    objc_setAssociatedObject(self, key, name, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}
@end
```
#### 归档和解档 一键序列化
- 原理描述：用runtime提供的函数遍历Model自身所有属性，并对属性进行encode和decode操作。
- 核心方法：在Model的基类中重写方法：
如果需要实现一些基本数据的数据持久化(data persistance)或者数据共享(data share)。我们可以选择归档和解档。如果用一般的方法:
```
- (void)encodeWithCoder:(NSCoder *)aCoder {
    [aCoder encodeObject:self.name forKey:@"nameKey"];
    [aCoder encodeObject:self.gender forKey:@"genderKey"];
    [aCoder encodeObject:[NSNumber numberWithInteger:self.age] forKey:@"ageKey"];
}
```
也可以实现。但是如果实体类有很多的成员变量，这种方法很显然就无力了。这个时候，我们就可以利用runtime来实现快速归档、解档:
- 让实体类遵循<NSCoding>协议。并在.m文件导入头文件<objc/runtime.h>。
- 实现
```
- (instancetype)initWithCoder:(NSCoder
 *)aDecoder;
```
和`- (void)encodeWithCoder:(NSCoder *)aCoder;`方法。
```
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super init];
    if (self) {
        //
        unsigned int count = 0;
        objc_property_t *properties = class_copyPropertyList([self class], &count);
        for (int i = 0; i < count; i ++) {
            objc_property_t property = properties[i];
            const char *propertyChar = property_getName(property);
            NSString *propertyString = [NSString stringWithUTF8String:propertyChar];
            id value = [aDecoder decodeObjectForKey:propertyString];
            [self setValue:value forKey:propertyString];
        }
        free(properties);
    }
    return self;
}
- (void)encodeWithCoder:(NSCoder *)aCoder {
    unsigned int count = 0;
    objc_property_t *properties = class_copyPropertyList([self class], &count);
    for (int i = 0; i < count; i ++) {
        objc_property_t property = properties[i];
        const char *propertyChar = property_getName(property);
        NSString *propertyString = [NSString stringWithUTF8String:propertyChar];
        id object = [self valueForKey:propertyString];
        [aCoder encodeObject:object forKey:propertyString];
    }
    free(properties);
}
```
或者这种写法：
```
- (id)initWithCoder:(NSCoder *)aDecoder {
    if (self = [super init]) {
        unsigned int outCount;
        Ivar * ivars = class_copyIvarList([self class], &outCount);
        for (int i = 0; i < outCount; i ++) {
            Ivar ivar = ivars[i];
            NSString * key = [NSString stringWithUTF8String:ivar_getName(ivar)];
            [self setValue:[aDecoder decodeObjectForKey:key] forKey:key];
        }
    }
    return self;
}
- (void)encodeWithCoder:(NSCoder *)aCoder {
    unsigned int outCount;
    Ivar * ivars = class_copyIvarList([self class], &outCount);
    for (int i = 0; i < outCount; i ++) {
        Ivar ivar = ivars[i];
        NSString * key = [NSString stringWithUTF8String:ivar_getName(ivar)];
        [aCoder encodeObject:[self valueForKey:key] forKey:key];
    }
}
```
#### 控制器的万能跳转
应用场景：
- 推送：根据服务端推送过来的数据规则，跳转到对应的控制器
- 列表：不同类似的名字，可能跳转不同的控制器，任意跳转
```
- (void)testRuntime
{
    NSDictionary *userInfo = @{@"class":@"CCRuntimePushVC",
                               @"property": @{
                                       @"ID":@"81198",
                                       @"type":@"2"
                                       }
                               };
    [self push:userInfo];
}
// 跳转
- (void)push:(NSDictionary *)params
{
    // 得到类名
    NSString *className = [NSString stringWithFormat:@"%@",params[@"class"]];
    // 通过名称转换成Class
    Class getClass = NSClassFromString([NSString stringWithFormat:@"%@",className]);
    // 判断得到的这个class 是否存在
    if (getClass) {
        // 创建 class 对象
        id creatClass = [[getClass alloc] init];
        NSDictionary *propertys = params[@"property"];
        [propertys enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            if ([self checkIsExistPropertyWithInstance:creatClass verifyPropertyName:key]) {
                // 利用 kvc 赋值
                [creatClass setValue:obj forKey:key];
            }
        }];
        [self.navigationController pushViewController:creatClass animated:YES];
    }else{
        NSLog(@"not this class,can not push");
    }
}
// 检查对象是否存在该属性
- (BOOL)checkIsExistPropertyWithInstance:(id)instance verifyPropertyName:(NSString *)verifyPropertyName
{
    unsigned int outCount, i;
    // 获取对象的属性列表
    objc_property_t *properties = class_copyPropertyList([instance class], &outCount);
    for (i = 0; i < outCount; i++) {
        objc_property_t property = properties[i];
        // 属性名转换成字符串
        NSString *propertyName = [[NSString alloc] initWithCString:property_getName(property)  encoding:NSUTF8StringEncoding];
        // 判断该属性是否存在
        if ([propertyName isEqualToString:verifyPropertyName]) {
            free(properties);
            return YES;
        }
    }
    free(properties);
    return NO;
}
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self testRuntime];
}
```
# 参考链接
[Objc 对象的今生今世](http://www.jianshu.com/p/f725d2828a2f)
[Objective-C 维基百科](https://zh.wikipedia.org/wiki/Objective-C)
[精神病院Objective-C runtime系列](https://halfrost.com/tag/runtime/)
[苹果官方API文档解释](https://developer.apple.com/reference/objectivec/objective_c_runtime)
[苹果官方Objective-C运行时编程指南文档](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40008048)
