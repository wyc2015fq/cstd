# iOS 如何实现Aspect Oriented Programming (下) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月27日 16:37:44[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：636
![](http://upload-images.jianshu.io/upload_images/1194012-362f68c0c95d15f4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
(接上篇)
#### 五. Aspects hook过程详解
先看看函数调用栈的情况
```
- aspect_prepareClassAndHookSelector(self, selector, error);
  ├── aspect_hookClass(self, error)
  │    ├──aspect_swizzleClassInPlace
  │    ├──aspect_swizzleForwardInvocation
  │    │  └──__ASPECTS_ARE_BEING_CALLED__
  │    │       ├──aspect_aliasForSelector
  │    │       ├──aspect_getContainerForClass
  │    │       ├──aspect_invoke
  │    │       └──aspect_remove
  │    └── aspect_hookedGetClass
  ├── aspect_isMsgForwardIMP
  ├──aspect_aliasForSelector(selector)
  └── aspect_getMsgForwardIMP
```
从调用栈可以看出，Aspects hook过程主要分4个阶段，hookClass，ASPECTS_ARE_BEING_CALLED，prepareClassAndHookSelector，remove。
![](http://upload-images.jianshu.io/upload_images/1194012-f5d728cf23df4d91.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 1. hookClass
```
NSCParameterAssert(self);
 Class statedClass = self.class;
 Class baseClass = object_getClass(self);
 NSString *className = NSStringFromClass(baseClass);
```
statedClass 和 baseClass是有区别的的。
```
Class object_getClass(id obj)
{
    if (obj) return obj->getIsa();
    else return Nil;
}
+ (Class)class {
    return self;
}
```
statedClass 是获取类对象，baseClass是获取到类的isa。
```
// Already subclassed
 if ([className hasSuffix:AspectsSubclassSuffix]) {
  return baseClass;
        // We swizzle a class object, not a single object.
 }else if (class_isMetaClass(baseClass)) {
        return aspect_swizzleClassInPlace((Class)self);
        // Probably a KVO'ed class. Swizzle in place. Also swizzle meta classes in place.
    }else if (statedClass != baseClass) {
        return aspect_swizzleClassInPlace(baseClass);
    }
```
先判断是用来className是否包含hasSuffix:AspectsSubclassSuffix
`static NSString *const AspectsSubclassSuffix = @"_Aspects_";`
如果包含了@"_Aspects_"后缀，代表该类已经被hook过了，直接return。
如果不包含@"_Aspects_"后缀，再判断是否是baseClass是否是元类，如果是元类，调用aspect_swizzleClassInPlace。如果也不是元类，再判断statedClass 和 baseClass是否相等，如果不相等，说明为KVO过的对象，因为KVO的对象isa指针会指向一个中间类。对KVO中间类调用aspect_swizzleClassInPlace。
```
static Class aspect_swizzleClassInPlace(Class klass) {
    NSCParameterAssert(klass);
    NSString *className = NSStringFromClass(klass);
    _aspect_modifySwizzledClasses(^(NSMutableSet *swizzledClasses) {
        if (![swizzledClasses containsObject:className]) {
            aspect_swizzleForwardInvocation(klass);
            [swizzledClasses addObject:className];
        }
    });
    return klass;
}
```
_aspect_modifySwizzledClasses会传入一个入参为(NSMutableSet *swizzledClasses)的block，block里面就是判断在这个Set里面是否包含当前的ClassName，如果不包含，就调用aspect_swizzleForwardInvocation()方法，并把className加入到Set集合里面。
```
static void _aspect_modifySwizzledClasses(void (^block)(NSMutableSet *swizzledClasses)) {
    static NSMutableSet *swizzledClasses;
    static dispatch_once_t pred;
    dispatch_once(&pred, ^{
        swizzledClasses = [NSMutableSet new];
    });
    @synchronized(swizzledClasses) {
        block(swizzledClasses);
    }
}
```
_aspect_modifySwizzledClasses方法里面保证了swizzledClasses这个Set集合是全局唯一的，并且给传入的block加上了线程锁@synchronized( )，保证了block调用中线程是安全的。
关于调用aspect_swizzleForwardInvocation，将原IMP指向forwardInvocation是下个阶段的事情，我们先把hookClass看完。
```
// Default case. Create dynamic subclass.
 const char *subclassName = [className stringByAppendingString:AspectsSubclassSuffix].UTF8String;
 Class subclass = objc_getClass(subclassName);
 if (subclass == nil) {
  subclass = objc_allocateClassPair(baseClass, subclassName, 0);
  if (subclass == nil) {
            NSString *errrorDesc = [NSString stringWithFormat:@"objc_allocateClassPair failed to allocate class %s.", subclassName];
            AspectError(AspectErrorFailedToAllocateClassPair, errrorDesc);
            return nil;
        }
  aspect_swizzleForwardInvocation(subclass);
  aspect_hookedGetClass(subclass, statedClass);
  aspect_hookedGetClass(object_getClass(subclass), statedClass);
  objc_registerClassPair(subclass);
 }
 object_setClass(self, subclass);
```
当className没有包含@"_Aspects_"后缀，并且也不是元类，也不是KVO的中间类，即statedClass = = baseClass 的情况，于是，默认的新建一个子类subclass。
到此，我们可以了解到Aspects的设计思想，**hook 是在runtime中动态创建子类的基础上实现的**。所有的 swizzling 操作都发生在子类，这样做的好处是你不需要去更改对象本身的类，也就是，当你在 remove aspects 的时候，如果发现当前对象的 aspect 都被移除了，那么，你可以将 isa 指针重新指回对象本身的类，从而消除了该对象的 swizzling ,同时也不会影响到其他该类的不同对象)这样对原来替换的类或者对象没有任何影响而且可以在子类基础上新增或者删除aspect。
新建的类的名字，会先加上AspectsSubclassSuffix后缀，即在className后面加上@"_Aspects_"，标记成子类。再调用objc_getClass方法，创建这个子类。
```
/***********************************************************************
* objc_getClass.  Return the id of the named class.  If the class does
* not exist, call _objc_classLoader and then objc_classHandler, either of 
* which may create a new class.
* Warning: doesn't work if aClassName is the name of a posed-for class's isa!
**********************************************************************/
Class objc_getClass(const char *aClassName)
{
    if (!aClassName) return Nil;
    // NO unconnected, YES class handler
    return look_up_class(aClassName, NO, YES);
}
```
objc_getClass会调用look_up_class方法。
```
/***********************************************************************
* look_up_class
* Look up a class by name, and realize it.
* Locking: acquires runtimeLock
**********************************************************************/
Class look_up_class(const char *name, 
              bool includeUnconnected __attribute__((unused)), 
              bool includeClassHandler __attribute__((unused)))
{
    if (!name) return nil;
    Class result;
    bool unrealized;
    {
        rwlock_reader_t lock(runtimeLock);
        result = getClass(name);
        unrealized = result  &&  !result->isRealized();
    }
    if (unrealized) {
        rwlock_writer_t lock(runtimeLock);
        realizeClass(result);
    }
    return result;
}
```
这个方法会去查看有没有实现叫name的class，查看过程中会用到rwlock_reader_t lock(runtimeLock)，读写锁，底层是用pthread_rwlock_t实现的。
由于是我们刚刚新建的一个子类名，很有可能是objc_getClass()返回nil。那么我们需要新建这个子类。调用objc_allocateClassPair()方法。
```
/***********************************************************************
* objc_allocateClassPair
* fixme
* Locking: acquires runtimeLock
**********************************************************************/
Class objc_allocateClassPair(Class superclass, const char *name, 
                             size_t extraBytes)
{
    Class cls, meta;
    rwlock_writer_t lock(runtimeLock);
    // Fail if the class name is in use.
    // Fail if the superclass isn't kosher.
    if (getClass(name)  ||  !verifySuperclass(superclass, true/*rootOK*/)) {
        return nil;
    }
    // Allocate new classes.
    cls  = alloc_class_for_subclass(superclass, extraBytes);
    meta = alloc_class_for_subclass(superclass, extraBytes);
    // fixme mangle the name if it looks swift-y?
    objc_initializeClassPair_internal(superclass, name, cls, meta);
    return cls;
}
```
调用objc_allocateClassPair会新建一个子类，它的父类是入参superclass。
如果新建的子类subclass = = nil，就会报错，objc_allocateClassPair failed to allocate class。
aspect_swizzleForwardInvocation(subclass)这是下一阶段的事情，主要作用是替换当前类forwardInvocation方法的实现为__ASPECTS_ARE_BEING_CALLED__，先略过。
接着调用aspect_hookedGetClass( ) 方法。
```
static void aspect_hookedGetClass(Class class, Class statedClass) {
    NSCParameterAssert(class);
    NSCParameterAssert(statedClass);
 Method method = class_getInstanceMethod(class, @selector(class));
 IMP newIMP = imp_implementationWithBlock(^(id self) {
  return statedClass;
 });
 class_replaceMethod(class, @selector(class), newIMP, method_getTypeEncoding(method));
}
```
aspect_hookedGetClass方法是把class的实例方法替换成返回statedClass，也就是说把调用class时候的isa指向了statedClass了。
```
aspect_hookedGetClass(subclass, statedClass);
  aspect_hookedGetClass(object_getClass(subclass), statedClass);
```
这两句的意图我们也就明白了。
第一句是把subclass的isa指向了statedClass，第二句是把subclass的元类的isa，也指向了statedClass。
最后调用objc_registerClassPair( ) 注册刚刚新建的子类subclass，再调用object_setClass(self, subclass);把当前self的isa指向子类subclass。
至此，hookClass阶段就完成了，成功的把self hook成了其子类 xxx_Aspects_。
![](http://upload-images.jianshu.io/upload_images/1194012-73519d92a9a5c214.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 2. ASPECTS_ARE_BEING_CALLED
在上一阶段hookClass的时候，有几处都调用了aspect_swizzleForwardInvocation方法。
```
static NSString *const AspectsForwardInvocationSelectorName = @"__aspects_forwardInvocation:";
static void aspect_swizzleForwardInvocation(Class klass) {
    NSCParameterAssert(klass);
    // If there is no method, replace will act like class_addMethod.
    IMP originalImplementation = class_replaceMethod(klass, @selector(forwardInvocation:), (IMP)__ASPECTS_ARE_BEING_CALLED__, "v@:@");
    if (originalImplementation) {
        class_addMethod(klass, NSSelectorFromString(AspectsForwardInvocationSelectorName), originalImplementation, "v@:@");
    }
    AspectLog(@"Aspects: %@ is now aspect aware.", NSStringFromClass(klass));
}
```
aspect_swizzleForwardInvocation就是整个Aspects hook方法的开始。
```
/***********************************************************************
* class_replaceMethod
**********************************************************************/
IMP class_replaceMethod(Class cls, SEL name, IMP imp, const char *types)
{
    if (!cls) return nil;
    return _class_addMethod(cls, name, imp, types, YES);
}
```
调用class_replaceMethod方法，实际底层实现是调用_class_addMethod方法。
```
static IMP _class_addMethod(Class cls, SEL name, IMP imp, 
                            const char *types, bool replace)
{
    old_method *m;
    IMP result = nil;
    if (!types) types = "";
    mutex_locker_t lock(methodListLock);
    if ((m = _findMethodInClass(cls, name))) {
        // already exists
        // fixme atomic
        result = method_getImplementation((Method)m);
        if (replace) {
            method_setImplementation((Method)m, imp);
        }
    } else {
        // fixme could be faster
        old_method_list *mlist = 
            (old_method_list *)calloc(sizeof(old_method_list), 1);
        mlist->obsolete = fixed_up_method_list;
        mlist->method_count = 1;
        mlist->method_list[0].method_name = name;
        mlist->method_list[0].method_types = strdup(types);
        if (!ignoreSelector(name)) {
            mlist->method_list[0].method_imp = imp;
        } else {
            mlist->method_list[0].method_imp = (IMP)&_objc_ignored_method;
        }
        _objc_insertMethods(cls, mlist, nil);
        if (!(cls->info & CLS_CONSTRUCTING)) {
            flush_caches(cls, NO);
        } else {
            // in-construction class has no subclasses
            flush_cache(cls);
        }
        result = nil;
    }
    return result;
}
```
从上述源码中，我们可以看到，先_findMethodInClass(cls, name)，从cls中查找有没有name的方法。如果有，并且能找到对应的IMP的话，就进行替换method_setImplementation((Method)m, imp)，把name方法的IMP替换成imp。这种方式_class_addMethod返回的是name方法对应的IMP，实际上就是我们替换完的imp。
如果在cls中没有找到name方法，那么就添加该方法，在mlist -> method_list[0] 的位置插入新的name方法，对应的IMP就是传入的imp。这种方式_class_addMethod返回的是nil。
回到aspect_swizzleForwardInvocation中，
```
IMP originalImplementation = class_replaceMethod(klass, @selector(forwardInvocation:), (IMP)__ASPECTS_ARE_BEING_CALLED__, "v@:@");
if (originalImplementation) {
   class_addMethod(klass, NSSelectorFromString(AspectsForwardInvocationSelectorName), originalImplementation, "v@:@");
}
```
把forwardInvocation:的IMP替换成__ASPECTS_ARE_BEING_CALLED__ 。如果在klass里面找不到forwardInvocation:方法，就会新添加该方法。
~~由于子类本身并没有实现 forwardInvocation ，隐藏返回的 originalImplementation 将为空值，所以也不会生成 NSSelectorFromString(AspectsForwardInvocationSelectorName) 。所以还需要_class_addMethod会为我们添加了forwardInvocation:方法的实现~~
谢谢简书的大神 @zhao0 指点，这个坑在Aspects 1.4.1中已经修复了。
在aspect_swizzleForwardInvocation中，class_replaceMethod返回的是原方法的IMP，originalImplementation不为空的话说明原方法有实现，添加一个新方法__aspects_forwardInvocation:指向了原来的originalImplementation，在__ASPECTS_ARE_BEING_CALLED__那里如果不能处理，判断是否有实现__aspects_forwardInvocation，有的话就转发。
~~如果originalImplementation返回的不是nil，就说明已经替换成功。替换完方法之后，我们在klass中再加入一个叫“__aspects_forwardInvocation:”的方法，对应的实现也是(IMP)__ASPECTS_ARE_BEING_CALLED__。~~
接下来就是整个Aspects的核心实现了：__ASPECTS_ARE_BEING_CALLED__
```
static void __ASPECTS_ARE_BEING_CALLED__(__unsafe_unretained NSObject *self, SEL selector, NSInvocation *invocation) {
    NSCParameterAssert(self);
    NSCParameterAssert(invocation);
    SEL originalSelector = invocation.selector;
    SEL aliasSelector = aspect_aliasForSelector(invocation.selector);
    invocation.selector = aliasSelector;
    AspectsContainer *objectContainer = objc_getAssociatedObject(self, aliasSelector);
    AspectsContainer *classContainer = aspect_getContainerForClass(object_getClass(self), aliasSelector);
    AspectInfo *info = [[AspectInfo alloc] initWithInstance:self invocation:invocation];
    NSLog(@"%@",info.arguments);
    NSArray *aspectsToRemove = nil;
    …… ……
}
```
这一段是hook前的准备工作：
- 获取原始的selector
- 获取带有aspects_xxxx前缀的方法
- 替换selector
- 获取实例对象的容器objectContainer，这里是之前aspect_add关联过的对象。
- 获取获得类对象容器classContainer
- 初始化AspectInfo，传入self、invocation参数
```
// Before hooks.
    aspect_invoke(classContainer.beforeAspects, info);
    aspect_invoke(objectContainer.beforeAspects, info);
```
调用宏定义执行Aspects切片功能
```
#define aspect_invoke(aspects, info) \
for (AspectIdentifier *aspect in aspects) {\
    [aspect invokeWithInfo:info];\
    if (aspect.options & AspectOptionAutomaticRemoval) { \
        aspectsToRemove = [aspectsToRemove?:@[] arrayByAddingObject:aspect]; \
    } \
}
```
之所以这里用一个宏定义来实现里面的功能，是为了获得一个更加清晰的堆栈信息。
宏定义里面就做了两件事情，一个是执行了[aspect invokeWithInfo:info]方法，一个是把需要remove的Aspects加入等待被移除的数组中。
[aspect invokeWithInfo:info]方法在上篇里面详细分析过了其实现，这个函数的主要目的是把blockSignature初始化blockSignature得到invocation。然后处理参数，如果参数block中的参数大于1个，则把传入的AspectInfo放入blockInvocation中。然后从originalInvocation中取出参数给blockInvocation赋值。最后调用[blockInvocation invokeWithTarget:self.block];这里Target设置为self.block。也就执行了我们hook方法的block。
所以只要调用aspect_invoke(classContainer.Aspects, info);这个核心替换的方法，就能hook我们原有的SEL。对应的，函数第一个参数分别传入的是classContainer.beforeAspects、classContainer.insteadAspects、classContainer.afterAspects就能对应的实现before、instead、after对应时间的Aspects切片的hook。
```
// Instead hooks.
    BOOL respondsToAlias = YES;
    if (objectContainer.insteadAspects.count || classContainer.insteadAspects.count) {
        aspect_invoke(classContainer.insteadAspects, info);
        aspect_invoke(objectContainer.insteadAspects, info);
    }else {
        Class klass = object_getClass(invocation.target);
        do {
            if ((respondsToAlias = [klass instancesRespondToSelector:aliasSelector])) {
                [invocation invoke];
                break;
            }
        }while (!respondsToAlias && (klass = class_getSuperclass(klass)));
    }
```
这一段代码是实现Instead hooks的。先判断当前insteadAspects是否有数据，如果没有数据则判断当前继承链是否能响应aspects_xxx方法,如果能，则直接调用aliasSelector。**注意：这里的aliasSelector是原方法method**
```
// After hooks.
    aspect_invoke(classContainer.afterAspects, info);
    aspect_invoke(objectContainer.afterAspects, info);
```
这两行是对应的执行After hooks的。原理如上。
至此，before、instead、after对应时间的Aspects切片的hook如果能被执行的，都执行完毕了。
如果hook没有被正常执行，那么就应该执行原来的方法。
```
// If no hooks are installed, call original implementation (usually to throw an exception)
    if (!respondsToAlias) {
        invocation.selector = originalSelector;
        SEL originalForwardInvocationSEL = NSSelectorFromString(AspectsForwardInvocationSelectorName);
        if ([self respondsToSelector:originalForwardInvocationSEL]) {
            ((void( *)(id, SEL, NSInvocation *))objc_msgSend)(self, originalForwardInvocationSEL, invocation);
        }else {
            [self doesNotRecognizeSelector:invocation.selector];
        }
    }
```
invocation.selector先换回原来的originalSelector，如果没有被hook成功，那么AspectsForwardInvocationSelectorName还能再拿到原来的IMP对应的SEL。如果能相应，就调用原来的SEL，否则就报出doesNotRecognizeSelector的错误。
`[aspectsToRemove makeObjectsPerformSelector:@selector(remove)];`
最后调用移除方法，移除hook。
![](http://upload-images.jianshu.io/upload_images/1194012-45f6e609d2f15988.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 3. prepareClassAndHookSelector
现在又要回到上篇中提到的aspect_prepareClassAndHookSelector方法中来了。
```
static void aspect_prepareClassAndHookSelector(NSObject *self, SEL selector, NSError **error) {
    NSCParameterAssert(selector);
    Class klass = aspect_hookClass(self, error);
    Method targetMethod = class_getInstanceMethod(klass, selector);
    IMP targetMethodIMP = method_getImplementation(targetMethod);
    if (!aspect_isMsgForwardIMP(targetMethodIMP)) {
        // Make a method alias for the existing method implementation, it not already copied.
        const char *typeEncoding = method_getTypeEncoding(targetMethod);
        SEL aliasSelector = aspect_aliasForSelector(selector);
        if (![klass instancesRespondToSelector:aliasSelector]) {
            __unused BOOL addedAlias = class_addMethod(klass, aliasSelector, method_getImplementation(targetMethod), typeEncoding);
            NSCAssert(addedAlias, @"Original implementation for %@ is already copied to %@ on %@", NSStringFromSelector(selector), NSStringFromSelector(aliasSelector), klass);
        }
        // We use forwardInvocation to hook in.
        class_replaceMethod(klass, selector, aspect_getMsgForwardIMP(self, selector), typeEncoding);
        AspectLog(@"Aspects: Installed hook for -[%@ %@].", klass, NSStringFromSelector(selector));
    }
}
```
klass是我们hook完原始的class之后得到的子类，名字是带有_Aspects_后缀的子类。因为它是当前类的子类，所以也可以从它这里获取到原有的selector的IMP。
```
static BOOL aspect_isMsgForwardIMP(IMP impl) {
    return impl == _objc_msgForward
#if !defined(__arm64__)
    || impl == (IMP)_objc_msgForward_stret
#endif
    ;
}
```
这里是判断当前IMP是不是_objc_msgForward或者_objc_msgForward_stret，即判断当前IMP是不是消息转发。
如果不是消息转发，就先获取当前原始的selector对应的IMP的方法编码typeEncoding。
如果子类里面不能响应aspects_xxxx，就为klass添加aspects_xxxx方法，方法的实现为原生方法的实现。
Aspects整个hook的入口就是这句话：
`class_replaceMethod(klass, selector, aspect_getMsgForwardIMP(self, selector), typeEncoding);`
由于我们将slector指向_objc_msgForward 和_objc_msgForward_stret，可想而知，当selector被执行的时候，也会触发消息转发从而进入forwardInvocation，而我们又对forwardInvacation进行了swizzling，因此，最终转入我们自己的处理逻辑代码中。
![](http://upload-images.jianshu.io/upload_images/1194012-70a37b1a25849cb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 4. aspect_remove
aspect_remove整个销毁过程的函数调用栈
```
- aspect_remove(AspectIdentifier *aspect, NSError **error)
  └── aspect_cleanupHookedClassAndSelector
      ├──aspect_deregisterTrackedSelector
      │   └── aspect_getSwizzledClassesDict
      ├──aspect_destroyContainerForObject
      └── aspect_undoSwizzleClassInPlace
          └── _aspect_modifySwizzledClasses
                └──aspect_undoSwizzleForwardInvocation
```
```
static BOOL aspect_remove(AspectIdentifier *aspect, NSError **error) {
    NSCAssert([aspect isKindOfClass:AspectIdentifier.class], @"Must have correct type.");
    __block BOOL success = NO;
    aspect_performLocked(^{
        id self = aspect.object; // strongify
        if (self) {
            AspectsContainer *aspectContainer = aspect_getContainerForObject(self, aspect.selector);
            success = [aspectContainer removeAspect:aspect];
            aspect_cleanupHookedClassAndSelector(self, aspect.selector);
            // destroy token
            aspect.object = nil;
            aspect.block = nil;
            aspect.selector = NULL;
        }else {
            NSString *errrorDesc = [NSString stringWithFormat:@"Unable to deregister hook. Object already deallocated: %@", aspect];
            AspectError(AspectErrorRemoveObjectAlreadyDeallocated, errrorDesc);
        }
    });
    return success;
}
```
aspect_remove 是整个 aspect_add的逆过程。
aspect_performLocked是保证线程安全。把AspectsContainer都置为空，remove最关键的过程就是aspect_cleanupHookedClassAndSelector(self, aspect.selector);移除之前hook的class和selector。
```
static void aspect_cleanupHookedClassAndSelector(NSObject *self, SEL selector) {
    NSCParameterAssert(self);
    NSCParameterAssert(selector);
    Class klass = object_getClass(self);
    BOOL isMetaClass = class_isMetaClass(klass);
    if (isMetaClass) {
        klass = (Class)self;
    }
    ……  ……
}
```
klass是现在的class，如果是元类，就转换成元类。
```
// Check if the method is marked as forwarded and undo that.
    Method targetMethod = class_getInstanceMethod(klass, selector);
    IMP targetMethodIMP = method_getImplementation(targetMethod);
    if (aspect_isMsgForwardIMP(targetMethodIMP)) {
        // Restore the original method implementation.
        const char *typeEncoding = method_getTypeEncoding(targetMethod);
        SEL aliasSelector = aspect_aliasForSelector(selector);
        Method originalMethod = class_getInstanceMethod(klass, aliasSelector);
        IMP originalIMP = method_getImplementation(originalMethod);
        NSCAssert(originalMethod, @"Original implementation for %@ not found %@ on %@", NSStringFromSelector(selector), NSStringFromSelector(aliasSelector), klass);
        class_replaceMethod(klass, selector, originalIMP, typeEncoding);
        AspectLog(@"Aspects: Removed hook for -[%@ %@].", klass, NSStringFromSelector(selector));
    }
```
先回复MsgForward消息转发函数，获得方法签名，然后把原始转发方法替换回我们hook过的方法。
这里有一个需要注意的问题。
如果当前Student有2个实例，stu1和stu2，并且他们都同时hook了相同的方法study( )，stu2在执行完aspect_remove，把stu2的study( )方法还原了。这里会把stu1的study( )方法也还原了。因为remove方法这个操作是对整个类的所有实例都生效的。
要想每个实例还原各自的方法，不影响其他实例，上述这段代码删除即可。因为在执行 remove 操作的时候，其实和这个对象相关的数据结构都已经被清除了，即使不去恢复 stu2 的study( ) 的执行，在进入 __ASPECTS_ARE_BEING_CALLED__，由于这个没有响应的 aspects ，其实会直接跳到原来的处理逻辑，并不会有其他附加影响。
```
static void aspect_deregisterTrackedSelector(id self, SEL selector) {
    if (!class_isMetaClass(object_getClass(self))) return;
    NSMutableDictionary *swizzledClassesDict = aspect_getSwizzledClassesDict();
    NSString *selectorName = NSStringFromSelector(selector);
    Class currentClass = [self class];
    AspectTracker *subclassTracker = nil;
    do {
        AspectTracker *tracker = swizzledClassesDict[currentClass];
        if (subclassTracker) {
            [tracker removeSubclassTracker:subclassTracker hookingSelectorName:selectorName];
        } else {
            [tracker.selectorNames removeObject:selectorName];
        }
        if (tracker.selectorNames.count == 0 && tracker.selectorNamesToSubclassTrackers) {
            [swizzledClassesDict removeObjectForKey:currentClass];
        }
        subclassTracker = tracker;
    }while ((currentClass = class_getSuperclass(currentClass)));
}
```
还要移除AspectTracker里面所有标记的swizzledClassesDict。销毁全部记录的selector。
```
AspectsContainer *container = aspect_getContainerForObject(self, selector);
    if (!container.hasAspects) {
        // Destroy the container
        aspect_destroyContainerForObject(self, selector);
        // Figure out how the class was modified to undo the changes.
        NSString *className = NSStringFromClass(klass);
        if ([className hasSuffix:AspectsSubclassSuffix]) {
            Class originalClass = NSClassFromString([className stringByReplacingOccurrencesOfString:AspectsSubclassSuffix withString:@""]);
            NSCAssert(originalClass != nil, @"Original class must exist");
            object_setClass(self, originalClass);
            AspectLog(@"Aspects: %@ has been restored.", NSStringFromClass(originalClass));
            // We can only dispose the class pair if we can ensure that no instances exist using our subclass.
            // Since we don't globally track this, we can't ensure this - but there's also not much overhead in keeping it around.
            //objc_disposeClassPair(object.class);
        }else {
            // Class is most likely swizzled in place. Undo that.
            if (isMetaClass) {
                aspect_undoSwizzleClassInPlace((Class)self);
            }else if (self.class != klass) {
                aspect_undoSwizzleClassInPlace(klass);
            }
        }
    }
```
最后，我们还需要还原类的AssociatedObject关联对象，以及用到的AspectsContainer容器。
```
static void aspect_destroyContainerForObject(id<NSObject> self, SEL selector) {
    NSCParameterAssert(self);
    SEL aliasSelector = aspect_aliasForSelector(selector);
    objc_setAssociatedObject(self, aliasSelector, nil, OBJC_ASSOCIATION_RETAIN);
}
```
这个方法销毁了AspectsContainer容器，并且把关联对象也置成了nil。
```
static void aspect_undoSwizzleClassInPlace(Class klass) {
    NSCParameterAssert(klass);
    NSString *className = NSStringFromClass(klass);
    _aspect_modifySwizzledClasses(^(NSMutableSet *swizzledClasses) {
        if ([swizzledClasses containsObject:className]) {
            aspect_undoSwizzleForwardInvocation(klass);
            [swizzledClasses removeObject:className];
        }
    });
}
```
aspect_undoSwizzleClassInPlace会再调用aspect_undoSwizzleForwardInvocation方法。
```
static void aspect_undoSwizzleForwardInvocation(Class klass) {
    NSCParameterAssert(klass);
    Method originalMethod = class_getInstanceMethod(klass, NSSelectorFromString(AspectsForwardInvocationSelectorName));
    Method objectMethod = class_getInstanceMethod(NSObject.class, @selector(forwardInvocation:));
    // There is no class_removeMethod, so the best we can do is to retore the original implementation, or use a dummy.
    IMP originalImplementation = method_getImplementation(originalMethod ?: objectMethod);
    class_replaceMethod(klass, @selector(forwardInvocation:), originalImplementation, "v@:@");
    AspectLog(@"Aspects: %@ has been restored.", NSStringFromClass(klass));
}
```
最后还原ForwardInvocation的Swizzling，把原来的ForwardInvocation再交换回来。
#### 六. 关于Aspects 的一些 “坑”
![](http://upload-images.jianshu.io/upload_images/1194012-321f597970d064a9.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在Aspects这个库了，用到了Method Swizzling有几处，这几处如果处理不好，就会掉“坑”里了。
##### 1.aspect_prepareClassAndHookSelector 中可能遇到的“坑”
在aspect_prepareClassAndHookSelector方法中，会把原始的selector hook成_objc_msgForward。但是如果这里的selector就是_objc_msgForward会发生什么呢？
其实这里的坑在作者的代码注释里面已经隐藏的提到了。
在__ASPECTS_ARE_BEING_CALLED__方法中，最后转发消息的那段代码里面有这样一段注释
`// If no hooks are installed, call original implementation (usually to throw an exception)`
看到这段注释以后，你肯定会思考，为何到了这里就会throw an exception呢？原因是因为找不到NSSelectorFromString(AspectsForwardInvocationSelectorName)对应的IMP。
再往上找，就可以找到原因了。在实现aspect_prepareClassAndHookSelector中，会判断当前的selector是不是_objc_msgForward，如果不是msgForward，接下来什么也不会做。那么aliasSelector是没有对应的实现的。
由于 forwardInvocation 被 aspects 所 hook ,最终会进入到 aspects 的处理逻辑__ASPECTS_ARE_BEING_CALLED__中来，此时如果没有找不到 aliasSelector 的 IMP 实现，因此会在此进行消息转发。而且子类并没有实现 NSSelectorFromString(AspectsForwardInvocationSelectorName)，于是转发就会抛出异常。
这里的“坑”就在于，hook的selector如果变成了_objc_msgForward，就会出现异常了，但是一般我们不会去hook _objc_msgForward这个方法，出现这个问题的原因是有其他的Swizzling会去hook这个方法。
比如说JSPatch把传入的 selector 先被 JSPatch hook ,那么，这里我们将不会再处理,也就不会生成 aliasSelector 。就会出现闪退的异常了。
```
static Class aspect_hookClass(NSObject *self, NSError **error) {
    ...
    subclass = objc_allocateClassPair(baseClass, subclassName, 0);
    ...
    IMP originalImplementation = class_replaceMethod(subclass, @selector(forwardInvocation:), (IMP)__ASPECTS_ARE_BEING_CALLED__, "v@:@");
    if (originalImplementation) {
        class_addMethod(subclass, NSSelectorFromString(AspectsForwardInvocationSelectorName),   originalImplementation, "v@:@");
    } else {
        Method baseTargetMethod = class_getInstanceMethod(baseClass, @selector(forwardInvocation:));
        IMP baseTargetMethodIMP = method_getImplementation(baseTargetMethod);
        if (baseTargetMethodIMP) {
            class_addMethod(subclass, NSSelectorFromString(AspectsForwardInvocationSelectorName), baseTargetMethodIMP, "v@:@");
        }
    }
    ...
}
```
这里在这篇[文章](http://wereadteam.github.io/2016/06/30/Aspects/)中给出了一个解决办法：
在对子类的 forwardInvocation方法进行交换而不仅仅是替换，实现逻辑如下，强制生成一个 NSSelectorFromString(AspectsForwardInvocationSelectorName)指向原对象的 forwardInvocation的实现。
注意如果 originalImplementation为空，那么生成的 NSSelectorFromString(AspectsForwardInvocationSelectorName)
将指向 baseClass 也就是真正的这个对象的 forwradInvocation ,这个其实也就是 JSPatch hook 的方法。同时为了保证 block 的执行顺序（也就是前面介绍的 before hooks / instead hooks / after hooks ），这里需要将这段代码提前到 after hooks 执行之前进行。这样就解决了 forwardInvocation 在外面已经被 hook 之后的冲突问题。
谢谢简书的大神 @zhao0 指点，在这篇文章详细分析了[Aspect和JSPatch各种兼容性问题](http://www.jianshu.com/p/dc1deaa1b28e)，经过详细的分析，最后只有4种不兼容的情况。
##### 2. aspect_hookSelector 可能出现的 “坑”
在Aspects中主要是hook selector，此时如果有多个地方会和Aspects去hook相同方法，那么也会出现doesNotRecognizeSelector的问题。
举个例子，比如说在NSArray中用Aspects 去hook了objectAtIndex的方法，然后在NSMutableArray中Swizzling了objectAtIndex方法。在
NSMutableArray中，调用objectAtIndex就有可能出错。
因为还是在于Aspects hook 了selector之后，会把原来的selector变成_objc_msgForward。等到NSMutableArray再去hook这个方法的时候，记录的是IMP就是_objc_msgForward这个了。如果这时objc_msgSend执行原有实现，就会出错了。因为原有实现已经被替换为_objc_msgForward，而真的IMP由于被Aspects先Swizzling掉了，所以找不到。
解决办法还是类似JSPatch的解决办法：
把-forwardInvocation:也进行Swizzling，在自己的-forwardInvocation:方法中进行同样的操作，就是判断传入的NSInvocation的Selector，被Swizzling的方法指向了_objc_msgForward（或_objc_msgForward_stret）如果是自己可以识别的Selector，那么就将Selector变为原有Selector在执行，如果不识别，就直接转发。
#### 最后
最后用一张图总结一下Aspects整体流程：
![](http://upload-images.jianshu.io/upload_images/1194012-a778dc8827d370fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
请大家多多指教。
文／一缕殇流化隐半边冰霜（简书作者）
原文链接：http://www.jianshu.com/p/2ad7e90b521b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
