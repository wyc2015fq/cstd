# iOS开发 关于照片截取内容旋转的问题 - 月若无涯的专栏 - CSDN博客
2013年08月13日 18:09:47[月若无涯](https://me.csdn.net/u010124617)阅读数：2358
在iOS开发中，遇到一个比较怪的问题，对读取出来的照片进行截取部分时。
使用的截取方法是比较常见的：
```
CGImageRef imageRef=CGImageCreateWithImageInRect([image CGImage],rect);
    UIImage *newImage=[UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
```
但是实际使用中，发现在截取相机拍摄的图片时，有时候截取的部分不是我们所指定的区域，截取出来的部分似乎是图片被旋转后再截取出来的。
这个问题比较奇怪，通过仔细的对比排查，我发现原来图片其实有一个一直以来基本被忽略的属性：
`@property(nonatomic,readonly) UIImageOrientation imageOrientation;`这个属性标记了图片的方向，而在实际使用时，通常我们使用UIImageView做图片显示时，并不用关心这个图片的方向，因为UIImageView继承自UIView的属性：
`@property(nonatomic) CGAffineTransform transform;   `能够自动的按照图片的朝向进行显示。
那么当我们在自己做想要的照片部分截取时，就需要对照片的朝向事先做好处理。才能避免出现截取区域不是我们想要的区域问题。
我处理了一个图片截取方法，供大家参考：
```
+(UIImage*)imageFromImage:(UIImage*)image inRect:(CGRect)rect transform:(CGAffineTransform)transform{
    CGSize newSize=rect.size;
    UIGraphicsBeginImageContext(newSize);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextTranslateCTM(context, newSize.width / 2, newSize.height / 2);
    CGContextConcatCTM(context, transform);
    CGContextTranslateCTM(context, newSize.width / -2, newSize.height / -2);
    [image drawInRect:CGRectMake(-rect.origin.x, -rect.origin.y, image.size.width, image.size.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}
```
其中的rect是相对与图片的像素尺寸（注意，不是iOS默认的点尺寸）。
其中的transform是在正常显示原图的ImageView的transform属性。比如orginImageView.transform。
好了这个分享出来，希望对大家有所帮助。
最后给出对一个照片截取的开源控件：[PEPhotoCropEditor](https://github.com/kishikawakatsumi/PEPhotoCropEditor)，做的比较好，大家可以去参考研究下。
转载请注明出处：来自月若无涯的博客：[http://blog.csdn.net/u010124617/article/details/9952275](http://blog.csdn.net/u010124617/article/details/9952275)  欢迎指正
