# iOS 9: UIStackView 浅析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月24日 22:22:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：842
10/14 更新： 最近的见闻让我觉得自己的这篇浅析太过浅薄无知了。留着等有了足够好的认识后重写一下。
iOS 9中增加了新的控件：[UIStackView](https://developer.apple.com/library/prerelease/ios/documentation/UIKit/Reference/UIStackView_Class_Reference/index.html)，看下图就明白作用了。本来直接链接了官方文档的图片，不知道为何不能显示，只能再上传一次。
![](http://upload-images.jianshu.io/upload_images/37334-66d17a0053d4b141.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
UIStackView Workflow
刚开始看到让我更多地想起 Word 里的文字排版，左对齐，居中之类，这里是对 `UIView` 子类对象排版。找相似的控件的话可以说是 LineLayout 布局的
`UICollectionView`，不过更加灵活，可横向可竖向，还可以在 IB 里实时看到效果。在 `UICollectionView` 弄成这样的布局，还得好好设置一番。
![](http://upload-images.jianshu.io/upload_images/37334-581e8ce74a510501.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
LineLayout
我想到的第一个用处就是制作一个角色海报墙，类似这种：
![](http://upload-images.jianshu.io/upload_images/37334-fde47d3da6775366.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
妇联2角色海报
这要是用 `UICollectionView` 来实现，要实现 dataSource 方法，在 IB 中设置 Cell，调整布局参数，而使用
`UIStackView`，只需要拖进去几个 `UIImageView`，就会自动对齐，在 IB 如下设置就会有上面的效果：
![](http://upload-images.jianshu.io/upload_images/37334-590ebc56c0f83d95.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
Setting
当然，这个的效果可能还有一些瑕疵，也浪费我几个时间来调整细节，但得不到满意的效果。`UIStackView` 虽然小巧灵活，但是要想更精致的效果，最终还得靠
`UICollectionView`，看到微博上有人说这个类可能是脱胎于 Apple Watch，很可能，在那么小的屏幕上，还要拖各种约束来进行对齐和调整位置真是要烦死。弄了一通，觉得用处不大吧。在推上，看见很多开发者对这个类很是兴奋：
来自 [twitter 上的看法](https://twitter.com/jlaube77/status/608448539069456384)：
![](http://upload-images.jianshu.io/upload_images/37334-322cebf2d8d0e786.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
布局
因为 `UIStackView` 可以嵌套，实现以上的有丝分裂很方便，要是用 `UICollectionView` 简直要命。还有一位开发者说，这么多年了，终于有可以不滑动的
`UITableView` 了。由于我经验尚浅，不知道这些需求怎么产生的，或者我的脑洞还太小，不知道这个可以方便实现什么效果。
总体而言，`UIStackView` 实现有对齐要求的视图布局非常非常得简单，而使用 `UICollectionView` 和
`UITableView` 来实现就比较麻烦；还可以实时在 IB 里预览效果。最后有点很不方便的是，因为 `UIStackView` 是利用
`UIView` 类的 [intrinsicContentSize 属性](https://developer.apple.com/library/prerelease/ios/documentation/UIKit/Reference/UIView_Class/index.html#//apple_ref/occ/instm/UIView/intrinsicContentSize)来计算布局，对不同长宽比以及分辨率的图片进行对齐和位置调整不是很给力，即使用 `UIView` 来进行封装来一层也基本无效，除了对原图片进行缩放，暂时还没有找到好的解决方案。
文／seedante（简书作者）
原文链接：http://www.jianshu.com/p/184e3fb94bd8
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
