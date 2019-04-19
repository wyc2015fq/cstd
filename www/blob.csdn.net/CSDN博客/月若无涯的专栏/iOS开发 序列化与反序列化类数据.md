# iOS开发 序列化与反序列化类数据 - 月若无涯的专栏 - CSDN博客
2013年08月28日 23:29:58[月若无涯](https://me.csdn.net/u010124617)阅读数：1160
对于任何一门语言来说，序列化和反序列化操作都是很重要的部分。在iOS开发当中，经常可能会遇到把某个类处理成可存储的数据，以便之后使用（或者再恢复成之前的对象）。
这里我直接进入正题，给出三种解决方案：
第一种：使用iOS框架中的NSKeyedArchiver，使用这个的教程网上太多了，概括起来就是对需要序列化和反序列化的类实现NSCoding协议的两个方法
```
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (id)initWithCoder:(NSCoder *)aDecoder;
```
之后通过NSKeyedArchiver进行相应的操作即可。优势是比较简单易处理，能满足保存一个对象并恢复的需求，可以指定保存的类型为xml等等。
第二种：对需要自定义序列化的类进行特殊处理
比如我希望A类能序列化成JSON数据，然后再通过JSONString反序列化回来，这时候上面的那种方式已经满足不了需求了，可以相应的在A类里面添加如
```
-(NSString*)toJSONString;
-(void)fromJSONString;
```
然后自己去针对性的实现，这种方式总体上是比上面那种要更加灵活，输出格式也可以自己任意定制，简而言之就是根据自己的类针对性的写相应的序列化和反序列化方法。
上面这两种方式都能实现序列化和反序列化，优势是对不同的类很有针对性，缺点也比较明显，每次你需要对一个新的类做序列化和反序列化时，都要自己去手动实现相应的方法才能使用。如果有100个类需要序列化，难道我们要100个类都挨个写一遍么？
第三种：使用通用的序列化和反序列化方法。
所谓通用方法，即一套可以对任意类进行处理的方法。这里我先给出思路，同样是针对性的对类的所有Property进行遍历处理，存储输出为你想要的通用格式。
可供使用的技术包括Category、KVC、Reflect等等。
通用的序列化和反序列化的Category，这里我已经开源了。供大家研究和学习。能把一个对象转换为NSDictionary，此Dictionary可以被顺利的JSON或其他存储，如NSUserDefaults等。
使用方法见Demo。
[https://github.com/YueRuo/NSObject-YRSerialization](https://github.com/YueRuo/NSObject-YRSerialization)
目前不支持链式存储的对象。如A类中有一个Property也是A类。
另外对NSValue、NSString、NSSet、NSArray、NSDictionary对象不支持直接转换。这些类也不需要此操作。
NSCoding
