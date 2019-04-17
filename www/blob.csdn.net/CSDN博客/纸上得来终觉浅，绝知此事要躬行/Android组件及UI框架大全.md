# Android组件及UI框架大全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年08月20日 12:09:29[boonya](https://me.csdn.net/boonya)阅读数：52694标签：[android																[ui																[html5																[2d																[icons](https://so.csdn.net/so/search/s.do?q=icons&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)






原文地址：http://blog.csdn.net/smallnest/article/details/38658593

Android 是目前最流行的移动操作系统(还需要加之一吗？)。 随着新版本的不断发布， Android的功能也日益强大， 涌现了很多流行的应用程序， 也催生了一大批的优秀的组件。

本文试图将目前流行的组件收集起来以供参考， 如果你发现本文还没有列出的组件，欢迎在评论中贴出来，我会定期的更新本文。
- 
很好的中文教程
[Google Android官方培训课程中文版](https://github.com/kesenhoo/android-training-course-in-chinese)

- 
[awesome-android](https://github.com/JStumpp/awesome-android)， android列表。

- 
另，github上的一个项目， 收集了好多的Android开源项目。
[android-open-project](https://github.com/Trinea/android-open-project)。


以下是我收集和精选的Android开源项目。


# UI组件
- 
[**ActionBarSherlock**](https://github.com/JakeWharton/ActionBarSherlock)

一个功能强大的ActionBar组件(不仅仅是ActionBar)。 Android 4.0+上使用native action bar， 低版本的Android模拟实现。强大到无语了，必须关注的一个开源组件。
![](http://actionbarsherlock.com/static/feature.png)

- 
[**SlidingMenu**](https://github.com/jfeinstein10/SlidingMenu)

一款优秀的侧滑菜单组件。 Google+, YouTube和Facebook在它们的应用中采用的侧滑菜单流行开来， 国内也有很多的应用如网易新闻等也采用了这种菜单方式。 侧滑菜单组件很好几种， 尤以此款最为好用。 看看官方网站上的知名用户就知道了。

- 
[**Android-Universal-Image-Loader**](https://github.com/nostra13/Android-Universal-Image-Loader)

强大灵活的加载、缓存、显示图片的库。
![](https://github.com/nostra13/Android-Universal-Image-Loader/raw/master/UniversalImageLoader.png)

- 
[**Android-ViewPagerIndicator**](https://github.com/JakeWharton/Android-ViewPagerIndicator)

滑动页面几乎成了现在国内的应用的标配了，尤其在你第一次安装/启动应用时， 总会有几张介绍页面让你滑动显示。 用它吧，不会辜负你的。
![](https://raw.github.com/JakeWharton/Android-ViewPagerIndicator/master/sample/screens.png)

- 
[**Android-PullToRefresh**](https://github.com/chrisbanes/Android-PullToRefresh)

你不陌生， 在列表或者视图的顶部或者底部拖动可以刷新数据和页面。可以和ListView、ExpandableListView、GridView、WebView、ScrollView、HorizontalScrollView、ViewPager配合使用。
![](https://github.com/chrisbanes/Android-PullToRefresh/raw/master/header_graphic.png)

- 
[**picasso**](https://github.com/square/picasso)

强大的图像下载和缓存库。
![](http://colobu.com/square/picasso/raw/master/website/static/sample.png)

- 
[**Android-Bootstrap**](https://github.com/Bearded-Hen/Android-Bootstrap)

bootstrap风格的按钮。 使用Font Awesome图标字体。
![](https://raw.github.com/Bearded-Hen/Android-Bootstrap/master/images/device_image.png)

- 
[**facebook-android-sdk**](https://github.com/facebook/facebook-android-sdk)

Facebook提供的SDK， 可以在应用中集成Facebook.

- 
[**android-bootstrap**](https://github.com/AndroidBootstrap/android-bootstrap)

不同于上面的Android-Bootstrap。 此项目是一个Android应用程序的模版， 相当于一个脚手架的功能。
![](http://f.cl.ly/items/0e3T2F2x3M0K2l1X0A0u/Image%202013.07.08%201%3A46%3A09%20PM.png)

- 
[**AndroidStaggeredGrid**](https://github.com/etsy/AndroidStaggeredGrid)

想做瀑布流式的应用吗？用它吧。
![](http://f.cl.ly/items/2z1B0Y0M0G0O2k1l3J03/Trending.png)

- 
[**cardslib**](https://github.com/gabrielemariotti/cardslib)

一种卡片式的布局控件。 记得以前的有道词典的默认页就是这种布局。
![Screen](https://github.com/gabrielemariotti/cardslib/raw/master/demo/images/screen3.png)Screen

- 
[**PhotoView**](https://github.com/chrisbanes/PhotoView)

ImageView扩展， 支持放缩和手势。
![](https://raw.github.com/chrisbanes/PhotoView/master/header_graphic.png)

- 
[**ListViewAnimations**](https://github.com/nhaarman/ListViewAnimations)

为ListView增加动画， 还提供滑动删除选定项的功能。 类似Android的邮件的删除操作。

- 
[**ion**](https://github.com/koush/ion)

又一个图像异步加载库。至少推荐三个了， 轮子不少。

- 
[**android-menudrawer**](https://github.com/SimonVT/android-menudrawer)

又一个滑动菜单的组件。
![](http://simonvt.github.io/android-menudrawer/static/left_content.png)

- 
[**android-swipelistview**](https://github.com/47deg/android-swipelistview)

又一个ListView扩展， 支持滑动操作。
![](https://raw.github.com/47deg/android-swipelistview-sample/master/screenshot_swipelistview_small.png)

- 
[**android-pulltorefresh**](https://github.com/johannilsson/android-pulltorefresh)

又一个拖动刷新的组件。到此为止说了好多又了。 因为有些功能确实有多个不错的实现。
![](https://github.com/johannilsson/android-pulltorefresh/raw/master/android-pull-to-refresh.png)

- 
[**Crouton**](https://github.com/keyboardsurfer/Crouton)

Toast之外的另以选择。

- 
[**StickyListHeaders**](https://github.com/emilsjolander/StickyListHeaders)

header可以固定的组件。 看下图。
![](https://github.com/emilsjolander/StickyListHeaders/raw/master/demo.gif)

- 
[**drag-sort-listview**](https://github.com/bauerca/drag-sort-listview)

在ListItem拖动重排序。 开发者已经放弃维护了但是关注者依然不少。

- 
[**FreeFlow**](https://github.com/Comcast/FreeFlow)

comcast出品的一个布局引擎。 FreeFlow让你很容易的定制布局和漂亮的转换动画。

看到comcast好激动， 因为他是[笔者](http://colobu.com/)公司最大的客户。
![](https://raw.github.com/Comcast/FreeFlow/master/examples/Artbook/screenshots/freeflow.png)

- 
[**FadingActionBar**](https://github.com/ManuelPeinado/FadingActionBar)

折叠风格的ActionBar. 说起ActionBar， 第一选择不是[ActionBarSherlock](https://github.com/JakeWharton/ActionBarSherlock)么？
![](https://raw.github.com/ManuelPeinado/FadingActionBar/master/art/readme_pic.png)

- 
[**android-viewflow**](https://github.com/pakerfeldt/android-viewflow)

可以水平滚动的视图。

- 
[**android-flip**](https://github.com/openaphid/android-flip)

可以实现类似Flipboard一样的翻页效果。
![](http://openaphid.github.com/images/flipview-horizontal-demo.gif)

- 
[**android-times-square**](https://github.com/square/android-times-square)

square出品的日期选择组件。
![Screenshot](https://github.com/square/android-times-square/raw/master/timesSquareScreenshot.png)Screenshot

- 
[**circular-progress-button**](https://github.com/dmytrodanylyk/circular-progress-button)

环形进度条按钮。

- 
[**android-viewbadger**](https://github.com/jgilfelt/android-viewbadger)

为视图增加badger。可以做的更好看点。

- 
[**AndroidViewAnimations**](https://github.com/daimajia/AndroidViewAnimations)

漂亮的视图动画。

- 
[**Android-SlideExpandableListView**](https://github.com/tjerkw/Android-SlideExpandableListView)

有一个可以滑动的ExpandableListView组件。
![](https://github.com/tjerkw/Android-SlideExpandableListView/raw/master/raw/example-screens.png)

- 
[**pinned-section-listview**](https://github.com/beworker/pinned-section-listview)

GroupName滑动到顶端时会固定不动直到另外一个GroupName到达顶端的ExpandListView
![](https://camo.githubusercontent.com/bbabd30ab2af2c19854c89b6dc691529806127d9/68747470733a2f2f7261772e6769746875622e636f6d2f6265776f726b65722f70696e6e65642d73656374696f6e2d6c697374766965772f6d61737465722f73637265656e312e706e67)

- 
[**android-process-button**](https://github.com/dmytrodanylyk/android-process-button)

很酷的一个组件，可以在按钮下部显示漂亮的进度条，就像GMAIL一样。
![](http://colobu.com/dmytrodanylyk/android-process-button/raw/master/screenshots/sample1_small2.gif)

- 
[**android-betterpickers**](https://github.com/derekbrameyer/android-betterpickers)

更好的时间、日期库。
![](https://raw.github.com/derekbrameyer/android-betterpickers/master/sample/imagery/web_feature_graphic.png)

- 
[**StaggeredGridView**](https://github.com/maurycyw/StaggeredGridView)

瀑布流。类似Pinterest。
![](http://f.cl.ly/items/1I0n3i361o3R070y3k46/340616_1355789751.jpeg)

- 
[**ProgressWheel**](https://github.com/Todd-Davies/ProgressWheel)

环形进度条。
![](https://github.com/Todd-Davies/ProgressWheel/raw/master/sample_image.png)

- 
[**Calligraphy**](https://github.com/chrisjenx/Calligraphy)

在Android中更容易的使用字体。

- 
[**android-satellite-menu**](https://github.com/siyamed/android-satellite-menu)

类似Path一样的菜单。
![](http://i.imgur.com/0Igkktd.png)

- 
[**android-actionbarstylegenerator**](http://jgilfelt.github.com/android-actionbarstylegenerator/)

action bar风格在线制作。

- 
[**SuperToasts**](https://github.com/JohnPersano/SuperToasts)

扩展的Toast.

- 
[**GlassActionBar**](https://github.com/ManuelPeinado/GlassActionBar)

毛玻璃效果的ction bar.
![](https://raw.github.com/ManuelPeinado/GlassActionBar/master/art/readme_pic.png)

- 
[**GraphView**](https://github.com/jjoe64/GraphView)

可以产生放缩的线图和直方图。
![](https://github.com/jjoe64/GraphView/raw/master/GVBar.png)

- 
[**StickyGridHeaders**](https://github.com/TonicArtos/StickyGridHeaders)

可以固定header和分区的Grid。
![](http://4.bp.blogspot.com/-S_BbhWX6wTY/UQpW0cwUGEI/AAAAAAAAGvU/zzJXj-PcVbY/s1600/screen-landscape-smaller.png)

- 
[**BlurEffectForAndroidDesign**](https://github.com/PomepuyN/BlurEffectForAndroidDesign)

模糊效果的演示。
![](https://lh6.ggpht.com/IpUS0939TyEWcLfB8rLqASLxqGvXs3VD23kokwha80Qgs2adLuOc5hbW5bP9zH4i6A)

- 
[**ArcMenu**](https://github.com/daCapricorn/ArcMenu)

又一个类似Path的环形菜单。
![](https://dl.dropbox.com/u/11369687/preview0.png)

- 
[**cropper**](https://github.com/edmodo/cropper)

图像剪切和旋转。
![](http://i.imgur.com/3FhsTgfl.jpg)

- 
[**Android-AppMsg**](https://github.com/johnkil/Android-AppMsg)

- 
[**chromeview**](https://github.com/pwnall/chromeview)

Android webview组件的实现， 但是基于最新的Chromium 代码。

- 
[**StandOut**](https://github.com/pingpongboss/StandOut)

很容易的创建弹出窗口。
![](https://dl.dropbox.com/u/30367/hosted/StandOut.png)

- 
[**android-iconify**](https://github.com/JoanZapata/android-iconify)

集成FontAwesome 。
![](https://raw.github.com/JoanZapata/android-iconify/master/header.jpg)

- 
[**android-gif-drawable**](https://github.com/koral--/android-gif-drawable)

看名字就知道了。

- 
[**NumberProgressBar**](https://github.com/daimajia/NumberProgressBar)

漂亮的带数字的进度条。
![](http://ww3.sinaimg.cn/mw690/610dc034jw1efyrd8n7i7g20cz02mq5f.gif)

- 
[**Android-SwipeToDismiss**](https://github.com/romannurik/Android-SwipeToDismiss)

演示ListView滑动删除。

- 
[**Shimmer-android**](https://github.com/RomainPiel/Shimmer-android)

闪烁发光的文本框。
![ScreenShot](http://colobu.com/RomainPiel/Shimmer-android/raw/master/shimmer.gif)ScreenShot

- 
[**HoloColorPicker**](https://github.com/LarsWerkman/HoloColorPicker)

Holo风格的颜色选择器。
![](https://lh5.googleusercontent.com/-3KSukk_S94Y/UvKiNER-OBI/AAAAAAAAA-k/8SPfOmFhLjE//device-2014-02-05-180704_framed.png)

- 
[**android-crop**](https://github.com/jdamcd/android-crop)

图像裁剪

- 
[**android-edittext-validator**](https://github.com/vekexasia/android-edittext-validator)

文本框校验

- 
[**MultiChoiceAdapter**](https://github.com/ManuelPeinado/MultiChoiceAdapter)

ListView 支持多选。
![](https://raw.github.com/ManuelPeinado/MultiChoiceAdapter/master/art/readme_pic.png)

- 
[**PinterestLikeAdapterView**](https://github.com/dodola/PinterestLikeAdapterView)

瀑布流。
![01](https://github.com/dodola/PinterestLikeAdapterView/raw/master/01.png)01

- 
[**MPAndroidChart**](https://github.com/PhilJay/MPAndroidChart)

非常不错的图表工具。
![](https://raw.github.com/PhilJay/MPChart/master/screenshots/simpledesign_linechart4.png)

- 
[**Android-Action-Bar-Icons**](https://github.com/svenkapudija/Android-Action-Bar-Icons)

图标资源。

- 
[**Android-UndoBar**](https://github.com/jenzz/Android-UndoBar)
![](https://raw.github.com/jenzz/Android-UndoBar/master/assets/Screenshot4.png)

- 
[**android-pdfview**](https://github.com/JoanZapata/android-pdfview)
![](https://raw.github.com/JoanZapata/android-pdfview/master/device.png)

- 
[**AndroidCharts**](https://github.com/HackPlan/AndroidCharts)

图表控件
![](https://raw.github.com/dacer/AndroidCharts/master/pic/line.png)

- 
[**Android-Charts**](https://github.com/limccn/Android-Charts)

国人实现的图表控件
![](http://www.lidaren.com/code/androidcharts/screenshot.png)

- 
[**desCharts**](https://github.com/bradipao/desCharts)

图表控件


![XY chart](https://github.com/bradipao/desCharts/raw/master/media/xychartview_sm.png)XY
 chart
- 
[**PinnedHeaderExpandableListView**](https://github.com/singwhatiwanna/PinnedHeaderExpandableListView)

国人实现的一个ExpandableListView,头部可以固定， 并且可以伸缩。
![](https://camo.githubusercontent.com/fa7f516b0b6e2921007a17cbc359b3f661f32226/687474703a2f2f696d672e626c6f672e6373646e2e6e65742f32303134303531313135313534363834333f77617465726d61726b2f322f746578742f6148523063446f764c324a736232637559334e6b626935755a58517663326c755a33646f5958527064324675626d453d2f666f6e742f3561364c354c32542f666f6e7473697a652f3430302f66696c6c2f49304a42516b46434d413d3d2f646973736f6c76652f37302f677261766974792f536f75746845617374)

- 
[**XCL-Charts**](https://github.com/xcltapestry/XCL-Charts)

国人出品， 图表控件，种类很多。

- 
[**PullToZoomInListView**](https://github.com/matrixxun/PullToZoomInListView)

滑动ListView时使其HeaderView跟随滑动缩放.
![](https://github.com/matrixxun/PullToZoomInListView/raw/master/art/pull-to-zoom.gif)

- 
[**NotBoringActionBar**](https://github.com/flavienlaurent/NotBoringActionBar)

拖动时顶部可以收缩。
![](https://camo.githubusercontent.com/65f2d44e0e5064369aec8ee41838bd073e695975/68747470733a2f2f7261772e6769746875622e636f6d2f666c617669656e6c617572656e742f4e6f74426f72696e67416374696f6e4261722f6d61737465722f67726170686963732f6e6f74626f72696e6761622e676966)

- 
[**FancyCoverFlow**](https://github.com/davidschreiber/FancyCoverFlow)
![](https://camo.githubusercontent.com/ef5ced52b7b54652b50499521ed797c0188c7a6b/687474703a2f2f64617669647363687265696265722e6769746875622e696f2f46616e6379436f766572466c6f772f73637265656e73686f74322e706e67)

- 
[transparent-over-animtabsview](https://github.com/wuyexiong/transparent-over-animtabsview)

模仿网易云音乐
![](https://cloud.githubusercontent.com/assets/1940316/2728711/318826ca-c5fa-11e3-8f47-c261d395be14.png)

- 
**chromium-webview-samples**

webview例子。
[https://github.com/GoogleChrome/chromium-webview-samples](https://github.com/GoogleChrome/chromium-webview-samples)
[https://developer.chrome.com/multidevice/webview/gettingstarted](https://developer.chrome.com/multidevice/webview/gettingstarted)
[http://developer.android.com/guide/webapps/webview.html](http://developer.android.com/guide/webapps/webview.html)
[http://developer.android.com/reference/android/webkit/WebView.html](http://developer.android.com/reference/android/webkit/WebView.html)
[http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2013/1010/1569.html](http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2013/1010/1569.html)


# 类库
- 
[**roboguice**](https://github.com/roboguice/roboguice)

google guice框架在Android平台上的实现。 什么事guice? 它是一个类似Spring的IOC框架。 什么是IOC？ 算了，你还是先学习Java的基础知识吧。
![](http://f.cl.ly/items/1G3K1n2L3g452n3i1M2q/roboguice-200px.png)

- 
[**android-async-http**](https://github.com/loopj/android-async-http)

基于Apache http client实现的支持异步、回调的android http client组件。

- 
[**androidannotations**](https://github.com/excilys/androidannotations)

将注解引入到Android开发中，极大的减少代码里，可以更好的维护代码。

- 
[**retrofit**](https://github.com/square/retrofit)

类型安全的Restful client库。

- 
[**dagger**](https://github.com/square/dagger)

一个快速的依赖注入库。这个square公司出了不少的组件库。 square公司就是那个移动支付的创业公司。

- 
[**okhttp**](https://github.com/square/okhttp)

还是square出品， HTTP+SPDY 客户端。

- 
[**EventBus**](https://github.com/greenrobot/EventBus)

一个专为Android优化的发布订阅框架, event bus模式。 可以在Activity, Fragment 和后台线程之间交流数据。

- 
[**butterknife**](https://github.com/JakeWharton/butterknife)

专为视图注入库。


```
class ExampleActivity extends Activity {

  @InjectView(R.id.user) EditText username;

  @InjectView(R.id.pass) EditText password;

  @OnClick(R.id.submit) void submit() {

    // TODO call server...

  }

  @Override public void onCreate(Bundle savedInstanceState) {

    super.onCreate(savedInstanceState);

    setContentView(R.layout.simple_activity);

    ButterKnife.inject(this);

    // TODO Use "injected" views...

  }

}
```
- 
[**robospice**](https://github.com/stephanenicolas/robospice)

一个模块化的Android类库， 可以更容易的编写异步的长时间运行的任务。

- 
[****](http://colobu.com/2014/08/15/android-components-collection/)

一个加强的基于Guava event bus库。 square出品。

- 
[**AndroidAsync**](https://github.com/koush/AndroidAsync)

又是异步处理库， 支持socket,http (client+server),websocket 和socket.io ，基于nio。

- 
[**ActiveAndroid**](https://github.com/pardom/ActiveAndroid)

active record风格的操作Android SQLite 数据库。

- 
[**androidquery**](https://code.google.com/p/android-query/wiki/AsyncAPI)

JQuery很熟悉吧。 这是Android版的JQuery。

- 
[**greenDAO**](https://github.com/greenrobot/greenDAO)

一个轻量级的快速的ORM方案。 操作SQLite 数据库。

- 
[**Bolts-Android**](https://github.com/BoltsFramework/Bolts-Android)

一套底层库。

- 
[**scaloid**](https://github.com/pocorall/scaloid)
**极大的**减少Android得代码，更易维护。

- 
[**conceal**](https://github.com/facebook/conceal)

Facebook的快速加密类。

- 
[**android-common**](https://github.com/Trinea/android-common)

国人实现的一个工具包。

- 
[**xUtils**](https://github.com/wyouflf/xUtils)

国人实现的一个工具包。

- 
[**android-priority-jobqueue**](https://github.com/path/android-priority-jobqueue)

Path出品的job queue。

- 
[**android-sqlite-asset-helper**](https://github.com/jgilfelt/android-sqlite-asset-helper)

数据库辅助类。 包括数据库的创建与升级。

- 
[**tape**](https://github.com/square/tape)

一组queue类。 square出品。

- 
[**spring-android**](https://github.com/spring-projects/spring-android)

不多说了。

- 
[**joda-time-android**](https://github.com/dlew/joda-time-android)

为Android做了优化

- 
[**spydroid-ipcamera**](https://github.com/fyhertz/spydroid-ipcamera)

将你的收集作为监控设备。


# 游戏引擎
- 
[**libgdx**](https://github.com/libgdx/libgdx)

libGDX是基于OpenGL的跨平台的游戏开发框架。
![](http://libgdx.badlogicgames.com/img/logo.png)

- 
[**AndEngine**](https://github.com/nicolasgramlich/AndEngine)

知名的Android 2D OpenGL 游戏引擎。

- 
[**ogengine**](http://www.ogengine.com/)

国人开发的国际著名开源引擎AndEngine的一个分支 （往上看）。遵循LGPL开源协议使用OpenGL ES进行图形绘制。同时集成了Box2D物理引擎，因此可以实现复杂的物理效果。

- 
[**citrus**](http://citrusengine.com/)

快速且轻量级的2D & 3D游戏引擎。
![](http://citrusengine.com/wp-content/themes/citrus/images/citrus-stack.png)

- 
[**Cocos2d-Android**](https://code.google.com/p/cocos2d-android/)

知名的游戏引擎

- 
[**gameclosure**](http://www.gameclosure.com/license.html)

JavaScript 游戏引擎

- 
[**游戏引擎列表**](http://en.wikipedia.org/wiki/List_of_game_engines)

维基百科上的各种游戏引擎， 各种平台的引擎列表， 全。


# Android HTML5应用
- 
[**PhoneCap**](http://phonegap.com/)

早在2011年10月，Adobe收购了Nitobi Software和它的PhoneGap产品，然后宣布这个移动开发框架将会继续开源，并把它提交到Apache Incubator，以便完全接受ASF的管治。

- 
[**Cordova**](https://github.com/apache/cordova-android)

Cordova是贡献给Apache后的开源项目，是从PhoneGap中抽出的核心代码，是驱动PhoneGap的核心引擎。你可以把他想象成类似于Webkit和Google Chrome的关系。

Adobe将会继续以Cordova加上PhoneGap Build和Adobe Shadow的组合提供PhoneGap。　

- 
[**Cordova-SQLitePlugin**](https://github.com/brodysoft/Cordova-SQLitePlugin)


```
var db = window.sqlitePlugin.openDatabase({name: "my.db"});
```
- 
[**awesome-cordova**](https://github.com/busterc/awesome-cordova)

Cordova插件列表

- 
[**jquerymobile**](http://jquerymobile.com/)

JQuery公司出品的移动开发框架。
![](http://jquerymobile.com/resources/devices.png)

- [**lungo**](https://github.com/tapquo/Lungo.js)
- 
[**awesome-html5**](https://github.com/diegocard/awesome-html5)

- 
**开源的Javascript游戏引擎[**Pixi.js**]([https://github.com/GoodBoyDigital/pixi.js](https://github.com/GoodBoyDigital/pixi.js)) **[**phaser**](https://github.com/photonstorm/phaser)
**[**MelonJS**]([https://github.com/melonjs/melonJS](https://github.com/melonjs/melonJS)) **[**kiwi.js**](https://github.com/gamelab/kiwi.js)
**[**Crafty**]([https://github.com/craftyjs/Crafty](https://github.com/craftyjs/Crafty)) **[**PhysicsJS**](https://github.com/wellcaffeinated/PhysicsJS)
**[**cutjs**]([https://github.com/piqnt/cutjs](https://github.com/piqnt/cutjs)) [cocos2d](https://github.com/cocos2d/cocos2d-html5)**

-](https://so.csdn.net/so/search/s.do?q=2d&t=blog)](https://so.csdn.net/so/search/s.do?q=html5&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




