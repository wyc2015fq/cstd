# NSHashTable的特性和使用 - 月若无涯的专栏 - CSDN博客
2015年05月15日 15:57:03[月若无涯](https://me.csdn.net/u010124617)阅读数：1220
正好在看KVOController，发现NSHashTable，查资料时发现这篇文章，缘分。。
在看KVOController的代码时，又看到了NSHashTable这个类，所以就此整理一下。
NSHashTable效仿了NSSet(NSMutableSet)，但提供了比NSSet更多的操作选项，尤其是在对弱引用关系的支持上，NSHashTable在对象/内存处理时更加的灵活。相较于NSSet，NSHashTable具有以下特性：
- 
NSSet(NSMutableSet)持有其元素的强引用，同时这些元素是使用hash值及isEqual:方法来做hash检测及判断是否相等的。
- 
NSHashTable是可变的，它没有不可变版本。
- 
它可以持有元素的弱引用，而且在对象被销毁后能正确地将其移除。而这一点在NSSet是做不到的。
- 
它的成员可以在添加时被拷贝。
- 
它的成员可以使用指针来标识是否相等及做hash检测。
- 
它可以包含任意指针，其成员没有限制为对象。我们可以配置一个NSHashTable实例来操作任意的指针，而不仅仅是对象。
初始化NSHashTable时，我们可以设置一个初始选项，这个选项确定了这个NSHashTable对象后面所有的行为。这个选项是由NSHashTableOptions枚举来定义的，如下所示：
```
`enum {`
`    ``// 默认行为，强引用集合中的对象，等同于NSSet`
`    ``NSHashTableStrongMemory             = 0,`
`    ``// 在将对象添加到集合之前，会拷贝对象`
`    ``NSHashTableCopyIn                   = NSPointerFunctionsCopyIn,`
`    ``// 使用移位指针(shifted pointer)来做hash检测及确定两个对象是否相等；`
`    ``// 同时使用description方法来做描述字符串`
`    ``NSHashTableObjectPointerPersonality = NSPointerFunctionsObjectPointerPersonality,`
`    ``// 弱引用集合中的对象，且在对象被释放后，会被正确的移除。`
`    ``NSHashTableWeakMemory               = NSPointerFunctionsWeakMemory `
`};`
`typedef NSUInteger NSHashTableOptions;`
```
当然，我们还可以使用NSPointerFunctions来初始化，但只有使用NSHashTableOptions定义的这些值，才能确保NSHashTable的各个API可以正确的工作—包括拷贝、归档及快速枚举。
个人认为NSHashTable吸引人的地方在于可以持有元素的弱引用，而且在对象被销毁后能正确地将其移除。我们来写个示例：
```
`// 具体调用如下`
`@implementation TestHashAndMapTableClass {`
`    ``NSMutableDictionary *_dic;`
`    ``NSSet               *_set;`
`    ``NSHashTable         *_hashTable;`
`}`
`- (instancetype)init {`
`    ``self = [``super``init];`
`    ``if``(self) {`
`        ``[self testWeakMemory];`
`        ``NSLog(@``"hash table [init]: %@"``, _hashTable);`
`    ``}`
`    ``return``self;`
`}`
`- (void)testWeakMemory {`
`    ``if``(!_hashTable) {`
`        ``_hashTable = [NSHashTable weakObjectsHashTable];`
`    ``}`
`    ``NSObject *obj = [[NSObject alloc] init];`
`    ``[_hashTable addObject:obj];`
`    ``NSLog(@``"hash table [testWeakMemory] : %@"``, _hashTable);`
`}`
```
这段代码的输出结果如下：
```
`hash table [testWeakMemory] : NSHashTable {`
`[6] }`
`hash table [init]: NSHashTable {`
`}`
```
可以看到，在离开testWeakMemory方法，obj对象被释放，同时对象在集合中的引用也被安全的删除。
这样看来，NSHashTable似乎比NSSet(NSMutableSet)要好啊。那是不是我们就应用都使用NSHashTable呢？Peter Steinberger在[The Foundation
 Collection Classes](http://www.objc.io/issue-7/collections.html)给了我们一组数据，显示在添加对象的操作中，NSHashTable所有的时间差不多是NSMutableSet的2倍，而在其它操作中，性能大体相近。所以，如果我们只需要NSSet的特性，就尽量用NSSet。
另外，Mattt Thompson在[NSHashTable & NSMapTable](http://nshipster.com/nshashtable-and-nsmaptable/)的结尾也写了段挺有意思的话，在此直接摘抄过来：
**"`As always, it's important to remember that programming is not about being clever: always approach a problem from the highest viable level of abstraction. NSSet and NSDictionary are great classes. For 99% of problems, they are undoubtedly the correct tool ``for``the job. If, however, your problem has any of the particular memory management constraints described above, then NSHashTable & NSMapTable may be worth a look."`**
**参考**
- 
[NSHashTable Class Reference](https://developer.apple.com/library/ios/documentation/Cocoa/Reference/NSHashTable_class/)
- 
[NSHashTable & NSMapTable](http://nshipster.com/nshashtable-and-nsmaptable/)
- 
[NSHashTable & NSMapTable](http://billwang1990.github.io/blog/2014/03/31/nshashtable-and-nsmaptable/)
- 
[The Foundation Collection Classes](http://www.objc.io/issue-7/collections.html)
