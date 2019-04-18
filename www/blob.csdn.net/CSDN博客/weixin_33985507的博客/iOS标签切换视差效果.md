# iOS标签切换视差效果 - weixin_33985507的博客 - CSDN博客
2017年04月28日 11:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
![2742645-642ad56a04bb6f04.gif](https://upload-images.jianshu.io/upload_images/2742645-642ad56a04bb6f04.gif)
最终效果
仔细观察，可以发现`绿色控件`在移动过程中与文字`重合`的地方字体颜色会跟着改变。
其实，界面的结构是这样的。
![2742645-621ae2788d7f6a5b.png](https://upload-images.jianshu.io/upload_images/2742645-621ae2788d7f6a5b.png)
结构图
使用`CADisplayLink`监听绿色控件的`实时状态`
```
CADisplayLink *link = [CADisplayLink displayLinkWithTarget:self selector:@selector(updateTopView)];
[link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
```
对顶层控件进行裁剪
```
- (void)updateTopView {
    CALayer *layer = self.moveView.layer.presentationLayer;
    if (!layer) {
        layer = self.moveView.layer;
    }
    
    CAShapeLayer *maskLayer = [[CAShapeLayer alloc] init];
    CGRect maskRect = layer.frame;
    CGPathRef path = CGPathCreateWithRect(maskRect, NULL);
    maskLayer.path = path;
    CGPathRelease(path);
    self.topView.layer.mask = maskLayer;
}
```
源码[下载地址](https://link.jianshu.com?t=https://github.com/HJaycee/LabelSwitchAnimation)
