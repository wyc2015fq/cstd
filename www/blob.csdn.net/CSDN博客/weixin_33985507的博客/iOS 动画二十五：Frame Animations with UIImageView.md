# iOS 动画二十五：Frame Animations with UIImageView - weixin_33985507的博客 - CSDN博客
2018年10月20日 11:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
###### Frame Animations 设置
```
penguin.animationImages = walkFrames 
penguin.animationDuration = animationDuration / 3 
penguin.animationRepeatCount = 3
```
- 
animationImages: 存储帧动画的所有帧图像。
walkFrames 在代码中作为包含 UIImage对象的数组在前面已声明，所以这里我们只需要将它分配给 penguin.animationImages 来设置动画帧。
- 
animationDuration: 告诉 UIKit 动画的一次迭代应该持续多长时间; 因为动画将重复三次，所以将其设置为总 animationDuration 时长的三分之一。
- 
animationRepeatCount: 控制动画的重复次数; 在这种情况下，动画将重复三次。
```
var isLookingRight: Bool = true { 
  didSet { 
       let xScale: CGFloat = isLookingRight ? 1 : -1 
       penguin.transform = CGAffineTransform(scaleX: xScale, y: 1)
       slideButton.transform = penguin.transform
    } 
}
```
此代码将企鹅和滑动按钮的 x 轴刻度设置为 1 或 -1，具体取决于 isLookingRight 的值。 然后设置该变换将触发视图，以便企鹅可以面向正确的方向运动：
![130752-c8392d01059c494f.png](https://upload-images.jianshu.io/upload_images/130752-c8392d01059c494f.png)
在方法 actionLeft() 底部执行动画:
```
penguin.startAnimating()
```
当调用startAnimating() 时，播放动画：animationImages 数组中的每个帧在 1 秒内总共显示三次。
###### Translating your view
```
UIView.animate(withDuration: animationDuration, delay: 0, 
         options: .curveEaseOut,
         animations: { 
               self.penguin.center.x -= self.walkSize.width
         }, completion: nil )
```
设置企鹅每次移动距离。
###### Playing different frame animations
最终效果图：
![130752-4728daf4ce990023.gif](https://upload-images.jianshu.io/upload_images/130752-4728daf4ce990023.gif)
UIImageView frame animation
[demo下载](https://github.com/imJoeychang/SouthPoleFun.git)
