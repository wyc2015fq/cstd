# iOS 如何在 Release 环境下查看调试信息 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月14日 13:29:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：123
> 
最近老在释放环境下直接调试，发现一个很不爽的问题，不能直接查看调试信息，一些东东必须NSLog下才能打印看到的，然后po打印出来东西还都是nil，第一反应还以为传回来值为空的。
此时我们需要在Build Setting中的优化级别重新设置下，让其同调试模式下一样的
![](https://upload-images.jianshu.io/upload_images/784630-23d0883d5e032753.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/682/format/webp)
正常下
![](https://upload-images.jianshu.io/upload_images/784630-f9ca341cdf3a5614.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/687/format/webp)
改变后
> 
也就是在Release下切换下优化等级，然后就可以Po出调试信息啦，**注意正式发包的时候的一定不要忘记重新恢复原状**，没必要时可以不打开。
作者：中天空球的
链接：HTTPS：//www.jianshu.com/p/b830b50b5f90 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
