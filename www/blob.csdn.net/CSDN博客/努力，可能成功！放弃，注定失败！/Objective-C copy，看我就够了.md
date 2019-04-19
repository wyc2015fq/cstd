# Objective-C copy，看我就够了 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月28日 09:14:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：339
## 一、从面向对象到Objective-C概览copy
#### 1、面向对象：
> 
In object-oriented programming, object copying is creating a copy of an existing object, a unit of data in object-oriented programming. The resulting object is called an object copy or simply copy of the original object. Copying is basic but has subtleties
 and can have significant overhead. There are several ways to copy an object, most commonly by a copy constructor or cloning. Copying is done mostly so the copy can be modified or moved, or the current value preserved. If either of these is unneeded, a reference
 to the original data is sufficient and more efficient, as no copying occurs.
在面向对象的程序设计中，对象的copy就是创建一个已经存在的对象的copy。这种对象的创建的结果被称为原始对象的copy。copy是很基础的，但是也有其精巧的地方，并且可能造成巨大的消耗。有很多种方式可以copy对象，最常用的就是copy构造器和克隆。copy经常用于对象的修改、移动和保护。如果上述的几种应用都不需要，持有原始对象的引用就足够了，并不需要copy。
#### 2、OC:
> 
In Objective-C, the methods copy and mutableCopy are inherited by all objects and intended for performing copies; the latter is for creating a mutable type of the original object. These methods in turn call the copyWithZone and mutableCopyWithZone methods,
 respectively, to perform the copying. An object must implement the corresponding copyWithZone method to be copyable.
