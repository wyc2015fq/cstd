# 高手速成android开源项目【View篇】 - u013366022的专栏 - CSDN博客
2014年05月14日 15:48:12[slitaz](https://me.csdn.net/u013366022)阅读数：546
主要介绍那些不错个性化的View，包括ListView、ActionBar、Menu、ViewPager、Gallery、GridView、ImageView、ProgressBar及其他如Dialog、Toast、EditText、TableView、Activity Animation等等。
#### 一、ListView
- 
android-pulltorefresh 一个强大的拉动刷新开源项目，支持各种控件下拉刷新
ListView、ViewPager、WevView、ExpandableListView、GridView、(Horizontal
)ScrollView、Fragment上下左右拉动刷新，比下面johannilsson那个只支持ListView的强大的多。并且他实现的下拉刷新ListView在item不足一屏情况下也不会显示刷新提示，体验更好。
项目地址：[https://github.com/chrisbanes/Android-PullToRefresh](https://github.com/chrisbanes/Android-PullToRefresh)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refreshview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refreshview-demo.apk?raw=true)
APP示例：新浪微博各个页面
- 
android-pulltorefresh-listview 下拉刷新ListView
项目地址：[https://github.com/johannilsson/android-pulltorefresh](https://github.com/johannilsson/android-pulltorefresh)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refresh-listview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refresh-listview-demo.apk?raw=true)
PS：这个被很多人使用的项目实际有不少bug，推荐使用上面的android-pulltorefresh
- 
DropDownListView 下拉刷新及滑动到底部加载更多ListView
项目地址：[https://github.com/Trinea/AndroidCommon](https://github.com/Trinea/AndroidCommon)
Demo地址：[https://play.google.com/store/apps/details?id=cn.trinea.android.demo](https://play.google.com/store/apps/details?id=cn.trinea.android.demo)
文档介绍：[http://www.trinea.cn/?p=523](http://www.trinea.cn/?p=523)
- 
DragSortListView 拖动排序的ListView
同时支持ListView滑动item删除，各个Item高度不一、单选、复选、CursorAdapter做为适配器、拖动背景变化等
项目地址：[https://github.com/bauerca/drag-sort-listview](https://github.com/bauerca/drag-sort-listview)
Demo地址：[https://play.google.com/store/apps/details?id=com.mobeta.android.demodslv](https://play.google.com/store/apps/details?id=com.mobeta.android.demodslv)
APP示例：Wordpress Android
- 
SwipeListView 支持定义ListView左右滑动事件，支持左右滑动位移，支持定义动画时间
项目地址：[https://github.com/47deg/android-swipelistview](https://github.com/47deg/android-swipelistview)
Demo地址：[https://play.google.com/store/apps/details?id=com.fortysevendeg.android.swipelistview](https://play.google.com/store/apps/details?id=com.fortysevendeg.android.swipelistview)
APP示例：微信
- 
Android-SwipeToDismiss 滑动Item消失ListView
项目地址：[https://github.com/romannurik/Android-SwipeToDismiss](https://github.com/romannurik/Android-SwipeToDismiss)
支持3.0以下版本见：[https://github.com/JakeWharton/SwipeToDismissNOA](https://github.com/JakeWharton/SwipeToDismissNOA)
Demo地址：[https://github.com/JakeWharton/SwipeToDismissNOA/SwipeToDismissNOA.apk/qr_code](https://github.com/JakeWharton/SwipeToDismissNOA/SwipeToDismissNOA.apk/qr_code)
- 
StickyListHeaders GroupName滑动到顶端时会固定不动直到另外一个GroupName到达顶端的ExpandListView，支持快速滑动，支持Android2.3及以上
项目地址：[https://github.com/emilsjolander/StickyListHeaders](https://github.com/emilsjolander/StickyListHeaders)
APP示例：Android 4.0联系人
效果图：![Renderings](https://raw.github.com/emilsjolander/StickyListHeaders/master/demo.gif)
- 
pinned-section-listview GroupName滑动到顶端时会固定不动直到另外一个GroupName到达顶端的ExpandListView
项目地址：[https://github.com/beworker/pinned-section-listview](https://github.com/beworker/pinned-section-listview)
效果图：![Renderings](https://raw.github.com/beworker/pinned-section-listview/master/screen1.png)
- 
PinnedHeaderListView GroupName滑动到顶端时会固定不动直到另外一个GroupName到达顶端的ExpandListView
项目地址：[https://github.com/JimiSmith/PinnedHeaderListView](https://github.com/JimiSmith/PinnedHeaderListView)
- 
QuickReturnHeader ListView/ScrollView的header或footer，当向下滚动时消失，向上滚动时出现
项目地址：[https://github.com/ManuelPeinado/QuickReturnHeader](https://github.com/ManuelPeinado/QuickReturnHeader)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/quick-return-header-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/quick-return-header-demo.apk?raw=true)
APP示例：google plus
- 
IndexableListView ListView右侧会显示item首字母快捷索引，点击可快速滑动到某个item
项目地址：[https://github.com/woozzu/IndexableListView](https://github.com/woozzu/IndexableListView)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/indexable-listview.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/indexable-listview.apk?raw=true)
APP示例：微信通讯录、小米联系人
- 
CustomFastScrollView ListView快速滑动，同时屏幕中间PopupWindows显示滑动到的item内容或首字母
项目地址：[https://github.com/nolanlawson/CustomFastScrollViewDemo](https://github.com/nolanlawson/CustomFastScrollViewDemo)
效果图：![Renderings](https://raw.github.com/nolanlawson/CustomFastScrollViewDemo/master/example.png)
- 
Android-ScrollBarPanel ListView滑动时固定的Panel指示显示在scrollbar旁边
项目地址：[https://github.com/rno/Android-ScrollBarPanel](https://github.com/rno/Android-ScrollBarPanel)
效果展示：[https://github.com/rno/Android-ScrollBarPanel/raw/master/demo_capture.png](https://github.com/rno/Android-ScrollBarPanel/raw/master/demo_capture.png)
- 
SlideExpandableListView 用户点击listView item滑出固定区域，其他item的区域收缩
项目地址：[https://github.com/tjerkw/Android-SlideExpandableListView](https://github.com/tjerkw/Android-SlideExpandableListView)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/slide-expandable-listView-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/slide-expandable-listView-demo.apk?raw=true)
- 
JazzyListView ListView及GridView item以特殊动画效果进入屏幕，效果包括grow、cards、curl、wave、flip、fly等等
项目地址：[https://github.com/twotoasters/JazzyListView](https://github.com/twotoasters/JazzyListView)
Demo地址：[https://play.google.com/store/apps/details?id=com.twotoasters.jazzylistview.sample](https://play.google.com/store/apps/details?id=com.twotoasters.jazzylistview.sample)
效果展示：[http://lab.hakim.se/scroll-effects/](http://lab.hakim.se/scroll-effects/)
- 
ListViewAnimations 带Item显示动画的ListView，动画包括底部飞入、其他方向斜飞入、下层飞入、渐变消失、滑动删除等
项目地址：[https://github.com/nhaarman/ListViewAnimations](https://github.com/nhaarman/ListViewAnimations)
Demo地址：[https://play.google.com/store/apps/details?id=com.haarman.listviewanimations](https://play.google.com/store/apps/details?id=com.haarman.listviewanimations)
APP示例：Google plus、Google Now卡片式进入、小米系统中应用商店、联系人、游戏中心、音乐、文件管理器的ListView、Ultimate、Light Flow Lite、TreinVerkeer、Running Coach、Pearl Jam Lyrics、Calorie Chart、Car Hire、Super BART、DK FlashCards、Counter Plus、Voorlees Verhaaltjes 2.0
- 
DevsmartLib-Android 横向ListView
项目地址：[https://github.com/dinocore1/DevsmartLib-Android](https://github.com/dinocore1/DevsmartLib-Android)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/horizontal-listview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/horizontal-listview-demo.apk?raw=true)
#### [](https://github.com/Trinea/android-open-project#%E4%BA%8Cactionbar)二、ActionBar
- 
ActionBarSherlock 为Android所有版本提供统一的ActionBar，解决4.0以下ActionBar的适配问题
项目地址：[https://github.com/JakeWharton/ActionBarSherlock](https://github.com/JakeWharton/ActionBarSherlock)
Demo地址：[https://play.google.com/store/apps/details?id=com.actionbarsherlock.sample.demos](https://play.google.com/store/apps/details?id=com.actionbarsherlock.sample.demos)
APP示例：太多了。。现在连google都在用
- 
ActionBar-PullToRefresh 下拉刷新，ActionBar出现加载中提示
项目地址：[https://github.com/chrisbanes/ActionBar-PullToRefresh](https://github.com/chrisbanes/ActionBar-PullToRefresh)
Demo地址：[https://play.google.com/store/apps/details?id=uk.co.senab.actionbarpulltorefresh.samples.stock](https://play.google.com/store/apps/details?id=uk.co.senab.actionbarpulltorefresh.samples.stock)
APP示例：Gmail，Google plus，知乎等
- 
FadingActionBar ListView向下滚动逐渐显现的ActionBar
项目地址：[https://github.com/ManuelPeinado/FadingActionBar](https://github.com/ManuelPeinado/FadingActionBar)
Demo地址：[https://play.google.com/store/apps/details?id=com.manuelpeinado.fadingactionbar.demo](https://play.google.com/store/apps/details?id=com.manuelpeinado.fadingactionbar.demo)
APP示例：google music，知乎
- 
NotBoringActionBar google music下拉收缩的ActionBar
项目地址：[https://github.com/flavienlaurent/NotBoringActionBar](https://github.com/flavienlaurent/NotBoringActionBar)
Demo地址：[http://flavienlaurent.com/blog/2013/11/20/making-your-action-bar-not-boring/](http://flavienlaurent.com/blog/2013/11/20/making-your-action-bar-not-boring/)
APP示例：Google音乐
- 
RefreshActionItem 带进度显示和刷新按钮的ActionBar
项目地址：[https://github.com/ManuelPeinado/RefreshActionItem](https://github.com/ManuelPeinado/RefreshActionItem)
Demo地址：[https://play.google.com/store/apps/details?id=com.manuelpeinado.refreshactionitem.demo](https://play.google.com/store/apps/details?id=com.manuelpeinado.refreshactionitem.demo)
APP示例：The New York Times，DevAppsDirect.
- 
GlassActionBar 类似玻璃的有一定透明度的ActionBar
项目地址：[https://github.com/ManuelPeinado/GlassActionBar](https://github.com/ManuelPeinado/GlassActionBar)
Demo地址：[https://play.google.com/store/apps/details?id=com.manuelpeinado.glassactionbardemo](https://play.google.com/store/apps/details?id=com.manuelpeinado.glassactionbardemo)
APP示例：google music
#### [](https://github.com/Trinea/android-open-project#%E4%B8%89menu)三、Menu
- 
MenuDrawer 滑出式菜单，通过拖动屏幕边缘滑出菜单，支持屏幕上下左右划出，支持当前View处于上下层，支持Windows边缘、ListView边缘、ViewPager变化划出菜单等。
项目地址：[https://github.com/SimonVT/android-menudrawer](https://github.com/SimonVT/android-menudrawer)
Demo地址：[http://simonvt.github.io/android-menudrawer/](http://simonvt.github.io/android-menudrawer/)
APP示例：Gmail、Google Music等大部分google app
- 
SlidingMenu 滑出式菜单，通过拖动屏幕边缘滑出菜单，支持屏幕左右划出，支持菜单zoom、scale、slide up三种动画样式出现。
项目地址：[https://github.com/jfeinstein10/SlidingMenu](https://github.com/jfeinstein10/SlidingMenu)
Demo地址：[https://play.google.com/store/apps/details?id=com.slidingmenu.example](https://play.google.com/store/apps/details?id=com.slidingmenu.example)
APP示例：Foursquare, LinkedIn, Zappos, Rdio, Evernote Food, Plume, VLC for Android, ESPN ScoreCenter, MLS MatchDay, 9GAG, Wunderlist 2, The Verge, MTG Familiar, Mantano Reader, Falcon Pro (BETA), MW3 Barracks
MenuDrawer和SlidingMenu比较：SlidingMenu支持菜单动画样式出现，MenuDrawer支持菜单view处于内容的上下层
- 
ArcMenu 支持类似Path的左下角动画旋转菜单及横向划出菜单、圆心弹出菜单
项目地址：[https://github.com/daCapricorn/ArcMenu](https://github.com/daCapricorn/ArcMenu)
APP示例：Path
效果图：![Renderings](https://dl.dropboxusercontent.com/u/11369687/preview0.png)
[https://dl.dropboxusercontent.com/u/11369687/preview1.png](https://dl.dropboxusercontent.com/u/11369687/preview1.png)
[https://dl.dropboxusercontent.com/u/11369687/raymenu.png](https://dl.dropboxusercontent.com/u/11369687/raymenu.png)
- 
android-satellite-menu 类似Path的左下角动画旋转菜单
项目地址：[https://github.com/siyamed/android-satellite-menu](https://github.com/siyamed/android-satellite-menu)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/satellite-menu-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/satellite-menu-demo.apk?raw=true)
APP示例：Path
- 
radial-menu-widget 圆形菜单，支持二级菜单
项目地址：[https://code.google.com/p/radial-menu-widget/](https://code.google.com/p/radial-menu-widget/)
效果图：[http://farm8.staticflickr.com/7377/11621125154_d1773c2dcc_o.jpg](http://farm8.staticflickr.com/7377/11621125154_d1773c2dcc_o.jpg)
- 
Android Wheel Menu 圆形旋转选取菜单
项目地址：[https://github.com/anupcowkur/Android-Wheel-Menu](https://github.com/anupcowkur/Android-Wheel-Menu)
效果图：![Renderings](https://raw.github.com/anupcowkur/Android-Wheel-Menu/master/graphics/wheel.gif)
- 
FoldingNavigationDrawer滑动并以折叠方式打开菜单
项目地址：[https://github.com/tibi1712/FoldingNavigationDrawer-Android](https://github.com/tibi1712/FoldingNavigationDrawer-Android)
使用介绍：[https://play.google.com/store/apps/details?id=com.ptr.folding.sample](https://play.google.com/store/apps/details?id=com.ptr.folding.sample)
效果图：![Renderings](https://lh6.ggpht.com/VnKUZenAozQ0KFAm5blFTGqMaKFjvX-BK2JH-jrX1sIXVTqciACqRhqFH48hc4pm2Q=h310-rw)
#### [](https://github.com/Trinea/android-open-project#%E5%9B%9Bviewpager-gallery)四、ViewPager 、Gallery
- 
Android-ViewPagerIndicator 配合ViewPager使用的Indicator，支持各种位置和样式
项目地址：[https://github.com/JakeWharton/Android-ViewPagerIndicator](https://github.com/JakeWharton/Android-ViewPagerIndicator)
Demo地址：[https://play.google.com/store/apps/details?id=com.viewpagerindicator.sample](https://play.google.com/store/apps/details?id=com.viewpagerindicator.sample)
APP示例：太多了。。
- 
JazzyViewPager 支持Fragment切换动画的ViewPager，动画包括转盘、淡入淡出、翻页、层叠、旋转、方块、翻转、放大缩小等
项目地址：[https://github.com/jfeinstein10/JazzyViewPager](https://github.com/jfeinstein10/JazzyViewPager)
Demo地址：[https://github.com/jfeinstein10/JazzyViewPager/blob/master/JazzyViewPager.apk?raw=true](https://github.com/jfeinstein10/JazzyViewPager/blob/master/JazzyViewPager.apk?raw=true)
效果类似桌面左右切换的各种效果，不过桌面并非用ViewPager实现而已
- 
Android-DirectionalViewPager 支持横向和纵向(垂直)的ViewPager
项目地址：[https://github.com/JakeWharton/Android-DirectionalViewPager](https://github.com/JakeWharton/Android-DirectionalViewPager)
Demo地址：[https://market.android.com/details?id=com.directionalviewpager.sample](https://market.android.com/details?id=com.directionalviewpager.sample)
- 
android-pulltorefresh 支持下拉刷新的ViewPager
项目地址：[https://github.com/chrisbanes/Android-PullToRefresh](https://github.com/chrisbanes/Android-PullToRefresh)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refreshview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/pull-to-refreshview-demo.apk?raw=true)
APP示例：新浪微博各个页面
- 
FancyCoverFlow支持Item切换动画效果的类似Gallery View
项目地址：[https://github.com/davidschreiber/FancyCoverFlow](https://github.com/davidschreiber/FancyCoverFlow)
Demo地址：[https://play.google.com/store/apps/details?id=at.technikum.mti.fancycoverflow.samples](https://play.google.com/store/apps/details?id=at.technikum.mti.fancycoverflow.samples)
效果图：![Renderings](https://github-camo.global.ssl.fastly.net/ef5ced52b7b54652b50499521ed797c0188c7a6b/687474703a2f2f64617669647363687265696265722e6769746875622e696f2f46616e6379436f766572466c6f772f73637265656e73686f74322e706e67)
- 
AndroidTouchGallery 支持双击或双指缩放的Gallery(用ViewPager实现)
相比下面的PhotoView，在被放大后依然能滑到下一个item，并且支持直接从url和文件中获取图片，
项目地址：[https://github.com/Dreddik/AndroidTouchGallery](https://github.com/Dreddik/AndroidTouchGallery)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/touch-gallery-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/touch-gallery-demo.apk?raw=true)
APP示例：类似微信中查看聊天记录图片时可双击放大，并且放大情况下能正常左右滑动到前后图片
- 
Salvage view 带View缓存的Viewpager PagerAdapter，很方便使用
项目地址：[https://github.com/JakeWharton/salvage](https://github.com/JakeWharton/salvage)
#### [](https://github.com/Trinea/android-open-project#%E4%BA%94gridview)五、GridView
- 
StaggeredGridView 允许非对齐行的GridView
类似Pinterest的瀑布流，并且跟ListView一样自带View缓存，继承自ViewGroup
项目地址：[https://github.com/maurycyw/StaggeredGridView](https://github.com/maurycyw/StaggeredGridView)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/staggered-gridview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/staggered-gridview-demo.apk?raw=true)
APP示例：Pinterest等
- 
AndroidStaggeredGrid 允许非对齐行的GridView
类似Pinterest的瀑布流，继承自AbsListView
项目地址：[https://github.com/etsy/AndroidStaggeredGrid](https://github.com/etsy/AndroidStaggeredGrid)
APP示例：Pinterest等
- 
PinterestLikeAdapterView 允许非对齐行的GridView
类似Pinterest的瀑布流，允许下拉刷新
项目地址：[https://github.com/GDG-Korea/PinterestLikeAdapterView](https://github.com/GDG-Korea/PinterestLikeAdapterView)
APP示例：Pinterest等
- 
DraggableGridView Item可拖动交换位置的GridView，类似桌面的单屏效果
项目地址：[https://github.com/thquinn/DraggableGridView](https://github.com/thquinn/DraggableGridView)
Demo地址：[https://github.com/thquinn/DraggableGridView/blob/master/bin/DraggableGridViewSample.apk?raw=true](https://github.com/thquinn/DraggableGridView/blob/master/bin/DraggableGridViewSample.apk?raw=true)
#### [](https://github.com/Trinea/android-open-project#%E5%85%ADimageview)六、ImageView
- 
PhotoView 支持双击或双指缩放的ImageView
在ViewPager等Scrolling view中正常使用，相比上面的AndroidTouchGallery，不仅支持ViewPager，同时支持单个ImageView
项目地址：[https://github.com/chrisbanes/PhotoView](https://github.com/chrisbanes/PhotoView)
Demo地址：[https://play.google.com/store/apps/details?id=uk.co.senab.photoview.sample](https://play.google.com/store/apps/details?id=uk.co.senab.photoview.sample)
APP示例：photup
- 
android-gif-drawable 支持gif显示的view
项目地址：[https://github.com/koral--/android-gif-drawable](https://github.com/koral--/android-gif-drawable)
用jni实现的，编译生成so库后直接xml定义view即可，而且本身不依赖于其他开源项目所以相对下面的ImageViewEx简单的多
- 
ImageViewEx 支持Gif显示的ImageView
项目地址：[https://github.com/frapontillo/ImageViewEx](https://github.com/frapontillo/ImageViewEx)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/imageviewex-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/imageviewex-demo.apk?raw=true)
依赖很多，编译过程很繁琐!|_|!
- 
RoundedImageView 带圆角的ImageView
项目地址：[https://github.com/vinc3m1/RoundedImageView](https://github.com/vinc3m1/RoundedImageView)
效果图：![Renderings](https://raw.github.com/makeramen/RoundedImageView/master/screenshot.png)
#### [](https://github.com/Trinea/android-open-project#%E4%B8%83progressbar)七、ProgressBar
- 
SmoothProgressBar 水平进度条
项目地址：[https://github.com/castorflex/SmoothProgressBar](https://github.com/castorflex/SmoothProgressBar)
Demo地址：[https://play.google.com/store/apps/details?id=fr.castorflex.android.smoothprogressbar.sample](https://play.google.com/store/apps/details?id=fr.castorflex.android.smoothprogressbar.sample)
- 
ProgressWheel 支持进度显示的圆形ProgressBar
项目地址：[https://github.com/Todd-Davies/ProgressWheel](https://github.com/Todd-Davies/ProgressWheel)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/progress-wheel-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/progress-wheel-demo.apk?raw=true)
- 
android-square-progressbar 在图片周围显示进度
项目地址：[https://github.com/mrwonderman/android-square-progressbar](https://github.com/mrwonderman/android-square-progressbar)
Demo地址：[https://play.google.com/store/apps/details?id=net.yscs.android.square_progressbar_example](https://play.google.com/store/apps/details?id=net.yscs.android.square_progressbar_example)
APP示例：square
效果图：![Renderings](https://googledrive.com/host/0BwESwPCuXtw7eExwSFVLQkR2TTg/newscreen1.png)
- 
HoloCircularProgressBar Android4.1 时钟App样式
项目地址：[https://github.com/passsy/android-HoloCircularProgressBar](https://github.com/passsy/android-HoloCircularProgressBar)
APP示例：Android4.1时钟App
效果图：[https://raw.github.com/passsy/android-HoloCircularProgressBar/master/raw/screenshot1.png](https://raw.github.com/passsy/android-HoloCircularProgressBar/master/raw/screenshot1.png)
#### [](https://github.com/Trinea/android-open-project#%E5%85%AB%E5%85%B6%E4%BB%96)八、其他
- - 
achartengine 强大的图标绘制工具
支持折线图、面积图、散点图、时间图、柱状图、条图、饼图、气泡图、圆环图、范围（高至低）条形图、拨号图/表、立方线图及各种图的结合
项目地址：[https://code.google.com/p/achartengine/](https://code.google.com/p/achartengine/)
官方网站：[http://www.achartengine.org/](http://www.achartengine.org/)
效果图：![Renderings](https://github-camo.global.ssl.fastly.net/d3a4fa07ace8e6aca2f1e4c5131009de2e897a3e/687474703a2f2f7777772e616368617274656e67696e652e6f72672f64696d616765732f617665726167655f74656d70657261747572652e706e67)
[http://www.achartengine.org/dimages/sales_line_and_area_chart.png](http://www.achartengine.org/dimages/sales_line_and_area_chart.png)
[http://www.achartengine.org/dimages/temperature_range_chart.png](http://www.achartengine.org/dimages/temperature_range_chart.png)
[http://www.achartengine.org/dimages/combined_chart.png](http://www.achartengine.org/dimages/combined_chart.png)
[http://www.achartengine.org/dimages/budget_chart.png](http://www.achartengine.org/dimages/budget_chart.png)
APP示例：Wordpress Android，Google Analytics
- 
GraphView 绘制图表和曲线图的View
可用于Android上的曲形图、柱状图、波浪图展示
项目地址：[https://github.com/jjoe64/GraphView](https://github.com/jjoe64/GraphView)
Demo工程：[https://github.com/jjoe64/GraphView-Demos](https://github.com/jjoe64/GraphView-Demos)
Demo地址：[https://play.google.com/store/apps/details?id=com.sothree.umano](https://play.google.com/store/apps/details?id=com.sothree.umano)
APP示例：Wordpress Android，Google Analytics
- 
android-flip 类似Flipboard翻转动画的实现
项目地址：[https://github.com/openaphid/android-flip](https://github.com/openaphid/android-flip)
Demo地址：[https://github.com/openaphid/android-flip/blob/master/FlipView/Demo/APK/Aphid-FlipView-Demo.apk?raw=true](https://github.com/openaphid/android-flip/blob/master/FlipView/Demo/APK/Aphid-FlipView-Demo.apk?raw=true)
APP示例：flipboard
- 
FlipImageView 支持x、y、z及动画选择的翻转动画的实现
项目地址：[https://github.com/castorflex/FlipImageView](https://github.com/castorflex/FlipImageView)
Demo地址：[https://play.google.com/store/apps/details?id=fr.castorflex.android.flipimageview](https://play.google.com/store/apps/details?id=fr.castorflex.android.flipimageview)
- 
SwipeBackLayout 左右或向上滑动返回的Activity
项目地址：[https://github.com/Issacw0ng/SwipeBackLayout](https://github.com/Issacw0ng/SwipeBackLayout)
Demo地址：[https://play.google.com/store/apps/details?id=me.imid.swipebacklayout.demo](https://play.google.com/store/apps/details?id=me.imid.swipebacklayout.demo)
APP示例：知乎
- 
Cards-UI 卡片式View，支持单个卡片，item为卡片的ListView
项目地址：[https://github.com/afollestad/Cards-UI](https://github.com/afollestad/Cards-UI)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/cards-ui-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/cards-ui-demo.apk?raw=true)
- 
cardslib 卡片式View，支持单个卡片，item为卡片的ListView和GridView
项目地址：[https://github.com/gabrielemariotti/cardslib](https://github.com/gabrielemariotti/cardslib)
Demo地址：[https://play.google.com/store/apps/details?id=it.gmariotti.cardslib.demo](https://play.google.com/store/apps/details?id=it.gmariotti.cardslib.demo)
- 
android-styled-dialogs 可自定义样式的dialog
默认与Holo主题样式一致，在Android2.2以上同一样式
项目地址：[https://github.com/inmite/android-styled-dialogs](https://github.com/inmite/android-styled-dialogs)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/styled-dialogs-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/styled-dialogs-demo.apk?raw=true)
- 
Crouton 丰富样式的Toast
允许alert、comfirm、info样式及点击消失样式，允许设置Toast显示时间，允许自定义View。
项目地址：[https://github.com/keyboardsurfer/Crouton](https://github.com/keyboardsurfer/Crouton)
Demo地址：[http://play.google.com/store/apps/details?id=de.keyboardsurfer.app.demo.crouton](http://play.google.com/store/apps/details?id=de.keyboardsurfer.app.demo.crouton)
- 
supertooltips 带动画效果的Tips显示
项目地址：[https://github.com/nhaarman/supertooltips](https://github.com/nhaarman/supertooltips)
Demo地址：[https://play.google.com/store/apps/details?id=com.haarman.supertooltips](https://play.google.com/store/apps/details?id=com.haarman.supertooltips)
- 
Android ViewBadger为其他View添加角标等
项目地址：[https://github.com/jgilfelt/android-viewbadger](https://github.com/jgilfelt/android-viewbadger)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/android-viewbadger.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/android-viewbadger.apk?raw=true)
效果图：[https://github-camo.global.ssl.fastly.net/a705a3e88c75ae2394943bd7c56f725697616ea8/687474703a2f2f7777772e6a65666667696c66656c742e636f6d2f766965776261646765722f76622d31612e706e67](https://github-camo.global.ssl.fastly.net/a705a3e88c75ae2394943bd7c56f725697616ea8/687474703a2f2f7777772e6a65666667696c66656c742e636f6d2f766965776261646765722f76622d31612e706e67)
- 
Android Sliding Up Panel 可拖动的View，能在当前Activity上扶起一个可拖动的Panel
项目地址：[https://github.com/umano/AndroidSlidingUpPanel](https://github.com/umano/AndroidSlidingUpPanel)
Demo地址：[https://play.google.com/store/apps/details?id=com.sothree.umano](https://play.google.com/store/apps/details?id=com.sothree.umano)
APP示例：Google Music精简播放栏
- 
android-times-square Android日历部件
支持选取单个日期，多个日期，及日期区间段和对话框形式显示
项目地址：[https://github.com/square/android-times-square](https://github.com/square/android-times-square)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/times-square-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/times-square-demo.apk?raw=true)
- 
android-calendar-card 日历
项目地址：[https://github.com/kenumir/android-calendar-card](https://github.com/kenumir/android-calendar-card)
Demo地址：[https://play.google.com/store/apps/details?id=com.wt.calendarcardsample](https://play.google.com/store/apps/details?id=com.wt.calendarcardsample)
效果图：![Renderings](https://raw.github.com/kenumir/android-calendar-card/master/calendar-card-sample/_work/device-2013-10-12-151801.png)
- 
ColorPickerView 颜色选择器，支持PopupWindows或新的Activity中打开
项目地址：[https://code.google.com/p/color-picker-view/](https://code.google.com/p/color-picker-view/)
效果图：![Renderings](https://github-camo.global.ssl.fastly.net/c1b120db21965381e28349dcb019a8ec5525c326/687474703a2f2f6f6934312e74696e797069632e636f6d2f333363366d6d382e6a7067)
- 
HoloColorPicker 颜色选择器
项目地址：[https://github.com/LarsWerkman/HoloColorPicker](https://github.com/LarsWerkman/HoloColorPicker)
Demo地址：[https://docs.google.com/file/d/0BwclyDTlLrdXRzVnTGJvTlRfU2s/edit](https://docs.google.com/file/d/0BwclyDTlLrdXRzVnTGJvTlRfU2s/edit)
- 
AndroidWheel Android Wheel支持城市、多种日期时间、密码、图片
项目地址：[https://github.com/sephiroth74/AndroidWheel](https://github.com/sephiroth74/AndroidWheel)
效果图：![Renderings](https://github-camo.global.ssl.fastly.net/e9a9430ac807dcfcde5ebc407684a8b7459bb9bd/687474703a2f2f6661726d362e737461746963666c69636b722e636f6d2f353533322f31313632313532383738365f323230633034306261355f6f2e6a7067)
- 
android-flowtextview文字自动环绕其他View的Layout
项目地址：[https://code.google.com/p/android-flowtextview/](https://code.google.com/p/android-flowtextview/)
效果图：[http://i949.photobucket.com/albums/ad332/vostroman1500/1.png](http://i949.photobucket.com/albums/ad332/vostroman1500/1.png)
- 
Segmented Radio Buttons for Android iOS’s segmented controls的实现
项目地址：[https://github.com/vinc3m1/android-segmentedradiobutton](https://github.com/vinc3m1/android-segmentedradiobutton)
Demo地址：[https://github.com/thquinn/DraggableGridView/blob/master/bin/DraggableGridViewSample.apk?raw=true](https://github.com/thquinn/DraggableGridView/blob/master/bin/DraggableGridViewSample.apk?raw=true)
效果图：![Renderings](https://raw.github.com/vinc3m1/android-segmentedradiobutton/master/screens/segmentedradio.png)
- 
TableFixHeaders 第一列固定的Table
项目地址：[https://github.com/InQBarna/TableFixHeaders](https://github.com/InQBarna/TableFixHeaders)
Demo地址：[http://bit.ly/13buAIq](http://bit.ly/13buAIq)
- 
Android Form EditText 验证输入合法性的编辑框
支持输入、英文、ip、url等多种正则验证
项目地址：[https://github.com/vekexasia/android-edittext-validator](https://github.com/vekexasia/android-edittext-validator)
Demo地址：[https://play.google.com/store/apps/details?id=com.andreabaccega.edittextformexample](https://play.google.com/store/apps/details?id=com.andreabaccega.edittextformexample)
- 
UITableView ios风格控件
包括Button、ListView、TableView
项目地址：[https://github.com/thiagolocatelli/android-uitableview](https://github.com/thiagolocatelli/android-uitableview)
Demo地址：[https://github.com/Trinea/TrineaDownload/blob/master/ui-tableview-demo.apk?raw=true](https://github.com/Trinea/TrineaDownload/blob/master/ui-tableview-demo.apk?raw=true)
- 
ATableView ios风格控件
项目地址：[https://github.com/dmacosta/ATableView](https://github.com/dmacosta/ATableView)
Demo地址：[https://play.google.com/store/apps/details?id=com.nakardo.atableview.demo](https://play.google.com/store/apps/details?id=com.nakardo.atableview.demo)
- 
UndoBar屏幕底部显示取消或是确认的PopupWindows
项目地址：[https://github.com/soarcn/UndoBar](https://github.com/soarcn/UndoBar)
效果图：![Renderings](https://github.com/soarcn/UndoBar/blob/master/art/redo.png?raw=true)
- 
Inscription可用于展示应用change和new feature信息
项目地址：[https://github.com/MartinvanZ/Inscription](https://github.com/MartinvanZ/Inscription)
- 
ActivityTransition Activity切换动画，包括渐变、flip、某个位置进入等等
项目地址：[https://github.com/ophilbert/ActivityTransition](https://github.com/ophilbert/ActivityTransition)
使用介绍：[https://github.com/jfeinstein10/JazzyViewPager/blob/master/JazzyViewPager.apk?raw=true](https://github.com/jfeinstein10/JazzyViewPager/blob/master/JazzyViewPager.apk?raw=true)
效果图：类似桌面左右切换的各种效果，不过桌面并非用ViewPager实现而已
- 
Cropper 图片局部剪切工具，可触摸控制选择区域或旋转
项目地址：[https://github.com/edmodo/cropper](https://github.com/edmodo/cropper)
使用介绍：[https://github.com/edmodo/cropper/wiki](https://github.com/edmodo/cropper/wiki)
效果图：![Renderings](https://github-camo.global.ssl.fastly.net/e4fde77bf41d4a60b234b4e268e5cfa8c17d9b6f/687474703a2f2f692e696d6775722e636f6d2f334668735467666c2e6a7067)
- 
GlowPadBackport将Android4.2的锁屏界面解锁扩展到Android1.6及1.6+
项目地址：[https://github.com/rock3r/GlowPadBackport](https://github.com/rock3r/GlowPadBackport)
Demo地址：[https://play.google.com/store/apps/details?id=net.sebastianopoggi.samples.ui.GlowPadSample](https://play.google.com/store/apps/details?id=net.sebastianopoggi.samples.ui.GlowPadSample)
效果图：![Renderings](https://lh6.ggpht.com/U070b6Lh6cVsVwx4jN-5nq0xqiB1PBzrYABPeJIEe2hZQ5UWOxc-FDUG77wADelToHA=h310-rw)
- 
GlowPadView Android4锁屏界面解锁
项目地址：[https://github.com/nadavfima/GlowPadView](https://github.com/nadavfima/GlowPadView)
效果图：[https://raw.github.com/nadavfima/GlowPadView/master/example.png](https://raw.github.com/nadavfima/GlowPadView/master/example.png)
- 
android-lockpattern Android的图案密码解锁
项目地址：[https://code.google.com/p/android-lockpattern/](https://code.google.com/p/android-lockpattern/)
Demo地址：[https://play.google.com/store/apps/details?id=group.pals.android.lib.ui.lockpattern.demo](https://play.google.com/store/apps/details?id=group.pals.android.lib.ui.lockpattern.demo)
使用介绍：[https://code.google.com/p/android-lockpattern/wiki/QuickUse](https://code.google.com/p/android-lockpattern/wiki/QuickUse)
示例APP：Android开机的图案密码解锁，支付宝的密码解锁
