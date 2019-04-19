# 如何使在Swift中使用UIScrollView进行滚动和缩放 - =朝晖= - 博客园
# [如何使在Swift中使用UIScrollView进行滚动和缩放](https://www.cnblogs.com/dhcn/articles/7447139.html)
http://www.tairan.com/archives/7790/
![Learn how to use UIScrollViews for paging, zooming, scrolling, and more!](http://cdn3.raywenderlich.com/usr/uploads/2014/07/Scroll_View.png)
UIScrollView是IOS中非常有用的一个组件。它是通用组件UITableView的基类也是放置尺寸大于一个屏幕的内容时很好的组织方式。本教程会介绍UIScrollView的下列用法:
- 如何使用UIScrollView浏览大尺寸图片。
- 如何在缩放时保持UIScrollView中的内容居中.
- 如何在UIScrollView中嵌入复杂的显示层次关系。
- 如何使用UIScrollView的分页（paging）特性, 联合使用UIPageControl在不同页面之间移动。
- 通过“偷窥”UIScrollView得到前一页，后一页还有当前页的缩略图。
- 其他内容!
本文假设你熟悉Swift语言和IOS开发。如果你完全是一名初学者, 你可能需要先学习一下本站的[其他课程](http://www.raywenderlich.com/tutorials)。
本文还假设你知道如何使用Interface Builder在view中添加对象和在你的代码和Storyboard连接outlets。 如果你刚刚接触Storyboards和Interface Builder, 你可以在开始本教程之前线学习一下本站的[Storyboards教程](http://www.raywenderlich.com/5138/beginning-storyboards-in-ios-5-part-1)。
## 开始
打开Xcode创建工程。选择File\New\Project…, 接下来选择iOS\Application\Single View Application模版。 在product name输入框中输入ScrollViews, language选择Swift, devices选择 iPhone.
![Create Project](http://cdn1.raywenderlich.com/usr/uploads/2014/07/Create_Project-700x413.png)
单击Next选择工程文件的保存位置。
之后下载[工程的资源压缩包](http://cdn1.raywenderlich.com/downloads/ScrollViewsResources.zip), 然后将解加压后的文件拖拽到工程的根目录。并确保选中了“Copy items if needed”复选框。
![Copy Resources](http://cdn1.raywenderlich.com/usr/uploads/2014/07/Copy_Resources-700x412.png)
本文会介绍Scroll view 的四种不同使用方式, 工程采用一个tableview来提供这四个选项。每个选项打开一个新的视图控制器（view controller）来展示scroll views的一种用法。
下图展示了完成时storyboard的状态：
![Storyboard Overall](http://cdn1.raywenderlich.com/usr/uploads/2014/07/Storyboard_Overall-655x500.png)
通过下列步骤创建一个tableview:
- 
打开Main.storyboard删除当前场景。 选择view controller (单击 storyboard) 然后删除它。
- 
关闭Auto Layout功能。在Utilities面板中选择File Inspector, 取消Use Auto Layout复选框的选中状态。这会弹出一个警告窗口。在弹出窗口中确保“Keep size class data for:”选中了iPhone。然后选择Disable Size Classes。
![Disable Size Classes](http://cdn1.raywenderlich.com/usr/uploads/2014/07/Disable_Size_Classes.png)
- 
接下来, 添加一个Table View Controller，从Object Library中拖拽一个Table View Controller到storyboard。
- 
选中刚添加的table, 然后在菜单中选择Editor\Embed In\Navigation Controller。
- 
同时选中table view和table view controller, 将Attributes Inspector中的content type修改为Static Cells(如下图所示)。
![Static Cells](http://cdn2.raywenderlich.com/usr/uploads/2014/07/Static_Cells.png)
- 
在文档概要视图中Table View单击下面的按钮显示storyboard层次关系视图，然后选中Table View Section. 将行数设置为4。如下图所示
![UIScrollView table view section](http://cdn3.raywenderlich.com/usr/uploads/2014/08/UIScrollView-table-view-section.png)
- 
依次编辑table view的每一行, 将style设置为Basic，编辑显示的标签:
- Image Scroll
- Custom View Scroll
- Paged
- Paged with Peeking
注释: 由于你将每一行的style设置成了“Basic”, 表单的每一行会出现额外的标签子组件。你要再次展开每行的子项目来进行编辑。
保存storyboard, 之后构建并运行。你会看到你编辑的table view如下图所示.不过现在table view中还没有任何东西 - 但你可以改好它！
![Scroll View Run 1](http://cdn3.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_11-281x500.png)
## 浏览和缩放大尺寸图片
第一步我们先来学习设置scroll view来允许用户缩放和滚动浏览图片。
首先, 你需要设置view controller。打开ViewController.swift, 修改文件头部的类声明使它符合UIScrollViewDelegate协议:
    class ViewController: UIViewController, UIScrollViewDelegate {
在类的声明中添加下面的outlet属性:
    @IBOutlet var scrollView: UIScrollView!
下一步你将会将它连接到真正的scroll view。
打开storyboard从objects library拖拽一个View Controller到画布。选择新的view controller在Identity Inspector中 将class修改为ViewController。
![View controller class type](http://cdn3.raywenderlich.com/usr/uploads/2014/07/View_controller_class-_type.png)
这个view controller展示图片的滚动. 从table view中选中Image，按住Control键，从这一行拖动到新添加的view controller。在弹出的菜单中选择Selection Segue下面的Push按键。 当用户选择第一行时，view controller将会被压到导航栈（navigation stack）上。
从Object Library中拖拽一个Scroll View来填充这个新添加的view controller。
![Add Scroll View](http://cdn4.raywenderlich.com/usr/uploads/2014/07/Add_Scroll_View-293x500.png)
连接scroll view和view controller， 将view controller附加到scroll view的outlet中并设置view controller作为scroll view的代理。
![Scroll View Outlets](http://cdn2.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Outlets.png)
现在你要处理代码了。打开ViewController.swift在类中添加新的属性：
    var imageView: UIImageView!
这个变量代表用户滚动时使用的image view。
现在我们要进行设置scroll view的过程中最有趣的部分。用下面的方法替换viewDidLoad:
    override func viewDidLoad() {
      super.viewDidLoad()
      // 1
      let image = UIImage(named: "photo1.png")
      imageView = UIImageView(image: image)
      imageView.frame = CGRect(origin: CGPoint(x: 0, y: 0), size:image.size)
      scrollView.addSubview(imageView)
      // 2
      scrollView.contentSize = image.size
      // 3
      var doubleTapRecognizer = UITapGestureRecognizer(target: self, action: "scrollViewDoubleTapped:")
      doubleTapRecognizer.numberOfTapsRequired = 2
      doubleTapRecognizer.numberOfTouchesRequired = 1
      scrollView.addGestureRecognizer(doubleTapRecognizer)
      // 4
      let scrollViewFrame = scrollView.frame
      let scaleWidth = scrollViewFrame.size.width / scrollView.contentSize.width
      let scaleHeight = scrollViewFrame.size.height / scrollView.contentSize.height
      let minScale = min(scaleWidth, scaleHeight);
      scrollView.minimumZoomScale = minScale;
      // 5
      scrollView.maximumZoomScale = 1.0
      scrollView.zoomScale = minScale;
      // 6
      centerScrollViewContents()
    }
好像有点复杂，让我们一步一步的进行解释，你会发现并没有那么坏。
- 
首先, 你需要使用photo1.png(已经添加到工程中)创建一个image view。然后设置image view的frame (大小和位置)尺寸为图片的尺寸，位置在上层视图的（0，0）点。 最后将image view添加为scroll view的子视图。
- 
告诉scroll view它里面内容的尺寸, 这样他就知道在水平方向和竖直方向上如何滚动。 在这里它的大小是图片的尺寸。
- 
这里我们设置了一个双击的手势来实现缩放的功能。UIScrollView已经内建实现了捏合的手势（UIPinchGestureRecognizer）实现缩放功能，因此你不需要自己添加了。
- 
下一步, 你要计算出scroll view的最小的缩放尺寸。 当缩放尺寸的值是1时表示它的内容会正常显示。小于1的值表示内容会被缩小，大于1的值表示内容会被放大。 为了计算最小的缩放尺寸, 你要计算出图片正好符合scroll view的宽度的值scaleWidth和正好符合scroll view的高度的值scaleHeight，scaleWidth和scaleHeight的最小值就是scroll view的最小缩放尺寸。这样在缩到最小的时候，你能够看到整个图片。
- 
将最大的缩放尺寸设置成1是因为，放大图片超过它的分辨率会引起图片的模糊。将初始的缩放尺寸设置成了最小尺寸，就是说开始时整个图片缩到了最小，你能看到完整的图片。
- 
调用centerScrollViewContents函数将图片放置到了scroll view的中央。centerScrollViewContents是如何函数的实现的？稍后会做详细解释！
在类中添加centerScrollViewContents的实现:
    func centerScrollViewContents() {
        let boundsSize = scrollView.bounds.size
        var contentsFrame = imageView.frame
        if contentsFrame.size.width < boundsSize.width {
        contentsFrame.origin.x = (boundsSize.width - contentsFrame.size.width) / 2.0
        } else {
        contentsFrame.origin.x = 0.0
        }
        if contentsFrame.size.height < boundsSize.height {
        contentsFrame.origin.y = (boundsSize.height - contentsFrame.size.height) / 2.0
        } else {
        contentsFrame.origin.y = 0.0
        }
        imageView.frame = contentsFrame
    }
函数中设置的位置有一点不好理解：如果scroll view的内容大小比它的边界小，那么会将位置放在左上角而不是中心。这是因为你允许用户缩小图片，将图片放置在中心会好一些。函数通过这样放置图片使它在scroll view的中心。
最后，在类中添加scrollViewDoubleTapped的实现来处理双击的手势:
    func scrollViewDoubleTapped(recognizer: UITapGestureRecognizer) {
      // 1        
      let pointInView = recognizer.locationInView(imageView)
      // 2
      var newZoomScale = scrollView.zoomScale * 1.5
      newZoomScale = min(newZoomScale, scrollView.maximumZoomScale)
      // 3
      let scrollViewSize = scrollView.bounds.size
      let w = scrollViewSize.width / newZoomScale
      let h = scrollViewSize.height / newZoomScale
      let x = pointInView.x - (w / 2.0)
      let y = pointInView.y - (h / 2.0)
      let rectToZoomTo = CGRectMake(x, y, w, h);
      // 4
      scrollView.zoomToRect(rectToZoomTo, animated: true)
    }
当识别出双击的手势时这个函数就会被调用。 下面详细解释这个函数的实现:
- 
首先，计算出双击在image view中的位置。就像用户期望的那样，我们将会使用这个点作为中心进行放大。
- 
接下来, 我们将新的缩放尺寸放大到现在的1.5倍，但不要超过之前在viewDidLoad函数中设置的最大缩放尺寸。
- 
然后使用第一步计算出的位置计算要放大的矩形尺寸。
- 
最后, 告诉scroll view进行缩放并使用动画效果是缩放看起来更美观。
想了解IOS中其他可用的手势，可以参考本站的 [UIGestureRecognizer教学](http://www.raywenderlich.com/76020/using-uigesturerecognizer-with-swift-tutorial)
现在, 还记得我们怎么设置ViewController实现UIScrollViewDelegate的吗? 接下来, 你将会实现这个协议中需要用到的若干方法。 在类中添加下面的方法:
    func viewForZoomingInScrollView(scrollView: UIScrollView!) -> UIView! {
      return imageView
    }
这是scroll view实现缩放机制的核心。你指出了当进行捏合操作的时候哪一个view将会被放大或者缩小。也就是我们的imageView。
最后在类中添加代理方法:
    func scrollViewDidZoom(scrollView: UIScrollView!) {
      centerScrollViewContents()
    }
当用户完成缩放操作时这个方法就会被调用。这里你需要重新将试图放置到中心。 如果不这样做，scroll view就不会自然地进行缩放，他会出现在左上角。
深呼吸, 调整一下你的姿势然后重新构建和运行你的工程！ 单击Image Scroll，如果事情进展的很顺利的话，你会看到一张图片，你可以缩放浏览和双击。耶！
![Scroll View Run 2](http://cdn4.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_2-281x500.png)
## 滚动和缩放视图层级
如果在scroll view里有多于一张图片会怎么样呢？如果你有一个复杂的显示层次需要缩放和浏览呢？好的，这里就有这样一个scroll view！此外，你只需要在上一步的基础上做很少的改动。
使用iOS\Source\Cocoa Touch Class subclass模版创建一个新的文件。将类命明为CustomScrollViewController并将父类设置为UIViewController。确保“Also create XIB file”没有选中，语言使用Swift。 单击Next将文件保存到工程目录中。
打开CustomScrollViewController.swift替换为下面内容：
    import UIKit
    class CustomScrollViewController: UIViewController, UIScrollViewDelegate {
      @IBOutlet var scrollView: UIScrollView!
    }
接下来, 打开Main.storyboard, 添加一个新的View Controller并和table的第二行通过push segue 进行连接。 将view controller的class修改成刚创建的CustomScrollViewController。
添加一个新的Scroll View并连接outlet和设置view controller作为它的delegate，就和上面的步骤一样。
然后, 打开CustomScrollViewController.swift并在你的scrollView outlet下面添加如下属性:
    var containerView: UIView!
和上一节的view controller的不同点是，你有一个叫做containerView的UIView，而上一节你用的是UIImageView。 这对如何他将工作是一个提示。
现在, 实现viewDidLoad方法.
    override func viewDidLoad() {
      super.viewDidLoad()
      // 设置container view来保持你定制的视图层次
      let containerSize = CGSize(width: 640.0, height: 640.0)
      containerView = UIView(frame: CGRect(origin: CGPoint(x: 0, y: 0), size:containerSize))
      scrollView.addSubview(containerView)
      // 设置你定制的视图层次
      let redView = UIView(frame: CGRect(x: 0, y: 0, width: 640, height: 80))
      redView.backgroundColor = UIColor.redColor();
      containerView.addSubview(redView)
      let blueView = UIView(frame: CGRect(x: 0, y: 560, width: 640, height: 80))
      blueView.backgroundColor = UIColor.blueColor();
      containerView.addSubview(blueView)
      let greenView = UIView(frame: CGRect(x: 160, y: 160, width: 320, height: 320))
      greenView.backgroundColor = UIColor.greenColor();
      containerView.addSubview(greenView)
      let imageView = UIImageView(image: UIImage(named: "slow.png"))
      imageView.center = CGPoint(x: 320, y: 320);
      containerView.addSubview(imageView)
      // 告诉scroll view内容的尺寸
      scrollView.contentSize = containerSize;
      // 设置最大和最小的缩放系数
      let scrollViewFrame = scrollView.frame
      let scaleWidth = scrollViewFrame.size.width / scrollView.contentSize.width
      let scaleHeight = scrollViewFrame.size.height / scrollView.contentSize.height
      let minScale = min(scaleWidth, scaleHeight)
      scrollView.minimumZoomScale = minScale
      scrollView.maximumZoomScale = 1.0
      scrollView.zoomScale = 1.0
      centerScrollViewContents()
    }
viewDidLoad使用一个独立的根视图（root view）设置一个现实层级, 这个root view就是你的实例变量containerView。 然后你讲这个独立的视图添加到scroll view中。 这里就是关键所在 – 由于代理的回调viewForZoomingInScrollView只能返回一个视图，因此你只能想scroll view中添加一个视图。将zoomScale设置为1而不是minScale，这样内容视图就会在正常尺寸而不是适合屏幕的大小。
再次实现centerScrollViewContents和UIScrollViewDelegate的两个代理方法， 将原始版本中的imageView替换为containerView。
    func centerScrollViewContents() {
      let boundsSize = scrollView.bounds.size
      var contentsFrame = containerView.frame
      if contentsFrame.size.width < boundsSize.width {
        contentsFrame.origin.x = (boundsSize.width - contentsFrame.size.width) / 2.0
      } else {
        contentsFrame.origin.x = 0.0
      }
      if contentsFrame.size.height < boundsSize.height {
        contentsFrame.origin.y = (boundsSize.height - contentsFrame.size.height) / 2.0
      } else {
        contentsFrame.origin.y = 0.0
      }
      containerView.frame = contentsFrame
    }
    func viewForZoomingInScrollView(scrollView: UIScrollView!) -> UIView! {
      return containerView
    }
    func scrollViewDidZoom(scrollView: UIScrollView!) {
      centerScrollViewContents()
    }
注意: 你会发现这里没有使用UITapGestureRecognizer. 这只是为了使这部分的教学更简单明了。你可以添加这个功能作为额外的练习。
现在构建和运行你的工程。 这次选择Custom View Scroll，你会惊奇的发现你可以浏览和缩放你手动创建的一个UIView场景！
![Scroll View Run 3](http://cdn5.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_3-281x500.png)
## 使用UIScrollView进行分页
在本教程的第三部分, 你会创建一个允许翻页的scroll view。 也就是说当你停止拖拽的时候scroll view会锁定到一页。就像在AppStore中你查看某个应用的截图一样。
使用iOS\Source\Cocoa Touch Class subclass模版创建名为PagedScrollViewController的类， PagedScrollViewControllerand继承自UIViewController。 确保Also create XIB file的复选框没有选中，编程语言选择Swift. 单击Next保存到工程。
打开PagedScrollViewController.swift替换城下面的内容:
    import UIKit
    class PagedScrollViewController: UIViewController, UIScrollViewDelegate {
      @IBOutlet var scrollView: UIScrollView!
      @IBOutlet var pageControl: UIPageControl!
    }
接下来, 打开Main.storyboard，像之前操作的那样添加View Controller使用push segue连接table的第三行。 设置view controller的class为刚创建的PagedScrollViewController。
设置指示图的background color为黑色（black）， 这样你要添加的page control才能可见 – background color缺省情况下是白色, 你是无法看到覆盖在白色view上的白色view的!
添加Page Control元素到view的底部并使他宽度上充满整个view。 连接到pageControl outlet。
你还要添加并链接一个Scroll View到创建的outlet, 设置view controller作为代理, 具体步骤不再赘述。 调整Scroll View的尺寸填满view controller但要为Page Control留下一些空间。
这一次, 通过Attributes Inspector打开scroll view的Paging Enabled选项。
![Paging-enabled](http://cdn4.raywenderlich.com/usr/uploads/2012/03/Paging-enabled.png)
现在PagedScrollViewController.swift文件并在outlets之后添加下列属性:
    var pageImages: [UIImage] = []
    var pageViews: [UIImageView?] = []
你会注意带这次和之前有些不同， 这次没有container view,而是两个数组。
- pageImages: 这个数组保存要显示的图片，每页显示一张。
- pageViews: 这个数组保存每页显示图片用的UIImageView对象。由于我们会延迟加载（当使用的时候再加载），因此它是一个可选数组，因此你要处理数组中的空值。
接下来实现viewDidLoad:
override func viewDidLoad() {
      super.viewDidLoad()
      // 1
      pageImages = [UIImage(named: "photo1.png"),
        UIImage(named: "photo2.png"),
        UIImage(named: "photo3.png"),
        UIImage(named: "photo4.png"),
        UIImage(named: "photo5.png")]
      let pageCount = pageImages.count
      // 2
      pageControl.currentPage = 0
      pageControl.numberOfPages = pageCount
      // 3
      for _ in 0..
将上面的函数进行分解，详细介绍一下每个步骤:
- 
首先, 设置pageImages数组. 之前向工程中添加了5张照片。这个数组会包含着5张图片。
- 
页面索引从0开始, 设置pageControl的第一页和总页数。
- 
接下来, 设置pageViews，它包含UIImageView对象. 开始时还没有页面载入进来，我们使用nil对象占位-数组下标对应一个页面。之后会使用可选绑定检查页面是否已经加载。
- 
scrollView需要知道他的内容的尺寸。 你需要一个支持水平的页面滚动（如果想使用竖直页面滚动也是很容易修改的）的scroll view，使用页面总数乘以pagesScrollViewSize的宽度的乘积作为scrollView的内容宽度，内容的高度与pagesScrollViewSize的高度相同。
- 
在初始化是你需要显示某些页面，所以调用了loadVisiblePages方法, 之后我们会实现它.
向类中添加谢列方法:
    func loadPage(page: Int) {
      if page < 0 || page >= pageImages.count {
        // 如果超出了页面显示的范围，什么也不需要做
        return
      }
      // 1
      if let pageView = pageViews[page] {
        // 页面已经加载，不需要额外操作
      } else {
        // 2
        var frame = scrollView.bounds
        frame.origin.x = frame.size.width * CGFloat(page)
        frame.origin.y = 0.0
        // 3
        let newPageView = UIImageView(image: pageImages[page])
        newPageView.contentMode = .ScaleAspectFit
        newPageView.frame = frame
        scrollView.addSubview(newPageView)
        // 4
        pageViews[page] = newPageView
      }
    }
每个页面都存储在了可选数组中。当view controller加载时, 使用nil填充数组， 这个方法会加载每个页面的内容:
- 
首先, 使用可先绑定检查是否已经加载了视图，如果pageView包含UIImageView就什么也不需要做，忽略后续处理。
- 
如果pageView是nil就需要创建一个页面。 先计算出页面的frame大小。 它与scrollView具有相同的尺寸，y方向偏移为0，将页面尺寸乘以页面序号作为x方向(水平方向)的偏移量。
- 
创建新的UIImageView设置并添加到scrollView.
- 
最后, 使用新创建的newPageView替换原来的nil对象。这样如果需要再次加载这个页面，程序会进入if分支并且不需要额外的操作了,因为页面已经创建过了。
接下来, 向类中添加如下函数:
    func purgePage(page: Int) {
      if page < 0 || page >= pageImages.count {
        // 如果超出要显示的范围，什么也不做
        return
      }
      // 从scrollView中移除页面并重置pageViews容器数组响应页面
      if let pageView = pageViews[page] {
        pageView.removeFromSuperview()
        pageViews[page] = nil
      }
    }
这个函数会清理之前使用loadPage创建的一个页面，它首先检查pageViews数组中这个页面的对象是否是nil,如果不是nil就从scroll view中移除这个对象并把pageViews中对应页面再次设置成nil，表明这个页面已经清理完成。
你问为什么要这么麻烦的使用延迟加载和清理页面？好的, 在本例中, 你在开始的时候加载所有的页面并没什么关系，因为一共只加载5个页面，这并不会消耗太多内存。但如果你有100个页面要加载，每张图5MB。如果你一次加载所有的页面这将会占用500MB的内存! 你的应用很快就会耗尽可用的内存空间并被操作系统杀死。延迟加载意味着你在给定时刻在内存里只有一定数量的页面。
上面的loadPage和purgePage两个函数通过loadVisiblePages函数联合在一起。向类中添加这个方法:
    func loadVisiblePages() {
      // 首先确定当前可见的页面
      let pageWidth = scrollView.frame.size.width
      let page = Int(floor((scrollView.contentOffset.x * 2.0 + pageWidth) / (pageWidth * 2.0)))
      // 更新pageControl
      pageControl.currentPage = page
      // 计算那些页面需要加载
      let firstPage = page - 1
      let lastPage = page + 1
      // 清理firstPage之前的所有页面
      for var index = 0; index < firstPage; ++index {
        purgePage(index)
      }
      // 加载范围内（firstPage到lastPage之间）的所有页面
      for index in firstPage...lastPage {
        loadPage(index)
      }
      // 清理lastPage之后的所有页面
      for var index = lastPage+1; index < pageImages.count; ++index {
        purgePage(index)
      }
    }
这里先计算出scroll view当前所在的页面，更新pageControl然后加载和清理相关的页面。计算当前所在页面的方式有些可怕，但也没那么坏。你可以通过计算几个页面来确信这是正确的。(注意floor()函数会取里传入的值最近的最小整数)
你预先加载当前页面和当前页面相邻的页面。因此当用户开始滚动页面的时候用户可以在下一个页面居中的时候可以看到下一个页面。如果你想的话你也可以加载前后两个甚至三个页面，但这除了增加内存外没有其他用处。
最后要做的事就是实现UIScrollView协议。 这次只需要实现scrollViewDidScroll(). 添加到 PagedScrollViewController.swift:
    func scrollViewDidScroll(scrollView: UIScrollView!) {
      // Load the pages that are now on screen
      loadVisiblePages()
    }
所有这些是为了确保当页面滚动完后相关的页面总会加载完毕(不需要的页面被清理完毕)。
构建和运行工程, 选择Paged然后对你创建的scroll view的奇妙页面感到惊奇吧！
![Scroll View Run 4](http://cdn3.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_4-281x500.png)
## 显示前面后面的页面
这个工程的最后一部分, 我会向你展示如何是scroll view像App Store中的应用截图那样展示。你可以看到前个页面和后一个页面的一部分，这是一个有用的技术，因为用户可以立即看到是否有额外的内容可以滚动。
使用iOS\Source\Cocoa Touch Class subclass模版创建名为PeekPagedScrollViewController的类并将他的父类设置为UIViewController。确保Also create XIB file没有选中，语言使用Swift。单击 Next保存到工程.
打开PeekPagedScrollViewController.swift替换为如下内容:
    import UIKit
    class PeekPagedScrollViewController: UIViewController, UIScrollViewDelegate {
      @IBOutlet var scrollView: UIScrollView!
      @IBOutlet var pageControl: UIPageControl!
      var pageImages: [UIImage] = []
      var pageViews: [UIImageView?] = []
    }
实现viewDidLoad方法, 这和前一部分得内容是一样的：
override func viewDidLoad() {
      super.viewDidLoad()
      // 设置需要滚动和缩放的图片
      pageImages = [UIImage(named: "photo1.png"),
        UIImage(named: "photo2.png"),
        UIImage(named: "photo3.png"),
        UIImage(named: "photo4.png"),
        UIImage(named: "photo5.png")]
      let pageCount = pageImages.count
      // 设置pageControl
      pageControl.currentPage = 0
      pageControl.numberOfPages = pageCount
      // 设置保存每个页面的view的数组
      for _ in 0..
之后, 实现loadVisiblePages, loadPage:, purgePage:和scrollViewDidScroll: UIScrollView 代理函数, 这些函数也和上一部分的函数内容相同, 只有loadPage有一点不一样，下面会进行解释：
    func loadVisiblePages() {
      // 首先确定当前可见页面
      let pageWidth = scrollView.frame.size.width
      let page = Int(floor((scrollView.contentOffset.x * 2.0 + pageWidth) / (pageWidth * 2.0)))
      // 更新pageControl
      pageControl.currentPage = page
      // 计算哪些页面需要加载
      let firstPage = page - 1
      let lastPage = page + 1
      // 清理firstPage之前页面
      for var index = 0; index < firstPage; ++index {
        purgePage(index)
      }
      // 加载范围内的页面
      for index in firstPage...lastPage {
        loadPage(index)
      }
      // 清理lastPage之后的页面
      for var index = lastPage+1; index < pageImages.count; ++index {
        purgePage(index)
      }
    }
    func loadPage(page: Int) {
      if page < 0 || page >= pageImages.count {
        // 如果在显示的范围外，什么也不做
        return
      }
      // 加载特定页面，首先检查是否已经加载
      if let pageView = pageViews[page] {
        // 已经加载，什么也不需要做
      } else {
        var frame = scrollView.bounds
        frame.origin.x = frame.size.width * CGFloat(page)
        frame.origin.y = 0.0
        frame = CGRectInset(frame, 10.0, 0.0)
        let newPageView = UIImageView(image: pageImages[page])
        newPageView.contentMode = .ScaleAspectFit
        newPageView.frame = frame
        scrollView.addSubview(newPageView)
        pageViews[page] = newPageView
      }
    }
    func purgePage(page: Int) {
      if page < 0 || page >= pageImages.count {
        // 如果在显示的范围外，什么也不做
        return
      }
      // 从scroll view移除页面并重置容器数组
      if let pageView = pageViews[page] {
        pageView.removeFromSuperview()
        pageViews[page] = nil
      }
    }
    func scrollViewDidScroll(scrollView: UIScrollView!) {
      // 加载现在在屏幕中的页面
      loadVisiblePages()
    }
和上一节的实现不同的是loadPage函数的下面一行代码:
    frame = CGRectInset(frame, 10.0, 0.0)
这行代码设置image view的frame有一个小的水平偏移，以至于这些页面不能触摸。就像App Store中应用截图显示的那样。
现在打开Main.storyboard, 像之前的示例那样，添加View Controller并使用push segue连接到table的第四行，设置view controller的class为刚刚添加的PeekPagedScrollViewController。
像之前一样, 设置主视图的背景为黑色, 添加Page Control元素并进行连接. 同样添加并连接一个“Paging Enabled” Scroll View到创建按的outlet, 设置view controller作为他的代理。
使scroll view比屏幕稍小一点 – 建议240×312 – 放置到屏幕中心. 最后会如下图所示:
![Peek paged scroll view](http://cdn1.raywenderlich.com/usr/uploads/2014/07/Peek_paged_scroll_view-295x500.png)
之后, 去掉scroll view的Clip Subviews选项. 这将允许绘制视图外面的部分，这对查看页面很重要。
![Clip subviews](http://cdn5.raywenderlich.com/usr/uploads/2014/07/Clip_subviews.png)
构建和运行, 选择Paged with peeking, 这就是你想要的! 干得好!
![Scroll View Run 5](http://cdn3.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_5-281x500.png)
## 检测Scroll View外面的触摸
你可能已经注意到查看的页面，你不能点击到scroll view外面的部分。这还不太理想，不是吗？我们可以修正它！
问题是scroll view只能得到在他的边界内的触摸， 现在它的边界比绘制区域小（因为关闭了Clip SubViews), 这会丢失一些触摸。你将通过将scroll view包装到一个容器类来修复这个问题，这个容器类的任务是拦截触摸时间并将它切换到scroll view。
使用iOS\Source\Cocoa Touch Class subclass模版创建类名为ScrollViewContainer的文件，将他的父类设置为UIView。 语言设置为Swift单击Next保存到工程。
打开ScrollViewContainer.swift替换长下面的内容:
    import UIKit
    class ScrollViewContainer: UIView {
      @IBOutlet var scrollView: UIScrollView!
      override func hitTest(point: CGPoint, withEvent event: UIEvent!) -> UIView? {
        let view = super.hitTest(point, withEvent: event)?
        if let theView = view {
          if theView == self {
            return scrollView
          }
        }
        return view
      }
    }
很简单，对吗？我打赌你认为会有很有很多代码。好的，不是现在。它所要做的事情是将在容器边界内触发的触摸移交到scroll view。
现在你需要使用你新创建的容器。
打开Main.storyboard后退这个例子中的Peek Paged Scroll View Controller。 选择scroll view然后选择Editor\Embed In\View. 这件创建一个新的view，然后将scroll view放到新的view中。
设置新的view的宽度（width）为屏幕宽度高度为scroll view的高度。然后设置他的class为 ScrollViewContainer。 连接容器的outletdao scroll, 如下图所示:
![Embed in container](http://cdn3.raywenderlich.com/usr/uploads/2014/07/Embed_in_container-463x500.png)
将容器的背景色设置为明晰的颜色, 因为他在主视图的上层, 而容器类的默认背景是白色的。
构建并运行。选择Paged with peeking。 就像你想要的那样，现在你可以点击到scroll view的边界之外了。耶！只用几行代码就搞定了是多么酷啊！
![Scroll View Run 6](http://cdn4.raywenderlich.com/usr/uploads/2014/07/Scroll_View_Run_6-281x500.png)
## 下一步可以做什么？
这里是包含本教程所有代码的[示例工程](http://cdn5.raywenderlich.com/usr/uploads/2014/08/ScrollViews-Swift.zip)。
你已经研究了scroll view支持的许多有趣的特性。如果你对本片教程所做的事情很有信心你可以尝试下面的内容:
- 创建竖直翻页的scroll view。
- 在分页的scroll view中嵌入可以缩放的scroll view这样每个页面都可以独立的浏览和缩放。
- 通过在水平翻页的scroll view中嵌入数值翻页的scroll view制造2D网格的假象。
现在来使用你获得的scroll view的技能制作优秀的应用吧！
关于本教程，如果你遇到任何问题或者有任何反馈，加入下面的讨论吧。
标签: [Swift](http://www.tairan.com/tag/Swift/), [UIScrollView](http://www.tairan.com/tag/UIScrollView/)

