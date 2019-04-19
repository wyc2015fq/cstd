# [iOS]如何重新架构 JPVideoPlayer ? - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月23日 08:38:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：568
![](http://upload-images.jianshu.io/upload_images/2122663-087e75dd2299af7a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/66638bdfd537](http://www.jianshu.com/p/66638bdfd537)
#### 注意：此文为配合 `JPVideoPlayer` version 2.0 版本发布而写，如果你想了解 2.0 版本的更新内容和所有实现细节，请点击前往 [GitHub](https://github.com/Chris-Pan/JPVideoPlayer)。
> 
##### 导言：我几个月前写了一个在 `UITableView` 中滑动 `UITableViewCell` 播放视频的框架，类似于“新浪微博” 和 “Facebook” 首页视频播放。我也为第一版本的 `JPVideoPlayer` 实现写了两篇文章：
###### 01、[[iOS]仿微博视频边下边播之封装播放器](http://www.jianshu.com/p/0d4588a7540f) 讲述如何封装一个实现了边下边播并且缓存的视频播放器。
###### 02、[[iOS]仿微博视频边下边播之滑动TableView自动播放](http://www.jianshu.com/p/3946317760a6) 讲述如何实现在`UITableView`中滑动播放视频，并且是流畅，不阻塞线程，没有任何卡顿的实现滑动播放视频。同时也将讲述当`UITableView`滚动时，以什么样的策略，来确定究竟哪一个cell应该播放视频。
当时匆忙实现功能，没有仔细斟酌架构的问题，也没有彻底的实现单个功能的组件化。而且由于架构设计不足带来一些不可避免的问题。这些问题，1.0 版本的用户应该有所体会。前段时间独自回了一趟老家，车程比较长，而且可以专注而不被打扰，在车上写了一个 2.0 版本。2.0 版本有部分内容模仿了`SDWebImage` 的设计。
##### 2.0 版本效果如下：
![](http://upload-images.jianshu.io/upload_images/2122663-98368842707af8f1.gif?imageMogr2/auto-orient/strip)
### 01.JPVideoPlayer Version 2.0 如何使用？
考虑到 API 和架构都重新设计了，尤其是 API 全部重新设计，1.0 版本的 API 已经不能沿用了，机智的我已经做好了被骂得很惨的心理准备了。
如果你是新的用户，就不会受之前版本的影响。最终不管你是新用户还是老用户，你一定会喜欢这种新的 API 设计的，因为这是目前 iOS 最受欢迎的 API 设计。
```
Objective-C:
#import <UIView+WebVideoCache.h>
...
NSURL *url = [NSURL URLWithString:@"http://lavaweb-10015286.video.myqcloud.com/%E5%B0%BD%E6%83%85LAVA.mp4"];
[aview jp_playVideoWithURL:url];
```
### 02.JPVideoPlayer Version 2.0 内部运行细节？
很多用户可能不会有时间去读源码，但是也许会关心 2.0 版本内部实现的一些大的方式，所以这里我总结了一张 2.0 版本的实现大致结构图表，如下：
![](http://upload-images.jianshu.io/upload_images/2122663-69aa406de370d404.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1200)
下面我用文字来表述一下框架内部的运作顺序：
- 
01.提供给外部调用的 API 非常简洁，采用为 `UIView` 添加分类方法的形式为外界调用，只要导入了头文件，所有 `UIView` 的子类都拥有播放视频的方法。框架把 `UIView`的分类方法作为和框架内部交互的桥梁。
- 
02.`JPVideoPlayerManager` 负责甄别用户传过来的 URL，根据不同的 URL 作出不同的反应进行视频播放。具体细节如下：
- 
02.1、是否是本地文件路径，如果是本地路径，直接把路径给负责视频播放的工具类 `JPVideoPlayerPlayVideoTool` 进行视频播放；
- 
02.2、如果不是本地路径，再根据 URL 生成缓存的 key 给 `JPVideoPlayerCache` 工具类查找是否有本地缓存文件，如果有缓存就把缓存路径返还给`JPVideoPlayerManager`，`JPVideoPlayerManager` 会把路径给负责视频播放的工具类 `JPVideoPlayerPlayVideoTool` 进行视频播放；
- 
02.3、如果没有本地缓存，就把 URL 给 `JPVideoPlayerDownloader` 下载工具类，这个工具类就会去网络上下载视频数据，每下载完一段数据，都会返回给 `JPVideoPlayerManager`，`JPVideoPlayerManager` 会先把这段数据给 `JPVideoPlayerCache`， `JPVideoPlayerCache` 先把数据缓存到磁盘，然后再把缓存的路径返还给`JPVideoPlayerManager`，`JPVideoPlayerManager` 会把路径给负责视频播放的工具类 `JPVideoPlayerPlayVideoTool` 进行视频播放。
### 03. JPVideoPlayer Version 2.0 更新了哪些内容？
|类名|功能点|
|----|----|
|`JPVideoPlayerDownloaderOperation`|下载单个视频文件工具|
|`JPVideoPlayerDownloader`|下载工具类，管理下载操作队列|
|`JPVideoPlayerCachePathTool`|管理临时和完整视频存储路径|
|`JPVideoPlayerCacheConfig`|缓存配置文件，包括缓存周期，最大磁盘缓存等|
|`JPVideoPlayerCache`|缓存工具类，负责视频数据的存、取、删、更新|
|`JPVideoPlayerResourceLoader`|视频播放器的数据代理，负责将网络视频数据填充给播放器|
|`JPVideoPlayerPlayVideoTool`|视频播放工具类|
|`JPVideoPlayerManager`|管理者，协调各个模块相互配合工作|
接下来我将大致描述一下每个类的实现：
- 
01.`JPVideoPlayerDownloaderOperation`：它继承自 `NSOperation`，它内部持有一个 `NSURLSession` 实例对象，由这个实例对象去负责下载视频数据，`JPVideoPlayerDownloaderOperation` 成为这个实例对象的代理，监听获取下载到的数据，并将获得的数据回传给操作的所有者 `JPVideoPlayerDownloader`。
- 
02.`JPVideoPlayerDownloader`：它持有一个下载队列，下载队列里存放的是 `JPVideoPlayerDownloaderOperation` 实例对象。考虑到播放视频数据量较大而且是持续的，为了将所有的网络资源用于加载当前用户播放的视频，提升使用体验，所以这个队列在任何时候都只允许一个下载操作运行。它接收到 `JPVideoPlayerDownloaderOperation` 回调的数据以后会继续把数据回传出去。
- 
03.`JPVideoPlayerCachePathTool`：它负责管理缓存文件的路径，包括临时和完整视频存储路径两个部分。
- 
04.`JPVideoPlayerCacheConfig`：这个类存放着缓存存储周期，最大磁盘缓存等缓存配置数据。
- 
05.`JPVideoPlayerCache`：它负责数据的存、取、删、更新等功能。当调用存数据的功能时，会在临时文件存放的文件夹内新建一个
 mp4 文件，并开始将数据写入到这个文件内，存完一段数据以后，会将存储数据的路径回调出去。每次播放视频的时候，都会去查询缓存中有没有这个 URL 的完整数据缓存，如果有就会把路径回调出去。它还对外提供获取缓存大小的功能，如果你需要在设置里获取当前视频缓存，可以调用这个接口。它还有删除缓存视频数据的功能。
- 
06.`JPVideoPlayerResourceLoader` 这个类沿用自 1.0 版本，负责从已经下载的数据中找出播放器需要的数据，并将数据填充到播放器的数据请求中进行视频播放。
- 
07.`JPVideoPlayerPlayVideoTool` 这个类持有一个视频播放器，负责串联从视频数据到图像和影音的整个播放功能。
- 
08.`JPVideoPlayerManager` 它是框架的管理者、大管家，它的具体功能上面“内部细节”已经陈述过了，这里不重复了。
- 
09.其他还有一个数据加载进度条和一个缓冲状态指示器，这些都很简单。还有就是 2.0 版本的 Demo 将 `UITableViewController` 视频播放抽到一个分类中集中管理，也方便后期维护，如果你的需求和
 Demo 类似，可以考虑直接把这个分类拽进项目，需要改动的代码很少，就能实现。
### 04.为什么要这么改？
这个问题我想从用户和我自己，还有它本来应该是什么样子这些维度来分析。
##### 04.1、用户角度
- 
01.首先，受用户欢迎的框架应该要具备两个基本的素质。第一，调用起来方便，能一行代码解决的，绝不搞两行。第二，不侵入用户的项目，万一哪天框架不维护了，也不要成为用户的累赘。
- 
02.其次，我说一个我今天看一个框架的感受，是一个类似微信选择多张照片的框架，框架对外提供的接口不够我用，所以我只能去框架内部改，但是当我找一个功能的时候，框架文档没有说明，我在那些代码应该放的位置也没有找到，需求就是改字体和颜色，我找了一上午，终于在一个不可能猜到的位置找到了那几行代码。这里就有几个问题点：
> - 我们对外提供的接口尽量让使用者够用，如果没有考虑到，那用户就可能会来框架里改。
- 我们的框架类的命名，方法的命名都应该遵守苹果的那套标准，因为大家天天在用的都是苹果 API，如果完全不遵守苹果那套，那用户来到框架里就是一头雾水，这里有一个沟通成本的问题，看懂代码之前还要先熟悉我们特有的标准。就像去美国，要和美国人说话，要先学英语。
- 文档一定要详实，每个人水平都不一样，代码可能看不懂，但是文字谁都认识，这是作者和用户沟通的基础。
##### 04.2、作者角度
维护框架其实也是很花时间的一件事情，1.0 版本的时候，就经常有用户给我留言，发QQ消息和我沟通实现的细节，还要他们希望下个版本希望加进去的一些功能。
当时很多功能的代码都混在一个类里，第一就是这个类上千行代码，我自己要改一个东西都需要用搜索功能才能找到，各个方法之间相互调用的时候跳来跳去，头晕眼花。
现在每个模块划分完功能以后，每个功能的核心代码都高度集中在对应的类里，隐藏实现的细节，屏蔽了外部的干扰，只对外提供必要的接口。现在调试问题的时候，分析到出现问题可能的模块以后，能快速定位到对应类的对应方法里，只需要在当前类里专注当前的问题就可以了，不需要考虑外部的影响。这个效率的提升还是蛮明显的。
所以从作者的角度，这个架构的好处就是，第一，方便我后期的维护，提高效率；第二，方便和用户的沟通，减少沟通成本；第三，当有新功能的时候，我能快速的把代码写到对应的类别里。
##### 04.3、它本来的样子
前段时间看 BBC 的 “Planet Earth” 纪录片，里面说蚂蚁巢穴里有几千万只蚂蚁，但是却分工明确，秩序尽然。原来，蚂蚁分为四类：
- 蚁后，也叫蚁皇，是一族之主，专管产卵繁殖，一般一群只有一个，体型特大，行动不便，由工蚁侍候。
- 雄蚁，专与蚁后交配，交配后即死亡，一群中有数十只或数百只，要看蚁群的大小。
- 工蚁，是蚁群中的主要成员，专司觅食、饲养幼蚁、侍候蚁后、搬家清扫等等杂勤工作。
- 兵蚁，个头较大，双颚发达，是蚁群中的保卫者，担负着本蚁群的安全，如有外蚁入侵，或争夺食物时，必誓死决斗。
我们的代码或许也可以仿照大自然，先划分功能，再列出几个类，将功能点挨个集中到类里，武装出类，就是所谓的对象。这就是我理解的框架应该有的美。
## 我的文章集合
> 
下面这个链接是我所有文章的一个集合目录。这些文章凡是涉及实现的，每篇文章中都有 [Github](https://github.com/Chris-Pan) 地址，[Github](https://github.com/Chris-Pan) 上都有源码。如果某篇文章刚好在你的实际开发中帮到你，又或者提供一种不同的实现思路，让你觉得有用，那就看看这句话
 “坚持每天点赞的人，99%都是帅哥美女，再也不用单身了”
[我的文章集合索引](http://www.jianshu.com/p/e03cd37db0d5)
###### 你还可以关注我自己维护的简书专题 [](http://www.jianshu.com/users/e2f2d779c022/latest_articles)[iOS开发心得](http://www.jianshu.com/collection/72d7b853d415)。这个专题的文章都是实打实的干货。
###### 如果你有问题，除了在文章最后留言，还可以在微博 [@盼盼_HKbuy](http://weibo.com/u/5590458451/home?wvr=5)上给我留言，以及访问我的 [Github](https://github.com/Chris-Pan)。
