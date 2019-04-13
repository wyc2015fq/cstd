
# android 4.2修改设置菜单的背景颜色 - 嵌入式Linux - CSDN博客

2013年09月06日 15:09:12[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：12107


设置中的背景主要来主题的设置, 在4.X后, android添加了新的主题: Holo
从Settings/AndroidManifest.xml中找到:
Xml代码![收藏代码](http://ansonlai.iteye.com/images/icon_star.png)
<applicationandroid:label="@string/settings_label"
android:icon="@mipmap/ic_launcher_settings"
android:taskAffinity=""
android:theme="@android:style/Theme.Holo"
android:uiOptions="splitActionBarWhenNarrow"
android:hardwareAccelerated="true"
android:supportsRtl="true"
android:name=".Screenshot">
android:theme="@android:style/Theme.Holo",
在Theme中, 定义了window的background,
<item name="windowBackground">@android:drawable/screen_background_selector_dark</item>
最终会找到以下文件:
Xml代码![收藏代码](http://ansonlai.iteye.com/images/icon_star.png)
frameworks/base/core/res/res/drawable/background_holo_dark.xml
@@ -17,6 +17,7 @@
<shapexmlns:android="http://schemas.android.com/apk/res/android">
<gradient
android:startColor="\#ff000000"
-android:endColor="\#ff272d33"
+android:endColor="\#ff000000"
android:angle="270"/>
</shape>
所以, 只需要修改上面文件对Color的定义, 上面修改让背景颜色变成了黑色，默认的话是渐变色，一些屏对渐变色的显示不好。所以改成黑色的话是比较好的。
还有几个命令

```python
1、mmm frameworks/base/core/res/                                      编译源码生成APK
2、adb remount               这个命令使adb命令正常连接
3、adb push out/target/product/rk30sdk/system/framework/framework-res.apk /system/framework/     不能直接adb install -r 这个apk要push到这个目录下面
4、adb reboot                            重启设备让上面第3步的操作生效
```





