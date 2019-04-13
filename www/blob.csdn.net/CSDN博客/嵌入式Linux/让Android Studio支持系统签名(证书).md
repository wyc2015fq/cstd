
# 让Android Studio支持系统签名(证书) - 嵌入式Linux - CSDN博客

2018年11月01日 10:13:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：192


有时候，我们开发的apk需要用到系统权限，需要在AndroidManifest.xml中添加共享系统进程属性：
`android:sharedUserId="android.uid.system" 
android:sharedUserId="android.uid.shared" 
android:sharedUserId="android.media"`这时候apk的签名就需要是系统签名(platform、shared或media)才能正常使用。
----------------------------------------------------------------------------------------------
## 常用系统签名方式
## 1、
## Android源码环境下签名
这种方式比较麻烦，你需要有编译过的源码环境，并按如下步骤：
1、拷贝App源码到Android源码的`packages/apps/`目录下，且App源码是普通(Eclipse)格式的
2、配置Android.mk，在其中添加
LOCAL_CERTIFICATE := platform 或 shared 或 media
3、使用**mm****-B**编译App，生成的apk即系统签名
-----------------------------------------------------------------------------------------------
## 2、
## 手动重新签名
这种方式比在源码环境下签名简单，App可以在Eclipse或Android Studio下编译，然后给apk重新签名即可。
但这种方式在频繁调试的时候比较痛苦，即使写成脚本，也需要重复一样的操作。
**2.1、相关文件**
platform.x509.pem、platform.pk8、signapk.jar
**文件位置**
platform.x509.pem、platform.pk8:
../build/target/product/security
signapk.jar:
../out/host/linux-x86/framework
signapk源码路径:
../build/tools/signapk
**2.2、签名命令**
java -jar signapk.jar platform.x509.pem platform.pk8 old.apk new.apk
**步骤**
1、将相关文件及源apk文件置于同一路径下
2、检查源apk包，去掉`META-INF/CERT.SF`和`META-INF/CERT.RSA`文件
3、执行签名命令即可
-----------------------------------------------------------------------------------------------
## 3、
## Android Studio系统签名
让Android Studio集成系统签名，需要用到一个工具`keytool-importkeypair`，详见下文。
> keytool-importkeypair – A shell script to import key/certificate pairs into an existing Java keystore

这个工具的作用是将系统签名的相关信息导入到已有的签名文件里。可从**[这里](http://link.zhihu.com/?target=https%3A//github.com/getfatday/keytool-importkeypair)**下载。
工具的使用方法可以通过--help或README.textile来寻求帮助。
**相关文件**
platform.x509.pem、platform.pk8、keytool-importkeypair、demo.jks、signature.sh
我的做法是在App根目录新建Signature文件夹专门存放签名相关文件。
**步骤**
**1、生成demo.jks签名文件**
**相关文件**
platform.x509.pem、platform.pk8、keytool-importkeypair、demo.jks、signature.sh
我的做法是在App根目录新建Signature文件夹专门存放签名相关文件。
**步骤**
**生成demo.jks签名文件**
![](https://pic2.zhimg.com/80/v2-d9dd638604900eb845138b7bfc895155_hd.jpg)
在这里选择签名的文件
![](https://pic3.zhimg.com/80/v2-3740ea8410048c2f121c15463928c0a2_hd.jpg)
图片上的密码要和命令的密码对应，别名Alias也要对应起来，还有路径，其他信息是次要的
这个步骤后，可以在对应的位置生成一个demo.jks文件
![](https://pic4.zhimg.com/80/v2-bc3b80662037c6409685b7bed95b0ca7_hd.jpg)
**2、编写签名脚本signature.sh，利用步骤1生成的文件生成新的文件**
**内容如下：**
**注意！！一定要把这个文件放到linux环境下编译，在windows下编译目前我还没测试成功，我相信坐签名的同学一定有linux编译环境**
`#!/bin/sh
# 转换平台签名命令
./keytool-importkeypair -k demo.jks -p 123456 -pk8 platform.pk8 -cert platform.x509.pem -alias demo
# demo.jks : 签名文件
# 123456 : 签名文件密码
# platform.pk8、platform.x509.pem : 系统签名文件
# demo : 签名文件别名`
$ sudo chmod a+x signature.sh
执行脚本：
$ ./signature.sh
**注意！！这时候会生成新的demo.jks，把这个demo.jks替换掉原来的那个才可以。**
**3、配置builde.gradle**
在android区域下(与defaultConfig同级)添加配置：
**注意 ！！Android studio有两个builde.gradle，要在有Android关键字的那个添加，别搞错了。**
`signingConfigs {
        release {
            storeFile file("../signature/demo.jks")
            storePassword '123456'
            keyAlias 'demo'
            keyPassword '123456'
        }
        debug {
            storeFile file("../signature/demo.jks")
            storePassword '123456'
            keyAlias 'demo'
            keyPassword '123456'
        }`
这样debug或release apk就带有系统签名了。
如果想直接`Run app`就是release版且带系统签名的apk，还需修改：
**修改buildTypes：**
`buildTypes {
    release {
        minifyEnabled false
        proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.txt'
        signingConfig signingConfigs.release
    }
}`这样直接`Run app`就是带系统签名的release版apk了。
如果对你有用，请关注微信公众号【**嵌入式Linux**】
参考：[Android Studio自动生成带系统签名的apk](http://link.zhihu.com/?target=https%3A//blog.csdn.net/cxq234843654/article/details/51557025)

