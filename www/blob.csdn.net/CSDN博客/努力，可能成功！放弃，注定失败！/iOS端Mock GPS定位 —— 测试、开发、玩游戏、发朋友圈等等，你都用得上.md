# iOS端Mock GPS定位 —— 测试、开发、玩游戏、发朋友圈等等，你都用得上 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月14日 15:19:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1839
## 前言
2017年，淘宝、天猫线下活动会场日益增多，个性化的地理围栏圈人投放成了运营司空见惯的方式，那么每次测试就变得非常蛋疼。安卓能找到一大波能虚拟定位的工具，那ios如何测试。我们Awareness的开发测试也遇到了这个问题。一开始我设置了园区1号楼和6号楼2个点，但是iphone的后台LBSPush位置偏移比较严重，好难进入围栏，每天都能有1万多步捐献。呵呵，好傻的测试方式。那就想办法吧。越狱无疑是一个手段，可惜越狱手机比较少，ios10也没出完美越狱的方法。所以今天我要介绍的是一个不用越狱就能mock测试的方式，利用GPX文件来虚拟定位。这个是Xcode自带的一个功能，知道如何使用的直接忽略此Blog。
## 准备
- Mac电脑一台
- 装上Xcode
## 开始
这个方法原本苹果的用意是mock所开发APP内的位置，但是其实，使用这个方法，能将本机所有的app的定位都mock掉，有木有很激动？什么？你不知道激动啥？Pokemon Go 侬晓得伐，哪里出了快龙看着安卓小伙伴一个虚拟定位就飞过去的时候，你还在原地傻站着？朋友圈发状态 侬又晓得伐，看着别人满世界跑，咋们也要装个B~ 点到为止！看完本期教程，你想怎么用就怎么用！
### Step 1
首先我们打开Xcode，新建一个工程：
![](https://upload-images.jianshu.io/upload_images/1603768-907ad686fe3c3475.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/479)
就选第一个Single View App：
![](https://upload-images.jianshu.io/upload_images/1603768-5266177ea896ec19.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/690)
然后随便输入个工程名，这么快怎么来，不要在意这些细节。
### Step 2
添加一个New File，找到“GPX File”，选中，next，添加到指定Targets。至此，你的任务已经完成了一半了。这时候我们就要去找想要Mock的目的地的经纬度了。这里提供一个查询的网站：
[http://www.gpsspg.com/maps.htm](https://link.jianshu.com?t=http://www.gpsspg.com/maps.htm)
![](https://upload-images.jianshu.io/upload_images/1603768-cdafd80e4065eb09.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/369)
由于每一家公司的定位标准不同，经纬度就会存在偏差，这时候就要看你所使用的APP使用哪家的地图了。系统后台地理围栏推送你需要用谷歌地图会更精准一些。而百度地图APP多次测试后发现用的却是谷歌地球的经纬度标准好滑稽~微信竟然也是用的谷歌地球的经纬度标准。这个反正比较一下就能发现。
决定你需要用哪个经纬度之后，我们打开刚刚生成的GPX文件，填写好经纬度和对应的Name，如图：
![](https://upload-images.jianshu.io/upload_images/1603768-963c2744b53cc205.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/377)
由于我手机上装了百度地图，所以我这里的经纬度就用谷歌地球吧。好了这个GPX文件就编辑好了
### Step 3
接下来我们就要让程序在手机里运行起来了，在此之前我们要让程序的Target运行的时候读取该GPX文件：
点击:Target-> Edit scheme -> Options -> Default Location 选中你刚刚自己建立的GPX文件
![](https://upload-images.jianshu.io/upload_images/1603768-f30250e0a0d34983.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
### 验证
好了，一切就绪，选好你的手机设备，选好你免费创建的debug证书(网上有很多教程)，直接run起来~~
启动后你会发现程序空白一片，不用怕！其实这时候你手机的定位已经变了！不信？我们打开百度地图和微信看看~
![](https://upload-images.jianshu.io/upload_images/1603768-aa8d565575e49999.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/620)
![](https://upload-images.jianshu.io/upload_images/1603768-ec46830c0006f728.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/620)
哈哈，位置都是我修改过的位置！到此教程结束！大家快去玩吧~爱怎么玩就这么玩！

###### 作者：尛破孩波波链接：https://www.jianshu.com/p/290115c64051來源：简书简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
