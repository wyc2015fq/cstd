# ios小贴士-自定义导航栏返回按钮并且保留滑动返回手势 - weixin_33985507的博客 - CSDN博客
2017年06月23日 13:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
在ios中,系统提供的导航栏返回按钮,会自动包含许多其他我们可能用不到的View导致最后呈现的效果不如所愿,最常见的就是返回按钮所占的位置太大了,比如这样:
![728963-28b350d18cbff8b3.png](https://upload-images.jianshu.io/upload_images/728963-28b350d18cbff8b3.png)
返回按钮占位太大
因为UIBarButtonItem是一个包裹严密的类,它将UIView包裹在他的内部,使得我们没办法很方便地去获取或者修改其中真正的Button(当然我们可以用Runtime,KVC的方式来做,但我觉得还是稍显麻烦).
因此我们通常都用自定义的方法来修改返回按钮,但如果只是用`[[UIBarButtonItem alloc] initWithCustomView:customBtn];`来修改的话是无法使返回按钮右边的空隙减小的.
在系统的`UIBarButtonSystemItem`中有一个`UIBarButtonSystemItemFixedSpace`,它就是用来控制Item之间距离的,我们将它放在各个Item之间,并且将其width定为负数,便可以使得Item之间的间隔缩小了,由于系统默认的backItem距离最左边还是有一些像素的间隔,如果需要缩短距离屏幕左边的距离,那么就是按钮的左右各需要一个`UIBarButtonSystemItemFixedSpace`,代码如下
```
UIButton *leftBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [leftBtn setImage:[UIImage imageNamed:@"nav_back_icon"] forState:UIControlStateNormal];
    [leftBtn addTarget:self action:@selector(backNavAction) forControlEvents:UIControlEventTouchUpInside];
    leftBtn.frame = CGRectMake(-20, 0, 30, 30);
    UIBarButtonItem *leftItem = [[UIBarButtonItem alloc] initWithCustomView:leftBtn];
    UIBarButtonItem *leftSpace1 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    leftSpace1.width = -20;
    UIBarButtonItem *leftSpace2 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    leftSpace2.width = -20;
    self.navigationItem.leftBarButtonItems = @[leftSpace1,leftItem,leftSpace2];
```
这样,就达到我们想要的效果了:
![728963-d1839322e393eb3f.png](https://upload-images.jianshu.io/upload_images/728963-d1839322e393eb3f.png)
调整后的返回按钮
但是现在有一个新的问题,我们的滑动返回上级页面的手势没了.在`UINavigationController`的属性中,我们可以找到一个手势`interactivePopGestureRecognizer`,这个手势,就是我们滑动返回的手势,打印这个代理`self.navigationController.interactivePopGestureRecognizer.delegate`得到结果:
![728963-1d714662ae1dd355.png](https://upload-images.jianshu.io/upload_images/728963-1d714662ae1dd355.png)
默认的滑动返回手势代理
我们可以通过将手势的代理设置为控制器本身来重新激活滑动返回手势:
`self.navigationController.interactivePopGestureRecognizer.delegate = (id)self;`但是这又会带来新的问题,当你返回上级页面后(实际上还是同一个`UINavigationController`)会无法响应所有的Action,所以你需要将代理重新还给原主人:
```
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.oldNavGesDelegate = self.navigationController.interactivePopGestureRecognizer.delegate;
    self.navigationController.interactivePopGestureRecognizer.delegate = (id)self;
}
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    self.navigationController.interactivePopGestureRecognizer.delegate = self.oldNavGesDelegate;
}
```
这样我们就实现了既自定义返回按钮,又保留了滑动返回手势.
