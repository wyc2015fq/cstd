# ios 仿京东商品详情(背景缩小,下部弹出功能) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月09日 10:38:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3226
![](http://upload-images.jianshu.io/upload_images/1907672-2839882ede2142ef.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_0698.PNG
最近搞了一些动画效果如下:
1.双曲线波浪动画([http://www.jianshu.com/p/7db295fd38eb](http://www.jianshu.com/p/7db295fd38eb))
2.环形倒计时动画([http://www.jianshu.com/p/d1d16dff33c9](http://www.jianshu.com/p/d1d16dff33c9))
3.仪表式数字跳动动画([http://www.jianshu.com/p/0d6f50385861](http://www.jianshu.com/p/0d6f50385861))
4.iOS折线图实现（一）[http://www.jianshu.com/p/067825bb104f](http://www.jianshu.com/p/067825bb104f)
5.折线图实现(二)[http://www.jianshu.com/p/1c6a119149bd](http://www.jianshu.com/p/1c6a119149bd)
6.iOS 动画(core Animation) 一 [http://www.jianshu.com/p/0c9ea3ab8c9e](http://www.jianshu.com/p/0c9ea3ab8c9e)
今准备模仿一下 京东 天猫 商品详情里面的一个动画效果(如顶部的图片).
本文分为四部分 分别是: 效果展示,思路分析,代码实现,问题和疑惑
一.先上我自己搞得动画效果
![](http://upload-images.jianshu.io/upload_images/1907672-67446defffbd5d33.gif?imageMogr2/auto-orient/strip)
7月-08-2016 10-54-10.gif
效果就是点击屏幕红色的页面(是一个view 添加在self.view上)旋转并且缩小 这是弹出一个黄色的页面,再点击 黄色退去,红色归位
可以看出来界面还是很粗糙京东的线条还是很流畅(我想这也能看出来,京东天猫的效果确实不随便搞出来的),
二, 看了效果大概的思路估计也就清晰啦
- 点击屏幕是 首先让红色为 绕x 轴做旋转并且实现3D效果
2.同样归位是也是同样的倒立
3.用到的只是就是 UIView animation方法
和3D效果的实现
三.直接上代码
创建一个和屏幕大小一样的红色view 
```
- (void)viewDidLoad {
    [super viewDidLoad];
    UIView * view = [[UIView alloc]initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height)];
    view.backgroundColor = [UIColor redColor];
    self.myView = view;
    [self.view addSubview:view];
}
```
设置旋转的3d效果
```cpp
```cpp
- (CATransform3D)firstTransform{
    CATransform3D t1 = CATransform3DIdentity;
    t1.m34 = 1.0/-900;
    //带点缩小的效果
    t1 = CATransform3DScale(t1, 0.95, 0.95, 1);
    //绕x轴旋转
    t1 = CATransform3DRotate(t1, 15.0 * M_PI/180.0, 1, 0, 0);
    return t1;
}
```
```
下面开始点击屏幕 实现第一个效果
```
[UIView animateWithDuration:0.5 animations:^{
            [self.myView.layer setTransform:[self firstTransform]];//红色view调用了上面的旋转效果
            UIView * all = [[UIView alloc]initWithFrame:CGRectMake(0, self.view.bounds.size.height , [UIScreen mainScreen].bounds.size.width,  400)];//创建了黄色的view但是显示在屏幕以外,并且添加在window上面(这里会有一个坑)
            self.allview = all;
            all.backgroundColor = [UIColor yellowColor];
            UIWindow *window = [[UIApplication sharedApplication].windows lastObject];
            [window addSubview:all];
        } completion:^(BOOL finished) {
            [UIView animateWithDuration:0.3 animations:^{
                self.myView.transform = CGAffineTransformMakeScale(0.9, 0.95);//旋转完成以后页面缩小 同事改变黄色页面的frame的y值
                self.allview.frame = CGRectMake(0, self.view.bounds.size.height - 400, [UIScreen mainScreen].bounds.size.width, 400);
            } ];
        }];
```
接下来就是点击复位的3D旋转效果 
```cpp
```cpp
- (CATransform3D)firstTransform2{
    CATransform3D t1 = CATransform3DIdentity;
    t1.m34 = 1.0/-900;
    //带点缩小的效果
      t1 = CATransform3DScale(t1, 1, 1, 1);
    //绕x轴旋转
    t1 = CATransform3DRotate(t1, 15 * M_PI/180.0, 1, 0, 0);
    return t1;
}
```
```
接下来是整体实现
```
[UIView animateWithDuration:0.5 animations:^{
            self.allview.frame = CGRectMake(0, self.view.bounds.size.height, [UIScreen mainScreen].bounds.size.width, 400);
              [self.myView.layer setTransform:[self firstTransform2]];
        }completion:^(BOOL finished) {
            [self.allview removeFromSuperview];
            [UIView animateWithDuration:0.5 animations:^{
                self.myView.transform = CGAffineTransformMakeScale(1, 1);
            }];
        }];
//这段代码估计就不用解释了吧
```
到这里呢代码就完事了(感觉实现的方法可定有很多)
四, 问题和疑问
这一点再谈一下实现的过程中的一些问题和疑问
上面提到,进行缩小的是一个红色的view 而他是添加在了self.view 那么他会存在一些问题就是 如果页面上布局了其他的控件就会出现一些玛法.
那么解决的办法就是 将控制器的self.view 进行缩小
那么接下啦就会带来新的问题(我还不知道该怎么解决求指导)
看一下带来的问题:
![](http://upload-images.jianshu.io/upload_images/1907672-7096f1fd8a709f54.gif?imageMogr2/auto-orient/strip)
7月-08-2016 11-21-41.gif
可以看出来是现在是self.view在缩小 问题是 黄色弹出时,红色还覆盖在黄色上面.等动画完成时 黄色又出现在的前面.
我猜想的问题可能是 红色页面在进行旋转导致的解决办法目前我还不知道怎么搞
~~~~~~~******
如果哪位大神看到了这里请指点一二关于遇到的这个问题
最后放上代码链接[https://git.oschina.net/GAOZEJIAN/view-suoxiaofangJD.git](https://git.oschina.net/GAOZEJIAN/view-suoxiaofangJD.git)
文／健健锅（简书作者）
原文链接：http://www.jianshu.com/p/e0503b600e34
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
