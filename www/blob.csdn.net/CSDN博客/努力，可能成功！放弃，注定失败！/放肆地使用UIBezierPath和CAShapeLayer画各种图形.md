# 放肆地使用UIBezierPath和CAShapeLayer画各种图形 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月21日 20:41:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：291标签：[ios																[iOS开发																[CAShapeLayer																[UIBezierPath](https://so.csdn.net/so/search/s.do?q=UIBezierPath&t=blog)](https://so.csdn.net/so/search/s.do?q=CAShapeLayer&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
CAShapeLayer 是 CALayer 的子类，但是比 CALayer 更灵活，可以画出各种图形，当然，你也可以使用其他方式来画，随你。
**杂谈**
在 CAShapeLayer 中，也可以像 CALayer 一样指定它的 frame 来画，就像这样：
```
```
let layer = CAShapeLayer()
```
```
layer.frame = CGRectMake(110, 100, 150, 100)
```
```
layer.backgroundColor = UIColor.blackColor().CGColor
```
` `
`view.layer.addSublayer(layer)`
```
然后你就会得到如图这样的黑色矩形
![QQ截图20160214095926.png](http://cc.cocimg.com/api/uploads/20160214/1455415173829788.png)
但是，CAShapeLayer 有一个神奇的属性 path 用这个属性配合上 UIBezierPath 这个类就可以达到超神的效果。
UIBezierPath 顾名思义，这是用贝塞尔曲线的方式来构建一段弧线，你可以用任意条弧线来组成你想要的形状，比如，你想用它来和上面一样画一个矩形，那就可以这样子来做：
```
```
let path = UIBezierPath(rect: CGRectMake(110, 100, 150, 100))
```
```
let layer = CAShapeLayer()
```
```
layer.path = path.CGPath
```
```
layer.fillColor = UIColor.blackColor().CGColor
```
`view.layer.addSublayer(layer)`
```
要注意的是，这里就不要用backgroundColor 这个属性了，而要使用 fillColor 和 strokeColor ，前者代表设置这个 Layer 的填充色，后者代表设置它的边框色
```
```
layer.fillColor = UIColor.clearColor().CGCo
```
`lorlayer.strokeColor = UIColor.blackColor().CGColor`
```
可以试一下把上面代码设置颜色的部分改成这个样子，那么运行程序的时候就会是这种样子
![302.png](http://cc.cocimg.com/api/uploads/20160214/1455415491322317.png)
**玩一下UIBezierPath**
在说回 UIBezierPath ，在 UIBezierPath 的 init 方法中，就有很多方便你画各种图形的方法，比如你可以画一个带圆角的图形
```
```
let path = UIBezierPath(roundedRect: CGRectMake(110, 100, 150, 100), cornerRadius: 50)
```
```
let layer = CAShapeLayer()
```
```
layer.path = path.CGPath
```
```
layer.fillColor = UIColor.clearColor().CGColor
```
`layer.strokeColor = UIColor.blackColor().CGColor`
```
还可以指定起始角和半径画圆
```
```
let radius: CGFloat = 60.0
```
```
let startAngle: CGFloat = 0.0
```
```
let endAngle: CGFloat = CGFloat(M_PI * 2)
```
`let path = UIBezierPath(arcCenter: view.center, radius: radius, startAngle: startAngle, endAngle: endAngle, clockwise: ``true`
```
)
```
```
let layer = CAShapeLayer()
```
```
layer.path = path.CGPath
```
```
layer.fillColor = UIColor.clearColor().CGColor
```
`layer.strokeColor = UIColor.blackColor().CGColor`
```
![120336-0d0087ee3b02abc4.png](http://cc.cocimg.com/api/uploads/20160214/1455416011118810.png)
在这里涉及到角度的问题，起始角和结束角，这里的角度使使用弧度制来表示，这里我收藏了一张图片，以方便参考。
![120336-7f77dcb0cc28e0ef.png](http://cc.cocimg.com/api/uploads/20160214/1455416085288481.png)
**怎么画曲线**
贝塞尔曲线的画法是由起点、终点、控制点三个参数来画的，为了解释清楚这个点，我写了几行代码来解释它
```
```
let startPoint = CGPointMake(50, 300)
```
```
let endPoint = CGPointMake(300, 300)
```
```
let controlPoint = CGPointMake(170, 200)
```
` `
```
let layer1 = CALayer()
```
```
layer1.frame = CGRectMake(startPoint.x, startPoint.y, 5, 5)
```
```
layer1.backgroundColor = UIColor.redColor().CGColor
```
` `
```
let layer2 = CALayer()
```
```
layer2.frame = CGRectMake(endPoint.x, endPoint.y, 5, 5)
```
```
layer2.backgroundColor = UIColor.redColor().CGColor
```
` `
```
let layer3 = CALayer()
```
```
layer3.frame = CGRectMake(controlPoint.x, controlPoint.y, 5, 5)
```
```
layer3.backgroundColor = UIColor.redColor().CGColor
```
` `
```
let path = UIBezierPath()
```
```
let layer = CAShapeLayer()
```
` `
```
path.moveToPoint(startPoint)
```
```
path.addQuadCurveToPoint(endPoint, controlPoint: controlPoint)
```
` `
```
layer.path = path.CGPath
```
```
layer.fillColor = UIColor.clearColor().CGColor
```
```
layer.strokeColor = UIColor.blackColor().CGColor
```
` `
```
view.layer.addSublayer(layer)
```
```
view.layer.addSublayer(layer1)
```
```
view.layer.addSublayer(layer2)
```
`view.layer.addSublayer(layer3)`
```
我很随意的定义了三个点，为了清楚显示它们的位置，我放了三个矩形在上面以便观察，然后调用 path.moveToPoint(startPoint) 让它移动到起始点，然后调用path.addQuadCurveToPoint(endPoint, controlPoint: controlPoint) 这个方法告诉它结束点和控制点，这样它就能画出一条有弧度的线条了，如果把fillColor设置一个颜色，那么它就会变成一个很丑的形状了，示例图如下
![120336-0d292e7a1fab289b.png](http://cc.cocimg.com/api/uploads/20160214/1455416252865544.png)
控制点决定了它的曲率，曲线的顶点不等于控制点的位置，具体可以看一下贝塞尔曲线的定义，你还可以使用两个控制点来画，两个控制点可以使用方法 path.addCurveToPoint(endPoint, controlPoint1: controlPoint, controlPoint2: controlPoint2)来搞定
这样它会是这个样子
![120336-f19bd76f1caa0c35.png](http://cc.cocimg.com/api/uploads/20160214/1455416278817961.png)
**再来说说 CAShapeLayer**
CAShapeLayer 是个神奇的东西，给它一个path它就能变成你想要的形状，它还有很多可以玩的地方。综合使用可以组合成不同的动画，比如下面这样
![01.gif](http://cc.cocimg.com/api/uploads/20160214/1455416308234433.gif)
demo1.gif
![02.gif](http://cc.cocimg.com/api/uploads/20160214/1455416317370698.gif)
demo2.gif
![03.gif](http://cc.cocimg.com/api/uploads/20160214/1455416325292440.gif)
demo3.gif
这三个动画就是使用了 strokeEnd、strokeStart和lineWidth 三个属性，第一个动画用了strokeEnd这个属性的值范围是0-1，动画显示了从0到1之间每一个值对这条曲线的影响，strokeStart的方法则是相反的，如果把这两个值首先都设置成0.5然后慢慢改变成0和1的时候就会变成第二个动画，配合lineWidth则曲线会慢慢变粗，这里的很多属性都是支持动画的。
示例代码
```
```
private func animation1() {
```
`        ``let animation = CABasicAnimation(keyPath: ``"strokeEnd"`
```
)
```
`        `
```
animation.fromValue = 0
```
`        `
```
animation.toValue = 1
```
`        `
```
animation.duration = 2
```
`        ``layer.addAnimation(animation, forKey: ``""`
```
)
```
`    `
```
}
```
`     `
`    `
```
private func animation2() {
```
`        `
```
layer.strokeStart = 0.5
```
`        `
```
layer.strokeEnd = 0.5
```
`         `
`        ``let animation = CABasicAnimation(keyPath: ``"strokeStart"`
```
)
```
`        `
```
animation.fromValue = 0.5
```
`        `
```
animation.toValue = 0
```
`        `
```
animation.duration = 2
```
`         `
`        ``let animation2 = CABasicAnimation(keyPath: ``"strokeEnd"`
```
)
```
`        `
```
animation2.fromValue = 0.5
```
`        `
```
animation2.toValue = 1
```
`        `
```
animation2.duration = 2
```
`         `
`        ``layer.addAnimation(animation, forKey: ``""`
```
)
```
`        ``layer.addAnimation(animation2, forKey: ``""`
```
)
```
`    `
```
}
```
`     `
`    `
```
private func animation3() {
```
`        ``let animation = CABasicAnimation(keyPath: ``"lineWidth"`
```
)
```
`        `
```
animation.fromValue = 1
```
`        `
```
animation.toValue = 10
```
`        `
```
animation.duration = 2
```
`        ``layer.addAnimation(animation, forKey: ``""`
```
)
```
`    ``}`
```
**应用一下**
前一阵子在仿时光网这个APP，其中有一个Layer的形状很怪异，是这样的
![120336-7b526aaef0bbdc19.png](http://cc.cocimg.com/api/uploads/20160214/1455416445749925.png)
很明显它可以用 CAShapeLayer + UIBezierPath 来做，思路大概是这样，先移动到左上方的位置，然后向下划线，然后往右划线，然后往上划线，还剩一个盖子，这个盖子就用一个控制点控制曲率，非常简单，代码如下
```
```
let finalSize = CGSizeMake(CGRectGetWidth(view.frame), 400)
```
```
let layerHeight = finalSize.height * 0.2
```
```
let layer = CAShapeLayer()
```
```
let bezier = UIBezierPath()
```
```
bezier.moveToPoint(CGPointMake(0, finalSize.height - layerHeight))
```
```
bezier.addLineToPoint(CGPointMake(0, finalSize.height - 1))
```
```
bezier.addLineToPoint(CGPointMake(finalSize.width, finalSize.height - 1))
```
```
bezier.addLineToPoint(CGPointMake(finalSize.width, finalSize.height - layerHeight))
```
```
bezier.addQuadCurveToPoint(CGPointMake(0,finalSize.height - layerHeight),
```
`    `
```
controlPoint: CGPointMake(finalSize.width / 2, (finalSize.height - layerHeight) - 40))
```
```
layer.path = bezier.CGPath
```
```
layer.fillColor = UIColor.blackColor().CGColor
```
`view.layer.addSublayer(layer)`
```
就能画出这样的形状来
![120336-06410bfad53fb73e.png](http://cc.cocimg.com/api/uploads/20160214/1455416663688444.png)
再来一个复杂点的，微信下拉拍小视频的那只眼睛很有趣，来模仿一下那个效果吧，它是这样的
![120336-3c41264e40f49e68.gif](http://cc.cocimg.com/api/uploads/20160214/1455416677427704.gif)
首先你得画出这只眼睛，这是眼睛包括5个部分组成（这个是用OC写的）
```
```
@property (strong, nonatomic) CAShapeLayer *eyeFirstLightLayer;
```
```
@property (strong, nonatomic) CAShapeLayer *eyeSecondLightLayer;
```
```
@property (strong, nonatomic) CAShapeLayer *eyeballLayer;
```
```
@property (strong, nonatomic) CAShapeLayer *topEyesocketLayer;
```
`@property (strong, nonatomic) CAShapeLayer *bottomEyesocketLayer;`
```
然后，还是通过 UIBezierPath 和 CAShapeLayer 这样的老套路来画，代码较多
```
```
- (CAShapeLayer *)eyeFirstLightLayer {
```
`    ``if`
```
(!_eyeFirstLightLayer) {
```
`        `
```
_eyeFirstLightLayer = [CAShapeLayer layer];
```
`        `
```
CGPoint center = CGPointMake(CGRectGetWidth(self.frame) / 2, CGRectGetHeight(self.frame) / 2);
```
`        `
```
UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:center
```
`                                                            `
```
radius:CGRectGetWidth(self.frame) * 0.2
```
`                                                        `
```
startAngle:(230.f / 180.f) * M_PI
```
`                                                          `
```
endAngle:(265.f / 180.f) * M_PI
```
`                                                         `
```
clockwise:YES];
```
`        `
```
_eyeFirstLightLayer.borderColor = [UIColor blackColor].CGColor;
```
`        `
```
_eyeFirstLightLayer.lineWidth = 5.f;
```
`        `
```
_eyeFirstLightLayer.path = path.CGPath;
```
`        `
```
_eyeFirstLightLayer.fillColor = [UIColor clearColor].CGColor;
```
`        `
```
_eyeFirstLightLayer.strokeColor = [UIColor whiteColor].CGColor;
```
`    `
```
}
```
`    ``return`
```
_eyeFirstLightLayer;
```
`} `
` `
```
- (CAShapeLayer *)eyeSecondLightLayer {
```
`    ``if`
```
(!_eyeSecondLightLayer) {
```
`        `
```
_eyeSecondLightLayer = [CAShapeLayer layer];
```
`        `
```
CGPoint center = CGPointMake(CGRectGetWidth(self.frame) / 2, CGRectGetHeight(self.frame) / 2);
```
`        `
```
UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:center
```
`                                                            `
```
radius:CGRectGetWidth(self.frame) * 0.2
```
`                                                        `
```
startAngle:(211.f / 180.f) * M_PI
```
`                                                          `
```
endAngle:(220.f / 180.f) * M_PI
```
`                                                         `
```
clockwise:YES];
```
`        `
```
_eyeSecondLightLayer.borderColor = [UIColor blackColor].CGColor;
```
`        `
```
_eyeSecondLightLayer.lineWidth = 5.f;
```
`        `
```
_eyeSecondLightLayer.path = path.CGPath;
```
`        `
```
_eyeSecondLightLayer.fillColor = [UIColor clearColor].CGColor;
```
`        `
```
_eyeSecondLightLayer.strokeColor = [UIColor whiteColor].CGColor;
```
`    `
```
}
```
`    ``return`
```
_eyeSecondLightLayer;
```
`} `
` `
```
- (CAShapeLayer *)eyeballLayer {
```
`    ``if`
```
(!_eyeballLayer) {
```
`        `
```
_eyeballLayer = [CAShapeLayer layer];
```
`        `
```
CGPoint center = CGPointMake(CGRectGetWidth(self.frame) / 2, CGRectGetHeight(self.frame) / 2);
```
`        `
```
UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:center
```
`                                                            `
```
radius:CGRectGetWidth(self.frame) * 0.3
```
`                                                        `
```
startAngle:(0.f / 180.f) * M_PI
```
`                                                          `
```
endAngle:(360.f / 180.f) * M_PI
```
`                                                         `
```
clockwise:YES];
```
`        `
```
_eyeballLayer.borderColor = [UIColor blackColor].CGColor;
```
`        `
```
_eyeballLayer.lineWidth = 1.f;
```
`        `
```
_eyeballLayer.path = path.CGPath;
```
`        `
```
_eyeballLayer.fillColor = [UIColor clearColor].CGColor;
```
`        `
```
_eyeballLayer.strokeColor = [UIColor whiteColor].CGColor;
```
`        `
```
_eyeballLayer.anchorPoint = CGPointMake(0.5, 0.5);
```
`    `
```
}
```
`    ``return`
```
_eyeballLayer;
```
`} `
` `
```
- (CAShapeLayer *)topEyesocketLayer {
```
`    ``if`
```
(!_topEyesocketLayer) {
```
`        `
```
_topEyesocketLayer = [CAShapeLayer layer];
```
`        `
```
CGPoint center = CGPointMake(CGRectGetWidth(self.frame) / 2, CGRectGetHeight(self.frame) / 2);
```
`        `
```
UIBezierPath *path = [UIBezierPath bezierPath];
```
`        `
```
[path moveToPoint:CGPointMake(0, CGRectGetHeight(self.frame) / 2)];
```
`        `
```
[path addQuadCurveToPoint:CGPointMake(CGRectGetWidth(self.frame), CGRectGetHeight(self.frame) / 2)
```
`                     `
```
controlPoint:CGPointMake(CGRectGetWidth(self.frame) / 2, center.y - center.y - 20)];
```
`        `
```
_topEyesocketLayer.borderColor = [UIColor blackColor].CGColor;
```
`        `
```
_topEyesocketLayer.lineWidth = 1.f;
```
`        `
```
_topEyesocketLayer.path = path.CGPath;
```
`        `
```
_topEyesocketLayer.fillColor = [UIColor clearColor].CGColor;
```
`        `
```
_topEyesocketLayer.strokeColor = [UIColor whiteColor].CGColor;
```
`    `
```
}
```
`    ``return`
```
_topEyesocketLayer;
```
`} `
` `
```
- (CAShapeLayer *)bottomEyesocketLayer {
```
`    ``if`
```
(!_bottomEyesocketLayer) {
```
`        `
```
_bottomEyesocketLayer = [CAShapeLayer layer];
```
`        `
```
CGPoint center = CGPointMake(CGRectGetWidth(self.frame) / 2, CGRectGetHeight(self.frame) / 2);
```
`        `
```
UIBezierPath *path = [UIBezierPath bezierPath];
```
`        `
```
[path moveToPoint:CGPointMake(0, CGRectGetHeight(self.frame) / 2)];
```
`        `
```
[path addQuadCurveToPoint:CGPointMake(CGRectGetWidth(self.frame), CGRectGetHeight(self.frame) / 2)
```
`                     `
```
controlPoint:CGPointMake(CGRectGetWidth(self.frame) / 2, center.y + center.y + 20)];
```
`        `
```
_bottomEyesocketLayer.borderColor = [UIColor blackColor].CGColor;
```
`        `
```
_bottomEyesocketLayer.lineWidth = 1.f;
```
`        `
```
_bottomEyesocketLayer.path = path.CGPath;
```
`        `
```
_bottomEyesocketLayer.fillColor = [UIColor clearColor].CGColor;
```
`        `
```
_bottomEyesocketLayer.strokeColor = [UIColor whiteColor].CGColor;
```
`    `
```
}
```
`    ``return`
```
_bottomEyesocketLayer;
```
`}`
```
然后更改一下某些属性的值，方便稍后的动画
```
```
- (void)setupAnimation {
```
`    `
```
self.eyeFirstLightLayer.lineWidth = 0.f;
```
`    `
```
self.eyeSecondLightLayer.lineWidth = 0.f;
```
`    `
```
self.eyeballLayer.opacity = 0.f;
```
`    `
```
_bottomEyesocketLayer.strokeStart = 0.5f;
```
`    `
```
_bottomEyesocketLayer.strokeEnd = 0.5f;
```
`    `
```
_topEyesocketLayer.strokeStart = 0.5f;
```
`    ``_topEyesocketLayer.strokeEnd = 0.5f;}`
```
最后根据 UIScrollView 的 contentOffset 来控制各种属性，办法较笨，但管用。
![46.png](http://cc.cocimg.com/api/uploads/20160214/1455417075479068.png)
**最后**
总之使用 UIbezierPath 和 CAShapeLayer 可以画出你想要的任何形状，没有它做不到，只有你想不到，搞定了它们你就可以轻松定制你想要的任何控件了。
