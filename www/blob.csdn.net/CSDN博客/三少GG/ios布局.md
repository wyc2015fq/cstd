# ios布局 - 三少GG - CSDN博客
2016年02月19日 11:13:43[三少GG](https://me.csdn.net/scut1135)阅读数：673
#########################
经典入门教程： http://www.cocoachina.com/industry/20131203/7462.html
http://www.raywenderlich.com/50319/beginning-auto-layout-tutorial-in-ios-7-part-2
############################
### 1.经验法则：每个维度至少有两个约束
在每个维度(水平与竖直)上，一个视图的位置和大小由三个值来定义：头部空间(leading space)，大小和尾部空间(trailing space)[注释1]。一个视图的leading和trailing空间可以相对于父视图来定义，也可以相对于视图层级架构中的兄弟视图来定义。一般来说，我们的布局约束必须满足这两个值，以便计算第三个值(size)。其结果是，一个标准的视图在每个维度上必须至少有两个约束，以明确视图的布局。
### 2.拥抱固有大小(Intrinsic Size)
一些控件，如标签和按钮，都有一个所谓的固有大小(Intrinsic Size)。视控件的不同，固有大小可以在水平或竖直或同时两个方向上有效。当一个控件没有明确的宽度和高度约束时，就会使用它的固有大小作为约束。这允许我们在每个方向上只使用一个显式约束就可以创建明确的布局(相对于上面第1条规则)，并让控件可以根据自身的内容来自动调整大小。这是在本地化中创建一个最佳布局的关键。
### 5.不要显式地调整控件的大小
尝试不要显式地设置一个控件的大小。只要我们不手动去改变它们的大小，大部分控件都会根据它们的内容来调整自己的大小，并使用固有大小(intrinsic size)来创建一个完美的、内容敏感的布局。这对于需要做本地化的UI尤其重要。一旦我们(无意或有意地)手动调整了控件的大小，IB将创建一个很难摆脱的显式大小约束。为了回归到固有大小，可以使用Editor > Size to Fit Content命令。
### 7.忘记Frame吧
忘记frame属性吧。不要直接设置它。一个视图的frame在自动布局过程中会被自动设置，而不是一个输入结果。我们可以通过改变约束来改变frame。这将强迫我们改变看待UI的方式。不用再去考虑位置和大小了，而是考虑每个视图相对于它的兄弟视图和父视图的位置。这与CSS没有什么不同。
### 10.让约束动起来，而不是frame
在Auto Layout中，我们需要重新考虑动画。我们不再可以简单的动画一个视图的frame了；如果我们这样做了，视图将在动画完成后自动恢复到Auto Layout计算出来的位置和大小上。相反，我们需要直接动画布局的约束。要做到这一点，或者修改已存在的约束（我们可以为IB中创建的约束声明IBOutlet变量），也可以添加一个新的约束，然后在一个动画block中给我们的视图发送layoutIfNeeded消息。
以上参考：[https://autolayout.club/2015/09/01/%E7%BF%BB%E8%AF%91%E7%AF%87%EF%BC%9A10-Things-You-Need-to-Know-About-Cocoa-Auto-Layout/](https://autolayout.club/2015/09/01/%E7%BF%BB%E8%AF%91%E7%AF%87%EF%BC%9A10-Things-You-Need-to-Know-About-Cocoa-Auto-Layout/)
###############################
# Autolayout使用小结（一）http://blog.cnbluebox.com/blog/2014/12/13/autolayout1/
# 如何做好IOS View的布局 [http://blog.cnbluebox.com/blog/2015/09/18/howtolayoutview/](http://blog.cnbluebox.com/blog/2015/09/18/howtolayoutview/)
### 1.如何布局UIViewController的view
首先给出设计原则：
- - 屏幕尺寸变化时能自适应，如不同尺寸设备，屏幕旋转，热点，电话等。
- - 无论是否有navigationBar或tabBar都能够正常显示，即要考虑是否有这些bar的所有场景
- - 尽量避免hard code间距，如20，44，49等
系列初级教程： [https://lvwenhan.com/sort/ios/page/2](https://lvwenhan.com/sort/ios/page/2)
