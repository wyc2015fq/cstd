# iOS 如何设置一整张图片作为背景图片(穿透导航栏) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月08日 17:45:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：7822
![](http://upload-images.jianshu.io/upload_images/1743443-b6fb10b33a983218.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
WeChat_1460024275.jpeg
如图所示，这个界面的背景是一张偏黑色的背景图片。有意思的是，这张图片作为背景，严丝合缝的穿透了导航栏，以及导航栏下面的控制器，使整个页面看起来更加像一个整体。
兄弟们，项目里面很有可能会遇到这种需求，比如说我。废话不多说，如何实现这种效果呢
ps：以前看过我上一篇简书里面提到的抽取出来的一个公共类WXNavigationController，下面的代码就加在这个公共类中即可
下面代码的是将导航栏self.navigationBar变透明，这里变透明是将self.navigationBar设置一张透明的背景图片，这样当你设置导航栏下的背景图片的时候，背景图片不会被覆盖掉
```
- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self.navigationBar setBackgroundImage:[UIImage imageNamed:@"bigShadow"] forBarMetrics:UIBarMetricsCompact];
    //compact：及时更新背景样式,让导航条彻底透明，不显示底部那条线。 maskToBounds：不让透明图片影响状态栏
    self.navigationBar.layer.masksToBounds = YES;
}
```
抽取公告类的好处是，只用在这个地方设置一下背景图片就OK了,这样所有的页面都会变成同一个背景图片
```
#pragma mark -推出另外一个控制器的逻辑控制
- (void)pushViewController:(UIViewController *)viewController animated:(BOOL)animated {
    //设置所有界面的背景图片
    UIImageView * backImageView = [UIImageView ff_imageViewWithImageName:@"Rectangle 363"];
    [viewController.view insertSubview:backImageView atIndex:0];
    [backImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(viewController.view);
    }];
    if (self.childViewControllers.count > 0) {
        // 设置返回按钮
        viewController.navigationItem.leftBarButtonItem = [UIBarButtonItem ff_barButtonWithTitle:@"" imageName:@"Chevron" target:self action:@selector(goBack)];
    }
    [super pushViewController:viewController animated:animated];
}
```
###### 注意：
- 这里的self.navigationBar设置透明后，你的控制器的约束就会以最顶部开始计算，而不是从导航栏开始计算
- 设置所有界面的背景图片全部为clearcolor
- 当有个别界面导航栏是黑色的，你可以采用下面的方法,将tableview的contentoffset设置一下
`self.tableView.contentOffset = CGPointMake(0, 64);`
