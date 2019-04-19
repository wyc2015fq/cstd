# startActivityForResult报错 - 左直拳的马桶_日用桶 - CSDN博客
2012年12月11日 18:38:41[左直拳](https://me.csdn.net/leftfist)阅读数：3348
个人分类：[Android](https://blog.csdn.net/leftfist/article/category/1302161)
startActivityForResult报错！
```java
startActivityForResult(new Intent(MainActivity.this, OtherActivity.class), 0);
```
骗子！从教程、网上滴字不漏抄来的例子，运行居然报错！
后来抄翻互联网，才知道要在AndroidManifest.xml 加上这么一句：
<activity android:name=".OtherActivity" />
其中，OtherActivity是新加入的Activity。启动Activity是MainActivity。完整的AndroidManifest.xml是：
```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.cq.activity2"
    android:versionCode="1"
    android:versionName="1.0" >
    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="16" />
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name=".MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <activity android:name=".OtherActivity" /> 
    </application>
</manifest>
```
但教程和网上的例子，只一味地罗列代码，而这个配置文件却提都不提，真他妈的。

