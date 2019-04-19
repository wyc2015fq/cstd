# IOS开发之深拷贝与浅拷贝(mutableCopy与Copy)详解 - game3108的专栏 - CSDN博客
2015年09月21日 22:14:55[game3108](https://me.csdn.net/game3108)阅读数：301
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
阅读本文之前首先了解copy与retain的区别，如果有不正确的地方望大家多多指教:
copy与retain的区别：
copy是创建一个新对象，retain是创建一个指针，引用对象计数加1。Copy属性表示两个对象内容相同，新的对象retain为1 ，与旧有对象的引用计数无关，旧有对象没有变化。copy减少对象对上下文的依赖。
       retain属性表示两个对象地址相同（建立一个指针，指针拷贝），内容当然相同，这个对象的retain值+1也就是说，retain 是指针拷贝，copy 是内容拷贝。
当然在ios中并不是所有的对象都支持copy，mutableCopy，遵守NSCopying 协议的类可以发送copy消息，遵守NSMutableCopying
 协议的类才可以发送mutableCopy消息。假如发送了一个没有遵守上诉两协议而发送 copy或者 mutableCopy,那么就会发生异常。但是默认的ios类并没有遵守这两个协议。如果想自定义一下copy 那么就必须遵守NSCopying,并且实现 copyWithZone: 方法，如果想自定义一下mutableCopy
 那么就必须遵守NSMutableCopying,并且实现 mutableCopyWithZone: 方法。
首先我们需要有这样的一个前提：
　　［array addObject:obj];
　　这样obj的引用计数会增加1，如果使用remove则obj的引用计数会减一。
　　ios对集合的内存处理就是这样的。
　　那么，假设obj只被array拥有：
　　id temp ＝ [array objectAtIndex:0];
　　[array removeObjectAtIndex:0];
　　如果你再要使用temp就会出错，因为这个时候obj已经被释放了。
　　（提醒一下，如果用NSString做测试，要注意@“abc”是常量 :-)  ）
　　由于在程序中经常会遇到集合类的传值，所以，简单的retain未必够用，需要对集合内容的拷贝，也就是深拷贝。
　　下面我们就来探讨一下。
　　Ios提供了copy和mutablecopy方法，顾名思义，copy就是复制了一个imutable的对象，而mutablecopy就是复制了一个mutable的对象。以下将举几个例子来说明。
　　1.     系统的非容器类对象
　　这里指的是NSString,NSNumber等等一类的对象。
　　NSString *string = @"origion";
　　NSString *stringCopy = [string copy];
　　NSMutableString *stringMCopy = [string mutableCopy];
　　[stringMCopy appendString:@"!!"];
　　查看内存可以发现，string和stringＣopy指向的是同一块内存区域(又叫apple弱引用weak reference)，此时stringCopy的引用计数和string的一样都为２。而stringMCopy则是我们所说的真正意义上的复制，系统为其分配了新内存，但指针所指向的字符串还是和string所指的一样。
　　再看下面的例子： 
    NSMutableString *string = [NSMutableString stringWithString: @"origion"];
    NSString *stringCopy = [string copy];
    NSMutableString *mStringCopy = [string copy];
    NSMutableString *stringMCopy = [string mutableCopy];
    [mStringCopy appendString:@"mm"];//error
    [string appendString:@" origion!"];
    [stringMCopy appendString:@"!!"];
以上四个NSString对象所分配的内存都是不一样的。但是对于mStringCopy其实是个imutable对象，所以上述会报错。
对于系统的非容器类对象，我们可以认为，如果对一不可变对象复制，copy是指针复制（浅拷贝）和mutableCopy就是对象复制（深拷贝）。如果是对可变对象复制，都是深拷贝，但是copy返回的对象是不可变的。
2.     系统的容器类对象 
指NSArray，NSDictionary等。对于容器类本身，上面讨论的结论也是适用的，需要探讨的是复制后容器内对象的变化。
    //copy返回不可变对象，mutablecopy返回可变对象
    NSArray *array1 = [NSArray arrayWithObjects:@"a",@"b",@"c",nil];
    NSArray *arrayCopy1 = [array1 copy];
    //arrayCopy1是和array同一个NSArray对象（指向相同的对象），包括array里面的元素也是指向相同的指针
    NSLog(@"array1 retain count: %d",[array1 retainCount]);
    NSLog(@"array1 retain count: %d",[arrayCopy1 retainCount]);
    NSMutableArray *mArrayCopy1 = [array1 mutableCopy];
    //mArrayCopy1是array1的可变副本，指向的对象和array1不同，但是其中的元素和array1中的元素指向的是同一个对象。mArrayCopy1还可以修改自己的对象
    [mArrayCopy1 addObject:@"de"];
