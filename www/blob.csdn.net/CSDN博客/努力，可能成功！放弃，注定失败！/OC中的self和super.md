# OC中的self和super - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月09日 22:31:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：324
在面试时一般都会注重基础的，不管是对初级，中级还是高级。本人主要介绍一下在OC中最常用的两个关键字self和super，它们常用在对象初始化方法里。不知道大家有没有想过研究过这个初始化方法？
# 问题：
> 
下面是对象初始化方法：
```
@implementation Son : Father
- (id)init {
    self = [super init];
    if (self) {
    }
    return self;
}
```
相信很多人对这段代码非常熟悉，在读完后你需要明白几个问题。
1. self和super是什么？
2. [super init]做了什么？
2. 为什么要把[super init]赋值给self？
## 答案：
问题1解析：
self是一个隐藏参数变量，指向当前调用方法的对象，还有一个隐藏参数是_cmd,代表当前方法selector。在runtime时会调用objc_msgSend()方法。
super并不是隐藏参数，只是编译器的指令符号，在runtime时调用objc_msgSendSuper()方法。
官方文档针对self和super已有解释。
> 
官方文档中self相关解释
Whenever you’re writing a method implementation, you have access to an important hidden value, self. Conceptually, self is a way to refer to “the object that’s received this message.” It’s apointer, just like the greeting value above, and can be used to call
 a method on the current receiving object.
super解释
There’s anotherimportant keyword available to you in Objective-C, called super. Sending a message to super is a way to call through to a method implementation defined by a superclass further up the inheritance chain. The most common use of super is when overriding
 a method.
所以可以简单理解：
> 
self调用自己方法，super调用父类方法
self是类，super是预编译指令
[self class]和[super class]输出是一样的
问题2解析：
想要明白这个问题需要理解self和super的底层实现原理。
Self调用方法：
> - 当使用 self 调用方法时，会首先从当前类的方法列表中开始寻找，如果没有再从父类中寻找；
- 当使用 self 调用方法时，RT时会使用 objc_msgSend 函数：
`id objc_msgSend(id theReceiver, SEL theSelector, ...)`
第一个参数是消息接收者，第二个参数是调用的具体类方法的selector，后面是 selector 方法的可变参数。
- 以 [self setSize:] 为例，编译器会转换成objc_msgSend的函数调用，其中 theReceiver 是 self，theSelector 是 @selector(setSize:)，这个 selector 是从当前 self 的 class 的方法列表开始找的 setSize，一旦找到后把对应的selector传递过去。
Super调用方法：
> - 当使用 super 时，则从父类的方法列表中开始找，然后调用父类的这个方法。
- 当使用 super 调用时，RT时会使用 objc_msgSendSuper 函数：
`id objc_msgSendSuper(struct objc_super *super, SEL op, ...)`
第一个参数是个objc_super的结构体，第二个参数还是类似objc_msgSend类方法的selector。而objc_super的结构体如下：
```
struct objc_super {
id receiver;
Class superClass;
};
```
- 当编译器遇到 [super setSize:] 时，开始做下面几个事：
1）构建objc_super的结构体，此时这个结构体的第一个成员变量receiver就是子类，和 self 中相同。而第二个成员变量superClass就是指父类
调用 objc_msgSendSuper 的方法，将这个结构体和setSize的selector传递过去。
2）函数里面做的事情类似这样：从objc_super结构体指向的superClass的方法列表开始找 setSize的selector，找到后再用objc_super->receiver去调用这个selector。
知道self和super的原理就会很容易明白为什么[self class]和[super class]输出结果会是一样的。
问题3解析：
OC具有类继承特性，子类继承父类从而获得相关的属性和方法，所以在子类的初始化方法中，必须首先调用父类的初始化方法，完成父类相关资源的初始化。
[super init]去self的super中调用init, 然后super会调用其父类的init，以此类推，直到找到根类NSObject中的init。然后根类中的init负责初始化内存区域，添加一些必要的属性，返回内存指针，延着继承链，指针从上到下进行传递，同时在不同的子类中可以向内存添加必要的属性。最后直到我们当前类中把内存地址赋值给self参数。当然，如果调用[super init]失败的话，通过判断self来决定是否执行子类的初始化操作。
为了更清楚理解self和super，看下面两个例子：
示例#1：
```
@implementation Son : Father
- (id)init {
    self = [super init];
    if (self)  {
        NSLog(@"%@", NSStringFromClass([self class]));
        NSLog(@"%@", NSStringFromClass([super class]));
    }
    return self;
}
@end
```
答案：
> 
控制台输出打印结果：
```
Son
Son
```
当发送class消息时不管是用self还是用super，接受消息主体依然是self ,也就是说self和super指向同一个对象，所以都会打印出Son。
示例2#：
```
@interface Father : NSObject
- (void)printCurrentClass;
@end
@implementation Father
- (void)printCurrentClass {
    NSLog(@"printCurrentClass:%@", [self class]);
}
@end
@interface Son : Father
- (void)printSuperClass;
@end
@implementation Son
- (void)printSuperClass {
    [super printCurrentClass];
}
@end
// 调用方法
Son *son = [Son new];
[son printCurrentClass]; // 直接调用父类方法，子类没有重载
[son printSuperClass]; // 间接调用父类方法
```
答案：
> 
控制台输出结果：
```
printCurrentClass:Son
printCurrentClass:Son
```
printCurrentClass方法体中self始终指向方法的接收者对象son，倘若换成[super class]，结果也是一样的。
