# iOS动画——Layer Animations(二) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月07日 10:03:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：255
上一篇我们初步了解了一下**Layer Animations**，并且做了一个还算看的过去的动画，这一篇继续介绍一些**Layer Animations**的一些使用。
这是上一篇的地址： [iOS动画——Layer Animations(一)](http://www.jianshu.com/p/059c2d63dea3) （感觉不错的话请关注我哦～）
Core Animation相对于UIView Animation的优势在哪呢，其一就是你能够操作动画的过程，对动画的操作更加详细。
既然我们需要监控整个动画过程，很显然我们需要继承一个协议。然而，我们并不需要去写代码去继承一个协议，每一个**NSObject**都早已经继承了这么两个方法：
```
override func animationDidStop(anim: CAAnimation!, finished flag: Bool) {}
override func animationDidStart(anim: CAAnimation!) {}
```
当很多个**CABasicAnimation**都设置了**.delegate = self**的时候，我们如何再上述两个函数中区分呢？我们可以给** CABasicAnimation**设置value，像这样：
```bash
caBasic.setValue("Value", forKey: "key")
```
然后你就能这么识别：
```java
```java
if let name = anim.valueForKey("name") as? String {
    if name == "Vaule" {
        println("This is caBasic")
    }
}
```
```
那么有什么用呢，这个就要考想象力了～～
前面我们创建了一些**CABasicAnimation**，在这里我们再介绍一个**CAAnimation**类——**CAAnimationGroup**，这个类的作用就是将许多动画放在一起，统一管理。像下面这样：
```
let groupAnimation = CAAnimationGroup()
    groupAnimation.beginTime = CACurrentMediaTime() + 0.5
    groupAnimation.duration = 0.5
    groupAnimation.fillMode = kCAFillModeBackwards
    groupAnimation.timingFunction = CAMediaTimingFunction(
      name: kCAMediaTimingFunctionEaseIn)
    let scaleDown = CABasicAnimation(keyPath: "transform.scale")
    scaleDown.fromValue = 3.5
    scaleDown.toValue = 1.0
    let rotate = CABasicAnimation(keyPath: "transform.rotation")
    rotate.fromValue = CGFloat(M_PI_4)
    rotate.toValue = 0.0
    let fade = CABasicAnimation(keyPath: "opacity")
    fade.fromValue = 0.0
    fade.toValue = 1.0
    groupAnimation.animations = [scaleDown, rotate, fade]
    loginButton.layer.addAnimation(groupAnimation, forKey: nil)
```
> 
像上述那样，我们就给一个对象添加了一组动画，注意动画时间并不是一个结束后执行另一个！
**CAAnimationGroup**还有一个属性：**timingFunction**，他控制这一组动画的运行状态，他有一下几个值：
> 
**kCAMediaTimingFunctionEaseIn：**
![运行速度开始慢，然后加速.png](http://upload-images.jianshu.io/upload_images/255820-d05bd9657525b54f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
运行速度开始慢，然后加速.png
**kCAMediaTimingFunctionEaseOut：**
![1.png](http://upload-images.jianshu.io/upload_images/255820-ad26c2782234dfe8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
**kCAMediaTimingFunctionEaseInEaseOut：**
![1.png](http://upload-images.jianshu.io/upload_images/255820-42313caf90935333.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
**kCAMediaTimingFunctionLinear：**
默认的就是这个值，这个值就是速度保持一定。
这里还有几个**CABasicAnimation**的属性：
> - .repeatCount：重复次数
- .speed：动画运行速度（.speed ＝ 2，意为以原本两倍速度运行，时间减半，运行次数不变。）
- view.layer.speed = 2.0：如此操作，将给view上的所有动画加速一倍。
继续介绍一个新的类**CAKeyframeAnimation**，这个类和前面大不相同，前面的** CABasicAnimation**动画是从**.fromValue**到**.toValue**。而**CAKeyframeAnimation**类则是更加精确的控制动画，我们可以这样定义一个**CAKeyframeAnimation**类：
```cpp
```cpp
let wobble = CAKeyframeAnimation(keyPath: "transform.rotation") wobble.duration = 0.25
wobble.repeatCount = 4
wobble.values = [0.0, -M_PI_4/4, 0.0, M_PI_4/4, 0.0] 
wobble.keyTimes = [0.0, 0.25, 0.5, 0.75, 1.0] 
heading.layer.addAnimation(wobble, forKey: nil)
```
```
> 
我们看到了什么？在这个类当中将value分割成了很多部分，然后对每一个部分都有一个对应的完成时间，和前面的** CABasicAnimation**比较，我们可以将它理解为，将**froemValue**，***toValue**，**duration**分割成**n**个部分，那么这样做的好处就是对动画的控制更加精确。
我们来看这样一个动画：
![9.gif](http://upload-images.jianshu.io/upload_images/255820-6abeda68758a1305.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
9.gif
> - 当我们点击**Log In**按钮以后，从左上角出现了一个热气球，热气球从左上角先是右下方移动，移动到头以后继续向左下角移动，那么这个动画很明显分为三个点，那么先后到达这三个点就可以实现动画。
来看一下代码：
```cpp
```cpp
let balloon = CALayer()
    balloon.contents = UIImage(named: "balloon")!.CGImage
    balloon.frame = CGRect(x: -50.0, y: 0.0,width: 50.0, height: 65.0)
    view.layer.insertSublayer(balloon, below: username.layer)
    let flight = CAKeyframeAnimation(keyPath: "position")
    flight.duration = 12.0
    flight.values = [
      CGPoint(x: -50.0, y: 0.0),
      CGPoint(x: view.frame.width + 50.0, y: 160.0),
      CGPoint(x: -50.0, y: loginButton.center.y)
      ].map { NSValue(CGPoint: $0) }
     flight.keyTimes = [0.0, 0.5, 1.0]
    balloon.addAnimation(flight, forKey: nil)
```
```
> - 代码较为容易理解，我们将 flight.values分为了三段，并且给每一段都设定了完成时间，最后一步就是给视图添加动画。
这一篇的东西比较零散，不过我们还是有一个Demo的。
这是Github地址：[https://github.com/superxlx/iOS_Animation_Test4](https://github.com/superxlx/iOS_Animation_Test4)
如果觉得这篇文章不错的话请点赞，并关注本人。
