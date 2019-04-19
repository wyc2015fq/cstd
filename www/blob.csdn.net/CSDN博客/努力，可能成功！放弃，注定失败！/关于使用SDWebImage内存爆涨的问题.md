# 关于使用SDWebImage内存爆涨的问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月15日 13:56:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：7087
这几天在做一个需求，需要在一系列的控制器里下载N张图片。最方便最省事的解决方案当然首选SDWebImage这个框架（相信也很少不去用它的）。把图片地址放进去，返回一个图片，轻松，快捷，令人幸福，直到看到下面这个画面。
![](http://upload-images.jianshu.io/upload_images/50992-20a5c484dc0f9aaf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-07-14 下午4.56.39.png
什么鬼！内存竟然从20M给我彪到了180+M！而且当我把控制器移除之后，内存竟然也没有丝毫的下降，这在APP上可完全不行。一开始我并没有想到是SD的问题，以为是我代码有什么问题，造成了循环引用，内存没有释放，于是乎，打开Leaks，查起来。
![](http://upload-images.jianshu.io/upload_images/50992-9563c2ddd656ad13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-07-14 下午5.04.59.png
恩，是发现了AF中的一些内存泄漏问题，但是也只有10+KB啊，剩下的150MB的内存是哪里来的呢，只能一步步盘查起来。
![](http://upload-images.jianshu.io/upload_images/50992-d6d52c1f13f1121b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
B5573D21-43A2-4453-BE49-853A758935D6.png
抽丝剥茧，我们看到，大部分的内存都显示在SDWebImage的
```
- (UIImage *)diskImageForKey:(NSString *)key {
    NSData *data = [self diskImageDataBySearchingAllPathsForKey:key];
    if (data) {
        UIImage *image = [UIImage sd_imageWithData:data];
        image = [self scaledImageForKey:key image:image];
        if (self.shouldDecompressImages) {
            image = [UIImage decodedImageWithImage:image];
        }
        return image;
    }
    else {
        return nil;
    }
}
```
这个方法里产生的
SDWebImage的源码分析网上一搜一大把，这里就不多说了。这里说一下怎么解决这个内存问题。
首先在网上搜了一下，有的人是直接修改第三方内部源码解决的。这里不推荐，因为大部分开发者是通过cocoapods安装的的框架，这里修改起来也不是很方便。
其实很简单，清除cache，只要在合适的时机去调用它就行
`` [[SDImageCache sharedImageCache] setValue:nil forKey:@"memCache"];``
使用之后，内存瞬间下来，清清爽爽。
网上说把它放在applicationDidReceiveMemoryWarning里调用，但是我没有，因为在一些老机型里，比如iPhone4，内存爆炸的时候瞬间就crash了，来不及去调这个，所以我建议在做一些大图片多图片处理的时候，最好随时的去清理一下。
文／司机王（简书作者）
原文链接：http://www.jianshu.com/p/e6e52efc5a35
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
