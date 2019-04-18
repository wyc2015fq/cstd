# Android studio 生成签名问题 - weixin_33985507的博客 - CSDN博客
2018年04月26日 19:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
问题现象：发布一个签名之后的应用。在模拟器和android 7.0真机运行没有问题，但是在android 5.0、5.1上面就是一直报错：
![5075267-152814aaa6fcd89f.png](https://upload-images.jianshu.io/upload_images/5075267-152814aaa6fcd89f.png)
image.png
没错，就是INSTALL_FAILED_UPDATE_INCOMPATIBLE ,不兼容！！！
问题定位：
INSTALL_FAILED_UPDATE_INCOMPATIBLE 问题无非以下几个方面定位：
1、SDK的使用版本不正确，比如minSdkVersion=21 ,运行到android 4.4的设备上（这种问题一般不会在生成环境啦）
2、签名文件被更改。
3、内存不足了。
4、特殊情况，比如应用无法卸载干净（就有这样的恶心的厂商）。解决方案也请搜索，这方面也有资料。
我的问题却不是上面的任何一个，觉得很诡异啊。。搞了一下午。
问题根源：
![5075267-29ab55e05165428d.png](https://upload-images.jianshu.io/upload_images/5075267-29ab55e05165428d.png)
image.png
就是我之前在应用发包时，用android studio直接生成jks文件，我选择了V2(full APK signature)，可能没有选中V（Jar signature）,忘记了。
问题解决：重新生成签名文件，把V1、V2都选中。
