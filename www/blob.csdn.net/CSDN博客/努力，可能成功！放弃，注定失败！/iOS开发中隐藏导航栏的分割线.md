# iOS开发中隐藏导航栏的分割线 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:54:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：35
iOS开发中经常会遇到需要隐藏导航栏的分割线的需求，之前已经有两种方法
### 方法一
直接设置navigationBar的ShadowImage和BackgroundImag，这样做可以一劳永逸，在当前导航控制器的所有子控制器页面看到的导航栏都是没有分割线的。
弊端：想恢复比较麻烦
```
[self.navigationController.navigationBar setShadowImage:[UIImage new]];
 [self.navigationController.navigationBar setBackgroundImage:[UIImage new] forBarMetrics:UIBarMetricsDefault];
```
### 方法二
遍历navigationBar的所有子View和子View的子View，最终找到一个UIImageView，然后持有该对象，设置其隐藏或者显示即可达到目的。
弊端：代码繁琐，遍历次数太多。并且iOS10下已经没有效果
```
if ([self.navigationController.navigationBar respondsToSelector:@selector( setBackgroundImage:forBarMetrics:)]){
    NSArray *list=self.navigationController.navigationBar.subviews;
    for (id obj in list) {
        if ([obj isKindOfClass:[UIImageView class]]) {
            UIImageView *imageView=(UIImageView *)obj;
            NSArray *list2=imageView.subviews;
            for (id obj2 in list2) {
                if ([obj2 isKindOfClass:[UIImageView class]]) {
                    //将分割线 移除
                    UIImageView *imageView2=(UIImageView *)obj2;
                    imageView2.hidden=YES;
                }
            }
        }
    }
}
```
### 方法三（方法二的变种版本）
下面的代码是在iOS10下打印了UINavigationBar对象的所有子View，我们发现之前存放分割线的容器在iOS上已经变为了_UIBarBackground，他是一个私有的类的对象，我们无法直接获得。可以使用NSClassFromString(@"_UIBarBackground")来判断。
```
2016-12-02 12:29:46.898125 Demo[13941:3014012] <_UIBarBackground: 0x151d0cf00; frame = (0 -20; 375 64); userInteractionEnabled = NO; layer = <CALayer: 0x174030e60>>,UIView
2016-12-02 12:29:46.900464 Demo[13941:3014012] <<UINavigationItemView: 0x151df50c0; frame = (153 8; 69.5 27); opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x17423ff40>>: item=<<UINavigationItem: 0x1701c6270>: title:'数据统计'> title=数据统计>,UIView
2016-12-02 12:29:46.902259 Demo[13941:3014012] <UIButton: 0x153923400; frame = (1 2; 40 40); opaque = NO; layer = <CALayer: 0x174424560>>,UIControl
2016-12-02 12:29:46.902681 Demo[13941:3014012] <UIButton: 0x153a08ca0; frame = (295 0; 64 44); opaque = NO; layer = <CALayer: 0x1704309a0>>,UIControl
2016-12-02 12:29:46.904273 Demo[13941:3014012] <_UINavigationBarBackIndicatorView: 0x151e13760; frame = (8 11.5; 13 21); alpha = 0; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x170035ae0>>,UIImageView
```
我们将方法二的代码改为下面这个样子，发现在iOS10上分割线被隐藏了。
```
if ([self.navigationController.navigationBar respondsToSelector:@selector(setBackgroundImage:forBarMetrics:)]){
    NSArray *list=self.navigationController.navigationBar.subviews;
    for (id obj in list) {
        if ([obj isKindOfClass:NSClassFromString(@"_UIBarBackground")]) {
            UIImageView *imageView=(UIImageView *)obj;
            NSArray *list2 = imageView.subviews;
            for (id obj2 in list2) {
                if ([obj2 isKindOfClass:[UIImageView class]]) {
                    _navLine = (UIImageView *)obj2;
                }
            }
        }
    }
}
```
但是这么做在iOS10之前的版本又不行了，解决版本是把第一次遍历的判断的条件改为[obj isKindOfClass:[UIView class]]。并且需要判断obj2的高度，因为条件更改后会遍历出两个UIImageView。
```
if ([self.navigationController.navigationBar respondsToSelector:@selector(setBackgroundImage:forBarMetrics:)]){
    NSArray *list=self.navigationController.navigationBar.subviews;
    for (id obj in list) {
        if ([obj isKindOfClass:[UIView class]]) {
            UIImageView *imageView=(UIImageView *)obj;
            NSArray *list2 = imageView.subviews;
            for (id obj2 in list2) {
                if ([obj2 isKindOfClass:[UIImageView class]]) {
                    if (CGRectGetHeight([obj2 frame]) == 0.5) {
                        _navLine = (UIImageView *)obj2;
                    }
                }
            }
        }
    }
}
```
### 方法四
发现以上方法都太过繁琐，所以我把iOS10和之前版本的UINavigationBar的子试图的对象都打印了出来，如下：
```
iOS9
2016-12-02 12:31:39.201259 Demo[13948:3014909] <_UIBarBackground: 0x10302ee80; frame = (0 -20; 375 64); userInteractionEnabled = NO; layer = <CALayer: 0x17003ae80>>,UIView
2016-12-02 12:31:39.202724 Demo[13948:3014909] <UIImageView: 0x10302f270; frame = (0 64; 375 0.5); userInteractionEnabled = NO; layer = <CALayer: 0x17003aec0>>,UIView
2016-12-02 12:31:39.204211 Demo[13948:3014909] <<UINavigationItemView: 0x1030ef330; frame = (153 8; 69.5 27); opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x17023cd40>>: item=<<UINavigationItem: 0x1741d4be0>: title:'数据统计'> title=数据统计>,UIView
2016-12-02 12:31:39.205571 Demo[13948:3014909] <UILabel: 0x103077180; frame = (0 3.5; 69.5 21.5); text = '数据统计'; opaque = NO; userInteractionEnabled = NO; layer = <_UILabelLayer: 0x17028c850>>,UIView
2016-12-02 12:31:39.207938 Demo[13948:3014909] <UIButton: 0x10dd195a0; frame = (1 2; 40 40); opaque = NO; layer = <CALayer: 0x174428220>>,UIControl
2016-12-02 12:31:39.208315 Demo[13948:3014909] <UIImageView: 0x1030e8270; frame = (5 5; 30 30); clipsToBounds = YES; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x170421c20>>,UIView
2016-12-02 12:31:39.208874 Demo[13948:3014909] <UIButton: 0x103172720; frame = (295 0; 64 44); opaque = NO; layer = <CALayer: 0x174430320>>,UIControl
2016-12-02 12:31:39.209393 Demo[13948:3014909] <UIButtonLabel: 0x1031bc670; frame = (3.5 13.5; 57.5 17); text = '数据说明'; opaque = NO; userInteractionEnabled = NO; layer = <_UILabelLayer: 0x174287a80>>,UILabel
2016-12-02 12:31:39.209964 Demo[13948:3014909] <_UINavigationBarBackIndicatorView: 0x1031277c0; frame = (8 11.5; 13 21); alpha = 0; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x17403b7c0>>,UIImageView
iOS10
2016-12-02 12:33:59.139 Demo[512:301114] <_UINavigationBarBackground: 0x13cdf4a60; frame = (0 -20; 320 64); autoresize = W; userInteractionEnabled = NO; layer = <CALayer: 0x13cdf4ed0>>,_UIBarBackgroundImageView
2016-12-02 12:33:59.139 Demo[512:301114] <UIImageView: 0x13cdf53f0; frame = (0 64; 320 0.5); userInteractionEnabled = NO; layer = <CALayer: 0x13cdf55a0>>,UIView
2016-12-02 12:33:59.141 Demo[512:301114] <UINavigationItemView: 0x13e2c66f0; frame = (126 8; 68 27); opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x13e058300>>,UIView
2016-12-02 12:33:59.142 Demo[512:301114] <UILabel: 0x13e532b30; frame = (0 3.5; 68 21.5); text = '数据统计'; opaque = NO; userInteractionEnabled = NO; layer = <_UILabelLayer: 0x13e2e9df0>>,UIView
2016-12-02 12:33:59.142 Demo[512:301114] <UIButton: 0x13e255d30; frame = (240 0; 64 44); opaque = NO; layer = <CALayer: 0x13e236200>>,UIControl
2016-12-02 12:33:59.143 Demo[512:301114] <UIButtonLabel: 0x13e2db400; frame = (4 13.5; 56 17); text = '数据说明'; opaque = NO; userInteractionEnabled = NO; layer = <_UILabelLayer: 0x13e04cad0>>,UILabel
2016-12-02 12:33:59.144 Demo[512:301114] <UIButton: 0x13e2d38e0; frame = (1 2; 40 40); opaque = NO; layer = <CALayer: 0x13e202030>>,UIControl
2016-12-02 12:33:59.144 Demo[512:301114] <UIImageView: 0x13e24ca40; frame = (5 5; 30 30); clipsToBounds = YES; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x13cdeec50>>,UIView
2016-12-02 12:33:59.145 Demo[512:301114] <_UINavigationBarBackIndicatorView: 0x13cdfa350; frame = (8 11.5; 13 21); alpha = 0; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x13cdfa740>>,UIImageView
```
仔细研究后发现，iOS10只是把_UINavigationBarBackground（UIImageView的子类）改为了_UIBarBackground（UIView的子类）。这两个对象虽然不同，但是作用是一样的，他的Y值为-20，猜测作用应该是为了将NavigationBar延伸到状态栏下面。通过打印出来的顺序可以看到不管是_UINavigationBarBackground还是_UIBarBackground，他们都是在UINavigationBar的最底层，也就是最先被addSuview到父View上的。然后他们的唯一一个子View就是UIImageViewe类型的对象，并且该对象的高度就是0.5，可以确定就是分割线。
所以经过精简后，去掉导航栏分割线的代码就变成了下面这个样子。
```
UIView *backgroundView = [self.navigationController.navigationBar subviews].firstObject;
    _navLine = backgroundView.subviews.firstObject;
```
经过测试，在iOS10和iOS9上都可以隐藏分割线。
欢迎有更好的方法的同学一起交流。
作者：lxyz22zp
链接：https://www.jianshu.com/p/23d9bde85f13
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
