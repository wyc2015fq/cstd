# iOS最全性能优化(上) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月14日 09:23:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：569
### 导读
性能对 iOS 应用的开发尤其重要，如果你的应用失去反应或者很慢，失望的用户会把他们的失望写满App Store的评论。然而由于iOS设备的限制，有时搞好性能是一件难事。开发过程中你会有很多需要注意的事项，你也很容易在做出选择时忘记考虑性能影响。
iOS最全性能优化(上)[http://www.jianshu.com/p/9c450e512020](http://www.jianshu.com/p/9c450e512020)
iOS最全性能优化(中)[http://www.jianshu.com/p/ad90e90dce4f](http://www.jianshu.com/p/ad90e90dce4f)
iOS最全性能优化(下)[http://www.jianshu.com/p/25e2f1ae51d2](http://www.jianshu.com/p/25e2f1ae51d2)
请耐心读完这篇文章，为你未来的app提个速！
### 目录
##### 我要给出的建议将分为三个不同的等级： 入门级、 中级和进阶级：
- 
###### 入门级（这是些你一定会经常用在你app开发中的建议）
```
1. 用ARC管理内存
2. 在正确的地方使用reuseIdentifier
3. 尽可能使Views透明
4. 避免庞大的XIB
5. 不要block主线程
6. 在Image Views中调整图片大小
7. 选择正确的Collection
8. 打开gzip压缩
```
- 
###### 中级（这些是你可能在一些相对复杂情况下可能用到的）
```
9. 重用和延迟加载Views
10. Cache, Cache, 还是Cache！
11. 权衡渲染方法
12. 处理内存警告
13. 重用大开销的对象
14. 使用Sprite Sheets
15. 避免反复处理数据
16. 选择正确的数据格式
17. 正确地设定Background Images
18. 减少使用Web特性
19. 设定Shadow Path
20. 优化你的Table View
21. 选择正确的数据存储选项
```
- 
###### 进阶级（这些建议只应该在你确信他们可以解决问题和得心应手的情况下采用）
```
22. 加速启动时间
23. 使用Autorelease Pool
24. 选择是否缓存图片
25. 尽量避免日期格式转换
```
无需赘述，让我们进入正题吧~
初学者性能提升
这个部分致力于一些能提高性能的基本改变。但所有层次的开发者都有可能会从这个记录了一些被忽视的项目的小小的性能备忘录里获得一些提升。
##### 1.用ARC管理内存
- 
ARC(Automatic Reference Counting, 自动引用计数)和iOS5一起发布，它避免了最常见的也就是经常是由于我们忘记释放内存所造成的内存泄露。它自动为你管理retain和release的过程，所以你就不必去手动干预了。
下面是你会经常用来去创建一个View的代码段:
```
UIView *view = [[UIView alloc] init];
   // ...
  [self.view addSubview:view];
   [view release];
```
- 
忘掉代码段结尾的release简直像记得吃饭一样简单。而ARC会自动在底层为你做这些工作。除了帮你避免内存泄露，ARC还可以帮你提高性能，它能保证释放掉不再需要的对象的内存。这都啥年代了，你应该在你的所有项目里使用ARC!
##### 2.在正确的地方使用 reuseIdentifier
- 一个开发中常见的错误就是没有给UITableViewCells， UICollectionViewCells，甚至是UITableViewHeaderFooterViews设置正确的reuseIdentifier。
- 为了性能最优化，table view用 `tableView:cellForRowAtIndexPath:` 为rows分配cells的时候，它的数据应该重用自UITableViewCell。 一个table view维持一个队列的数据可重用的UITableViewCell对象。不使用reuseIdentifier的话，每显示一行table view就不得不设置全新的cell。这对性能的影响可是相当大的，尤其会使app的滚动体验大打折扣。
- 自iOS6起，除了UICollectionView的cells和补充views，你也应该在header和footer views中使用reuseIdentifiers
##### 3.尽量把views设置为不透明
- 如果你有透明的Views你应该设置它们的opaque(不透明)属性为YES。例如一个黑色半透明的可以设置为一个灰色不透明的View替代.原因是这会使系统用一个最优的方式渲染这些views。这个简单的属性在IB或者代码里都可以设定。
- Apple的文档对于为图片设置透明属性的描述是：
(opaque)这个属性给渲染系统提供了一个如何处理这个view的提示。如果设为YES， 渲染系统就认为这个view是完全不透明的，这使得渲染系统优化一些渲染过程和提高性能。如果设置为NO，渲染系统正常地和其它内容组成这个View。默认值是YES。
- 在相对比较静止的画面中，设置这个属性不会有太大影响。然而当这个view嵌在scroll view里边，或者是一个复杂动画的一部分，不设置这个属性的话会在很大程度上影响app的性能。
#### 4. 避免过于庞大的XIB
- iOS5中加入的Storyboards(分镜)正在快速取代XIB。然而XIB在一些场景中仍然很有用。比如你的app需要适应iOS5之前的设备，或者你有一个自定义的可重用的view,你就不可避免地要用到他们。
- 如果你不得不XIB的话，使他们尽量简单。尝试为每个Controller配置一个单独的XIB，尽可能把一个View Controller的view层次结构分散到单独的XIB中去。
需要注意的是，当你加载一个XIB的时候所有内容都被放在了内存里，包括任何图片。如果有一个不会即刻用到的view，你这就是在浪费宝贵的内存资源了。Storyboards就是另一码事儿了，storyboard仅在需要时实例化一个view controller.
- 当你加载一个引用了图片或者声音资源的nib时，nib加载代码会把图片和声音文件写进内存。在OS X中，图片和声音资源被缓存在named cache中以便将来用到时获取。在iOS中，仅图片资源会被存进named caches。取决于你所在的平台，使用NSImage 或UIImage 的`imageNamed:`方法来获取图片资源。
##### 5. 不要阻塞主线程
- 
永远不要使主线程承担过多。因为UIKit在主线程上做所有工作，渲染，管理触摸反应，回应输入等都需要在它上面完成。一直使用主线程的风险就是如果你的代码真的block了主线程，你的app会失去反应
- 
大部分阻碍主进程的情形是你的app在做一些牵涉到读写外部资源的I/O操作，比如存储或者网络。或者使用像 AFNetworking这样的框架来异步地做这些操作。
如果你需要做其它类型的需要耗费巨大资源的操作(比如时间敏感的计算或者存储读写)那就用 Grand Central Dispatch，或者 NSOperation 和 NSOperationQueues.
你可以使用`NSURLConnection`异步地做网络操作:
`  + (void)sendAsynchronousRequest:(NSURLRequest *)request queue:(NSOperationQueue *)queue completionHandler:(void (^)(NSURLResponse*, NSData*, NSError*))handler`
##### 6. 在Image Views中调整图片大小
- 如果要在`UIImageView`中显示一个来自bundle的图片，你应保证图片的大小和UIImageView的大小相同。在运行中缩放图片是很耗费资源的，特别是`UIImageView`嵌套在`UIScrollView`中的情况下。
- 如果图片是从远端服务加载的你不能控制图片大小，比如在下载前调整到合适大小的话，你可以在下载完成后，最好是用background thread，缩放一次，然后在UIImageView中使用缩放后的图片。
##### 7. 选择正确的Collection
学会选择对业务场景最合适的类或者对象是写出能效高的代码的基础。当处理collections时这句话尤其正确。
Apple有一个 Collections Programming Topics 的文档详尽介绍了可用的classes间的差别和你该在哪些场景中使用它们。这对于任何使用collections的人来说是一个必读的文档。
呵呵，我就知道你因为太长没看…这是一些常见collection的总结：
- Arrays: 有序的一组值。使用index来lookup很快，使用value lookup很慢， 插入/删除很慢。
- Dictionaries: 存储键值对。 用键来查找比较快。
- Sets: 无序的一组值。用值来查找很快，插入/删除很快。
#### 8. 打开gzip压缩
- 大量app依赖于远端资源和第三方API，你可能会开发一个需要从远端下载XML, JSON, HTML或者其它格式的app。
- 问题是我们的目标是移动设备，因此你就不能指望网络状况有多好。一个用户现在还在edge网络，下一分钟可能就切换到了3G。不论什么场景，你肯定不想让你的用户等太长时间。
- 减小文档的一个方式就是在服务端和你的app中打开gzip。这对于文字这种能有更高压缩率的数据来说会有更显著的效用。好消息是，iOS已经在NSURLConnection中默认支持了gzip压缩，当然AFNetworking这些基于它的框架亦然。像Google App Engine这些云服务提供者也已经支持了压缩输出。