在OC中，copy和mutableCopy两个方法是被所有对象继承的(有点小毛病，应该指所有继承自NSObject的类)，这两个方法就是为copy准备的。其中，mutableCopy是为了创建原始对象的可变类型的copy。这两个方法分别调用copyWithZone和mutableCopyWithZone两个方法来进行copy。一个对象必须实现copyWithZone或者mutableCopyWithZone，才能进行copy或者mutableCopy。
那么，我们可以从以上获取到什么信息?
- copy经常用于对象的修改、移动和保护。如果上述的几种应用都不需要，持有原始对象的引用就足够了，并不需要copy。
- 一个类必须实现*copyWithZone*或者*mutableCopyWithZone*，才能进行copy或者mutableCopy。
下一阶段，本文将展开讲述OC中的copy相关信息以及如何使用copy方法。
## 二、Objective-C中copy相关
#### 1、OC中的copy相关内容
- 在XCode 里Foundation.framework下的Headers里，也在系统里找到原文件：/System/Library/Frameworks/Foundation.framework/Versions/C/Headers/NSObject.h
```
@protocol NSCopying
－ (id)copyWithZone:(nullable NSZone *)zone;
@end
@protocol NSMutableCopying
－ (id)mutableCopyWithZone:(nullable NSZone *)zone;
@end
```
- 在/usr/include/objc 下面找到 runtime 的 NSObject.h
```
－ (id)copy;
－ (id)mutableCopy;
```
- 修饰属性的关键字copy
#### 2、这里需要注意的有以下几点
- 若想使用**copy**和**mutableCopy**，需要分别实现**NSCopying**协议和**NSMutableCopying**协议，即实现**copyWithZone:**和**mutableCopyWithZone:**方法。
- 继承自NSObject的大部分框架类均默认实现了NSCopying，并且一些具备可变类型的类如NSString、NSArray、NSDictionary，以及它们的可变类型类NSMutableString、NSMutableArray和NSMutableDictionary也实现了NSMutableCopying。(查了大部分常用类，均实现了NSCopying，所以暂时这么说吧，可能有人说NSNumber并没有实现NSCopying，那你可以看一下它的父类NSValue，其实现了NSCopying)
- 对于一些自定义类，需要自己实现NSCopying。具体方式且看下部分。
## 三、非容器对象的深浅copy
首先，我们谈一下非容器对象的深浅copy，这些非容器对象，包含常用的NSString、NSNumber等，也包括我们自定义的一些非容器类的实例。下面分三个三面进行分析。
#### 1、首先说说深浅copy
**准则**
浅copy:指针复制，不会创建一个新的对象。
深copy:内容复制，会创建一个新的对象。
> 
此处，不进行过多的解释，从下面的结果分析中，按例子来理解。
#### 2、框架类的深浅copy
**准则**
探究框架类深copy还是浅copy，需要清楚的是该类如何实现的NSCopying和NSMutableCopy的两个方法copyWithZone:和mutableCopyWithZone:。然而OC并不开源，并且本文这里也不会进行源码的推测。
那么，我们应该遵循怎样一个原则呢？如下：
- 对immutableObject，即不可变对象，执行copy，会得到不可变对象，并且是浅copy。
- 对immutableObject，即不可变对象，执行mutableCopy，会得到可变对象，并且是深copy。
- 对mutableObject，即可变对象，执行copy，会得到不可变对象，并且是深copy。
- 对mutableObject，即可变对象，执行mutableCopy，会得到可变对象，并且是深copy。
**代码**
```
// 此处以NSString为例探究框架类深浅copy
// 不可变对象
NSString *str = @"1";
NSString *str1 = [str copy];
NSString *str2 = [str mutableCopy];
// 可变对象
NSMutableString *mutableStr = [NSMutableString stringWithString:@"1"];
NSMutableString *mutableStr1 = [mutableStr copy];
NSMutableString *mutableStr2 = [mutableStr mutableCopy];
// 打印对象的指针来确认是否创建了一个新的对象
// 不可变对象原始指针
NSLog(@"%p", str);
// 不可变对象copy后指针
NSLog(@"%p", str1);
// 不可变对象mutalbeCopy后指针
NSLog(@"%p", str2);
// 可变对象原始指针
NSLog(@"%p", mutableStr);
// 可变对象copy后指针
NSLog(@"%p", mutableStr1);
// 可变对象mutalbeCopy后指针
NSLog(@"%p", mutableStr2);
```
**结果分析**
```cpp
```cpp
// 此处依次对应上述6个log，可见与前面所讲的原则吻合（此处不验证可变类型和不可变类型，默认上述原则正确即可）。
2016-10-21 10:50:52.879 Memory[67680:5623387] 0x10d85a1b0
2016-10-21 10:50:52.879 Memory[67680:5623387] 0x10d85a1b0
2016-10-21 10:50:52.879 Memory[67680:5623387] 0x60800007a080
2016-10-21 10:50:52.879 Memory[67680:5623387] 0x60800007a9c0
2016-10-21 10:50:52.880 Memory[67680:5623387] 0xa000000000000311
2016-10-21 10:50:52.880 Memory[67680:5623387] 0x60800007a900
```
```
#### 3、自定义类的深浅copy
**准则**
对于一个我们自定义的类型，显然比框架类容易操纵的多。此处就拿NSCopying举例（因为从没有自定义过具有可变类型的类，当然，如果有需要的话，也可以实现NSMutableCopying）。自定义的类就和2中的原则没有半毛钱关系了，一切就看你怎么实现NSCopying协议中的copyWithZone:方法。
**代码**
```
// Model定义，copyWithZone第一种实现（浅copy）
@interface Model1 : NSObject <NSCopying>
@property (nonatomic, assign) NSInteger a;
@end
@implementation Model1
- (id)copyWithZone:(NSZone *)zone {
    return self;
}
@end
// Model定义，copyWithZone第二种实现（深copy）
@interface Model1 : NSObject <NSCopying>
@property (nonatomic, assign) NSInteger a;
@end
@implementation Model1
- (id)copyWithZone:(NSZone *)zone {
    Model1 *model = [[Model1 allocWithZone:zone] init];
    model.a = self.a;
    return model;
}
@end
// 分别选择上述两种model进行指针打印。
Model1 *model = [[Model1 alloc] init];
Model1 *copyModel = [model copy];
NSLog(@"%p", model);
NSLog(@"%p", copyModel);
```
**结果分析**
```cpp
```cpp
// 对应上述一，可见实现了浅copy
2016-10-21 11:12:03.149 Memory[67723:5636292] 0x60000000c9d0
2016-10-21 11:12:03.149 Memory[67723:5636292] 0x60000000c9d0
// 对应上述二，可见实现了深copy
2016-10-21 11:16:46.803 Memory[67752:5640133] 0x60800001df00
2016-10-21 11:16:46.803 Memory[67752:5640133] 0x60800001def0
```
```
## 四、容器对象的深浅copy
前文已经知道了深浅copy的区别，你也大致猜到了为什么将容器对象拿出来作为一块。对，因为容器中可能包含很多对象，而这些对象也需要区分深浅copy。往深里说，容器中可能包含容器对象，那更是麻烦了。不要急，看下面，以NSArray的深浅copy为例，将容器的深浅copy分为四种。
#### 1、浅copy
**准则**
容器的浅copy，符合三.2中的原则。
**代码**
```
// 和NSString浅copy的验证步骤一样
NSArray *arr = [NSArray arrayWithObjects:@"1", nil];
NSArray *copyArr = [arr copy];
NSLog(@"%p", arr);
NSLog(@"%p", copyArr);
```
**结果分析**
```cpp
```cpp
// 无疑是浅copy(你可能会问，为什么不看一下arr和copyArr内部元素的指针对比？这里并没有必要，最外层对象都没有创建新的，里面不用验证)
2016-10-21 11:27:57.554 Memory[67778:5646253] 0x600000010690
2016-10-21 11:27:57.554 Memory[67778:5646253] 0x600000010690
```
```
#### 2、单层深copy
**准则**
容器的单层深copy，符合三.2中的原则（只是深copy变成了单层深copy）。这里的单层指的是完成了NSArray对象的深copy，而未对其容器内对象进行处理。
**代码**
```
NSArray *arr = [NSArray arrayWithObjects:@"1", nil];
NSArray *copyArr = [arr mutableCopy];
NSLog(@"%p", arr);
NSLog(@"%p", copyArr);
// 打印arr、copyArr内部元素进行对比
NSLog(@"%p", arr[0]);
NSLog(@"%p", copyArr[0]);
```
**结果分析**
```cpp
```cpp
// 可发现前两项地址不同，即完成深copy，但是后两项相同，这代表容器内部的元素并没有完成深copy，所有称之为单层深copy
2016-10-21 11:32:27.157 Memory[67801:5649757] 0x6000000030d0
2016-10-21 11:32:27.157 Memory[67801:5649757] 0x600000242e50
2016-10-21 11:32:27.157 Memory[67801:5649757] 0x10dd811b0
2016-10-21 11:32:27.157 Memory[67801:5649757] 0x10dd811b0
```
```
#### 3、双层深copy
**准则**
容器的双层深copy已经脱离了三.2中的原则。这里的双层指的是完成了NSArray对象和NSArray容器内对象的深copy（为什么不说完全，是因为无法处理NSArray中还有一个NSArray这种情况）。
**代码**
```
// 随意创建一个NSMutableString对象
NSMutableString *mutableString = [NSMutableString stringWithString:@"1"];
// 随意创建一个包涵NSMutableString的NSMutableArray对象
NSMutableString *mutalbeString1 = [NSMutableString stringWithString:@"1"];
NSMutableArray *mutableArr = [NSMutableArray arrayWithObjects:mutalbeString1, nil];
// 将mutableString和mutableArr放入一个新的NSArray中
NSArray *testArr = [NSArray arrayWithObjects:mutableString, mutableArr, nil];
// 通过官方文档提供的方式创建copy
NSArray *testArrCopy = [[NSArray alloc] initWithArray:testArr copyItems:YES];
// testArr和testArrCopy指针对比
NSLog(@"%p", testArr);
NSLog(@"%p", testArrCopy);
// testArr和testArrCopy中元素指针对比
// mutableString对比
NSLog(@"%p", testArr[0]);
NSLog(@"%p", testArrCopy[0]);
// mutableArr对比
NSLog(@"%p", testArr[1]);
NSLog(@"%p", testArrCopy[1]);
// mutableArr中的元素对比，即mutalbeString1对比
NSLog(@"%p", testArr[1][0]);
NSLog(@"%p", testArrCopy[1][0]);
```
**结果分析**
```cpp
```cpp
// 这里可以发现，copy后，只有mutableArr中的mutalbeString1指针地址没有变化。而testArr的指针和testArr中的mutableArr、mutableString的指针地址均发生变化。所以称之为双层深复制。
2016-10-21 12:03:15.549 Memory[67855:5668888] 0x60800003c7a0
2016-10-21 12:03:15.549 Memory[67855:5668888] 0x60800003c880
2016-10-21 12:03:15.549 Memory[67855:5668888] 0x608000260540
2016-10-21 12:03:15.550 Memory[67855:5668888] 0xa000000000000311
2016-10-21 12:03:15.550 Memory[67855:5668888] 0x60800005d610
2016-10-21 12:03:15.550 Memory[67855:5668888] 0x60800000d2e0
2016-10-21 12:03:15.550 Memory[67855:5668888] 0x608000260980
2016-10-21 12:03:15.550 Memory[67855:5668888] 0x608000260980
```
```
**限制**
initWithArray: copyItems:会使NSArray中元素均执行copy方法。这也是我在testArr中放入NSMutableArray和NSMutableString的原因。如果我放入的是NSArray或者NSString，执行copy后，只会发生指针复制；如果我放入的是未实现NSCopying协议的对象，调用这个方法甚至会crash。这里，官方文档的描述有误。
> 
If the objects in the collection have adopted the [NSCopying](https://developer.apple.com/reference/foundation/nscopying)
protocol, the objects are deeply copied to the new collection, which is then the sole owner of the copied objects.
#### 4、完全深copy
**准则**
如果想完美的解决NSArray嵌套NSArray这种情形，可以使用归档、解档的方式。
**代码**
```
// 随意创建一个NSMutableString对象
NSMutableString *mutableString = [NSMutableString stringWithString:@"1"];
// 随意创建一个包涵NSMutableString的NSMutableArray对象
NSMutableString *mutalbeString1 = [NSMutableString stringWithString:@"1"];
NSMutableArray *mutableArr = [NSMutableArray arrayWithObjects:mutalbeString1, nil];
// 将mutableString和mutableArr放入一个新的NSArray中
NSArray *testArr = [NSArray arrayWithObjects:mutableString, mutableArr, nil];
// 通过归档、解档方式创建copy
NSArray *testArrCopy = [NSKeyedUnarchiver unarchiveObjectWithData:
                            [NSKeyedArchiver archivedDataWithRootObject:testArr]];;
// testArr和testArrCopy指针对比
NSLog(@"%p", testArr);
NSLog(@"%p", testArrCopy);
// testArr和testArrCopy中元素指针对比
// mutableString对比
NSLog(@"%p", testArr[0]);
NSLog(@"%p", testArrCopy[0]);
// mutableArr对比
NSLog(@"%p", testArr[1]);
NSLog(@"%p", testArrCopy[1]);
// mutableArr中的元素对比，即mutalbeString1对比
NSLog(@"%p", testArr[1][0]);
NSLog(@"%p", testArrCopy[1][0]);
```
**结果分析**
```cpp
```cpp
// 可见完成了完全深复制，testArr和testArrCopy中的元素，以及容器中容器的指针地址完全不同，所以完成了完全深复制。
2016-10-21 12:19:34.022 Memory[67887:5677318] 0x60800002db00
2016-10-21 12:19:34.022 Memory[67887:5677318] 0x60800002dc20
2016-10-21 12:19:34.022 Memory[67887:5677318] 0x608000260400
2016-10-21 12:19:34.023 Memory[67887:5677318] 0x6080002603c0
2016-10-21 12:19:34.023 Memory[67887:5677318] 0x608000051d90
2016-10-21 12:19:34.023 Memory[67887:5677318] 0x6080000521e0
2016-10-21 12:19:34.023 Memory[67887:5677318] 0x608000260600
2016-10-21 12:19:34.023 Memory[67887:5677318] 0x6080002606c0
```
```
**限制**
归档和解档的前提是NSArray中所有的对象都实现了NSCoding协议。
## 五、拾遗
#### 1、关键字copy
**代码与结果**
```cpp
```cpp
// 首先分别给出copy和strong修饰的属性，以NSString举例
// 1、strong
@property (nonatomic, strong) NSString *str;
// 2、copy
@property (nonatomic, copy) NSString *str;
// 分别对1和2执行下述代码
NSMutableString *mutableStr = [NSMutableString stringWithFormat:@"123"];
self.str = mutableStr;
[mutableStr appendString:@"456"];
NSLog(@"%@", self.str);
NSLog(@"%p", self.str);
NSLog(@"%@", mutableStr);
NSLog(@"%p", mutableStr);
// 结果1
2016-10-21 14:08:46.657 Memory[68242:5714288] 123456
2016-10-21 14:08:46.657 Memory[68242:5714288] 0x608000071040
2016-10-21 14:08:46.657 Memory[68242:5714288] 123456
2016-10-21 14:08:46.657 Memory[68242:5714288] 0x608000071040
// 结果2
2016-10-21 14:11:16.879 Memory[68264:5716282] 123
2016-10-21 14:11:16.880 Memory[68264:5716282] 0xa000000003332313
2016-10-21 14:11:16.880 Memory[68264:5716282] 123456
2016-10-21 14:11:16.880 Memory[68264:5716282] 0x60000007bbc0
```
```
**分析**
- 结果1为strong修饰的结果，可见** [mutableStr appendString:@"456"]**修改mutableStr造成了self.str的改变，显然不安全；结果2为copy修饰的结果，可见
**[mutableStr appendString:@"456"]**修改mutableStr未造成self.str的改变，显然安全。（从内存地址的变化也可以看出来）
- 这里可以推测出，copy关键字是在str属性的set方法里面返回了mutableStr的copy，而strong关键字仅仅是返回了mutableStr。
#### 2、深浅copy对引用计数的影响
浅copy，类似strong，持有原始对象的指针，会使retainCount加一。
深copy，会创建一个新的对象，不会对原始对象的retainCount变化。
```
// 也许你会疑问arc下如何访问retainCount属性，这里提供了两种方式（下面代码中a代表一个任意对象，这个对象最好不要是NSString和NSNumber，因为用它们进行测试会出问题）
// kvc方式
NSLog(@"Retain count is %ld", CFGetRetainCount((__bridge CFTypeRef)a));
// 桥接字方式
NSLog(@"Retain count %@", [a valueForKey:@"retainCount"]);
```
#### 3、可变和不可变
可变和不可变上文谈的不是很多，因为本文认为这完全与NSCopying和NSMutableCopying的实现息息相关。当然，对于框架类，我们可以简单的认为，copy方法返回的就是不可变对象，mutableCopy返回的就是可变对象。如果是自定义的类，就看你怎么实现NSCopying和NSMutableCopying协议了。
#### 4、copy和block
首先，**MRR**时代用retain修饰block会产生崩溃，因为作为属性的block在初始化时是被存放在静态区的，如果block内调用外部变量，那么block无法保留其内存，在初始化的作用域内使用并不会有什么影响，但一旦出了block的初始化作用域，就会引起崩溃。所有MRC中使用copy修饰，将block拷贝到堆上。
其次，在**ARC**时代，因为ARC自动完成了对block的copy，所以修饰block用copy和strong都无所谓。
#### 5、strong和shallowCopy
这个问题困惑了很久，最后只能得出一个结论，浅copy和strong引用的区别仅仅是浅copy多执行一步copyWithZone:方法。
## 六、文献
1、[https://developer.apple.com/library/prerelease/content/documentation/Cocoa/Conceptual/Collections/Articles/Copying.html#//apple_ref/doc/uid/TP40010162-SW1](https://developer.apple.com/library/prerelease/content/documentation/Cocoa/Conceptual/Collections/Articles/Copying.html#//apple_ref/doc/uid/TP40010162-SW1)
2、[https://en.wikipedia.org/wiki/Object_copying](https://en.wikipedia.org/wiki/Object_copying)
## 七、感谢
最后，感谢来自百度的@杨飞宇同学，和他的讨论给了我很多灵感。也感谢大家的阅读，希望对您有所帮助。如果有错误的地方或者不理解的地方，希望大家在评论区积极指出。如果对您有所帮助，希望给作者一个喜欢和关注，您的支持是我最核心的动力。
文／天口三水羊（简书作者）
原文链接：http://www.jianshu.com/p/ebbac2fec4c6
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
