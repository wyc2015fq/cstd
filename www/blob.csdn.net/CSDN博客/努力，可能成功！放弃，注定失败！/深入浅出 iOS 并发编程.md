# 深入浅出 iOS 并发编程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:06:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：83标签：[深入浅出iOS并发编程](https://so.csdn.net/so/search/s.do?q=深入浅出iOS并发编程&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
> 
本文是我在上海 T 沙龙4月7日分享内容的文字版总结和拓展。相关视频和文档请见链接：[深入浅出 iOS 并发编程](https://speakerdeck.com/soapyigu/shen-ru-qian-chu-ios-bing-fa-bian-cheng)
其中主要内容包括：GCD与Operation的用法、并发编程中常见的问题、使用Operation进行流程化开发示范。
# 什么是并发编程
在大多数场景下，我们所写的代码是逐行顺序执行——在固定的时段内，程序只执行一个任务。而所谓并发编程，就是指在固定的时段内，程序执行多个任务。举个例子，当我们在微博 App 的首页滑动浏览时，微博也在从网络端预加载新的内容或者图片。并发编程可以充分利用硬件性能，合理分配软件资源，带来优秀的用户体验。在 iOS 开发中，我们主要依靠 GCD 和 Operation 来操作线程切换、异步操作，从而实现并发编程。
![](https://upload-images.jianshu.io/upload_images/1721232-5c577ee274f9953c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)
新闻类App首页经常需要同时处理 UI 显示、内容加载、缓存等多个任务
在 iOS 并发编程中，我们要知道这几个基本概念：
- 串行（Serial）：在固定时间内只能执行单个任务。例如主线程，只负责 UI 显示。
- 并发（Concurrent）：在固定时间内可以执行多个任务。注意，它和并行（Parallel）的区别在于，并发不会同时执行多个任务，而是通过在任务间不断切换去完成所有工作。
- 同步（Sync）：会把当前的任务加入到队列中，除非该任务执行完成，线程才会返回继续运行，也就是说同步会阻塞线程。任务在执行和结束一定遵循先后顺序，即先执行的任务一定先结束。
- 异步（Async）：会把当前的任务加入到队列中，但它会立刻返回，无需等任务执行完成，也就是说异步不会阻塞线程。任务在执行和结束不遵循先后顺序。可能先执行的任务先结束，也可能后执行的任务先结束。
为了进一步说明说明串行/并发与同步/异步之间的关系，我们来看下面这段代码会打印出什么内容：
```
// serial, sync
serialQueue.sync {
  print(1)
}
print(2)
serialQueue.sync {
  print(3)
}
print(4)
// serial, async
serialQueue.async {
  print(1)
}
print(2)
serialQueue.async {
  print(3)
}
print(4)
// serial, sync in async
print(1)
serialQueue.async {
  print(2)
  serialQueue.sync {
    print(3)
  }
  print(4)
}
print(5)
// serial, async in sync
print(1)
serialQueue.sync {
  print(2)
  serialQueue.async {
    print(3)
  }
  print(4)
}
print(5)
```
首先，在串行队列上进行同步操作，所有任务将顺序发生，所以第一段的打印结果一定是 1234；
其次，在串行队列上进行异步操作，此时任务完成的顺序并不保证。所以可能会打印出这几种结果：1234 ，2134，1243，2413，2143。注意 1 一定在 3 之前打印出来，因为前者在后者之前派发，串行队列一次只能执行一个任务，所以一旦派发完成就执行。同理 2 一定在 4 之前打印，2 一定在 3 之前打印。
接着，对同一个串行队列中进行异步、同步嵌套。这里会构成死锁（具体原因参见下文），所以只会打印出 125 或者 152。
最后，在串行队列中进行同步、异步嵌套，不会构成死锁。这里会打印出 3 个结果：12345，12435，12453。这里1一定在最前，2 一定在 4 前，4 一定在 5 前。
现在我们把串行队列改为并发队列：
```
// concurrent, sync
concurrentQueue.sync {
  print(1)
}
print(2)
concurrentQueue.sync {
  print(3)
}
print(4)
// concurrent, async
concurrentQueue.async {
  print(1)
}
print(2)
concurrentQueue.async {
  print(3)
}
print(4)
// concurrent, sync in async
print(1)
concurrentQueue.async {
  print(2)
  concurrentQueue.sync {
    print(3)
  }
  print(4)
}
print(5)
// concurrent, async in sync
print(1)
concurrentQueue.sync {
  print(2)
  concurrentQueue.async {
    print(3)
  }
  print(4)
}
print(5)
```
首先，在并发队列上进行同步操作，所有任务将顺序执行、顺序完成，所以第一段的打印结果一定是 1234；
其次，在并发队列上进行异步操作，因为并行对列有多个线程 。所以这里只能保证 24 顺序执行，13 乱序，可能插在任意位置：2413 ，2431，2143，2341，2134，2314。
接着，对同一个并发队列中进行异步、同步嵌套。这里不会构成死锁，因为同步操作只会阻塞一个线程，而并发队列对应多个线程。这里会打印出 4 个结果：12345，12534，12354，15234。注意同步操作保证了 3 一定会在 4 之前打印出来。
最后，在并发队列中进行同步、异步嵌套，不会构成死锁。而且由于是并发队列，所以在运行异步操作时也同时会运行其他操作。这里会打印出 3 个结果：12345，12435，12453。这里同步操作保证了 2 和 4 一定在 3 和 5 之前打印出来。
在实际开发中，我们还需要知道主线程的特性、GCD 和 Operation 的 API、如发现并调试并发编程中的技巧。
# GCD vs. Operation
在 iOS 开发中，我们一般用 GCD 和 Operation 来处理并发编程问题。我们先来看看 GCD 的基本用法：
```
// serial queue
let serialQueue = DispatchQueue(label: "serial")
// global queue, gcd defined concurrent queue
let globalQueue = DispatchQueue.global(qos: .default)
// custom concurrent queue
let concurrentQueue = DispatchQueue(label: "concurrent", attributes: .concurrent)
```
其中，全局队列的优先级由 QoS (Quality of Service)决定。如果不指定优先级，就是默认（default）优先级。另外还有 background，utility，user-Initiated，unspecified，user-Interactive。下面按照优先级顺序从低到高来排列：
- **Background**：用来处理特别耗时的后台操作，例如同步、数据持久化。
- **Utility**：用来处理需要一点时间而又不需要立刻返回结果的操作。特别适用于网络加载、计算、输入输出等。
- **Default**：默认优先级。一般来说开发者应该指定优先级。属于特殊情况。
- **User-Initiated**：用来处理用户触发的、需要立刻返回结果的操作。比如打开用户点击的文件、加载图片等。
- **User-Interactive**：用来处理用户交互的操作。一般用于主线程，如果不及时响应就可能阻塞主线程的操作。
- **Unspecified**：未确定优先级，由系统根据不同环境推断。比如使用过时的 API 不支持优先级，此时就可以设定为未确定优先级。属于特殊情况。
在日常开发中，GCD 的常见应用有处理后台任务、延时、单例（Objective-C）、线程组等操作，这里不作赘述。下面我们来看看 Operation 的基本操作：
```
// serial queue
let serialQueue = OperationQueue()
serialQueue.maxConcurrentOperationCount = 1
// concurrent queue
let concurrentQueue = OperationQueue()
```
Operation 作为 NSObject 的子类，一般被用于单独的任务。我们将其继承重写之后加入到 OperationQueue 中去运行。iOS 亦提供 BlockOperation 这个子类去方便地执行多个代码片段。相比于 GCD，Operation 最主要的特点在于其拥有暂停、继续、终止等多个可控状态，从而可以更加灵活得适应并发编程的场景。
基于 Operation 和 GCD API 的特点，我们可以得出以下结论：GCD 适用于处理并行开发中的简单小任务，总体写法轻便快捷；Operation 适合于封装模块化的任务，支持多任务之间相互依赖的场景。两者之间的区别同 UIAnimation 和 CALayor Animation 差别异曲同工——由此可见苹果在设计 API 时一以贯之的思路：提供一个简单快捷的 API 满足80%的场景，在提供一套更全面的 API 应对剩下20%更复杂的场景。
# 并发编程中常见问题
在并发编程中，一般会面对这样的三个问题：竞态条件、优先倒置、死锁问题。针对 iOS 开发，它们的具体定义为：
- **竞态条件（Race Condition）**。指两个或两个以上线程对共享的数据进行读写操作时，最终的数据结果不确定的情况。例如以下代码：
```
var num = 0
DispatchQueue.global().async {
  for _ in 1…10000 {
    num += 1
  }
}
for _ in 1…10000 {
  num += 1
}
```
最后的计算结果 num 很有可能小于 20000，因为其操作为非原子操作。在上述两个线程对num进行读写时其值会随着进程执行顺序的不同而产生不同结果。
竞态条件一般发生在多个线程对同一个资源进行读写时。解决方法有两个，第一是串行队列加同步操作，无论读写，指定时间只能优先做当前唯一操作，这样就保证了读写的安全。其缺点是速度慢，尤其在大量读写操作发生时，每次只能做单个读或写操作的效率实在太低。另一个方法是，用并发队列和 barrier flag，这样保证此时所有并发队列只进行当前唯一的写操作（类似将并发队列暂时转为串行队列），而无视其他操作。
- **优先倒置（Priority Inverstion）**。指低优先级的任务会因为各种原因先于高优先级任务执行。例如以下代码：
```
var highPriorityQueue = DispatchQueue.global(qos: .userInitiated)
var lowPriorityQueue = DispatchQueue.global(qos: .utility)
let semaphore = DispatchSemaphore(value: 1)
lowPriorityQueue.async {
  semaphore.wait()
  for i in 0...10 {
    print(i)
  }
  semaphore.signal()
}
highPriorityQueue.async {
  semaphore.wait()
  for i in 11...20 {
    print(i)
  }
  semaphore.signal()
}
```
上述代码如果没有 semaphore，高优先权的 highPriorityQueue 会优先执行，所以程序会优先打印完 11 到 20。而加了 semaphore 之后，低优先权的 lowPriorityQueue 会先挂起 semaphore，高优先权的highPriorityQueue 就只有等 semaphore 被释放才能再执行打印。
也就是说，低优先权的线程可以锁上某种高优先权线程需要的资源，从而优于迫使高优先权的线程等待低优先权的线程，这就叫做优先倒置。其对应的解决方法是，对同一个资源不同队列的操作，我们应该用同一个QoS指定其优先级。
- **死锁问题（Dead Lock）**。指两个或两个以上的线程，它们之间互相等待彼此停止执行，以获得某种资源，但是没有一方会提前退出的情况。iOS 中有个经典的例子就是两个 Operation 互相依赖：
```
let operationA = Operation()
let operationB = Operation()
operationA.addDependency(operationB)
operationB.addDependency(operationA)
```
还有一种经典的情况，就是在对同一个串行队列中进行异步、同步嵌套：
```
serialQueue.async {
  serialQueue.sync {
  }
}
```
因为串行队列一次只能执行一个任务，所以首先它会把异步 block 中的任务派发执行，当进入到 block 中时，同步操作意味着阻塞当前队列 。而此时外部 block 正在等待内部 block 操作完成，而内部block 又阻塞其操作完成，即内部 block 在等待外部 block 操作完成。所以串行队列自己等待自己释放资源，构成死锁。
对于死锁问题的解决方法是，注意Operation的依赖添加，以及谨慎使用同步操作。其实聪明的读者应该已经发现，在主线程使用同步操作是一定会构成死锁的，所以我个人建议在串行队列中不要使用同步操作。
尽管我们已经知道了并发编程中的问题，以及其对应方法。但是日常开发中，我们怎样及时发现这些问题呢？其实 Xcode 提供了一个非常便利的工具 —— Thread Sanitizer (TSan)。在Schemes中勾选之后，TSan就会将所有的并发问题在 Runtime 中显示出来，如下图：
![](https://upload-images.jianshu.io/upload_images/1721232-aa643e9788f600cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
这里我们有7个线程问题，TSan清晰地告诉了我们这是读写问题，展开之后会告诉我们具体触发代码，十分方便。16年的WWDC上，苹果也郑重向大家宣告，如果有并发问题，请记得用 TSan。
# Operation 流程化开发
上文中提到 Operation 特别适合模块化工作，也支持多任务的互相依赖。这里我们就来看一个具体的开发案例吧：
实现一个相册 App，其首页是个滑动列表（Table View）。列表每行展示加上了滤镜的图片。具体实现如下图：
![](https://upload-images.jianshu.io/upload_images/1721232-d19d6967974e609a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/333/format/webp)
仔细分析一下相关的操作，实际上就是三步：先加载数据，然后解码成图片，最后再给图片加上滤镜。所以用 Operation 实现起来如下图：
![](https://upload-images.jianshu.io/upload_images/1721232-4df537b798806c02.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
对于加载数据，我们可以定义如下的 Operation 子类来进行操作：
```
class DataLoadOperation: Operation {
  
  fileprivate let url: URL
  fileprivate var loadedData: Data?
  fileprivate let completion: ((Data?) -> ())?
  
  init(url: URL, completion: ((Data?) -> ())? = nil) {
    ...
  }
  
  override func main() {
    if isCancelled { return }
    ImageService.loadData(at: url) { data in
      if isCancelled { return }
        loadedData = data
        completion?(data)
    }
  }
}
```
这里我们要注意，DataLoadOperation中的三个变量皆为私有。这是因为其实后续图片解码操作并不关心数据是如何操作的，它只关心是否能提供解码图片的数据，所以我们可以用 Protocol 来提供这个借口即可：
```
// 此协议定义应和 ImageDecodeOperation 放在同一文件
protocol ImageDecodeOperationDataProvider {
  var encodedData: Data? { get }
}
// 次扩展应和 DataLoadOperation 放在同一文件
extension DataLoadOperation: ImageDecodeOperationDataProvider {
  var encodedData: Data? { return loadedData }
}
```
接着再来看看解码图片的 Operation 如何实现：
```
class ImageDecodeOperation: Operation {
  
  fileprivate let inputData: Data?
  fileprivate var outputImage: UIImage?
  fileprivate let completion: ((UIImage?) -> ())?
  
  init(data: Data?, completion: ((UIImage?) -> ())? = nil) {
    ...
  }
  
  override func main() {    
    let encodedData: Data?
    if isCancelled { return }
    if let inputData = inputData {
      encodedData = inputData
    } else {
      let dataProvider = dependencies
        .filter { $0 is ImageDecodeOperationDataProvider }
        .first as? ImageDecodeOperationDataProvider
      encodedData = dataProvider?.encodedData
    }
    
    guard let data = encodedData else { return }
    
    if isCancelled { return }
    if let decodedData = Decoder.decodeData(data) {
      outputImage = UIImage(data: decodedData)
    }
    
    if isCancelled { return }
    completion?(outputImage)
  }
}
extension ImageDecodeOperation: ImageFilterDataProvider {
  var image: UIImage? { return outputImage }
}
```
最后我们再来看 ImageFilterOperation 及其子类如何实现。这里由于直接输出 Image，所以就无需用：
```
protocol ImageFilterDataProvider {
  var image: UIImage? { get }
}
class ImageFilterOperation: Operation {
  fileprivate let filterInput: UIImage?
  fileprivate var filterOutput: UIImage?
  fileprivate let completion: ((UIImage?) -> ())?
  
  init(image: UIImage?, completion:
 ((UIImage?) -> ())? = nil) {
    ...
  }
  
  var filterInput: UIImage? {
    var image: UIImage?
    if let inputImage = _filterInput {
      image = inputImage
    } else if let dataProvider = dependencies
      .filter({ $0 is ImageFilterDataProvider })
      .first as? ImageFilterDataProvider {
        image = dataProvider.image
    }
    return image
  }
}
// LarkFilter 和 ReyesFilter 的实现也类似
class MoonFilterOperation : ImageFilterOperation {
  override func main() {
    if isCancelled { return }
    guard let filterInput = filterInput else { return }
 
    if isCancelled { return }
    filterOutput = filterInput.applyMoonEffect()
    if isCancelled { return }
    completion(imageFiltered)
  }
}
```
最后我们用 OperationQueue 将这些 Operation 拼接在一起：
```
let operationQueue = OperationQueue()
let dataLoadOperation = DataLoadOperation(url: url)
let imageDecodeOperation = imageDecodeOperation(data: nil)
let moonFilterOperation = MoonFilterOperation(image: nil, completion: completion)
let operations = [dataLoadOperation, imageDecodeOperation, moonFilterOperation]
// Add dependencies
imageDecodeOperation.addDependency(dataLoadOperation)
moonFilterOperation.addDependency(imageDecodeOperation)
operationQueue.addOperations(operations, waitUntilFinished: false)
```
大功告成。从上面我们可以发现，每个操作模块都可以用 Operation 进行自定义和封装。模块的对应逻辑非常清楚，代码复用率和灵活度也非常之高。如果要继续改进，我们还可以实现一个 AsyncOperation 的类，然后让 DataLoadOperation 继承该类，这样数据加载由同步变为异步，其效率会大大提高。
# 总结
iOS 开发中，并发编程主要用于提升 App 的运行性能，保证App实时响应用户的操作。主线程一般用于负责 UI 相关操作，如绘制图层、布局、交互相应。很多 UIKit 相关的控件如果不在主线程操作，会产生未知效果。Xcode 中的 Main Thread Checker 可以将相关问题检测出来并报错。
其他线程例如后天线程一般用来处理比较耗时的工作。网络请求、数据解析、复杂计算、图片的编码解码管理等都属于耗时的工作，应该放在其他线程处理。iOS 提供了两套灵活丰富的 API：GCD 和 Operation。GCD的优点在于简单快捷，Operation 胜在功能丰富、适合模块化操作。我们享受其便利的同时，也应该及时发现和处理并发编程中的三大问题。
作者：故胤道长
链接：https://www.jianshu.com/p/39d6edb54d24
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
