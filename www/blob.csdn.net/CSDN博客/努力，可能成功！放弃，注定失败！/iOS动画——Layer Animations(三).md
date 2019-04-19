# iOS动画——Layer Animations(三) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月07日 10:07:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：287标签：[iOS																[iOS动画																[iOS开发																[Layer Animations](https://so.csdn.net/so/search/s.do?q=Layer Animations&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS动画&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
**我胡汉三又回来啦，忙过了前一段时间以后，今天又给大家带来了两个动画，一个是形变、一个是颜色渐变。如下所示：**
**![动画来源《iOS Animation by tutorials》](http://upload-images.jianshu.io/upload_images/255820-f7ec5124ade14674.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)**
**动画来源《iOS Animation by tutorials》**
**源码在github，地址在本文最后。**
**第一个动画：我们来分析一下第一个动画，两个头像的位移我们前面已经讲过了，今天主要讲讲形变，在这个动画里一个是变椭圆，一个是变回正方形。这两个忍者（这里以及后面都把他称呼为忍者）并不是UIImageView，而是一个UIView，然后在UIView中添加了UIImage（事实上用UIImageView也可以）。我们来看一下他的层次结构：**
**![事实上有三层哦](http://upload-images.jianshu.io/upload_images/255820-f01b4954020dad0d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)**
**事实上有三层哦**
**那么我们要做的第一步就是吧这三个图层添加到UIView：**
`**    let photoLayer         = CALayer()    let circleLayer        = CAShapeLayer()    let maskLayer          = CAShapeLayer()  /**    将Layer添加到父类    */    override func didMoveToWindow() {        layer.addSublayer(photoLayer)        photoLayer.mask = maskLayer        //遮罩层        layer.addSublayer(circleLayer)    }    /**    初始化三个Layer    */    override func layoutSubviews() {        //Size the avatar image to fit        photoLayer.frame = CGRect(            x: (bounds.size.width - image.size.width + lineWidth)/2,            y: (bounds.size.height - image.size.height - lineWidth)/2,            width: image.size.width,            height: image.size.height)        //Draw the circle        circleLayer.path = UIBezierPath(ovalInRect: bounds).CGPath        circleLayer.strokeColor = UIColor.whiteColor().CGColor        circleLayer.lineWidth = lineWidth        circleLayer.fillColor = UIColor.clearColor().CGColor        //Size the layer        maskLayer.path = circleLayer.path        maskLayer.position = CGPoint(x: 0.0, y: 10.0)    }**`
**初始化部分：我们声明了三个CALayer，CAShapeLayer继承自CALayer，然而这个layer可以用 UIBezierPath画出你想要的图形。我们可以看到photoLayer声明为了一个正方形、circleLayer则用UIBezierPath画了一个圆圈，并且设置了线条的颜色和线条的宽度，同时把填充颜色设置为clearColor。maskLayer也是一个圆，但是我们同时设置他的positon属性，讲他向下挪了10个point，注意position的坐标远点为左上角。设置部分：我们将maskLayer和photoLayer添加到了UIView上，前面我们说过了photoLayer是一个正方形，那么他是如何显示为圆形的呢，我们给他设置了一个遮罩层：**
`**   photoLayer.mask = maskLayer        //遮罩层**`
**写过flash的同学一定知道遮罩层的概念是什么，遮罩层就是显示的部分，如上设置以后，photoLayer就只显示maskLayer的部分，所以当maskLayer发生改变的时候，photoLayer显示的部分也会发生改变，所以我们接下来只需要对maskLayer进行动画设置就可以。**
**我们现在运行一下程序看一下效果，你可以看到是这样的：**
**![1.png](http://upload-images.jianshu.io/upload_images/255820-25be49a77db5cd88.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)**
**1.png**
**好的接下来就是关键的动画代码了，如下：**
`**func bounceOffPoint(bouncePoint: CGPoint, morphSize: CGSize) {        let originalCenter = center        UIView.animateWithDuration(animationDuration, delay: 0.0, usingSpringWithDamping: 0.8, initialSpringVelocity: 0.0, options: nil, animations: {            self.center = bouncePoint            }, completion: {_ in                //complete bounce to                if self.shouldTransitionToFinishedState {                    self.animateToSquare()                }        })        UIView.animateWithDuration(animationDuration, delay: animationDuration, usingSpringWithDamping: 0.7, initialSpringVelocity: 1.0, options: nil, animations: {            self.center = originalCenter            }, completion: {_ in                delay(seconds: 0.1) {                    if !self.isSquare {                        self.bounceOffPoint(bouncePoint, morphSize: morphSize)                    }                }        }) /// 椭圆动画        let morphedFrame = (originalCenter.x > bouncePoint.x) ?            CGRect(x: 0.0, y: bounds.height - morphSize.height,                width: morphSize.width, height: morphSize.height):            CGRect(x: bounds.width - morphSize.width,                y: bounds.height - morphSize.height,                width: morphSize.width, height: morphSize.height)        let morphAnimation = CABasicAnimation(keyPath: "path")        morphAnimation.duration = animationDuration        morphAnimation.toValue = UIBezierPath(ovalInRect: morphedFrame).CGPath        morphAnimation.timingFunction = CAMediaTimingFunction(name: kCAMediaTimingFunctionEaseOut)        circleLayer.addAnimation(morphAnimation, forKey:nil)        maskLayer.addAnimation(morphAnimation, forKey: nil)    }    /**    方形动画    */    func animateToSquare() {        isSquare = true        let squarePath = UIBezierPath(rect: self.bounds).CGPath        let morph = CABasicAnimation(keyPath: "path")        morph.duration = 0.25        morph.fromValue = circleLayer.path        morph.toValue = squarePath        circleLayer.addAnimation(morph, forKey: nil)        maskLayer.addAnimation(morph, forKey: nil)        circleLayer.path = squarePath        maskLayer.path = squarePath    }**`
**我们主要看椭圆动画部分和方形动画部分。我们在这里可以看到我们还是创建了一个CALayer Aniamtion CABasicAnimation，这个动画在前面部分我们已经看到很多回了，但是在这里我们的keypath为“path”确实第一次做，当keypath为“path”的时候主要是对UIView的形状进行改变。我们可以看到这个 CABasicAnimation的结构和之前还是一样的，同样是.toValue、.fromValue、.duration。所以当对形状进行动画的时候关键就是如何用 UIBezierPath来描绘你想要编程的图像。我们今天要描绘的是两个非常简单的图案。**
`**morphAnimation.toValue = UIBezierPath(ovalInRect: morphedFrame).CGPath    //描绘了一个椭圆let squarePath = UIBezierPath(rect: self.bounds).CGPath //描绘了一个正方形**`
**UIBezierPath给我们定制很多非常方便的初始化函数，更多信息请查看官方文档。**
**中间还有一些时间的计算和距离的计算这里就不多进行讲述，相信大家能自己看懂。**
**来看第二个动画**
**第二个动画就是一个渐变的色的改变，在这里我们需要了解一个类CAGradientLayer，这个图层很好的实现了对颜色的控制。他有几个属性需要了解，startPoint、endPoint、colors、locations，我们来看一下我们这里的对CAGradientLayer的初始化过程**
`**  let gradientLayer: CAGradientLayer = {    let gradientLayer = CAGradientLayer()    // Configure the gradient here    gradientLayer.startPoint = CGPoint(x: 0.0, y: 0.5)    gradientLayer.endPoint = CGPoint(x: 1.0, y: 0.5)    let colors = [      UIColor.yellowColor().CGColor,      UIColor.greenColor().CGColor,      UIColor.orangeColor().CGColor,      UIColor.cyanColor().CGColor,      UIColor.redColor().CGColor,      UIColor.yellowColor().CGColor    ]    gradientLayer.colors = colors    let locations = [      0.0, 0.0, 0.0, 0.0, 0.0, 0.25    ]    gradientLayer.locations = locations    return gradientLayer    }()**`
**这里需要注意的就是渐变方向默认是由上到下的，而当我们设施startPoint和endPoint以后可以改变渐变的方向。locations对应每一个colors中的color的位置。然后我们将这个CALayer添加到UIView。**
`**override func layoutSubviews() {    gradientLayer.frame = CGRect(      x: -bounds.size.width,      y: bounds.origin.y,      width: 3 * bounds.size.width,      height: bounds.size.height)  }  override func didMoveToWindow() {    super.didMoveToWindow()    layer.addSublayer(gradientLayer)  }**`
**当然我们必须记得给这个CALayer设置frame，不然默认为0，我们现在可以运行看一下效果：**
**![1.png](http://upload-images.jianshu.io/upload_images/255820-bd060f9038791923.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)**
**1.png**
**我们已经实现了颜色的渐变，那么我们现在给他添加动画：**
```cpp
```cpp
**    let gradientAnimation         = CABasicAnimation(keyPath: "locations")    gradientAnimation.fromValue   = [      0.0, 0.0, 0.0, 0.0, 0.0, 0.25    ]    gradientAnimation.toValue     = [      0.65, 0.8, 0.85, 0.9, 0.95, 1.0    ]    gradientAnimation.duration    = 3.0    gradientAnimation.repeatCount = Float.infinity    gradientLayer.addAnimation(gradientAnimation, forKey: nil)**
```
```
**我们这里的keypath为locations也就是改变每一个颜色的位置，那么就实现了动画的过程。 CABasicAnimation的框架和前面并没有太大的区别，我们 再来运行一下程序看一下效果:**
**![10.gif](http://upload-images.jianshu.io/upload_images/255820-ebc46104625d30c6.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)**
**10.gif**
**效果如上所示，我们已经大概完成了动画，颜色唰唰的滚动，咦，但是怎么好像和最前面看到的不大一样的，开篇我们看到的明明是一串字符串啊。哦～我们还需要制作一个遮罩层，这个遮罩层是一个文字的遮罩层。代码如下：**
`** @IBInspectable var text: String! {    didSet {      UIGraphicsBeginImageContextWithOptions(frame.size, false, 0)      let context               = UIGraphicsGetCurrentContext()      text.drawInRect(bounds, withAttributes: textAttributes)      let image                 = UIGraphicsGetImageFromCurrentImageContext()      UIGraphicsEndImageContext()      let maskLayer             = CALayer()      maskLayer.backgroundColor = UIColor.clearColor().CGColor      maskLayer.frame           = CGRectOffset(bounds, bounds.size.width, 0)      maskLayer.contents        = image.CGImage      gradientLayer.mask        = maskLayer    }  }**`
**这里出现了很多没有见过的类，乍一眼看过去，“娘的，吓尿了～”，然而事实上并不复杂。UIGraphicsBeginImageContextWithOptions用来创建一个context，他和 UIGraphicsEndImageContext是一对,在他们俩之间我们可以来绘制一些图片，UIGraphicsGetCurrentContext创建了一个栈，**
``**text.drawInRect(bounds, withAttributes: textAttributes)**``
**这句话将文字绘制成图片，并压入上面我们创建的那一个栈，**
`** let image                 = UIGraphicsGetImageFromCurrentImageContext()**`
**这句话取栈顶，也就是我们之前的用text绘制的图片，那么现在image就是我们之前的文字，只不过是UIImage格式的。你可以理解为PNG格式～（不知道我这么理解对不还是不对，若是有大神看到请指出。）**
**接下来设置遮罩层和上一个动画就没有太大的区别的了；**
**好了 好了，我们终于写完了这两个动画，这两个动画并不复杂，结合代码和我的解释 大家都应该能够看懂。**
**代码已上传github：[https://github.com/superxlx/iOS_Animation_Test5](https://github.com/superxlx/iOS_Animation_Test5)**
**觉得本文对你有帮助的请关注我，并点击一下喜欢～亲～**
