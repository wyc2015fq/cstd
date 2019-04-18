# iOS图层蒙版实现彩色字体效果 - weixin_33985507的博客 - CSDN博客
2017年01月07日 14:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
今天下雪了，来北京半年好像第一次见得雪，工作忙完了，梳理下今天学的东西，分享一下：
**彩虹颜色的实现**
CAGradientLayer *gradientLayer = [CAGradientLayer layer];
gradientLayer.frame = self.view.bounds;
colors是一个数组, 这里放置了彩虹的七种颜色(赤橙黄绿青蓝紫),你也可以放你喜欢的颜色
gradientLayer.colors = @[RGB(255, 0, 0), RGB(255, 165, 0), RGB(255, 255, 0), RGB(0, 255, 0), RGB(0, 255, 255), RGB(0, 0, 255), RGB(43, 0, 255), RGB(87, 0, 255)];
[self.view.layer addSublayer:gradientLayer];
如图：
![3474734-429d1efab664be30.png](https://upload-images.jianshu.io/upload_images/3474734-429d1efab664be30.png)
**    字体变为彩虹色**
UILabel *label = [[UILabel alloc] init];
label.text = @"赤橙黄绿青蓝紫";
label.textAlignment = NSTextAlignmentCenter;
label.numberOfLines = 0;
label.font = [UIFont systemFontOfSize:667.0/12];
[self.view addSubview:label];
1.子图层mask和父图层gradientLayer 构成了蒙板图层
2.mask图层只关注图层的形状，在这里也就是文字的内容
3.父图层去填充这个模板的颜色color
这里为了方便宽度我直接设置为了固定的375和667
gradientLayer.mask = label.layer;
label.layer.frame = CGRectMake((375-667/7)/2, 0, 667.0/7, 667);
如图：
![3474734-036c28fd562a788e.png](https://upload-images.jianshu.io/upload_images/3474734-036c28fd562a788e.png)
最后有一个很详细的讲解图层的地址，平时项目中用到特效或者动画比较多的推荐大家看看，真心不错，我也正在学习：[iOS核心动画](https://link.jianshu.com?t=https://zsisme.gitbooks.io/ios-/content/chapter4/layer-masking.html)
好啦，回家了，下雪天，心都跑到窗外了，祝大家生活愉快！
