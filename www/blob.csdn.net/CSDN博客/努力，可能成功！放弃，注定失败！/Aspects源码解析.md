# Aspects源码解析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月13日 11:00:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：899
> 
对于面向方面的编程一个愉快的，简单的库
- 
关键字`面向切片编程`：`OC动态性`，`消息转发`，`类型编码`， ，`Swizzle`...
- 
使用场景：
- 1.统一处理逻辑
- 2.在不改变源码的情况下，插入代码（如无侵染更改第三方库代码，干一些坏坏的事情）
`Aspects`只有一个类文件，非常轻量级，实现在思路的上状语从句：`JSPatch`差不多。都主要用到`OC的消息转发`，都名单最终交给`ForwardInvocation`实现。二者很多地方有异曲同工之妙。
## 基本原理
知道我们`OC`的英文动态语言，我们执行一个函数的时候，其实是在发一条消息：`[receiver message]`，过程这个就是根据`message`生成`selector`，根据然后`selector`电子邮件寻找指向函数具体实现的指针`IMP`，然后找到真正的函数执行逻辑这种处理流程给我们提供了动态性的可能，试想一下，**如果在运行时，动态的改变了选择和IMP的对应关系，那么就能使得原来的[接收消息]进入到新的函数实现了。**
还是先来普及一下：
`OC`上，每个类都是这样一个结构体：
```
struct objc_class {
  struct objc_class * isa;
  const char *name;
  ….
  struct objc_method_list **methodLists; /*方法链表*/
};
```
其中`methodList`方法链表里存储的的英文`Method`类型：
```
typedef struct objc_method *Method;
typedef struct objc_ method {
  SEL method_name;
  char *method_types;
  IMP method_imp;
};
```
`Method`保存了一个方法的全部信息，包括`SEL`方法名，`type`各参数和返回值类型，`IMP`该方法具体实现的函数指针。
通过`Selector`调用方法时，从会`methodList`链表里找到对应`Method`进行调用，这个`methodList`上的的元素的英文可以动态替换的，把可以某个`Selector`对应的函数指针`IMP`替换新分类中翻译的，可以也。拿到已有的某个`Selector`对应的函数指针`IMP`，另一个让`Selector`跟它对应，`Runtime`提供了一些接口做这些事。
比如：
```
static void viewDidLoadIMP (id slf, SEL sel) {
 // Custom Code
}
Class cls = NSClassFromString(@"UIViewController");
SEL selector = @selector(viewDidLoad);
Method method = class_getInstanceMethod(cls, selector);
//获得viewDidLoad方法的函数指针
IMP imp = method_getImplementation(method)
//获得viewDidLoad方法的参数类型
char *typeDescription = (char *)method_getTypeEncoding(method);
//新增一个ORIGViewDidLoad方法，指向原来的viewDidLoad实现
class_addMethod(cls, @selector(ORIGViewDidLoad), imp, typeDescription);
//把viewDidLoad IMP指向自定义新的实现
class_replaceMethod(cls, selector, viewDidLoadIMP, typeDescription);
```
就把这样`UIViewController`的`-viewDidLoad`方法给替换分类中翻译我们自定义的方法，`APP`里调用`UIViewController`的`viewDidLoad`方法都会去到上述`viewDidLoadIMP`函数里，这个在新的`IMP`函数里调用新增的方法，实现就替换了`viewDidLoad`方法，为同时`UIViewController`新增了个方法`-ORIGViewDidLoad`原来指向`viewDidLoad`的`IMP`，可以通过这个方法调用到原来的实现。
。**一方面要的是实现一个通用的IMP，任意方法任意参数都可以通过这个进出口中转。**上面讲的都是针对某一个方法的替换，但如果这个方法有参数，把怎样参数值传给我们新的`IMP`函数呢？例如`UIViewController`的`-viewDidAppear:`方法，者调用会传一个`Bool`值，我们需要在自己实现的`IMP`（上述的`viewDidLoadIMP`）上拿到这个值，怎样能拿到？如果只是针对一个方法写`IM`P，是可以直接拿到这个参数值的。如何达到通用的效果呢？
### 如何实现方法替换
- va_list的实现（一次取出方法的参数）
这段代码摘至`JSPatch`：
```
static void commonIMP(id slf, ...)
  va_list args;
  va_start(args, slf);
  NSMutableArray *list = [[NSMutableArray alloc] init];
  NSMethodSignature *methodSignature = [cls  instanceMethodSignatureForSelector:selector];
  NSUInteger numberOfArguments = methodSignature.numberOfArguments;
  id obj;
  for (NSUInteger i = 2; i < numberOfArguments; i++) {
      const char *argumentType = [methodSignature getArgumentTypeAtIndex:i];
      switch(argumentType[0]) {
          case 'i':
              obj = @(va_arg(args, int));
              break;
          case 'B':
              obj = @(va_arg(args, BOOL));
              break;
          case 'f':
          case 'd':
              obj = @(va_arg(args, double));
              break;
          …… //其他数值类型
          default: {
              obj = va_arg(args, id);
              break;
          }
      }
      [list addObject:obj];
  }
  va_end(args);
  [function callWithArguments:list];
}
```
这样无论方法参数是什么，有多少个，都可以通过`va_list`的一组方法一个个取出来，组成`NSArray`。很完美地解决了参数的问题，一直运行正常，但是在`arm64`下`va_list`的结构改变了，导致无法上述这样取参数。
所以需要找到另一种方法。
- 
ForwardInvocation实现
- 看图说话
![](http://upload-images.jianshu.io/upload_images/664334-fa53043130a7d5fd?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
从上面我们可以发现，在发消息的时候，如果`selector`有对应的`IMP`，则直接执行，如果没有，`oc`给我们提供了几个可供补救的机会，依次有`resolveInstanceMethod`，`forwardingTargetForSelector`，`forwardInvocation`。
`Aspects`选择之所以在`forwardInvocation`这里处理的英文因为，这几个阶段特性都不太一样：
- `resolvedInstanceMethod`：适合给类/对象动态添加一个相应的实现，
- `forwardingTargetForSelector`：适合将消息转发给其他对象处理，
- `forwardInvocation`：是里面最灵活，最能符合需求的。
**因此`Aspects`的方案就是，待对于`hook`的`selector`，将其指向`objc_msgForward / _objc_msgForward_stret`，生成同时新一个的`aliasSelector`指向原来的IMP，并且`hook`住`forwardInvocation`函数，通过`forwardInvocation`调用到原来的进出口。**
> 
核心原理：按照上面的思路，被当`hook`的`selector`被执行的时候，根据首先`selector`找到了`objc_msgForward / _objc_msgForward_stret`，而这个会触发消息转发，从而进入`forwardInvocation`。由于同时`forwardInvocation`的指向也。被修改了，因此转入会新的`forwardInvocation`函数，在里面执行需要嵌入的附加代码，完成之后，再转回原来的`IMP`。
大致流程如下：
![](https://camo.githubusercontent.com/48cbbd8ee1c8af0ef8f18a2e0ab0d50a085afab1/687474703a2f2f626c6f672e636e62616e672e6e65742f77702d636f6e74656e742f75706c6f6164732f323031352f30362f4a535061746368322e706e67)
摘至
`-forwardInvocation:`方法的实现给替换掉了，如果程序里真有用到这个方法对消息进行转发，原来的逻辑怎么办？首先我们在替换`-forwardInvocation:`方法前会新建一个方法`-ORIGforwardInvocation:`，保存原来的实现IMP，在新的`-forwardInvocation:`实现里做了个判断，如果转发的方法是我们想改写的，就走我们的逻辑，若不是，调就`-ORIGforwardInvocation:`走原来的流程。
将了这么多可能有些饶。`Talk is sheap,show me the code`
## 源码分析
从头文件中可以看到使用方面有两种使用方式：
- 1.类方法 
- 2.实例方法
```
/// Adds a block of code before/instead/after the current `selector` for a specific class.
///
/// @param block Aspects replicates the type signature of the method being hooked.
/// The first parameter will be `id<AspectInfo>`, followed by all parameters of the method.
/// These parameters are optional and will be filled to match the block signature.
/// You can even use an empty block, or one that simple gets `id<AspectInfo>`.
///
/// @note Hooking static methods is not supported.
/// @return A token which allows to later deregister the aspect.
+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;
/// Adds a block of code before/instead/after the current `selector` for a specific instance.
- (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;
```
两者的主要原理基本差不多。
**先来看看有哪些定义：**
### AspectOptions
```cpp
```cpp
typedef NS_OPTIONS(NSUInteger, AspectOptions) {
    AspectPositionAfter   = 0,            /// Called after the original implementation (default)
    AspectPositionInstead = 1,            /// Will replace the original implementation.
    AspectPositionBefore  = 2,            /// Called before the original implementation.
    AspectOptionAutomaticRemoval = 1 << 3 /// Will remove the hook after the first execution.
};
```
```
定义切片的调用时机
### AspectErrorCode
```
typedef NS_ENUM(NSUInteger, AspectErrorCode) {
    AspectErrorSelectorBlacklisted,                   /// Selectors like release, retain, autorelease are blacklisted.
    AspectErrorDoesNotRespondToSelector,              /// Selector could not be found.
    AspectErrorSelectorDeallocPosition,               /// When hooking dealloc, only AspectPositionBefore is allowed.
    AspectErrorSelectorAlreadyHookedInClassHierarchy, /// Statically hooking the same method in subclasses is not allowed.
    AspectErrorFailedToAllocateClassPair,             /// The runtime failed creating a class pair.
    AspectErrorMissingBlockSignature,                 /// The block misses compile time signature info and can't be called.
    AspectErrorIncompatibleBlockSignature,            /// The block signature does not match the method or is too large.
    AspectErrorRemoveObjectAlreadyDeallocated = 100   /// (for removing) The object hooked is already deallocated.
};
```
这里定义了在执行的时候的错误码，在平时开发中我们也经常使用这种方式，尤其是在定义网络请求的时候。
### AspectsContainer
```
// Tracks all aspects for an object/class.
@interface AspectsContainer : NSObject
- (void)addAspect:(AspectIdentifier *)aspect withOptions:(AspectOptions)injectPosition;
- (BOOL)removeAspect:(id)aspect;
- (BOOL)hasAspects;
@property (atomic, copy) NSArray *beforeAspects;
@property (atomic, copy) NSArray *insteadAspects;
@property (atomic, copy) NSArray *afterAspects;
@end
```
对象一个类或者所有的的`Aspects`本世纪的牛顿情况，这里注意的英文数组通过`atomic`修饰的。
关于`atomic`需要注意在默认情况下，由编译器所合成的方法会通过锁定机制确保其原子性`(atomicity)`。如果具备属性`nonatomic`特质，则不需要同步锁。
注意一共有两中容器，一个是对象的切片，一个是类的切片。
### AspectIdentifier
```
// Tracks a single aspect.
@interface AspectIdentifier : NSObject
+ (instancetype)identifierWithSelector:(SEL)selector object:(id)object options:(AspectOptions)options block:(id)block error:(NSError **)error;
- (BOOL)invokeWithInfo:(id<AspectInfo>)info;
@property (nonatomic, assign) SEL selector;
@property (nonatomic, strong) id block;
@property (nonatomic, strong) NSMethodSignature *blockSignature;
@property (nonatomic, weak) id object;
@property (nonatomic, assign) AspectOptions options;
@end
```
一个`Aspect`的具体内容主要所有游戏了单个的。`aspect`的具体信息，包括执行时机，要执行模块所需要用到的具体信息：包括方法签名，参数等等其实就是将我们传入的`bloc`，包装成AspectIdentifier，便于后续使用。**通过我们替换的块实例化。也就是将我们传入的块，包装成了AspectIdentifier**
### AspectInfo
```
@interface AspectInfo : NSObject <AspctInfo>
- (id)initWithInstance:(__unsafe_unretained id)instance invocation:(NSInvocation *)invocation;
@property (nonatomic, unsafe_unretained, readonly) id instance;
@property (nonatomic, strong, readonly) NSArray *arguments;
@property (nonatomic, strong, readonly) NSInvocation *originalInvocation;
@end
```
的英文主要`NSInvocation`信息。将`NSInvocation`包装一层，比如参数信息等。便于直接使用。
### AspectTracker
```
@interface AspectTracker : NSObject
- (id)initWithTrackedClass:(Class)trackedClass parent:(AspectTracker *)parent;
@property (nonatomic, strong) Class trackedClass;
@property (nonatomic, strong) NSMutableSet *selectorNames;
@property (nonatomic, weak) AspectTracker *parentEntry;
@end
```
用于跟踪所改变的类，打上标记，用于替换类方法，**防止重复替换类方法**。
### 流程
![](http://upload-images.jianshu.io/upload_images/664334-8c878b908de1a051?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 读源码是一件辛苦的事情.
