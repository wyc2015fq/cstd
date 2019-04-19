# 高手速成android开源项目【tool篇】 - u013366022的专栏 - CSDN博客
2014年05月14日 15:47:20[slitaz](https://me.csdn.net/u013366022)阅读数：476
主要包括那些不错的开发库，包括依赖注入框架、图片缓存、网络相关、数据库ORM建模、Android公共库、Android 高版本向低版本兼容、多媒体相关及其他。
#### [](https://github.com/Trinea/android-open-project#%E4%B8%80%E4%BE%9D%E8%B5%96%E6%B3%A8%E5%85%A5di)一、依赖注入DI
通过依赖注入减少View、服务、资源简化初始化，事件绑定等重复繁琐工作
- 
AndroidAnnotations(Code Diet)android快速开发框架
项目地址：[https://github.com/excilys/androidannotations](https://github.com/excilys/androidannotations)
文档介绍：[https://github.com/excilys/androidannotations/wiki](https://github.com/excilys/androidannotations/wiki)
官方网站：[http://androidannotations.org/](http://androidannotations.org/)
特点：(1) 依赖注入：包括view，extras，系统服务，资源等等
(2) 简单的线程模型，通过annotation表示方法运行在ui线程还是后台线程
(3) 事件绑定：通过annotation表示view的响应事件，不用在写内部类
(4) REST客户端：定义客户端接口，自动生成REST请求的实现
(5) 没有你想象的复杂：AndroidAnnotations只是在在编译时生成相应子类
(6) 不影响应用性能：仅50kb，在编译时完成，不会对运行时有性能影响。
PS：与roboguice的比较：roboguice通过运行时读取annotations进行反射，所以可能影响应用性能，而AndroidAnnotations在编译时生成子类，所以对性能没有影响
- 
roboguice 帮你处理了很多代码异常，利用annotation使得更少的代码完成项目
项目地址：[https://github.com/roboguice/roboguice](https://github.com/roboguice/roboguice)
文档介绍：[https://github.com/roboguice/roboguice/wiki](https://github.com/roboguice/roboguice/wiki)
- 
butterknife 利用annotation帮你快速完成View的初始化，减少代码
项目地址：[https://github.com/JakeWharton/butterknife](https://github.com/JakeWharton/butterknife)
文档介绍：[http://jakewharton.github.io/butterknife/](http://jakewharton.github.io/butterknife/)
- 
Dagger 依赖注入，适用于Android和Java
项目地址：[https://github.com/square/dagger](https://github.com/square/dagger)
文档介绍：[http://square.github.io/dagger/](http://square.github.io/dagger/)
#### [](https://github.com/Trinea/android-open-project#%E4%BA%8C%E5%9B%BE%E7%89%87%E7%BC%93%E5%AD%98)二、图片缓存
- 
Android-Universal-Image-Loader 图片缓存
目前使用最广泛的图片缓存，支持主流图片缓存的绝大多数特性。
项目地址：[https://github.com/nostra13/Android-Universal-Image-Loader](https://github.com/nostra13/Android-Universal-Image-Loader)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/universal-imageloader-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/universal-imageloader-demo.apk?raw=true)
文档介绍：[http://www.intexsoft.com/blog/item/74-universal-image-loader-part-3.html](http://www.intexsoft.com/blog/item/74-universal-image-loader-part-3.html)
- 
picasso square开源的图片缓存
项目地址：[https://github.com/square/picasso](https://github.com/square/picasso)
文档介绍：[http://square.github.io/picasso/](http://square.github.io/picasso/)
特点：(1)可以自动检测adapter的重用并取消之前的下载
(2)图片变换
(3)可以加载本地资源
(4)可以设置占位资源
(5)支持debug模式
- 
ImageCache 图片缓存，包含内存和Sdcard缓存
项目地址：[https://github.com/Trinea/AndroidCommon](https://github.com/Trinea/AndroidCommon)
Demo地址：[https://play.google.com/store/apps/details?id=cn.trinea.android.demo](https://play.google.com/store/apps/details?id=cn.trinea.android.demo)
文档介绍：[http://www.trinea.cn/?p=704](http://www.trinea.cn/?p=704)
特点：(1)支持预取新图片，支持等待队列
(2)包含二级缓存，可自定义文件名保存规则
(3)可选择多种缓存算法(FIFO、LIFO、LRU、MRU、LFU、MFU等13种)或自定义缓存算法
(4)可方便的保存及初始化恢复数据
(5)支持不同类型网络处理
(6)可根据系统配置初始化缓存等
#### [](https://github.com/Trinea/android-open-project#%E4%B8%89%E7%BD%91%E7%BB%9C%E7%9B%B8%E5%85%B3)三、网络相关
- 
Asynchronous Http Client for Android Android异步Http请求
项目地址：[https://github.com/loopj/android-async-http](https://github.com/loopj/android-async-http)
文档介绍：[http://loopj.com/android-async-http/](http://loopj.com/android-async-http/)
特点：(1) 在匿名回调中处理请求结果
(2) 在UI线程外进行http请求
(3) 文件断点上传
(4) 智能重试
(5) 默认gzip压缩
(6) 支持解析成Json格式
(7) 可将Cookies持久化到SharedPreferences
- 
android-query 异步加载，更少代码完成Android加载
项目地址：[https://github.com/androidquery/androidquery或https://code.google.com/p/android-query/](https://github.com/androidquery/androidquery%E6%88%96https://code.google.com/p/android-query/)
文档介绍：[https://code.google.com/p/android-query/#Why_AQuery](https://code.google.com/p/android-query/#Why_AQuery)?
Demo地址：[https://play.google.com/store/apps/details?id=com.androidquery](https://play.google.com/store/apps/details?id=com.androidquery)
特点：[https://code.google.com/p/android-query/#Why_AQuery](https://code.google.com/p/android-query/#Why_AQuery)?
- 
Async Http Client Java异步Http请求
项目地址：[https://github.com/AsyncHttpClient/async-http-client](https://github.com/AsyncHttpClient/async-http-client)
文档介绍：[http://sonatype.github.io/async-http-client/](http://sonatype.github.io/async-http-client/)
- 
Ion 支持图片、json、http post等异步请求
项目地址：[https://github.com/koush/ion](https://github.com/koush/ion)
文档介绍：[https://github.com/koush/ion#more-examples](https://github.com/koush/ion#more-examples)
- 
HttpCache Http缓存
项目地址：[https://github.com/Trinea/AndroidCommon](https://github.com/Trinea/AndroidCommon)
Demo地址：[https://play.google.com/store/apps/details?id=cn.trinea.android.demo](https://play.google.com/store/apps/details?id=cn.trinea.android.demo)
Demo代码：[https://github.com/Trinea/AndroidDemo/blob/master/src/cn/trinea/android/demo/HttpCacheDemo.java](https://github.com/Trinea/AndroidDemo/blob/master/src/cn/trinea/android/demo/HttpCacheDemo.java)
特点是：(1) 根据cache-control、expires缓存http请求
(2) 支持同步、异步Http请求
(3) 在匿名回调中处理请求结果
(4) 在UI线程外进行http请求
(5) 默认gzip压缩
- 
Http Request
项目地址：[https://github.com/kevinsawicki/http-request](https://github.com/kevinsawicki/http-request)
文档介绍：[https://github.com/kevinsawicki/http-request#examples](https://github.com/kevinsawicki/http-request#examples)
- 
okhttp square开源的http工具类
项目地址：[https://github.com/square/okhttp](https://github.com/square/okhttp)
文档介绍：[http://square.github.io/okhttp/](http://square.github.io/okhttp/)
特点：(1) 支持SPDY([http://zh.wikipedia.org/wiki/SPDY)协议。SPDY协议是Google开发的基于传输控制协议的应用层协议，通过压缩，多路复用(一个TCP链接传送网页和图片等资源)和优先级来缩短加载时间。](http://zh.wikipedia.org/wiki/SPDY)%E5%8D%8F%E8%AE%AE%E3%80%82SPDY%E5%8D%8F%E8%AE%AE%E6%98%AFGoogle%E5%BC%80%E5%8F%91%E7%9A%84%E5%9F%BA%E4%BA%8E%E4%BC%A0%E8%BE%93%E6%8E%A7%E5%88%B6%E5%8D%8F%E8%AE%AE%E7%9A%84%E5%BA%94%E7%94%A8%E5%B1%82%E5%8D%8F%E8%AE%AE%EF%BC%8C%E9%80%9A%E8%BF%87%E5%8E%8B%E7%BC%A9%EF%BC%8C%E5%A4%9A%E8%B7%AF%E5%A4%8D%E7%94%A8(%E4%B8%80%E4%B8%AATCP%E9%93%BE%E6%8E%A5%E4%BC%A0%E9%80%81%E7%BD%91%E9%A1%B5%E5%92%8C%E5%9B%BE%E7%89%87%E7%AD%89%E8%B5%84%E6%BA%90)%E5%92%8C%E4%BC%98%E5%85%88%E7%BA%A7%E6%9D%A5%E7%BC%A9%E7%9F%AD%E5%8A%A0%E8%BD%BD%E6%97%B6%E9%97%B4%E3%80%82)
(2) 如果SPDY不可用，利用连接池减少请求延迟
(3) Gzip压缩
(4) Response缓存减少不必要的请求
- 
Retrofit RESTFUL API设计
项目地址：[https://github.com/square/retrofit](https://github.com/square/retrofit)
文档介绍：[http://square.github.io/retrofit/](http://square.github.io/retrofit/)
#### [](https://github.com/Trinea/android-open-project#%E5%9B%9B%E6%95%B0%E6%8D%AE%E5%BA%93-orm%E5%B7%A5%E5%85%B7%E5%8C%85)四、数据库 orm工具包
orm的db工具类，简化建表、查询、更新、插入、事务、索引的操作
- 
greenDAO Android Sqlite orm的db工具类
项目地址：[https://github.com/greenrobot/greenDAO](https://github.com/greenrobot/greenDAO)
文档介绍：[http://greendao-orm.com/documentation/](http://greendao-orm.com/documentation/)
官方网站：[http://greendao-orm.com/](http://greendao-orm.com/)
特点：(1) 性能佳
(2) 简单易用的API
(3) 内存小好小
(4) 库大小小
- 
ActiveAndroid Android Sqlite orm的db工具类
项目地址：[https://github.com/pardom/ActiveAndroid](https://github.com/pardom/ActiveAndroid)
文档介绍：[https://github.com/pardom/ActiveAndroid/wiki/_pages](https://github.com/pardom/ActiveAndroid/wiki/_pages)
- 
Sprinkles Android Sqlite orm的db工具类
项目地址：[https://github.com/emilsjolander/sprinkles](https://github.com/emilsjolander/sprinkles)
文档介绍：[http://emilsjolander.github.io/blog/2013/12/18/android-with-sprinkles/](http://emilsjolander.github.io/blog/2013/12/18/android-with-sprinkles/)
特点：比较显著的特点就是配合[https://github.com/square/retrofit能保存从服务器获取的数据](https://github.com/square/retrofit%E8%83%BD%E4%BF%9D%E5%AD%98%E4%BB%8E%E6%9C%8D%E5%8A%A1%E5%99%A8%E8%8E%B7%E5%8F%96%E7%9A%84%E6%95%B0%E6%8D%AE)
- 
ormlite-android
项目地址：[https://github.com/j256/ormlite-android](https://github.com/j256/ormlite-android)
文档介绍：[http://ormlite.com/sqlite_java_android_orm.shtml](http://ormlite.com/sqlite_java_android_orm.shtml)
#### [](https://github.com/Trinea/android-open-project#%E4%BA%94android%E5%85%AC%E5%85%B1%E5%BA%93)五、Android公共库
- 
Guava Google的基于java1.6的类库集合的扩展项目
包括collections, caching, primitives support, concurrency libraries, common annotations, string processing, I/O等等. 这些高质量的API可以使你的JAVa代码更加优雅，更加简洁
项目地址：[https://code.google.com/p/guava-libraries/](https://code.google.com/p/guava-libraries/)
文档介绍：[https://code.google.com/p/guava-libraries/wiki/GuavaExplained](https://code.google.com/p/guava-libraries/wiki/GuavaExplained)
- 
AndroidCommon Android公共库
项目地址：[https://github.com/Trinea/AndroidCommon](https://github.com/Trinea/AndroidCommon)
Demo地址：[https://play.google.com/store/apps/details?id=cn.trinea.android.demo](https://play.google.com/store/apps/details?id=cn.trinea.android.demo)
文档介绍：[http://www.trinea.cn/?p=778](http://www.trinea.cn/?p=778)
包括：(1)缓存(图片缓存、预取缓存、网络缓存)
(2) 公共View(下拉及底部加载更多ListView、底部加载更多ScrollView、滑动一页Gallery)
(3) Android常用工具类(网络、下载、Android资源操作、shell、文件、Json、随机数、Collection等等)
#### [](https://github.com/Trinea/android-open-project#%E5%85%ADandroid-%E9%AB%98%E7%89%88%E6%9C%AC%E5%90%91%E4%BD%8E%E7%89%88%E6%9C%AC%E5%85%BC%E5%AE%B9)六、Android 高版本向低版本兼容
- 
ActionBarSherlock 为Android所有版本提供统一的ActionBar，解决4.0以下ActionBar的适配问题
项目地址：[https://github.com/JakeWharton/ActionBarSherlock](https://github.com/JakeWharton/ActionBarSherlock)
Demo地址：[https://play.google.com/store/apps/details?id=com.actionbarsherlock.sample.demos](https://play.google.com/store/apps/details?id=com.actionbarsherlock.sample.demos)
APP示例：太多了。。现在连google都在用
- 
Nine Old Androids 将Android 3.0(Honeycomb)所有动画API(ObjectAnimator ValueAnimator等)兼容到Android1.0
项目地址：[https://github.com/JakeWharton/NineOldAndroids](https://github.com/JakeWharton/NineOldAndroids)
Demo地址：[https://play.google.com/store/apps/details?id=com.jakewharton.nineoldandroids.sample](https://play.google.com/store/apps/details?id=com.jakewharton.nineoldandroids.sample)
文档介绍：[http://nineoldandroids.com/](http://nineoldandroids.com/)
- 
HoloEverywhere 将Android 3.0的Holo主题兼容到Android2.1++
项目地址：[https://github.com/Prototik/HoloEverywhere](https://github.com/Prototik/HoloEverywhere)
Demo地址：[https://raw.github.com/Prototik/HoloEverywhere/repo/org/holoeverywhere/demo/2.1.0/demo-2.1.0.apk](https://raw.github.com/Prototik/HoloEverywhere/repo/org/holoeverywhere/demo/2.1.0/demo-2.1.0.apk)
文档介绍：[http://android-developers.blogspot.com/2012/01/holo-everywhere.html](http://android-developers.blogspot.com/2012/01/holo-everywhere.html)
#### [](https://github.com/Trinea/android-open-project#%E4%B8%83%E5%A4%9A%E5%AA%92%E4%BD%93%E7%9B%B8%E5%85%B3)七、多媒体相关
- 
cocos2d-x 跨平台的2d游戏框架，支持Android、IOS、Linux、Windows等众多平台
项目地址：[https://github.com/cocos2d/cocos2d-x](https://github.com/cocos2d/cocos2d-x)
文档介绍：[http://www.cocos2d-x.org/wiki](http://www.cocos2d-x.org/wiki)
官方网站：[http://www.cocos2d-x.org/](http://www.cocos2d-x.org/)
- 
Vitamio 是一款Android与iOS平台上的全能多媒体开发框架
项目地址：[https://github.com/yixia/VitamioBundle](https://github.com/yixia/VitamioBundle)
网站介绍：[http://www.vitamio.org/docs/](http://www.vitamio.org/docs/)
特点：(1) 全面支持硬件解码与GPU渲染
(2) 能够流畅播放720P甚至1080P高清MKV，FLV，MP4，MOV，TS，RMVB等常见格式的视频
(3) 在Android与iOS上跨平台支持 MMS, RTSP, RTMP, HLS(m3u8)等常见的多种视频流媒体协议，包括点播与直播。
- 
PhotoProcessing 利用ndk处理图片库，支持Instafix、Ansel、Testino、XPro、Retro、BW、Sepia、Cyano、Georgia、Sahara、HDR、Rotate、Flip
项目地址：[https://github.com/lightbox/PhotoProcessing](https://github.com/lightbox/PhotoProcessing)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/photo-processing.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/photo-processing.apk?raw=true)
- 
Android StackBlur 图片模糊效果工具类
项目地址：[https://github.com/kikoso/android-stackblur](https://github.com/kikoso/android-stackblur)
Demo地址：[https://github.com/kikoso/android-stackblur/blob/master/StackBlurDemo/bin/StackBlurDemo.apk?raw=true](https://github.com/kikoso/android-stackblur/blob/master/StackBlurDemo/bin/StackBlurDemo.apk?raw=true)
文档介绍：[https://github.com/kikoso/android-stackblur#usage](https://github.com/kikoso/android-stackblur#usage)
#### [](https://github.com/Trinea/android-open-project#%E5%85%AB%E5%85%B6%E4%BB%96-1)八、其他
- 
Salvage view 带View缓存的Viewpager PagerAdapter，很方便使用
项目地址：[https://github.com/JakeWharton/salvage](https://github.com/JakeWharton/salvage)
- 
Android-PasscodeLock 应用锁，每次启动或从任何Activity启动应用都需要输入四位数字的密码方可进入
项目地址：[https://github.com/wordpress-mobile/Android-PasscodeLock](https://github.com/wordpress-mobile/Android-PasscodeLock)
Demo地址：[https://play.google.com/store/apps/details?id=com.sothree.umano](https://play.google.com/store/apps/details?id=com.sothree.umano)
APP示例：Wordpress Android，支付宝，挖财
- 
android-lockpattern Android的图案密码解锁
项目地址：[https://code.google.com/p/android-lockpattern/](https://code.google.com/p/android-lockpattern/)
Demo地址：[https://play.google.com/store/apps/details?id=group.pals.android.lib.ui.lockpattern.demo](https://play.google.com/store/apps/details?id=group.pals.android.lib.ui.lockpattern.demo)
使用介绍：[https://code.google.com/p/android-lockpattern/wiki/QuickUse](https://code.google.com/p/android-lockpattern/wiki/QuickUse)
示例APP：Android开机的图案密码解锁，支付宝的密码解锁
- 
GlowPadBackport将Android4.2的锁屏界面解锁扩展到Android1.6及1.6+
项目地址：[https://github.com/rock3r/GlowPadBackport](https://github.com/rock3r/GlowPadBackport)
Demo地址：[https://play.google.com/store/apps/details?id=net.sebastianopoggi.samples.ui.GlowPadSample](https://play.google.com/store/apps/details?id=net.sebastianopoggi.samples.ui.GlowPadSample)
效果图：![Renderings](https://lh6.ggpht.com/U070b6Lh6cVsVwx4jN-5nq0xqiB1PBzrYABPeJIEe2hZQ5UWOxc-FDUG77wADelToHA=h310-rw)
- 
GlowPadView Android4锁屏界面解锁
项目地址：[https://github.com/nadavfima/GlowPadView](https://github.com/nadavfima/GlowPadView)
效果图：![Renderings](https://raw.github.com/nadavfima/GlowPadView/master/example.png)
- 
Android Priority Job Queue Android后台任务队列
项目地址：[https://github.com/path/android-priority-jobqueue](https://github.com/path/android-priority-jobqueue)
文档介绍：[https://github.com/path/android-priority-jobqueue#getting-started](https://github.com/path/android-priority-jobqueue#getting-started)
- 
jsoup 一个解析html的java库，可方便的提取和操作数据
项目地址：[https://github.com/jhy/jsoup](https://github.com/jhy/jsoup)
官方网站：[http://jsoup.org/](http://jsoup.org/)
作用：(1) 从一个url、文件或string获得html并解析
(2) 利用dom遍历或css选择器查找、提取数据
(3) 操作html元素
(4) 根据白名单去除用于提交的非法数据防止xss攻击
(5) 输出整齐的html
- 
ZIP java压缩和解压库
项目地址：[https://github.com/zeroturnaround/zt-zip](https://github.com/zeroturnaround/zt-zip)
文档介绍：[https://github.com/zeroturnaround/zt-zip#examples](https://github.com/zeroturnaround/zt-zip#examples)
作用：(1) 解压和压缩，并支持文件夹内递归操作
(2) 支持包含和排除某些元素
(3) 支持重命名元素
(4) 支持遍历zip包内容
(5) 比较两个zip包等功能
- 
Cobub Razor 开源的mobile行为分析系统，包括web端、android端，支持ios和window phone
项目地址：[https://github.com/cobub/razor](https://github.com/cobub/razor)
Demo地址：[http://demo.cobub.com/razor](http://demo.cobub.com/razor)
网站介绍：[http://dev.cobub.com/](http://dev.cobub.com/)
- 
aFileChooser 文件选择器，可内嵌到程序中，而无需使用系统或三方文件选择器。
项目地址：[https://github.com/iPaulPro/aFileChooser](https://github.com/iPaulPro/aFileChooser)
- 
androidpn 基于xmpp协议的消息推送解决方案，包括服务器端和android端。
项目地址：[https://github.com/dannytiehui/androidpn](https://github.com/dannytiehui/androidpn)
- 
Android插件式开发
项目地址：[https://github.com/umeng/apf](https://github.com/umeng/apf)
