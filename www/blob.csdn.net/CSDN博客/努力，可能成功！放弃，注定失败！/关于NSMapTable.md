# 关于NSMapTable - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月11日 17:20:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1629
## 1. 定义
打开官方文档，我们可以找到官方文档对于NSMapTable的定义：
> 
The NSMapTable class is a mutable collection modeled after NSDictionary, with the following differences:
- 
The major option is to have keys and/or values held “weakly” in a manner that entries are removed when one of the objects is reclaimed.
- 
Its keys or values may be copied on input or may use pointer identity for equality and hashing.
- 
It can contain arbitrary pointers (its contents are not constrained to being objects).
You can configure an NSMapTable instance to operate on arbitrary pointers and not just objects, although typically you are encouraged to use the C function API for void * pointers. (See Managing Map Tables for more information) The object-based API (such
 as setObject:forKey:) will not work for non-object pointers without type-casting.
上面那段话大致的意思是，NSMapTable对于NSDictionary来说，有几点特别的地方，其中表现在它可以指定key/value是需要strong，weak，甚至是copy，如果使用的是weak，当key、value在被释放的时候，会自动从NSMapTable中移除这一项。NSMapTable中可以包含任意指针，使用指针去做检查操作。
## 2. NSMapTable与NSDictionary
接下来我们针对上面提出的几点优点，做一下浅析：
- NSDcitionary有一个可变的类型即NSMutableDictionary，然而NSMapTable没有另外一个可变类，因为它本身就是可变的
- NSDcitionary或者NSMutableDictionary中对于key和value的内存管理是，对key进行copy，对value进行强引用。
- NSDcitionary中对于key的类型，我们看一下NSDitionary中的声明：
``+ (instancetype)dictionaryWithObject:(ObjectType)object forKey:(KeyType <NSCopying>)key;``
是需要key支持NSCopying协议，并且在NSDictionary中，object是由“key”来索引的，key的值不能改变，为了保证这个特性在NSDcitionary中对key的内存管理为copy，在复制的时候需要考虑对系统的负担，因此key应该是轻量级的，所以通常我们都用字符串和数字来做索引，但这只能说是key-to-object映射，不能说是object-to-object的映射。
- NSMapTabTable更适合于我们一般所说的映射标准，它既可以处理key-to-value又可以处理object-to-object
## 3. 使用
接下来我们看一下NSMapTable的一般使用
```
- (instancetype)initWithKeyOptions:(NSPointerFunctionsOptions)keyOptions valueOptions:(NSPointerFunctionsOptions)valueOptions capacity:(NSUInteger)initialCapacity NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithKeyPointerFunctions:(NSPointerFunctions *)keyFunctions valuePointerFunctions:(NSPointerFunctions *)valueFunctions capacity:(NSUInteger)initialCapacity NS_DESIGNATED_INITIALIZER;
+ (NSMapTable<KeyType, ObjectType> *)mapTableWithKeyOptions:(NSPointerFunctionsOptions)keyOptions valueOptions:(NSPointerFunctionsOptions)valueOptions;
```
我们会注意到在创建方法中有这么一个选项keyOptions和valueOptions，值类型都是NSPointerFunctionsOptions，NSMapTable就是通过这个选项来设定key和value的内存管理类型，点击进去可以看到其中有这么几个选项：
```
static const NSPointerFunctionsOptions NSMapTableStrongMemory ;
static const NSPointerFunctionsOptions NSMapTableZeroingWeakMemory;
static const NSPointerFunctionsOptions NSMapTableCopyIn;
static const NSPointerFunctionsOptions NSMapTableObjectPointerPersonality;
static const NSPointerFunctionsOptions NSMapTableWeakMemory;
```
根据参考资料中的描述
> 
The options provided to NSMapTable are composed of three parts: a "memory option", a "personality option" and the "copy in" flag. You may use one option for each part (there are default behaviors that will be used if no option is provided for the part).
 The parts are all bit flags (binary "or" them together to combine parts).Officially, NSMapTable allows the following options:
- NSMapTableStrongMemory (a "memory option")
- NSMapTableWeakMemory (a "memory option")
- NSMapTableObjectPointerPersonality (a "personality option")
- NSMapTableCopyIn (a "copy option")
- NSPointerFunctionsObjectPersonality
关于StrongMemory ，WeakMemory，TableCopyIn这三个都不难理解，对应的是key和value的内存管理类型，而关于Peronality这两个选项是用来是用isEqual和hash比较标准
- NSMapTableStrongMemory: 类似于我们之前使用的NSSet
- NSPointerFunctionsWeakMemory： 用__weak来存储
- NSPointerFunctionsCopyIn：对key和value进行copy处理
- NSPointerFunctionsObjectPersonality: isEqual和hash比较的是－description方法的值
- NSPointerFunctionsObjectPointerPersonality ： isEqual和hash比较的是指针的地址
## 4. 例子
### Rose 和 Jack的爱好
我们定义一个Person类，用来记录人名，我们再创建一个Favourite类用来创建爱好对象，现在有Rose和Jack两个人，分别的爱好是ObjC和Swift，人和爱好必须要用对象实现，而且必须关联起来在一个表里，以便我们进行查询和记录。如果是以前的话需要自己建立一个Dictionary，把人名的name字段作为key，favourite的对象作为value。但是这样有一个问题，如果突然某一天，我Person里面增加了个字段age，我这个表还要记录每个人的年龄，供我以后来查询不同年龄段的人统计使用呢？这下就很尴尬了，因为Dicitionary没办法实现我们要的这个效果，不过没关系NSMapTable可以实现，我们来看代码：
```
Person *p1 = [[Person alloc] initWithName:@"jack"];
Favourite *f1 = [[Favourite alloc] initWithName:@"ObjC"];
Person *p2 = [[Person alloc] initWithName:@"rose"];
Favourite *f2 = [[Favourite alloc] initWithName:@"Swift"];
NSMapTable *MapTable = [NSMapTable mapTableWithKeyOptions:NSMapTableWeakMemory valueOptions:NSMapTableWeakMemory];
// 设置对应关系表
// p1 => f1;
// p2 => f2
[MapTable setObject:f1 forKey:p1];
[MapTable setObject:f2 forKey:p2];
NSLog(@"%@ %@", p1, [MapTable objectForKey:p1]);
NSLog(@"%@ %@", p2, [MapTable objectForKey:p2]);
```
我们来看一下打印出来的内容
```cpp
```cpp
2016-06-14 18:34:10.289 NSMapTableDemo[40414:1323838] jack favourite is ObjC
2016-06-14 18:34:10.290 NSMapTableDemo[40414:1323838] rose favourite is Swift
```
```
我们创建了一个weak － weak的映射表，是不是不同类的对象一一对应起来了呢
## 参考资料
[NSMapTable: more than an NSDictionary for weak pointers](http://www.cocoawithlove.com/2008/07/nsmaptable-more-than-nsdictionary-for.html)
文／郑钦洪_（简书作者）
原文链接：http://www.jianshu.com/p/62d12b01be5c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
