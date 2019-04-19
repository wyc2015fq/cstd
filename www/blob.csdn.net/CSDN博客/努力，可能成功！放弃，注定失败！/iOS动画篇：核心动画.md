# iOS动画篇：核心动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月02日 09:16:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：988
### 基本概念
###### 1、什么是核心动画
　　Core Animation(核心动画)是一组功能强大、效果华丽的动画API，无论在iOS系统或者在你开发的App中，都有大量应用。
　　核心动画所在的位置如下图所示：
![](http://upload-images.jianshu.io/upload_images/1644426-330ae214a7101048.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Core Animation.png
　　可以看到，核心动画位于UIKit的下一层，相比UIView动画，它可以实现更复杂的动画效果。
　　核心动画作用在CALayer（Core animation layer）上，CALayer从概念上类似UIView，我们可以将UIView看成是一种特殊的CALayer（可以响应事件）。
　　实际上，每一个view都有其对应的layer，这个layer是root layer：
`    @property(nonatomic,readonly,strong)                 CALayer  *layer;`
　　给view加上动画，本质上是对其layer进行操作，layer包含了各种支持动画的属性，动画则包含了属性变化的值、变化的速度、变化的时间等等，两者结合产生动画的过程。
　　核心动画和UIView动画的对比：UIView动画可以看成是对核心动画的封装，和UIView动画不同的是，通过核心动画改变layer的状态（比如position），动画执行完毕后实际上是没有改变的（表面上看起来已改变）。
###### 　　总体来说核心动画的优点有：
　　1）性能强大，使用硬件加速，可以同时向多个图层添加不同的动画效果
　　2）接口易用，只需要少量的代码就可以实现复杂的动画效果。
　　3）运行在后台线程中，在动画过程中可以响应交互事件（UIView动画默认动画过程中不响应交互事件）。
###### 2、核心动画类的层次结构
![](http://upload-images.jianshu.io/upload_images/1644426-7d5b97696b996ed5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Core Animation classes and protocol.png
　　CAAnimation是所有动画对象的父类，实现CAMediaTiming协议，负责控制动画的时间、速度和时间曲线等等，是一个抽象类，不能直接使用。
　　CAPropertyAnimation ：是CAAnimation的子类，它支持动画地显示图层的keyPath，一般不直接使用。
　　iOS9.0之后新增CASpringAnimation类，它实现弹簧效果的动画，是CABasicAnimation的子类。
###### 　　综上，核心动画类中可以直接使用的类有：
　　CABasicAnimation
　　CAKeyframeAnimation
　　CATransition
　　CAAnimationGroup
　　CASpringAnimation
###### ３、核心动画类的核心方法
　　1.初始化CAAnimation对象
　　一般使用animation方法生成实例
`     + (instancetype)animation;`
　　如果是CAPropertyAnimation的子类，还可以通过animationWithKeyPath生成实例
``     + (instancetype)animationWithKeyPath:(nullable NSString *)path;``
　　2.设置动画的相关属性
　　设置动画的执行时间，执行曲线，keyPath的目标值，代理等等
　　3.动画的添加和移除
　　调用CALayer的addAnimation:forKey:方法将动画添加到CALayer中，这样动画就开始执行了
`     - (void)addAnimation:(CAAnimation *)anim forKey:(nullable NSString *)key;`
　　调用CALayer的removeAnimation方法停止CALayer中的动画
```
- (void)removeAnimationForKey:(NSString *)key;
     - (void)removeAllAnimations;
```
###### ４、核心动画类的常用属性
　　keyPath：可以指定keyPath为CALayer的属性值，并对它的值进行修改，以达到对应的动画效果，需要注意的是部分属性值是不支持动画效果的。
　　以下是具有动画效果的keyPath：
```
//CATransform3D Key Paths : (example)transform.rotation.z
     //rotation.x
     //rotation.y
     //rotation.z
     //rotation 旋轉
     //scale.x
     //scale.y
     //scale.z
     //scale 缩放
     //translation.x
     //translation.y
     //translation.z
     //translation 平移
     //CGPoint Key Paths : (example)position.x
     //x
     //y
     //CGRect Key Paths : (example)bounds.size.width
     //origin.x
     //origin.y
     //origin
     //size.width
     //size.height
     //size
     //opacity
     //backgroundColor
     //cornerRadius 
     //borderWidth
     //contents 
     //Shadow Key Path:
     //shadowColor 
     //shadowOffset 
     //shadowOpacity 
     //shadowRadius
```
　　duration：动画的持续时间
　　repeatCount：动画的重复次数
　　timingFunction：动画的时间节奏控制
```
timingFunctionName的enum值如下：
     kCAMediaTimingFunctionLinear 匀速
     kCAMediaTimingFunctionEaseIn 慢进
     kCAMediaTimingFunctionEaseOut 慢出
     kCAMediaTimingFunctionEaseInEaseOut 慢进慢出
     kCAMediaTimingFunctionDefault 默认值（慢进慢出）
```
　　fillMode：视图在非Active时的行为
　　removedOnCompletion：动画执行完毕后是否从图层上移除，默认为YES（视图会恢复到动画前的状态），可设置为NO（图层保持动画执行后的状态，前提是fillMode设置为kCAFillModeForwards）
　　beginTime：动画延迟执行时间（通过CACurrentMediaTime() + your time 设置）
　　delegate：代理
```
代理方法如下：
     - (void)animationDidStart:(CAAnimation *)anim;  //动画开始
     - (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag; //动画结束
```
### CABasicAnimation
　　CABasicAnimation可以设定keyPath的起点，终点的值，动画会沿着设定点进行移动，CABasicAnimation可以看成是只有两个关键点的特殊的CAKeyFrameAnimation。
下面以改变视图的position为例演示其使用：
```
- (void)position {
    CABasicAnimation * ani = [CABasicAnimation animationWithKeyPath:@"position"];
    ani.toValue = [NSValue valueWithCGPoint:self.centerShow.center];
    ani.removedOnCompletion = NO;
    ani.fillMode = kCAFillModeForwards;
    ani.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    [self.cartCenter.layer addAnimation:ani forKey:@"PostionAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-e376a4199763c39a.gif?imageMogr2/auto-orient/strip)
position.gif
　　下面是部分keyPath对应的动画效果（图片名为其对应的keyPath）：
![](http://upload-images.jianshu.io/upload_images/1644426-f00e128b30fa7a65.gif?imageMogr2/auto-orient/strip)
positionX.gif
![](http://upload-images.jianshu.io/upload_images/1644426-1a0def3bd9755111.gif?imageMogr2/auto-orient/strip)
positionY.gif
![](http://upload-images.jianshu.io/upload_images/1644426-18137aca5f93a937.gif?imageMogr2/auto-orient/strip)
rotation.gif
![](http://upload-images.jianshu.io/upload_images/1644426-d0e5022918e3b2b1.gif?imageMogr2/auto-orient/strip)
rotationX.gif
![](http://upload-images.jianshu.io/upload_images/1644426-6f90d0d49163fd23.gif?imageMogr2/auto-orient/strip)
rotationY.gif
![](http://upload-images.jianshu.io/upload_images/1644426-a3e042c6247aa4f2.gif?imageMogr2/auto-orient/strip)
rotationZ.gif
![](http://upload-images.jianshu.io/upload_images/1644426-f192b61c2e84bdd4.gif?imageMogr2/auto-orient/strip)
scale.gif
![](http://upload-images.jianshu.io/upload_images/1644426-24071785074d0c20.gif?imageMogr2/auto-orient/strip)
translation.gif
![](http://upload-images.jianshu.io/upload_images/1644426-98c0138a3b92af63.gif?imageMogr2/auto-orient/strip)
transform的组合效果.gif
![](http://upload-images.jianshu.io/upload_images/1644426-a4c0010089db40d0.gif?imageMogr2/auto-orient/strip)
bounds.gif
![](http://upload-images.jianshu.io/upload_images/1644426-13bba7ca496322fb.gif?imageMogr2/auto-orient/strip)
size.gif
![](http://upload-images.jianshu.io/upload_images/1644426-189cb12f7ab17703.gif?imageMogr2/auto-orient/strip)
sizeW.gif
![](http://upload-images.jianshu.io/upload_images/1644426-da1a68183cb8ac9f.gif?imageMogr2/auto-orient/strip)
sizeH.gif
![](http://upload-images.jianshu.io/upload_images/1644426-da93f8e14652cdca.gif?imageMogr2/auto-orient/strip)
opacity.gif
![](http://upload-images.jianshu.io/upload_images/1644426-7a42250f5e77a418.gif?imageMogr2/auto-orient/strip)
backgroundColor.gif
![](http://upload-images.jianshu.io/upload_images/1644426-24077a0b5bb9bdb6.gif?imageMogr2/auto-orient/strip)
cornerRadius.gif
![](http://upload-images.jianshu.io/upload_images/1644426-e80a2e185b6eced1.gif?imageMogr2/auto-orient/strip)
borderWidth.gif
![](http://upload-images.jianshu.io/upload_images/1644426-2f260766f0490518.gif?imageMogr2/auto-orient/strip)
contents.gif
![](http://upload-images.jianshu.io/upload_images/1644426-3b5967df5bbd46cd.gif?imageMogr2/auto-orient/strip)
shadowColor.gif
![](http://upload-images.jianshu.io/upload_images/1644426-f5a5611b72383d18.gif?imageMogr2/auto-orient/strip)
shadowOffset.gif
![](http://upload-images.jianshu.io/upload_images/1644426-f573cfcd82ce143c.gif?imageMogr2/auto-orient/strip)
shadowOpacity.gif
![](http://upload-images.jianshu.io/upload_images/1644426-096da744fa211de4.gif?imageMogr2/auto-orient/strip)
shadowRadius.gif
### CAKeyframeAnimation
　　可以设定keyPath起点、中间关键点（不止一个）、终点的值，每一帧所对应的时间，动画会沿着设定点进行移动。
###### 　　CAKeyframeAnimation的重要属性：
　　values：关键帧数组对象，里面每一个元素即为一个关键帧，动画会在对应的时间段内，依次执行数组中每一个关键帧的动画。
　　path：动画路径对象，可以指定一个路径，在执行动画时路径会沿着路径移动，Path在动画中只会影响视图的Position。
　　keyTimes：设置关键帧对应的时间点，范围：0-1。如果没有设置该属性，则每一帧的时间平分。
　　下面以让视图绕圈为例演示其使用：
###### 　　1、设置values使其沿正方形运动
```
- (void)valueKeyframeAni {
    CAKeyframeAnimation * ani = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    ani.duration = 4.0;
    ani.removedOnCompletion = NO;
    ani.fillMode = kCAFillModeForwards;
    ani.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    NSValue * value1 = [NSValue valueWithCGPoint:CGPointMake(150, 200)];
    NSValue *value2=[NSValue valueWithCGPoint:CGPointMake(250, 200)];
    NSValue *value3=[NSValue valueWithCGPoint:CGPointMake(250, 300)];
    NSValue *value4=[NSValue valueWithCGPoint:CGPointMake(150, 300)];
    NSValue *value5=[NSValue valueWithCGPoint:CGPointMake(150, 200)];
    ani.values = @[value1, value2, value3, value4, value5];
    [self.centerShow.layer addAnimation:ani forKey:@"PostionKeyframeValueAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-870c4f1a7da41532.gif?imageMogr2/auto-orient/strip)
valueKeyframeAni.gif
###### 　　2、设置path使其绕圆圈运动
```
- (void)pathKeyframeAni {
    CAKeyframeAnimation * ani = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathAddEllipseInRect(path, NULL, CGRectMake(130, 200, 100, 100));
    ani.path = path;
    CGPathRelease(path);
    ani.duration = 4.0;
    ani.removedOnCompletion = NO;
    ani.fillMode = kCAFillModeForwards;
    [self.centerShow.layer addAnimation:ani forKey:@"PostionKeyframePathAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-ae84aa8ce2d54925.gif?imageMogr2/auto-orient/strip)
pathKeyframeAni.gif
### CATransition
　　转场动画，比UIVIew转场动画具有更多的动画效果，比如Nav的默认Push视图的效果就是通过CATransition的kCATransitionPush类型来实现。
###### 　　CAKeyframeAnimation的重要属性：
　　type：过渡动画的类型
```
type的enum值如下：
    kCATransitionFade 渐变
    kCATransitionMoveIn 覆盖
    kCATransitionPush 推出
    kCATransitionReveal 揭开
```
　　还有一些私有动画类型，效果很炫酷，不过不推荐使用。
　　私有动画类型的值有："cube"、"suckEffect"、"oglFlip"、 "rippleEffect"、"pageCurl"、"pageUnCurl"等等。
　　subtype： 过渡动画的方向
```
subtype的enum值如下：
    kCATransitionFromRight 从右边
    kCATransitionFromLeft 从左边
    kCATransitionFromTop 从顶部
    kCATransitionFromBottom 从底部
```
　　以渐变效果为例
```
- (void)transitionAni {
    CATransition * ani = [CATransition animation];
    ani.type = kCATransitionFade;
    ani.subtype = kCATransitionFromLeft;
    ani.duration = 1.5;
    self.centerShow.image = [UIImage imageNamed:@"Raffle"];
    [self.centerShow.layer addAnimation:ani forKey:@"transitionAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-c6c0bc393093bb31.gif?imageMogr2/auto-orient/strip)
kCATransitionFade.gif
以下是另外三种转场类型的动画效果（图片名称对应其type类型）：
![](http://upload-images.jianshu.io/upload_images/1644426-2fef735cbded411d.gif?imageMogr2/auto-orient/strip)
kCATransitionMoveIn.gif
![](http://upload-images.jianshu.io/upload_images/1644426-9d5bd82f6a143ff5.gif?imageMogr2/auto-orient/strip)
kCATransitionPush.gif
![](http://upload-images.jianshu.io/upload_images/1644426-166f0e634d0538ae.gif?imageMogr2/auto-orient/strip)
kCATransitionReveal.gif
以下是部分私有转场类型的动画效果（图片名称对应其type类型）：
![](http://upload-images.jianshu.io/upload_images/1644426-ae0bbcdc6a9aa7c2.gif?imageMogr2/auto-orient/strip)
transitionAni - rippleEffect.gif
![](http://upload-images.jianshu.io/upload_images/1644426-96669e6504df94c1.gif?imageMogr2/auto-orient/strip)
transitionAni - cube.gif
![](http://upload-images.jianshu.io/upload_images/1644426-faf54837bc1fafcd.gif?imageMogr2/auto-orient/strip)
transitionAni - pageCurl.gif
![](http://upload-images.jianshu.io/upload_images/1644426-cc46bc2148a6dc53.gif?imageMogr2/auto-orient/strip)
transitionAni - suckEffect.gif
### CASpringAnimation
　　CASpringAnimation是iOS9新加入动画类型，是CABasicAnimation的子类，用于实现弹簧动画。
###### 　　CASpringAnimation的重要属性：
　　mass：质量（影响弹簧的惯性，质量越大，弹簧惯性越大，运动的幅度越大）
　　stiffness：弹性系数（弹性系数越大，弹簧的运动越快）
　　damping：阻尼系数（阻尼系数越大，弹簧的停止越快）
　　initialVelocity：初始速率（弹簧动画的初始速度大小，弹簧运动的初始方向与初始速率的正负一致，若初始速率为0，表示忽略该属性）
　　settlingDuration：结算时间（根据动画参数估算弹簧开始运动到停止的时间，动画设置的时间最好根据此时间来设置）
###### 　　CASpringAnimation和UIView的SpringAnimation对比：
　　1.CASpringAnimation 可以设置更多影响弹簧动画效果的属性，可以实现更复杂的弹簧动画效果，且可以和其他动画组合。
　　2.UIView的SpringAnimation实际上就是通过CASpringAnimation来实现。
　　以实现视图的bounds变化的弹簧动画效果为例：
```
- (void)springAni {
    CASpringAnimation * ani = [CASpringAnimation animationWithKeyPath:@"bounds"];
    ani.mass = 10.0; //质量，影响图层运动时的弹簧惯性，质量越大，弹簧拉伸和压缩的幅度越大
    ani.stiffness = 5000; //刚度系数(劲度系数/弹性系数)，刚度系数越大，形变产生的力就越大，运动越快
    ani.damping = 100.0;//阻尼系数，阻止弹簧伸缩的系数，阻尼系数越大，停止越快
    ani.initialVelocity = 5.f;//初始速率，动画视图的初始速度大小;速率为正数时，速度方向与运动方向一致，速率为负数时，速度方向与运动方向相反
    ani.duration = ani.settlingDuration;
    ani.toValue = [NSValue valueWithCGRect:self.centerShow.bounds];
    ani.removedOnCompletion = NO;
    ani.fillMode = kCAFillModeForwards;
    ani.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    [self.cartCenter.layer addAnimation:ani forKey:@"boundsAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-b738557ab9ca1201.gif?imageMogr2/auto-orient/strip)
springAni.gif
### CAAnimationGroup
　　使用Group可以将多个动画合并一起加入到层中，Group中所有动画并发执行，可以方便地实现需要多种类型动画的场景。
　　以实现视图的position、bounds和opacity改变的组合动画为例
```
- (void)groupAni {
    CABasicAnimation * posAni = [CABasicAnimation animationWithKeyPath:@"position"];
    posAni.toValue = [NSValue valueWithCGPoint:self.centerShow.center];
    CABasicAnimation * opcAni = [CABasicAnimation animationWithKeyPath:@"opacity"];
    opcAni.toValue = [NSNumber numberWithFloat:1.0];
    opcAni.toValue = [NSNumber numberWithFloat:0.7];
    CABasicAnimation * bodAni = [CABasicAnimation animationWithKeyPath:@"bounds"];
    bodAni.toValue = [NSValue valueWithCGRect:self.centerShow.bounds];
    CAAnimationGroup * groupAni = [CAAnimationGroup animation];
    groupAni.animations = @[posAni, opcAni, bodAni];
    groupAni.duration = 1.0;
    groupAni.fillMode = kCAFillModeForwards;
    groupAni.removedOnCompletion = NO;
    groupAni.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    [self.cartCenter.layer addAnimation:groupAni forKey:@"groupAni"];
}
```
　　动画效果：
![](http://upload-images.jianshu.io/upload_images/1644426-26c4ccd453af5342.gif?imageMogr2/auto-orient/strip)
groupAni.gif
### CATransaction
　　最后讲一下事务，在核心动画里面存在事务（CATransaction）这样一个概念，它负责协调多个动画原子更新显示操作。
　　简单来说事务是核心动画里面的一个基本的单元，动画的产生必然伴随着layer的Animatable属性的变化，而layer属性的变化必须属于某一个事务。
　　因此，核心动画依赖事务。
　　事务的作用：保证一个或多个layer的一个或多个属性变化同时进行
　　事务分为隐式和显式：
　　1.隐式：没有明显调用事务的方法，由系统自动生成事务。比如直接设置一个layer的position属性，则会在当前线程自动生成一个事务，并在下一个runLoop中自动commit事务。
　　2.显式：明显调用事务的方法（[CATransaction begin]和[CATransaction commit]）。
　　CA事务的可设置属性（会覆盖隐式动画的设置）：
```bash
animationDuration：动画时间
    animationTimingFunction：动画时间曲线
    disableActions：是否关闭动画
    completionBlock：动画执行完毕的回调
```
　　事务支持嵌套使用：当最外层的事务commit后动画才会开始。
　　使用实例：
```
[CATransaction begin];
    [CATransaction setAnimationDuration:2.0];
    [CATransaction setAnimationTimingFunction:[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
//    [CATransaction setDisableActions:YES]; //设置为YES就关闭动画
    self.subLayer.bounds = self.centerShow.layer.bounds;
    [CATransaction commit];
```
　　注意：只有非root layer才有隐式动画，如果你是直接设置`self.cartCenter.layer.bounds = self.centerShow.layer.bounds;`是不会有动画效果的。
#### Next
　　接下来将更新iOS绘图引擎Quartz2D
文／明仔Su（简书作者）
原文链接：http://www.jianshu.com/p/d05d19f70bac
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
