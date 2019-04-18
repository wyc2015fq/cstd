# Steam VR Unity VRTK Examples 5 使用已抓住的物体和贝塞尔指针 - weixin_33985507的博客 - CSDN博客
2017年02月22日 22:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
**注意：VRTK官方已经在准备3.0版本，这里讲的是2.2.1这个版本。签出代码的时候，注意checkout到2.2.1这个tag下面。**
008_Controller_UsingAGrabbedObject 使用已抓住的物体
这个场景演示了如何使用已经抓住的物体，比如我们捡起了一把手枪拿在手里（按下Grip键），然后点击Trigger键进行发射子弹。该场景中还有一个玩具盒子，拿起之后执行use操作，上面的胶囊体可以旋转。
![2772495-b47d612826f2665f.jpg](https://upload-images.jianshu.io/upload_images/2772495-b47d612826f2665f.jpg)
UsingAGrabbedObjectScene
![2772495-157f20c964b75921.jpg](https://upload-images.jianshu.io/upload_images/2772495-157f20c964b75921.jpg)
usingrotate
![2772495-fdd7583cc3d39239.jpg](https://upload-images.jianshu.io/upload_images/2772495-fdd7583cc3d39239.jpg)
usinggun
其实关于如何使用物体，[前面的文章](https://www.jianshu.com/p/2cbd6a040de7)中已经介绍过了，这里再简单介绍一下吧。
要自定义使用物体的行为，需要从VRTK_InteractableObject类继承，重写基类的Start、Update、StartUsing、StopUsing等函数。我们先来看Gun类的实现。
![2772495-0c5be7abdf294def.jpg](https://upload-images.jianshu.io/upload_images/2772495-0c5be7abdf294def.jpg)
GunClass
最重要的就是StartUsing函数中调用FireBullet函数。该函数实例化了一个子弹，然后对子弹的刚体施加了一定的力，发射出去，经过了一段时间之后销毁该对象。
再看看Whirlygig类怎么实现的：
![2772495-73fa065597fe3fef.jpg](https://upload-images.jianshu.io/upload_images/2772495-73fa065597fe3fef.jpg)
WhirlygigClass
该类中在StartUsing的时候，设置了一个旋转速度，并在StopUsing的时候设置成0，这实现了按住Trigger的时候胶囊体是一直旋转的，松开的时候就停止了。该类重写了Update函数，保证每一帧都会根据旋转速度进行一定量的旋转。So easy！
009_Controller_BezierPointer 贝塞尔指针
该场景主要演示了如何在场景中使用贝塞尔曲线的指针，传送器还是[前面文章中](https://www.jianshu.com/p/85684957c00c)提到过的可适应高度的传送器。由贝塞尔曲线生成的激光束并不是直线，而是贝塞尔曲线，该功能允许用户在看不到物体顶部的情况下爬到上面。效果示意图如下：
![2772495-0331d790f786c8bf.jpg](https://upload-images.jianshu.io/upload_images/2772495-0331d790f786c8bf.jpg)
BezierPointerScene
VRTK_BezierPointer类
该场景中最大的变化就是用BezierPointer替换了SimplePointer。BezierPointer从控制器顶端发出一条曲线指向地面，这在地面高度不一致的时候进行传送比较有用，因为这样可以直接把用户传送到看不到的物体顶部。通过按住控制器上面的触摸板可以激活光束，可以通过VRTK_ControllerEvents脚本中的参数更改默认按键。BezierPointer脚本可以添加到[CameraRig]下面的控制器对象上，控制器对象需要VRTK_ControllerEvents脚本，因为它需要监听控制器的按键事件来开启或者关闭光束。也可以把BezierPointer脚本添加到其他对象上，让其他对象发射光束。
下面看一下BezierPointer类的主要参数。
![2772495-6226709021a3047f.png](https://upload-images.jianshu.io/upload_images/2772495-6226709021a3047f.png)
BezierPointer
**Pointer Length**: 发射的指针光束的长度。这基本上就是从控制器位置能够指到的距离。
**Pointer Density**: 在贝塞尔曲线中渲染线段的数量。数量过高的话，会影响性能。
**Show Pointer Cursor**: 光束指向地面上的时候会显示一个游标，能看到光束终点的位置非常有用，可以使用这个参数开关。
**Pointer Cursor Radius**: 地面上指针的大小半径。这个数字也影响贝塞尔曲线中物体的大小，半径越大，物体越大。
**Pointer Cursor Match Target Rotation**: 如果是true的话，游标将会旋转来匹配目标表面的角度；如果是false的话，游标总是水平的。
**Beam Curve Offset**: 光束曲线高度偏移量。
**Beam Height Limit Angle**: 在曲线高度光束高度受限制之前，控制器的最大角度。小一些的角度设置会使光束弯曲下来，并防止光束投射到天空上。
**Custom Pointer Tracer**: 自定义指针路径对象，默认是个圆球。自定义的对象将会匹配控制器的旋转。
**Custom Pointer Cursor**: 自定义的指针游标，默认是个平面的圆。
**Layers To Ignore**: 指定要忽略投射的层。
**Valid Teleport Location Object**: 在允许传送的情况下，要显示的自定义对象。
**Rescale Pointer Tracer**: 根据贝塞尔曲线的长度，重新缩放指针路径对象。
参考：
[](https://link.jianshu.com?t=https://vrtoolkit.readme.io/)[http://www.jianshu.com/p/25350f24d7f4](https://www.jianshu.com/p/25350f24d7f4)
