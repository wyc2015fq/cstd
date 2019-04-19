# ASDK的Layout API(六) - 简书
# ASDK的Layout API(六)
#### 一: Layout Element Properties(布局元素属性)
- `ASStackLayoutElement`属性 - 仅对作为堆栈规范的子节点或布局规范生效
- `ASAbsoluteLayoutElement`属性 - 仅对绝对规格的子节点或布局规范生效
- `ASLayoutElement`属性 - 适用于所有节点和布局规格
###### 1.1: ASStackLayoutElement属性
> 
**Please note that the following properties will only take effect if set on the child of an[STACK](http://texturegroup.org/docs/layout2-layoutspec-types.html#asstacklayoutspec-flexbox-container) layout spec.**
|Property|Description|
|----|----|
|`CGFloat .style.spacingBefore`|在堆叠方向上放置此对象之前需要额外的空间。|
|`CGFloat .style.spacingAfter`|在堆叠方向上放置此对象之后需要额外的空间。|
|`CGFloat .style.flexGrow`|如果子布局堆叠尺寸的总和小于最小尺寸，该物体是否应该增长？|
|`CGFloat .style.flexShrink`|如果子布局的堆栈尺寸总和大于最大尺寸，这个对象是否应该缩小？|
|`ASDimension .style.flexBasis`|在应用`flexGrow` /`flexShrink`属性并分配剩余空间之前，在堆栈维度（水平或垂直）中指定此对象的初始大小。|
|`ASStackLayoutAlignSelf .style.alignSelf`|对象沿着横轴取向，覆盖alignItems。选项包括：- (1)`ASStackLayoutAlignSelfAuto`(2)`ASStackLayoutAlignSelfStart`、(3)`ASStackLayoutAlignSelfEnd`、(4)`ASStackLayoutAlignSelfCenter`、(5)`ASStackLayoutAlignSelfStretch`|
|`CGFloat .style.ascender`|用于基线对齐。从物体顶部到其基线的距离。|
|`CGFloat .style.descender`|用于基线对齐。物体底部到底部的距离。|
###### 1.2： ASAbsoluteLayoutElement属性
> 
**Please note that the following properties will only take effect if set on the child of an[ABSOLUTE](http://texturegroup.org/docs/layout2-layoutspec-types.html#asabsolutelayoutspec) layout spec.**
|Property|Description|
|----|----|
|`CGPoint .style.layoutPosition`|该对象的`CGPoint`在其ASAbsoluteLayoutSpec`父规范中的位置|
###### 1.3: ASLayoutElement属性
> 
请注意，以下属性适用于所有布局元素。
|Property|Description|
|----|----|
|[ASDimension](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension).style.width|`width`属性指定了`ASLayoutElement`的内容区域的宽度。 `minWidth`和`maxWidth`属性覆盖`width`。默认为`ASDimensionAuto`。|
|[ASDimension](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension).style.height|`height`属性指定了`ASLayoutElement`的内容区域的高度。 `minHeight`和`maxHeight`属性覆盖`height`。默认为`ASDimensionAuto`。|
|[ASDimension](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension).style.minWidth|`minWidth`属性用于设置给定元素的最小宽度。 它可以防止`width`属性的使用值变得小于`minWidth`指定的值。 `minWidth`的值覆盖`maxWidth`和`width`。 默认为`ASDimensionAuto`。|
|[ASDimension](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension).style.maxWidth|`maxWidth`属性用于设置给定元素的最大宽度。 它可以防止`width`属性的使用值变得大于为`maxWidth`指定的值。 “maxWidth”的值覆盖了“width”，但“minWidth”覆盖了“maxWidth”。 默认为`ASDimensionAuto`。|
|[ASDimension](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension).style.minHeight|`minHeight`属性用于设置给定元素的最小高度。 它可以防止`height`属性的使用值变得小于`minHeight`指定的值。 `minHeight`的值覆盖`maxHeight`和`height`。 默认为`ASDimensionAuto`。|
|[`ASDimension .style.maxHeight`](http://texturegroup.org/docs/layout2-api-sizing.html#values-cgfloat-asdimension)|'maxHeight`属性用于设置给定元素的最大高度。 它可以防止`height`属性的使用值变得大于为`maxHeight`指定的值。`maxHeight`的值覆盖`height`，但是`minHeight`覆盖`maxHeight`。 默认为`ASDimensionAuto`|
|`CGSize .style.preferredSize`|为布局元素提供建议大小。 如果提供了可选的minSize或maxSize，并且preferredSize超过了这些值，则minSize或maxSize将被强制执行。 如果未提供此可选值，则布局元素的大小将默认为提供的内部大小`calculateSizeThatFits`,         此方法是可选的，但preferredSize或preferredLayoutSize中的一个对于没有内在内容大小的节点或者应以与内在大小不同的大小布置的节点是必需的。 例如，可以在ASImageNode上将此属性设置为以与底层图像大小不同的大小显示。警告：当尺寸的宽度或高度相对时调用getter将导致断言。|
|`CGSize .style.minSize`|为布局元素提供最小大小限制的可选属性。 如果提供，则此限制将始终强制执行。 如果父级布局元素的最小大小小于其子级的最小大小，则会强制执行子级的最小大小，并且其大小将扩展到布局规格之外。例如，如果您在全屏容器中的某个元素上设置50％的首选相对宽度和200点的最小宽度，则这会导致iPhone屏幕上的宽度为160点。 但是，由于160分低于200分的最小宽度，因此将使用最小宽度。|
|`CGSize .style.maxSize`|为布局元素提供最大大小限制的可选属性。 如果提供，则此限制将始终强制执行。 如果子布局元素的最大尺寸小于其父元素，则会强制执行子元素的最大尺寸，并且其尺寸将超出布局规范。 例如，如果您在全屏容器中将元素的首选相对宽度设置为50％，最大宽度设置为120点，则这会导致iPhone屏幕上的宽度为160点。 但是，由于160分高于120分的最大宽度，因此将使用最大宽度。|
|[ASLayoutSize](http://texturegroup.org/docs/layout2-api-sizing.html#sizes-cgsize-aslayoutsize).style.preferredLayoutSize|为布局元素提供建议的`RELATIVE`大小。 `ASLayoutSize`使用百分比而不是点来指定布局。 例如。 宽度应该是父宽度的50％。 如果提供了可选的`minLayoutSize`或`maxLayoutSize`，并且preferredLayoutSize超过了这些值，则`minLayoutSize`或`maxLayoutSize`将被强制执行。 如果未提供此可选值，则布局元素的大小将默认为其提供的“内部内容大小” `calculateSizeThatFits`：|
|**[ASLayoutSize](http://texturegroup.org/docs/layout2-api-sizing.html#sizes-cgsize-aslayoutsize).style.minLayoutSize**|一个可选属性，为布局元素提供了一个最小的`RELATIVE`大小。 如果提供，则此限制将始终强制执行。 如果父级布局元素的最小相对大小小于其子级的最小相对大小，则会强制执行子级的最小相对大小，并且其大小将扩展到布局规范之外。|
|**[ASLayoutSize](http://texturegroup.org/docs/layout2-api-sizing.html#sizes-cgsize-aslayoutsize).style.maxLayoutSize**|一个可选属性，为布局元素提供了最大的RELATIVE大小。 如果提供，则此限制将始终强制执行。 如果父级布局元素的最大相对大小小于其子级的最大相对大小，则会强制执行子级的最大相对大小，并且其大小将扩展到布局规范之外。|
#### 二: Layout API Sizing
理解Layout API中复合维度类型的最简单方法是查看所有单位的相互关系。
![](https://upload-images.jianshu.io/upload_images/2092929-bf748c97996d46b7..png)
image
###### 2.1: Values (CGFloat, ASDimension)
ASDimension基本上是一个正常的CGFloat，支持表示点值，相对百分比值或自动值。
该单元允许相同的API采用固定值以及相关值。
```
OC
// dimension returned is relative (%)
ASDimensionMake(@"50%");  
ASDimensionMakeWithFraction(0.5);
// dimension returned in points
ASDimensionMake(@"70pt");
ASDimensionMake(70);      
ASDimensionMakeWithPoints(70);
```
```
swift
// dimension returned is relative (%)
ASDimensionMake("50%")
ASDimensionMakeWithFraction(0.5)
// dimension returned in points
ASDimensionMake("70pt")
ASDimensionMake(70)
ASDimensionMakeWithPoints(70)
```
###### 2.2: 使用ASDimension的示例
`ASDimension`用于在`ASStackLayoutSpec`的子级上设置`flexBasis`属性。 flexBasis属性指定对象在堆栈维度中的初始大小，其中堆栈维度是它是水平还是垂直堆栈。
在下面的视图中，我们希望左堆占据水平宽度的40％，右堆占据宽度的60％。
![](https://upload-images.jianshu.io/upload_images/2092929-1efc37df7118c240..png)
image
我们通过在水平堆栈的两个childen上设置.flexBasis属性来做到这一点：
```
self.leftStack.style.flexBasis = ASDimensionMake("40%")
self.rightStack.style.flexBasis = ASDimensionMake("60%")
horizontalStack.children = [self.leftStack, self.rightStack]
```
###### 2.3: Sizes (CGSize, ASLayoutSize)
ASLayoutSize类似于CGSize，但其宽度和高度值可以表示点或百分比值。宽度和高度的类型是独立的;任何一个都可能是一个点或百分比值。
```
OC：
ASLayoutSizeMake(_ width: ASDimension, _ height: ASDimension)
```
```
swift:
ASLayoutSizeMake(_ width: ASDimension, _ height: ASDimension)
```
`ASLayoutSize`用于设置布局元素的`.preferredLayoutSize`，`.minLayoutSize`和`.maxLayoutSize`属性。它允许相同的API采用固定大小以及相关的大小。
```
// Dimension type "Auto" indicates that the layout element may 
// be resolved in whatever way makes most sense given the circumstances
ASDimension width = ASDimensionMake(ASDimensionUnitAuto, 0);  
ASDimension height = ASDimensionMake(@"50%");
layoutElement.style.preferredLayoutSize = ASLayoutSizeMake(width, height);
```
如果您不需要相对值，则可以设置布局元素的`.preferredSize`，`.minSize`和`.maxSize`属性。这些属性采用常规CGSize值。
```
layoutElement.style.preferredSize = CGSizeMake(30, 160);
```
大多数时候，你不会想要限制宽度和高度。在这些情况下，您可以使用`ASDimension`值单独设置布局元素的大小属性。
```
layoutElement.style.width     = ASDimensionMake(@"50%");
layoutElement.style.minWidth  = ASDimensionMake(@"50%");
layoutElement.style.maxWidth  = ASDimensionMake(@"50%");
layoutElement.style.height    = ASDimensionMake(@"50%");
layoutElement.style.minHeight = ASDimensionMake(@"50%");
layoutElement.style.maxHeight = ASDimensionMake(@"50%");
```
###### 2.4: Size Range (ASSizeRange)
UIKit不提供捆绑最小和最大CGSize的结构。所以，创建ASSizeRange以支持最小和最大CGSize对。
`ASSizeRange`主要用于布局API的内部。但是，作为`layoutSpecThatFits`的输入传递的`constrainedSize`值是一个`ASSizeRange`。
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize;
```
传递给`ASDisplayNode`子类的`layoutSpecThatFits`方法的`constrainedSize`是节点应该适应的最小和最大尺寸。包含在`constrainedSize`中的最小和最大CGSizes可用于调整节点的布局元素的大小。
#### 三: Layout Transition API
布局转换API旨在使所有带纹理的动画变得简单 - 甚至将整个视图转换为完全不同的视图！
有了这个系统，你只需指定所需的布局，Texture将完成工作，从而找出与当前布局的差异。 它会自动添加新元素，删除transiton之后的不需要的元素，并更新任何现有元素的位置。
也有易于使用的API，可以让您完全自定义新引入的元素的起始位置，以及已移除元素的结束位置。
> 
Use of [Automatic Subnode Management](http://texturegroup.org/docs/automatic-subnode-mgmt.html) is required to use the Layout Transition API.
###### 3.1: 布局之间的动画
布局转换API可以很容易地在节点生成的布局之间进行动画，以响应节点中的某些内部状态更改。
想象一下，当您点击下一个按钮时，您想要实现此注册表单并在新字段中生成动画：
![](https://upload-images.jianshu.io/upload_images/2092929-527af7e3de1066d7.gif)
image
实现这一点的标准方法是创建一个名为SignupNode的容器节点，其中包含两个可编辑的文本字段节点和一个按钮节点作为子节点。 我们将在SignupNode上包含一个名为fieldState的属性，用于选择在节点计算其布局时显示哪个可编辑的文本字段节点。
SignupNode容器的内部布局规格如下所示：
```
- (ASLayoutSpec *)layoutSpecThatFits:(ASSizeRange)constrainedSize
{
  FieldNode *field;
  if (self.fieldState == SignupNodeName) {
    field = self.nameField;
  } else {
    field = self.ageField;
  }
  ASStackLayoutSpec *stack = [[ASStackLayoutSpec alloc] init];
  [stack setChildren:@[field, self.buttonNode]];
  UIEdgeInsets insets = UIEdgeInsetsMake(15.0, 15.0, 15.0, 15.0);
  return [ASInsetLayoutSpec insetLayoutSpecWithInsets:insets child:stack];
}
```
为了在本例中触发从nameField到ageField的转换，我们将更新SignupNode的.fieldState属性，并使用`transitionLayoutWithAnimation`开始转换。
此方法将使当前计算的布局无效，并重新计算现在在堆栈中的ageField的新布局。
```
self.signupNode.fieldState = SignupNodeAge;
[self.signupNode transitionLayoutWithAnimation:YES];
```
在此API的默认实现中，布局将重新计算新布局，并使用其子布局来在不使用动画的情况下确定SignupNode的子节点的大小和位置。 该API的未来版本可能会包含布局之间的默认动画，我们欢迎您提供有关您想要在此处看到的内容的反馈。 但是，我们需要实现一个自定义动画块来处理注册表单的情况。
下面的例子代表了`animateLayoutTransition`的覆盖：在`SignupNode`中。
在通过`transitionLayoutWithAnimation`计算新布局之后调用此方法：在实现中，我们将根据动画触发前设置的fieldState属性执行特定的动画。
```
- (void)animateLayoutTransition:(id<ASContextTransitioning>)context
{
  if (self.fieldState == SignupNodeName) {
    CGRect initialNameFrame = [context initialFrameForNode:self.ageField];
    initialNameFrame.origin.x += initialNameFrame.size.width;
    self.nameField.frame = initialNameFrame;
    self.nameField.alpha = 0.0;
    CGRect finalAgeFrame = [context finalFrameForNode:self.nameField];
    finalAgeFrame.origin.x -= finalAgeFrame.size.width;
    [UIView animateWithDuration:0.4 animations:^{
      self.nameField.frame = [context finalFrameForNode:self.nameField];
      self.nameField.alpha = 1.0;
      self.ageField.frame = finalAgeFrame;
      self.ageField.alpha = 0.0;
    } completion:^(BOOL finished) {
      [context completeTransition:finished];
    }];
  } else {
    CGRect initialAgeFrame = [context initialFrameForNode:self.nameField];
    initialAgeFrame.origin.x += initialAgeFrame.size.width;
    self.ageField.frame = initialAgeFrame;
    self.ageField.alpha = 0.0;
    CGRect finalNameFrame = [context finalFrameForNode:self.ageField];
    finalNameFrame.origin.x -= finalNameFrame.size.width;
    [UIView animateWithDuration:0.4 animations:^{
      self.ageField.frame = [context finalFrameForNode:self.ageField];
      self.ageField.alpha = 1.0;
      self.nameField.frame = finalNameFrame;
      self.nameField.alpha = 0.0;
    } completion:^(BOOL finished) {
      [context completeTransition:finished];
    }];
  }
}
```
此方法中传递的`ASContextTransitioning`上下文对象包含相关信息，可帮助您确定转换前后的节点状态。 它将getter包括到旧的和新的约束大小，插入和移除节点，甚至包括原始的旧的和新的ASLayout对象。 在`SignupNode`示例中，我们使用它来确定每个字段的框架并在不合适的位置为它们设置动画。
一旦动画完成，就必须调用`completeTransition`在上下文对象上，因为它将执行必要的内部步骤，使新计算的布局成为当前的`calculateLayout`。
请注意，在转换过程中没有使用`addSubnode`或`removeFromSupernode`。 Texture的布局转换API分析旧布局和新布局之间节点层次结构的差异，通过自动子节点[管理](http://texturegroup.org/docs/automatic-subnode-mgmt.html)隐式执行节点插入和删除。
在调用`animateLayoutTransition`之前插入节点，这是在开始动画之前手动管理层次结构的好地方。 在`didCompleteLayoutTransition`中执行删除：在上下文对象上调用`completeTransition`之后。 如果您需要手动执行删除操作，请覆盖`didCompleteLayoutTransition`并执行您的自定义操作。 请注意，这将覆盖默认行为，建议您调用super或遍历上下文对象中`removedSubnodes getter`来执行清理。
将NO传递给`transitionLayoutWithAnimation`仍将贯穿`animateLayoutTransition`和`didCompleteLayoutTransition`将`[context isAnimated]`属性设置为NO的实现。 这是您如何处理此案的选择 - 如果有的话。 提供默认实现的简单方法是调用`super`
```
- (void)animateLayoutTransition:(id<ASContextTransitioning>)context
{
  if ([context isAnimated]) {
    // perform animation
  } else {
    [super animateLayoutTransition:context];
  }
}
```
###### 3.2: 动画约束大小更改
有时您只需要响应对节点的边界更改并为重新计算其布局设置动画。 要处理这种情况，请在节点上调用`transitionLayoutWithSizeRange：animated`。
此方法类似于`transitionLayoutWithAnimation`，但如果传递的`ASSizeRange`等于当前的`constrainedSizeForCalculatedLayout`值，则不会触发动画。 这对于响应旋转事件和查看控制器大小更改非常有用：
```
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
  [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
  [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext>  _Nonnull context) {
    [self.node transitionLayoutWithSizeRange:ASSizeRangeMake(size, size) animated:YES];
  } completion:nil];
}
```
[使用布局转换API的示例](https://github.com/texturegroup/texture/tree/master/examples/ASDKLayoutTransition)
