# iOS动画——Layer Animations(一) - 月若无涯的专栏 - CSDN博客
2015年05月21日 10:06:54[月若无涯](https://me.csdn.net/u010124617)阅读数：551标签：[ios开发																[动画](https://so.csdn.net/so/search/s.do?q=动画&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[动画																[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)](https://blog.csdn.net/u010124617/article/category/3253887)
Layer Animations与我们前面第一篇讲过的View Animation有点类似，但是Layer Animation比之多了很多效果，更加强大。
我们先来看一下今天我们要实现的效果，今天实现的效果用第一篇View Animations能实现相同效果。
> - 本文所讲为实现此动画的相关知识。
![动画由书籍《iOS Animations by tutorials》提供，我只是一个复述者](http://upload-images.jianshu.io/upload_images/255820-e4815273593560eb.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
动画由书籍《iOS Animations by tutorials》提供，我只是一个复述者
哦～先来看一下Layer是什么吧：
> 
比较通俗的来说，CALayer就是UIView的视图层，你所看到的UIView，其实是UIView的layer。这么说吧，CALayer就是树叶的叶绿素，和叶绿素不同的就是，CALayer更加的“单纯”，我们知道叶绿素是包括很多基质的，而CALayer仅仅是代表了你能看到的一切。
我们今天把所有的重点都放在动画的编写上，默认有**swift**基础
如果你观察都仔细的话，你会发现，背景上的云是渐入的，也就是透明度由0到1，当然这个用我们前面学的UIViewAnimation是很容易实现的，那么用CALayer如何实现呢，看下面的代码：
```
//1
    let fadeIn = CABasicAnimation(keyPath: "opacity")
    //2
    fadeIn.fromValue = 0.0
    //3
    fadeIn.toValue = 1.0
    //4
    fadeIn.duration = 0.5
    //5
    fadeIn.fillMode = kCAFillModeBackwards
    //6
    fadeIn.beginTime = CACurrentMediaTime() + 0.5
    cloud1.layer.addAnimation(fadeIn, forKey: nil)
    fadeIn.beginTime = CACurrentMediaTime() + 0.7
    cloud2.layer.addAnimation(fadeIn, forKey: nil)
    fadeIn.beginTime = CACurrentMediaTime() + 0.9
    cloud3.layer.addAnimation(fadeIn, forKey: nil)
    fadeIn.beginTime = CACurrentMediaTime() + 1.1
    cloud4.layer.addAnimation(fadeIn, forKey: nil)
```
> 
很明显我们是给四朵云的**layer**添加了动画，然后实现了渐入的效果。
- 1、这句话声明了一个** CABasicAnimation**，注意到里面填写的参数没，填的是**opacity**，就是透明度的意思，这里头还能填写很多其他值，比如**position**，当然这些我们后面都会讲的。
- 2、我们对动画的初始值进行设定，也就是透明度最开始为0.
- 3、我们对动画的最终值进行设定，也就是透明度为1。
- 4、动画持续时间，我们设定为0.5秒。和前面三句合起来，就表达了这么一个意思：这个动画是对对象的透明度进行改变，在0.5秒内，透明度从0变化为1.
- 5、我们给fillMode属性填的值是kCAFillModeBackwards，那么kCAFillModeBackwards这个值有什么用呢，这个属性可以显示对象的frame，我们可以把这一句注释以后运行程序来看一下效果，我们会发现，在进行动画之前，云朵任然可见，而这显然是一个BUG，如何解决这个BUG呢，其实方法很多，比如我们可以讲云朵的透明度都设置为0，然后计算好动画时间，当动画结束以后将云朵的透明度设置为1，这样做当然可以实现相同的效果，但是这样做实在是～～～～太不优雅了，还有一种做法就是添加fillMode属性，kCAFillModeBackwards的意思是显示动画的初始状态，同时还有其他两个值**kCAFillModeForwards**可以显示对象动画之后的效果，**kCAFillModeBoth**则是兼顾以上两个效果。
- 6、这个属性很好解释，每一朵云朵的动画并不是同时进行的，那么我们就给云朵设定开始的时间，这个属性和我们前面说过的UIViewAnimation的delay这个参数比较类似。
以上内容实现了云朵的渐入动画。
如果对我已经说过好几遍的UIViewAniamtion有疑问的话，请自行阅读本人前面的文章，觉得不错的话请关注本人，再点一个喜欢吧，亲～～
接下来实现的是标题、**Username**、**PassWord**有screen外由左向右移动到屏幕中心的动画，直接上代码：
```
//1
    let flyRight = CABasicAnimation(keyPath: "position.x")
    flyRight.toValue = view.bounds.size.width/2
    flyRight.fromValue = -view.bounds.size.width/2
    flyRight.duration = 0.5
    heading.layer.addAnimation(flyRight, forKey: nil)
    //2
    flyRight.beginTime = CACurrentMediaTime() + 0.3
    flyRight.fillMode = kCAFillModeBackwards
    username.layer.addAnimation(flyRight, forKey: nil)
   flyRight.beginTime = CACurrentMediaTime() + 0.4
   password.layer.addAnimation(flyRight, forKey: nil)
```
> - //1 通过对云朵动画的讲解，相信其实我们已经能够大致看懂这一段代码了，和上面唯一不同的就是，我们这里创建的CABasicAnimation的动画对象为**"position.x"**，**fromvalue**和**toVaule**相信也不用进行太多讲解，值得一题的是我们的值指的是对象的**center.x**，而不是左上角。
- //2 对username延迟0.3秒进行。同时同样设定
**flyRight.fillMode = kCAFillModeBackwards**
是不是很简单呢，是的～
Log in 按钮的动画，上代码：
```
UIView.animateWithDuration(0.5, delay: 0.5, usingSpringWithDamping: 0.5, initialSpringVelocity: 0.0, options: nil, animations: {
        self.loginButton.center.y -= 30.0
        self.loginButton.alpha = 1.0
    }, completion: nil)
```
> 
对于这一段代码的解释在这里，说的十分详细，作者也长得很帅，看头像就看得出来：[http://www.jianshu.com/p/bd7bf438b288](http://www.jianshu.com/p/bd7bf438b288) 喜欢的话请关注他。
我们还发现云朵是不断的移动的，继续上代码：
```
func animateCloud(cloud: UIImageView) {
    let cloudSpeed = 60.0 / view.frame.size.width
    let duration = (view.frame.size.width - cloud.frame.origin.x) * cloudSpeed
    UIView.animateWithDuration(NSTimeInterval(duration), delay: 0.0, options: .CurveLinear, animations: {
      cloud.frame.origin.x = self.view.frame.size.width
    }, completion: {_ in
      cloud.frame.origin.x = -cloud.frame.size.width
      self.animateCloud(cloud)
    })
  }
```
> 解释请参考Log in按钮的解释。 ------------- 先就只剩下点击**Log in**按钮以后的动画了，我们先来看一下发生什么了，当我们点击按钮以后，按钮duang～的一下蹦到下面了，同时颜色变了，圆角变大了，然后添加了一个活动指示器。
 上代码：
```
@IBAction func login() {
    //1
    UIView.animateWithDuration(1.5, delay: 0.0, usingSpringWithDamping: 0.2, initialSpringVelocity: 0.0, options: nil, animations: {
      self.loginButton.bounds.size.width += 80.0
    }, completion: nil)
    //2
    UIView.animateWithDuration(0.33, delay: 0.0, usingSpringWithDamping: 0.7, initialSpringVelocity: 0.0, options: nil, animations: {
      self.loginButton.center.y += 60.0
    //3
      self.spinner.center = CGPoint(x: 40.0, y: self.loginButton.frame.size.height/2)
      self.spinner.alpha = 1.0
    }, completion: {_ in
    //4
      self.showMessage(index: 0)
    })
    //5
    let tintColor = UIColor(red: 0.85, green: 0.83, blue: 0.45, alpha: 1.0)
    tintBackgroundColor(layer: loginButton.layer, toColor: tintColor)
    //6
    roundCorners(layer: loginButton.layer, toRadius: 25.0)
  }
```
> 
这一段就厉害了，因为这一段特别的长。
- 1、duang～的一下按钮变长了。
- 2、duang～的一下按钮下移了。
- 3、添加活动指示器。
- 4、添加**message**这个后面再说。
- 5、调用tintBackgroundColor方法，改变颜色，这是**tintBackgroundColor**方法的代码：
```
func tintBackgroundColor(#layer: CALayer, #toColor: UIColor) {
let tint = CABasicAnimation(keyPath: "backgroundColor")
tint.fromValue = layer.backgroundColor
layer.backgroundColor = toColor.CGColor
tint.toValue = toColor.CGColor
tint.duration = 1.0
tint.fillMode = kCAFillModeBoth
layer.addAnimation(tint, forKey: nil)
}
```
其实这个方法和前面的CABasicgroundColor大体是相同的，我们先把颜色改变成我们需要变成的颜色，然后执行动画。
- 6、增大圆角的动画
```
func roundCorners(#layer: CALayer, #toRadius: CGFloat) {
let round = CABasicAnimation(keyPath: "cornerRadius")
round.fromValue = layer.cornerRadius
layer.cornerRadius = toRadius
round.toValue = toRadius
round.duration = 0.33
layer.addAnimation(round, forKey: nil)
}
```
这个实现的方法大体是上面改变颜色的思想是一模一样的。也是先改变圆角，然后执行动画，最后显示的会是你一开始设定的圆角。
现在整个动画就只剩下了那个message的动画，和message的动画结束以后，Log in按钮弹回的动画，而Log in按钮弹回的动画和前面刚说过的按钮弹下是一模一样的，只是相反而已。我们来看一下**message**的动画：
```
func removeMessage(#index: Int) {
    UIView.animateWithDuration(0.33, delay: 0.0, options: nil, animations: {
      self.status.center.x += self.view.frame.size.width
    }, completion: {_ in
      self.status.hidden = true
      self.status.center = self.statusPosition
      self.showMessage(index: index+1)
    })
  }
  func resetForm() {
    UIView.transitionWithView(status, duration: 0.2, options: .TransitionFlipFromTop, animations: {
      self.status.hidden = true
      self.status.center = self.statusPosition
    }, completion: nil)
    UIView.animateWithDuration(0.2, delay: 0.0, options: nil, animations: {
      self.spinner.center = CGPoint(x: -20.0, y: 16.0)
      self.spinner.alpha = 0.0
      self.loginButton.bounds.size.width -= 80.0
      self.loginButton.center.y -= 60.0
    }, completion: {_ in
      let tintColor = UIColor(red: 0.63, green: 0.84, blue: 0.35, alpha: 1.0)
      tintBackgroundColor(layer: self.loginButton.layer, toColor: tintColor)
      roundCorners(layer: self.loginButton.layer, toRadius: 10.0)
    })
  }
```
> 
这一段代码用的全部都是UIViewAnimation，就不做多解释，还是比较好理解的，若是不理解，请看我前面的写过的文章。
我应该没有落下什么了～～～
各位看官看到这里也是难为你们了。若是有问题欢迎留言提问。
所有代码已经上传github：[https://github.com/superxlx/iOS_Animation_Test3](https://github.com/superxlx/iOS_Animation_Test3)
