# Android开发中遇到的问题(四)——Android中WARNING: Application does not specify an API level requirement!的解决方法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发中遇到的问题(四)——Android中WARNING: Application does not specify an API level requirement!的解决方法](https://www.cnblogs.com/xdp-gacl/p/4340882.html)



　　今天在手机上调试运行Andorid项目时，发现Console打印出"**WARNING: Application does not specify an API level requirement**!"这样的警告信息，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/160006513709014.png)

　　虽然不影响项目的正常运行，不过还是要找出原因，上网查了一下出现警告的原因，原来是创建项目时AndroidManifest.xml文件中没有指定Min SDK Version

　　解决办法：修改AndroidManifest.xml文件，在<manifest> </manifest> 之间加入如下代码：

```
<uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="21"/>
```

　　使用android:minSdkVersion指明运行这个Android应用的最低SDK版本，minSdkVersion="8"表示使用的Android 2.2的版本，android:targetSdkVersion指明编译这个Android应用的SDK版本，targetSdkVersion="21"表示使用的Android 5.0.1的版本。

　　添加<uses-sdk/>节点的配置项之后，如果，<application></application>节点没有设置android:allowBackup属性，那么就为<application>节点设置android:allowBackup="true"属性，否则<application>节点会出现警告信息，修改过后的AndroidManifest.xml文件的内容如下：

```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
      package="me.gacl.ui"
      android:versionCode="1"
      android:versionName="1.0">
      
  <!-- 使用android:minSdkVersion指明运行这个Android应用的最低SDK版本
  minSdkVersion="8"表示使用的Android 2.2的版本
  android:targetSdkVersion指明编译这个Android应用的SDK版本
  targetSdkVersion="21"表示使用的Android 5.0.1的版本
   -->
  <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="21"/>
    
    <application 
        android:label="@string/app_name" 
        android:icon="@drawable/ic_launcher"
        android:allowBackup="true">
        <activity android:name="FrameLayoutTest"
                  android:label="@string/app_name">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
    
</manifest>
```

　　AndroidManifest.xml文件这样修改过之后，运行Android项目时就不会出现警告信息了。









