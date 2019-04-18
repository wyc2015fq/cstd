# UGUI中的Canvas、Camera和Canvas Scaler - 简书 - fighting！！！ - CSDN博客
2017年08月14日 17:46:22[dujiahei](https://me.csdn.net/dujiahei)阅读数：215
个人分类：[UGUI，unity](https://blog.csdn.net/dujiahei/article/category/7100179)
# UGUI中的Canvas、Camera和Canvas Scaler - 简书 http://www.jianshu.com/p/96fd1fbe8409
# 投影空间和屏幕空间
先不说UI，如果是一个普通渲染流程，一个Cube普通地被渲染。
![800X600的屏幕中，右边的Cube和中间的Image大小相仿。（可能由于网页显示问题导致本图显示尺寸变小。请自行脑补成其真实800x600大小，起码比下图248X186大好多）](http://upload-images.jianshu.io/upload_images/55056-9c3216bc38db33a0.png?imageView2/2/w/1240/q/100)
800X600的屏幕中，右边的Cube和中间的Image大小相仿。（可能由于网页显示问题导致本图显示尺寸变小。请自行脑补成其真实800x600大小，起码比下图248X186大好多）
![248X186的屏幕中，发现正方体已变小，但Image却没变](http://upload-images.jianshu.io/upload_images/55056-9bda15b996219428.png?imageView2/2/w/1240/q/100)
248X186的屏幕中，发现正方体已变小，但Image却没变
如果屏幕在现实中变小了，这个Cube在现实中也会变小。
具体地说，屏幕变小时，Cube在投影空间中是恒定大小的。但透视空间转换到屏幕空间时，一般都是使用默认的GPU流程：直接把投影空间投影面的四个角直接映射到屏幕空间的四个角。因为屏幕变小了，屏幕空间就变小了，所以这个Cube也就变小了。
> 
总之，**屏幕变小不影响投影空间，但会让屏幕空间变小。**
# Canvas
注，文章以下部分如不特殊注明，都认为Canvas.RenderMode是ScreenSpace，而并非WorldSpace。
# Canvas Scaler
Canvas Scaler的各种模式最终都是设置了`Canvas.scaleFactor`（还有`Canvas.referencePixelsPerUnit`，但并非本文重点）。
`Canvas.scaleFactor`会影响Canvas的RectTransform的ScaleXYZ。
# Canvas.RenderMode：ScreenSpace - Overlay
当是ScreenOverlay的情况，因为根本没有经过投影空间，而是直接在屏幕空间绘制，所以可以更加简单地讨论问题。
因为在屏幕空间的坐标直接绘制像素，所以Canvas的RectTransform的width*scaleX必须等于屏幕的宽、RectTransform的height*scaleY必须等于屏幕的高。
其中，scaleXY事实上是可以通过`Canvas.scaleFactor`给脚本设置的，也就是说：
> 
**Canvas的核心功能是，自动根据屏幕设备的分辨率、当前的scaleXY，算出正确的width和height，确保和屏幕匹配。**
![Canvas的核心功能是自动根据屏幕设备的分辨率、当前的scaleXY，算出正确的width和height，确保和屏幕匹配。](http://upload-images.jianshu.io/upload_images/55056-3381c3e354910485.png?imageView2/2/w/1240/q/100)
Canvas的核心功能是自动根据屏幕设备的分辨率、当前的scaleXY，算出正确的width和height，确保和屏幕匹配。
注，Overlay情况下，Scene View是世界空间的、Canvas是屏幕空间的，两者处于不同空间，它们的大小比较也就没有可比性了。但也总得把Canvas在SceneView显示出来对吧，所以Unity这里的Editor实现是，屏幕空间1像素等于世界空间1米地把Canvas在SceneView显示出来。
## CanvasScaler.ScaleMode: ConstantPixelSize
![](http://upload-images.jianshu.io/upload_images/55056-87a60861ec64c646.png?imageView2/2/w/1240/q/100)
以图中Scale Factor=1为例，ConstantPixelSize的作用是直接设置scaleXYZ为1。所以，Canvas的width和height总是等于屏幕大小(244x183)。
所以当设备分辨率变小时，scaleXYZ总是1，UI元素的大小并不改变，但Canvas的width和height变小了，所以UI元素占比Canvas的比例也随之变大。结果：屏幕分辨率变小，UI元素在设备的实际像素数不变，也就大小不变。
## CanvasScaler.ScaleMode: ScaleWithScreenSize
### ScreenMatchMode.MatchWidthOrHeight
![](http://upload-images.jianshu.io/upload_images/55056-c7bf3c3a5de56214.png?imageView2/2/w/1240/q/100)
以图中Canvas的Reference Resolution等于800X600比例为4：3、屏幕恰好是同比例4：3的244X183为例，
scaleXYZ总是等于屏幕分辨率/参考分辨率=0.305，所以Canvas的width和height总是800X600。
所以当屏幕分辨率变小于800X600时，Canvas的width/height不变。Canvas的scaleXYZ变小，Canvas本身和UI元素的全局scaleXYZ都变小，但UI元素占比Canvas的比例不变。结果：屏幕分辨率变小，UI元素在设备的比例不变，所以UI的实际像素数变小。
事实上，CanvasScaler是ScaleWithScreenSize的作用就是：
> 
**根据参考分辨率和当前设备分辨率，得出一个“合适”的scaleFactor。至于设备分辨率和参考分辨率比例不同，这个问题交由Canvas自动设置width/height来解决。**
当设备分辨率比参考分辨率大时，scaleFactor需要“合适”的大，当设备分辨率比参考分辨率小时，scaleFactor需要“合适”地小。
这个“合适”，在内部实现的代码是：
```
float logWidth = Mathf.Log(screenSize.x / m_ReferenceResolution.x, kLogBase);
float logHeight = Mathf.Log(screenSize.y / m_ReferenceResolution.y, kLogBase);
float logWeightedAverage = Mathf.Lerp(logWidth, logHeight, m_MatchWidthOrHeight);
scaleFactor = Mathf.Pow(kLogBase, logWeightedAverage);
```
### ScreenMatchMode.Expand
`scaleFactor = Mathf.Min(screenSize.x / m_ReferenceResolution.x, screenSize.y / m_ReferenceResolution.y);`
scaleFactor尽可能小，可知，结果是尽量把UI在设备屏幕给放大
### ScreenMatchMode.Shrink
`scaleFactor = Mathf.Max(screenSize.x / m_ReferenceResolution.x, screenSize.y / m_ReferenceResolution.y);`
scaleFactor尽可能大，可知，结果是尽量把UI在设备屏幕给缩小
## CanvasScaler.ScaleMode: ConstantPhysicalSize
根据屏幕的PPI信息和ConstantPhysicalSize本身的配置信息，得出一个“合适”的scaleFactor，以达到UI在不同PPI设备上的最终大小都是一致的。
# Canvas.RenderMode：ScreenSpace - Camera
由于使用了Camera，所以此时的Canvas和Scene View里的任意GameObject一样，都是处于世界空间了。
**所以对于ScreenSpace - Camera的Canvas来说，关键是如何把处于世界空间中的Canvas能够占满投影空间的投影平面。**
然而，由于在世界坐标系中任意GameObject的默认scaleXYZ都是1，Canvas也不例外。所以，需要根据Camera的设置、Canvas的plane distance设置，算出scaleFactorCamera出来。
## 正交（Orthographic）Camera
正交Camera的可调参数是Size。Size的定义为视体的投影平面高的1/2。 比如Camera的Size是300，即投影平面高为600。投影平面的宽根据平面的高和屏幕比例得出。
所以Canvas必须和这个投影平面一样大小（且位置重合）。此时，有两种方法和投影平面大小一样：
- 调整Canvas的width/height
- 调整Canvas的ScaleXY
由于第1种方法会导致切换Overlay和Camera的方式时，UI的表现不一致，所以Unity的实现是采取了第2种方法。即：
> 
当Canvas是Screen Space - Overlay时，
`Canvas.RectTransform.scaleXYZ = Canvas.scaleFactor`
当Canvas是Screen Space - Camera时，
`Canvas.RectTransform.scaleXYZ = Canvas.scaleFactor * scaleFactorCamera`
所以，在这种情况下，
- Canvas.height==Screen.height
- Canvas.height * scaleFactor * scaleFactorCamera = Camera.size * 2，
- 所以scaleFactorCamera = Camera.size * 2 / Canvas.height / scaleFactor = Camera.size * 2 / Screen.height / scaleFactor
## 透视（Perspective）Camera
透视Camera的可调参数为fov。同时Canvas的plane distance也认为是参数之一。
- Canvas.height == Screen.height
- Canvas.height * scaleFactor * scaleFactorCamera = 2 * Canvas.planeDistance * tan(Camera.fov/2)
- 所以scaleFactorCamera = 2 * Canvas.planeDistance * tan(Camera.fov/2) / Screen.height / scaleFactor
## CanvasScaler
至此，已求出不同Camera模式的scaleFactorCamera。
> 
**在ScreenSpace - Camera的情况下，不管CanvasScaler的模式再怎么变，都是在上面Overlay的情况下的CanvasScaler的讨论基础上，再乘多一个scaleFactorCamera即可。**
# WorldSpace
Canvas的width和height、scaleXYZ都可以任意设置。Canvas不再自动根据屏幕设备的分辨率、当前的scaleXY，算出正确的width和height。
此时的CanvasScaler在此模式下几乎形同虚设。只是拿来设置Canvas.referencePixelsPerUnit
作者：DonaldW
链接：http://www.jianshu.com/p/96fd1fbe8409
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
