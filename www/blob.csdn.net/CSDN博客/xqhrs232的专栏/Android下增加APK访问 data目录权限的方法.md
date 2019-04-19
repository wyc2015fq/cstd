# Android下增加APK访问/data目录权限的方法 - xqhrs232的专栏 - CSDN博客
2012年04月11日 18:23:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4969
原文地址::[http://www.eoeandroid.com/thread-74615-1-1.html](http://www.eoeandroid.com/thread-74615-1-1.html)
我的android手机data目录下是没有权限访问的opendir failed, Permission denied
而我的开发的应用程序却要使用到这个目录
如何设计我的应用程序可以自动打开权限访问此目录，
或者重新刷机就可以让手机的data正常访问？
android.permission.WRITE_OWNER_DATA
android.permission.READ_OWNER_DATA
看下这两个可以不？没试过，不晓得是否可以
android:sharedUserId="android.uid.system"
网上说这个可以
我的办法，
首先在mk文件里添加LOCAL_CERTIFICATE := platform，认证为platform，解决权限问题
然后再manifest.xml中添加       android:sharedUserId="android.uid.system" 更改你的沙盒ID
我这是在linux下做的，eclipse可能不行 你可以先试下更改userid，然后再签名是签成platfor，如果你用debug签名应该不行，你可以先试下。
有结果欢迎讨论
//================================================
备注::
 1>
|**android.permission.READ_OWNER_DATA**|允许程序读取所有者数据|
|**android.permission.WRITE_OWNER_DATA**|允许一个程序写入但不读取所有者数据|
2>android:sharedUserId="android.uid.system" 不在APK里面设置这一项，然后对APK进行signapk签名的话，APK在/data目录就没法创建文件，就会产生Permission denied错误
            
