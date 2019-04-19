# 使用android隐藏api实现亮度调节 - xqhrs232的专栏 - CSDN博客
2011年05月31日 15:14:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：840标签：[android																[api																[include																[module																[build																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/ziip/archive/2011/03/01/1968388.html](http://www.cnblogs.com/ziip/archive/2011/03/01/1968388.html)
Android 1.5之后将调节亮度的api隐藏了，不过android的源码中依然有方法可以实现，如下：
[view source](http://writeblog.csdn.net/#viewSource)[print](http://writeblog.csdn.net/#printSource)[?](http://writeblog.csdn.net/#about)
```java
IPowerManager power = IPowerManager.Stub.asInterface(ServiceManager
```
`              `
```java
.getService(
```
```java
"power"
```
```java
));
```
```java
**if**
```
```java
(power !=
```
```java
**null**
```
```java
) {
```
`   `
```java
power.setBacklightBrightness(brightness);
```
上面的代码无法直接在官方的sdk中调用，会提示找不到IpowerManager和I ServiceManager，需要下载android的源码，将调用这个方法的工程在android源码环境下编译。
编译方法如下，在调用该方法的工程根目录下创建Android.mk文件：
```
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
 
 
LOCAL_MODULE_TAGS := user
 
LOCAL_SRC_FILES := $(call all-subdir-java-files)
 
LOCAL_PACKAGE_NAME := myProject
LOCAL_CERTIFICATE := platform
 
include $(BUILD_PACKAGE)
```
另外，还需要在应用程序的AndroidManifest.xml中的manifest节点中加入android:sharedUserId="android.uid.system"这个属性。(更正，这个属性不需要添加）
将工程目录拷贝到/packages/apps/下，执行mmm编译。会在out目录下生成apk文件，这里的apk还无法直接使用，会提示签名不符，原因是程序想要运行在系统进程中还要有目标系统的platform key。
接下来使用platform key对apk签名：
1. 用压缩软件打开apk文件，删掉META-INF目录下的CERT.SF和CERT.RSA两个文件。
2. 使用目标系统的platform密钥来重新给apk文件签名。首先找到密钥文件，在我的Android源码目录中的位置是"build/target/product/security"，下面的platform.pk8和platform.x509.pem两个文件。然后用Android提供的Signapk工具来签名，signapk的源代码是在"build/tools/signapk"下，用法为"java –jar signapk.jar platform.x509.pem platform.pk8 input.apk
 output.apk"
签名后的apk就可以使用了，通过这种方法还可以使用其他的隐藏api。