[mArrayCopy1 removeObjectAtIndex:0];
array1和arrayCopy1是指针复制，而mArrayCopy1是对象复制，mArrayCopy1还可以改变期内的元素：删除或添加。但是注意的是，容器内的元素内容都是指针复制。
下面用另一个例子来测试一下。
    NSArray *mArray1 = [NSArray arrayWithObjects:[NSMutableString stringWithString:@"a"],@"b",@"c",nil];
    NSArray *mArrayCopy2 = [mArray1 copy];
    NSLog(@"mArray1 retain count: %d",[mArray1 retainCount]);
    NSMutableArray *mArrayMCopy1 = [mArray1 mutableCopy];
    NSLog(@"mArray1 retain count: %d",[mArray1 retainCount]);
    //mArrayCopy2,mArrayMCopy1和mArray1指向的都是不一样的对象，但是其中的元素都是一样的对象——同一个指针
    //一下做测试
    NSMutableString *testString = [mArray1 objectAtIndex:0];
    //testString = @"1a1";//这样会改变testString的指针，其实是将@“1a1”临时对象赋给了testString
[testString appendString:@" tail"];//这样以上三个数组的首元素都被改变了
由此可见，对于容器而言，其元素对象始终是指针复制。如果需要元素对象也是对象复制，就需要实现深拷贝。[http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/Collections/Articles/Copying.html](http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/Collections/Articles/Copying.html)
    NSArray *array = [NSArray arrayWithObjects:[NSMutableString stringWithString:@"first"],[NSStringstringWithString:@"b"],@"c",nil];
    NSArray *deepCopyArray=[[NSArray alloc] initWithArray: array copyItems: YES];
    NSArray* trueDeepCopyArray = [NSKeyedUnarchiver unarchiveObjectWithData:
    [NSKeyedArchiver archivedDataWithRootObject: array]];
trueDeepCopyArray是完全意义上的深拷贝，而deepCopyArray则不是，对于deepCopyArray内的不可变元素其还是指针复制。或者我们自己实现深拷贝的方法。因为如果容器的某一元素是不可变的，那你复制完后该对象仍旧是不能改变的，因此只需要指针复制即可。除非你对容器内的元素重新赋值，否则指针复制即已足够。举个例子，[[array objectAtIndex:0]appendstring:@”sd”]后其他的容器内对象并不会受影响。[[array
 objectAtIndex:1]和[[deepCopyArray objectAtIndex:0]尽管是指向同一块内存，但是我们没有办法对其进行修改——因为它是不可改变的。所以指针复制已经足够。所以这并不是完全意义上的深拷贝，但是apple的官方文档将其列为deep copy了，并添加了copy和mutablity的关系说明，故在此做一说明（这边还有点疑惑，有了解的赐教啊）。
或者我们自己实现深拷贝的方法（略）。
3.     自定义对象
如果是我们定义的对象，那么我们自己要实现NSCopying,NSMutableCopying这样就能调用copy和mutablecopy了。举个例子：
@interface MyObj : NSObject<NSCopying,NSMutableCopying>
{
         NSMutableString *name;
         NSString *imutableStr;
         int age;
}
@property (nonatomic, retain) NSMutableString *name;
@property (nonatomic, retain) NSString *imutableStr;
@property (nonatomic) int age;
@end
@implementation MyObj
@synthesize name;
@synthesize age;
@synthesize imutableStr;
- (id)init
{
         if (self = [super init])
         {
                   self.name = [[NSMutableString alloc]init];
                   self.imutableStr = [[NSString alloc]init];
                   age = -1;
         }
         return self;
}
- (void)dealloc
{
         [name release];
         [imutableStr release];
         [super dealloc];
}
- (id)copyWithZone:(NSZone *)zone
{
         MyObj *copy = [[[self class] allocWithZone:zone] init];
         copy->name = [name copy];
         copy->imutableStr = [imutableStr copy];
//       copy->name = [name copyWithZone:zone];;
//       copy->imutableStr = [name copyWithZone:zone];//
         copy->age = age;
         return copy;
}
- (id)mutableCopyWithZone:(NSZone *)zone
{
         MyObj *copy = NSCopyObject(self, 0, zone);
         copy->name = [self.name mutableCopy];
         copy->age = age;
         return copy;
}
