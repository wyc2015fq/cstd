# 分享iOS中常用的绘图, 截屏方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月07日 17:22:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2763

- 
设置某些角为圆角, 使用UIBezierPath
```
let layer = CAShapeLayer()
     // 这里byRoundingCorners -> 指定圆角
     layer.path = UIBezierPath(roundedRect: testView.bounds, byRoundingCorners: [UIRectCorner.TopLeft, .TopRight], cornerRadii: CGSize(width: 20, height: 20)).CGPath
     testView.layer.mask = layer
```
- 
设置view的四个角都为圆角, 最简单的方法, 使用当然最简单的某些情况下肯定就是不适合的, 因为这个方法会触发离屏渲染, 如果界面中有大量的圆角view的处理, 会出现掉帧的"现象"
```bash
testView.layer.cornerRadius = 20.0
     testView.layer.masksToBounds = true
```
- 
缩放图片
- 
3.1
```
// 缩放图片
func resizeImage(image: UIImage, toWidth width: CGFloat, withHeight height: CGFloat) -> UIImage {
   let size = CGSize(width: width, height: height)
   // 开启一个上下文
   /// 设置为false 表示背景不透明 -> 黑色
   /// 设置为 0 -> 可以自适应不同的屏幕的scale显示
   UIGraphicsBeginImageContextWithOptions(size, false, 0)
   // 获取上下文
   let context = UIGraphicsGetCurrentContext()
   // 转换坐标原点到左下角
   CGContextTranslateCTM(context, 0.0, height)
   CGContextScaleCTM(context, 1.0, -1.0)
   // 设置绘制模式
   CGContextSetBlendMode(context, .Copy)
   // 绘图
   CGContextDrawImage(context, CGRect(origin: CGPointZero, size: size), image.CGImage)
   // 取得图片
   let retrivedImage = UIGraphicsGetImageFromCurrentImageContext()
   // 结束上下文
   UIGraphicsEndImageContext()
   return retrivedImage
}
```
- 
3.2
```
// 缩放图片
func resizeImage(image: UIImage, toWidth width: CGFloat, withHeight height: CGFloat) -> UIImage {
   let size = CGSize(width: width, height: height)
   // 开启一个上下文
   /// 设置为false 表示背景不透明 -> 黑色
   /// 设置为 0 -> 可以自适应不同的屏幕的scale显示
   UIGraphicsBeginImageContextWithOptions(size, false, 0)
   /// 绘制图片
   image.drawInRect(CGRect(origin: CGPointZero, size: size))
   // 取得图片
   let retrivedImage = UIGraphicsGetImageFromCurrentImageContext()
   // 结束上下文
   UIGraphicsEndImageContext()
   return retrivedImage
}
```
- 
截屏功能
- 
4.1
```
func snapView(targetView: UIView) -> UIImage {
   UIGraphicsBeginImageContextWithOptions(targetView.bounds.size, false, 0)
   // iOS7.0 之后系统提供的截屏的功能
   targetView.drawViewHierarchyInRect(targetView.bounds, afterScreenUpdates: false)
   let snapdImage = UIGraphicsGetImageFromCurrentImageContext()
   UIGraphicsEndImageContext()
   return snapdImage
}
```
- 
4.2
```
//这个方法会返回一个"截屏"后的view
// withCapInsets  -> 这个和拉伸图片的image.resizableImageWithCapInsets()使用一样
  let snapShotView  = view.resizableSnapshotViewFromRect(view.bounds, afterScreenUpdates: false, withCapInsets: UIEdgeInsetsZero)
```
- 4.3
```
func snapView(targetView: UIView) -> UIImage {
             UIGraphicsBeginImageContextWithOptions(targetView.bounds.size, false, 0.0)
// 区分和4.1中的使用
   targetView.layer.renderInContext(UIGraphicsGetCurrentContext()!)
     let image = UIGraphicsGetImageFromCurrentImageContext()
     UIGraphicsEndImageContext()
     return image
 }
```
- 
剪裁图片的指定部分
- 
5.1
```
func cutImage(image: UIImage, cutRect: CGRect) -> UIImage {
   UIGraphicsBeginImageContextWithOptions(cutRect.size, false, 0.0)
   // 获取上下文
   let context = UIGraphicsGetCurrentContext()
   // 转换坐标原点到左下角
   CGContextTranslateCTM(context, 0.0, cutRect.size.height)
   CGContextScaleCTM(context, 1.0, -1.0)
   // 设置绘制模式
   CGContextSetBlendMode(context, .Copy)
   // 绘图
   CGContextDrawImage(context, cutRect, image.CGImage)
   let image = UIGraphicsGetImageFromCurrentImageContext()
   UIGraphicsEndImageContext()
   return image
}
```
- 
5.2
```
func cutImage(image: UIImage, cutRect: CGRect) -> UIImage {
   UIGraphicsBeginImageContextWithOptions(cutRect.size, false, 0.0)
   // 获取上下文
   let context = UIGraphicsGetCurrentContext()
   image.drawInRect(cutRect)
   let image = UIGraphicsGetImageFromCurrentImageContext()
   UIGraphicsEndImageContext()
   return image
}
```
- 
将颜色绘制为图片
- 
6.1
```
func imageFromColor(color: UIColor, size: CGSize) -> UIImage {
   UIGraphicsBeginImageContextWithOptions(size, false, 0.0)
   let rect = CGRect(origin: CGPointZero, size: size)
   color.setFill()
   UIRectFill(rect)
   let image = UIGraphicsGetImageFromCurrentImageContext()
   UIGraphicsEndImageContext()
   return image
}
```
- 
6.2
```
func imageFromColor(color: UIColor, size: CGSize) -> UIImage {
   UIGraphicsBeginImageContextWithOptions(size, false, 0.0)
   let rect = CGRect(origin: CGPointZero, size: size)
//        color.setFill()
//        UIRectFill(rect)
   let context = UIGraphicsGetCurrentContext()
   CGContextSetFillColorWithColor(context, color.CGColor)
   CGContextFillRect(context, rect)
   let image = UIGraphicsGetImageFromCurrentImageContext()
   UIGraphicsEndImageContext()
   return image
}
```
#### 如果你觉得平时很少接触过Quartzs 2D来绘图, 可能看上面的代码不是很明白, 如果想要更清楚, 建议阅读相关的绘图教程, 同时下一篇可能会简单分享一下使用Quartzs 2D来绘图的相关知识, 欢迎关注

文／ZeroJ（简书作者）
原文链接：http://www.jianshu.com/p/524454440bea
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
