# iOS10个实用小技巧（总有你不知道的和你会用到的） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月01日 14:46:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：163标签：[iOS10个实用小技巧（总有你不知道的和你会用到的）](https://so.csdn.net/so/search/s.do?q=iOS10个实用小技巧（总有你不知道的和你会用到的）&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
在开发过程中我们总会遇到各种各样的小问题，有些小问题并不是十分容易解决。在此我就总结一下，我在开发中遇到的各种小问题，以及我的解决方法。比较普遍的我就不再提了，这里主要讲一些你可能不知道的（当然，也有可能你都知道，大神就不必往下看了）
1、控件的局部圆角问题
你是不是也遇到过这样的问题，一个button或者label，只要右边的两个角圆角，或者只要一个圆角。该怎么办呢。这就需要图层蒙版来帮助我们了
```
CGRect rect = CGRectMake(0, 0, 100, 50);
    CGSize radio = CGSizeMake(5, 5);//圆角尺寸
    UIRectCorner corner = UIRectCornerTopLeft|UIRectCornerTopRight;//这只圆角位置
    UIBezierPath *path = [UIBezierPath bezierPathWithRoundedRect:rect byRoundingCorners:corner cornerRadii:radio];
    CAShapeLayer *masklayer = [[CAShapeLayer alloc]init];//创建shapelayer
    masklayer.frame = button.bounds;
    masklayer.path = path.CGPath;//设置路径
    button.layer.mask = masklayer;
```
举例为button，其它继承自UIView的控件都可以
2、navigationBar的透明问题
如果仅仅把navigationBar的alpha设为0的话，那就相当于把navigationBar给隐藏了，大家都知道，父视图的alpha设置为0的话，那么子视图全都会透明的。那么相应的navigationBar的标题和左右两个按钮都会消失。这样显然达不到我们要求的效果。
　　（1）如果仅仅是想要navigationBar透明，按钮和标题都在可以使用以下方法：
```
[self.navigationController.navigationBar setBackgroundImage:[UIImage new] 
forBarMetrics:UIBarMetricsDefault];//给navigationBar设置一个空的背景图片即可实现透明，而且标题按钮都在
```
细心的你会发现上面有一条线如下图：
![](https://upload-images.jianshu.io/upload_images/966908-3b7d38b83a9e353c.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
IMG_0025.PNG
这就需要我们做进一步处理，把线去掉，如下方法即可：
```
self.navigationController.navigationBar.shadowImage = [UIImage new];
//其实这个线也是image控制的。设为空即可
```
（2）如果你想在透明的基础上实现根据下拉距离，由透明变得不透明的效果，那么上面那个就显得力不从心了，这就需要我们采用另外一种方法了
```
//navigationBar是一个复合视图，它是有许多个控件组成的，那么我们就可以从他的内部入手
[[self.navigationController.navigationBar subviews] objectAtIndex:0].alpha = 0;//这里可以根据scrollView的偏移量来设置alpha就实现了渐变透明的效果
```
3、全局设置navigationBar标题的样式和barItem的标题样式
```
//UIColorWithHexRGB( )这个方法是自己定义的，这里只需要给个颜色就好了
 [[UINavigationBar appearance] setBarTintColor:UIColorWithHexRGB(0xfefefe)];
    
    [[UINavigationBar appearance] setTitleTextAttributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:18],NSForegroundColorAttributeName:UIColorWithHexRGB(0xfe6d27)}];
    
    [[UITabBarItem appearance] setTitleTextAttributes:@{NSFontAttributeName : [UIFont boldSystemFontOfSize:10],NSForegroundColorAttributeName : UIColorWithHexRGB(0x666666)} forState:UIControlStateNormal];
    
    [[UITabBarItem appearance] setTitleTextAttributes:@{NSFontAttributeName : [UIFont boldSystemFontOfSiz
```
4、navigationBar隐藏显示的过度
相信在使用中肯定遇到过，一个页面隐藏navigationBar，另一个不隐藏。两个页面进行push和pop的时候，尤其是有侧滑手势返回的时候，不做处理就会造成滑动返回时，navigationBar位置是空的，直接显示一个黑色或者显示下面一层视图，很难看。这就需要我们加入过度动画来隐藏或显示navigationBar：
在返回后将要出现的页面实现viewWillAppear方法，需要隐藏就设为YES，需要显示就设为NO
```
- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:NO animated:YES];
}
```
5、侧滑手势返回
iOS的侧滑返回手势有着很好的操作体验，不支持侧滑返回的应用绝对不是好应用。但是在开发过程中在自定义了返回按钮，或者某些webView,tableView等页面，侧滑返回手势失效，这时候就需要我们来进行设置一下了,可以在基类里面协商如下代码：
```
if ([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)]) {
//需要遵循一下手势的代理 
       self.navigationController.interactivePopGestureRecognizer.delegate = self;
        self.navigationController.interactivePopGestureRecognizer.enabled = YES;
    }
```
问题：当返回navigationController的最顶层的Controller的时候。再次侧滑，这个时候你在点击一个push页面的操作，你会发现卡那了，半天才会有反应。
　　这是由于，在最顶层Controller手势依然有效，但是滑动后，并找不到返回的页面。造成软件卡顿，假死所以就要在rootViewController中让此手势失效。把下面的设为NO
```
//记得写在didAppear里面
- (void)viewDidAppear:(BOOL)animated{
    self.navigationController.interactivePopGestureRecognizer.enabled = NO;
}
```
当然你也可以使用一个第三方库，写的相当棒。他对系统的侧滑返回手势进行拓展，不用从边缘滑动，只要右滑即可返回。最重要的是，他只需要加入项目中即可，不需要一行代码即可实现。附上[github 网址](https://github.com/forkingdog/FDFullscreenPopGesture)
6、给webView添加头视图
webView是一个复合视图，里面包含有一个scrollView，scrollView里面是一个UIWebBrowserView（负责显示WebView的内容）
```
UIView *webBrowserView = self.webView.scrollView.subviews[0];//拿到webView的webBrowserView
    self.backHeadImageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, kScreenWidth, kScreenWidth*2/3.0)];
    [_backHeadImageView sd_setImageWithURL:[NSURL URLWithString:self.imageUrl] placeholderImage:[UIImage imageNamed:@"placeholderImage"]];
    [self.webView insertSubview:_backHeadImageView belowSubview:self.webView.scrollView];
    //把backHeadImageView插入到webView的scrollView下面
    CGRect frame = self.webBrowserView.frame;
    frame.origin.y = CGRectGetMaxY(_backHeadImageView.frame);
    self.webBrowserView.frame = frame;
    //更改webBrowserView的frame向下移backHeadImageView的高度，使其可见
```
7、模态跳转的动画设置
设置模态跳转的动画，系统提供了四种可供选择
```
DetailViewController *detailVC = [[DetailViewController alloc]init];
    //UIModalTransitionStyleFlipHorizontal 翻转
    //UIModalTransitionStyleCoverVertical 底部滑出
    //UIModalTransitionStyleCrossDissolve 渐显
    //UIModalTransitionStylePartialCurl 翻页
    detailVC.modalTransitionStyle = UIModalTransitionStylePartialCurl;
    [self presentViewController:detailVC animated:YES completion:nil];
```
8、图片处理只拿到图片的一部分
```
UIImage *image = [UIImage imageNamed:filename];
CGImageRef imageRef = image.CGImage;
CGRect rect = CGRectMake(origin.x, origin.y ,size.width, size.height);
//这里的宽高是相对于图片的真实大小
//比如你的图片是400x400的那么（0，0，400，400）就是图片的全尺寸，想取哪一部分就设置相应坐标即可
CGImageRef imageRefRect = CGImageCreateWithImageInRect(imageRef, rect);
UIImage *imageRect = [[UIImage alloc] initWithCGImage:imageRefRect];
```
9、给UIView设置图片
```
UIImage *image = [UIImage imageNamed:@"playing"];
    _layerView.layer.contents = (__bridge id)image.CGImage;
_layerView.layer.contentsCenter = CGRectMake(0.25, 0.25, 0.5, 0.5);
//同样可以设置显示的图片范围
//不过此处略有不同，这里的四个值均为0-1之间；对应的依然是写x,y,widt,height
```
10、给TableView或者CollectionView的cell添加简单动画，像这样
![](https://upload-images.jianshu.io/upload_images/966908-4f826be8fca0bb4a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/529/format/webp)
888.gif
![](https://upload-images.jianshu.io/upload_images/966908-a6cf7dd499ba351e.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/529/format/webp)
555.gif
只要在willDisplayCell方法中对将要显示的cell做动画即可：
```
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath{
    NSArray *array =  tableView.indexPathsForVisibleRows;
    NSIndexPath *firstIndexPath = array[0];
    
    //设置anchorPoint
    cell.layer.anchorPoint = CGPointMake(0, 0.5);
    //为了防止cell视图移动，重新把cell放回原来的位置
    cell.layer.position = CGPointMake(0, cell.layer.position.y);
    
    //设置cell 按照z轴旋转90度，注意是弧度
    if (firstIndexPath.row < indexPath.row) {
            cell.layer.transform = CATransform3DMakeRotation(M_PI_2, 0, 0, 1.0);
    }else{
        cell.layer.transform = CATransform3DMakeRotation(- M_PI_2, 0, 0, 1.0);
    }
    cell.alpha = 0.0;
    
    [UIView animateWithDuration:1 animations:^{
        cell.layer.transform = CATransform3DIdentity;
        cell.alpha = 1.0;
    }];
}
- (void)collectionView:(UICollectionView *)collectionView willDisplayCell:(UICollectionViewCell *)cell forItemAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row % 2 != 0) {
        cell.transform = CGAffineTransformTranslate(cell.transform, kScreenWidth/2, 0);
    }else{
        cell.transform = CGAffineTransformTranslate(cell.transform, -kScreenWidth/2, 0);
    }
    cell.alpha = 0.0;
    [UIView animateWithDuration:0.7 animations:^{
        cell.transform = CGAffineTransformIdentity;
        cell.alpha = 1.0;
    } completion:^(BOOL finished) {
        
    }];
}
```
如需转载请注明出处，请尊重作者的劳动成果。
作者：_南山忆
链接：https://www.jianshu.com/p/a3156826c27c
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
