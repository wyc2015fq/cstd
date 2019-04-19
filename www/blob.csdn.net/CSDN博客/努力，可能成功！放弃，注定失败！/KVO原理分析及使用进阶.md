# KVO原理分析及使用进阶 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年03月28日 10:06:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：158

##### 该文章属于<简书 — 刘小壮>原创，转载请注明：
[<简书 — 刘小壮> https://www.jianshu.com/p/badf5cac0130](https://www.jianshu.com/p/badf5cac0130)
> 
我们在工作中经常会用到`KVO`，但是系统原生的`KVO`并不好用，很容易导致`Crash`。而且编写代码时，需要编写大量`KVO`相关的代码，由于不支持`block`的形式，代码会写的很分散。
本篇文章对`KVO`的实现原理进行了详细的分析，并且简单的实现了一个`KVO`，来当做技术交流。由于系统提供的`KVO`存在很多问题，在文章的最下面给出了解决方案。
![](https://upload-images.jianshu.io/upload_images/270478-ab196e24bf11be78.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
博客配图
### 概述
`KVO`全称`KeyValueObserving`，是苹果提供的一套事件通知机制。允许对象监听另一个对象特定属性的改变，并在改变时接收到事件。由于`KVO`的实现机制，所以对属性才会发生作用，一般继承自`NSObject`的对象都默认支持`KVO`。
`KVO`和`NSNotificationCenter`都是`iOS`中观察者模式的一种实现。区别在于，相对于被观察者和观察者之间的关系，`KVO`是一对一的，而一对多的。`KVO`对被监听对象无侵入性，不需要修改其内部代码即可实现监听。
`KVO`可以监听单个属性的变化，也可以监听集合对象的变化。通过`KVC`的`mutableArrayValueForKey:`等方法获得代理对象，当代理对象的内部对象发生改变时，会回调`KVO`监听的方法。集合对象包含`NSArray`和`NSSet`。
### 基础使用
使用`KVO`分为三个步骤：
- 通过`addObserver:forKeyPath:options:context:`方法注册观察者，观察者可以接收`keyPath`属性的变化事件。
- 在观察者中实现`observeValueForKeyPath:ofObject:change:context:`方法，当`keyPath`属性发生改变后，`KVO`会回调这个方法来通知观察者。
- 当观察者不需要监听时，可以调用`removeObserver:forKeyPath:`方法将`KVO`移除。需要注意的是，调用`removeObserver`需要在观察者消失之前，否则会导致`Crash`。
#### 注册方法
在注册观察者时，可以传入`options`参数，参数是一个枚举类型。如果传入`NSKeyValueObservingOptionNew`和`NSKeyValueObservingOptionOld`表示接收新值和旧值，默认为只接收新值。如果想在注册观察者后，立即接收一次回调，则可以加入`NSKeyValueObservingOptionInitial`枚举。
还可以通过方法`context`传入任意类型的对象，在接收消息回调的代码中可以接收到这个对象，是KVO中的一种传值方式。
在调用`addObserver`方法后，`KVO`并不会对观察者进行强引用，所以需要注意观察者的生命周期，否则会导致观察者被释放带来的`Crash`。
#### 监听方法
观察者需要实现`observeValueForKeyPath:ofObject:change:context:`方法，当`KVO`事件到来时会调用这个方法，如果没有实现会导致`Crash`。`change`字典中存放`KVO`属性相关的值，根据`options`时传入的枚举来返回。枚举会对应相应`key`来从字典中取出值，例如有`NSKeyValueChangeOldKey`字段，存储改变之前的旧值。
`change`中还有`NSKeyValueChangeKindKey`字段，和`NSKeyValueChangeOldKey`是平级的关系，来提供本次更改的信息，对应`NSKeyValueChange`枚举类型的`value`。例如被观察属性发生改变时，字段为`NSKeyValueChangeSetting`。
如果被观察对象是集合对象，在`NSKeyValueChangeKindKey`字段中会包含`NSKeyValueChangeInsertion`、`NSKeyValueChangeRemoval`、`NSKeyValueChangeReplacement`的信息，表示集合对象的操作方式。
#### 兼容的调用方式
调用`KVO`属性对象时，不仅可以通过点语法和`set`语法进行调用，`KVO`兼容很多种调用方式。
```php
```php
// 直接调用set方法，或者通过属性的点语法间接调用
[account setName:@"Savings"];
 
// 使用KVC的setValue:forKey:方法
[account setValue:@"Savings" forKey:@"name"];
 
// 使用KVC的setValue:forKeyPath:方法
[document setValue:@"Savings" forKeyPath:@"account.name"];
// 通过mutableArrayValueForKey:方法获取到代理对象，并使用代理对象进行操作
Transaction *newTransaction = <#Create a new transaction for the account#>;
NSMutableArray *transactions = [account mutableArrayValueForKey:@"transactions"];
[transactions addObject:newTransaction];
```
```
#### 实际应用
`KVO`主要用来做键值观察操作，想要一个值发生改变后通知另一个对象，则用`KVO`实现最为合适。斯坦福大学的`iOS`教程中有一个很经典的案例，通过`KVO`在`Model`和`Controller`之间进行通信。
![](https://upload-images.jianshu.io/upload_images/270478-9fa25531de8275b5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
斯坦福大学 KVO示例
### 注意点
`KVO`的`addObserver`和`removeObserver`需要是成对的，如果重复`remove`则会导致`NSRangeException`类型的`Crash`，如果忘记`remove`则会在观察者释放后再次接收到`KVO`回调时`Crash`。
苹果官方推荐的方式是，在`init`的时候进行`addObserver`，在`dealloc`时`removeObserver`，这样可以保证`add`和`remove`是成对出现的，是一种比较理想的使用方式。
### 手动调用KVO
`KVO`在属性发生改变时的调用是自动的，如果想要手动控制这个调用时机，或想自己实现`KVO`属性的调用，则可以通过`KVO`提供的方法进行调用。
```
- (void)setBalance:(double)theBalance {
    if (theBalance != _balance) {
        [self willChangeValueForKey:@"balance"];
        _balance = theBalance;
        [self didChangeValueForKey:@"balance"];
    }
}
```
可以看到调用`KVO`主要依靠两个方法，在属性发生改变之前调用`willChangeValueForKey:`方法，在发生改变之后调用`didChangeValueForKey:`方法。
如果想控制当前对象的自动调用过程，也就是由上面两个方法发起的`KVO`调用，则可以重写下面方法。方法返回`YES`则表示可以调用，如果返回`NO`则表示不可以调用。
```
+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)theKey {
    BOOL automatic = NO;
    if ([theKey isEqualToString:@"balance"]) {
        automatic = NO;
    }
    else {
        automatic = [super automaticallyNotifiesObserversForKey:theKey];
    }
    return automatic;
}
```
### 实现原理
`KVO`是通过`isa-swizzling`技术实现的(这句话是整个`KVO`实现的重点)。在运行时根据原类创建一个中间类，这个中间类是原类的子类，并动态修改当前对象的`isa`指向中间类。并且将`class`方法重写，返回原类的`Class`。所以苹果建议在开发中不应该依赖`isa`指针，而是通过`class`实例方法来获取对象类型。
#### 测试代码
为了测试`KVO`的实现方式，我们加入下面的测试代码。首先创建一个`KVOObject`类，并在里面加入两个属性，然后重写`description`方法，并在内部打印一些关键参数。
```
@interface KVOObject : NSObject
@property (nonatomic, copy  ) NSString *name;
@property (nonatomic, assign) NSInteger age;
@end
@implementation KVOObject
- (NSString *)description {
    NSLog(@"object address : %p \n", self);
    
    IMP nameIMP = class_getMethodImplementation(object_getClass(self), @selector(setName:));
    IMP ageIMP = class_getMethodImplementation(object_getClass(self), @selector(setAge:));
    NSLog(@"object setName: IMP %p object setAge: IMP %p \n", nameIMP, ageIMP);
    
    Class objectMethodClass = [self class];
    Class objectRuntimeClass = object_getClass(self);
    Class superClass = class_getSuperclass(objectRuntimeClass);
    NSLog(@"objectMethodClass : %@, ObjectRuntimeClass : %@, superClass : %@ \n", objectMethodClass, objectRuntimeClass, superClass);
    
    NSLog(@"object method list \n");
    unsigned int count;
    Method *methodList = class_copyMethodList(objectRuntimeClass, &count);
    for (NSInteger i = 0; i < count; i++) {
        Method method = methodList[i];
        NSString *methodName = NSStringFromSelector(method_getName(method));
        NSLog(@"method Name = %@\n", methodName);
    }
    
    return @"";
}
```
在另一个类中分别创建两个`KVOObject`对象，其中一个对象被观察者通过`KVO`的方式监听，另一个对象则始终没有被监听。在`KVO`前后分别打印两个对象的关键信息，看`KVO`前后有什么变化。
```
@property (nonatomic, strong) KVOObject *object1;
@property (nonatomic, strong) KVOObject *object2;
self.object1 = [[KVOObject alloc] init];
self.object2 = [[KVOObject alloc] init];
[self.object1 description];
[self.object2 description];
[self.object1 addObserver:self forKeyPath:@"name" options:NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld context:nil];
[self.object1 addObserver:self forKeyPath:@"age" options:NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld context:nil];
[self.object1 description];
[self.object2 description];
self.object1.name = @"lxz";
self.object1.age = 20;
```
下面是`KVO`前后打印的关键信息，我们在下面做详细分析。
```
// 第一次
object address : 0x604000239340
object setName: IMP 0x10ddc2770 object setAge: IMP 0x10ddc27d0
objectMethodClass : KVOObject, ObjectRuntimeClass : KVOObject, superClass : NSObject
object method list
method Name = .cxx_destruct
method Name = description
method Name = name
method Name = setName:
method Name = setAge:
method Name = age
object address : 0x604000237920
object setName: IMP 0x10ddc2770 object setAge: IMP 0x10ddc27d0
objectMethodClass : KVOObject, ObjectRuntimeClass : KVOObject, superClass : NSObject
object method list
method Name = .cxx_destruct
method Name = description
method Name = name
method Name = setName:
method Name = setAge:
method Name = age
// 第二次
object address : 0x604000239340
object setName: IMP 0x10ea8defe object setAge: IMP 0x10ea94106
objectMethodClass : KVOObject, ObjectRuntimeClass : NSKVONotifying_KVOObject, superClass : KVOObject
object method list
method Name = setAge:
method Name = setName:
method Name = class
method Name = dealloc
method Name = _isKVOA
object address : 0x604000237920
object setName: IMP 0x10ddc2770 object setAge: IMP 0x10ddc27d0
objectMethodClass : KVOObject, ObjectRuntimeClass : KVOObject, superClass : NSObject
object method list
method Name = .cxx_destruct
method Name = description
method Name = name
method Name = setName:
method Name = setAge:
method Name = age
```
我们发现对象被`KVO`后，其真正类型变为了`NSKVONotifying_KVOObject`类，已经不是之前的类了。`KVO`会在运行时动态创建一个新类，将对象的`isa`指向新创建的类，新类是原类的子类，命名规则是`NSKVONotifying_xxx`的格式。`KVO`为了使其更像之前的类，还会将对象的`class`实例方法重写，使其更像原类。
在上面的代码中还发现了`_isKVOA`方法，这个方法可以当做使用了`KVO`的一个标记，系统可能也是这么用的。如果我们想判断当前类是否是`KVO`动态生成的类，就可以从方法列表中搜索这个方法。
#### 重写setter方法
![](https://upload-images.jianshu.io/upload_images/270478-9e478d93384890c2.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
KVO调用栈
`KVO`会重写`keyPath`对应属性的`setter`方法，没有被`KVO`的属性则不会重写其`setter`方法。在重写的`setter`方法中，修改值之前会调用`willChangeValueForKey:`方法，修改值之后会调用`didChangeValueForKey:`方法，这两个方法最终都会被调用到`observeValueForKeyPath:ofObject:change:context:`方法中。
#### object_getClass
为什么上面调用`runtime`的`object_getClass`函数，就可以获取到真正的类呢？
调用`object_getClass`函数后其返回的是一个`Class`类型，`Class`是`objc_class`定义的一个`typedef`别名，通过`objc_class`就可以获取到对象的`isa`指针指向的`Class`，也就是对象的类对象。
由此可以推测，`object_getClass`函数内部返回的是对象的`isa`指针。
```cpp
```cpp
typedef struct objc_class *Class;
struct objc_class {
    Class _Nonnull isa  OBJC_ISA_AVAILABILITY;
#if !__OBJC2__
    Class _Nullable super_class                              OBJC2_UNAVAILABLE;
    const char * _Nonnull name                               OBJC2_UNAVAILABLE;
    long version                                             OBJC2_UNAVAILABLE;
    long info                                                OBJC2_UNAVAILABLE;
    long instance_size                                       OBJC2_UNAVAILABLE;
    struct objc_ivar_list * _Nullable ivars                  OBJC2_UNAVAILABLE;
    struct objc_method_list * _Nullable * _Nullable methodLists                    OBJC2_UNAVAILABLE;
    struct objc_cache * _Nonnull cache                       OBJC2_UNAVAILABLE;
    struct objc_protocol_list * _Nullable protocols          OBJC2_UNAVAILABLE;
#endif
}
```
```
### 缺点
苹果提供的`KVO`自身存在很多问题，首要问题在于，`KVO`如果使用不当很容易崩溃。例如重复`add`和`remove`导致的`Crash`，`Observer`被释放导致的崩溃，`keyPath`传错导致的崩溃等。
在调用`KVO`时需要传入一个`keyPath`，由于`keyPath`是字符串的形式，所以其对应的属性发生改变后，字符串没有改变容易导致`Crash`。我们可以利用系统的反射机制将`keyPath`反射出来，这样编译器可以在`@selector()`中进行合法性检查。
```
NSStringFromSelector(@selector(isFinished))
```
`KVO`是一种事件绑定机制的实现，在`keyPath`对应的值发生改变后会回调对应的方法。这种数据绑定机制，在对象关系很复杂的情况下，很容易导致不好排查的`bug`。例如`keyPath`对应的属性被调用的关系很复杂，就不太建议对这个属性进行`KVO`，可以想一下`RAC`的信号脑补一下。
### 自己实现KVO
除了上面的缺点，`KVO`还不支持`block`语法，需要单独重写父类方法，这样加上`add`和`remove`方法就会导致代码很分散。所以，我通过`runtime`简单的实现了一个`KVO`，源码放在我的`Github`上，叫做[EasyKVO](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FDeveloperErenLiu%2FEasyKVO)。
```
self.object1 = [[KVOObject alloc] init];
[self.object1 lxz_addObserver:self originalSelector:@selector(name) callback:^(id observedObject, NSString *observedKey, id oldValue, id newValue) {
    // callback
}];
self.object1.name = @"lxz";
[self.object1 lxz_removeObserver:self originalSelector:@selector(name)];
```
调用代码很简单，直接通过`lxz_addObserver:originalSelector:callback:`方法就可以添加`KVO`的监听，可以通过`callback`的`block`接收属性发生改变后的回调，而且方法的`keyPath`接收的是一个`SEL`类型参数，所以可以通过`@selector()`传入参数时进行方法合法性检查，如果是未实现的方法直接就会报警告。
通过`lxz_removeObserver:originalSelector:`方法传入观察者和`keyPath`，当观察者所有`keyPath`都移除后则从`KVO`中移除观察者对象。
如果重复`addObserver`和`removeObserver`也没事，内部有判断逻辑。`EasyKVO`内部通过`weak`对观察者做引用，并不会影响观察者的生命周期，并且在观察者释放后不会导致`Crash`。一次`add`方法调用对应一个`block`，如果观察者监听多个`keyPath`属性，不需要在`block`回调中判断`keyPath`。
##### 注意
需要注意的是，`EasyKVO`只是做技术交流，不建议在项目中使用。因为`KVO`实现需要考虑很多情况，继承关系、多个观察者等很多问题。
### KVOController
想在项目中安全便捷的使用`KVO`的话，推荐`Facebook`的一个`KVO`开源第三方框架-[KVOController](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffacebook%2FKVOController)。`KVOController`本质上是对系统`KVO`的封装，具有原生`KVO`所有的功能，而且规避了原生`KVO`的很多问题，兼容`block`和`action`两种回调方式。
#### 源码分析
从源码来看还是比较简单的，主要分为`NSObject`的`Category`和`FBKVOController`两部分。
![](https://upload-images.jianshu.io/upload_images/270478-76163654e4154097.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/528)
FBKVOController
在`Category`中提供了`KVOController`和`KVOControllerNonRetaining`两个属性，顾名思义第一个会对`observer`产生强引用，第二个则不会。其内部代码就是创建`FBKVOController`对象的代码，并将创建出来的对象赋值给`Category`的属性，直接通过这个`Category`就可以懒加载创建`FBKVOController`对象。
```
- (FBKVOController *)KVOControllerNonRetaining
{
  id controller = objc_getAssociatedObject(self, NSObjectKVOControllerNonRetainingKey);
  
  if (nil == controller) {
    controller = [[FBKVOController alloc] initWithObserver:self retainObserved:NO];
    self.KVOControllerNonRetaining = controller;
  }
  
  return controller;
}
```
#### FBKVOController部分
在`FBKVOController`中分为三部分，`_FBKVOInfo`是一个私有类，这个类的功能很简单，就是以结构化的形式保存`FBKVOController`所需的各个对象，类似于模型类的功能。
还有一个私有类`_FBKVOSharedController`，这是`FBKVOController`框架实现的关键。从命名上可以看出其是一个单例，所有通过`FBKVOController`实现的`KVO`，观察者都是它。每次通过`FBKVOController`添加一个`KVO`时，`_FBKVOSharedController`都会将自己设为观察者，并在其内部实现`observeValueForKeyPath:ofObject:change:context:`方法，将接收到的消息通过`block`或`action`进行转发。
其功能很简单，通过`observe:info:`方法添加`KVO`监听，并用一个`NSHashTable`保存`_FBKVOInfo`信息。通过`unobserve:info:`方法移除监听，并从`NSHashTable`中将对应的`_FBKVOInfo`移除。这两个方法内部都会调用系统的`KVO`方法。
在外界使用时需要用`FBKVOController`类，其内部实现了初始化以及添加和移除监听的操作。在调用添加监听方法后，其内部会创建一个`_FBKVOInfo`对象，并通过一个`NSMapTable`对象进行持有，然后会调用`_FBKVOSharedController`来进行注册监听。
使用`FBKVOController`的话，不需要手动调用`removeObserver`方法，在被监听对象消失的时候，会在`dealloc`中调用`remove`方法。如果因为业务需求，可以手动调用`remove`方法，重复调用`remove`方法不会有问题。
```
- (void)_observe:(id)object info:(_FBKVOInfo *)info
{
    NSMutableSet *infos = [_objectInfosMap objectForKey:object];
    _FBKVOInfo *existingInfo = [infos member:info];
    if (nil != existingInfo) {
      return;
    }
    if (nil == infos) {
      infos = [NSMutableSet set];
      [_objectInfosMap setObject:infos forKey:object];
    }
    [infos addObject:info];
    [[_FBKVOSharedController sharedController] observe:object info:info];
}
```
因为`FBKVOController`的实现很简单，所以这里就很简单的讲讲，具体实现可以去[Github](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffacebook%2FKVOController)下载源码仔细分析一下。

作者：刘小壮
链接：https://www.jianshu.com/p/badf5cac0130
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。            
