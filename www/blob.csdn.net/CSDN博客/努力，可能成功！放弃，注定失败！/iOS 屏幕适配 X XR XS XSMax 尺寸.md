# iOS 屏幕适配 X XR XS XSMax 尺寸 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月03日 10:34:58[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：84
🍎又发布了新手机
看来要选择新的支付方式了
![](https://upload-images.jianshu.io/upload_images/1484644-dca9ad67df736336.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/515/format/webp)
image
![](https://upload-images.jianshu.io/upload_images/1484644-ce78e9b04b6da6e0.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/698/format/webp)
image
废话不多说先列下最新系列屏幕尺寸和以往主流屏幕尺寸
（所列均为[[UIScreen mainScreen] bounds].size）
![](https://upload-images.jianshu.io/upload_images/1484644-42a19e4ca129227b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/724/format/webp)
WX20180920-112731.png
![](https://upload-images.jianshu.io/upload_images/1484644-9498603da6e0973f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/697/format/webp)
WX20180920-112736.png
```
XR
width 414.0f
height 896.0f
xs
width 375.0f
height 812.0f
xs max
width 414.0f
height 896.0f
x
width 375.0f
height 812.0f
8plus
width 414.0f
height 736.0f
8
width 375.0f
height 667.0f
7 plus
width 414.0f
height 736.0f
7
width 375.0f
height 667.0f
6s plus
width 414.0f
height 736.0f
6s
width 375.0f
height 667.0f
6plus
width 414.0f
height 736.0f
6
width 375.0f
height 667.0f
se
width 320.0f
height 568.0f
5s
width 320.0f
height 568.0f
```
顶部高度的话
![](https://upload-images.jianshu.io/upload_images/1484644-5f150008e649c80f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/696/format/webp)
WX20180920-102354.png
![](https://upload-images.jianshu.io/upload_images/1484644-c87bb9f8541f63a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/721/format/webp)
WX20180920-102402.png
（图片网络资源 侵删）
送一些适配的宏
```
#define DEVICE_WIDTH  [[UIScreen mainScreen] bounds].size.width
#define DEVICE_HEIGHT  [[UIScreen mainScreen] bounds].size.height
//iPhone X or XS
#define iPhoneX     (DEVICE_WIDTH == 375.f && DEVICE_HEIGHT == 812.f)
//iPhone XR or XS Max
#define iPhoneXR    (DEVICE_WIDTH == 414.f && DEVICE_HEIGHT == 896.f)
#define iPhoneMaxScreen (DEVICE_WIDTH >= 375.f && DEVICE_HEIGHT >= 812.f)
//底部虚拟home键高度 一般用于最底部view到底部的距离 
#define VirtualHomeHeight (iPhoneMaxScreen ? 34.f : 0.f)
//顶部适配的话 系统有参数可以直接获取到
#define STATUS_BAR_FRAME [[UIApplication sharedApplication] statusBarFrame]
```
作者：Bc_wh1te_Le1
链接：https://www.jianshu.com/p/5208fbfe28fb
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
