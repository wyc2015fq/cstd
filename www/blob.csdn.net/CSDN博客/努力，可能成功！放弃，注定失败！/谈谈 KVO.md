# 谈谈 KVO - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月05日 09:18:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：339标签：[KVO																[iOS常用方法																[iOS 资源大全																[iOS学习资料](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=KVO&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
> 
文章结构如下：
- Why? （为什么要用KVO）
- What? （KVO是什么）
- How? （ KVO怎么用）
- More （更多细节）
- 原理
- 自己实现KVO
在我的上一篇文章[浅谈 iOS Notification](http://www.jianshu.com/p/8832f019c17f)中，我们说到了iOS中观察者模式的一种实现方式：**NSNotification 通知**，这次我们再来谈谈iOS中观察者模式的另一种实现方式：**KVO** 。
### Why？
假如，有一个person类，和一个Account类，account类中又有两个公开的属性，balance和interestRate，当account中的balance和interestRate发生变化时，需要知道通知到这个person，这个要求很正常，我的银行账户里的钱增加或减少了我当然要及时知道啊。有人可能会想，每隔一段时间去轮询Account中的balance和interestRate，当其发生变化就通知person，但是这样做不仅低效而且通知也不能及时发出。
![](http://upload-images.jianshu.io/upload_images/762048-24b746481d92f553.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个时候KVO就派上用场了。
### What？
KVO到底是什么呢？不着急，要说KVO还得先说下KVC，KVC（Key-value coding）是一种基于`NSKeyValueCoding`非正式协议的机制，能让我们直接使用一个或一串字符串标识符去访问，操作类的属性。
常用的方法比如：
```
```
- (nullable id)valueForKey:(NSString *)key;
- (void)setValue:(nullable id)value forKey:(NSString *)key;
- (nullable id)valueForKeyPath:(NSString *)keyPath;
- (void)setValue:(nullable id)value forKeyPath:(NSString *)keyPath;
```
```
通过这些方法加上正确的标识符(一般和属性同名)，可以直接获取或者设置一个类的属性，甚至可以轻易越过多个类的层级结构，直接获取目标属性。
![](http://upload-images.jianshu.io/upload_images/762048-01b82c3cd9c0e962.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
KVC还提供了集合操作的方法，直接获取到集合属性的同时还能对其进行求和，取平均数，求最大最小值等操作，如下为求和操作，具体可以到[苹果官方文档](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueCoding/index.html#//apple_ref/doc/uid/10000107-SW1)详细了解。
`NSNumber *amountSum = [self.transactions valueForKeyPath:@"@sum.amount"];`
##### KVO
KVO (Key-Value Observing) 是Cocoa提供的一种基于KVC的机制，允许一个对象去监听另一个对象的某个属性，当该属性改变时系统会去通知监听的对象(不是被监听的对象)。
上面那个例子如果用KVO实现的话，大概就是，用Person类的一个对象去监听Account类的一个对象的属性，然后当Account类对象的相应属性改变时，Person类的对象就会收到通知。这也是iOS种观察者模式的一种实现方式。
也就是说，一般情况下，任何一个对象可以监听任何一个对象（当然也包括自己本身）的任意属性，然后在其属性变化后收到通知。
### How?
那么KVO怎么用呢？KVO的使用步骤主要分为3步：**添加监听**，**接收通知**和**移除监听**。
##### 1. 添加监听
通过以下方法添加一个监听者：
`- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;`
我们重点关注一下这个方法的4个参数：
- 
**observer**：就是要添加的监听者对象，，当监听的属性发生改变时就会去通知该对象，该对象必须实现`- observeValueForKeyPath:ofObject:change:context:`方法，要不然当监听的属性的改变通知发出来，却发现没有相应的接收方法时，程序会抛出异常。
- 
**keyPath**：就是要被监听的属性，这里和KVC的规则一样。但是这个值不能传nil，要不然会报错。通常我们在用的时候会传一个与属性同名的字符串，但是这样可能会因为拼写错误，导致监听不成功，一个推荐的做法是，用这种方式`NSStringFromSelector(@selector(propertyName))`，其实就是是将属性的getter方法转换成了字符串，这样做的好处就是，如果你写错了属性名，xcode会用警告提醒你。
- 
**options**：是一些配置选项，用来指明通知发出的时机和通知响应方法`- observeValueForKeyPath:ofObject:change:context:`的`change`字典中包含哪些值，它的取值有4个，定义在`NSKeyValueObservingOptions`中，可以用`|`符号连接，如下：
1> **NSKeyValueObservingOptionNew**：指明接受通知方法参数中的`change`字典中应该包含改变后的新值。
2>**NSKeyValueObservingOptionOld**: 指明接受通知方法参数中的`change`字典中应该包含改变前的旧值。
3>**NSKeyValueObservingOptionInitial**: 当指定了这个选项时，在`addObserver:forKeyPath:options:context:`消息被发出去后，甚至不用等待这个消息返回，监听者对象会马上收到一个通知。这种通知只会发送一次，你可以利用这种“一次性“的通知来确定要监听属性的初始值。当同时制定这3个选项时，这种通知的`change`字典中只会包含新值，而不会包含旧值。虽然这时候的新值实际上是改变前的'旧值'，但是这个值对于监听者来说是新的。
4>**NSKeyValueObservingOptionPrior**：当指定了这个选项时，在被监听的属性被改变前，监听者对象就会收到一个通知（一般的通知发出时机都是在属性改变后，虽然change字典中包含了新值和旧值，但是通知还是在属性改变后才发出），这个通知会包含一个`NSKeyValueChangeNotificationIsPriorKey`key，其对应的值为一个NSNumber类型的`YES`。当同时指定该值、new和old的话，`change`字典会包含旧值而不会包含新值。你可以在这个通知中调用
```
-
 (void)willChangeValueForKey:(NSString *)key;
```
- 
**context**：添加监听方法的最后一个参数，是一个可选的参数，可以传任何数据，这个参数最后会被传到监听者的响应方法中，可以用来区分不同通知，也可以用来传值。如果你要用context来区分不同的通知，一个推荐的做法是声明一个静态变量，其保持它自己的地址，这个变量没有什么意义，但是却能起到区分的作用，如下：
```
static void *PersonAccountBalanceContext = &PersonAccountBalanceContext;
static void *PersonAccountInterestRateContext = &PersonAccountInterestRateContext;
```
然后，结合上面Person，account的例子，我们可以给Account对象添加监听：
```
- (void)registerAsObserverForAccount:(Account*)account {
  [account addObserver:self
            forKeyPath:@"balance"
               options:(NSKeyValueObservingOptionNew |
                        NSKeyValueObservingOptionOld)
               context:PersonAccountBalanceContext];
  [account addObserver:self
            forKeyPath:@"interestRate"
               options:(NSKeyValueObservingOptionNew |
                        NSKeyValueObservingOptionOld)
                context:PersonAccountInterestRateContext];
}
```
> 
需要注意的是，添加监听的方法`addObserver:forKeyPath:options:context:`并不会对监听和被监听的对象以及context做强引用，你必须自己保证他们在监听过程中不被释放。
##### 2. 接受通知
前面说过了，每一个监听者对象都必须实现下面这个方法来接收通知：
`- (void)observeValueForKeyPath:(nullable NSString *)keyPath ofObject:(nullable id)object change:(nullable NSDictionary<NSKeyValueChangeKey, id> *)change context:(nullable void *)context;`
keyPath，object，context和监听方法中指定的一样，关于change参数，它是一个字典，有五个常量作为它的键：
```
NSString *const NSKeyValueChangeKindKey;  
NSString *const NSKeyValueChangeNewKey;  
NSString *const NSKeyValueChangeOldKey;  
NSString *const NSKeyValueChangeIndexesKey;  
NSString *const NSKeyValueChangeNotificationIsPriorKey;
```
一个一个分析下：
- 
**NSKeyValueChangeKindKey**：指明了变更的类型，值为“NSKeyValueChange”枚举中的某一个，类型为NSNumber。
```
enum {
 NSKeyValueChangeSetting = 1,
 NSKeyValueChangeInsertion = 2,
 NSKeyValueChangeRemoval = 3,
 NSKeyValueChangeReplacement = 4
};
typedef NSUInteger NSKeyValueChange;
```
一般情况下返回的都是1也就是第一个NSKeyValueChangeSetting，但是如果你监听的属性是一个集合对象的话，当这个集合中的元素被插入，删除，替换时，就会分别返回`NSKeyValueChangeInsertion`，`NSKeyValueChangeRemoval`和`NSKeyValueChangeReplacement`。
- 
**NSKeyValueChangeNewKey**：被监听属性改变后新值的key，当监听属性为一个集合对象，且`NSKeyValueChangeKindKey`不为`NSKeyValueChangeSetting`时，该值返回的是一个数组，包含插入，替换后的新值（删除操作不会返回新值）。
- 
**NSKeyValueChangeOldKey**：被监听属性改变前旧值的key，当监听属性为一个集合对象，且`NSKeyValueChangeKindKey`不为`NSKeyValueChangeSetting`时，该值返回的是一个数组，包含删除，替换前的旧值（插入操作不会返回旧值）
- 
**NSKeyValueChangeIndexesKey**：如果`NSKeyValueChangeKindKey`的值为`NSKeyValueChangeInsertion`,
`NSKeyValueChangeRemoval`, 或者 `NSKeyValueChangeReplacement`，这个键的值是一个NSIndexSet对象，包含了增加，移除或者替换对象的index。
- **NSKeyValueChangeNotificationIsPriorKey**：如果注册监听者是options中指明了`NSKeyValueObservingOptionPrior`，`change`字典中就会带有这个key，值为NSNumber类型的YES.
最后，完整的change字典大概就类似这样：
```
NSDictionary *change = @{
                             NSKeyValueChangeKindKey : NSKeyValueChange(枚举值),
                             NSKeyValueChangeNewKey : newValue,
                             NSKeyValueChangeOldKey : oldValue,
                             NSKeyValueChangeIndexesKey : @[NSIndexSet, NSIndexSet],
                             NSKeyValueChangeNotificationIsPriorKey : @1,
                             };
```
继续用上面的例子实现接受通知如下：
```
```
- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context {
    if (context == PersonAccountBalanceContext) {
        // Do something with the balance…
    } else if (context == PersonAccountInterestRateContext) {
        // Do something with the interest rate…
    } else {
        // Any unrecognized context must belong to super
        [super observeValueForKeyPath:keyPath
                             ofObject:object
                               change:change
                               context:context];
    }
}
```
```
> 
你可以通过context或者keypath来区分不同的通知，但是要注意的是，正如上面实例代码中那样，当接收到一个不能识别的context或者keypath的话，需要调用一下父类的`- observeValueForKeyPath:ofObject:change:context:`方法
##### 3. 移除监听
当一个监听者完成了它的监听任务之后，就需要注销(移除)监听者，调用以下2个方法来移除监听。通常会在`-dealloc`方法或者`-observeValueForKeyPath:ofObject:change:context:`方法中移除。
```
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context
或者
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath;
```
有几点需要注意的：
- 
当你向一个不是监听者的对象发送remove消息的时候（也可能是，你发送remove消息时，接受消息的对象已经被remove了一次，或者在注册为监听者前就调用了remove），xcode会抛出一个`NSRangeException`异常，所以，保险的做法是，把remove操作放在`try/catch`中。
- 
一个监听者在其被销毁时，并不会自己注销监听，而给一个已经销毁的监听者发送通知，会造成野指针错误。所以至少保证，在监听者被释放前，将其监听注销。保证有一个add方法，就有一个remove方法。
### More
再说更多的一些东西，想让类的某个属性支持KVO机制的话，这个类必须满足一下3点：
- 这个类必须使得该属性支持KVC。
- 这个类必须保证能够将改变通知发出。
- 当有依赖关系的时候，注册合适的依赖键。
- 
第一个条件：这个类必须使得该属性支持KVC
就是需要实现与该属性对应的getter和setter方法和其他一些可选方法。幸运的是，`NSObject`类已经帮我们实现了这些，只要你的类最终是继承自`NSObject`，并且使用正常的方式创建属性，这些属性都是支持KVO的。
> 
KVO支持的类型和KVC一样，包括对象类型，标量（例如 `int` 和 `CGFloat`）和 `struct`（例如
`CGRect`）。
- 
第二个条件：这个类必须保证能够将改变通知发出。
通知发出的方式又分为**自动通知**和**手动通知**：
1> **自动通知**
自动通知由`NSObject`默认实现了，也就是说一般情况下，你不用写额外的一些代码，属性改变的通知就会自动发出，这也是我们平常开发中接触最多的。
触发自动通知发出的方式包括下面这些：
```
```
// Call the accessor method.
[account setName:@"Savings"];
// Use setValue:forKey:.
[account setValue:@"Savings" forKey:@"name"];
// Use a key path, where 'account' is a kvc-compliant property of 'document'.
[document setValue:@"Savings" forKeyPath:@"account.name"];
// Use mutableArrayValueForKey: to retrieve a relationship proxy object.
Transaction *newTransaction = <#Create a new transaction for the account#>;
NSMutableArray *transactions = [account mutableArrayValueForKey:@"transactions"];
[transactions addObject:newTransaction];
```
```
其中包括调用`setter`方法，调用KVC的`setValue:forKey:`和`setValue:forKeyPath:`,最后一个方法需要说一下,`mutableArrayValueForKey:`也是KVC的方法，大家应该都知道，如果你用KVO监听了一个集合对象（比如一个数组），当你给数组发送`addObject:`消息时，是不会触发KVO通知的，但是通过`mutableArrayValueForKey:`这个方法对集合对象进行的相关操作（增加，删除，替换元素）就会触发KVO通知，这个方法会返回一个**中间代理对象**，这个中间代理对象的类会指向一个**中间类**，你在这个代理对象上进行的操作最终应在原始对象上造成同样的效果。
2> **手动通知**
有时候，你可能会想控制通知的发送，比如，阻止一些不必要的通知发出，或者把一组类似的通知合并成一个，这时候就需要手动发送通知了。
首先，你需要重写`NSObject`的一个类方法，来指明你不想让哪个属性的改变通知自动发出。
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
如上，return `NO`就可以阻止，该`key`对应的属性改变时，通知不会自动发送给监听者对象，当然对于其他的属性别忘了调用`super`方法保持它原来的状态。（改方法默认返回YES）
然后，你需要重写你想手动发送通知属性的setter方法，然后在属性值改变之前和之后分别调用`willChangeValueForKey:`和`didChangeValueForKey:`方法。
```
- (void)setBalance:(double)theBalance {
  [self willChangeValueForKey:@"balance"];
  _balance = theBalance;
  [self didChangeValueForKey:@"balance"];
}
```
这样就基本实现了一个KVO的手动通知，当该属性值改变时，监听者对象就能收到改变通知了。
你还可以过滤一些通知，像下面的例子就是只有当属性真正改变时才会发出通知
```
- (void)setBalance:(double)theBalance {
  if (theBalance != _balance) {
      [self willChangeValueForKey:@"balance"];
      _balance = theBalance;
      [self didChangeValueForKey:@"balance"];
  }
}
```
如果一个操作导致了多个键的变化，你必须嵌套变更通知：
```
- (void)setBalance:(double)theBalance {
  [self willChangeValueForKey:@"balance"];
  [self willChangeValueForKey:@"itemChanged"];
  _balance = theBalance;
  _itemChanged = _itemChanged+1;
  [self didChangeValueForKey:@"itemChanged"];
  [self didChangeValueForKey:@"balance"];
}
```
在to-many关系操作的情形中，你不仅必须表明key是什么，还要表明变更类型和影响到的索引。变更类型是一个 NSKeyValueChange值，被影响对象的索引是一个 NSIndexSet对象。
下面的代码示范了在to-many关系transactions对象中的删除操作：
```
- (void)removeTransactionsAtIndexes:(NSIndexSet *)indexes {
  [self willChange:NSKeyValueChangeRemoval
      valuesAtIndexes:indexes forKey:@"transactions"];
  // Remove the transaction objects at the specified indexes.
  [self didChange:NSKeyValueChangeRemoval
      valuesAtIndexes:indexes forKey:@"transactions"];
}
```
- 
第三个条件：这个类必须使得该属性支持KVC
有时候会存在这样一种情况，一个属性的改变依赖于别的一个或多个属性的改变，也就是说当别的属性改了，这个属性也会跟着改变，比如说一个人的全名fullName包括firstName和lastName，当firstName或者lastName中任何一个值改变了，fullName也就改变了。一个监听者监听了fullName，当firstName或者lastName改变时，这个监听者也应该被通知。
一种方法就是重写`keyPathsForValuesAffectingValueForKey:`方法去指明`fullName`属性是依赖于`lastName`和`firstName`的：
```
+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
  NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
  if ([key isEqualToString:@"fullName"]) {
      NSArray *affectingKeys = @[@"lastName", @"firstName"];
      keyPaths = [keyPaths setByAddingObjectsFromArray:affectingKeys];
  }
  return keyPaths;
}
```
另一种实现同样结果的方法是实现一个遵循命名方式为`keyPathsForValuesAffecting<Key>`的类方法，`<Key>`是依赖于其他值的属性名（首字母大写），用上面代码的例子来重新实现一下：
```
+ (NSSet *)keyPathsForValuesAffectingFullName {
  return [NSSet setWithObjects:@"lastName", @"firstName", nil];
}
```
但是在To-many Relationships中（比如数组属性），上面的方法就不管用了，比如，假如你有一个`Department`类，它有一个针对`Employee`类的to-many关系（即拥有一个装有`Employee`类对象的数组），`Employee`类有`salary`属性。你希望`Department`类有一个`totalSalary`属性来计算所有员工的薪水，也就是在这个关系中`Department`的`totalSalary`依赖于所有`Employee`的`salary`属性。这种情况你不能通过实现`keyPathsForValuesAffectingTotalSalary`方法并返回`employees.salary`。
有两种解决方法：
- 
你可以用KVO将parent（比如Department）作为所有children（比如Employee）相关属性的观察者。你必须在把child添加或删除到parent时也把parent作为child的观察者添加或删除。在`observeValueForKeyPath:ofObject:change:context:`方法中我们可以针对被依赖项的变更来更新依赖项的值：
```
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
if (context == totalSalaryContext) {
   [self updateTotalSalary];
}
else
// deal with other observations and/or invoke super...
}
- (void)updateTotalSalary {
[self setTotalSalary:[self valueForKeyPath:@"employees.@sum.salary"]];
}
- (void)setTotalSalary:(NSNumber *)newTotalSalary {
if (totalSalary != newTotalSalary) {
   [self willChangeValueForKey:@"totalSalary"];
   _totalSalary = newTotalSalary;
   [self didChangeValueForKey:@"totalSalary"];
}
}
- (NSNumber *)totalSalary {
return _totalSalary;
}
```
- 使用iOS中观察者模式的另一种实现方式：通知 (NSNotification) ，有关通知相关的概念和用法，可以参考我上一篇文章 [浅谈 iOS Notification](http://www.jianshu.com/p/8832f019c17f) 。
### 原理
说了这么多，KVO的原理到底是什么呢？
先上官方文档：
```
Automatic key-value observing is implemented using a technique called 
isa-swizzling...When an observer is registered for an attribute of an
object the isa pointer of the observed object is modified, pointing to
 an intermediate class rather than at the true class. As a result the 
value of the isa pointer does not necessarily reflect the actual class
of the instance.
```
对于KVO实现的原理，苹果官方文档描述的比较少，从中只能知道苹果使用了一张叫做`isa-swizzling`的黑魔法...
**其实，当某个类的对象第一次被观察时，系统就会在运行期动态地创建该类的一个派生类（类名就是在该类的前面加上`NSKVONotifying_` 前缀），在这个派生类中重写基类中任何被观察属性的 setter 方法。**
**派生类在被重写的 setter 方法实现真正的通知机制，就如前面手动实现键值观察那样，调用`willChangeValueForKey:`和`didChangeValueForKey:`方法。这么做是基于设置属性会调用 setter 方法，而通过重写就获得了 KVO 需要的通知机制。当然前提是要通过遵循 KVO 的属性设置方式来变更属性值，如果仅是直接修改属性对应的成员变量，是无法实现 KVO 的。**
**同时派生类还重写了 class 方法以“欺骗”外部调用者它就是起初的那个类。然后系统将这个对象的 isa 指针指向这个新诞生的派生类，因此这个对象就成为该派生类的对象了，因而在该对象上对 setter 的调用就会调用重写的 setter，从而激活键值通知机制。此外，派生类还重写了 dealloc 方法来释放资源。**
### 自己实现KVO
港真，原生的KVO API是不太友好的，需要监听者对象，和被监听的对象分别去实现一些东西，代码实现比较分散，并且响应通知的方法也不能自定义，只能在苹果提供的方法中处理，不能用我们熟悉的`block`或者`Target-Action`，最后还不能忘了调用`removeObserve`方法，一忘可能程序运行的时候就奔溃了...
在知道了KVO的使用方法和内部原理之后，我们其实可以自己去实现一个使用起来更加便捷，API更加友好的KVO的，这类的实现网上有很多，我就不献丑了... github上也有一些开源的实现代码，感兴趣的童鞋可以自行查阅。
其实基本思路和苹果官方的原理差不多，都是创建一个原类的派生类当做中间类，再把原来的对象指向这个中间类，再重写监听属性的Setter方法，在属性改变后调用回调通知监听者。
#### 参考：
[KVO官方文档](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html#//apple_ref/doc/uid/10000177-BCICJDHA)
[KVC官方文档](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueCoding/index.html#//apple_ref/doc/uid/10000107-SW1)
[Objective-C中的KVC和KVO](http://yulingtianxia.com/blog/2014/05/12/objective-czhong-de-kvche-kvo/)
[详解键值观察（KVO）及其实现机理](http://blog.csdn.net/kesalin/article/details/8194240)
文／XcodeMen（简书作者）
原文链接：http://www.jianshu.com/p/cfd553f250f9
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
