# 从UIImage的矩阵变换看矩阵运算的原理 - game3108的专栏 - CSDN博客
2015年09月07日 14:59:56[game3108](https://me.csdn.net/game3108)阅读数：310
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
转载地址：http://blog.csdn.net/likendsl/article/details/7595611
**1.矩阵的基本知识：**
**struct CGAffineTransform**
{
  CGFloat a, b, c, d;
  CGFloat tx, ty;
};
**CGAffineTransform CGAffineTransformMake **(CGFloat a,CGFloat b,CGFloat c,CGFloat d,CGFloat tx,CGFloat ty);
为了把二维图形的变化统一在一个坐标系里，引入了齐次坐标的概念，即把一个图形用一个三维矩阵表示，其中第三列总是(0,0,1)，用来作为坐标系的标准。**所以所有的变化都由前两列完成。**
以上参数在矩阵中的表示为：
 |a    b    0|
 |c    d    0|
 |tx   ty   1|
运算原理：原坐标设为（X,Y,1）;
                            |a    b    0|
       [X，Y,  1]      |c    d    0|     =     [aX + cY + tx   bX + dY + ty  1] ;
                            |tx    ty  1|
通过矩阵运算后的坐标[aX + cY + tx   bX + dY + ty  1]，我们对比一下可知：
第一种：**设a=d=1, b=c=0.  **
**[aX + cY + tx   bX + dY + ty  1] = [X  + tx  Y + ty  1];**
可见，这个时候，坐标是按照向量（tx，ty）进行平移，其实这也就是函数
**CGAffineTransform CGAffineMakeTranslation(CGFloat tx,CGFloat ty)的计算原理。**
第二种：**设b=c=tx=ty=0.  **
**[aX + cY + tx   bX + dY + ty  1] = [aX    dY   1];**
可见，这个时候，坐标X按照a进行缩放，Y按照d进行缩放，a，d就是X，Y的比例系数，其实这也就是函数
**CGAffineTransform CGAffineTransformMakeScale(CGFloat sx, CGFloat sy)的计算原理。a对应于sx，d对应于sy。**
第三种：**设tx=ty=0，a=cosɵ，b=sinɵ，c=-sinɵ，d=cosɵ。**
**[aX + cY + tx   bX + dY + ty  1] = [Xcosɵ - Ysinɵ    Xsinɵ + Ycosɵ  1] ;**
可见，这个时候，ɵ就是旋转的角度，逆时针为正，顺时针为负。其实这也就是函数
**CGAffineTransform CGAffineTransformMakeRotation(CGFloat angle)的计算原理。angle即ɵ的弧度表示。**
**2.利用上面的变换写一个UIImage矩阵变换的例子：**
下面是一个关于image的矩阵运算的例子,无外乎是运用以上三种变换的组合，达到所定义的效果：
**[cpp]**[view
 plain](http://blog.csdn.net/likendsl/article/details/7595611#)[copy](http://blog.csdn.net/likendsl/article/details/7595611#)[print](http://blog.csdn.net/likendsl/article/details/7595611#)[?](http://blog.csdn.net/likendsl/article/details/7595611#)
- //UIImageOrientation的定义，定义了如下几种变换
- typedefenum
- {  
-     UIImageOrientationUp,            // default orientation
- 
-     UIImageOrientationDown,          // 180 deg rotation
- 
-     UIImageOrientationLeft,          // 90 deg CCW
- 
-     UIImageOrientationRight,         // 90 deg CW
- 
-     UIImageOrientationUpMirrored,    // as above but image mirrored along other axis. horizontal flip
- 
-     UIImageOrientationDownMirrored,  // horizontal flip
- 
-     UIImageOrientationLeftMirrored,  // vertical flip
- 
-     UIImageOrientationRightMirrored, // vertical flip
- 
- } UIImageOrientation;  
- 
- //按照UIImageOrientation的定义，利用矩阵自定义实现对应的变换；
- 
- -(UIImage *)transformImage:(UIImage *)aImage    
- 
- {    
- 
-     CGImageRef imgRef = aImage.CGImage;    
- 
-     CGFloat width = CGImageGetWidth(imgRef);    
- 
-     CGFloat height = CGImageGetHeight(imgRef);    
- 
-     CGAffineTransform transform = CGAffineTransformIdentity;    
- 
-     CGRect bounds = CGRectMake(0, 0, width, height);    
- 
-     CGFloat scaleRatio = 1;    
- 
-     CGFloat boundHeight;    
- 
-     UIImageOrientation orient = aImage.imageOrientation;    
- 
- switch(UIImageOrientationLeftMirrored)    
- 
-     {    
- 
- case UIImageOrientationUp:    
- 
-             transform = CGAffineTransformIdentity;  
- 
- break;    
- 
- case UIImageOrientationUpMirrored:   
- 
-             transform = CGAffineTransformMakeTranslation(width, 0.0);    
- 
-             transform = CGAffineTransformScale(transform, -1.0, 1.0);    
- 
- break;    
- 
- case UIImageOrientationDown:              
-             transform = CGAffineTransformMakeTranslation(width, height);    
- 
-             transform = CGAffineTransformRotate(transform, M_PI);    
- 
- break;    
- 
- case UIImageOrientationDownMirrored:   
- 
-             transform = CGAffineTransformMakeTranslation(0.0, height);    
- 
-             transform = CGAffineTransformScale(transform, 1.0, -1.0);    
- 
- break;    
- 
- case UIImageOrientationLeft:     
- 
-             boundHeight = bounds.size.height;    
- 
-             bounds.size.height = bounds.size.width;    
- 
-             bounds.size.width = boundHeight;    
- 
-             transform = CGAffineTransformMakeTranslation(0.0, width);    
- 
-             transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);    
- 
- break;  
- 
- case UIImageOrientationLeftMirrored:     
- 
-             boundHeight = bounds.size.height;    
- 
-             bounds.size.height = bounds.size.width;    
- 
-             bounds.size.width = boundHeight;    
- 
-             transform = CGAffineTransformMakeTranslation(height, width);    
- 
-             transform = CGAffineTransformScale(transform, -1.0, 1.0);    
- 
-             transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);    
- 
- break;    
- 
- case UIImageOrientationRight: //EXIF = 8  
- 
-             boundHeight = bounds.size.height;    
- 
-             bounds.size.height = bounds.size.width;    
- 
-             bounds.size.width = boundHeight;    
- 
-             transform = CGAffineTransformMakeTranslation(height, 0.0);    
- 
-             transform = CGAffineTransformRotate(transform, M_PI / 2.0);    
- 
- break;  
- 
- case UIImageOrientationRightMirrored:   
- 
-             boundHeight = bounds.size.height;    
- 
-             bounds.size.height = bounds.size.width;    
- 
-             bounds.size.width = boundHeight;    
- 
-             transform = CGAffineTransformMakeScale(-1.0, 1.0);    
- 
-             transform = CGAffineTransformRotate(transform, M_PI / 2.0);    
- 
- break;    
- 
- default:    
- 
-             [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];    
- 
-     }    
- 
-     UIGraphicsBeginImageContext(bounds.size);    
- 
-     CGContextRef context = UIGraphicsGetCurrentContext();    
- 
- if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {    
- 
-         CGContextScaleCTM(context, -scaleRatio, scaleRatio);    
- 
-         CGContextTranslateCTM(context, -height, 0);    
- 
-     }    
- 
- else {    
- 
-         CGContextScaleCTM(context, scaleRatio, -scaleRatio);    
- 
-         CGContextTranslateCTM(context, 0, -height);    
- 
-     }    
- 
-     CGContextConcatCTM(context, transform);    
- 
-     CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);    
- 
-     UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();  
- 
-     UIGraphicsEndImageContext();    
- 
- return imageCopy;    
- 
- }   
掌握矩阵运算的原理，对视图的矩阵操作便会得心应手，巧妙利用旋转，平移，缩放，组合起来达到你所想要的变换效果！
