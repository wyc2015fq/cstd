# iOS给一张图片添加带有雷达效果的标记，类似地图标记位置 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月19日 10:58:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：73
引言
©️本文首发个人博客
- 
[一之笔](https://yizibi.github.io/)
- 
[iOS给一张图片添加带有雷达效果的标记，类似地图标记位置](https://yizibi.github.io/2018/09/10/iOS%E7%BB%99%E4%B8%80%E5%BC%A0%E5%9B%BE%E7%89%87%E6%B7%BB%E5%8A%A0%E5%B8%A6%E6%9C%89%E9%9B%B7%E8%BE%BE%E6%95%88%E6%9E%9C%E7%9A%84%E6%A0%87%E8%AE%B0-%E7%B1%BB%E4%BC%BC%E5%9C%B0%E5%9B%BE%E6%A0%87%E8%AE%B0%E4%BD%8D%E7%BD%AE/)
> 
在实际开发过程中，往往有这样一种场景，比如，某一个地方需要添加一个标记，这个标记要足够醒目，一眼就可以让用户知道，标记的地方在哪里，就像百度地图或者谷歌地图里面的，用来标记用户位置的标记，以一种“雷达”效果，或者说“水波”效果，动态闪烁，引起用户的注意，并且，背景图片是可以放大的；
以上就是我们产品的需求，以下就是最终的实现效果；
![](https://upload-images.jianshu.io/upload_images/1360502-778bd79e7db08509.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
报警点展示
> 
[点击获取Demo](https://github.com/lucyios/CustomeUILib)
### 可行性
- 在一张图片上添加一个`雷达效果`的一个标记，首先需要给图片添加标记，然后想办法在标记的地方(Point)，add 一个View;可行
### 需求分析
- 在一张已知的背景图上画一个标记，我首先想到的是，图片水印；
事实也就是那么回事，水印是在原有的图片添加另外一张图片，标记仅仅是一个纯色的图片而已
- 在标记的位置点，添加一个显示动画的View；
### 技术点
- 添加标记
根据颜色生成一张图片，然后将生成的图片，绘制到背景图片上,具体需要用到
> 
drawInrect：在哪里绘制图片
- 关键代码
```
+ (instancetype)imageWithColor:(UIColor *)color
{
    CGRect rect = CGRectMake(0.0f, 0.0f, 10.0f, 10.0f);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);
    UIImage *theImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return theImage;
}
// 给图片添加标记
+ (UIImage *)imageWithOriginalName:(UIImage *)image signColor:(UIColor *)signColor signPositionX:(double)positionX ignPositionY:(double)positionY {
    
    //1.获取图片
    UIImage *signalImage = [[UIImage imageWithColor:signColor] circleImag];
    //2.开启上下文
    UIGraphicsBeginImageContextWithOptions(image.size, NO, 0);
    //3.绘制背景图片
    [image drawInRect:CGRectMake(0, 0, image.size.width, image.size.height)];
    //绘制标记图片到当前上下文
    CGFloat signX = positionX * image.size.width;
    CGFloat signY = positionY * image.size.height;
    
    CGRect rect = CGRectMake(signX, signY, 40, 40);
    [signalImage drawInRect:rect];
    //4.从上下文中获取新图片
    UIImage * newImage = UIGraphicsGetImageFromCurrentImageContext();
    //5.关闭图形上下文
    UIGraphicsEndImageContext();
    //返回图片
    return newImage;
}
```
- 定点位置，雷达动画的添加
具体实现可以参考文章，基本原理就是，将单一动画利用循环，创建多次，组成动画组，依次添加，就可以实现最终的效果了；
[感谢]
[iOS动画-扩散波纹效果](http://www.cocoachina.com/ios/20180327/22778.html)
[图片预览库WMPhotoBrowser](https://github.com/zhengwenming/WMPhotoBrowser)
作者：一之笔
链接：https://www.jianshu.com/p/53a4bcc33217
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
