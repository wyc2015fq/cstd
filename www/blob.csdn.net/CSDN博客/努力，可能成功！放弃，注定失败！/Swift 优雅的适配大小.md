# Swift 优雅的适配大小 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:42:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：67
> 
在日常开发中常常会对设备进行一定的适配，为了方便在多个项目里统一管理和使用，所以封装并开源了`SwiftyFitsize`这个库，可用于适配视图及字体大小，同时也支持xib和故事板
GitHub：[SwiftyFitsize](https://github.com/LinXunFeng/SwiftyFitsize)
最终的效果如下图所示
![](https://upload-images.jianshu.io/upload_images/2144614-94f0994ecee3a792.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
效果图
## 安装
使用的CocoaPods安装，或手动拖入项目
```
pod 'SwiftyFitsize'
```
## 使用
`SwiftyFitsize`默认在下状况所使用的参照宽度为`iphone6`的`375`
如果设计图产品所选用设备的宽度与默认值不同，在可以`AppDelegate`下初始化所参照的宽度
```
SwiftyFitsize.reference(width: 414)
```
下面列出一些设备对应的分辨率，方便查找
|设备|逻辑分辨率（点）|设备分辨率（像素）|
|----|----|----|
|SE|320x568|640x1136|
|6（S）/ 7/8|375x667|750x1334|
|6（S）+ / 7 + / 8 +|414x736|1080×1920|
|X（S）|375x812|1125x2436|
|XR|414x896|828x1792|
|XS Max|414x896|1242x2688|
使用也是非常方便的，只需要在`Number`，`UIFont`，`CGPoint`，`CGSize`，`UIEdgeInsetsMake`这些类型的值后面加上`~`即可
```
100~
UIFont.systemFont(ofSize: 14)~
CGPoint(x: 10, y: 10)~
CGSize(width: 100, height: 100)~
CGRect(x: 10, y: 10, width: 100, height: 100)~
UIEdgeInsetsMake(10, 10, 10, 10)~
```
xib / storyboard字体适配
支持控件 `UILabel``UIButton``UITextView``UITextField`
![](https://upload-images.jianshu.io/upload_images/2144614-36ecaf5c65bacf1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/626/format/webp)
字体fitsize
xib / storyboard约束适配
![](https://upload-images.jianshu.io/upload_images/2144614-34eedf06d31603db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/686/format/webp)
约束fittsize
注：`~`请不要相互嵌套使用，如
```
CGPoint(x: 10~, y: 10~)~
```
作者：LinXunFeng 
链接：HTTPS：//www.jianshu.com/p/d0463a042543 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
