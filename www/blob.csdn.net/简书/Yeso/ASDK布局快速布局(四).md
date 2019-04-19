# ASDK布局快速布局(四) - 简书
# ASDK布局快速布局(四)
#### 快速开始
###### 1.1: Motivation & Benefits
对于复杂的视图层次结构来说，UIKit的自动布局会变得极其昂贵。`Layout API`是作为`UIKit Auto Layout`的高性能替代品创建的。Texture的布局API比使用UIKit的自动布局有许多好处：
- 1: 快速：与手动布局代码一样快，比自动布局快得多
- 2: 异步和并发：布局可以在后台线程上计算，因此用户交互流畅和及时响应。
- 3: 声明性：布局是用不可变的数据结构声明的。这使得布局代码更容易开发，文档，代码审查，测试，调试，配置文件和维护。
- 4: 可缓存：布局结果是不可变的数据结构，因此它们可以在后台预先计算并缓存以提高用户感知的性能。
- 5: 可扩展：易于在类之间共享代码。
###### 1.2: 受CSS Flexbox的启发
熟悉Flexbox的人会注意到这两个系统有许多相似之处。但是，Texture的Layout API不会重新实现所有的CSS。
###### 1.3： 基本概念
**`Texture`的布局系统围绕着两个基本概念：**
- 布局规格
- 布局元素
###### 1.4： 布局规格
布局规范（简称“布局规范”）没有实体存在。相反，通过了解这些子布局元素如何相互关联，布局规范充当其他布局元素的容器。
Texture提供了ASLayoutSpec的几个子类，从一个插入单个子元素的简单布局规范到更复杂的布局规范，这些布局规范将多个子元素排列在不同的堆栈配置中。
###### 1.5: 布局元素
布局规格包含并排列布局元素。
所有ASDisplayNodes和ASLayoutSpecs都符合<ASLayoutElement>协议。这意味着您可以从节点和其他布局规格构建布局规格。cool！
ASLayoutElement协议有几个属性可以用来创建非常复杂的布局。此外，布局规范还有其自己的一组属性，可用于调整布局元素的排列。
###### 1.6:  结合布局规格和布局元素来制作复杂的用户界面
在这里，您可以看到如何组合ASTextNodes（以黄色突出显示），ASVideoNode（顶部图像）和ASStackLayoutSpec（“堆栈布局规格”）来创建复杂布局。
![](https://upload-images.jianshu.io/upload_images/2092929-c6694e0c0fe0a565..png)
layoutSpec
使用 ASCenterLayoutSpec（“中央布局规范”）和ASOverlayLayoutSpec（“覆盖布局规范”）放置ASVideoNode顶部的播放按钮（顶部图像）。
![](https://upload-images.jianshu.io/upload_images/2092929-3043b29cb046bd39..png)
image
###### 1.7: 有些节点需要尺寸集
一些元素基于其立即可用的内容具有“固有尺寸”。例如，ASTextNode可以根据其属性字符串计算其大小。其他具有固有尺寸的节点包括
- ASImageNode
- ASTextNode
- ASButtonNode
所有其他节点在加载外部资源之前，不具有固有大小或缺少固有大小。例如，ASNetworkImageNode在从URL下载图像之前不知道其大小。 这些元素包括
- ASVideoNode
- ASVideoPlayerNode
- ASNetworkImageNode
- ASEditableTextNode
缺少初始固有尺寸的这些节点必须使用ASRatioLayoutSpec，ASAbsoluteLayoutSpec或样式对象上的尺寸属性为它们设置初始尺寸。
###### 1.8: 布局调试
在任何ASDisplayNode或ASLayoutSpec上调用-asciiArtString都会返回对象及其子对象的ascii-art表示形式。 或者，如果您在任何节点或布局规范上设置.debugName，那么它也将包含在ascii art中。 下面是一个例子。
```
-----------------------ASStackLayoutSpec----------------------
|  -----ASStackLayoutSpec-----  -----ASStackLayoutSpec-----  |
|  |       ASImageNode       |  |       ASImageNode       |  |
|  |       ASImageNode       |  |       ASImageNode       |  |
|  ---------------------------  ---------------------------  |
--------------------------------------------------------------
```
您还可以在任何ASLayoutElement（节点或布局规范）上打印样式对象。这在调试大小属性时特别有用。
```
(lldb) po _photoImageNode.style
Layout Size = min {414pt, 414pt} <= preferred {20%, 50%} <= max {414pt, 414pt}
```
#### 二: Layout Examples
查看布局规范[示例项目](https://github.com/texturegroup/texture/tree/master/examples/LayoutSpecExamples)以使用下面的代码。
**带左右对齐文本的简单标题**
![](https://upload-images.jianshu.io/upload_images/2092929-413735c72f2a9c57..png)
image
要创建这个布局，我们将使用：
- 垂直ASStackLayoutSpec
- 和水平ASStackLayoutSpec
- ASInsetLayoutSpec插入整个标题
下图显示了布局元素的组成（节点+布局规格）。
![](https://upload-images.jianshu.io/upload_images/2092929-f7a6c23856ecbfd7..png)
image
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  // when the username / location text is too long, 
  // shrink the stack to fit onscreen rather than push content to the right, offscreen
  ASStackLayoutSpec *nameLocationStack = [ASStackLayoutSpec verticalStackLayoutSpec];
  nameLocationStack.style.flexShrink = 1.0;
  nameLocationStack.style.flexGrow = 1.0;
  
  // if fetching post location data from server, 
  // check if it is available yet and include it if so
  if (_postLocationNode.attributedText) {
    nameLocationStack.children = @[_usernameNode, _postLocationNode];
  } else {
    nameLocationStack.children = @[_usernameNode];
  }
  
  // horizontal stack
  ASStackLayoutSpec *headerStackSpec = [ASStackLayoutSpec stackLayoutSpecWithDirection:ASStackLayoutDirectionHorizontal
                                                                               spacing:40
                                                                        justifyContent:ASStackLayoutJustifyContentStart
                                                                            alignItems:ASStackLayoutAlignItemsCenter
                                                                              children:@[nameLocationStack, _postTimeNode]];
  
  // inset the horizontal stack
  return [ASInsetLayoutSpec insetLayoutSpecWithInsets:UIEdgeInsetsMake(0, 10, 0, 10) child:headerStackSpec];
}
```
###### 2.1：带有插入文本覆盖的照片
![](https://upload-images.jianshu.io/upload_images/2092929-afbadde16c77dc70..png)
image
要创建这个布局，我们将使用：
- ASInsetLayoutSpec插入文本
- ASOverlayLayoutSpec覆盖照片顶部的插入文本规范
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  _photoNode.style.preferredSize = CGSizeMake(USER_IMAGE_HEIGHT*2, USER_IMAGE_HEIGHT*2);
  // INIFINITY is used to make the inset unbounded
  UIEdgeInsets insets = UIEdgeInsetsMake(INFINITY, 12, 12, 12);
  ASInsetLayoutSpec *textInsetSpec = [ASInsetLayoutSpec insetLayoutSpecWithInsets:insets child:_titleNode];
  
  return [ASOverlayLayoutSpec overlayLayoutSpecWithChild:_photoNode overlay:textInsetSpec];
}
```
###### 2.2：带有起始图标叠加层的照片
![](https://upload-images.jianshu.io/upload_images/2092929-a635ecfe24c005bc..png)
image
要创建这个布局，我们将使用：
ASAbsoluteLayoutSpec放置照片和图标，这些照片和图标已经使用其ASLayoutable属性单独调整大小和放置
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  _iconNode.style.preferredSize = CGSizeMake(40, 40);
  _iconNode.style.layoutPosition = CGPointMake(150, 0);
  
  _photoNode.style.preferredSize = CGSizeMake(150, 150);
  _photoNode.style.layoutPosition = CGPointMake(40 / 2.0, 40 / 2.0);
  
  return [ASAbsoluteLayoutSpec absoluteLayoutSpecWithSizing:ASAbsoluteLayoutSpecSizingSizeToFit
                                                   children:@[_photoNode, _iconNode]];
}
```
###### 2.3: 简单的插入文本单元格
![](https://upload-images.jianshu.io/upload_images/2092929-02f8f5a12accba8b..png)
image
要重新制作Pinterest在上述搜索视图中使用的单个单元格的布局，我们将使用a：
- ASInsetLayoutSpec插入文本
- ASCenterLayoutSpec根据指定的属性将文本居中
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
    UIEdgeInsets insets = UIEdgeInsetsMake(0, 12, 4, 4);
    ASInsetLayoutSpec *inset = [ASInsetLayoutSpec insetLayoutSpecWithInsets:insets
                                                                      child:_titleNode];
    return [ASCenterLayoutSpec centerLayoutSpecWithCenteringOptions:ASCenterLayoutSpecCenteringY
                                                      sizingOptions:ASCenterLayoutSpecSizingOptionMinimumX
                                                              child:inset];
}
```
###### 2.4: 顶部和底部分隔线
![](https://upload-images.jianshu.io/upload_images/2092929-d0cffe6b4bee034e..png)
image
要创建上面的布局，我们将使用a：
- 一个ASInsetLayoutSpec来插入文本
- 垂直ASStackLayoutSpec将文本顶部和底部的两个分隔线叠加起来
下图显示了可布置的组成（布局规格+节点）。
![](https://upload-images.jianshu.io/upload_images/2092929-c404f542b33da224..png)
image
以下代码也可以在ASLayoutSpecPlayground[示例项目](http://texturegroup.org/docs/automatic-layout-examples-2.html)中找到。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  _topSeparator.style.flexGrow = 1.0;
  _bottomSeparator.style.flexGrow = 1.0;
  ASInsetLayoutSpec *insetContentSpec = [ASInsetLayoutSpec insetLayoutSpecWithInsets:UIEdgeInsetsMake(20, 20, 20, 20) child:_textNode];
  return [ASStackLayoutSpec stackLayoutSpecWithDirection:ASStackLayoutDirectionVertical
                                                 spacing:0
                                          justifyContent:ASStackLayoutJustifyContentCenter
                                              alignItems:ASStackLayoutAlignItemsStretch
                                                children:@[_topSeparator, insetContentSpec, _bottomSeparator]];
}
```

