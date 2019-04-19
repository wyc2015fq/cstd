# iOS小问题（五） - game3108的专栏 - CSDN博客
2017年10月23日 13:46:00[game3108](https://me.csdn.net/game3108)阅读数：216
## 随手记几个最近碰到的小问题
tips：如果有错误，或者有更好的详细解答，请随时联系我进行修改
#### 1.iOS11NSCFDictionary与__NSCFDictionary关系变化
在iOS11上，NSCFDictionary变成了__NSCFDictioanry的子类 
如下测试： 
iOS11:
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1829891-903687496d3a95d9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS10:
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1829891-7866579f8733c479.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这边可以看到NSCFDictionary的superclass，从NSMutableDictionary变成了__NSCFDictionary
#### 2.Method Swizzling父子类问题
碰到一个crash，最后找出来原因，就是父子类的Method Swizzling问题。子类没有实现该方法，导致hook到了父类方法，出现crash。这边有一篇不错的文章，就是讲这个的，记录一下： 
[Objective-C Method Swizzling](http://yulingtianxia.com/blog/2017/04/17/Objective-C-Method-Swizzling/)
#### 3.UITableViewCell点击时候子view背景色消失问题
UITableViewCell上面如果subView有背景色的时候，点击UITableView，subView的背景色会消失。 
原因是点击后会触发Cell的`setSelected:`与`setHighlighted:`方法，里面会设置子view的`setBackgroundColor:`方法，将颜色设置为空。
解决方案： 
1.重写Cell的`setSelected:`与`setHighlighted:`方法，在其中将子view的背景色重新设一遍 
2.重写子view的
```
- (void)setBackgroundColor:(UIColor *)backgroundColor {
    return;
}
```
方法，阻断背景色的修改，然后在view的layer上面设置初始的背景色。
#### 4.圆角与阴影并存问题
有的时候要写圆角与阴影同时存在的情况，就可以CALayer加上shadowPath的方法。
```
UIBezierPath *maskPath = [UIBezierPath bezierPathWithRoundedRect:unitView.bounds cornerRadius:radius];
            CALayer *shadowLayer = [CALayer layer];
            shadowLayer.shadowPath = maskPath.CGPath;
            shadowLayer.frame = unitView.frame;
            shadowLayer.shadowRadius = shadowOffset;
            shadowLayer.shadowColor = [UIColor blackColor].CGColor;
            shadowLayer.shadowOffset = CGSizeMake(shadowOffset, shadowOffset);
            shadowLayer.shadowOpacity = 0.1;
            [self.contentView.layer insertSublayer:shadowLayer below:unitView.layer];
```
## 参考连接
无
