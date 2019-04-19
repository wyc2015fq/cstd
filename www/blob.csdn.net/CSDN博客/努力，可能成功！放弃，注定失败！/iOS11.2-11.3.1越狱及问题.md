# iOS11.2-11.3.1越狱及问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年07月20日 11:47:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4880
> 
设备环境：Electra、iOS11.13.1
PS:Electra最新版本进行越狱只支持11.14以下的版本。同时，由于Electra版本推出仓促，一次直接越狱成功率较低，需要多试几次。仅建议喜欢瞎折腾的童鞋尝试。
越狱结果截图
![](https://upload-images.jianshu.io/upload_images/459563-bcfea6aaa489b018.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image.png
# 越狱过程
- 进入[Electra](https://coolstar.org/electra/)官网下载Electra ipa文件，注意官网有两种方法，笔者此次使用的是第二个版本(Non Dev Accont)的文件
	
![](https://upload-images.jianshu.io/upload_images/459563-d1321ab5c4271637.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/690)
image
2.下载完后安装到手机上，方法很多
2.1. iOS开发者可直接进行重签名安装
```
codesign -f -s ‘iPhone Developer: xxx(xxx)’ —entitlements xxx.entitlements xxx.app
```
2.2. 没有开发者账号的可以使用工具[Cydia Impactor](http://www.cydiaimpactor.com)辅助安装，下载安装后打开，设备连接电脑，直接将ipa文件拉入软件中即可，或者菜单栏选择install package，之后需要输入apple账号密码。
![](https://upload-images.jianshu.io/upload_images/459563-a6dfd19f9a653401.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/158)
image.png
- 安装后打开Electra，建议手机先重启等待一分钟，开启飞行模式，关闭Siri，然后开始越狱，理想情况下，第一次开始后手机自动重启完会多了一个Cydia，此时手机还未越狱成功，Cydia打开会闪退。当然这一步成功装上cydia的概率不高，要么直接报错，要么在1/3进度处手机立即被重启，这些都属于失败的情况，此时需要重启（也许可以不用），等待一分钟以上，再次重新点击越狱，直接看到cydia。。。
![](https://upload-images.jianshu.io/upload_images/459563-99892d8619f548c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/670)
如果在此处重启，大概率本次操作失败，需要重来
- 
到这里，你需要能看到cydia，否则请回到步骤3继续。重新打开Electra，再次按下按钮，情况基本和步骤上一致，也是如果在1/3处重启则视为失败，能走到2/3基本就成功了，手机自动重启后，cydia可以打开，不闪退。
- 
此时，如果cydia正常打开并使用，那么恭喜，越狱过程已经结束。可直接忽略下面的步骤。
- 
如果打开的cydia没有连接到网络，如下图。则断开WIFI，在打开的cydia页面上，上拉控制中心，开启飞行模式，关闭，打开，这时候应该会有提醒，接着到手机设置重的移动数据网络打开cydia的联网权限即可使用。
![](https://upload-images.jianshu.io/upload_images/459563-f9bd9c74764ff6e4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
image.png
> 
再一次温馨提示：当前Electra越狱成功率较低，基本需要很多次尝试后才能越狱成功，要有耐心，如果能一次成功，那你肯定是被选中的人～
作者：WillkYang
链接：https://www.jianshu.com/p/5458ac6323e0
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
