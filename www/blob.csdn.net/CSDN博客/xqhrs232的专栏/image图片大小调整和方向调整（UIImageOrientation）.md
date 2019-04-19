# image图片大小调整和方向调整（UIImageOrientation） - xqhrs232的专栏 - CSDN博客
2019年03月12日 17:24:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：16
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/weixin_39339407/article/details/81162726](https://blog.csdn.net/weixin_39339407/article/details/81162726)
相关文章
1、iOS image旋转90,180,270度的方法----[https://blog.csdn.net/bitcser/article/details/52055442](https://blog.csdn.net/bitcser/article/details/52055442)
2、UIImage图片处理，旋转、截取、平铺、缩放等操作，持续更新中----[https://www.jianshu.com/p/9ab1205f5166](https://www.jianshu.com/p/9ab1205f5166)
3、UIImage扩展方法（category），支持放大和旋转----[https://blog.csdn.net/zhibudefeng/article/details/7689824](https://blog.csdn.net/zhibudefeng/article/details/7689824)
图片大小调整：
此处是写在UIImage分类中的方法self即为image，size为自己传入的大小，根据size生成图片图片不会被剪裁，但可能会被拉伸变形。
- (UIImage *)thumWithSize:(CGSize)size{
    UIGraphicsBeginImageContext(size);
    [self drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage * newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}
图片方向调整：
利用已经定义好的宏定义
typedef NS_ENUM(NSInteger, UIImageOrientation) {
    UIImageOrientationUp,            // default orientation
    UIImageOrientationDown,          // 180 deg rotation
    UIImageOrientationLeft,          // 90 deg CCW
    UIImageOrientationRight,         // 90 deg CW
    UIImageOrientationUpMirrored,    // as above but image mirrored along other axis. horizontal flip
    UIImageOrientationDownMirrored,  // horizontal flip
    UIImageOrientationLeftMirrored,  // vertical flip
    UIImageOrientationRightMirrored, // vertical flip
};
以上是UIImage自带的宏定义（下面介绍没有按照以上顺序）
UIImageOrientationUp：表示图片的正常状态
UIImageOrientationUpMirrored：表示图片的正常状态的镜面图像
UIImageOrientationDown：表示图片被翻转180°
UIImageOrientationDownMirrored：表示图片被翻转180°后的镜面图像
UIImageOrientationLeft：表示图片被逆时针翻转90°
UIImageOrientationLeftMirrored：表示图片被逆时针翻转90°后的镜面图像
UIImageOrientationRight：表示图片被顺时针翻转90°
UIImageOrientationRightMirrored：表示图片被顺时针翻转90°后的镜面图像
如图：
介绍两种图片位置和方向调整的方法（都是对坐标系的调整）
第一种：
(平移)正数xy变大的方向 负数xy缩小的方向
CGAffineTransformTranslate
CGAffineTransformMakeTranslation
（旋转）正数顺时针 负数逆时针
CGAffineTransformRotate
CGAffineTransformMakeRotation
（缩放）当值为负数的时候表示翻转，x为-1则沿Y轴翻转
CGAffineTransformScale
CGAffineTransformMakeScale
CGAffineTransform transform = CGAffineTransformIdentity;
transform = CGAffineTransformMakeTranslation(20, 20);
等同于
transform = CGAffineTransformTranslate(transform, 20, 20);
 其它的旋转，和缩放操作也是一样。
但如果需要连续操作
 如：先平移后旋转
transform = CGAffineTransformMakeTranslation(self.size.width, self.size.height);//平移
 transform = CGAffineTransformRotate(transform, M_PI/9);//旋转
则第二个需要采用用没有make的形式，表示在第一个操作的基础上进行第一个操作，如果连续使用带make的形式则只会保留最后一个操作。
 另外推荐最后进行旋转操作 优先进行平移操作，方便坐标系判断。
第二种：
CGContextTranslateCTM(平移)
CGContextRotateCTM（旋转）
CGContextScaleCTM（缩放）
对图片的操作与第一种相似，只是参数不同，使用过程中会看见区别。
图片方向调整讲解：
  UIImageView * imageView = [self.view viewWithTag:100 + 1];
    UIImage * image1 = imageView.image;
    CGImageRef  cgimage = image1.CGImage;
    UIGraphicsBeginImageContext(image1.size);
    CGContextDrawImage(UIGraphicsGetCurrentContext(),CGRectMake(0, 0, image1.size.width, image1.size.height), cgimage);//绘制图片
    UIImage * image2 = UIGraphicsGetImageFromCurrentImageContext();//取出图片
    UIGraphicsEndImageContext();
    imageView.image = image2;
imageView为已经创建好的UIImageView，这里重新绘制，再没有进行任何
CGAffineTransformTranslate或者是CGContextTranslateCTM等操作的情况下，图片将是这样的（后者）：
所以实际上我们对图片的方向的修改是对后面这张图片进行操作，
以UIImageOrientationUpMirrored为例：
（self即为image）
CGAffineTransform transform = CGAffineTransformIdentity;
CGSize size = self.size;
transform = CGAffineTransformMakeTranslation(size.width, size.height);//平移
transform = CGAffineTransformRotate(transform, M_PI);//顺时针旋转180度
UIGraphicsBeginImageContext(size);
CGContextRef contextRef = UIGraphicsGetCurrentContext();
CGContextConcatCTM(contextRef, transform);
CGContextDrawImage(UIGraphicsGetCurrentContext(),CGRectMake(0, 0, self.size.width, self.size.height), self.CGImage);//绘制图片
UIImage * image = UIGraphicsGetImageFromCurrentImageContext();//取出图片
UIGraphicsEndImageContext();
关键代码
transform = CGAffineTransformMakeTranslation(size.width, size.height);//平移
transform = CGAffineTransformRotate(transform, M_PI);//顺时针旋转180度
将图像（后者图像）进行平移平移后旋转180°，可以用一个长方形的书本进行操作执行完之后应该得到这样一个图像（后者）：
所以图片方向的调整只需利用其中一种方法便可实现，但是有些方向使用一种方法，可能需要多个步骤，而坐标系在被进行非平移（如旋转，翻转）等操作后不容易判断x 轴 y轴方向，所以需要利用两个方法进行更改。（第一种方法引起的坐标系的变化不会影响第二种方法中使用的坐标系）
如：UIImageOrientationLeft和UIImageOrientationRight，都是两步达不到目的的。
以UIImageOrientationLeft为例：
将图片： 
进行第一步操作，由于图片需要被放倒所以要交换图片的宽高。
（self即为image）
CGAffineTransform transform = CGAffineTransformIdentity;
CGSize size = self.size;
size = CGSizeMake(size.height, size.width);
transform = CGAffineTransformMakeTranslation(size.width,0);//平移
transform = CGAffineTransformRotate(transform, M_PI/2);
还是可以利用书本比对着进行操作 平移加上顺时针旋转90度后图片应该是这个样子：
之后将此图片进行平移 和 翻转
UIGraphicsBeginImageContext(size);
CGContextRef contextRef = UIGraphicsGetCurrentContext();
CGContextTranslateCTM(contextRef,0.0,size.height);
CGContextScaleCTM(contextRef, 1.0, -1.0);
CGContextConcatCTM(contextRef, transform);//
CGContextDrawImage(UIGraphicsGetCurrentContext(),CGRectMake(0, 0, self.size.width, self.size.height), self.CGImage);//绘制图片
UIImage * image = UIGraphicsGetImageFromCurrentImageContext();//取出图片
UIGraphicsEndImageContext();
则可得到目标图像：
更多方向及具体操作可以在Demo中查看
CSDN下载地址
GitHub下载地址
