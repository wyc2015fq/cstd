# UIButton设置图片和文字（一）-----本人备注 - xqhrs232的专栏 - CSDN博客
2019年02月25日 14:46:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/c49bda3249ad](https://www.jianshu.com/p/c49bda3249ad)
相关文章
1、UIButton设置图片靠近左边----[https://www.jianshu.com/p/99506d09cbd2](https://www.jianshu.com/p/99506d09cbd2)
2、[IOS-UIButton的文本与图片的布局](https://www.cnblogs.com/oc-bowen/p/5851849.html)----[https://www.cnblogs.com/oc-bowen/p/5851849.html](https://www.cnblogs.com/oc-bowen/p/5851849.html)
在开发的过程中经常会遇到需要在button中放置图片和文字，比如将图片放置在button左边，文字放置在右边。因为UIButton也是继承自UIView，因此可以像其它的view一样添加subView，
```
//创建button
UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
// 创建imageview
UIImage *image = [UIImage imageNamed:@"yourImage.png"];
UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(/*frame*/)];
[imageView setImage:image];
// 创建label
UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(/*frame*/)];
[label setText:@"Your title"];
// 添加到button中
[button addSubview:label];
[button addSubview:imageView];
```
这种方法的好处是简单明了，但是其实在UIButton中已经包含了UIImageView，我们不需要在自己添加该imageView的。也可以采用如下的方法，但是该方法的在调整UI时比较不方便
```
cardNameBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [cardNameBtn setTitle:@"集卡名称" forState:UIControlStateNormal];
//图片
    [cardNameBtn setImage:[UIImage imageNamed:@"card_flag_image"] forState:UIControlStateNormal];
//图片基于文字的相对位置
    cardNameBtn.imageEdgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
    cardNameBtn.titleEdgeInsets = UIEdgeInsetsMake(0, 10, 0, 0);
//文字和图片居中 
    cardNameBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    [self addSubview:cardNameBtn];
```
还可以设置点击时候的图片改变
```
[self.forwardBtn setImage:[UIImage imageNamed:@"Forward_select.png"] forState:UIControlStateHighlighted];
[self.forwardBtn setTitleColor:RGBCOLOR(255, 160, 31) forState:UIControlStateHighlighted];
```
样式如下：
![](https://upload-images.jianshu.io/upload_images/2013577-728eae3e184c2a39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/328/format/webp)
209BB1F4-E4C1-45A0-AD6B-5799C6155B8F.png
//==============================================================================================
本人备注：：
1》图片添加到XCODE工程里面去---------找到文件的路径，直接拖进工程就好！！！
