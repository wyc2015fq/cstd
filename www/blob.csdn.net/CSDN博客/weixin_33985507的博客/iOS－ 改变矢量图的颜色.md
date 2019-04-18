# iOS－ 改变矢量图的颜色 - weixin_33985507的博客 - CSDN博客
2015年09月08日 19:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
为啥要改变矢量图的颜色呢，直接叫设计师切个图不就好呢。
主要最近呢，设计师回家了，没办法只有一张图，我又懒的用ps。
就给自己弄了个课题。
其实也简单在ps中也不就是`选中选区`，`填充颜色`，然后`保存`。
在iOS也是差不多的。
```
//UIImage+ImageColor.h
#import <UIKit/UIKit.h>
@interface UIImage (ImageColor)
/**
 *  修改矢量图颜色
 *
 *  @param      maskColor 修改颜色
 *  @return
 */
- (UIImage *)imageMaskWithColor:(UIColor *)maskColor;
@end
```
```
//UIImage+ImageColor.m
#import "UIImage+ImageColor.h"
@implementation UIImage (ImageColor)
- (UIImage *)imageMaskWithColor:(UIColor *)maskColor {
    if (!maskColor) {
        return nil;
    }
    
    UIImage *newImage = nil;
    
    CGRect imageRect = (CGRect){CGPointZero,self.size};
    UIGraphicsBeginImageContextWithOptions(imageRect.size, NO, self.scale);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextTranslateCTM(context, 0.0, -(imageRect.size.height));
   
    CGContextClipToMask(context, imageRect, self.CGImage);//裁剪蒙版覆盖
    CGContextSetFillColorWithColor(context, maskColor.CGColor);//设置颜色
    CGContextFillRect(context, imageRect);//绘制
    
    newImage = UIGraphicsGetImageFromCurrentImageContext();//提取图片
    
    UIGraphicsEndImageContext();
    return newImage;
}
@end
```
总结：
设计师能给你切全套那是最好的，自己也省事。
不过嘛，这个是有好处滴，可以节省app大小，可以省去`按钮`的`高亮、选中`等等图片。
