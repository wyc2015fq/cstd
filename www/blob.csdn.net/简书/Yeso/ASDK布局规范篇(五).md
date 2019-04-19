# ASDK布局规范篇(五) - 简书
# ASDK布局规范篇(五)
#### 一: Layout Spec
以下ASLayoutSpec子类可用于组合简单或非常复杂的布局。
- ASWrapperLayoutSpec
- ASStackLayoutSpec
- ASInsetLayoutSpec
- ASOverlayLayoutSpec
- ASBackgroundLayoutSpec
- ASCenterLayoutSpec
- ASRatioLayoutSpec
- ASRelativeLayoutSpec
- ASAbsoluteLayoutSpec
您也可以继承ASLayoutSpec，以制作自己的自定义布局规范。
###### 1.1: ASWrapperLayoutSpec
ASWrapperLayoutSpec是一个简单的ASLayoutSpec子类，它可以包装ASLayoutElement并根据布局元素上设置的大小计算子级布局。
ASWrapperLayoutSpec是从-layoutSpecThatFits：中轻松返回单个子节点的理想选择。 或者，此子节点可以在其上设置大小信息。 但是，如果您需要设置大小以外的位置，请改为使用ASAbsoluteLayoutSpec。
```
// return a single subnode from layoutSpecThatFits: 
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  return [ASWrapperLayoutSpec wrapperWithLayoutElement:_subnode];
}
// set a size (but not position)
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  _subnode.style.preferredSize = CGSizeMake(constrainedSize.max.width,
                                            constrainedSize.max.height / 2.0);
  return [ASWrapperLayoutSpec wrapperWithLayoutElement:subnode];
}
```
###### 1.2: ASStackLayoutSpec（Flexbox容器）
在Texture中的所有layoutSpec中，ASStackLayoutSpec是最有用和最强大的。ASStackLayoutSpec使用弹性盒算法来确定其子项的位置和大小。Flexbox旨在为不同屏幕尺寸提供一致的布局。在堆叠布局中，您可以将物品以垂直或水平堆叠方式对齐。堆栈布局可以是另一个堆栈布局的子布局，这使得使用堆栈布局规范几乎可以创建任何布局。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize{
    self.childNodeA.style.preferredSize = CGSizeMake(100, 100);
    self.childNodeB.style.preferredSize = CGSizeMake(200, 200);
    ASStackLayoutSpec *stackLayout = [ASStackLayoutSpec stackLayoutSpecWithDirection:ASStackLayoutDirectionVertical
                                                                             spacing:12
                                                                      justifyContent:ASStackLayoutJustifyContentStart
                                                                          alignItems:ASStackLayoutAlignItemsStart
                                                                            children:@[self.childNodeA, self.childNodeB]];
    return stackLayout;
}
```
除了<ASLayoutElement>属性之外，ASStackLayoutSpec还有7个属性：
**简单的说明下各个参数的作用：**
- 1: direction：主轴的方向，有两个可选值:
- 纵向：`ASStackLayoutDirectionVertical`
- 横向：`ASStackLayoutDirectionHorizontal`
- 2:spacing:主轴上视图排列的间距，比如有四个视图，那么它们之间的存在三个间距值都应该是spacing
- 3: justifyContent: 主轴上的排列方式，有五个可选值：
- `ASStackLayoutJustifyContentStart` 从前往后排列
- `ASStackLayoutJustifyContentCenter` 居中排列
- `ASStackLayoutJustifyContentEnd` 从后往前排列
- `ASStackLayoutJustifyContentSpaceBetween` 间隔排列，两端无间隔
- `ASStackLayoutJustifyContentSpaceAround` 间隔排列，两端有间隔
- 4: alignItems: 交叉轴上的排列方式，有五个可选值：
- `ASStackLayoutAlignItemsStart` 从前往后排列
- `ASStackLayoutAlignItemsEnd` 从后往前排列
- `ASStackLayoutAlignItemsCenter` 居中排列
- `ASStackLayoutAlignItemsStretch` 拉伸排列
- `ASStackLayoutAlignItemsBaselineFirst` 以第一个文字元素基线排列（主轴是横向才可用）
- `ASStackLayoutAlignItemsBaselineLast`以最后一个文字元素基线排列（主轴是横向才可用）
- 5: children: 包含的视图。数组内元素顺序同样代表着布局时排列的顺序，所以需要注意
**主轴的方向设置尤为重要**，如果主轴设置的是 `ASStackLayoutDirectionVertical`, 那么 **justifyContent** 各个参数的意义就是：
- `ASStackLayoutJustifyContentStart` 从上往下排
- `ASStackLayoutJustifyContentCenter` 居中排列
- `ASStackLayoutJustifyContentEnd` 从下往上排列
- `ASStackLayoutJustifyContentSpaceBetween` 间隔排列，两端无间隔
- `ASStackLayoutJustifyContentSpaceAround` 间隔排列，两端有间隔
**alignItems** 就是：
- `ASStackLayoutAlignItemsStart` 从左往右排列
- `ASStackLayoutAlignItemsEnd` 从右往左排列
- `ASStackLayoutAlignItemsCenter` 居中排列
- `ASStackLayoutAlignItemsStretch` 拉伸排列
- `ASStackLayoutAlignItemsBaselineFirst` 无效
- `ASStackLayoutAlignItemsBaselineLast` 无效
对于子视图间距不一样的布局方法，后面实战中会讲到。
###### 1.3： ASInsetLayoutSpec
在布局过程中，ASInsetLayoutSpec将其constrainedSize.max CGSize传递给其子节点，然后减去其insets。一旦孩子确定了它的最终尺寸，插图规格将其最终尺寸作为孩子的尺寸加上其插入的边距。 由于嵌套布局规范根据其子级的大小来确定大小，因此子级必须具有固有大小或明确设置其大小。
[图片上传失败...(image-4ee435-1521620385941)]
如果在UIEdgeInsets中将INFINITY设置为一个值，则插入规范将仅使用子内在尺寸。看到这个例子。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ...
  UIEdgeInsets *insets = UIEdgeInsetsMake(10, 10, 10, 10);
  ASInsetLayoutSpec *headerWithInset = [ASInsetLayoutSpec insetLayoutSpecWithInsets:insets child:textNode];
  ...
}
```
###### 1.4: ASOverlayLayoutSpec
ASOverlayLayoutSpec布置其子项（蓝色），将其上的另一个组件作为叠加层（红色）展开。
![](https://upload-images.jianshu.io/upload_images/2092929-16af8c632e53a00c..png)
image
重叠规格的大小是根据child的大小计算的。 在下图中，child是蓝色的层。然后将child的大小作为constrainedSize传递给叠加布局元素（红色图层）。因此，重要的是child（蓝色图层）必须具有设置的固有尺寸或尺寸。
```
在ASOverlayLayoutSpec中使用自动子节点管理时，节点有时可能会显示错误的顺序。 这是一个即将解决的已知问题。 目前的解决方法是手动添加节点，叠加布局元素（红色）必须作为子节点添加到子布局元素（蓝色）之后的父节点。
```
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ASDisplayNode *backgroundNode = ASDisplayNodeWithBackgroundColor([UIColor blueColor]);
  ASDisplayNode *foregroundNode = ASDisplayNodeWithBackgroundColor([UIColor redColor]);
  return [ASOverlayLayoutSpec overlayLayoutSpecWithChild:backgroundNode overlay:foregroundNode];
}
```
###### 1.5: ASBackgroundLayoutSpec
ASBackgroundLayoutSpec绘制一个组件（蓝色），将背后的另一个组件拉伸为背景（红色）。
![](https://upload-images.jianshu.io/upload_images/2092929-240cfa5f56678d9d..png)
image
背景规格的大小是根据孩子的大小计算出来的。 在下图中，孩子是蓝色的层。然后将孩子的大小作为constrainedSize传递给背景布局元素（红色图层）。因此，重要的是孩子（蓝色图层）必须具有设置的固有尺寸或尺寸。
```
在ASOverlayLayoutSpec中使用自动子节点管理时，节点有时可能会显示错误的顺序。 这是一个即将解决的已知问题。
目前的解决方法是手动添加节点，子布局元素（蓝色）必须作为子节点添加到子节点元素（红色）之后的父节点。
```
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ASDisplayNode *backgroundNode = ASDisplayNodeWithBackgroundColor([UIColor redColor]);
  ASDisplayNode *foregroundNode = ASDisplayNodeWithBackgroundColor([UIColor blueColor]);
  return [ASBackgroundLayoutSpec backgroundLayoutSpecWithChild:foregroundNode background:backgroundNode];
}
```
注意：添加子节点的顺序与此布局规范相关; 必须将背景对象作为子节点添加到前景对象之前的父节点。 使用ASM目前不保证此订单！
###### 1.6: ASCenterLayoutSpec
ASCenterLayoutSpec将其子中心置于其最大约束大小内。
![](https://upload-images.jianshu.io/upload_images/2092929-5fb98602fe8d4880..png)
image
如果中心规格的宽度或高度不受约束，则会缩小到小孩的大小。
ASCenterLayoutSpec有两个属性：
- centeringOptions。确定孩子在中心规格内的居中方式。选项包括：无，X??，Y，XY。
- sizingOptions。确定中心规格将占用多少空间。选项包括：默认，最小X，最小Y，最小XY。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ASStaticSizeDisplayNode *subnode = ASDisplayNodeWithBackgroundColor([UIColor greenColor], CGSizeMake(70, 100));
  return [ASCenterLayoutSpec centerLayoutSpecWithCenteringOptions:ASCenterLayoutSpecCenteringXY
                                                    sizingOptions:ASCenterLayoutSpecSizingOptionDefault
                                                            child:subnode]
}
```
###### 1.7： ASRatioLayoutSpec
ASRatioLayoutSpec以可以缩放的固定高宽比来布局组件,此规范必须有宽度或高度作为constrainedSize传递给它，因为它使用此值进行自我缩放。
![](https://upload-images.jianshu.io/upload_images/2092929-e99a0b10b5e71d50..png)
image
使用比率规格为ASNetworkImageNode或ASVideoNode提供内在大小是非常常见的，因为两者在内容从服务器返回之前都不具有固有大小。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  // Half Ratio
  ASStaticSizeDisplayNode *subnode = ASDisplayNodeWithBackgroundColor([UIColor greenColor], CGSizeMake(100, 100));
  return [ASRatioLayoutSpec ratioLayoutSpecWithRatio:0.5 child:subnode];
}
```
###### 1.8: ASRelativeLayoutSpec
根据垂直和水平位置说明符放置一个组件并将其放置在布局边界内。与“9部分”图像区域类似，child可以放置在4个角的任何一个角落，或4个边的任何一个的中间，以及中心。
把它称为顶点布局可能有点不恰当，实际上它可以把视图布局在：左上、左下、右上、右下四个顶点以外，还可以设置成居中布局。
[图片上传失败...(image-603d7d-1521622259187)]
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize{
    self.childNodeA.style.preferredSize = CGSizeMake(100, 100);
    ASRelativeLayoutSpec *relativeLayout = [ASRelativeLayoutSpec relativePositionLayoutSpecWithHorizontalPosition:ASRelativeLayoutSpecPositionEnd verticalPosition:ASRelativeLayoutSpecPositionStart sizingOption:ASRelativeLayoutSpecSizingOptionDefault child:self.childNodeA];
    return relativeLayout;
}
```
上面的例子就是把 childNodeA 显示在右上角。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ...
  ASDisplayNode *backgroundNode = ASDisplayNodeWithBackgroundColor([UIColor redColor]);
  ASStaticSizeDisplayNode *foregroundNode = ASDisplayNodeWithBackgroundColor([UIColor greenColor], CGSizeMake(70, 100));
  ASRelativeLayoutSpec *relativeSpec = [ASRelativeLayoutSpec relativePositionLayoutSpecWithHorizontalPosition:ASRelativeLayoutSpecPositionStart
                                  verticalPosition:ASRelativeLayoutSpecPositionStart
                                      sizingOption:ASRelativeLayoutSpecSizingOptionDefault
                                             child:foregroundNode]
  ASBackgroundLayoutSpec *backgroundSpec = [ASBackgroundLayoutSpec backgroundLayoutSpecWithChild:relativeSpec background:backgroundNode];
  ...
}
```
###### 1.9: ASAbsoluteLayoutSpec
在ASAbsoluteLayoutSpec中，您可以通过设置它们的layoutPosition属性来指定其子项的确切位置（x / y坐标）。绝对布局比其他类型的布局更不灵活和难以维护。
ASAbsoluteLayoutSpec有一个属性：
- sizing。确定绝对规格将占用多少空间。选项包括：默认和适合的大小。请注意，Size to Fit选项将复制旧ASStaticLayoutSpec的行为。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  CGSize maxConstrainedSize = constrainedSize.max;
  // Layout all nodes absolute in a static layout spec
  guitarVideoNode.style.layoutPosition = CGPointMake(0, 0);
  guitarVideoNode.style.size = ASSizeMakeFromCGSize(CGSizeMake(maxConstrainedSize.width, maxConstrainedSize.height / 3.0));
  nicCageVideoNode.style.layoutPosition = CGPointMake(maxConstrainedSize.width / 2.0, maxConstrainedSize.height / 3.0);
  nicCageVideoNode.style.size = ASSizeMakeFromCGSize(CGSizeMake(maxConstrainedSize.width / 2.0, maxConstrainedSize.height / 3.0));
  simonVideoNode.style.layoutPosition = CGPointMake(0.0, maxConstrainedSize.height - (maxConstrainedSize.height / 3.0));
  simonVideoNode.style.size = ASSizeMakeFromCGSize(CGSizeMake(maxConstrainedSize.width/2, maxConstrainedSize.height / 3.0));
  hlsVideoNode.style.layoutPosition = CGPointMake(0.0, maxConstrainedSize.height / 3.0);
  hlsVideoNode.style.size = ASSizeMakeFromCGSize(CGSizeMake(maxConstrainedSize.width / 2.0, maxConstrainedSize.height / 3.0));
  return [ASAbsoluteLayoutSpec absoluteLayoutSpecWithChildren:@[guitarVideoNode, nicCageVideoNode, simonVideoNode, hlsVideoNode]];
}
```
###### 1.10: ASLayoutSpec
ASLayoutSpec是所有布局规范的子类的主要类。它的主要工作是处理所有的儿童管理，但它也可以用来创建自定义布局规范。 但是，只有超级高级应该需要/需要创建ASLayoutSpec的自定义子类。请尝试使用提供的布局规范并将它们组合在一起以创建更高级的布局。
当应用.flexGrow和/或.flexShrink时，ASLayoutSpec的另一个用途是作为ASStackLayoutSpec与其他孩子的间隔符使用。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  ...
  // ASLayoutSpec as spacer
  ASLayoutSpec *spacer = [[ASLayoutSpec alloc] init];
  spacer.style.flexGrow = true;
  stack.children = @[imageNode, spacer, textNode];
  ...
}
```
