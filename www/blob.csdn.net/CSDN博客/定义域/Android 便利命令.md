# Android 便利命令 - 定义域 - CSDN博客
2016年04月07日 16:28:30[定义域](https://me.csdn.net/u010278882)阅读数：223
卸载应用 adb shell pm uninstall APP_NAME 
启动应用 adb shell am start -n APP_PKG_NAME/MAIN_ACT_NAME 
查看APK基本信息 aapt dump badging APK_FILE 
查看APK签名 jarsigner -verify -verbose -certs APK_FILE
```java
// 重装（用于带签名调试）
adb shell pm uninstall com.p.w.box
adb shell pm uninstall com.p.w.box.deamon
adb shell rm -r /sdcard/app.apk
d:
cd D:\var\workspace\p_box_v1.1\keystore
adb install p_box_v1.1.apk
adb shell am start -n com.p.w.box/com.p.box.main.MainActivity
```
