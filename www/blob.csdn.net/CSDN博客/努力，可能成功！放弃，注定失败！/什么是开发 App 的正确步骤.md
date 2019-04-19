# 什么是开发 App 的正确步骤? - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月17日 09:38:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：370标签：[iOS 资源大全																[iOS学习资料																[iOS常用方法](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
本文来自简书，原文地址:[http://www.jianshu.com/p/4ef3d3737661](http://www.jianshu.com/p/4ef3d3737661)
在 iOS 开发中，写一个 App 很容易，但是要写好一个 App，确是要下另一番功夫。首先，我们来看一个 App 的开发要求：
> 
写一个 App，显示出 Spotify 上 Lady Gaga 相关的所有音乐专辑，相关信息可以通过以下网址查到：
[https://api.spotify.com/v1/search?q=lady+gaga&type=album](https://api.spotify.com/v1/search?q=lady+gaga&type=album)
## 需求分析
首先拿到开发要求，最重要的是明确开发细节。这里面有很多我们不清楚的地方需要与产品经理和设计师交流：显示是要用 TableView 还是 CollectionView？每个音乐专辑的哪些信息需要显示？如果专辑数量过多，我们优先显示哪些专辑？这个 App 除了显示信息以外，还需要哪些拓展功能？这个产品的大小是否有要求？需要多少天完成？
经过讨论之后，大家的一致意见是做个如下的 App:
![](http://upload-images.jianshu.io/upload_images/1721232-e3de3b984d4658c7.gif?imageMogr2/auto-orient/strip)
LadyGaga
于是我们就清楚了，是要做一个 tableView，每个 Cell 对应一个专辑信息，左边是图片，右边是专辑名。点击 Cell，可以看到相应的专辑大图。
## 构建架构
首先这个 App 比较简单，我们只要用最基本的 MVC 就可做好。
- 
Model 部分：
只需要一个 Model， 为 Album，对应每一个专辑的信息；
- 
View 部分：
主体的部分可以在 Storyboard 里面完成；
最好单独新建一个 UITableViewCell 的子类，用来对应设置专辑的UI；
- 
ViewController 部分：
其中一个 ViewController 为 TableViewController，负责现实所有专辑的信息；
另一个 ViewController 负责展示 detail info，比如专辑的大图；
- 
Network 部分：
负责从网络上 fetch 专辑信息；以及根据专辑的图片网址，fetch 图片数据；
![](http://upload-images.jianshu.io/upload_images/1721232-6f63a76508c7afb2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
基本架构
## 细节实现
- Network 部分：
fetchAlbums 和 downloadImage 都用Apple 自带的 URLSession 和 JSONserialization 就可以实现，或者也可以用优秀的第三方库 AlamoFire。因为这个 App 比较简单，AlamoFire 优势不明显，且引入第三方库会增加 App 的体积，故而推荐使用前者。基本上就是实现下面两个函数：
```
func fetchAlbums(with url: String, completion : @escaping (_ albums: [Album]?, _ error : NSError?) -> Void) 
func downloadImage(_ url: String) -> UIImage?
```
对于第一个函数 `fetchAlbums`，因为网络请求是耗时耗力的工作，我们一般会将它们用后台线程而非主线程（UI线程）来处理，这样可以保持UI的流畅运行。用闭包则是为了异步多线程完成后可以回调，同时
 error 是为了监视网络请求是否出错。
对于第二个函数 `downloadImage`，最简单的方法是通过 url 拿到对应的 data，然后通过相应的
 data 拿到 image。返回为 optional 的原因是有可能 URL 有问题或者网络请求出错，此时返回 nil。
从API设计的角度来说，以上的`downloadImage`并不是最佳设计。最佳的设计是我们能知道哪里出错了，比如下面这样：
```
enum DownloadImageError: Error {
  case InvalidURL
  case InvalidData
}
func downloadImage(_ url: String) throws -> UIImage {
  guard let aUrl = URL(string: url) else {
    throw DownloadImageError.InvalidURL
  }
  do {
    let data = try Data(contentsOf: aUrl)
    if let image = UIImage(data: data) {
      return image
    } else {
      throw DownloadImageError.InvalidData
    }
  } catch {
    throw DownloadImageError.InvalidURL
  }
}
```
- ViewController 部分：
对于 AlbumsController，我们用到了代理模式(Delegate)，即将 tableView 代理到了 AlbumsController 上。我们只要实现相应的 dataSource 和 delegate 方法即可。其中对于 dataSource 而言，有两个方法是必须实现的，它们是：
```
func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int
func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell
```
同时，AlbumsController 里面，还有两个数组，一个用来装专辑（[Album]），一个用来装图片([UIImage?])，这样我们只需下载数据一次，并将其存入相应数组，之后就无需再次进行相关的网络请求了。也就是说，这两个数组起到了缓存的作用。
具体的实现是：首先在 `viewDidLoad()` 中请求服务器取出相应的数据。之后根据专辑数量设定 TableView
 的相应行数。在具体的一行当中，我们可以根据 indexPath 确定相应的专辑。根据相应专辑的图片 URL ，我们可以拿到相应的图片，之后缓存进图片数组。由于我们复用了 TableView 的 Cell，所以如果不缓存图片而每次去进行网络请求，会因为延时很严重而会造成图片闪烁的后果。
最后两个 ViewController 之间的跳转可以用 navigationController 来实现。
- View 部分：
自定义 AlbumCell 可以保证 App 的扩展性很好。同时，为了处理有些专辑名字过长 Label 显示不了的问题，可以用 autoshrink 来处理。
![](http://upload-images.jianshu.io/upload_images/1721232-4d4e695904c573e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
App 运行流程
## 优化拓展
上面的设计和实现比较理想化，现在我们要考虑一个边界情况，假如网络不稳定，怎么办？
一个简单的解决方法就是，当网络好的时候把数据下载下来，存入 cache 和 storage 中，之后即使网络中断、App 崩溃，我们都能从 storage 中拿到相应数据。
这里引入外观模式（Facade），创建一个新的 class 名为 LibraryAPI，提供两个接口：
```
func getAlbums(completion : @escaping (_ albums: [Album]?, _ error : NSError?) -> Void)
func getImage(_ url: String) throws -> UIImage
```
这里的方法跟之前 Network 的不同之处在于：getAlbums 方法会先尝试从 storage 中取出相应数据，如果没有，则去访问 Network，之后再把从 Network 中拿到的值存入 storage 中。这里面的实现有点复杂，牵涉到两大模块和多线程操作，但是我们并不必关心方法内部的实现，而仅仅关心接口，这就是外观模式的优点。同时，LibraryAPI 这个 class 最好用单例模式(singleton)，因为它应该被当做是全局
 API 被各个 ViewController 来访问，同时这样设计也节省资源。
![](http://upload-images.jianshu.io/upload_images/1721232-8b05f96d2fb37cd9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
优化后的 App 流程
另外一个优化点在于，假如我们一开始拿到很多数据 —— 例如10000 个专辑，那么我们该怎么操作？
正确的做法是分页。我们可以先只拿20个，显示在 TableView 上。当用户快滑到底端的时候，我们可以再取下面20个，然后我们总共有40个在内存中可以显示，以此类推。这样做的好处是，我们无需下载所有的数据，以最快、最流畅的方式布局 TableView，同时根据用户的需求增加相应的专辑数据。
最后一个优化点在于，假如用户上下滑动很快，我们如何能够用最快速度加载图片？
答案是用 operationQueue 来处理，当前 cell 是可见的时候，我们就 resume 下载图片的进程，否则就 suspend。这样保证了我们用有限的内存和 CPU 去最高效的下载用户需要、当前要见的图片。
值得一提的是，大家还可以借鉴 ASDK 的思路来进一步优化程序。
## 总结
本文从一个简单的 tableView App 说起，谈论了开发一个 App 的4个步骤：需求分析、构建架构、细节实现、优化拓展。简单介绍了多线程和几种设计模式，希望对大家有所帮助。
