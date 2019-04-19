# WebView.loadUrl()在真机环境中执行即报错的问题 - 左直拳的马桶_日用桶 - CSDN博客
2012年12月29日 16:36:46[左直拳](https://me.csdn.net/leftfist)阅读数：5715
对于一个毫无JAVA和Android开发经验，甚至不久前才对2G、3G有点粗略概念，几天前才用上智能手机的码农来说，千辛万苦拼凑出来的apk，放到真机里安装、执行，结果老实不客气地弹出一个“意外停止执行”的对话框，这种打击是多么的沉重！
那一刻，我觉得天都倒塌了。
这是一个多么简陋的安卓客户端，它只有一个重要的功能，就是开启后，自动打开我们网站。我用的是WebView。
可问题偏偏出现在这里。就是这一句：
```java
WebView.loadUrl(url);
```
什么状况？在模拟器里执行得好好的。
我怀疑是版本的问题，开发默认用的是Android4.2，但我的手机是2.3。
于是在开发环境将版本调回2.1，够低了吧。问题依旧。
难道我写错了什么？是在主线程访问了网络报错吗？明明是放在 new Thread 里的。
难道是在非主线程描绘了UI？也没有啊。
再次疯狂搜索，个个大牛都说的差不多，要么在xml里定义一个啦，要么在代码里new一个啦，然后记得开启 javascript 功能喔。。。问题是我照抄这些代码，放到真机里运行，一样会报错。
该不会是我这个手机不支持WebView吧。天哪，没有WebView，我简直不能活，谁那么牛可以去写一个浏览器，或者直接去处理那些HTML标记？
我想起不久前用过一个自动生成网站安卓客户端的软件，生成了一个apk，于是拿到我的手机上安装，结果人家一切都很正常，可以打开我们网站。问题是，它用的是什么控件呢？
反编译这个apk，代码看不懂，但它里面也是用到了WebView！我精神大振！不过它的AndroidMainifest.xml里面有这么几句：
```
<uses-sdk android:minSdkVersion="4" />
    <uses-permission android:name="android.permission.RESTART_PACKAGES" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
```
其中
`<uses-permission android:name="android.permission.INTERNET" />`
我是有的，其他就没有。结果将这几句加上以后，问题解决。
其实这几条语句中，只须补加后面这3条：
```
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
```
就可以。也许中间`    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />`这条不用也可以，但我嫌麻烦，没有继续测试。这个权限好像是获取大致GPS位置的，主要用于加强手机广告的地域性。
鸣谢，参考文章：
[Android APK反编译详解（附图）](http://blog.csdn.net/sunboy_2050/article/details/6727581)
http://blog.csdn.net/sunboy_2050/article/details/6727581
# 网站Android客户端制作软件绿色版.zip
http://vdisk.weibo.com/s/44B7L
我的手机：酷派8180，安卓：2.3.7，中移动。

