# iOS逆向之分析微信导航栏实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月19日 21:53:43[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1415
![](http://upload-images.jianshu.io/upload_images/128529-12f2198a44ca7729.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
最近需要实现微信的毛玻璃导航栏效果，尝试了各种方式后还是有点差别，这在追求完美的设计师眼里是绝不能忍的，于是只好“看看”原作是怎么实现的。在逆向分析了微信的实现后，发现微信的实现十分特殊，文末会告诉大家答案:)
### 环境准备
- 一台越狱设备
- OpenSSH 插件
- Cycript 插件
[盘古](http://www.pangu.io/?flag=cn)支持 9.3.3 以下版本的越狱，越狱过程十分快速方便。越狱完成后还要在 Cydia 里搜索安装 OpenSSH 和 Cycript 插件。OpenSSH 用以电脑远程登录 iPhone，Cycript 用来运行时钩住应用程序。Cycript 允许开发者动态查看和修改正在运行的应用程序，更多介绍请查看[官网](http://www.cycript.org/)。
环境准备完毕后，我们就可以开始逆向了。我手里的是 iOS 9.2.1 版本的越狱设备，以下都是用这个设备进行演示。
> 
越狱有风险，建议使用备用机进行越狱，并且越狱前先备份资料
### 实际操作
首先打开电脑的命令行工具，执行 `ssh` 远程登录到 iPhone 手机上，默认连接密码是 alpine，建议连上后修改默认密码，修改命令是
`passwd`，连上后的界面如下图所示，
![](http://upload-images.jianshu.io/upload_images/128529-879ae75c8db20d54.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下面我们可以用 `ps` 命令查看微信进程，执行
```bash
ps -e|grep WeChat
```
结果如下图，
![](http://upload-images.jianshu.io/upload_images/128529-fcf26293cf5e3a8f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里我们能够获取到微信的沙盒路径，进入到这个目录后我们能看到如下的内容，主要是一些静态图片资源，如下图：
![](http://upload-images.jianshu.io/upload_images/128529-28d7c3d24d612c7d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### Cycript
前面已经介绍了 Cycript，我们分析导航栏的实现就是需要借助这个工具，[官网](http://www.cycript.org/)里有更多详细说明。
首先在越狱手机上打开微信，然后使用 `ps` 命令获取到微信的进程id，然后再输入
`cycript -p xxx`
当我们看到命令行界面出现 `cy#` 就说明我们已成功 hook 住微信进程，这个过程如下图所示，
![](http://upload-images.jianshu.io/upload_images/128529-fdd69f6a233d108c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上面这张图里的信息很丰富，展示了我是如何一步一步获取到 `ContactsViewController`。在最下方我打印出了`UINavigationBar`的视图层级关系，我们能看到控制导航栏样式的视图有两个，一个是view1，一个是view2，其中view2应该是系统自带的毛玻璃效果。
为了验证这个想法，我们来做一些设置。首先将 view1 的 alpha 设置为0，结果如下图，
![](http://upload-images.jianshu.io/upload_images/128529-64715ed1f2dfc850.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
左边的是 view1.alpha = 0，右边的是原来的样式。我们能看到导航栏的颜色变浅了，页面滑动时的毛玻璃穿透效果很明显。我在自己的 App 里也实现了一样的效果，做法是将 barStyle 设置成 UIBarStyleBlack。
打印出微信的导航栏类型属性，发现也是 UIBarStyleBlack
![](http://upload-images.jianshu.io/upload_images/128529-be60a6dff10fdf28.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
至此，我们基本能确定微信就是在系统提供的黑色风格导航栏上做了一些处理。那到底是做了什么处理呢？我们继续往下看。
将 view1 的 alpha 还原回去，再将 view2 的 alpha 设置为 0，会看到下图的样式，
![](http://upload-images.jianshu.io/upload_images/128529-bd539bbe2d777c55.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
再把 view2 的实际显示的图层打印出来，发现这是一个渐变图层
![](http://upload-images.jianshu.io/upload_images/128529-f700bf1435661bcd.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
至此，我们发现微信导航栏的最终显示效果是**由一个渐变图层加系统提供的黑色毛玻璃组合显示而成。**
根据这些信息，我在 App 里也用相同的做法尝试了一下，显示效果终于基本一致了，真是泪流满面啊。关键代码如下，
```cpp
```cpp
- (UIView *)blurBackView
{
    if (_blurBackView == nil) {
        _blurBackView = [UIView new];
        _blurBackView.frame = CGRectMake(0, -20, SCREEN_WIDTH, 64);
        CAGradientLayer *gradientLayer = [[CAGradientLayer alloc] init];
        gradientLayer.frame = CGRectMake(0, 0, SCREEN_WIDTH, 64);
        gradientLayer.colors = @[(__bridge id)[UIColor colorWithHex:0x040012 alpha:0.76].CGColor,(__bridge id)[UIColor colorWithHex:0x040012 alpha:0.28].CGColor];
        gradientLayer.startPoint = CGPointMake(0, 0);
        gradientLayer.endPoint = CGPointMake(0, 1.0);
        [_blurBackView.layer addSublayer:gradientLayer];
        _blurBackView.userInteractionEnabled = NO;
        _blurBackView.alpha = 0.5;
    }
    return _blurBackView;
}
```
```
`blurBackView` 的作用类似上述例子里的 view1，插入到 `UINavigationBar` 视图里的最下方。
Cycript 还能执行一些其他操作，例如弹出 Alert 弹框，
![](http://upload-images.jianshu.io/upload_images/128529-f70f57e5b18a6613.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
结果如下
![](http://upload-images.jianshu.io/upload_images/128529-24a149a43112c7bc.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 总结
本文整理了我探究微信导航栏实现的过程，发现微信导航栏的最终显示效果是**由一个渐变图层加系统提供的黑色毛玻璃组合显示而成。**如果有其他实现方式也可以给我留言，学习一下~
逆向的用法肯定不止这一种场景，结合 class-dump 还能做更深入的分析。如果感兴趣也可以自己实际操作体验一下，enjoy~
> 
转载请标注出处，有任何疑问都可联系我，欢迎探讨。
文／hi_xgb（简书作者）
原文链接：http://www.jianshu.com/p/a9cd03044a31
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
