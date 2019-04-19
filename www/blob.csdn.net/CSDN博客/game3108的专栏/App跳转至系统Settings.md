# App跳转至系统Settings - game3108的专栏 - CSDN博客
2015年07月06日 15:53:46[game3108](https://me.csdn.net/game3108)阅读数：224
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
之前在年初的时候，曾经查过这个问题。
当时找到的并试验下来的结果时：对于iOS6以后的系统，并不能使用openurl打开相应的设置界面。
（iOS8可以[[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];)
最近又看这个问题，发现了新的进展，居然可以跳转设置界面了，原文如下，记录一下，学习使用：
链接：http://www.2cto.com/kf/201506/410295.html
很多著名和非著名的App有在App内通过某种方式跳转到系统Settings的功能。但是iOS5.1以后不能正常调用了，经过模索，本人总结了以下两种方法：
方法一:
其实稍微**百度**下你都可以找到以下这个列表。
[?](http://www.2cto.com/kf/201506/410295.html#)
```
```java
<code>Notification
 — prefs:root=NOTIFICATI_ID
```
```java
Phone
 — prefs:root=Phone
```
```java
Photos
 — prefs:root=Photos
```
```java
Profile
 — prefs:root=General&path=ManagedConfigurationList
```
```java
Reset
 — prefs:root=General&path=Reset
```
```java
Safari
 — prefs:root=Safari
```
```java
Siri
 — prefs:root=General&path=Assistant
```
```java
Sounds
 — prefs:root=Sounds
```
```java
Software
 Update — prefs:root=General&path=SOFTWARE_UPDATE_LINK
```
```java
Store
 — prefs:root=STORE
```
```java
Twitter
 — prefs:root=TWITTER
```
```java
Usage
 — prefs:root=General&path=USAGE
```
```java
VPN
 — prefs:root=General&path=Network/VPN
```
```java
Wallpaper
 — prefs:root=Wallpaper
```
```java
Wi-Fi
 — prefs:root=WIFI</code>
```
```
是的，本人接下来会提到这个调用：
[?](http://www.2cto.com/kf/201506/410295.html#)
```
```java
<code>[[UIApplication
 sharedApplication] openURL:[NSURL URLWithString:@
```
```java
"prefs:root=WIFI"
```
```java
]];</code>
```
```
这个在iOS6，7和8中确实可以跳转，只是还少了一个步骤。
![加载中...](http://www.2cto.com/statics/images/s_nopic.gif)
截图1.png
在**URL Types**中添加一个新项。我没有深入研究，只填写**prefs**就可以了。
![截图2.png](http://upload-images.jianshu.io/upload_images/299018-1be3099ebad9a128.png?imageView2/2/w/1240/q/100)
截图2.png
这样子，以上介绍的方法就可以跳转了。
除非万不得已，在可选择的情况下还是使用以下调用：（就算扩展[阅读](http://book.2cto.com/)吧）
iOS8新支持：
[?](http://www.2cto.com/kf/201506/410295.html#)
```
```java
<code>[[UIApplication
 sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];</code>
```
```
方法二:
还是用[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"prefs:root=WIFI"]];
但是只能在alertview里可用。

