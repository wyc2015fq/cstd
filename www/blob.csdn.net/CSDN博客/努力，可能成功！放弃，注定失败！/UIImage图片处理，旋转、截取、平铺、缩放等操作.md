# UIImage图片处理，旋转、截取、平铺、缩放等操作 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月12日 09:33:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：310标签：[UIImage图片处理																[iOS 资源大全																[ios开发																[iOS学习资料](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=UIImage图片处理&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
本文来自简书原文地址:[http://www.jianshu.com/p/9ab1205f5166](http://www.jianshu.com/p/9ab1205f5166)
有时候我们需要处理图片，比如改变大小，旋转，截取等等，所以今天说一说图片处理相关的一些操作。
本文所说的方法都是写在UIImage的Category中，这样使用起来也方便；由于代码太多，这里就不贴具体实现代码了，大家可以去我的Github查看[demo](https://github.com/lisongrc/UIImage-Categories)，效果如下：
![](http://upload-images.jianshu.io/upload_images/1608265-5023a6afa907f819.gif?imageMogr2/auto-orient/strip)
demo效果.gif
#### 颜色相关
1.根据颜色生成纯色图片
就是根据制定的颜色生成一张纯色的图片
`+ (UIImage *)imageWithColor:(UIColor *)color;`
使用方法，比如设置UIImageView的图片为红色纯图片：
`self.imageView.image = [UIImage imageWithColor:[UIColor redColor]];`
2.取图片上某一像素的颜色
有时候我们需要获取图片上的某一点的颜色，比如画板应用选择画笔颜色的时候，其实是在一张有所有颜色的图片上点击选择实现的。
需要注意的是这里要传的参数point是相对于图片上的点。
`- (UIColor *)colorAtPixel:(CGPoint)point;`
使用方法，比如我们在图片上加个tap手势，然后在响应方法里面这样写就可以了：
```
- (void)handleTap:(UITapGestureRecognizer *)tap
{
    CGPoint point = [tap locationInView:tap.view];
    UIImage *image = self.imageView.image;
    CGPoint pointInImage = CGPointMake(point.x * image.size.width / self.imageView.frame.size.width, point.y * image.size.height / self.imageView.frame.size.height);
    self.view.backgroundColor = [image colorAtPixel:pointInImage];
}
```
3.获得灰度图 
获取一张彩色图片的黑白图片
`- (UIImage *)convertToGrayImage;`
使用方法：
`self.imageView.image = [image convertToGrayImage];`
#### 旋转相关
1.纠正图片的方向
当我们需要读取相册的图片，发现相册里面的方向和展示出来的图片的方向不一样，这时候就要矫正方向了。
`- (UIImage *)fixOrientation;`
使用:
`self.imageView.image = [image fixOrientation];`
2.按给定的方向旋转图片
在做图片处理工具的时候，我们可能需要旋转图片。
这个方法的参数是系统枚举UIImageOrientation。
```
typedef NS_ENUM(NSInteger, UIImageOrientation) {
    UIImageOrientationUp,            // default orientation
    UIImageOrientationDown,          // 180 deg rotation
    UIImageOrientationLeft,          // 90 deg CCW
    UIImageOrientationRight,         // 90 deg CW
    UIImageOrientationUpMirrored,    // as above but image mirrored along other axis. horizontal flip
    UIImageOrientationDownMirrored,  // horizontal flip
    UIImageOrientationLeftMirrored,  // vertical flip
    UIImageOrientationRightMirrored, // vertical flip
};
- (UIImage*)rotate:(UIImageOrientation)orient;
```
使用，比如顺时针旋转180度：
`self.imageView.image = [image rotate:UIImageOrientationDown];`
3.垂直翻转
其实就是上面的方法传UIImageOrientationDownMirrored参数。
`- (UIImage *)flipVertical;`
4.水平翻转
其实就是传UIImageOrientationUpMirrored参数。
`- (UIImage *)flipHorizontal;`
5.将图片旋转degrees角度
传入一个自定义的角度。
`- (UIImage *)imageRotatedByDegrees:(CGFloat)degrees;`
6.将图片旋转radians弧度
`- (UIImage *)imageRotatedByRadians:(CGFloat)radians;`
#### 生成图相关
1.截取image对象rect区域内的图像
`- (UIImage *)subImageWithRect:(CGRect)rect;`
2.压缩图片至指定尺寸
`- (UIImage *)rescaleImageToSize:(CGSize)size;`
3.压缩图片至指定像素
`- (UIImage *)rescaleImageToPX:(CGFloat )toPX;`
4.生成一个size大小的平铺图片
`- (UIImage *)getTiledImageWithSize:(CGSize)size;`
5..UIView转化为UIImage
`+ (UIImage *)imageFromView:(UIView *)view;`
6.将两个图片生成一张图片
firstImage在下面，secondImage在上面
`+ (UIImage*)mergeImage:(UIImage*)firstImage withImage:(UIImage*)secondImage;`
#### Gif相关
将一个Gif直接设置为UIImageView的image就可以显示动态图了。
```
/** 用一个Gif生成UIImage，传入一个GIFData */
+ (UIImage *)animatedImageWithAnimatedGIFData:(NSData *)theData;
/** 用一个Gif生成UIImage，传入一个GIF路径 */
+ (UIImage *)animatedImageWithAnimatedGIFURL:(NSURL *)theURL;
```
使用：
```
NSString *path = [[NSBundle mainBundle] pathForResource:@"gif" ofType:@"gif"];
self.imageView.image = [UIImage animatedImageWithAnimatedGIFURL:[NSURL fileURLWithPath:path]];
//或者
self.imageView.image = [UIImage animatedImageWithAnimatedGIFData:[NSData dataWithContentsOfFile:path]];
```
```
```
