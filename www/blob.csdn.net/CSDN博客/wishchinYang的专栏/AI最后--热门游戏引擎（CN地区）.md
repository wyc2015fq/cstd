# AI最后--热门游戏引擎（CN地区） - wishchinYang的专栏 - CSDN博客
2014年01月07日 12:24:07[wishchin](https://me.csdn.net/wishchin)阅读数：1126
个人分类：[AI/ES](https://blog.csdn.net/wishchin/article/category/1730787)
原文链接：[http://blog.renren.com/GetEntry.do?id=723979478&owner=253982684](http://blog.csdn.net/cubuntu/article/details/8454290)
文章链接：[http://blog.csdn.net/cubuntu/article/details/8454290](http://blog.csdn.net/cubuntu/article/details/8454290)
商业引擎大排名：[http://news.52pk.com/cyyw/20120523/5309290.shtml](http://news.52pk.com/cyyw/20120523/5309290.shtml)
主流手机游戏引擎介绍：[http://www.apkbus.com/android-1344-1.html](http://www.apkbus.com/android-1344-1.html)
**摘要**: 游戏引擎是指一些已编写好的可编辑电脑游戏系统或者一些交互式实时图像应用程序的核心组件。这些系统为游戏设计者提供各种编写游戏所需的各种工具，其目的在于让游戏设计者能容易和快速地做出游戏程式而不用由零开始 ...
游戏引擎是指一些已编写好的可编辑电脑游戏系统或者一些交互式实时图像应用程序的核心组件。这些系统为游戏设计者提供各种编写游戏所需的各种工具，其目的在于让游戏设计者能容易和快速地做出游戏程式而不用由零开始。[这里](http://www.apkbus.com/android-112997-1-1.html)有更详细的游戏引擎原理。
手机游戏引擎则是在手机和平板上运行的游戏的引擎，智能手机和平板的触摸+传感器的操作方式与传统游戏相比有很大的不同，而操作方式直接影响手机上流行的游戏模式与PC和主机的不同，因此不能简单的将游戏引擎移植到手机上，而是需要根据手机与平板的特性重新设计。
目前手机游戏市场正处于上升期，手机游戏引擎市场也方兴未艾：
一方面，趁着新市场的机会期，类似cocos2d-x、Unity3D等引擎站稳脚跟；
另一方面，传统游戏引擎对手机和平板愈加重视，虚幻引擎已移植到iOS、Android平台，预计更多的顶级引擎会移植到移动平台上来；
最后，拥有WebGL技术的HTML5也显示出了游戏应用的潜力，浏览器厂商动作频频，Opera发布Sphinx引擎，Firefox移植虚幻引擎，UC欲发布X-Canvas引擎，HTML5引擎中间件的大战即将来临。
那么，现在主流的手机游戏引擎都有哪些呢？下面为读者一一介绍：
**2D引擎**
![1749260.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105113ax88jf4jm26je6m7.jpg)
**cocos2D-x**
cocos2D-x是全球知名的开源跨平台手机游戏引擎，易学易用，目前已经支持iOS、Android、Windows8、Mac OSX、Linux、BlackBerry、Windows
 Phone等平台。它由只支持iOS平台的cocos2D演化而来，x即意味着跨平台，而它现在已经推出了HTML5版本，显示了这个引擎强大的生命力和扩展能力。目前在iOS和Android平台上有许多知名游戏都使用此引擎开发，而且其项目创世人王哲是中国人，因此对中文的支持非常好，且有开发者社区cocoachina，目前此引擎属于触控科技名下。[官网](http://cocos2d-x.org/)、[cocos2D-x游戏开发入门专题](http://mobile.51cto.com/mobile/cocos2d-x/)
![1749261.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105113go1hc1c731ufwch1.jpg)
**Corona SDK**
著名跨平台游戏开发工具，支持iOS，Android，据说有150000开发者使用这个工具。[官网](http://www.coronalabs.com/)、[详细介绍](http://mobile.51cto.com/web-342339.htm)、[开发入门专题](http://mobile.51cto.com/iphone-371484.htm)
![1749262.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105118e3sz33ss5zqh4s4v.jpg)
**Construct 2**
原本是开发Windows的游戏引擎，但现在无需编程知识就可以利用一个基于事件的界面，开发iOS、Android及Facebook游戏，有70多种可视化游戏效果供你选择。更值得一提的是，它还是个很不错的HTML5游戏引擎。[官网](https://www.scirra.com/)
![1749263.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105118qwt1bfybz6wafazr.jpg)
**木瓜游戏引擎**
2D游戏引擎，帮助轻松开发移动社交游戏，支持iOS及Android。另外还提供游戏内购买系统，帮助你实现游戏的货币化。由木瓜移动开发和提供。[官网](http://papayamobile.com/developer/engine)、[中文版](http://cn.papayamobile.com/developer/engine)
![1749264.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105124zlq0kl4xxq0kux3y.jpg)
**WiEngine**
由微云科技开发的一款2D移动游戏引擎，目前已支持2D图像渲染、粒子系统、碰撞检测系统、脚本引擎、动画特效、光影特效、人工智能以及场景管理等超过十大类的特性和特效，
 支持Android(2.0+), iOS(3.2+), Mac OS X(10.6+)等平。[官网](http://www.wiyun.com/a/wiengine/)
![1749265.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105124bggsbs3bor382ttf.jpg)
**Cyclone 2D**
Cylone2D是一套手机游戏设计和制作的综合解决方案。整体包括两大部分，即引擎和设计工具集。Cyclone2D（简称C2D）引擎目前已经在多个平台应用，如Android、IOS、Windows、J2me等，制作的游戏产品可以运行在手机或者电视机终端，因此可以用来制作跨平台的手机游戏。它由国人制作，引擎免费但设计工具收费。[官网](http://www.cyclone2d.com/index.html)
**3D引擎**
![1749266.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105124f7kf23g602f7gl8f.jpg)
**Unity3D**
在国内的移动3D引擎领域，Unity3D是当之无愧的王者，有很多3D游戏均是由它开发而成，社区和教程资源也很丰富。支持iOS与Android。[官网](http://china.unity3d.com/)
![1749267.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105124w2qsc577cc12drcr.jpg)
**Unreal Development Kit**
即大名鼎鼎的虚幻引擎，是目前世界最知名的授权最广的顶尖游戏引擎，占有全球商用游戏引擎80%的市场份额。支持iOS及Android。虚幻3引擎的免费版本，不包含源代码，但包含了开发基于虚幻3引擎独立游戏的所有工具。2013年初爆出虚幻引擎东家Epic
 Games被腾讯收购，UDK在国内未来的表现令人期待。[官网](http://www.unrealengine.com/udk/)
![1749268.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105124i5had5a15f15f1pf.jpg)
**ShiVa 3D**
号称最具跨平台意义的移动游戏引擎，是一个3D游戏引擎及开发工具包，支持iOS，Android，WP及黑莓。国内有专门的书作介绍。[官网](http://www.stonetrip.com/)
![1749269.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105125tfrc057d78hhuy34.jpg)
**Marmalade**
采用C/C++开发跨平台的原生游戏及App，能适配到桌面端及移动端，支持iOS、Android及黑莓，可循环利用代码，兼容PhoneGap API。[官网](http://www.madewithmarmalade.com/)、[详细介绍](http://mobile.51cto.com/iphone-346790.htm)
**HTML5引擎**
![17492610.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105125ly0fp0v5ew00wbjh.jpg)
**Sphinx**
浏览器厂商Opera在MWC2013上发布的HTML5加速引擎，基于python开发。[官网](http://sphinx-doc.org/)
![17492611.jpg](http://www.apkbus.com/data/attachment/forum/201304/19/105125b1vgg01e81ljgggg.jpg)
**X-Canvas**
X-Canvas是一款跨平台的HTML5游戏引擎，提供手机游戏开发的完整解决方案。包含了加速引擎，游戏框架，物理引擎。由UC浏览器开发，将在2013年中发布。[官网](http://xc.uc.cn/)
