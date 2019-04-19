# iOS初级到中级的进阶之路 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:46:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：39
> 
[线程/内存管理/性能优化](https://ke.qq.com/course/314526)总结所得，断断续续历时3个月左右，把课堂听的东西给做了一下笔记。
- [1、一个NSObject对象占用多少内存](https://github.com/SunshineBrother/JHBlog)
- [2、OC对象的分类](https://github.com/SunshineBrother/JHBlog)
- [3、KVO实现原理](https://github.com/SunshineBrother/JHBlog)
- [4、KVC实现原理](https://github.com/SunshineBrother/JHBlog)
- [5、分类](https://github.com/SunshineBrother/JHBlog)- [5.1、分类的实现原理](https://github.com/SunshineBrother/JHBlog)
- [5.2、Load和Initialize实现原理](https://github.com/SunshineBrother/JHBlog)
- [6、Block底层解密](https://github.com/SunshineBrother/JHBlog)
- [7、RunLoop实现原理](https://github.com/SunshineBrother/JHBlog)
- [8、RunTime实现原理](https://github.com/SunshineBrother/JHBlog)- [8.1、isa解析](https://github.com/SunshineBrother/JHBlog)
- [8.2、方法缓存](https://github.com/SunshineBrother/JHBlog)
- [8.3、objc_msgSend执行流程](https://github.com/SunshineBrother/JHBlog)
- [8.4、@dynamic关键字](https://github.com/SunshineBrother/JHBlog)
- [8.5、Class和SuperClass区别](https://github.com/SunshineBrother/JHBlog)
- [8.6、isKindOfClass和isMemberOfClass区别](https://github.com/SunshineBrother/JHBlog)
- [8.7、RunTime的相关API](https://github.com/SunshineBrother/JHBlog)
- [9、多线程](https://github.com/SunshineBrother/JHBlog)- [9.1、多线程面试题](https://github.com/SunshineBrother/JHBlog)
- [9.2、多线程之NSThread](https://github.com/SunshineBrother/JHBlog)
- [9.3、多线程之GCD](https://github.com/SunshineBrother/JHBlog)
- [9.4、多线程之NSOperation](https://github.com/SunshineBrother/JHBlog)
- [9.5、多线程之线程安全](https://github.com/SunshineBrother/JHBlog)
- [9.6、死锁](https://github.com/SunshineBrother/JHBlog)
- [9.7、GCD高级用法](https://github.com/SunshineBrother/JHBlog)
- [9.8、线程之间的通讯](https://github.com/SunshineBrother/JHBlog)
- [10、内存管理]()- [10.1、定时器target的内存泄漏](https://github.com/SunshineBrother/JHBlog)
- [10.2、Tagged Pointer](https://github.com/SunshineBrother/JHBlog)
- [10.3、copy&retain&strong原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86/3%E3%80%81copy%26retain%26strong%E5%8E%9F%E7%90%86.md)
- [10.4、weak&assign原理](https://github.com/SunshineBrother/JHBlog)
- [10.5、@property 的本质是什么](https://github.com/SunshineBrother/JHBlog)
- [10.6、autorelease原理](https://github.com/SunshineBrother/JHBlog)
- [10.7、atomic 一定是线程安全的吗](https://github.com/SunshineBrother/JHBlog)
**总结不易，耗时耗力，您的一颗小星星✨是我无限的动力。原文[地址](https://github.com/SunshineBrother/JHBlog)**
![](https://upload-images.jianshu.io/upload_images/2348494-c6eda5d140d0d43f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
iOS底层原理.png
我们经常会看一些面试题，但是好多面试题我们都是知其然不知其所以然，你如果认真的看了我上面总结的几十篇文章，那么你也会知其所以然。
### OC对象本质
**1、一个NSObject对象占用多少内存？**
系统分配了16个字节给NSObject对象（通过malloc_size函数获得），但NSObject对象内部只使用了8个字节的空间（64bit环境下，可以通过class_getInstanceSize函数获得）
**2、对象的isa指针指向哪里？**
- instance对象的isa指向class对象
- class对象的isa指向meta-class对象
- meta-class对象的isa指向基类的meta-class对象
**3、OC的类信息存放在哪里？**
- 对象方法、属性、成员变量、协议信息，存放在class对象中
- 类方法，存放在meta-class对象中
- 成员变量的具体值，存放在instance对象
具体实现请参考：
[1、一个NSObject对象占用多少内存](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/1%E3%80%81%E4%B8%80%E4%B8%AANSObject%E5%AF%B9%E8%B1%A1%E5%8D%A0%E7%94%A8%E5%A4%9A%E5%B0%91%E5%86%85%E5%AD%98.md)
[2、OC对象的分类](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/2%E3%80%81OC%E5%AF%B9%E8%B1%A1%E7%9A%84%E5%88%86%E7%B1%BB.md)
### KVO
**1、iOS用什么方式实现对一个对象的KVO？(KVO的本质是什么？)**
- 利用RuntimeAPI动态生成一个子类，并且让instance对象的isa指向这个全新的子类
- 当修改instance对象的属性时，会调用Foundation的`_NSSetXXXValueAndNotify`函数
	- 1、调用`willChangeValueForKey`方法
- 2、调用`setAge`方法
- 3、调用`didChangeValueForKey`方法
- 4、`didChangeValueForKey`方法内部调用oberser的`observeValueForKeyPath:ofObject:change:context:`方法
**2、如何手动触发KVO？**
手动调用willChangeValueForKey:和didChangeValueForKey:
**3、直接修改成员变量会触发KVO么？**
不会触发KVO
具体实现请参考：[3、KVO实现原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/3%E3%80%81KVO.md)
### KVC
**1、通过KVC修改属性会触发KVO么？**
会触发KVO，因为KVC是调用`set`方法，KVO就是监听`set`方法
**2、KVC的赋值和取值过程是怎样的？原理是什么？**
`KVO的setValue:forKey原理`
![](https://upload-images.jianshu.io/upload_images/2348494-56f63a95cb8e6b10.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/961/format/webp)
KVC2.png
- 1、按照setKey，_setKey的顺序查找成员方法，如果找到方法，传递参数，调用方法
- 2、如果没有找到，查看accessInstanceVariablesDirectly的返回值（accessInstanceVariablesDirectly的返回值默认是YES），
	- 返回值为YES，按照_Key,_isKey,Key,isKey的顺序查找成员变量， 如果找到，直接赋值，如果没有找到，调用setValue:forUndefinedKey:，抛出异常
- 返回NO，直接调用setValue:forUndefinedKey:，抛出异常
`KVO的ValueforKey原理`
![](https://upload-images.jianshu.io/upload_images/2348494-b94a86cbb4e250a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/959/format/webp)
KVC3.png
- 1、按照getKey,key,isKey,_key的顺序查找成员方法，如果找到直接调用取值
- 2、如果没有找到，查看accessInstanceVariablesDirectly的返回值
	- 返回值为YES，按照_Key,_isKey,Key,isKey的顺序查找成员变量，如果找到，直接取值，如果没有找到，调用setValue:forUndefinedKey:，抛出异常
- 返回NO，直接调用setValue:forUndefinedKey:，抛出异常
具体实现请参考：[4、KVC实现原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/4%E3%80%81KVC.md)
### Category
**1、Category的实现原理**
- Category编译之后的底层结构是struct category_t，里面存储着分类的对象方法、类方法、属性、协议信息
- 在程序运行的时候，runtime会将Category的数据，合并到类信息中（类对象、元类对象中）
**2、Category和Class Extension的区别是什么？**
- Class Extension在编译的时候，它的数据就已经包含在类信息中
- Category是在运行时，才会将数据合并到类信息中
**3、load、initialize方法的区别什么？**
- 
1.调用方式
- 1> load是根据函数地址直接调用
- 2> initialize是通过objc_msgSend调用
- 
2.调用时刻
- 1> load是runtime加载类、分类的时候调用（只会调用1次 ）
- 2> initialize是类第一次接收到消息的时候调用，每一个类只会initialize一次（父类的initialize方法可能会被调用多次）
**4、load、initialize的调用顺序**
1.load
- 1> 先调用类的load
	- a) 先编译的类，优先调用load
- b) 调用子类的load之前，会先调用父类的load
- 2> 再调用分类的load
	- a) 先编译的分类，优先调用load
2.initialize
- 1> 先初始化父类
- 2> 再初始化子类（可能最终调用的是父类的initialize方法）
**5、如何实现给分类“添加成员变量”？**
默认情况下，因为分类底层结构的限制，不能添加成员变量到分类中。但可以通过关联对象来间接实现
```
关联对象提供了以下API
添加关联对象
void objc_setAssociatedObject(id object, const void * key,
                                id value, objc_AssociationPolicy policy)
获得关联对象
id objc_getAssociatedObject(id object, const void * key)
移除所有的关联对象
void objc_removeAssociatedObjects(id object)
```
具体实现请参考：
[5.1、分类的实现原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/%E5%88%86%E7%B1%BB/1%E3%80%81%E5%88%86%E7%B1%BB%E7%9A%84%E5%AE%9E%E7%8E%B0%E5%8E%9F%E7%90%86.md)
[5.2、Load和Initialize实现原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/%E5%88%86%E7%B1%BB/2%E3%80%81Load%E5%92%8CInitialize%E5%AE%9E%E7%8E%B0%E5%8E%9F%E7%90%86.md)
### Block
**1、block的原理是怎样的？本质是什么？**
- block本质上也是一个OC对象，它内部也有个isa指针
- block是封装了函数调用以及函数调用环境的OC对象
![](https://upload-images.jianshu.io/upload_images/2348494-3c53169c8940e4be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/876/format/webp)
block的底层.png
**2、block的（capture）**
![](https://upload-images.jianshu.io/upload_images/2348494-543eefa7d44a8278.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
变量捕获.png
为了保证block内部能够正常访问外部的变量，block有个变量捕获机制
**3、Block类型有哪几种**
block有3种类型，可以通过调用class方法或者isa指针查看具体类型，最终都是继承自NSBlock类型
- 1、**NSGlobalBlock** （ _NSConcreteGlobalBlock
- 2、**NSStackBlock** （ _NSConcreteStackBlock ）
- 3、**NSMallocBlock** （ _NSConcreteMallocBlock ）
![](https://upload-images.jianshu.io/upload_images/2348494-4d906fe2cd903236.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/798/format/webp)
Block类型.png
**4、block的copy**
在ARC环境下，编译器会根据情况自动将栈上的block复制到堆上，比如以下情况
- 1、block作为函数返回值时
- 2、将block赋值给__strong指针时
- 3、block作为Cocoa API中方法名含有usingBlock的方法参数时
- 4、block作为GCD API的方法参数时
```
MRC下block属性的建议写法
@property (copy, nonatomic) void (^block)(void);
ARC下block属性的建议写法
@property (strong, nonatomic) void (^block)(void);
@property (copy, nonatomic) void (^block)(void);
```
**5、__block修饰符**
- `__block`可以用于解决block内部无法修改auto变量值的问题
- `__block`不能修饰全局变量、静态变量（static）
- 编译器会将`__block`变量包装成一个对象
- 
当__block变量在栈上时，不会对指向的对象产生强引用
- 
当__block变量被copy到堆时
- 会调用__block变量内部的copy函数
- copy函数内部会调用_Block_object_assign函数
- _Block_object_assign函数会根据所指向对象的修饰符（__strong、__weak、__unsafe_unretained）做出相应的操作，形成强引用（retain）或者弱引用（注意：这里仅限于ARC时会retain，MRC时不会retain）
- 
如果__block变量从堆上移除
- 会调用__block变量内部的dispose函数
- dispose函数内部会调用_Block_object_dispose函数
- _Block_object_dispose函数会自动释放指向的对象（release）
**6、循环引用**
- 用__weak、__unsafe_unretained解决
```
__unsafe_unretained typeof(self) weakSelf = self;
self.block = ^{
 print(@"%p", weakSelf);
}
```
```
__weak typeof(self) weakSelf = self;
self.block = ^{
 print(@"%p", weakSelf);
}
```
- 用__block解决（必须要调用block）
```
__block id weakSelf = self;
self.block = ^{
weakSelf = nil;
}
self.block();
```
具体实现请参考：[6、Block底层解密](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/7%E3%80%81Block%E5%BA%95%E5%B1%82%E8%A7%A3%E5%AF%86.md)
### RunTime
**1、讲一下 OC 的消息机制**
- OC中的方法调用其实都是转成了objc_msgSend函数的调用，给receiver（方法调用者）发送了一条消息（selector方法名）
- objc_msgSend底层有3大阶段：消息发送（当前类、父类中查找）、动态方法解析、消息转发
**2、消息转发机制流程**
- 1、消息发送
- 2、动态方法解析
- 3、消息转发
***消息发送阶段***
消息发送流程是我们平时最经常使用的流程，其他的像动态方法解析和消息转发其实是补救措施。具体流程如下
![](https://upload-images.jianshu.io/upload_images/2348494-67419d90a29ec8d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
消息发送1.png
- 1、首先判断消息接受者receiver是否为nil，如果为nil直接退出消息发送
- 2、如果存在消息接受者receiverClass，首先在消息接受者receiverClass的cache中查找方法，如果找到方法，直接调用。如果找不到，往下进行
- 3、没有在消息接受者receiverClass的cache中找到方法，则从receiverClass的class_rw_t中查找方法，如果找到方法，执行方法，并把该方法缓存到receiverClass的cache中；如果没有找到，往下进行
- 4、没有在receiverClass中找到方法，则通过superClass指针找到superClass，也是现在缓存中查找，如果找到，执行方法，并把该方法缓存到receiverClass的cache中；如果没有找到，往下进行
- 5、没有在消息接受者superClass的cache中找到方法，则从superClass的class_rw_t中查找方法，如果找到方法，执行方法，并把该方法缓存到receiverClass的cache中；如果没有找到，重复4、5步骤。如果找不到了superClass了，往下进行
- 6、如果在最底层的superClass也找不到该方法，则要转到动态方法解析
***动态方法解析***
![](https://upload-images.jianshu.io/upload_images/2348494-1dbb51c60aeb2ac3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/934/format/webp)
消息发送2.png
开发者可以实现以下方法，来动态添加方法实现
- `+resolveInstanceMethod:`
- `+resolveClassMethod:`
	动态解析过后，会重新走“消息发送”的流程，从receiverClass的cache中查找方法这一步开始执行
***消息转发***
如果方法一个方法在消息发送阶段没有找到相关方法，也没有进行动态方法解析，这个时候就会走到消息转发阶段了。
![](https://upload-images.jianshu.io/upload_images/2348494-67286b0265b49de7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/831/format/webp)
消息发送6.png
- 调用`forwardingTargetForSelector`，返回值不为nil时，会调用`objc_msgSend`(返回值, SEL)
- 调用`methodSignatureForSelector`,返回值不为nil，调用forwardInvocation:方法；返回值为nil时，调用`doesNotRecognizeSelector:`方法
- 开发者可以在`forwardInvocation:`方法中自定义任何逻辑
- 以上方法都有对象方法、类方法2个版本（前面可以是加号+，也可以是减号-）
**3、什么是Runtime？平时项目中有用过么？**
- OC是一门动态性比较强的编程语言，允许很多操作推迟到程序运行时再进行
- OC的动态性就是由Runtime来支撑和实现的，Runtime是一套C语言的API，封装了很多动态性相关的函数
- 平时编写的OC代码，底层都是转换成了Runtime API进行调用
***具体应用***
- 利用关联对象（AssociatedObject）给分类添加属性
- 遍历类的所有成员变量（修改textfield的占位文字颜色、字典转模型、自动归档解档）
- 交换方法实现（交换系统的方法）
- 利用消息转发机制解决方法找不到的异常问题
**4、super的本质**
- super调用，底层会转换为objc_msgSendSuper2函数的调用，接收2个参数
	- `struct objc_super2`
- `SEL`
- receiver是消息接收者
- current_class是receiver的Class对象
具体实现请参考：
- [8.1、isa解析](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/1%E3%80%81isa.md)
- [8.2、方法缓存](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/2%E3%80%81%E6%96%B9%E6%B3%95%E7%BC%93%E5%AD%98.md)
- [8.3、objc_msgSend执行流程](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/3%E3%80%81objc_msgSend%E6%89%A7%E8%A1%8C%E6%B5%81%E7%A8%8B.md)
- [8.4、@dynamic关键字](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/4%E3%80%81@dynamic%E5%85%B3%E9%94%AE%E5%AD%97.md)
- [8.5、Class和SuperClass区别](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/5%E3%80%81Class%26SuperClass.md)
- [8.6、isKindOfClass和isMemberOfClass区别](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/6%E3%80%81isMemberOfClass%26.isKindOfClassmd.md)
- [8.7、RunTime的相关API](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/RunTime/7%E3%80%81API.md)
### RunLoop
**1、讲讲 RunLoop，项目中有用到吗？**
1、定时器切换的时候，为了保证定时器的准确性，需要添加runLoop
2、在聊天界面，我们需要持续的把聊天信息存到数据库中，这个时候需要开启一个保活线程，在这个线程中处理
**2、runloop内部实现逻辑**
每次运行RunLoop，线程的RunLoop会自动处理之前未处理的消息，并通知相关的观察者。具体顺序
- 1、通知观察者（observers）RunLoop即将启动
- 2、通知观察者（observers）任何即将要开始的定时器
- 3、通知观察者（observers）即将处理source0事件
- 4、处理source0
- 5、如果有source1，跳到第9步
- 6、通知观察者（observers）线程即将进入休眠
- 7、将线程置于休眠知道任一下面的事件发生
	- 1、source0事件触发
- 2、定时器启动
- 3、外部手动唤醒
- 8、通知观察者（observers）线程即将唤醒
- 9、处理唤醒时收到的时间，之后跳回2
	- 1、如果用户定义的定时器启动，处理定时器事件
- 2、如果source0启动，传递相应的消息
- 10、通知观察者RunLoop结束
![](https://upload-images.jianshu.io/upload_images/2348494-1f4b26df7f6fefb8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/910/format/webp)
RunLoop7.png
**3、RunLoop与线程**
- 每条线程都有唯一的一个与之对应的RunLoop对象
- RunLoop保存在一个全局的Dictionary里，线程作为key，RunLoop作为value
- 线程刚创建时并没有RunLoop对象，RunLoop会在第一次获取它时创建
- RunLoop会在线程结束时销毁
- 主线程的RunLoop已经自动获取（创建），子线程默认没有开启RunLoop
**4、timer 与 runloop 的关系？**
- 一个RunLoop包含若干个Mode，每个Mode又包含若干个Source0/Source1/Timer/Observer
- RunLoop启动时只能选择其中一个Mode，作为currentMode
- 如果需要切换Mode，只能退出当前Loop，再重新选择一个Mode进入
- 不同组的Source0/Source1/Timer/Observer能分隔开来，互不影响
- 如果Mode里没有任何Source0/Source1/Timer/Observer，RunLoop会立马退出
解决定时器在滚动视图上面失效问题`NSTimer添加到两种RunLoop中`
```
[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
[[NSRunLoop currentRunLoop] addTimer:timer forMode:UITrackingRunLoopMode];
```
```
[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```
**5、RunLoop有几种状态**
```
kCFRunLoopEntry = (1UL << 0), // 即将进入RunLoop 
kCFRunLoopBeforeTimers = (1UL << 1), // 即将处理Timer 
kCFRunLoopBeforeSources = (1UL << 2), // 即将处理Source 
kCFRunLoopBeforeWaiting = (1UL << 5), //即将进入休眠 
kCFRunLoopAfterWaiting = (1UL << 6),// 刚从休眠中唤醒
 kCFRunLoopExit = (1UL << 7),// 即将退出RunLoop
```
**6、RunLoop的mode的作用 **
RunLoop的mode的作用 系统注册了5中mode
```
kCFRunLoopDefaultMode //App的默认Mode，通常主线程是在这个Mode下运行
UITrackingRunLoopMode //界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
UIInitializationRunLoopMode // 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
GSEventReceiveRunLoopMode // 接受系统事件的内部 Mode，通常用不到
kCFRunLoopCommonModes //这是一个占位用的Mode，不是一种真正的Mode
```
但是我们只能使用两种mode
```
kCFRunLoopDefaultMode //App的默认Mode，通常主线程是在这个Mode下运行
UITrackingRunLoopMode //界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
```
具体实现请参考：[7、RunLoop实现原理](https://github.com/SunshineBrother/JHBlog/blob/master/iOS%E7%9F%A5%E8%AF%86%E7%82%B9/iOS%E5%BA%95%E5%B1%82/8%E3%80%81RunLoop.md)
### 多线程
**1、你理解的多线程？**
**2、iOS的多线程方案有哪几种？你更倾向于哪一种？**
**3、你在项目中用过 GCD 吗？**
**4、GCD 的队列类型**
**5、说一下 OperationQueue 和 GCD 的区别，以及各自的优势**
**6、线程安全的处理手段有哪些？**
使用线程锁
- 1、OSSpinLock
- 2、os_unfair_lock
- 3、pthread_mutex
- 4、dispatch_semaphore
- 5、dispatch_queue(DISPATCH_QUEUE_SERIAL)
- 6、NSLock
- 7、NSRecursiveLock
- 8、NSCondition
- 9、NSConditionLock
- 10、@synchronized
- 11、pthread_rwlock
- 12、dispatch_barrier_async
- 13、atomic
**7、线程通讯**
线程间通信的体现
- 1、一个线程传递数据给另一个线程
- 2、在一个线程中执行完特定任务后，转到另一个线程继续执行任务
***1、NSThread***
可以先将自己的当前线程对象注册到某个全局的对象中去，这样相互之间就可以获取对方的线程对象，然后就可以使用下面的方法进行线程间的通信了，由于主线程比较特殊，所以框架直接提供了在主线程执行的方法
```
- (void)performSelectorOnMainThread:(SEL)aSelector withObject:(nullable id)arg waitUntilDone:(BOOL)wait;
- (void)performSelector:(SEL)aSelector onThread:(NSThread *)thr withObject:(nullable id)arg waitUntilDone:(BOOL)wait NS_AVAILABLE(10_5, 2_0);
```
***2、GCD***
```
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
      
 });
```
### 内存管理
**1、使用CADisplayLink、NSTimer有什么注意点？**
CADisplayLink、NSTimer会对target产生强引用，如果target又对它们产生强引用，那么就会引发循环引用
**2、介绍下内存的几大区域**
- 代码段：编译之后的代码
- 数据段
	- 字符串常量：比如NSString *str = @"123"
- 已初始化数据：已初始化的全局变量、静态变量等
- 未初始化数据：未初始化的全局变量、静态变量等
- 栈：函数调用开销，比如局部变量。分配的内存空间地址越来越小
- 堆：通过alloc、malloc、calloc等动态分配的空间，分配的内存空间地址越来越大
**3、讲一下你对 iOS 内存管理的理解**
在iOS中，使用引用计数来管理OC对象的内存
- 一个新创建的OC对象引用计数默认是1，当引用计数减为0，OC对象就会销毁，释放其占用的内存空间
- 调用retain会让OC对象的引用计数+1，调用release会让OC对象的引用计数-1
内存管理的经验总结
- 当调用alloc、new、copy、mutableCopy方法返回了一个对象，在不需要这个对象时，要调用release或者autorelease来释放它
- 想拥有某个对象，就让它的引用计数+1；不想再拥有某个对象，就让它的引用计数-1
可以通过以下私有函数来查看自动释放池的情况
`extern void _objc_autoreleasePoolPrint(void);`
**4、ARC 都帮我们做了什么**
LLVM + Runtime
- LVVM生成release代码
- RunTime负责执行
**5、weak指针的实现原理**
runtime维护了一个weak表，用于存储指向某个对象的所有weak指针。weak表其实是一个hash（哈希）表，key是所指对象的地址，Value是weak指针的地址（这个地址的值是所指对象指针的地址）数组
- 1、初始化时：runtime会调用objc_initWeak函数，初始化一个新的weak指针指向对象的地址
- 2、添加引用时：objc_initWeak函数会调用 storeWeak() 函数， storeWeak() 的作用是更新指针指向，创建对应的弱引用表
- 3、释放时,调用clearDeallocating函数。clearDeallocating函数首先根据对象地址获取所有weak指针地址的数组，然后遍历这个数组把其中的数据设为nil，最后把这个entry从weak表中删除，最后清理对象的记录
**6、autorelease对象在什么时机会被调用release**
- 1、iOS在主线程的Runloop中注册了2个Observer
- 2、第1个Observer监听了kCFRunLoopEntry事件，会调用objc_autoreleasePoolPush()
- 3、第2个Observer监听了kCFRunLoopBeforeWaiting事件，会调用objc_autoreleasePoolPop()、objc_autoreleasePoolPush() 监听了kCFRunLoopBeforeExit事件，会调用objc_autoreleasePoolPop()
`autoreleased 对象是在 runloop 的即将进入休眠时进行释放的`
**7、方法里有局部对象， 出了方法后会立即释放吗**
在ARC情况下会立即释放
在MRC情况下，对象是在 runloop 的即将进入休眠时进行释放的
**文章中可以提炼出来的题目太多了，我这里也就简单的总结几道题，想要了解具体实现请到我的[github](https://github.com/SunshineBrother/JHBlog)中找到相关文章进行阅读。欢迎点赞哦，如果里面有什么我理解的不太正确，欢迎提出，我们相互印证**
作者：辉哥de简书
链接：https://www.jianshu.com/p/a0bc8d6525b7
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
