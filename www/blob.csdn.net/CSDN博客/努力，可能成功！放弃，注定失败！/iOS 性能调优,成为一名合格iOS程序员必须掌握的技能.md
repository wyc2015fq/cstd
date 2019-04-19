# iOS 性能调优,成为一名合格iOS程序员必须掌握的技能 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月07日 17:27:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：865
提供了基于Swift3.0模仿的新浪微博的Demo，大家可以下载看一看:**[基于Swift3.0高仿的微博客户端](https://github.com/iosyaowei/Weibo)**,里面针对于微博首页的复杂页面的优化做了很多的处理，页面的FPS 一直保持在59 ~ 60 。
看下demo的效果：
![](http://upload-images.jianshu.io/upload_images/2240549-fe781f51e175ef86.gif?imageMogr2/auto-orient/strip)
FPS测试.gif
### CPU 和GPU
关于绘图和动画有两种处理方式CPU(中央处理器)和GPU(图形处理器)，CPU的工作都在软件层面，而GPU的在硬件层面。
总的来说，可以使用CPU做任何事情，但是对于图像的处理，通常GPU会更快，因为GPU使用图像对高度并行浮点运算做了优化（尽管我也不知道是什么鬼？？），所以，我们想尽可能的把屏幕渲染的工作交给硬件去处理，而问题在于GPU并没有无限制处理的性能，一旦资源用尽，即使CPU并没有完全占用，GPU性能还是会下降。
所以，目前大多的性能优化都是关于智能利用GPU和CPU，平衡它们之间工作负载。
### 测量，而不是猜测
现在知道哪些可能会影响性能，该如何修复呢？有许多传统的诡计来优化，但如果盲目使用的话，可能会造成更多性能上的问题，而不是优化了
如何正确的测量而不是猜测这点很重要，根据性能相关知识写出的代码不同于仓促优化，前者是正确的姿势，后者则是在浪费生命
那改如何测量，第一步就是确保在真实环境下测试你的程序
#### 真机测试，而不是模拟器
当你开始做一些性能方面的工作时候，一定要在真机上测试，而不是模拟器，模拟器虽然可以加快开发效率，但却不能准确提供真机的性能参数
模拟器运行在Mac上，然而Mac上的cpu比ios设备要快很多，相反，Mac上的GPU和ios设备上的不一样，模拟器不得已需要在软件层面(CPU)模拟ios设备，所以GPU的相关操作在模拟器上面运行的会更慢
另一件重要的就是性能测试的时候一定要用发布配置，而不是调试模式，因为当用发布环境打包的时候，编译器会引入一些提高性能的优化，比如：去除调试符号或者移除并重新组织代码，因为可以自己做到这些，比如禁用NSlog、print语句，因为 ，只需要关心发布性能。
### 测试帧率
可以在程序中使用CADisplayLink来测量帧率，在屏幕上显示出来,我用Swift3.0模仿YY大神的代码写了一个简单的FPS指示器 [YWFPSLabel](https://github.com/iosyaowei/Weibo/blob/master/WeiBo/Classes/Utils(%E5%B7%A5%E5%85%B7)/YWFPSLabel/YWFPSLabel.swift),使用CADisplayLink监视FPS数值，日常开发的时候，可以有直接的体现，不用再靠猜了...
[YWFPSLabel](https://github.com/iosyaowei/Weibo/blob/master/WeiBo/Classes/Utils(%E5%B7%A5%E5%85%B7)/YWFPSLabel/YWFPSLabel.swift)集成也很方便,在AppDelegate的application方法里加下面两句即可
```
let FPSLab = YWFPSLabel(frame: CGRect())
        UIApplication.shared.keyWindow!.addSubview(FPSLab)
```
不知道大家有木有看到头部那个小label啊~~~
但是应用内的FPS显示并不能完全真实的测量出性能，因为它仅仅能测试出应用内的帧率，还有很多是动画都是在应用外发生（在渲染进程中处理），但应用内FPS计数可以对一些性能问题提供参考，一旦找到问题，需要更多的精确详细信息来定位问题所在，我们就要使用Instuments了，它可以看到更多准确是信息，查看到所有与显示的数据。
### Instuments
Instuments是Xcode套件中没有被充分利用的工具，很多iOS开发者从来没用过Instrument，特别是通过短暂培训出来的同学们，所以，很多面试官也会问性能条调优方面的知识，来判断面试的同学是否真正应用对年开发经验。
- 
#### Activity Monitor
个人觉的很像Windows的任务管理器，可以查看所有的进程，以及进程的内存、cpu使用百分比等数据等，就不多介绍了，打开一看大概就知道怎么回事
- 
#### Allocations
管理内存是app开发中最重要的一个方面，对于开发者来说，在程序架构中减少内存的使用通常都是使用Allocations去定位和找出减少内存使用的方式
接下来，谈一下内存泄漏的两种情况
- 第一种：为对象A申请了内存空间，之后再也没用过对象A，也没释放过A导致内存泄漏，这种是Leaked Memory内存泄漏
- 第二种：类似于递归，不断地申请内存空间导致的内存泄漏，这种情况是Abandoned Momory
此工具可以让开发者很好的了解每个方法占用内存的情况，并定位相关的代码
![](http://upload-images.jianshu.io/upload_images/2240549-b3c8cf33409d836d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Allocations查看方法占用内存.png
右键就可以打开Xcode自动定位到相关占用内存方法的代码上
![](http://upload-images.jianshu.io/upload_images/2240549-c1912028172776fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
定位到相关代码.png
第二种情况可以根据下图的操作清晰的找到对用的代码问题
![](http://upload-images.jianshu.io/upload_images/2240549-dd35664a4ffa2416.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
定位Abandoned Momory.png
解释一下，第二种情况我们应该如何操作，重复的执行一系列的操作时候内存不会继续增加，比如打开和关闭一个窗口，这样的操作，每一次操作的前后，内存应该是相同的，通过多次循环操作，内存不会递增下去，通过这种分析结果，观察内存分配趋势，当发现不正确的结果或者矛盾的结果，就可以研究是不是Abandoned Momory的问题，并可以修正这个问题了
#### Core Animation
之前说过自己写的[YWFPSLabel](https://github.com/iosyaowei/Weibo/blob/master/WeiBo/Classes/Utils(%E5%B7%A5%E5%85%B7)/YWFPSLabel/YWFPSLabel.swift)只能检测应用内的FPS,而此工具考虑到了程序外的动画，理想的FPS值为60左右，过低的话就用该进性优化了，根据WWDC的说法，当FPS 低于45的时候，用户就会察觉到到滑动有卡顿
![](http://upload-images.jianshu.io/upload_images/2240549-c72efd271ae3bad7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Core Animation.png
圈着数字红色方框中的数字，代表着FPS值，理论上60最佳，实际过程中59就可以了，说明就是很流畅的，说明一下操作方式：在手指不离开屏幕的情况下，上下滑动屏幕列表
介绍一下Deug Display中选项的作用
- 
###### Color Blended Layers（混合过度绘制）
打开此选项屏幕的效果图如下：
![](http://upload-images.jianshu.io/upload_images/2240549-0f7b0b69d9980910.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Color Blended Layers.jpg
这个选项基于渲染程度对屏幕中的混合区域进行绿到红的高亮（也就是多个半透明图层的叠加），由于重绘的原因，混合对GPU性能会有影响，同时也是滑动或者动画掉帧的罪魁祸首之一
GPU每一帧的绘制的像素有最大限制，这个情况下可以轻易绘制整个屏幕的像素，但如果发生重叠像素的关系需要不停的重绘同一区域的，掉帧和卡顿就有可能发生
GPU会放弃绘制那些完全被其他图层遮挡的像素，但是要计算出一个图层是否被遮挡也是相当复杂并且会消耗CPU的资源，同样，合并不同图层的透明重叠元素消耗的资源也很大，所以，为了快速处理，一般不要使用透明图层，
1). 给View添加一个固定、不透明的颜色
2). 设置opaque 属性为true
但是这对性能调优的帮助并不大，因为UIView的opaque 属性默认为true，也就是说，只要不是认为设置成透明，都不会出现图层混合
而对于UIIimageView来说，不仅需要自身需要不是透明的，它的图片也不能含有alpha通道，这也上图9张图片是绿色的原因，因此图像自身的性质也可能会对结果有影响，所以你确定自己的代码没问题，还出现了混合图层可能就是图片的问题了
而针对于屏幕中的文字高亮成红色，是因为一没有给文字的label增加不透明的背景颜色，而是当UILabel内容为中文时，label的实际渲染区域要大于label的size，因为外围有了一圈的阴影，才会出现图层混合我们需要给中文的label加上如下代码：
```
retweededTextLab?.layer.masksToBounds = true
        retweededTextLab?.backgroundColor = UIColor.groupTableViewBackground
        statusLab.layer.masksToBounds = true
        statusLab.backgroundColor = UIColor.white
```
看下效果图：
![](http://upload-images.jianshu.io/upload_images/2240549-2a417a6e3bb1bc38.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图层混合优化.png
那些label的颜色也变成蓝色的了，这里有一点需要说明一下，
1). `statusLab.layer.masksToBounds = true` 单独使用不会出现离屏渲染
2). 如果对label设置了圆角的话，圆角部分会离屏渲染，离屏渲染的前提是位图发生了形变
- 
###### Color Hits Green and Misses Red(光栅化缓存图层的命中情况)
这个选项主要是检测我们有无滥用或正确使用layer的shouldRasterize属性.成功被缓存的layer会标注为绿色,没有成功缓存的会标注为红色。
很多视图Layer由于Shadow、Mask和Gradient等原因渲染很高，因此UIKit提供了API用于缓存这些Layer,`self.layer.shouldRasterize = true`系统会将这些Layer缓存成Bitmap位图供渲染使用，如果失效时便丢弃这些Bitmap重新生成。图层Rasterization栅格化好处是对刷新率影响较小，坏处是删格化处理后的Bitmap缓存需要占用内存，而且当图层需要缩放时，要对删格化后的Bitmap做额外计算。 使用这个选项后时，如果Rasterized的Layer失效，便会标注为红色，如果有效标注为绿色。当测试的应用频繁闪现出红色标注图层时，表明对图层做的Rasterization作用不大。
在测试的过程中,第一次加载时,开启光栅化的layer会显示为红色,这是很正常的,因为还没有缓存成功。但是如果在接下来的测试,。例如我们来回滚动TableView时,我们仍然发现有许多红色区域,那就需要谨慎对待了
- 
###### Color Copied Image (拷贝的图片)
这个选项主要检查我们有无使用不正确图片格式,由于手机显示都是基于像素的，所以当手机要显示一张图片的时候，系统会帮我们对图片进行转化。比如一个像素占用一个字节，故而RGBA则占用了4个字节，则1920 x 1080的图片占用了7.9M左右，但是平时jpg或者png的图片并没有那么大，因为它们对图片做了压缩，但是是可逆的。所以此时，如果图片的格式不正确，则系统将图片转化为像素的时间就有可能变长。而该选项就是检测图片的格式是否是系统所支持的，若是GPU不支持的色彩格式的图片则会标记为青色,则只能由CPU来进行处理。CPU被强制生成了一些图片，然后发送到渲染服务器，而不是简单的指向原始图片的的指针。我们不希望在滚动视图的时候,CPU实时来进行处理,因为有可能会阻塞主线程。
- 
###### Color Immediately (颜色立即更新)
通常 Core Animation 以每秒10此的频率更新图层的调试颜色，对于某些效果来说，这可能太慢了，这个选项可以用来设置每一帧都更新（可能会影响到渲染性能，所以不要一直都设置它）
- 
###### Color Misaligned Image (图片对齐方式)
这里会高亮那些被缩放或者拉伸以及没有正确对齐到像素边界的图片，即图片Size和imageView中的Size不匹配，会使图过程片缩放，而缩放会占用CPU，所以在写代码的时候保证图片的大小匹配好imageView，如下图所示：
图片尺寸 170 * 220px
```cpp
```cpp
let imageView = UIImageView(frame: CGRect(x: 50, y: 100, width: 170, height: 220))
        imageView.image = UIImage(named: "cat")
        view.addSubview(imageView)
```
```
苹果的单位以点计算，而 imageView的尺寸是170 *220 pt 而图片是 170 *220px，所以相当于在屏幕上对图片方法了一倍，看效果图如下：
![](http://upload-images.jianshu.io/upload_images/2240549-28d9369c482b5375.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Color Misaligned Image.png
可以看到图片高亮成黄色显示，更改下imageView的大小
```cpp
```cpp
let imageView = UIImageView(frame: CGRect(x: 50, y: 100, width: 85, height: 110))
        imageView.image = UIImage(named: "cat")
        view.addSubview(imageView)
```
```
看下效果图
![](http://upload-images.jianshu.io/upload_images/2240549-212550d0ec32e60a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Color Misaligned Image -2.png
当imageView和image的大小一致的时候，就正常显示了
- 
###### Color Offscreen- Rendered Yellow (离屏渲染)
这里会把那些需要离屏渲染的到图层高亮成黄色，而出发离屏渲染的可能有
```
/* 圆角处理 */
view.layer.maskToBounds = truesomeView.clipsToBounds = true
/* 设置阴影 */
view.shadow..
/* 栅格化 */
view.layer.shouldRastarize = true
```
针对栅格化处理，我们需要指定屏幕的分辨率
```
//离屏渲染 - 异步绘制  耗电
        self.layer.drawsAsynchronously = true
        //栅格化 - 异步绘制之后 ，会生成一张独立的图片 cell 在屏幕上滚动的时候，本质上滚动的是这张图片 
        //cell 优化，要尽量减少图层的数量，想当于只有一层
        //停止滚动之后，可以接受监听
        self.layer.shouldRasterize = true
        //使用 “栅格化” 必须指定分辨率
        self.layer.rasterizationScale = UIScreen.main.scale
```
指定阴影的路径，可以防止离屏渲染
```
// 指定阴影曲线，防止阴影效果带来的离屏渲染
        imageView.layer.shadowPath = UIBezierPath(rect: imageView.bounds).cgPath
```
这行代码制定了阴影路径，如果没有手动指定，Core Animation会去自动计算，这就会触发离屏渲染。如果人为指定了阴影路径，就可以免去计算，从而避免产生离屏渲染。
设置cornerRadius本身并不会导致离屏渲染，但很多时候它还需要配合layer.masksToBounds = true使用。根据之前的总结，设置masksToBounds会导致离屏渲染。解决方案是尽可能在滑动时避免设置圆角，如果必须设置圆角，可以使用光栅化技术将圆角缓存起来：
```cpp
```cpp
// 设置圆角
label.layer.masksToBounds = true
label.layer.cornerRadius = 8
label.layer.shouldRasterize = true
label.layer.rasterizationScale = layer.contentsScale
```
```
如果界面中有很多控件需要设置圆角，比如tableView中，当tableView有超过25个圆角，使用如下方法
```cpp
```cpp
view.layer.cornerRadius = 10
view.maskToBounds = Yes
```
```
那么fps将会下降很多，特别是对某些控件还设置了阴影效果，更会加剧界面的卡顿、掉帧现象，对于不同的控件将采用不同的方法进行处理：
1). 对于label类，可以通过CoreGraphics来画出一个圆角的label
2). 对于imageView，通过CoreGraphics对绘画出来的image进行裁边处理，形成一个圆角的imageView,代码如下:
```
/// 创建圆角图片
    ///
    /// - parameter radius:    圆角的半径
    /// - parameter size:      图片的尺寸
    /// - parameter backColor: 背景颜色 默认 white
    /// - parameter lineWith:  圆角线宽 默认 1
    /// - parameter lineColor: 线颜色 默认 darkGray
    ///
    /// - returns: image
    func yw_drawRectWithRoundCornor(radius: CGFloat, size: CGSize, backColor: UIColor = UIColor.white, lineWith: CGFloat = 1, lineColor: UIColor = UIColor.darkGray) -> UIImage? {
        let rect = CGRect(origin: CGPoint(x: 0, y: 0), size: size)
        UIGraphicsBeginImageContextWithOptions(rect.size, true, 0)
        let bezier = UIBezierPath(roundedRect: rect, byRoundingCorners: UIRectCorner.allCorners, cornerRadii: CGSize(width: radius, height: radius))
        backColor.setFill()
        UIRectFill(rect)
        bezier.addClip()
        draw(in: rect)
        bezier.lineWidth = 1
        lineColor.setStroke()
        bezier.stroke()
        let result = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        return result
    }
```
- 
###### Color Compositing Fast-Path Blue
这个选项会对任何直接使用OpenGL绘制的图层进行高亮，如果仅仅使用UIKit或者Core Animation的API，不会有任何效果，恕我才疏学浅，所以，我在测试的时候，确实在屏幕上没有任何反应的，openGL 绘制，我也不会，所以，就不知道到底会有什么效果了，哪位大神会的话，贴段代码，给我试试啊~~~
- 
###### Flash Updated Regions (Core Graphics 绘制的图层)
此选项会对重绘的内容进行高亮成黄色，也就是软件层面使用Core Graphics 绘制的图层。我测试的时候，好像有点问题，这种解释，不知道是不是我写代码的问题，所以，就不多说了
上面说的这些高亮图层，几个常用的选项在模拟器里面可以直接调试，非常方便
![](http://upload-images.jianshu.io/upload_images/2240549-c0125396e45acb61.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
模拟器高亮图层.png
红框中的选项，上面都有解释，这里就不说了，勾选项，打开模拟器，一看就知道了~
麻蛋、Core Animation 部分终于扯完了，扯了好多啊。。。
- 
#### Leaks
又一个灰常重要的工具，主要检查内存泄漏，在前面Allcations里面我们提到内存泄漏分两种，现在我们研究Leaked Memory， 从用户使用角度来看，内存泄漏本身不会产生什么危害，作为用户，根本感觉不到内存泄漏的存在，真正的危害在于内存泄漏的堆积，最终会耗尽系统所有的内存。我们直接看图：
![](http://upload-images.jianshu.io/upload_images/2240549-07e85030b3b379e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
界面的介绍
在 instruments 中,虽然选择了 Leaks 模板,但默认情况下也会添加 Allocations 模板.基本上凡是内存分析都会使用 Allocations 模板, 它可以监控内存分布情况。
选中 Allocations 模板3区域会显示随着时间的变化内存使用的折线图,同时在4区域会显示内存使用的详细信息,以及对象分配情况.
点击 Leaks 模板, 可以查看内存泄露情况。如果在3区域有 红X 出现, 则有内存泄露, 4区域则会显示泄露的对象.
打用leaks进行监测：点击泄露对象可以在(下图)看到它们的内存地址, 占用字节, 所属框架和响应方法等信息.打开扩展视图, 可以看到右边的跟踪堆栈信息，4 黑色代码最有可能出现内存泄漏的方法
![](http://upload-images.jianshu.io/upload_images/2240549-0178cbac8f7001a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
监测结果的分析，
![](http://upload-images.jianshu.io/upload_images/2240549-24fc96bb7b9d984c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### Time Profiler
在开发的过程中，我们经常能感觉到，点击某一按钮，或者做了某一操作，有卡顿，这就是延迟，那使用此工具，就可以揪出耗时的函数，先看一下，调试界面介绍：
![](http://upload-images.jianshu.io/upload_images/2240549-fc28a11ba943ce3d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
time Profiler.png
根据查看的相关耗时操作，我们就可以右键定位当耗时的方法：
写一个简单例子看一下：
![](http://upload-images.jianshu.io/upload_images/2240549-f7e89be0cdb57de7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.23.02.png
看上图，可以很清楚看到此方法耗时比较严重，右键打开定位到此方法的代码：
![](http://upload-images.jianshu.io/upload_images/2240549-e793e6648eb4b30d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.24.57.png
代码截图如下：
![](http://upload-images.jianshu.io/upload_images/2240549-518b8f4d9c0613fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.25.06.png
这时候，我们把循环放到子线程来做
```
@IBAction func btnAction(_ sender: AnyObject) {
        let svc = SecondViewController()
        svc.title = "第二个页面"
        //全局队列异步执行
        DispatchQueue.global().async {
            for i in 0..<8888 {
                print(i)
            }
        }
        navigationController?.pushViewController(svc, animated: true)
    }
```
看效果图：
![](http://upload-images.jianshu.io/upload_images/2240549-29192d461798e858.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.33.43.png
到这里比较重要Instrument调试工具介绍的差不多了，说一个Xcode8.0新出的功能，很好用也很重要的功能：
![](http://upload-images.jianshu.io/upload_images/2240549-95af00e9cc66fd1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.40.08.png
还是以例子说说吧，Viewcontroller里面一个button，点击跳到SecondViewcontroller，SecondViewcontroller里面有个View，view里面有个button，button点击回到ViewController,实现是通过view的属性拿到SecondviewConroller的引用，pop回去
子view的代码如下：
```
class SubView: UIView {
    var  delegate: SecondViewController?
     @IBAction func brnAction(_ sender: AnyObject) {
        delegate?.navigationController!.popViewController(animated: true)
     }
}
```
当我们从第二个控制器，回到第一个控制器的时候，我们点一下，刚那个按钮，看图：
![](http://upload-images.jianshu.io/upload_images/2240549-26cdf4b29bcfccab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.47.53.png
第二个控制器和子View都内存中，我们很容易，就可以发现问题了，这是因为，SecondViewController强引用了SubView，而我们SubView也强引用了SecondViewcontroller，就造成相互强引用，引用计数器不能为0，不能销毁了，我们只要把属性前面加个weak，变成弱引用就可以了
```
weak var  delegate: SecondViewController?
     @IBAction func brnAction(_ sender: AnyObject) {
        delegate?.navigationController!.popViewController(animated: true)
     }
```
这时候，我们从第二个控制器pop回来的时候，看下内存：
![](http://upload-images.jianshu.io/upload_images/2240549-b6d98af07c69aef4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-10-30 下午4.54.06.png
现在就没问题了，怎样这个工具是不是挺好用啊
还有一些针对TableView，collectionView的优化，有空再聊吧，
- 声明：
1.本文有些文字描述，来自于其他博客和文章，因为觉的描述挺好（好吧我承认、自己也写不出来），所以就拿来用了，若有不妥的地方，请告知我，我会立即删除修改
2.因为本人的水平有限，可能有写的不对的地方，欢迎各位大大多指点、不胜感激
最后的最后：码字不易，如果对你有用，点个喜欢呗！
文／iOS巍（简书作者）
原文链接：http://www.jianshu.com/p/05b68c84913a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
