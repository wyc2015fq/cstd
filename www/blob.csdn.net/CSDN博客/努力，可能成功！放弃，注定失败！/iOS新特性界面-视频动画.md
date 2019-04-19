# iOS新特性界面-视频动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月14日 19:57:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：929
## 前言
昨天晚上一个简友`简信`我,问我`咸鱼`最新版的新特性动画有没有什么思路. 原谅 我都没怎么用过`咸鱼`, 再加上已经四点多了,就跟他胡诌了一下思路.
今天`广州`一如既往的是大雨倾盆, 想起昨晚的事, 就下载了一下`咸鱼`的最新版, 看了一下`新特性`, 感觉还是蛮`炫酷`的. 脑中想了几组动画, 准备自己写一下, 随后拿到了`咸鱼`的所有资源, 然后我就发现, 我想多了, 他们的做法更方便...
## 效果图
![](http://upload-images.jianshu.io/upload_images/1038348-8aca5e5b57b66949.gif?imageMogr2/auto-orient/strip)
咸鱼新特性
## github下载地址
OC版和swift版都在这一个地址:
如果对你有些许帮助, 请star, 有什么疑问记得给我留言
[github下载地址](https://github.com/SunLiner/FleaMarket)
## 友情链接
Swift版 iOS高仿完整版唯美植物系项目<花田小憩>实战, 点击进行跳转
[github地址](https://github.com/SunLiner/Floral)
[Blog地址](http://www.jianshu.com/p/2893be49c50e)
## 思路
最初拿到`咸鱼`的所有资源的时候, 我是拒绝的, 因为我第一眼看到的就是4个`新特性`视频文件, 虽然有想到过可能是和`UBer`一样用的是视频, 但是证实了之后, 还是有点小吃惊. 此处, 大神毋鄙视...
随后翻了一下`咸鱼`的所有图片资源, 随带找出了视频的`封面`图片...
① 整体的新特性模块, 我的做法是一个`UICollectionViewController` + `UIPageControl`, 这个应该是毋庸置疑的了...
② 重点在于`UICollectionViewCell`. 播放本地视频, 我使用的是`MediaPlayer`框架里面的`MPMoviePlayerController`. 步骤: 先建一个`MPMoviePlayerController`的对象``, 然后把它的`view`添加到`UICollectionViewCell
`的`contentView`上. 然后再先建一个封面图片`UIImageView`, 添加到`MPMoviePlayerController
`的`view`上.
③ 我们需要在`UICollectionViewCell`监听`MPMoviePlayerLoadStateDidChangeNotification`和`MPMoviePlayerPlaybackDidFinishNotification`.
在`MPMoviePlayerLoadStateDidChangeNotification`监听方法中, 当`MPMoviePlayerController`的`loadState`等于`PlaythroughOK`的时候,我们需要将封面图片的hidden设置成true.
tip:此处不能直接`removeFromSuperview`, 不然会有闪烁的效果.
刚好也利用了`UICollectionViewCell`的重用特性, 因为我们加载过一次视频后, 就不再需要用封面图片了.
在`MPMoviePlayerPlaybackDidFinishNotification`的监听方法中, 判断当前的`UICollectionViewCell`是否被选择了, 如果用户点击(选中)了当前的`cell`且当前`cell`是最后一个`cell`, 我们才会进行跳转
```
private lazy var moviePlayer : MPMoviePlayerController = {
        let player = MPMoviePlayerController()
        player.view.frame = self.contentView.bounds
        // 设置自动播放
        player.shouldAutoplay = true
        // 设置源类型
        player.movieSourceType = .File
        // 取消下面的控制视图: 快进/暂停等...
        player.controlStyle = .None
        NSNotificationCenter.defaultCenter().addObserver(self, selector: #selector(NewFeatureCell.loadStatus), name: MPMoviePlayerLoadStateDidChangeNotification, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: #selector(NewFeatureCell.playFinished), name: MPMoviePlayerPlaybackDidFinishNotification, object: nil)
        return player
    }()
    // MARK: - private method
    func loadStatus()
    {
        // 将要自动播放
        if moviePlayer.loadState == .PlaythroughOK {
            imageView.hidden = true
            moviePlayer.play()
        }
    }
    func playFinished()
    {
        NSNotificationCenter.defaultCenter().postNotificationName(PlayFinishedNotify, object: nil)
    }
```
## 用法
我对这个小Demo进行了一些简单的封装, 不管是`OC`还是`Swift`, 下载之后, 直接将`NewFeature`文件夹拖进您的项目中
OC用法:
`#import "NewFeatureViewController.h"`
```
NewFeatureViewController *newFeatureVC = [[NewFeatureViewController alloc] init];
    NSMutableArray *array = [NSMutableArray array];
    for (int i = 0; i<4; i++) {
        [array addObject:[[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"guide%d",i] ofType:@"mp4"]];
    }
    // 1.设置本地视频路径数组
    newFeatureVC.guideMoviePathArr = array;
    // 2.设置封面图片数组
    newFeatureVC.guideImagesArr = @[@"guide0", @"guide1", @"guide2", @"guide3"];
    // 3.设置最后一个视频播放完成之后的block
    [newFeatureVC setLastOnePlayFinished:^{
        UINavigationController *rootNav = [[UINavigationController alloc] initWithRootViewController:[[ViewController alloc] init]];
        [UIApplication sharedApplication].keyWindow.rootViewController = rootNav;
    }];
    self.window.rootViewController = newFeatureVC;
```
Swift用法
```
// 配置本地视频路径和视频的封面图片
var paths = [String]()
var images = [UIImage]()
for i in 0..<4 {
    paths.append(NSBundle.mainBundle().pathForResource("guide\(i)", ofType: "mp4")!)
    images.append(UIImage(named: "guide\(i)")!)
}
// 设置rootViewController为新特性控制器
window?.rootViewController = NewFeatureViewController(images: images, moviePaths: paths, playFinished: { [unowned self] in
    self.window?.rootViewController = UINavigationController(rootViewController: ViewController())
})
```
## 联系我
[github](https://github.com/SunLiner)
[简书](http://www.jianshu.com/users/9723687edfb5/latest_articles)
文／Monkey_ALin（简书作者）
原文链接：http://www.jianshu.com/p/2b03d19f4acd
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
