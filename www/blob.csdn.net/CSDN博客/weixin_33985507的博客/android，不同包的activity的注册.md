# android，不同包的activity的注册 - weixin_33985507的博客 - CSDN博客
2012年05月31日 14:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
众所周知，一个android应用，一般会有多个activity，多个activity通过Intent来进行通信，而每个activity要想正常“出生”，必须要被AndroidManifest.xml注册，一下粗体部分就是一个注册的实例其实注册的全路径是（package）：**com.lql.bill.BillQueryActivity**
```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"**    package="com.lql.bill"**
    android:versionCode="1"
    android:versionName="1.0" >
    <uses-sdk android:minSdkVersion="7" />
    <application
        android:icon="@drawable/app_logo"
        android:label="@string/app_name" >
        **<activity            android:name=".BillQueryActivity"            android:label="@string/app_name"             android:theme="@style/title">            <intent-filter>                <action android:name="android.intent.action.MAIN" />                <category android:name="android.intent.category.LAUNCHER" />            </intent-filter>        </activity>**
        
    </application>
    <uses-permission android:name="android.permission.INTERNET"/>
    <uses-permission android:name="android.permission.READ_PHONE_STATE"/>
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"/>
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"/>
</manifest>
```
对不不同包的我们把类似的加入进去就是了：关键是路径写对：com.lql.对应的不同的包名.xxxActivity
```
<activity
            android:name="com.lql.**diffrentPackage**.BillQueryActivity"
            android:label="@string/app_name" 
            android:theme="@style/title">
        </activity>
```
算是对今天logcat输出错误，某activity未被注册的解决，OK，接着搞...
