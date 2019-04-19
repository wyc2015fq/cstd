# 兼容 Android 4.4 透明状态栏与导航栏 - =朝晖= - 博客园
# [兼容 Android 4.4 透明状态栏与导航栏](https://www.cnblogs.com/dhcn/p/7121296.html)
http://www.apkbus.com/[Android](http://lib.csdn.net/base/android)-163388-1-1.html?_dsign=73d41229
[android](http://lib.csdn.net/base/android) 系统自4.2 开始 UI 上就没多大改变，4.4 也只是增加了透明状态栏与导航栏的功能，如图
左边为 4.2.2 右边为 4.4.2
![](http://www.apkbus.com/data/attachment/forum/201402/27/154714jxaom0117a66o81t.png)![](http://www.apkbus.com/data/attachment/forum/201402/27/154715kdfcmvm941gtkd44.png)
那么现在我就来给大家讲解下如何使用这个新特性，让你的 app 跟随潮流，当然如果你不在乎外观就算了，
使用这个特性能开发出很漂亮的UI，尤其对于 google 原生系统，屏幕下方的导航栏白白占据一块屏幕空间，看起来很不爽
![](http://www.apkbus.com/data/attachment/forum/201402/27/154958qgczo5a17ia3u3c4.png)
图为 sommth 客户端，
OK废话不多讲，开始讲技术吧，第一种方法，在代码设置：
- if(VERSION.SDK_INT >= VERSION_CODES.KITKAT) {
-                                 //透明状态栏
-                                 getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
-                                 //透明导航栏
-                                 getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
-                         }
复制代码
直接调用上面2行代码可以透明，但是你会发现你的 view 跑到 actionbar 上面去了，很明显 google 的意图是使你的 view 可以占据整个屏幕，然后 状态栏和导航栏 透明覆盖在上面很明显这样不可行。
那有没有办法使你的 view 保持原来大小呢？
有，你需要在这个 activity 的 layout xml 文件添加两个属性
- <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
-     android:layout_width="fill_parent"
-     android:layout_height="fill_parent"
-     android:gravity="center_horizontal"
- 
-     android:fitsSystemWindows="true"
-     android:clipToPadding="true"
- 
-     android:orientation="vertical" >
复制代码
这样状态栏的背景就是你的 activity 的主背景，倘若actionbar 在，将会很难看，如图：
![](http://www.apkbus.com/data/attachment/forum/201402/27/162928k0jtdidttdiscm05.png)
事实证明，google 并没有提供一个比较好的解决方案，他的 透明状态栏与导航栏的应用局限于，全屏阅读文字或玩游戏那种情景，
第二种方式，是是设置 theme 属性
- android:theme="@android:style/Theme.DeviceDefault.Light.NoActionBar.TranslucentDecor"
-             android:theme="@android:style/Theme.Holo.Light.NoActionBar.TranslucentDecor"
-             android:theme="@android:style/Theme.Holo.NoActionBar.TranslucentDecor"
复制代码
如果你使用自定主题，只需在在 values-19 文件添加以下属性：
- <style name="AppBaseTheme" parent="android:Theme.Holo.Light.DarkActionBar">
- 
-         <!-- API 19 theme customizations can go here. -->
-         <item name="android:windowTranslucentStatus">true</item>
-         <item name="android:windowTranslucentNavigation">true</item>
-     </style>
复制代码
刚刚说了这个使用有局限性，不过好在有一个开源的东西
[https://github.com/jgilfelt/SystemBarTint](https://github.com/jgilfelt/SystemBarTint)
可以设置 statusbar 背景，原理是在 Window 的 DocView 添加 view，大家可以下载这个项目学习如何使用
![](http://www.apkbus.com/static/image/filetype/zip.gif)[SystemBarTint-master.zip](http://www.apkbus.com/forum.php?mod=attachment&aid=MTM1NjM0fGViZTc5MTNjfDE0NTA3NDk3MzJ8MHwxNjMzODg%3D)(513.74 KB, 下载次数: 1995) 

