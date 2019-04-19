# iOS磨砂（半透明）效果实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月30日 15:08:17[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5337
![](http://upload-images.jianshu.io/upload_images/1743443-e9434defdcceb346.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
291F12A6-B038-4D48-9A26-E650F2BE2D1A.png
上图是我做的一个小demo里面的截图，如图所示，弹出框的背景是现在app大量使用的半透明磨砂效果，那么如果去实现这种效果呢？
```
// 判断系统版本是否支持 8.0
    UIView *blurEffectView;
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        // 磨砂效果
        UIBlurEffect *blurEffect = [UIBlurEffect effectWithStyle:UIBlurEffectStyleDark];
        // 磨砂视图
        blurEffectView = [[UIVisualEffectView alloc] initWithEffect:blurEffect];
    } else {
        // 屏幕截图 - 调用苹果官方框架实现磨砂效果
        UIImage *screenShot = [UIImage screenShot].applyLightEffect;
        blurEffectView = [[UIImageView alloc] initWithImage:screenShot];
    }
    //    [self addSubview:blurEffectView];
    [self insertSubview:blurEffectView atIndex:0];
    [blurEffectView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self);
    }];
```
苹果在8.0之前提供了这种效果的第三方框架，在8.0之后干脆将这个框架封装在api里面，所以兄弟们在使用的时候像我这样做一下判断即可。
- 判断版本号
- 
[self insertSubview:blurEffectView atIndex:0];插入最下面
```
ps：另外附上苹果提供的第三方框架给大家。
UIImage+ImageEffects下载地址http://download.csdn.net/detail/baitxaps/8893093
```
