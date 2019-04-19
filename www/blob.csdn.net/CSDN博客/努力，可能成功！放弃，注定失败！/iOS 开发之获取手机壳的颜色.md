# iOS 开发之获取手机壳的颜色 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:08:43[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：179
从刷爆IT圈的一个事件说起：
新闻：某互联网公司产品经理提出一个需求 - 要求APP开发人员做到软件根据用户的手机壳改变软件的主题颜色。
![](https://upload-images.jianshu.io/upload_images/3373235-804822c013a9d909.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/600/format/webp)
图片来源于网络
什么他妈的！还有这操作，PM，你过来，保证不打屎你。
![](https://upload-images.jianshu.io/upload_images/3373235-b08f5cdfcd491d64.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片来源于网络
　结果出现了。传送中出现的“亲热”的肢体接触事件（PS：平安产险的伙食一定很丰盛额，䅰〜）。
笑话看过了，要认真的研读下这个需求了：根据手机壳颜色来设置应用主题色。
很多“牛（KAN）逼（KE）”的技术团队给出了多个解决方案：
![](https://upload-images.jianshu.io/upload_images/3373235-ba9a41fb989de05a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/440/format/webp)
图片来源于网络
![](https://upload-images.jianshu.io/upload_images/3373235-f60d7b838e2f25b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片来源于网络
那么除了那么多穿戴设备及高端的天文组加入外，我们纯APP开发人员怎么实现这个需求可以考虑换个思路来实现这个需求：通过手机摄像头拍照或相册选择手机壳照片，然后提取照片的主色调或者照片虚化设置背景图片（如酷我音乐那种）。当然如果PM认为这种方法太笨，想要智能的那种，我建议公司多给PM申请一份保险【捂脸笑】.gif注意。
假定PM原定想的是获取手机出厂时颜色（黑，银，土豪金）。
代码如下：
```
#warning 该方法是私有 API，上线会被拒
    UIDevice *device = [UIDevice currentDevice];
    SEL selector = NSSelectorFromString(@"deviceInfoForKey:");
    if (![device respondsToSelector:selector]) {
        selector = NSSelectorFromString(@"_deviceInfoForKey:");
    }
    if ([device respondsToSelector:selector]) {
        IMP imp = [device methodForSelector:selector];
        NSString *(*func)(id, SEL, NSString *) = (void*)imp;
        NSString *deviceColor = func(device, selector, @"DeviceColor");
        NSString *deviceEnclosureColor = func(device, selector, @"DeviceEnclosureColor");
        NSLog(@"deviceColor -- >%@\n deviceEnclosureColor===>%@ ", deviceColor, deviceEnclosureColor);
        
    }
```
可获得手机外壳颜色。
如果产品指获取手机购买的外壳颜色，可以使用拍照，获取图片的平均颜色值。
找到一个不错的开源项目：[SOZOChromoplast](https://github.com/sozorogami/SOZOChromoplast)，可直接使用。
使用时：
```
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    _imageView.image = image;
    _chromoplast = [[SOZOChromoplast alloc] initWithImage:image];
    self.view.backgroundColor = _chromoplast.dominantColor;
    [picker dismissViewControllerAnimated:YES completion:nil];
}
```
效果如下：
![](https://upload-images.jianshu.io/upload_images/3373235-89b056829cfb3434.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
图片来源于网络
文章来源：[http](http://www.cnblogs.com/ZachRobin/p/9407029.html)：[//www.cnblogs.com/ZachRobin/p/9407029.html](http://www.cnblogs.com/ZachRobin/p/9407029.html)
作者：Clark_new 
链接：HTTPS：//www.jianshu.com/p/3e2ebb67a25f 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
