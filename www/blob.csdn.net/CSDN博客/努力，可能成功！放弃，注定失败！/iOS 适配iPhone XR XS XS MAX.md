# iOS 适配iPhone XR/XS/XS MAX - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月19日 10:56:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：430
适配只看三个参数
> 
1.渲染像素--屏幕截图的图片尺寸，单位px
2.逻辑像素--程序员开发所用尺寸，单位pt
3.倍率--渲染像素/逻辑像素，得到倍数关系。常见倍率@2x,@3x
Safe Area（安全区）
> 
1.页面内容不能超出安全区（Safe Area）
2.底部区域用于手势进入主屏或切换应用，如果将触发交互行为的按钮放在屏幕的底部，会破坏App操作体验。
![](https://upload-images.jianshu.io/upload_images/3069532-ab003f54776ecfe8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/792/format/webp)
安全区
iphoneX和XR/XS/XS MAX安全区计算方式相同
![](https://upload-images.jianshu.io/upload_images/3069532-ee7940e319181b54.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/713/format/webp)
安全区计算方式
> 
通过友盟关于iOS屏幕分辨率活跃度占比分析，640 * 960的活跃度跌破10%。
如果除去640 * 1136，iPhone在逻辑像素层面，只剩余两种宽度、四种高度。
![](https://upload-images.jianshu.io/upload_images/3069532-a71daa3805f8258e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/798/format/webp)
两种宽度，四种高度
常用宏
```
// 判断是否是ipad
#define isPad ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
// 判断iPhone4系列
#define kiPhone4 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 960), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPhone5系列
#define kiPhone5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPhone6系列
#define kiPhone6 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(750, 1334), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
//判断iphone6+系列
#define kiPhone6Plus ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(1242, 2208), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPhoneX
#define IS_IPHONE_X ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(1125, 2436), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPHoneXr
#define IS_IPHONE_Xr ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(828, 1792), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPhoneXs
#define IS_IPHONE_Xs ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(1125, 2436), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
// 判断iPhoneXs Max
#define IS_IPHONE_Xs_Max ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(1242, 2688), [[UIScreen mainScreen] currentMode].size) && !isPad : NO)
#define Height_StatusBar ((IS_IPHONE_X == YES || IS_IPHONE_Xr == YES || IS_IPHONE_Xs == YES || IS_IPHONE_Xs_Max == YES) ? 44.0 : 20.0)
#define Height_NavBar ((IS_IPHONE_X == YES || IS_IPHONE_Xr == YES || IS_IPHONE_Xs == YES || IS_IPHONE_Xs_Max == YES) ? 88.0 : 64.0)
#define Height_TabBar ((IS_IPHONE_X == YES || IS_IPHONE_Xr == YES || IS_IPHONE_Xs == YES || IS_IPHONE_Xs_Max == YES) ? 83.0 : 49.0)
```
> 
附：iPhone全尺寸
![](https://upload-images.jianshu.io/upload_images/3069532-86fc67bac9774884.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
iPhone全家福
> 
参考文章
[http://www.ui.cn/detail/401348.html](http://www.ui.cn/detail/401348.html)
[https://www.jianshu.com/p/80535a2867b9](https://www.jianshu.com/p/80535a2867b9)
[https://www.jianshu.com/p/8d1d02a21d67](https://www.jianshu.com/p/8d1d02a21d67)
[https://blog.csdn.net/sinat_15735647/article/details/82762238](https://blog.csdn.net/sinat_15735647/article/details/82762238)
作者：iOS安年
链接：https://www.jianshu.com/p/3e010551f5f1
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
