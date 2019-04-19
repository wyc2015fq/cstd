# iOS最全性能优化(中) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月14日 09:25:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1207
## 续 性能优化(上)
###### 9. 重用和延迟加载(lazy load) Views
更多的view意味着更多的渲染，也就是更多的CPU和内存消耗，对于那种嵌套了很多view在UIScrollView里边的app更是如此。
这里我们用到的技巧就是模仿`UITableView`和`UICollectionView`的操作: 不要一次创建所有的subview，而是当需要时才创建，当它们完成了使命，把他们放进一个可重用的队列中。
这样的话你就只需要在滚动发生时创建你的views，避免了不划算的内存分配。
创建views的能效问题也适用于你app的其它方面。想象一下一个用户点击一个按钮的时候需要呈现一个view的场景。有两种实现方法：
- 创建并隐藏这个view当这个screen加载的时候，当需要时显示它；
- 当需要时才创建并展示。
每个方案都有其优缺点。
用第一种方案的话因为你需要一开始就创建一个view并保持它直到不再使用，这就会更加消耗内存。然而这也会使你的app操作更敏感因为当用户点击按钮的时候它只需要改变一下这个view的可见性。
第二种方案则相反-消耗更少内存，但是会在点击按钮的时候比第一种稍显卡顿。
###### 10. Cache, Cache, 还是Cache!
一个极好的原则就是，缓存所需要的，也就是那些不大可能改变但是需要经常读取的东西。
我们能缓存些什么呢？一些选项是，远端服务器的响应，图片，甚至计算结果，比如UITableView的行高。
NSURLConnection默认会缓存资源在内存或者存储中根据它所加载的HTTP Headers。你甚至可以手动创建一个NSURLRequest然后使它只加载缓存的值。
下面是一个可用的代码段，你可以可以用它去为一个基本不会改变的图片创建一个NSURLRequest并缓存它：
```
+ (NSMutableURLRequest *)imageRequestWithURL:(NSURL *)url {
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.cachePolicy = NSURLRequestReturnCacheDataElseLoad;// this will make sure the request always returns the cached image
    request.HTTPShouldHandleCookies = NO;
    request.HTTPShouldUsePipelining = YES;
    [request addValue:@"image/*"forHTTPHeaderField:@"Accept"];
    returnrequest;
    }
```
注意你可以通过 NSURLConnection 获取一个URL request， AFNetworking也一样的。这样你就不必为采用这条tip而改变所有的networking代码了。
如果想了解更多关于HTTP caching, NSURLCache, NSURLConnection的相关知识，可以读下这篇文章()
如果你需要缓存其它不是HTTP Request的东西，你可以用NSCache。
NSCache和NSDictionary类似，不同的是系统回收内存的时候它会自动删掉它的内容。
###### 11. 权衡渲染方法
在iOS中可以有很多方法做出漂亮的按钮。你可以用整幅的图片，可调大小的图片，uozhe可以用CALayer， CoreGraphics甚至OpenGL来画它们。
当然每个不同的解决方法都有不同的复杂程度和相应的性能。有一篇Apple UIKit team中的一员Andy Matuschak推荐过的很棒的关于graphic性能的帖子很值得一读。
- 简单来说，就是用事先渲染好的图片更快一些，因为如此一来iOS就免去了创建一个图片再画东西上去然后显示在屏幕上的程序。问题是你需要把所有你需要用到的图片放到app的bundle里面，这样就增加了体积 – 这就是使用可变大小的图片更好的地方了: 你可以省去一些不必要的空间，也不需要再为不同的元素(比如按钮)来做不同的图。
然而，使用图片也意味着你失去了使用代码调整图片的机动性，你需要一遍又一遍不断地重做他们，这样就很浪费时间了，而且你如果要做一个动画效果，虽然每幅图只是一些细节的变化你就需要很多的图片造成bundle大小的不断增大。
- 总得来说，你需要权衡一下利弊，到底是要性能能还是要bundle保持合适的大小。
###### 12. 处理内存警告
一旦系统内存过低，iOS会通知所有运行中app。在官方文档中是这样记述:
如果你的app收到了内存警告，它就需要尽可能释放更多的内存。最佳方式是移除对缓存，图片object和其他一些可以重创建的objects的strong references.
幸运的是，UIKit提供了几种收集低内存警告的方法:
- 在app delegate中使用`applicationDidReceiveMemoryWarning:` 的方法
- 在你的自定义UIViewController的子类(subclass)中覆盖`didReceiveMemoryWarning`
- 注册并接收 UIApplicationDidReceiveMemoryWarningNotification 的通知
一旦收到这类通知，你就需要释放任何不必要的内存使用。
例如，UIViewController的默认行为是移除一些不可见的view， 它的一些子类则可以补充这个方法，删掉一些额外的数据结构。一个有图片缓存的app可以移除不在屏幕上显示的图片。
这样对内存警报的处理是很必要的，若不重视，你的app就可能被系统杀掉。
然而，当你一定要确认你所选择的object是可以被重现创建的来释放内存。一定要在开发中用模拟器中的内存提醒模拟去测试一下。
###### 13. 重用大开销对象
一些objects的初始化很慢，比如NSDateFormatter和NSCalendar。然而，你又不可避免地需要使用它们，比如从JSON或者XML中解析数据。
想要避免使用这个对象的瓶颈你就需要重用他们，可以通过添加属性到你的class里或者创建静态变量来实现。
注意如果你要选择第二种方法，对象会在你的app运行时一直存在于内存中，和单例(singleton)很相似。
下面的代码说明了使用一个属性来延迟加载一个date formatter. 第一次调用时它会创建一个新的实例，以后的调用则将返回已经创建的实例：
```
// in your .h or inside a class extension
@property (nonatomic, strong) NSDateFormatter *formatter;
// inside the implementation (.m)
// When you need, just use self.formatter
- (NSDateFormatter *)formatter {
    if(! _formatter) {
        _formatter = [[NSDateFormatter alloc] init];
        _formatter.dateFormat = @"EEE MMM dd HH:mm:ss Z yyyy";// twitter date format
    }
    return_formatter;
}
```
还需要注意的是，其实设置一个NSDateFormatter的速度差不多是和创建新的一样慢的！所以如果你的app需要经常进行日期格式处理的话，你会从这个方法中得到不小的性能提升。
###### 14. 使用Sprite Sheets
你是一个游戏开发者吗，那么Sprite sheets一定是一个你的最好的朋友了。Sprite sheet可以让渲染速度加快，甚至比标准的屏幕渲染方法节省内存。
我们有两个很好的关于Sprite的教程：
- How To Use Animations and Sprite Sheets in Cocos2D
- How to Create and Optimize Sprite Sheets in Cocos2D with Texture Packer and Pixel Formats
第二个教程涵盖了可能在很大程度上影响你游戏性能的pixel格式的细节。
如果你对于spirte sheet还不是很熟悉，可以看下这两个(youtube)视频SpriteSheets – The Movie, Part 1 和Part 2。视频的作者是创建Sprite sheet很流行的工具之一Texture Packer的作者Andreas Löw。
除了使用Sprite sheets，其它写在这里的建议当然也可以用于游戏开发中。比如你需要很多的Sprite sheets，像敌人，导弹之类的动作类必备元素，你可以重用这些sprites而不用每次都要重新创建。
###### 15. 避免反复处理数据
许多应用需要从服务器加载功能所需的常为JSON或者XML格式的数据。在服务器端和客户端使用相同的数据结构很重要。在内存中操作数据使它们满足你的数据结构是开销很大的。
比如你需要数据来展示一个table view,最好直接从服务器取array结构的数据以避免额外的中间数据结构改变。
类似的，如果需要从特定key中取数据，那么就使用键值对的dictionary。
###### 16. 选择正确的数据格式
从app和网络服务间传输数据有很多方案，最常见的就是JSON和XML。你需要选择对你的app来说最合适的一个。
- 解析JSON会比XML更快一些，JSON也通常更小更便于传输。从iOS5起有了官方内建的JSON deserialization 就更加方便使用了。
- 但是XML也有XML的好处，比如使用SAX 来解析XML就像解析本地文件一样，你不需像解析json一样等到整个文档下载完成才开始解析。当你处理很大的数据的时候就会极大地减低内存消耗和增加性能。
###### 17. 正确设定背景图片
在View里放背景图片就像很多其它iOS编程一样有很多方法:
- 使用UIColor的 colorWithPatternImage来设置背景色；
- 在view中添加一个UIImageView作为一个子View。
如果你使用全画幅的背景图，你就必须使用UIImageView因为UIColor的colorWithPatternImage是用来创建小的重复的图片作为背景的。这种情形下使用UIImageView可以节约不少的内存：
```
// You could also achieve the same result in Interface Builder
UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"background"]];
[self.view addSubview:backgroundView];
```
如果你用小图平铺来创建背景，你就需要用UIColor的colorWithPatternImage来做了，它会更快地渲染也不会花费很多内存：
` self.view.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"background"]];`
###### 18. 减少使用Web特性
UIWebView很有用，用它来展示网页内容或者创建UIKit很难做到的动画效果是很简单的一件事。
但是你可能有注意到UIWebView并不像驱动Safari的那么快。这是由于以JIT compilation 为特色的Webkit的Nitro Engine的限制。
所以想要更高的性能你就要调整下你的HTML了。第一件要做的事就是尽可能移除不必要的javascript，避免使用过大的框架。能只用原生js就更好了。
另外，尽可能异步加载例如用户行为统计script这种不影响页面表达的javascript。
最后，永远要注意你使用的图片，保证图片的符合你使用的大小。使用Sprite sheet提高加载速度和节约内存。
更多相关信息可以看下 WWDC 2012 session #601 – Optimizing Web Content in UIWebViews and Websites on iOS
###### 19. 设定Shadow Path
如何在一个View或者一个layer上加一个shadow呢，QuartzCore框架是很多开发者的选择：
```
#import <QuartzCore/QuartzCore.h>
// Somewhere later ...
UIView *view = [[UIView alloc] init];
// Setup the shadow ...
view.layer.shadowOffset = CGSizeMake(-1.0f, 1.0f);
view.layer.shadowRadius = 5.0f;
view.layer.shadowOpacity = 0.6;
```
看起来很简单，对吧。
可是，坏消息是使用这个方法也有它的问题… Core Animation不得不先在后台得出你的图形并加好阴影然后才渲染，这开销是很大的。
使用shadowPath的话就避免了这个问题：
view.layer.shadowPath = [[UIBezierPath bezierPathWithRect:view.bounds] CGPath];
使用shadow path的话iOS就不必每次都计算如何渲染，它使用一个预先计算好的路径。但问题是自己计算path的话可能在某些View中比较困难，且每当view的frame变化的时候你都需要去update shadow path.
想了解更多可以看看Mark Pospesel的这篇。
###### 20. 优化Table View
Table view需要有很好的滚动性能，不然用户会在滚动过程中发现动画的瑕疵。
为了保证table view平滑滚动，确保你采取了以下的措施:
- 正确使用`reuseIdentifier`来重用cells
- 尽量使所有的view opaque，包括cell自身
- 避免渐变，图片缩放，后台选人
- 缓存行高
- 如果cell内现实的内容来自web，使用异步加载，缓存请求结果
- 使用`shadowPath`来画阴影
- 减少subviews的数量
- 尽量不适用`cellForRowAtIndexPath:`，如果你需要用到它，只用一次然后缓存结果
- 使用正确的数据结构来存储数据
- 尽量使用`rowHeight`, `sectionFooterHeight` 和
`sectionHeaderHeight`来设定固定的高，不要请求delegate
###### 21. 选择正确的数据存储选项
当做本地数据存储时你会怎么做？
你有很多选择，比如：
- 使用`NSUerDefaults`
- 使用XML, JSON, 或者 plist
- 使用NSCoding存档
- 使用类似SQLite的本地SQL数据库
- 使用 Core Data
NSUserDefaults的问题是什么？虽然它很nice也很便捷，但是它只适用于小数据，比如一些简单的布尔型的设置选项，再大点你就要考虑其它方式了
XML这种结构化档案呢？总体来说，你需要读取整个文件到内存里去解析，这样是很不经济的。使用SAX又是一个很麻烦的事情。
NSCoding？不幸的是，它也需要读写文件，所以也有以上问题。
当存储大块数据时,以上的方法都不适用. 在这种应用场景下，使用SQLite 或者 Core Data比较好。使用这些技术你用特定的查询语句就能只加载你需要的对象。
在性能层面来讲，SQLite和Core Data是很相似的。他们的不同在于具体使用方法。Core Data代表一个对象的graph model，但SQLite就是一个DBMS。Apple在一般情况下建议使用Core Data，但是如果你有理由不使用它，那么就去使用更加底层的SQLite吧。
