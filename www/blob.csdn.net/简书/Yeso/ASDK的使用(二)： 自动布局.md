# ASDK的使用(二)： 自动布局 - 简书
# ASDK的使用(二)： 自动布局
在`AsyncDisplayKit2.0` 教程的第二部分中，了解在iOS应用程序中构建快速灵活的布局是多么容易。
![](https://upload-images.jianshu.io/upload_images/2092929-60aabff66bd0c247.png)
image
`AsyncDisplayKit` 的布局系统让你以声明式语法编写布局代码，快到令人难以置信。你可以编写一个能够在`view controller` 或者`iPad app` 的`popover` 中使用的节点。如果正确编写它的布局的话，你可以将这个节点迁移到新的环境中，而无需修改布局代码！
在本教程中，你将使用第一课中用过的`CardNode` 类，并利用它来学习布局约束。你会看到要实现它的布局约束是多么的简单，这正是你所期望的。
### 一： 自动布局的问题
你也许会问“用自动布局不行吗?”。使用自动布局，每个约束都会以表达式的形式放在表达式系统中。也就是说，每个约束都让约束解析器的执行时间以指数级增长。这种计算总是在主线程中进行的。
ASDK 的设计目标之一，就是尽可能地接近 `UIKit` 的`API`。不幸的是，自动布局是一个封闭的系统，无法告诉布局解析器在另一个线程中完成这个工作。
### 二： 开始
要开始，请在[此处](https://koenig-media.raywenderlich.com/uploads/2016/10/ASDKTutorial2Starter-3.zip)下载入门项目。因为接下来将学习的内容和布局规范相关，我们需要将本系列第一课中的完成项目的版本进行修改。
> 
注意：在完成AsyncDisplayKit 2.0教程之前，请务必查看第1部分，了解[Async Display Kit的介绍](https://www.raywenderlich.com/?p=124311)。
### 三： ASLayoutSpec 介绍
首先，科普一点历史。
布局规范在[构建 Paper 大事记](https://youtu.be/OiY1cheLpmI?t=14m56s)中提到的一个布局系统。目的是让节点及其子节点的位置、大小的计算和应用规范化和可重用。
在 ASDK 1.9.x 中，你可以创建异步的布局，但布局代码和 UIKit 中预布局的方式差不多。节点的子节点的大小在一个 calculateSizeThatFits: 方法中计算。这个 size 可被缓存并在随后的 -layout 方法中应用。节点的位置仍然需要进行老旧的数学方法计算——没有人喜欢和数学计算打交道。
呃，好吧，大部分人都不喜欢和数学打交道！
![](https://upload-images.jianshu.io/upload_images/2092929-749d63ef76d87d06.png)
image
好的，很好，大多数人不喜欢搞乱数学！
### 四： Layout Specs
使用ASDK 2.0，`ASDisplayNode`子类会实现 `-layoutSpecThatFits` 方法。 ASLayoutSpec对象确定所有子节点的大小和位置。在这样做时，布局规范还确定所述父节点的大小.
一个节点会返回一个`layout spec` 对象——通过 `-layoutSpecThatFits:`方法。这个对象会结算节点的大小，并循环计算它的所有子节点的大小和位置。
`ThatFits` 方法参数是一个`ASSizeRange`。它有两个`CGSize` 属性，一个最小`size`，一个最大`size`。分别定义该节点的最小尺寸和最大尺寸。
ASDK 提供了很多种`layout spec`。它们是：
- 1:`ASStackLayoutSpec`: 允许你定义一个水平或垂直的子节点栈。它的`justifyContent` 属性决定栈在相应方向上的子节点之间的间距。`alignItems`属性决定了它们在另一个坐标轴上的间距。这种`layout specs` 有点像`UIKit` 的`UIStackView`
- 2:`ASOverlayLayoutSpec`: 允许你拉伸一个元素横跨到另一个元素。被覆盖的对象必须要有一个固定的`content size`，否则无法工作
- 3:`ASRelativeLayoutSpec`: 一种相对布局，允许将一个东西以相对位置放置在它的有效空间内。参考一下“9-切片图”的 9 个切片。你可以让一个东西放在这 9 个切片中的某个上。.
- 4: `ASInsetLayoutSpec`: 一个 inset 布局，允许你在一个已有的对象的基础上添加某些间距。你想在你的 cell 四周加上经典的 iOS 16 像素的边距吗？用这个就对了。
### 五： ASLayoutElement协议
`Layout specs`负责管理一个或多个子节点。一个`layout spec` 的子节点应该是一个节点比如`ASTextNode` 或`ASImageNode`。或者除了节点之外，`layou spec`的子节点也可以是另一个`layout spec`。
噢，太不可思议了！
`Layout sepc` 必须实现`ASLayoutElement` 协议。`ASLayoutSpec`和`ASDisplayNode` 都实现了`ASLayoutElement`。因此这两者及其子类都可以作为`layout spec` 的子节点。
![](https://upload-images.jianshu.io/upload_images/2092929-550558b1a2828adb.png)
1.png
这个简单的概念带来的功能无比强大。最重要的一种`layout spec` 是`ASStackLayoutSpec`。它能够将一张图片和一个文本装在一起，也能够将一张图片和另一个 stack 装在一起。
你猜对了。该实战检验一下了！我指的是敲代码…
![](https://upload-images.jianshu.io/upload_images/2092929-242f9128dd88e1a8.png)
4.png
### 六: 添加动物图片
假设在你上班的时候，设计师发来一张图片，她希望新的动物百科 app 应当长成这个样子：
![](https://upload-images.jianshu.io/upload_images/2092929-f66c8e6f2d034a67.png)
动物截图.png
为了表达整个布局，首先需要将它分解成几个对应的`layout spec`。有时候可能会觉得无所适从，但别忘了，layout spec 的威力取决于它们能够组合起来有多容易。开始越简单越好。
稍稍的剧透一下，你可以用一个 stack 将上半部分和下半部分组合起来就是了。现在你明白了吧，你可以分别对两个分开的部分单独布局，然后将它们组合在一起。
解压缩启动项目并打开`RainforestStarter.xcworkspace`。找到`CardNode.m`的`layoutSpecThatFits` 方法。现在它只是简单地返回一个空的`ASLayoutSpec` 对象。
如果你运行 app，你会看到：
![](https://upload-images.jianshu.io/upload_images/2092929-2932fc0ba2db46a0.png)
3.png
呃，这只是开头。首先来一张动物图片怎样？
默认，一个网络图片节点没有内容，当然也没有固定尺寸（intrinsic size）。当然，从截屏图中你可以算出动物图片大概占据整屏宽度和 2/3 的屏幕高度。
要实现这个，将 return 语句替换为：
```
//1
CGFloat ratio = constrainedSize.min.height/constrainedSize.min.width;
//2
ASRatioLayoutSpec *imageRatioSpec = [ASRatioLayoutSpec 
                                            ratioLayoutSpecWithRatio:ratio 
                                                               child:self.animalImageNode];
//3
return imageRatioSpec;
```
依次记录每个编号的评论：
- 
1: 计算比例：首先以高/宽比的形式定义图片所用的比例。这里，你指定图片的高度为 cell 最小高度即屏幕高度的 2/3。
- 
2: 创建一个`ratio layout spec`然后，创建一个`ASRatioLayoutSpec`，使用我们算好的比例以及一个子节点`animalImageNode` 作为参数。
- 
3: 返回 spec：返回`imageRatioSpec`，用于指定 cell 的宽高。
运行 app，看看你的 layout spec 是什么样子：
呃，这么简单？因为只有图片拥有 size，cell 会自动被它撑大。
![](https://upload-images.jianshu.io/upload_images/2092929-afbf285f512abc98.png)
image.png
> 
注意：传递给`cell` 的`constrainedSize` 由一个最小值`(0,0)` 和一个最大值`(tableNodeWidth,INF)` 构成，这是为什么需要用`perfrerredFrameSize` 作为图片高度的原因。`preferredFrameSize`在第一课的 AnimalPageController 中进行赋值。
### 七: 添加渐变
现在您已拥有动物图像，下一个逻辑步骤是在其上添加渐变节点。 `ASOverlayLayoutSpec`只是作业的规范。
首先，在`imageRatioSpec`初始化后添加以下行：
```
ASOverlayLayoutSpec *gradientOverlaySpec = [ASOverlayLayoutSpec
                                              overlayLayoutSpecWithChild:imageRatioSpec 
                                                                 overlay:self.gradientNode];
```
在创建自己的`layout spec`时，总是用一个`spec` 将其他`spec` 包含起来。目前，这个 spec 就是`gradientOverlaySpec`。
将 return 语句替换为：
```
return gradientOverlaySpec;
```
运行 app，可以看到每个 imageNode 上面都会覆盖以一个渐变色。
![](https://upload-images.jianshu.io/upload_images/2092929-f0be4b35b7ff4cef.png)
image.png
每只鸟都覆盖着一层渐变色——很漂亮！
### 八: 加入动物名称
上半部唯一要做的就是显示动物的名字。
要完成上半部分，还剩下一件事情就是：显示动物名称。
这好像不是太难，只需要注意几个地方：
- 1: 名字应当放在渐变层之上。
- 2：名字应当位于动物图片的左下角
- 3: 左边留白 16 个像素，底部留白 8 个像素
你已经知道如何将文字放在哪个布局上面了。现在让我们付诸实践。
在`gradientOverlaySpec`之后添加以下行。
```
ASOverlayLayoutSpec *nameOverlaySpec = [ASOverlayLayoutSpec
                                          overlayLayoutSpecWithChild:gradientOverlaySpec 
                                                             overlay:self.animalNameTextNode];
```
然后，将 return 语句改成：
```
return nameOverlaySpec;
```
运行程序，你能看到文字了：
![](https://upload-images.jianshu.io/upload_images/2092929-7a8d34df6d3e3c02.png)
image.png
不错，现在需要把它挪到左下角。
这时需要解释一下了。在鸟的图片上有一些文字，因此你很自然地会将 nameOverlaySpec 添加到另一个 spec 中并放到指定的位置。这时你需要后头想想，你到底想要什么。
这里，你用 nameOverlaySpec 去包含某个对象，它是独立于已有内容之外的。
但你并不真的想把名字包含在内容中。你想要的是，告诉这个名字，它应当位于它的有效空间的左下角，然后将 spec 拉伸到占满整个有效空间。
### 九： ASRelativeLayoutSpec介绍
你真正需要的是`ASRelativeLayoutSpec`。
`ASRelativeLayoutSpec` 用一个`ASLayoutElement` 作为子对象，并把这个空间作为它的有效空间，然后根据你的需要放置子对象。
定义相对规范时，可以设置其`verticalPosition`和`horizo??ntalPosition`属性。
这两个属性可以是以下之一：
- ASRelativeLayoutSpecPositionStart
- ASRelativeLayoutSpecPositionCenter
- ASRelativeLayoutSpecPositionEnd
这些值可以组合，允许你把对象放在某个角、某条变或者它的有效空间的中央。
给你布置个作业，你能将这只青蛙放在它的有效空间的右边吗？
![](https://upload-images.jianshu.io/upload_images/2092929-03a7d1a3cddb07d5.png)
image.png
如果你回答“将`verticalPostion` 设为`ASRelativeLayoutSpecPositionCenter`，将`horizontalPosition` 设为`ASRelativeLayoutSpecPositionEnd`就对了!
现在再来点难一点的，下面这句将更有感觉。在`nameOverlaySpec`一句前加入这行：
```
ASRelativeLayoutSpec *relativeSpec = [ASRelativeLayoutSpec    
      relativePositionLayoutSpecWithHorizontalPosition:ASRelativeLayoutSpecPositionStart       
                                      verticalPosition:ASRelativeLayoutSpecPositionEnd 
                                          sizingOption:ASRelativeLayoutSpecSizingOptionDefault 
                                                 child:self.animalNameTextNode];
```
如你所见，我们将子对象的`horiazontalPosition` 设为`start`，`verticalPosition`设为`end`。用青蛙的话来说，它应该是这样：
现在你已经创建了一个`relative spec`，将`nameOverlaySpec` 定义修改为：
```
ASOverlayLayoutSpec *nameOverlaySpec = [ASOverlayLayoutSpec 
                                           overlayLayoutSpecWithChild:gradientOverlaySpec 
                                                              overlay:relativeSpec];
```
运行 app，你会看见：
好了！cell 的上半部分只剩下一件事情要干了。
### 十： ASInsetLayoutSpec 介绍
最后一件事情是让动物名称左边留白 16 像素，下边留白 8 个像素。这需要用到`ASInsetLayoutSpec`。
要在对象的四边添加留白，只需将对象放到 inset spec 中，并提供 UIEdgeInsets 来指定需要留白多少像素。
在`nameOverlaySpec` 之后添加一句：
```
ASInsetLayoutSpec *nameInsetSpec = [ASInsetLayoutSpec insetLayoutSpecWithInsets:UIEdgeInsetsMake(0, 16.0, 8.0, 0.0) child:nameOverlaySpec];
```
然后，将`return` 语句修改成返回最外层的`spec`：
```
return nameInsetSpec;
```
运行程序，你会看到：
你不需要将 inset 应用到整个 overlay 所包含的空间，因为它还包含了动物图片。
你真正的目的是将 inset 应用到`relativeSpec` 的有效空间。要解决这个问题，首先删除当前的`nameInsetSpec` 定义。
然后，在 nameOverlaySpec 定义之前加入这个修改过的版本：
```
ASInsetLayoutSpec *nameInsetSpec = [ASInsetLayoutSpec 
            insetLayoutSpecWithInsets:UIEdgeInsetsMake(0, 16.0, 8.0, 0.0) child:relativeSpec];
```
现在，你需要让`nameOverlaySpec` 去包含`inset spec`，而不是`relativeSpec`。将原来的`nameOverlaySpec` 声明修改为:
```
ASOverlayLayoutSpec *nameOverlaySpec = [ASOverlayLayoutSpec overlayLayoutSpecWithChild:gradientOverlaySpec overlay:nameInsetSpec];
```
然后修改 return 语句：
```
return nameOverlaySpec;
```
运行 app，正是你希望看到的：
### 十一： 下半部分
下半部分非常简单，将动物的介绍用一个`inset` 包围住… 你懂的。
在`return` 语句之前用动物介绍创建一个`inset spec`。
```
ASInsetLayoutSpec *descriptionTextInsetSpec = [ASInsetLayoutSpec 
insetLayoutSpecWithInsets:UIEdgeInsetsMake(16.0, 28.0, 12.0, 28.0)  child:self.animalDescriptionTextNode];
```
当你返回这个 spec 并运行 app，你会看到：
这正是你想要的。现在你已经弄好上下两部分了，将它们合在一起是小菜一碟。
### 十二： 固有内容尺寸
你也许注意到了，你不需要担心文字的内容的尺寸如何填充这个空间。因为`ASTextNode` 有一个固有内容尺寸 (`intrinsic content size`)，它会根据 text 属性和 attributes 属性自动计算。
下列节点没有默认尺寸：
- ASDisplayNode 子类
- ASNetworkImageNode 和 ASMultiplexImageNode
- ASVideoNode 和 ASVideoPlayerNode
通常这些节点一开始都没有内容，因此无法定义它们自己的大小。这些节点要么需要设置`preferredFrameSize`，要么在它们拥有具体 size 之前放到一个`layout spec` 中。
### 十三： ASStackLayoutSpec介绍
该说说`stack layout spec` 了。你可以把它看成是一种和`UIStackView` 类似的`layout spec`，但它能够自动向后兼容，这确实太爽了。 Stacks 可以定义要么横向要么纵向，和其它`layout spec` 一样，它可以把其它节点或`layout spec` 作为子对象。
为了使用`stack spec`，在`description inset` 一句后加入：
```
ASStackLayoutSpec *verticalStackSpec = [[ASStackLayoutSpec alloc] init];
verticalStackSpec.direction = ASStackLayoutDirectionVertical;
verticalStackSpec.children = @[nameOverlaySpec, descriptionTextInsetSpec];
```
这里创建了一个 stack，方向设置为 vertical，然后将上下两部分添加进去。
同样，返回新的`layou spec`:
```
return verticalStackSpec;
```
运行程序，马上就大功告成了
> 
注意：前面说过，stack 是核心 layout spec 中比较简单的一个。大部分布局都能用一种 stack 或一系列嵌套的 stack 来进行表达。
将 stack 进行嵌套，可以让 stack 变得变化无穷、极其麻烦。要深入了解，请参考[flex box froggy game](http://nguyenhuy.github.io/froggy-asdk-layout/) 以及 [Async Display Kit 文档](http://asyncdisplaykit.org/docs/automatic-layout-containers.html)。
### 十四： ASBackgroundLayoutSpec 介绍
还记得你的老朋友 `overlay spec`吗？它有一个原则是：在一个`overlay spec` 中，被覆盖的对象必须有一个 size。
在后面的对象定义一个 size，而前面的对象将被拉伸并占据它的大小。
`background spec` 则截然相反。如果你有一个对象能够确定自己的大小，你想在它下面拉伸出另一个对象，你可以使用`background spec`。
例如，你可以用`background layout spec` 将模糊的动物图片拉伸后放到整个 stack 的后面。
要这样做，需要增加这行：
```
ASBackgroundLayoutSpec *backgroundLayoutSpec = [ASBackgroundLayoutSpec backgroundLayoutSpecWithChild:verticalStackSpec background:self.backgroundImageNode];
```
然后将 return 语句修改为：
```
return backgroundLayoutSpec;
```
运行 app，查看最后的效果：
![](https://upload-images.jianshu.io/upload_images/2092929-4c8bdaaf2d906a84.png)
image.png
### 十五： 结束
在[这里](https://koenig-media.raywenderlich.com/uploads/2016/10/ASDKTutorial2Completed-2.zip)下载完成后的项目。另外，[这里](https://koenig-media.raywenderlich.com/uploads/2016/10/RainForestSwift-1.zip)可以下载 Swift 项目。
当你熟悉了这些概念之后，有一个学习更多内容的好地方就是[这里](http://asyncdisplaykit.org/docs/getting-started.html)。这真的是一道关于布局系统能做什么的美味大餐。
希望你喜欢本教程，有任何关于布局的问题，请在下面留言。

