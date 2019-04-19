# 「 iOS干货 」提高代码简洁的方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月05日 20:27:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：250标签：[iOS学习资料																[iOS 资源大全																[iOS常用方法](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
### 前言
开发项目的时候，常常会有一些代码重复而且冗余，在用的时候需要写很长的代码或者用的地方也多，所以这个时候就需要集中起来封装，使代码复用性强，用短短的几行代码搞定。这些方法我都封装在`category、tools、View`，加上代码里都有写注释，用起来很方便，实用性强，读起来也易懂，可以让代码更加的简洁。里面有一些也是参考别人源码，也做了下搬运工，希望对你们开发项目有所帮助。
### 简介
###### WTSDK
![](http://upload-images.jianshu.io/upload_images/313852-9cb44b55010e3d94.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
WTSDK目录.png
看目录结构就觉得清晰易懂，反正我是这么觉得的☺️
###### Category（分类）
![](http://upload-images.jianshu.io/upload_images/313852-587c6fdd1beac01a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Category.png
###### NS（NS前缀的分类）
![](http://upload-images.jianshu.io/upload_images/313852-6d81e58cb62b89c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NS.png
###### UI（UI前缀的分类）
![](http://upload-images.jianshu.io/upload_images/313852-0edd13072ec89677.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UI.png
看`class名称`，它们都是对应对象的分类方法，里面扩展了很多实用性的功能，具体可以看源码。
###### Tool（工具类集合）
![](http://upload-images.jianshu.io/upload_images/313852-85a3b882287d9d02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Tool.png
`Singleton`是一个单例模式的宏使用方法：
在`.h`里写上`singleton_interface(类名称)`与`.m`里写上`singleton_implementation(类名称)`即完美支持MRC和ARC下的单例使用。
`WTConst`里面包括很多实用性很强的宏定义，具体看源码。
`WTUtility`里面定义了很多实用性很强的类方法，具体看源码。
###### View（UITextField与UITextView）
![](http://upload-images.jianshu.io/upload_images/313852-73e7ca5f371de3fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
View.png
`View`文件夹里包括`UITextField`与`UITextView`原生控件的封装。我一直觉得这两个原生控件每次用的时候都要加入好多的判断与代理方法，而且都是差不多一样的，所以我就针对这两个进行了封装，把常用的功能都集合在里面，比如限制文字输入长度等等，这样使用起来才更加的简洁。具体可以看下源码。
### 使用方法
你可以看下`WTSDK`里面有啥需要的，单独拉某个文件到你的项目里使用即可。
如果你都要使用的话，直接导入`#import "WTConst.h"`即可。
注意如果用到`UIViewController+WT`还需要项目里有[MBProgressHUD](https://github.com/jdg/MBProgressHUD)这个库。
#### 源码在这
[GitHub](https://github.com/Tate-zwt/WTSDK) 如果你觉得不错的话，麻烦你点一下Star，thank you all!
文／Tate_zwt（简书作者）
原文链接：http://www.jianshu.com/p/ec1684b0fad9
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
