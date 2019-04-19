# iOS中字迹动画效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月23日 09:26:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2519
> 
最近自己着手一款关于中国风的app，其中需要的一个想法就是诗词可以像ppt中的一种模式：字可以一个个的显示出来。最先的想法是将诗词分成一个个字放在一个个label上面，然后添加动画将其显示出来!然后这无疑是相当笨拙，且代码也是相当的丑陋！最后查阅了相关资料！终于找到了不错的解决方法！效果如下图所示
![](http://upload-images.jianshu.io/upload_images/1292550-cd6cb4d27a1f266e.gif?imageMogr2/auto-orient/strip)
s24.gif
- 废话不多说，直接上思路
### 如何将字迹给显示出来（重点）
> 
首先我们要获取字的轨迹，然后才能做出相应的操作，比如给其添加path，添加动画等等。 这里有一个自己添加不是系统的字体，我就大概的说一下了。首先下载好ttf格式的字体，添加到项目中来，在info.plist中 添加Fonts provided by application 然后在下面添加对应的字体。
#### 获取字体的轨迹
```
let paths = CGPathCreateMutable() //创建一个变量path添加到贝瑟尔曲线
   let fontName = __CFStringMakeConstantString("MFTongXin_Noncommercial-Regular")//自己添加的字体，（貌似没有用，不知道什么鬼，难倒不支持系统外的？求解！）可以换成系统的字体
     print("\(fontName)")
   let fontRef:AnyObject = CTFontCreateWithName(fontName, 25, nil)
   let attrString = NSAttributedString(string: string, attributes: [kCTFontAttributeName as String : fontRef])
   let line = CTLineCreateWithAttributedString(attrString as CFAttributedString)
   let runA = CTLineGetGlyphRuns(line)
```
- 
上面的runA获取了所有的字迹的line CFArrayGetCount(runA)获取一个数组。遍历数组 去除每个元素 （其中涉及到了一些与c桥接的内容 估计有的会比较陌生，慢慢看，还是很好懂的）
```
let run = CFArrayGetValueAtIndex(runA, runIndex);
    let runb = unsafeBitCast(run, CTRun.self)
    let  CTFontName = unsafeBitCast(kCTFontAttributeName, UnsafePointer<Void>.self)
    let runFontC = CFDictionaryGetValue(CTRunGetAttributes(runb),CTFontName)
    let runFontS = unsafeBitCast(runFontC, CTFont.self)
    let width = UIScreen.mainScreen().bounds.width
    var temp = 0
    var offset:CGFloat = 0.0
```
- 
unsafeBitCast是非常危险的操作，它会将一个指针指向的内存强制按位转换为目标的类型。因为这种转换是在Swift的类型管理之外进行的，因此编译器无法确保得到的类型是否确实正确，你必须明确地知道你在做什么。
#### 遍历每个字迹，在这边可以修改横竖的方向
```
//在这边修改应该可以修改横竖的方向
      for(var i = 0; i < CTRunGetGlyphCount(runb); i++){
          let range = CFRangeMake(i, 1)
          let glyph:UnsafeMutablePointer<CGGlyph> = UnsafeMutablePointer<CGGlyph>.alloc(1)//在Swift中不能像C里那样使用&符号直接获取地址来进行操作。如果我们想对某个变量进行指针操作，我们可以借助withUnsafePointer这个辅助方法。
          glyph.initialize(0)
          let position:UnsafeMutablePointer<CGPoint> = UnsafeMutablePointer<CGPoint>.alloc(1)
          position.initialize(CGPointZero)
          CTRunGetGlyphs(runb, range, glyph)
          CTRunGetPositions(runb, range, position);
          let temp3 = CGFloat(position.memory.x)
          let temp2 = (Int) (temp3 / width)
          let temp1 = 0
          if(temp2 > temp1){
              temp = temp2
              offset = position.memory.x - (CGFloat(temp) * width)
          }
          let path = CTFontCreatePathForGlyph(runFontS,glyph.memory,nil)
          let x = position.memory.x - (CGFloat(temp) * width) - offset
          let y = position.memory.y - (CGFloat(temp) * 80)
          var transform = CGAffineTransformMakeTranslation(x, y)
          CGPathAddPath(paths, &transform, path)
          glyph.destroy()
          glyph.dealloc(1)
          position.destroy()
          position.dealloc(1)  //销毁操作
      }
```
将paths添加到贝瑟尔曲线 
```
let bezierPath = UIBezierPath()
       bezierPath.moveToPoint(CGPointZero)
       bezierPath.appendPath(UIBezierPath(CGPath: paths))
```
- 好了最主要的事情已经OK了下面就可以为这个动画添加点酷炫的效果了，比如现在流行的渐变色。
`    private var gradientLayer = CAGradientLayer() //创建全局变量`
添加渐变色的layer和动画
```cpp
```cpp
1.添加颜色
        // 渐变色的颜色数
        let count = 10
        var colors:[CGColorRef] = []
        let topColor = UIColor(red: (91/255.0), green: (91/255.0), blue: (91/255.0), alpha: 1)
        let buttomColor = UIColor(red: (24/255.0), green: (24/255.0), blue: (24/255.0), alpha: 1)
        let gradientColors: [CGColor] = [topColor.CGColor, buttomColor.CGColor]
        for _ in 0 ..< count {
            let color = UIColor.init(red: CGFloat(arc4random() % 256) / 255, green: CGFloat(arc4random() % 256) / 255, blue: CGFloat(arc4random() % 256) / 255, alpha: 1.0)
            colors.append(color.CGColor)
        }
```
```
```cpp
```cpp
添加方向
        gradientLayer.startPoint = CGPoint(x: 0, y: 0.5)
        gradientLayer.endPoint = CGPoint(x: 1, y: 0.5)
        gradientLayer.colors = gradientColors
        gradientLayer.frame = self.bounds
        gradientLayer.type = kCAGradientLayerAxial
        self.layer.addSublayer(gradientLayer)
添加动画
        // 渐变色的动画
        let animation = CABasicAnimation(keyPath: "colors")
        animation.duration = 0.5
        animation.repeatCount = MAXFLOAT
        var toColors:[CGColorRef] = []
        for _ in 0 ..< count {
            let color = UIColor.init(red: CGFloat(arc4random() % 256) / 255, green: CGFloat(arc4random() % 256) / 255, blue: CGFloat(arc4random() % 256) / 255, alpha: 1.0)
            toColors.append(color.CGColor)
        }
        animation.autoreverses = true
        animation.toValue = toColors
        gradientLayer.addAnimation(animation, forKey: "gradientLayer")
```
```
最后将笔迹的path添加到layer上面去，将其以动画的形式显示出来 然后讲pathLayer添加到渐变色的layer的mask属性
`        gradientLayer.mask = pathLayer`
#### 最终在ViewController调用
```
```
这里用了一个延时的线程，里面的时间可以根据你动画的时间自己来算
        let viewPoem = PoemShow(frame: CGRectMake(0, 200, self.view.frame.size.width, 50), message: "楚城今近远，")
         view.addSubview(viewPoem)
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, Int64(5*Double(NSEC_PER_SEC))), dispatch_get_main_queue()) {
            let viewPoem1 = PoemShow(frame: CGRectMake(0, 250, self.view.frame.size.width, 50), message: "积霭寒塘暮。")
            self.view.addSubview(viewPoem1)
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, Int64(5*Double(NSEC_PER_SEC))), dispatch_get_main_queue()) {
                let viewPoem2 = PoemShow(frame: CGRectMake(0, 300, self.view.frame.size.width, 50), message: "水浅舟且迟，")
                self.view.addSubview(viewPoem2)
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, Int64(5*Double(NSEC_PER_SEC))), dispatch_get_main_queue()) {
                    let viewPoem3 = PoemShow(frame: CGRectMake(0, 350, self.view.frame.size.width, 50), message: "淮潮至何处。")
                    self.view.addSubview(viewPoem3)
                }
            }
        }
```
```
### 语言表达的可能有点不清楚，大家可以参考我的[Demo](https://github.com/Loissoul/PoemShow)，[Demo](https://github.com/Loissoul/PoemShow) readme中也有OC的相关解决方法,不足之处，请多多指出！如果有更好的解决方法，也要分享下哦。
文／那一片阳光（简书作者）
原文链接：http://www.jianshu.com/p/5d51f3522ca1
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
