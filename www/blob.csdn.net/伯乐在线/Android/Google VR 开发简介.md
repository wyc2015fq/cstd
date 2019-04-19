# Google VR 开发简介 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [姜家志](http://www.jobbole.com/members/jiangjiazhi) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
Google 有两个 Virtual Reality(VR) 平台，一个是Cardboard，一个廉价的VR解决方案，2015年Goolgle的I/O推出。另一个就是今年的I/O大会刚刚推出的Daydream，期待着Daydream能够带来更加优质的，性价比高的VR解决方案。
Cardboard:
![](http://ww1.sinaimg.cn/mw690/7cc829d3gw1f4uuuw6fiyj20j40arq3g.jpg)
Daydream:
![](http://ww4.sinaimg.cn/mw690/7cc829d3gw1f4uuv3gq6wj20j40armxn.jpg)
Google有专门的VR的开发者页面：[https://developers.google.com/vr/](https://developers.google.com/vr/%E3%80%82%E5%8C%85%E6%8B%AC%E6%A0%B7%E4%BE%8B%E4%BB%A3%E7%A0%81%EF%BC%8CAPI%E6%96%87%E6%A1%A3%EF%BC%8C%E8%AE%BE%E8%AE%A1%E8%A7%84%E8%8C%83%E7%AD%89%E3%80%82) 。包括样例代码，API文档，设计规范等。
Google VR有三个SDK：Unity 3D、Android、iOS，它们同样都支持 Cardboard 和 Daydream。
## Android VR
Android平台有Java版本对应的SDK，和 C/C++ 对应的NDK。
熟悉OpenGL的开发者能够快速的使用Google VR SDK创建VR程序，还可以Android Studio直接开发。
Android VR的SDK可以做到：
- 校正镜头
- 立体声音
- 头部跟踪
- 3D校准
- 并排呈现
- 立体几何
- 输入事件处理
Android SDK使用教程:
[https://developers.google.com/vr/android/get-started#treasure_hunt_sample_app](https://developers.google.com/vr/android/get-started#treasure_hunt_sample_app)
Android NDK使用教程:
[https://developers.google.com/vr/android/ndk/get-started](https://developers.google.com/vr/android/ndk/get-started)
## iOS VR
Google VR 的iOS SDK是采用 Objective-C 编写的。当然是需要在XCode中运行的，iOS平台上面也提供了对应的CocoaPod类库，使用：
> 
pod CardboardSDK
可以直接使用CardboardSDK。
同样的，在Android的SDK上面能够处理的事情，使用iOS的SDK也可以做到。
iOS SDK使用教程：[https://developers.google.com/vr/iOS/get-started](https://developers.google.com/vr/iOS/get-started)
## Unity 3D VR
Unity 3D是三维动画的开发工具，Google VR 有对应的Unity 3D下的SDK。
Unity SDK支持构建Android下的Daydream和Cardboard，而在iOS设备上面目前还不支持使用Unity 3D开发Daydream，只支 Cardboard。
Unity 3D VR的特点:
- 提供的插件和组件让开发变得更容易
- 可以从头开始一个VR Unity项目
- 当前的Unity 3D项目转向VR
- 让应用容易的切换VR模式
Ggoole VR 在 Stackoverflow 的专门问答区：[http://stackoverflow.com/questions/tagged/google-vr](http://stackoverflow.com/questions/tagged/google-vr)
