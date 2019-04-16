# 解决添加codova plugin 编译出现问题：Execution failed for task '-processDebugManifest' - ggabcda的博客 - CSDN博客





2017年08月17日 14:18:25[ggabcda](https://me.csdn.net/ggabcda)阅读数：988








## 问题背景：

  ionic3项目上，添加cordova-plugin-cszbar，编译运行在android平台上 ，结果编译不成功。出现以下问题。

Element uses-feature#android.hardware.camera at AndroidManifest.xml:23:5-84 duplicated with element declared at AndroidManifest.x
ml:17:5-60
E:\xxxx\xxx\xxx\app\platforms\android\AndroidManifest.xml Error:
Validation failed, exiting

FAILURE: Build failed with an exception.

* What went wrong:
Execution failed for task ':processDebugManifest'.
> Manifest merger failed with multiple errors, see logs

多次移除安卓平台重新编译依旧这个问题。

编译后打开androidMainfest.xml文件在构建后有以下内容

```
<uses-feature android:name="android.hardware.camera" android:required="false" />

<uses-feature android:name="android.hardware.camera.autofocus" android:required="false" />

<uses-feature android:name="android.hardware.camera.flash" android:required="false" />

<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />

<uses-feature android:name="android.hardware.camera" />

<uses-feature android:name="android.hardware.camera.autofocus" />
```



## `平台信息：`

![](http://images2017.cnblogs.com/blog/833855/201708/833855-20170816102159021-1267430481.png)



## 问题原因：

原因是我已经安装了一个插件也是跟cordova-plugin-cszbar功能相同

## 解决方案：

1.最简单的方法是移除我之前安装的那个插件。

2.假如出现这种情况，两个插件都想同时存在。因为这两个插件他们都有相同的uses-feature.所以编译后会将引用插件的重复的权限条目添加到了androidMainfest.xml,

添加好androidp平台后，打开AndroidManifest.xml文件，可以看类似于：`<uses-permission android:name="android.permission.CAMERA" /> 删除，重新构建，结果没有报错了。`







