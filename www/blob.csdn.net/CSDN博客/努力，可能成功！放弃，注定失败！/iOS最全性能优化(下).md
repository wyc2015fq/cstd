# iOS最全性能优化(下) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月14日 09:27:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2435
## 续 性能优化(中)
###### 22. 加速启动时间
快速打开app是很重要的，特别是用户第一次打开它时，对app来讲，第一印象太太太重要了。
- 你能做的就是使它尽可能做更多的异步任务，比如加载远端或者数据库数据，解析数据。
还是那句话，避免过于庞大的XIB，因为他们是在主线程上加载的。所以尽量使用没有这个问题的Storyboards吧！
注意，用Xcode debug时watchdog并不运行，一定要把设备从Xcode断开来测试启动速度
###### 23. 使用Autorelease Pool
`NSAutoreleasePool`负责释放block中的autoreleased objects。一般情况下它会自动被UIKit调用。但是有些状况下你也需要手动去创建它。
假如你创建很多临时对象，你会发现内存一直在减少直到这些对象被release的时候。这是因为只有当UIKit用光了autorelease pool的时候memory才会被释放。
好消息是你可以在你自己的@autoreleasepool里创建临时的对象来避免这个行为：
```
NSArray *urls = <# An array of file URLs #>;
    for(NSURL *url in urls) {
        @autoreleasepool {
            NSError *error;
            NSString *fileContents = [NSString stringWithContentsOfURL:url
                                             encoding:NSUTF8StringEncoding error:&error];
            /* Process the string, creating and autoreleasing more objects. */
        }
    }
```
这段代码在每次遍历后释放所有autorelease对象
更多关于NSAutoreleasePool请参考官方文档。
###### 24. 选择是否缓存图片
常见的从bundle中加载图片的方式有两种，一个是用`imageNamed`，二是用`imageWithContentsOfFile`，第一种比较常见一点。
既然有两种类似的方法来实现相同的目的，那么他们之间的差别是什么呢？
`imageNamed`的优点是当加载时会缓存图片。`imageNamed`的文档中这么说:
这个方法用一个指定的名字在系统缓存中查找并返回一个图片对象如果它存在的话。如果缓存中没有找到相应的图片，这个方法从指定的文档中加载然后缓存并返回这个对象。
相反的，`imageWithContentsOfFile`仅加载图片。
下面的代码说明了这两种方法的用法:
```
UIImage *img = [UIImage imageNamed:@"myImage"];// caching
// or
UIImage *img = [UIImage imageWithContentsOfFile:@"myImage"];// no caching
```
那么我们应该如何选择呢？
如果你要加载一个大图片而且是一次性使用，那么就没必要缓存这个图片，用`imageWithContentsOfFile`足矣，这样不会浪费内存来缓存它。
然而，在图片反复重用的情况下`imageNamed`是一个好得多的选择。
###### 25. 避免日期格式转换
如果你要用`NSDateFormatter`来处理很多日期格式，应该小心以待。就像先前提到的，任何时候重用`NSDateFormatters`都是一个好的实践。
然而，如果你需要更多速度，那么直接用C是一个好的方案。Sam Soffes有一个不错的帖子([http://soff.es/how-to-drastically-improve-your-app-with-an-afternoon-and-instruments)里面有一些可以用来解析ISO-8601日期字符串的代码，简单重写一下就可以拿来用了。](http://soff.es/how-to-drastically-improve-your-app-with-an-afternoon-and-instruments)%E9%87%8C%E9%9D%A2%E6%9C%89%E4%B8%80%E4%BA%9B%E5%8F%AF%E4%BB%A5%E7%94%A8%E6%9D%A5%E8%A7%A3%E6%9E%90ISO-8601%E6%97%A5%E6%9C%9F%E5%AD%97%E7%AC%A6%E4%B8%B2%E7%9A%84%E4%BB%A3%E7%A0%81%EF%BC%8C%E7%AE%80%E5%8D%95%E9%87%8D%E5%86%99%E4%B8%80%E4%B8%8B%E5%B0%B1%E5%8F%AF%E4%BB%A5%E6%8B%BF%E6%9D%A5%E7%94%A8%E4%BA%86%E3%80%82)
嗯，直接用C来搞，看起来不错了，但是你相信吗，我们还有更好的方案！
如果你可以控制你所处理的日期格式，尽量选择Unix时间戳。你可以方便地从时间戳转换到NSDate:
```
- (NSDate*)dateFromUnixTimestamp:(NSTimeInterval)timestamp {
 return[NSDate dateWithTimeIntervalSince1970:timestamp];
 }
```
这样会比用C来解析日期字符串还快！
需要注意的是，许多web API会以微秒的形式返回时间戳，因为这种格式在javascript中更方便使用。记住用`dateFromUnixTimestamp`之前除以1000就好了。
