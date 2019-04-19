# 公司用到的一些 iOS 开源库和第三方组件 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月15日 15:12:48[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：450标签：[iOS																[iOS开发																[第三方组件																[开源库](https://so.csdn.net/so/search/s.do?q=开源库&t=blog)](https://so.csdn.net/so/search/s.do?q=第三方组件&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
分享一下我目前所在公司 iOS 项目中用到的一些 iOS 开源库和第三方组件, 感谢开源, 减少了我们的劳动力, 节约了我们大量的时间, 让我们有更多的时间和精力能更加专注的做好自己的产品
欢迎大家关注[我的 GitHub](https://github.com/Aufree)或[微博](http://weibo.com/jinfali)了解更多关于我的信息 ^_^
## 用到的组件
#### 1、通过CocoaPods安装
|项目名称|项目信息|
|----|----|
|[AFNetworking](https://github.com/AFNetworking/AFNetworking)|网络请求组件|
|[FMDB](https://github.com/ccgus/fmdb)|本地数据库组件|
|[SDWebImage](https://github.com/rs/SDWebImage)|多个缩略图缓存组件|
|[UICKeyChainStore](https://github.com/kishikawakatsumi/UICKeyChainStore)|存放用户账号密码组件|
|[Reachability](https://github.com/tonymillion/Reachability)|监测网络状态|
|[DateTools](https://github.com/MatthewYork/DateTools)|友好化时间|
|[MBProgressHUD](https://github.com/jdg/MBProgressHUD)|一款提示框第三方库|
|[MWPhotoBrowser](https://github.com/mwaterfall/MWPhotoBrowser)|一款简单的 iOS 照片浏览控件|
|[CTAssetsPickerController](https://github.com/chiunam/CTAssetsPickerController)|一个选择器组件, 支持从用户的相片库选择多张照片和视频。|
|[QBPopupMenu](https://github.com/questbeat/QBPopupMenu)|无需使用图片文件的 iOS 弹出式菜单|
|[UAAppReviewManager](https://github.com/UrbanApps/UAAppReviewManager)|一个能够让你方便地将提醒用户评分的功能加入 App 的工具|
|[Mantle](https://github.com/Mantle/Mantle)|主要用来将 JSON 数据模型化为 Model 对象|
|[MTLFMDBAdapter](https://github.com/tanis2000/MTLFMDBAdapter)|Mantle 和 FMDB 的转换工具|
|[FMDBMigrationManager](https://github.com/layerhq/FMDBMigrationManager)|支持 iOS SQLite 数据库迁移|
|[Bugsnag](https://github.com/bugsnag/bugsnag-cocoa)|集成 Bugsnag 到应用中|
|[GVUserDefaults](https://github.com/gangverk/GVUserDefaults)|对 NSUserDefaults 进行了封装, 方便的进行本地化存储操作|
|[FCCurrentLocationGeocoder](https://github.com/fabiocaccamo/FCCurrentLocationGeocoder)|对用户当前地理位置进行正向地址编码和反向地址编码|
|[AFSwipeToHide](https://github.com/appfigures/AFSwipeToHide)|页面滚动时隐藏工具栏|
|[BaiduTranslate](https://github.com/summerblue/baidu-translate-ios-sdk)|非官方百度翻译工具|
|[JSBadgeView](https://github.com/JaviSoto/JSBadgeView)|添加带动画效果的未读消息数提醒|
|[Masonry](https://github.com/SnapKit/Masonry)|一个轻量级的布局框架, 用于替换官方的 AutoLayout 写法|
|[MMPlaceHolder](https://github.com/adad184/MMPlaceHolder)|可在应用中显示视图的尺寸|
|[TOWebViewController](https://github.com/TimOliver/TOWebViewController)|可在应用中嵌套 Web 页面|
#### 2、第三方类库安装
|项目名称|项目信息|
|----|----|
|[MJRefresh](https://github.com/CoderMJLee/MJRefresh)|下拉刷新，上拉加载更多组件|
|[VPImageCropper](https://github.com/windshg/VPImageCropper)|用户上传头像组件|
|[JCScrollView](https://github.com/zys456465111/JCScrollView)|快速给应用视图添加滚动分页效果|
|[BWTitlePagerView](https://github.com/brunow/BWTitlePagerView)|创建类似 Twitter 的导航栏分页效果|
|[CNPPopupController](https://github.com/carsonperrotti/CNPPopupController)|创建弹出卡片视图|
|[FeSpinner](https://github.com/NghiaTranUIT/FeSpinner)|加入沙漏等多种动画加载效果|
|[PulsingHalo](https://github.com/shu223/PulsingHalo)|涟漪动画效果|
|[JMHoledView](https://github.com/leverdeterre/JMHoledView)|方便创建用户引导视图|
|[RKNotificationHub](https://github.com/cwRichardKim/RKNotificationHub)|添加未读消息数显示提醒|
|[CRToast](https://github.com/cruffenach/CRToast)|从应用状态栏弹出消息提醒|
|[KeychainIDFA](https://github.com/qixin1106/KeychainIDFA)|获取 IDFA 标示当设备唯一识别, 并保存到 KeyChain 中|
|[DZNEmptyDataSet](https://github.com/dzenbot/DZNEmptyDataSet)|为 TableView 和 CollectionView 展示空数据视图|
|[SwipeView](https://github.com/nicklockwood/SwipeView)|快速给应用添加上滑动视图|
|[FXBlurView](https://github.com/nicklockwood/FXBlurView)|对视图进行模糊操作|
|[AreaPicker](https://github.com/cloudorz/areapicker)|省市地理位置选择器|
|[MLAudioRecorder](MLAudioRecorder)|边录边转码, 播放网络音频Button(本地缓存), 实时语音|
|[MPTextView](https://github.com/cbowns/MPTextView)|支持占位符的 UITextView 子类扩展|
|[MagicalRecord](https://github.com/magicalpanda/MagicalRecord)|帮助清除 CoreData 引用的代码, 协助 CoreData 工作|
|[THProgressView](https://github.com/tiagomnh/THProgressView)|微型加载进度条|
|[VPImageCropper](https://github.com/windshg/VPImageCropper)|轻松实现微信选取图片并裁剪成头像的控件|
|[WCAlertView](https://github.com/m1entus/WCAlertView)|对官方 Alert View 进行封装, 更加容易使用|
#### 3、第三方服务
- [友盟社会化分享组件](http://dev.umeng.com/social/ios/sdk-download)
- [友盟用户反馈](http://dev.umeng.com/images/dl.png)
- [友盟自动更新](http://dev.umeng.com/analytics/ios/sdk-download)
- [环信即时IM系统](http://www.easemob.com/downloads/iOSSDK-20141018.zip)
- [极光推送](http://docs.jpush.io/client/ios_sdk/)
