# iOS开发 刮刮乐效果的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月09日 10:24:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1890
#### 引言
> 
我们平时在使用支付宝的时候, 会看到类似彩票刮刮乐的效果。如何实现的呢?下面就直接上代码了...
#### 刮刮乐效果:
1.创建视图:
```
-(void)createSubViews {
    /**
            注意:
        1. 这两个控件的位置要相同
        2. 一定要先创建下面的label, 再创建图片
     */
    // 展示刮出来的效果的view
    UILabel *labelL        = [[UILabel alloc] initWithFrame:CGRectMake(20, 100, 280, 200)];
    labelL.text            = @"刮刮乐效果展示";
    labelL.numberOfLines   = 0;
    labelL.backgroundColor = [UIColor brownColor];
    labelL.font            = [UIFont systemFontOfSize:30];
    labelL.textAlignment   = NSTextAlignmentCenter;
    [self.view addSubview:labelL];
    // 被刮的图片
    self.imageView       = [[UIImageView alloc] initWithFrame:labelL.frame];
    self.imageView.image = [UIImage imageNamed:@"222.jpg"];
    [self.view addSubview:self.imageView];
}
```
2.实现刮刮乐效果:
```
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    // 触摸任意位置
    UITouch *touch = touches.anyObject;
    // 触摸位置在图片上的坐标
    CGPoint cententPoint = [touch locationInView:self.imageView];
    // 设置清除点的大小
    CGRect  rect = CGRectMake(cententPoint.x, cententPoint.y, 20, 20);
    // 默认是去创建一个透明的视图
    UIGraphicsBeginImageContextWithOptions(self.imageView.bounds.size, NO, 0);
    // 获取上下文(画板)
    CGContextRef ref = UIGraphicsGetCurrentContext();
    // 把imageView的layer映射到上下文中
    [self.imageView.layer renderInContext:ref];
    // 清除划过的区域
    CGContextClearRect(ref, rect);
    // 获取图片
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    // 结束图片的画板, (意味着图片在上下文中消失)
    UIGraphicsEndImageContext();
    self.imageView.image = image;
}
```
#### 效果图:
![](http://upload-images.jianshu.io/upload_images/1975627-11ae8e3e37de281d.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果图
文／Li_Cheng（简书作者）
原文链接：http://www.jianshu.com/p/7c2042764e0c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
