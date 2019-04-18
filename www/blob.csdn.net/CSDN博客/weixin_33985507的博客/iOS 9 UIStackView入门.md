# iOS 9: UIStackView入门 - weixin_33985507的博客 - CSDN博客
2017年04月18日 15:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

## <欢迎大家增加iOS开发学习交流群：QQ529560119>
## iOS 9: UIStackView入门
![photo-1421757295538-9c80958e75b0.jpg](http://cc.cocimg.com/api/uploads/20150623/1435028656700495.jpg)
同iOS以往每一个迭代一样。iOS 9带来了非常多新特性。
UIKit框架每一个版本号都在改变，而在iOS 9比較特别的是UIStackView，它将从根本上改变开发人员在iOS上创建用户界面的方式。
本文将带你学习如何使用UIStackView创建用户界面。
本文假定你已经熟悉Auto Layout基础。
不熟悉的能够先看看[Auto Layout](http://code.tutsplus.com/tutorials/getting-started-with-auto-layout-in-xcode-5--cms-21016)教程。要理解Stack
 View为何如此实用及它是怎样起作用的，须要首先对Auto Layout有深入了解。
**1. 实例预览**
我们将使用UIStackView模拟一个app评分提示。
用户能够添加星星或者移除星星来打分。
完毕后看起来是这样。
![tpsv_Finished.png](http://cc.cocimg.com/api/uploads/20150623/1435027220398746.png)
先从[GitHub下载例子工](https://github.com/tutsplus/iOS-StackViewStarterProject)程并打开。
在Main.Storyboard里有两个Stack View
![tpsv_InitialSB.png](http://cc.cocimg.com/api/uploads/20150623/1435027282704488.png)
我们将使用这两个Stack View来布局界面。
開始编码前。让我们先看一下Stack View是怎样工作的。
**2. UIStackView 概述**
Stack View的核心便是方便垂直或水平排布多个subview。假设你做过Android开发。那它和[LinearLayout](http://developer.android.com/guide/topics/ui/layout/linear.html) 控件很相似。
Stack View最实用的就是它会自己主动为每一个subview创建和加入Auto Layout constraints。当然你能够控制subview的大小和位置。
能够通过选项配置subview的大小、排布以及彼此间的间距。
**布局内容**
打开Main.Storyboard。选择当中一个Stack View能够查看其选项，并选中一个Stack View。在 Attributes Inspector中。注意Stack View以下列出的选择。
![tpsv_SVOptions.png](http://cc.cocimg.com/api/uploads/20150623/1435027330575371.png)
Axis表示Stack View的subview是水平排布还是垂直排布。Alignment控制subview对齐方式。
Distribution定义subview的分布方式。Spacing 为subview间的最小间距。
把术语简化一下，你可这样理解：Alignment 用于控制X 和 Y值，而Distribution 用于控制高度和宽度。另两个值都会影响对齐。假设选中Baseline Relative将依据subview的基线调整垂直间距。假设选中Layout Margins Relative 将相对于标准边界空白来调整subview位置。
还有一个须要记住的是，Stack View会被当成Container View。所以它是一个不会被渲染的UIView子类。它不像其它UIView子类一样。会被渲染到屏幕上。这也意味着设置其backgroundColor属性或重载drawRect:方法都不会产生不论什么效果。
**subView和arrangedSubView**
開始使用Stack View前。我们先看一下它的属性subViews和arrangedSubvies属性的不同。假设你想加入一个subview给Stack View管理，你应该调用addArrangedSubview:或insertArrangedSubview:atIndex: arrangedSubviews数组是subviews属性的子集。
要移除Stack View管理的subview。须要调用removeArrangedSubview:和removeFromSuperview。移除arrangedSubview仅仅是确保Stack View不再管理其约束，而非从视图层次结构中删除。理解这一点很重要。
如今我们对Stack View有一定了解。開始使用它。
**3. 配置垂直布局的Stack View**
打开Main.Storyboard选择上面的Stack View。对Attributes Inspector作例如以下改变：
- 
Alignment 设为 Center
- 
Distribution 设为 Equal Spacing
- 
Spacing 设为 30
这告诉Stack View为subview加入约束，使其垂直居中并沿Stack View的轴线对称。然后为subview设置边距30。
假设subview大小和Stack View内容区不相符，将依据compression resistance priorities对subview进行拉伸或压缩。在执行时给Stack View加入subview也相同会如此。
布局有不论什么歧义Stack View都会依据subview在arrangedSubviews中index一步步回退去调整subview的大小。直至其刚好适应Stack View的大小。
**4. 给Stack View加入垂直布局的Subview**
加入一个label。一个image view和一个button到上面的Stack View里。label在最上面，image view在中间。button在以下。
加入完毕后看起来是这样：
![30.png](http://cc.cocimg.com/api/uploads/20150623/1435027439562460.png)
接下来，我们要在Attributes Inspector里改动一下刚才加入的subview的属性。把label的文本秘诀成“How do you likeour app?”， Text Alignment选择Center 。image view的Image 输入“logo”。 Content Mode选Aspect Fit。
最后把button的Text 设置成“Add Star!”。
执行app。能看到我们仅仅做了非常少工作。但已经加入了三个能响应方向、size class等改变的subview。
其实你并不需手动加入不论什么约束。
当app执行时，点击Xcode窗体底部Debug View Hierarchybutton能够进行[实时视图调试](http://code.tutsplus.com/tutorials/view-debugging-in-xcode-6--cms-22530)
![tpsv_ViewDebugging.png](http://cc.cocimg.com/api/uploads/20150623/1435027512407710.png)
选择先前加入的随意一个subview。查看size inspector，我们注意到Stack View已经自己主动为其加入了约束。下图显示的是为button加入的约束
![tpsv_Constraints.png](http://cc.cocimg.com/api/uploads/20150623/1435027546248907.png)
**5. 加入五角星**
button事件还没和app界面关联，我们先关联起来。停止执行app，打开storyboard。创建一个名为addStar的IBAction 关联到button的Touch Up Inside事件。
![tpsv_AddStar.png](http://cc.cocimg.com/api/uploads/20150623/1435027576854683.png)
addStar(_:)方法实现：
```
`@IBAction func addStar(sender: AnyObject) {`
`    ``let starImgVw:UIImageView = UIImageView(image: UIImage(named: ``"star"``))`
`    ``self.horizontalStackView.addArrangedSubview(starImgVw)`
`    ``UIView.animateWithDuration(0.25, animations: {`
`        ``self.horizontalStackView.layoutIfNeeded()`
`    ``})`
`}`
```
给水平布局的Stack View里的星星image加入动画。
记住，因为Stack View自己主动为我们管理Auto Layout constraints，我们仅仅能调用layoutIfNeeded来实现动画。
编译执行app，点击add starbutton。能看到最后结果却不尽人意。
![tpsv_SkewedStar.png](http://cc.cocimg.com/api/uploads/20150623/1435027618872997.png)
选择以下的Stack View，查看Attributes Inspector会看到问题所在。
因为Alignment 和Distribution 都设置成了Fill，Stack View 拉伸了星星以适应其大小。
这在加入很多其它的星星的时候会引起很多其它问题。我们希望星星居中显示。而不是被拉伸来适应Stack View的宽度。
改动Alignment 的值为Center 。改动Distribution 的值为Fill Equally。
最后在addStar(_:)方法中设置image view的内容。
```
`@IBAction func addStar(sender: AnyObject) {`
`    ``let starImgVw:UIImageView = UIImageView(image: UIImage(named: ``"star"``))`
`    ``starImgVw.contentMode = .ScaleAspectFit`
`    ``self.horizontalStackView.addArrangedSubview(starImgVw)`
`    ``UIView.animateWithDuration(0.25, animations: {`
`        ``self.horizontalStackView.layoutIfNeeded()`
`    ``})`
`}`
```
执行app。点几次加入button，我们发现全部星星居中了。
![21.png](http://cc.cocimg.com/api/uploads/20150623/1435027704921191.png)
**6. Stack View嵌套**
不能删除星星，我们的app评分什么用处不大。打开storyboard，加入一个水平布局的Stack View到上面的Stack View里。把它放置在logo之下，button之上。
![22.png](http://cc.cocimg.com/api/uploads/20150623/1435027786839852.png)
把“Add Star!”button拖到新加入的Stack View里。再加入一个button到新的Stack View里。
改变button的title为“Remove Star”，文本颜色设为red。预览例如以下：
![23.png](http://cc.cocimg.com/api/uploads/20150623/1435027795436834.png)
在Attributes Inspector中编辑新Stack View的属性，改变例如以下：
- 
Alignment 设为 Center
- 
Distribution 设为 Equal Spacing
- 
Spacing 设为 10
![25.png](http://cc.cocimg.com/api/uploads/20150623/1435027836351375.png)
**7. 删除五角星**
为“Remove Star”button创建名为removeStar，事件类型为Touch Up Inside的IBAction响应方法。
![26.png](http://cc.cocimg.com/api/uploads/20150623/1435027880805497.png)
removeAction(_:)实现：
```
`@IBAction func removeStar(sender: AnyObject) {`
`    ``let star:UIView?`
` = self.horizontalStackView.arrangedSubviews.last`
``
`    ``if``let aStar = star`
`    ``{`
`        ``self.horizontalStackView.removeArrangedSubview(aStar)`
`        ``aStar.removeFromSuperview()`
`        ``UIView.animateWithDuration(0.25, animations: {`
`            ``self.horizontalStackView.layoutIfNeeded()`
`        ``})`
`    ``}`
`}`
```
执行app。如今既可添加，也可删除了。改变模拟器方向或者旋转设备看看app会如何调整其界面。我们并未加入一行约束就构建好了app的用户界面。
须要注意的是：removeStar(_:)里调用removeFromSuperview是把subview从视图层级中移除。再次调用removeArrangedSubview(_:)仅仅是告诉Stack View不再须要管理subview的约束。而subview会一直保持在视图层级结构中直到调用removeFromSuperview把它移除。
**结论**
UIStackView类大大简化了用户界面开发。这是好事，特别是随着硬件的改变。
使用UIStackView。降低了开发人员为简单场景设置枯燥的约束，把繁杂的工作交给了UIKit。
假设对文中的不论什么知识点感兴趣。能够从 [GitHub](https://github.com/tutsplus/iOS-StackViewFinishedProject)下载完整project。
