# Appium的新版本：Appium-desktop（四） - weixin_33985507的博客 - CSDN博客
2019年01月19日 21:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
> 
1、什么是 Appium-desktop？
            Appium的不同版本有不同的叫法，以2015 年底为界限,之前叫Appium-Server(即1/2/3篇中使用的appium) ，之后叫 Appium-desktop ，当然， Appium-Server当前仍然是可用的。
Appium-Server  <-----------------  2015 年底  ----------------->  Appium-desktop
#### 我用的还是Windows版在 2015 年底止步于的 **AppiumForWindows_1_4_16_1.zip 的Appium-Server  ！！**
> 
2、下载安装
https://github.com/appium/appium-desktop/releases  [点击下载](https://github.com/appium/appium-desktop/releases)
或者
https://github.com/appium/appium-desktop/releases/tag/v1.2.1   （appium-desktop-Setup-1.2.1.exe）
安装过程太简单了，双击 exe 文件，然后，等待安装完就好了，中间都不需要你设置任何选项。所以，这里就不贴图了。
> 
3、启动运行（使用上同appium-server）
安装完成桌面会生成一个紫色的 appium 图标，双击打开。
默认显示监控的 host 和 port ，这和 Appium-Server中是一致的。点击 **“Start Server V 1.7.1”** 按钮启动服务。
然后就可以启动移动设备（真机或模拟器），编写 Appium 自动化测试脚本，可以通过 Appium-desktop 来运行测试了。
至于 Appium-Server , 你可以把它卸载了，也可以留着 \(^o^)/~
![12359666-ee2fed2dbfd94211.png](https://upload-images.jianshu.io/upload_images/12359666-ee2fed2dbfd94211.png)

![12359666-6e1d05f5e8556082.png](https://upload-images.jianshu.io/upload_images/12359666-6e1d05f5e8556082.png)
