# iOS开发--TTFramework - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月19日 15:47:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：604标签：[iOS学习资料																[ios开发																[iOS 资源大全																[Framework](https://so.csdn.net/so/search/s.do?q=Framework&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/2363764-5727154e8383e55a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 本文来自简书，原文地址：[http://www.jianshu.com/p/0bd0ec075409](http://www.jianshu.com/p/0bd0ec075409)
# 初衷
接触iOS开发已经有一年多的时间了,在这一年多的时间里,学到了很多,现在依照自己日常开发总结了一套自己的Framework,相关的项目已经上传到Github,,这篇文章会面针对TTFrameWork中的结构进行相关的说明.
# TTFramework
### 一. TTShare
这样是涉及到的项目中的分享模块,这里支持常用的QQ(包括空间),微信(包括朋友圈),新浪.
- 
##### ShareDataSource
这里面存放的是分享的原生SDK和相对应的分享类
- 
##### ShareModel
这里是涉及到的相关的分享Model,如获取授权的用户的数据模型
- 
##### TTBaseShareCenter
每个分享的基础类,相对应的子类为TTSinaCenter, TTTencentCenter, TTWeChatCenter
### 二. Common
这是一个工具类,集合了项目中实际用到的方法
- 
#### Category
项目中用到的扩展,举几个栗子
- 
###### NSString+TTString
这是一个有关字符串的扩展,主要是对字符串进行判断,如字符串的判空isNilOrEmpty, 获取加密的MD5:md5Str等等.
- 
###### UIView+TTSuperView
这里主要是针对UIView进行的分类,内容包括获取View的各种信息,如width,height.同时可以设置View的数值属性,并设置动画.
另外,还可以针对View的Layer层进行操作,如弧度或者圆等等.
- 
###### UIDevice+TTDevide
UIDevice的分类,这里可以获取当前手机的系统号,手机的类型,手机的版本等各项信息.
- 
###### NSUserDefaults+TTUserDefaults
数据持久化的解决方案之一,方便进行数据保存,可以保存Bool,Array,Value等类型,同时也可以进行读取
- 
###### NSDate+TTDate
时间分类,可进行时间的获取,时间的格式化,,时间戳的转化等
- 
#### Libs
这里第三方应用的集合
- 
##### SSZipArchive
解压Zip文件
- 
##### AliyunOSS
阿里云服务器的SDK
TTAliyunOSSManager,阿里云上传信息的管理类,可以上传图片和视频
- 
##### SMS_SDK
Mob短信的SDK,其中自定义TTSMSManager,封装相关的获取和验证的功能
- 
##### CustomPopAnimation
自定义过场动画,点击区域圆圈放大效果
- 
##### NewWorkReachability
自定义网络判断,参考Reachability封装的可以支持4G
- 
##### SDCycleScrollView
收录的一个轮播图,使用简单
- 
##### CalendarPopView
收录的一个日历,效果还不错,Layer层的动画(目前该日历在github上面已经更新,但是针对项目而言,还是比较喜欢之前的版本)
![](http://upload-images.jianshu.io/upload_images/2363764-0c28ebca4a652e6d.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
日历.PNG
- 
##### PrintToolsChinese
控制台字典或者数字打印中文
- 
##### MLImageCrop
图片的截取,带截取框
- 
##### SDWebImage
不多说,图片下载
- 
#### Tools
自定义Tool工具,用于项目中针对部分功能的封装,接下来,介绍几个常用的:
- 
##### TTTools
项目常用Tool,包括文本在Label中的尺寸计算,字符串收尾空格去除,MD5加密等,还包括手机号,邮箱等正则.
- 
##### AppPurchaseManager
内购管理
- 
##### AutoLayoutLabelView
Label长度自适应,一般用于热门推荐的标签
![](http://upload-images.jianshu.io/upload_images/2363764-6a7780e1d88a5cd5.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
AutoLayoutLabelView.PNG
- 
##### CleanLocalCache
缓存清除,用于项目中图片的缓存,SDWebImage的Clear的封装,并在清除完成之后发送通知
- 
##### AllowMobileNetManager
用户选择视频,音频或者耗流量的使用移动网络数据的管理
- 
##### TTAudioTool
音频播放工具,包括音频的播放,暂停等等功能
- 
#### TTConst
这个是项目中的一个常用的宏定义整合或者Public类的NSString,如CachePath沙盒路径,KEY_WINDOW窗口等等
PS:在项目中建议自定义一个Const,用于常用的宏定义,比较实用
### 三. ReasourceFiles
目前TTFrameWork相关内容所需要的文件.
### 四. TTNetWork
基于AFN做的进一步的封装.具体的内容建议下载TTFramework详细了解
- 
##### Support
AFNetworking的SDK
- 
##### TTBaseRequest
请求基础类,这个可以看做是链接项目中的请求数据和AFNetWorking的中间件
- 
##### TTRequest
项目的实际数据请求类,继承自TTBaseRequest,并实现了针对项目的具体方法,同时也是针对每个接口的基础类
### 五. TTHUDMessage
自定义的HUD,基于MBProgressHUD做的封装,封装了几种常用的显示等待的方案,具体的可以看项目中的内容
- 
##### TTHUDMessage
项目中实际用到的加载等待的类
### 六. Model
基于MJExtension做的相关的封装,涉及到模型的转换等
- 
##### TTBaseModel
基于MJExtension做的Model的基础类,用于和服务器对接,字典转数据模型,数据模型转字典,而且支持数组中的模型数组转换.并且支持Model归档.
##### PS:在项目中的具体的model需要继承自TTBaseModel,并且实现相关的功能,如替换,属性为自定义类的实现等等
### 七. View
自定义的View,针对项目中常用的View的继承和封装,下面简单的列举几种:
- 
##### Carousel
收录的强大的轮播图,自定义CarouselManager针对iCarousel使用功能进行封装.具体的可以参考Github上的iCarousel
- 
##### TTBottomBarView
位于底部的View,常用于项目中的评论等等
- 
##### TableViewAndCollection
基于UITableView和UICollectionView做的下拉刷新和上拉加载(加载更多),分别为TTTableView 和TTCollectionView
共用同一套HeaderView和FooterView
相对应的文件中均有相关的Annotation_XX说明文件
这里需要注意的是:
#### 在TTCollectionView中收录了两套瀑布流,并做出了相关人性化的调整
- 
##### TTTextView
针对需求选择区域输入,从左上角开始的UITextView,添加设置占位符和占位符的颜色
- 
##### TTAlertView
这里面主要分为两中,一种是对系统的UIAlertView的使用进行的封装,另一种动画的FRAlertView(继承自UIView),方面使用多种设置类型.
### 八. Controller
- 
##### TTBaseViewController
Controller的基类,主要设置了相关的Controller属性,融合了Navigation的相关设置,如左侧按钮,右侧按钮(们),导航栏的颜色,状态栏等,同时,可以设置当前Controller的方向等等
> 
需要注意的是,如果当前控制器存在数据请求,在实际的控制其中,需要在dealloc中调用cancelAllRequest,这个需要适当调用
- 
##### TTTabBarController
用于项目中自定义的TabBar
### 总结
这个TTFrameWork总结了日常开发会遇到的一些东西,同时也收录的很多网上的优秀的第三方.这个FrameWork我会经常更新,针对项目中出现的很多Bug可能会做出相关的修改,同时,如果你发现有Bug,请在文章的最后,通关相关的联系方式联系我,欢迎大家多多学习和交流.
PS:这里对TTFrameWork的做的简单介绍,具体的,我在相关对应的方法中进行了详细的说明,欢迎下载TTFrameWork.
如果你感觉这个TTFrameWork对你有所帮助,希望可以去Github点亮小星星,这是对我最大的鼓励.
## 1. 项目地址:
- Github 搜索 "TTFrameWork", 作者:zhangfurun
- [TT](https://github.com/zhangfurun/TT)
## 2. 联系方式
- 文章后留言或者简书私信
- Github项目地址上有我的具体联系方式
- 
