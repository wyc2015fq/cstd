# iOS9自动布局神器StackView - =朝晖= - 博客园
# [iOS9自动布局神器StackView](https://www.cnblogs.com/dhcn/p/7130922.html)
http://www.jianshu.com/p/767f72b7d754
     这篇文章紧跟上边autolayout的一些小技巧，如果你没有看过，不防先看下《你真的会用autolayout吗？》这篇文章。上篇讲到autolayout的一些小技巧，但是还是免不了要加那些烦人的约束，自己去计算约束。而且autolayout出来这么久了，依然有好多应用在用手动布局。当然不是说手动布局有什么不好，有些特别复杂的界面还是要用手动布局的。主要是现在的autolayout对稍微复杂一些的界面加约束是特别头疼的一件事情，约束太多了，而且好多是重复繁杂的。iOS9新特性StackView，让适配变得非常简单，你甚至不用加约束。只需要简单设置几个属性。不多说，还是老规矩先上图。
![](https://upload-images.jianshu.io/upload_images/745638-036908af5ce53932.gif?imageMogr2/auto-orient/strip)
StackView
      可以看到，我这里除了最外面的垂直StackView加了上下左右四个约束，其余的没有加一个约束，就适配了。
     由于这里步骤较多，分开讲解一下
![](https://upload-images.jianshu.io/upload_images/745638-fe876d44f3d9d758.gif?imageMogr2/auto-orient/strip)
Vertical Stack View
首先添加一个纵向的StackView。这里不多解释。
![](https://upload-images.jianshu.io/upload_images/745638-0b559f3205450652.gif?imageMogr2/auto-orient/strip)
属性
这里解释一下这几个属性的含义：
Axis:StackView的布局属性（水平和垂直两个方向）
Alignment：设置子视图的对其方式（水平和垂直有所差别）
    1）Fill：子视图填充StackView。
    2）Leading：靠左对齐。
    3）Trailing：靠右对齐。
    4）Center：子视图以中线为基准对齐。
    5）Top：靠顶部对齐。
    6）Bottom：靠底部对齐。
    7）First Baseline：按照第一个子视图中文字的第一行对齐。
    8）Last Baseline：按照最后一个子视图中文字的最后一行对齐。
Distribution：设置子视图的分布比例
    1）Fill：默认分布方式。
    2）Fill Equally：子视图的高度或宽度保持一致。
    3）Fill：Proportionally：StackView自己计算出它认为合适的分布方式。
    4）Equal Spacing：子视图保持同等间隔的分布方式。
    5）Equal Centering：每个子视图中心线之间保持一致的分布方式。
Spacing：设置子视图之间的间距
![](https://upload-images.jianshu.io/upload_images/745638-369f2369fc0eac6d.gif?imageMogr2/auto-orient/strip)
Horizontal Stack View
然后向垂直StackView中放三个水平StackView，可以看出StackView是可以嵌套的，三个水平StackView按照之前设置好的属性，等宽登高，间距为10排列。我这里设置三个水平StackView的属性是全选统一设置的。
![](https://upload-images.jianshu.io/upload_images/745638-62f62378c2677dbd.gif?imageMogr2/auto-orient/strip)
扔子视图
这步没什么要解释的，扔子视图就行了，不用加一个约束。
下面再试下在iPhone5上效果
![](https://upload-images.jianshu.io/upload_images/745638-e3188a488906e56d.gif?imageMogr2/auto-orient/strip)
适配
      StackView只在iOS8和iOS9上能用，iOS7不支持，所以这半年用到的应该不会太多，根据以往惯例，明年估计将有很多应用不再对iOS7适配，StackView让适配变得非常简单。这里仅以9宫格为例，当然StackView还有很多强大之处。尤其是对于动态单元格的布局，更加有利。我这里就不再过多介绍，后面有可能我会把，我项目里面有动态单元格的用StackView单独做一个出来。
收工，感谢支持！

