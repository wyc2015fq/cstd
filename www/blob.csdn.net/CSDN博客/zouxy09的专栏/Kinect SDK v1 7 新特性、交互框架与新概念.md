
# Kinect SDK v1.7 新特性、交互框架与新概念 - zouxy09的专栏 - CSDN博客


2013年03月20日 17:50:02[zouxy09](https://me.csdn.net/zouxy09)阅读数：9977


**Kinect SDK v1.7 新特性、交互框架与新概念**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
2013年3月18日，微软Kinect for Windows团队发布了新版的Kinect for Windows SDK 1.7，这是自去年发布Kinect forWindows SDK以来最重要的一次更新。其更新的核心内容包括：
**1、Kinect Interactions**：
手势识别（可以识别pressing、gripping和releasing等手势，从而实现拖拽、滚屏、点击和放大等等功能）、多人智能适应方式、双人交互等。之前OpenNI中都有手势识别的功能，但以前版本的Kinect for Windows SDK却没有手势识别相关的功能，对手势体验方面的处理得开发者自己去开发，这是非常耗时的。微软意识到这个趋势，顺势而为还是比较赞的。况且还可以借此机会规定下标准啊啥的。
**2、Kinect Fusion**：
实时捕捉和构建物体的3D模型。这点听起来还是比较喜人的。在Kinect开放至今，很多的研究和开发都是针对于3D重构的，的确这也是3D摄像头的一个诱人之处。3D场景重构可利用于虚拟现实、3D打印、工业设计、服饰购物体验等。有些产品都已经表现出了其独特的魅力。但这个如果需要实时的话，对GPU的要求那是高啊。Kinect Fusion官方是在NVidia GeForce GTX560和 AMD Radeon 6950中测试的，更高端的情况是NVidia
 GeForce GTX680和AMD Radeon HD 7850（对于显卡，咱们看型号的第2位，第2位数字越大，越高端）。对于台式机，最好大于3GHz的多核CPU和具有2GB以上独立显存的显卡。
**3、与Matlab和OpenCV嫁接：**
在计算机视觉领域的人相信都知道这两大图像处理巨头。在这个功能没有出现之前，很多人都是自己对Kinect的图像数据进行转换到OpenCV的图像格式进行处理的。现在微软也应这个潮流，自己把这个桥梁给大家搭好了。当然，这给Kinect的交互发展感觉还是有很大作用的，毕竟在Matlab和OpenCV领域的开发群体不是一般的大啊。
**一、KinectInteractionArchitecture交互框架**
KinectInteraction通过结合深度流、骨骼流和复杂的算法来提供人手跟踪和手势识别，以及一些其他特性。其架构图如下：
![](https://img-my.csdn.net/uploads/201303/20/1363773231_3056.jpg)

Kinect v1.7 给用户提供了两种编程API，分别是用C++实现的native code APIs ，和用C\# 与 Visual Basic编写的、被更高层封装的managed code APIs。
**Native API**提供用户识别、人手跟踪、人手状态（被跟踪、交互等等）和瞄准按下等基础特性。这个API还提供了一个全新的数据流，称为interaction stream（交互流）（在所有的版本中，Kinect都是以流的形式来访问数据的，包括旧版本的深度流、颜色数据流和骨骼流等等），它反映的是手势识别事件的触发。
**Managed API**通过C\#实现，它除了提供和nativeAPI相同的上述功能外，还通过C\#/WPF controls提供了更高层的处理和交互控制封装，它可以使用户更快的构建交互程序。这些控制包括交互区域、抓取-拖拽和针对用户按下的按钮控制等。
**二、KinectInteractionConcepts交互新概念（术语）**
KinectInteraction是v1.7版新加入的特性，其中有很多新的概念，所以我们有必要去深入了解它们，清楚这些新的特性能做什么，不能做什么。另外，Kinect交互控制可以与键盘和鼠标控制兼容。
**2.1、Hand Tracking**
第一个概念是手势跟踪，如果我们之前用过Kinect的SDK的都会熟悉骨骼跟踪，它可以为在Kinect视觉范围的人建立骨骼。但是，在KinectInteraction中，这个功能本身没有加强，而是通过结合深度信息和骨骼跟踪信息去对用户的手势进行跟踪。除了跟踪人手，KinectInteraction还可以检测和报告人手和手臂的状态，使得一些例如握紧gripping、放开releasing和压下pressing的自然手势可以被识别。所以，用户可以在传感器的正常操作范围内（近距离模式0.4米，标准模式会达到3-4米）对Kinect程序进行非接触式交互。Kinect可以检测到在它前面的用户的手，并且进行跟踪。当然，如果用户是面对Kinect的，那么跟踪效果会更好，还有就是用户刚开始是用手掌来启动控制的。
**2.2、The Physical Interaction Zone (PhIZ)**
物理交互区域指处于真实物理世界中用户前面的3D空间与 程序员在程序中构建的3D坐标空间 之间的映射。对于每一个当前被跟踪的用户（可以同时跟踪两个）的手势都存在一个物理交互区域PhIZ。在大部分的交互中，PHIZ都是需要的。
**2.3、What Gets Tracked?**
交互层可以跟踪两个手，但它可以分别属于一个或者两个用户的手。第一个与系统发生交互的用户将被指定为主用户primary user。主用户保持对系统的交互控制权，直到系统检测到用户不再使用该应用程序。虽然主用户的两个手都可以被跟踪，但只有一个被指定为primary hand（用户用来控制experience的那个手）。只有primary hand才可以控制experience（什么意思？）。当然，如果两个手都被跟踪，那么用户可以通过将当前的primary
 hand放下，并把另一个手抬起，放进PhIZ中来切换primary hand。
**2.4、Hand State**
对每一个手，都会维护一个手的状态，它指明该手属于哪个用户，对于该用户，这个手是否是primary hand，这个手是否处于交互状态，还有就是这个手是在gripping、pressing还是其他手势状态。
**2.5、Tracked vs. Interactive**
一个跟踪的手trackedhand表示KinectInteraction正在盯着的，并且观察这个手可能会发生的交互。一个交互的手interactive hand是处于PhIZ中，并被KinectInteraction观测直接用于交互的。
**2.6、The User Viewer**
在Kinect的交互控制集中，存在一个用户视图User Viewer。它是从Kinect视角观测到的用户的一个小窗口。这个视图来源于深度数据流，它会在一个Kinect Region被创建的时候自动产生。
**2.7、The Hand Pointer**
Hand pointer是用来标记处于PhIZ中的用户的手势的当前位置的。它可以通过Kinect Cursor光标来可视化。如果程序开启了cursor可视功能（在C\#/WPF controls中是默认开启的），那么在Kinect Region(s)中，cursor表现人手形状。另外，当用户当前进行拖拽控制时，cursor就表现为一个握着的拳头。
**2.8、The Hand Pointer and Other Controls**
为了提供一个流畅的交互体验，一旦hand pointer在一个给定的Kinect Interaction Control中停留一定的时间，那么就认为该handpointer被该control捕捉，直到用户将手移出该control的边界。被一个给定的control捕捉到的属性由该hand pointer和捕捉到该hand pointer的control共同表示。
**2.9、Interaction Types**
KinectInteraction提供了以下几种交互方式：
**1）Grip and Release**
当用户从一个张开的手掌，到握紧变为拳头的时候，就触发一个grip交互，并且绑定手势跟踪到当前的被grip触发的control上，直到用户releases，也就是张开拳头。这个可以用来实现拖拽或者滚屏的控制功能。通过运动速度映射的改变，还可以在长列表中实现快速滚动。Control Basics例程中展示了其用法和效果。
**2）Press**
当用户用手掌伸向（压向）Kinect的方向（理想情况是手掌面向Kinect，而且手臂并没有完全在Kinect的方向伸展开），就触发一个press交互指令。它可以用以表示鼠标的单击功能。还可以用来配合滚屏查看器操作。Interaction Gallery例程中展示了其用法和效果。
**3）Scroll**
实际就是通过上面的哪些控制指令进行Scroll。
**2.10、The Interaction Stream**
交互系统提供了交互帧流，和其他的数据源（音频流、深度流、骨骼流等等）的流模型相似。通过处理交互帧给用户与应用程序的交互提供信息，例如人手位置、人手是pressing、gripping还是releasing，还有就是当前用户正在瞄准哪个control。
**三、一些新的例程**
**1）Controls Basics - WPF:**
Demonstrates the new KinectInteraction Controls, including hands-free button pressing and scrolling through large lists. This replaces the Basic Interactions sample from previous releases.
**2）Interaction Gallery - WPF:**
Demonstrates basic interactions using the new KinectInteraction Controls.
**3）KinectBridge with MATLAB Basics - D2D:**
Demonstrates how to do image processing with the Kinect sensor using MATLAB API.
**4）KinectBridge with OpenCV Basics - D2D:**
Demonstrates how to do image processing with the Kinect sensor using OpenCV API.
**5）Kinect Explorer - D2D:**
Demonstrates how to use the Kinect's ColorImageStream, DepthImageStream, SkeletonStream, and AudioSource with C++ and Direct2D. This replaces the SkeletalViewer C++ sample.
**6）Kinect Fusion Basics - WPF, Kinect Fusion Basics - D2D:**
Demonstrates basic use of the Kinect Fusion APIs for 3D reconstruction.
**7）Kinect Fusion Explorer - WPF, Kinect Fusion Explorer - D2D:**
Demonstrates advanced 3D reconstruction features of Kinect Fusion, allowing adjustment of many reconstruction parameters, and export of reconstructed meshes.
水平有限，以上理解难免有错，请大家不吝指正。谢谢

