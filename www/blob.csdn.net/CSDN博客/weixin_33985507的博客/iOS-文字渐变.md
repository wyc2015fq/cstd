# iOS-文字渐变 - weixin_33985507的博客 - CSDN博客
2018年09月12日 19:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(10, 50, 200, 50)];
    label.text = @"渐变文字展示";
    [self.view addSubview:label];
    
    CAGradientLayer *gradientLayer = [CAGradientLayer layer];
    gradientLayer.frame = label.frame;
    UIColor *color_2 = [UIColor colorWithRed:45/255.0 green:240/255.0 blue:219/255.0 alpha:1.0];
    UIColor *color_1 = [UIColor colorWithRed:255/255.0 green:225/255.0 blue:50/255.0 alpha:0.5];
    gradientLayer.colors = @[(__bridge id)color_1.CGColor,
                                   (__bridge id)color_2.CGColor];
    gradientLayer.startPoint = CGPointMake(0, 0);
    gradientLayer.endPoint = CGPointMake(0, 1);
    [self.view.layer addSublayer:gradientLayer];
    gradientLayer.mask = label.layer;
    label.frame = gradientLayer.bounds;
```
效果展示：
![1787970-75e7850b07918fc6.png](https://upload-images.jianshu.io/upload_images/1787970-75e7850b07918fc6.png)
image.png
