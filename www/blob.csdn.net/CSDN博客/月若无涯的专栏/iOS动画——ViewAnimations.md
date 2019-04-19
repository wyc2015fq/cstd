# iOS动画——ViewAnimations - 月若无涯的专栏 - CSDN博客
2015年05月21日 10:05:14[月若无涯](https://me.csdn.net/u010124617)阅读数：712标签：[ios开发																[动画](https://so.csdn.net/so/search/s.do?q=动画&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[iOS开发																[动画](https://blog.csdn.net/u010124617/article/category/3253887)](https://blog.csdn.net/u010124617/article/category/1393991)

又给自己挖了一个坑，我很喜欢动画不错，但是写出来又是另外一个问题了～～～
这一篇我们来说说UIKit中的动画API，期中包括：
- UIView.UIView.animateWithDuration
- UIView.transitionWithView
- UIView.animateKeyframesWithDuration
- UIView.addKeyframeWithRelativeStartTime
今天的故事就将围绕这些API展开，阐述他的前世今生。
UIKit动画API使用起来十分简单与方便，他避免了Core Animation的复杂性，虽然事实上UIKit动画API的底层使用的也是Core Animation。
来看第一个，UIView.UIView.animateWithDuration
先来看一下函数原型：
`   class func animateWithDuration(duration: NSTimeInterval, delay: NSTimeInterval, usingSpringWithDamping dampingRatio: CGFloat, initialSpringVelocity velocity: CGFloat, options: UIViewAnimationOptions, animations: () -> Void, completion: ((Bool) -> Void)?)`
一共七个参数：
- duration
- 表示动画执行时间。
- delay
- 动画延迟时间。
- usingSpringWithDamping
- 表示弹性属性。
- initialSpringVelocity
- 初速度。
- options
- 可选项，一些可选的动画效果，包括重复等。
- animations
- 表示执行的动画内容，包括透明度的渐变，移动，缩放。
- completion
- 表示执行完动画后执行的内容。
关于这些参数，选一个列子，尝试不同的参数，这样可以更好的理解每个参数的意义。
![好丑的动画](http://upload-images.jianshu.io/upload_images/255820-921c2916be7ff3fe.gif?imageView2/2/w/1240/q/100)
好丑的动画
```
self.animationView2.alpha = 0
        self.animationView3.alpha = 0
        UIView.animateWithDuration(5, delay: 0.5, usingSpringWithDamping: 0.5, initialSpringVelocity: 0, options: .Repeat, animations: { () -> Void in
            self.animationView.center.y += 100
        }) { (Bool) -> Void in
            println("finish")
        }
        UIView.animateWithDuration(5, delay: 0.5, usingSpringWithDamping: 0.5, initialSpringVelocity: 0, options: .Repeat, animations: { () -> Void in
            self.animationView2.alpha = 1
            }) { (Bool) -> Void in
                println("finish")
        }
        UIView.animateWithDuration(5, delay: 0.5, usingSpringWithDamping: 0.5, initialSpringVelocity: 0, options: .Repeat, animations: { () -> Void in
            self.animationView3.center.y -= 100
            self.animationView3.alpha = 1
            }) { (Bool) -> Void in
                println("finish")
        }
```
代码就不逐行解释，三个UIView.animateWithDuration分别操作三个方块。第一个方块是一个移动动画，第二个方块是一个透明度渐变动画，第三个方块是移动加透明度渐变的组合动画，可能看的不是很清楚。不得不说这个动画真的很丑~~~
UIView.UIView.animateWithDuration这个API说穿了就是逐渐改变UIView的某项属性，这些属性包括：位置，大小，透明度，颜色等等。
再来看一下**UIView.transitionWithView**,这是一个过度动画，主要用于UIView进入或者离开视图。
先看一下这一个动画效果吧：
![9.gif](http://upload-images.jianshu.io/upload_images/255820-7952eba7acb3e6ef.gif?imageView2/2/w/1240/q/100)
9.gif
其中banner右移消失的动画用的就是上面提到的UIView.UIView.animateWithDuration，而进入的动画用的就是现在要讲的UIView.transitionWithView。很像一页书页翻下来的感觉哈。
我们来看一下函数原型
`    class func transitionWithView(view: UIView, duration: NSTimeInterval, options: UIViewAnimationOptions, animations: () -> Void, completion: ((Bool) -> Void)?)`
一共五个参数：
- view
- 这当然就是指定要进行动画的对象了。
- duration
- 和上面一样这个参数指定动画时间长短。
- options
- 这是一个可选参数，虽然是可选的但是不填这个API就没意义了，因为UIView如何进入视图就是由这个参数决定。到底是像书页一样翻进去，还是像百叶窗一样转动就是由这个参数决定，具体有哪些可以选择，点进去看看就知道了。
- animations
- 这个选项你可以将它理解为在动画结束后UIView的形态。
- completion
- 动画结束后运行的代码。
代码大概长这样
```
UIView.transitionWithView(status, duration: 0.33, options:
            .CurveEaseOut | .TransitionCurlDown, animations: {
                self.yourView.hidden = false
            }, completion:nil
        })
```
这一部分代码已上传Github，Github地址在文章的最后面。
我相信看看源代码，大家都能明白的。
这里再给大家看一个动画，也是用前面提到过的函数写的：
![尼玛～这显示效果太差了吧，不知道你们那里看到的是什么样的](http://upload-images.jianshu.io/upload_images/255820-cc5b72c4fe90024d.gif?imageView2/2/w/1240/q/100)
尼玛～这显示效果太差了吧，不知道你们那里看到的是什么样的
仿3D效果，代码也在上传的那个Demo中，大家自己看啦～
到最后一个函数啦啦，**UIView.animateKeyframesWithDuration**，先来看一下动画效果吧。
![小飞机～飞啊飞～](http://upload-images.jianshu.io/upload_images/255820-d0466657e2fb3277.gif?imageView2/2/w/1240/q/100)
小飞机～飞啊飞～
我们很容易就可以发现，这个动画分了很多阶段完成，我们当然可以用我们提到的第一个函数**UIView.UIView.animateWithDuration**来完成，但是，你不觉得嵌套加嵌套显得很不好看吗，我们当然还有更好的方法来实现，就是我们现在要说的，先来看一下函数原型：
`class func animateKeyframesWithDuration(duration: NSTimeInterval, delay: NSTimeInterval, options: UIViewKeyframeAnimationOptions, animations: () -> Void, completion: ((Bool) -> Void)?)`
一共五个参数:
- duration：整个动画过程的时间。
- 
delay：延迟多久开始。
- 
options：可选项，比如说重复，倒着来一遍等效果，自己都试试看吧。
- 
animations：需要执行的动画，里面可以是多个**UIView.addKeyframeWithRelativeStartTime**。
- 至于这个**UIView.addKeyframeWithRelativeStartTime**方法，类似于我们提到的第一个**UIView.UIView.animateWithDuration**，也是一个属性渐变的方法，不过这个方法只能写在他的爸爸**UIView.addKeyframeWithRelativeStartTime**的animation参数函数块中。
- completion：动画执行结束之后执行的代码。
来看一下我们实现这个小飞机～飞啊飞～～的代码：
```
let originalCenter = planeImage.center
        UIView.animateKeyframesWithDuration(1.5, delay: 0.0, options: .Repeat, animations: {
            //add keyframes
            UIView.addKeyframeWithRelativeStartTime(0.0, relativeDuration: 0.25, animations: {
                self.planeImage.center.x += 80.0
                self.planeImage.center.y -= 10.0
            })
            UIView.addKeyframeWithRelativeStartTime(0.1, relativeDuration: 0.4) {
                self.planeImage.transform = CGAffineTransformMakeRotation(CGFloat(-M_PI_4/2))
            }
            UIView.addKeyframeWithRelativeStartTime(0.25, relativeDuration: 0.25) {
                self.planeImage.center.x += 100.0
                self.planeImage.center.y -= 50.0
                self.planeImage.alpha = 0.0
            }
            UIView.addKeyframeWithRelativeStartTime(0.51, relativeDuration: 0.01) {
                self.planeImage.transform = CGAffineTransformIdentity
                self.planeImage.center = CGPoint(x: 0.0, y: originalCenter.y)
            }
            UIView.addKeyframeWithRelativeStartTime(0.55, relativeDuration: 0.45) {
                self.planeImage.alpha = 1.0
                self.planeImage.center = originalCenter
            }
            }, completion:nil)
```
完整的代码在最后提供的源代码中有。
事实告诉我们动画是要靠设计的，你看我上面的动画抽的一笔，但事实上用同样的代码可以写出很漂亮的动画。
代码已上传Github：[https://github.com/superxlx/iOS_Animation_Test1](https://github.com/superxlx/iOS_Animation_Test1)
[](https://github.com/superxlx/iOS_Animation_Test1)
