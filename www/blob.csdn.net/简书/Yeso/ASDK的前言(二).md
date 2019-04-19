# ASDK的前言(二) - 简书
# ASDK的前言(二)
texture的学习开启
> 
一: Texture的开始
二: Resources
三: Installation
#### 一: Texture的开始
Texture的基本单位是 `node`。 `ASDisplayNode`是`UIView`和`CALayer`的一个抽象。 不像`UIView`的操作只能在主线程，`node`是线程安全的：您可以在后台线程上实例对象和配置对象的整个层次结构。
为了保持交互界面的流畅和响应速度，应用应该以每秒60帧的速度渲染 - 这是iOSUI的黄金标准。 这意味着主线程有六十分之一秒时间渲染每一帧。 也就是只有16毫秒来处理布局和绘图代码！ 由于系统开销，防止帧丢失，代码通常渲染时间不能超过10毫秒。
Texture将 **图像解码**, **文本大小调整**和 **渲染**以及 **其他昂贵的UI操作**移出主线程，用来保证主线程可以及时响应用户交互。 `Texture`也有很多高级技巧，待我娓娓道来~
###### 1.1: Nodes(节点)
如果你熟悉使用视图，你已经知道如何使用`node`。 大多数`UIView`和`CALayer`属性和方法`Node`都有对应的替代。 当然也有一些差异（例如`.clipsToBounds` vs `.masksToBounds`），`nodes`有`UIView`几乎全部的功能。 唯一的例外是`node`使用`position`而不是`center`。
当然，你总是可以通过`node.view`或`node.layer`直接访问底层视图或图层，当一定确保在主线程！
Texture提供了[多种节点](http://texturegroup.org/docs/node-overview.html)来替换您经常使用的大部分UIKit组件。大规模的应用程序已经能够使用Texture完全编写他们的界面。
###### 1.2: Node Containers(节点容器)
**当我们使用Texture时，常见的错误是将`node`直接添加到现有视图层次结构中。这样做可能导致您的`node`在渲染时会闪烁。**
取而代之，您应该将`node`添加到[node container classes](http://texturegroup.org/docs/containers-overview.html)中的某一种，作为`node`容器的子节点。 这些容器负责告诉包含`node`它们当前处于什么状态，以便可以加载数据并尽可能高效地渲染节点。 您应该将这些类视为`UIKit`和`Texture`之间的整合点。
###### 1.3: Layout Engine(布局引擎)
Texture的布局引擎既是其最强大的功能之一，也是其最独特的功能之一。 基于`CSS FlexBox`模型，它提供了一种声明方式来指定自定义`node`的大小和布局子节点。 尽管默认情况下所有节点都同时呈现，但通过为每个节点提供`ASLayoutSpec`来执行异步测量和布局。
###### 1.4: Advanced Developer Features(高级特性)
`Texture`提供了`UIKit`或`Foundation`没有提供的各种高级开发功能特性。我们的开发人员发现Texture允许简化架构并提高开发人员的速度。
###### 1.5: Adding Texture to your App
如果您刚接触`Texure`，我们建议您查看提供的`ASDKgram`示例应用程序。我们已经创建了一个方便的指南，并提供了一步一步的指导你如何融合`Texure`到我们的项目中
如果您遇到任何问题，请向我们的GitHub或[Texture Slack](http://texturegroup.org/docs/resources.html#slack)社区寻求帮助。
#### 二: Resources
###### 2.1: Slack
加入700+纹理开发人员和Slack上的Texture核心团队，以进行实时调试，最新更新和异步嘲讽。在此注册。
###### 2.2: Examples
浏览我们的许多[示例项目](https://github.com/texturegroup/texture/tree/master/examples)。
如果您对Texture不熟悉，我们建议您从[ASDKgram](https://github.com/texturegroup/texture/tree/master/examples/ASDKgram)示例应用程序开始，该应用程序将使用UIKit实现的照片Feed与用Texture实现的相同Feed进行比较。该应用程序的特点
- 一个无限滚动的家庭饲料，演示Texture的平滑滚动性能。
- 一个尺寸很大的代码库，用于演示使用Texture设计应用程序需要多少代码。
###### 2.3: Videos
- [Building smooth and responsive UI with Texture](https://www.youtube.com/watch?v=XA_NkUbu9WA) [CocoaHeadsNL 2017]
- [AsyncDisplayKit 2.0: Defining the 7th Abstraction Layer](https://www.youtube.com/watch?v=64Wgt_Uf50o) [Pinterest HQ 2016]
- [Layout at Scale with AsyncDisplayKit 2.0](https://www.youtube.com/watch?v=sqkinHYXTuc) [NSMeetup 2016]
- [ASCollectionNode](https://youtu.be/yuDqvE5n_1g) [Pinterest HQ 2016]
- [AsyncDisplayKit State of the Code](https://www.youtube.com/watch?v=8ngXakpE2x8) [WWDC 2016]
- [AsyncDisplayKit 2.0: Intelligent User Interfaces](https://www.youtube.com/watch?v=RY_X7l1g79Q) [NSSpain 2015]
- [Effortless Responsiveness with AsyncDisplayKit](https://www.youtube.com/watch?v=ZPL4Nse76oY) [MCE 2015]
- [Asynchronous UI](https://www.youtube.com/watch?v=h4QDbgB7RLo) [NSLondon 2014]
###### 2.4: Tutorials / Articles
[Using AsyncDisplayKit to Develop Responsive UIs in iOS](http://www.appcoda.com/introduction-asyncdisplaykit-2-0/) [Ziad Tamim, 12.29.2016]
[AsyncDisplayKit 2.0 Tutorial: Automatic Layout](https://www.raywenderlich.com/124696/asyncdisplaykit-2-0-tutorial-automatic-layout) [Luke Parham, 12.19.2016]
[AsyncDisplayKit 2.0 Tutorial: Getting Started](https://www.raywenderlich.com/124311/asyncdisplaykit-2-0-tutorial-getting-started) [Luke Parham, 12.5.2016]
[iOS Smooth Scrolling in Buffer for iOS: How (and Why) We Implemented AsyncDisplayKit](https://overflow.buffer.com/2016/10/04/implementing-asyncdisplaykit-within-buffer-ios/) [Andy Yates, 11.4.2016]
###### 2.5: Layout Resources
Texture的强大布局系统基于CSS FlexBox模型。这些网站对于学习这个系统的基础知识很有用。
- [ASStackLayout Game](http://nguyenhuy.github.io/froggy-asdk-layout/)
- [Visual Guide to CSS3 Flexbox](https://demos.scotch.io/visual-guide-to-css3-flexbox-flexbox-playground/demos/)
- [FlexBox Patterns](http://www.flexboxpatterns.com/home)
#### 三: Installation
Texture可以通过CocoaPods或Carthage(本文不提及)添加到您的项目中。不要忘记导入框架标题：
```
#import <AsyncDisplayKit/AsyncDisplayKit.h>
```
或者创建一个Objective-C桥接头（Swift）。如果您在安装Texture时遇到任何问题，请通过Github或Slack与我们联系！
**CocoaPods**
CocoaPods上有texture。将以下内容添加到您的Podfile中：
```
target 'MyApp' do
    pod "Texture"
end
```
运行之前完全退出Xcode
```
> pod install
```
在终端的项目目录中。
要更新您的texture版本，请运行
```
> pod update Texture
```
在终端的项目目录中。
不要忘记使用工作区.xcworkspace文件，而不是项目.xcodeproj文件。
