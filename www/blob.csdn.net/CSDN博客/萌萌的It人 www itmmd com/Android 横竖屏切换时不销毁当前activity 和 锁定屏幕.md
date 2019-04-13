
# Android 横竖屏切换时不销毁当前activity 和 锁定屏幕 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月09日 17:03:34[Jlins](https://me.csdn.net/dyllove98)阅读数：3793


首先在Mainifest.xml的Activity元素中加入android:configChanges="orientation|keyboardHidden"属性
<activityandroid:name=".FileBrowser"android:label="@string/app_name"android:configChanges="orientation|keyboardHidden">
<intent-filter>
<actionandroid:name="android.intent.action.MAIN"/>
<categoryandroid:name="android.intent.category.LAUNCHER"/>
</intent-filter>
</activity>
加入这条属性的含义是，应用程序将会处理屏幕方向和键盘状态(推出或合上）信息的改动。但对于其他的设备配置信息的改动则会由Android系统来处理（销毁当前Activity，然后重启一个新的Activity实例）。
那么，现在还需要在java代码的activity子类中加入配置信息改动的处理代码。这个也很简单
/**
* onConfigurationChanged
* the package:android.content.res.Configuration.
* @param newConfig, The new device configuration.
* 当设备配置信息有改动（比如屏幕方向的改变，实体键盘的推开或合上等）时，
* 并且如果此时有activity正在运行，系统会调用这个函数。
* 注意：onConfigurationChanged只会监测应用程序在AnroidMainifest.xml中通过
* android:configChanges="xxxx"指定的配置类型的改动；
* 而对于其他配置的更改，则系统会onDestroy()当前Activity，然后重启一个新的Activity实例。
*/
@Override publicvoid onConfigurationChanged(Configuration newConfig) {
super.onConfigurationChanged(newConfig); // 检测屏幕的方向：纵向或横向
if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
//当前为横屏， 在此处添加额外的处理代码
} else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
//当前为竖屏， 在此处添加额外的处理代码
}
//检测实体键盘的状态：推出或者合上
if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO){
//实体键盘处于推出状态，在此处添加额外的处理代码
} else if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES){
//实体键盘处于合上状态，在此处添加额外的处理代码
}
}
别忘了在java文件中加上import android.content.res.Configuration。
这样就OK了，屏幕方向改变时，应用程序的显示界面也会随着改动，而不是被销毁！

Activity中还有一属性和屏幕方向有关：
<activity . . . android:screenOrientation=["unspecified" | "user" | "behind" | "landscape" | "portrait" | "sensor" | "nosensor"] . . . </activity>
比如，在Mainifest.xml的Activity元素中增加这么一个属性：
*android:screenOrientation="portrait"*
则无论手机如何变动，拥有这个属性的activity都将是竖屏显示。
*android:screenOrientation="landscape"*，为横屏显示。
这里提一个小知识，Anroid模拟器中，快捷键"ctrl+F11"可以实现转屏。

参考资料：


## android学习笔记（60）使用Bundle在Activity之间文换教接
## android学习笔记（59） android开发启动和关闭activity

## [android学习笔记（58）Activity学习过程](http://www.itmmd.com/201502/597.html)
## 更多
## [android开发](http://www.itmmd.com/)
## 教程



